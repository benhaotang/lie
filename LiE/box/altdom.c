#include "lie.h"
#include "memtype.h"
#include "defs.h"

static poly* thorough_copy(poly* p) /* does freepol(p) */
{
  lie_index i;
  lie_index n;
  poly* result;
  bigint** b;
  
  n = p->nrows;
  result = private_pol(p);
  
  for (i=0; i<n; i++) {
    b = &result->coef[i];
    if (isshared(*b)>1) {
      clrshared(*b);
      *b = copybigint(*b,NULL);
      setshared(*b);
    }
  }
  return result;
}

poly* Alt_dom(poly* p)
{
  lie_index j;
  lie_index s;
  lie_index i;
  lie_index n;
  entry* v;
  bigint* c;
  
  s = Ssrank(grp); /* keep out of torus */
  if (!s) return p;
  p = thorough_copy(p);
  
  for (j=0; j<p->nrows; j++) {
    i = 0;
    n = 0;
    v = p->elm[j];
    c = p->coef[j];
    
    while(1) {
      while (v[i]>=0) {
        if (++i==s) goto finish_j;
      }
      if (++v[i]==0) {
        c->size=0;
        break;
      }
      w_refl(v,i);
      n++;
      v[i]--;
      i-=(i<2 ? i : 2);
    }
finish_j:
    if (n%2) c->size= -c->size;
  }
  return Reduce_pol(p);
}

/* the follwing code is largely copied (with permission) from weyl.c, which is
   now incorporated into weyl.w; shortly, thid file will be assimilated too */

/* This routine is large because we have taken the tests out of the loops */
#define loop(body)  while (n-->0) \
  { if (b= *c++,(d= *(v=(*m++)+k)+1)<0) { b->size= -b->size; body; *v= -1-d; }}
/* OK this is tricky, and we have to refrain from using the comma-operator in
   the body. But unless we slip in a closing brace after the body, we will be
   in if-else matching problems
*/

static void simp_alt_refls(poly* p, lie_index offset, lie_index i, simpgrp* g)
{
  bigint** c;
  entry** m;
  lie_index n;
  lie_index j;
  lie_index k;
  lie_index r;
  entry d;
  entry* v;
  bigint* b;
  lie_index temp_j;
  
  c = p->coef;
  m = p->elm;
  n = p->nrows;
  k = offset+i;
  r = g->lierank;
  
  for (j=0; j<n; j++) {
    if (m[j][k] == -1) {
      temp_j = j--;
      swap_terms(m,c,temp_j,--n);
      clrshared(c[n]);
    }
  }
  p->nrows = n;
  
  switch (g->lietype) {
    case 'A':
      if (i>0 && i<r-1) {
        loop ( v[-1]+=d; v[1]+=d )
      }
      else if (i==0) {
        if (r==1) {
          loop ({/* empty */} )
        }
        else {
          loop ( v[1]+=d )
        }
      }
      else { /* i==r-1 */
        loop ( v[-1]+=d )
      }
      break;
      
    case 'B':
      if (i>0 && i<r-2) {
        loop ( v[-1]+=d; v[1]+=d )
      }
      else if (i==0) {
        if (r==2) {
          loop ( v[1]+=2*d )
        }
        else {
          loop ( v[1]+=d )
        }
      }
      else if (i==r-1) {
        loop ( v[-1]+=d )
      }
      else { /* i==r-2 */
        loop ( v[-1]+=d; v[1]+=2*d )
      }
      break;
      
    case 'C':
      if (i>0 && i<r-1) {
        loop ( v[-1]+=d; v[1]+=d )
      }
      else if (i==0) {
        loop ( v[1]+=d )
      }
      else { /* i==r-1 */
        loop ( v[-1]+=2*d )
      }
      break;
      
    case 'D':
      if (i>0 && i<r-3) {
        loop ( v[-1]+=d; v[1]+=d )
      }
      else if (i==0) {
        if (r==3) {
          loop ( v[1]+=d; v[2]+=d )
        }
        else {
          loop ( v[1]+=d )
        }
      }
      else if (i==r-3) {
        loop ( v[-1]+=d; v[1]+=d; v[2]+=d )
      }
      else { /* i==r-2 or i==r-1 */
        lie_index j = r-3-i;
        loop ( v[j]+=d )
      }
      break;
      
    case 'E':
      if (i>3 && i<r-1) {
        loop ( v[-1]+=d; v[1]+=d )
      }
      else if (i==r-1) {
        loop ( v[-1]+=d )
      }
      else if (i<2) {
        loop ( v[2]+=d )
      }
      else if (i==2) {
        loop ( v[-2]+=d; v[1]+=d )
      }
      else { /* i==3 */
        loop ( v[-2]+=d; v[-1]+=d; v[1]+=d )
      }
      break;
      
    case 'F':
      switch(i) {
        case 0:
          loop ( v[1]+=d )
          break;
        case 1:
          loop ( v[-1]+=d; v[1]+=2*d )
          break;
        case 2:
          loop ( v[-1]+=d; v[1]+=d )
          break;
        case 3:
          loop ( v[-1]+=d )
          break;
      }
      break;
      
    case 'G':
      if (i==0) {
        loop ( v[1]+=d )
      }
      else {
        loop ( v[-1]+=3*d )
      }
      break;
  }
  Reduce_pol(p);
}

static void alt_refls(poly* p, lie_index nr)
{
  lie_index i;
  lie_index d;
  lie_index offset;
  
  if (type_of(grp)==SIMPGRP) {
    simp_alt_refls(p,0,nr,&grp->s);
  }
  else if (simpgroup(grp)) {
    simp_alt_refls(p,0,nr,Liecomp(grp,0));
  }
  else {
    offset = 0;
    for (i=0; nr>=(d=Liecomp(grp,i)->lierank); i++) {
      offset += d;
      nr -= d;
    }
    simp_alt_refls(p,offset,nr,Liecomp(grp,i));
  }
}

poly* Alt_dom_w(poly* p, vector* word)
{
  lie_index i;
  entry* w;
  entry wi;
  
  w = word->compon;
  p = thorough_copy(p);
  
  for (i=0; i<word->ncomp; i++) {
    wi = w[i];
    if(wi != 0) {
      alt_refls(p,wi-1);
    }
  }
  return p;
}

/* For Demazure we shall employ an ordering not used elsewhere, namely by
   first comparing one specific position, and if that is not decisive
   the remaining positions lexicographically
*/

static lie_index maindex;

static cmp_tp main_decr(entry* v, entry* w, lie_index len)
{
  if (v[maindex]!=w[maindex]) {
    return v[maindex]>w[maindex] ? 1 : -1;
  }
  while (len-->0) {
    if (*v++ != *w++) {
      return *--v > *--w ? 1 : -1;
    }
  }
  return 0;
}

static poly* demaz(poly* p, lie_index nr)
{
  lie_index i;
  lie_index k;
  lie_index pos;
  lie_index r;
  lie_index upb_size;
  poly* result;
  matrix* cart;
  entry* root;
  entry* x;
  cmpfn_tp sav_cmp;
  bigint* c;
  
  k = 0;
  r = p->ncols;
  upb_size = 0;
  cart = Cartan();
  root = cart->elm[nr];
  x = mkintarray(r);
  sav_cmp = cmpfn;
  cmpfn = main_decr;
  maindex = nr;
  
  alt_refls(p,nr); /* start by trying to cancel */
  
  for (i=0; i<p->nrows; i++) {
    upb_size += p->elm[i][nr]+1;
  }
  result = mkpoly(upb_size,r);
  
  for (i=0; i<p->nrows; i++) {
    if (p->coef[i]) { /* terms may get cancelled */
      c = p->coef[i];
      copyrow(p->elm[i],x,r);
      p->coef[i] = NULL;
      clrshared(c);
      
      do {
        copyrow(x,result->elm[k],r);
        result->coef[k] = c;
        setshared(c);
        k++;
        
        if (x[nr]==0) break; /* stop when reflection hyperplane is reached */
        
        copyrow(x,result->elm[k],r);
        result->coef[k] = c;
        setshared(c);
        w_refl(result->elm[k],nr); /* add weight reflected in hyperplane */
        k++;
        
        add_xrow_to(x,-1,root,r);
        if (x[nr]<0) break; /* stop if reflection hyperplane is passed */
        
        pos = searchterm(p,x);
        if (pos >= 0) {
          clrshared(p->coef[pos]);
          c = add(c,p->coef[pos]);
          p->coef[pos] = NULL;
        }
      } while (c->size!=0); /* stop if coefficients cancel exactly */
    }
  }
  
  freemem(p); /* coeff's already discarded */
  freemem(cart);
  freearr(x);
  cmpfn = sav_cmp;
  result->nrows = k;
  return result; /* terms are distinct but not sorted */
}

poly* Demazure(poly* p, vector* word)
{
  lie_index i;
  
  for (i=0; i<word->ncomp; i++) {
    p = demaz(thorough_copy(p),word->compon[i]-1);
  }
  return p;
}
