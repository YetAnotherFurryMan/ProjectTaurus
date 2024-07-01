BUILD ?= build

INCLUDE := include

CXXSTD := 17
CSTD := 17

TBUILD ?= $(BUILD)/testware

dirs = $(BUILD) $(BUILD)/trsap.dir $(BUILD)/csv.dir $(BUILD)/trsp.dir $(BUILD)/trsp.dir/subprogs $(BUILD)/trsre.dir $(BUILD)/trsc.dir
dirs_testware = $(TBUILD) $(TBUILD)/trsap.dir $(TBUILD)/trsap++.dir $(TBUILD)/trsap++getAll.dir $(TBUILD)/csv.dir $(TBUILD)/csv++.dir $(TBUILD)/trsre_token.dir

.PHONY: all 
all: $(dirs) $(BUILD)/libtrsap.a $(BUILD)/libcsv.a $(BUILD)/libtrsre.a $(BUILD)/trsp
testware: all $(dirs_testware) $(TBUILD)/trsap $(TBUILD)/trsap++ $(TBUILD)/trsap++getAll $(TBUILD)/csv $(TBUILD)/csv++ $(TBUILD)/trsre_token

WF := -Wall -Wextra -Wpedantic

clean:
	$(RM) -r $(BUILD)

$(dirs) $(dirs_testware):
	mkdir -p $@

trsap_src := $(wildcard trsap/**/*.c trsap/*.c)
trsap_bin := $(patsubst trsap/%,$(BUILD)/trsap.dir/%.o,$(trsap_src))
$(BUILD)/libtrsap.a: $(trsap_bin)
	$(AR) qc $@ $^

$(filter %.c.o,$(trsap_bin)): $(BUILD)/trsap.dir/%.o: trsap/%
	$(CC) -c -o $@ $^ -std=c$(CSTD) -I $(INCLUDE) -fPIC $(WF)

csv_src := $(wildcard csv/**/*.c csv/*.c)
csv_bin := $(patsubst csv/%,$(BUILD)/csv.dir/%.o,$(csv_src))
$(BUILD)/libcsv.a: $(csv_bin)
	$(AR) qc $@ $^

$(filter %.c.o,$(csv_bin)): $(BUILD)/csv.dir/%.o: csv/%
	$(CC) -c -o $@ $^ -std=c$(CSTD) -I $(INCLUDE) -fPIC $(WF)

trsre_src := $(wildcard trsre/**/*.c trsre/*.c)
trsre_bin := $(patsubst trsre/%,$(BUILD)/trsre.dir/%.o,$(trsre_src))
$(BUILD)/libtrsre.a: $(trsre_bin)
	$(AR) qc $@ $^

$(filter %.c.o,$(trsre_bin)): $(BUILD)/trsre.dir/%.o: trsre/%
	$(CC) -c -o $@ $^ -std=c$(CSTD) -I $(INCLUDE) -fPIC $(WF)

trsp_src := $(wildcard trsp/**/*.cpp trsp/*.cpp)
trsp_bin := $(patsubst trsp/%,$(BUILD)/trsp.dir/%.o,$(trsp_src))
$(BUILD)/trsp: $(trsp_bin) $(BUILD)/libtrsap.a $(BUILD)/libcsv.a
	$(CXX) -o $@ $^ -std=c++$(CXXSTD)

$(filter %.cpp.o,$(trsp_bin)): $(BUILD)/trsp.dir/%.o: trsp/%
	$(CXX) -c -o $@ $^ -std=c++$(CXXSTD) -I $(INCLUDE) $(WF)

# Testware

testware_trsap_src := $(wildcard testware/trsap/**/*.c testware/trsap/*.c)
testware_trsap_bin := $(patsubst testware/trsap/%,$(TBUILD)/trsap.dir/%.o,$(testware_trsap_src))
$(TBUILD)/trsap: $(testware_trsap_bin) $(BUILD)/libtrsap.a
	$(CC) -o $@ $^ -std=c$(CSTD) -ggdb

$(filter %.c.o,$(testware_trsap_bin)): $(TBUILD)/trsap.dir/%.o: testware/trsap/%
	$(CC) -c -o $@ $^ -std=c$(CSTD) -I $(INCLUDE) -fPIE $(WF) -ggdb

testware_trsapxx_src := $(wildcard testware/trsap++/**/*.cpp testware/trsap++/*.cpp)
testware_trsapxx_bin := $(patsubst testware/trsap++/%,$(TBUILD)/trsap++.dir/%.o,$(testware_trsapxx_src))
$(TBUILD)/trsap++: $(testware_trsapxx_bin) $(BUILD)/libtrsap.a
	$(CXX) -o $@ $^ -std=c++$(CSTD) -ggdb

$(filter %.cpp.o,$(testware_trsapxx_bin)): $(TBUILD)/trsap++.dir/%.o: testware/trsap++/%
	$(CXX) -c -o $@ $^ -std=c++$(CSTD) -I $(INCLUDE) -fPIE $(WF) -ggdb

testware_trsapxxgetAll_src := $(wildcard testware/trsap++getAll/**/*.cpp testware/trsap++getAll/*.cpp)
testware_trsapxxgetAll_bin := $(patsubst testware/trsap++getAll/%,$(TBUILD)/trsap++getAll.dir/%.o,$(testware_trsapxxgetAll_src))
$(TBUILD)/trsap++getAll: $(testware_trsapxxgetAll_bin) $(BUILD)/libtrsap.a
	$(CXX) -o $@ $^ -std=c++$(CSTD) -ggdb

$(filter %.cpp.o,$(testware_trsapxxgetAll_bin)): $(TBUILD)/trsap++getAll.dir/%.o: testware/trsap++getAll/%
	$(CXX) -c -o $@ $^ -std=c++$(CSTD) -I $(INCLUDE) -fPIE $(WF) -ggdb

testware_csv_src := $(wildcard testware/csv/**/*.c testware/csv/*.c)
testware_csv_bin := $(patsubst testware/csv/%,$(TBUILD)/csv.dir/%.o,$(testware_csv_src))
$(TBUILD)/csv: $(testware_csv_bin) $(BUILD)/libcsv.a
	$(CC) -o $@ $^ -std=c$(CSTD) -ggdb

$(filter %.c.o,$(testware_csv_bin)): $(TBUILD)/csv.dir/%.o: testware/csv/%
	$(CC) -c -o $@ $^ -std=c$(CSTD) -I $(INCLUDE) -fPIE $(WF) -ggdb

$(TBUILD)/csv++:
	echo Not implemented yet: $@

testware_trsre_token_src := $(wildcard testware/trsre_token/**/*.c testware/trsre_token/*.c)
testware_trsre_token_bin := $(patsubst testware/trsre_token/%,$(TBUILD)/trsre_token.dir/%.o,$(testware_trsre_token_src))
$(TBUILD)/trsre_token: $(testware_trsre_token_bin) $(BUILD)/libtrsre.a
	$(CC) -o $@ $^ -std=c$(CSTD) -ggdb

$(filter %.c.o,$(testware_trsre_token_bin)): $(TBUILD)/trsre_token.dir/%.o: testware/trsre_token/%
	$(CC) -c -o $@ $^ -std=c$(CSTD) -I $(INCLUDE) -fPIE $(WF) -ggdb


