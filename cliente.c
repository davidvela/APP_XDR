#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "muestra.h"
#include <rpc/types.h>
#include <rpc/xdr.h>

#define MAXBUFF 100 //longitud máxima mensajes replicar con ECO

#define PORTCLIE 5001  //puerto del cliente


void error (char * mensaje)
{
	perror (mensaje);
	exit(1);
}


int main()		// ./cliente  
{

int ds; 				//descriptor del socket
struct sockaddr_in servidor, cliente;  //info. servidor y cliente 
int long_serv, long_cli, n , m,p ;	//LONGITUDES...
int serv_puerto;
char localhost[MAXBUFF]= "127.0.0.1";

//ESTRUCTURAS XDR
	XDR xdrp;
	char buf[MAXBUFF];
	char buff1[MAXBUFF];
	struct registro datos_app; 
		
	system("clear");	
	bzero( (char*) & cliente, sizeof (cliente));   //inicializo a cero la estructura
	bzero( (char*) & servidor, sizeof (servidor));   //inicializo a cero la estructura

//1 abrir socket (canal)

	if( (ds = socket(AF_INET, SOCK_DGRAM, 0) ) == -1)
	{	error("socket");	}

//2 asociar el canal y el puerto


	cliente.sin_family = AF_INET; 			//familia	
	cliente.sin_port = htons (PORTCLIE); /*formato red */
	cliente.sin_addr.s_addr = inet_addr(localhost); 		// IP de localhost
	long_cli = sizeof(cliente);
	

	
	for(;;)
	{
		//REALIZO LA LECTURA DE LOS DATOS DEL TECLADO:
		int i;
		//limpio los buffer
		fflush(stdin);
		for (i=0; i<MAXBUFF; i++)
		{	buf[i]=0;	}		
		bzero((char *)&buf, sizeof(buf));

	
		printf("\n ESCRIBIR CADENA: ");		
		//COMPRUEBO: si es esta vacio me salgo
		if ( (fgets(buff1, MAXBUFF, stdin) == NULL) && (n=strlen(buff1)==0) ) 
		{	error("fgets");	}
	
		if( (n=strlen(buff1)) == 1)
			{
				printf("\nCadena vacia.\n ADIOS.\n");
				break;
			}


		datos_app.dni=121212;
		strcpy(datos_app.nombre,"hola");

		//xdrp.x_op = XDR_ENCODE;
		//Crear flujo XDR (XDR ENCODE)
		xdrmem_create(&xdrp,buf, MAXBUFF, XDR_ENCODE);	//Aplicar filtro de conversion
		
		//Aplicar filtro de conversion
 		if(!xdr_registro(&xdrp, &datos_app));
		{	error("filtro de envio");	}

		//Calcular longitud de los datos
		n = xdr_getpos(&xdrp);
		printf("Estoy enviando: %d \n", n);		
		//Enviar datos codificados
		m= sendto (ds, &xdrp, n, 0, (struct sockaddr *) &cliente, long_cli);
		if(m == -1)
		{	error("sendto");	}
		if(m != n)
		{	printf("\nse han perdido caracteres\n");	}
			
		printf("\n Mensaje enviado: %s \n Longitud: %d .",buf,strlen(buf));
		
		//Destruir flujo XDR		
		xdr_destroy(&xdrp);

/*
		//Crear flujo XDR (XDR DECODE)
		//Recibir datos codificados

		//Aplicar filtro de conversion
		//Destruir flujo XDR

		//4recibir datagrama:
		n= recvfrom(ds, buf, MAXBUFF, 0, (struct sockaddr *) &servidor, &long_serv);
		if(n == -1)
		{ 	error("recvfrom");	}
			
		
	
			
	
		//VISUALIZO LA CADENA RECIBIDA.
			printf("\n CADENA RECIBIDA: %s", men_rec);
			printf("\n\n\n");
	

*/	
	}		
		
 	//Cerrar el socket
	close(ds);
}
		
		
