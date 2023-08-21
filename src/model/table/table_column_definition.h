#ifndef TABLE_COLUMN_INCLUDED
#define TABLE_COLUMN_INCLUDED

#include <memory>
#include <string>
#include <vector>

struct TableColumnDefinition {
  std::string caption;
  size_t width;

  TableColumnDefinition() { width = 100; }
};

typedef std::shared_ptr<TableColumnDefinition> TableColumnDefinitionPtr;
typedef std::vector<TableColumnDefinitionPtr> TableColumnDefinitions;

#endif
