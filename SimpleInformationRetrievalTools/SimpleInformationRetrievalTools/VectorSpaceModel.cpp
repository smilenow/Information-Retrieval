//
//  interpreter.cpp
//  SimpleInformationRetrievalTools
//
//  Created by ryecao on 6/21/15.
//  Copyright (c) 2015 Zhendong Cao. All rights reserved.
//

#include <string>
#include <vector>
#include <cmath>
#include <unordered_map>
#include "VectorSpaceModel.h"
#include "InvertedIndex.h"

using namespace std;


void VectorSpaceModel::NormalizeVector(vector<double>& v){
	double length = 0;
	for(auto &d : v){
		length += d * d;
	}
	if (length != 0){
		for(auto &d : v){
			d /= sqrt(length) ;
		}	
	}
}

double VectorSpaceModel::GetScore (vector<double>& v1, vector<double>& v2){
	if (v1.size() != v2.size()){
		cout << "sizes of two vectors must be same" <<endl;
		return -1;
	}
	
	double score = 0;

	for (int i = 0; i < v1.size(); ++i)
	{
		score += v1[i] * v2[i];
	}

	return score;
}

bool CompareDocID(string id1, string id2){
	int p1 = id1.find_last_of(".");
	int iid1 = stoi(id1.substr(0, p1));	

	int p2 = id2.find_last_of(".");
	int iid2 = stoi(id2.substr(0, p2));

	return iid1 < iid2;	
}

bool IsDocIDEqual(string id1, string id2){
	int p1 = id1.find_last_of(".");
	int iid1 = stoi(id1.substr(0, p1));	

	int p2 = id2.find_last_of(".");
	int iid2 = stoi(id2.substr(0, p2));

	return iid1 == iid2;	
}

vector<pair<string, double>> VectorSpaceModel::GetRankingResult(vector<pair<string,string> >& query){
	vector<string> terms; // merge duplicate terms
	vector<double> query_tfidf;
	vector<double> idfs;
	unordered_map<string, vector<double> > doc_tfidf;
	vector<string> possible_DocIDs;
	vector<pair<string, double> >doc_score;
	//calculate query tf
	for(auto &t: query){
		auto fpos = find(terms.begin(), terms.end(), t.second);
		if ( fpos != terms.end()){
			auto pos = fpos - terms.begin();
			query_tfidf[pos] += 1;
		}
		else{
			terms.push_back(t.second);
			query_tfidf.push_back(1);
		}
	}

	//multiply tf by idf
	for (int i = 0; i < terms.size(); ++i){
		if (_inverted_index.find(terms[i])!= _inverted_index.end()){
			auto nodes = _inverted_index[terms[i]];
			for(auto &d: nodes ){
				if (find(possible_DocIDs.begin(), possible_DocIDs.end(), d.DocID) == possible_DocIDs.end()){
					possible_DocIDs.push_back(d.DocID);
				}
			}
			double df = nodes.size(); //get df
			double idf = log10(_N / df);
			query_tfidf[i] *= idf;
		}
		else{
			query_tfidf[i] = 0;
		}
	}

	NormalizeVector(query_tfidf);

	sort(possible_DocIDs.begin(), possible_DocIDs.end(),[](string id1, string id2){
															int p1 = id1.find_last_of(".");
															int iid1 = stoi(id1.substr(0, p1));	

															int p2 = id2.find_last_of(".");
															int iid2 = stoi(id2.substr(0, p2));

															return iid1 < iid2;	
														});

	for (auto &t: terms){
		if (_inverted_index.find(t)!= _inverted_index.end()){
			int cur_pos = 0;
			auto nodes = _inverted_index[t];
			double df = nodes.size();
			double idf = log10(_N / df);


			for (auto & id: possible_DocIDs){
				while(cur_pos < df && CompareDocID(nodes[cur_pos].DocID, id) ){
					cur_pos++;
				}

				if (cur_pos > df-1){
					doc_tfidf[id].push_back(0);
					continue;
				}

				if (IsDocIDEqual(nodes[cur_pos].DocID, id)){
					double tf = nodes[cur_pos].sum;
					doc_tfidf[id].push_back(tf * idf);
				}
				else{
					doc_tfidf[id].push_back(0);
				}

			}
		}
		else{
			for (auto & id: possible_DocIDs){
				doc_tfidf[id].push_back(0);
			}
		}
	}


	for (auto &d: doc_tfidf){
		NormalizeVector(d.second);
		double score = GetScore(query_tfidf, d.second);
		doc_score.push_back(make_pair(d.first, score));
	}


	sort(doc_score.begin(), doc_score.end(), [](pair<string, double> s1, pair<string, double> s2){
													return s1.second > s2.second;
												});

	return doc_score;
}