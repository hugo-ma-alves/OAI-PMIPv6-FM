/* A Bison parser, made by GNU Bison 2.7.12-4996.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7.12-4996"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 28 "gram.y"


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <pthread.h>
#include <netinet/in.h>
#include <net/if.h>
#include <string.h>
#include <stdarg.h>
#include <netinet/ip6mh.h>
#include "mipv6.h"
#include "ha.h"
#include "mn.h"
#include "cn.h"
#include "conf.h"
#include "policy.h"
#include "xfrm.h"
#include "prefix.h"
#include "util.h"
#include "ipsec.h"
#include "rtnl.h"

struct net_iface ni = {
	.mip6_if_entity = MIP6_ENTITY_NO,
	.mn_if_preference = POL_MN_IF_DEF_PREFERENCE,
	.is_tunnel = 0,
};
	
struct home_addr_info hai = {
	.ro_policies = LIST_HEAD_INIT(hai.ro_policies),
	.mob_net_prefixes = LIST_HEAD_INIT(hai.mob_net_prefixes)
};

LIST_HEAD(prefixes);

int mv_prefixes(struct list_head *list)
{
	struct list_head *l, *n;
	int res = 0;
	list_for_each_safe(l, n, &prefixes) {
		list_del(l);
		list_add_tail(l, list);
		res++;
	}
	return res;
}

struct policy_bind_acl_entry *bae = NULL;
struct cn_binding_pol_entry *cnbpol = NULL;

struct ipsec_policy_set {
	struct in6_addr ha;
	struct list_head hoa_list;
};

struct ipsec_policy_set ipsec_ps = {
	.hoa_list = LIST_HEAD_INIT(ipsec_ps.hoa_list)
};

extern int lineno;
extern char *yytext;
extern char *incl_file; /* If not NULL, name of included file being parsed.
			 * If NULL, we are in main configuration file */

static void yyerror(char *s) {
	fprintf(stderr, "Error in configuration file %s ",
		incl_file ? incl_file : conf_parsed->config_file);
	fprintf(stderr, "at line %d: %s at '%s'\n", lineno, s, yytext);
}

static void uerror(const char *fmt, ...) {
	char s[1024];
	va_list args;

	fprintf(stderr, "Error in configuration file %s ",
		incl_file ? incl_file : conf_parsed->config_file);
	va_start(args, fmt);
	vsprintf(s, fmt, args);
	fprintf(stderr, "at line %d: %s\n", lineno, s);
	va_end(args);
}


/* Line 371 of yacc.c  */
#line 154 "y.tab.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     QSTRING = 258,
     ADDR = 259,
     MACADDR = 260,
     BOOL = 261,
     NUMBER = 262,
     DECIMAL = 263,
     NUMPAIR = 264,
     MIP6ENTITY = 265,
     DEBUGLEVEL = 266,
     DEBUGLOGFILE = 267,
     DOROUTEOPTIMIZATIONCN = 268,
     DOROUTEOPTIMIZATIONMN = 269,
     HOMEADDRESS = 270,
     HOMEAGENTADDRESS = 271,
     INITIALBINDACKTIMEOUTFIRSTREG = 272,
     INITIALBINDACKTIMEOUTREREG = 273,
     INITIALSOLICITTIMER = 274,
     INTERFACEINITIALINITDELAY = 275,
     LINKNAME = 276,
     HAMAXBINDINGLIFE = 277,
     MNMAXHABINDINGLIFE = 278,
     MNMAXCNBINDINGLIFE = 279,
     MAXMOBPFXADVINTERVAL = 280,
     MINMOBPFXADVINTERVAL = 281,
     MNHOMELINK = 282,
     HAHOMELINK = 283,
     NONVOLATILEBINDINGCACHE = 284,
     SENDMOBPFXSOLS = 285,
     SENDUNSOLMOBPFXADVS = 286,
     SENDMOBPFXADVS = 287,
     IPSECPOLICYSET = 288,
     IPSECPOLICY = 289,
     IPSECTYPE = 290,
     USEALTCOA = 291,
     USEESP = 292,
     USEAH = 293,
     USEIPCOMP = 294,
     BLOCK = 295,
     USEMNHAIPSEC = 296,
     KEYMNGMOBCAPABILITY = 297,
     HOMEREGBINDING = 298,
     MH = 299,
     MOBPFXDISC = 300,
     TUNNELHOMETESTING = 301,
     TUNNELMH = 302,
     TUNNELPAYLOAD = 303,
     USEMOVEMENTMODULE = 304,
     USEPOLICYMODULE = 305,
     MIP6CN = 306,
     MIP6MN = 307,
     MIP6HA = 308,
     INTERNAL = 309,
     MNROPOLICY = 310,
     ICMP = 311,
     ANY = 312,
     DOROUTEOPT = 313,
     DEFAULTBINDINGACLPOLICY = 314,
     BINDINGACLPOLICY = 315,
     MNADDRESS = 316,
     USECNBUACK = 317,
     INTERFACE = 318,
     IFNAME = 319,
     IFTYPE = 320,
     MNIFPREFERENCE = 321,
     ISTUNNEL = 322,
     MNUSEALLINTERFACES = 323,
     MNROUTERPROBES = 324,
     MNROUTERPROBETIMEOUT = 325,
     MNDISCARDHAPARAMPROB = 326,
     MNRESETDHAADATHOME = 327,
     MNFLUSHALLATHOME = 328,
     MNMAXHACONSECUTIVERESENDS = 329,
     MNMAXCNCONSECUTIVERESENDS = 330,
     OPTIMISTICHANDOFF = 331,
     NOHOMERETURN = 332,
     HOMEPREFIX = 333,
     HAACCEPTMOBRTR = 334,
     ISMOBRTR = 335,
     HASERVEDPREFIX = 336,
     MOBRTRUSEEXPLICITMODE = 337,
     CNBINDINGPOLICYSET = 338,
     RFC5213TIMESTAMPBASEDAPPROACHINUSE = 339,
     RFC5213MOBILENODEGENERATEDTIMESTAMPINUSE = 340,
     RFC5213FIXEDMAGLINKLOCALADDRESSONALLACCESSLINKS = 341,
     RFC5213FIXEDMAGLINKLAYERADDRESSONALLACCESSLINKS = 342,
     RFC5213MINDELAYBEFOREBCEDELETE = 343,
     RFC5213MAXDELAYBEFORENEWBCEASSIGN = 344,
     RFC5213TIMESTAMPVALIDITYWINDOW = 345,
     RFC5213ENABLEMAGLOCALROUTING = 346,
     MIP6LMA = 347,
     MIP6MAG = 348,
     PROXYMIPLMA = 349,
     PROXYMIPMAG = 350,
     ALLLMAMULTICASTADDRESS = 351,
     LMAPMIPNETWORKADDRESS = 352,
     LMAPMIPNETWORKDEVICE = 353,
     LMACORENETWORKADDRESS = 354,
     LMACORENETWORKDEVICE = 355,
     MAGADDRESSINGRESS = 356,
     MAGADDRESSEGRESS = 357,
     MAGDEVICEINGRESS = 358,
     MAGDEVICEEGRESS = 359,
     OURADDRESS = 360,
     HOMENETWORKPREFIX = 361,
     PBULIFETIME = 362,
     PBALIFETIME = 363,
     RETRANSMISSIONTIMEOUT = 364,
     MAXMESSAGERETRANSMISSIONS = 365,
     TUNNELINGENABLED = 366,
     DYNAMICTUNNELINGENABLED = 367,
     MAXDELAYBEFOREDYNAMICTUNNELINGDELETE = 368,
     RADIUSPASSWORD = 369,
     RADIUSCLIENTCONFIGFILE = 370,
     PCAPSYSLOGASSOCIATIONGREPSTRING = 371,
     PCAPSYSLOGDEASSOCIATIONGREPSTRING = 372,
     INV_TOKEN = 373
   };
#endif
/* Tokens.  */
#define QSTRING 258
#define ADDR 259
#define MACADDR 260
#define BOOL 261
#define NUMBER 262
#define DECIMAL 263
#define NUMPAIR 264
#define MIP6ENTITY 265
#define DEBUGLEVEL 266
#define DEBUGLOGFILE 267
#define DOROUTEOPTIMIZATIONCN 268
#define DOROUTEOPTIMIZATIONMN 269
#define HOMEADDRESS 270
#define HOMEAGENTADDRESS 271
#define INITIALBINDACKTIMEOUTFIRSTREG 272
#define INITIALBINDACKTIMEOUTREREG 273
#define INITIALSOLICITTIMER 274
#define INTERFACEINITIALINITDELAY 275
#define LINKNAME 276
#define HAMAXBINDINGLIFE 277
#define MNMAXHABINDINGLIFE 278
#define MNMAXCNBINDINGLIFE 279
#define MAXMOBPFXADVINTERVAL 280
#define MINMOBPFXADVINTERVAL 281
#define MNHOMELINK 282
#define HAHOMELINK 283
#define NONVOLATILEBINDINGCACHE 284
#define SENDMOBPFXSOLS 285
#define SENDUNSOLMOBPFXADVS 286
#define SENDMOBPFXADVS 287
#define IPSECPOLICYSET 288
#define IPSECPOLICY 289
#define IPSECTYPE 290
#define USEALTCOA 291
#define USEESP 292
#define USEAH 293
#define USEIPCOMP 294
#define BLOCK 295
#define USEMNHAIPSEC 296
#define KEYMNGMOBCAPABILITY 297
#define HOMEREGBINDING 298
#define MH 299
#define MOBPFXDISC 300
#define TUNNELHOMETESTING 301
#define TUNNELMH 302
#define TUNNELPAYLOAD 303
#define USEMOVEMENTMODULE 304
#define USEPOLICYMODULE 305
#define MIP6CN 306
#define MIP6MN 307
#define MIP6HA 308
#define INTERNAL 309
#define MNROPOLICY 310
#define ICMP 311
#define ANY 312
#define DOROUTEOPT 313
#define DEFAULTBINDINGACLPOLICY 314
#define BINDINGACLPOLICY 315
#define MNADDRESS 316
#define USECNBUACK 317
#define INTERFACE 318
#define IFNAME 319
#define IFTYPE 320
#define MNIFPREFERENCE 321
#define ISTUNNEL 322
#define MNUSEALLINTERFACES 323
#define MNROUTERPROBES 324
#define MNROUTERPROBETIMEOUT 325
#define MNDISCARDHAPARAMPROB 326
#define MNRESETDHAADATHOME 327
#define MNFLUSHALLATHOME 328
#define MNMAXHACONSECUTIVERESENDS 329
#define MNMAXCNCONSECUTIVERESENDS 330
#define OPTIMISTICHANDOFF 331
#define NOHOMERETURN 332
#define HOMEPREFIX 333
#define HAACCEPTMOBRTR 334
#define ISMOBRTR 335
#define HASERVEDPREFIX 336
#define MOBRTRUSEEXPLICITMODE 337
#define CNBINDINGPOLICYSET 338
#define RFC5213TIMESTAMPBASEDAPPROACHINUSE 339
#define RFC5213MOBILENODEGENERATEDTIMESTAMPINUSE 340
#define RFC5213FIXEDMAGLINKLOCALADDRESSONALLACCESSLINKS 341
#define RFC5213FIXEDMAGLINKLAYERADDRESSONALLACCESSLINKS 342
#define RFC5213MINDELAYBEFOREBCEDELETE 343
#define RFC5213MAXDELAYBEFORENEWBCEASSIGN 344
#define RFC5213TIMESTAMPVALIDITYWINDOW 345
#define RFC5213ENABLEMAGLOCALROUTING 346
#define MIP6LMA 347
#define MIP6MAG 348
#define PROXYMIPLMA 349
#define PROXYMIPMAG 350
#define ALLLMAMULTICASTADDRESS 351
#define LMAPMIPNETWORKADDRESS 352
#define LMAPMIPNETWORKDEVICE 353
#define LMACORENETWORKADDRESS 354
#define LMACORENETWORKDEVICE 355
#define MAGADDRESSINGRESS 356
#define MAGADDRESSEGRESS 357
#define MAGDEVICEINGRESS 358
#define MAGDEVICEEGRESS 359
#define OURADDRESS 360
#define HOMENETWORKPREFIX 361
#define PBULIFETIME 362
#define PBALIFETIME 363
#define RETRANSMISSIONTIMEOUT 364
#define MAXMESSAGERETRANSMISSIONS 365
#define TUNNELINGENABLED 366
#define DYNAMICTUNNELINGENABLED 367
#define MAXDELAYBEFOREDYNAMICTUNNELINGDELETE 368
#define RADIUSPASSWORD 369
#define RADIUSCLIENTCONFIGFILE 370
#define PCAPSYSLOGASSOCIATIONGREPSTRING 371
#define PCAPSYSLOGDEASSOCIATIONGREPSTRING 372
#define INV_TOKEN 373



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 114 "gram.y"

	char *string;
	struct in6_addr addr;
	struct in6_addr macaddr;
	char bool;
	unsigned int num;
	unsigned int numpair[2];
	double dec;


/* Line 387 of yacc.c  */
#line 444 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 472 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#  define __attribute__(Spec) /* empty */
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif


/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  106
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   515

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  126
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  47
/* YYNRULES -- Number of rules.  */
#define YYNRULES  174
/* YYNRULES -- Number of states.  */
#define YYNSTATES  406

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   373

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     123,   124,     2,     2,   125,     2,     2,   122,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   119,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   120,     2,   121,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    12,    16,    20,    24,    27,
      31,    35,    39,    43,    47,    51,    55,    59,    63,    67,
      71,    75,    79,    83,    86,    90,    94,    98,   102,   106,
     110,   114,   118,   122,   126,   131,   135,   139,   143,   147,
     151,   155,   159,   163,   167,   171,   176,   179,   182,   184,
     186,   188,   190,   193,   197,   199,   201,   204,   208,   212,
     216,   221,   223,   226,   230,   234,   238,   242,   246,   252,
     255,   259,   263,   267,   269,   272,   276,   280,   282,   285,
     291,   294,   296,   298,   300,   302,   304,   306,   308,   310,
     311,   313,   316,   320,   322,   324,   326,   327,   329,   332,
     333,   335,   338,   339,   341,   343,   345,   348,   353,   355,
     357,   359,   361,   363,   367,   369,   370,   374,   376,   380,
     384,   387,   391,   393,   395,   398,   402,   406,   410,   414,
     418,   422,   426,   430,   434,   438,   442,   446,   450,   454,
     458,   462,   466,   470,   474,   478,   482,   486,   489,   493,
     495,   497,   500,   504,   508,   512,   516,   520,   524,   528,
     532,   536,   540,   544,   548,   552,   556,   560,   564,   568,
     572,   576,   580,   584,   588
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     127,     0,    -1,   128,    -1,   127,   128,    -1,    10,   129,
     119,    -1,    11,     7,   119,    -1,    12,     3,   119,    -1,
      29,     6,   119,    -1,    63,   130,    -1,    30,     6,   119,
      -1,    31,     6,   119,    -1,    32,     6,   119,    -1,    25,
       7,   119,    -1,    26,     7,   119,    -1,    13,     6,   119,
      -1,    14,     6,   119,    -1,    22,     7,   119,    -1,    23,
       7,   119,    -1,    24,     7,   119,    -1,    17,     8,   119,
      -1,    18,     8,   119,    -1,    19,     8,   119,    -1,    20,
       8,   119,    -1,    27,   134,    -1,    41,     6,   119,    -1,
      42,     6,   119,    -1,    49,   157,   119,    -1,    50,   158,
     119,    -1,    59,   159,   119,    -1,    79,     6,   119,    -1,
      81,   164,   119,    -1,    82,     6,   119,    -1,    60,   160,
     119,    -1,    62,     6,   119,    -1,    33,   120,   139,   121,
      -1,    68,     6,   119,    -1,    69,     7,   119,    -1,    70,
       8,   119,    -1,    71,     6,   119,    -1,    72,     6,   119,
      -1,    73,     6,   119,    -1,    74,     7,   119,    -1,    75,
       7,   119,    -1,    76,     6,   119,    -1,    77,     6,   119,
      -1,    83,   120,   155,   121,    -1,    94,   165,    -1,    95,
     169,    -1,    51,    -1,    52,    -1,    53,    -1,    93,    -1,
       3,   131,    -1,   120,   132,   121,    -1,   119,    -1,   133,
      -1,   132,   133,    -1,    65,   129,   119,    -1,    66,     7,
     119,    -1,    67,     6,   119,    -1,     3,   120,   135,   121,
      -1,   136,    -1,   135,   136,    -1,    16,     4,   119,    -1,
      15,   137,   119,    -1,    36,     6,   119,    -1,    55,   152,
     119,    -1,    80,     6,   119,    -1,    78,     4,   122,   161,
     119,    -1,   138,   162,    -1,     4,   122,   161,    -1,   140,
     141,   144,    -1,    16,     4,   119,    -1,   142,    -1,   141,
     142,    -1,    15,   143,   119,    -1,     4,   122,   161,    -1,
     145,    -1,   144,   145,    -1,   146,   148,   150,   151,   119,
      -1,    34,   147,    -1,    43,    -1,    44,    -1,    45,    -1,
      46,    -1,    47,    -1,    48,    -1,    56,    -1,    57,    -1,
      -1,   149,    -1,   149,   149,    -1,   149,   149,   149,    -1,
      37,    -1,    38,    -1,    39,    -1,    -1,     7,    -1,     7,
       7,    -1,    -1,     6,    -1,   153,   154,    -1,    -1,     4,
      -1,     6,    -1,   156,    -1,   155,   156,    -1,     4,   153,
       6,   119,    -1,    54,    -1,     3,    -1,     3,    -1,     6,
      -1,     7,    -1,     4,   162,   159,    -1,     7,    -1,    -1,
     123,   163,   124,    -1,   164,    -1,   163,   125,   164,    -1,
       4,   122,   161,    -1,     3,   166,    -1,   120,   167,   121,
      -1,   119,    -1,   168,    -1,   167,   168,    -1,    97,     4,
     119,    -1,    98,     3,   119,    -1,    99,     4,   119,    -1,
     100,     3,   119,    -1,    84,     6,   119,    -1,    85,     6,
     119,    -1,    86,     4,   119,    -1,    87,     5,   119,    -1,
      88,     7,   119,    -1,    89,     7,   119,    -1,    90,     7,
     119,    -1,   105,     4,   119,    -1,   106,     4,   119,    -1,
     111,     6,   119,    -1,   112,     6,   119,    -1,   113,     7,
     119,    -1,   107,     7,   119,    -1,   108,     7,   119,    -1,
     109,     7,   119,    -1,   110,     7,   119,    -1,   101,     4,
     119,    -1,   102,     4,   119,    -1,     3,   170,    -1,   120,
     171,   121,    -1,   119,    -1,   172,    -1,   171,   172,    -1,
      97,     4,   119,    -1,    84,     6,   119,    -1,    85,     6,
     119,    -1,    86,     4,   119,    -1,    87,     5,   119,    -1,
      91,     6,   119,    -1,   105,     4,   119,    -1,   101,     4,
     119,    -1,   102,     4,   119,    -1,   103,     3,   119,    -1,
     104,     3,   119,    -1,   106,     4,   119,    -1,   111,     6,
     119,    -1,   112,     6,   119,    -1,   113,     7,   119,    -1,
     107,     7,   119,    -1,   108,     7,   119,    -1,   109,     7,
     119,    -1,   110,     7,   119,    -1,   114,     3,   119,    -1,
     115,     3,   119,    -1,   116,     3,   119,    -1,   117,     3,
     119,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   259,   259,   260,   263,   267,   271,   275,   279,   280,
     284,   288,   292,   296,   300,   304,   308,   317,   326,   335,
     339,   343,   347,   351,   352,   356,   360,   361,   362,   366,
     370,   375,   379,   383,   387,   388,   392,   396,   401,   405,
     409,   413,   417,   421,   425,   429,   430,   431,   434,   435,
     436,   437,   440,   483,   484,   487,   488,   491,   495,   506,
     512,   561,   562,   565,   569,   570,   574,   575,   580,   587,
     593,   600,   623,   629,   630,   633,   636,   652,   653,   656,
     727,   730,   731,   732,   733,   734,   735,   736,   737,   741,
     745,   746,   747,   750,   751,   752,   755,   756,   757,   760,
     761,   764,   779,   780,   783,   786,   787,   790,   806,   810,
     818,   829,   836,   839,   856,   866,   867,   870,   871,   874,
     890,   897,   898,   901,   902,   905,   909,   913,   917,   921,
     925,   929,   933,   937,   944,   951,   958,   962,   966,   970,
     974,   981,   988,   995,  1002,  1006,  1010,  1019,  1026,  1027,
    1030,  1031,  1034,  1038,  1042,  1046,  1050,  1054,  1058,  1062,
    1066,  1070,  1074,  1078,  1082,  1086,  1090,  1097,  1104,  1111,
    1118,  1122,  1126,  1130,  1134
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "QSTRING", "ADDR", "MACADDR", "BOOL",
  "NUMBER", "DECIMAL", "NUMPAIR", "MIP6ENTITY", "DEBUGLEVEL",
  "DEBUGLOGFILE", "DOROUTEOPTIMIZATIONCN", "DOROUTEOPTIMIZATIONMN",
  "HOMEADDRESS", "HOMEAGENTADDRESS", "INITIALBINDACKTIMEOUTFIRSTREG",
  "INITIALBINDACKTIMEOUTREREG", "INITIALSOLICITTIMER",
  "INTERFACEINITIALINITDELAY", "LINKNAME", "HAMAXBINDINGLIFE",
  "MNMAXHABINDINGLIFE", "MNMAXCNBINDINGLIFE", "MAXMOBPFXADVINTERVAL",
  "MINMOBPFXADVINTERVAL", "MNHOMELINK", "HAHOMELINK",
  "NONVOLATILEBINDINGCACHE", "SENDMOBPFXSOLS", "SENDUNSOLMOBPFXADVS",
  "SENDMOBPFXADVS", "IPSECPOLICYSET", "IPSECPOLICY", "IPSECTYPE",
  "USEALTCOA", "USEESP", "USEAH", "USEIPCOMP", "BLOCK", "USEMNHAIPSEC",
  "KEYMNGMOBCAPABILITY", "HOMEREGBINDING", "MH", "MOBPFXDISC",
  "TUNNELHOMETESTING", "TUNNELMH", "TUNNELPAYLOAD", "USEMOVEMENTMODULE",
  "USEPOLICYMODULE", "MIP6CN", "MIP6MN", "MIP6HA", "INTERNAL",
  "MNROPOLICY", "ICMP", "ANY", "DOROUTEOPT", "DEFAULTBINDINGACLPOLICY",
  "BINDINGACLPOLICY", "MNADDRESS", "USECNBUACK", "INTERFACE", "IFNAME",
  "IFTYPE", "MNIFPREFERENCE", "ISTUNNEL", "MNUSEALLINTERFACES",
  "MNROUTERPROBES", "MNROUTERPROBETIMEOUT", "MNDISCARDHAPARAMPROB",
  "MNRESETDHAADATHOME", "MNFLUSHALLATHOME", "MNMAXHACONSECUTIVERESENDS",
  "MNMAXCNCONSECUTIVERESENDS", "OPTIMISTICHANDOFF", "NOHOMERETURN",
  "HOMEPREFIX", "HAACCEPTMOBRTR", "ISMOBRTR", "HASERVEDPREFIX",
  "MOBRTRUSEEXPLICITMODE", "CNBINDINGPOLICYSET",
  "RFC5213TIMESTAMPBASEDAPPROACHINUSE",
  "RFC5213MOBILENODEGENERATEDTIMESTAMPINUSE",
  "RFC5213FIXEDMAGLINKLOCALADDRESSONALLACCESSLINKS",
  "RFC5213FIXEDMAGLINKLAYERADDRESSONALLACCESSLINKS",
  "RFC5213MINDELAYBEFOREBCEDELETE", "RFC5213MAXDELAYBEFORENEWBCEASSIGN",
  "RFC5213TIMESTAMPVALIDITYWINDOW", "RFC5213ENABLEMAGLOCALROUTING",
  "MIP6LMA", "MIP6MAG", "PROXYMIPLMA", "PROXYMIPMAG",
  "ALLLMAMULTICASTADDRESS", "LMAPMIPNETWORKADDRESS",
  "LMAPMIPNETWORKDEVICE", "LMACORENETWORKADDRESS", "LMACORENETWORKDEVICE",
  "MAGADDRESSINGRESS", "MAGADDRESSEGRESS", "MAGDEVICEINGRESS",
  "MAGDEVICEEGRESS", "OURADDRESS", "HOMENETWORKPREFIX", "PBULIFETIME",
  "PBALIFETIME", "RETRANSMISSIONTIMEOUT", "MAXMESSAGERETRANSMISSIONS",
  "TUNNELINGENABLED", "DYNAMICTUNNELINGENABLED",
  "MAXDELAYBEFOREDYNAMICTUNNELINGDELETE", "RADIUSPASSWORD",
  "RADIUSCLIENTCONFIGFILE", "PCAPSYSLOGASSOCIATIONGREPSTRING",
  "PCAPSYSLOGDEASSOCIATIONGREPSTRING", "INV_TOKEN", "';'", "'{'", "'}'",
  "'/'", "'('", "')'", "','", "$accept", "grammar", "topdef", "mip6entity",
  "ifacedef", "ifacesub", "ifaceopts", "ifaceopt", "linksub", "linkdefs",
  "linkdef", "homeaddress", "homeaddrdef", "ipsecpolicyset",
  "ipsechaaddrdef", "ipsecmnaddrdefs", "ipsecmnaddrdef", "ipsecmnaddr",
  "ipsecpolicydefs", "ipsecpolicydef", "ipsectype", "ipsectypeval",
  "ipsecprotos", "ipsecproto", "ipsecreqid", "xfrmaction", "mnropolicy",
  "mnropolicyaddr", "dorouteopt", "cnbindingpoldefs", "cnbindingpoldef",
  "movemodule", "policymodule", "bindaclpolval", "bindaclpolicy",
  "prefixlen", "prefixlistsub", "prefixlist", "prefixlistentry",
  "proxymiplmadef", "proxymiplmasub", "proxymiplmaopts", "proxymiplmaopt",
  "proxymipmagdef", "proxymipmagsub", "proxymipmagopts", "proxymipmagopt", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,    59,
     123,   125,    47,    40,    41,    44
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   126,   127,   127,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   129,   129,
     129,   129,   130,   131,   131,   132,   132,   133,   133,   133,
     134,   135,   135,   136,   136,   136,   136,   136,   136,   137,
     138,   139,   140,   141,   141,   142,   143,   144,   144,   145,
     146,   147,   147,   147,   147,   147,   147,   147,   147,   148,
     148,   148,   148,   149,   149,   149,   150,   150,   150,   151,
     151,   152,   153,   153,   154,   155,   155,   156,   157,   157,
     158,   159,   159,   160,   161,   162,   162,   163,   163,   164,
     165,   166,   166,   167,   167,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   169,   170,   170,
     171,   171,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     3,     3,     3,     3,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     4,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     4,     2,     2,     1,     1,
       1,     1,     2,     3,     1,     1,     2,     3,     3,     3,
       4,     1,     2,     3,     3,     3,     3,     3,     5,     2,
       3,     3,     3,     1,     2,     3,     3,     1,     2,     5,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     2,     3,     1,     1,     1,     0,     1,     2,     0,
       1,     2,     0,     1,     1,     1,     2,     4,     1,     1,
       1,     1,     1,     3,     1,     0,     3,     1,     3,     3,
       2,     3,     1,     1,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     3,     1,
       1,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     2,    48,    49,    50,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    23,     0,     0,     0,
       0,     0,     0,     0,   109,   108,     0,   110,     0,   111,
     112,     0,   115,     0,     0,     0,     8,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    46,     0,    47,     1,     3,     4,     5,
       6,    14,    15,    19,    20,    21,    22,    16,    17,    18,
      12,    13,     0,     7,     9,    10,    11,     0,     0,     0,
      24,    25,    26,    27,    28,     0,     0,    32,    33,    54,
       0,    52,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    29,     0,    30,    31,   102,     0,   105,   122,
       0,   120,   149,     0,   147,     0,     0,     0,   102,     0,
       0,     0,    61,     0,    34,     0,     0,    73,     0,   117,
     113,     0,     0,     0,     0,    55,   114,   119,   103,     0,
      45,   106,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   123,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     150,     0,     0,   115,     0,     0,     0,     0,     0,     0,
      60,    62,    72,     0,     0,     0,    74,    71,    77,    89,
     116,     0,     0,     0,     0,    53,    56,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     121,   124,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   148,   151,     0,    64,    69,
      63,    65,    66,   104,   101,     0,    67,     0,    75,    81,
      82,    83,    84,    85,    86,    87,    88,    80,    78,    93,
      94,    95,    96,    90,   118,    57,    58,    59,   107,   129,
     130,   131,   132,   133,   134,   135,   125,   126,   127,   128,
     145,   146,   136,   137,   141,   142,   143,   144,   138,   139,
     140,   153,   154,   155,   156,   157,   152,   159,   160,   161,
     162,   158,   163,   167,   168,   169,   170,   164,   165,   166,
     171,   172,   173,   174,    70,     0,    76,    97,    99,    91,
      68,    98,   100,     0,    92,    79
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    45,    46,    51,    86,   141,   184,   185,    66,   171,
     172,   242,   243,   128,   129,   176,   177,   254,   257,   258,
     259,   337,   342,   343,   398,   403,   246,   189,   324,   157,
     158,    76,    78,    81,    83,   187,   136,   178,    99,   103,
     161,   214,   215,   105,   164,   239,   240
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -336
static const yytype_int16 yypact[] =
{
     110,   -38,     9,     8,    31,    53,    58,    65,    71,    74,
      54,    78,    79,    80,    81,    96,    95,    99,   100,   103,
      -9,   104,   106,     4,   114,    12,   109,   108,   116,   119,
     124,   130,   144,   147,   148,   154,   155,   150,   157,   158,
     151,   159,    46,   164,   165,    21,  -336,  -336,  -336,  -336,
    -336,   -12,    52,    55,    56,    57,    69,    75,    82,    84,
      87,    88,    90,    91,    92,    70,  -336,   112,   121,   122,
     123,   161,   125,   126,  -336,  -336,   133,  -336,   134,  -336,
    -336,   153,    76,   156,   160,   -63,  -336,   186,   187,   188,
     189,   190,   191,   198,   199,   209,   210,   211,   107,   212,
     213,   196,   -51,  -336,   -44,  -336,  -336,  -336,  -336,  -336,
    -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,
    -336,  -336,    -6,  -336,  -336,  -336,  -336,   226,   142,   228,
    -336,  -336,  -336,  -336,  -336,   151,    12,  -336,  -336,  -336,
     -41,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,
    -336,  -336,  -336,   261,  -336,  -336,   265,    -3,  -336,  -336,
     214,  -336,  -336,   180,  -336,   269,   270,   272,   265,   276,
     327,   -13,  -336,   215,  -336,   331,     2,  -336,   -47,  -336,
    -336,   -38,   329,   332,   -61,  -336,  -336,  -336,  -336,   333,
    -336,  -336,   334,   335,   338,   271,   330,   336,   337,   341,
     343,   344,   346,   347,   348,   349,   350,   340,   351,   352,
     353,   355,   356,   357,   149,  -336,   359,   360,   363,   345,
     362,   365,   366,   367,   354,   369,   370,   371,   372,   373,
     374,   375,   377,   378,   379,   382,   384,   385,   386,   111,
    -336,   233,   237,    76,   244,   254,   257,   387,   255,   259,
    -336,  -336,  -336,   268,   273,   101,  -336,   361,  -336,   -10,
    -336,   151,   275,   277,   278,  -336,  -336,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
    -336,  -336,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   322,   323,   324,  -336,  -336,   261,  -336,  -336,
    -336,  -336,  -336,  -336,  -336,   261,  -336,   261,  -336,  -336,
    -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,
    -336,  -336,   437,   -10,  -336,  -336,  -336,  -336,  -336,  -336,
    -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,
    -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,
    -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,
    -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,
    -336,  -336,  -336,  -336,  -336,   326,  -336,   439,   441,   -10,
    -336,  -336,  -336,   339,  -336,  -336
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -336,  -336,   403,   274,  -336,  -336,  -336,   207,  -336,  -336,
     325,  -336,  -336,  -336,  -336,  -336,   328,  -336,  -336,   192,
    -336,  -336,  -336,  -335,  -336,  -336,  -336,   342,  -336,  -336,
     358,  -336,  -336,   364,  -336,  -305,   208,  -336,  -135,  -336,
    -336,  -336,   236,  -336,  -336,  -336,   217
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
     179,   156,   165,   166,   181,   182,   183,    74,   399,   165,
     166,    53,   394,    47,    48,    49,    52,   175,    79,    80,
     395,   106,   396,   167,   181,   182,   183,   339,   340,   341,
     167,     1,     2,     3,     4,     5,   255,    54,     6,     7,
       8,     9,   168,    10,    11,    12,    13,    14,    15,   168,
      16,    17,    18,    19,    20,    50,   139,   140,    75,    55,
     265,    60,    21,    22,   404,   169,    56,   170,   159,   160,
      23,    24,   169,    57,   170,   162,   163,   260,   261,    58,
      25,    26,    59,    27,    28,    61,    62,    63,    64,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    65,
      39,    67,    40,    41,    42,    68,    69,   108,   250,    70,
      72,    71,    73,    82,    84,    43,    44,    77,   190,    85,
       1,     2,     3,     4,     5,    87,   344,     6,     7,     8,
       9,    88,    10,    11,    12,    13,    14,    15,    89,    16,
      17,    18,    19,    20,   329,   330,   331,   332,   333,   334,
      90,    21,    22,    91,    92,    98,    95,   335,   336,    23,
      24,    93,    94,    96,    97,   100,   101,   102,   104,    25,
      26,   109,    27,    28,   110,   111,   112,   127,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,   113,    39,
     122,    40,    41,    42,   114,   216,   217,   218,   219,   135,
     156,   115,   220,   116,    43,    44,   117,   118,   221,   119,
     120,   121,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   153,
     173,   123,   315,   192,   193,   194,   195,   196,   197,   198,
     124,   125,   126,   175,   130,   131,   199,   200,   201,   202,
     203,   204,   132,   133,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   174,   216,   217,   218,   219,   186,   188,
     290,   220,   134,   241,   244,   137,   271,   221,   245,   138,
     248,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   192,   193,
     194,   195,   196,   197,   198,   142,   143,   144,   145,   146,
     147,   199,   200,   201,   202,   203,   204,   148,   149,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   150,   151,
     152,   154,   155,   249,   252,   253,   263,   272,   264,   267,
     268,   269,   270,   273,   274,   275,   276,   283,   277,   278,
     295,   279,   280,   281,   282,   317,   318,   300,   284,   285,
     286,   287,   288,   320,   289,   292,   293,   294,   296,   297,
     298,   299,   301,   321,   302,   303,   322,   325,   326,   304,
     305,   306,   307,   308,   309,   311,   310,   312,   313,   314,
     327,   266,   328,   323,   345,   255,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   397,   400,   401,   402,   107,   338,
     291,   319,     0,     0,     0,   262,   316,     0,   405,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   251,     0,     0,     0,
     180,     0,     0,     0,   256,     0,     0,     0,     0,     0,
     247,     0,     0,     0,     0,   191
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-336)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
     135,     4,    15,    16,    65,    66,    67,     3,   343,    15,
      16,     3,   317,    51,    52,    53,     7,    15,     6,     7,
     325,     0,   327,    36,    65,    66,    67,    37,    38,    39,
      36,    10,    11,    12,    13,    14,    34,     6,    17,    18,
      19,    20,    55,    22,    23,    24,    25,    26,    27,    55,
      29,    30,    31,    32,    33,    93,   119,   120,    54,     6,
     121,     7,    41,    42,   399,    78,     8,    80,   119,   120,
      49,    50,    78,     8,    80,   119,   120,   124,   125,     8,
      59,    60,     8,    62,    63,     7,     7,     7,     7,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,     3,
      79,     6,    81,    82,    83,     6,     6,   119,   121,     6,
       6,   120,     6,     4,     6,    94,    95,     3,   121,     3,
      10,    11,    12,    13,    14,     6,   261,    17,    18,    19,
      20,     7,    22,    23,    24,    25,    26,    27,     8,    29,
      30,    31,    32,    33,    43,    44,    45,    46,    47,    48,
       6,    41,    42,     6,     6,     4,     6,    56,    57,    49,
      50,     7,     7,     6,     6,     6,   120,     3,     3,    59,
      60,   119,    62,    63,   119,   119,   119,    16,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,   119,    79,
     120,    81,    82,    83,   119,    84,    85,    86,    87,   123,
       4,   119,    91,   119,    94,    95,   119,   119,    97,   119,
     119,   119,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   122,
       4,   119,   121,    84,    85,    86,    87,    88,    89,    90,
     119,   119,   119,    15,   119,   119,    97,    98,    99,   100,
     101,   102,   119,   119,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   121,    84,    85,    86,    87,     7,     4,
     121,    91,   119,     4,     4,   119,     5,    97,     6,   119,
       4,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    84,    85,
      86,    87,    88,    89,    90,   119,   119,   119,   119,   119,
     119,    97,    98,    99,   100,   101,   102,   119,   119,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   119,   119,
     119,   119,   119,     6,   119,     4,     7,     7,     6,     6,
       6,     6,     4,     7,     7,     4,     3,     7,     4,     3,
       5,     4,     4,     4,     4,   122,   119,     3,     7,     7,
       7,     6,     6,   119,     7,     6,     6,     4,     6,     4,
       4,     4,     3,   119,     4,     4,   119,   122,   119,     7,
       7,     7,     7,     6,     6,     3,     7,     3,     3,     3,
     122,   184,   119,     6,   119,    34,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,     7,   119,     7,     6,    45,   257,
     214,   243,    -1,    -1,    -1,   181,   239,    -1,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
     136,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,
     168,    -1,    -1,    -1,    -1,   157
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    10,    11,    12,    13,    14,    17,    18,    19,    20,
      22,    23,    24,    25,    26,    27,    29,    30,    31,    32,
      33,    41,    42,    49,    50,    59,    60,    62,    63,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    79,
      81,    82,    83,    94,    95,   127,   128,    51,    52,    53,
      93,   129,     7,     3,     6,     6,     8,     8,     8,     8,
       7,     7,     7,     7,     7,     3,   134,     6,     6,     6,
       6,   120,     6,     6,     3,    54,   157,     3,   158,     6,
       7,   159,     4,   160,     6,     3,   130,     6,     7,     8,
       6,     6,     6,     7,     7,     6,     6,     6,     4,   164,
       6,   120,     3,   165,     3,   169,     0,   128,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   120,   119,   119,   119,   119,    16,   139,   140,
     119,   119,   119,   119,   119,   123,   162,   119,   119,   119,
     120,   131,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   122,   119,   119,     4,   155,   156,   119,
     120,   166,   119,   120,   170,    15,    16,    36,    55,    78,
      80,   135,   136,     4,   121,    15,   141,   142,   163,   164,
     159,    65,    66,    67,   132,   133,     7,   161,     4,   153,
     121,   156,    84,    85,    86,    87,    88,    89,    90,    97,
      98,    99,   100,   101,   102,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   167,   168,    84,    85,    86,    87,
      91,    97,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   171,
     172,     4,   137,   138,     4,     6,   152,   153,     4,     6,
     121,   136,   119,     4,   143,    34,   142,   144,   145,   146,
     124,   125,   129,     7,     6,   121,   133,     6,     6,     6,
       4,     5,     7,     7,     7,     4,     3,     4,     3,     4,
       4,     4,     4,     7,     7,     7,     7,     6,     6,     7,
     121,   168,     6,     6,     4,     5,     6,     4,     4,     4,
       3,     3,     4,     4,     7,     7,     7,     7,     6,     6,
       7,     3,     3,     3,     3,   121,   172,   122,   119,   162,
     119,   119,   119,     6,   154,   122,   119,   122,   119,    43,
      44,    45,    46,    47,    48,    56,    57,   147,   145,    37,
      38,    39,   148,   149,   164,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   161,   161,   161,     7,   150,   149,
     119,     7,     6,   151,   149,   119
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YYUSE (yytype);
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
/* Line 1787 of yacc.c  */
#line 264 "gram.y"
    {
			conf_parsed->mip6_entity = (yyvsp[(2) - (3)].num);
		}
    break;

  case 5:
/* Line 1787 of yacc.c  */
#line 268 "gram.y"
    {
			conf_parsed->debug_level = (yyvsp[(2) - (3)].num);
		}
    break;

  case 6:
/* Line 1787 of yacc.c  */
#line 272 "gram.y"
    {
			conf_parsed->debug_log_file = (yyvsp[(2) - (3)].string);
		}
    break;

  case 7:
/* Line 1787 of yacc.c  */
#line 276 "gram.y"
    {
			conf_parsed->NonVolatileBindingCache = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 9:
/* Line 1787 of yacc.c  */
#line 281 "gram.y"
    {
			conf_parsed->SendMobPfxSols = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 10:
/* Line 1787 of yacc.c  */
#line 285 "gram.y"
    {
			conf_parsed->SendUnsolMobPfxAdvs = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 11:
/* Line 1787 of yacc.c  */
#line 289 "gram.y"
    {
			conf_parsed->SendMobPfxAdvs = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 12:
/* Line 1787 of yacc.c  */
#line 293 "gram.y"
    {
			conf_parsed->MaxMobPfxAdvInterval = (yyvsp[(2) - (3)].num);
		}
    break;

  case 13:
/* Line 1787 of yacc.c  */
#line 297 "gram.y"
    {
			conf_parsed->MinMobPfxAdvInterval = (yyvsp[(2) - (3)].num);
		}
    break;

  case 14:
/* Line 1787 of yacc.c  */
#line 301 "gram.y"
    {
			conf_parsed->DoRouteOptimizationCN = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 15:
/* Line 1787 of yacc.c  */
#line 305 "gram.y"
    {
			conf_parsed->DoRouteOptimizationMN = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 16:
/* Line 1787 of yacc.c  */
#line 309 "gram.y"
    {
			if ((yyvsp[(2) - (3)].num) > MAX_BINDING_LIFETIME) {
				uerror("max allowed binding lifetime is %d", 
				       MAX_BINDING_LIFETIME);
				return -1;
			}
			conf_parsed->HaMaxBindingLife = (yyvsp[(2) - (3)].num);
		}
    break;

  case 17:
/* Line 1787 of yacc.c  */
#line 318 "gram.y"
    {
			if ((yyvsp[(2) - (3)].num) > MAX_BINDING_LIFETIME) {
				uerror("max allowed binding lifetime is %d", 
				       MAX_BINDING_LIFETIME);
				return -1;
			}
			conf_parsed->MnMaxHaBindingLife = (yyvsp[(2) - (3)].num);
		}
    break;

  case 18:
/* Line 1787 of yacc.c  */
#line 327 "gram.y"
    {
			if ((yyvsp[(2) - (3)].num) > MAX_RR_BINDING_LIFETIME) {
				uerror("max allowed binding lifetime is %d", 
				       MAX_RR_BINDING_LIFETIME);
				return -1;
			}
			conf_parsed->MnMaxCnBindingLife = (yyvsp[(2) - (3)].num);
		}
    break;

  case 19:
/* Line 1787 of yacc.c  */
#line 336 "gram.y"
    {
			tssetdsec(conf_parsed->InitialBindackTimeoutFirstReg_ts, (yyvsp[(2) - (3)].dec));
		}
    break;

  case 20:
/* Line 1787 of yacc.c  */
#line 340 "gram.y"
    {
			tssetdsec(conf_parsed->InitialBindackTimeoutReReg_ts, (yyvsp[(2) - (3)].dec));
		}
    break;

  case 21:
/* Line 1787 of yacc.c  */
#line 344 "gram.y"
    {
			tssetdsec(conf_parsed->InitialSolicitTimer_ts, (yyvsp[(2) - (3)].dec));
		}
    break;

  case 22:
/* Line 1787 of yacc.c  */
#line 348 "gram.y"
    {
			tssetdsec(conf_parsed->InterfaceInitialInitDelay_ts, (yyvsp[(2) - (3)].dec));
		}
    break;

  case 24:
/* Line 1787 of yacc.c  */
#line 353 "gram.y"
    {
			conf_parsed->UseMnHaIPsec = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 25:
/* Line 1787 of yacc.c  */
#line 357 "gram.y"
    {
			conf_parsed->KeyMngMobCapability = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 28:
/* Line 1787 of yacc.c  */
#line 363 "gram.y"
    {
			conf_parsed->DefaultBindingAclPolicy = (yyvsp[(2) - (3)].num);
		}
    break;

  case 29:
/* Line 1787 of yacc.c  */
#line 367 "gram.y"
    {
			conf_parsed->HaAcceptMobRtr = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 30:
/* Line 1787 of yacc.c  */
#line 371 "gram.y"
    {
			list_splice(&prefixes,
				    conf_parsed->nemo_ha_served_prefixes.prev);
		}
    break;

  case 31:
/* Line 1787 of yacc.c  */
#line 376 "gram.y"
    {
			conf_parsed->MobRtrUseExplicitMode = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 32:
/* Line 1787 of yacc.c  */
#line 380 "gram.y"
    {
			bae = NULL;
		}
    break;

  case 33:
/* Line 1787 of yacc.c  */
#line 384 "gram.y"
    {
			conf_parsed->CnBuAck = (yyvsp[(2) - (3)].bool) ? IP6_MH_BU_ACK : 0;
		}
    break;

  case 35:
/* Line 1787 of yacc.c  */
#line 389 "gram.y"
    {
			conf_parsed->MnUseAllInterfaces = (yyvsp[(2) - (3)].bool) ? POL_MN_IF_DEF_PREFERENCE : 0;
		}
    break;

  case 36:
/* Line 1787 of yacc.c  */
#line 393 "gram.y"
    {
			conf_parsed->MnRouterProbes = (yyvsp[(2) - (3)].num);
		}
    break;

  case 37:
/* Line 1787 of yacc.c  */
#line 397 "gram.y"
    {
			if ((yyvsp[(2) - (3)].dec) > 0)
				tssetdsec(conf_parsed->MnRouterProbeTimeout_ts, (yyvsp[(2) - (3)].dec));
		}
    break;

  case 38:
/* Line 1787 of yacc.c  */
#line 402 "gram.y"
    {
			conf_parsed->MnDiscardHaParamProb = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 39:
/* Line 1787 of yacc.c  */
#line 406 "gram.y"
    {
			conf_parsed->MnResetDhaadAtHome = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 40:
/* Line 1787 of yacc.c  */
#line 410 "gram.y"
    {
			conf_parsed->MnFlushAllAtHome = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 41:
/* Line 1787 of yacc.c  */
#line 414 "gram.y"
    {
			conf_parsed->MnMaxHaConsecutiveResends = (yyvsp[(2) - (3)].num);
		}
    break;

  case 42:
/* Line 1787 of yacc.c  */
#line 418 "gram.y"
    {
			conf_parsed->MnMaxCnConsecutiveResends = (yyvsp[(2) - (3)].num);
		}
    break;

  case 43:
/* Line 1787 of yacc.c  */
#line 422 "gram.y"
    {
			conf_parsed->OptimisticHandoff = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 44:
/* Line 1787 of yacc.c  */
#line 426 "gram.y"
    {
			conf_parsed->NoHomeReturn = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 48:
/* Line 1787 of yacc.c  */
#line 434 "gram.y"
    { (yyval.num) = MIP6_ENTITY_CN;	}
    break;

  case 49:
/* Line 1787 of yacc.c  */
#line 435 "gram.y"
    { (yyval.num) = MIP6_ENTITY_MN; }
    break;

  case 50:
/* Line 1787 of yacc.c  */
#line 436 "gram.y"
    { (yyval.num) = MIP6_ENTITY_HA; }
    break;

  case 51:
/* Line 1787 of yacc.c  */
#line 437 "gram.y"
    { (yyval.num) = MIP6_ENTITY_MAG; }
    break;

  case 52:
/* Line 1787 of yacc.c  */
#line 441 "gram.y"
    {
			struct net_iface *nni;
			strncpy(ni.name, (yyvsp[(1) - (2)].string), IF_NAMESIZE - 1);
			ni.ifindex = if_nametoindex((yyvsp[(1) - (2)].string));

			if (is_if_ha(&ni) && ni.is_tunnel) {
				/* We do not allow tunnel interfaces
				   for HA, only for MN and CN */
				uerror("Use of tunnel interface is not"
				       " possible for HA yet");
				free((yyvsp[(1) - (2)].string));
				return -1;
			}
			if (ni.ifindex <= 0) {
				if (is_if_ha(&ni)) {
					/* We do not allow unavailable
					   ifaces for HA ... */
					uerror("HA interface %s "
					       "unavailable", (yyvsp[(1) - (2)].string));
					free((yyvsp[(1) - (2)].string));
					return -1;
				}
				/* ... but allow them for CN and MN */
			}
			free((yyvsp[(1) - (2)].string));

			nni = malloc(sizeof(struct net_iface));
			if (nni == NULL) {
				uerror("out of memory");
				return -1;
			}
			memcpy(nni, &ni, sizeof(struct net_iface));
			list_add_tail(&nni->list, &conf_parsed->net_ifaces);
			if (is_if_ha(nni))
				homeagent_if_init(nni->ifindex);

			memset(&ni, 0, sizeof(struct net_iface));
			ni.mip6_if_entity = MIP6_ENTITY_NO;
			ni.mn_if_preference = POL_MN_IF_DEF_PREFERENCE;
		}
    break;

  case 57:
/* Line 1787 of yacc.c  */
#line 492 "gram.y"
    {
			ni.mip6_if_entity = (yyvsp[(2) - (3)].num);
		}
    break;

  case 58:
/* Line 1787 of yacc.c  */
#line 496 "gram.y"
    {
			int pref = (yyvsp[(2) - (3)].num);
			if ((pref > POL_MN_IF_MIN_PREFERENCE) || (pref < 0)) {
				uerror("Found bad interface preference value "
				       "(%d). Valid range is [0,%d].\n", pref,
				       POL_MN_IF_MIN_PREFERENCE);
				return -1;
			}
 			ni.mn_if_preference = pref;
		}
    break;

  case 59:
/* Line 1787 of yacc.c  */
#line 507 "gram.y"
    {
			ni.is_tunnel = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 60:
/* Line 1787 of yacc.c  */
#line 513 "gram.y"
    {
			struct home_addr_info *nhai;
			if (IN6_IS_ADDR_UNSPECIFIED(&hai.hoa.addr)) {
				uerror("No home addresses defined"
					"for homelink %d", hai.if_home);
				free((yyvsp[(1) - (4)].string));
				return -1;
			}
			strncpy(hai.name, (yyvsp[(1) - (4)].string), IF_NAMESIZE - 1);
			hai.if_home = if_nametoindex((yyvsp[(1) - (4)].string));
			free((yyvsp[(1) - (4)].string));
			if (hai.if_home <= 0) {
				uerror("invalid interface");
				return -1;
			}
			nhai = malloc(sizeof(struct home_addr_info));
			if (nhai == NULL) {
				uerror("out of memory");
				return -1;
			}
			if (hai.plen == 64) {
				struct in6_addr lladdr;
				ipv6_addr_llocal(&hai.hoa.addr, &lladdr);
				if (!addr_do(&lladdr, 64, 
					     hai.if_home, NULL, NULL))
					hai.lladdr_comp = IP6_MH_BU_LLOCAL;
			}
			if (IN6_IS_ADDR_UNSPECIFIED(&hai.home_prefix)) {
				ipv6_addr_prefix(&hai.home_prefix, 
						 &hai.hoa.addr, hai.plen);
				hai.home_plen = hai.plen;
			}
			memcpy(nhai, &hai, sizeof(struct home_addr_info));
			INIT_LIST_HEAD(&nhai->ro_policies);
			INIT_LIST_HEAD(&nhai->ha_list.home_agents);
			INIT_LIST_HEAD(&nhai->mob_net_prefixes);
			nhai->ha_list.dhaad_id = -1;
			list_splice(&hai.ro_policies, &nhai->ro_policies);
			list_splice(&hai.mob_net_prefixes,
				    &nhai->mob_net_prefixes);
			list_add_tail(&nhai->list, &conf_parsed->home_addrs);

			memset(&hai, 0, sizeof(struct home_addr_info));
			INIT_LIST_HEAD(&hai.ro_policies);
			INIT_LIST_HEAD(&hai.mob_net_prefixes);
		}
    break;

  case 63:
/* Line 1787 of yacc.c  */
#line 566 "gram.y"
    {
			memcpy(&hai.ha_addr, &(yyvsp[(2) - (3)].addr), sizeof(struct in6_addr));
		}
    break;

  case 65:
/* Line 1787 of yacc.c  */
#line 571 "gram.y"
    {
		        hai.altcoa = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 67:
/* Line 1787 of yacc.c  */
#line 576 "gram.y"
    {
			if ((yyvsp[(2) - (3)].bool))
				hai.mob_rtr = IP6_MH_BU_MR;
		}
    break;

  case 68:
/* Line 1787 of yacc.c  */
#line 581 "gram.y"
    {
			ipv6_addr_prefix(&hai.home_prefix, &(yyvsp[(2) - (5)].addr), (yyvsp[(4) - (5)].num));
			hai.home_plen = (yyvsp[(4) - (5)].num);
		}
    break;

  case 69:
/* Line 1787 of yacc.c  */
#line 588 "gram.y"
    {
			hai.mnp_count = mv_prefixes(&hai.mob_net_prefixes);
		}
    break;

  case 70:
/* Line 1787 of yacc.c  */
#line 594 "gram.y"
    {
			hai.hoa.addr = (yyvsp[(1) - (3)].addr);
			hai.plen = (yyvsp[(3) - (3)].num);
		}
    break;

  case 71:
/* Line 1787 of yacc.c  */
#line 601 "gram.y"
    {
			if (!list_empty(&ipsec_ps.hoa_list)) {
				struct list_head *lp, *tmp;

				/* free each hoa entry */
				list_for_each_safe(lp, tmp,
						   &ipsec_ps.hoa_list) {
					struct home_addr_info *hoa;

					list_del(lp);
					hoa = list_entry(lp,
							 struct home_addr_info,
							 list);

					free(hoa);
				}
			}
			memset(&ipsec_ps, 0, sizeof(ipsec_ps));
			INIT_LIST_HEAD(&ipsec_ps.hoa_list);
		}
    break;

  case 72:
/* Line 1787 of yacc.c  */
#line 624 "gram.y"
    {
			ipsec_ps.ha = (yyvsp[(2) - (3)].addr);
		}
    break;

  case 76:
/* Line 1787 of yacc.c  */
#line 637 "gram.y"
    {
			struct home_addr_info *hai;

			hai = malloc(sizeof(struct home_addr_info));
			if (hai == NULL) {
				uerror("out of memory");
				return -1;
			}
			memset(hai, 0, sizeof(struct home_addr_info)); 
			hai->hoa.addr = (yyvsp[(1) - (3)].addr);
			hai->plen = (yyvsp[(3) - (3)].num);
			list_add_tail(&hai->list, &ipsec_ps.hoa_list);
		}
    break;

  case 79:
/* Line 1787 of yacc.c  */
#line 657 "gram.y"
    {
			struct list_head *lp;

			if (IN6_IS_ADDR_UNSPECIFIED(&ipsec_ps.ha)) {
				uerror("HomeAgentAddress missing for IPsecPolicy");
				return -1;
			}
			if (list_empty(&ipsec_ps.hoa_list)) {
				uerror("HomeAddress missing for IPsecPolicy");
				return -1;
			}

			list_for_each(lp, &ipsec_ps.hoa_list) {
				struct home_addr_info *hai;
				struct ipsec_policy_entry *e;

				hai = list_entry(lp, struct home_addr_info,
						 list);

				e = malloc(sizeof(*e));
				if (e == NULL) {
					uerror("out of memory");
					return -1;
				}
				memset(e, 0, sizeof(*e));
				e->ha_addr = ipsec_ps.ha;
				e->mn_addr = hai->hoa.addr;
				e->type = (yyvsp[(1) - (5)].num);
#ifndef XFRM_MSG_MIGRATE
				switch (e->type) {
				case IPSEC_POLICY_TYPE_TUNNELHOMETESTING:
				case IPSEC_POLICY_TYPE_TUNNELMH:
				case IPSEC_POLICY_TYPE_TUNNELPAYLOAD:
					uerror("cannot use IPsec tunnel because it is not built with MIGRATE");
					free(e);
					return -1;
				default:
					break;
				}
#endif
#ifndef MULTIPROTO_MIGRATE
				if ((yyvsp[(2) - (5)].num) != IPSEC_PROTO_ESP) {
					uerror("only UseESP is allowed");
					free(e);
					return -1;
				}
#endif
				e->ipsec_protos = (yyvsp[(2) - (5)].num);
				e->reqid_toha = (yyvsp[(3) - (5)].numpair)[0];
				e->reqid_tomn = (yyvsp[(3) - (5)].numpair)[1];
				e->action = (yyvsp[(4) - (5)].bool);

				if (ipsec_policy_entry_check(&e->ha_addr,
							     &e->mn_addr,
							     e->type)) {
					uerror("overlapping IPsec policies "
					       "found for "
					       "HA %x:%x:%x:%x:%x:%x:%x:%x "
					       "MN %x:%x:%x:%x:%x:%x:%x:%x "
					       "pair\n",
					       NIP6ADDR(&e->ha_addr),
					       NIP6ADDR(&e->mn_addr));
					free(e);
					return -1;
				}
				list_add_tail(&e->list, &conf_parsed->ipsec_policies);
			}
		}
    break;

  case 80:
/* Line 1787 of yacc.c  */
#line 727 "gram.y"
    { (yyval.num) = (yyvsp[(2) - (2)].num); }
    break;

  case 81:
/* Line 1787 of yacc.c  */
#line 730 "gram.y"
    { (yyval.num) = IPSEC_POLICY_TYPE_HOMEREGBINDING; }
    break;

  case 82:
/* Line 1787 of yacc.c  */
#line 731 "gram.y"
    { (yyval.num) = IPSEC_POLICY_TYPE_MH; }
    break;

  case 83:
/* Line 1787 of yacc.c  */
#line 732 "gram.y"
    { (yyval.num) = IPSEC_POLICY_TYPE_MOBPFXDISC; }
    break;

  case 84:
/* Line 1787 of yacc.c  */
#line 733 "gram.y"
    { (yyval.num) = IPSEC_POLICY_TYPE_TUNNELHOMETESTING; }
    break;

  case 85:
/* Line 1787 of yacc.c  */
#line 734 "gram.y"
    { (yyval.num) = IPSEC_POLICY_TYPE_TUNNELMH; }
    break;

  case 86:
/* Line 1787 of yacc.c  */
#line 735 "gram.y"
    { (yyval.num) = IPSEC_POLICY_TYPE_TUNNELPAYLOAD; }
    break;

  case 87:
/* Line 1787 of yacc.c  */
#line 736 "gram.y"
    { (yyval.num) = IPSEC_POLICY_TYPE_ICMP; }
    break;

  case 88:
/* Line 1787 of yacc.c  */
#line 737 "gram.y"
    { (yyval.num) = IPSEC_POLICY_TYPE_ANY; }
    break;

  case 89:
/* Line 1787 of yacc.c  */
#line 741 "gram.y"
    {
			uerror("IPsecPolicy must set at least one protocol");
			return -1;
		}
    break;

  case 90:
/* Line 1787 of yacc.c  */
#line 745 "gram.y"
    { (yyval.num) = (yyvsp[(1) - (1)].num); }
    break;

  case 91:
/* Line 1787 of yacc.c  */
#line 746 "gram.y"
    { (yyval.num) = (yyvsp[(1) - (2)].num) | (yyvsp[(2) - (2)].num); }
    break;

  case 92:
/* Line 1787 of yacc.c  */
#line 747 "gram.y"
    { (yyval.num) = (yyvsp[(1) - (3)].num) | (yyvsp[(2) - (3)].num) | (yyvsp[(3) - (3)].num); }
    break;

  case 93:
/* Line 1787 of yacc.c  */
#line 750 "gram.y"
    { (yyval.num) = IPSEC_PROTO_ESP; }
    break;

  case 94:
/* Line 1787 of yacc.c  */
#line 751 "gram.y"
    { (yyval.num) = IPSEC_PROTO_AH; }
    break;

  case 95:
/* Line 1787 of yacc.c  */
#line 752 "gram.y"
    { (yyval.num) = IPSEC_PROTO_IPCOMP; }
    break;

  case 96:
/* Line 1787 of yacc.c  */
#line 755 "gram.y"
    { (yyval.numpair)[0] = (yyval.numpair)[1] = 0; }
    break;

  case 97:
/* Line 1787 of yacc.c  */
#line 756 "gram.y"
    { (yyval.numpair)[0] = (yyval.numpair)[1] = (yyvsp[(1) - (1)].num); }
    break;

  case 98:
/* Line 1787 of yacc.c  */
#line 757 "gram.y"
    { (yyval.numpair)[0] = (yyvsp[(1) - (2)].num); (yyval.numpair)[1] = (yyvsp[(2) - (2)].num); }
    break;

  case 99:
/* Line 1787 of yacc.c  */
#line 760 "gram.y"
    { (yyval.bool) = XFRM_POLICY_ALLOW; }
    break;

  case 100:
/* Line 1787 of yacc.c  */
#line 761 "gram.y"
    { (yyval.bool) = (yyvsp[(1) - (1)].bool) ? XFRM_POLICY_ALLOW : XFRM_POLICY_BLOCK; }
    break;

  case 101:
/* Line 1787 of yacc.c  */
#line 765 "gram.y"
    { 
			struct xfrm_ro_pol *rp;
			rp = malloc(sizeof(struct xfrm_ro_pol));
			if (rp == NULL) {
				uerror("out of memory");
				return -1;
			}
			memset(rp, 0, sizeof(struct xfrm_ro_pol)); 
			rp->cn_addr = (yyvsp[(1) - (2)].addr);
			rp->do_ro = (yyvsp[(2) - (2)].bool);
			list_add_tail(&rp->list, &hai.ro_policies);
		}
    break;

  case 102:
/* Line 1787 of yacc.c  */
#line 779 "gram.y"
    { (yyval.addr) = in6addr_any; }
    break;

  case 103:
/* Line 1787 of yacc.c  */
#line 780 "gram.y"
    { (yyval.addr) = (yyvsp[(1) - (1)].addr); }
    break;

  case 104:
/* Line 1787 of yacc.c  */
#line 783 "gram.y"
    { (yyval.bool) = (yyvsp[(1) - (1)].bool); }
    break;

  case 107:
/* Line 1787 of yacc.c  */
#line 791 "gram.y"
    {
			cnbpol = malloc(sizeof(struct cn_binding_pol_entry));
			if (cnbpol == NULL) {
				uerror("out of memory");
				return -1;
			}
			memset(cnbpol, 0, sizeof(struct cn_binding_pol_entry));
			cnbpol->remote_hoa = (yyvsp[(1) - (4)].addr);
			cnbpol->local_addr = (yyvsp[(2) - (4)].addr);
			cnbpol->bind_policy = (yyvsp[(3) - (4)].bool);
			list_add_tail(&cnbpol->list,
				      &conf_parsed->cn_binding_pol);
		}
    break;

  case 108:
/* Line 1787 of yacc.c  */
#line 807 "gram.y"
    {
			conf_parsed->MoveModulePath = NULL;
		}
    break;

  case 109:
/* Line 1787 of yacc.c  */
#line 811 "gram.y"
    {
			/* Unused at the moment */
			free((yyvsp[(1) - (1)].string));
			conf_parsed->MoveModulePath = NULL;
		}
    break;

  case 110:
/* Line 1787 of yacc.c  */
#line 819 "gram.y"
    {
			if (pmgr_init((yyvsp[(1) - (1)].string), &conf_parsed->pmgr) < 0) {
				uerror("error loading shared object %s", (yyvsp[(1) - (1)].string));
				free((yyvsp[(1) - (1)].string));
				return -1;
			}
			free((yyvsp[(1) - (1)].string));
		}
    break;

  case 111:
/* Line 1787 of yacc.c  */
#line 830 "gram.y"
    { 
			if ((yyvsp[(1) - (1)].bool))
				(yyval.num) = IP6_MH_BAS_ACCEPTED;
			else
				(yyval.num) = IP6_MH_BAS_PROHIBIT;
		}
    break;

  case 112:
/* Line 1787 of yacc.c  */
#line 836 "gram.y"
    { (yyval.num) = (yyvsp[(1) - (1)].num); }
    break;

  case 113:
/* Line 1787 of yacc.c  */
#line 840 "gram.y"
    {
			bae = malloc(sizeof(struct policy_bind_acl_entry));
			if (bae == NULL) {
				uerror("out of memory");
				return -1;
			}
			memset(bae, 0, sizeof(struct policy_bind_acl_entry)); 
			bae->hoa = (yyvsp[(1) - (3)].addr);
			bae->plen = 128;
			INIT_LIST_HEAD(&bae->mob_net_prefixes);
			bae->mnp_count = mv_prefixes(&bae->mob_net_prefixes);
			bae->bind_policy = (yyvsp[(3) - (3)].num);
			list_add_tail(&bae->list, &conf_parsed->bind_acl);
		}
    break;

  case 114:
/* Line 1787 of yacc.c  */
#line 857 "gram.y"
    {
			if ((yyvsp[(1) - (1)].num) > 128) {
				uerror("invalid prefix length %d", (yyvsp[(1) - (1)].num));
				return -1;
			}
			(yyval.num) = (yyvsp[(1) - (1)].num);
		}
    break;

  case 119:
/* Line 1787 of yacc.c  */
#line 875 "gram.y"
    {
			struct prefix_list_entry *p;
			p = malloc(sizeof(struct prefix_list_entry));
			if (p == NULL) {
				fprintf(stderr,
					"%s: out of memory\n", __FUNCTION__);
				return -1;
			}
			memset(p, 0, sizeof(struct prefix_list_entry));
			p->ple_prefix = (yyvsp[(1) - (3)].addr);
			p->ple_plen = (yyvsp[(3) - (3)].num);
			list_add_tail(&p->list, &prefixes);
		}
    break;

  case 120:
/* Line 1787 of yacc.c  */
#line 891 "gram.y"
    {
			conf_parsed->HomeNetworkPrefix = in6addr_any;
			conf_parsed->OurAddress        = in6addr_loopback;
		}
    break;

  case 125:
/* Line 1787 of yacc.c  */
#line 906 "gram.y"
    {
			memcpy(&conf_parsed->LmaAddress, &(yyvsp[(2) - (3)].addr), sizeof(struct in6_addr));
		}
    break;

  case 126:
/* Line 1787 of yacc.c  */
#line 910 "gram.y"
    {
			conf_parsed->LmaPmipNetworkDevice = (yyvsp[(2) - (3)].string);
		}
    break;

  case 127:
/* Line 1787 of yacc.c  */
#line 914 "gram.y"
    {
			memcpy(&conf_parsed->LmaCoreNetworkAddress, &(yyvsp[(2) - (3)].addr), sizeof(struct in6_addr));
		}
    break;

  case 128:
/* Line 1787 of yacc.c  */
#line 918 "gram.y"
    {
			conf_parsed->LmaCoreNetworkDevice = (yyvsp[(2) - (3)].string);
		}
    break;

  case 129:
/* Line 1787 of yacc.c  */
#line 922 "gram.y"
    {
			conf_parsed->RFC5213TimestampBasedApproachInUse = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 130:
/* Line 1787 of yacc.c  */
#line 926 "gram.y"
    {
			conf_parsed->RFC5213MobileNodeGeneratedTimestampInUse = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 131:
/* Line 1787 of yacc.c  */
#line 930 "gram.y"
    {
			memcpy(&conf_parsed->RFC5213FixedMAGLinkLocalAddressOnAllAccessLinks, &(yyvsp[(2) - (3)].addr), sizeof(struct in6_addr));
		}
    break;

  case 132:
/* Line 1787 of yacc.c  */
#line 934 "gram.y"
    {
			memcpy(&conf_parsed->RFC5213FixedMAGLinkLayerAddressOnAllAccessLinks, &(yyvsp[(2) - (3)].macaddr), sizeof(struct in6_addr));
		}
    break;

  case 133:
/* Line 1787 of yacc.c  */
#line 938 "gram.y"
    {
			struct timespec lifetime;
			lifetime.tv_sec = (yyvsp[(2) - (3)].num)/1000;
			lifetime.tv_nsec = ((yyvsp[(2) - (3)].num) % 1000)*1000000;
			conf_parsed->RFC5213MinDelayBeforeBCEDelete = lifetime;
		}
    break;

  case 134:
/* Line 1787 of yacc.c  */
#line 945 "gram.y"
    {
			struct timespec lifetime;
			lifetime.tv_sec = (yyvsp[(2) - (3)].num)/1000;
			lifetime.tv_nsec = ((yyvsp[(2) - (3)].num) % 1000)*1000000;
			conf_parsed->RFC5213MaxDelayBeforeNewBCEAssign = lifetime;
		}
    break;

  case 135:
/* Line 1787 of yacc.c  */
#line 952 "gram.y"
    {
			struct timespec lifetime;
			lifetime.tv_sec = (yyvsp[(2) - (3)].num)/1000;
			lifetime.tv_nsec = ((yyvsp[(2) - (3)].num) % 1000)*1000000;
			conf_parsed->RFC5213TimestampValidityWindow = lifetime;
		}
    break;

  case 136:
/* Line 1787 of yacc.c  */
#line 959 "gram.y"
    {
			memcpy(&conf_parsed->OurAddress, &(yyvsp[(2) - (3)].addr), sizeof(struct in6_addr));
		}
    break;

  case 137:
/* Line 1787 of yacc.c  */
#line 963 "gram.y"
    {
			memcpy(&conf_parsed->HomeNetworkPrefix, &(yyvsp[(2) - (3)].addr), sizeof(struct in6_addr));
		}
    break;

  case 138:
/* Line 1787 of yacc.c  */
#line 967 "gram.y"
    {
			conf_parsed->TunnelingEnabled = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 139:
/* Line 1787 of yacc.c  */
#line 971 "gram.y"
    {
			conf_parsed->DynamicTunnelingEnabled = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 140:
/* Line 1787 of yacc.c  */
#line 975 "gram.y"
    {
			struct timespec lifetime;
			lifetime.tv_sec = (yyvsp[(2) - (3)].num)/1000;
			lifetime.tv_nsec = ((yyvsp[(2) - (3)].num) % 1000)*1000000;
			conf_parsed->MaxDelayBeforeDynamicTunnelingDelete = lifetime;
		}
    break;

  case 141:
/* Line 1787 of yacc.c  */
#line 982 "gram.y"
    {
			struct timespec lifetime;
			lifetime.tv_sec = (yyvsp[(2) - (3)].num)/1000;
			lifetime.tv_nsec = ((yyvsp[(2) - (3)].num) % 1000)*1000000;
			conf_parsed->PBULifeTime = lifetime;
		}
    break;

  case 142:
/* Line 1787 of yacc.c  */
#line 989 "gram.y"
    {
			struct timespec lifetime;
			lifetime.tv_sec = (yyvsp[(2) - (3)].num)/1000;
			lifetime.tv_nsec = ((yyvsp[(2) - (3)].num) % 1000)*1000000;
			conf_parsed->PBALifeTime = lifetime;
		}
    break;

  case 143:
/* Line 1787 of yacc.c  */
#line 996 "gram.y"
    {
			struct timespec lifetime;
			lifetime.tv_sec = (yyvsp[(2) - (3)].num)/1000;
			lifetime.tv_nsec = ((yyvsp[(2) - (3)].num) % 1000)*1000000;
			conf_parsed->RetransmissionTimeOut = lifetime;
		}
    break;

  case 144:
/* Line 1787 of yacc.c  */
#line 1003 "gram.y"
    {
			conf_parsed->MaxMessageRetransmissions = (yyvsp[(2) - (3)].num);
		}
    break;

  case 145:
/* Line 1787 of yacc.c  */
#line 1007 "gram.y"
    {
			memcpy(&conf_parsed->MagAddressIngress[conf_parsed->NumMags], &(yyvsp[(2) - (3)].addr), sizeof(struct in6_addr));
		}
    break;

  case 146:
/* Line 1787 of yacc.c  */
#line 1011 "gram.y"
    {
			memcpy(&conf_parsed->MagAddressEgress[conf_parsed->NumMags], &(yyvsp[(2) - (3)].addr), sizeof(struct in6_addr));
			if (! IN6_IS_ADDR_UNSPECIFIED(&conf_parsed->MagAddressEgress[conf_parsed->NumMags])) {
				conf_parsed->NumMags = conf_parsed->NumMags + 1;
			}
		}
    break;

  case 147:
/* Line 1787 of yacc.c  */
#line 1020 "gram.y"
    {
			conf_parsed->HomeNetworkPrefix = in6addr_any;
			conf_parsed->OurAddress        = in6addr_loopback;
		}
    break;

  case 152:
/* Line 1787 of yacc.c  */
#line 1035 "gram.y"
    {
			memcpy(&conf_parsed->LmaAddress, &(yyvsp[(2) - (3)].addr), sizeof(struct in6_addr));
		}
    break;

  case 153:
/* Line 1787 of yacc.c  */
#line 1039 "gram.y"
    {
			conf_parsed->RFC5213TimestampBasedApproachInUse = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 154:
/* Line 1787 of yacc.c  */
#line 1043 "gram.y"
    {
			conf_parsed->RFC5213MobileNodeGeneratedTimestampInUse = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 155:
/* Line 1787 of yacc.c  */
#line 1047 "gram.y"
    {
			memcpy(&conf_parsed->RFC5213FixedMAGLinkLocalAddressOnAllAccessLinks, &(yyvsp[(2) - (3)].addr), sizeof(struct in6_addr));
		}
    break;

  case 156:
/* Line 1787 of yacc.c  */
#line 1051 "gram.y"
    {
			memcpy(&conf_parsed->RFC5213FixedMAGLinkLayerAddressOnAllAccessLinks, &(yyvsp[(2) - (3)].macaddr), sizeof(struct in6_addr));
		}
    break;

  case 157:
/* Line 1787 of yacc.c  */
#line 1055 "gram.y"
    {
			conf_parsed->RFC5213EnableMAGLocalRouting = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 158:
/* Line 1787 of yacc.c  */
#line 1059 "gram.y"
    {
			memcpy(&conf_parsed->OurAddress, &(yyvsp[(2) - (3)].addr), sizeof(struct in6_addr));
		}
    break;

  case 159:
/* Line 1787 of yacc.c  */
#line 1063 "gram.y"
    {
			memcpy(&conf_parsed->MagAddressIngress[0], &(yyvsp[(2) - (3)].addr), sizeof(struct in6_addr));
		}
    break;

  case 160:
/* Line 1787 of yacc.c  */
#line 1067 "gram.y"
    {
			memcpy(&conf_parsed->MagAddressEgress[0], &(yyvsp[(2) - (3)].addr), sizeof(struct in6_addr));
		}
    break;

  case 161:
/* Line 1787 of yacc.c  */
#line 1071 "gram.y"
    {
			conf_parsed->MagDeviceIngress = (yyvsp[(2) - (3)].string);
		}
    break;

  case 162:
/* Line 1787 of yacc.c  */
#line 1075 "gram.y"
    {
			conf_parsed->MagDeviceEgress = (yyvsp[(2) - (3)].string);
		}
    break;

  case 163:
/* Line 1787 of yacc.c  */
#line 1079 "gram.y"
    {
			memcpy(&conf_parsed->HomeNetworkPrefix, &(yyvsp[(2) - (3)].addr), sizeof(struct in6_addr));
		}
    break;

  case 164:
/* Line 1787 of yacc.c  */
#line 1083 "gram.y"
    {
			conf_parsed->TunnelingEnabled = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 165:
/* Line 1787 of yacc.c  */
#line 1087 "gram.y"
    {
			conf_parsed->DynamicTunnelingEnabled = (yyvsp[(2) - (3)].bool);
		}
    break;

  case 166:
/* Line 1787 of yacc.c  */
#line 1091 "gram.y"
    {
			struct timespec lifetime;
			lifetime.tv_sec = (yyvsp[(2) - (3)].num)/1000;
			lifetime.tv_nsec = ((yyvsp[(2) - (3)].num) % 1000)*1000000;
			conf_parsed->MaxDelayBeforeDynamicTunnelingDelete = lifetime;
		}
    break;

  case 167:
/* Line 1787 of yacc.c  */
#line 1098 "gram.y"
    {
			struct timespec lifetime;
			lifetime.tv_sec = (yyvsp[(2) - (3)].num)/1000;
			lifetime.tv_nsec = ((yyvsp[(2) - (3)].num) % 1000)*1000000;
			conf_parsed->PBULifeTime = lifetime;
		}
    break;

  case 168:
/* Line 1787 of yacc.c  */
#line 1105 "gram.y"
    {
			struct timespec lifetime;
			lifetime.tv_sec = (yyvsp[(2) - (3)].num)/1000;
			lifetime.tv_nsec = ((yyvsp[(2) - (3)].num) % 1000)*1000000;
			conf_parsed->PBALifeTime = lifetime;
		}
    break;

  case 169:
/* Line 1787 of yacc.c  */
#line 1112 "gram.y"
    {
			struct timespec lifetime;
			lifetime.tv_sec = (yyvsp[(2) - (3)].num)/1000;
			lifetime.tv_nsec = ((yyvsp[(2) - (3)].num) % 1000)*1000000;
			conf_parsed->RetransmissionTimeOut = lifetime;
		}
    break;

  case 170:
/* Line 1787 of yacc.c  */
#line 1119 "gram.y"
    {
			conf_parsed->MaxMessageRetransmissions = (yyvsp[(2) - (3)].num);
		}
    break;

  case 171:
/* Line 1787 of yacc.c  */
#line 1123 "gram.y"
    {
			conf_parsed->RadiusPassword = (yyvsp[(2) - (3)].string);
		}
    break;

  case 172:
/* Line 1787 of yacc.c  */
#line 1127 "gram.y"
    {
			conf_parsed->RadiusClientConfigFile = (yyvsp[(2) - (3)].string);
		}
    break;

  case 173:
/* Line 1787 of yacc.c  */
#line 1131 "gram.y"
    {
			conf_parsed->PcapSyslogAssociationGrepString = (yyvsp[(2) - (3)].string);
		}
    break;

  case 174:
/* Line 1787 of yacc.c  */
#line 1135 "gram.y"
    {
			conf_parsed->PcapSyslogDeAssociationGrepString = (yyvsp[(2) - (3)].string);
		}
    break;


/* Line 1787 of yacc.c  */
#line 3399 "y.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2050 of yacc.c  */
#line 1139 "gram.y"

