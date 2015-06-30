
#ifndef __SimpleInformationRetrievalTools__ClusterPruning__
#define __SimpleInformationRetrievalTools__ClusterPruning__

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <memory>
#include <cmath>
#include <algorithm>
#include <set>
#include <cassert>
#include <unordered_map>
#include "InvertedIndex.h"
#include "VectorSpaceModel.h"

using namespace std;
using type_II=unordered_map<string,vector<InvertedIndex::InvertedIndexListSubNode> >;
using pii=pair<int,int>;
using pdi=pair<double,int>;



class ClusterPruning{
public:
  ClusterPruning(type_II& II,int b1=-1,int b2=-1);
	virtual ~ClusterPruning(){};
	vector<pair<string, double>> GetRankingResult(vector<pair<string, string> >&q);

private:
  void preprocess();
  int calc_ndoc();
  void construct_leader();
  void construct_follower();
  vector<int> rnd_k(int n,int k);   //random choose k numbers int [0,n)
  vector<pdi> get_k_nearest(const vector<double> &vec_id,int k); // get k nearest doc_ids
  double calc_dis(int a,int b);
  double calc_dis(const vector<double> a_vec,int b);
  vector<double> get_doc_vec(int a);
  double vec_dot(const vector<double> &pa,const vector<double> &pb);
	vector<double> get_query_vec(const vector<pair<string, string> >&q);
  void cmbline(vector<pair<string,double> >&a,vector<pair<string,double> >&b);
  void gosomeleader();
  vector<pair<string,double>> query_res(vector<pair<string,string> >&q);
  
private:
  int b1,b2;  //clustering pruning parameter
  int n_term;      //number of terms
  int n_doc;      //number of docs
  int n_leader;   //number of leader
  type_II *ptr_ii;
  unordered_map<string,int> term_pos;
  vector<pii> leader;  //first: doc_id  second:index in @docs
  vector<int> docs;
  unordered_map<int,vector<int> >g;
  int doc_lb(const vector<InvertedIndex::InvertedIndexListSubNode>&p,int doc_id);
};


#endif
