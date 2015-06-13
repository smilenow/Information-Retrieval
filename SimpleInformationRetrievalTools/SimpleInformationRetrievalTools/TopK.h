//
//  TopK.h
//  SimpleInformationRetrievalTools
//
//  Created by SmilENow on 6/10/15.
//  Copyright (c) 2015 Jiaquan Yin. All rights reserved.
//

#ifndef __SimpleInformationRetrievalTools__TopK__
#define __SimpleInformationRetrievalTools__TopK__

#include <iostream>
#include <algorithm>
#include "InvertedIndex.h"

class TopK{
// Method
public:
    TopK();
    virtual ~TopK();
    vector<string> TopK_Heap(int, vector<pair<string,double>>);
private:
    void BuildHeap(vector<pair<string,double>>&);
    void heapify(int);
    void heapdelete();
// Variables
private:
    int heapsize;
    vector<pair<string,double>> TopK_Max_Heap;
};

#endif /* defined(__SimpleInformationRetrievalTools__TopK__) */
