#ifndef PROJECT_MINMAXQUERY_H
#define PROJECT_MINMAXQUERY_H

#include <vector>
#include <utility>
#include <string>

#include "../Query.h"

class MinQuery : public ComplexQuery
{
        static constexpr const char *qname = "MIN";

    public:
        explicit MinQuery(std::string table,
                             std::vector<std::string> operandtoken,
                             std::vector<QueryCondition> conditiontoken)
                             : ComplexQuery(std::move(table), std::move(operandtoken), std::move(conditiontoken)){}

        QueryResult::Ptr execute() override;

        std::string toString() override;

};

void FindSectionMin(const int secID, int * minres, Table *tb,std::vector<int> *col, MinQuery * q);

class MaxQuery : public ComplexQuery
{
        static constexpr const char *qname = "MAX";

    public:
        explicit MaxQuery(std::string table,
                             std::vector<std::string> operandtoken,
                             std::vector<QueryCondition> conditiontoken)
                             : ComplexQuery(std::move(table), std::move(operandtoken), std::move(conditiontoken)){}

        QueryResult::Ptr execute() override;

        std::string toString() override;
};
void FindSectionMax(const int secID, int * maxres, Table *tb,std::vector<int> *col, MaxQuery * q);

#endif
