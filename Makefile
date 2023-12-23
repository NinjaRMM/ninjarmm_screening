
.PHONY: clean

SUBDIRS := step1 step3

.PHONY: $(SUBDIRS) clean

$(SUBDIRS): 
	make -C $@ $(MAKECMDGOALS)

clean: $(SUBDIRS)
	
help: $(SUBDIRS) 		
	
test: $(SUBDIRS) 

all: $(SUBDIRS) 	
	
run : $(SUBDIRS) 	