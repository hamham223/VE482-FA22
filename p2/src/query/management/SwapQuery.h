#ifndef PROJECT_SWAPQUERY_H
#define PROJECT_SWAPQUERY_H
#include <vector>
#include <utility>
#include <string>



#include "../Query.h"


class SwapQuery : public ComplexQuery
{
  static constexpr const char *qname = "SWAP";
  //const std::vector<std::string> operands;
  //const std::vector<QueryCondition> conditions;

  public:
    explicit SwapQuery(
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

void SwapSection(const int secID, u_int64_t op1, u_int64_t op2, Table *tb, int *count, SwapQuery *q);
void SwapAll(u_int64_t op1, u_int64_t op2, Table *tb, int *count, SwapQuery *q);

#endif // PROJECT_SWAPQUERY_H