BUILD ?= build

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
getsrc=$(patsubst $(BUILD)/obj/%.o,src/%,$1)

dirs := $(BUILD)/obj $(BUILD)/bin $(BUILD)/lib $(BUILD)/test $(BUILD)/obj/ap $(BUILD)/obj/vec $(BUILD)/obj/assoc $(BUILD)/obj/pgm $(BUILD)/obj/error $(BUILD)/obj/horn $(BUILD)/obj/trs.cg.nasm_x86 $(BUILD)/obj/trs.cg.lisp $(BUILD)/obj/trsiron.trs $(BUILD)/obj/trsbee.c $(BUILD)/obj/horn_tool $(BUILD)/obj/iron_tool $(BUILD)/obj/bee_tool $(BUILD)/obj/trsc

.PHONY: clean all test

all: $(dirs) $(BUILD)/lib/libap.a $(BUILD)/lib/ap.so $(BUILD)/lib/libvec.a $(BUILD)/lib/vec.so $(BUILD)/lib/libassoc.a $(BUILD)/lib/assoc.so $(BUILD)/lib/libpgm.a $(BUILD)/lib/pgm.so $(BUILD)/lib/liberror.a $(BUILD)/lib/error.so $(BUILD)/lib/libhorn.a $(BUILD)/lib/horn.so $(BUILD)/lib/libtrs.cg.nasm_x86.a $(BUILD)/lib/trs.cg.nasm_x86.so $(BUILD)/lib/libtrs.cg.lisp.a $(BUILD)/lib/trs.cg.lisp.so $(BUILD)/lib/libtrsiron.trs.a $(BUILD)/lib/trsiron.trs.so $(BUILD)/lib/libtrsbee.c.a $(BUILD)/lib/trsbee.c.so $(BUILD)/bin/horn_tool $(BUILD)/bin/iron_tool $(BUILD)/bin/bee_tool $(BUILD)/bin/trsc

clean:
	$(RM) -r $(BUILD)

$(dirs):
	mkdir -p $@

exebin := 
libbin := 

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/ap, *.c))
libbin += $(bin)
$(BUILD)/lib/libap.a: $(bin)
	$(AR) qc $@ $^

$(BUILD)/lib/ap.so: $(bin)
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic -Wl,-soname,ap.so --shared $(if $(DEBUG),-ggdb,)

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/vec, *.c))
libbin += $(bin)
$(BUILD)/lib/libvec.a: $(bin)
	$(AR) qc $@ $^

$(BUILD)/lib/vec.so: $(bin)
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic -Wl,-soname,vec.so --shared $(if $(DEBUG),-ggdb,)

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/assoc, *.c))
libbin += $(bin)
$(BUILD)/lib/libassoc.a: $(bin)
	$(AR) qc $@ $^

$(BUILD)/lib/assoc.so: $(bin)
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic -Wl,-soname,assoc.so --shared $(if $(DEBUG),-ggdb,)

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/pgm, *.c))
libbin += $(bin)
$(BUILD)/lib/libpgm.a: $(bin)
	$(AR) qc $@ $^

$(BUILD)/lib/pgm.so: $(bin)
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic -Wl,-soname,pgm.so --shared $(if $(DEBUG),-ggdb,)

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/error, *.c))
libbin += $(bin)
$(BUILD)/lib/liberror.a: $(bin)
	$(AR) qc $@ $^

$(BUILD)/lib/error.so: $(bin)
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic -Wl,-soname,error.so --shared $(if $(DEBUG),-ggdb,)

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/horn, *.c))
libbin += $(bin)
$(BUILD)/lib/libhorn.a: $(bin)
	$(AR) qc $@ $^

$(BUILD)/lib/horn.so: $(bin)
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic -Wl,-soname,horn.so --shared $(if $(DEBUG),-ggdb,)

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/trs.cg.nasm_x86, *.c))
libbin += $(bin)
$(BUILD)/lib/libtrs.cg.nasm_x86.a: $(bin)
	$(AR) qc $@ $^

$(BUILD)/lib/trs.cg.nasm_x86.so: $(bin)
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic -Wl,-soname,trs.cg.nasm_x86.so --shared $(if $(DEBUG),-ggdb,)

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/trs.cg.lisp, *.c))
libbin += $(bin)
$(BUILD)/lib/libtrs.cg.lisp.a: $(bin)
	$(AR) qc $@ $^

$(BUILD)/lib/trs.cg.lisp.so: $(bin)
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic -Wl,-soname,trs.cg.lisp.so --shared $(if $(DEBUG),-ggdb,)

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/trsiron.trs, *.c))
libbin += $(bin)
$(BUILD)/lib/libtrsiron.trs.a: $(bin)
	$(AR) qc $@ $^

$(BUILD)/lib/trsiron.trs.so: $(bin)
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic -Wl,-soname,trsiron.trs.so --shared $(if $(DEBUG),-ggdb,)

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/trsbee.c, *.c))
libbin += $(bin)
$(BUILD)/lib/libtrsbee.c.a: $(bin)
	$(AR) qc $@ $^

$(BUILD)/lib/trsbee.c.so: $(bin)
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic -Wl,-soname,trsbee.c.so --shared $(if $(DEBUG),-ggdb,)

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/horn_tool, *.c))
exebin += $(bin)
$(BUILD)/bin/horn_tool: $(bin) $(BUILD)/lib/libhorn.a $(BUILD)/lib/liberror.a $(BUILD)/lib/libassoc.a $(BUILD)/lib/libpgm.a
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic -Wl,-rpath,../lib -L$(BUILD) $(if $(DEBUG),-ggdb,)

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/iron_tool, *.c))
exebin += $(bin)
$(BUILD)/bin/iron_tool: $(bin) $(BUILD)/lib/libhorn.a $(BUILD)/lib/liberror.a $(BUILD)/lib/libassoc.a $(BUILD)/lib/libpgm.a
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic -Wl,-rpath,../lib -L$(BUILD) $(if $(DEBUG),-ggdb,) -Wl,--export-dynamic -ldl

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/bee_tool, *.c))
exebin += $(bin)
$(BUILD)/bin/bee_tool: $(bin) $(BUILD)/lib/libhorn.a $(BUILD)/lib/liberror.a $(BUILD)/lib/libassoc.a $(BUILD)/lib/libpgm.a
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic -Wl,-rpath,../lib -L$(BUILD) $(if $(DEBUG),-ggdb,) -Wl,--export-dynamic -ldl

bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/trsc, *.c))
exebin += $(bin)
$(BUILD)/bin/trsc: $(bin) $(BUILD)/lib/liberror.a $(BUILD)/lib/libhorn.a $(BUILD)/lib/libassoc.a $(BUILD)/lib/libpgm.a
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic -Wl,-rpath,../lib -L$(BUILD) $(if $(DEBUG),-ggdb,) -ldl

test: all $(BUILD)/test/ap $(BUILD)/test/vec $(BUILD)/test/vec_int $(BUILD)/test/assoc $(BUILD)/test/assoc_int $(BUILD)/test/pgm

.SECONDEXPANSION:

$(filter %.c.o, $(libbin)): %: $$(call getsrc,%)
	$(CC) -c $^ -o $@ -std=gnu17 -Wall -Wextra -Wpedantic -Iinclude -fPIC $(if $(RELEASE),-O3 -DNODEBUG -DRELEASE,) $(if $(DEBUG),-ggdb -DDEBUG,)

$(filter %.c.o, $(exebin)): %: $$(call getsrc,%)
	$(CC) -c $^ -o $@ -std=gnu17 -Wall -Wextra -Wpedantic -Iinclude -fPIE $(if $(RELEASE),-O3 -DNODEBUG -DRELEASE,) $(if $(DEBUG),-ggdb -DDEBUG,)

$(filter %.cpp.o, $(libbin)): %: $$(call getsrc,%)
	$(CXX) -c $^ -o $@ -std=gnu++17 -Wall -Wextra -Wpedantic -Iinclude -fPIC $(if $(RELEASE),-O3 -DNODEBUG -DRELEASE,) $(if $(DEBUG),-ggdb -DDEBUG,)

$(filter %.cpp.o, $(exebin)): %: $$(call getsrc,%)
	$(CXX) -c $^ -o $@ -std=gnu++17 -Wall -Wextra -Wpedantic -Iinclude -fPIE $(if $(RELEASE),-O3 -DNODEBUG -DRELEASE,) $(if $(DEBUG),-ggdb -DDEBUG,)

$(BUILD)/test/ap: test/ap.c $(BUILD)/lib/libap.a
	$(CC) -o $@ $^ -std=gnu17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb 

$(BUILD)/test/vec: test/vec.c $(BUILD)/lib/libvec.a
	$(CC) -o $@ $^ -std=gnu17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb 

$(BUILD)/test/vec_int: test/vec_int.c $(BUILD)/lib/libvec.a
	$(CC) -o $@ $^ -std=gnu17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb 

$(BUILD)/test/assoc: test/assoc.c $(BUILD)/lib/libassoc.a
	$(CC) -o $@ $^ -std=gnu17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb 

$(BUILD)/test/assoc_int: test/assoc_int.c $(BUILD)/lib/libassoc.a
	$(CC) -o $@ $^ -std=gnu17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb 

$(BUILD)/test/pgm: test/pgm.c $(BUILD)/lib/libpgm.a
	$(CC) -o $@ $^ -std=gnu17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb 

