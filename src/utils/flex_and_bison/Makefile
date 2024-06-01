EXEC = logic_parser
MAIN_FILE = main.c
GENERATED_DIR = generated_files

all: $(GENERATED_DIR)/$(EXEC)	# Build .exe and run tests
	 $(GENERATED_DIR)/$(EXEC) tests/test_evaluation_input.txt tests/test_ouput.txt

$(GENERATED_DIR)/$(EXEC).tab.c $(GENERATED_DIR)/$(EXEC).tab.h: $(EXEC).y
	mkdir $(GENERATED_DIR)
	bison -t -v -d -o $(GENERATED_DIR)/$(EXEC).tab.c $(EXEC).y

$(GENERATED_DIR)/$(EXEC).yy.c: $(EXEC).l $(GENERATED_DIR)/$(EXEC).tab.h
	flex -o$(GENERATED_DIR)/$(EXEC).yy.c $(EXEC).l

$(GENERATED_DIR)/$(EXEC): $(GENERATED_DIR)/$(EXEC).yy.c $(GENERATED_DIR)/$(EXEC).tab.c $(GENERATED_DIR)/$(EXEC).tab.h $(MAIN_FILE)
	gcc -o $(GENERATED_DIR)/$(EXEC)  $(GENERATED_DIR)/$(EXEC).tab.c $(GENERATED_DIR)/$(EXEC).yy.c $(MAIN_FILE)

clean:
	rm -rf $(GENERATED_DIR)

rebuild: clean all