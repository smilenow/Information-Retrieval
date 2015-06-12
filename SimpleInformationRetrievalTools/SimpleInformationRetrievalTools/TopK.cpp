//
//  TopK.cpp
//  SimpleInformationRetrievalTools
//
//  Created by SmilENow on 6/10/15.
//  Copyright (c) 2015 Jiaquan Yin. All rights reserved.
//

#include "TopK.h"

TopK::TopK(){
    TopK_Max_Heap.clear();
}

TopK::~TopK(){
    TopK_Max_Heap.clear();
}

vector<string> TopK::TopK_Heap(int K, vector<pair<string, double> > DocScore){
    vector<string> ret;
    
    heapsize = (int)DocScore.size();
    TopK_Max_Heap.resize(heapsize+1);
    BuildHeap(DocScore);
    
    for (int i=0;i<std::min(K,(int)DocScore.size());i++){
        ret.push_back(TopK_Max_Heap[1].first);
        heapdelete();
    }
    
    return ret;
}

void TopK::BuildHeap(vector<pair<string, double> > &DocScore){
    int cnt = 0;
    for (auto &i:DocScore) TopK_Max_Heap[++cnt] = i;
    for (int i = heapsize/2; i>=1; i--) heapify(i);
}

void TopK::heapify(int i){
    int left = i*2;
    int right = left+1;
    int maxindex = i;
    if (left<=heapsize && TopK_Max_Heap[left].second > TopK_Max_Heap[i].second) maxindex = left;
    if (right<=heapsize && TopK_Max_Heap[right].second > TopK_Max_Heap[maxindex].second) maxindex = right;
    if (maxindex != i){
        std::swap(TopK_Max_Heap[i],TopK_Max_Heap[maxindex]);
        heapify(maxindex);
    }
}

void TopK::heapdelete(){
    swap(TopK_Max_Heap[1],TopK_Max_Heap[heapsize]);
    heapsize--;

    // push down
    for (int i=1;i<=heapsize;){
        int left = i*2;
        int right = left+1;
        int maxindex = i;
        if (left<=heapsize && TopK_Max_Heap[left].second > TopK_Max_Heap[i].second) maxindex = left;
        if (right<=heapsize && TopK_Max_Heap[right].second > TopK_Max_Heap[maxindex].second) maxindex = right;
        if (maxindex != i){
            std::swap(TopK_Max_Heap[i],TopK_Max_Heap[maxindex]);
            i = maxindex;
        } else break;
    }
}