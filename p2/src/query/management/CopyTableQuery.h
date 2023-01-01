#ifndef PROJECT_COPYTABLEQUERY_H
#define PROJECT_COPYTABLEQUERY_H

#include <utility>
#include <string>

#include "../Query.h"

// Operation to the whole table
class CopyTableQuery : public Query{
    static constexpr const char *qname = "COPYTABLE";
    const std::string output_table;

    public:
        explicit CopyTableQuery(std::string input_table, std::string output_table)
            : Query(std::move(input_table)), output_table(std::move(output_table)){}

    QueryResult::Ptr execute() override;

    std::string toString() override;
};

#endif // PROJECT_COPYTABLEQUERY_H
