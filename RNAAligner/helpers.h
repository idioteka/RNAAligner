
/*
 *
 * SHRINKS
 *
 */
vector<vector<int> > shrinkArrays(vector<int> &offsets, vector<int> &keys, int num_hits);
vector<vector<int> > shrink(vector<int> &starts, vector<int> &stops, vector<int> &offsets, vector<int> &keys, int len);

/*
 *
 * ALL BASE COVERED
 *
 */
bool checkIfAllBasesCovered(Config &config, vector<int> &offsets, string &read);
bool calculatePretendAllBasesCovered(Config &config, vector<int> &read_keys,vector<int> &read_keys_final,
				vector<int> &offsets, bool all_bases_covered, string &read);
bool isFullyDefined(string &read);


/*
 *
 * PROCESSES
 *
 */
int parseLine(char* line);
int getValue();
