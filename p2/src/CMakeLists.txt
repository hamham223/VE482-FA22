project(lemondb)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS
    "-Wall -Wextra -Werror -pedantic -Wno-unused-result -Wconversion -Wno-unused-parameter -O2"
)
#set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=-*,clang-analyzer-*,performance-*,misc-*;-fix")

set(SOURCE_FILES
    parsedArgs.h
    db/Database.h
    db/Database.cpp
    db/Table.h
    db/Table.cpp
    query/Query.h
    query/Query.cpp
    query/QueryParser.h
    query/QueryParser.cpp
    query/QueryResult.h
    query/QueryResult.cpp
    query/QueryBuilders.h
    query/QueryBuilders.cpp
    query/management/DeleteQuery.h
    query/management/DeleteQuery.cpp
    query/management/LoadTableQuery.h
    query/management/LoadTableQuery.cpp
    query/management/SelectQuery.h
    query/management/SelectQuery.cpp
    query/management/SumQuery.h
    query/management/SumQuery.cpp
    query/management/MinMaxQuery.h
    query/management/MinMaxQuery.cpp
    query/management/CopyTableQuery.h
    query/management/CopyTableQuery.cpp
    query/management/CountQuery.h
    query/management/CountQuery.cpp
    query/management/DropTableQuery.h
    query/management/DropTableQuery.cpp
    query/management/DumpTableQuery.h
    query/management/DumpTableQuery.cpp
    query/management/ListTableQuery.h
    query/management/ListTableQuery.cpp
    query/management/QuitQuery.h
    query/management/QuitQuery.cpp
    query/management/PrintTableQuery.h
    query/management/PrintTableQuery.cpp
    query/management/TruncateTableQuery.h
    query/management/TruncateTableQuery.cpp
    query/management/DuplicateQuery.h
    query/management/DuplicateQuery.cpp
    query/management/SwapQuery.h
    query/management/SwapQuery.cpp
    query/data/InsertQuery.h
    query/data/InsertQuery.cpp
    query/data/UpdateQuery.h
    query/data/UpdateQuery.cpp
    query/data/SubQuery.h
    query/data/SubQuery.cpp
    query/management/TruncateTableQuery.h
    query/management/TruncateTableQuery.cpp
    utils/formatter.h
    utils/uexception.h)
add_executable(lemondb main.cpp ${SOURCE_FILES})
target_link_libraries(lemondb pthread)
