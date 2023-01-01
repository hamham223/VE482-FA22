#ifndef PARSEDARGS_H
#define PARSEDARGS_H
#include <string>
struct ParsedArgs{
  std::string listen;
  long threads = 0;
};

#endif