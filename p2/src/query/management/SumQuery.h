#ifndef PROJECT_SUMQUERY_H
#define PROJECT_SUMQUERY_H

#include <vector>
#include <utility>
#include <string>

#include "../Query.h"

class SumQuery : public ComplexQuery
{
        static constexpr const char *qname = "SUM";

    public:
        explicit SumQuery(std::string table,
                             std::vector<std::string> operandtoken,
                             std::vector<QueryCondition> conditiontoken)
                             : ComplexQuery(std::move(table), std::move(operandtoken), std::move(conditiontoken)){}

        QueryResult::Ptr execute() override;

        std::string toString() override;
};

class SumMulTQuery : public ComplexQuery
{
        static constexpr const char *qname = "SUM";

    public:
        explicit SumMulTQuery(std::string table,
                             std::vector<std::string> operandtoken,
                             std::vector<QueryCondition> conditiontoken)
                             : ComplexQuery(std::move(table), std::move(operandtoken), std::move(conditiontoken)){}

        QueryResult::Ptr execute() override;

        std::string toString() override;
};

void FindSectionSum(const int secID, int * sumres, Table *tb, std::vector<int> *col, SumMulTQuery * q);
void FindAllSum(int * sumres, Table * tb, std::vector<int> * col, SumMulTQuery * q);

#endif
