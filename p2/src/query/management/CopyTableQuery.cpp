#include <memory>
#include <iostream>
#include <string>

#include "CopyTableQuery.h"
#include "../../db/Database.h"

constexpr const char *CopyTableQuery::qname;


QueryResult::Ptr CopyTableQuery::execute(){
    // using namespace std;
    using std::make_unique;
    using std::literals::string_literals::operator ""s;
    Database &db = Database::getInstance();

    try{
        Table const &input_table = db[targetTable];
        db.registerTable(std::make_unique<Table>(output_table,input_table));
        return make_unique<NullQueryResult>();
    } catch (const TableNameNotFound &e){
        return make_unique<ErrorMsgResult>(qname,this->targetTable,"No such table."s);
    } catch (const IllFormedQueryCondition &e){
        return make_unique<ErrorMsgResult>(qname,this->targetTable,e.what());
    } catch (const std::invalid_argument &e){
        return make_unique<ErrorMsgResult>(qname,this->targetTable,"Unkown error '?'"_f % e.what());
    } catch (const std::exception &e){
        return make_unique<ErrorMsgResult>(qname,this->targetTable,"Unkown error '?'."_f % e.what());
    }
}

std::string CopyTableQuery::toString(){
    return "QUERY = COPYTABLE from \"" + this->targetTable + "\" to \"" + this->output_table + "\"";
}
