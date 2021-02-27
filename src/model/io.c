Model* loadmdl(FILE *fp) {
	Model* ret = (Model*)malloc(sizeof(Model));
	//
	ret->times = 0;
	fread(&ret->vars, sizeof(size_t), 1, fp);
	//
	ret->Mvar = (Ts**)malloc(ret->vars * sizeof(Ts*));
	ret->Mgrad = (Ts**)malloc(ret->vars * sizeof(Ts*));
	//Main vars & grads
	for (size_t i=0; i < ret->vars; i++) {
		ret->Mvar[i] = readts(fp);
		ret->Mgrad[i] = cpyts(ret->Mvar[i]);
		fillts(ret->Mgrad[i], 0);
	}
	//
	return ret;
};

Model* cpymdl(Model *mdl) {
	Model* ret = (Model*)malloc(sizeof(Model));
	//
	ret->times = mdl->times;
	ret->vars = mdl->vars;
	//
	ret->Mvar = (Ts**)malloc(ret->vars * sizeof(Ts*));
	ret->Mgrad = (Ts**)malloc(ret->vars * sizeof(Ts*));
	//Main vars & grads
	for (size_t i=0; i < ret->vars; i++) {
		ret->Mvar[i] = cpyts(mdl->Mvar[i]);
		ret->Mgrad[i] = cpyts(ret->Mgrad[i]);
	}
	//Vars & Grads
	ret->var = (Ts***)malloc(ret->times * sizeof(Ts**));
	ret->grad = (Ts***)malloc(ret->times * sizeof(Ts**));
	//Main vars & grads
	for (size_t t=0; t < ret->times; t++) {
		ret->var[t] = (Ts**)malloc(ret->vars * sizeof(Ts*));
		ret->grad[t] = (Ts**)malloc(ret->vars * sizeof(Ts*));
		for (size_t i=0; i < ret->vars; i++) {
			ret->var[t][i] = cpyts(mdl->var[t][i]);
			ret->grad[t][i] = cpyts(mdl->grad[t][i]);
		}
	}
	//
	return ret;
};

void freemdl(Model *mdl) {
	for (size_t t=0; t < mdl->times; t++) {
		for (size_t i=0; i < mdl->vars; i++) {
			freets(mdl->var[t][i]);
			freets(mdl->grad[t][i]);
		}
		free(mdl->var[t]);
		free(mdl->grad[t]);
	}
	free(mdl->var);
	free(mdl->grad);
	//
	for (size_t i=0; i < mdl->vars; i++) {
		freets(mdl->Mvar[i]);
		freets(mdl->Mgrad[i]);
	}
	free(mdl->Mvar);
	free(mdl->Mgrad);
	//
	free(mdl);
};

void savemdl(Model *mdl, FILE *fp) {
	fread(&mdl->vars, sizeof(size_t), 1, fp);
	//Main vars & grads
	for (size_t i=0; i < mdl->vars; i++)
		writets(mdl->Mvar[i], fp);
};

Insts* loadinsts(FILE *fp) {
	Insts* ret = (Insts*)malloc(sizeof(Insts));
	fread(&ret->insts, sizeof(size_t), 1, fp);
	//
	fread(&ret->inputs, sizeof(size_t), 1, fp);
	fread(&ret->outputs, sizeof(size_t), 1, fp);
	ret->input = (size_t*)malloc(ret->inputs * sizeof(size_t));
	ret->output = (size_t*)malloc(ret->outputs * sizeof(size_t));
	fread(ret->input, sizeof(size_t), ret->inputs, fp);
	fread(ret->output, sizeof(size_t), ret->outputs, fp);
	//
	size_t len;
	ret->inst = (size_t**)malloc(ret->insts*sizeof(size_t*));
	for (size_t i=0; i < ret->insts; i++) {
		len = 0;
		//param's lenght:param's  where param's is (command id,real param's)
		fread(&len, sizeof(size_t), 1, fp);
		ret->inst[i] = (size_t*)malloc((len+1) * sizeof(size_t));
		ret->inst[i][0] = len;
		fread(ret->inst[i]+1, sizeof(size_t), len, fp);
	}
	//
	return ret;
};

Insts* cpyinsts(Insts *insts) {
	Insts* ret = (Insts*)malloc(sizeof(Insts));
	//
	memcpy(ret, insts, sizeof(Insts));
	//
	ret->input = (size_t*)malloc(ret->inputs * sizeof(size_t));
	ret->output = (size_t*)malloc(ret->outputs * sizeof(size_t));
	memcpy(ret->input, insts->input, sizeof(size_t)*ret->inputs);
	memcpy(ret->output, insts->output, sizeof(size_t)*ret->outputs);
	//
	ret->inst = (size_t**)malloc(ret->insts * sizeof(size_t*));
	for (size_t i=0; i < ret->insts; i++) {
		ret->inst[i] = (size_t*)malloc(sizeof(size_t)*insts->inst[i][0]);
		memcpy(ret->inst[i], insts->inst[i], sizeof(size_t)*insts->inst[i][0]);
	}
	//
	return ret;
};

void freeinsts(Insts *insts) {
	free(insts->input);
	free(insts->output);
	//
	for (size_t i=0; i < insts->insts; i++)
		free(insts->inst[i]);
	free(insts->inst);
	//
	free(insts);
};

void saveinsts(Insts *insts, FILE *fp) {
	fwrite(&insts->insts, sizeof(size_t), 1, fp);
	//
	fwrite(&insts->inputs, sizeof(size_t), 1, fp);
	fwrite(&insts->outputs, sizeof(size_t), 1, fp);
	fwrite(insts->input, sizeof(size_t), insts->inputs, fp);
	fwrite(insts->output, sizeof(size_t), insts->outputs, fp);
	//
	for (size_t i=0; i < insts->insts; i++) {
		//param's lenght:param's  where param's is (command id,real param's)
		fread(&insts->inst[i][0], sizeof(size_t), 1, fp);
		fread(insts->inst[i]+1, sizeof(size_t), insts->inst[i][0], fp);
	}
};