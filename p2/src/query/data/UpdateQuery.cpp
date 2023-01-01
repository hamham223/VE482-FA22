//
// Created by liu on 18-10-25.
//

#include "UpdateQuery.h"

#include <memory>
#include <string>
#include <thread>
#include <cmath>

#include "../../db/Database.h"
#include "../../parsedArgs.h"
extern ParsedArgs parsedArgs;
static int64_t suggested_threads = 0;

constexpr const char *UpdateQuery::qname;

void UpdateSection(const int secID, int * res, Table *tb, UpdateQuery * q)
{
  const int secLen = (int)(tb->size()/(size_t)suggested_threads);
  //Init of result
  *res = 0;
  for(auto i = tb->begin() + secID*secLen; i != ((secID == suggested_threads-1)? tb->end() : tb->begin()+(secID+1)*secLen); i++)
    {
      if(q->evalCondition(*i))
      {
        if (q->keyValue.empty()) {
            (*i)[q->fieldId] = q->fieldValue;
          } else {
            i->setKey(q->keyValue);
          }
          ++(*res);
      }
    }
}

QueryResult::Ptr UpdateQuery::execute() {
  // using namespace std;
  using std::thread;
  using std::make_unique;
  using std::literals::string_literals::operator ""s;

  if (this->operands.size() != 2)
    return make_unique<ErrorMsgResult>(
        qname, this->targetTable.c_str(),
        "Invalid number of operands (? operands)."_f % operands.size());
  Database &db = Database::getInstance();
  Table::SizeType counter = 0;
  try {
    auto &table = db[this->targetTable];
    if (this->operands[0] == "KEY") 
    {
      this->keyValue = this->operands[1];
    } 
    else 
    {
      this->fieldId = table.getFieldIndex(this->operands[0]);
      this->fieldValue =
          (Table::ValueType)strtol(this->operands[1].c_str(), nullptr, 10);
    }
    auto result = initCondition(table);
    //if (result.second) {
      suggested_threads = (int64_t)round(((double)(table.size())/pow(2,11)));
      if (parsedArgs.threads > 0 && parsedArgs.threads < suggested_threads)
      {
          suggested_threads = parsedArgs.threads;
      }
      if (suggested_threads > 64) suggested_threads = 64;

      if(suggested_threads == 0)
      {
        for (auto i = table.begin(); i < table.end(); i++)
        {
          if(this->evalCondition(*i))
          {
            if (this->keyValue.empty())
            {
              (*i)[this->fieldId] = this->fieldValue;
            } 
            else 
            {
              i->setKey(this->keyValue);
            }
            ++counter;
          }
        }
        return make_unique<RecordCountResult>(counter);
      }
      else
      {
        //Claim threads
        thread * t = new thread[(u_int64_t)suggested_threads];
        // array to store results
        int * res = new int[(u_int64_t)suggested_threads];
        for (int i = 0; i < suggested_threads;i++)
        {
            t[i] = thread(UpdateSection,i,res+i,&table,this);
        }
        for (int i = 0; i < suggested_threads;i++)
        {
            t[i].join();
            counter += (Table::SizeType)(res[i]);
        }
        delete [] res;
        delete [] t;
      }
      return make_unique<RecordCountResult>(counter);
    //}  
  } catch (const TableNameNotFound &e) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable,
                                       "No such table."s);
  } catch (const IllFormedQueryCondition &e) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable, e.what());
  } catch (const std::invalid_argument &e) {
    // Cannot convert operand to string
    return make_unique<ErrorMsgResult>(qname, this->targetTable,
                                       "Unknown error '?'"_f % e.what());
  } catch (const std::exception &e) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable,
                                       "Unkonwn error '?'."_f % e.what());
  }
}

std::string UpdateQuery::toString() {
  return "QUERY = UPDATE " + this->targetTable + "\"";
}
