all: folders server client

server: bin/monitor

client: bin/tracer

folders:
	@mkdir -p src obj bin tmp

bin/monitor: obj/monitor.o
	gcc -g obj/monitor.o -o bin/monitor

obj/monitor.o: src/server/monitor.c
	gcc -Wall -g -c src/server/monitor.c -o obj/monitor.o

obj/exe.o: src/server/exe.c src/server/exe.h
	gcc -Wall -g -c src/server/exe.c -o obj/exe.o

obj/llExec.o: src/server/llExec.c src/server/llExec.h
	gcc -Wall -g -c src/server/llExec.c -o obj/llExec.o

bin/tracer: obj/tracer.o
	gcc -g obj/tracer.o -o bin/tracer

obj/tracer.o: src/client/tracer.c
	gcc -Wall -g -c src/client/tracer.c -o obj/tracer.o

clean:
	rm -f obj/* tmp/* bin/{tracer,monitor}