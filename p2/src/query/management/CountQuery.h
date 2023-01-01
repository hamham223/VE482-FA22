#ifndef PROJECT_COUNTQUERY_H
#define PROJECT_COUNTQUERY_H

#include <vector>
#include <utility>
#include <string>

#include "../Query.h"

class CountQuery : public ComplexQuery
{
        static constexpr const char *qname = "COUNT";

    public:
        explicit CountQuery(std::string table,
                             std::vector<std::string> operandtoken,
                             std::vector<QueryCondition> conditiontoken)
                             : ComplexQuery(std::move(table), std::move(operandtoken), std::move(conditiontoken)){}

        QueryResult::Ptr execute() override;

        std::string toString() override;
};

#endif // PROJECT_COUNTQUERY_H
