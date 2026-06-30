#include <string>
#include <ctime>
using namespace std;
#ifndef NODE_H
#define NODE_H
class Node{
public:
    Node *next=nullptr;
    Node* prev=nullptr;
    string key,val;
    time_t expirytime;      //time_t=time as a number
    Node(string _key,string _val){
        key=_key;
        val=_val;
        expirytime=-1;
    }
};
#endif
