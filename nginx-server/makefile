test:test.o traffic_resolv.o traffic_data_store.o cJSON.o
	gcc -o test test.o traffic_data_store.o traffic_resolv.o cJSON.o -lm -lsqlite3 -Wall

traffic_resolv.o:traffic_resolv.c  traffic_resolv.h cJSON.h
	gcc -c traffic_resolv.c -lm -Wall

cJSON.o:cJSON.c cJSON.h
	gcc -c cJSON.c -lm

traffic_data_store.o:traffic_data_store.c
	gcc -c traffic_data_store.c -lm -Wall
