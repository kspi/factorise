APP := factorise

SOURCES := main.c factorise.c number.c list.c multiset.c 
HEADERS := factorise.h number.h list.h multiset.h

OBJS := $(patsubst %.c,%.o,$(SOURCES))

CFLAGS := -std=c99 -pedantic -Wall -g -ggdb -g3 -O0
LDFLAGS := 

.PHONY: all
all: $(APP) test-$(APP)

.PHONY: test-$(APP)
test-$(APP): $(APP) test
	./test

$(APP): depend.mk $(OBJS)
	$(CC) $(LDFLAGS) -o $(APP) $(OBJS)

$(OBJS): depend.mk Makefile

.PHONY: clean
clean:
	rm -f $(APP) $(OBJS) depend.mk

depend.mk: $(SOURCES) $(INCLUDES)
# the -M switch probably only works with gcc
	$(CC) $(CFLAGS) -M $(SOURCES) $(INCLUDES) > depend.mk

-include depend.mk
