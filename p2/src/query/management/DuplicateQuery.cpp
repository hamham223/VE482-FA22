#include <iostream>
#include <thread>
#include <sstream>
#include <memory>
#include <cmath>

#include "DuplicateQuery.h"

#include "../../db/Database.h"
#include "../../parsedArgs.h"
#include "../../db/Table.h"

extern ParsedArgs parsedArgs;

constexpr const char *DuplicateQuery::qname;

static int64_t suggested_threads = 0;

static Table *copy_table;

void DuplicateSection(const int secID, Table *tb , std::string *res, int *count, DuplicateQuery *q){
    const int secLen = (int)(tb->size()/(size_t)suggested_threads);

    for(auto i = tb->begin() + secID*secLen; i != ((secID == suggested_threads-1)? tb->end() : tb->begin()+(secID+1)*secLen); i++){
        if (q->evalCondition(*i)){
            if (tb->duplicate(i->key())){
                *res += i->key();
                *res += " ";
                *count = *count + 1;
            }
        }
    }
}

void DuplicateAll(Table *tb, std::string *res, int *count, DuplicateQuery *q){
    for(auto i = tb->begin(); i !=tb->end(); i++){
        if (q->evalCondition(*i)){
            if (tb->duplicate(i->key())){
                *res += i->key();
                *res += " ";
                *count = *count + 1;
            }
        }
    }
}

QueryResult::Ptr DuplicateQuery::execute(){
    // using namespace std;
    using std::make_unique;
    using std::string;
    using std::thread;
    Database &db = Database::getInstance();

    try {
        Table *tb = &db[targetTable];
        auto result = initCondition(*tb);
        copy_table = tb;

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
            string *res = new string;
            DuplicateAll(copy_table,res,count,this);

            std::istringstream record(*res);
            string newKey;
            while (record >> newKey){
                tb->insertDuplicate(newKey);
            }
            //std::cout << "No threads" << "\n";
            std::cout << "Affected " << *count << " rows.\n";

            delete count;
            delete res;
            return make_unique<SuccessMsgResult>(qname, targetTable);
        }else{
            int *count = new int[(u_int64_t)suggested_threads];
            for (int i = 0;i < suggested_threads;i++){
                count[i] = 0;
            }
            // Array of strings to store results
            string *res = new string[(u_int64_t)suggested_threads];

            thread *t = new thread[(u_int64_t)suggested_threads];

            for (int i = 0;i < suggested_threads;i++){
                t[i] = thread(DuplicateSection,i,copy_table,res+i,count+i,this);
            }

            for (int i = 0;i < suggested_threads;i++){
                t[i].join();
            }

            // insert the new rows
            for (int i = 0;i < suggested_threads;i++){
                std::istringstream record(res[i]);
                string newKey;
                while (record >> newKey){
                    tb->insertDuplicate(newKey);
                }
            }

            int counter = 0;
            for (int i = 0;i < suggested_threads;i++){
                counter = counter + count[i];
            }

            std::cout << "Affected " << counter << " rows.\n";

            delete[] count;
            delete[] t;
            delete[] res;

            return make_unique<SuccessMsgResult>(qname, targetTable);
        }
    }
    catch (const TableNameNotFound &e){
        return make_unique<ErrorMsgResult>(qname,this->targetTable,"No such table.");
    } catch (const IllFormedQueryCondition &e){
        return make_unique<ErrorMsgResult>(qname,this->targetTable,e.what());
    } catch (const std::invalid_argument &e){
        return make_unique<ErrorMsgResult>(qname,this->targetTable,"Unkown error '?'"_f % e.what());
    } catch (const std::exception &e){
        return make_unique<ErrorMsgResult>(qname,this->targetTable,"Unkown error '?'."_f % e.what());
    }
}

std::string DuplicateQuery::toString(){
    return "QUERY = DUPLICATE TABLE, TABLE = \"" + 
                    this->targetTable + "\"";
}
