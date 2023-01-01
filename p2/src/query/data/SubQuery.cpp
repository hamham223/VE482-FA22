#include "SubQuery.h"

#include <iostream>
#include <memory>
#include <thread>
#include <string>
#include <vector>
#include <cmath>

#include "../../db/Database.h"
#include "../../parsedArgs.h"
extern ParsedArgs parsedArgs;
static int64_t suggested_threads = 0;

std::string SubQuery::toString() {
    return "QUERY = SUB " + this->targetTable + "\"";
}

QueryResult::Ptr SubQuery::execute() {

    Database &db = Database::getInstance();

    try {
        // using namespace std;
        using std::cout;
        using std::endl;
        using std::vector;
        using std::thread;
        using std::make_unique;
        Table * tb = &(db[targetTable]);
        auto result = initCondition(*tb);

        // must have at least 2 args (not complete)
        if (operands.size() < 2) throw std::exception(); 

        const uint32_t fSrc = (uint32_t) tb->getFieldIndex(operands[0]);
        const uint32_t fDest = (uint32_t) tb->getFieldIndex(*(operands.end() - 1));
        vector<int> col;
        col.reserve(operands.size()-2);
        for(auto i = operands.begin()+1; i != operands.end() - 1; i++ ){
            if (*i == "KEY") throw std::exception();
            else col.push_back((int)tb->getFieldIndex(*i));
        }

        suggested_threads = (int64_t)round(((double)(tb->size())/pow(2,13)));
        if (parsedArgs.threads > 0 && parsedArgs.threads < suggested_threads)
        {
            suggested_threads = parsedArgs.threads;
        }
        if (suggested_threads == 0) { // No thread version
            int count = 0; int diff = 0;
            for(auto i = tb->begin(); i != tb->end(); i++) {
                if(this->evalCondition(*i)) {
                    diff = (*i)[fSrc];
                    for (auto & j : (col)) {
                        diff -= (*i)[(u_int64_t)j];
                    }
                    (*i)[fDest] = diff;
                    count++;
                }
            }
            cout << "Affected " << count << " rows." << endl;
        }   else {

        int * count = new int[(u_int64_t)parsedArgs.threads]{0};
    
        thread * t = new thread[(u_int64_t)parsedArgs.threads];
        for (int i = 0; i < parsedArgs.threads; i++) 
            t[i] = thread(SectionSub,i,count+i,fSrc,fDest,tb,&col,this);
        for (int i = 0; i < parsedArgs.threads; i++)
            t[i].join();

        int total_count = 0;
        for (int i = 0; i < parsedArgs.threads; i++) 
            total_count += count[i];

        delete[] t;
        delete[] count;

        cout << "Affected " << total_count << " rows." << endl;
        }
    }
    catch (const std::exception &e) {
        return std::make_unique<ErrorMsgResult>(qname, e.what());
    }

    return std::make_unique<SuccessMsgResult>(qname, targetTable);
    //return std::make_unique<SucceededQueryResult>(qname);
}

void SectionSub(const int secID, int * count, const uint32_t fSrc, const uint32_t fDest, 
        Table * tb, std::vector<int> * col, SubQuery * q){

    const int secLen = (int)(tb->size()/(size_t)parsedArgs.threads);
    int diff;
    for(auto i = tb->begin() + secID*secLen; i != ((secID == parsedArgs.threads-1)? tb->end() : tb->begin()+(secID+1)*secLen); i++) {
        if(q->evalCondition(*i)) {
            diff = (*i)[fSrc];
            for (auto & j : (*col)) {
                diff -= (*i)[(u_int64_t)j];
            }
            (*i)[fDest] = diff;
            (*count) = (*count) + 1;
        }
    }
}
