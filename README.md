# Kalkulator

This is a simple and small spreadsheet application which uses a Lisp-like language for cell formulas.

This is by no means bug free and / or production ready - **USE AT YOUR OWN RISK**.

I have implemented a similar application in C with ncurses before ("simplecalc"), but for usability reasons I wanted to create a GUI application now.

This is one item of the "applications every software engineer should implement once in their lifetime" list :)

# TODO

- FEAT: Add avg function
- FEAT: Complete minimal lisp interpreter: https://de.wikipedia.org/wiki/Lisp#Minimaler_Funktionsumfang_f%C3%BCr_Lisp
  - first
  - rest
  - cons
  - DONE quote (DONE with "list")
  - eq (see prepared lisp_eq on LispValue)
  - cond (use "if" as identifier)
  - lambda
- MAYB: Cache function results (see field _func_result on LispValue)
- FEAT: Save (sqlite)
- FEAT: Open (sqlite)
- BUG: Fix circular formula bug: Referencing the same cell in a formula with "cell" causes the app to crash
- REFA: function execution contexts use the same pattern everytime when checking for function or numerical value. This can be cleaned up.

# DONE

- FEAT_ Add cell_range function which returns lists of values of cells
- FEAT: Support lists for mathematical operations: This helps later with cell_ranges
- REFA: There are functionalities in LispFunctionExecutionContext derived classes which principally just say "I want a number or a function which returns a number" - introduce function which streamlines this process
- BUG: Add tests for function contexts
- BUG: LispValue does not support IDENTIFIERS - could be useful for variables
- Change numbers in UI to be zero based (helps with reference of cells)
- Add lists: (list 3 556 "Hello")
