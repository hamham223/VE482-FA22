#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <cmath>

#include "MinMaxQuery.h"
#include "../../db/Database.h"
#include "../../parsedArgs.h"
extern ParsedArgs parsedArgs;
static int64_t suggested_threads = 0;

constexpr const char *MaxQuery::qname;

void FindSectionMax(const int secID, int * maxres,Table *tb, std::vector<int> *col, MaxQuery * q)
{
    const int secLen = (int)(tb->size()/(size_t)parsedArgs.threads);
    //Init to MinVal
    for(size_t i = 0; i < col->size(); i++)
    {
        maxres[i] = Table::ValueTypeMin;
    }

    for(auto i = tb->begin() + secID*secLen; i != ((secID == parsedArgs.threads-1)? tb->end() : tb->begin()+(secID+1)*secLen); i++)
    {
        if(q->evalCondition(*i))
        {
            for(size_t j = 0; j < col->size(); j ++)
            {
                if((*i)[(uint64_t)(*col)[j]] > maxres[j])
                {
                    maxres[j] = (*i)[(uint64_t)(*col)[j]];
                }
            }
        }
    }
}


QueryResult::Ptr MaxQuery::execute()
{
    // using namespace std;
    using std::make_unique;
    using std::vector;
    using std::thread;
    using std::cout;
    using std::endl;
    Database &db = Database::getInstance();

    Table *tb = &db[targetTable];
    auto result = initCondition(*tb);
    //record the numbers of the promted colums
    vector<int> col = vector<int>();
    try
    {
        for(const auto &i: operands)
        {
            //NOT expecting "KEY"
            if (i == "KEY")
            {
                return make_unique<ErrorMsgResult>(qname, this->targetTable,"Cannot compare KEY!");
            }
            else
            {
                col.push_back((int)tb->getFieldIndex(i)); // Can not find index...
            }
        }

        suggested_threads = (int64_t)round(((double)(tb->size())/pow(2,13)));
        if (parsedArgs.threads > 0 && parsedArgs.threads < suggested_threads)
        {
            suggested_threads = parsedArgs.threads;
        }
        if (suggested_threads > 64) suggested_threads = 64;
        if (suggested_threads == 0) // No thread version
        {
            int * maxres = new int [col.size()];
            //init
            for (int i = 0; i < (int)col.size(); i++)
            {
                maxres[i] = Table::ValueTypeMin;
            }
            for(auto i = tb->begin(); i != tb->end(); i++)
            {
                if(this->evalCondition(*i))
                {
                    for(size_t j = 0; j < col.size(); j ++)
                    {
                        if((*i)[(uint64_t)(col)[j]] > maxres[j])
                        {
                            maxres[j] = (*i)[(uint64_t)(col)[j]];
                        }
                    }
                }
            }
            if (maxres[0] != Table::ValueTypeMin) //has found max
            {
                //Output:
                cout<< "ANSWER = ( ";
                for(size_t i = 0; i < operands.size(); i++)
                {
                    cout << maxres[i] << " ";
                }
                cout << ")" << endl;
                //Free mem
                delete[] maxres;
                return make_unique<SuccessMsgResult>(qname, targetTable);
            }
            else //Not found
            {
                //Free Mem
                delete[] maxres;
                return make_unique<NullQueryResult>();
            }
        }
        else
        {
            //2-D Array to store max val of all sections
            int ** maxres = new int* [(u_int64_t)parsedArgs.threads];
            for (int i = 0; i < parsedArgs.threads; i++)
            {
                maxres[i] = new int[operands.size()];
            }

            //Claim threads
            thread * t = new thread[(u_int64_t)parsedArgs.threads];
            for (int i = 0; i < parsedArgs.threads;i++)
            {
                t[i] = thread(FindSectionMax,i,maxres[i],tb,&col,this);
            }
            for (int i = 0; i < parsedArgs.threads;i++)
            {
                t[i].join();
            }
            //Find Max of Max:
            for (int i = 1; i < parsedArgs.threads; i++)
            {
                for(size_t j = 0; j < operands.size(); j++)
                {
                    if(maxres[i][j] > maxres[0][j]) maxres[0][j] = maxres[i][j];
                }
            }
            if(maxres[0][0] !=  Table::ValueTypeMin)
            {
                //Output:
                cout<< "ANSWER = ( ";
                for(size_t i = 0; i < operands.size(); i++)
                {
                    cout << maxres[0][i] << " ";
                }
                cout << ")" << endl;

                //Free Mem
                for (int i = 0; i < parsedArgs.threads; i++)
                {
                    delete[] maxres[i];
                }
                delete[] maxres;
                delete[] t;
                return make_unique<SuccessMsgResult>(qname, targetTable);
            }
            else // Can not find result
            {
                //Free Mem
                for (int i = 0; i < parsedArgs.threads; i++)
                {
                    delete[] maxres[i];
                }
                delete[] maxres;
                delete[] t;
                return make_unique<NullQueryResult>();
            }
        }
        
    }
    catch (const TableFieldNotFound&e)
    {
        //TODO output standard
        return make_unique<ErrorMsgResult>(qname, this->targetTable,e.what());
    }
}

std::string MaxQuery::toString() {
  return "QUERY = Max TABLE, TABLE = \"" + this->targetTable + "\"";
}

constexpr const char *MinQuery::qname;

void FindSectionMin(const int secID, int * minres,Table *tb, std::vector<int> *col, MinQuery * q)
{
    const int secLen = (int)(tb->size()/(size_t)suggested_threads);
    //Init to MaxVal
    for(size_t i = 0; i < col->size(); i++)
    {
        minres[i] = Table::ValueTypeMax;
    }

    for(auto i = tb->begin() + secID*secLen; i != ((secID == suggested_threads-1)? tb->end() : tb->begin()+(secID+1)*secLen); i++)
    {
        if(q->evalCondition(*i))
        {
            for(size_t j = 0; j < col->size(); j ++)
            {
                if((*i)[(uint64_t)(*col)[j]] < minres[j])
                {
                    minres[j] = (*i)[(uint64_t)(*col)[j]];
                }
            }
        }
    }
}

QueryResult::Ptr MinQuery::execute()
{
    // using namespace std;
    using std::make_unique;
    using std::vector;
    using std::thread;
    using std::cout;
    using std::endl;
    Database &db = Database::getInstance();

    Table *tb = &(db[targetTable]);
    auto result = initCondition(*tb);
    //record the numbers of the promted colums
    vector<int> col = vector<int>();

    try
    {
        for(const auto &i: operands)
        {
            //NOT expecting "KEY"
            if (i == "KEY")
            {
                return make_unique<ErrorMsgResult>(qname, this->targetTable,"Cannot compare KEY!");
            }
            else
            {
                col.push_back((int)tb->getFieldIndex(i)); // Can not find index...
            }
        }

        suggested_threads = (int64_t)round(((double)(tb->size())/pow(2,13)));
        if (parsedArgs.threads > 0 && parsedArgs.threads < suggested_threads)
        {
            suggested_threads = parsedArgs.threads;
        }
        if (suggested_threads > 64) suggested_threads = 64;
        if (suggested_threads == 0) // No thread version
        {
            int * minres = new int [col.size()];
            //init
            for (int i = 0; i < (int)col.size(); i++)
            {
                minres[i] = Table::ValueTypeMax;
            }
            for(auto i = tb->begin(); i != tb->end(); i++)
            {
                if(this->evalCondition(*i))
                {
                    for(size_t j = 0; j < col.size(); j ++)
                    {
                        if((*i)[(uint64_t)(col)[j]] < minres[j])
                        {
                            minres[j] = (*i)[(uint64_t)(col)[j]];
                        }
                    }
                }
            }
            if (minres[0] != Table::ValueTypeMax) //has found max
            {
                //Output:
                cout<< "ANSWER = ( ";
                for(size_t i = 0; i < operands.size(); i++)
                {
                    cout << minres[i] << " ";
                }
                cout << ")" << endl;
                //Free mem
                delete[] minres;
                return make_unique<SuccessMsgResult>(qname, targetTable);
            }
            else //Not found
            {
                //Free Mem
                delete[] minres;
                return make_unique<NullQueryResult>();
            }
        }
        else
        {
            //2-D Array to store min val of all sections
            int ** minres = new int* [(u_int64_t)suggested_threads];
            for (int i = 0; i < suggested_threads; i++)
            {
                minres[i] = new int[operands.size()];
            }

            //Claim threads
            thread * t = new thread[(u_int64_t)suggested_threads];
            for (int i = 0; i < suggested_threads;i++)
            {
                t[i] = thread(FindSectionMin,i,minres[i],tb,&col,this);
            }
            for (int i = 0; i < suggested_threads;i++)
            {
                t[i].join();
            }
            //Find Min of Min:
            for (int i = 1; i < suggested_threads; i++)
            {
                for(size_t j = 0; j < operands.size(); j++)
                {
                    if(minres[i][j] < minres[0][j]) minres[0][j] = minres[i][j];
                }
            }
            if (minres[0][0] != Table::ValueTypeMax) // Find min
            {
                //Output:
                cout<< "ANSWER = ( ";
                for(size_t i = 0; i < operands.size(); i++)
                {
                    cout << minres[0][i] << " ";
                }
                cout << ")" << endl;

                //Free Mem
                for (int i = 0; i < suggested_threads; i++)
                {
                    delete[] minres[i];
                }
                delete[] minres;
                delete[] t;
                return make_unique<SuccessMsgResult>(qname, targetTable);
            }
            else // Can not find result
            {
                //Free Mem
                for (int i = 0; i < suggested_threads; i++)
                {
                    delete[] minres[i];
                }
                delete[] minres;
                delete[] t;
                return make_unique<NullQueryResult>();
            }
        }
    }
    catch (const TableFieldNotFound&e)
    {
        //TODO output standard
        return make_unique<ErrorMsgResult>(qname, this->targetTable,e.what());
    }
}

std::string MinQuery::toString() {
  return "QUERY = Min TABLE, TABLE = \"" + this->targetTable + "\"";
}