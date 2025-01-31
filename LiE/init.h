

extern object defaultgrp, grp;
extern bigint* one,* null,* minus_one;
extern intcel* bool_false,* bool_true;
extern object nothing;

void init(void);
boolean Mat_null(object a);
boolean Vec_null(object a);
boolean Pol_null(object a);
poly *poly_one(lie_index r);
poly *poly_null(lie_index r);


