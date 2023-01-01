//
// Created by liu on 18-10-25.
//

#ifndef PROJECT_UPDATEQUERY_H
#define PROJECT_UPDATEQUERY_H

#include <string>

#include "../Query.h"

class UpdateQuery : public ComplexQuery {
  static constexpr const char *qname = "UPDATE";
public:
  Table::ValueType
      fieldValue; // = (operands[0]=="KEY")? 0 :std::stoi(operands[1]);
  Table::FieldIndex fieldId;
  Table::KeyType keyValue;

public:
  using ComplexQuery::ComplexQuery;

  QueryResult::Ptr execute() override;

  std::string toString() override;
};

void UpdateSection(const int secID, int * res, Table *tb, UpdateQuery * q);

#endif // PROJECT_UPDATEQUERY_H
