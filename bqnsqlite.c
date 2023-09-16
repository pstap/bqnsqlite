#include <stdlib.h>
#include <stdio.h>
#include "bqnffi.h"
#include <sqlite3.h>
#include <stdint.h>

// DB pool
#define MAX_HANDLES 4096U

static sqlite3* connections[MAX_HANDLES] = {NULL};

#define INVALID_HANDLE (-1)

typedef int64_t DB_HANDLE;

DB_HANDLE new_handle(sqlite3* db)
{
    static int handle = -1; // -1 to handle first preincrement
    connections[++handle] = db;
    return handle;
}

sqlite3* lookup_handle(DB_HANDLE db_handle)
{
    fprintf(stderr, "lookup_handle: got %ld\n", db_handle);
    
    // negative handles are invalid
    if (0 > db_handle)
    {
	fprintf(stderr, "lookup_handle: handle is negactive\n");
	return NULL;
    }
    else if (db_handle < MAX_HANDLES)
    {
	fprintf(stderr, "lookup_handle: handle looks fine\n");
	return connections[db_handle];
    }
    else
    {
	fprintf(stderr, "lookup_handle: handle outside of range (%d)\n", MAX_HANDLES);
	return NULL;
    }	
}

// TODO does this leak if we pass an argument and don't use it?
DB_HANDLE bqnsqlite_open(BQNV path)
{    
    // can be called multiple times, so it seems safe to do it here
    bqn_init();

    const size_t rank = bqn_rank(path);
    if (1 != rank)
    {
	fprintf(stderr, "query is not of rank 1");
	return INVALID_HANDLE;
    }
    
    const size_t path_len = bqn_bound(path);
    uint8_t buf[256];
    bqn_readC8Arr(path, buf);
    buf[path_len] = '\0'; // null terminate the query


    sqlite3* local_con;
    const int err = sqlite3_open_v2(buf, &local_con, SQLITE_OPEN_READONLY, NULL);

    if (SQLITE_OK == err)
    {
	DB_HANDLE h = new_handle(local_con);
	fprintf(stderr, "open: allocated handle %ld\n", h);
	return h;
    }
    else
    {
	fprintf(stderr, "error %s: %s\n", buf, sqlite3_errmsg(local_con));
	return INVALID_HANDLE;
    }
}

// right now can only allocate up to 1024 integers of a single column
BQNV do_query(sqlite3* db, char* query)
{
    sqlite3_stmt* stmt;
    
    if (NULL == db)
    {
	fprintf(stderr, "db is null");
	return 0;
    }
    
    if (SQLITE_OK != sqlite3_prepare_v2(db, query, -1, &stmt, NULL))
    {
	fprintf(stderr, "error preparing statement: %s\n", sqlite3_errmsg(db));
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

BQNV bqnsqlite_select(DB_HANDLE db_handle, BQNV query)
{
    sqlite3* db = lookup_handle(db_handle);
    if (NULL == db)
    {
	fprintf(stderr, "Could not lookup handle: %ld\n", db_handle);
    }
    
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
    return do_query(db, buf);
}
