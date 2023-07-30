#ifndef TABLE_WORKBOOK_FILE_ERROR_INCLUDED
#define TABLE_WORKBOOK_FILE_ERROR_INCLUDED

#include <stdexcept>

class TableWorkbookFileError : public std::runtime_error {
public:
  TableWorkbookFileError(const std::string &msg) : std::runtime_error(msg) {}
};

#endif
