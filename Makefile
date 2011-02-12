APP = factorise
OBJS = factorise.o

CFLAGS = -std=c99 -pedantic -Wall -g -ggdb -g3 -O3 -pipe
LDFLAGS = 

.PHONY: all
all: $(APP) test-$(APP)

.PHONY: test-$(APP)
test-$(APP): $(APP) test
	./test

$(APP): $(OBJS)
	$(CC) $(LDFLAGS) -o $(APP) $(OBJS)

clean:
	rm -f $(APP) $(OBJS)
