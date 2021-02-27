Data* loadcsv(FILE *fp, Model *mdl, Insts *insts, size_t bsize) {
	Data* data = (Data*)malloc(sizeof(Data));
	data->bsize = bsize;
	data->batchs = 0;
	//
	data->batch = (Line***)malloc(0);	//data[batch=0]
	//
	bool new_batch=true;
	int batch=-1;
	size_t line=0;	//how many lines in current batch% bsize
	size_t nb=0;	//whitch input/output id : out[id]
	size_t ln=0;	//pixel
	//
	char ch;
	char* string=malloc(2);
	size_t used=0;
	size_t stringlen=2;
	//
	bool type=0;	//0-input, 1-output
	bool read_double=0;
	bool nextline=false;
	//
	double value;
	//
	do {
		ch = fgetc(fp);
		//
		if (new_batch) {
			new_batch = false;
			data->batch = (Line***)realloc(data->batch, ++data->batchs * sizeof(Line**));
			data->batch[++batch] = (Line**)malloc(bsize * sizeof(Line*));	//data[batch=0][line=0]
			//
			for (size_t line_=0; line_ < bsize; line_++) {
				data->batch[batch][line_] = (Line*)malloc(sizeof(Line));
				data->batch[batch][line_]->input = (Ts**)malloc(sizeof(Ts*) * insts->inputs);
				data->batch[batch][line_]->output = (Ts**)malloc(sizeof(Ts*) * insts->outputs);
				//
				for (size_t i=0; i < insts->inputs; i++)
					data->batch[batch][line_]->input[i] = cpyts(mdl->Mvar[insts->input[i]]);
				for (size_t i=0; i < insts->outputs; i++)
					data->batch[batch][line_]->output[i] = cpyts(mdl->Mvar[insts->output[i]]);
			}
		}
		///////////////////////////
		if (ch == ',' || ch == ';' || ch == '\n' || ch == EOF) {
			read_double = 1;
		} else {
			if (used+1 == stringlen) {
				stringlen*=2;
				string = realloc(string, stringlen);
			}
			string[used++] = ch;
			string[used] = '\0';
		}
		//
		if (read_double) {
			read_double = 0;
			value = atof(string);
			//
			free(string);
			string = malloc(2);
			used = 0;
			stringlen = 2;
			//
			if (type==0) {
				data->batch[batch][line]->input[nb]->arr[ln++] = value;
				if (ln == data->batch[batch][line]->input[nb]->len) {
					ln = 0;
					nb++;
				}
				if (nb == insts->inputs) {
					nb = 0;
					type = 1;
				}
			} else {
				data->batch[batch][line]->output[nb]->arr[ln++] = value;
				if (ln == data->batch[batch][line]->output[nb]->len) {
					ln = 0;
					nb++;
				}
				if (nb == insts->outputs) {
					nb = 0;
					type = 0;
					line++;
					if (line == bsize) {
						line = 0;
						new_batch = true;
					}
				}
			}
			//
		}
	} while (ch != EOF);
	return data;
}