APP := factorise

SOURCES := main.c factorise.c number.c list.c multiset.c 
HEADERS := factorise.h number.h list.h multiset.h

TESTS := test_list

OBJS := $(patsubst %.c,%.o,$(SOURCES))
TEST_OBJS := $(subst main.o,,$(OBJS))

CFLAGS := -std=c99 -pedantic -Wall -g -ggdb -g3 -O0 -I.
LDFLAGS := 

.PHONY: all
all: $(APP) test-all

.PHONY: test-all
test-all: test.sh $(TESTS)
	for test in $(TESTS); do ./$$test || exit 1; done
	./test.sh

.PHONY: test-$(APP)
test-$(APP): $(APP) test.sh
	./test.sh

test_%: tests/test_%.c $(TEST_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(APP): depend.mk $(OBJS)
	$(CC) $(LDFLAGS) -o $(APP) $(OBJS)

$(OBJS): depend.mk Makefile

.PHONY: clean
clean:
	rm -f $(APP) $(OBJS) depend.mk $(TESTS)

depend.mk: $(SOURCES) $(INCLUDES)
# the -M switch probably only works with gcc
	$(CC) $(CFLAGS) -M $(SOURCES) $(INCLUDES) > depend.mk

-include depend.mk
