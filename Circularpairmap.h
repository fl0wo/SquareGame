//
// Created by flo on 10/28/2021.
//

#ifndef NOMPROJET_CIRCULARPAIRMAP_H
#define NOMPROJET_CIRCULARPAIRMAP_H


#include <unordered_map>
#include <queue>
#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cstring>
#include <string>
#include <map>

using namespace std;

#define MAXSUPPORTEDCYCLE 500

// key
typedef pair<int,int> pi;

// value
template <class V>

// circular map
struct circularpairmap{

    // priority queue FIFO
    queue<pi> q;

    struct hash_pair {
        template <class T1, class T2>
        size_t operator()(const pair<T1, T2>& p) const{
            auto hash1 = hash<T1>{}(p.first);
            auto hash2 = hash<T2>{}(p.second);
            return hash1 ^ hash2;
        }
    };

    unordered_map<pi,V,hash_pair> m; // O(1)
    //map<pi,V> m; // O(log n)

    int MAX_CYCLE;
    int cur = 0;
    unsigned int efficency = 0;

    circularpairmap(){
        this->MAX_CYCLE = MAXSUPPORTEDCYCLE/2;
        efficency = 0;
    }

    circularpairmap(int MAX_CYCLE){
        this->MAX_CYCLE = max(0,min(MAX_CYCLE,MAXSUPPORTEDCYCLE));
        cur = 0;
        efficency = 0;
    }

    void insert(pi key,V value){
        if(efficency>MAX_CYCLE){
            pi key = q.front();
            q.pop();
            m.erase(m.find(key));
        }else{
            efficency++;
        }

        q.push(key);
        m[key]=value;

        cur = (cur + 1) % MAX_CYCLE;
    }

};



#endif //NOMPROJET_CIRCULARPAIRMAP_H
