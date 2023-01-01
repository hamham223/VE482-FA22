#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <cmath>

#include "DeleteQuery.h"
#include "../../db/Database.h"
#include "../../parsedArgs.h"

constexpr const char *DeleteQuery::qname;
extern ParsedArgs parsedArgs;
static int64_t suggested_threads = 0;

std::string DeleteQuery::toString(){
    return "QUERY = DELETE TABLE, TABLE = \"" + 
                    this->targetTable + "\"";
}

QueryResult::Ptr DeleteQuery::execute(){
    // using namespace std;
    using std::make_unique;
    using std::cout;
    Database &db = Database::getInstance();

    try {
        Table & tb = db[targetTable];

        auto result = initCondition(tb);

        int count = 0;

        for (auto i = tb.begin(); i != tb.end(); ) {
            if (this->evalCondition(*i)) 
            {tb.deleteDatum(i->key()); count++;}
                //{tb.erase(i); count++;}
            else i++;
        }

        cout << "Affected " << count << " rows.\n";

    }
    catch (const std::exception &e){
        return make_unique<ErrorMsgResult>(qname,e.what());
    }
    return make_unique<SuccessMsgResult>(qname, targetTable);
}

constexpr const char *DeleteMulTQuery::qname;


std::string DeleteMulTQuery::toString(){
    return "QUERY = DELETE TABLE, TABLE = \"" + 
                    this->targetTable + "\"";
}

QueryResult::Ptr DeleteMulTQuery::execute(){
    using std::make_unique;
    using std::cout;
    using std::thread;
    using std::forward_list;
    Database &db = Database::getInstance();

    try {
        Table * tb = &(db[targetTable]);
        auto result = initCondition(*tb);
        suggested_threads = (int64_t)round(((double)(tb->size())/pow(2,14)));
        if (parsedArgs.threads > 0 && parsedArgs.threads < suggested_threads)
        {
            suggested_threads = parsedArgs.threads;
        } 
        if (suggested_threads==0) {
            int count = 0;
            for (auto i = tb->begin(); i != tb->end(); ) {
                if (this->evalCondition(*i)) 
                    {tb->deleteDatum(i->key()); count++;}
                else i++;
            }
            cout << "Affected " << count << " rows.\n";
        }   else {

        int * count = new int[(u_int64_t)suggested_threads]{0};
        thread * t = new thread[(u_int64_t)suggested_threads];
        forward_list<int> * l = new forward_list<int>[(u_int64_t)suggested_threads]{};

        for (int i = 0; i < suggested_threads; i++) 
            t[i] = thread(FindSectionDel,i,count+i,l+i,tb,this);
        for (int i = 0; i < suggested_threads; i++)
            t[i].join();

        for (int64_t i = suggested_threads-1; i >= 0; i--)
            while (!l[i].empty()){
                //tb->erase(tb->begin()+l[i].front());
                tb->deleteDatum((tb->begin()+l[i].front())->key());
                l[i].erase_after(l[i].before_begin());
            }

        int total_count = 0;
        for (int i = 0; i < suggested_threads; i++) 
            total_count += count[i];
        delete[] count;
        delete[] t;
        delete[] l;
        cout << "Affected " << total_count << " rows.\n";
        }
    }
    catch (const std::exception &e){
        return make_unique<ErrorMsgResult>(qname,e.what());
    }
    return make_unique<SuccessMsgResult>(qname, targetTable);
}

void FindSectionDel(const int secID, int * count, std::forward_list<int> * l,
    Table * tb, DeleteMulTQuery * q){

    const int secLen = (int)(tb->size()/(size_t)suggested_threads);
    //int j = secID*secLen;
    for(int i = secID*secLen; i != int((secID == suggested_threads-1)? tb->size() : uint64_t((secID+1)*secLen)); i++) {
        if(q->evalCondition(*(tb->begin()+i))) {
            l->push_front(i);
            (*count) = (*count) + 1;
        }
        //j++;
    }
}

