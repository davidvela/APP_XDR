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
#define PORTSERV 5001
void error (char * mensaje)
{perror (mensaje);	exit(1);	}




int main()	// ./servidor
{


int ds; 				//descriptor del socket
struct sockaddr_in servidor, cliente;  //info. servidor y cliente 
int long_serv, long_cli, n , m ,p;	//LONGITUDES...
int serv_puerto;
 
 //ESTRUCTURAS XDR
	XDR xdrp;
	char buf[MAXBUFF]; 
	struct registro datos_app; 

//Abrir socket (canal)

	if( (ds = socket(AF_INET, SOCK_DGRAM, 0) ) == -1)
	{	error("socket");	}

//Asociar el canal y el puerto

	bzero( (char*) & servidor, sizeof (servidor));   //inicializo a cero la estructura
	servidor.sin_family = AF_INET; 			//familia	
	servidor.sin_addr.s_addr = INADDR_ANY; 		//cualquier IP
	servidor.sin_port = htons (PORTSERV); /*formato red */
	long_serv = sizeof(servidor);
	if( bind(ds, (struct sockaddr *) &servidor, long_serv) == -1)
	{	error("bind");		}


	for(;;)
	{
		long_cli=sizeof(cliente);
		bzero( (char*) &cliente, long_cli); 	//inicializo a cero la estructura

		int is;
		fflush(stdin);
	
		for (is=0; is<MAXBUFF; is++)
		{buf[is]=0;		}	
		bzero((char *)&buf, sizeof(buf));

		//xdrp.x_op = XDR_DECODE;
		// Crear flujo XDR (XDR DECODE)
    		xdrmem_create(&xdrp,buf, MAXBUFF, XDR_DECODE);

 		//Recibir datos codificados (Datagrama)
		n= recvfrom(ds,&xdrp, MAXBUFF, 0, (struct sockaddr *) &cliente, &long_cli);
		if(n == -1)
		{ 	error("recvfrom");	}

		//printf("\n Mensaje recibido: %s \n Longitud: %d .",buf,n);//strlen(buf));

		
		//Aplicar filtro de conversion
 		if(!xdr_registro(&xdrp, &datos_app));
		{	error("filtro de recepcion\n");	}
		
		printf("nombre : %s \n ", datos_app.nombre);
		printf("dni : %s \n ", datos_app.dni);
		
		//Destruir flujo XDR		
		xdr_destroy(&xdrp);

/*
			//REALIZO EL SERVICIO:
				//invertir el mensaje:
					int i;	 	//indice de la cadena inversa
					int d=n-1;	//indice de la cadena original
				
					for( i=0; i<n; i++, d--)
					{	men_env[i] = men_rec[d]; 	}
		
		 

		//Crear flujo XDR (XDR ENCODE)
                  xdrmem_create(&xdrp,buf, MAXBUFF, XDR_ENCODE);
		
		//Aplicar filtro de conversion
		
		//Calcular longitud de los datos
		n = xdr getpos(&xdrp) // Llamar después de invocar al filtro 


		//Enviar datos codificados
				m= sendto (ds, buf, n, 0, (struct sockaddr *) &cliente, long_cli);
				if(m == -1)
				{	error("sendto");	}
				if(m != n)
				{	printf("\nse han perdido caracteres\n");	}
				printf("\n Mensaje enviado al cliente: %s \n",men_env);

		//Destruir flujo XDR


*/
	}		

	//cerrar el socket		
	close(ds); 	
}
		
		





