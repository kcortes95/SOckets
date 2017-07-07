#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sqlite3.h"
#define DB_NAME "so.db"

#define ACCEPTED 0
#define DENNIED 1

typedef struct {
	int q;
} DBStruct;

int callback(void*, int, char**, char**);
int checkUser(DBStruct* dbs, char* sql, char* err, sqlite3* db, char* id);

int callback(void* data, int argc, char** argv, char** azColName){
	int i;

	DBStruct* dbs_aux = (DBStruct*) data;
	dbs_aux->q = atoi(argv[0]);  

	return ACCEPTED;
}

int checkUser(DBStruct* dbs, char* sql, char* err, sqlite3* db, char* id){
	sql = "select count(id) from cliente where id = ";

	char* sql_aux = malloc(sizeof(char) * (strlen(sql) + strlen(id)));
	sprintf(sql_aux,"%s %s",sql, id);

	int rc = sqlite3_exec(db, sql_aux, callback, dbs, &err);

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

	DBStruct* dbs = malloc(sizeof(DBStruct));
	dbs->q = 127;
	printf("El valor de dbs->q : %d\n", dbs->q);

	checkUser(dbs, sql, err, db, "5");

	printf("El valor de dbs->q despues : %d\n", dbs->q);

	sqlite3_close(db);
	return ACCEPTED;
}
