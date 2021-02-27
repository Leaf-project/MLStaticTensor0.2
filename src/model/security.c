void track_unused(Model* mdl, Insts* insts) {
	bool where_computed[mdl->vars];
	//
	for (size_t i=0; i < insts->insts; i++) {
		if (insts->inst[i][0] != 0) {
			if (mdl->Mvar[insts->inst[i][2]]->type == 0)
				where_computed[i] = true;
		} else {
			_Msg(WARRNING, "model/security.c:track_unused", "Inst #%li have 0 lenght", i);
		}
	}
	//
	bool isinput=false;
	for (size_t i=0; i < mdl->vars; i++) {
		for (size_t j=0; j < insts->inputs; j++)
			if (i == insts->input[j]) isinput = true;
		//
		if (!where_computed[i] && !isinput)
			mdl->Mvar[i]->type = 4;	//directly set to unused
		//
		isinput=false;
	}
};