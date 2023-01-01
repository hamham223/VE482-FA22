#include <iostream>
#include <string>
#include <memory>

#include "CountQuery.h"
#include "../../db/Database.h"

constexpr const char *CountQuery::qname;


std::string CountQuery::toString(){
    return "QUERY = COUNT TABLE, TABLE = \"" + 
                    this->targetTable + "\"";
}

QueryResult::Ptr CountQuery::execute(){
    // using namespace std;
    using std::make_unique;
    using std::cout;
    Database &db = Database::getInstance();

    try {
        Table & tb = db[targetTable];

        auto result = initCondition(tb);

        int count = 0;

        for (auto i = tb.begin(); i != tb.end(); i++) {
            if (this->evalCondition(*i)) count++;
        }

        cout << "ANSWER = " << count << "\n";

    }
    catch (const std::exception &e){
        return make_unique<ErrorMsgResult>(qname,e.what());
    }
    return make_unique<SuccessMsgResult>(qname, targetTable);
}
