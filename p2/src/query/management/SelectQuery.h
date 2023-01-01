#ifndef PROJECT_SELECTQUERY_H
#define PROJECT_SELECTQUERY_H

#include <vector>
#include <utility>
#include <string>

#include "../Query.h"

class SelectQuery : public ComplexQuery
{
        static constexpr const char *qname = "SELECT";

    public:
        explicit SelectQuery(std::string table,
                             std::vector<std::string> operandtoken,
                             std::vector<QueryCondition> conditiontoken)
                             : ComplexQuery(std::move(table), std::move(operandtoken), std::move(conditiontoken)){}

        QueryResult::Ptr execute() override;

        std::string toString() override;
};
void SelectSection(const int secID, Table *tb,std::vector<int> *col, SelectQuery * q);

class SelectQuery2 : public ComplexQuery
{
        static constexpr const char *qname = "SELECT";

    public:
        explicit SelectQuery2(std::string table,
                             std::vector<std::string> operandtoken,
                             std::vector<QueryCondition> conditiontoken)
                             : ComplexQuery(std::move(table), std::move(operandtoken), std::move(conditiontoken)){}

        QueryResult::Ptr execute() override;

        std::string toString() override;
};
void SelectSection2(const int secID,std::vector<std::string> * res, Table *tb,std::vector<int> *col, SelectQuery2 * q);
bool cmpStr(std::string a, std::string b);
#endif