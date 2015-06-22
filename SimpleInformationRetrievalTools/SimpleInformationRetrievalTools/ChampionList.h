#ifndef __SimpleInformationRetrievalTools__ChampionList__
#define __SimpleInformationRetrievalTools__ChampionList__

#include <vector>
#include <algorithm>
#include <map>
#include "InvertedIndex.h"

class ChampionList
{
public:
	ChampionList(vector<InvertedIndex::InvertedIndexListNode>& inverted_index);
	~ChampionList();

	vector< pair<string, double> > GetRankingResult(vector< pair<string, string> >& query);
private:
	void BuildChampionList(vector<InvertedIndex::InvertedIndexListNode> inverted_index);
	void BuildChampionListMap();
	vector<InvertedIndex::InvertedIndexListSubNode>::iterator FindDoc(vector<InvertedIndex::InvertedIndexListSubNode> &v, const string & d);
	map<string, double>::iterator FindQueryWord(map<string, double> & m, const string & s);
	unordered_map<string, vector<InvertedIndex::InvertedIndexListSubNode>>::iterator FindTerm(const string & s);
	void Normalize(map<string, double> &m);
	vector< pair<string, double> > GetScore(map<string, double> & q, map<string, map<string, double> > & d);

	vector<InvertedIndex::InvertedIndexListNode> _ChampionList;
	unordered_map<string, vector<InvertedIndex::InvertedIndexListSubNode>> _ChampionListMap;
};

#endif