BUILD ?= build

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
getsrc=$(patsubst $(BUILD)/obj/%.o,src/%,$1)

dirs := $(BUILD)/obj $(BUILD)/bin $(BUILD)/lib $(BUILD)/test $(BUILD)/obj/toollib/ap $(BUILD)/obj/toollib/csv $(BUILD)/obj/toollib/vec $(BUILD)/obj/toollib/assoc $(BUILD)/obj/toollib/pgm $(BUILD)/obj/trs/horn $(BUILD)/obj/trs/trs.cg.nasm_x86 $(BUILD)/obj/trs/trs.cg.lisp $(BUILD)/obj/trs/trsc

.PHONY: clean all test

all: $(dirs) $(BUILD)/lib/libap.a $(if $(RELEASE),$(BUILD)/lib/ap.so,) $(BUILD)/lib/libcsv.a $(if $(RELEASE),$(BUILD)/lib/csv.so,) $(BUILD)/lib/libvec.a $(if $(RELEASE),$(BUILD)/lib/vec.so,) $(BUILD)/lib/libassoc.a $(if $(RELEASE),$(BUILD)/lib/assoc.so,) $(BUILD)/lib/libpgm.a $(if $(RELEASE),$(BUILD)/lib/pgm.so,) $(BUILD)/lib/libhorn.a $(if $(RELEASE),$(BUILD)/lib/horn.so,) $(BUILD)/lib/libtrs.cg.nasm_x86.a $(if $(RELEASE),$(BUILD)/lib/trs.cg.nasm_x86.so,) $(BUILD)/lib/libtrs.cg.lisp.a $(if $(RELEASE),$(BUILD)/lib/trs.cg.lisp.so,) $(BUILD)/bin/trsc

clean:
	$(RM) -r $(BUILD)

$(dirs):
	mkdir -p $@

exebin := 
libbin := 

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/toollib/ap, *.c))
libbin += $(bin)
$(BUILD)/lib/libap.a: $(bin)
	$(AR) qc $@ $^

$(BUILD)/lib/ap.so: $(bin)
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic --shared $(if $(DEBUG),-ggdb,)

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/toollib/csv, *.c))
libbin += $(bin)
$(BUILD)/lib/libcsv.a: $(bin)
	$(AR) qc $@ $^

$(BUILD)/lib/csv.so: $(bin)
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic --shared $(if $(DEBUG),-ggdb,)

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/toollib/vec, *.c))
libbin += $(bin)
$(BUILD)/lib/libvec.a: $(bin)
	$(AR) qc $@ $^

$(BUILD)/lib/vec.so: $(bin)
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic --shared $(if $(DEBUG),-ggdb,)

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/toollib/assoc, *.c))
libbin += $(bin)
$(BUILD)/lib/libassoc.a: $(bin)
	$(AR) qc $@ $^

$(BUILD)/lib/assoc.so: $(bin)
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic --shared $(if $(DEBUG),-ggdb,)

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/toollib/pgm, *.c))
libbin += $(bin)
$(BUILD)/lib/libpgm.a: $(bin)
	$(AR) qc $@ $^

$(BUILD)/lib/pgm.so: $(bin)
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic --shared $(if $(DEBUG),-ggdb,)

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/trs/horn, *.c))
libbin += $(bin)
$(BUILD)/lib/libhorn.a: $(bin)
	$(AR) qc $@ $^

$(BUILD)/lib/horn.so: $(bin)
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic --shared $(if $(DEBUG),-ggdb,)

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/trs/trs.cg.nasm_x86, *.c))
libbin += $(bin)
$(BUILD)/lib/libtrs.cg.nasm_x86.a: $(bin)
	$(AR) qc $@ $^

$(BUILD)/lib/trs.cg.nasm_x86.so: $(bin)
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic --shared $(if $(DEBUG),-ggdb,)

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/trs/trs.cg.lisp, *.c))
libbin += $(bin)
$(BUILD)/lib/libtrs.cg.lisp.a: $(bin)
	$(AR) qc $@ $^

$(BUILD)/lib/trs.cg.lisp.so: $(bin)
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic --shared $(if $(DEBUG),-ggdb,)

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/trs/trsc, *.c))
exebin += $(bin)
$(BUILD)/bin/trsc: $(bin) $(BUILD)/lib/libhorn.a $(BUILD)/lib/libassoc.a
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic -L$(BUILD) $(if $(DEBUG),-ggdb,) -ldl

test: all $(BUILD)/test/ap $(BUILD)/test/ap++ $(BUILD)/test/ap++getAll $(BUILD)/test/csv $(BUILD)/test/csv++ $(BUILD)/test/vec $(BUILD)/test/vec_int $(BUILD)/test/assoc $(BUILD)/test/assoc_int $(BUILD)/test/pgm

.SECONDEXPANSION:

$(filter %.c.o, $(libbin)): %: $$(call getsrc,%)
	$(CC) -c $^ -o $@ -std=gnu17 -Wall -Wextra -Wpedantic -Iinclude -fPIC $(if $(RELEASE),-O3 -DNODEBUG -DRELEASE,) $(if $(DEBUG),-ggdb -DDEBUG,)

$(filter %.c.o, $(exebin)): %: $$(call getsrc,%)
	$(CC) -c $^ -o $@ -std=gnu17 -Wall -Wextra -Wpedantic -Iinclude -fPIE $(if $(RELEASE),-O3 -DNODEBUG -DRELEASE,) $(if $(DEBUG),-ggdb -DDEBUG,)

$(filter %.cpp.o, $(libbin)): %: $$(call getsrc,%)
	$(CXX) -c $^ -o $@ -std=gnu++17 -Wall -Wextra -Wpedantic -Iinclude -fPIC $(if $(RELEASE),-O3 -DNODEBUG -DRELEASE,) $(if $(DEBUG),-ggdb -DDEBUG,)

$(filter %.cpp.o, $(exebin)): %: $$(call getsrc,%)
	$(CXX) -c $^ -o $@ -std=gnu++17 -Wall -Wextra -Wpedantic -Iinclude -fPIE $(if $(RELEASE),-O3 -DNODEBUG -DRELEASE,) $(if $(DEBUG),-ggdb -DDEBUG,)

$(BUILD)/test/ap: test/toollib/ap.c $(BUILD)/lib/libap.a
	$(CC) -o $@ $^ -std=gnu17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb 

$(BUILD)/test/ap++: test/toollib/ap++.cpp $(BUILD)/lib/libap.a
	$(CXX) -o $@ $^ -std=gnu++17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb 

$(BUILD)/test/ap++getAll: test/toollib/ap++getAll.cpp $(BUILD)/lib/libap.a
	$(CXX) -o $@ $^ -std=gnu++17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb 

$(BUILD)/test/csv: test/toollib/csv.c $(BUILD)/lib/libcsv.a
	$(CC) -o $@ $^ -std=gnu17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb 

$(BUILD)/test/csv++: test/toollib/csv++.cpp $(BUILD)/lib/libcsv.a
	$(CXX) -o $@ $^ -std=gnu++17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb 

$(BUILD)/test/vec: test/toollib/vec.c $(BUILD)/lib/libvec.a
	$(CC) -o $@ $^ -std=gnu17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb 

$(BUILD)/test/vec_int: test/toollib/vec_int.c $(BUILD)/lib/libvec.a
	$(CC) -o $@ $^ -std=gnu17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb 

$(BUILD)/test/assoc: test/toollib/assoc.c $(BUILD)/lib/libassoc.a
	$(CC) -o $@ $^ -std=gnu17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb 

$(BUILD)/test/assoc_int: test/toollib/assoc_int.c $(BUILD)/lib/libassoc.a
	$(CC) -o $@ $^ -std=gnu17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb 

$(BUILD)/test/pgm: test/toollib/pgm.c $(BUILD)/lib/libpgm.a
	$(CC) -o $@ $^ -std=gnu17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb 

