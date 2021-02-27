#pragma once

#include "tensor.h"

//Functions
typedef Ts* (*Func)(Ts**, size_t n);
typedef bool (*__Func)(Ts*, Ts**, size_t n);

//Repited Patern
Ts* func(Ts* x, Fn fn);
bool __func(Ts* y, Ts* x, Fn fn);
//Classic Activation functions
Ts* identity(Ts** x, size_t n);
Ts* bin_step(Ts** x, size_t n);
Ts* logistic(Ts** x, size_t n);
Ts* Tanh(Ts** x, size_t n);
Ts* relu(Ts** x, size_t n);
Ts* gelu(Ts** x, size_t n);
Ts* softplus(Ts** x, size_t n);
Ts* elu(Ts** x, size_t n);
Ts* selu(Ts** x, size_t n);
Ts* leaky_relu(Ts** x, size_t n);
Ts* softsign(Ts** x, size_t n);
Ts* sqnl(Ts** x, size_t n);
Ts* bent_ind(Ts** x, size_t n);
Ts* gauss(Ts** x, size_t n);
Ts* sq_rbf(Ts** x, size_t n);
//Spetials
Ts* softmax(Ts** x, size_t n);
Ts* maxout(Ts** x, size_t n);
//Complex
Ts* seuil(Ts** x, size_t n);
Ts* condition_like(Ts** x, size_t n);
//
//
bool __identity(Ts* y, Ts** x, size_t n);
bool __bin_step(Ts* y, Ts** x, size_t n);
bool __logistic(Ts* y, Ts** x, size_t n);
bool __Tanh(Ts* y, Ts** x, size_t n);
bool __relu(Ts* y, Ts** x, size_t n);
bool __gelu(Ts* y, Ts** x, size_t n);
bool __softplus(Ts* y, Ts** x, size_t n);
bool __elu(Ts* y, Ts** x, size_t n);
bool __selu(Ts* y, Ts** x, size_t n);
bool __leaky_relu(Ts* y, Ts** x, size_t n);
bool __softsign(Ts* y, Ts** x, size_t n);
bool __sqnl(Ts* y, Ts** x, size_t n);
bool __bent_ind(Ts* y, Ts** x, size_t n);
bool __gauss(Ts* y, Ts** x, size_t n);
bool __sq_rbf(Ts* y, Ts** x, size_t n);
//Spetials
bool __softmax(Ts* y, Ts** x, size_t n);
bool __maxout(Ts* y, Ts** x, size_t n);
//Complex
bool __seuil(Ts* y, Ts** x, size_t n);
bool __condition_like(Ts* y, Ts** x, size_t n); 
#include "../src/func/func.c"

Func funcs[20] = {identity, bin_step, logistic, Tanh, relu, gelu, softplus, elu, selu, leaky_relu, softsign, sqnl, bent_ind, gauss, sq_rbf, softmax, maxout, seuil, condition_like};
__Func __funcs[20] = {__identity, __bin_step, __logistic, __Tanh, __relu, __gelu, __softplus, __elu, __selu, __leaky_relu, __softsign, __sqnl, __bent_ind, __gauss, __sq_rbf, __softmax, __maxout, __seuil, __condition_like};

//1st order Derivative
Ts* didentity(Ts** x, size_t n);
Ts* dbin_step(Ts** x, size_t n);
Ts* dlogistic(Ts** x, size_t n);
Ts* dTanh(Ts** x, size_t n);
Ts* drelu(Ts** x, size_t n);
Ts* dgelu(Ts** x, size_t n);
Ts* dsoftplus(Ts** x, size_t n);
Ts* delu(Ts** x, size_t n);
Ts* dselu(Ts** x, size_t n);
Ts* dleaky_relu(Ts** x, size_t n);
Ts* dsoftsign(Ts** x, size_t n);
Ts* dsqnl(Ts** x, size_t n);
Ts* dbent_ind(Ts** x, size_t n);
Ts* dgauss(Ts** x, size_t n);
Ts* dsq_rbf(Ts** x, size_t n);
//
Ts* dsoftmax(Ts** x, size_t n);
Ts* dmaxout(Ts** x, size_t n);
//
Ts* dseil(Ts** x, size_t n);
Ts* dcondition_like(Ts** x, size_t n);
#include "../src/func/dfunc.c"

Func dfuncs[20] = {didentity, dbin_step, dlogistic, dTanh, drelu, dgelu, dsoftplus, delu, dselu, dleaky_relu, dsoftsign, dsqnl, dbent_ind, dgauss, dsq_rbf, dsoftmax, dmaxout, dseil, dcondition_like};

//Pool functions
double pool_max(Ts* ts) {
	double max = 0.0;
	for (size_t i=0; i < ts->len; i++) {
		if (GTSPOS(ts, i) > max) max = GTSPOS(ts, i);
	}
	return max;
};

double pool_mean(Ts* ts) {
	double ts_sum = 0.0;
	for (size_t i=0; i < ts->len; i++) {
		ts_sum += GTSPOS(ts, i);
	}
	return ts_sum / ts->len;
};