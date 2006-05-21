#!/usr/bin/perl -w

use strict;
use File::Basename;

my @files = map glob, @ARGV;

print "#define NUM_PALS ".@files."\n\n";

print "extern U8 defaultpalettes[NUM_PALS][256*3];\n\n";

my $palind = 0;

for (@files) {
	$_ = basename $_;
	s/\..*//;
	print "#define PAL_$_ $palind\n";
	$palind++;
}

print "\n#ifdef DEFINE_PALS\n\n";
print "  U8 defaultpalettes[NUM_PALS][256*3] = {\n";

@files = map glob, @ARGV;

for (@files) {
	open FILE, "<$_" or die "Can't open $_: $!";
	binmode FILE;
	local $/ = \3;		# read three bytes at a time
	$.=0;

	$_ = basename $_;
	s/\..*//;
	print "// PAL_$_\n";

	print "    {\n";

	while (<FILE>) {
		s/(.)/sprintf "%3d,",ord $1/ge;
		print "\t" if $. % 4 == 1 ;
		print "$_   ";
		printf "\t// %d-%d\n", $.-4, $.-1 if $. % 4 == 0;
	}
	close FILE;
	print "    },\n";
}
printf("  };\n");
print "\n#endif // DEFINE_PALS\n"