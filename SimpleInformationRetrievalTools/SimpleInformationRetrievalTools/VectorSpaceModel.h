//
//  VectorSpaceModel.h
//  SimpleInformationRetrievalTools
//
//  Created by ryecao on 6/21/15.
//  Copyright (c) 2015 Zhendong Cao. All rights reserved.
//

#ifndef __SimpleInformationRetrievalTools__VectorSpaceModel__
#define __SimpleInformationRetrievalTools__VectorSpaceModel__

#include <string>
#include <vector>
#include <unordered_map>
#include "InvertedIndex.h"

using namespace std;

class VectorSpaceModel
{
public:
	VectorSpaceModel(unordered_map<string, vector<InvertedIndex::InvertedIndexListSubNode> >& inverted_index)
					{_inverted_index = inverted_index; _N = double(inverted_index.size());};
	~VectorSpaceModel();
	// <DocID, td-idf> (sorted)
	vector<pair<string, double>> GetRankingResult(vector<pair<string,string> >& query);
	vector<pair<string, double>> GetRankingResult(vector<pair<string,string> >& query, unordered_map<string, double> static_scores);
private:
	void NormalizeVector(vector<double>& v);
	double GetScore (vector<double>& v1, vector<double>& v2);

	unordered_map<string, vector<InvertedIndex::InvertedIndexListSubNode> > _inverted_index;
	double _N; // total # of docs	
};
#endif