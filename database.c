#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "sqlite3.h"

#define ROW                   25
#define COL                   6
#define MAX_FLIGHT_NUMBER     6
#define MAX_CONSULT_CHAR      500
#define ERROR                -1
#define ACCEPTED              0
#define DENNIED               1
#define INCORRECT_CLIENT      2
#define INCORRECT_FLIGHT      3

typedef enum { false, true } bool;
static  int  res = ERROR;
static  bool in_valid = false;
static  char from[100];
static	char to[100];
static	char status[50];

struct Info{
	char* from[100];
	char* to[100];
	char* status[50];	
} info;

int valid(void* NotUsed, int argc, char** argv, char** azColName){
	in_valid = true;
	return ACCEPTED;
}

int checkFlight(sqlite3* db, char* err, int id_flight){
	char* sql = "select * from vuelos where id_vuelo =";
	char* sql_aux = malloc(sizeof(char) * MAX_CONSULT_CHAR);
	sprintf(sql_aux,"%s %i",sql, id_flight);
	int rc = sqlite3_exec(db, sql_aux, valid, 0, &err);
	if(rc != SQLITE_OK){
		sqlite3_free(err);
		sqlite3_close(db);
		return DENNIED;
	}else{
		if(in_valid){
			in_valid = false;
			return ACCEPTED;
		}
		return INCORRECT_FLIGHT;
	}
}

int checkUser(sqlite3* db, char* err, int dni_cli){
	char* sql = "select * from cliente where dni = ";
	char* sql_aux = malloc(sizeof(char) * MAX_CONSULT_CHAR);
	sprintf(sql_aux,"%s %i",sql, dni_cli);
	int rc = sqlite3_exec(db, sql_aux,valid, 0, &err);
	if(rc != SQLITE_OK){
		sqlite3_free(err);
		sqlite3_close(db);
		return DENNIED;
	}else{
		if(in_valid){
			in_valid = false;
			return ACCEPTED;
		}
		return INCORRECT_CLIENT;
	}
}

//arg 1 -> fila. arg 2 -> col.
int print_plane(void* NotUsed, int argc, char** argv, char** azColName){
	printf("%c", argv[2][0]);
	bool flag = false;
	for(int i = 0; i < ROW; i++){
		flag = true;
		int k = 0;

		//Imprime los rotulos de las columnas.
		for(int j = 0; j < COL; j++){
			if(i == 0){
				if(j == 0){
					printf("\t%i\t",k++);
				}else {
					printf("%i\t",k++);
				}
			}
		}

		printf("\n");
		k = i;

		for(int j = 0; j < COL; j++)
			if(argv[1][0] == i && argv[2][0] == j){
				if(flag){
					printf("%i\tX\t", k);
					flag = false;
				} else{
					printf("X\t");
				}
			}else{
				if(flag){
					printf("%i\tO\t",k);
					flag = false;
				}else{
					printf("O\t");
				}
			}
		}
		printf("\n");
	
	return ACCEPTED;
}

int print_ok(void* NotUsed, int argc, char** argv, char** azColName){
	return ACCEPTED;
}


int print_dav(sqlite3* db, char * err, int id_flight){
	char* sql = "select * from asientos where id_vuelo = "; 
	char* sql_aux = malloc(sizeof(char) * MAX_CONSULT_CHAR);
	sprintf(sql_aux,"%s'%i';",sql, id_flight);
	int rc = sqlite3_exec(db, sql_aux, print_plane, 0, &err);
	if(rc != SQLITE_OK){
		fprintf(stderr, "Failed\n");
		fprintf(stderr, "SQL Error : %s\n",err);
		sqlite3_free(err);
		sqlite3_close(db);
		return DENNIED;
	}else{
		return ACCEPTED;
	}
}


int book(sqlite3* db, char * err, int id_flight, int f, int c, char* fecha, int dni_client){	
	if(checkFlight(db,err,id_flight) == ACCEPTED){
		if(checkUser(db,err,dni_client) == ACCEPTED){
			char* sql = "insert into reserva values(";
			char* sql_aux = malloc(sizeof(char)* MAX_CONSULT_CHAR);
			sprintf(sql_aux,"%s %i, %i, %i, %i, %s);",sql,id_flight,dni_client,f,c,fecha);
			int rc = sqlite3_exec(db, sql_aux, print_ok, 0, &err);
			if(rc != SQLITE_OK){
					fprintf(stderr, "Failed\n");
					fprintf(stderr, "SQL Error : %s\n",err);
					sqlite3_free(err);
					sqlite3_close(db);
					return DENNIED;
			}else {
				return ACCEPTED;
			}
		}else {
			return INCORRECT_CLIENT;
		}
	}else {
		return INCORRECT_FLIGHT;
	}	
}


int removeBook(sqlite3* db, char * err, int id_flight, int id_client){
	if(checkUser(db,err,id_client) == ACCEPTED && checkFlight(db,err,id_flight) == ACCEPTED){
		char* sql = "delete from reserva where id_vuelo =";
		char* sql_aux = malloc(sizeof(char)* MAX_CONSULT_CHAR);
		sprintf(sql_aux,"%s%i and id_cliente = %i;",sql,id_flight,id_client);
		int rc = sqlite3_exec(db, sql_aux, print_ok, 0, &err);
		if(rc != SQLITE_OK){
			fprintf(stderr, "Failed\n");
			fprintf(stderr, "SQL Error : %s\n",err);
			sqlite3_free(err);
			sqlite3_close(db);
			return DENNIED;
		}else{
			return ACCEPTED;
		}
	} else{
		return ACCEPTED;
	}
}

int save_info(void* NotUsed, int argc, char** argv, char** azColName){
	for(int i = 0; argv[1][i] != '\0';i++){
		from[i] = argv[1][i];
	}
	for(int i = 0; argv[2][i] != '\0';i++){
		to[i] = argv[2][i];
	}
	for(int i = 0; argv[3][i] != '\0';i++){
		status[i] = argv[3][i];
	}
	return ACCEPTED;
}

int check(sqlite3* db, char * err, int id_flight, struct Info* info){
	char* sql = "select * from vuelos where id_vuelo = ";
	char* sql_aux = malloc(sizeof(char)* MAX_CONSULT_CHAR);
	sprintf(sql_aux,"%s %i;",sql,id_flight);
	int rc = sqlite3_exec(db, sql_aux, save_info, 0, &err);
	if(rc != SQLITE_OK){
		fprintf(stderr, "Failed\n");
		fprintf(stderr, "SQL Error : %s\n",err);
		sqlite3_free(err);
		sqlite3_close(db);
		return DENNIED;
	}else{

	//	info -> from = from;
	//	info -> to = to;
	//	info -> status = status;
	//	clean(from);
	//	clean(to);
	//	clean(status);

		return ACCEPTED;
	}
}

void clean(char c[]){
	for(int i = 0; i < strlen(c); i++){
		c[i] = " ";
	}
}
