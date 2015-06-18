
#include "score.h"
#include "config.h"

/*
 *
 * MAKE MATCH STRING
 *
 */

int scoreNoIndelsAndMakeMatchString(string &read, string &ref, int refStart, string &match);
int makeGref(string &ref, vector<int> &gaps, int refStartLoc, int refEndLoc, string &gref);
void fillUnlimited(string &read, string &ref, int refStartLoc, int refEndLoc,
		long max[], vector<vector<vector<long> > > &packed);
void traceback(string &read, string &ref, int refStartLoc, int refEndLoc, int row,
		int col, int state, Result &r, vector<vector<vector<long> > > &packed);
void fillLimited(string &read, string &ref, int refStartLoc, int refEndLoc, int score,
		vector<int> &gaps, long max[], Result &r);
void makeMatchStringForSite(SiteScore ss, string &read, int *sizes,
		int *sites, Result &r, string &whole_genome, int threadId);
void makeMatchString(vector<SiteScore> &results, string &read, string &read_reverse,
		int *sizes, int *sites, vector<Result> &resultsFinal, string &whole_genome,
		int threadId, int br, int maxScore);
