
define allow-override
  $(if $(or $(findstring environment,$(origin $(1))),\
            $(findstring command line,$(origin $(1)))),,\
    $(eval $(1) = $(2)))
endef

TOOL_PREFIX=
CC=$(TOOL_PREFIX)gcc
LD=$(TOOL_PREFIX)ld

ifdef TOOL_PREFIX
$(call allow-override,CC,$(CC))
$(call allow-override,LD,$(LD))
endif

CDEBUG=-g
CFLAGS=

BINARY=leb128

.PHONY: all clean tests

all: $(BINARY)

obj/src/%.o: src/%.c
	mkdir -p obj/src
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BINARY): obj/src/main.o obj/src/uleb128.o
	$(CC) $(CFLAGS) -o $@ $^

release:
release: $(BINARY)

debug: CFLAGS += -g
debug: $(BINARY)


clean:
	rm -rf $(BINARY) obj 2>/dev/null
