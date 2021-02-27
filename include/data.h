#pragma once

#include "model.h"

typedef struct {
	Ts* *input;
	Ts* *output;
} Line;

typedef struct {
	Line* **batch;	//batch[batch id][line id]->input[var id] or ->output
	//
	size_t batchs;
	size_t bsize;
	//
	size_t inputs, outputs;
} Data;

//IO
Data* loaddata(FILE *fp);
Data* loadcsv(FILE *fp, Model *mdl, Insts *insts, size_t bsize);
void freedata(Model *ses, Insts* insts, Data *data);
#include "../src/data/mem.c"

//Controle
void set_inputs(Model* mdl, Insts* insts, Line* line, size_t time);
//void set_errors(Model* mdl, Insts* insts, Line* line, Loss loss, size_t time);	//Make outputs gradients
//void compute_score(Model* mdl, Insts* insts, Batch* batch, Loss loss, size_t bsize, Scorefn scorefn);
//void compute_all_scores(Model* mdl, Insts* insts, Data* data, Loss loss, Scorefn scorefn);
//void sort_data(Data *data);		//better to worst
#include "../src/data/controle.c"

//Str
void plumdata(Model* mdl, Insts *insts, Data *data);
//void show_result(Session *ses, Data *data, Loss loss, Scorefn scorefn);
//void show_result_v_by_v(Session *ses, Data *data, Loss loss, Scorefn scorefn);
#include "../src/data/str.c"

//Score fns
//double sum_score(double* list, size_t len);
//double moy_score(double* list, size_t len);
//#include "../src/data/scorefns.c"

//Losss
//double mbe(double want, double get);	//Classic:mean bias error
//double mse(double want, double get);	//mean Square Error (n-how many) : MSE = (want_i - get_i)**2/n
//double mae(double want, double get);	//mean absolut Error (n-how many) : MSE = |want_i - get_i|/n
//double cel(double want, double get);	//Cross Entropy loss -(w*log(g) + (1-w)*log(1 - g))
//#include "../src/data/losss.c"