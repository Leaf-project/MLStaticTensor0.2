char is_in(size_t i, size_t* lst, size_t len) {
	for (size_t j=0; j < len; j++) if (i == lst[j]) return 1;
	return 0;
};

void _plum_tensors(Model *mdl, Insts *insts, Ts** M, Ts*** timed, const char* Head_Label, const char* color) {
	char*** strss = (char***)malloc((1 + mdl->times) * sizeof(char**));
	size_t lengths;
	//
	printf("\033[100m%s\033[0m\n", Head_Label);
	for (size_t i=0; i < mdl->vars; i++) {
		printf("\033[%sm#%s\033[0m \033[30m%li\033[0m", color, str_types[(size_t)M[i]->type], i);
		//
		printf(" \033[4;1;93;40m%s\033[0m", M[i]->label);
		if (is_in(i, insts->input, insts->inputs)) printf(" is \033[100minput\033[0m\n");
		else if (is_in(i, insts->output, insts->outputs)) printf(" is \033[100moutput\033[0m\n");
		else printf("\n");
		//
		//debbug_ptrmat(ses->var[0][i]);
		strss[0] = strts(M[i]);
		for (size_t t=0; t < mdl->times; t++) {
			strss[t+1] = strts(timed[t][i]);
		}
		lengths = lineslen(strss[0]);
		//
		for (size_t j=0; j < lengths; j++) {
			printf("%s||", strss[0][j]);
			for (size_t t=0; t < mdl->times; t++) {
				printf("%s|", strss[t+1][j]);
			}
			printf("\n");
		}
		//
		for (size_t t=0; t < mdl->times+1; t++) {
			for (size_t j=0; j < lengths; j++) {
				free(strss[t][j]);
			}
			free(strss[t]);
		}
	}
	free(strss);
};

void plumvars(Model *mdl, Insts *insts) {
	_plum_tensors(mdl, insts, mdl->Mvar, mdl->var, "VARIABLES", "93");
};

void plumgrads(Model *mdl, Insts *insts) {
	_plum_tensors(mdl, insts, mdl->Mgrad, mdl->grad, "GRADIENTS", "91");
};

void plummdl(Model *mdl, Insts *insts) {
	plumvars(mdl, insts);
	plumgrads(mdl, insts);
};

void pluminsts(Insts *insts, Model *mdl) {
	size_t i,j;
	for (i=0; i < insts->insts; i++) {
		printf("%li| ", i);
		if (insts->inst[i][0] == 0) {
			printf("No command here");
		} else {
			switch (insts->inst[i][1]) {
				//cmd:len,id,...
				case 0:case 1:case 2:case 3:case 4:case 5:
					if (insts->inst[i][4] == 0) {
						printf("%s for $%li[%li] but n=0", 
							str_ops_names[insts->inst[i][1]],
							insts->inst[i][2], 
							insts->inst[i][3]);
					} else {
						printf("\033[1;93;40m%s\033[0m[\033[96m%li\033[0m]$\033[90m%li\033[0m = \033[1;93;40m%s\033[0m[\033[96m%li\033[0m]$\033[90m%li\033[0m", 
							mdl->Mvar[insts->inst[i][2]]->label, insts->inst[i][3], insts->inst[i][2],
							mdl->Mvar[insts->inst[i][5]]->label, insts->inst[i][6], insts->inst[i][5]);	//a[at] = b[bt]
						for (j=0; j < insts->inst[i][4]-1; j++)
							printf(" \033[1;95m%s\033[0m \033[1;93;40m%s\033[0m[\033[96m%li\033[0m]$\033[90m%li\033[0m", 
								str_ops[insts->inst[i][1]],
								mdl->Mvar[insts->inst[i][7+j*2]]->label, insts->inst[i][8+j*2], insts->inst[i][7+j*2]);
					}
					break;
				case 6:case 7:case 8:case 9:case 10:
					printf("%s[%li]($%li) = iter(%s) %s[%li]($%li)\n", 
						mdl->Mvar[insts->inst[i][2]]->label, insts->inst[i][3], insts->inst[i][2],
						str_ops_names[insts->inst[i][1]-6],
						mdl->Mvar[insts->inst[i][3]]->label, insts->inst[i][4], insts->inst[i][2]
						);
					break;
				case 11:	//func	y = f(a,b,c)
					if (insts->inst[i][5] == 0) {
						printf("\033[1;93;40m%s\033[0m[\033[96m%li\033[0m]$\033[90m%li\033[0m = %s( But no args\n", 
							mdl->Mvar[insts->inst[i][2]]->label, insts->inst[i][3], insts->inst[i][2],
							str_funcs[insts->inst[i][4]]
							);
						break;
					}
					printf("\033[1;93;40m%s\033[0m[\033[96m%li\033[0m]$\033[90m%li\033[0m = \033[92m%s\033[0m(\033[1;93;40m%s\033[0m[\033[96m%li\033[0m]$\033[90m%li\033[0m",
						mdl->Mvar[insts->inst[i][2]]->label, insts->inst[i][3], insts->inst[i][2],
						str_funcs[insts->inst[i][4]],
						mdl->Mvar[insts->inst[i][6]]->label, insts->inst[i][7], insts->inst[i][6]
						);
					for (j=0; j < insts->inst[i][5]-1; j++)
						printf("\033[1m,\033[0m \033[1;93;40m%s\033[0m[\033[96m%li\033[0m]$\033[90m%li\033[0m",
							mdl->Mvar[insts->inst[i][8+j*2]]->label, insts->inst[i][9+j*2], insts->inst[i][8+j*2]
							);
						printf(")");
					break;
				case 12:	//transpose
					printf("\033[1;93;40m%s\033[0m[\033[96m%li\033[0m]$\033[90m%li\033[0m = (\033[1;93;40m%s\033[0m[\033[96m%li\033[0m]$\033[90m%li\033[0m).\033[1mT\033m", 
						mdl->Mvar[insts->inst[i][2]]->label, insts->inst[i][3], insts->inst[i][2],
						mdl->Mvar[insts->inst[i][4]]->label, insts->inst[i][5], insts->inst[i][4]);
					break;
				case 13:	//y = a | K valid/same
					printf("\033[1;93;40m%s\033[0m[\033[96m%li\033[0m]$\033[90m%li\033[0m = \033[1;93;40m%s\033[0m[\033[96m%li\033[0m]$\033[90m%li\033[0m | \033[1;93;40m%s\033[0m[\033[96m%li\033[0m]$\033[90m%li\033[0m", 
						mdl->Mvar[insts->inst[i][2]]->label, insts->inst[i][3], insts->inst[i][2],
						mdl->Mvar[insts->inst[i][4]]->label, insts->inst[i][5], insts->inst[i][4],
						mdl->Mvar[insts->inst[i][6]]->label, insts->inst[i][7], insts->inst[i][6]);
					if (insts->inst[i][8] == 0) printf(" mode:valid");
					else printf(" mode:same");
					break;
				default:
					printf("Not recognised cmd id : %li, with params : ", insts->inst[i][1]);
					for (j=2; j < insts->inst[i][0]+1; j++) printf("%li,", insts->inst[i][j]);
					break;
			}
		}
		printf("\n");
	}
};