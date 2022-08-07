/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "bc.y"

/* bc.y: The grammar for a POSIX compatable bc processor with some
         extensions to the language. */

/*  This file is part of bc written for MINIX.
    Copyright (C) 1991, 1992, 1993, 1994 Free Software Foundation, Inc.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License , or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; see the file COPYING.  If not, write to
    the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

    You may contact the author by:
       e-mail:  phil@cs.wwu.edu
      us-mail:  Philip A. Nelson
                Computer Science Department, 9062
                Western Washington University
                Bellingham, WA 98226-9062
       
*************************************************************************/

#include "bcdefs.h"
#include "global.h"
#include "proto.h"

#line 106 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NEWLINE = 258,                 /* NEWLINE  */
    AND = 259,                     /* AND  */
    OR = 260,                      /* OR  */
    NOT = 261,                     /* NOT  */
    STRING = 262,                  /* STRING  */
    NAME = 263,                    /* NAME  */
    NUMBER = 264,                  /* NUMBER  */
    MUL_OP = 265,                  /* MUL_OP  */
    ASSIGN_OP = 266,               /* ASSIGN_OP  */
    REL_OP = 267,                  /* REL_OP  */
    INCR_DECR = 268,               /* INCR_DECR  */
    Define = 269,                  /* Define  */
    Break = 270,                   /* Break  */
    Quit = 271,                    /* Quit  */
    Length = 272,                  /* Length  */
    Return = 273,                  /* Return  */
    For = 274,                     /* For  */
    If = 275,                      /* If  */
    While = 276,                   /* While  */
    Sqrt = 277,                    /* Sqrt  */
    Else = 278,                    /* Else  */
    Scale = 279,                   /* Scale  */
    Ibase = 280,                   /* Ibase  */
    Obase = 281,                   /* Obase  */
    Auto = 282,                    /* Auto  */
    Read = 283,                    /* Read  */
    Warranty = 284,                /* Warranty  */
    Halt = 285,                    /* Halt  */
    Last = 286,                    /* Last  */
    Continue = 287,                /* Continue  */
    Print = 288,                   /* Print  */
    Limits = 289,                  /* Limits  */
    UNARY_MINUS = 290              /* UNARY_MINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define NEWLINE 258
#define AND 259
#define OR 260
#define NOT 261
#define STRING 262
#define NAME 263
#define NUMBER 264
#define MUL_OP 265
#define ASSIGN_OP 266
#define REL_OP 267
#define INCR_DECR 268
#define Define 269
#define Break 270
#define Quit 271
#define Length 272
#define Return 273
#define For 274
#define If 275
#define While 276
#define Sqrt 277
#define Else 278
#define Scale 279
#define Ibase 280
#define Obase 281
#define Auto 282
#define Read 283
#define Warranty 284
#define Halt 285
#define Last 286
#define Continue 287
#define Print 288
#define Limits 289
#define UNARY_MINUS 290

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 38 "bc.y"

	char	 *s_value;
	char	  c_value;
	int	  i_value;
	arg_list *a_value;
       

#line 237 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NEWLINE = 3,                    /* NEWLINE  */
  YYSYMBOL_AND = 4,                        /* AND  */
  YYSYMBOL_OR = 5,                         /* OR  */
  YYSYMBOL_NOT = 6,                        /* NOT  */
  YYSYMBOL_STRING = 7,                     /* STRING  */
  YYSYMBOL_NAME = 8,                       /* NAME  */
  YYSYMBOL_NUMBER = 9,                     /* NUMBER  */
  YYSYMBOL_MUL_OP = 10,                    /* MUL_OP  */
  YYSYMBOL_ASSIGN_OP = 11,                 /* ASSIGN_OP  */
  YYSYMBOL_REL_OP = 12,                    /* REL_OP  */
  YYSYMBOL_INCR_DECR = 13,                 /* INCR_DECR  */
  YYSYMBOL_Define = 14,                    /* Define  */
  YYSYMBOL_Break = 15,                     /* Break  */
  YYSYMBOL_Quit = 16,                      /* Quit  */
  YYSYMBOL_Length = 17,                    /* Length  */
  YYSYMBOL_Return = 18,                    /* Return  */
  YYSYMBOL_For = 19,                       /* For  */
  YYSYMBOL_If = 20,                        /* If  */
  YYSYMBOL_While = 21,                     /* While  */
  YYSYMBOL_Sqrt = 22,                      /* Sqrt  */
  YYSYMBOL_Else = 23,                      /* Else  */
  YYSYMBOL_Scale = 24,                     /* Scale  */
  YYSYMBOL_Ibase = 25,                     /* Ibase  */
  YYSYMBOL_Obase = 26,                     /* Obase  */
  YYSYMBOL_Auto = 27,                      /* Auto  */
  YYSYMBOL_Read = 28,                      /* Read  */
  YYSYMBOL_Warranty = 29,                  /* Warranty  */
  YYSYMBOL_Halt = 30,                      /* Halt  */
  YYSYMBOL_Last = 31,                      /* Last  */
  YYSYMBOL_Continue = 32,                  /* Continue  */
  YYSYMBOL_Print = 33,                     /* Print  */
  YYSYMBOL_Limits = 34,                    /* Limits  */
  YYSYMBOL_35_ = 35,                       /* '+'  */
  YYSYMBOL_36_ = 36,                       /* '-'  */
  YYSYMBOL_37_ = 37,                       /* '^'  */
  YYSYMBOL_UNARY_MINUS = 38,               /* UNARY_MINUS  */
  YYSYMBOL_39_ = 39,                       /* ';'  */
  YYSYMBOL_40_ = 40,                       /* '('  */
  YYSYMBOL_41_ = 41,                       /* ')'  */
  YYSYMBOL_42_ = 42,                       /* '{'  */
  YYSYMBOL_43_ = 43,                       /* '}'  */
  YYSYMBOL_44_ = 44,                       /* ','  */
  YYSYMBOL_45_ = 45,                       /* '['  */
  YYSYMBOL_46_ = 46,                       /* ']'  */
  YYSYMBOL_YYACCEPT = 47,                  /* $accept  */
  YYSYMBOL_program = 48,                   /* program  */
  YYSYMBOL_input_item = 49,                /* input_item  */
  YYSYMBOL_semicolon_list = 50,            /* semicolon_list  */
  YYSYMBOL_statement_list = 51,            /* statement_list  */
  YYSYMBOL_statement_or_error = 52,        /* statement_or_error  */
  YYSYMBOL_statement = 53,                 /* statement  */
  YYSYMBOL_54_1 = 54,                      /* $@1  */
  YYSYMBOL_55_2 = 55,                      /* $@2  */
  YYSYMBOL_56_3 = 56,                      /* @3  */
  YYSYMBOL_57_4 = 57,                      /* $@4  */
  YYSYMBOL_58_5 = 58,                      /* $@5  */
  YYSYMBOL_59_6 = 59,                      /* $@6  */
  YYSYMBOL_60_7 = 60,                      /* $@7  */
  YYSYMBOL_61_8 = 61,                      /* $@8  */
  YYSYMBOL_print_list = 62,                /* print_list  */
  YYSYMBOL_print_element = 63,             /* print_element  */
  YYSYMBOL_opt_else = 64,                  /* opt_else  */
  YYSYMBOL_65_9 = 65,                      /* $@9  */
  YYSYMBOL_function = 66,                  /* function  */
  YYSYMBOL_67_10 = 67,                     /* $@10  */
  YYSYMBOL_opt_parameter_list = 68,        /* opt_parameter_list  */
  YYSYMBOL_opt_auto_define_list = 69,      /* opt_auto_define_list  */
  YYSYMBOL_define_list = 70,               /* define_list  */
  YYSYMBOL_opt_argument_list = 71,         /* opt_argument_list  */
  YYSYMBOL_argument_list = 72,             /* argument_list  */
  YYSYMBOL_opt_expression = 73,            /* opt_expression  */
  YYSYMBOL_return_expression = 74,         /* return_expression  */
  YYSYMBOL_expression = 75,                /* expression  */
  YYSYMBOL_76_11 = 76,                     /* $@11  */
  YYSYMBOL_77_12 = 77,                     /* $@12  */
  YYSYMBOL_78_13 = 78,                     /* $@13  */
  YYSYMBOL_named_expression = 79           /* named_expression  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
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
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   616

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  33
/* YYNRULES -- Number of rules.  */
#define YYNRULES  98
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  166

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   290


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      40,    41,     2,    35,    44,    36,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    39,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    45,     2,    46,    37,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    42,     2,    43,     2,     2,     2,     2,
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
      38
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   106,   106,   114,   116,   118,   120,   127,   128,   129,
     130,   133,   134,   135,   136,   137,   138,   140,   141,   144,
     146,   148,   157,   164,   174,   185,   187,   189,   191,   194,
     199,   210,   221,   193,   239,   238,   252,   258,   251,   270,
     273,   272,   276,   277,   279,   285,   288,   290,   289,   300,
     298,   319,   320,   323,   324,   326,   329,   331,   333,   335,
     339,   340,   342,   347,   353,   358,   366,   370,   373,   377,
     384,   383,   411,   410,   424,   423,   439,   445,   473,   478,
     483,   490,   495,   500,   509,   525,   527,   543,   562,   585,
     587,   589,   591,   597,   599,   604,   606,   608,   610
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NEWLINE", "AND", "OR",
  "NOT", "STRING", "NAME", "NUMBER", "MUL_OP", "ASSIGN_OP", "REL_OP",
  "INCR_DECR", "Define", "Break", "Quit", "Length", "Return", "For", "If",
  "While", "Sqrt", "Else", "Scale", "Ibase", "Obase", "Auto", "Read",
  "Warranty", "Halt", "Last", "Continue", "Print", "Limits", "'+'", "'-'",
  "'^'", "UNARY_MINUS", "';'", "'('", "')'", "'{'", "'}'", "','", "'['",
  "']'", "$accept", "program", "input_item", "semicolon_list",
  "statement_list", "statement_or_error", "statement", "$@1", "$@2", "@3",
  "$@4", "$@5", "$@6", "$@7", "$@8", "print_list", "print_element",
  "opt_else", "$@9", "function", "$@10", "opt_parameter_list",
  "opt_auto_define_list", "define_list", "opt_argument_list",
  "argument_list", "opt_expression", "return_expression", "expression",
  "$@11", "$@12", "$@13", "named_expression", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-125)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-14)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -125,   156,  -125,   352,   534,  -125,   -31,  -125,    49,     2,
    -125,  -125,   -19,   -17,  -125,   -15,  -125,    -5,    19,  -125,
    -125,    20,  -125,  -125,  -125,  -125,  -125,  -125,   534,   534,
     196,  -125,     4,  -125,  -125,  -125,    73,     9,  -125,  -125,
     110,   555,   534,   -12,  -125,  -125,    21,   534,   534,    28,
     534,    29,   534,   534,    30,   513,  -125,   385,   476,    13,
    -125,  -125,   282,  -125,  -125,   534,   534,   534,   534,   534,
    -125,  -125,   -28,    31,   -10,    73,     1,    33,   394,    35,
      73,   534,   397,   534,   406,   440,  -125,  -125,  -125,    26,
      73,  -125,   239,   476,  -125,  -125,   534,   534,    45,    -7,
      -6,    -6,    45,   534,    90,  -125,   576,  -125,    39,    46,
      42,  -125,  -125,     3,    73,  -125,    73,  -125,  -125,   513,
    -125,  -125,   110,   131,    -7,  -125,   -21,    73,    43,    51,
      80,  -125,   476,    50,  -125,   317,  -125,    94,    55,   534,
      82,   476,  -125,    79,    65,    74,  -125,  -125,  -125,    33,
    -125,  -125,  -125,   476,     5,   196,   534,  -125,  -125,  -125,
      15,    78,  -125,  -125,   476,  -125
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     1,     0,     0,    22,    93,    84,     0,     0,
      23,    25,     0,    27,    29,     0,    36,     0,    97,    95,
      96,     0,    19,    26,    98,    24,    40,    20,     0,     0,
       0,     3,     0,     8,    17,     5,    21,    83,     6,    18,
      76,    60,     0,    93,    97,    87,     0,     0,    68,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
      12,     4,     0,    72,    74,     0,     0,     0,     0,     0,
      70,    88,    93,     0,    61,    62,     0,    51,     0,     0,
      69,    66,     0,     0,     0,     0,    92,    44,    41,    42,
      45,    85,     0,    15,    39,     9,     0,     0,    80,    77,
      78,    79,    81,     0,     0,    86,     0,    94,    56,     0,
      52,    89,    28,     0,    67,    34,    37,    90,    91,     0,
      14,    16,    73,    75,    71,    63,    93,    64,     0,     0,
       0,    30,     0,     0,    43,     0,    57,     0,    58,    66,
      46,     0,    65,    53,     0,     0,    47,    35,    38,     0,
      49,    59,    31,     0,     0,     0,    66,    48,    54,    55,
       0,     0,    50,    32,     0,    33
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -125,  -125,  -125,  -125,   -32,     0,    -3,  -125,  -125,  -125,
    -125,  -125,  -125,  -125,  -125,     6,  -125,  -125,  -125,  -125,
    -125,  -125,  -125,   -25,  -125,  -125,  -124,  -125,    -2,  -125,
    -125,  -125,   119
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,    31,    32,    59,    60,    34,    49,   139,   156,
     164,   132,    51,   133,    55,    88,    89,   147,   153,    35,
     155,   109,   150,   110,    73,    74,   113,    79,    36,   103,
      96,    97,    37
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      39,    33,    40,    65,    65,    63,    64,    61,   158,    41,
      46,    65,    41,    66,    42,   145,    92,   104,    92,    41,
      70,    47,    71,    48,   135,    50,    56,    57,    67,    68,
      69,    69,   161,    42,   106,    52,    67,    68,    69,    75,
      76,   108,   131,    62,   159,    78,    80,   107,    82,   130,
      84,    85,    93,    90,    93,    39,    94,    43,   162,    53,
      54,    77,    95,    98,    99,   100,   101,   102,    81,    83,
     119,    86,   105,    44,    19,    20,   112,    63,    64,   114,
      24,   116,    69,    65,   128,    66,   130,   129,   138,   136,
     121,   141,   120,   137,   122,   123,     4,   143,     6,     7,
     144,   124,    76,     8,   127,   146,   149,    12,    67,    68,
      69,   151,    17,   152,    18,    19,    20,    90,    21,   163,
      65,    24,    66,   160,   154,   134,    28,    45,     0,   140,
      29,     0,     0,    76,     0,    63,   125,   114,   148,     0,
       0,    65,     0,    66,     0,    67,    68,    69,     0,     0,
     157,     0,     0,     0,   114,     0,     2,     3,     0,    -7,
       0,   165,     4,     5,     6,     7,    67,    68,    69,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,     0,
      18,    19,    20,     0,    21,    22,    23,    24,    25,    26,
      27,     0,    28,     0,     0,    -7,    29,    58,    30,   -11,
       0,     0,     4,     5,     6,     7,     0,     0,     0,     8,
       0,    10,    11,    12,    13,    14,    15,    16,    17,     0,
      18,    19,    20,     0,    21,    22,    23,    24,    25,    26,
      27,     0,    28,     0,     0,   -11,    29,     0,    30,   -11,
      58,     0,   -13,     0,     0,     4,     5,     6,     7,     0,
       0,     0,     8,     0,    10,    11,    12,    13,    14,    15,
      16,    17,     0,    18,    19,    20,     0,    21,    22,    23,
      24,    25,    26,    27,     0,    28,     0,     0,   -13,    29,
       0,    30,   -13,    58,     0,   -10,     0,     0,     4,     5,
       6,     7,     0,     0,     0,     8,     0,    10,    11,    12,
      13,    14,    15,    16,    17,     0,    18,    19,    20,     0,
      21,    22,    23,    24,    25,    26,    27,     0,    28,     0,
       0,   -10,    29,     4,    30,     6,     7,     0,     0,     0,
       8,     0,     0,     0,    12,     0,     0,     0,     0,    17,
       0,    18,    19,    20,     0,    21,     0,     0,    24,     0,
       0,     0,     0,    28,     0,    38,     0,    29,     4,     5,
       6,     7,     0,   142,     0,     8,     0,    10,    11,    12,
      13,    14,    15,    16,    17,     0,    18,    19,    20,     0,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    63,
      64,     0,    29,     0,    30,    65,     0,    66,    63,    64,
       0,    63,    64,     0,    65,     0,    66,    65,     0,    66,
      63,    64,     0,     0,     0,     0,    65,     0,    66,     0,
      67,    68,    69,     0,     0,     0,    91,     0,     0,    67,
      68,    69,    67,    68,    69,   111,     0,     0,   115,     0,
       0,    67,    68,    69,    63,    64,     0,   117,     0,     0,
      65,     0,    66,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,     0,     0,
       0,   118,     4,     5,     6,     7,     0,     0,     0,     8,
       0,    10,    11,    12,    13,    14,    15,    16,    17,     0,
      18,    19,    20,     0,    21,    22,    23,    24,    25,    26,
      27,     0,    28,     0,     0,     0,    29,     0,    30,     4,
      87,     6,     7,     0,     0,     0,     8,     0,     0,     0,
      12,     0,     0,     0,     0,    17,     0,    18,    19,    20,
       4,    21,     6,     7,    24,     0,     0,     8,     0,    28,
       0,    12,     0,    29,     0,     0,    17,     0,    18,    19,
      20,     4,    21,    72,     7,    24,     0,     0,     8,     0,
      28,     0,    12,     0,    29,     0,     0,    17,     0,    18,
      19,    20,     4,    21,   126,     7,    24,     0,     0,     8,
       0,    28,     0,    12,     0,    29,     0,     0,    17,     0,
      18,    19,    20,     0,    21,     0,     0,    24,     0,     0,
       0,     0,    28,     0,     0,     0,    29
};

static const yytype_int16 yycheck[] =
{
       3,     1,     4,    10,    10,     4,     5,     3,     3,    40,
       8,    10,    40,    12,    45,   139,     3,    45,     3,    40,
      11,    40,    13,    40,    45,    40,    28,    29,    35,    36,
      37,    37,   156,    45,    44,    40,    35,    36,    37,    41,
      42,     8,    39,    39,    39,    47,    48,    46,    50,    44,
      52,    53,    39,    55,    39,    58,    43,     8,    43,    40,
      40,    40,    62,    65,    66,    67,    68,    69,    40,    40,
      44,    41,    41,    24,    25,    26,    41,     4,     5,    81,
      31,    83,    37,    10,    45,    12,    44,    41,     8,    46,
      93,    41,    92,    42,    96,    97,     6,     3,     8,     9,
      45,   103,   104,    13,   106,    23,    27,    17,    35,    36,
      37,    46,    22,    39,    24,    25,    26,   119,    28,    41,
      10,    31,    12,   155,   149,   119,    36,     8,    -1,   132,
      40,    -1,    -1,   135,    -1,     4,    46,   139,   141,    -1,
      -1,    10,    -1,    12,    -1,    35,    36,    37,    -1,    -1,
     153,    -1,    -1,    -1,   156,    -1,     0,     1,    -1,     3,
      -1,   164,     6,     7,     8,     9,    35,    36,    37,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    26,    -1,    28,    29,    30,    31,    32,    33,
      34,    -1,    36,    -1,    -1,    39,    40,     1,    42,     3,
      -1,    -1,     6,     7,     8,     9,    -1,    -1,    -1,    13,
      -1,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    26,    -1,    28,    29,    30,    31,    32,    33,
      34,    -1,    36,    -1,    -1,    39,    40,    -1,    42,    43,
       1,    -1,     3,    -1,    -1,     6,     7,     8,     9,    -1,
      -1,    -1,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    24,    25,    26,    -1,    28,    29,    30,
      31,    32,    33,    34,    -1,    36,    -1,    -1,    39,    40,
      -1,    42,    43,     1,    -1,     3,    -1,    -1,     6,     7,
       8,     9,    -1,    -1,    -1,    13,    -1,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    26,    -1,
      28,    29,    30,    31,    32,    33,    34,    -1,    36,    -1,
      -1,    39,    40,     6,    42,     8,     9,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    22,
      -1,    24,    25,    26,    -1,    28,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    36,    -1,     3,    -1,    40,     6,     7,
       8,     9,    -1,    46,    -1,    13,    -1,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    26,    -1,
      28,    29,    30,    31,    32,    33,    34,    -1,    36,     4,
       5,    -1,    40,    -1,    42,    10,    -1,    12,     4,     5,
      -1,     4,     5,    -1,    10,    -1,    12,    10,    -1,    12,
       4,     5,    -1,    -1,    -1,    -1,    10,    -1,    12,    -1,
      35,    36,    37,    -1,    -1,    -1,    41,    -1,    -1,    35,
      36,    37,    35,    36,    37,    41,    -1,    -1,    41,    -1,
      -1,    35,    36,    37,     4,     5,    -1,    41,    -1,    -1,
      10,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    36,    37,    -1,    -1,
      -1,    41,     6,     7,     8,     9,    -1,    -1,    -1,    13,
      -1,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    26,    -1,    28,    29,    30,    31,    32,    33,
      34,    -1,    36,    -1,    -1,    -1,    40,    -1,    42,     6,
       7,     8,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    22,    -1,    24,    25,    26,
       6,    28,     8,     9,    31,    -1,    -1,    13,    -1,    36,
      -1,    17,    -1,    40,    -1,    -1,    22,    -1,    24,    25,
      26,     6,    28,     8,     9,    31,    -1,    -1,    13,    -1,
      36,    -1,    17,    -1,    40,    -1,    -1,    22,    -1,    24,
      25,    26,     6,    28,     8,     9,    31,    -1,    -1,    13,
      -1,    36,    -1,    17,    -1,    40,    -1,    -1,    22,    -1,
      24,    25,    26,    -1,    28,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    40
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    48,     0,     1,     6,     7,     8,     9,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    24,    25,
      26,    28,    29,    30,    31,    32,    33,    34,    36,    40,
      42,    49,    50,    52,    53,    66,    75,    79,     3,    53,
      75,    40,    45,     8,    24,    79,     8,    40,    40,    54,
      40,    59,    40,    40,    40,    61,    75,    75,     1,    51,
      52,     3,    39,     4,     5,    10,    12,    35,    36,    37,
      11,    13,     8,    71,    72,    75,    75,    40,    75,    74,
      75,    40,    75,    40,    75,    75,    41,     7,    62,    63,
      75,    41,     3,    39,    43,    52,    77,    78,    75,    75,
      75,    75,    75,    76,    45,    41,    44,    46,     8,    68,
      70,    41,    41,    73,    75,    41,    75,    41,    41,    44,
      52,    53,    75,    75,    75,    46,     8,    75,    45,    41,
      44,    39,    58,    60,    62,    45,    46,    42,     8,    55,
      53,    41,    46,     3,    45,    73,    23,    64,    53,    27,
      69,    46,    39,    65,    70,    67,    56,    53,     3,    39,
      51,    73,    43,    41,    57,    53
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    47,    48,    48,    49,    49,    49,    50,    50,    50,
      50,    51,    51,    51,    51,    51,    51,    52,    52,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    54,
      55,    56,    57,    53,    58,    53,    59,    60,    53,    53,
      61,    53,    62,    62,    63,    63,    64,    65,    64,    67,
      66,    68,    68,    69,    69,    69,    70,    70,    70,    70,
      71,    71,    72,    72,    72,    72,    73,    73,    74,    74,
      76,    75,    77,    75,    78,    75,    75,    75,    75,    75,
      75,    75,    75,    75,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    79,    79,    79,    79,    79,    79
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     1,     2,     0,     1,     3,
       2,     0,     1,     2,     3,     2,     3,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     0,
       0,     0,     0,    13,     0,     7,     0,     0,     7,     3,
       0,     3,     1,     3,     1,     1,     0,     0,     3,     0,
      11,     0,     1,     0,     3,     3,     1,     3,     3,     5,
       0,     1,     1,     3,     3,     5,     0,     1,     0,     1,
       0,     4,     0,     4,     0,     4,     2,     3,     3,     3,
       3,     3,     2,     1,     1,     3,     4,     2,     2,     4,
       4,     4,     3,     1,     4,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: %empty  */
#line 106 "bc.y"
                            {
			      (yyval.i_value) = 0;
			      if (interactive)
				{
				  printf ("%s\n", BC_VERSION);
				  welcome ();
				}
			    }
#line 1506 "y.tab.c"
    break;

  case 4: /* input_item: semicolon_list NEWLINE  */
#line 117 "bc.y"
                            { run_code (); }
#line 1512 "y.tab.c"
    break;

  case 5: /* input_item: function  */
#line 119 "bc.y"
                            { run_code (); }
#line 1518 "y.tab.c"
    break;

  case 6: /* input_item: error NEWLINE  */
#line 121 "bc.y"
                            {
			      yyerrok;
			      init_gen ();
			    }
#line 1527 "y.tab.c"
    break;

  case 7: /* semicolon_list: %empty  */
#line 127 "bc.y"
                            { (yyval.i_value) = 0; }
#line 1533 "y.tab.c"
    break;

  case 11: /* statement_list: %empty  */
#line 133 "bc.y"
                            { (yyval.i_value) = 0; }
#line 1539 "y.tab.c"
    break;

  case 18: /* statement_or_error: error statement  */
#line 142 "bc.y"
                            { (yyval.i_value) = (yyvsp[0].i_value); }
#line 1545 "y.tab.c"
    break;

  case 19: /* statement: Warranty  */
#line 145 "bc.y"
                            { warranty (""); }
#line 1551 "y.tab.c"
    break;

  case 20: /* statement: Limits  */
#line 147 "bc.y"
                            { limits (); }
#line 1557 "y.tab.c"
    break;

  case 21: /* statement: expression  */
#line 149 "bc.y"
                            {
			      if ((yyvsp[0].i_value) & 2)
				warn ("comparison in expression");
			      if ((yyvsp[0].i_value) & 1)
				generate ("W");
			      else 
				generate ("p");
			    }
#line 1570 "y.tab.c"
    break;

  case 22: /* statement: STRING  */
#line 158 "bc.y"
                            {
			      (yyval.i_value) = 0;
			      generate ("w");
			      generate ((yyvsp[0].s_value));
			      free ((yyvsp[0].s_value));
			    }
#line 1581 "y.tab.c"
    break;

  case 23: /* statement: Break  */
#line 165 "bc.y"
                            {
			      if (break_label == 0)
				yyerror ("Break outside a for/while");
			      else
				{
				  sprintf (genstr, "J%1d:", break_label);
				  generate (genstr);
				}
			    }
#line 1595 "y.tab.c"
    break;

  case 24: /* statement: Continue  */
#line 175 "bc.y"
                            {
			      warn ("Continue statement");
			      if (continue_label == 0)
				yyerror ("Continue outside a for");
			      else
				{
				  sprintf (genstr, "J%1d:", continue_label);
				  generate (genstr);
				}
			    }
#line 1610 "y.tab.c"
    break;

  case 25: /* statement: Quit  */
#line 186 "bc.y"
                            { exit (0); }
#line 1616 "y.tab.c"
    break;

  case 26: /* statement: Halt  */
#line 188 "bc.y"
                            { generate ("h"); }
#line 1622 "y.tab.c"
    break;

  case 27: /* statement: Return  */
#line 190 "bc.y"
                            { generate ("0R"); }
#line 1628 "y.tab.c"
    break;

  case 28: /* statement: Return '(' return_expression ')'  */
#line 192 "bc.y"
                            { generate ("R"); }
#line 1634 "y.tab.c"
    break;

  case 29: /* $@1: %empty  */
#line 194 "bc.y"
                            {
			      (yyvsp[0].i_value) = break_label; 
			      break_label = next_label++;
			    }
#line 1643 "y.tab.c"
    break;

  case 30: /* $@2: %empty  */
#line 199 "bc.y"
                            {
			      if ((yyvsp[-1].i_value) > 1)
				warn ("Comparison in first for expression");
			      (yyvsp[-1].i_value) = next_label++;
			      if ((yyvsp[-1].i_value) < 0)
				sprintf (genstr, "N%1d:", (yyvsp[-1].i_value));
			      else
				sprintf (genstr, "pN%1d:", (yyvsp[-1].i_value));
			      generate (genstr);
			    }
#line 1658 "y.tab.c"
    break;

  case 31: /* @3: %empty  */
#line 210 "bc.y"
                            {
			      if ((yyvsp[-1].i_value) < 0) generate ("1");
			      (yyvsp[-1].i_value) = next_label++;
			      sprintf (genstr, "B%1d:J%1d:", (yyvsp[-1].i_value), break_label);
			      generate (genstr);
			      (yyval.i_value) = continue_label;
			      continue_label = next_label++;
			      sprintf (genstr, "N%1d:", continue_label);
			      generate (genstr);
			    }
#line 1673 "y.tab.c"
    break;

  case 32: /* $@4: %empty  */
#line 221 "bc.y"
                            {
			      if ((yyvsp[-1].i_value) > 1)
				warn ("Comparison in third for expression");
			      if ((yyvsp[-1].i_value) < 0)
				sprintf (genstr, "J%1d:N%1d:", (yyvsp[-7].i_value), (yyvsp[-4].i_value));
			      else
				sprintf (genstr, "pJ%1d:N%1d:", (yyvsp[-7].i_value), (yyvsp[-4].i_value));
			      generate (genstr);
			    }
#line 1687 "y.tab.c"
    break;

  case 33: /* statement: For $@1 '(' opt_expression ';' $@2 opt_expression ';' @3 opt_expression ')' $@4 statement  */
#line 231 "bc.y"
                            {
			      sprintf (genstr, "J%1d:N%1d:",
				       continue_label, break_label);
			      generate (genstr);
			      break_label = (yyvsp[-12].i_value);
			      continue_label = (yyvsp[-4].i_value);
			    }
#line 1699 "y.tab.c"
    break;

  case 34: /* $@5: %empty  */
#line 239 "bc.y"
                            {
			      (yyvsp[-1].i_value) = if_label;
			      if_label = next_label++;
			      sprintf (genstr, "Z%1d:", if_label);
			      generate (genstr);
			    }
#line 1710 "y.tab.c"
    break;

  case 35: /* statement: If '(' expression ')' $@5 statement opt_else  */
#line 246 "bc.y"
                            {
			      sprintf (genstr, "N%1d:", if_label); 
			      generate (genstr);
			      if_label = (yyvsp[-4].i_value);
			    }
#line 1720 "y.tab.c"
    break;

  case 36: /* $@6: %empty  */
#line 252 "bc.y"
                            {
			      (yyvsp[0].i_value) = next_label++;
			      sprintf (genstr, "N%1d:", (yyvsp[0].i_value));
			      generate (genstr);
			    }
#line 1730 "y.tab.c"
    break;

  case 37: /* $@7: %empty  */
#line 258 "bc.y"
                            {
			      (yyvsp[0].i_value) = break_label; 
			      break_label = next_label++;
			      sprintf (genstr, "Z%1d:", break_label);
			      generate (genstr);
			    }
#line 1741 "y.tab.c"
    break;

  case 38: /* statement: While $@6 '(' expression $@7 ')' statement  */
#line 265 "bc.y"
                            {
			      sprintf (genstr, "J%1d:N%1d:", (yyvsp[-6].i_value), break_label);
			      generate (genstr);
			      break_label = (yyvsp[-3].i_value);
			    }
#line 1751 "y.tab.c"
    break;

  case 39: /* statement: '{' statement_list '}'  */
#line 271 "bc.y"
                            { (yyval.i_value) = 0; }
#line 1757 "y.tab.c"
    break;

  case 40: /* $@8: %empty  */
#line 273 "bc.y"
                            {  warn ("print statement"); }
#line 1763 "y.tab.c"
    break;

  case 44: /* print_element: STRING  */
#line 280 "bc.y"
                            {
			      generate ("O");
			      generate ((yyvsp[0].s_value));
			      free ((yyvsp[0].s_value));
			    }
#line 1773 "y.tab.c"
    break;

  case 45: /* print_element: expression  */
#line 286 "bc.y"
                            { generate ("P"); }
#line 1779 "y.tab.c"
    break;

  case 47: /* $@9: %empty  */
#line 290 "bc.y"
                            {
			      warn ("else clause in if statement");
			      (yyvsp[0].i_value) = next_label++;
			      sprintf (genstr, "J%d:N%1d:", (yyvsp[0].i_value), if_label); 
			      generate (genstr);
			      if_label = (yyvsp[0].i_value);
			    }
#line 1791 "y.tab.c"
    break;

  case 49: /* $@10: %empty  */
#line 300 "bc.y"
                            {
			      /* Check auto list against parameter list? */
			      check_params ((yyvsp[-4].a_value),(yyvsp[0].a_value));
			      sprintf (genstr, "F%d,%s.%s[",
				       lookup((yyvsp[-6].s_value),FUNCTDEF), 
				       arg_str ((yyvsp[-4].a_value)), arg_str ((yyvsp[0].a_value)));
			      generate (genstr);
			      free_args ((yyvsp[-4].a_value));
			      free_args ((yyvsp[0].a_value));
			      (yyvsp[-7].i_value) = next_label;
			      next_label = 1;
			    }
#line 1808 "y.tab.c"
    break;

  case 50: /* function: Define NAME '(' opt_parameter_list ')' '{' NEWLINE opt_auto_define_list $@10 statement_list '}'  */
#line 313 "bc.y"
                            {
			      generate ("0R]");
			      next_label = (yyvsp[-10].i_value);
			    }
#line 1817 "y.tab.c"
    break;

  case 51: /* opt_parameter_list: %empty  */
#line 319 "bc.y"
                            { (yyval.a_value) = NULL; }
#line 1823 "y.tab.c"
    break;

  case 53: /* opt_auto_define_list: %empty  */
#line 323 "bc.y"
                            { (yyval.a_value) = NULL; }
#line 1829 "y.tab.c"
    break;

  case 54: /* opt_auto_define_list: Auto define_list NEWLINE  */
#line 325 "bc.y"
                            { (yyval.a_value) = (yyvsp[-1].a_value); }
#line 1835 "y.tab.c"
    break;

  case 55: /* opt_auto_define_list: Auto define_list ';'  */
#line 327 "bc.y"
                            { (yyval.a_value) = (yyvsp[-1].a_value); }
#line 1841 "y.tab.c"
    break;

  case 56: /* define_list: NAME  */
#line 330 "bc.y"
                            { (yyval.a_value) = nextarg (NULL, lookup ((yyvsp[0].s_value),SIMPLE)); }
#line 1847 "y.tab.c"
    break;

  case 57: /* define_list: NAME '[' ']'  */
#line 332 "bc.y"
                            { (yyval.a_value) = nextarg (NULL, lookup ((yyvsp[-2].s_value),ARRAY)); }
#line 1853 "y.tab.c"
    break;

  case 58: /* define_list: define_list ',' NAME  */
#line 334 "bc.y"
                            { (yyval.a_value) = nextarg ((yyvsp[-2].a_value), lookup ((yyvsp[0].s_value),SIMPLE)); }
#line 1859 "y.tab.c"
    break;

  case 59: /* define_list: define_list ',' NAME '[' ']'  */
#line 336 "bc.y"
                            { (yyval.a_value) = nextarg ((yyvsp[-4].a_value), lookup ((yyvsp[-2].s_value),ARRAY)); }
#line 1865 "y.tab.c"
    break;

  case 60: /* opt_argument_list: %empty  */
#line 339 "bc.y"
                            { (yyval.a_value) = NULL; }
#line 1871 "y.tab.c"
    break;

  case 62: /* argument_list: expression  */
#line 343 "bc.y"
                            {
			      if ((yyvsp[0].i_value) > 1) warn ("comparison in argument");
			      (yyval.a_value) = nextarg (NULL,0);
			    }
#line 1880 "y.tab.c"
    break;

  case 63: /* argument_list: NAME '[' ']'  */
#line 348 "bc.y"
                            {
			      sprintf (genstr, "K%d:", -lookup ((yyvsp[-2].s_value),ARRAY));
			      generate (genstr);
			      (yyval.a_value) = nextarg (NULL,1);
			    }
#line 1890 "y.tab.c"
    break;

  case 64: /* argument_list: argument_list ',' expression  */
#line 354 "bc.y"
                            {
			      if ((yyvsp[0].i_value) > 1) warn ("comparison in argument");
			      (yyval.a_value) = nextarg ((yyvsp[-2].a_value),0);
			    }
#line 1899 "y.tab.c"
    break;

  case 65: /* argument_list: argument_list ',' NAME '[' ']'  */
#line 359 "bc.y"
                            {
			      sprintf (genstr, "K%d:", -lookup ((yyvsp[-2].s_value),ARRAY));
			      generate (genstr);
			      (yyval.a_value) = nextarg ((yyvsp[-4].a_value),1);
			    }
#line 1909 "y.tab.c"
    break;

  case 66: /* opt_expression: %empty  */
#line 366 "bc.y"
                            {
			      (yyval.i_value) = -1;
			      warn ("Missing expression in for statement");
			    }
#line 1918 "y.tab.c"
    break;

  case 68: /* return_expression: %empty  */
#line 373 "bc.y"
                            {
			      (yyval.i_value) = 0;
			      generate ("0");
			    }
#line 1927 "y.tab.c"
    break;

  case 69: /* return_expression: expression  */
#line 378 "bc.y"
                            {
			      if ((yyvsp[0].i_value) > 1)
				warn ("comparison in return expresion");
			    }
#line 1936 "y.tab.c"
    break;

  case 70: /* $@11: %empty  */
#line 384 "bc.y"
                            {
			      if ((yyvsp[0].c_value) != '=')
				{
				  if ((yyvsp[-1].i_value) < 0)
				    sprintf (genstr, "DL%d:", -(yyvsp[-1].i_value));
				  else
				    sprintf (genstr, "l%d:", (yyvsp[-1].i_value));
				  generate (genstr);
				}
			    }
#line 1951 "y.tab.c"
    break;

  case 71: /* expression: named_expression ASSIGN_OP $@11 expression  */
#line 395 "bc.y"
                            {
			      if ((yyvsp[0].i_value) > 1) warn("comparison in assignment");
			      if ((yyvsp[-2].c_value) != '=')
				{
				  sprintf (genstr, "%c", (yyvsp[-2].c_value));
				  generate (genstr);
				}
			      if ((yyvsp[-3].i_value) < 0)
				sprintf (genstr, "S%d:", -(yyvsp[-3].i_value));
			      else
				sprintf (genstr, "s%d:", (yyvsp[-3].i_value));
			      generate (genstr);
			      (yyval.i_value) = 0;
			    }
#line 1970 "y.tab.c"
    break;

  case 72: /* $@12: %empty  */
#line 411 "bc.y"
                            {
			      warn("&& operator");
			      (yyvsp[0].i_value) = next_label++;
			      sprintf (genstr, "DZ%d:p", (yyvsp[0].i_value));
			      generate (genstr);
			    }
#line 1981 "y.tab.c"
    break;

  case 73: /* expression: expression AND $@12 expression  */
#line 418 "bc.y"
                            {
			      sprintf (genstr, "DZ%d:p1N%d:", (yyvsp[-2].i_value), (yyvsp[-2].i_value));
			      generate (genstr);
			      (yyval.i_value) = (yyvsp[-3].i_value) | (yyvsp[0].i_value);
			    }
#line 1991 "y.tab.c"
    break;

  case 74: /* $@13: %empty  */
#line 424 "bc.y"
                            {
			      warn("|| operator");
			      (yyvsp[0].i_value) = next_label++;
			      sprintf (genstr, "B%d:", (yyvsp[0].i_value));
			      generate (genstr);
			    }
#line 2002 "y.tab.c"
    break;

  case 75: /* expression: expression OR $@13 expression  */
#line 431 "bc.y"
                            {
			      int tmplab;
			      tmplab = next_label++;
			      sprintf (genstr, "B%d:0J%d:N%d:1N%d:",
				       (yyvsp[-2].i_value), tmplab, (yyvsp[-2].i_value), tmplab);
			      generate (genstr);
			      (yyval.i_value) = (yyvsp[-3].i_value) | (yyvsp[0].i_value);
			    }
#line 2015 "y.tab.c"
    break;

  case 76: /* expression: NOT expression  */
#line 440 "bc.y"
                            {
			      (yyval.i_value) = (yyvsp[0].i_value);
			      warn("! operator");
			      generate ("!");
			    }
#line 2025 "y.tab.c"
    break;

  case 77: /* expression: expression REL_OP expression  */
#line 446 "bc.y"
                            {
			      (yyval.i_value) = 3;
			      switch (*((yyvsp[-1].s_value)))
				{
				case '=':
				  generate ("=");
				  break;

				case '!':
				  generate ("#");
				  break;

				case '<':
				  if ((yyvsp[-1].s_value)[1] == '=')
				    generate ("{");
				  else
				    generate ("<");
				  break;

				case '>':
				  if ((yyvsp[-1].s_value)[1] == '=')
				    generate ("}");
				  else
				    generate (">");
				  break;
				}
			    }
#line 2057 "y.tab.c"
    break;

  case 78: /* expression: expression '+' expression  */
#line 474 "bc.y"
                            {
			      generate ("+");
			      (yyval.i_value) = (yyvsp[-2].i_value) | (yyvsp[0].i_value);
			    }
#line 2066 "y.tab.c"
    break;

  case 79: /* expression: expression '-' expression  */
#line 479 "bc.y"
                            {
			      generate ("-");
			      (yyval.i_value) = (yyvsp[-2].i_value) | (yyvsp[0].i_value);
			    }
#line 2075 "y.tab.c"
    break;

  case 80: /* expression: expression MUL_OP expression  */
#line 484 "bc.y"
                            {
			      genstr[0] = (yyvsp[-1].c_value);
			      genstr[1] = 0;
			      generate (genstr);
			      (yyval.i_value) = (yyvsp[-2].i_value) | (yyvsp[0].i_value);
			    }
#line 2086 "y.tab.c"
    break;

  case 81: /* expression: expression '^' expression  */
#line 491 "bc.y"
                            {
			      generate ("^");
			      (yyval.i_value) = (yyvsp[-2].i_value) | (yyvsp[0].i_value);
			    }
#line 2095 "y.tab.c"
    break;

  case 82: /* expression: '-' expression  */
#line 496 "bc.y"
                            {
			      generate ("n");
			      (yyval.i_value) = (yyvsp[0].i_value);
			    }
#line 2104 "y.tab.c"
    break;

  case 83: /* expression: named_expression  */
#line 501 "bc.y"
                            {
			      (yyval.i_value) = 1;
			      if ((yyvsp[0].i_value) < 0)
				sprintf (genstr, "L%d:", -(yyvsp[0].i_value));
			      else
				sprintf (genstr, "l%d:", (yyvsp[0].i_value));
			      generate (genstr);
			    }
#line 2117 "y.tab.c"
    break;

  case 84: /* expression: NUMBER  */
#line 510 "bc.y"
                            {
			      int len = strlen((yyvsp[0].s_value));
			      (yyval.i_value) = 1;
			      if (len == 1 && *(yyvsp[0].s_value) == '0')
				generate ("0");
			      else if (len == 1 && *(yyvsp[0].s_value) == '1')
				generate ("1");
			      else
				{
				  generate ("K");
				  generate ((yyvsp[0].s_value));
				  generate (":");
				}
			      free ((yyvsp[0].s_value));
			    }
#line 2137 "y.tab.c"
    break;

  case 85: /* expression: '(' expression ')'  */
#line 526 "bc.y"
                            { (yyval.i_value) = (yyvsp[-1].i_value) | 1; }
#line 2143 "y.tab.c"
    break;

  case 86: /* expression: NAME '(' opt_argument_list ')'  */
#line 528 "bc.y"
                            {
			      (yyval.i_value) = 1;
			      if ((yyvsp[-1].a_value) != NULL)
				{ 
				  sprintf (genstr, "C%d,%s:",
					   lookup ((yyvsp[-3].s_value),FUNCT),
					   call_str ((yyvsp[-1].a_value)));
				  free_args ((yyvsp[-1].a_value));
				}
			      else
				{
				  sprintf (genstr, "C%d:", lookup ((yyvsp[-3].s_value),FUNCT));
				}
			      generate (genstr);
			    }
#line 2163 "y.tab.c"
    break;

  case 87: /* expression: INCR_DECR named_expression  */
#line 544 "bc.y"
                            {
			      (yyval.i_value) = 1;
			      if ((yyvsp[0].i_value) < 0)
				{
				  if ((yyvsp[-1].c_value) == '+')
				    sprintf (genstr, "DA%d:L%d:", -(yyvsp[0].i_value), -(yyvsp[0].i_value));
				  else
				    sprintf (genstr, "DM%d:L%d:", -(yyvsp[0].i_value), -(yyvsp[0].i_value));
				}
			      else
				{
				  if ((yyvsp[-1].c_value) == '+')
				    sprintf (genstr, "i%d:l%d:", (yyvsp[0].i_value), (yyvsp[0].i_value));
				  else
				    sprintf (genstr, "d%d:l%d:", (yyvsp[0].i_value), (yyvsp[0].i_value));
				}
			      generate (genstr);
			    }
#line 2186 "y.tab.c"
    break;

  case 88: /* expression: named_expression INCR_DECR  */
#line 563 "bc.y"
                            {
			      (yyval.i_value) = 1;
			      if ((yyvsp[-1].i_value) < 0)
				{
				  sprintf (genstr, "DL%d:x", -(yyvsp[-1].i_value));
				  generate (genstr); 
				  if ((yyvsp[0].c_value) == '+')
				    sprintf (genstr, "A%d:", -(yyvsp[-1].i_value));
				  else
				      sprintf (genstr, "M%d:", -(yyvsp[-1].i_value));
				}
			      else
				{
				  sprintf (genstr, "l%d:", (yyvsp[-1].i_value));
				  generate (genstr);
				  if ((yyvsp[0].c_value) == '+')
				    sprintf (genstr, "i%d:", (yyvsp[-1].i_value));
				  else
				    sprintf (genstr, "d%d:", (yyvsp[-1].i_value));
				}
			      generate (genstr);
			    }
#line 2213 "y.tab.c"
    break;

  case 89: /* expression: Length '(' expression ')'  */
#line 586 "bc.y"
                            { generate ("cL"); (yyval.i_value) = 1;}
#line 2219 "y.tab.c"
    break;

  case 90: /* expression: Sqrt '(' expression ')'  */
#line 588 "bc.y"
                            { generate ("cR"); (yyval.i_value) = 1;}
#line 2225 "y.tab.c"
    break;

  case 91: /* expression: Scale '(' expression ')'  */
#line 590 "bc.y"
                            { generate ("cS"); (yyval.i_value) = 1;}
#line 2231 "y.tab.c"
    break;

  case 92: /* expression: Read '(' ')'  */
#line 592 "bc.y"
                            {
			      warn ("read function");
			      generate ("cI"); (yyval.i_value) = 1;
			    }
#line 2240 "y.tab.c"
    break;

  case 93: /* named_expression: NAME  */
#line 598 "bc.y"
                            { (yyval.i_value) = lookup((yyvsp[0].s_value),SIMPLE); }
#line 2246 "y.tab.c"
    break;

  case 94: /* named_expression: NAME '[' expression ']'  */
#line 600 "bc.y"
                            {
			      if ((yyvsp[-1].i_value) > 1) warn("comparison in subscript");
			      (yyval.i_value) = lookup((yyvsp[-3].s_value),ARRAY);
			    }
#line 2255 "y.tab.c"
    break;

  case 95: /* named_expression: Ibase  */
#line 605 "bc.y"
                            { (yyval.i_value) = 0; }
#line 2261 "y.tab.c"
    break;

  case 96: /* named_expression: Obase  */
#line 607 "bc.y"
                            { (yyval.i_value) = 1; }
#line 2267 "y.tab.c"
    break;

  case 97: /* named_expression: Scale  */
#line 609 "bc.y"
                            { (yyval.i_value) = 2; }
#line 2273 "y.tab.c"
    break;

  case 98: /* named_expression: Last  */
#line 611 "bc.y"
                            { (yyval.i_value) = 3;
			      warn ("Last variable");
			    }
#line 2281 "y.tab.c"
    break;


#line 2285 "y.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 615 "bc.y"

