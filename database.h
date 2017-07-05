
#ifndef DATABASE_INCLUDED
#define DATABASE_H_INCLUDED

struct Info{
	char* from[100];
	char* to[100];
	char* status[50];	
} info;

//Funciones para utilizarse en el lado del cliente.
int book(sqlite3* db, char * err, int id_flight, int f, int c, char* fecha, int id_cliente);
int removeBook(sqlite3* db, char * err, int id_flight, int id_client);
int check(sqlite3* db, char * err, int id_flight,struct Info* inf);

//Funciones utilizadas internamente.
int print_dav(sqlite3* db, char * err, int id_flight);
int print_plane(void* NotUsed, int argc, char** argv, char** azColName);
int print_info(void* NotUsed, int argc, char** argv, char** azColName);
void clean(char c[]);

int valid(void* NotUsed, int argc, char** argv, char** azColName);
int checkFlight(sqlite3* db, char* err, int id_flight);
int checkUser(sqlite3* db, char* err, int id_cli);

#endif
