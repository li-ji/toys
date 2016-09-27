#!/usr/bin/perl
#===============================================================================
#
#         FILE:  run.pl
#
#        USAGE:  ./run.pl -b excutable_name -i input_netlist_file
#        [-o output_suffix] [-f html|pdf] [-h]
#
#  DESCRIPTION:  Show the symbolic MNA matrix of a linear circuit
#
#      OPTIONS:
#      --binary, -bin, -b:
#      		To set the excutable name.
#      --input, -i:
#      		To set the input netlist file name.
#      --output, -o:
#      		To set the output suffix name for PDF format;
#      		to set the output directory name for HTML format.
#      --format, -f:
#      		To set the output format other than the LaTeX file.
#      --help, -h:
#      		Get the help information and exit.
# REQUIREMENTS:  GNU tools: pdflatex, latex2html, acroread, firefox
#         BUGS:  ---
#        NOTES:  ---
#       AUTHOR:  Bob Li
#      COMPANY:  SNPS
#      VERSION:  1.0
#      CREATED:  2012/05/24 22:54:30
#     REVISION:  ---
#===============================================================================

use strict;
use warnings;

use Getopt::Long qw(GetOptions);

use constant DEFAULT_OUTPUT_PREFIX => 'mna';
use constant DEFAULT_OUTPU_FORMAT => 'pdf';

my @supported_output_format = qw(pdf html);

&main();

##########################
## function definitions ##
##########################
# command-line option parser
sub parse_cmdopt {
    local $_;
    my $ref_exec = shift;
    my $ref_input_file = shift;
    my $ref_output_prefix = shift;
    my $ref_output_format = shift;
    &GetOptions(
      "binary|bin|b=s" => $ref_exec, # To set the excutable name.
      "input|i=s" => $ref_input_file, # To set the input netlist file name.
      "output|o=s" => $ref_output_prefix, # To set the output suffix name for PDF format; give the output directory name for HTML format.
      "format|f=s" => $ref_output_format, # To set the output format other than the LaTeX file
      "help|h" => \ my $need_help # To dump help information
    );
    # dump help information and exit
    if ($need_help) {
	&help_info();
	die;
    }
    # check the excutable name
    die "Fatal error: No executable found!\n" unless (defined $$ref_exec);
    die "Error: $$ref_exec is NOT executable\n" unless (-x $$ref_exec);
    # check intput file name
    die "Error: no input netlist" unless (defined $$ref_input_file);
    die "Error: cannot find $$ref_input_file: no such file\n" unless (-f $$ref_input_file);
    # check output prefix
    $$ref_output_prefix = DEFAULT_OUTPUT_PREFIX unless (defined $$ref_output_prefix);
    # check output format
    $$ref_output_format = DEFAULT_OUTPU_FORMAT unless (defined $$ref_output_format);
    unless (grep {$_ eq $$ref_output_format} @supported_output_format) {
        die "Error: unsupported output format: $$ref_output_format\n";
    }
}

# dump help information
sub help_info {
    local $_;
    print <<HELP_INFO;
USAGE:  ./run.pl -b executable_name -i input_netlist_file [-o output_suffix] [-f html|pdf] [-h]

DESCRIPTION:  Show the symbolic MNA matrix of a linear circuit

OPTIONS:
	--binary, -bin, -b:
		To set the excutable name.
	--input, -i:
		To set the input netlist file name.
	--output, -o:
		To set the output suffix name for PDF format;
		to set the output directory name for HTML format.
	--format, -f:
		To set the output format other than the LaTeX file.
	--help, -h:
		Get the help information and exit.
REQUIREMENTS:  GNU tools: pdflatex, latex2html, acroread, firefox
HELP_INFO
}

sub main {
    local $_;
    my $exec;
    my $input_file;
    my $output_prefix;
    my $output_format;
    &parse_cmdopt(\$exec, \$input_file, \$output_prefix, \$output_format);
    if ( $output_format eq $supported_output_format[0]) {
	# PDF format
	system ("$exec $input_file -o $output_prefix && pdflatex $output_prefix && acroread $output_prefix.$output_format");
    } elsif ( $output_format eq $supported_output_format[1]) {
	# HTML format
	system ("$exec $input_file -o $output_prefix && latex2html $output_prefix && firefox $output_prefix/index.html");
    } else {
	die "Error: Unsupported output format found!";
    }
}
