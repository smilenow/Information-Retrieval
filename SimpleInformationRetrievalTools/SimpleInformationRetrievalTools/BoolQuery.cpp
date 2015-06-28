//
//  BoolQuery.cpp
//  SimpleInformationRetrievalTools
//
//  Created by SmilENow on 6/10/15.
//  Copyright (c) 2015 Jiaquan Yin. All rights reserved.
//

#include "BoolQuery.h"

vector<pair<string, double>> BoolQuery::FindBoolQuery(vector<pair<string, string> > query){
    vector<pair<string, double>> ret;
    unordered_map<string, int> umap,tmp;

    for (auto &i:II->AllDoc) umap[i] = 1;
    
    for (auto &q:query){
        tmp.clear();
        if (q.first=="AND"){
            for (auto &i:II->AllDoc) tmp[i] = 0;
            for (auto &i:II->InvertedIndexListMap[q.second]) tmp[i.DocID] = 1;
            for (auto &i:II->AllDoc) umap[i] = umap[i] & tmp[i];
        } else if (q.first=="OR"){
            for (auto &i:II->AllDoc) tmp[i] = 0;
            for (auto &i:II->InvertedIndexListMap[q.second]) tmp[i.DocID] = 1;
            for (auto &i:II->AllDoc) umap[i] = umap[i] | tmp[i];
        } else {
            for (auto &i:II->AllDoc) tmp[i] = 1;
            for (auto &i:II->InvertedIndexListMap[q.second]) tmp[i.DocID] = 0;
            for (auto &i:II->AllDoc) umap[i] = umap[i] & tmp[i];
        }
    }
    
    for (auto &i:umap)
        if (i.second == 1) ret.push_back(make_pair(i.first,0.0));
    
    return ret;
}