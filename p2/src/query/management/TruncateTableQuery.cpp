#include <iostream>
#include <memory>
#include <string>

#include "TruncateTableQuery.h"
#include "../../db/Database.h"

constexpr const char *TruncateTableQuery::qname;


QueryResult::Ptr TruncateTableQuery::execute(){
    // using namespace std;
    using std::make_unique;
    using std::literals::string_literals::operator ""s;
    Database &db = Database::getInstance();

    try{
        auto &table = db[targetTable];
        // Clean the content of table, but not influence its fields.
        table.clear();
        return make_unique<NullQueryResult>();
    } catch (const TableNameNotFound &e) {
        return make_unique<ErrorMsgResult>(qname,this->targetTable,"No such table."s);
    } catch (const IllFormedQueryCondition &e){
        return make_unique<ErrorMsgResult>(qname,this->targetTable,e.what());
    } catch (const std::invalid_argument &e){
        return make_unique<ErrorMsgResult>(qname,this->targetTable,"Unknown error '?'"_f % e.what());
    } catch (const std::exception &e){
        return make_unique<ErrorMsgResult>(qname,this->targetTable,"Unknown error '?'."_f % e.what());
    }
}

std::string TruncateTableQuery::toString(){
    return "QUERY = TRUNCATE table \"" + this-> targetTable + "\"";
}
