CC=clang
CXX=clang++
RM=rm -f

SRC_DIR=src
TESTS_DIR=src/tests
TARGET_DIR=bin
TOOLS_DIR=src/tools

WX_DIR=vendor/wxWidgets-3.2.2.1
# used "vendor/wxWidgets-3.2.2.1/buildgtk/wx-config --cppflags"
WX_INCLUDES=-I$(WX_DIR)/buildgtk/lib/wx/include/gtk3-unicode-3.2 -I$(WX_DIR)/include
WX_DEFINES=-D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXGTK__
# used "vendor/wxWidgets-3.2.2.1/buildgtk/wx-config --libs"
WX_LDFLAGS=-L$(WX_DIR)/buildgtk/lib -pthread -Wl,-rpath,$(WX_DIR)/buildgtk/lib -lwx_gtk3u_xrc-3.2 -lwx_gtk3u_html-3.2 -lwx_gtk3u_qa-3.2 -lwx_gtk3u_core-3.2 -lwx_baseu_xml-3.2 -lwx_baseu_net-3.2 -lwx_baseu-3.2

#CPPFLAGS=-std=c++17 -O3 -Wall -Wpedantic -Wextra -Weffc++ -I$(TOOLS_DIR) -I$(TESTS_DIR) $(WX_INCLUDES) $(WX_DEFINES)
CPPFLAGS=-std=c++17 -g -Wall -Wpedantic -Wextra -Weffc++ -I$(TOOLS_DIR) -I$(TESTS_DIR) $(WX_INCLUDES) $(WX_DEFINES)
LDFLAGS=$(WX_LDFLAGS)
LDLIBS=

SRCS=$(SRC_DIR)/main.cpp
SRCS+=$(SRC_DIR)/view/table_control.cpp
SRCS+=$(SRC_DIR)/view/table_formula_text_control.cpp
SRCS+=$(SRC_DIR)/model/table/table_sheet.cpp
SRCS+=$(SRC_DIR)/model/table/table_cell.cpp
SRCS+=$(SRC_DIR)/model/table/table_cell_formula.cpp
SRCS+=$(SRC_DIR)/model/table/table_workbook_document.cpp
SRCS+=$(SRC_DIR)/model/table/table_column_definition.cpp
SRCS+=$(SRC_DIR)/model/table/table_row_definition.cpp
SRCS+=$(SRC_DIR)/model/lisp/lisp_parser.cpp
SRCS+=$(SRC_DIR)/model/lisp/lisp_tokens.cpp
SRCS+=$(TOOLS_DIR)/tools.cpp
SRCS+=$(TESTS_DIR)/tests.cpp
SRCS+=$(TESTS_DIR)/lisp/lisp_tests.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

TARGET=$(TARGET_DIR)/kalkulator

.PHONY: all
all: $(TARGET)

.PHONY: run
run: $(TARGET)
	sh -c "$(TARGET)"

# .PHONY: test
# test: $(TARGET)
# 	sh -c "$(TARGET) test"

.PHONY: unittests
unittests: $(TARGET)
	sh -c "$(TARGET) unittests"

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(SRC_DIR)/tests.o: $(TESTS_DIR)/tests.cpp $(TESTS_DIR)/tests.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(OBJS)
	$(RM) $(TARGET)

.PHONY: checkleak
checkleak: $(TARGET)
	valgrind --leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--verbose \
			--log-file=valgrind-out.txt \
			$(TARGET) test

.PHONY: callgrind
callgrind: $(TARGET)
	valgrind --tool=callgrind $(TARGET) test

.PHONY: debug
debug: $(TARGET)
	ddd $(TARGET)
