//
//  NeedlemanWunsch.cpp
//  RNAAligner
//
//  Created by Josip Maric on 19/07/15.
//  Copyright (c) 2015 Josip Maric. All rights reserved.
//

#include "config.h"
#include "headers.h"
#include "key_manager.h"
#include "NeedlemanWunsch.h"

struct ThreadData4 {
    int thread_id;
    vector<NeedWunshStruct> *structs;
    int start;
    int stop;
    int referenceSize;
    ThreadData4() {
    }
    ThreadData4(int thread_id_, vector<NeedWunshStruct> *structs_, int start_, int stop_, int referenceSize_) {
        thread_id = thread_id_;
        structs = structs_;
        start = start_;
        stop = stop_;
        referenceSize = referenceSize_;
    }
};
//
//long del = -766656;
//long elem = 500;
//long secondDel = 18000;
//long match = 143360;
//long sub = -260096;
//long ins = -808960;

long del = -8000;
long elem = 5;
long secondDel = 18000;
long match = 1400;
long sub = -2600;
long ins = -8000;

long matchElem = 40;

int readSize = 2000;
int referenceSize = 110000;

long matchOrSubScore(char ref, char read, bool prevMatch) {
    if (toupper(ref) == toupper(read)) {
        if (prevMatch) {
            return match + matchElem;
        } else {
            return match;
        }
    } else {
        return sub;
    }
}

void printMatrix(long **matrix, int rows, int columns) {
    cout << "Matrix" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
}

long returnCleanedScore(long previousScore, long addingValue) {
    long result = previousScore + addingValue;
    bool leftOverflow = previousScore < 0 && addingValue < 0 && result > 0;
    bool rightOverlow = previousScore > 0 && addingValue > 0 && result < 0;
    if (leftOverflow) {
        return LONG_MIN;
    } else if (rightOverlow) {
        return LONG_MAX;
    } else {
        return result;
    }
}

void initMatrixG(long** matrixG, int referenceSize, int readSize) {
    for (int i = 1; i < readSize+1; i++) {
        matrixG[i][0] = returnCleanedScore(del, -(i-1) * elem);
    }
    for (int j = 1; j < referenceSize +1; j++) {
        matrixG[0][j] = LONG_MIN;
    }
    
   // printMatrix(matrixG, readSize+1, referenceSize+1);
}

void initMatrixH(long** matrixH, int referenceSize, int readSize) {
    for (int i = 1; i < readSize+1; i++) {
        matrixH[i][0] = LONG_MIN;
    }
    for (int j = 1; j < referenceSize +1; j++) {
        matrixH[0][j] = returnCleanedScore(ins, (j-1) * ins);
    }
    
 //   printMatrix(matrixH, readSize+1, referenceSize+1);
}

void initMatrixV(long** matrixV, int referenceSize, int readSize) {
    matrixV[0][0] = 0;
    for (int i = 1; i < readSize+1; i++) {
        matrixV[i][0] = returnCleanedScore(del, -(i-1) * elem);
    }
    for (int j = 1; j < referenceSize+1; j++) {
        matrixV[0][j] = returnCleanedScore(ins, (j-1) * ins);
    }
    
  //  printMatrix(matrixV, readSize+1, referenceSize+1);
}

void fillMatrices(long** matrixG, long** matrixH, long** matrixF, long** matrixV, int referenceSize, int readSize, string reference, string read) {
    bool prevMatch = false;
    for (int i = 1; i < readSize+1; i++) {
        //cout << "Row: " << i << endl;
        for (int j = 1; j < referenceSize+1; j++) {
            if (j > 1 && i > 1) {
                if (toupper(reference[j-2]) == toupper(read[i-2])) {
                    prevMatch = true;
                } else {
                    prevMatch = false;
                }
            }
            matrixF[i][j] = returnCleanedScore(matrixV[i-1][j-1], matchOrSubScore(reference[j-1], read[i-1], prevMatch));
            long vValueSubtracted = returnCleanedScore(matrixV[i][j-1], del);
            long gValueSubtracted = returnCleanedScore(matrixG[i][j-1], -elem);
            long vValueSubtractedIns = returnCleanedScore(matrixV[i-1][j], ins);
            long gValueSubtractedIns = returnCleanedScore(matrixG[i-1][j], ins);
            matrixG[i][j] = max(vValueSubtracted, gValueSubtracted);
            matrixH[i][j] = max(vValueSubtractedIns, gValueSubtractedIns);
            //matrixH[i][j] = returnCleanedScore(matrixV[i][j-1], ins);
            //matrixV[i][j] = max(max(max(matrixF[i][j], matrixG[i][j]), matrixH[i][j]), (long)0);
            matrixV[i][j] = max(max(matrixF[i][j], matrixG[i][j]), matrixH[i][j]);
        }
    }
}

void traceback(long** matrix, int rows, int columns) {
    
}

void processNWS(long** matrixG, long** matrixH, long** matrixF, long** matrixV, int referenceSize, int readSize, NeedWunshStruct &s, int &correct, int &wrong, int &impossible, int threadId) {
    //NeedWunshStruct s = nws[k];
    string reverseRead;
    string reverseReference;
    
//    if (s.stopLocation < 32000) {
//        return;
//    }
    
    if (s.stopLocation > 110000) {
        impossible++;
    } else {
        if (s.stopLocation < 32000) {
            referenceSize = 32000;
        } else if (s.stopLocation < 64000) {
            referenceSize = 64000;
        } else {
            referenceSize = 110000;
        }
        if (s.side == 1) {
            for (int i = readSize-1; i >= 0; i--) {
                reverseRead.push_back(s.read[i]);
            }
            for (int i = 110000-1; i >= 0; i--) {
                reverseReference.push_back(s.reference[i]);
            }
            
            cout << "Filling matrices" << endl;
            
            if (s.strand == 1) {
                string revComp;
                reverseComplementRead(revComp, reverseRead);
                
                fillMatrices(matrixG, matrixH, matrixF, matrixV, referenceSize, readSize, reverseReference, revComp);
                
                cout << "Matrices filled" << endl;
            } else {
                fillMatrices(matrixG, matrixH, matrixF, matrixV, referenceSize, readSize, reverseReference, reverseRead);
                
                cout << "Matrices filled" << endl;
                
            }
            
        } else {
            
            cout << "Filling matrices" << endl;
            
            if (s.strand == 1) {
                string revComp;
                reverseComplementRead(revComp, s.read);
                fillMatrices(matrixG, matrixH, matrixF, matrixV, referenceSize, readSize, s.reference, revComp);
            } else {
                fillMatrices(matrixG, matrixH, matrixF, matrixV, referenceSize, readSize, s.reference, s.read);
            }
            
            cout << "Matrices filled" << endl;
        }
        
        int index = 1990;
        long maxLastRowValue = matrixV[readSize][index];
        for(int j = 1990; j < referenceSize + 1; j++) {
            if (matrixV[readSize][j] > maxLastRowValue) {
                maxLastRowValue = matrixV[readSize][j];
                index = j;
            }
        }
        
        for (int i = index-10; i < index+10; i++) {
            cout << matrixV[readSize][i] << " ";
        }
        cout << endl;
        
        int rowIndex = readSize;
        int columnIndex = index;
        int deletionCount = 0;
        int gapsStart = columnIndex;
        vector<long> gaps;
        gaps.push_back(columnIndex);
        while (rowIndex > 0 && columnIndex > 0) {
            long left = matrixV[rowIndex][columnIndex-1];
            long upper = matrixV[rowIndex-1][columnIndex];
            long diagonal = matrixV[rowIndex-1][columnIndex-1];
            if (diagonal > left && diagonal > upper) {
               // cout << "diag" << endl;
                rowIndex--;
                columnIndex--;
                if (deletionCount > 30) {
                    gaps.push_back(gapsStart);
                    gaps.push_back(columnIndex);
                }
                deletionCount = 0;
            } else if (left > upper) {
               // cout << "left" << endl;
                if (deletionCount == 0) {
                    gapsStart = columnIndex;
                }
                columnIndex--;
                deletionCount++;
            } else {
              //  cout << "upper" << endl;
                rowIndex--;
                if (deletionCount > 30) {
                    gaps.push_back(gapsStart);
                    gaps.push_back(columnIndex);
                }
                deletionCount = 0;
            }
        }
        gaps.push_back(0);
        int sum = 0;
        for (int i = gaps.size()-1; i >= 0; i-=2) {
            cout << abs(gaps[i] - gaps[i-1]) << " ";
            if (abs(gaps[i] - gaps[i-1]) > 60) {
                sum += abs(gaps[i] - gaps[i-1]);
            }
//            if (i>1) {
//                cout << abs(gaps[i-1] - gaps[i-2]) << " ";
//            }
         //   cout << gaps[i] << " ";
        }
        cout << endl;
        cout << "Sum: " << sum << endl;
        
        if (sum < 1600) {
            cout << "Index updated: " << index;
            index += 100;
            long maxLastRowValue = matrixV[readSize][index];
            for(int j = index; j < referenceSize + 1; j++) {
                if (matrixV[readSize][j] > maxLastRowValue) {
                    maxLastRowValue = matrixV[readSize][j];
                    index = j;
                }
            }
            cout << ", new index: " << index << endl;
        }
        
        cout << "strand: " << s.strand << ", side: " << s.side << endl;
        cout << "Location: " << s.stopLocation << ", index: " << index << ", difference: " << (s.stopLocation - index) << endl;
        if (abs(s.stopLocation - index) < 15 && s.stopLocation < 110000) {
            correct++;
        } else if (abs(s.stopLocation - index) > 15 && s.stopLocation < 110000) {
            wrong++;
        } else {
            impossible++;
        }
    }
    cout << "reference size: " << referenceSize << endl;
    cout << "start: " << s.start << ", stop: " << s.stop << endl;
    cout << "start: " << s.correctStart << ", stop: " << s.correctStop << endl;
    cout << threadId << " correct: " << correct << ", wrong: " << wrong << ", impossible: " << impossible << endl;
    
}

void *processNWSs(void *threadid) {
    ThreadData4 *td = (ThreadData4 *) threadid;
    
    long **matrixG;
    matrixG = new long *[readSize+1];
    for (int i = 0; i < readSize+1; i++) {
        matrixG[i] = new long[referenceSize+1];
    }
    
    long **matrixH;
    matrixH = new long *[readSize+1];
    for (int i = 0; i < readSize+1; i++) {
        matrixH[i] = new long[referenceSize+1];
    }
    
    long **matrixF;
    matrixF = new long *[readSize+1];
    for (int i = 0; i < readSize+1; i++) {
        matrixF[i] = new long[referenceSize+1];
    }
    
    long **matrixV;
    matrixV = new long *[readSize+1];
    for (int i = 0; i < readSize+1; i++) {
        matrixV[i] = new long[referenceSize+1];
    }
    
    initMatrixG(matrixG, referenceSize, readSize);
    cout << "matrix G initilized" << endl;
    initMatrixH(matrixH, referenceSize, readSize);
    cout << "matrix H initilized" << endl;
    initMatrixV(matrixV, referenceSize, readSize);
    cout << "matrix V initilized" << endl;
    
    cout << "size: " << (*(td->structs)).size() << endl;
    int correct = 0;
    int wrong = 0;
    int impossible = 0;
    
    for(int i = td->start; i < td->stop; i++) {
        
        NeedWunshStruct nws = (*(td->structs))[i];
        
        if(true) {
            //		if((i+1)%100 == 0) cout << "Read: " << read.br << " started." << endl;
        }
        /*if((i+1)%10 == 0)*/ //cout << "Read: " << read.br << " started." << endl;
        processNWS(matrixG, matrixH, matrixF, matrixV, referenceSize, readSize, nws, correct, wrong, impossible, td->thread_id);
    }
    
    for (int i = 0; i < readSize+1; i++) {
        delete [] matrixG[i];
    }
    delete [] matrixG;
    
    for (int i = 0; i < readSize+1; i++) {
        delete [] matrixH[i];
    }
    delete [] matrixH;
    
    for (int i = 0; i < readSize+1; i++) {
        delete [] matrixF[i];
    }
    delete [] matrixF;
    
    for (int i = 0; i < readSize+1; i++) {
        delete [] matrixV[i];
    }
    delete [] matrixV;
    
    pthread_exit(NULL);
}


void tryNeedlemanWunschWithAfinaGaps(vector<NeedWunshStruct> &nws) {
    
    
    int rc;
    pthread_attr_t attr;
    void *status;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    int threadNumber = 4;
    
    ThreadData4 datas[4];
    pthread_t threads[threadNumber];
    int difference = nws.size()/threadNumber;
    
    for(int i = 0; i < threadNumber; i++) {
        int start =  i*difference;
        int stop =  i*difference + difference;
        datas[i] = ThreadData4(i, &nws, start, stop, referenceSize);
    }
    datas[threadNumber-1].stop = nws.size();
    for(int i = 0; i < threadNumber; i++) {
        cout << "Thread " << i << " started." << endl;
        rc = pthread_create(&threads[i], NULL, processNWSs, (void *) &datas[i]);
    }
    
    pthread_attr_destroy(&attr);
    for(int i=0; i < threadNumber; i++ ){
        rc = pthread_join(threads[i], &status);
        if (rc){
            cout << "Error:unable to join," << rc << endl;
            exit(-1);
        }
        cout << "Main: completed thread id :" << i ;
        cout << "  exiting with status :" << status << endl;
    }

    return;
    
//
//
//    string read = nws.read;
//    string reference = nws.reference;
//
//    string reverseRead;
//    string reverseReference;
//    
//    int location = nws.stopLocation;
//    int side = nws.side;
//    int strand = nws.strand;
//
    
//    for (int k = 0; k < nws.size(); k++) {
//        
//        NeedWunshStruct s = nws[k];
//        string reverseRead;
//        string reverseReference;
//        
//        if (s.side == 1) {
//            for (int i = readSize-1; i >= 0; i--) {
//                reverseRead.push_back(s.read[i]);
//            }
//            for (int i = referenceSize-1; i >= 0; i--) {
//                reverseReference.push_back(s.reference[i]);
//            }
//            
//            cout << "Filling matrices" << endl;
//            
//            if (s.strand == 1) {
//                string revComp;
//                reverseComplementRead(revComp, reverseRead);
//                
//                fillMatrices(matrixG, matrixH, matrixF, matrixV, referenceSize, readSize, reverseReference, revComp);
//                
//                cout << "Matrices filled" << endl;
//            } else {
//                fillMatrices(matrixG, matrixH, matrixF, matrixV, referenceSize, readSize, reverseReference, reverseRead);
//                
//                cout << "Matrices filled" << endl;
//
//            }
//            
//        } else {
//            
//            cout << "Filling matrices" << endl;
//
//            if (s.strand == 1) {
//                string revComp;
//                reverseComplementRead(revComp, s.read);
//                fillMatrices(matrixG, matrixH, matrixF, matrixV, referenceSize, readSize, s.reference, revComp);
//            } else {
//                fillMatrices(matrixG, matrixH, matrixF, matrixV, referenceSize, readSize, s.reference, s.read);
//            }
//            
//            cout << "Matrices filled" << endl;
//        }
//        
//        int index = 1;
//        long maxLastRowValue = matrixV[readSize][index];
//        for(int j = 1; j < referenceSize + 1; j++) {
//            if (matrixV[readSize][j] > maxLastRowValue) {
//                maxLastRowValue = matrixV[readSize][j];
//                index = j;
//            }
//        }
//        
//        cout << "strand: " << s.strand << ", side: " << s.side << endl;
//        cout << "Location: " << s.stopLocation << ", index: " << index << ", difference: " << (s.stopLocation - index) << endl;
//        if (s.stopLocation - index < 15 && s.stopLocation < 32000) {
//            correct++;
//        } else if (s.stopLocation - index > 15 && s.stopLocation < 32000) {
//            wrong++;
//        } else {
//            impossible++;
//        }
//        
//        cout << "correct: " << correct << ", wrong: " << wrong << ", impossible: " << impossible << endl;
//
//    }
//    if (strand == 1) {
//        reverseComplementRead(read, nws.read);
//    }
//    
//    int referenceSize = reference.size();
//    int readSize = read.size();
//    
//    if (side == 1) {
//        for (int i = readSize-1; i >= 0; i--) {
//            reverseRead.push_back(read[i]);
//        }
//        for (int i = referenceSize-1; i >= 0; i--) {
//            reverseReference.push_back(reference[i]);
//        }
//        
//        cout << "Filling matrices" << endl;
//        
//        fillMatrices(matrixG, matrixH, matrixF, matrixV, referenceSize, readSize, reverseReference, reverseRead);
//        
//        cout << "Matrices filled" << endl;
//    } else {
//        cout << "Filling matrices" << endl;
//        
//        fillMatrices(matrixG, matrixH, matrixF, matrixV, referenceSize, readSize, reference, read);
//        
//        cout << "Matrices filled" << endl;
//    }
    
//    printMatrix(matrixG, readSize+1, referenceSize+1);
//    printMatrix(matrixH, readSize+1, referenceSize+1);
//    printMatrix(matrixF, readSize+1, referenceSize+1);
//    printMatrix(matrixV, readSize+1, referenceSize+1);
    
//    int maxIndexJ = 1;
//    int maxIndexI = 1;
//    long maxValue = matrixV[maxIndexI][maxIndexJ];
//    for (int i = maxIndexI; i < readSize + 1; i++) {
//        for (int j = maxIndexJ; j < referenceSize+1; j++) {
//            if (maxValue < matrixV[i][j]) {
//                maxValue = matrixV[i][j];
//                maxIndexI = i;
//                maxIndexJ = j;
//            }
//        }
//    }
    
//    cout << "Max index i = " << maxIndexI << endl;
//    cout << "Max index j = " << maxIndexJ << endl;
//    cout << "Max value = " << maxValue << endl;
//    
 //   traceback(matrixV, readSize+1, referenceSize+1);
    
//    int index = 1;
//    long maxLastRowValue = matrixV[readSize][index];
//    for(int j = 1; j < referenceSize + 1; j++) {
//        if (matrixV[readSize][j] > maxLastRowValue) {
//            maxLastRowValue = matrixV[readSize][j];
//            index = j;
//        }
//    }
//    
//    cout << "strand: " << strand << ", side: " << side << endl;
//    cout << "Location: " << location << ", index: " << index << ", difference: " << (location - index) << endl;
    
    
//    for (int i = 1107; i < 1117; i++) {
//        for (int j = 1107; j < 1117; j++) {
//            cout << matrixV[i][j] << "\t";
//        }
//        cout << endl;
//    }
//
//    cout << "Index = " << index << endl;
//    cout << "Max value = " << maxLastRowValue << endl;
//    
}