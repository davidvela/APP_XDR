PHONY: all
CFLAGS= -g -Wall -c
allxdr: cabecera filtros 
all: servidor cliente 

cabecera: muestra.x
	rpcgen -h muestra.x >> muestra.h 	#cabecera 

filtros: muestra.x
	rpcgen -c muestra.x > muestra_xdr.c 	#filtros 


servidor: servidor.o muestra_xdr.o
	gcc servidor.o muestra_xdr.o  -o servidor

cliente: cliente.o muestra_xdr.o
	gcc cliente.o muestra_xdr.o -o cliente

muestra_xdr.o: muestra_xdr.c
	gcc $(CFLAGS) muestra_xdr.c 

servidor.o: servidor.c
	gcc $(CFLAGS) servidor.c 

cliente.o: cliente.c 
	gcc $(CFLAGS) cliente.c 
clean: 
	-rm -f *.o
	-rm -f cliente servidor 
	
