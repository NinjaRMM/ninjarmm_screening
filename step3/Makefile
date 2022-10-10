.DEFAULT_GOAL:=all

PROJ_NAME   = step3

OUTPUT_DIR 	= output/
PLATFORMS 	= Linux Windows

PLATFORM 	?= Linux

.NOTPARALLEL:
.PHONY: all clean $(PLATFORMS)
all: $(PLATFORMS)

$(PLATFORMS):
	@$(MAKE) -B --no-print-directory -C src -f $@.mk clean all
	@mkdir -p $(OUTPUT_DIR)$@/
	@rm -rf $(OUTPUT_DIR)$@/*
	@echo "Call $@_copy"
	@$(MAKE) --no-print-directory $@_copy PLATFORM=$@

Linux_copy:
	@echo "Coping to $(OUTPUT_DIR)$(PLATFORM)"
	@cp src/app$(PROJ_NAME) $(OUTPUT_DIR)$(PLATFORM)

Windows_copy:
	@echo "Coping to $(OUTPUT_DIR)$(PLATFORM)"
	@cp src/app$(PROJ_NAME).exe $(OUTPUT_DIR)$(PLATFORM)

clean:
	@$(MAKE) -C src -f Linux.mk clean
	@$(MAKE) -C src -f Windows.mk clean
	@rm -rf $(OUTPUT_DIR)
