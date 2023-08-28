#ifndef TABLE_CELL_COLOR_INCLUDED
#define TABLE_CELL_COLOR_INCLUDED

struct TableCellColor {
  TableCellColor() {
    r = 0;
    g = 0;
    b = 0;
  }

  unsigned char r;
  unsigned char g;
  unsigned char b;
};

#endif
