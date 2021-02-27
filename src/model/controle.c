bool rnd_noise_cells(Model *mdl, size_t time) {

};

bool clear(Model *mdl) {
	for (size_t t=0; t < mdl->times; t++) {
		for (size_t i=0; i < mdl->vars; i++) {
			if (!freets(mdl->var[t][i])) return false;
			if (!freets(mdl->grad[t][i])) return false;
		}
		free(mdl->var[t]);
		free(mdl->grad[t]);
	}
	free(mdl->var);
	free(mdl->grad);
	//
	mdl->times = 0;
	//
	return true;
};

bool expand(Model *mdl, size_t n) {
	mdl->var = (Ts***)realloc(mdl->var, sizeof(Ts**) * (mdl->times + n));
	mdl->grad = (Ts***)realloc(mdl->grad, sizeof(Ts**) * (mdl->times + n));
	//
	for (size_t i=mdl->times; i < mdl->times+n; i++) {
		mdl->var[i] = (Ts**)malloc(sizeof(Ts*)*mdl->vars);
		mdl->grad[i] = (Ts**)malloc(sizeof(Ts*)*mdl->vars);
		//
		for (size_t j=0; j < mdl->vars; j++) {
			switch (mdl->Mvar[j]->type) {
				case 0:
					mdl->var[i][j] = cpytsnull(mdl->Mvar[j]);
					mdl->grad[i][j] = cpytsnull(mdl->Mgrad[j]);
					break;
				case 1:
					if (mdl->times > 0) {
						mdl->var[i][j] = cpyts(mdl->var[i-1][j]);
						mdl->grad[i][j] = cpyts(mdl->grad[i-1][j]);
					} else {
						mdl->var[i][j] = cpyts(mdl->Mvar[j]);
						mdl->grad[i][j] = cpyts(mdl->Mgrad[j]);
					}
					break;
				case 2:case 3:case 4:
					mdl->var[i][j] = cpyts(mdl->Mvar[j]);
					mdl->grad[i][j] = cpyts(mdl->Mgrad[j]);
					break;
				default:
					_Msg(WARRNING, "model/controle.c:expand", "Type %li isn't recognised", mdl->Mvar[j]->type);
					break;
			}
		}
	}
	//
	mdl->times += n;
};

bool sum_grads(Model* mdl) {
	for (size_t t=0; t < mdl->times; t++) {
		for (size_t v=0; v < mdl->vars; v++) {
			for (size_t i=0; i < mdl->Mvar[v]->len; i++) {
				STSPOS(mdl->Mgrad[v], i) += GTSPOS(mdl->var[t][v], i);
			}
		}
	}
};