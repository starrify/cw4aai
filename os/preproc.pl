#!/usr/bin/env perl 

# macros:
# 	format: .def <NAME> arg1, arg2, ... {<BODY>}
# 	features: 
# 		nested macros supported
# 		private labels supported
# 			format: <LBn>, n starts from 1
# 	note:
# 		char set [\w] is allowed in formal param while char set [\w\$+-] is allowed in real param
# 		macros are expanded in whole word mode, word boundary is (bol|eol|[^\w])
# include:
# 	format: .inc "<FILENAME>"
# 	features:
# 		nested include supported
# 		avoid multiple includes
#

use strict;
use warnings;
use utf8;
use 5.010;

my $keystr = '\.\w+';
my %macros = (), my %included = ();
my $label_cnt = 0;
my $error_len = 30;

sub readSrc() {
	#open SRC, "< $_[0]" or die;
	$/ = undef;
	chomp(my $src = <STDIN>);
	#close SRC;
	$src;
}

sub rmComm() {
	$_[0] =~ s/#.*?\n/\n/g;
	$_[0];
}

sub getline() {
	$_[0] =~ /.*/;
	$&;
}

sub addDef() {
	my $ret;
	my $leftover = $_[0];

	$leftover =~ /^\s*(\w+)\s*(?:(\w+(\s*,\s*\w+)*))?/ 
		or die 'Invalid macro definition: ".def ' . &getline($leftover) . '"';
	my $macro_name = $1;
	#my $macros_item = $macros{$macro_name};
	$keystr .= "|$macro_name";
	$macros{$macro_name} = [];
	my $args = $2;
	$leftover = $';
	$leftover =~ /^\s*{(.*?)}/s 
		or die "Invalid macro definition: \".def $macro_name " . &getline($leftover) . '"';
	$macros{$macro_name}->[0] = $1;
	$ret = $';
	defined($args) and push(@{$macros{$macro_name}}, grep { $_ } split(/\s|,/, $args));
	$ret;
}

sub labelExpand() {
	my $label, my $max_label_inc = 0;
	while($_[0] =~ /<LB\s*(\d+)>/) {
		$label = "_PPLB" . ($label_cnt + $1) . ":";
		$max_label_inc = $1 if $1 > $max_label_inc;
		$_[0] =~ s/$&/$label/g;
	}
	$label_cnt += $max_label_inc;
	$_[0];
}

sub defaultExpand() {
	$_[0] = &labelExpand($_[0]);
	$_[0];
}

sub expandDef() {
	my ($cmd, $ret) = @_;
	my $str = $macros{$cmd}->[0];
	my $argc = $#{$macros{$cmd}};
	my @args = ();
	if($argc > 0) {
		my $tmp = $argc - 1;
		#print("$argc\n");
		$ret =~ /^\s*([\w\$\+-]+(\s*,\s*[\w\$\+-]+){$tmp})/ 
			or die "Invalid macro format: \"$cmd " . &getline($ret) . '"';
		$ret = $';
		@args = defined($1) ? (grep { $_ } split(/\s|,/, $1)) : ();

		#die "Invalid macro reference, argument mismatch, \"$cmd\": " . ($#{$macros{$cmd}} - 1) . " expected but $#args found" 
		#	if $#args != $#{$macros{$cmd}} - 1;
	}

	my $i;
	for $i (0 .. $argc - 1) {
		$str =~ s/(^|[^\w])$macros{$cmd}->[$i + 1](^|[^\w])/$1$args[$i]$2/g;
	}
	$str = &defaultExpand($str);
	#print("$str $ret\n");
	$ret = $str . $ret;
}

sub include() {
	my $ret;
	$_[0] =~ /\s*"(.*?)"/ 
		or die 'Invalid include format: ".inc ' . &getline($_[0]) . '"';
	#my $filename = $1;
	$ret = $';
	if(!exists($included{$1})) {
		$included{$1} = 1;
		open INC, $1 or die "Cannot open file $1";
		my $src = <INC>;
		$src = &rmComm($src);
		$ret = $src . $ret;
		close INC;
	} 
	$ret;
}

sub process() {
	my $ret;
	my ($cmd, $leftover) = @_;
	given($cmd) {
		when('.def') {
			$ret = &addDef($leftover);
		}
		when('.inc') {
			$ret = &include($leftover);
		}
		when(/\.\w+/) {
			print($cmd);
			$ret = $leftover;
		}
		default {
			$ret = &expandDef($cmd, $leftover);
		}
	}
	$ret;
}

my $src = &readSrc();
$src = &rmComm($src);

while($src =~ /(^|[^\w])($keystr)($|[^\w])/) {
	print($` . $1);
	$src = &process($2, $3 . $');
}

print($src);



