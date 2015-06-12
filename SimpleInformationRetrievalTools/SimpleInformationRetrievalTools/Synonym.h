//
//  Synonym.h
//  SimpleInformationRetrievalTools
//
//  Created by SmilENow on 6/12/15.
//  Copyright (c) 2015 Jiaquan Yin. All rights reserved.
//

#ifndef __SimpleInformationRetrievalTools__Synonym__
#define __SimpleInformationRetrievalTools__Synonym__

#include <iostream>
#include "InvertedIndex.h"

class Synonym{
// Method
public:
    Synonym(){
        SynonymMap.clear();
    };
    virtual ~Synonym(){};
    void BuildSynonymList();
    vector<pair<string, string>> findSynonym(vector<pair<string, string>>);
// Variable
public:
    unordered_map<string,vector<string>> SynonymMap;
};

#endif /* defined(__SimpleInformationRetrievalTools__Synonym__) */
