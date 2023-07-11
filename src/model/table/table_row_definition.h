#ifndef TABLE_ROW_DEFINITION_INCLUDED
#define TABLE_ROW_DEFINITION_INCLUDED

#include <memory>
#include <string>
#include <vector>

struct TableRowDefinition {
    std::string name;
    size_t height;
};

typedef std::shared_ptr<TableRowDefinition> TableRowDefinitionPtr;
typedef std::vector<TableRowDefinitionPtr> TableRowDefinitions;

#endif
