#include "ChampionList.h"

vector< pair<string, double> > ChampionList::GetRankingResult(vector< pair<string, string> >& query)
{
	int TotalNumberOfDocs;
	vector<string> QueryDocs;
	map<string, map<string, double> > doc_tf;
	map<string, double> query_tf;
	vector< pair<string, double> > Scores;

	// find all set A
	// begin = clock();
	for (auto &item : query)
	{
		auto pos = _ChampionListMap.find(item.second);
		if (pos != _ChampionListMap.end())
		{
			for (auto &doc : pos->second)
			{
				auto docPos = find(QueryDocs.begin(), QueryDocs.end(), doc.DocID);
				if (docPos == QueryDocs.end())
				{
					QueryDocs.push_back(doc.DocID);
				}
			}
		}
	}

	TotalNumberOfDocs = QueryDocs.size();

	// doc tf TOO NAIVE TOO SLOW
/*	for (auto &doc : QueryDocs)
	{
		begin = clock();
		map<string, double> doc_tf_map;
		for (auto &t : _ChampionListMap)
		{
			auto pos = FindDoc(t.second, doc);
			if (pos != t.second.end())
			{
				doc_tf_map.insert(make_pair(t.first, pos->sum));
			}
		}
		doc_tf.insert(make_pair(doc, doc_tf_map));
		end = clock();
		cout << doc + " " << end - begin << endl;
	}*/

	// doc tf
	// better version
	for (auto &t : _ChampionListMap)
	{
		for (auto &tt : t.second)
		{
			auto doc = find(QueryDocs.begin(), QueryDocs.end(), tt.DocID);
			if (doc != QueryDocs.end())
			{
				auto ttt = FindDocInMap(doc_tf, tt.DocID);
				if (ttt != doc_tf.end())
				{
					ttt->second.insert(make_pair(t.first, tt.sum));
				}
				else
				{
					map<string, double> doc_tf_map;
					doc_tf_map.insert(make_pair(t.first, tt.sum));
					doc_tf.insert(make_pair(tt.DocID, doc_tf_map));
				}
			}
		}
	}
	// end = clock();
	// cout << end - begin << endl;

	// query tf
	for (auto &item : query)
	{
		auto t = FindTerm(item.second);
		if (t != _ChampionListMap.end())
		{
			auto tt = FindQueryWord(query_tf, item.second);
			if (tt != query_tf.end())
			{
				tt->second = tt->second + 1;
			}
			else
			{
				query_tf.insert(pair<string, double>(item.second, 1.0));
			}
		}
	}

	// multiply idf
	for (auto &t : query_tf)
	{
		t.second *= log10(TotalNumberOfDocs * 1.0 / (_ChampionListMap[t.first].size() - 1));
	}

	// normalize
	Normalize(query_tf);

	for (auto &item : doc_tf)
	{
		Normalize(item.second);
	}


	Scores = GetScore(query_tf, doc_tf);


	sort(Scores.begin(), Scores.end(), [](pair<string, double> t1, pair<string, double> t2){
		return t1.second > t2.second;
	});


	return Scores;
}

map<string, map<string, double> >::iterator ChampionList::FindDocInMap(map<string, map<string, double> > &m, string & s)
{
	for (auto it = m.begin(); it != m.end(); it++)
	{
		if (it->first == s)
		{
			return it;
		}
	}
	return m.end();
}

vector< pair<string, double> > ChampionList::GetScore(map<string, double> & q, map<string, map<string, double> > & d)
{
	vector< pair<string, double> > result;
	for (auto &doc : d)
	{
		double score = 0;
		auto qit = q.begin();
		auto dit = doc.second.begin();
		for (; qit != q.end() && dit != doc.second.end(); )
		{
			if (qit->first == dit->first)
			{
				score += qit->second * dit->second;
				qit++;
				dit++;
			}
			else if(qit->first < dit->first)
			{
				qit++;
			}
			else if(qit->first > dit->first)
			{
				dit++;
			}
		}
		result.push_back(make_pair(doc.first, score));
	}
	return result;
}

void ChampionList::Normalize(map<string, double> &m)
{
	double sum = 0;
	for (auto &item : m)
	{
		sum += pow(item.second, 2);
	}
	// cout << sum << endl;
	sum = pow(sum, 0.5);
	for (auto &item : m)
	{
		// cout << item.second << endl;
		item.second /= sum;
		// cout << item.second << endl;
	}
}

map<string, double>::iterator ChampionList::FindQueryWord(map<string, double> & m, const string & s)
{
	for (auto iter = m.begin(); iter != m.end(); iter++)
	{
		if (iter->first == s)
		{
			return iter;
		}
	}
	return m.end();
}

unordered_map<string, vector<InvertedIndex::InvertedIndexListSubNode>>::iterator ChampionList::FindTerm(const string & s)
{
	for (auto iter = _ChampionListMap.begin(); iter != _ChampionListMap.end(); iter++)
	{
		if (iter->first == s)
		{
			return iter;
		}
	}
	return _ChampionListMap.end();
}

vector<InvertedIndex::InvertedIndexListSubNode>::iterator ChampionList::FindDoc(vector<InvertedIndex::InvertedIndexListSubNode> &v, const string & d)
{
	for (auto subnode = v.begin(); subnode != v.end(); subnode++)
	{
		if (subnode->DocID == d)
		{
			return subnode;
		}
	}
	return v.end();
}

ChampionList::ChampionList(vector<InvertedIndex::InvertedIndexListNode>& inverted_index)
{
	BuildChampionList(inverted_index);
	BuildChampionListMap();
}

void ChampionList::BuildChampionList(vector<InvertedIndex::InvertedIndexListNode> inverted_index)
{
	int DocCount;
	int halfOfDoc;
	for (auto &node : inverted_index)
	{
		DocCount = 0;
		InvertedIndex::InvertedIndexListNode ChampionListNode(node.term);
		vector<InvertedIndex::InvertedIndexListSubNode> value;
		sort(node.value.begin(), node.value.end(),[]
			(InvertedIndex::InvertedIndexListSubNode a, InvertedIndex::InvertedIndexListSubNode b) { return a.sum > b.sum; });
		DocCount = (node.value.size() + 1) / 2;
		DocCount = min(100, DocCount);
		halfOfDoc = 0;
 		for (auto subnode = node.value.begin(); halfOfDoc < DocCount; subnode++, halfOfDoc++)
		{
			value.push_back(InvertedIndex::InvertedIndexListSubNode(subnode->DocID, subnode->sum));
		}
		ChampionListNode.value = value;
		_ChampionList.push_back(ChampionListNode);
	}
}

void ChampionList::BuildChampionListMap()
{
	for (auto &node : _ChampionList)
	{
		_ChampionListMap.insert(make_pair(node.term, node.value));
	}
}