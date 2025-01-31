#ifndef LIE_NODETYPE_H
#define LIE_NODETYPE_H

#include "lie.h"

typedef struct symbol symbrec,* symblst;
typedef unsigned int strtype;
typedef short objclass;

/* Function pointer types - using fobject from lie.h */
typedef fobject f0object;  /* () -> object */
typedef fobject f1object;  /* (object) -> object */
typedef fobject f2object;  /* (object,object) -> object */
typedef fobject f3object;  /* (object,object,object) -> object */
typedef fobject f4object;  /* (object,object,object,object) -> object */
typedef fobject f5object;  /* (object,object,object,object,object) -> object */
typedef fobject f6object;  /* (object,object,object,object,object,object) -> object */

typedef struct
{ strtype name;
  int line;
  char *fname;
} *labeltp, labelrec;

typedef struct
{ int n_nodes;
  int n_labels;
  int n_args;
  symblst start_nodes;
  symblst start_args;
  labeltp start_labels;
  symbrec* top_expr;
  symbrec* top_arg;
} funcrec, *funclst;

typedef union { char* init_string; strtype name; labeltp label; int size;}
        atp;

struct symbol
{ objtype type;
  objclass class;
  atp a;
  strtype formal;
  union { object val; fobject f; symbrec* expr; funclst func; } data;
  symbrec* arglst;
  symbrec* next;
};

typedef object (*symbfobj) (symblst); 

enum
{ FUN     =0x0200   /* whether expression has function type */
, DEFAULT =0x0400   /* whether final argument defaults to group */
, ARR     =0x0800   /* Dummy list variable length arg */
};

enum
{ INTFUN=INTEGER|FUN
, VECFUN=VECTOR|FUN
, MATFUN=MATRIX|FUN
, VIDFUN=VOID|FUN
, INTLST=INTEGER|ARR
, VECLST=VECTOR|ARR
, MATLST=MATRIX|ARR
, VIDLST=VOID|ARR
, ARGLST=ARGTYPE|ARR
, GRPDFT=GROUP|DEFAULT
};

enum
{ VALUE, OPERATOR, FUNCTION, DUMMY, MAP, FUNCTION_COPIED, ARG, BLOCK=8 };

#endif /* LIE_NODETYPE_H */
