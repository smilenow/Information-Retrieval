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
#include "StaticQualityScore.h"
#include "PhraseQuery.h"

using namespace std;

InvertedIndex *II = new InvertedIndex();
Synonym *SYN = new Synonym();
Interpreter *IP = new Interpreter();
VectorSpaceModel *VSM;
StaticQualityScore *SQS;
PhraseQuery *PQ;

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

	    unordered_map<string, double> SQS_score;
	    if (IP->GetTopKMode() == TOP_K_STATIC_QUALITY_SCORE){
	    	SQS = new StaticQualityScore(II->InvertedIndexList);
	    	SQS_score = SQS->GetStaticQualityScore();
	    }

	    for(auto &r:q){
	    	cout <<"op: "<<r.first<<" term: "<<r.second<<endl;
	    }

	    vector<pair<string, double>> res;

	    if (IP->GetTopKMode() == TOP_K_STATIC_QUALITY_SCORE){
		    res = VSM->GetRankingResult(q, SQS_score);
	    }
	    else{
		    res = VSM->GetRankingResult(q);
	    }
	    

	    cout<<"---------------- TOP 50 RESULTS ----------------" <<endl;
	    int i= 0;
	    for(auto &r:res){
	    	cout <<"docID: "<<r.first<<" score: "<<r.second<<" SQS: "<< SQS_score[r.first] <<endl;
	    	i++;
	    	if (i == 50){
	    		break;
	    	}
	    }

	    cout<<"---------------- TOP 50 RESULTS ----------------" <<endl;
    }

    return 0;
}
