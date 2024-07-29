BUILD ?= build

INCLUDE := include

CXXSTD := 17
CSTD := 17

toollib_BUILD ?= $(BUILD)/toollib

testware_BUILD ?= $(BUILD)/testware
testware_toollib_BUILD ?= $(testware_BUILD)/toollib

dirs = $(BUILD)
dirs_toollib = $(toollib_BUILD) $(toollib_BUILD)/ap.dir $(toollib_BUILD)/csv.dir

dirs_testware = $(testware_BUILD)
dirs_testware_toollib = $(testware_toollib_BUILD) $(testware_toollib_BUILD)/ap.dir $(testware_toollib_BUILD)/ap++.dir $(testware_toollib_BUILD)/ap++getAll.dir $(testware_toollib_BUILD)/csv.dir $(testware_toollib_BUILD)/csv++.dir

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

toollib: $(dirs_toollib) $(toollib_BUILD)/libap.a $(toollib_BUILD)/libcsv.a

.PHONY: all 
all: $(dirs) toollib

testware_toollib: $(dirs_testware_toollib) $(testware_toollib_BUILD)/ap $(testware_toollib_BUILD)/ap++ $(testware_toollib_BUILD)/ap++getAll $(testware_toollib_BUILD)/csv $(testware_toollib_BUILD)/csv++
testware: all $(dirs_testware) testware_toollib

WF := -Wall -Wextra -Wpedantic

clean:
	$(RM) -r $(BUILD)

$(dirs) $(dirs_toollib) $(dirs_testware) $(dirs_testware_toollib):
	mkdir -p $@

toollib_ap_SRC := $(wildcard toollib/ap/**/*.c toollib/ap/*.c)
toollib_ap_BIN := $(patsubst toollib/ap/%,$(toollib_BUILD)/ap.dir/%.o,$(toollib_ap_SRC))
$(toollib_BUILD)/libap.a: $(toollib_ap_BIN)
	$(AR) qc $@ $^

$(filter %.c.o,$(toollib_ap_BIN)): $(toollib_BUILD)/ap.dir/%.o: toollib/ap/%
	$(CC) -c -o $@ $^ -std=c$(CSTD) -I $(INCLUDE) -fPIC $(WF)

toollib_csv_SRC := $(wildcard toollib/csv/**/*.c toollib/csv/*.c)
toollib_csv_BIN := $(patsubst toollib/csv/%,$(toollib_BUILD)/csv.dir/%.o,$(toollib_csv_SRC))
$(toollib_BUILD)/libcsv.a: $(toollib_csv_BIN)
	$(AR) qc $@ $^

$(filter %.c.o,$(toollib_csv_BIN)): $(toollib_BUILD)/csv.dir/%.o: toollib/csv/%
	$(CC) -c -o $@ $^ -std=c$(CSTD) -I $(INCLUDE) -fPIC $(WF)

# Testware

testware_toollib_ap_SRC := $(wildcard testware/toollib/ap/**/*.c testware/toollib/ap/*.c)
testware_toollib_ap_BIN := $(patsubst testware/toollib/ap/%,$(testware_toollib_BUILD)/ap.dir/%.o,$(testware_toollib_ap_SRC))
$(testware_toollib_BUILD)/ap: $(testware_toollib_ap_BIN) $(toollib_BUILD)/libap.a
	$(CC) -o $@ $^ -std=c$(CSTD) -ggdb

$(filter %.c.o,$(testware_toollib_ap_BIN)): $(testware_BUILD)/toollib/ap.dir/%.o: testware/toollib/ap/%
	$(CC) -c -o $@ $^ -std=c$(CSTD) -I $(INCLUDE) -fPIE $(WF) -ggdb

testware_trsapxx_SRC := $(wildcard testware/toollib/ap++/**/*.cpp testware/toollib/ap++/*.cpp)
testware_trsapxx_BIN := $(patsubst testware/toollib/ap++/%,$(testware_toollib_BUILD)/ap++.dir/%.o,$(testware_trsapxx_SRC))
$(testware_toollib_BUILD)/ap++: $(testware_trsapxx_BIN) $(toollib_BUILD)/libap.a
	$(CXX) -o $@ $^ -std=c++$(CXXSTD) -ggdb

$(filter %.cpp.o,$(testware_trsapxx_BIN)): $(testware_toollib_BUILD)/ap++.dir/%.o: testware/toollib/ap++/%
	$(CXX) -c -o $@ $^ -std=c++$(CXXSTD) -I $(INCLUDE) -fPIE $(WF) -ggdb

testware_trsapxxgetAll_SRC := $(wildcard testware/toollib/ap++getAll/**/*.cpp testware/toollib/ap++getAll/*.cpp)
testware_trsapxxgetAll_BIN := $(patsubst testware/toollib/ap++getAll/%,$(testware_toollib_BUILD)/ap++getAll.dir/%.o,$(testware_trsapxxgetAll_SRC))
$(testware_toollib_BUILD)/ap++getAll: $(testware_trsapxxgetAll_BIN) $(toollib_BUILD)/libap.a
	$(CXX) -o $@ $^ -std=c++$(CXXSTD) -ggdb

$(filter %.cpp.o,$(testware_trsapxxgetAll_BIN)): $(testware_toollib_BUILD)/ap++getAll.dir/%.o: testware/toollib/ap++getAll/%
	$(CXX) -c -o $@ $^ -std=c++$(CXXSTD) -I $(INCLUDE) -fPIE $(WF) -ggdb

testware_toollib_csv_SRC := $(wildcard testware/toollib/csv/**/*.c testware/toollib/csv/*.c)
testware_toollib_csv_BIN := $(patsubst testware/toollib/csv/%,$(testware_toollib_BUILD)/csv.dir/%.o,$(testware_toollib_csv_SRC))
$(testware_toollib_BUILD)/csv: $(testware_toollib_csv_BIN) $(toollib_BUILD)/libcsv.a
	$(CC) -o $@ $^ -std=c$(CSTD) -ggdb

$(filter %.c.o,$(testware_toollib_csv_BIN)): $(testware_toollib_BUILD)/csv.dir/%.o: testware/toollib/csv/%
	$(CC) -c -o $@ $^ -std=c$(CSTD) -I $(INCLUDE) -fPIE $(WF) -ggdb

testware_toollib_csvxx_SRC := $(wildcard testware/toollib/csv++/**/*.c testware/toollib/csv++/*.cpp)
testware_toollib_csvxx_BIN := $(patsubst testware/toollib/csv++/%,$(testware_toollib_BUILD)/csv++.dir/%.o,$(testware_toollib_csvxx_SRC))
$(testware_toollib_BUILD)/csv++: $(testware_toollib_csvxx_BIN) $(toollib_BUILD)/libcsv.a
	$(CXX) -o $@ $^ -std=c++$(CXXSTD) -ggdb

$(filter %.cpp.o,$(testware_toollib_csvxx_BIN)): $(testware_toollib_BUILD)/csv++.dir/%.o: testware/toollib/csv++/%
	$(CXX) -c -o $@ $^ -std=c++$(CXXSTD) -I $(INCLUDE) -fPIE $(WF) -ggdb


