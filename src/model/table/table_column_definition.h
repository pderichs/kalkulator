#ifndef TABLE_COLUMN_INCLUDED
#define TABLE_COLUMN_INCLUDED

#include <memory>
#include <string>
#include <vector>

#define DEFAULT_COLUMN_WIDTH 100

struct TableColumnDefinition {
  std::string caption;
  size_t width;

  TableColumnDefinition() { width = DEFAULT_COLUMN_WIDTH; }
};

typedef std::shared_ptr<TableColumnDefinition> TableColumnDefinitionPtr;
typedef std::vector<TableColumnDefinitionPtr> TableColumnDefinitions;

#endif
