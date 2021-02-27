void plumdata(Model* mdl, Insts *insts, Data *data) {
	char*** strinputs = (char***)malloc(insts->inputs * sizeof(char**));
	char*** stroutputs = (char***)malloc(insts->outputs * sizeof(char**));
	//
	size_t max;
	//
	for (size_t batch=0; batch < data->batchs; batch++) {
		for (size_t line=0; line < data->bsize; line++) {
			max = 0;
			for (size_t i=0; i < insts->inputs; i++) {
				strinputs[i] = strts(data->batch[batch][line]->input[i]);
				if (lineslen(strinputs[i]) > max) max = lineslen(strinputs[i]);
			}
			//
			for (size_t i=0; i < insts->outputs; i++) {
				stroutputs[i] = strts(data->batch[batch][line]->output[i]);
				if (lineslen(stroutputs[i]) > max) max = lineslen(stroutputs[i]);
			}
			//PRINT out
			for (size_t strline=0; strline < max; strline++) {
				for (size_t i=0; i < insts->inputs; i++) {
					if (lineslen(strinputs[i]) < max) {
						for (size_t j=0; j < strlen(strinputs[i][0]); j++) {
							printf(" ");
						}
					} else {
						printf("%s", strinputs[i][strline]);
					}
					printf("|");
				}
				//
				printf(";|");
				for (size_t i=0; i < insts->outputs; i++) {
					if (lineslen(stroutputs[i]) < max) {
						for (size_t j=0; j < strlen(stroutputs[i][0]); j++) {
							printf(" ");
						}
					} else {
						printf("%s", stroutputs[i][strline]);
					}
					printf("|");
				}
				printf("\n");
			}
			//Free
			for (size_t i=0; i < insts->inputs; i++)
				freestrs(strinputs[i]);
			for (size_t i=0; i < insts->outputs; i++)
				freestrs(stroutputs[i]);
		}

	}
	//
	free(strinputs);
	free(stroutputs);
};