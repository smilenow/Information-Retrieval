//
//  InvertedIndex.h
//  SimpleInformationRetrievalTools
//
//  Created by SmilENow on 6/9/15.
//  Copyright (c) 2015 Jiaquan Yin. All rights reserved.
//

#ifndef __SimpleInformationRetrievalTools__InvertedIndex__
#define __SimpleInformationRetrievalTools__InvertedIndex__

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <unordered_map>

using namespace std;

class InvertedIndex{
    
public:
    /* Use For Positing List */
    struct PositingListSubNode{
        string DocID;
        vector<int> pos;
        PositingListSubNode(string D):DocID(D){};
    };
    
    struct PositingListNode{
        string term;
        vector<PositingListSubNode> value;
        PositingListNode(string t):term(t){};
    };
    
    static bool cmp(const PositingListNode& x, const PositingListNode& y){
        return x.term < y.term;
    };
    
    /* Use For Inverted Index List*/
    struct InvertedIndexListSubNode{
        string DocID;
        int sum;
        InvertedIndexListSubNode(string d,int s):DocID(d),sum(s){};
    };
    
    struct InvertedIndexListNode{
        string term;
        vector<InvertedIndexListSubNode> value;
        InvertedIndexListNode(string t):term(t){};
    };
    
// Method
public:
    InvertedIndex();
    virtual ~InvertedIndex();
    void BuildFromFiles();
    void LoadStopWordList();
    void PrintPositingList();
    void PrintInvertedIndexList();
    void SaveToFiles();
    void LoadFromFiles();
    string trim(string);
    bool CheckStopWord(string);
    
private:
    string GetFileName(int);
    string GetFileDir(int);
    void AddToMap(string,int,string);
    void SortByTerm();
    void BuildInvertedIndexList();
    
// Variable
public:
    vector<PositingListNode> PositingList;
    unordered_map<string, vector<PositingListSubNode>> PositingListMap;
    vector<InvertedIndexListNode> InvertedIndexList;
    unordered_map<string, vector<InvertedIndexListSubNode>> InvertedIndexListMap;
    vector<string> AllDoc;
    unordered_map<string, int> StopWordListMap;
};

#endif /* defined(__SimpleInformationRetrievalTools__InvertedIndex__) */
