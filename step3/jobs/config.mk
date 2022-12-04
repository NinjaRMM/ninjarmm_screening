IPATH += -I./jobs

JOBS_OBJ := $(shell find jobs/ -name "*.cpp")
OBJ += $(JOBS_OBJ:.cpp=.o)
