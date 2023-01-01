#ifndef PROJECT_TRUNCATETABLEQUERY_H
#define PROJECT_TRUNCATETABLEQUERY_H

#include <utility>
#include <string>

#include "../Query.h"

// Operation to the whole table
class TruncateTableQuery : public Query {
    static constexpr const char *qname = "TRUNCATE";

    public:
        explicit TruncateTableQuery(std::string table) : Query(std::move(table)){}

        QueryResult::Ptr execute() override;

        std::string toString() override;
};

#endif // PORJECT_TRUNCATETABLEQUERY_H
