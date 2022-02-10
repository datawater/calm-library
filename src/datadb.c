#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "include/datadb.h"

//TODO(#1): Fully implement datadb

bool file_exists(char *fname) {
    if (access(fname, F_OK) == 0) {
        return true;
    } else {
        return false;
    }
}  

datadb_t datadb_init(datadb_t db) {

    if (file_exists(db.fname)) {
        db.STATUS = DATADB_FILE_EXIST;
        return db;
    }

    db.db_file = fopen(db.fname, "wb");
    FILE* fp = db.db_file;

    if (fp == NULL) {
        db.STATUS = DATADB_ERROR;
        return db;
    }

    fwrite("DATASQL_1\01", sizeof(char), 10, fp);
    fclose(fp);
    
    db.STATUS = DATADB_OK;
    db.INIT_DONE = true;
    return db;

}
