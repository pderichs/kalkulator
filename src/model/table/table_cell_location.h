/**
 * Kalkulator - a simple and small spread sheet app.
 * Copyright (C) 2023  pderichs (derichs@posteo.de)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef TABLE_CELL_LOCATION_H
#define TABLE_CELL_LOCATION_H

#include <string>
#include <set>
#include "location.h"

class TableCellLocation {
public:
  explicit TableCellLocation(const std::string &sheet_name,
                             const Location &location)
      : _sheet(sheet_name), _location(location) {}

  std::string sheet() const { return _sheet; }
  Location location() const { return _location; }

  bool operator<(const TableCellLocation &other) const {
    if (_sheet < other.sheet()) {
      return true;
    } else if (_sheet == other.sheet()) {
      return _location < other.location();
    }

    return false;
  }

  bool operator==(const TableCellLocation &other) const {
    return _sheet == other.sheet() && _location == other.location();
  }

private:
  std::string _sheet;
  Location _location;
};

typedef std::set<TableCellLocation> TableCellLocationSet;

#endif //TABLE_CELL_LOCATION_H
