//
//  InvertedIndex.cpp
//  SimpleInformationRetrievalTools
//
//  Created by SmilENow on 6/9/15.
//  Copyright (c) 2015 Jiaquan Yin. All rights reserved.
//

#include "InvertedIndex.h"

// Utility //
string InvertedIndex::GetFileName(int x){
    string ret = "";
    stringstream sin;
    sin << x;
    sin >> ret;
    ret = ret + ".html";
    return ret;
}

string InvertedIndex::GetFileDir(int x){
    string ret;
    ret = "./Reuters/" + GetFileName(x);
    return ret;
}

void InvertedIndex::AddToMap(string filename, int index, string term){
    if (PositingListMap.find(term) == PositingListMap.end()){
        PositingListSubNode plsn(filename);
        plsn.pos.push_back(index);
        PositingListMap[term].push_back(plsn);
    } else {
        if (PositingListMap[term].back().DocID == filename){
            PositingListMap[term].back().pos.push_back(index);
        } else {
            PositingListSubNode plsn(filename);
            plsn.pos.push_back(index);
            PositingListMap[term].push_back(plsn);
        }
    }
}

void InvertedIndex::SortByTerm(){
    for (auto &i:PositingListMap){
        PositingListNode cur(i.first);
        cur.value = i.second;
        PositingList.push_back(cur);
    }
    std::sort(PositingList.begin(),PositingList.end(),cmp);
}

void InvertedIndex::BuildInvertedIndexList(){
    for (auto &i:PositingList){
        InvertedIndexListNode cur(i.term);
        for (auto &j:i.value){
            InvertedIndexListSubNode tmp(j.DocID,(int)j.pos.size());
            cur.value.push_back(tmp);
            InvertedIndexListMap[i.term].push_back(tmp);
        }
        InvertedIndexList.push_back(cur);
    }
}

string InvertedIndex::trim(string str){
    string ret = "";
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    int st,ed;
    st = 0;
    ed = (int)str.length()-1;
    if (st>ed) return "";
    while (st<=ed && (str[st]=='\'' || str[st]=='.' || str[st]=='-' || str[st]=='\"')) st++;
    if (st>ed) return "";
    while (st<=ed && (str[ed]=='\'' || str[ed]=='.' || str[ed]=='-' || str[ed]=='\"')) ed--;
    if (st>ed) return "";
    for (int i=st;i<=ed;i++){
        if (str[i] == '\"' || str[i] == ',' || str[i] == '>' || str[i] == '<' || str[i] == '(' || str[i] == ')') continue;
        if (i+3<=ed && str[i] == '&' && str[i+1] == 'l' && str[i+2] == 't' && str[i+3] == ';')
            i=i+3;
        else ret += str[i];
    }
    return ret;
}

bool InvertedIndex::CheckStopWord(string str){
    if (str=="" || str==" ") return true;
    if (StopWordListMap.find(str) == StopWordListMap.end()) return false;
    return true;
}

// Main Public Function //

InvertedIndex::InvertedIndex(){
    PositingList.clear();
    PositingListMap.clear();
    InvertedIndexList.clear();
    InvertedIndexListMap.clear();
    AllDoc.clear();
    StopWordListMap.clear();
}

InvertedIndex::~InvertedIndex(){
}

void InvertedIndex::BuildFromFiles(){
    ifstream fin;
    for (int fileindex = 1; fileindex < 21580; fileindex++){
        string str;
        string filename = GetFileName(fileindex);
        AllDoc.push_back(filename);
        string filedir = GetFileDir(fileindex);
        int cnt = 0;
        fin.open(filedir.c_str(),ios::in);
        if (fin.is_open()){
            while (fin.peek() != EOF){
                fin >> str;
                if (fin.eof()) break;
//                cout << str << " ";
                str = trim(str);
                if (CheckStopWord(str)) continue;
                cnt++;
                AddToMap(filename,cnt,str);
            }
//            cout << endl;
        } else {
//            cout << "Can\'t open the file" << endl;
        }
        fin.close();
    }
    SortByTerm();
    BuildInvertedIndexList();
}

void InvertedIndex::LoadStopWordList(){
    ifstream fin;
    string str;
    string filedir = "./StopWordLongList.txt";
    fin.open(filedir.c_str(),ios::in);
    if (fin.is_open()){
        while (fin.peek() != EOF){
            fin >> str;
            if (fin.eof()) break;
            StopWordListMap[str] = 1;
        }
    }
    fin.close();
}

void InvertedIndex::PrintPositingList(){
    ofstream fout;
    fout.open("PositingList.txt",ios::out);
    if (fout.is_open()){
        for (auto &i:PositingList){
            fout << "Term : " << i.term << endl;
            for (auto &j:i.value){
                fout << "\tDocID: " << j.DocID << endl;
                fout << "\t\t";
                for (auto &k:j.pos){
                    fout << k << ",";
                }
                fout << endl;
            }
            fout << "---------------------" << endl;
        }
    } else {
        cout << "Can\'t create the file" << endl;
    }
    fout.close();
}

void InvertedIndex::PrintInvertedIndexList(){
    ofstream fout;
    fout.open("InvertedIndexList.txt",ios::out);
    if (fout.is_open()){
        for (auto &i:InvertedIndexList){
            fout << "Term : " << i.term << endl;
            for (auto &j:i.value){
                fout << "\tDocID: " << j.DocID << ":";
                fout << j.sum << endl;
            }
        }
    } else {
        cout << "Can\'t create the file" << endl;
    }
    fout.close();
}

/*
void InvertedIndex::SaveToFiles(){
    ofstream fout;
    // PositingList
    fout.open("PositingList.hex",ios::binary);
    if (fout.is_open()){
        fout << (int)PositingList.size() << endl;
        for (auto &i:PositingList){
            fout << i.term << " ";
            fout << (int)i.value.size() << " ";
            for (auto &j:i.value){
                fout << j.DocID << " ";
                fout << (int)j.pos.size() << " ";
                for (auto &k:j.pos) fout << k << " ";
            }
            fout << endl;
        }
    }
    fout.close();
    // TODO
}

void InvertedIndex::LoadFromFiles(){
    ifstream fin;
    // PositingList
    PositingList.clear();
    fin.open("PositingList.hex",ios::binary);
    if (fin.is_open()){
        int PLsize;
        fin >> PLsize;
        PositingList.resize(10);
        for (int i=0;i<PLsize;i++){
            fin >> PositingList[i].term;
            int x;
            fin >> x;
            PositingList[i].value.resize(x);
            for (int j=0;j<x;j++){
                fin >> PositingList[i].value[j].DocID;
                int y;
                fin >> y;
                PositingList[i].value[j].pos.resize(y);
                for (int k=0;k<y;k++) fin>>PositingList[i].value[j].pos[k];
            }
        }
    }
    fin.close();
    // TODO
}
*/