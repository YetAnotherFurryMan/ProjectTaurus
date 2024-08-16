BUILD ?= build

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
getsrc=$(patsubst $(BUILD)/%,%,$(patsubst $(word 1,$(subst .dir,.dir ,$1))/%.o,$(word 1,$(subst .dir, ,$1))/%,$1))

dirs := $(BUILD)/toollib $(BUILD)/toollib/ap.dir $(BUILD)/toollib/csv.dir $(BUILD)/toollib/cvec.dir

testware_dirs := $(BUILD)/testware/toollib

.PHONY: all
all: $(dirs) toollib

.PHONY: toollib
toollib: $(filter $(BUILD)/toollib/%, $(dirs)) $(BUILD)/toollib/libap.a $(if $(RELEASE),$(BUILD)/toollib/ap.so,) $(BUILD)/toollib/libcsv.a $(if $(RELEASE),$(BUILD)/toollib/csv.so,) $(BUILD)/toollib/libcvec.a $(if $(RELEASE),$(BUILD)/toollib/cvec.so,)

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
	$(CXX) -o $@ $^ -std=c++17 -Wall -Wextra -Wpedantic --shared $(if $(DEBUG),-ggdb,)

bin = $(patsubst toollib/csv/%,$(BUILD)/toollib/csv.dir/%.o,$(call rwildcard,toollib/csv,*.c *.cpp))
libbin += $(bin)
$(BUILD)/toollib/libcsv.a: $(bin)
	$(AR) qc $@ $^

$(BUILD)/toollib/csv.so: $(bin)
	$(CXX) -o $@ $^ -std=c++17 -Wall -Wextra -Wpedantic --shared $(if $(DEBUG),-ggdb,)

bin = $(patsubst toollib/cvec/%,$(BUILD)/toollib/cvec.dir/%.o,$(call rwildcard,toollib/cvec,*.c *.cpp))
libbin += $(bin)
$(BUILD)/toollib/libcvec.a: $(bin)
	$(AR) qc $@ $^

$(BUILD)/toollib/cvec.so: $(bin)
	$(CXX) -o $@ $^ -std=c++17 -Wall -Wextra -Wpedantic --shared $(if $(DEBUG),-ggdb,)

.SECONDEXPANSION:

$(filter %.c.o, $(libbin)): %: $$(call getsrc,%)
	$(CC) -c $^ -o $@ -std=c17 -Wall -Wextra -Wpedantic -Iinclude -fPIC $(if $(RELEASE),-O3 -DNODEBUG -DRELEASE,) $(if $(DEBUG),-ggdb -DDEBUG,)

$(filter %.c.o, $(exebin)): %: $$(call getsrc,%)
	$(CC) -c $^ -o $@ -std=c17 -Wall -Wextra -Wpedantic -Iinclude -fPIE $(if $(RELEASE),-O3 -DNODEBUG -DRELEASE,) $(if $(DEBUG),-ggdb -DDEBUG,)

$(filter %.cpp.o, $(libbin)): %: $$(call getsrc,%)
	$(CXX) -c $^ -o $@ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude -fPIC $(if $(RELEASE),-O3 -DNODEBUG -DRELEASE,) $(if $(DEBUG),-ggdb -DDEBUG,)

$(filter %.cpp.o, $(exebin)): %: $$(call getsrc,%)
	$(CXX) -c $^ -o $@ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude -fPIE $(if $(RELEASE),-O3 -DNODEBUG -DRELEASE,) $(if $(DEBUG),-ggdb -DDEBUG,)

.PHONY: testware
testware: all $(testware_dirs) $(BUILD)/testware/toollib/ap $(BUILD)/testware/toollib/ap++ $(BUILD)/testware/toollib/ap++getAll $(BUILD)/testware/toollib/csv $(BUILD)/testware/toollib/csv++ $(BUILD)/testware/toollib/cvec

$(BUILD)/testware/toollib/ap: testware/toollib/ap.c $(BUILD)/toollib/libap.a
	$(CC) -o $@ $^ -std=c17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb 

$(BUILD)/testware/toollib/ap++: testware/toollib/ap++.cpp $(BUILD)/toollib/libap.a
	$(CXX) -o $@ $^ -std=c++17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb 

$(BUILD)/testware/toollib/ap++getAll: testware/toollib/ap++getAll.cpp $(BUILD)/toollib/libap.a
	$(CXX) -o $@ $^ -std=c++17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb 

$(BUILD)/testware/toollib/csv: testware/toollib/csv.c $(BUILD)/toollib/libcsv.a
	$(CC) -o $@ $^ -std=c17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb 

$(BUILD)/testware/toollib/csv++: testware/toollib/csv++.cpp $(BUILD)/toollib/libcsv.a
	$(CXX) -o $@ $^ -std=c++17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb 

$(BUILD)/testware/toollib/cvec: testware/toollib/cvec.c $(BUILD)/toollib/libcvec.a
	$(CC) -o $@ $^ -std=c17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb 

