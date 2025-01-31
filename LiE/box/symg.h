

bigint* fac(lie_index n);

bigint* n_tableaux(entry* lambda, lie_index l);

bigint* Classord(entry* kappa, lie_index l);

lie_index check_part(entry* lambda, lie_index l);

vector* check_tabl(vector* v);

boolean Nextperm(entry* w, lie_index n);

boolean Nextpart(entry* lambda, lie_index l);

boolean Nexttableau(entry* t, lie_index n);

matrix* Permutations(entry* v,lie_index n);

matrix* Partitions(lie_index n);

matrix* Tableaux(entry* lambda, lie_index l);

vector* Trans_part(entry* lambda, lie_index l);

entry Sign_part(entry* lambda, lie_index l);

void Robinson_Schensted (entry* P, entry* Q, lie_index n, entry* sigma);

void Schensted_Robinson (entry* sigma, lie_index n, entry* P, entry* Q);

poly* MN_char(entry* lambda, lie_index l);

bigint* MN_char_val(entry* lambda, entry* mu, lie_index l, lie_index m);


