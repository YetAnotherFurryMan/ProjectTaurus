BUILD ?= build

INCLUDE := include

CXXSTD := 17
CSTD := 17

TBUILD ?= $(BUILD)/testware

dirs = $(BUILD) $(BUILD)/trsap.dir $(BUILD)/csv.dir $(BUILD)/trsp.dir $(BUILD)/trsre $(BUILD)/trsc
dirs_testware = $(TBUILD) $(TBUILD)/trsap.dir $(TBUILD)/trsap++.dir $(TBUILD)/trsap++getAll.dir $(TBUILD)/csv_parseRow.dir

.PHONY: all 
all: $(dirs) $(BUILD)/libtrsap.a $(BUILD)/libcsv.a
testware: all $(dirs_testware) $(TBUILD)/trsap $(TBUILD)/trsap++ $(TBUILD)/trsap++getAll $(TBUILD)/csv_parseRow

clean:
	$(RM) -r $(BUILD)

$(dirs) $(dirs_testware):
	mkdir -p $@

trsap_src := $(wildcard trsap/**/*.c trsap/*.c)
trsap_bin := $(patsubst trsap/%,$(BUILD)/trsap.dir/%.o,$(trsap_src))
$(BUILD)/libtrsap.a: $(trsap_bin)
	$(AR) qc $@ $^

$(filter %.c.o,$(trsap_bin)): $(BUILD)/trsap.dir/%.o: trsap/%
	$(CC) -c -o $@ $^ -std=c$(CSTD) -I $(INCLUDE) -fPIC

csv_src := $(wildcard csv/**/*.c csv/*.c)
csv_bin := $(patsubst csv/%,$(BUILD)/csv.dir/%.o,$(csv_src))
$(BUILD)/libcsv.a: $(csv_bin)
	$(AR) qc $@ $^

$(filter %.c.o,$(csv_bin)): $(BUILD)/csv.dir/%.o: csv/%
	$(CC) -c -o $@ $^ -std=c$(CSTD) -I $(INCLUDE) -fPIC

testware_trsap_src := $(wildcard testware/trsap/**/*.c testware/trsap/*.c)
testware_trsap_bin := $(patsubst testware/trsap/%,$(TBUILD)/trsap.dir/%.o,$(testware_trsap_src))
$(TBUILD)/trsap: $(testware_trsap_bin) $(BUILD)/libtrsap.a
	$(CC) -o $@ $^ -std=c$(CSTD)

$(filter %.c.o,$(testware_trsap_bin)): $(TBUILD)/trsap.dir/%.o: testware/trsap/%
	$(CC) -c -o $@ $^ -std=c$(CSTD) -I $(INCLUDE) -fPIE

testware_trsapxx_src := $(wildcard testware/trsap++/**/*.cpp testware/trsap++/*.cpp)
testware_trsapxx_bin := $(patsubst testware/trsap++/%,$(TBUILD)/trsap++.dir/%.o,$(testware_trsapxx_src))
$(TBUILD)/trsap++: $(testware_trsapxx_bin) $(BUILD)/libtrsap.a
	$(CXX) -o $@ $^ -std=c++$(CSTD)

$(filter %.cpp.o,$(testware_trsapxx_bin)): $(TBUILD)/trsap++.dir/%.o: testware/trsap++/%
	$(CXX) -c -o $@ $^ -std=c++$(CSTD) -I $(INCLUDE) -fPIE

testware_trsapxxgetAll_src := $(wildcard testware/trsap++getAll/**/*.cpp testware/trsap++getAll/*.cpp)
testware_trsapxxgetAll_bin := $(patsubst testware/trsap++getAll/%,$(TBUILD)/trsap++getAll.dir/%.o,$(testware_trsapxxgetAll_src))
$(TBUILD)/trsap++getAll: $(testware_trsapxxgetAll_bin) $(BUILD)/libtrsap.a
	$(CXX) -o $@ $^ -std=c++$(CSTD)

$(filter %.cpp.o,$(testware_trsapxxgetAll_bin)): $(TBUILD)/trsap++getAll.dir/%.o: testware/trsap++getAll/%
	$(CXX) -c -o $@ $^ -std=c++$(CSTD) -I $(INCLUDE) -fPIE

testware_csv_parseRow_src := $(wildcard testware/csv_parseRow/**/*.c testware/csv_parseRow/*.c)
testware_csv_parseRow_bin := $(patsubst testware/csv_parseRow/%,$(TBUILD)/csv_parseRow.dir/%.o,$(testware_csv_parseRow_src))
$(TBUILD)/csv_parseRow: $(testware_csv_parseRow_bin) $(BUILD)/libcsv.a
	$(CC) -o $@ $^ -std=c$(CSTD)

$(filter %.c.o,$(testware_csv_parseRow_bin)): $(TBUILD)/csv_parseRow.dir/%.o: testware/csv_parseRow/%
	$(CC) -c -o $@ $^ -std=c$(CSTD) -I $(INCLUDE) -fPIE

