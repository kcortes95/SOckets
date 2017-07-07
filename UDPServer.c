#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <string.h>

#include "datastructure.h"
#include "dbmanager.c"

#define STRING_LENGTH 100

int main (int argc, char* argv[]) {

	int sock; // socket descriptor
	struct sockaddr_in *udpServerAddr; // Local address
	struct sockaddr_in *udpClientAddr; // Client address
	unsigned short serverPort; // server port number
	int clientAddrLen;

	// check for correct no. of parameters
	if (argc != 2) {
		perror("Numero invalido de argumentos\n");
		exit(1);
	}
	
	serverPort = atoi(argv[1]);

	/* Create socket for sending and receiving datagram*/
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("Fallo la creacion del Socket en el server\n");
		exit(1);
	}
	
	udpServerAddr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));	

	/* fill the server address structure with zeros*/
	memset(udpServerAddr, 0, sizeof(*udpServerAddr));

	/*Creat the server address structure*/
	udpServerAddr->sin_family = AF_INET;
	udpServerAddr->sin_addr.s_addr = htonl(INADDR_ANY); // any incoming interface
	udpServerAddr->sin_port = htons(serverPort); // Local Port
	
	/* bind to the local address */
	if (bind(sock, (struct sockaddr *)udpServerAddr, sizeof(*udpServerAddr) ) < 0) {
		perror("Fallo el bind del Socket en el servidor\n");
		exit(1);
	}
	
	printf("\n************ System Server Running *************** \n");
	
	/*Create or open database*/
	//initLibraryDatabase();

	udpClientAddr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
	memset(udpClientAddr, 0, sizeof(*udpClientAddr));
	
	ClientMessage * clientMsg = malloc(sizeof(ClientMessage));	
	memset(clientMsg, 0, sizeof(*clientMsg));
	
	//Inicio de variables
	DBStruct* dbs;

	while (1){
		
		printf("Conexion establecida...\n");

		clientAddrLen = sizeof(struct sockaddr_in *);
		
		if (recvfrom(sock, clientMsg, sizeof(*clientMsg), 0, (struct sockaddr *)udpClientAddr, &clientAddrLen) < 0){
			perror("Fallo mensaje recibido del cliente\n");
			exit(1);
		}

		ServerMessage * serverMsg = NULL;
		serverMsg = malloc(sizeof(ServerMessage));
		memset(serverMsg, 0, sizeof(*serverMsg));
		serverMsg->requestID = clientMsg->requestID;
		serverMsg->userID = clientMsg->userID;

		int t = -1;
		switch (clientMsg->t) {
			case BOOKFLIGHT:

				if ( checkUser(clientMsg->id_db) == DENNIED || checkVuelo( clientMsg->id_vuelo ) == DENNIED || clientMsg->fila > MAX_FIL || clientMsg->col > MAX_COL ){
					serverMsg->t = BOOKNO;
				} else {

					if( checkNoBook(clientMsg->fecha, clientMsg->id_vuelo, clientMsg->fila, clientMsg->col) == ACCEPTED ){
						serverMsg->t = BOOKOK;
						insertNewBook(clientMsg->id_db, clientMsg->fecha, clientMsg->id_vuelo, clientMsg->fila, clientMsg->col);
					} else {
						serverMsg->t = BOOKNO;
					}

				}

			break;

			case CANCELFLIGHT:

				if ( checkUser(clientMsg->id_db) == DENNIED || checkVuelo( clientMsg->id_vuelo ) == DENNIED || clientMsg->fila > MAX_FIL || clientMsg->col > MAX_COL ){
					serverMsg->t = CANCELNO;
				} else {

					if( checkCancelBook(clientMsg->id_db, clientMsg->fecha, clientMsg->id_vuelo, clientMsg->fila, clientMsg->col) == ACCEPTED ){
						serverMsg->t = CANCELOK;
						cancelBook(clientMsg->id_db, clientMsg->fecha, clientMsg->id_vuelo, clientMsg->fila, clientMsg->col);
					} else {
						serverMsg->t = CANCELNO;
					}

				}

			break;

			case AVAILABLESINFLIGHT:

				if( checkVuelo( clientMsg->id_vuelo ) == DENNIED ){
					serverMsg->t = AINFNO;
				}
	
				dbs = getFlightState(clientMsg->fecha, clientMsg->id_vuelo);

				memset(serverMsg->msg, 0, 1201);
				sprintf(serverMsg->msg," ");

				for(int i = 0 ; i < dbs->tot_elems ; i++){			
					sprintf(serverMsg->msg, "%s %s", serverMsg->msg, dbs->ocupados[i]);
				}

				serverMsg->t = AINFOK;

			break;

			case AVAILABLEFLIGHTS:

				dbs = getAllFlights();

				memset(serverMsg->msg, 0, 1201);
				sprintf(serverMsg->msg,"");

				if(dbs->tot_elems == 0){
					serverMsg->t = AFNO;
				} else {
					serverMsg->t = AFOK;					
				}

				for(int i = 0 ; i < dbs->tot_elems ; i++){			
					sprintf(serverMsg->msg, "%s %s\n", serverMsg->msg, dbs->ocupados[i]);
				}

			break;

			case NEWF:

				if (checkVuelo( clientMsg->id_vuelo ) == DENNIED){
					insertNewFlight(clientMsg->id_vuelo, clientMsg->from, clientMsg->to);			
					serverMsg->t = NEWFOK;
				}else{
					serverMsg->t = NEWFNO;
				}

			break;

			case DELF:

				if (checkVuelo( clientMsg->id_vuelo ) != DENNIED){
					deleteFlight(clientMsg->id_vuelo);			
					serverMsg->t = DELFOK;
				}else{
					serverMsg->t = DELFNO;
				}

			break;

			case NEWU:

				insertNewUser(clientMsg->nombre, clientMsg->apellido, clientMsg->id_db);
				serverMsg->t = NEWUOK;
			break;

			case DELU:

				if (checkUser (clientMsg->id_db) == ACCEPTED){
					deleteUser(clientMsg->id_db);			
					serverMsg->t = DELUOK;
				}else{
					serverMsg->t = DELUNO;
				}

			break;

			case SHOWU:

				dbs = getAllUsers();

				memset(serverMsg->msg, 0, 1201);
				sprintf(serverMsg->msg,"");

				if(dbs->tot_elems == 0){
					serverMsg->t = SHOWUNO;
				} else {
					serverMsg->t = SHOWUOK;
				}

				for(int i = 0 ; i < dbs->tot_elems ; i++){			
					sprintf(serverMsg->msg, "%s %s\n", serverMsg->msg, dbs->ocupados[i]);
				}		

			break;

			case EXIT:

				break;

			default:
				break;

		}

		if (sendto(sock, serverMsg, sizeof(*serverMsg), 0, (struct sockaddr *)udpClientAddr, sizeof(*udpClientAddr)) < 0) {
			perror("Fallo el envio de informacion\n");
		}

	}	
}