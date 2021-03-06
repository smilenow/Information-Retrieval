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
#include <chrono>
#include "InvertedIndex.h"
#include "BoolQuery.h"
#include "TopK.h"
#include "Synonym.h"
#include "Interpreter.h"
#include "VectorSpaceModel.h"
#include "StaticQualityScore.h"
#include "SpellingChecker.h"
#include "ChampionList.h"
#include "PhraseQuery.h"
#include "ClusterPruning.h"

using namespace std;

InvertedIndex *II = new InvertedIndex();
Synonym *SYN = new Synonym();
Interpreter *IP = new Interpreter();
SpellingChecker *SC = new SpellingChecker();
ChampionList *CL;
VectorSpaceModel *VSM;
StaticQualityScore *SQS;
PhraseQuery *PQ;
TopK *TOPKHEAP = new TopK();
ClusterPruning *CP;

int main(int argc, const char * argv[]) {
	cout << "Calculating Inverted Index..." << endl;
	II->LoadStopWordList();
	II->BuildFromFiles();
	II->PrintInvertedIndexList();
	II->PrintPositingList();
	SYN->BuildSynonymList();
	VSM = new VectorSpaceModel(II->InvertedIndexListMap);
	CL = new ChampionList(II->InvertedIndexList);
	CP=new ClusterPruning(II->InvertedIndexListMap);
	PQ=new PhraseQuery(II->PositingListMap);
	unordered_map<string, double> SQS_score;

	cout<<"Inverted Index Size: "<< II->InvertedIndexListMap.size()<<endl;
	SQS = new StaticQualityScore(II->InvertedIndexList);
	SQS_score = SQS->GetStaticQualityScore();
	
	while(true){
		string query;
		cout<<"> ";
		getline(cin, query);
		auto start = chrono::system_clock::now();
		auto q  = IP->ProcessQuery(II, query);

		SC->CheckQuery(q);

		cout<<"Search type: "<< IP->GetSearchType()<<" TopK mode: " << IP->GetTopKMode() <<" Synonym: "<< IP->GetSynonymMode()<<endl;
		if (IP->GetSynonymMode() == SYNONYM_ON) q = SYN->findSynonym(q);


		
		// if (IP->GetTopKMode() == TOP_K_STATIC_QUALITY_SCORE){
			
		// }

		// for(auto &r:q){
		// 	cout <<"op: "<<r.first<<" term: "<<r.second<<endl;
		// }

		vector<pair<string, double>> res;

		if (IP->GetTopKMode() == TOP_K_STATIC_QUALITY_SCORE){
			res = VSM->GetRankingResult(q, SQS_score);
		}
		else if (IP->GetTopKMode() == TOP_K_CLUSTER_PRUNING){        
			res = CP->GetRankingResult(q);
		}
		else if (IP->GetTopKMode() == TOP_K_CHAMPION_LIST){
			res = CL->GetRankingResult(q);
		}
		else if (IP->GetTopKMode() == TOP_K_HEAP){
			res = TOPKHEAP->TopK_Heap(50, VSM->GetOriginalRankingResult(q));
		}
		else if (IP->GetSearchType() == BOOL){
			res = BoolQuery::FindBoolQuery(q);
		}
		else if (IP->GetSearchType() == PHRASE_SEARCH){
			res=PQ->GetRankingResult(q);
		}
		else
		{
			res = VSM->GetRankingResult(q);
		}
		auto duration = chrono::duration_cast< chrono::milliseconds> (chrono::system_clock::now() - start);
		cout << "Time Elapsed: " << duration.count() << "ms" << endl;

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
