#include "PhraseQuery.h"
PhraseQuery::PhraseQuery(type_PI &PI){
  ptr_pi=&PI;
  _k=3;
}

vector<pair<string,double> >PhraseQuery::GetRankingResult(const vector<pair<string, string> >&qq){
  auto q=qq;
  for(auto &e:q)
    for(auto &c:e.second)
      c=tolower(c);
  if(q.size()<2){
    puts("You should input at lease 2 words for phrase query.");
    return vector<pair<string,double> >();
  }
  vector<vector<string> >cres;
  for(size_t i=0;i<q.size()-1;++i){
    cres.push_back(query(q[i].second,q[i+1].second));
  }
  auto res=get_intersect(cres);
  vector<pair<string,double> >rr;
  for(auto &e:res)rr.push_back(make_pair(e,0.0));
  sort(rr.begin(),rr.end());
  rr.erase(unique(rr.begin(),rr.end()),rr.end());
  return rr;
}

vector<string> PhraseQuery::query(const string &qt1,const string &qt2){
  auto p1=get_posting_list(qt1);
  auto p2=get_posting_list(qt2);
  if(p1.empty() || p2.empty())
    return vector<string>();
  auto r=positional_intersect(p1,p2,_k);
  vector<string> ans;
  for(auto &e:r)
    ans.push_back(get<0>(e));
  return ans;
}

PhraseQuery::type_PL PhraseQuery::get_posting_list(const string &term){
  auto &dic=*ptr_pi;
  auto f=dic.find(term);
  if(f==dic.end())
    return type_PL();
  return f->second;
}

void prt(const PhraseQuery::type_PL &P){
  puts("**********************************");
  for(auto &e:P){
    cout<<e.DocID<<" ";
    for(auto &ps:e.pos)
      cout<<ps<<" ";
    cout<<endl;
  }
  puts("**********************************");
}
vector<PhraseQuery::type_T> PhraseQuery::positional_intersect(const type_PL &P1,const type_PL &P2,int k){
  vector<PhraseQuery::type_T> ans;
  size_t p1=0,p2=0;
  // prt(P1),prt(P2);
  while(p1<P1.size() && p2<P2.size()){
    if(P1[p1].DocID==P2[p2].DocID){
      deque<int> l;
      const auto &PP1=P1[p1].pos;
      const auto &PP2=P2[p2].pos;
      size_t pp1=0,pp2=0;
      while(pp1<PP1.size()){
        while(pp2<PP2.size()){
          if(abs(PP1[pp1]-PP2[pp2])<=k){
            l.push_back(PP2[pp2]);
          }else if(PP2[pp2]>PP1[pp1]+k)
            break;
          ++pp2;
        }
        while(!l.empty() && abs(l[0]-PP1[pp1])>k)
          l.pop_front();
        for(auto it=l.begin();it!=l.end();++it)
          ans.push_back(type_T(P1[p1].DocID,PP1[pp1],*it));
        ++pp1;
      }
      ++p1,++p2;
    }else if(stoi(P1[p1].DocID)<stoi(P2[p2].DocID))
      ++p1;
    else
      ++p2;
  }
  return ans;
}

vector<string> PhraseQuery::get_intersect(vector<vector<string> >&cres){
  if(cres.empty())return vector<string>();
  for(auto &e:cres)
    sort(e.begin(),e.end());
  vector<string> ans=cres[0];
  for(size_t i=1;i<cres.size();++i){
    auto &cur=cres[i];
    vector<string> tp;
    set_intersection(ans.begin(),ans.end(),cur.begin(),cur.end(),tp.begin());
    ans.swap(tp);
  }
  return ans;
}
