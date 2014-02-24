/*
 * $Id: conf.c 1.50 06/05/12 11:48:36+03:00 vnuorval@tcs.hut.fi $
 *
 * This file is part of the MIPL Mobile IPv6 for Linux.
 * 
 * Authors: Antti Tuominen <anttit@tcs.hut.fi>
 *          Ville Nuorvala <vnuorval@tcs.hut.fi>
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
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

#include <netinet/in.h>
#include <netinet/ip6mh.h>
#include <arpa/inet.h>
#include "defpath.h"
#include "conf.h"
#include "debug.h"
#include "util.h"
#include "mipv6.h"
#include "mn.h"
#include "cn.h"
#include "xfrm.h"
#include "prefix.h"
#include "ipsec.h"
#ifdef ENABLE_VT
#include "vt.h"
#endif

static void conf_usage(char *exec_name)
{
	fprintf(stderr,
		"Usage: %s [options]\nOptions:\n"
		"  -V, --version            Display version information and copyright\n"
		"  -?, -h, --help           Display this help text\n"
		"  -c <file>                Read configuration from <file>\n"
#ifdef ENABLE_VT
		"      --vt-service <serv>  Set VT service (default=" VT_DEFAULT_SERVICE ")\n"
#endif
		"\n These options override values read from config file:\n"
		"  -d <number>              Set debug level (0-10)\n"
		"  -l <file>                Write debug log to <file> instead of stderr\n"
		"  -C, --correspondent-node Node is CN\n"
		"  -H, --home-agent         Node is HA\n"
		"  -M, --mobile-node        Node is MN\n\n"
		"For bug reporting, see %s.\n",
		exec_name, PACKAGE_BUGREPORT);
}

static void conf_version(void)
{
	fprintf(stderr,
		"%s (%s) %s\n"
		"%s\n"
		"This is free software; see the source for copying conditions.  There is NO\n"
		"warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n",
		PACKAGE, PACKAGE_NAME, PACKAGE_VERSION, PACKAGE_COPYRIGHT);
}

static int conf_alt_file(char *filename, int argc, char **argv)
{
	int args_left = argc;
	char **cur_arg = argv;

	while (args_left--) {
		if (strcmp(*cur_arg, "-c") == 0 && args_left > 0) {
			cur_arg++;
			if (**cur_arg == '-')
				return -EINVAL;
			if (strlen(*cur_arg) >= MAXPATHLEN)
				return -ENAMETOOLONG;
			strcpy(filename, *cur_arg);
			return 0;
		}
		cur_arg++;
	}

	return 1;
}

static int conf_file(struct mip6_config *c, char *filename)
{
	extern FILE *yyin;
	int ret;

	yyin = fopen(filename, "r");
	if (yyin == NULL)
		return -ENOENT;

	c->config_file = malloc(strlen(filename) + 1);
	if (c->config_file == NULL)
		return -ENOMEM;
	strcpy(c->config_file, filename);

	ret = yyparse();

	fclose(yyin);

	/* Free memory allocated by yyparse() */
	yylex_destroy();

	if (ret) return -EINVAL;

	return 0;
}

static int conf_cmdline(struct mip6_config *cfg, int argc, char **argv)
{
	static struct option long_opts[] = {
		{"version", 0, 0, 'V'},
		{"help", 0, 0, 'h'},
		{"correspondent-node", 0, 0, 'C'},
		{"home-agent", 0, 0, 'H'},
		{"mobile-node", 0, 0, 'M'},
		{"show-config", 0, 0, 0},
#ifdef ENABLE_VT
		{"vt-service", 1, 0, 0 },
#endif
		{0, 0, 0, 0}
	};

	/* parse all other cmd line parameters than -c */
	while (1) {
		int idx, c;
		c = getopt_long(argc, argv, "c:d:l:Vh?CMH", long_opts, &idx);
		if (c == -1) break;

		switch (c) {
		case 0:
#ifdef ENABLE_VT
			if (strcmp(long_opts[idx].name, "vt-service") == 0) {
				cfg->vt_service = optarg;
				break;
			}
#endif
			if (idx == 5)
				conf_show(cfg);
			return -1;
		case 'V':
			conf_version();
			return -1;
		case '?':
		case 'h':
			conf_usage(basename(argv[0]));
			return -1;
		case 'd':
			cfg->debug_level = atoi(optarg);
			break;
		case 'l':
			cfg->debug_log_file = optarg;
			break;
		case 'C':
			cfg->mip6_entity = MIP6_ENTITY_CN;
			break;
		case 'H':
			cfg->mip6_entity = MIP6_ENTITY_HA;
			break;
		case 'M':
			cfg->mip6_entity = MIP6_ENTITY_MN;
			break;
		default:
			break;
		};
	}
	return 0;
}

static void conf_default(struct mip6_config *c)
{
	int i;
	memset(c, 0, sizeof(*c));

	/* Common options */
#ifdef ENABLE_VT
	c->vt_hostname = VT_DEFAULT_HOSTNAME;
	c->vt_service = VT_DEFAULT_SERVICE;
#endif
	c->mip6_entity = MIP6_ENTITY_CN;
	pmgr_init(NULL, &c->pmgr);
	INIT_LIST_HEAD(&c->net_ifaces);
	INIT_LIST_HEAD(&c->bind_acl);
	c->DefaultBindingAclPolicy = IP6_MH_BAS_ACCEPTED;

	/* IPsec options */
	c->UseMnHaIPsec = 1;
	INIT_LIST_HEAD(&c->ipsec_policies);

	/* MN options */
	c->MnMaxHaBindingLife = MAX_BINDING_LIFETIME;
	c->MnMaxCnBindingLife = MAX_RR_BINDING_LIFETIME;
	tssetdsec(c->InitialBindackTimeoutFirstReg_ts, 1.5);/*seconds*/
	tssetsec(c->InitialBindackTimeoutReReg_ts, INITIAL_BINDACK_TIMEOUT);/*seconds*/
	tssetsec(c->InitialSolicitTimer_ts, INITIAL_SOLICIT_TIMER);/*seconds*/
	tssetsec(c->InterfaceInitialInitDelay_ts, 2); /*seconds*/
	INIT_LIST_HEAD(&c->home_addrs);
	c->MoveModulePath = NULL; /* internal */
	c->DoRouteOptimizationMN = 1;
	c->MobRtrUseExplicitMode = 1;
	c->SendMobPfxSols = 1;
	c->OptimisticHandoff = 0;
	c->NoHomeReturn = 0;
	c->MnResetDhaadAtHome = 0;
	c->MnFlushAllAtHome = 0;
	c->MnMaxCnConsecutiveResends = 0;
	c->MnMaxHaConsecutiveResends = 5;

	/* HA options */
	c->SendMobPfxAdvs = 1;
	c->SendUnsolMobPfxAdvs = 1;
	c->MaxMobPfxAdvInterval = 86400; /* seconds */
	c->MinMobPfxAdvInterval = 600; /* seconds */
	c->HaMaxBindingLife = MAX_BINDING_LIFETIME;
	INIT_LIST_HEAD(&c->nemo_ha_served_prefixes);


	/* PMIP global options */
	c->RFC5213TimestampBasedApproachInUse = 1;
	c->RFC5213MobileNodeGeneratedTimestampInUse = 1;
	c->RFC5213FixedMAGLinkLocalAddressOnAllAccessLinks = in6addr_any;
	c->RFC5213FixedMAGLinkLayerAddressOnAllAccessLinks = in6addr_any;
	struct timespec lifetime_tunnels;
	lifetime_tunnels.tv_sec  = 60;
	lifetime_tunnels.tv_nsec = 0;
	c->MaxDelayBeforeDynamicTunnelingDelete   = lifetime_tunnels; // 60000 milliseconds

	/* PMIP LMA options */
	struct timespec lifetime1;
	lifetime1.tv_sec = 10;
	lifetime1.tv_nsec = 0;
	c->RFC5213MinDelayBeforeBCEDelete         = lifetime1; // 10000 milliseconds
	struct timespec lifetime2;
	lifetime2.tv_sec = 1;
	lifetime2.tv_nsec = 500000000;
	c->RFC5213MaxDelayBeforeNewBCEAssign      = lifetime2; // 1500 milliseconds
	struct timespec lifetime3;
	lifetime3.tv_sec = 0;
	lifetime3.tv_nsec = 300000000;
	c->RFC5213TimestampValidityWindow         = lifetime3; // 300 milliseconds


	/* CN bindings */
	c->DoRouteOptimizationCN = 1;
	INIT_LIST_HEAD(&c->cn_binding_pol);

	/* PMIP MAG options */
	c->RFC5213EnableMAGLocalRouting = 0;
	c->HomeNetworkPrefix             = in6addr_any;
	c->NumMags                       = 0;
	for (i = 0; i <  PMIP_MAX_MAGS; i++) {
		c->MagAddressIngress[i]      = in6addr_loopback;
		c->MagAddressEgress[i]       = in6addr_loopback;
	}
	c->MagDeviceIngress              = "";
	c->MagDeviceEgress               = "";
	c->LmaAddress                    = in6addr_loopback;
	c->OurAddress                    = in6addr_loopback;
	//Lifetime for PB entry
	struct timespec lifetime4;
	lifetime4.tv_sec  = 40;
	lifetime4.tv_nsec = 0;
	c->PBULifeTime    = lifetime4;
	struct timespec lifetime5;
	lifetime5.tv_sec  = 40;
	lifetime5.tv_nsec = 0;
	c->PBALifeTime    = lifetime5;
	//Time for N_Retransmissions
	struct timespec lifetime6;
	lifetime6.tv_sec  = 1;
	lifetime6.tv_nsec = 0;
	c->RetransmissionTimeOut = lifetime6;
	//Define the maximum # of message retransmissions.
	int Max_rets = 5;
	c->MaxMessageRetransmissions = Max_rets;
	c->TunnelingEnabled          = 0;
	c->DynamicTunnelingEnabled   = 0;
	c->RadiusClientConfigFile    = "";
	c->RadiusPassword            = "";

}

int conf_parse(struct mip6_config *c, int argc, char **argv)
{
	char cfile[MAXPATHLEN];
	int ret;

	/* set config defaults */
	conf_default(c);

	if ((ret = conf_alt_file(cfile, argc, argv)) != 0) {
		if (ret == -EINVAL) {
			fprintf(stderr,
				"%s: option requires an argument -- c\n",
				argv[0]);
			conf_usage(basename(argv[0]));
			return -1;
		} else if (ret == -ENAMETOOLONG) {
			fprintf(stderr,
				"%s: argument too long -- c <file>\n",
				argv[0]);
			return -1;
		}
		strcpy(cfile, DEFAULT_CONFIG_FILE);
	}

	if (conf_file(c, cfile) < 0 && ret == 0)
		return -1;

	if (conf_cmdline(c, argc, argv) < 0)
		return -1;

	return 0;
}

#define CONF_BOOL_STR(x) ((x) ? "enabled" : "disabled")

void conf_show(struct mip6_config *c)
{
	struct list_head *list;
	int i;
	/* Common options */
	dbg("Configuration file = %s\n", c->config_file);
#ifdef ENABLE_VT
	dbg("VT hostname = %s\n", c->vt_hostname);
	dbg("VT service = %s\n", c->vt_service);
#endif
	dbg("NodeConfig = %u\n", c->mip6_entity);
	dbg("DebugLevel = %u\n", c->debug_level);
	dbg("DebugLogFile = %s\n",
	    (c->debug_log_file ? c->debug_log_file : "stderr"));
	dbg("DoRouteOptimizationCN = %s\n",
	    CONF_BOOL_STR(c->DoRouteOptimizationCN));
	list_for_each(list, &c->cn_binding_pol) {
		struct cn_binding_pol_entry *pol;
		pol = list_entry(list, struct cn_binding_pol_entry, list);
		dbg("CnBindingPolicySet %x:%x:%x:%x:%x:%x:%x:%x "
		    "%x:%x:%x:%x:%x:%x:%x:%x %s\n",
		    NIP6ADDR(&pol->remote_hoa),
		    NIP6ADDR(&pol->local_addr),
		    pol->bind_policy ? "enabled" : "disabled" );
	}

	dbg("NonVolatileBindingCache = %s\n",
	    CONF_BOOL_STR(c->NonVolatileBindingCache));
	if (c->pmgr.so_path)
		dbg("PolicyModulePath = %s\n", c->pmgr.so_path);
	
	/* IPsec options */
	dbg("KeyMngMobCapability = %s\n",
	    CONF_BOOL_STR(c->KeyMngMobCapability));
	dbg("UseMnHaIPsec = %s\n", CONF_BOOL_STR(c->UseMnHaIPsec));

	switch (c->mip6_entity) {
		case MIP6_ENTITY_MN:
		/* MN options */
		dbg("MnMaxHaBindingLife = %u\n", c->MnMaxHaBindingLife);
		dbg("MnMaxCnBindingLife = %u\n", c->MnMaxCnBindingLife);
		dbg("MnDiscardHaParamProb = %s\n",
		    CONF_BOOL_STR(c->MnDiscardHaParamProb));
		dbg("MnResetDhaadAtHome = %s\n",
		    CONF_BOOL_STR(c->MnResetDhaadAtHome));
		dbg("MnFlushAllAtHome = %s\n",
		    CONF_BOOL_STR(c->MnFlushAllAtHome));
		dbg("MnMaxHaConsecutiveResends = %u\n",
		    c->MnMaxHaConsecutiveResends);
		dbg("MnMaxCnConsecutiveResends = %u\n",
		    c->MnMaxCnConsecutiveResends);
		dbg("SendMobPfxSols = %s\n",
		    CONF_BOOL_STR(c->SendMobPfxSols));
		dbg("DoRouteOptimizationMN = %s\n",
		    CONF_BOOL_STR(c->DoRouteOptimizationMN));
		dbg("MnUseAllInterfaces = %s\n",
		    CONF_BOOL_STR(c->MnUseAllInterfaces));
		dbg("UseCnBuAck = %s\n", CONF_BOOL_STR(c->CnBuAck));
		dbg("InterfaceInitialInitDelay = %f\n",
		    tstodsec(c->InterfaceInitialInitDelay_ts));
		dbg("MnRouterProbes = %u\n", c->MnRouterProbes);
		dbg("MnRouterProbeTimeout = %f\n",
		    tstodsec(c->MnRouterProbeTimeout_ts));
		dbg("InitialBindackTimeoutFirstReg = %f\n", 
		    tstodsec(c->InitialBindackTimeoutFirstReg_ts));
		dbg("InitialBindackTimeoutReReg = %f\n", 
		    tstodsec(c->InitialBindackTimeoutReReg_ts));
		dbg("InitialSolicitTimer = %f\n",
		    tstodsec(c->InitialSolicitTimer_ts));
		dbg("OptimisticHandoff = %s\n",
		    CONF_BOOL_STR(c->OptimisticHandoff));
		dbg("MobRtrUseExplicitMode = %s\n",
		    CONF_BOOL_STR(c->MobRtrUseExplicitMode));
		dbg("NoHomeReturn = %s\n", CONF_BOOL_STR(c->NoHomeReturn));
		if (c->MoveModulePath)
			dbg("MoveModulePath = %s\n", c->MoveModulePath);
		break;

		case MIP6_ENTITY_HA:
		/* HA options */
		dbg("HaMaxBindingLife = %u\n", c->HaMaxBindingLife);
		dbg("SendMobPfxAdvs = %s\n", CONF_BOOL_STR(c->SendMobPfxAdvs));
		dbg("SendUnsolMobPfxAdvs = %s\n",
		    CONF_BOOL_STR(c->SendUnsolMobPfxAdvs));
		dbg("MinMobPfxAdvInterval = %u\n", c->MinMobPfxAdvInterval);
		dbg("MaxMobPfxAdvInterval = %u\n", c->MaxMobPfxAdvInterval);
		dbg("HaAcceptMobRtr = %s\n", CONF_BOOL_STR(c->HaAcceptMobRtr));
		if (list_empty(&c->nemo_ha_served_prefixes)) {
			dbg("HaServedPrefix = no prefixes\n");
		} else {
			dbg("HaServedPrefix =\n");
			list_for_each(list, &c->nemo_ha_served_prefixes) {
				struct prefix_list_entry *pfx;
				pfx = list_entry(list, struct prefix_list_entry, list);
				dbg("- %x:%x:%x:%x:%x:%x:%x:%x/%d\n",
				    NIP6ADDR(&pfx->ple_prefix), pfx->ple_plen);
			}
		}
		dbg("DefaultBindingAclPolicy = %s\n",
		    (c->DefaultBindingAclPolicy  == IP6_MH_BAS_ACCEPTED) ?
		     "allow":"deny");
		list_for_each(list, &c->bind_acl) {
			struct policy_bind_acl_entry *acl;
			acl = list_entry(list, struct policy_bind_acl_entry, list);
			dbg("- HoA %x:%x:%x:%x:%x:%x:%x:%x (%d MNP): %s\n",
			    NIP6ADDR(&acl->hoa), acl->mnp_count,
			    (acl->bind_policy == IP6_MH_BAS_ACCEPTED) ?
			    "allow":"deny");
		}

		/* PMIPv6 LMA options */
		dbg("RFC5213TimestampBasedApproachInUse = %s\n",
			CONF_BOOL_STR(c->RFC5213TimestampBasedApproachInUse));
		dbg("RFC5213MobileNodeGeneratedTimestampInUse = %s\n",
			CONF_BOOL_STR(c->RFC5213MobileNodeGeneratedTimestampInUse));
		dbg("RFC5213FixedMAGLinkLocalAddressOnAllAccessLinks = %x:%x:%x:%x:%x:%x:%x:%x\n",
			NIP6ADDR(&c->RFC5213FixedMAGLinkLocalAddressOnAllAccessLinks));
		dbg("RFC5213FixedMAGLinkLayerAddressOnAllAccessLinks = %x:%x:%x:%x:%x:%x:%x:%x\n",
			NIP6ADDR(&c->RFC5213FixedMAGLinkLayerAddressOnAllAccessLinks));
		dbg("RFC5213MinDelayBeforeBCEDelete = %u.%9u seconds\n",
			c->RFC5213MinDelayBeforeBCEDelete.tv_sec,
			c->RFC5213MinDelayBeforeBCEDelete.tv_nsec);
		dbg("RFC5213MaxDelayBeforeNewBCEAssign = %u.%9u seconds\n",
			c->RFC5213MaxDelayBeforeNewBCEAssign.tv_sec,
			c->RFC5213MaxDelayBeforeNewBCEAssign.tv_nsec);
		dbg("RFC5213TimestampValidityWindow = %u.%9u seconds\n",
			c->RFC5213TimestampValidityWindow.tv_sec,
			c->RFC5213TimestampValidityWindow.tv_nsec);

		dbg("LmaPmipNetworkAddress = %x:%x:%x:%x:%x:%x:%x:%x\n",
			NIP6ADDR(&c->LmaAddress));
		dbg("OurAddress = %x:%x:%x:%x:%x:%x:%x:%x\n",
			NIP6ADDR(&c->OurAddress));
		dbg("HomeNetworkPrefix = %x:%x:%x:%x:%x:%x:%x:%x\n",
			NIP6ADDR(&c->HomeNetworkPrefix));
		dbg("MaxMessageRetransmissions = %u\n",
			c->MaxMessageRetransmissions);
		dbg("TunnelingEnabled = %s\n",
			CONF_BOOL_STR(c->TunnelingEnabled));
		dbg("DynamicTunnelingEnabled = %s\n",
			CONF_BOOL_STR(c->DynamicTunnelingEnabled));
		dbg("MaxDelayBeforeDynamicTunnelingDelete = %u.%9u seconds\n",
			c->MaxDelayBeforeDynamicTunnelingDelete.tv_sec,
			c->MaxDelayBeforeDynamicTunnelingDelete.tv_nsec);

		for (i = 0; i < c->NumMags; i++) {
			dbg("MAG %d: \n", i);
			dbg("- MagAddressIngress = %x:%x:%x:%x:%x:%x:%x:%x\n",
				NIP6ADDR(&c->MagAddressIngress[i]));
			dbg("- MagAddressEgress = %x:%x:%x:%x:%x:%x:%x:%x\n",
				NIP6ADDR(&c->MagAddressEgress[i]));
		}
		break;

		case MIP6_ENTITY_MAG:
		/* PMIPv6 MAG options */
		dbg("RFC5213TimestampBasedApproachInUse = %s\n",
			CONF_BOOL_STR(c->RFC5213TimestampBasedApproachInUse));
		dbg("RFC5213MobileNodeGeneratedTimestampInUse = %s\n",
			CONF_BOOL_STR(c->RFC5213MobileNodeGeneratedTimestampInUse));
		dbg("RFC5213FixedMAGLinkLocalAddressOnAllAccessLinks = %x:%x:%x:%x:%x:%x:%x:%x\n",
			NIP6ADDR(&c->RFC5213FixedMAGLinkLocalAddressOnAllAccessLinks));
		dbg("RFC5213FixedMAGLinkLayerAddressOnAllAccessLinks = %x:%x:%x:%x:%x:%x:%x:%x\n",
			NIP6ADDR(&c->RFC5213FixedMAGLinkLayerAddressOnAllAccessLinks));
		dbg("RFC5213EnableMAGLocalRouting = %s\n",
			CONF_BOOL_STR(c->RFC5213EnableMAGLocalRouting));

		dbg("MagAddressIngress = %x:%x:%x:%x:%x:%x:%x:%x\n",
			NIP6ADDR(&c->MagAddressIngress[0]));
		dbg("MagAddressEgress = %x:%x:%x:%x:%x:%x:%x:%x\n", 
			NIP6ADDR(&c->MagAddressEgress[0]));
		dbg("MagDeviceIngress = %s\n",
			(c->MagDeviceIngress ? c->MagDeviceIngress : "No device"));
		dbg("MagDeviceEgress = %s\n",
			(c->MagDeviceEgress ? c->MagDeviceEgress : "No device"));

		dbg("PBULifeTime = %u.%9u seconds\n",
			c->PBULifeTime.tv_sec,c->PBULifeTime.tv_nsec);
		dbg("RetransmissionTimeOut = %u.%9u seconds\n",
			c->RetransmissionTimeOut.tv_sec,
			c->RetransmissionTimeOut.tv_nsec);

		dbg("LmaPmipNetworkAddress = %x:%x:%x:%x:%x:%x:%x:%x\n",
			NIP6ADDR(&c->LmaAddress));
		dbg("OurAddress = %x:%x:%x:%x:%x:%x:%x:%x\n",
			NIP6ADDR(&c->OurAddress));
		dbg("HomeNetworkPrefix = %x:%x:%x:%x:%x:%x:%x:%x\n",
			NIP6ADDR(&c->HomeNetworkPrefix));
		dbg("MaxMessageRetransmissions = %u\n",
			c->MaxMessageRetransmissions);
		dbg("TunnelingEnabled = %s\n",
			CONF_BOOL_STR(c->TunnelingEnabled));
		dbg("DynamicTunnelingEnabled = %s\n",
			CONF_BOOL_STR(c->DynamicTunnelingEnabled));
		dbg("MaxDelayBeforeDynamicTunnelingDelete = %u.%9u seconds\n",
			c->MaxDelayBeforeDynamicTunnelingDelete.tv_sec,
			c->MaxDelayBeforeDynamicTunnelingDelete.tv_nsec);

		dbg("RadiusClientConfigFile = %s\n",
			(c->RadiusClientConfigFile ?
			 c->RadiusClientConfigFile : "No Config file"));
		dbg("RadiusPassword = %s\n",
			(c->RadiusPassword ? c->RadiusPassword : "No password"));

		dbg("PcapSyslogAssociationGrepString = %s\n",
			(c->PcapSyslogAssociationGrepString ?
			 c->PcapSyslogAssociationGrepString :
			 "No syslog association grep string"));
		dbg("PcapSyslogDeAssociationGrepString = %s\n",
			(c->PcapSyslogDeAssociationGrepString ?
			 c->PcapSyslogDeAssociationGrepString :
			 "No syslog de-association grep string"));
		break;

		case MIP6_ENTITY_CN:
		/* CN options: no specific options (DoRouteOptimizationCN 
		 * is shared by all nodes) */
		break;

		default:
		break;
	}
}

void conf_free(struct mip6_config *c)
{
	struct list_head *h, *nh;

	if (c->config_file) {
		free(c->config_file);
	}

	pmgr_close(&c->pmgr);

	/* Cleanup the net_ifaces list */
	list_for_each_safe(h, nh, &c->net_ifaces) {
		list_del(h);
		free(list_entry(h, struct net_iface, list));
	}

	/* For each home_addr_info, we have to remove the 
	 * intern lists mob_net_prefix and ro_policy */
	list_for_each_safe(h, nh, &c->home_addrs) {
		struct home_addr_info *hai;
		list_del(h);

		hai = list_entry(h, struct home_addr_info, list);
		prefix_list_free(&hai->ro_policies);
		prefix_list_free(&hai->mob_net_prefixes);
		free(hai);
	}

	/* Free the ipsec_policies list */
	list_for_each_safe(h, nh, &c->ipsec_policies) {
		list_del(h);
		free(list_entry(h, struct ipsec_policy_entry, list));
	}

	/* Free the nemo_ha_served_prefixes list */
	prefix_list_free(&c->nemo_ha_served_prefixes);

	/* bind_acl is cleaned by by policy.c/policy_cleanup() */
	/* cn_binding_pol is cleaned by by cn.c/cn_cleanup() */

	/* Free debug_log_file and MoveModulePath,
	 * allocated by gram.y */
	if (c->debug_log_file)
		free (c->debug_log_file);
	if (c->MoveModulePath)
		free (c->MoveModulePath);
}

int conf_update(struct mip6_config *c,
		void (*apply_changes_cb)(struct mip6_config *,
					 struct mip6_config *))
{
	/* c is a pointer to the current config.
	 * We want to update some data from c according
	 * to the changed configuration file. */
	struct mip6_config *conf_new = NULL;
	int ret = 0;

	conf_new = malloc(sizeof(*conf_new));
	if (!conf_new) {
		perror("conf_update");
		return -1;
	}

	conf_default(conf_new);

	/* gram.y stores configuration items in conf_parsed,
	 * so point it to conf_new */
	conf_parsed = conf_new;

	if ((ret = conf_file(conf_new, c->config_file)) < 0) {
		dbg("Error in the new configuration file, "
		    "changes not applied\n");
	}

	/* Make conf_parsed point back to the original config */
	conf_parsed = c;

	/* We now have the new config in conf_new, we can compute
	 * the differences that we are interrested in */
	if (!ret) {
		apply_changes_cb(c, conf_new);
	}

	/* Free the memory allocated during the process.
	 * conf_free() does not free conf_new so we free it 
	 * by ourselves */
	conf_free(conf_new);
	free(conf_new);

	return ret;
}
