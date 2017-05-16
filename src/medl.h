/*
 * medl.h
 * File ID: decca9ba-390a-11e7-9f45-f74d993421b0
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

#ifndef _medl_add_H
#define _medl_add_H

/*
 * Defines
 */

#define VERSION       "0.0.0"
#define RELEASE_DATE  "2017-00-00"

#define FALSE  0
#define TRUE   1

#define T_RESET  "\x1b[m\x0f"
#define T_RED    "\x1b[31m"
#define T_GREEN  "\x1b[32m"

#define stddebug  stderr

#undef NDEBUG

#define DB_VERSION  0 /* Increase if incompatible changes to the database */
#define ENV_DBNAME  "MEDL_DB"
#define STD_DBNAME  "%s/.pir-medl/pir-medl.sqlite"

/*
 * All commands specified on the command line must be referenced via the 
 * appropriate CMD_* macros to avoid hardcoding.
 */
#define CMD_DIAG  "diag"

/*
 * Standard header files
 */

#include "sqlite3.h"

#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * Macros
 */

#define DEBL  msg(2, "%s, line %u in %s()", __FILE__, __LINE__, __func__)
#define in_range(a,b,c)  ((a) >= (b) && (a) <= (c) ? TRUE : FALSE)

/*
 * Typedefs
 */

typedef unsigned char bool;
struct Options {
	char *dbname;
	bool help;
	bool license;
	int verbose;
	bool version;
};
struct Rc {
	char *dbname;
};

/*
 * Function prototypes
 */

#if 1 /* Set to 0 to test without prototypes */

/* dbinit.c */
extern int init_db(const char *dbfile);

/* environ.c */
extern char *get_dbname(const struct Options *opt);

/* medl.c */
extern int verbose_level(const int action, ...);
extern int msg(const int verbose, const char *format, ...);
extern int myerror(const char *format, ...);
extern int print_license(void);
extern int print_version(void);
extern int usage(const int retval);
extern int choose_opt_action(struct Options *dest,
                             const int c, const struct option *opts);
extern int parse_options(struct Options *dest,
                         const int argc, char * const argv[]);
extern int do_your_thing_please(const struct Options *opt,
                                const int argc, char * const argv[]);

#endif

/*
 * Global variables
 */

extern char *progname;

#endif /* ifndef _medl_add_H */

/* vim: set ts=8 sw=8 sts=8 noet fo+=w tw=79 fenc=UTF-8 : */
