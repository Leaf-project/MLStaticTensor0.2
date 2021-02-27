#include "../include/data.h"

int main(int argc, char **argv) {
	FILE *fp;
	//Read Mdl & Insts
	fp = fopen("Test/xor.bin", "rb");
	Model* mdl = loadmdl(fp);
	Insts* insts = loadinsts(fp);
	fclose(fp);
	//Read data
	fp = fopen("Test/xordata.csv", "r");
	Data* data = loadcsv(fp, mdl, insts, 4);
	fclose(fp);
	//
	plumdata(mdl, insts, data);
	//
	pluminsts(insts, mdl);
	expand(mdl, 4);
	//
	for (size_t i=0; i < 4; i++) {
		set_inputs(mdl, insts, data->batch[0][i], i);
		forward(mdl, insts, i);
	}
	//
	plummdl(mdl, insts);
	//
	freemdl(mdl);
	freeinsts(insts);
	return 0;
};