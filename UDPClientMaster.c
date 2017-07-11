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
void displayDataNewFlight(ClientMessage* clientMsg, int type);
void displayFlightData(ClientMessage* clientMsg, int type);
void displayNewUser(ClientMessage* clientMsg, int type);
void displayDeleteUser(ClientMessage* clientMsg, int type);


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
		printf("1) Crear vuelo\n");
		printf("2) Eliminar vuelo\n");
		printf("3) Ver vuelos disponibles\n");
		printf("4) Crear usuario\n");
		printf("5) Eliminar usuario\n");
		printf("6) Ver usuarios registrados\n");
		printf("7) Salir\n");

		do {
			printf("\nSeleccione una opcion valida: ");
			scanf("%d", &option);
		}
		while (option < 1 && option > 5);

		clc();

		printf("\n======================================================================\n");

		switch (option)
		{
			case 1:

				displayDataNewFlight(clientMsg, NEWF);

				sendMessageToServer(sock, clientMsg);
				receiveDataFromServer(sock, serverMsg);

				clc();

				if (serverMsg->requestID == clientMsg->requestID && serverMsg->t == NEWFOK) {
					printf("VUELO %d - %s >>> %s - Estado: AÑADIDO\n",clientMsg->id_vuelo, clientMsg->from, clientMsg->to);
				} else {
					printf("Encontramos un problema!. Fijate de haber ingresado todos los campos correctamente\n");
				}

			break;

			case 2:

				displayFlightData(clientMsg, DELF);

				sendMessageToServer(sock, clientMsg);
				receiveDataFromServer(sock, serverMsg);

				clc();

				if (serverMsg->requestID == clientMsg->requestID && serverMsg->t == DELFOK) {
					printf("VUELO %d - Estado: ELIMINADO\n", clientMsg->id_vuelo);
				} else {
					printf("Encontramos un problema!. Fijate de haber ingresado todos los campos correctamente\n");
				}

			break;


			case 3:

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


			case 4: 

				displayNewUser(clientMsg, NEWU);

				sendMessageToServer(sock, clientMsg);
				receiveDataFromServer(sock, serverMsg);

				clc();

				if (serverMsg->requestID == clientMsg->requestID && serverMsg->t == NEWUOK) {
					printf("Pasajero AÑADIDO con exito!\n");
				} else {
					printf("Encontramos un problema!. Fijate de haber ingresado todos los campos correctamente.\n");
				}

			break;

			case 5: // Ver vuelos disponibles

				displayDeleteUser(clientMsg, DELU);

				sendMessageToServer(sock, clientMsg);
				receiveDataFromServer(sock, serverMsg);

				clc();

				if (serverMsg->requestID == clientMsg->requestID && serverMsg->t == DELUOK) {
					printf("Pasajero ELIMINADO con exito\n");
				} else {
					printf("Encontramos un problema!. Fijate de haber ingresado todos los campos correctamente.\n");
				}

			break;


			case 6:

				clientMsg->requestID = rand() % 100 + 1;
				clientMsg->t = SHOWU;

				sendMessageToServer(sock, clientMsg);
				receiveDataFromServer(sock, serverMsg);

				clc();

				if (serverMsg->requestID == clientMsg->requestID && serverMsg->t == SHOWUNO){
					printf("No hay usuarios registrados por ahora.\n");
				} 

				if (serverMsg->requestID == clientMsg->requestID && serverMsg->t == SHOWUOK){ 
					printf("Estos son los usuarios registrados:\n");
					printf("%s\n", serverMsg->msg);

				} else {
					printf("No estamos en condiciones de mostrarte el resultado.\n");
				}

			break;


			case 7: // Salir
				exit(1);
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

void displayDataNewFlight(ClientMessage* clientMsg, int type){

	int id;
	char* from = malloc(sizeof(char) * MAX_INPUT_STRING);
	char* to = malloc(sizeof(char) * MAX_INPUT_STRING);

	int len_from, len_to = 0;

	printf("Ingrese ID del vuelo: \n");
	scanf("%d", &id);

	do {
		printf("Ingrese ORIGEN: \n");
		scanf("%s", from);

		len_from = strlen(from);

		if(len_from > MAX_INPUT_STRING)
			printf("Origen demasiado largo. Maximo permitido: 25 caracteres\n");

	} while(len_from > MAX_INPUT_STRING);

	do {
		printf("Ingrese DESTINO: \n");
		scanf("%s", to);

		len_to = strlen(to);

		if(len_to > MAX_INPUT_STRING)
			printf("Destino demasiado largo. Maximo permitido: 25 caracteres\n");

	} while(len_to > MAX_INPUT_STRING);

	clientMsg->requestID = rand() % 100 + 1;
	clientMsg->t = type;

	clientMsg->id_vuelo = id;
	memset(clientMsg->from, 0, MAX_INPUT_STRING);
	memcpy(clientMsg->from, from, strlen(from));

	memset(clientMsg->to, 0, MAX_INPUT_STRING);
	memcpy(clientMsg->to, to, strlen(to));

}

void displayFlightData(ClientMessage* clientMsg, int type){

	int id;

	printf("Ingrese ID del vuelo: \n");
	scanf("%d", &id);

	clientMsg->requestID = rand() % 100 + 1;
	clientMsg->t = type;

	clientMsg->id_vuelo = id;

}

void displayNewUser(ClientMessage* clientMsg, int type){

	int id;
	int id_int = 0;

	char* nombre = malloc(sizeof(char) * MAX_INPUT_STRING);
	int len_nombre = 0;

	char* apellido = malloc(sizeof(char) * MAX_INPUT_STRING);
	int len_apellido = 0;

	do {
		printf("Ingrese ID unico: \n");
		id_int = scanf("%d", &id);

	} while ( id_int != 1 );
	
	do {
		printf("Ingrese NOMBRE: \n");
		scanf("%s", nombre);

		len_nombre = strlen(nombre);

		if(len_nombre > MAX_INPUT_STRING)
			printf("Nombre demasiado largo. Maximo permitido: 25 caracteres\n");

	} while(len_nombre > MAX_INPUT_STRING);

	do {
		printf("Ingrese APELLIDO: \n");
		scanf("%s", apellido);

		len_apellido = strlen(apellido);

		if(len_apellido > MAX_INPUT_STRING)
			printf("Nombre demasiado largo. Maximo permitido: 25 caracteres\n");

	} while (len_apellido > MAX_INPUT_STRING);

	clientMsg->requestID = rand() % 100 + 1;
	clientMsg->t = type;

	clientMsg->id_db = id;

	memset(clientMsg->nombre, 0, MAX_INPUT_STRING);
	memcpy(clientMsg->nombre, nombre, strlen(nombre));

	memset(clientMsg->apellido, 0, MAX_INPUT_STRING);
	memcpy(clientMsg->apellido, apellido, strlen(apellido));

}

void displayDeleteUser(ClientMessage* clientMsg, int type){

	int id;

	printf("Ingrese ID del usuario: \n");
	scanf("%d", &id);

	clientMsg->requestID = rand() % 100 + 1;
	clientMsg->t = type;

	clientMsg->id_db = id;

}