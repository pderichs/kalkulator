#ifndef TABLE_ROW_DEFINITION_INCLUDED
#define TABLE_ROW_DEFINITION_INCLUDED

#include <memory>
#include <string>
#include <vector>

#define DEFAULT_ROW_HEIGHT 30

struct TableRowDefinition {
  std::string caption;
  size_t height;

  TableRowDefinition() { height = DEFAULT_ROW_HEIGHT; }
};

typedef std::shared_ptr<TableRowDefinition> TableRowDefinitionPtr;
typedef std::vector<TableRowDefinitionPtr> TableRowDefinitions;

#endif
