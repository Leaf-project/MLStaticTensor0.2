#pragma once

#include "tensor.h"

typedef struct {
	//Array of Instructions (Math code)
	size_t insts;	//each command have a length (can be variable or even mutable)
	size_t **inst;	/*[cmd len][cmd id][param id's]	(in file lenght of param's is stored, but not in inst)
	Forward is just execution from 0 to last.
	Backward is execution from last to 0.
	*/
	size_t inputs, outputs;	//How many inputs and outputs (lenght of Code.input & Code.output arrays)
	//
	size_t *input, *output;	//Whitch id correspond tho 0'th input, 1'rst input ...
} Insts;	//Instructions

typedef struct {
	//Main
	Ts* *Mvar;	//	Main vars
	Ts* *Mgrad;	//	Main grads; it will be sum (or other) of grad
	//Times (temporary-like)
	Ts* **var;	//	var[time][var id]
	Ts* **grad;	//	1st order gradient [time][var id]
	//
	size_t times;	//lenght of {var & grad} arrays
	size_t vars;	//lenght of {var[time] & grad[time]} arrays
	//
	size_t grnd;	//global srand of model
} Model;

//Security Mesure
void _track_unused(Model *mdl, Insts *insts);	//track unused tensors in Instructions and set thoes vars to unused type
//void _track_nan(Model *mdl);	//Replace -nan by -1e6 and +nan by 1e6
#include "../src/model/security.c"

//Memory io
Model* loadmdl(FILE *fp);	//Load only one grad
Model* cpymdl(Model *mdl);
void freemdl(Model *mdl);
void savemdl(Model *mdl, FILE *fp);
//
Insts* loadinsts(FILE *fp);
Insts* cpyinsts(Insts *insts);
void freeinsts(Insts *insts);
void saveinsts(Insts *insts, FILE *fp);
/*Encrypted
Model* loadmdl(FILE *fp);	//Load only one grad
Model* cpymdl(Model *mdl);
void freemdl(Model *mdl);
void savemdl(Model *mdl, FILE *fp);
//
Insts* loadinsts(FILE *fp);
Insts* cpyinsts(Insts *insts);
void freeinsts(Insts *insts);
void saveinsts(Insts *insts, FILE *fp);*/
#include "../src/model/io.c"

//Controle
bool rnd_noise_cells(Model *mdl, size_t time);
bool clear(Model *mdl);	//clear from 1 to ses->times (not the first)
bool expand(Model *mdl, size_t n);
bool sum_grads(Model* mdl);	//sum only weights gradients
#include "../src/model/controle.c"

//Str
void plumvars(Model *mdl, Insts *insts);
void plumgrads(Model *mdl, Insts *insts);
void plummdl(Model *mdl, Insts *insts);
void pluminsts(Insts *insts, Model *mdl);
#include "../src/model/str.c"

//Compute
bool forward(Model *mdl, Insts *insts, size_t time);
bool backward_1ord(Model *mdl, Insts *insts, size_t time);
#include "../src/model/compute.c"