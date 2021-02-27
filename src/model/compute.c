bool forward(Model *mdl, Insts *insts, size_t time) {
	//each var is already fill, just have to change arr
	#define INST insts->inst[i]
	//
	Ts* *xargs;
	for (size_t i=0; i < insts->insts; i++) {
		//printf("%li\n", i);
		switch (INST[1]) {
			case 0:case 1:case 2:case 3:case 4:		//line op
				if (INST[4] == 2) {
					if (!_lineop(
						mdl->var[time-INST[3]][INST[2]],	//y
						mdl->var[time-INST[6]][INST[5]],	//a
						mdl->var[time-INST[8]][INST[7]],	//b
						ops[INST[1]])) {
						_Msg(ERROR, "forward", "Fails line op");
						return false;
					}
				} else if (INST[4] > 2) {
					_lineop(
						mdl->var[time-INST[3]][INST[2]],	//y
						mdl->var[time-INST[6]][INST[5]],	//a
						mdl->var[time-INST[8]][INST[7]],	//b
						ops[INST[1]]);
					for (size_t n=2; n < INST[4]; n++) {
						_lineop(
							mdl->var[time-INST[3]][INST[2]],
							mdl->var[time-INST[3]][INST[2]],
							mdl->var[time-INST[6+n*2]][INST[5+n*2]],
							ops[INST[1]]);
					}
				} else {
					//n < 2
					_Msg(ERROR, "forward", "line op %s line %li: n < 2", str_ops[INST[1]]);
				};
				break;
			case 5:		//dot
				_dot(mdl->var[time-INST[3]][INST[2]],
					mdl->var[time-INST[5]][INST[4]],
					mdl->var[time-INST[7]][INST[6]]);
				break;
			case 6:case 7:case 8:case 9:case 10:	//iter 
				break;
			case 11:	//function
				//printf("%s\n", str_funcs[INST[4]]);
				xargs = (Ts**)malloc(INST[5] * sizeof(Ts*));
				//
				for (size_t j=0; j < INST[5]; j++)
					xargs[j] = mdl->var[time-INST[7+j*2]][INST[6+j*2]];
				//
				if (!__funcs[INST[4]](mdl->var[time-INST[3]][INST[2]], xargs, INST[5])) return false;
				free(xargs);
				break;
			case 12:	//transpose
				break;
			case 13:	//kernel convolution
				break;
			case 14:	//convolution
				break;
			case 15:	//Pooling
				break;
			case 16:	//Random
				break;
			case 17:	//Split
				break;
			case 18:	//Pyramid
				break;
			default:
				break;
		}
	}
	return true;
};

bool backward_1ord(Model *mdl, Insts *insts, size_t time) {

};