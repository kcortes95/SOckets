#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sqlite3.h"
#define DB_NAME "so.db"

#define ACCEPTED 0
#define DENNIED 1

/*
gcc so.c sqlite3.c -o so -lpthread -ldl
*/

int callback(void*, int, char**, char**);
int checkUser(char* sql, char* err, sqlite3* db, char* id);

int callback(void* NotUsed, int argc, char** argv, char** azColName){
	int i;
	
	//printf("%s - %s\n",argv[0], argv[3]);

	printf("%s\n",argv[0]);

	return ACCEPTED;
}

int checkUser(char* sql, char* err, sqlite3* db, char* id){
	sql = "select count(id) from cliente where id = ";

	char* sql_aux = malloc(sizeof(char) * (strlen(sql) + strlen(id)));
	sprintf(sql_aux,"%s %s",sql, id);

	int rc = sqlite3_exec(db, sql_aux, callback, 0, &err);

	if(rc != SQLITE_OK){
		fprintf(stderr, "Failed\n");
		fprintf(stderr, "SQL Error : %s\n",err);
		sqlite3_free(err);
		sqlite3_close(db);
		return DENNIED;
	}else{
		printf("Todo ok\n");
		return ACCEPTED;
	}

}

int main(void){
	sqlite3* db;
	int rc = sqlite3_open(DB_NAME,&db);
	char* err;
	char* sql;

	if( rc != SQLITE_OK ){
		fprintf(stderr, "Error al abrir DB : %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return 1;
	}

	checkUser(sql, err, db, "3");

	sqlite3_close(db);
	return ACCEPTED;
}
