#ifndef LIE_MEMTYPE_H
#define LIE_MEMTYPE_H

#include "lie.h"

/* Use types from lie.h */

enum
{ UNKNOWN = 0x00
, TEKST   = 0x11 /* character based type */
  , INTEGER = 0x02, VECTOR  = 0x12, MATRIX  = 0x32 /* integer based types */
  , BIGINT  = 0x03
  , POLY    = 0x14
, SIMPGRP = 0x06, GROUP   = 0x16 /* group based types */
  , ERROR   = 0x08
  , ARGTYPE = 0x09
  , VOID    = 0x0A
};

/* Object type definitions */
struct intcel {
    objtype type;
    reftype nref;
    entry intval;
};

struct bigint {
    objtype type;
    reftype nref;
    short allocsize, size;
    digit *data;
};

struct vector {
    objtype type;
    reftype nref;
    lie_index ncomp;
    lie_index size;
    entry *compon;
};

struct matrix {
    objtype type;
    reftype nref;
    lie_index nrows, ncols;
    lie_index rowsize;
    entry **elm;
    bigint **null;
};

struct poly {
    objtype type;
    reftype nref;
    lie_index nrows, ncols;
    lie_index rowsize;
    entry **elm;
    bigint **coef;
};

struct simpgrp_struct {
    objtype type;
    reftype nref;
    char lietype;
    entry lierank;
    matrix* cartan,* icartan,* roots;
    vector* exponents,* level,* root_norm;
    simpgrp* nextgrp;
};

struct group {
    objtype type;
    reftype nref;
    lie_index ncomp, toraldim;
    simpgrp **liecomp;
};

struct tekst {
    objtype type;
    reftype nref;
    lie_index len;
    char *string;
};

union objcel {
    struct {
        objtype type;
        reftype nref;
    } any;
    intcel i;
    bigint b;
    vector v;
    matrix m;
    poly pl;
    simpgrp s;
    group g;
    tekst t;
};
#endif /* LIE_MEMTYPE_H */

