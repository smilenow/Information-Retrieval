#ifndef __SimpleInformationRetrievalTools__SpellingChecker__
#define __SimpleInformationRetrievalTools__SpellingChecker__

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
// #include "include/rapidjson/document.h"
// #include "include/rapidjson/writer.h"
// #include "include/rapidjson/stringbuffer.h"
// #include "include/rapidjson/filereadstream.h"

using namespace std;

class SpellingChecker
{
public:
	SpellingChecker();
	~SpellingChecker();
	vector< pair<string, string> > CheckQuery(vector< pair<string, string> > &query);
private:
	map<string, int> dict;
	int LoadDict();
	int CalculateEditDist(const string & a, const string & b);
	string CheckWord(const string & queryword);
	// vector<string> &SplitAndCheck(const string & query, char delimiter, vector<string> & words);
};
#endif