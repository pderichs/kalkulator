# Kalkulator

This is a simple and small spreadsheet application which uses a Lisp-like language for cell formulas.

> [!WARNING]
> This is by no means bug free and / or production ready - **USE AT YOUR OWN RISK**.

I have implemented a similar application in C before ("simplecalc"), but for usability reasons I wanted to create a GUI
application now. There are of course much more advanced and more sophisticated spread sheet applications out there -
this has just been implemented for learning purpose and fun :)

## How does it work?

Start the application

```sh 
  bin/kalkulator
```

You can move the cell selection with your cursor keys or by using the mouse. Alternatively you can press `Ctrl+G` and
enter the coordinates in the form `row column` to navigate to the cell with coordinates `row` and `col` (e.g. to
navigate to cell with coordinates row 10 and column 20 just enter `10 20`).

Press `F2` to edit cell contents (this moves the cursor to the formula text field above the cells view). Press `Enter`
to apply your changes to the cell or `Esc` to abort.

### Sample

1. Move the cursor to cell row 0, col 0.
2. Press F2 and enter `40`.
3. Move the cursor to cell row 1, col 0.
4. Press F2 and enter `2`.
5. Move the cursor to cell row 2, col 0.
6. Press F2 and enter `The Answer to Life, the Universe and Everything:`.
7. In the menu bar select `Table -> Resize Column...`.
8. Extend the column size to e.g. `300` to give more space for the text in cell 2, 0.
9. Move the cursor to cell row 2, col 1.
10. Press F2 and enter `=(+ (cell 0 0) (cell 1 0))`.

## Build

Install dependencies in Debian:

``` sh
  apt install libwxgtk3.2-dev libsqlite3-dev
```

### Clean

``` sh
  cmake --build . --target clean -j 5
```

### Build

```sh
  cmake --build . --target all -j 5
```

### Run Tests

```sh 
  bin/kalkulator_tests
```

## License

This project is licensed under the [GNU GENERAL PUBLIC LICENSE v3](https://www.gnu.org/licenses/gpl-3.0.txt).

This project makes use of other external libraries and projects. Please refer to their respective LICENSES, terms and
conditions on their websites:

- [wxWidgets](https://www.wxwidgets.org/)
- [SQLite](https://www.sqlite.org/index.html)
- [Google Material Design Icons](https://github.com/google/material-design-icons)
- [Google Test](https://github.com/google/googletest) (see tests/lib folder, based on
  commit: `e40661d89b051e9ef4eb8a2420b74bf78b39ef41`)

## TODO List

- FEAT: Add github action for tests
- FEAT: Multiple cell selection
- FEAT: Replace
- FEAT: Print
- FEAT: Add repl for lisp
- FEAT: Add code editor for macros?
- FEAT: Add macro support: OnLoad?
- FEAT: Macros: Add OnTimer and Timer start functionality
- FEAT: Direct editing? (without the need of pressing F2)
- FEAT: Add application settings (e.g. size of undo steps)
- DOC: Add explanation of functions (especially behaviour of eq)
- FEAT: Cell comments
- FEAT: Make cell reference and cell_range update when dragging a cell downwards or to the right (also add fixed
  references, like $ function)
- BUG: Something (CLion?) is creating a "Testing" folder on the root folder with every build
- MAYB: Support cell type currency
- MAYB: Support high precision decimals and integers? Option: GMP lib: https://gmplib.org/
- MAYB: Cache function results (see field _func_result on LispValue)

## DONE

- FEAT: Mark current row/cell in header controls
- FEAT: Search (maybe a non modal dialog which lists the matching cell locations)
- FEAT: Extend cell reference function with additional parameter to reference a cell on another sheet
- FEAT: Add possibility to add or remove sheets
- FEAT: defun (with newly added lambda support, this should be doable now)
- FEAT: Lisp EPSILON (provide access to std::numeric_limits<double>::epsilon()) (could be a variable in a default scope)
- FEAT: Lisp Support for variables (set-var)
- FEAT: Lisp is-none
- FEAT: Lisp log(arithm)
- FEAT: Lisp pow
- FEAT: Lisp sqrt
- FEAT: Lisp ceil
- FEAT: Lisp abs
- FEAT: Store true and false lisp bool values (don't create them everytime)
- FEAT: Store error strings instead of creating them every time (PARAMCOUNTERR, PARAMERR)
- FEAT: Lisp sin
- FEAT: Lisp cos
- FEAT: Lisp tan
- FEAT: Add < function
- FEAT: Add > function
- BUG: Cell does not check for numbers (expect_number)
- BUG: More tests for and
- BUG: More tests for or
- BUG: More tests for if
- REFA: Design: Function contexts are always calling extract_params_from_list, then
  execute_functions_and_extract_list_results
- REFA: eval tests refactoring
- BUG: gtest is creating a lib folder on the root with every build
- BUG: Change name of tests executable
- BUG: ScrollToCell -> Cell should be in center of view
- FEAT: Add simple format: bold
- FEAT: Add simple format: italic
- FEAT: Add simple format: underlined
- FEAT: Add simple format: background color
- FEAT: Add simple format: foreground color
- FEAT: Add simple format: font size
- EVAL: Is recalculation dependent on repaint events? -> if so, change it. Should only be a concern of the model.
- BUG: Add tests for test kal files (e.g. cell references which get their location from table cells)
- REFA: License update
- REFA: CMake
- BUG: Format dialog does not show current format of cell
- BUG: Newly applied format of cell (e.g. background color) is not visible when cell is selected
- BUG: Moving current cell to the bottom or right seems to not check boundaries of cell correctly
- BUG: Lambda implementation somewhat feels like a hack
- BUG: "Invalid file structure" error on save
- FEAT: Lisp progn
- BUG: When header controls get focus, they handle key presses by themselves - switch focus back to cells as soon as
  header gets focused
- FEAT: Go to cell function
- FEAT: Pressing Escape in editor control should move focus back to cells view
- BUG: Store row and column sizes
- FEAT: Lisp floor
- FEAT: Provide possibility to change column width
- FEAT: Provide possibility to change row height
- FEAT: Add sheet selection
- FEAT: Add "Open" action to toolbar
- FEAT: Add "Save" menu option
- FEAT: Add "Save" action to toolbar
- FEAT: Update about dialog
- FEAT: Toolbar
- REFA: Copy/Paste should only copy values (provide alternative copy for formulas)
- FEAT: Undo
- FEAT: Redo
- REFA: pens and brushes and some scroll related functions are duplicated atm
- BUG: Cell content should not be drawn outside of cells
- BUG: Hide scrollbars from header controls
- BUG: Scrolling to the outer left crashes app
- BUG: Height is fixed atm
- BUG: Scrolling should not scroll col/row header controls
- BUG: Add tests for LispValue
- REFA: Move avg function to normal lisp functions
- BUG: Add tests for cell function
- BUG: Add tests for cell_range function
- FEAT: Complete minimal lisp interpreter: https://de.wikipedia.org/wiki/Lisp#Minimaler_Funktionsumfang_f%C3%BCr_Lisp
    - DONE lambda
    - DONE and
    - DONE or
    - DONE xor
    - DONE not
    - DONE first
    - DONE rest
    - DONE quote (DONE with "list")
    - DONE join
    - DONE cons
    - DONE eq (see prepared lisp_eq on LispValue)
    - DONE if/cond
- REFA: function execution contexts use the same pattern everytime when checking for function or numerical value. This
  can be cleaned up.
- BUG: Fix scroll bar behaviour (scrolling is not useable atm)
  Refactoring of table control might be needed
- FEAT: Implement DELETE operation on cells
- BUG: Fix cell_range circular reference
- FEAT: Copy / Paste
- BUG: Respect changed flag in UI
- BUG: Fix color of selected cell (should be blueish)
- FEAT: Add "New" operation to menu
- REFA: Clean up main.cpp
- FEAT: Add bool type for LispValue (could be useful for if)
- BUG: Fix circular reference formula bug: Referencing the same cell in a formula with "cell" causes the app to crash
- BUG: Support dark UI of Desktop Environment (
  see: https://stackoverflow.com/questions/58920057/wxwidgets-dark-mode-on-macos)
- BUG: Make sure cell_range ranges behave correctly  (with reversed directions)
- FEAT: Add avg function
- FEAT: Save (sqlite)
- FEAT: Open (sqlite)
- BUG: Clearing cell content is crashing the app
- FEAT_ Add cell_range function which returns lists of values of cells
- FEAT: Support lists for mathematical operations: This helps later with cell_ranges
- REFA: There are functionalities in LispFunctionExecutionContext derived classes which principally just say "I want a
  number or a function which returns a number" - introduce function which streamlines this process
- BUG: Add tests for function contexts
- BUG: LispValue does not support IDENTIFIERS - could be useful for variables
- Change numbers in UI to be zero based (helps with reference of cells)
- Add lists: (list 3 556 "Hello")
