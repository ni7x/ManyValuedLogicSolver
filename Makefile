PARSER = FormulaParser
EXEC = FormulaSolver
MAIN_FILE = src/main.cpp
GENERATED_DIR = src/generated_files
GENERATED_DIR_WIN = src\generated_files

SRC_DIR = src


all: $(GENERATED_DIR)/$(EXEC) 


$(GENERATED_DIR)/$(PARSER).tab.c $(GENERATED_DIR)/$(PARSER).tab.h: $(SRC_DIR)/flex_bison/$(PARSER).y
	mkdir  $(GENERATED_DIR_WIN)
	win_bison -t -v -d -o $(GENERATED_DIR)/$(PARSER).tab.c $(SRC_DIR)/flex_bison/$(PARSER).y

$(GENERATED_DIR)/$(PARSER).yy.c: $(SRC_DIR)/flex_bison/$(PARSER).l $(GENERATED_DIR)/$(PARSER).tab.h
	win_flex -o$(GENERATED_DIR)/$(PARSER).yy.c $(SRC_DIR)/flex_bison/$(PARSER).l

$(GENERATED_DIR)/$(EXEC): $(GENERATED_DIR)/$(PARSER).yy.c $(GENERATED_DIR)/$(PARSER).tab.c $(GENERATED_DIR)/$(PARSER).tab.h
	g++ -g -Ofast -o $(EXEC) $(GENERATED_DIR)/$(PARSER).tab.c $(GENERATED_DIR)/$(PARSER).yy.c $(SRC_DIR)/FormulaSolver.cpp $(SRC_DIR)/FormulaEvaluator.cpp ${MAIN_FILE} -I./$(SRC_DIR) -I./$(SRC_DIR)/flex_bison


clean:
	rm -rf $(GENERATED_DIR_WIN)

rebuild: clean all

run: rebuild
	./$(EXEC) 2 1

.PHONY: all clean rebuild
