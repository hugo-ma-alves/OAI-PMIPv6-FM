/*
 * $Id: conf_changes.c $
 *
 * This file is part of the MIPL Mobile IPv6 for Linux.
 * 
 * Authors: Sebastien Decugis <sdecugis@hongo.wide.ad.jp>
 *
 * Copyright 2003-2005 Go-Core Project
 * Copyright 2003-2006 Helsinki University of Technology
 *
 * MIPL Mobile IPv6 for Linux is free software; you can redistribute
 * it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; version 2 of
 * the License.
 *
 * MIPL Mobile IPv6 for Linux is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MIPL Mobile IPv6 for Linux; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>

#include "conf.h"
#include "debug.h"
#include "list.h"
#include "policy.h"
#include "prefix.h"
#include "ipsec.h"

/* Helper function to parse changes in a list.
 * Function parse_change_in_<struct_name> must be defined with
 * FCT_LIST_CHANGES(<struct_name>, <member>) before being used. 
 */
#define FCT_LIST_CHANGES(struct_name, member) \
static void parse_change_in_##struct_name ( \
			struct list_head *cur_list, \
  			struct list_head *new_list, \
			int (*cb_cmp)(struct struct_name * a, \
				      struct struct_name * b), \
			void (*cb_free)(struct struct_name * elem), \
			void (*cb_add)(struct struct_name * new_item), \
			void (*cb_rem)(struct struct_name * old_item) )\
{ \
  	/* We update the cur_list to add new elements from new_list and remove 
	 * elements that in are not there.
	 * The comparison function between elements is cb_cmp (cannot be NULL)
	 * if cb_free is NULL, free() is called to delete an unused element of
	 * the list, otherwise cb_free is called.
	 * if cb_add is not NULL, it is called for each new element that is
	 * being added in the list.
	 * if cb_rem is not NULL, it is called for each element that is removed
	 * from cur_list */ \
	struct list_head *cur_i, *cur_n, *new_i, *new_n; \
	struct struct_name *cur_el, *new_el; \
	int found; \
	\
	list_for_each_safe(cur_i, cur_n, cur_list) \
	{ \
		found=0; \
		cur_el = list_entry(cur_i, struct struct_name, member); \
		list_for_each_safe(new_i, new_n, new_list) \
		{ \
			new_el = list_entry(new_i, struct struct_name, member); \
			\
			if (! cb_cmp(cur_el, new_el) )\
			{ \
				/* This element is in both lists, so it is not
				 * interesting, we remove from new_list */ \
				found = 1; \
				list_del(new_i); \
				if (cb_free) \
					cb_free(new_el); \
				else \
					free(new_el); \
			} \
		} \
		\
		if ( ! found ) \
		{ \
			/* The element was not found in the new list:
			 * it has been removed */ \
			list_del(cur_i); \
			if (cb_rem) \
				cb_rem(cur_el); \
			if (cb_free) \
				cb_free(cur_el); \
			else \
				free(cur_el); \
		} \
	} \
	/* Now new_list contains only the elements that have been added
	 * in the config */ \
	list_for_each_safe(new_i, new_n, new_list) \
	{ \
		list_del(new_i); \
		if (cb_add) \
		{ \
			new_el = list_entry(new_i, struct struct_name, member); \
			cb_add(new_el); \
		} \
		list_add_tail(new_i, cur_list); \
	} \
	/* The cur_list has been updated */ \
}


/********************************/ 
/* Updating the net_ifaces list */
/********************************/ 

/* define parse_change_in_net_iface */
FCT_LIST_CHANGES(net_iface, list)

/* Compare function */
int cmp_net_iface(struct net_iface *a, struct net_iface *b)
{
	if ((a->ifindex != b->ifindex)
	     || (a->is_rtr != b->is_rtr)
	     || (a->mip6_if_entity != b->mip6_if_entity)
	     || (a->mn_if_preference != b->mn_if_preference))
		return -1;

	return strncmp(a->name, b->name, IF_NAMESIZE);
}

/* Add function (just to be verbose, can be safely removed) */
void add_net_iface(struct net_iface *nif)
{
	char nifname[IF_NAMESIZE + 1];
	memcpy(&nifname[0], nif->name, IF_NAMESIZE);
	nifname[IF_NAMESIZE] = '\0';
	dbg("Added new interface %d:%s with preference %d\n",
	    nif->ifindex, nifname, nif->mn_if_preference);
}

/* Remove function (just to be verbose, can be safely removed) */
void rem_net_iface(struct net_iface *nif)
{
	char nifname[IF_NAMESIZE + 1];
	memcpy(&nifname[0], nif->name, IF_NAMESIZE);
	nifname[IF_NAMESIZE] = '\0';
	dbg("Removed interface %d:%s with preference %d\n",
	    nif->ifindex, nifname, nif->mn_if_preference);
}


/******************************/
/* Updating the bind_acl list */
/******************************/

/* define parse_change_in_policy_bind_acl_entry */
FCT_LIST_CHANGES(policy_bind_acl_entry, list)

/* Compare function */
int cmp_policy_bind_acl_entry(struct policy_bind_acl_entry *a,
			      struct policy_bind_acl_entry *b)
{
	struct list_head *a_item, *b_item ;
	struct prefix_list_entry *a_ple, *b_ple;

	/* Check if both entries are for the same hoa */
	if (!IN6_ARE_ADDR_EQUAL(&a->hoa, &b->hoa))
		return -1;

	/* Check other parameters */
	if ((a->plen != b->plen)
	     || (a->bind_policy	!= b->bind_policy)
	     || (a->mnp_count != b->mnp_count )) {
		dbg("Found change in bind_acl configuration for hoa: "
		    "%x:%x:%x:%x:%x:%x:%x:%x\n", NIP6ADDR(&a->hoa));
	     	return -1;
	}

	/* Now we should check that the prefixes in the mob_net_prefixes
	 * are the same. We just suppose that the parser would always create
	 * the list in the same order. */
	for (a_item = (a->mob_net_prefixes).next,
	     b_item = (b->mob_net_prefixes).next;
	     a_item != (&a->mob_net_prefixes)
	     && b_item != (&b->mob_net_prefixes);
	     a_item = a_item->next,
	     b_item = b_item->next) {
		a_ple = list_entry(a_item, struct prefix_list_entry, list);
		b_ple = list_entry(b_item, struct prefix_list_entry, list);
		if ((a_ple->ple_plen != b_ple->ple_plen)
		    || !IN6_ARE_ADDR_EQUAL(&a_ple->ple_prefix,
					   &b_ple->ple_prefix)) {
			dbg("Found difference in mobiles prefixes lists "
			    "for hoa %x:%x:%x:%x:%x:%x:%x:%x\n",
			    NIP6ADDR(&a->hoa));
			dbg("%x:%x:%x:%x:%x:%x:%x:%x/%d and "
			    "%x:%x:%x:%x:%x:%x:%x:%x/d\n",
			    NIP6ADDR(&a_ple->ple_prefix), a_ple->ple_plen,
			    NIP6ADDR(&b_ple->ple_prefix), b_ple->ple_plen);
		    	return -1;
		}
	}
	/* We know already the lists have the same # of elements */

	return 0;
}

/* these functions are in policy.c to manage the
 * hash table entries related to the config */
extern int policy_bind_acl_add(struct policy_bind_acl_entry *acl);
extern int policy_bind_acl_rem(struct policy_bind_acl_entry *acl);

/* Add function */
void add_policy_bind_acl_entry(struct policy_bind_acl_entry *nif)
{
	dbg("Adding bind_acl entry for hoa: %x:%x:%x:%x:%x:%x:%x:%x\n",
	    NIP6ADDR(&nif->hoa));

	if (policy_bind_acl_add(nif) < 0)
		dbg("Adding the bind_acl failed.\n");
}

/* Remove function */
void rem_policy_bind_acl_entry(struct policy_bind_acl_entry *nif)
{
	dbg("Removing bind_acl entry for hoa: %x:%x:%x:%x:%x:%x:%x:%x\n",
	    NIP6ADDR(&nif->hoa));

	if (policy_bind_acl_rem(nif) < 0)
		dbg("Removing the bind_acl failed.\n");
}

/* Cleanup function */
void free_policy_bind_acl_entry(struct policy_bind_acl_entry *nif)
{
	prefix_list_free(&nif->mob_net_prefixes);
	free(nif);
}

/****************************************/
/* Updating the DefaultBindingAclPolicy */
/****************************************/
extern int def_bind_policy;

/************************************/
/* Updating the ipsec_policies list */
/************************************/

/* define parse_change_in_ipsec_policy_entry */
FCT_LIST_CHANGES(ipsec_policy_entry, list)

/* Compare function */
int cmp_ipsec_policy_entry(struct ipsec_policy_entry *a,
			   struct ipsec_policy_entry *b)
{
	if ((! IN6_ARE_ADDR_EQUAL(&a->mn_addr, &b->mn_addr))
	    || (!IN6_ARE_ADDR_EQUAL(&a->ha_addr, &b->ha_addr))
	    || (a->type != b->type)
	    || (a->ipsec_protos != b->ipsec_protos)
	    || (a->action  !=  b->action)
	    || (a->reqid_toha != b->reqid_toha)
	    || (a->reqid_tomn != b->reqid_tomn))
	   	return -1;

	return 0;
}

extern int _ha_mn_ipsec_init(const struct in6_addr *haaddr,
			     const struct in6_addr *hoa,
			     struct ipsec_policy_entry *e,
			     void *arg);
extern int _ha_mn_ipsec_cleanup(const struct in6_addr *haaddr,
				const struct in6_addr *hoa,
				struct ipsec_policy_entry *e,
				void *arg);
extern int _mn_ha_ipsec_init(const struct in6_addr *haaddr,
			     const struct in6_addr *hoa,
			     struct ipsec_policy_entry *e,
			     void *arg);
extern int _mn_ha_ipsec_cleanup(const struct in6_addr *haaddr,
				const struct in6_addr *hoa,
				struct ipsec_policy_entry *e,
				void *arg);
extern int ipsec_policy_dump_config(const struct in6_addr *haaddr,
				    const struct in6_addr *hoa,
				    struct ipsec_policy_entry *e,
				    void *arg);

/* Add function */
void add_ipsec_policy_entry(struct ipsec_policy_entry *e)
{
	dbg("New IPsec policy: (HA %x:%x:%x:%x:%x:%x:%x:%x) "
	    "(MN %x:%x:%x:%x:%x:%x:%x:%x) (t:%x)\n",
	    NIP6ADDR(&e->ha_addr), NIP6ADDR(&e->mn_addr), e->type);

	if (is_ha()
	    && _ha_mn_ipsec_init(&e->ha_addr, &e->mn_addr, e, NULL)) {
		fprintf(stderr, "Failed to insert new IPsec policy\n");
		ipsec_policy_dump_config(&e->ha_addr, &e->mn_addr,
					 e, NULL);
	}

	if (is_mn()
	    && _mn_ha_ipsec_init(&e->ha_addr, &e->mn_addr, e, NULL)) {
		fprintf(stderr, "Failed to insert new IPsec policy\n");
		ipsec_policy_dump_config(&e->ha_addr, &e->mn_addr,
					 e, NULL);
	}
}

/* Remove function */
void rem_ipsec_policy_entry(struct ipsec_policy_entry *e)
{
	dbg("Removing IPsec policy: (HA %x:%x:%x:%x:%x:%x:%x:%x) "
	    "(MN %x:%x:%x:%x:%x:%x:%x:%x) (t:%x)\n",
	    NIP6ADDR(&e->ha_addr), NIP6ADDR(&e->mn_addr), e->type);

	if (is_ha() 
	    && _ha_mn_ipsec_cleanup(&e->ha_addr, &e->mn_addr, e, NULL)) {
		fprintf(stderr, "Failed to remove IPsec policy\n");
		ipsec_policy_dump_config(&e->ha_addr, &e->mn_addr,
					 e, NULL);
	}

	if (is_mn()
	    && _mn_ha_ipsec_cleanup(&e->ha_addr, &e->mn_addr, e, NULL)) {
		fprintf(stderr, "Failed to remove IPsec policy\n");
		ipsec_policy_dump_config(&e->ha_addr, &e->mn_addr,
					 e, NULL);
	}
}


/************************************/
/* Updating the ipsec_policies list */
/************************************/

/* define parse_change_in_ipsec_policy_entry */
FCT_LIST_CHANGES(prefix_list_entry, list)

/* Compare function */
int cmp_prefix_list_entry(struct prefix_list_entry *a,
			  struct prefix_list_entry *b)
{
	if ((a->ple_plen != b->ple_plen)
	    || !IN6_ARE_ADDR_EQUAL(&a->ple_prefix, &b->ple_prefix)) {
		return -1;
	}

	return 0;
}

/* Add function */
void add_prefix_list_entry(struct prefix_list_entry *ple)
{
	dbg("HaServedPrefix: added prefix: %x:%x:%x:%x:%x:%x:%x:%x/%d\n",
	    NIP6ADDR(&ple->ple_prefix), ple->ple_plen);
	return;
}

/* Rem function */
void rem_prefix_list_entry(struct prefix_list_entry *ple)
{
	dbg("HaServedPrefix: removed prefix: %x:%x:%x:%x:%x:%x:%x:%x/%d\n",
	    NIP6ADDR(&ple->ple_prefix), ple->ple_plen);
	return;
}


/************************************/
/* Function that updates the config */
/************************************/
void conf_apply_changes(struct mip6_config *cur, struct mip6_config *new)
{
	/* config_file: this field is not changed */

	/* (NodeConfig) mip6_entity: cannot change on-the-fly */
	if (cur->mip6_entity != new->mip6_entity) {
		fprintf(stderr, "Cannot change the role on-the-fly\n");
		return;
	}


#if defined(ENABLE_VT) && 0
	/* Actually the VT definition is not in the configuration file so 
	 * we should not change it */

	/* If the vt options are changed, we close the old vt and open a new one */
	if (strcmp(cur->vt_hostname, new->vt_hostname)
	    || strcmp(cur->vt_service, new->vt_service)) {
		dbg("Restarting virtual terminal (%s(%s) -> %s(%s))\n",
		    cur->vt_hostname, cur->vt_service,
		    new->vt_hostname, new->vt_service);

		vt_fini();

		free(cur->vt_hostname);
		cur->vt_hostname = new->vt_hosname;

		free(cur->vt_service);
		cur->vt_service  = new->vt_service;

		if (vt_start(cur->vt_hostname, cur->vt_service) < 0)
			fprintf(stderr, "Failed to restart the virtual "
				"terminal on new host/port\n");
	}
#endif /* ENABLE_VT && 0 */


	/* (DebugLevel) debug_level: update the value */
	if (new->debug_level != cur->debug_level) {
		dbg("Changing DebugLogFile (old=%d)\n", cur->debug_level);
		cur->debug_level = new->debug_level;
		dbg("Changing DebugLogFile (new=%d)\n", cur->debug_level);
	}


	/* (DebugLogFile) debug_log_file: not supported */
	if (strcmp(cur->debug_log_file?cur->debug_log_file:"",
		   new->debug_log_file?new->debug_log_file:"")) {
		fprintf(stderr, "Changing the DebugLogFile is not "
			"supported.\n");
	}

	/* (UsePolicyModule) pmgr: not supported */
	if (strncmp(cur->pmgr.so_path, new->pmgr.so_path,
		    _POSIX_PATH_MAX)) {
		fprintf(stderr, "Changing the UsePolicyModule is not "
			"supported (%s -> %s).\n",
			cur->pmgr.so_path, new->pmgr.so_path);
	}

	/* (Interface) net_ifaces: update the list */
	parse_change_in_net_iface(&cur->net_ifaces,
				  &new->net_ifaces,
				  cmp_net_iface,
				  NULL,
				  add_net_iface,
				  rem_net_iface);

	/* (BindingAclPolicy) bind_acl: update the conf and the
	 * hash (policy_bind_acl_hash) in policy.c */
	parse_change_in_policy_bind_acl_entry(
					&cur->bind_acl,
					&new->bind_acl,
					cmp_policy_bind_acl_entry,
					free_policy_bind_acl_entry,
					add_policy_bind_acl_entry,
					rem_policy_bind_acl_entry);

	/* (DefaultBindingAclPolicy) DefaultBindingAclPolicy:
	 * update the value in config and copy in policy.c */
	if (new->DefaultBindingAclPolicy != cur->DefaultBindingAclPolicy) {
		dbg("New DefaultBindingAclPolicy: %u\n",
		    new->DefaultBindingAclPolicy);
		cur->DefaultBindingAclPolicy = new->DefaultBindingAclPolicy;
		def_bind_policy = new->DefaultBindingAclPolicy;
	}

	/* (NonVolatileBindingCache) NonVolatileBindingCache:
	 * update the value in config */
	if (new->NonVolatileBindingCache != cur->NonVolatileBindingCache) {
		dbg("New NonVolatileBindingCache: %s\n",
		    new->NonVolatileBindingCache ? "enabled" : "disabled");
		cur->NonVolatileBindingCache = new->NonVolatileBindingCache;
	}

	/* (KeyMngMobCapability) KeyMngMobCapability: update the
	 * value in config */
	if (new->KeyMngMobCapability != cur->KeyMngMobCapability) {
		dbg("New KeyMngMobCapability: %s\n",
		    new->KeyMngMobCapability ? "enabled" : "disabled");
		cur->KeyMngMobCapability = new->KeyMngMobCapability;
	}

	/* (UseMnHaIPsec) UseMnHaIPsec: No change allowed. */
	if (new->UseMnHaIPsec != cur->UseMnHaIPsec) {
		fprintf(stderr,
			"Changing UseMnHaIPsec is not currently supported\n");
	}

	/* (IPsecPolicySet) ipsec_policies: update the conf and
	 * insert / remove SPD entries if required */
	if (cur->UseMnHaIPsec)
		parse_change_in_ipsec_policy_entry(
					&cur->ipsec_policies,
					&new->ipsec_policies,
					cmp_ipsec_policy_entry,
					NULL,
					add_ipsec_policy_entry,
					rem_ipsec_policy_entry);

	/* (MnMaxCnBindingLife) MnMaxCnBindingLife: update the value */
	if (new->MnMaxCnBindingLife != cur->MnMaxCnBindingLife) {
		dbg("New MnMaxCnBindingLife: %u (only affects new bindings)\n",
		    new->MnMaxCnBindingLife);
		cur->MnMaxCnBindingLife = new->MnMaxCnBindingLife;
	}

	/* (MnRouterProbes) MnRouterProbes: update the value */
	if (new->MnRouterProbes != cur->MnRouterProbes) {
		dbg("New MnRouterProbes: %u\n", new->MnRouterProbes);
		cur->MnRouterProbes = new->MnRouterProbes;
	}

#define ts_equal(a , b) \
	(((a).tv_sec == (b).tv_sec) && ((a).tv_nsec == (b).tv_nsec))

	/* (MnRouterProbeTimeout) MnRouterProbeTimeout_ts: update the value */
	if (!ts_equal(new->MnRouterProbeTimeout_ts,
		      cur->MnRouterProbeTimeout_ts)) {
		dbg("New MnRouterProbeTimeout: %f\n",
		    tstodsec(new->MnRouterProbeTimeout_ts));
		cur->MnRouterProbeTimeout_ts = new->MnRouterProbeTimeout_ts;
	}

	/* (InitialBindackTimeoutFirstReg) InitialBindackTimeoutFirstReg_ts:
	 * update the value */
	if (!ts_equal(new->InitialBindackTimeoutFirstReg_ts,
		      cur->InitialBindackTimeoutFirstReg_ts)) {
		dbg("New InitialBindackTimeoutFirstReg: %f "
		    "(will only affect new bindings)\n",
		    tstodsec(new->InitialBindackTimeoutFirstReg_ts));
		cur->InitialBindackTimeoutFirstReg_ts
			= new->InitialBindackTimeoutFirstReg_ts;
	}

	/* (InitialBindackTimeoutReReg) InitialBindackTimeoutReReg_ts:
	 * update the value */
	if (!ts_equal(new->InitialBindackTimeoutReReg_ts,
		      cur->InitialBindackTimeoutReReg_ts)) {
		dbg("New InitialBindackTimeoutReReg: %f "
		    "(does not affect existing bindings until they expire)\n",
		    tstodsec(new->InitialBindackTimeoutReReg_ts));
		cur->InitialBindackTimeoutReReg_ts
			= new->InitialBindackTimeoutReReg_ts;
	}


	/* (MnHomeLink) home_addrs: not supported */
	/* TODO:
	 * compare the old list and the new one, using hai_name for example.
	 * For new entries, call conf_home_addr_info.
	 * Do not remove deleted entries (or think carefully at the side 
	 * effects). For the entries that match the hai_name, compare all
	 * elements and update the matching entry in home_addr_list 
	 * global list if required. 
	 */
	if (is_mn())
		dbg("MnHomeLink directive ignored currently (only used on MN nodes)\n");

	/* (UseMovementModule) MoveModulePath: not implemented yet,
	 * should be NULL */
	if (new->MoveModulePath != cur->MoveModulePath) {
		fprintf(stderr, "Changing UseMovementModule is not supported.\n");
	}

	/* (UseCnBuAck) CnBuAck: update the value in config */
	if (new->CnBuAck != cur->CnBuAck) {
		dbg("New UseCnBuAck: %s\n",
		    new->CnBuAck ? "enabled" : "disabled");
		cur->CnBuAck = new->CnBuAck;
	}

	/* (MobRtrUseExplicitMode) MobRtrUseExplicitMode:
	 * update the value in config */
	if (new->MobRtrUseExplicitMode != cur->MobRtrUseExplicitMode) {
		dbg("New MobRtrUseExplicitMode: %s\n",
		    new->MobRtrUseExplicitMode ? "enabled" : "disabled");
		cur->MobRtrUseExplicitMode = new->MobRtrUseExplicitMode;
	}

	/* (DoRouteOptimizationMN) DoRouteOptimizationMN: deactivation
	 * not supported (may need changes in xfrm.c) */
	if (new->DoRouteOptimizationMN != cur->DoRouteOptimizationMN) {
		if (!new->DoRouteOptimizationMN) {
			fprintf(stderr, "Deactivating DoRouteOptimizationMN "
				"is not supported.\n");
		} else {
			cur->DoRouteOptimizationMN = new->DoRouteOptimizationMN;
			dbg("New DoRouteOptimizationMN: %s\n",
			    new->DoRouteOptimizationMN ? "enabled" : "disabled");
		}
	}

	/* (MnUseAllInterfaces) MnUseAllInterfaces:
	 * update the value in config */
	if (new->MnUseAllInterfaces != cur->MnUseAllInterfaces) {
		dbg("New MnUseAllInterfaces: %s "
		    "(only affects new interfaces)\n",
		    new->MnUseAllInterfaces ? "enabled" : "disabled");
		cur->MnUseAllInterfaces = new->MnUseAllInterfaces;
	}

	/* (MnDiscardHaParamProb) MnDiscardHaParamProb:
	 * update the value in config */
	if (new->MnDiscardHaParamProb != cur->MnDiscardHaParamProb) {
		dbg("New MnDiscardHaParamProb: %s\n",
		    new->MnDiscardHaParamProb ? "enabled" : "disabled");
		cur->MnDiscardHaParamProb = new->MnDiscardHaParamProb;
	}

	/* (MnResetDhaadAtHome) MnResetDhaadAtHome:
	 * update the value in config */
	if (new->MnResetDhaadAtHome != cur->MnResetDhaadAtHome) {
		dbg("New MnResetDhaadAtHome: %s\n",
		    new->MnResetDhaadAtHome ? "enabled" : "disabled");
		cur->MnResetDhaadAtHome = new->MnResetDhaadAtHome;
	}

	/* (MnFlushAllAtHome) MnFlushAllAtHome:
	 * update the value in config */
	if (new->MnFlushAllAtHome != cur->MnFlushAllAtHome) {
		dbg("New MnFlushAllAtHome: %s\n",
		    new->MnFlushAllAtHome ? "enabled" : "disabled");
		cur->MnFlushAllAtHome = new->MnFlushAllAtHome;
	}

	/* (MnMaxCnConsecutiveResends) MnMaxCnConsecutiveResends:
	 * update the value in config */
	if (new->MnMaxCnConsecutiveResends != cur->MnMaxCnConsecutiveResends) {
		dbg("New MnMaxCnConsecutiveResends: %u\n",
		    new->MnMaxCnConsecutiveResends);
		cur->MnMaxCnConsecutiveResends = new->MnMaxCnConsecutiveResends;
	}

	/* (MnMaxHaConsecutiveResends) MnMaxHaConsecutiveResends:
	 * update the value in config */
	if (new->MnMaxHaConsecutiveResends != cur->MnMaxHaConsecutiveResends) {
		dbg("New MnMaxHaConsecutiveResends: %u\n",
		    new->MnMaxHaConsecutiveResends);
		cur->MnMaxHaConsecutiveResends = new->MnMaxHaConsecutiveResends;
	}

	/* (SendMobPfxSols) SendMobPfxSols: update the value in config */
	if (new->SendMobPfxSols != cur->SendMobPfxSols) {
		dbg("New SendMobPfxSols: %s\n",
		    new->SendMobPfxSols ? "enabled" : "disabled");
		cur->SendMobPfxSols = new->SendMobPfxSols;

		/* If disabled: need to cancel the pending job? 
		 * (will be the last) */
		/* If enabled: need to call mpd_trigger_mps? 
		 * With what Hoa / HA ? */
	}

	/* (OptimisticHandoff) OptimisticHandoff:
	 * update the value in config */
	if (new->OptimisticHandoff != cur->OptimisticHandoff) {
		dbg("New OptimisticHandoff: %s\n",
		    new->OptimisticHandoff ? "enabled" : "disabled");
		cur->OptimisticHandoff = new->OptimisticHandoff;
	}

	/* (NoHomeReturn) NoHomeReturn:
	 * update the value in config */
	if (new->NoHomeReturn != cur->NoHomeReturn) {
		dbg("New NoHomeReturn: %s\n",
		    new->NoHomeReturn ? "enabled" : "disabled");
		cur->NoHomeReturn = new->NoHomeReturn;
	}

	/* (HaAcceptMobRtr) HaAcceptMobRtr: update the value in config */
	if (new->HaAcceptMobRtr != cur->HaAcceptMobRtr) {
		dbg("New HaAcceptMobRtr: %s\n",
		    new->HaAcceptMobRtr ? "enabled" : "disabled");
		cur->HaAcceptMobRtr = new->HaAcceptMobRtr;
	}

	/* (SendMobPfxAdvs) SendMobPfxAdvs: update the value in config */
	if (new->SendMobPfxAdvs != cur->SendMobPfxAdvs) {
		dbg("New SendMobPfxAdvs: %s\n",
		    new->SendMobPfxAdvs ? "enabled" : "disabled");
		cur->SendMobPfxAdvs = new->SendMobPfxAdvs;
	}

	/* (SendUnsolMobPfxAdvs) SendUnsolMobPfxAdvs:
	 * update the value in config */
	if (new->SendUnsolMobPfxAdvs != cur->SendUnsolMobPfxAdvs) {
		dbg("New SendUnsolMobPfxAdvs: %s\n",
		    new->SendUnsolMobPfxAdvs ? "enabled" : "disabled");
		cur->SendUnsolMobPfxAdvs = new->SendUnsolMobPfxAdvs;

		/* If enabled: mpd_start_mpa will be called for following
		 * BUs (that's enough) */
	}

	/* (MaxMobPfxAdvInterval) MaxMobPfxAdvInterval:
	 * update the value in config */
	if (new->MaxMobPfxAdvInterval != cur->MaxMobPfxAdvInterval) {
		dbg("New MaxMobPfxAdvInterval: %u\n",
		    new->MaxMobPfxAdvInterval);
		cur->MaxMobPfxAdvInterval = new->MaxMobPfxAdvInterval;
	}

	/* (MinMobPfxAdvInterval) MinMobPfxAdvInterval:
	 * update the value in config */
	if (new->MinMobPfxAdvInterval != cur->MinMobPfxAdvInterval) {
		dbg("New MinMobPfxAdvInterval: %u\n",
		    new->MinMobPfxAdvInterval);
		cur->MinMobPfxAdvInterval = new->MinMobPfxAdvInterval;
	}

	/* (HaMaxBindingLife) HaMaxBindingLife:
	 * update the value in config */
	if (new->HaMaxBindingLife != cur->HaMaxBindingLife) {
		dbg("New HaMaxBindingLife: %u\n", new->HaMaxBindingLife);
		cur->HaMaxBindingLife = new->HaMaxBindingLife;
	}

	/* (HaServedPrefix) nemo_ha_served_prefixes:
	 * update the value in config */
	parse_change_in_prefix_list_entry(&cur->nemo_ha_served_prefixes,
					  &new->nemo_ha_served_prefixes,
					  cmp_prefix_list_entry,
					  NULL,
					  add_prefix_list_entry,
					  rem_prefix_list_entry);

	/* (DoRouteOptimizationCN) DoRouteOptimizationCN:
	 * Update the value in config */
	if (new->DoRouteOptimizationCN != cur->DoRouteOptimizationCN) {
		dbg("New DoRouteOptimizationCN: %s\n",
		    new->DoRouteOptimizationCN ? "enabled" : "disabled");
		cur->DoRouteOptimizationCN = new->DoRouteOptimizationCN;
	}

	return;
}

