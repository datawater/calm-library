//TODO: Fully implement datadb

#ifndef DATADB_H
#define DATADB_H

#define DATADB_OK 30 // 0 in hex
#define DATADB_ERROR 31 //1 in hex
#define DATADB_FILE_EXIST 45 //E in hex
#define DATADB_FILE_NOT_EXIST 4445 //DE in hex

struct datadb_t{
    char *fname;
    FILE* db_file;
    bool INIT_DONE;
    int STATUS;
};

typedef struct datadb_t datadb_t;

extern datadb_t datadb_init(datadb_t db_name);

#endif