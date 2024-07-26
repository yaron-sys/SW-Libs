CC=clang
CFLAGS_DBG=-std=c89 -ansi -fpic -Wall -pedantic -Werror -D_DEBUG -g
CFLAGS=-std=c89 -ansi -fpic -Wall -pedantic -Werror -DENDEBUG -O3

SRC=$(wildcard *.c)
TEST_FILES=$(wildcard *test.c)
SRC_FILES = $(filter-out $(TEST_FILES), $(SRC))
DBG_TEST_FILES=$(TEST_FILES:.test.c=.dbg.out)
RLS_TEST_FILES=$(TEST_FILES:.test.c=.rls.out)

#LDFLAGS = -shared


.depends:
	gcc -MM $(SRC_FILES) > .depends

-include .depends


dbgtest: $(DBG_TEST_FILES)

rlstest: $(RLS_TEST_FILES)
	

%dbg.out: %test.c lib.dbg.so
	$(CC) $(CFLAGS_DBG) $< -Wl,-rpath=. lib.dbg.so -o $@
	

%rls.out: %test.c lib.so 
	$(CC) $(CFLAGS) $< -Wl,-rpath=. lib.so -o $@


release: rlstest lib.so



debug: dbgtest lib.dbg.so


lib.dbg.so: $(filter-out $(wildcard *test*),$(wildcard *.c))
	$(CC) $(CFLAGS_DBG) $(filter-out $(wildcard *test*),$(wildcard *.c)) -shared -o $@

lib.so: $(filter-out $(wildcard *test*),$(wildcard *.c))
	$(CC) $(CFLAGS) $(filter-out $(wildcard *test*),$(wildcard *.c)) -shared -o $@ 

current: 
	$(CC) $(CFLAGS_DBG) wd_exec.c heap.c dyn_vec.c wd.c scheduler.c heapify.c pq.c scheduler_task.c uuid.c -pthread -o wd_exec
	$(CC) $(CFLAGS_DBG) heap.c dyn_vec.c app.c wd.c scheduler.c heapify.c pq.c scheduler_task.c uuid.c -pthread -o app
	
libstatic.o: libstatic.c
	$(CC) $(CFLAGS) -c libstatic.c
	


	
libstatic.a: libstatic.o
	ar rc libstatic.a libstatic.o

libload.so: dynload.c
	$(CC) $(CFLAGS) -fpic dynload.c -shared -o libload.so

libdyn.so: dynlib.c
	$(CC) $(CFLAGS) -c -fpic dynlib.c -shared -o libdyn.so


clean:
	rm -f *.o main *.a .depends *.so *.out
