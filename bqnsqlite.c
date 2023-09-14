#include <stdlib.h>
#include <stdio.h>
#include "bqnffi.h"
#include <sqlite3.h>
#include <stdint.h>

// TODO: temporary globals, remove when we actually track
sqlite3 *con; // sqlite3 connection

// TODO: remove, for testing BQN FFI
BQNV timesTen(BQNV v) {
    size_t len = bqn_bound(v);
    int32_t* buf = malloc(len*4);
    bqn_readI32Arr(v, buf);
    for (unsigned int i = 0; i < len; i++) buf[i] = buf[i] * 10;
    BQNV res = bqn_makeI32Vec(len, buf);
    free(buf);
    bqn_free(v);
    return res;
}

// TODO does this leak if we pass an argument and don't use it?
void bqnsqlite_open(void)
{
    // can be called multiple times, so it seems safe to do it here
    bqn_init();
    char* path = "/home/peter/src/bqnsqlite/test.db";
    const int err = sqlite3_open_v2(path, &con, SQLITE_OPEN_READONLY, NULL);
    if (SQLITE_OK == err)
    {
	fprintf(stderr, "opened %s\n", path);
    }
    else
    {
	fprintf(stderr, "error %s: %s\n", path, sqlite3_errmsg(con));
    }
}

BQNV read_x(void)
{
    char* query = "select x from test;";
    sqlite3_stmt* stmt;

    if (NULL == con)
    {
	fprintf(stderr, "con is null");
	return;
    }
    
    if (SQLITE_OK != sqlite3_prepare_v2(con, query, -1, &stmt, NULL))
    {
	fprintf(stderr, "error preparing statement: %s\n", sqlite3_errmsg(con));
	return;
    }

    int x[1024];
    int column_count = sqlite3_column_count(stmt);

    int i = 0;
    while (SQLITE_DONE != sqlite3_step(stmt))
    {	
	x[i] = sqlite3_column_int(stmt, 0);
	++i;
    }

    sqlite3_finalize(stmt);

    // we've copied up to 1024 values into x
    return bqn_makeI32Vec(i, &x);
}

// right now can only allocate up to 1024 integers of a single column
BQNV do_query(char* query)
{
    sqlite3_stmt* stmt;

    if (NULL == con)
    {
	fprintf(stderr, "con is null");
	return 0;
    }
    
    if (SQLITE_OK != sqlite3_prepare_v2(con, query, -1, &stmt, NULL))
    {
	fprintf(stderr, "error preparing statement: %s\n", sqlite3_errmsg(con));
	return 0;
    }

    int x[1024];
    int column_count = sqlite3_column_count(stmt);

    int i = 0;
    while (SQLITE_DONE != sqlite3_step(stmt))
    {	
	x[i] = sqlite3_column_int(stmt, 0);
	++i;
    }

    sqlite3_finalize(stmt);

    // we've copied up to 1024 values into x
    return bqn_makeI32Vec(i, &x);
}

BQNV bqnsqlite_select(BQNV query)
{
    const size_t rank = bqn_rank(query);
    if (1 != rank)
    {
	fprintf(stderr, "query is not of rank 1");
	return;
    }
    const size_t query_len = bqn_bound(query);
    uint8_t buf[128];
    bqn_readC8Arr(query, buf);
    buf[query_len] = '\0'; // null terminate the query
    return do_query(buf);
}
