CFLAGS = -ggdb -std=c++11

all: serverA.cpp serverB.cpp servermain.cpp client.cpp readfile.cpp recommend.cpp
	g++ $(CFLAGS) serverA.cpp readfile.cpp recommend.cpp -o serverA
	g++ $(CFLAGS) serverB.cpp readfile.cpp recommend.cpp -o serverB
	g++ $(CFLAGS) servermain.cpp -o servermain
	g++ $(CFLAGS) client.cpp -o client


.PHONY: serverA
serverA:
	./serverA

.PHONY: serverB
serverB:
	./serverB

.PHONY: mainserver
mainserver:
	./servermain

.PHONY: client
client:
	./client

clean:
	rm serverA serverB servermain client
