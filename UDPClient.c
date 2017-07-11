#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#include <sys/types.h>
#include <stdbool.h>
#include <string.h>

#include "datastructure.h"

int sendMessageToServer(int sock, ClientMessage* msg);
int receiveDataFromServer(int sock, ServerMessage* serverMsg);
void clc();
void displayAllUserData(ClientMessage* clientMsg, int type);
void displayIdDateUserData(ClientMessage* clientMsg, int type);


struct sockaddr_in *udpServerAddr; // server address details - ip and port number

int main(int argc, char *argv[]){
	int sock; // socket descriptor
	char *serverIP; // server IP address
	unsigned short serverPort; // server port number
	int userId = 0;

	if (argc != 3){
		perror("Numero invalido de argumentos\n");
		exit(1);
	}
	
	serverIP = argv[1];
	serverPort = atoi(argv[2]);
	srand(time(NULL)); // set up the seed for the random generator

	/* allocate pointer for udp server addr structure*/
	udpServerAddr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in *));
	/*  fill the structure with zeros */
	memset(udpServerAddr, 0, sizeof(*udpServerAddr));
	/* construct the server address structure. */
	udpServerAddr->sin_family = AF_INET;
	udpServerAddr->sin_addr.s_addr = inet_addr(serverIP);
	udpServerAddr->sin_port = htons(serverPort);
	
	/* Create a UDP socket to connect*/
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP )) < 0){
		perror("Creacion del socket fallo\n");
		exit(1);
	}		
	
	printf("************ CONECTADO ************\n");

	/* Allocate for the client message to be sent*/
	ClientMessage* clientMsg = (ClientMessage*) malloc(sizeof(ClientMessage));
	memset(clientMsg, 0, sizeof(*clientMsg));

 	// Create the structure for receiving the server response
	ServerMessage* serverMsg = (ServerMessage*) malloc(sizeof(ServerMessage));
	memset(serverMsg, 0, sizeof(*serverMsg));

	int option;

	//Todos los parametros que el usuario ingresara por consola
	int id = 0; //id del vuelo cuando la persona ingresa por consola
	int fila = 0;
	int columna = 0;
	char* nombre = malloc(sizeof(char) * 20);
	int dia, mes, anio;
	int id_db;

	while(1)
	{
		printf("\n======================================================================\n");
		printf("Seleccione una de las siguientes operaciones: \n");
		printf("1) Reservar asiento de un vuelo\n");
		printf("2) Cancelar reserva de un vuelo\n");
		printf("3) Ver ocupados de un vuelo\n");
		printf("4) Ver vuelos disponibles\n");
		printf("5) Salir\n");

		do {
			printf("\nSeleccione una opcion valida: ");
			scanf("%d", &option);
		}
		while (option < 1 && option > 5);

		clc();

		printf("\n======================================================================\n");

		switch (option)
		{
			case 1: // Reservar

				displayAllUserData(clientMsg, BOOKFLIGHT);

				sendMessageToServer(sock, clientMsg);
				receiveDataFromServer(sock, serverMsg);

				clc();

				if (serverMsg->requestID == clientMsg->requestID && serverMsg->t == BOOKOK) {
					printf("Felicitaciones! Reservaste el vuelo %d, asientos [%d, %d] para el dia %s\n", clientMsg->id_vuelo , clientMsg->fila, clientMsg->col, clientMsg->fecha);
				} else {
					printf("Encontramos un problema!. Fijate de haber ingresado todos los campos correctamente\n");
				}

				break;

			case 2: // Cancelar

				displayAllUserData(clientMsg, CANCELFLIGHT);

				sendMessageToServer(sock, clientMsg);
				receiveDataFromServer(sock, serverMsg);

				clc();

				if (serverMsg->requestID == clientMsg->requestID && serverMsg->t == CANCELOK) {
					printf("Cancelaste la reserva del vuelo %d, asientos [%d, %d] para el %s\n", clientMsg->id_vuelo , clientMsg->fila, clientMsg->col, clientMsg->fecha);
				} else {
					printf("Encontramos un problema!. Fijate de haber ingresado todos los campos correctamente\n");
				}

				break;

			case 3: // Ver ocupados de 1 vuelo

				displayIdDateUserData(clientMsg, AVAILABLESINFLIGHT);

				sendMessageToServer(sock, clientMsg);
				receiveDataFromServer(sock, serverMsg);

				clc();

				if (serverMsg->requestID == clientMsg->requestID && serverMsg->t == AINFOK){ 
					printf("Asientos ocupados del dia %s, vuelo %d:\n", clientMsg->fecha, clientMsg->id_vuelo);
					printf("%s\n", serverMsg->msg);
				} else {
					printf("No estamos en condiciones de mostrarte el resultado!\n");
				}

				break;

			case 4: // Ver vuelos disponibles

				clientMsg->requestID = rand() % 100 + 1;
				clientMsg->t = AVAILABLEFLIGHTS;

				sendMessageToServer(sock, clientMsg);
				receiveDataFromServer(sock, serverMsg);

				clc();

				if(serverMsg->requestID == clientMsg->requestID && serverMsg->t == AFNO){
					printf("No hay vuelos registrados en este momento.\n");
				}

				if (serverMsg->requestID == clientMsg->requestID && serverMsg->t == AFOK){ 
					printf("Estos son los vuelos disponibles:\n");
					printf("%s\n", serverMsg->msg);

				} else {
					printf("No estamos en condiciones de mostrarte el resultado.\n");
				}

				break;

			case 5: // Salir
				exit(1);
				break;

			default:
				break;
		}
	}
	return 0;
}
	 
int sendMessageToServer(int sock, ClientMessage * msg) {
	int sendLen = -1;

	// send message to server
	sendLen = sendto(sock, msg, sizeof(*msg), 0, (struct sockaddr *)udpServerAddr, sizeof(*udpServerAddr));
	
	if (sendLen < 0){
		perror("Fallo el envio de informacion en cliente\n");
		exit(1);
	}

	return sendLen;
}

int receiveDataFromServer(int sock, ServerMessage* serverMsg) {
	int responseSize = -1;
	int serverAddrLen = -1;
	struct sockaddr_in *responseAddr; // response source address

	responseAddr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
	memset(responseAddr, 0, sizeof(*responseAddr));

	while (1) {
		serverAddrLen = sizeof(struct sockadr_in *);
		responseSize = recvfrom(sock, serverMsg, sizeof(*serverMsg), 0, (struct sockaddr *)responseAddr, &serverAddrLen);
		if (responseSize > 0) {
			break;
		} else {
			perror("Fallo el mensaje llegado del servidor en cliente.\n");
			exit(1);
		}
	}

	if (responseAddr->sin_addr.s_addr != udpServerAddr->sin_addr.s_addr) {
		perror("El mensaje es de una fuente desconocida\n");
		exit(1);
	}

	return responseSize;
}

void clc(){
	system("clear");
	return;
}

void displayAllUserData(ClientMessage* clientMsg, int type){

	int id, fila, columna, dia, mes, anio, id_db;
	int fecha_longitud = 11;
	char* fecha_string = malloc(sizeof(char) * fecha_longitud);

	printf("Ingrese ID del vuelo a reservar: \n");
	scanf("%d", &id);

	printf("Ingrese FILA y COLUMNA de los asientos: [Ej: 5, 1]\n");
	scanf("%d, %d", &fila, &columna);

	printf("Ingrese su numero de IDENTIFICACION UNICO\n");
	scanf("%d",&id_db);

	printf("Ingrese la FECHA de la reserva [Ej: 01/01/2017]\n");
	scanf("%d/%d/%d",&dia,&mes,&anio);

	sprintf(fecha_string,"%d/%d/%d",dia,mes,anio);

	clientMsg->requestID = rand() % 100 + 1;
	clientMsg->t = type;

	clientMsg->id_vuelo = id;
	clientMsg->fila = fila;
	clientMsg->col = columna;
	clientMsg->id_db = id_db;
	memcpy(clientMsg->fecha, fecha_string, fecha_longitud);

}

void displayIdDateUserData(ClientMessage* clientMsg, int type){

	int id, dia, mes, anio;
	int fecha_longitud = 11;
	char* fecha_string = malloc(sizeof(char) * fecha_longitud);

	printf("Ingrese ID del vuelo: \n");
	scanf("%d", &id);

	printf("Ingrese la FECHA [Ej: 01/01/2017]\n");
	scanf("%d/%d/%d",&dia,&mes,&anio);

	clientMsg->requestID = rand() % 100 + 1;
	clientMsg->t = type;
	clientMsg->id_vuelo = id;

	sprintf(fecha_string,"%d/%d/%d",dia,mes,anio);
	memcpy(clientMsg->fecha, fecha_string, fecha_longitud);

}