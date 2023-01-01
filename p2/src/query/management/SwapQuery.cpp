#include <iostream>
#include <thread>
#include <memory>
#include <cmath>

#include "SwapQuery.h"
#include "../../db/Database.h"
#include "../../parsedArgs.h"
extern ParsedArgs parsedArgs;
static int64_t suggested_threads = 0;

constexpr const char *SwapQuery::qname;

void SwapSection(const int secID, uint64_t op1, uint64_t op2, Table *tb, int *count, SwapQuery *q){
    const int secLen = (int)(tb->size()/(size_t)suggested_threads);
    
    // Indicate how many rows have been modified
    for(auto i = tb->begin() + secID*secLen; i != ((secID == suggested_threads-1)? tb->end() : tb->begin()+(secID+1)*secLen); i++){
        if (q->evalCondition(*i)){
            auto temp = (*i)[op1];
            (*i)[op1] = (*i)[op2];
            (*i)[op2] = temp;
            *count = *count + 1;
        }
    }
}

void SwapAll(uint64_t op1, uint64_t op2, Table *tb, int *count, SwapQuery *q){
    for(auto i = tb->begin(); i != tb->end(); i++){
        if (q->evalCondition(*i)){
            auto temp = (*i)[op1];
            (*i)[op1] = (*i)[op2];
            (*i)[op2] = temp;
            *count = *count + 1;
        }
    }
}

QueryResult::Ptr SwapQuery::execute(){
    // using namespace std;
    using std::make_unique;
    using std::cout;
    using std::endl;
    using std::thread;
    using std::string;
    Database &db = Database::getInstance();

    try {
        Table *tb = &db[targetTable];
        auto result = initCondition(*tb);

        suggested_threads = (int64_t)round(((double)(tb->size())/pow(2,15)));

        if (parsedArgs.threads > 0 && parsedArgs.threads < suggested_threads){
            suggested_threads = parsedArgs.threads;
        }
        if (suggested_threads > 64){
            suggested_threads = 64;
        }

        if (suggested_threads == 0){
            int *count = new int;
            *count = 0;

            uint32_t const op1 = (uint32_t)tb->getFieldIndex(this->operands[0]);
            uint32_t const op2 = (uint32_t)tb->getFieldIndex(this->operands[1]);

            SwapAll(op1,op2,tb,count,this);
            //cout << "No threads" << "\n";
            cout << "Affected " << *count << " rows.\n";

            delete count;

            return make_unique<SuccessMsgResult>(qname, targetTable);
        }else{
            int *count = new int[(u_int64_t)suggested_threads];
            for (int i = 0;i < suggested_threads;i++){
                count[i] = 0;
            }

            uint32_t const op1 = (uint32_t)tb->getFieldIndex(this->operands[0]);
            uint32_t const op2 = (uint32_t)tb->getFieldIndex(this->operands[1]);

            thread *t = new thread[(u_int64_t)suggested_threads];

            for (int i = 0;i < suggested_threads;i++){
                t[i] = thread(SwapSection,i,op1,op2,tb,count+i,this);
            }

            for (int i = 0;i < suggested_threads;i++){
                t[i].join();
            }
            
            int counter = 0;

            for (int i = 0;i < suggested_threads;i++){
                counter = counter + count[i];
            }

            cout << "Affected " << counter << " rows.\n";
            
            delete[] count;
            delete[] t;

            return make_unique<SuccessMsgResult>(qname, targetTable);
        }
    }
    catch (const TableNameNotFound &e) 
    {
        return make_unique<ErrorMsgResult>(qname, this->targetTable,
                                       "No such table.");
    }
    catch (const TableFieldNotFound&e)
    {
        //TODO output standard
        return make_unique<ErrorMsgResult>(qname, this->targetTable,e.what());
    }
    catch (const IllFormedQueryCondition &e) 
    {
        return make_unique<ErrorMsgResult>(qname, this->targetTable, e.what());
    }
    catch (const std::invalid_argument &e) 
    {
        // Cannot convert operand to string
        return make_unique<ErrorMsgResult>(qname, this->targetTable,
                                       "Unknown error '?'"_f % e.what());
    } 
    catch (const std::exception &e) 
    {
        return make_unique<ErrorMsgResult>(qname, this->targetTable,
                                       "Unkonwn error '?'."_f % e.what());
    }
}

std::string SwapQuery::toString(){
    return "QUERY = SWAP TABLE, TABLE = \"" + this->targetTable + "\"";
}
