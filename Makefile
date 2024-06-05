BUILD ?= build

INCLUDE := include

CXXSTD := 17
CSTD := 17

dirs = $(BUILD) $(BUILD)/trsap.dir $(BUILD)/trsp.dir $(BUILD)/trsre $(BUILD)/trsc

.PHONY: all 
all: $(dirs) $(BUILD)/libtrsap.a

clean:
	$(RM) -r $(BUILD)

$(dirs):
	mkdir -p $@

trsap_src := $(wildcard trsap/**/*.c trsap/*.c)
trsap_bin := $(patsubst trsap/%,$(BUILD)/trsap.dir/%.o,$(trsap_src))
$(BUILD)/libtrsap.a: $(trsap_bin)
	$(AR) qc $@ $^

$(filter %.c.o,$(trsap_bin)): $(BUILD)/trsap.dir/%.o: trsap/%
	$(CC) -c -o $@ $^ -std=c$(CSTD) -I $(INCLUDE) -fPIC

