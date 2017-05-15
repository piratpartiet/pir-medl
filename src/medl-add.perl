#!/usr/bin/env perl

#==============================================================================
# medl-add
# File ID: 098fb3f8-3902-11e7-85a0-db5caa6d21d3
#
# Add members to the database
#
# Character set: UTF-8
# ©opyleft 2017– Øyvind A. Holm <sunny@sunbase.org>
# License: GNU General Public License version 2 or later, see end of file for 
# legal stuff.
#==============================================================================

use strict;
use warnings;
use Getopt::Long;
use IPC::Open3;

local $| = 1;

our %Opt = (

	'help' => 0,
	'quiet' => 0,
	'verbose' => 0,
	'version' => 0,

);

our $progname = $0;
$progname =~ s/^.*\/(.*?)$/$1/;
our $VERSION = '0.0.0';

my $SQLITE = "sqlite3";

Getopt::Long::Configure('bundling');
GetOptions(

	'help|h' => \$Opt{'help'},
	'quiet|q+' => \$Opt{'quiet'},
	'verbose|v+' => \$Opt{'verbose'},
	'version' => \$Opt{'version'},

) || die("$progname: Option error. Use -h for help.\n");

$Opt{'verbose'} -= $Opt{'quiet'};
$Opt{'help'} && usage(0);
if ($Opt{'version'}) {
	print_version();
	exit(0);
}

my $sql_error = 0; # Is set to !0 if some sqlite3 error happened

exit(main());

sub main {
	my $Retval = 0;

	return $Retval;
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

Add members to the database.

Usage: $progname [options] [file [files [...]]]

Options:

  -h, --help
    Show this help.
  -q, --quiet
    Be more quiet. Can be repeated to increase silence.
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

	if ($Opt{'verbose'} >= $verbose_level) {
		print(STDERR "$progname: $Txt\n");
	}
	return;
}

sub sql {
	my ($db, $sql) = @_;
	my @retval = ();

	msg(3, "sql(): db = '$db'");
	local(*CHLD_IN, *CHLD_OUT, *CHLD_ERR);

	my $pid = open3(*CHLD_IN, *CHLD_OUT, *CHLD_ERR, $SQLITE, $db) or (
		$sql_error = 1,
		msg(0, "sql(): open3() error: $!"),
		return "sql() error",
	);
	msg(3, "sql(): sql = '$sql'");
	print(CHLD_IN "$sql\n") or msg(0, "sql(): print CHLD_IN error: $!");
	close(CHLD_IN);
	@retval = <CHLD_OUT>;
	msg(3, "sql(): retval = '" . join('|', @retval) . "'");
	my @child_stderr = <CHLD_ERR>;
	if (scalar(@child_stderr)) {
		msg(0, "$SQLITE error: " . join('', @child_stderr));
		$sql_error = 1;
	}
	return join('', @retval);
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