#include <stdio.h>

#define MAX_ARRAY 2000
#define MAX_STRING 2000
#define MAX_FIL 50
#define MAX_COL 6
#define MAX_LONG_FECHA 11
#define MAX_INPUT_STRING 25

typedef enum {BOOKFLIGHT, CANCELFLIGHT, AVAILABLEFLIGHTS, AVAILABLESINFLIGHT, EXIT, NEWF, DELF, NEWU, DELU, SHOWU} RQType;
typedef enum {BOOKOK, BOOKNO, CANCELOK, CANCELNO, AFOK, AFNO, AINFOK, AINFNO, NEWFOK, DELFOK, NEWUOK, DELUOK, NEWFNO, DELFNO, NEWUNO, DELUNO, SHOWUOK, SHOWUNO} RSType;

typedef struct {
	unsigned int requestID;
	unsigned int userID;
	RQType t;
	char nombre[MAX_STRING];
	char apellido[MAX_STRING];
	char fecha[MAX_LONG_FECHA];
	char from[MAX_STRING];
	char to[MAX_STRING];
	int id_db;
	int fila;
	int col;
	int id_vuelo;
} ClientMessage;

typedef struct {
	unsigned int requestID;
	unsigned int userID;
	RSType t;
	char msg[MAX_STRING];
	int rando;
} ServerMessage;