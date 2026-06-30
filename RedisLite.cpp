#include "RedisLite.h"
#include <iostream>
using namespace std;
#include <ctime>
#include <fstream>
#include "Node.h"
#include <unordered_map>
#include <vector>
#include <queue>
#include <functional>
RedisLite::RedisLite(int cap){
        capacity=cap;
        head=new Node("","");
        tail=new Node("","");
        head->next=tail;
        tail->prev=head;
}
RedisLite::~RedisLite(){
    clear();
    delete head;
    delete tail;
}
void RedisLite::removeexpiredkey(){
    while(!pq.empty()){
        auto top=pq.top();
        if(top.first>time(0))
        break;
        pq.pop();
        if(mpp.find(top.second)==mpp.end())
        continue;
        Node *node=mpp[top.second];
        if(node->expirytime!=top.first)
        continue;
        mpp.erase(top.second);
        deletenode(node);
        delete (node);
        expiredkeys++;
    }
}
bool RedisLite::expiredtime(Node *node){
    if(node->expirytime!=-1 && time(0)>node->expirytime) //time now as passed expired time
    return true;
    return false;
}
void RedisLite::addnode(Node* node){
    Node *headnext=head->next;
    head->next=node;
    node->prev=head;
    node->next=headnext;
    headnext->prev=node;
}

void RedisLite::deletenode(Node *node){
    Node* prevnode=node->prev;
    Node* nextnode=node->next;
    prevnode->next=nextnode;
    nextnode->prev=prevnode;
}

string RedisLite::get(string key_){
    removeexpiredkey();
    if(mpp.find(key_)!=mpp.end()){
        Node *node=mpp[key_];
        if(expiredtime(node)){ // current time as passed expire time
            mpp.erase(key_);
            deletenode(node);
            delete(node);
            return "NOT FOUND";
        }
        string val=node->val;
        deletenode(node);
        addnode(node);
        return val;
    }
    else
    return "NOT FOUND";
}

void RedisLite:: put(string _key,string _val){
    removeexpiredkey();
    time_t oldexpiredtime=-1;
    if(mpp.find(_key)!=mpp.end()){
        Node *node=mpp[_key];
        mpp.erase(_key);
        oldexpiredtime=node->expirytime;
        deletenode(node);
        delete(node);
    }
    if(mpp.size()>=capacity){
        Node *del=tail->prev;
        evictedkeys++;
        mpp.erase(del->key);
        deletenode(del);
        delete(del);
    }
    Node *newnode=new Node(_key,_val);
    addnode(newnode);
    newnode->expirytime=oldexpiredtime;
    mpp[_key]=newnode;
}

void RedisLite:: deletefunc(string _key){
    if(mpp.find(_key)!=mpp.end()){
        Node *node=mpp[_key];
        mpp.erase(_key);
        deletenode(node);
        delete(node);
    }
}

bool RedisLite::isexist(string _key){
    if(mpp.find(_key)==mpp.end())
    return false;
    if(mpp.find(_key)!=mpp.end()){
        Node *node=mpp[_key];
        if(expiredtime(node)){
            mpp.erase(_key);
            deletenode(node);
            delete(node);
            return false;
        }
    }
    return true;
}

void RedisLite:: display(){
    removeexpiredkey();
    Node *node=head->next;
    while(node!=tail){
        if(expiredtime(node)){
            Node* temp=node;
            node=node->next;
            mpp.erase(temp->key);
            deletenode(temp);
            delete (temp);
            continue;
        }
        if(node->expirytime==-1)
        cout<<"["<<node->key<<"->"<<node->val<<"->"<<"NO EXPIRY TIME"<<"]";
        else
        cout<<"["<<node->key<<"->"<<node->val<<"->"<<node->expirytime<<"]";
        node=node->next;
    }
    cout<<endl;
}

void RedisLite::setex(string _key,int ttl,string _val){
    put(_key,_val);
    Node *node=mpp[_key];
    node->expirytime=time(0)+ttl; //time(0)=time now
    pq.push({node->expirytime,_key}); 
}

void RedisLite::tosaveinfile(string memory){
    ofstream fout(memory);
    Node *temp=head->next;
    while(temp!=tail){
        fout<<temp->key<<":"<<temp->expirytime<<":"<<temp->val<<endl;
        temp=temp->next;
    }
    fout.close();
}

void RedisLite::toloadthefile(string memory){
    ifstream fin(memory);
    if(!fin.is_open())
    return;
    string key,value;
    time_t expiry;
    while(fin>>key>>value>>expiry){
        if(expiry!=-1 && time(0)>expiry){
            continue;
        }
        put(key,value);
        mpp[key]->expirytime=expiry;
        if(expiry!=-1)
        pq.push({expiry,key});
    }
    fin.close();
}

int RedisLite::size(){
    removeexpiredkey();
    return mpp.size();
}

void RedisLite::clear(){
    Node *node=head->next;
    while(node!=tail){
        Node *temp=node;
        node=node->next;
        deletenode(temp);
        delete(temp);
    }
    head->next=tail;
    tail->prev=head;
    mpp.clear();
    while(!pq.empty())
    pq.pop();

}
vector<string>RedisLite:: getallkeys(){
    removeexpiredkey();
    vector<string>keys;
    Node *node=head->next;
    while(node!=tail){
        if(!expiredtime(node))
            keys.push_back(node->key);
        node=node->next;
    }
    return keys;
}
void RedisLite::append(string key,string extra){
    if(mpp.find(key)==mpp.end()){
        put(key,extra);
        return;
    }
    Node *node=mpp[key];
    if(expiredtime(node)){
        mpp.erase(key);
        deletenode(node);
        delete(node);
        put(key,extra);
        return;
    }
    string newvalue=node->val+extra;
    put(key,newvalue);
    cout<<"DONE"<<endl;
}
void RedisLite::incr(string key){
    int num=0;
    if(isexist(key))
    num=stoi(get(key));
    num++;
    put(key,to_string(num));
    cout<<num<<endl;
}
void RedisLite::decr(string key){
    int num=0;
    if(isexist(key))
    num=stoi(get(key));
    num--;
    put(key,to_string(num));
    cout<<num<<endl;
}
void RedisLite::renamekey(string oldkey,string newkey){
    if(!isexist(oldkey)){
        cout<<"Key not found"<<endl;
        return;
    }
    Node *node=mpp[oldkey];
    time_t oldtime=node->expirytime;
    string value=get(oldkey);
    deletefunc(oldkey);
    put(newkey,value);
    mpp[newkey]->expirytime=oldtime;
    if(oldtime!=-1){
        pq.push({oldtime,newkey});
    }
    cout<<"Renamed Successfully"<<endl;
}
void RedisLite::addhistory(string data){
    history.push_back(data);
    if(history.size()>10)
    history.erase(history.begin());
}
void RedisLite::showstats(){
    cout<<"Cache Hits : "<<cachehit<<endl;
    cout<<"Cache Miss : "<<cachemiss<<endl;
    cout<<"Expired Keys : "<<expiredkeys<<endl;
    cout<<"Evicted Keys : "<<evictedkeys<<endl;
    }
