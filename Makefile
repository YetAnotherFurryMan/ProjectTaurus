BUILD ?= build

INCLUDE := include

CXXSTD := 17
CSTD := 17

dirs = $(BUILD) $(BUILD)/trsap.dir $(BUILD)/trsp.dir $(BUILD)/trsre $(BUILD)/trsc
dirs_testware = $(BUILD)/testware $(BUILD)/testware/trsap.dir $(BUILD)/testware/trsap++.dir $(BUILD)/testware/trsap++getAll.dir

.PHONY: all 
all: $(dirs) $(BUILD)/libtrsap.a
testware: all $(dirs_testware) $(BUILD)/testware/trsap $(BUILD)/testware/trsap++ $(BUILD)/testware/trsap++getAll

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

testware_trsap_src := $(wildcard testware/trsap/**/*.c testware/trsap/*.c)
testware_trsap_bin := $(patsubst testware/trsap/%,$(BUILD)/testware/trsap.dir/%.o,$(testware_trsap_src))
$(BUILD)/testware/trsap: $(testware_trsap_bin) $(BUILD)/libtrsap.a
	$(CC) -o $@ $^ -std=c$(CSTD)

$(filter %.c.o,$(testware_trsap_bin)): $(BUILD)/testware/trsap.dir/%.o: testware/trsap/%
	$(CC) -c -o $@ $^ -std=c$(CSTD) -I $(INCLUDE) -fPIE

testware_trsapxx_src := $(wildcard testware/trsap++/**/*.cpp testware/trsap++/*.cpp)
testware_trsapxx_bin := $(patsubst testware/trsap++/%,$(BUILD)/testware/trsap++.dir/%.o,$(testware_trsapxx_src))
$(BUILD)/testware/trsap++: $(testware_trsapxx_bin) $(BUILD)/libtrsap.a
	$(CXX) -o $@ $^ -std=c++$(CSTD)

$(filter %.cpp.o,$(testware_trsapxx_bin)): $(BUILD)/testware/trsap++.dir/%.o: testware/trsap++/%
	$(CXX) -c -o $@ $^ -std=c++$(CSTD) -I $(INCLUDE) -fPIE

testware_trsapxxgetAll_src := $(wildcard testware/trsap++getAll/**/*.cpp testware/trsap++getAll/*.cpp)
testware_trsapxxgetAll_bin := $(patsubst testware/trsap++getAll/%,$(BUILD)/testware/trsap++getAll.dir/%.o,$(testware_trsapxxgetAll_src))
$(BUILD)/testware/trsap++getAll: $(testware_trsapxxgetAll_bin) $(BUILD)/libtrsap.a
	$(CXX) -o $@ $^ -std=c++$(CSTD)

$(filter %.cpp.o,$(testware_trsapxxgetAll_bin)): $(BUILD)/testware/trsap++getAll.dir/%.o: testware/trsap++getAll/%
	$(CXX) -c -o $@ $^ -std=c++$(CSTD) -I $(INCLUDE) -fPIE

