
#include<unordered_map>
#include<queue>
#include<iostream>
#include <utility>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cstring>
#include <string>

using namespace std;


// key
typedef pair<int,int> pi;

// value
template <class V> 

// circular map 
struct circularpairmap{

    // priority queue FIFO
    //queue<pi> q;
    pi q*;
    unordered_map<pi,V,hash_pair> m;

    int MAX_CYCLE;
    int cur = 0;

    circularpairmap(int MAX_CYCLE){
        this->MAX_CYCLE=MAX_CYCLE;
        q = new pi[MAX_CYCLE];
        memset(q,MAX_CYCLE,nullptr);
        cur = 0;
    }
    
    // customized hash function
    struct hash_pair { 
        template <class T1, class T2> 
        size_t operator()(const pair<T1, T2>& p) const{ 
            auto hash1 = hash<T1>{}(p.first); 
            auto hash2 = hash<T2>{}(p.second); 
            return hash1 ^ hash2; 
        }
    }; 


    void insert(pi key,V value){
        if(q[cur]!=nullptr) m.erase(key);
        
        q[cur]=value;
        m[key]=value;

        cur = (cur + 1) % MAX_CYCLE;
    }

};

