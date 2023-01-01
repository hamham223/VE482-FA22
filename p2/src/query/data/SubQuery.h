#ifndef PROJECT_SUBQUERY_H
#define PROJECT_SUBQUERY_H

#include <string>
#include <vector>

#include "../Query.h"


class SubQuery : public ComplexQuery {

    static constexpr const char *qname = "SUB";
    
    using ComplexQuery::ComplexQuery;

    QueryResult::Ptr execute() override;

    std::string toString() override;
};

void SectionSub(const int secID, int * count, const uint32_t fSrc, 
        const uint32_t fDest, Table * tb, std::vector<int> * col, SubQuery * q);

#endif // PROJECT_SUBQUERY_H