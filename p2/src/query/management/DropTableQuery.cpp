//
// Created by liu on 18-10-25.
//

#include <memory>
#include <string>

#include "DropTableQuery.h"
#include "../../db/Database.h"

constexpr const char *DropTableQuery::qname;


QueryResult::Ptr DropTableQuery::execute() {
  // using namespace std;
  using std::make_unique;
  using std::literals::string_literals::operator ""s;
  Database &db = Database::getInstance();
  try {
    db.dropTable(this->targetTable);
    return make_unique<SuccessMsgResult>(qname);
  } catch (const TableNameNotFound &e) {
    return make_unique<ErrorMsgResult>(qname, targetTable, "No such table."s);
  } catch (const std::exception &e) {
    return make_unique<ErrorMsgResult>(qname, e.what());
  }
}

std::string DropTableQuery::toString() {
  return "QUERY = DROP, Table = \"" + targetTable + "\"";
}