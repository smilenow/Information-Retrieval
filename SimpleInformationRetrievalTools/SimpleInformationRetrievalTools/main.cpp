//
//  main.cpp
//  SimpleInformationRetrievalTools
//
//  Created by SmilENow on 6/9/15.
//  Copyright (c) 2015 Jiaquan Yin, Zhendong Cao. All rights reserved.
//


#include <iostream>
#include <string>
#include <unordered_map>
#include "InvertedIndex.h"
#include "BoolQuery.h"
#include "TopK.h"
#include "Synonym.h"
#include "Interpreter.h"
#include "VectorSpaceModel.h"

using namespace std;

InvertedIndex *II = new InvertedIndex();
Synonym *SYN = new Synonym();
Interpreter *IP = new Interpreter();
VectorSpaceModel *VSM;

int main(int argc, const char * argv[]) {
    II->LoadStopWordList();
    II->BuildFromFiles();
    II->PrintInvertedIndexList();
    II->PrintPositingList();
    SYN->BuildSynonymList();
    VSM = new VectorSpaceModel(II->InvertedIndexListMap);

    cout<<"Inverted Index Size: "<< II->InvertedIndexListMap.size()<<endl;
    
    while(true){
    	string query;
    	cout<<"> ";
    	getline(cin, query);
	    auto q  = IP->ProcessQuery(query);
	    
	    for(auto &r:q){
	    	cout <<"op: "<<r.first<<" term: "<<r.second<<endl;
	    }
	    auto res = VSM->GetRankingResult(q);

	    cout<<"---------------- TOP 100 RESULTS ----------------" <<endl;
	    int i= 0;
	    for(auto &r:res){
	    	cout <<"docID: "<<r.first<<" score: "<<r.second<<endl;
	    	i++;
	    	if (i == 100){
	    		break;
	    	}
	    }

	    cout<<"---------------- TOP 100 RESULTS ----------------" <<endl;
    }

    return 0;
}
