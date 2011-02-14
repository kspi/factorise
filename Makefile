APP = factorise
OBJS = main.o factorise.o number.o list.o multiset.o

CFLAGS = -std=c99 -pedantic -Wall -g -ggdb -g3 -O0
LDFLAGS = 

.PHONY: all
all: $(APP) test-$(APP)

.PHONY: test-$(APP)
test-$(APP): $(APP) test
	./test

$(APP): $(OBJS)
	$(CC) $(LDFLAGS) -o $(APP) $(OBJS)

$(OBJS): Makefile

clean:
	rm -f $(APP) $(OBJS)
