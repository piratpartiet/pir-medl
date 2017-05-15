/*
 * environ.c
 * File ID: 9abb8024-391f-11e7-a4b3-f74d993421b0
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
 * get_dbname() - Return pointer to allocated string with location of the 
 * database file. Use the value of opt->dbname if it's defined, otherwise use 
 * the environment variable defined in ENV_DBNAME, otherwise use hardcoded 
 * value STD_DBNAME. If that also fails, return NULL. opt is allowed to be 
 * NULL, it's called by usage().
 */

char *get_dbname(const struct Options *opt)
{
	char *retval = NULL;

	if (opt && opt->dbname) {
		/*
		 * The database file is specified via the -d/--dbname argument.
		 */
		retval = strdup(opt->dbname);
		if (!retval) {
			myerror("get_dbname(): Could not duplicate "
			        "-d/--dbname argument");
			return NULL;
		}
	} else if (getenv(ENV_DBNAME)) {
		/*
		 * Read database name from a dedicated environment variable.
		 */
		retval = strdup(getenv(ENV_DBNAME));
		if (!retval) {
			myerror("get_dbname(): Could not duplicate %s "
			        "environment variable", ENV_DBNAME);
			return NULL;
		}
	} else if (getenv("HOME")) {
		/*
		 * Use default hardcoded value.
		 */
		int size = strlen(getenv("HOME")) +
		           strlen(STD_DBNAME) + 1;

		retval = malloc(size + 1);
		if (!retval) {
			myerror("get_dbname(): Cannot allocate %lu bytes",
			        size);
			return NULL;
		}
		snprintf(retval, size, STD_DBNAME, getenv("HOME"));
	} else {
		fprintf(stderr, "%s: $%s and $HOME environment "
		                "variables are not defined, cannot "
		                "create dbname path\n", progname, ENV_DBNAME);
		return NULL;
	}

	return retval;
}

/* vim: set ts=8 sw=8 sts=8 noet fo+=w tw=79 fenc=UTF-8 : */
