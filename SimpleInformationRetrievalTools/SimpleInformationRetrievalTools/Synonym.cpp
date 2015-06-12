//
//  Synonym.cpp
//  SimpleInformationRetrievalTools
//
//  Created by SmilENow on 6/12/15.
//  Copyright (c) 2015 Jiaquan Yin. All rights reserved.
//

#include "Synonym.h"

void Synonym::BuildSynonymList(){
    vector<string> cur;
    ifstream fin;
    string str;
    string filedir = "./SynonymsList.txt";
    fin.open(filedir.c_str(),ios::in);
    if (fin.is_open()){
        while (fin.peek() != EOF){
            fin >> str;
            if (fin.eof()) break;
            transform(str.begin(), str.end(), str.begin(), ::tolower);
            if (str==";"){
                for (auto &i:cur){
                    for (auto &j:cur)
                    if (i!=j){
                        SynonymMap[i].push_back(j);
                    }
                }
                cur.clear();
            } else cur.push_back(str);
        }
    }
    fin.close();
}

vector<pair<string, string> > Synonym::findSynonym(vector<pair<string, string> > query){
    vector<pair<string, string> > ret;
    ret.clear();
    for (auto &q:query){
        ret.push_back(q);
        if (SynonymMap.find(q.second) != SynonymMap.end()){
            for (auto &i:SynonymMap[q.second]){
                ret.push_back(make_pair("OR", i));
            }
        }
    }
    return ret;
}
