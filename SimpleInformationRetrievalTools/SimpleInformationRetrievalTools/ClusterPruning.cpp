#include "ClusterPruning.h"

extern VectorSpaceModel *VSM;

ClusterPruning::ClusterPruning(type_II& II,int b1,int b2){
  this->b1=this->b2=1;
  if(b1>0)this->b1=b1;
  if(b2>0)this->b2=b2;
  ptr_ii=&II;
  n_term=II.size();
  preprocess();
  int pos_cnt=0;
  for(auto &t:*ptr_ii)
    term_pos[t.first]=pos_cnt++;
  // for(auto &e:leader)
  //   cout<<e.first<<" "<<g[e.first].size()<<"\n";
}

void ClusterPruning::preprocess(){
  docs.clear();
  n_doc=calc_ndoc();
  n_leader=sqrt(n_doc);
  construct_leader();
  construct_follower();
}

int ClusterPruning::calc_ndoc(){
  auto &tp_d=docs;
  auto &dic=*ptr_ii;
  for(auto &t:dic)
  for(auto &doc:t.second)
    tp_d.push_back(stoi(doc.DocID));
  sort(tp_d.begin(),tp_d.end());
  tp_d.erase(unique(tp_d.begin(),tp_d.end()),tp_d.end());
  return (int)tp_d.size();
}

void ClusterPruning::construct_leader(){
  leader.clear();
  // auto leader_id=rnd_k(n_doc,n_leader);
  // for(int i=0;i<n_leader;++i)
  //   leader.push_back(pii(docs[leader_id[i]],leader_id[i]));
  // sort(leader.begin(),leader.end(),[](pii a,pii b){return a.second<b.second;});
  FILE *fp=fopen("clustering_ctrs","r");
  if(!fp){
    puts("can not open file clustering_ctrs");
    exit(0);
  }
  int u;
  while(fscanf(fp,"%d",&u)==1)leader.push_back(make_pair(u,(int)(lower_bound(docs.begin(),docs.end(),u)-docs.begin())));
}

void ClusterPruning::construct_follower(){
  //Following commented codes are neccessary
  // ofstream fout("data",std::ofstream::out);
  // int c=0;
  // for(auto &pp:*ptr_ii){
  //   double df=pp.second.size();
  //   double idf=log10(n_doc/df);
  //   for(auto &d:pp.second)
  //       fout<<d.DocID<<" "<<d.sum*idf<<" : ";
  //   fout<<"\n";
  // }

  // fout.close();
  // exit(0);
  // ofstream fout("gdata",std::ofstream::out);
  // g.clear();
  // int lind=0;
  // for(int i=0;i<n_doc;++i){
  //   if(lind<n_leader && i==leader[i].second){
  //     ++lind;
  //     continue;
  //   }
  //   int doc_id=docs[i];
  //   auto tp=get_k_nearest(get_doc_vec(doc_id),b1);
  //   for(int j=0;j<b1;++j){
  //     g[doc_id].push_back(tp[j].second);
  //     fout<<doc_id<<" "<<tp[j].second<<endl;
  //   }
  //   for(int d:g[doc_id])
  //     g[d].push_back(doc_id);
  //   printf("i=%d\n",i);fflush(stdout);
  // }
  // exit(0);

  FILE *fp=fopen("clustering_data","r");
  if(!fp){
    puts("failed to open file clustering_data!");
    exit(0);
  }
  int u,v;
  while(fscanf(fp,"%d %d",&u,&v)==2){
    // cout<<u<<" "<<v<<endl;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  gosomeleader();
  fclose(fp);
}

vector<int> ClusterPruning::rnd_k(int n,int k){
  vector<int> p,r;
  for(int i=0;i<n;++i)p.push_back(i);
  srand(time(NULL));
  random_shuffle(p.begin(),p.end());
  for(int i=0;i<k;++i)r.push_back(p[i]);
  return r;
}

vector<pdi> ClusterPruning::get_k_nearest(const vector<double> &vec_id,int k){
  vector<pdi> p;
  for(auto &e:leader){
    int leader_did=e.first;
    auto vec_l=get_doc_vec(leader_did);
    p.push_back(make_pair(vec_dot(vec_id,vec_l),leader_did));
  }
  nth_element(p.begin(),p.begin()+k-1,p.end());
  p.erase(p.begin()+k,p.end());
  return p;
}

double ClusterPruning::calc_dis(int a,int b){
  auto pa=get_doc_vec(a);
  auto pb=get_doc_vec(b);
  return vec_dot(pa,pb);
}

double ClusterPruning::calc_dis(const vector<double> pa,int b){
  auto pb=get_doc_vec(b);
  return vec_dot(pa,pb);
}

vector<double> ClusterPruning::get_doc_vec(int doc_id){
  auto &dic=*ptr_ii;
  vector<double> r;
  double sum=0;
  for(auto &pp_term:dic){
    auto &pl=pp_term.second;
    double df=pl.size();
    double idf = log10(n_doc / df);
    int ind=doc_lb(pl,doc_id);
    if(ind<0)r.push_back(0);
    else r.push_back(pl[ind].sum*idf),sum+=r.back()*r.back();
  }
  sum=sqrt(sum);
  for(auto &x:r)x/=sum;
  return r;
}

void ClusterPruning::gosomeleader(){
  int m=sqrt(n_doc);
  for(auto &pp:leader){
    auto f=g.find(pp.first);
    if(f==g.end())continue;
    auto &e=f->second;
    random_shuffle(e.begin(),e.end());
    if((int)e.size()>m)
      e.erase(e.begin()+m,e.end());
  }    
}
void ClusterPruning::cmbline(vector<pair<string,double> >&a,vector<pair<string,double> >&b){
  int s=a.size(),i=0,os=s;
  while(s<50 && i<(int)b.size())
    a.push_back(b[i++]);  
  a.erase(unique(a.begin(),a.end()),a.end());
  if(os>10){
    auto rp=rnd_k(5,5);    
    vector<pair<string,double> > ra;
    for(int i=0;i<(int)a.size();++i){
      bool b=0;
      for(auto &e:rp)if(i+5==e)b=1;
      if(!b)ra.push_back(a[i]);
    }      
    a=ra;
  }  
}
vector<double> ClusterPruning::get_query_vec(const vector<pair<string, string> >&q){
  auto &dic=*ptr_ii;
  vector<double> r(n_term,0);
  double sum=0;
  set<string> distinct_term;
  for(auto &e:q){
    auto qterm=e.second;
    auto f=term_pos.find(qterm);
    if(f==term_pos.end())continue;
    int p=f->second;
    r[p]+=1;
    distinct_term.insert(qterm);
  }
  for(auto &qterm:distinct_term){
    auto f=term_pos.find(qterm);
    if(f==term_pos.end())continue;
    int p=f->second;
    double df=dic[qterm].size();
    assert(df>0);
    double idf=log10(n_doc/df);
    r[p]*=idf;
    sum+=r[p];
  }
  sum=sqrt(sum);
  for(auto &x:r)x/=sum;
  return r;
}

double ClusterPruning::vec_dot(const vector<double> &pa,const vector<double> &pb){
  double d=0;
  assert(pa.size()==pb.size());
  for(size_t i=0;i<pa.size();++i)
    d+=pa[i]*pb[i];
  return d;
}

vector<pair<string,double>> ClusterPruning::query_res(vector<pair<string,string> >&q){
  auto q_vec=get_query_vec(q);  
  vector<string> possible_docs;    
  for(auto &e:leader)
    possible_docs.push_back(to_string(e.first));
  return VSM->GetRankingResult(q,possible_docs);
}
int ClusterPruning::doc_lb(const vector<InvertedIndex::InvertedIndexListSubNode>&p, int id){
  int l=0,r=(int)p.size()-1;
  while(l<r){
    int m=(l+r)>>1;
    if(stoi(p[m].DocID)>=id)r=m;
    else l=m+1;
  }
  return stoi(p[l].DocID)==id?l:-1;
}

vector<pair<string, double>> ClusterPruning::GetRankingResult(vector<pair<string,string> >&q){
  auto oq=q;
  auto cres=VSM->GetRankingResult(q);  
  q=oq;  
  auto qres=query_res(q);
  cmbline(cres,qres);
  return cres;  
}
