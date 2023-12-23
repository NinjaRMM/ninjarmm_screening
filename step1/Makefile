BUILD_DIR=./build

SRCDIR = src
OBJDIR = $(BUILD_DIR)/obj
BINDIR = $(BUILD_DIR)/bin
EXE = $(BINDIR)/step1Exe
UNIT_TEST_EXE = $(BINDIR)/unit_test

all: build_dir $(EXE) $(UNIT_TEST_EXE)

CC=g++ -std=c++14


.PHONY: build_dir clean run unit_tests help

help: 
	@echo "\n\n******* Help from step1 *******"
	@echo "make - to build main exe and unit tests"
	@echo "make run - to run main exe"
	@echo "make test - to run unit test\n\n"
	
run : all 
	@echo "\n\n******* Executing binary $(EXE) from step1 *******"
	$(EXE)
	
	

test: all
	@echo "\n\n******* Tests from step1 *******"
	$(UNIT_TEST_EXE)	

	
$(UNIT_TEST_EXE): $(OBJDIR)/unitTest.o $(OBJDIR)/Pilot.o $(OBJDIR)/Programmer.o $(OBJDIR)/Job.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -lgtest -o $@ $(filter %.o,$^)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

build_dir:
	@[ -d $(OBJDIR) ] || mkdir -p $(OBJDIR) $(BINDIR)

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $(filter %.cpp,$^)


$(EXE): $(OBJDIR)/Pilot.o $(OBJDIR)/Programmer.o $(OBJDIR)/step1Main.o $(OBJDIR)/Job.o $(OBJDIR)/DemoCode.o
	$(CC) -o $@ $(filter %.o,$^)