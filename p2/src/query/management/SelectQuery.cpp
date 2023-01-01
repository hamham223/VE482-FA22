#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <thread>

#include <queue>
#include <mutex>
#include <algorithm>
#include <cmath>

#include "SelectQuery.h"

#include "../../db/Database.h"
#include "../../parsedArgs.h"
extern ParsedArgs parsedArgs;

constexpr const char *SelectQuery::qname;
static int64_t suggested_threads = 0;

struct key_prior{
    bool operator()(std::string &a, std::string &b){
        if (a >= b){
            return true;
        }else{
            return false;
        }
    }
    
};
static std::priority_queue<std::string, std::vector<std::string>,key_prior> key_queue;
static std::mutex key_mutex;

bool cmpStr(std::string a, std::string b)
{
    return a > b;
}

void SelectSection(const int secID, Table *tb,std::vector<int> *col, SelectQuery * q)
{
    const int secLen = (int)(tb->size()/(size_t)suggested_threads);
    std::string res = "";
    for(auto i = tb->begin() + secID*secLen; i != ((secID == suggested_threads-1)? tb->end() : tb->begin()+(secID+1)*secLen); i++)
    {
        if(q->evalCondition(*i))
        {
            res = "( ";
            for(const auto &j: (*col))
            {
                if( j == -1)
                {
                    res += i->key();
                    res += " ";
                }
                else 
                {
                    res += to_string((*i)[(u_int64_t)j]);
                    res += " ";
                }
            }
            res += ")";
            std::string const temp = res;
            key_mutex.lock();
            key_queue.push(temp);
            key_mutex.unlock();
        }
    }
}

QueryResult::Ptr SelectQuery::execute() {
    
    Database &db = Database::getInstance();
    // using namespace std;
    using std::make_unique;
    using std::cout;
    using std::endl;
    using std::vector;
    using std::thread;
    using std::string;
    using std::literals::string_literals::operator ""s;
    try
    {
        Table *tb = &db[targetTable];

        auto result = initCondition(*tb);
        //record the numbers of the promted colums
        vector<int> col = vector<int>();
        if(operands[0] != "KEY")
        {
            return make_unique<ErrorMsgResult>(qname, this->targetTable,
                                       "Invalid number of operands (? operands) or without leading KEY."_f % operands.size());
        }
        for(const auto &i: operands)
        {
            if (i == "KEY")
            {
                col.push_back(-1);
            } 
            else col.push_back((int)tb->getFieldIndex(i));
        }

        suggested_threads = (int64_t)round(((double)(tb->size())/pow(2,15)));
        if (parsedArgs.threads > 0 && parsedArgs.threads < suggested_threads)
        {
            suggested_threads = parsedArgs.threads;
        }
        if (suggested_threads > 64) suggested_threads = 64;
        //std::cerr<<suggested_threads<<std::endl;
        if (suggested_threads == 0)
        {
            vector<string> result = vector<string>();
            for (auto i = tb->begin(); i != tb->end(); i++)
            {
                if(evalCondition(*i))
                {
                    string res = "";
                    res += "( ";
                    for(const auto &j: (col))
                    {
                        if( j == -1)
                        {
                            res += i->key();
                            res += " ";
                        }
                        else 
                        {
                            res += to_string((*i)[(u_int64_t)j]);
                            res += " ";
                        }
                    }
                    res += ")";
                    result.push_back(res);
                }
            }
            if(result.size() == 0)
            {
                return make_unique<NullQueryResult>();
            }
            else
            {
                std::sort(result.begin(),result.end());
                for (auto &i:result)
                {
                    cout << i <<endl;
                }
            }
            return make_unique<SuccessMsgResult>(qname, targetTable);
        }
        else
        {
            //Claim threads
            thread * t = new thread[(u_int64_t)suggested_threads];

            for (int i = 0; i < suggested_threads;i++)
            {
                t[i] = thread(SelectSection,i,tb,&col,this);
            }
            for (int i = 0; i < suggested_threads;i++)
            {
                t[i].join();
            }
            // output
            if (key_queue.size() == 0){
                delete[] t;
                return make_unique<NullQueryResult>();
            }
            while (key_queue.size() != 0){
                auto top = key_queue.top();
                cout << top << "\n";
                key_queue.pop();
            }

            delete[] t;
            return make_unique<SuccessMsgResult>(qname, targetTable);
        }
    }
    catch (const TableNameNotFound &e) 
    {
        return make_unique<ErrorMsgResult>(qname, this->targetTable,
                                       "No such table."s);
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

std::string SelectQuery::toString() {
  return "QUERY = Select TABLE, TABLE = \"" + this->targetTable + "\"";
}


void SelectSection2(const int secID,std::vector<std::string> * res, Table *tb,std::vector<int> *col, SelectQuery2 * q)
{
    const int secLen = (int)(tb->size()/(size_t)suggested_threads);
    std::string r = "";
    for(auto i = tb->begin() + secID*secLen; i != ((secID == suggested_threads-1)? tb->end() : tb->begin()+(secID+1)*secLen); i++)
    {
        if(q->evalCondition(*i))
        {
            r = "( ";
            for(const auto &j: (*col))
            {
                if( j == -1)
                {
                    r += i->key();
                    r += " ";
                }
                else 
                {
                    r += to_string((*i)[(u_int64_t)j]);
                    r += " ";
                }
            }
            r += ")";
            res->push_back(r);
        }
    }
    std::sort(res->begin(),res->end(),cmpStr);
}

std::string SelectQuery2::toString() {
  return "QUERY = Select TABLE, TABLE = \"" + this->targetTable + "\"";
}


QueryResult::Ptr SelectQuery2::execute() {
    
    Database &db = Database::getInstance();
    // using namespace std;
    using std::make_unique;
    using std::cout;
    using std::endl;
    using std::vector;
    using std::thread;
    using std::string;
    using std::literals::string_literals::operator ""s;
    try
    {
        Table *tb = &db[targetTable];

        auto result = initCondition(*tb);
        //record the numbers of the promted colums
        bool hasKEY = false;
        vector<int> col = vector<int>();
        for(const auto &i: operands)
        {
            if (i == "KEY")
            {
                col.push_back(-1);
                hasKEY = true;
            } 
            else col.push_back((int)tb->getFieldIndex(i));
        }
        if(!hasKEY)
        {
            return make_unique<ErrorMsgResult>(qname, this->targetTable,
                                       "Invalid number of operands (? operands) or without leading KEY."_f % col.size());
        }

        suggested_threads = (tb->size()) >> 15;
        if (!(parsedArgs.threads > 0 && parsedArgs.threads < suggested_threads))
        {
            suggested_threads = parsedArgs.threads;
        }

        if (suggested_threads == 0)
        {
            std::vector<std::string> res = std::vector<std::string>();
            string r = "";
            for (auto i = tb->begin(); i != tb->end(); i++)
            {
                if(evalCondition(*i))
                {
                    
                    r = "( ";
                    for(const auto &j: (col))
                    {
                        if( j == -1)
                        {
                            r += i->key();
                            r += " ";
                        }
                        else 
                        {
                            r += to_string((*i)[(u_int64_t)j]);
                            r += " ";
                        }
                    }
                    r += ")";
                    res.push_back(r);
                }
            }
            if (res.empty() == true)
            {
                return make_unique<NullQueryResult>();
            }
            std::sort(res.begin(),res.end());
            for (auto &i: res){
                cout << i << "\n";
            }
            return make_unique<SuccessMsgResult>(qname, targetTable);
        }
        else
        {
            //Claim threads
            thread * t = new thread[(u_int64_t)suggested_threads];
            vector<string> *res = new vector<string>[(u_int64_t)suggested_threads];

            for (int i = 0; i < suggested_threads;i++)
            {
                t[i] = thread(SelectSection2,i,res+i,tb,&col,this);
            }
            for (int i = 0; i < suggested_threads;i++)
            {
                t[i].join();
            }
            // output
            // Check if All empty:
            bool flag = false;
            for(int i = 0; i < suggested_threads; i++)
            {
                if((res+i)->empty() == false)
                {
                    flag = true;
                    break;
                }
            }
            if (!flag)
            {
                delete[] t;
                delete[] res;
                return make_unique<NullQueryResult>();
            }
            //Out put the smallest
            while(true)
            {
                int smallest = 0;
                string smallestVal = "";
                bool flag = false;
                for(int i = 0; i < suggested_threads; i++)
                {
                    if((res+i)->empty() == false)
                    {
                        flag = true;
                        if(smallestVal == "")
                        {
                            smallestVal = *((res+i)->end()-1);
                            smallest = i;
                        }
                        else
                        {
                            if (*((res+i)->end()-1) < smallestVal)
                            {
                                smallestVal = *((res+i)->end()-1);
                                smallest = i;
                            }
                        }
                    }
                }
                if (flag == false) break;
                cout << smallestVal<<endl;
                (res+smallest)->pop_back();
            }

            delete[] t;
            delete[] res;
            return make_unique<SuccessMsgResult>(qname, targetTable);
        }
    }
    catch (const TableNameNotFound &e) 
    {
        return make_unique<ErrorMsgResult>(qname, this->targetTable,
                                       "No such table."s);
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