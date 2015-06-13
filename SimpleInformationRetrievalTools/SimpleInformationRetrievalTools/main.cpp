//
//  main.cpp
//  SimpleInformationRetrievalTools
//
//  Created by SmilENow on 6/9/15.
//  Copyright (c) 2015 Jiaquan Yin. All rights reserved.
//

#include <iostream>
#include "InvertedIndex.h"
#include "BoolQuery.h"
#include "TopK.h"
#include "Synonym.h"

InvertedIndex *II = new InvertedIndex();
Synonym *SYN = new Synonym();

int main(int argc, const char * argv[]) {
    II->LoadStopWordList();
    II->BuildFromFiles();
    II->PrintInvertedIndexList();
    II->PrintPositingList();
    SYN->BuildSynonymList();
    return 0;
}
