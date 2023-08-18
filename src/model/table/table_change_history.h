#ifndef TABLE_CHANGE_HISTORY_INCLUDED
#define TABLE_CHANGE_HISTORY_INCLUDED
#include "tools.h"
#include <chrono>
#include <cstddef>
#include <deque>
#include <memory>
#include <iostream>

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
  StateHistoryItem(const CellState &state) {
    update_time();
    cell_states.push_back(state);
  }

  // Constructor for many cell state items
  StateHistoryItem(const CellStates &states) : cell_states(states) {
    update_time();
  }

  void update_time() { time_stamp = std::chrono::system_clock::now(); }

  void reverse() {
    for (auto &state : cell_states) {
      state.reverse();
    }
  }
};

typedef std::shared_ptr<StateHistoryItem> StateHistoryItemPtr;

class StateChangeQueue {
public:
  StateChangeQueue(size_t max_items) { _max_items = max_items; }

  bool empty() const { return _queue.empty(); }

  size_t size() const { return _queue.size(); }

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
  TableSheetChangeHistory(size_t max_items = 50)
      : _undo_queue(max_items), _redo_queue(max_items) {}

  void push_state(const StateHistoryItemPtr &state) {
    _undo_queue.push_state(state);
  }

  StateHistoryItemPtr pop_and_swap(StateChangeQueue &source,
                                   StateChangeQueue &dest) {
    if (source.empty()) { return {}; }

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
