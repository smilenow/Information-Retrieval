//
//  interpreter.cpp
//  SimpleInformationRetrievalTools
//
//  Created by ryecao on 6/9/15.
//  Copyright (c) 2015 Zhendong Cao. All rights reserved.
//

#ifndef __SimpleInformationRetrievalTools__Interpreter__
#define __SimpleInformationRetrievalTools__Interpreter__

#include <string>
#include <vector>

using namespace std;

enum SEARCH_TYPE
{
	NORMAL,
	BOOL,
	PHRASE_SEARCH
};

enum TOP_K_MODE
{
	TOP_K_OFF,
	TOP_K_HEAP,
	TOP_K_CHAMPION_LIST,
	TOP_K_STATIC_QUALITY_SCORE,
	TOP_K_CLUSTER_PRUNING
};

enum SYNONYM
{
	SYNONYM_ON,
	SYNONYM_OFF
};

class Interpreter
{
public:
	Interpreter(){_search_type = NORMAL; _top_k_mode = TOP_K_OFF; _synonym_mode = SYNONYM_OFF;};
	~Interpreter();
	int GetSearchType(){return _search_type;};
	int GetTopKMode(){return _top_k_mode;};
	int GetSynonymMode(){return _synonym_mode;};
	vector<pair<string,string> > ProcessQuery(const string query);
private:
	int _search_type;
	int _top_k_mode;
	int _synonym_mode;
};
#endif