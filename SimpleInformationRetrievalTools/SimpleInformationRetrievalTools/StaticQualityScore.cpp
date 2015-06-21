//
//  StaticQualityScore.cpp
//  SimpleInformationRetrievalTools
//
//  Created by ryecao on 6/22/15.
//  Copyright (c) 2015 Zhendong Cao. All rights reserved.
//

#include <string>
#include <vector>
#include <cmath>
#include <unordered_map>
#include "StaticQualityScore.h"
#include "InvertedIndex.h"

StaticQualityScore::StaticQualityScore(vector<InvertedIndex::InvertedIndexListNode>& inverted_index){
	for(auto &l: inverted_index){
		for(auto &n: l.value){
			if (_scores.find(n.DocID) == _scores.end()){
				_scores.insert(make_pair(n.DocID, n.sum));
			}
			else{
				_scores[n.DocID] += n.sum;
			}
		}
	}

	for(auto &u: _scores){
		u.second = log10(u.second)/10;
	}
}