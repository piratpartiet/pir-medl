#!/usr/bin/env perl

#==============================================================================
# medl.t
# File ID: b72632ba-3913-11e7-a115-f74d993421b0
#
# Test suite for medl(1).
#
# Character set: UTF-8
# ©opyleft 2017– Øyvind A. Holm <sunny@sunbase.org>
# License: GNU General Public License version 2 or later, see end of file for 
# legal stuff.
#==============================================================================

use strict;
use warnings;

BEGIN {
	use Test::More qw{no_plan};
	# use_ok() goes here
}

use Getopt::Long;

local $| = 1;

our $CMD_BASENAME = "medl";
our $CMD = "../$CMD_BASENAME";

our %Opt = (

	'all' => 0,
	'help' => 0,
	'quiet' => 0,
	'todo' => 0,
	'valgrind' => 0,
	'verbose' => 0,
	'version' => 0,

);

our $progname = $0;
$progname =~ s/^.*\/(.*?)$/$1/;
our $VERSION = '0.0.0';

my %descriptions = ();

Getopt::Long::Configure('bundling');
GetOptions(

	'all|a' => \$Opt{'all'},
	'help|h' => \$Opt{'help'},
	'quiet|q+' => \$Opt{'quiet'},
	'todo|t' => \$Opt{'todo'},
	'valgrind' => \$Opt{'valgrind'},
	'verbose|v+' => \$Opt{'verbose'},
	'version' => \$Opt{'version'},

) || die("$progname: Option error. Use -h for help.\n");

$Opt{'verbose'} -= $Opt{'quiet'};
$Opt{'help'} && usage(0);
if ($Opt{'version'}) {
	print_version();
	exit(0);
}

if ($Opt{'valgrind'}) {
	$CMD = "valgrind -q --leak-check=full --show-leak-kinds=all -- " .
		   "../$CMD_BASENAME";
}

exit(main());

sub main {
	my $Retval = 0;

	diag(sprintf('========== Executing %s v%s ==========',
	             $progname, $VERSION));

	if ($Opt{'todo'} && !$Opt{'all'}) {
		goto todo_section;
	}

	test_standard_options();
	test_executable();

	todo_section:
	;

	if ($Opt{'all'} || $Opt{'todo'}) {
		diag('Running TODO tests...');
		TODO: {
			local $TODO = '';
			# Insert TODO tests here.
		}
	}

	diag('Testing finished.');

	return $Retval;
}

sub test_standard_options {
	diag('Testing -h (--help) option...');
	likecmd("$CMD -h",
	        '/  Show this help/i',
	        '/^$/',
	        0,
	        'Option -h prints help screen');

	diag('Testing -v (--verbose) option...');
	likecmd("$CMD -hv",
	        '/^\n\S+ \d+\.\d+\.\d+/s',
	        '/^$/',
	        0,
	        'Option -v with -h returns version number and help screen');

	diag('Testing --version option...');
	likecmd("$CMD --version",
	        '/^\S+ \d+\.\d+\.\d+/',
	        '/^$/',
	        0,
	        'Option --version returns version number');
	return;
}

sub test_executable {
	diag("Invalid or missing subcommand");
	testcmd("$CMD",
	        "",
	        "../$CMD_BASENAME: No command specified\n",
	        1,
	        'Without arguments or options');
	testcmd("$CMD gurgle",
	        "",
	        "../$CMD_BASENAME: gurgle: Unknown command\n",
	        1,
	        'Specify unknown command');
	testcmd("$CMD ''",
	        "",
	        "../$CMD_BASENAME: : Unknown command\n",
	        1,
	        'Specify empty command');

	test_exec_options();
	test_cmd_diag();
	test_cmd_init();

	return;
}

sub test_exec_options {
	diag("-d/--dbname");
	testcmd("$CMD -d",
	        "",
	        "../$CMD_BASENAME: option requires an argument -- 'd'\n" .
	          "../$CMD_BASENAME: Option error\n",
	        1,
	        '-d without argument');
	testcmd("$CMD --dbname",
	        "",
	        "../$CMD_BASENAME: option '--dbname' requires an argument\n" .
	          "../$CMD_BASENAME: Option error\n",
	        1,
	        '--dbname without argument');
	testcmd("$CMD -d blurfl",
	        "",
	        "../$CMD_BASENAME: No command specified\n",
	        1,
	        '-d with argument but no command');
}

sub test_cmd_diag {
	diag("$CMD_BASENAME diag");
	likecmd("$CMD diag",
	        '/^rc\.dbname = ".+"\n$/s',
	        '/^$/',
	        0,
	        'diag without options');
	testcmd("MEDL_DB=jukmifgguggh $CMD diag",
	        "rc.dbname = \"jukmifgguggh\"\n",
	        "",
	        0,
	        'Use MEDL_DB environment variable');
	testcmd("$CMD diag -d jukmifgguggh",
	        "rc.dbname = \"jukmifgguggh\"\n",
	        "",
	        0,
	        'Use diag -d');
	testcmd("$CMD diag --dbname jukmifgguggh",
	        "rc.dbname = \"jukmifgguggh\"\n",
	        "",
	        0,
	        'Use diag --dbname');
	testcmd("MEDL_DB=nowaynorway $CMD diag --dbname jukmifgguggh",
	        "rc.dbname = \"jukmifgguggh\"\n",
	        "",
	        0,
	        'diag: --dbname has priority over the MEDL_DB envvar');

	return;
}

sub test_cmd_init {
	diag("$CMD_BASENAME init");
	my $tmpdb = 'tmp-db.tmp';

	unlink($tmpdb);
	ok(!-e $tmpdb, "$tmpdb doesn't exist");
	testcmd("$CMD init -d ''",
	        "",
	        "../$CMD_BASENAME: Empty database name specified\n",
	        1,
	        'init: -d with empty argument');
	testcmd("$CMD init -d $tmpdb",
	        "",
	        "",
	        0,
	        "Create $tmpdb with -d");
	ok(-e $tmpdb, "$tmpdb exists");
	testcmd("$CMD init --dbname $tmpdb",
	        "",
	        "../$CMD_BASENAME: $tmpdb: Database already exists\n",
	        1,
	        "Refuse to overwrite $tmpdb with --dbname");
	ok(unlink($tmpdb), "Delete $tmpdb");
	testcmd("MEDL_DB=$tmpdb $CMD init",
	        "",
	        "",
	        0,
	        "Specify $tmpdb via the MEDL_DB envvar");
	testcmd("MEDL_DB=$tmpdb $CMD init",
	        "",
	        "../$CMD_BASENAME: $tmpdb: Database already exists\n",
	        1,
	        "Refuse to overwrite $tmpdb, using MEDL_DB");
	testcmd("MEDL_DB=jukmifgguggh $CMD init -d $tmpdb",
	        "",
	        "../$CMD_BASENAME: $tmpdb: Database already exists\n",
	        1,
	        "init: -d has priority over MEDL_DB");
	ok(unlink($tmpdb), "Delete $tmpdb");

	return;
}

sub testcmd {
	my ($Cmd, $Exp_stdout, $Exp_stderr, $Exp_retval, $Desc) = @_;
	defined($descriptions{$Desc}) &&
		BAIL_OUT("testcmd(): '$Desc' description is used twice");
	$descriptions{$Desc} = 1;
	my $stderr_cmd = '';
	my $cmd_outp_str = $Opt{'verbose'} >= 1 ? "\"$Cmd\" - " : '';
	my $Txt = join('', $cmd_outp_str, defined($Desc) ? $Desc : '');
	my $TMP_STDERR = "$CMD_BASENAME-stderr.tmp";
	my $retval = 1;

	if (defined($Exp_stderr)) {
		$stderr_cmd = " 2>$TMP_STDERR";
	}
	$retval &= is(`$Cmd$stderr_cmd`, $Exp_stdout, "$Txt (stdout)");
	my $ret_val = $?;
	if (defined($Exp_stderr)) {
		$retval &= is(file_data($TMP_STDERR),
		              $Exp_stderr, "$Txt (stderr)");
		unlink($TMP_STDERR);
	} else {
		diag("Warning: stderr not defined for '$Txt'");
	}
	$retval &= is($ret_val >> 8, $Exp_retval, "$Txt (retval)");

	return $retval;
}

sub likecmd {
	my ($Cmd, $Exp_stdout, $Exp_stderr, $Exp_retval, $Desc) = @_;
	defined($descriptions{$Desc}) &&
		BAIL_OUT("likecmd(): '$Desc' description is used twice");
	$descriptions{$Desc} = 1;
	my $stderr_cmd = '';
	my $cmd_outp_str = $Opt{'verbose'} >= 1 ? "\"$Cmd\" - " : '';
	my $Txt = join('', $cmd_outp_str, defined($Desc) ? $Desc : '');
	my $TMP_STDERR = "$CMD_BASENAME-stderr.tmp";
	my $retval = 1;

	if (defined($Exp_stderr)) {
		$stderr_cmd = " 2>$TMP_STDERR";
	}
	$retval &= like(`$Cmd$stderr_cmd`, $Exp_stdout, "$Txt (stdout)");
	my $ret_val = $?;
	if (defined($Exp_stderr)) {
		$retval &= like(file_data($TMP_STDERR),
		                $Exp_stderr, "$Txt (stderr)");
		unlink($TMP_STDERR);
	} else {
		diag("Warning: stderr not defined for '$Txt'");
	}
	$retval &= is($ret_val >> 8, $Exp_retval, "$Txt (retval)");

	return $retval;
}

sub file_data {
	# Return file content as a string
	my $File = shift;
	my $Txt;

	open(my $fp, '<', $File) or return undef;
	local $/ = undef;
	$Txt = <$fp>;
	close($fp);
	return $Txt;
}

sub create_file {
	# Create new file and fill it with data
	my ($file, $text) = @_;
	my $retval = 0;

	open(my $fp, ">$file") or return 0;
	print($fp $text);
	close($fp);
	$retval = is(file_data($file), $text,
	             "$file was successfully created");

	return $retval; # 0 if error, 1 if ok
}

sub print_version {
	# Print program version
	print("$progname $VERSION\n");
	return;
}

sub usage {
	# Send the help message to stdout
	my $Retval = shift;

	if ($Opt{'verbose'}) {
		print("\n");
		print_version();
	}
	print(<<"END");

Usage: $progname [options]

Contains tests for the $CMD_BASENAME(1) program.

Options:

  -a, --all
    Run all tests, also TODOs.
  -h, --help
    Show this help.
  -q, --quiet
    Be more quiet. Can be repeated to increase silence.
  -t, --todo
    Run only the TODO tests.
  --valgrind
    Run all tests under Valgrind to check for memory leaks and other 
    problems.
  -v, --verbose
    Increase level of verbosity. Can be repeated.
  --version
    Print version information.

END
	exit($Retval);
}

sub msg {
	# Print a status message to stderr based on verbosity level
	my ($verbose_level, $Txt) = @_;

	$verbose_level > $Opt{'verbose'} && return;
	print(STDERR "$progname: $Txt\n");
	return;
}

__END__

# This program is free software; you can redistribute it and/or modify it under 
# the terms of the GNU General Public License as published by the Free Software 
# Foundation; either version 2 of the License, or (at your option) any later 
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT 
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
# FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with 
# this program.
# If not, see L<http://www.gnu.org/licenses/>.

# vim: set ts=8 sw=8 sts=8 noet fo+=w tw=79 fenc=UTF-8 :
