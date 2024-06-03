EXEC = LogicParser
MAIN_FILE = main.cpp
GENERATED_DIR = generated_files

all: $(GENERATED_DIR)/$(EXEC) # build and run
	 $(GENERATED_DIR)/$(EXEC) 2 1

$(GENERATED_DIR)/$(EXEC).tab.c $(GENERATED_DIR)/$(EXEC).tab.h: $(EXEC).y
	mkdir $(GENERATED_DIR)
	win_bison -t -v -d -o $(GENERATED_DIR)/$(EXEC).tab.c $(EXEC).y

$(GENERATED_DIR)/$(EXEC).yy.c: $(EXEC).l $(GENERATED_DIR)/$(EXEC).tab.h
	win_flex -o$(GENERATED_DIR)/$(EXEC).yy.c $(EXEC).l

$(GENERATED_DIR)/$(EXEC): $(GENERATED_DIR)/$(EXEC).yy.c $(GENERATED_DIR)/$(EXEC).tab.c $(GENERATED_DIR)/$(EXEC).tab.h 
	g++ -o $(GENERATED_DIR)/$(EXEC)  $(GENERATED_DIR)/$(EXEC).tab.c $(GENERATED_DIR)/$(EXEC).yy.c FormulaSolver.cpp ${MAIN_FILE} -I.

clean:
	rm -rf $(GENERATED_DIR)

rebuild: clean all


