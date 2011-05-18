APP := factorise

SOURCES := $(wildcard src/*.c)
HEADERS := $(wildcard src/*.h)

TEST_SOURCES := $(wildcard tests/test_*.c)
TESTS := $(patsubst tests/test_%.c,test_%,$(TEST_SOURCES))

OBJS := $(patsubst %.c,%.o,$(SOURCES))
TEST_OBJS := $(subst src/main.o,,$(OBJS))

CFLAGS := -std=c99 -pedantic -Wall -g -ggdb -g3 -O0
LDFLAGS := 

.PHONY: all
all: $(APP) test-all

.PHONY: test-all
test-all: test-factorise $(TESTS)
	for test in $(TESTS); do ./$$test || exit 1; done

.PHONY: test-$(APP)
test-$(APP): $(APP) test.sh
	./test.sh

test_%: tests/test_%.c $(TEST_OBJS)
	$(CC) $(CFLAGS) -Isrc $(LDFLAGS) -o $@ $^

$(APP): depend.mk $(OBJS)
	$(CC) $(LDFLAGS) -o $(APP) $(OBJS)

$(OBJS): depend.mk Makefile

.PHONY: clean
clean:
	rm -f $(APP) $(OBJS) depend.mk $(TESTS)

depend.mk: $(SOURCES) $(INCLUDES)
# the -M switch probably only works with gcc
	$(CC) $(CFLAGS) -M $(SOURCES) $(INCLUDES) > depend.mk

.PHONY: check-syntax
check-syntax:
	$(CC) $(CFLAGS) -fsyntax-only $(CHK_SOURCES)

-include depend.mk
