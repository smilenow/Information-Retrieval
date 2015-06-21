//
//  StaticQualityScore.cpp
//  SimpleInformationRetrievalTools
//
//  Created by ryecao on 6/22/15.
//  Copyright (c) 2015 Zhendong Cao. All rights reserved.
//
#include <string>
#include <vector>
#include <unordered_map>
#include "InvertedIndex.h"

#ifndef __SimpleInformationRetrievalTools__StaticQualityScore__
#define __SimpleInformationRetrievalTools__StaticQualityScore__ value

class StaticQualityScore
{
public:
	StaticQualityScore(vector<InvertedIndex::InvertedIndexListNode>& inverted_index);
	~StaticQualityScore();
	unordered_map<string, double> GetStaticQualityScore(){return _scores;};

private:
	unordered_map<string, double> _scores;
};
#endif
