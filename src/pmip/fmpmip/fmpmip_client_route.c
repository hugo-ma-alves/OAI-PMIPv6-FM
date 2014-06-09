
#define PMIP
#define __FMPMIP_CLIENT_ROUTING_C
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
    struct route *client = NULL;

    mutex_return_code = pthread_rwlock_wrlock(&client_route_list_lock);
    if (mutex_return_code != 0) {
        dbg("pthread_rwlock_wrlock(&client_route_list_lock) %s\n", strerror(mutex_return_code));
    }

    client = client_is_in_list(client_nai);

    if(client== NULL){
        add_new_client(client_nai, &list_HEAD);
    }
    if(ret<0){
        dbg("+++Error adding route cache for client \n");
        return 0;
    }
    client = client_is_in_list(client_nai);
    add_route_to_client( client, mag_address);

    mutex_return_code = pthread_rwlock_unlock(&client_route_list_lock);
    if (mutex_return_code != 0) {
        dbg("pthread_rwlock_unlock(&client_route_list_lock) %s\n", strerror(mutex_return_code));
    }

    return 1;
}


int add_new_client(ip6mn_nai_t client_nai, struct route **route){

    struct route *temp, *r;

    //empty list
    if(*route==NULL){
        temp = (struct route*) malloc ( sizeof ( struct route ) ) ;
        if(temp==NULL){
            return -1;
        }
        memcpy(&temp->client_nai, &client_nai, sizeof(ip6mn_nai_t));
        temp->next = NULL ;
        *route = temp ;        
    }else{
        //look for last node
        temp =*route;
        while(temp->next != NULL){
            temp = temp->next;
        }
        r= (struct route*) malloc ( sizeof ( struct route ) ) ;
        if(r==NULL){
            return -1;
        }
        memcpy(&r->client_nai, &client_nai, sizeof(ip6mn_nai_t));
        r->next = NULL ;
        temp->next = r;
    }
    
    return 1;
}

struct route * getClient(ip6mn_nai_t client_nai){

    struct route * temp = list_HEAD;
    if(temp==NULL){
        return NULL;
    }
    while(temp->next != NULL ){
        if(IN6_ARE_ADDR_EQUAL(&client_nai, &temp->client_nai)){
            return temp;
        }
        temp = temp->next;
    }
    return temp;
}


struct route * client_is_in_list(ip6mn_nai_t client_nai){

    struct route * temp = list_HEAD;
    if(temp==NULL){
        return NULL;
    }
    while(temp->next != NULL ){
        if(IN6_ARE_ADDR_EQUAL(&client_nai, &temp->client_nai)){
            return temp;
        }
        temp = temp->next;
    }
    return temp;
}


int add_route_to_client(struct route *client, struct in6_addr mag_address){


    struct mag *mag, *m;
    //empty list
    if(client->mag==NULL){
        mag = (struct mag*) malloc ( sizeof ( struct mag ) ) ;
        memcpy(&mag->ip6_addr, &mag_address, sizeof(struct in6_addr));
        mag->mark= get_mag_mark(mag_address);
        mag->next = NULL ;
        client->mag = mag ;        
    }else{
        //look for last node
        mag = client->mag;
        if(IN6_ARE_ADDR_EQUAL(&mag->ip6_addr,&mag_address)){
            return 1;
        }
        while(mag->next != NULL){
            mag = mag->next;
            //check  if its already there
            if(IN6_ARE_ADDR_EQUAL(&mag->ip6_addr,&mag_address)){
                return 1;
            }
        }
        m = (struct mag*) malloc ( sizeof ( struct mag ) ) ;
        memcpy(&m->ip6_addr, &mag_address, sizeof(struct in6_addr));
        m->mark= get_mag_mark(mag_address);
        m->next = NULL ;
        mag->next = m;
    }
    return 1;

}

void remove_client_route(ip6mn_nai_t client_nai, struct in6_addr mag_address){


    int mutex_return_code;
    struct route *client =client_is_in_list(client_nai);
    
    if(client==NULL){
        return;
    }

    mutex_return_code = pthread_rwlock_wrlock(&client_route_list_lock);
    if (mutex_return_code != 0) {
        dbg("pthread_rwlock_wrlock(&client_route_list_lock) %s\n", strerror(mutex_return_code));
    }

    struct mag *temp, *previous;
    temp = client->mag;
    previous=NULL;

    while(temp != NULL){

        if(IN6_ARE_ADDR_EQUAL(&temp->ip6_addr,&mag_address)){
            if(previous==NULL){
                client->mag=temp->next;
                free(temp);
                break;
            }else{
                previous->next=temp->next;
                free(temp);
                break;
            }
        }else{
            previous=temp;
            temp = temp->next;
        }
        
    }
    mutex_return_code = pthread_rwlock_unlock(&client_route_list_lock);
    if (mutex_return_code != 0) {
        dbg("pthread_rwlock_unlock(&client_route_list_lock) %s\n", strerror(mutex_return_code));
    }

    if(client->mag==NULL){
        remove_client(client_nai);
    }

}


void remove_client(ip6mn_nai_t client_nai){

    int mutex_return_code;
    //return;
    struct route * temp = list_HEAD;
    struct route * previous;

    previous=NULL;
    if(temp==NULL){
        return ;
    }

    mutex_return_code = pthread_rwlock_wrlock(&client_route_list_lock);
    if (mutex_return_code != 0) {
        dbg("pthread_rwlock_wrlock(&client_route_list_lock) %s\n", strerror(mutex_return_code));
    }
    
    while(temp != NULL ){
        if(IN6_ARE_ADDR_EQUAL(&client_nai, &temp->client_nai)){
         if(previous==NULL){
            list_HEAD = temp->next;
        }else{
            previous->next=temp->next;
        }
        break;
    }
    previous=temp;
    temp = temp->next;
}

free(temp);

mutex_return_code = pthread_rwlock_unlock(&client_route_list_lock);
if (mutex_return_code != 0) {
    dbg("pthread_rwlock_unlock(&client_route_list_lock) %s\n", strerror(mutex_return_code));
}
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

