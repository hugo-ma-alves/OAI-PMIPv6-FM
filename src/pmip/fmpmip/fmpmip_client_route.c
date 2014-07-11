
#define PMIP
#define __FMPMIP_CLIENT_ROUTE_C
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdlib.h>
#include "debug.h"
#include <pthread.h>

#ifdef ENABLE_VT
#include "vt.h"
#endif

#include <fmpmip/fmpmip_client_route.h>

static struct route * list_HEAD=NULL;
static int    number_of_clients =0;
static int    number_of_routes  =0;

int client_route_init(){
    int ret=0;

    ret = pthread_rwlock_init(&client_route_list_lock, NULL);
    if(ret) {
        dbg("Error initializing lock\n");
        return -1;
    }

 #ifdef ENABLE_VT
    if (ret < 0){
        return ret;
    }
    ret = vt_client_route_init();
 #endif
    
    return ret;
}

int add_client_route(ip6mn_nai_t client_nai, struct in6_addr mag_address){

    int mutex_return_code;
    int ret=0;
    struct route *client = list_HEAD;
    struct route *tmp = NULL;
    struct mag *mag_temp = NULL;

    mutex_return_code = pthread_rwlock_wrlock(&client_route_list_lock);
    if (mutex_return_code != 0) {
        dbg("pthread_rwlock_wrlock(&client_route_list_lock) %s\n", strerror(mutex_return_code));
    }

    client = add_client_to_list(client_nai);
    
    if(client==NULL){
        dbg("Error adding client to routes list\n");
    }else{
        add_mag_to_client(&(client->mag),mag_address);
        number_of_routes++;
    }   

    dbg("added route to client\n");

    mutex_return_code = pthread_rwlock_unlock(&client_route_list_lock);
    if (mutex_return_code != 0) {
        dbg("pthread_rwlock_unlock(&client_route_list_lock) %s\n", strerror(mutex_return_code));
    }

    return 1;
}


struct route * get_route_for_client(ip6mn_nai_t client_nai){

    int mutex_return_code;
    struct route *client = list_HEAD;
    struct route *tmp = NULL;

    mutex_return_code = pthread_rwlock_wrlock(&client_route_list_lock);
    if (mutex_return_code != 0) {
        dbg("pthread_rwlock_wrlock(&client_route_list_lock) %s\n", strerror(mutex_return_code));
    }

    //Check if the client exists
    while(client != NULL){
        if(memcmp(&client->client_nai, &client_nai, sizeof(ip6mn_nai_t)) ==0 ){
            break;
        }
        client= client->next;
    }

    if (client) {
        mutex_return_code = pthread_rwlock_wrlock(&client->lock);
        if (mutex_return_code != 0) {
          dbg("pthread_rwlock_wrlock(&client->lock) %s\n", strerror(mutex_return_code));
      }
  } else {
    mutex_return_code = pthread_rwlock_unlock(&client_route_list_lock);
    if (mutex_return_code != 0) {
      dbg("pthread_rwlock_unlock(&client_route_list_lock) %s\n", strerror(mutex_return_code));
  }
}

return client;
}

struct client *add_client_to_list(ip6mn_nai_t client_nai){

    struct route *client = list_HEAD;
    struct route *tmp = NULL;

    //Check if the client exists
    while(client != NULL){
        if(memcmp(&client->client_nai, &client_nai, sizeof(ip6mn_nai_t)) ==0 ){
            return client;
        }
        client= client->next;
    }

    if(client == NULL){
        tmp=(struct route *)malloc(sizeof(struct route));
        if(tmp==NULL){
            dbg("Error malloc client route");
            return NULL;
        }
        memcpy(&tmp->client_nai,&client_nai, sizeof(ip6mn_nai_t));
        tmp->next=NULL;
        tmp->mag=NULL;
        if (pthread_rwlock_init(&tmp->lock, NULL)) {
          free(tmp);
          dbg("Error malloc client route");
          return NULL;
      }
      if(list_HEAD==NULL){
        list_HEAD = tmp;
        list_HEAD->next=NULL;
    }else{
        tmp->next=list_HEAD;
        list_HEAD=tmp;
    }
}

return tmp;
}

int add_mag_to_client(struct mag **mag_list ,struct in6_addr mag_address){

    struct mag *tmp =*mag_list;
    struct mag *new_mag =NULL;

    while(tmp!=NULL){
        if(IN6_ARE_ADDR_EQUAL(&tmp->ip6_addr,&mag_address)){
            return 1;
        }
        tmp= tmp->next;
    }

    new_mag=(struct mag *)malloc(sizeof(struct mag));
    new_mag->next=NULL;
    new_mag->mark= get_mag_mark(mag_address);
    memcpy(&new_mag->ip6_addr, &mag_address, sizeof(struct in6_addr));

    if(*mag_list==NULL){
        *mag_list = new_mag;
    }else{
        new_mag->next= *mag_list;
        *mag_list = new_mag;
    }
    return 1;
}


void client_route_release_entry(struct route * route){

   int mutex_return_code;
   assert(route);
   mutex_return_code = pthread_rwlock_unlock(&route->lock);
   if (mutex_return_code != 0) {
      dbg("pthread_rwlock_unlock(&route->lock) %s\n", strerror(mutex_return_code));
  }
  mutex_return_code = pthread_rwlock_unlock(&client_route_list_lock);
  if (mutex_return_code != 0) {
      dbg("pthread_rwlock_unlock(&client_route_list_lock) %s\n", strerror(mutex_return_code));
  }
}


int remove_client_route(ip6mn_nai_t client_nai, struct in6_addr mag_address){
    int mutex_return_code;
    int ret=0;
    struct route *client = list_HEAD;
    struct mag *tmp = NULL;
    struct mag *mag_prev = NULL;

    mutex_return_code = pthread_rwlock_wrlock(&client_route_list_lock);
    if (mutex_return_code != 0) {
        dbg("pthread_rwlock_wrlock(&client_route_list_lock) %s\n", strerror(mutex_return_code));
    }

    while(client != NULL){
        if(memcmp(&client->client_nai, &client_nai, sizeof(ip6mn_nai_t))==0 ){
            break;
        }
        client= client->next;
    }

    tmp=client->mag;
    while(tmp!=NULL){
        if(IN6_ARE_ADDR_EQUAL(&tmp->ip6_addr,&mag_address)){
            break;
        }
        mag_prev= tmp;
        tmp=tmp->next;
    }

    if(mag_prev==NULL){
        client->mag= tmp->next;
    }else{
        mag_prev->next= tmp->next;
    }

    if(client->mag==NULL){
        remove_client(client_nai);
    }

    dbg("Removed route to client\n");
    mutex_return_code = pthread_rwlock_unlock(&client_route_list_lock);
    if (mutex_return_code != 0) {
        dbg("pthread_rwlock_unlock(&client_route_list_lock) %s\n", strerror(mutex_return_code));
    }

    return 1;
}


void remove_client(ip6mn_nai_t client_nai){
    int mutex_return_code;
    struct route *client = list_HEAD;
    struct route *client_prev = NULL;

    //asumes someone has called lock on the list!!

    while(client != NULL){
        if(memcmp(&client->client_nai, &client_nai, sizeof(ip6mn_nai_t))==0 ){
            break;
        }
        client_prev= client;
        client= client->next;
    }

    //Removes(free) only the client, not his mag's

    //rearrange client list
    if(client_prev==NULL){
        list_HEAD = client->next;
    }else{
        client_prev->next=client->next;
    }
    free(client);
    number_of_clients--;
    dbg("Removed Client from client routes list\n");
  

}


int get_mag_mark(struct in6_addr mag_address){

    unsigned int i=0;
    uint32_t mark=0;
    for ( i = 0; i < conf.NumMags; ++i)
    {
        if( IN6_ARE_ADDR_EQUAL(&conf.MagAddressEgress[i],&mag_address)){
            return conf.MagRoutingMark[i];
        }
    }
    return mark;
}

int client_route_iterate( int (*func)(void *, void *), void *arg){

    int err=0;
    struct route *r =list_HEAD;
    while(r!=NULL){
        if ((err = func(r, arg))){
            return err;
        }
        r= r->next;
    }
    return err;
}

