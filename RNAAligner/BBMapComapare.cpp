//
//  BBMapComapare.cpp
//  RNAAligner
//
//  Created by Josip Maric on 27/07/15.
//  Copyright (c) 2015 Josip Maric. All rights reserved.
//

#include "BBMapComapare.h"
#include "headers.h"

void compareBBMap(vector<Result> BBMapResults, map<int, Result> correctResults) {
    cout << "Percentage of aligned reads: " << (double) (100 * BBMapResults.size()) / correctResults.size() << endl;
    
    int starts = 0;
    int stops = 0;
    int startsAndStops = 0;
    int startsOrStops = 0;
    int limit = 15;
    
    for (int i = 0; i < BBMapResults.size(); i++) {
        Result bbmapResult = BBMapResults[i];
        map<int, Result>::iterator pos = correctResults.find(bbmapResult.br);
        Result correctResult = pos->second;
//        if (bbmapResult.strand == 0) {
//            continue;
//        }
    //   cout << bbmapResult.start << "-" << bbmapResult.stop << "  " << correctResult.start << "-" <<correctResult.stop << endl;
    //    cout << abs(bbmapResult.start - correctResult.start) << " " << abs(bbmapResult.stop - correctResult.stop) << endl;
        if (abs(bbmapResult.start - correctResult.start) < limit) {
            starts++;
        }
        if (abs(bbmapResult.stop - correctResult.stop) < limit) {
            stops++;
        }
        if (abs(bbmapResult.start - correctResult.start) < limit && abs(bbmapResult.stop - correctResult.stop) < limit) {
            startsAndStops++;
        }
        if (abs(bbmapResult.start - correctResult.start) < limit || abs(bbmapResult.stop - correctResult.stop) < limit) {
            startsOrStops++;
        }
    }
    
    cout << "Percentage of correct starts: " << (double) (100 * starts) / correctResults.size() << "(" << starts << ")" << endl;
    cout << "Percentage of correct stops: " << (double) (100 * stops) / correctResults.size() << "(" << stops << ")" << endl;
    cout << "Percentage of correct start and stops: " << (double) (100 * startsAndStops) / correctResults.size() << "(" << startsAndStops << ")" << endl;
    cout << "Percentage of correct start or stops: " << (double) (100 * startsOrStops) / correctResults.size() << "(" << startsOrStops << ")" << endl;
}