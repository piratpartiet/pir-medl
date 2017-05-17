/*
 * database.c
 * File ID: 6eac22c6-3aec-11e7-b974-f74d993421b0
 *
 * (C)opyleft 2017- Øyvind A. Holm <sunny@sunbase.org>
 *
 * This program is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License as published by the Free 
 * Software Foundation; either version 2 of the License, or (at your option) 
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for 
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with 
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "medl.h"

/*
 * open_db() - Open the database and return database pointer. If the database 
 * doesn't exist or any errors occur, return NULL.
 */

sqlite3 *open_db(const char *dbfile)
{
	int result;
	sqlite3 *db;

	assert(dbfile);
	if (!strlen(dbfile)) {
		myerror("Empty database file name");
		return NULL;
	}

	result = sqlite3_open_v2(dbfile, &db,
	                         SQLITE_OPEN_READWRITE, NULL);
	if (result != SQLITE_OK) {
		myerror("Cannot open SQLite database '%s': %s",
		        dbfile, sqlite3_errmsg(db));
		sqlite3_close_v2(db);
		db = NULL;
	}

	return db;
}

/*
 * close_db() - Close the database pointed to by db, ignore NULL. Returns 
 * EXIT_SUCCESS or EXIT_FAILURE.
 */

int close_db(sqlite3 *db)
{
	if (!db)
		return EXIT_SUCCESS;

	if (sqlite3_close_v2(db) != SQLITE_OK) {
		myerror("Error when closing SQlite database: %s",
		        sqlite3_errmsg(db));
		return EXIT_FAILURE;
	}
	db = NULL;

	return EXIT_SUCCESS;
}

/* vim: set ts=8 sw=8 sts=8 noet fo+=w tw=79 fenc=UTF-8 : */
