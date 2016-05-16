
#include "config.h"

/*
 *
 * SCORE FUNCTIONS
 *
 */

int calcMaxScore(Config &config, string &read);
int calcMaxScoreZ(Config &config, vector<int> &offsets);
int scoreZ2(Config &config, vector<int> &locs, int centerIndex, vector<int> &offsets, int num_approx_hits, int num_hits);
int scoreY(Config &config, vector<int> &locs, int center_index, vector<int > &offsets);
int scoreLeft(Config &config, vector<int> &locs, int center_index);
int scoreRight(Config &config, vector<int> &locs, int center_index, int num_hits);
int quickScore(Config &config, vector<int> &locs, int center_index, vector<int> &offsets, int num_approx_hits, int num_hits);
int calcMaxQuickScore(Config &config, vector<int> &offsets, vector<int> keys);
int calcAffineScore(Config &config, vector<int> &loc_array, string &read, bool &isDirtyStart, bool &isDirtyStop, int &startError, int &stopError);
int extendScore(Config &config, string &read, vector<int> &offsets, vector<int> &values, int center_index, vector<int> &loc_array, int num_hits, int num_approx_hits, string &whole_genome, bool &isDirtyStart, bool &isDirtyStop, int &startError, int &stopError);
int maxImperfectScore(Config &config, string &read);
