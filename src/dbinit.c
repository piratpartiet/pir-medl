/*
 * dbinit.c
 * File ID: 58047254-39b5-11e7-b48a-f74d993421b0
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
 * init_db() - Initialise the SQLite file dbfile with the standard tables and 
 * metadata. Returns EXIT_SUCCESS or EXIT_FAILURE.
 */

int init_db(const char *dbfile)
{
	sqlite3 *db;
	int result;
	char *sql;
	char *errmsg;

	assert(dbfile);
	assert(strlen(dbfile));
	msg(3, "init_db(\"%s\")", dbfile);

	if (access(dbfile, F_OK) != -1) {
		fprintf(stderr, "%s: %s: Database already exists\n",
		                progname, dbfile);
		return EXIT_FAILURE;
	}

	result = sqlite3_open_v2(dbfile, &db,
	                         SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
	                         NULL);
	if (result != SQLITE_OK) {
		myerror("Cannot create SQLite database: %s",
		        sqlite3_errmsg(db));
		sqlite3_close_v2(db);
		return EXIT_FAILURE;
	}

	sql = sqlite3_mprintf(
	    "BEGIN TRANSACTION;\n"
	    "CREATE TABLE meta (\n"
	    "  key TEXT\n"
	    "    UNIQUE\n"
	    "    NOT NULL,\n"
	    "  value JSON\n"
	    "    CONSTRAINT meta_value_valid_json\n"
	    "      CHECK (json_valid(value))\n"
	    ");\n"
	    "INSERT INTO \"meta\" VALUES('dbversion', %u);\n"
	    "INSERT INTO \"meta\" VALUES('program', '\"pir-medl\"');\n"
	    "CREATE TABLE members (\n"
	    "  id INTEGER,\n"
	    "  name TEXT\n"
	    "    CONSTRAINT members_name_length\n"
	    "      CHECK (length(name) > 0)\n"
	    "    NOT NULL,\n"
	    "  address TEXT\n"
	    "  postcode TEXT,\n"
	    "  city TEXT,\n"
	    "  fylke INTEGER\n"
	    "    CONSTRAINT members_fylke_valid\n"
	    "      CHECK (fylke BETWEEN 1 AND 21 AND fylke <> 13)\n"
	    "    NOT NULL,\n"
	    "  email TEXT,\n"
	    "  phone TEXT,\n"
	    "  birthdate TEXT\n"
	    "    CONSTRAINT members_birthdate_length\n"
	    "      CHECK (birthdate IS NULL OR length(birthdate) = 10)\n"
	    "    CONSTRAINT members_birthdate_valid\n"
	    "      CHECK (birthdate IS NULL OR "
	             "datetime(birthdate) IS NOT NULL),\n"
	    "  susp INTEGER\n"
	    "    CONSTRAINT members_susp_valid\n"
	    "      CHECK (susp between 0 AND 2)\n"
	    "    NOT NULL\n"
	    ");\n"
	    "CREATE TABLE payment (\n"
	    "  date TEXT\n"
	    "    CONSTRAINT payment_date_length\n"
	    "      CHECK (date IS NULL OR length(date) = 10)\n"
	    "    CONSTRAINT payment_date_valid\n"
	    "      CHECK (date IS NULL OR datetime(date) IS NOT NULL),\n"
	    "  userid INTEGER\n"
	    ");\n"
	    "COMMIT;\n",
	    DB_VERSION
	);

	result = sqlite3_exec(db, sql, 0, 0, &errmsg);
	if (result != SQLITE_OK ) {
		myerror("init_db(): Cannot CREATE TABLE: %s\n", errmsg);
		sqlite3_free(errmsg);
		sqlite3_close_v2(db);
		return EXIT_FAILURE;
	}

	sqlite3_free(sql);

	result = sqlite3_close_v2(db);
	if (result != SQLITE_OK) {
		myerror("%s: Error when closing database: %s",
		        dbfile, sqlite3_errmsg(db));
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/* vim: set ts=8 sw=8 sts=8 noet fo+=w tw=79 fenc=UTF-8 : */
