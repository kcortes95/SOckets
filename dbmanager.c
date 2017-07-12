#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "includes/sqlite3.h"
#include "dbmanager.h"

/* --- CHEQUEO DE EXISTENCIA DE USUARIO --- */

int callback(void* data, int argc, char** argv, char** azColName){
	int i;

	DBStruct* dbs_aux = (DBStruct*) data;
	dbs_aux->q = atoi(argv[0]);  

	return ACCEPTED;
}

int exec_checkUser(DBStruct* dbs, char* sql, char* err, sqlite3* db, int id){
	sql = "select count(id) from cliente where id = ";

	char* sql_aux = malloc(sizeof(char) * MAX_SQL_QUERY);
	sprintf(sql_aux,"%s%d",sql, id);

	puts(sql_aux);

	int rc = sqlite3_exec(db, sql_aux, callback, dbs, &err);

	return checkConnectivity(rc, err, db);

}

int checkUser(int id){
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
	exec_checkUser(dbs, sql, err, db, id);

	sqlite3_close(db);

	if (dbs->q == 0){
		return DENNIED;
	}else{
		return ACCEPTED;
	}

}

/* --- FIN CHEQUEO DE EXISTENCIA DE USUARIO --- */

/* --- CHEQUEO DE EXISTENCIA DE VUELO --- */
int exec_checkVuelo(DBStruct* dbs, char* sql, char* err, sqlite3* db, int id){
	sql = "select count(id_vuelo) from vuelos where id_vuelo = ";

	char* sql_aux = malloc(sizeof(char) * MAX_SQL_QUERY);
	sprintf(sql_aux,"%s%d",sql, id);

	puts(sql_aux);

	int rc = sqlite3_exec(db, sql_aux, callback, dbs, &err);

	return checkConnectivity(rc, err, db);

}

int checkVuelo(int id){
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
	exec_checkVuelo(dbs, sql, err, db, id);

	sqlite3_close(db);

	if (dbs->q == 0){
		return DENNIED;
	}else{
		return ACCEPTED;
	}

}

/* --- FIN CHEQUEO DE EXISTENCIA DE VUELO --- */

/* --- CHEQUEO DE EXISTENCIA DE RESERVA --- */

int exec_nobook(DBStruct* dbs, char* sql, char* err, sqlite3* db, char* fecha, int id_vuelo, int fila, int col){
	sql = "select count(id_vuelo) from asientos where";

	char* sql_aux = malloc(sizeof(char) * MAX_SQL_QUERY);
	sprintf(sql_aux,"%s id_vuelo = %d and f = %d and c = %d and fecha = \"%s\"", sql, id_vuelo, fila, col, fecha);

	puts(sql_aux);

	int rc = sqlite3_exec(db, sql_aux, callback, dbs, &err);

	return checkConnectivity(rc, err, db);

}

int checkNoBook(char* fecha, int id_vuelo, int fila, int col){
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
	exec_nobook(dbs, sql, err, db, fecha, id_vuelo, fila, col);

	sqlite3_close(db);

	if (dbs->q == 0){
		return ACCEPTED;
	}else{
		return DENNIED;
	}

}

/* --- FIN CHEQUEO DE EXISTENCIA DE RESERVA --- */

/* --- INSERTO NUEVA RESERVA --- */

int callback_newbook(void* data, int argc, char** argv, char** azColName){
	return ACCEPTED;
}

int exec_newbook(DBStruct* dbs, char* sql, char* err, sqlite3* db, int id_pasajero, char* fecha, int id_vuelo, int fila, int col){
	sql = "insert into asientos values";

	char* sql_aux = malloc(sizeof(char) * MAX_SQL_QUERY);
	sprintf(sql_aux,"%s (%d,%d,%d,\"%s\",\"OCUPADO\",%d)", sql, id_vuelo, fila, col, fecha, id_pasajero);

	puts(sql_aux);

	int rc = sqlite3_exec(db, sql_aux, callback_newbook, dbs, &err);

	return checkConnectivity(rc, err, db);

}

int insertNewBook(int id_pasajero, char* fecha, int id_vuelo, int fila, int col){
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
	exec_newbook(dbs, sql, err, db, id_pasajero, fecha, id_vuelo, fila, col);

	sqlite3_close(db);

	if (dbs->q == 0){
		return ACCEPTED;
	}else{
		return DENNIED;
	}

}

/* --- FIN INSERTO NUEVA RESERVA --- */

/* --- CHEQUEO DE EXISTENCIA DE RESERVA 2 --- */

int exec_ccancelbook(DBStruct* dbs, char* sql, char* err, sqlite3* db, int id_pasajero, char* fecha, int id_vuelo, int fila, int col){
	sql = "select count(id_vuelo) from asientos where";

	char* sql_aux = malloc(sizeof(char) * MAX_SQL_QUERY);
	sprintf(sql_aux,"%s id_vuelo = %d and id_cliente = %d and f = %d and c = %d and fecha = \"%s\"", sql, id_vuelo, id_pasajero, fila, col, fecha);

	puts(sql_aux);

	int rc = sqlite3_exec(db, sql_aux, callback, dbs, &err);

	return checkConnectivity(rc, err, db);

}

int checkCancelBook(int id_pasajero, char* fecha, int id_vuelo, int fila, int col){
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
	exec_ccancelbook(dbs, sql, err, db, id_pasajero, fecha, id_vuelo, fila, col);

	sqlite3_close(db);

	if (dbs->q != 0){
		return ACCEPTED;
	}else{
		return DENNIED;
	}

}

/* --- FIN CHEQUEO DE EXISTENCIA DE RESERVA 2 --- */

/* --- CANCELAR LA RESERVA --- */

int exec_cancelbook(DBStruct* dbs, char* sql, char* err, sqlite3* db, int id_pasajero, char* fecha, int id_vuelo, int fila, int col){
	sql = "delete from asientos where";

	char* sql_aux = malloc(sizeof(char) * MAX_SQL_QUERY);
	sprintf(sql_aux,"%s id_vuelo = %d and id_cliente = %d and f = %d and c = %d and fecha = \"%s\"", sql, id_vuelo, id_pasajero, fila, col, fecha);

	puts(sql_aux);

	int rc = sqlite3_exec(db, sql_aux, callback, dbs, &err);

	return checkConnectivity(rc, err, db);

}

int exec_insertcancelbook(DBStruct* dbs, char* sql, char* err, sqlite3* db, int id_pasajero, char* fecha, int id_vuelo, int fila, int col){
	sql = "delete from asientos where";

	char* sql_aux = malloc(sizeof(char) * MAX_SQL_QUERY);
	sprintf(sql_aux,"insert into cancelados values (%d,%d,%d,%d)", id_pasajero, id_vuelo, fila, col);

	puts(sql_aux);

	int rc = sqlite3_exec(db, sql_aux, callback, dbs, &err);

	return checkConnectivity(rc, err, db);

}

int cancelBook(int id_pasajero, char* fecha, int id_vuelo, int fila, int col){
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
	exec_cancelbook(dbs, sql, err, db, id_pasajero, fecha, id_vuelo, fila, col);
	exec_insertcancelbook(dbs, sql, err, db, id_pasajero, fecha, id_vuelo, fila, col);

	sqlite3_close(db);

	if (dbs->q != 0){
		return ACCEPTED;
	}else{
		return DENNIED;
	}

}

/* --- FIN CANCELAR LA RESERVA  --- */

/* --- DEVUELVE TODOS LOS OCUPADOS DE UN VUELO --- */

int callback_flightstate(void* data, int argc, char** argv, char** azColName){
	int i; //atoi(argv[0]);

	DBStruct* dbs_aux = (DBStruct*) data;

	dbs_aux->ocupados[dbs_aux->tot_elems] = malloc(sizeof(char) * MAX_L);
	sprintf(dbs_aux->ocupados[dbs_aux->tot_elems],"%s:%s",argv[0], argv[1]);
	//puts(dbs_aux->ocupados[dbs_aux->tot_elems]);
	dbs_aux->tot_elems++;

	return ACCEPTED;
}

int exec_flightstate(DBStruct* dbs, char* sql, char* err, sqlite3* db, char* fecha, int id_vuelo){
	sql = "select f, c from asientos where";

	char* sql_aux = malloc(sizeof(char) * MAX_SQL_QUERY);
	sprintf(sql_aux,"%s id_vuelo = %d and fecha = \"%s\"", sql, id_vuelo, fecha);

	puts(sql_aux);

	int rc = sqlite3_exec(db, sql_aux, callback_flightstate, dbs, &err);

	return checkConnectivity(rc, err, db);

}

DBStruct* getFlightState(char* fecha, int id_vuelo){
	sqlite3* db;
	int rc = sqlite3_open(DB_NAME,&db);
	char* err;
	char* sql;

	if( rc != SQLITE_OK ){
		fprintf(stderr, "Error al abrir DB : %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return NULL;
	}

	DBStruct* dbs = malloc(sizeof(DBStruct));
	dbs->tot_elems = 0;

	exec_flightstate(dbs, sql, err, db, fecha, id_vuelo);

	sqlite3_close(db);

	return dbs;

}

/* --- FIN DEVUELVE TODOS LOS OCUPADOS DE UN VUELO --- */

/* --- DEVUELVE TODOS LOS VUELOS --- */

int callback_allflights(void* data, int argc, char** argv, char** azColName){
	int i; //atoi(argv[0]);

	DBStruct* dbs_aux = (DBStruct*) data;

	dbs_aux->ocupados[dbs_aux->tot_elems] = malloc(sizeof(char) * MAX_L);
	sprintf(dbs_aux->ocupados[dbs_aux->tot_elems],"ID: %s - %s - %s - %s",argv[0], argv[1], argv[2], argv[3]);
	//puts(dbs_aux->ocupados[dbs_aux->tot_elems]);
	dbs_aux->tot_elems++;

	return ACCEPTED;
}

int exec_allflights(DBStruct* dbs, char* sql, char* err, sqlite3* db){
	sql = "select * from vuelos";

	puts(sql);

	int rc = sqlite3_exec(db, sql, callback_allflights, dbs, &err);

	return checkConnectivity(rc, err, db);

}

DBStruct* getAllFlights(){
	sqlite3* db;
	int rc = sqlite3_open(DB_NAME,&db);
	char* err;
	char* sql;

	if( rc != SQLITE_OK ){
		fprintf(stderr, "Error al abrir DB : %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return NULL;
	}

	DBStruct* dbs = malloc(sizeof(DBStruct));
	dbs->tot_elems = 0;

	exec_allflights(dbs, sql, err, db);

	sqlite3_close(db);

	return dbs;

}

/* --- FIN DEVUELVE TODOS LOS VUELOS --- */

/* --- INSERTO NUEVO VUELO --- */

int callback_accepted(void* data, int argc, char** argv, char** azColName){
	return ACCEPTED;
}

int exec_newflight(DBStruct* dbs, char* sql, char* err, sqlite3* db, int id, char* from, char* to, char* aerolinea){
	sql = "insert into vuelos values";

	char* sql_aux = malloc(sizeof(char) * MAX_SQL_QUERY);
	sprintf(sql_aux,"%s (%d,\"%s\",\"%s\",\"%s\")", sql, id, aerolinea, from, to);

	puts(sql_aux);

	int rc = sqlite3_exec(db, sql_aux, callback_newbook, dbs, &err);

	return checkConnectivity(rc, err, db);

}

int insertNewFlight(int id, char* from, char* to, char* aerolinea){
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
	exec_newflight(dbs, sql, err, db, id, from, to, aerolinea);

	sqlite3_close(db);

	if (dbs->q == 0){
		return ACCEPTED;
	}else{
		return DENNIED;
	}

}

/* --- FIN INSERTO NUEVO VUELO --- */

/* --- ELIMINAR VUELO --- */

int exec_delflight(DBStruct* dbs, char* sql, char* err, sqlite3* db, int id){
	sql = "delete from vuelos where";

	char* sql_aux = malloc(sizeof(char) * MAX_SQL_QUERY);
	sprintf(sql_aux,"%s id_vuelo = %d", sql, id);

	puts(sql_aux);

	int rc = sqlite3_exec(db, sql_aux, callback_newbook, dbs, &err);

	return checkConnectivity(rc, err, db);

}

int deleteFlight(int id){
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
	exec_delflight(dbs, sql, err, db, id);

	sqlite3_close(db);

	if (dbs->q == 0){
		return ACCEPTED;
	}else{
		return DENNIED;
	}

}

/* --- FIN ELIMINAR VUELO --- */



/* --- INSERTO NUEVO USUARIO --- */


int exec_newuser(DBStruct* dbs, char* sql, char* err, sqlite3* db, char* nombre, char* apellido, int id){
	sql = "insert into cliente values";

	char* sql_aux = malloc(sizeof(char) * MAX_SQL_QUERY);
	sprintf(sql_aux,"%s (%d,\"%s\",\"%s\")", sql, id, nombre, apellido);

	puts(sql_aux);

	int rc = sqlite3_exec(db, sql_aux, callback_newbook, dbs, &err);

	return checkConnectivity(rc, err, db);

}

int insertNewUser(char* nombre, char* apellido, int id){
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
	exec_newuser(dbs, sql, err, db, nombre, apellido, id);

	sqlite3_close(db);

	if (dbs->q == 0){
		return ACCEPTED;
	}else{
		return DENNIED;
	}

}

/* --- FIN INSERTO NUEVO USUARIO --- */

/* --- ELIMINAR USUARIO --- */

int exec_deluser(DBStruct* dbs, char* sql, char* err, sqlite3* db, int id){
	sql = "delete from cliente where";

	char* sql_aux = malloc(sizeof(char) * MAX_SQL_QUERY);
	sprintf(sql_aux,"%s id = %d", sql, id);

	puts(sql_aux);

	int rc = sqlite3_exec(db, sql_aux, callback_newbook, dbs, &err);

	return checkConnectivity(rc, err, db);

}

int deleteUser(int id){
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
	exec_deluser(dbs, sql, err, db, id);

	sqlite3_close(db);

	if (dbs->q == 0){
		return ACCEPTED;
	}else{
		return DENNIED;
	}

}

/* --- FIN ELIMINAR USUARIO --- */

/* --- DEVUELVE TODOS LOS USUARIOS --- */

int callback_allusers(void* data, int argc, char** argv, char** azColName){
	int i; //atoi(argv[0]);

	DBStruct* dbs_aux = (DBStruct*) data;

	dbs_aux->ocupados[dbs_aux->tot_elems] = malloc(sizeof(char) * MAX_L);
	sprintf(dbs_aux->ocupados[dbs_aux->tot_elems],"ID: %s - %s - %s",argv[0], argv[1], argv[2]);
	dbs_aux->tot_elems++;

	return ACCEPTED;
}

int exec_allusers(DBStruct* dbs, char* sql, char* err, sqlite3* db){
	sql = "select * from cliente";

	puts(sql);

	int rc = sqlite3_exec(db, sql, callback_allusers, dbs, &err);

	return checkConnectivity(rc, err, db);

}

DBStruct* getAllUsers(){
	sqlite3* db;
	int rc = sqlite3_open(DB_NAME,&db);
	char* err;
	char* sql;

	if( rc != SQLITE_OK ){
		fprintf(stderr, "Error al abrir DB : %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return NULL;
	}

	DBStruct* dbs = malloc(sizeof(DBStruct));
	dbs->tot_elems = 0;

	exec_allusers(dbs, sql, err, db);

	sqlite3_close(db);

	return dbs;

}

/* --- FIN DEVUELVE TODOS LOS USUARIOS --- */

int checkConnectivity(int rc, char* err, sqlite3* db ){
	if(rc != SQLITE_OK){
		fprintf(stderr, "Failed\n");
		fprintf(stderr, "SQL Error : %s\n",err);
		sqlite3_free(err);
		sqlite3_close(db);
		return DENNIED;
	}else{
		return ACCEPTED;
	}

	return ACCEPTED;
}