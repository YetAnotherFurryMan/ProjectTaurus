BUILD ?= build

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
getsrc=$(patsubst $(BUILD)/%,%,$(patsubst $(word 1,$(subst .dir,.dir ,$1))/%.o,$(word 1,$(subst .dir, ,$1))/%,$1))

dirs := $(BUILD)/toollib $(BUILD)/toollib/ap.dir $(BUILD)/toollib/csv.dir

testware_dirs := $(BUILD)/testware/toollib

.PHONY: all
all: $(dirs) toollib

.PHONY: toollib
toollib: $(filter $(BUILD)/toollib/%, $(dirs)) $(BUILD)/toollib/libap.a $(BUILD)/toollib/libcsv.a

clean:
	$(RM) -r $(BUILD)

$(dirs) $(testware_dirs):
	mkdir -p $@

exebin := 
libbin := 

bin = $(patsubst toollib/ap/%,$(BUILD)/toollib/ap.dir/%.o,$(call rwildcard,toollib/ap,*.c *.cpp))
libbin += $(bin)
$(BUILD)/toollib/libap.a: $(bin)
	$(AR) qc $@ $^

$(BUILD)/toollib/ap.so: $(bin)
	$(CXX) -o $@ $^ -std=c++17 -Wall -Wextra -Wpedantic --shared

bin = $(patsubst toollib/csv/%,$(BUILD)/toollib/csv.dir/%.o,$(call rwildcard,toollib/csv,*.c *.cpp))
libbin += $(bin)
$(BUILD)/toollib/libcsv.a: $(bin)
	$(AR) qc $@ $^

$(BUILD)/toollib/csv.so: $(bin)
	$(CXX) -o $@ $^ -std=c++17 -Wall -Wextra -Wpedantic --shared

.SECONDEXPANSION:

$(filter %.c.o, $(libbin)): %: $$(call getsrc,%)
	$(CC) -c $^ -o $@ -std=c17 -Wall -Wextra -Wpedantic -Iinclude -fPIC

$(filter %.c.o, $(exebin)): %: $$(call getsrc,%)
	$(CC) -c $^ -o $@ -std=c17 -Wall -Wextra -Wpedantic -Iinclude -fPIE

$(filter %.cpp.o, $(libbin)): %: $$(call getsrc,%)
	$(CXX) -c $^ -o $@ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude -fPIC

$(filter %.cpp.o, $(exebin)): %: $$(call getsrc,%)
	$(CXX) -c $^ -o $@ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude -fPIE

.PHONY: testware
testware: all $(testware_dirs) $(BUILD)/testware/toollib/ap $(BUILD)/testware/toollib/ap++ $(BUILD)/testware/toollib/ap++getAll $(BUILD)/testware/toollib/csv $(BUILD)/testware/toollib/csv++

$(BUILD)/testware/toollib/ap: testware/toollib/ap.c
	$(CC) -o $@ $^ -std=c17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -l:toollib/libap.a

$(BUILD)/testware/toollib/ap++: testware/toollib/ap++.cpp
	$(CXX) -o $@ $^ -std=c++17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -l:toollib/libap.a

$(BUILD)/testware/toollib/ap++getAll: testware/toollib/ap++getAll.cpp
	$(CXX) -o $@ $^ -std=c++17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -l:toollib/libap.a

$(BUILD)/testware/toollib/csv: testware/toollib/csv.c
	$(CC) -o $@ $^ -std=c17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -l:toollib/libcsv.a

$(BUILD)/testware/toollib/csv++: testware/toollib/csv++.cpp
	$(CXX) -o $@ $^ -std=c++17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -l:toollib/libcsv.a

