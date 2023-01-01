#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <string>
#include <cmath>

#include "SumQuery.h"
#include "../../db/Database.h"
#include "../../parsedArgs.h"

extern ParsedArgs parsedArgs;

constexpr const char *SumQuery::qname;
static int64_t suggested_threads = 0;

QueryResult::Ptr SumQuery::execute()
{
    // using namespace std;
    using std::make_unique;
    using std::cout;
    using std::endl;
    using std::vector;
    using std::string;
    Database &db = Database::getInstance();

    Table &tb = db[targetTable];
    auto result = initCondition(tb);
    //record the numbers of the promted colums
    vector<int> col = vector<int>();
    for(const auto &i: operands)
    {
        if (i == "KEY") col.push_back(-1);
        else col.push_back((int)tb.getFieldIndex(i));
    }
    int * sumres = new int[operands.size()];
    //Initialize to 0
    for(size_t i = 0; i < operands.size(); i++)
    {
        sumres[i] = 0;
    }
    for(auto i = tb.begin(); i != tb.end(); i ++)
    {
        if(this->evalCondition(*i))
        {
            for(size_t j = 0; j < col.size(); j ++)
            {
                sumres[j] += (*i)[(u_int64_t)col[(u_int64_t)j]];
            }
        }
    }
    //Output:
    cout<< "ANSWER = ( ";
    for(size_t i = 0; i < operands.size(); i++)
    {
        cout << sumres[i] << " ";
    }
    cout << ")" << endl;
    delete[] sumres;

    return make_unique<SuccessMsgResult>(qname, targetTable);
}

std::string SumQuery::toString() {
  return "QUERY = Sum TABLE, TABLE = \"" + this->targetTable + "\"";
}

constexpr const char *SumMulTQuery::qname;

std::string SumMulTQuery::toString() {
  return "QUERY = Sum TABLE, TABLE = \"" + this->targetTable + "\"";
}

QueryResult::Ptr SumMulTQuery::execute() {
    // using namespace std;
    using std::make_unique;
    using std::cout;
    using std::endl;
    using std::vector;
    using std::thread;
    using std::string;

    try {
        Database &db = Database::getInstance();
        Table * tb = &(db[targetTable]);
        auto result = initCondition(*tb);
        //record the numbers of the promted colums
        vector<int> col = vector<int>();

        for(const auto &i: operands)
        {
            if (i == "KEY") throw std::exception();
            else col.push_back((int)tb->getFieldIndex(i));
        }

        suggested_threads = (int64_t)round(((double)(tb->size())/pow(2,15)));
        if (parsedArgs.threads > 0 && parsedArgs.threads < suggested_threads){
            suggested_threads = parsedArgs.threads;
        }
        if (suggested_threads > 64){
            suggested_threads = 64;
        }

        if (suggested_threads == 0){
            int * sumres = new int[operands.size()]{0};
            FindAllSum(sumres,tb,&col,this);

            //Output:
            //cout << "No threads" << "\n";
            cout<< "ANSWER = ( ";
            for(size_t i = 0; i < operands.size(); i++)
            {
                cout << sumres[i] << " ";
            }
            cout << ")" << endl;

            delete[] sumres;
        }else{
            int ** sumres = new int * [(u_int64_t)suggested_threads];

            for (int i = 0 ; i < suggested_threads; i++) {
                sumres[i] = new int[operands.size()]{0};
            }

            thread * t = new thread[(u_int64_t)suggested_threads];
            for (int i = 0; i < suggested_threads; i++) {
                t[i] = thread(FindSectionSum,i,sumres[i],tb,&col,this);
            }
            for (int i = 0; i < suggested_threads; i++)
                t[i].join();

            for (int i = 1; i < suggested_threads; i++){
                for (size_t j = 0; j < operands.size(); j++)
                    sumres[0][j] += sumres[i][j];
            }

            //Output:
            cout<< "ANSWER = ( ";
            for(size_t i = 0; i < operands.size(); i++)
            {
                cout << sumres[0][i] << " ";
            }
            cout << ")" << endl;

            for (int i = 0 ; i < suggested_threads; i++) {
                delete[] sumres[i];
            }
            delete[] sumres;
            delete[] t;
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
    return make_unique<SuccessMsgResult>(qname, targetTable);
}

void FindSectionSum(const int secID, int * sumres, Table * tb, std::vector<int> * col, SumMulTQuery * q){
    const int secLen = (int)(tb->size()/(size_t)suggested_threads);

    for(auto i = tb->begin() + secID*secLen; i != ((secID == suggested_threads-1)? tb->end() : tb->begin()+(secID+1)*secLen); i++) {
        if(q->evalCondition(*i)) {
            for(size_t j = 0; j < col->size(); j ++) {
                sumres[j] += (*i)[(u_int64_t)(*col)[j]];
            }
        }
    }
}

void FindAllSum(int * sumres, Table * tb, std::vector<int> * col, SumMulTQuery * q){
    for(auto i = tb->begin(); i != tb->end(); i++) {
        if(q->evalCondition(*i)) {
            for(size_t j = 0; j < col->size(); j ++) {
                sumres[j] += (*i)[(u_int64_t)(*col)[j]];
            }
        }
    }
}
