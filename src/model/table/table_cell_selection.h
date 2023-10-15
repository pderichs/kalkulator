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

#ifndef TABLE_CELL_SELECTION_H
#define TABLE_CELL_SELECTION_H

#include "location.h"

/**
 * Represents the selection state model within a sheet.
 * A selection can consist of one (primary) or more,
 * additional cell locations.
 */
class TableCellSelection {
public:
  TableCellSelection(): _primary_cell(), _additional_cells() {}

  void set_primary(const Location &location) {
    _primary_cell = location;

    _additional_cells.erase(location);
  }

  Location primary() const { return _primary_cell; }

  void toggle_additional_cell(const Location &location) {
    if (location == _primary_cell) {
      return;
    }

    auto it = _additional_cells.find(location);
    if (it == _additional_cells.end()) {
      _additional_cells.insert(location);
    } else {
      _additional_cells.erase(it);
    }
  }

  void reduce_selection_to_primary() {
    _additional_cells.clear();
  }

  void primary_move_up(size_t amount = 1) {
    _primary_cell.moveUp(amount);
  }

  void primary_move_down(size_t amount = 1) {
    _primary_cell.moveDown(amount);
  }

  void primary_move_left() {
    _primary_cell.moveLeft();
  }

  void primary_move_right() {
    _primary_cell.moveRight();
  }

  long primary_x() const {
    return _primary_cell.x();
  }

  long primary_y() const {
    return _primary_cell.y();
  }

  LocationSet additional_cells() const { return _additional_cells; }

  size_t addition_cell_count() const { return _additional_cells.size(); }

  LocationSet all_locations() const {
    LocationSet result(_additional_cells);
    result.insert(_primary_cell);
    return result;
  }

private:
  Location _primary_cell;
  LocationSet _additional_cells;
};

#endif // TABLE_CELL_SELECTION_H
