#ifndef __SimpleInformationRetrievalTools__SpellingChecker__
#define __SimpleInformationRetrievalTools__SpellingChecker__

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

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
};
#endif