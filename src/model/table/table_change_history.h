#ifndef TABLE_CHANGE_HISTORY_INCLUDED
#define TABLE_CHANGE_HISTORY_INCLUDED
#include "tools.h"
#include <chrono>
#include <cstddef>
#include <deque>
#include <memory>

struct CellState {
  Location cell;
  std::string prev;
  std::string next;

  void reverse() { std::swap(prev, next); }
};

typedef std::vector<CellState> CellStates;

struct WorkState {
  std::chrono::system_clock::time_point time_stamp;
  CellStates cell_states;

  WorkState() { time_stamp = std::chrono::system_clock::now(); }

  void reverse() {
    for (auto &state : cell_states) {
      state.reverse();
    }
  }
};

typedef std::shared_ptr<WorkState> WorkStatePtr;

class TableSheetChangeHistory {
public:
  TableSheetChangeHistory(size_t max_items = 50) {
    _max_items = max_items == 0 ? 50 : _max_items;
  }

  void push_state(WorkStatePtr state) {
    if (_queue.size() >= _max_items) {
      // Delete oldest state
      _queue.pop_back();
    }

    _queue.push_front(state);
  }

  WorkStatePtr pop_state() {
    auto result = _queue.front();
    _queue.pop_front();
    return result;
  }
  void clear() { _queue.clear(); }

private:
  size_t _max_items;
  std::deque<WorkStatePtr> _queue;
};

#endif
