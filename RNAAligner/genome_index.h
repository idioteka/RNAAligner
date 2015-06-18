#include "headers.h"
#include "config.h"

int ** createIndex(bool write_to_file, string &whole_genome, bool part_genome,
		string genome_ref, string index_loc, Config &config);
int ** readIndex(string &whole_genome, string genome_ref,
		string index_loc, bool part_genome, Config &config);
