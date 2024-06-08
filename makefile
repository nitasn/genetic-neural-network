SRC_DIR := src
OUT_DIR := build

OBJ_DIR := $(OUT_DIR)/.o
DEP_DIR := $(OUT_DIR)/.d

EXECUTABLE := $(OUT_DIR)/main

CXX := clang
CXXFLAGS := -std=c++20 -Wall -Ofast -MMD -MP -I./$(SRC_DIR)


#################################################################
###                 P H O N Y   T A R G E T S                 ###
#################################################################


PHONY_TARGETS := executable run time clean help

.PHONY: $(PHONY_TARGETS)

executable: $(EXECUTABLE)

args := # can be passed from command line, e.g. make run args="one two three"

run: executable
	@echo "running $(EXECUTABLE) $(args)"
	@$(EXECUTABLE) $(args)

time: executable
	@echo "timing $(EXECUTABLE) $(args)"
	@time $(EXECUTABLE) $(args)

clean:
	rm -rf $(OBJ_DIR) $(DEP_DIR) $(EXECUTABLE)

help:
	@echo "Available targets:"
	@echo "  executable  - Build the executable."
	@echo "  run         - Run the executable."
	@echo "  time        - Time the executable."
	@echo "  clean       - Remove all build files."
	@echo "  help        - Show this help message."


#################################################################
###                  A C T U A L   B U I L D                  ###
#################################################################


# recursively find all .cpp files in the SRC_DIR
SRC_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
DEP_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(DEP_DIR)/%.d,$(SRC_FILES))

-include $(DEP_FILES)

$(EXECUTABLE): $(OBJ_FILES)
	@mkdir -p $(OUT_DIR)
	@echo "linking $@"
	@$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@) $(dir $(DEP_DIR)/$*.d)
	@echo "compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@mv $(OBJ_DIR)/$*.d $(DEP_DIR)/$*.d