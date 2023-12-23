BUILD_DIR=./build

SRCDIR = ./
OBJDIR = $(BUILD_DIR)/obj
BINDIR = $(BUILD_DIR)/bin
EXE = $(BINDIR)/codeChallengeExe

all: build_dir $(EXE)

CC=g++ -std=c++14

.PHONY: build_dir clean run test help

help: 
	@echo "\n\n******* Help from step3 *******"
	@echo "make - to build main exe"
	@echo "make run - to run main exe\n\n"
	
run : all 
	@echo "\n\n******* Executing step3 (Code Challenge) *******"
	$(EXE)
	
test:
	@echo "\n\n******* Tests from step3 (Code Challenge) *******"
	@echo "No tests in directory step3"
	
clean:
	rm -rf $(OBJDIR) $(BINDIR)

build_dir:
	@[ -d $(OBJDIR) ] || mkdir -p $(OBJDIR) $(BINDIR)

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $(filter %.cpp,$^)


$(EXE): $(OBJDIR)/CodeChallenge.o
	$(CC) -o $@ $(filter %.o,$^)