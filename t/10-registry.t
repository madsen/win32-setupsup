# -*-perl-*-
# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl Win32-Setupsup.t'

######################### We start with some black magic to print on failure.

# Change 1..1 below to 1..last_test_to_print .
# (It may become useful if the test is moved to ./t subdirectory.)

BEGIN { $| = 1; print "1..3\n"; }
END {print "not ok 1\n" unless $loaded;}
use Win32::Setupsup;
$loaded = 1;
print "ok 1\n";

######################### End of black magic.

# Insert your test code below (better if it prints "ok 13"
# (correspondingly "not ok 13") depending on the success of chunk 13
# of the test code):

# These aren't very important functions in this module,
# but they're the only ones that are easy to test:

use Win32::TieRegistry ( Delimiter=>"/", ArrayValues=>0 );

my $winReg = $Registry->{'LMachine/Software/Microsoft/Windows/CurrentVersion/'};

my $value;
print "not " unless Win32::Setupsup::GetProgramFilesDir($value)
                and $value eq $winReg->{ProgramFilesDir};
print "ok 2\n";

print "not " unless Win32::Setupsup::GetCommonFilesDir($value)
                and $value eq $winReg->{CommonFilesDir};
print "ok 3\n";
