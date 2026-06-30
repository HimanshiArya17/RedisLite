#include <bits/stdc++.h>
using namespace std;
#include "Node.h"
#include "RedisLite.h"
int main(){
    RedisLite u(3);
    u.toloadthefile("memory.txt");
    cout<<"========== RedisLite =========="<<endl;
    cout<<"1. SET"<<endl;
    cout<<"2. GET"<<endl;
    cout<<"3. DELETE"<<endl;
    cout<<"4. EXIST"<<endl;
    cout<<"5. DISPLAY"<<endl;
    cout<<"6. SETEX"<<endl;
    cout<<"7. APPEND"<<endl;
    cout<<"8. INCR"<<endl;
    cout<<"9. DECR"<<endl;
    cout<<"10. RENAME"<<endl;
    cout<<"11. KEYS"<<endl;
    cout<<"12. SIZE"<<endl;
    cout<<"13. SAVE"<<endl;
    cout<<"14. LOAD"<<endl;
    cout<<"15. HISTORY"<<endl;
    cout<<"16. STATS"<<endl;
    cout<<"17. CLEAR"<<endl;
    cout<<"18. EXIT"<<endl;
    cout<<endl;
    cout<<"Enter command: "<<endl;
    string command;
    while(cin>>command){
        if(command=="SET" || command=="set"){
            string key,value;
            cin>>key>>value;
            u.put(key,value);
            u.addhistory("SET " + key + "->" + value);
        }
        else if(command=="GET" || command=="get"){
            string key;
            cin>>key;
            u.addhistory("GET " +key);
            cout<<u.get(key)<<endl;
        }
        else if(command=="DELETE" || command=="delete"){
            string key;
            cin>>key;
            u.deletefunc(key);
            u.addhistory("DELETE "+key);
        }
        else if(command=="EXIST" || command=="exist"){
            string key;
            cin>>key;
            u.addhistory("IS "+key + "EXIST");
            if(u.isexist(key)) 
            cout<<"YES"<<endl;
            else
            cout<<"NO"<<endl;
        }
        else if(command=="DISPLAY" || command=="display"){
            u.addhistory("DISPLAY");
            u.display();
        }
        else if(command=="SETEX" || command=="setex"){
            string key,value;
            int ttl;
            cin>>key>>ttl>>value;
            u.addhistory("SETEX"+key+"->"+value);
            u.setex(key,ttl,value);
        }
        else if(command=="SAVE" || command=="save"){
            u.addhistory("SAVE");
            u.tosaveinfile("memory.txt");
        }
        else if(command=="LOAD" || command=="load"){
            u.addhistory("LOAD");
            u.toloadthefile("memory.txt");
        }
        else if(command=="SIZE" || command=="size"){
            u.addhistory("SIZE");
            cout<<u.size()<<endl;
        }
        else if(command=="CLEAR" || command=="clear"){
            u.addhistory("CLEAR");
            u.clear();
        }
        else if(command=="APPEND" || command=="append"){
            string key,extra;
            cin>>key>>extra;
            u.addhistory("APPEND "+key);
            u.append(key,extra);
        }
        else if(command=="INCR" || command=="incr"){
            string key;
            cin>>key;
            u.addhistory("INCR "+key +" by 1");
            u.incr(key);
        }
        else if(command=="DECR" || command=="decr"){
            string key;
            cin>>key;
            u.addhistory("DECR "+key +" by -1");
            u.decr(key);
        }
        else if(command=="RENAME" || command=="rename"){
            string oldkey,newkey;
            cin>>oldkey>>newkey;
            u.addhistory("RENAME "+oldkey + " to "+ newkey);
            u.renamekey(oldkey,newkey);
        }
        else if(command=="KEYS" || command=="keys"){
            vector<string> keys = u.getallkeys();
            u.addhistory("KEYS ");
            if(keys.empty()){
                cout<<"(empty)"<<endl;
            } else {
                for(int i=0;i<keys.size();i++){
                    cout<<i+1<<"->"<<keys[i]<<endl;
                }
            }
        }
        else if(command=="HISTORY" || command=="history"){
            if(u.history.empty())
                cout<<"(no history)"<<endl;
            else{
                for(int i=0;i<(int)u.history.size();i++)
                    cout<<i+1<<")"<<u.history[i]<<endl;
            }
        }
        else if(command=="STATS" || command=="stats"){
            u.addhistory("STATS");
            u.showstats();
        }
        else if(command=="EXIT" || command=="exit"){
            break;
        }
    }

}

// g++ Dsa_project.cpp RedisLite.cpp -o redis
//  .\redis
