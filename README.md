# Kalkulator

This is a simple and small spreadsheet application which uses a Lisp-like language for cell formulas.

This is by no means bug free and / or production ready - **USE AT YOUR OWN RISK**.

I have implemented a similar application in C with ncurses before ("simplecalc"), but for usability reasons I wanted to create a GUI application now.

This is one item of the "applications every software engineer should implement once in their lifetime" list :)

# TODO

- FEAT: Copy / Paste
- BUG: Fix cell_range circular reference
- FEAT: Implement DELETE operation on cells
- FEAT: Direct editing? (without the need of pressing F2)
- FEAT: Complete minimal lisp interpreter: https://de.wikipedia.org/wiki/Lisp#Minimaler_Funktionsumfang_f%C3%BCr_Lisp
  - DONE first
  - DONE rest
  - DONE quote (DONE with "list")
  - cons
  - eq (see prepared lisp_eq on LispValue)
  - cond (use "if" as identifier)
  - lambda
- MAYB: Cache function results (see field _func_result on LispValue)
- REFA: function execution contexts use the same pattern everytime when checking for function or numerical value. This can be cleaned up.
- BUG: Fix scroll bar behaviour (scrolling is not useable atm)
- FEAT: Add simple format: bold
- FEAT: Add simple format: italic
- FEAT: Add simple format: underlined
- FEAT: Add simple format: background color
- FEAT: Add simple format: foreground color
- FEAT: Add simple format: font size
- FEAT: Funktion isnone

# DONE

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
