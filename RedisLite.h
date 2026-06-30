#include <unordered_map>
#include <string>
#include <ctime>
#include <fstream>
#include <vector>
#include <queue>
#include <functional>
using namespace std;
#ifndef REDISLITE_H
#define REDISLITE_H
#include "Node.h"
class RedisLite{
public:
    priority_queue<pair<time_t,string>,vector<pair<time_t,string>>,greater<pair<time_t,string>>>pq;
    int capacity;
    vector<string>history;
    int cachehit=0;
    int cachemiss=0;
    int expiredkeys=0;
    int evictedkeys=0;
    unordered_map<string,Node*>mpp;
    Node *head;
    Node *tail;
    RedisLite(int cap);
    ~RedisLite();
    void removeexpiredkey();
    bool expiredtime(Node *node);
    void addnode(Node* node);
    void deletenode(Node *node);
    string get(string key_);
    void put(string _key,string _val);
    void deletefunc(string _key);
    bool isexist(string _key);
    void display();
    void setex(string _key,int ttl,string _val);
    void tosaveinfile(string memory);
    void toloadthefile(string memory);    
    int size();
    void clear();
    vector<string> getallkeys();
    void append(string key,string extra);
    void incr(string key);
    void decr(string key);
    void renamekey(string oldkey,string newkey);
    void addhistory(string data);
    void showstats();
};

#endif
