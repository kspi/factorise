APP := factorise

SOURCES := \
	src/main.c \
	src/factorise.c \
	src/number.c \
	src/list.c \
	src/multiset.c \

HEADERS := \
	src/factorise.h \
	src/number.h \
	src/list.h \
	src/multiset.h

OBJS := $(patsubst src/%.c,build/%.o,$(SOURCES))

CFLAGS = -std=c99 -pedantic -Wall -g -ggdb -g3 -O0
LDFLAGS = 

.PHONY: all
all: $(APP) test-$(APP)

.PHONY: test-$(APP)
test-$(APP): $(APP) test
	./test

$(APP): depend.mk $(OBJS)
	$(CC) $(LDFLAGS) -o $(APP) $(OBJS)

$(OBJS): build depend.mk Makefile

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $(patsubst build/%.o,src/%.c,$@)

build:
	mkdir -p build

clean:
	rm -f $(APP) $(OBJS) depend.mk
	rmdir build

depend.mk: $(SOURCES) $(INCLUDES)
# the -M switch probably only works with gcc
	$(CC) $(CFLAGS) -M $(SOURCES) $(INCLUDES) \
		| sed -r 's,^(.*\.o:),build/\1,' > depend.mk

-include depend.mk
