double d_identity(double x) {
	(void)x;
	return 1.0;
};

double d_bin_step(double x) {
	(void)x;
	return 0.0;
};

double d_logistic(double x) {
	double a = _logistic(x);
	return a*(1 - a);
};

double d_tanh(double x) {
	return 1.0 - pow(tanh(x), 2);
};

double d_ReLU(double x) {
	if (x < 0) return 0;
	else return 1;
};

double d_GELU(double x) {
	double tmp0 = tanh(pow(2,0.5) * (0.044715*pow(x,3))/pow(M_PI, 0.5));
	return (pow(2,0.5)*x*(1-pow(tmp0, 2))*(0.134145*pow(x, 2) + 1)/pow(M_PI, 0.5) + tmp0 + 1) / 2;
};

double d_SoftPlus(double x) {
	return 1 / (1 + exp(-x));
};

double d_ELU(double x) {
	double a=1;
	if (x < 0) return a*exp(x);
	else return 1;
};

double d_SELU(double x) {
	double lambda=1.0507, alpha=1.67326;
	if (x < 0) return lambda * alpha * exp(x);
	else return lambda;
};

double d_Leaky_ReLU(double x) {
	if (x < 0) return 0.01;
	else return 1;
};

double d_Softsign(double x) {
	return 1 / pow(1 + fabs(x), 2);
};

double d_SQNL(double x) {
	return 1 + x/2;
};

double d_Bent_Ind(double x) {
	return x / (2 * pow(pow(x, 2)+1, 0.5)) + 1;
};

double d_Gauss(double x) {
	return -2*x*exp(-pow(x, 2));
};

double d_SQ_RBF(double x) {
	if (fabs(x) <= 1) return -x;
	else if (1 < fabs(x) && fabs(x) < 2) return x - 2*sign(x);
	else return 0;
};

//
#define MACROS(fn) {return func(x[0], &fn);};
Ts* didentity(Ts **x, size_t n)		MACROS(d_identity)
Ts* dbin_step(Ts **x, size_t n)		MACROS(d_bin_step)
Ts* dlogistic(Ts **x, size_t n)		MACROS(d_logistic)
Ts* dTanh(Ts **x, size_t n) 		MACROS(d_tanh)
Ts* drelu(Ts **x, size_t n)			MACROS(d_ReLU)
Ts* dgelu(Ts **x, size_t n)			MACROS(d_GELU)
Ts* dsoftplus(Ts **x, size_t n) 	MACROS(d_SoftPlus)
Ts* delu(Ts **x, size_t n)			MACROS(d_ELU)
Ts* dselu(Ts **x, size_t n)			MACROS(d_SELU)
Ts* dleaky_relu(Ts **x, size_t n)	MACROS(d_Leaky_ReLU)
Ts* dsoftsign(Ts **x, size_t n)		MACROS(d_Softsign)
Ts* dsqnl(Ts **x, size_t n)			MACROS(d_SQNL)
Ts* dbent_ind(Ts **x, size_t n)		MACROS(d_Bent_Ind)
Ts* dgauss(Ts **x, size_t n)		MACROS(d_Gauss)
Ts* dsq_rbf(Ts **x, size_t n)		MACROS(d_SQ_RBF)
#undef MACROS
//

Ts* dsoftmax(Ts** x, size_t n) {
	/*
	def softmax(arr):
		arr = np.exp(arr - max(arr))
		return arr / sum(arr)

	def dsoftmax(arr):
		arr = np.exp(arr - max(arr))
		sum = sum(arr)
		return arr * (sum - arr) / sum**2
	*/
	double *lst = (double*)malloc(x[0]->len * sizeof(double));
	//
	size_t i;
	double tmp = 0.0;	//here max
	for (i=0; i < x[0]->len; i++) if (x[0]->arr[i] > tmp) tmp = x[0]->arr[i];
	//
	for (i=0; i < x[0]->len; i++) lst[i] = exp(x[0]->arr[i] - tmp);
	//	here sum
	tmp = 0.0;
	for (i=0; i < x[0]->len; i++) tmp += lst[i];
	for (i=0; i < x[0]->len; i++) lst[i]  = lst[i] * (tmp - lst[i]) / pow(tmp, 2);
	//
	Ts *ret = cpyts(x[0]);
	filltslist(ret, lst);
	free(lst);
	return ret;
};

Ts* dmaxout(Ts **x, size_t n) {
	size_t i;
	double* lst = (double*)malloc(x[0]->len * sizeof(double));
	size_t max=0;	//here max
	for (i=0; i < x[0]->len; i++) if (x[0]->arr[i] > max) max = x[0]->arr[i];
	//
	for (i=0; i < x[0]->len; i++) {
		if (i == max) lst[i] = 1.0;
		else lst[i] = 0.0;
	};
	Ts *ret = cpyts(x[0]);
	filltslist(ret, lst);
	free(lst);
	return ret;
};

Ts* dseil(Ts** x, size_t n) {

};

Ts* dcondition_like(Ts** x, size_t n) {

};