#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "database.h"

#define DB_NAME "so.db"

#define ACCEPTED 0
#define DENNIED 1

/*
	gcc so.c sqlite3.c database.c -o so -lpthread -ldl
*/

int main(void){
	sqlite3* db;
	int rc = sqlite3_open(DB_NAME,&db);
	char* err;
	char* sql;
	char* fecha = "21/8/2019";

	if( rc != SQLITE_OK ){
		fprintf(stderr, "Error al abrir DB : %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return 1;
	}

	//printf("%i",checkFlight(db,err,11548));    -> Debería devolver 0
	//printf("%i",checkFlight(db,err,126485));   -> Debería devolver 3
	//printf("%i",checkFlight(db,err,13338));    -> Debería devolver 0
	//printf("%i",checkFlight(db,err,133388888));-> Debería devolver 3

	//printf("%i",checkUser(db,err,38668154));   -> Debería devolver 0
	//printf("%i",checkUser(db,err,11548));      -> Debería devolver 2
	//printf("%i",checkUser(db,err,17856954));   -> Debería devolver 0
	//printf("%i",checkUser(db,err,11333333));   -> Debería devolver 2

	//printf("%i",book(db,err,11548,1,2,fecha,17856954));   -> Se inserta
	//printf("%i",book(db,err,11548,4,4,fecha,0000));       -> No se inserta  
	//printf("%i",book(db,err,11548,5,6,fecha,5847221));    -> Se inserta
	//printf("%i",book(db,err,18888548,3,8,fecha,5847221)); -> No se inserta

	//printf("%i",removeBook(db,err,12648,38668154));
	//printf("%i",removeBook(db,err,12648,38668154));
	
	struct Info* info;
	check(db,err,13338,info);
	

	sqlite3_close(db);
	return ACCEPTED;
}
