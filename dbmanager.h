#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DB_NAME "so.db"
#define ACCEPTED 0
#define DENNIED 1
#define MAX_SQL_QUERY 150
#define MAX_OCUPADOS 300
#define MAX_L 100

typedef struct {
	int q;
	char* ocupados[MAX_OCUPADOS];
	int tot_elems;
} DBStruct;

int callback(void*, int, char**, char**);
int exec_checkUser(DBStruct* dbs, char* sql, char* err, sqlite3* db, int id);
int checkUser(int id);
//
int callback_vuelo(void*, int, char**, char**);
int exec_checkVuelo(DBStruct* dbs, char* sql, char* err, sqlite3* db, int id);
int checkVuelo(int id);
//
int callback_nobook(void* data, int argc, char** argv, char** azColName);
int exec_nobook(DBStruct* dbs, char* sql, char* err, sqlite3* db, char* fecha, int id_vuelo, int fila, int col);
int checkNoBook(char* fecha, int id_vuelo, int fila, int col);
//
int callback_newbook(void* data, int argc, char** argv, char** azColName);
int exec_newbook(DBStruct* dbs, char* sql, char* err, sqlite3* db, int id_pasajero, char* fecha, int id_vuelo, int fila, int col);
int insertNewBook(int id_pasajero, char* fecha, int id_vuelo, int fila, int col);
//
int callback_cancelbook(void* data, int argc, char** argv, char** azColName);
int exec_cancelbook(DBStruct* dbs, char* sql, char* err, sqlite3* db, int id_pasajero, char* fecha, int id_vuelo, int fila, int col);
int exec_insertcancelbook(DBStruct* dbs, char* sql, char* err, sqlite3* db, int id_pasajero, char* fecha, int id_vuelo, int fila, int col);
int checkCancelBook(int id_pasajero, char* fecha, int id_vuelo, int fila, int col);
//
int callback_flightstate(void* data, int argc, char** argv, char** azColName);
int exec_flightstate(DBStruct* dbs, char* sql, char* err, sqlite3* db, char* fecha, int id_vuelo);
DBStruct* getFlightState(char* fecha, int id_vuelo);
//
int callback_allflights(void* data, int argc, char** argv, char** azColName);
int exec_allflights(DBStruct* dbs, char* sql, char* err, sqlite3* db);
DBStruct* getAllFlights();
//
int insertNewFlight(int id, char* from, char* to);
int exec_newflight(DBStruct* dbs, char* sql, char* err, sqlite3* db, int id, char* from, char* to);
int callback_accepted(void* data, int argc, char** argv, char** azColName);
//
int exec_newuser(DBStruct* dbs, char* sql, char* err, sqlite3* db, char* nombre, char* apellido, int id);
int insertNewUser(char* nombre, char* apellido, int id);
//
int exec_deluser(DBStruct* dbs, char* sql, char* err, sqlite3* db, int id);
int deleteUser(int id);
//
DBStruct* getAllUsers();
int callback_allusers(void* data, int argc, char** argv, char** azColName);
int exec_allusers(DBStruct* dbs, char* sql, char* err, sqlite3* db);
//
int checkConnectivity(int rc, char* err, sqlite3* db );