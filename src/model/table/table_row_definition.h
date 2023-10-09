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

#ifndef TABLE_ROW_DEFINITION_INCLUDED
#define TABLE_ROW_DEFINITION_INCLUDED

#include <memory>
#include <string>
#include <vector>

#define DEFAULT_ROW_HEIGHT 30

/**
 * Represents the caption and height of a row within the table sheet.
 */
struct TableRowDefinition {
  std::string caption;
  size_t height;

  TableRowDefinition() : caption(), height(DEFAULT_ROW_HEIGHT) {}
};

typedef std::shared_ptr<TableRowDefinition> TableRowDefinitionPtr;
typedef std::vector<TableRowDefinitionPtr> TableRowDefinitions;

#endif
