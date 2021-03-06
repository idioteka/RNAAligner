
#include "config.h"

/*
 *
 * OFFSETS
 *
 */
int getDesiredKeyNumber(Config &config, int readlen, float density);
void makeOffsets(Config &config, string read, vector<int> &offsets);

/*
 *
 * KEY CREATION
 *
 */
int getKeyFromKmer(Config &config, int start, int stop, string &read);
void getReadKeys(Config &config, string read, vector<int> &offsets, vector<int> &keys);

/*
 *
 * KEY COUNTS
 *
 */
int reverseComplementBinary(int kmer, int k);
int countKeyHits(Config &config, int key, int *sizes);
int countHits(Config &config, vector<int> &keys, int *sizes, int max_len);
int trimHits(Config &config, vector<int> &keys, vector<int> &keys_temp, int *sizes, int num_hits);
int getHits(Config &config, vector<int> &keys, int max_len, vector<int> &starts, vector<int> &stops, int *sizes);


/*
 *
 * REVERSE FUNCTIONS
 *
 */

void reverseComplementKeys(vector<int> &reversed_keys, vector<int> &keys ,int k);
void reverseOffsets(vector<int> &offsets_reversed, vector<int> &offsets, int k, int readlen);
void reverseComplementRead(string &out, string &in);

