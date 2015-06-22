
#ifndef __SimpleInformationRetrievalTools__PhraseQuery__
#define __SimpleInformationRetrievalTools__PhraseQuery__

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <ctype.h>
#include <tuple>
#include <deque>
#include <string>
#include <memory>
#include <cmath>
#include <algorithm>
#include <set>
#include <cassert>
#include <unordered_map>
#include "InvertedIndex.h"
using type_T=tuple<string,int,int>;
class PhraseQuery{
public:
  using type_PI=unordered_map<string, vector<InvertedIndex::PositingListSubNode>>;
  using type_PL=vector<InvertedIndex::PositingListSubNode>;
  using pii=pair<int,int>;
  using pdi=pair<double,int>;
  using type_T=tuple<string,int,int>;

public:
  PhraseQuery(type_PI &PI);
  virtual ~PhraseQuery(){};
  vector<string> GetRankingResult(const vector<pair<string, string> >&q);

private:
  vector<string> query(const string &qt1,const string &qt2);
  type_PL get_posting_list(const string &term);
  vector<type_T> positional_intersect(const type_PL &p1,const type_PL &p2,int k);
  vector<string> get_intersect(vector<vector<string> >&cres);
private:
  type_PI *ptr_pi;
  int _k;           //parameter of the phrase query
};
#endif
