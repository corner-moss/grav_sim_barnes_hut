BUILD_DIR := ./build
SRC_DIRS := ./src

SRCS := $(shell find $(SRC_DIRS) -name '*.c' | grep -v direct_sum.c | grep -v barnes_hut.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

CC := clang

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
INC_FLAGS += -Ilib/ -Ilib/glad/include/ -Ilib/glfw/include/

CFLAGS :=-std=c11 -O3 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CFLAGS += $(INC_FLAGS)

FRAMEWORKS := -framework Cocoa -framework OpenGL -framework IOKit #-framework CoreVideo
LDFLAGS := lib/libvecmath/libvecmath.a lib/glfw/src/libglfw3.a lib/glad/src/glad.o 
LDFLAGS += $(FRAMEWORKS)

.PHONY = all clean

all: libs direct_sum barnes_hut

direct_sum: $(BUILD_DIR)/direct_sum
	@echo "Make sure libs are up to date"
	@echo $(SRCS)

barnes_hut: $(BUILD_DIR)/barnes_hut
	@echo "Make sure libs are up to date"

libs:
	cd lib/libvecmath && make
	cd lib/glfw && cmake . && make
	cd lib/glad && $(CC) -o src/glad.o -Iinclude -c src/glad.c

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC)  $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/direct_sum: $(BUILD_DIR)/src/direct_sum.c.o $(OBJS)
	$(CC) $(BUILD_DIR)/src/direct_sum.c.o $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/barnes_hut: $(BUILD_DIR)/src/barnes_hut.c.o $(OBJS)
	$(CC) $(BUILD_DIR)/src/barnes_hut.c.o $(OBJS) -o $@ $(LDFLAGS)

clean:
	rm -r $(BUILD_DIR)/*
