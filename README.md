# Kalkulator

This is a simple and small spreadsheet application which uses a Lisp-like language for cell formulas.

This is by no means bug free and / or production ready - **USE AT YOUR OWN RISK**.

I have implemented a similar application in C with ncurses before ("simplecalc"), but for usability reasons I wanted to create a GUI application now.

This is one item of the "applications every software engineer should implement once in their lifetime" list :)

# TODO

- Add lists: (list 3 556 "Hello")
- Complete minimal lisp interpreter: https://de.wikipedia.org/wiki/Lisp#Minimaler_Funktionsumfang_f%C3%BCr_Lisp
  - first
  - rest
  - cons
  - quote
  - eq
  - cond (if?)
  - lambda
- Cache function results
- Save (sqlite)
- Open (sqlite)
- Fix circular formula bug
- Support lists for mathematical operations
- Add cell_range function which returns lists of values of cells
- Refactoring: function execution contexts use the same pattern everytime when checking for function or numerical value. This can be cleaned up.

# DONE
