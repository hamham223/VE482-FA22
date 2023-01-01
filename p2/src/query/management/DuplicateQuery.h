#ifndef PROJECT_DUPLICATEEQUERY_H
#define PROJECT_DUPLICATEEQUERY_H

#include <vector>
#include <utility>
#include <string>

#include "../Query.h"

class DuplicateQuery : public ComplexQuery {
  static constexpr const char *qname = "DUPLICATE";

public:
  explicit DuplicateQuery(
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

void DuplicateSection(const int secID, Table *tb ,std::string *res, int *count , DuplicateQuery *q);
void DuplicateAll(Table *tb, std::string *res, int *count, DuplicateQuery *q);

#endif // PROJECT_DUPLICATEEQUERY_H
