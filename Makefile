# Variables
EXEC = LogicParser
MAIN_FILE = src/main.cpp
GENERATED_DIR = src/generated_files
GENERATED_DIR_WIN = src\generated_files
SRC_DIR = src


all: $(GENERATED_DIR)/$(EXEC) 


$(GENERATED_DIR)/$(EXEC).tab.c $(GENERATED_DIR)/$(EXEC).tab.h: $(SRC_DIR)/$(EXEC).y
	mkdir  $(GENERATED_DIR_WIN)
	win_bison -t -v -d -o $(GENERATED_DIR)/$(EXEC).tab.c $(SRC_DIR)/$(EXEC).y

$(GENERATED_DIR)/$(EXEC).yy.c: $(SRC_DIR)/$(EXEC).l $(GENERATED_DIR)/$(EXEC).tab.h
	win_flex -o$(GENERATED_DIR)/$(EXEC).yy.c $(SRC_DIR)/$(EXEC).l

$(GENERATED_DIR)/$(EXEC): $(GENERATED_DIR)/$(EXEC).yy.c $(GENERATED_DIR)/$(EXEC).tab.c $(GENERATED_DIR)/$(EXEC).tab.h 
	g++ -o $(EXEC) $(GENERATED_DIR)/$(EXEC).tab.c $(GENERATED_DIR)/$(EXEC).yy.c $(SRC_DIR)/FormulaSolver.cpp ${MAIN_FILE} -I./$(SRC_DIR)

clean:
	rm -rf $(GENERATED_DIR_WIN)

rebuild: clean all

run: rebuild
	./$(EXEC) 5 2

.PHONY: all clean rebuild
