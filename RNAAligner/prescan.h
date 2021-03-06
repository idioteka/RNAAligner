
#include "score.h"
#include "key_manager.h"
#include "config.h"

/*
 *
 * PRESCAN FUNCTIONS
 *
 */

void findMaxQscore2(Config &config, vector<int> &starts, vector<int> &stops, vector<int> &offsets,
		vector<Triplet> &triples, vector<int> &values, vector<int> &keys,
		  int prevMaxHits, bool earlyExit, bool perfectOnly, int *sizes, int *sites, vector<int> &temp);
void prescanAllBlocks(Config &config, vector<vector<int> > &prescanResults, int bestScores[], vector<int> &keysP, vector<int> &offsetsP,
		vector<int> &keysM, vector<int> &offsetsM, bool allBasesCovered, int *sizes, int *sites);
