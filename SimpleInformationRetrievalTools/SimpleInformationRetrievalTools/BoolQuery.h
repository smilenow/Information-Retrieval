//
//  BoolQuery.h
//  SimpleInformationRetrievalTools
//
//  Created by SmilENow on 6/10/15.
//  Copyright (c) 2015 Jiaquan Yin. All rights reserved.
//

#ifndef __SimpleInformationRetrievalTools__BoolQuery__
#define __SimpleInformationRetrievalTools__BoolQuery__

#include <iostream>
#include "InvertedIndex.h"

extern InvertedIndex* II;

class BoolQuery{
public:
    BoolQuery(){};
    virtual ~BoolQuery(){};
    static vector<string> FindBoolQuery(vector<pair<string,string>>);
};

#endif /* defined(__SimpleInformationRetrievalTools__BoolQuery__) */
