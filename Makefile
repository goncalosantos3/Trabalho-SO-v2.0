all: folders server client

server: bin/monitor

client: bin/tracer

folders:
	@mkdir -p src obj bin tmp

bin/monitor: obj/monitor.o obj/exec.o obj/llExec.o
	gcc -g obj/monitor.o obj/exec.o obj/llExec.o -o bin/monitor

obj/monitor.o: src/server/monitor.c
	gcc -Wall -g -c src/server/monitor.c -o obj/monitor.o

obj/exec.o: src/server/exec.c src/server/exec.h
	gcc -Wall -g -c src/server/exec.c -o obj/exec.o

obj/llExec.o: src/server/llExec.c src/server/llExec.h
	gcc -Wall -g -c src/server/llExec.c -o obj/llExec.o

bin/tracer: obj/tracer.o obj/exePrograms.o
	gcc -g obj/tracer.o obj/exePrograms.o -o bin/tracer

obj/tracer.o: src/client/tracer.c
	gcc -Wall -g -c src/client/tracer.c -o obj/tracer.o

obj/exePrograms.o: src/client/exePrograms.c src/client/exePrograms.h
	gcc -Wall -g -c src/client/exePrograms.c -o obj/exePrograms.o

clean:
	rm -f obj/* tmp/* bin/{tracer,monitor}