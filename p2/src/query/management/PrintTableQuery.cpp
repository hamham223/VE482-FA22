//
// Created by liu on 18-10-25.
//

#include <iostream>
#include <memory>
#include <string>

#include "PrintTableQuery.h"
#include "../../db/Database.h"

constexpr const char *PrintTableQuery::qname;


QueryResult::Ptr PrintTableQuery::execute() {
  // using namespace std;
  using std::make_unique;
  using std::cout;
  using std::endl;
  using std::literals::string_literals::operator ""s;
  Database &db = Database::getInstance();
  try {
    auto &table = db[this->targetTable];
    cout << "================\n";
    cout << "TABLE = ";
    cout << table;
    cout << "================\n" << endl;
    return make_unique<SuccessMsgResult>(qname, this->targetTable);
  } catch (const TableNameNotFound &e) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable,
                                       "No such table."s);
  }
}

std::string PrintTableQuery::toString() {
  return "QUERY = SHOWTABLE, Table = \"" + this->targetTable + "\"";
}