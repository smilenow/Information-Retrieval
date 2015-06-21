//
//  interpreter.cpp
//  SimpleInformationRetrievalTools
//
//  Created by ryecao on 6/9/15.
//  Copyright (c) 2015 Zhendong Cao. All rights reserved.
//

#include "interpreter.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cctype>

using namespace std;

vector<pair<string,string> > Interpreter::ProcessQuery(const string query){
	istringstream query_stream(query);
	string word;
	vector<int> bool_op_pos;
	vector<string> query_vector;
	vector<string> parameters;
	vector<pair<string,string> > result;
	
	bool search_type_set = false;
	bool synonym_set = false;
	bool top_k_set = false;

	int pos = 0;
	while(query_stream >> word){
		if (word == "AND" || word == "OR" || word == "NOT" ){
			_search_type = BOOL;
			search_type_set = true;
			bool_op_pos.push_back(pos);
		}
		if (word[0] == '-') {
			parameters.push_back(word);
		}
		else{
			query_vector.push_back(word);
		}
		pos++;
	}

	for (auto &para:parameters){
		if(search_type_set == false){
			if (para == "-PHRASE_SEARCH"){
				_search_type = PHRASE_SEARCH;
				search_type_set = true;
			}
		}

		if (synonym_set == false){
			if (para == "-SYNONYM_ON"){
				_synonym_mode = SYNONYM_ON;
				synonym_set = true;
			}
			else if (para == "-SYNONYM_OFF"){
				_synonym_mode = SYNONYM_OFF;
				synonym_set = true;
			}
		}
		
		if (top_k_set == false){
			if ( para == "-TOP_K_OFF"){
				_top_k_mode = TOP_K_OFF;
				top_k_set = true;
			}
			if ( para == "-TOP_K_HEAP"){
				_top_k_mode = TOP_K_HEAP;
				top_k_set = true;
			}
			if ( para == "-TOP_K_CHAMPION_LIST"){
				_top_k_mode = TOP_K_CHAMPION_LIST;
				top_k_set = true;
			}
			if ( para == "-TOP_K_STATIC_QUALITY_SCORE"){
				_top_k_mode = TOP_K_STATIC_QUALITY_SCORE;
				top_k_set = true;
			}
			if ( para == "-TOP_K_CLUSTER_PRUNING"){
				_top_k_mode = TOP_K_CLUSTER_PRUNING;
				top_k_set = true;
			}
		}
	}

	if (_search_type == BOOL){
		string segment = "";
		int bool_pos = 0;	
		for (int i = 0; i < query_vector.size(); ++i){
			if(i == bool_op_pos[bool_pos]){
				
				if (segment != ""){
					if (bool_pos == 0){
						result.push_back(make_pair("AND",segment));
					}
					else{
						result.push_back(make_pair(query_vector[bool_op_pos[bool_pos-1]],segment.substr(0, segment.size()-1)));
					}
				}
				bool_pos++;
				segment = "";
				continue;
			}
			segment += query_vector[i] + " ";
		}	

		result.push_back(make_pair(query_vector[bool_op_pos[bool_pos-1]],segment));

	}
	else{
		for (auto &q:query_vector){
			result.push_back(make_pair("AND", q));
		}
	}

	return result;
}
