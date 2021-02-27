Ts* func(Ts *x, Fn fn) {
	double* lst = (double*)malloc(x->len * sizeof(double));
	for (size_t i=0; i < x->len; i++) lst[i] = fn(x->arr[i]);
	Ts* ret = cpyts(x);
	filltslist(ret, lst);
	free(lst);
	return ret;
};

bool __func(Ts* y, Ts* x, Fn fn) {
	for (size_t i=0; i < y->len; i++) {
		STSPOS(y, i) = fn(GTSPOS(x, i));
	}
	return true;
};

double _identity(double x) {
	return x;
};

double _bin_step(double x) {
	if (x < 0) x = 0;
	else x = 1;
	return x;
};

double _logistic(double x) {	// 1 / (1 + e**(-x))
	//printf("logistic : %f\n", x);
	return 1 / (1 + exp(-x));
};

double _tanh(double x) {
	//printf("tanh : %f\n", x);
	return tanh(x);
};

double _ReLU(double x) {
	if (x <= 0) return 0;
	else return x;
};

double _GELU(double x) {
	return 0.5*x + 0.5*x*tanh(pow(2/M_PI, 0.5) * (x + 0.044715*pow(x, 3)));
};

double _SoftPlus(double x) {
	return log(1 + exp(x));
};

double _ELU(double x) {
	double a=1;	//(-a,inf)
	if (x <= 0) return a*(exp(x)-1);
	else return x;
};

double _SELU(double x) {
	double lambda=1.0507, alpha=1.67326;
	if (x <= 0) return lambda*alpha*(exp(x)-1);
	else return lambda*x;
};

double _Leaky_ReLU(double x) {
	if (x < 0) return 0.01*x;
	else return x;
};

double _Softsign(double x) {
	return x / (1 + fabs(x));
};

double _SQNL(double x) {
	if (x > 2) return 1;
	else if (x >= 0 && x <= 2) return x - pow(x, 2)/4;
	else if (x < 0 && -2 <= x) return x + pow(x, 2)/4;
	else return -1;
};

double _Bent_Ind(double x) {
	return (pow(pow(x,2) + 1,0.5) - 1)/2 + x;
};

double _Gauss(double x) {
	return exp(-pow(x, 2));
}

double _SQ_RBF(double x) {
	if (fabs(x) <= 1) return 1 - pow(x, 2)/2;
	else if (1 < fabs(x) && fabs(x) < 2) return pow(2 - fabs(x), 2)/2;
	else return 0;
};

#define MACROS(fn) {return func(x[0], &fn);};
Ts* identity(Ts **x, size_t n)		MACROS(_identity)
Ts* bin_step(Ts **x, size_t n)		MACROS(_bin_step)
Ts* logistic(Ts **x, size_t n)		MACROS(_logistic)
Ts* Tanh(Ts **x, size_t n) 			MACROS(_tanh)
Ts* relu(Ts **x, size_t n)			MACROS(_ReLU)
Ts* gelu(Ts **x, size_t n)			MACROS(_GELU)
Ts* softplus(Ts **x, size_t n) 		MACROS(_SoftPlus)
Ts* elu(Ts **x, size_t n)			MACROS(_ELU)
Ts* selu(Ts **x, size_t n)			MACROS(_SELU)
Ts* leaky_relu(Ts **x, size_t n)	MACROS(_Leaky_ReLU)
Ts* softsign(Ts **x, size_t n)		MACROS(_Softsign)
Ts* sqnl(Ts **x, size_t n)			MACROS(_SQNL)
Ts* bent_ind(Ts **x, size_t n)		MACROS(_Bent_Ind)
Ts* gauss(Ts **x, size_t n)			MACROS(_Gauss)
Ts* sq_rbf(Ts **x, size_t n)		MACROS(_SQ_RBF)
#undef MACROS

#define MACROS(fn) {return __func(y, x[0], &fn);};
bool __identity(Ts* y, Ts **x, size_t n)	MACROS(_identity)
bool __bin_step(Ts* y, Ts **x, size_t n)	MACROS(_bin_step)
bool __logistic(Ts* y, Ts **x, size_t n)	MACROS(_logistic)
bool __Tanh(Ts* y, Ts **x, size_t n) 		MACROS(_tanh)
bool __relu(Ts* y, Ts **x, size_t n)		MACROS(_ReLU)
bool __gelu(Ts* y, Ts **x, size_t n)		MACROS(_GELU)
bool __softplus(Ts* y, Ts **x, size_t n) 	MACROS(_SoftPlus)
bool __elu(Ts* y, Ts **x, size_t n)			MACROS(_ELU)
bool __selu(Ts* y, Ts **x, size_t n)		MACROS(_SELU)
bool __leaky_relu(Ts* y, Ts **x, size_t n)	MACROS(_Leaky_ReLU)
bool __softsign(Ts* y, Ts **x, size_t n)	MACROS(_Softsign)
bool __sqnl(Ts* y, Ts **x, size_t n)		MACROS(_SQNL)
bool __bent_ind(Ts* y, Ts **x, size_t n)	MACROS(_Bent_Ind)
bool __gauss(Ts* y, Ts **x, size_t n)		MACROS(_Gauss)
bool __sq_rbf(Ts* y, Ts **x, size_t n)		MACROS(_SQ_RBF)
#undef MACROS

Ts* softmax(Ts **x, size_t n) {
	double *lst = (double*)malloc(x[0]->len * sizeof(double));
	//
	size_t i;
	double tmp = 0.0;	//here max
	for (i=0; i < x[0]->len; i++) if (x[0]->arr[i] > tmp) tmp = x[0]->arr[i];
	//
	for (i=0; i < x[0]->len; i++) lst[i] = exp(x[0]->arr[i]-tmp);
	//	here sum
	tmp = 0.0;
	for (i=0; i < x[0]->len; i++) tmp += lst[i];
	for (i=0; i < x[0]->len; i++) lst[i] /= tmp;
	//
	Ts *ret = cpyts(x[0]);
	filltslist(ret, lst);
	free(lst);
	return ret;
};

Ts* maxout(Ts **x, size_t n) {
	size_t i;
	double *lst = (double*)malloc(x[0]->len * sizeof(double));
	size_t max=0;	//here max
	for (i=0; i < x[0]->len; i++) if (x[0]->arr[i] > max) max = x[0]->arr[i];
	//
	for (i=0; i < x[0]->len; i++) lst[i] = x[0]->arr[max];
	Ts *ret = cpyts(x[0]);
	filltslist(ret, lst);
	free(lst);
	return ret;
};

Ts* seuil(Ts** x, size_t n) {

};

Ts* condition_like(Ts** x, size_t n) {

};

bool __softmax(Ts* y, Ts** x, size_t n) {
	return 1;
};

bool __maxout(Ts* y, Ts** x, size_t n) {
	return 1;
}
//Complex
bool __seuil(Ts* y, Ts** x, size_t n) {
	return 1;
};

bool __condition_like(Ts* y, Ts** x, size_t n) {
	return 1;
};