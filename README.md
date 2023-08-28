# Kalkulator

This is a simple and small spreadsheet application which uses a Lisp-like language for cell formulas.

> [!WARNING]
> This is by no means bug free and / or production ready - **USE AT YOUR OWN RISK**.

I have implemented a similar application in C before ("simplecalc"), but for usability reasons I wanted to create a GUI application now.

## License

This project makes use of other external libraries and projects. The used parts of these external libraries and projects can be found in the `vendor` folder in this repository. Please refer to their respective LICENSES, terms and conditions on their websites:

- [wxWidgets](https://www.wxwidgets.org/)
- [SQLite](https://www.sqlite.org/index.html)
- [Google Material Design Icons](https://github.com/google/material-design-icons)

The source code and documentation of this project is licensed under the [Apache License 2.0](https://www.apache.org/licenses/LICENSE-2.0.txt).

## TODO List

- FEAT: Add simple format: bold
- FEAT: Add simple format: italic
- FEAT: Add simple format: underlined
- FEAT: Add simple format: background color
- FEAT: Add simple format: foreground color
- FEAT: Add simple format: font size
- FEAT: General command function like in Emacs (M-x) or Sublime (C+Shift+P):
  - @ x y (goto cell)
  - ...
- FEAT: Extend cell reference function with additional parameter to reference a cell on another sheet
- MAYB: Support cell type currency
- MAYB: Support high precision decimals and integers? Option: GMP lib: https://gmplib.org/
- FEAT: Print
- FEAT: Find / Replace
- FEAT: Add repl for lisp
- FEAT: Add code editor for macros?
- FEAT: Add macro support: OnLoad?
- FEAT: Macros: Add OnTimer and Timer start functionality
- FEAT: Direct editing? (without the need of pressing F2)
- FEAT: Lisp progn
- FEAT: Lisp abs
- FEAT: Lisp EPSILON (provide access to std::numeric_limits<double>::epsilon())
- FEAT: Lisp tan
- FEAT: Lisp cos
- FEAT: Lisp sin
- FEAT: Lisp root
- FEAT: Lisp ceil
- FEAT: Lisp log(arithm)
- FEAT: Lisp map (after lambda)
- FEAT: Lisp Support for variables (setq)
- FEAT: isnone
- FEAT: Complete minimal lisp interpreter: https://de.wikipedia.org/wiki/Lisp#Minimaler_Funktionsumfang_f%C3%BCr_Lisp
  - lambda
- MAYB: Cache function results (see field _func_result on LispValue)
- FEAT: Add application settings (e.g. size of undo steps)
- DOC: Add explanation of functions (especially behaviour of eq)
- FEAT: Cell comments

# DONE

- BUG: When header controls get focus, they handle key presses by themselves - switch focus back to cells as soon as header gets focused
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
- REFA: function execution contexts use the same pattern everytime when checking for function or numerical value. This can be cleaned up.
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
- BUG: Support dark UI of Desktop Environment (see: https://stackoverflow.com/questions/58920057/wxwidgets-dark-mode-on-macos)
- BUG: Make sure cell_range ranges behave correctly  (with reversed directions)
- FEAT: Add avg function
- FEAT: Save (sqlite)
- FEAT: Open (sqlite)
- BUG: Clearing cell content is crashing the app
- FEAT_ Add cell_range function which returns lists of values of cells
- FEAT: Support lists for mathematical operations: This helps later with cell_ranges
- REFA: There are functionalities in LispFunctionExecutionContext derived classes which principally just say "I want a number or a function which returns a number" - introduce function which streamlines this process
- BUG: Add tests for function contexts
- BUG: LispValue does not support IDENTIFIERS - could be useful for variables
- Change numbers in UI to be zero based (helps with reference of cells)
- Add lists: (list 3 556 "Hello")
