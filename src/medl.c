/*
 * medl.c
 * File ID: debbf69c-390a-11e7-9169-f74d993421b0
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
 * Global variables
 */

char *progname;

/*
 * verbose_level() - Get or set the verbosity level. If action is 0, return the 
 * current level. If action is non-zero, set the level to argument 2 and return 
 * the new level.
 */

int verbose_level(const int action, ...)
{
	static int level = 0;

	if (action) {
		va_list ap;

		va_start(ap, action);
		level = va_arg(ap, int);
		va_end(ap);
	}

	return level;
}

/*
 * msg() - Print a message prefixed with "[progname]: " to stddebug if the 
 * current verbose level is equal or higher than the first argument. The rest 
 * of the arguments are delivered to vfprintf().
 * Returns the number of characters written.
 */

int msg(const int verbose, const char *format, ...)
{
	va_list ap;
	int retval = 0;

	assert(format);
	assert(strlen(format));

	if (verbose_level(0) >= verbose) {
		va_start(ap, format);
		retval = fprintf(stddebug, "%s: ", progname);
		retval += vfprintf(stddebug, format, ap);
		retval += fprintf(stddebug, "\n");
		va_end(ap);
	}

	return retval;
}

/*
 * myerror() - Print an error message to stderr using this format:
 *   a: b: c
 * where a is the name of the program (progname), b is the output from the 
 * printf-like string and optional arguments, and c is the error message from 
 * errno. Returns the number of characters written.
 */

int myerror(const char *format, ...)
{
	va_list ap;
	int retval = 0;
	int orig_errno = errno;

	assert(format);
	assert(strlen(format));

	retval = fprintf(stderr, "%s: ", progname);
	va_start(ap, format);
	retval += vfprintf(stderr, format, ap);
	va_end(ap);
	if (orig_errno)
		retval += fprintf(stderr, ": %s", strerror(orig_errno));
	retval += fprintf(stderr, "\n");

	return retval;
}

/*
 * print_license() - Display the program license. Returns EXIT_SUCCESS.
 */

int print_license(void)
{
	puts("(C)opyleft 2017- Øyvind A. Holm <sunny@sunbase.org>");
	puts("");
	puts("This program is free software; you can redistribute it "
	     "and/or modify it \n"
	     "under the terms of the GNU General Public License as "
	     "published by the \n"
	     "Free Software Foundation; either version 2 of the License, "
	     "or (at your \n"
	     "option) any later version.");
	puts("");
	puts("This program is distributed in the hope that it will be "
	     "useful, but \n"
	     "WITHOUT ANY WARRANTY; without even the implied warranty of \n"
	     "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.");
	puts("See the GNU General Public License for more details.");
	puts("");
	puts("You should have received a copy of "
	     "the GNU General Public License along \n"
	     "with this program. If not, see "
	     "<http://www.gnu.org/licenses/>.");

	return EXIT_SUCCESS;
}

/*
 * print_version() - Print version information on stdout. Returns EXIT_SUCCESS.
 */

int print_version(void)
{
	printf("%s %s (%s)\n", progname, VERSION, RELEASE_DATE);
	printf("Linked against SQLite %s\n", sqlite3_libversion());

	return EXIT_SUCCESS;
}

/*
 * usage() - Prints a help screen. Returns retval.
 */

int usage(const int retval)
{
	char *logdir = get_dbname(NULL);

	if (retval != EXIT_SUCCESS) {
		fprintf(stderr, "\nType \"%s --help\" for help screen. "
		                "Returning with value %d.\n",
		                progname, retval);
		return retval;
	}
	puts("");
	if (verbose_level(0) >= 1) {
		print_version();
		puts("");
	}
	printf("Usage: %s [options]\n", progname);
	printf("\n");
	printf("Options:\n");
	printf("\n");
	printf("  -d FILE, --dbname FILE\n"
	       "    Use FILE as database instead of the default value.\n"
	       "    Current value: %s\n", logdir);
	printf("  -h, --help\n"
	       "    Show this help.\n");
	printf("  --license\n"
	       "    Print the software license.\n");
	printf("  -q, --quiet\n"
	       "    Be more quiet. Can be repeated to increase silence.\n");
	printf("  -v, --verbose\n"
	       "    Increase level of verbosity. Can be repeated.\n");
	printf("  --version\n"
	       "    Print version information.\n");
	printf("\n");
	printf("Subcommands:\n");
	printf("\n");
	printf("  diag\n"
	       "    Display various information about the system.\n");
	printf("  init\n"
	       "    Initialise the SQLite database if it doesn't exist.\n");
	printf("\n");
	free(logdir);

	return retval;
}

/*
 * choose_opt_action() - Decide what to do when option c is found. Store 
 * changes in dest. opts is the struct with the definitions for the long 
 * options.
 * Return EXIT_SUCCESS if ok, EXIT_FAILURE if c is unknown or anything fails.
 */

int choose_opt_action(struct Options *dest,
                      const int c, const struct option *opts)
{
	int retval = EXIT_SUCCESS;

	assert(dest);
	assert(opts);

	switch (c) {
	case 0:
		if (!strcmp(opts->name, "license"))
			dest->license = TRUE;
		else if (!strcmp(opts->name, "version"))
			dest->version = TRUE;
		break;
	case 'd':
		dest->dbname = optarg;
		break;
	case 'h':
		dest->help = TRUE;
		break;
	case 'q':
		dest->verbose--;
		break;
	case 'v':
		dest->verbose++;
		break;
	default:
		msg(3, "getopt_long() returned character code %d", c);
		retval = EXIT_FAILURE;
		break;
	}

	return retval;
}

/*
 * parse_options() - Parse command line options.
 * Returns EXIT_SUCCESS if ok, EXIT_FAILURE if error.
 */

int parse_options(struct Options *dest, const int argc, char * const argv[])
{
	int retval = EXIT_SUCCESS;
	int c;

	assert(dest);
	assert(argv);

	dest->dbname = NULL;
	dest->help = FALSE;
	dest->license = FALSE;
	dest->verbose = 0;
	dest->version = FALSE;

	while (retval == EXIT_SUCCESS) {
		int option_index = 0;
		static struct option long_options[] = {
			{"dbname", required_argument, 0, 'd'},
			{"help", no_argument, 0, 'h'},
			{"license", no_argument, 0, 0},
			{"quiet", no_argument, 0, 'q'},
			{"verbose", no_argument, 0, 'v'},
			{"version", no_argument, 0, 0},
			{0, 0, 0, 0}
		};

		c = getopt_long(argc, argv,
		                "d:" /* --dbname */
		                "h"  /* --help */
		                "q"  /* --quiet */
		                "v"  /* --verbose */
		                , long_options, &option_index);

		if (c == -1)
			break;

		retval = choose_opt_action(dest,
		                           c, &long_options[option_index]);
	}
	verbose_level(1, dest->verbose);

	return retval;
}

/*
 * do_your_thing_please() - Do everything in one place. Returns EXIT_SUCCESS or 
 * EXIT_FAILURE.
 */

int do_your_thing_please(const struct Options *opt,
                         const int argc, char * const argv[])
{
	struct Rc rc;
	int retval = EXIT_SUCCESS;

	rc.dbname = get_dbname(opt);
	if (!rc.dbname)
		return EXIT_FAILURE;
	msg(3, "optind = %d", optind);
	msg(3, "argc = %d, argv[optind] = \"%s\"", argc, argv[optind]);
	msg(3, "rc.dbname = \"%s\"", rc.dbname);
	msg(3, "opt.dbname = \"%s\"", opt->dbname);

	if (argv[optind]) {
		const char *cmd = argv[optind];

		msg(3, "argv[optind] is not NULL, setting cmd = \"%s\"", cmd);
		if (!strcmp(argv[optind], CMD_DIAG)) {
			printf("rc.dbname = \"%s\"\n", rc.dbname);
		} else if (!strcmp(argv[optind], CMD_INIT)) {
			retval = init_db(rc.dbname);
		} else {
			myerror("%s: Unknown command", cmd);
			retval = EXIT_FAILURE;
		}
	} else {
		msg(3, "argv[optind] is NULL");
		myerror("No command specified");
		retval = EXIT_FAILURE;
	}

	free(rc.dbname);

	return retval;
}

/*
 * main()
 */

int main(int argc, char *argv[])
{
	int retval;
	struct Options opt;

	progname = argv[0];

	if (parse_options(&opt, argc, argv) != EXIT_SUCCESS) {
		myerror("Option error");
		return EXIT_FAILURE;
	}

	msg(3, "Using verbose level %d", verbose_level(0));

	if (opt.help)
		return usage(EXIT_SUCCESS);
	if (opt.version)
		return print_version();
	if (opt.license)
		return print_license();

	retval = do_your_thing_please(&opt, argc, argv);

	msg(3, "Returning from main() with value %d", retval);
	return retval;
}

/* vim: set ts=8 sw=8 sts=8 noet fo+=w tw=79 fenc=UTF-8 : */
