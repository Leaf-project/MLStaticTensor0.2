void set_inputs(Model* mdl, Insts* insts, Line* line, size_t time) {
	if (time >= mdl->times) _Msg(ERROR, "data/controle.c:set_inputs", "Raise : time >= mdl->times");
	for (size_t i=0; i < insts->inputs; i++)
		cpyarr(mdl->var[time][insts->input[i]], line->input[i]);
};

/*
void set_errors(Model* mdl, Insts* insts, Line* line, Loss loss, size_t time) {

};

void compute_score(Model* mdl, Insts* insts, Batch* batch, Loss loss, size_t bsize, Scorefn scorefn) {

};

void compute_all_scores(Model* mdl, Insts* insts, Data* data, Loss loss, Scorefn scorefn) {

};

void sort_data(Data *data) {

};*/