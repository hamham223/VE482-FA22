#ifndef PROJECT_DELETEQUERY_H
#define PROJECT_DELETEQUERY_H

#include <vector>
#include <utility>
#include <string>
#include <forward_list>

#include "../Query.h"

class DeleteQuery : public ComplexQuery
{
  static constexpr const char *qname = "DELETE";
  const std::vector<std::string> operands;
  const std::vector<QueryCondition> conditions;

  public:
    explicit DeleteQuery(
        std::string table,
        std::vector<std::string> operandstoken,
        std::vector<QueryCondition> conditiontoken
    ) : ComplexQuery(std::move(table),
                    std::move(operandstoken),
                    std::move(conditiontoken))
    {}

    QueryResult::Ptr execute() override;

    std::string toString() override;
};

class DeleteMulTQuery : public ComplexQuery
{
  static constexpr const char *qname = "DELETE";
  const std::vector<std::string> operands;
  const std::vector<QueryCondition> conditions;

  public:
    explicit DeleteMulTQuery(
        std::string table,
        std::vector<std::string> operandstoken,
        std::vector<QueryCondition> conditiontoken
    ) : ComplexQuery(std::move(table),
                    std::move(operandstoken),
                    std::move(conditiontoken))
    {}

    QueryResult::Ptr execute() override;

    std::string toString() override;
};

void FindSectionDel(const int secID, int * count, std::forward_list<int> * l,
    Table * tb, DeleteMulTQuery * q);

#endif // PROJECT_DELETEQUERY_H
