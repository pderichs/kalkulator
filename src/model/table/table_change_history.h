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

#ifndef TABLE_CHANGE_HISTORY_INCLUDED
#define TABLE_CHANGE_HISTORY_INCLUDED

#include "tools.h"
#include <chrono>
#include <cstddef>
#include <deque>
#include <iostream>
#include <memory>

struct CellState {
  Location location;
  std::string prev;
  std::string next;

  void reverse() { std::swap(prev, next); }
};

typedef std::vector<CellState> CellStates;

struct StateHistoryItem {
  std::chrono::system_clock::time_point time_stamp;
  CellStates cell_states;

  // Constructor for single cell state item
  explicit StateHistoryItem(const CellState &state)
      : time_stamp(std::chrono::system_clock::now()), cell_states({state}) {}

  // Constructor for many cell state items
  explicit StateHistoryItem(const CellStates &states)
      : time_stamp(std::chrono::system_clock::now()), cell_states(states) {}

  void reverse() {
    for (auto &state : cell_states) {
      state.reverse();
    }
  }
};

typedef std::shared_ptr<StateHistoryItem> StateHistoryItemPtr;

class StateChangeQueue {
public:
  explicit StateChangeQueue(size_t max_items): _max_items(max_items), _queue() {}

  [[nodiscard]] bool empty() const { return _queue.empty(); }

  [[nodiscard]] size_t size() const { return _queue.size(); }

  void push_state(const StateHistoryItemPtr &state) {
    if (_queue.size() >= _max_items) {
      // Delete oldest state
      _queue.pop_back();
    }

    _queue.push_front(state);
  }

  StateHistoryItemPtr pop_state() {
    auto result = _queue.front();
    _queue.pop_front();
    return result;
  }

private:
  size_t _max_items;
  std::deque<StateHistoryItemPtr> _queue;
};

class TableSheetChangeHistory {
public:
  explicit TableSheetChangeHistory(size_t max_items = 50)
      : _undo_queue(max_items), _redo_queue(max_items) {}

  void push_state(const StateHistoryItemPtr &state) {
    _undo_queue.push_state(state);
  }

  static StateHistoryItemPtr pop_and_swap(StateChangeQueue &source,
                                   StateChangeQueue &dest) {
    if (source.empty()) {
      return {};
    }

    StateHistoryItemPtr item = source.pop_state();

    StateHistoryItemPtr reversed_item =
        std::make_shared<StateHistoryItem>(*item);
    reversed_item->reverse();
    dest.push_state(reversed_item);

    return item;
  }

  StateHistoryItemPtr undo() {
    std::cerr << "Undo queue count: " << _undo_queue.size() << std::endl;
    std::cerr << "Redo queue count: " << _redo_queue.size() << std::endl;

    return pop_and_swap(_undo_queue, _redo_queue);
  }

  StateHistoryItemPtr redo() { return pop_and_swap(_redo_queue, _undo_queue); }

private:
  StateChangeQueue _undo_queue;
  StateChangeQueue _redo_queue;
};

#endif
