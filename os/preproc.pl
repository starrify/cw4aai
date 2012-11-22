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
# declare:
# 	format: .decl <type> .end_decl
# 	supported types:
# 		func:(local|global)
# 			function being declared will not be coded until it is called
# 			local functions must be called within the file where it is declared
# 		var:
# 			static variables being declared will be moved to the end of the code

use strict;
use warnings;
use utf8;
use 5.010;

my %macros = (), my %included = ();
my %func = ();	#func[0]: filename, func[1]: code, func[2]: local?
my $var = "";
my $label_cnt = 0;
my $error_len = 30;
my $supported_decl = "func|var";
my $keystr = '\.\w+';
my $curfile = "main_file";

sub readSrc() {
	$/ = undef;
	chomp(my $src = <STDIN>);
	$src;
}

sub rmComm() {
	$_[0] =~ s/#.*?(\n|$)/\n/g;
	$_[0];
}

sub getline() {
	$_[0] =~ /.*/;
	$&;
}

sub addDef() {
	my $leftover = $_[0];

	$leftover =~ /^\s*(\w+)\s*(?:(\w+(\s*,\s*\w+)*))?/ 
		or die 'Invalid macro definition: ".def ' . &getline($leftover) . '"';
	my $macro_name = $1;
	$keystr .= "|$macro_name";
	$macros{$macro_name} = [];
	my $args = $2;
	$leftover = $';
	$leftover =~ /^\s*{(.*?)}/s 
		or die "Invalid macro definition: \".def $macro_name " . &getline($leftover) . '"';
	$macros{$macro_name}->[0] = $1;
	$leftover = $';
	defined($args) and push(@{$macros{$macro_name}}, grep { $_ } split(/\s|,/, $args));
	("", $leftover);
}

sub labelExpand() {
	my $label, my $max_label_inc = 0;
	while($_[0] =~ /<LB\s*(\d+)>/) {
		$label = "_PPLB" . ($label_cnt + $1) . ":";
		$max_label_inc = $1 if $1 > $max_label_inc;
		$_[0] =~ s/$&/$label/g;
	}
	$label_cnt += $max_label_inc;
	("", $_[0]);
}

sub defaultExpand() {
	my ($out, $leftover) = &labelExpand($_[0]);
}

sub expandDef() {
	my $out = "", my $tmp;
	my ($cmd, $leftover) = @_;
	my $str = $macros{$cmd}->[0];
	my $argc = $#{$macros{$cmd}};
	my @args = ();
	if($argc > 0) {
		$tmp = $argc - 1;
		$leftover =~ /^\s*([\w\$\+-]+(\s*,\s*[\w\$\+-]+){$tmp})/ 
			or die "Invalid macro format: \"$leftover " . &getline($leftover) . '"';
		$leftover = $';
		@args = defined($1) ? (grep { $_ } split(/\s|,/, $1)) : ();

	}

	my $i;
	for $i (0 .. $argc - 1) {
		$str =~ s/(^|[^\w])$macros{$cmd}->[$i + 1]($|[^\w])/$1$args[$i]$2/g;
	}
	($tmp, $str) = &defaultExpand($str);
	$out .= $tmp;

	$str =~ s/(^|[^\w])$cmd($|[^\w])/$1__current__cmd__$2/g;
	$str = &process($str);
	$str =~ s/(^|[^\w])__current__cmd__($|[^\w])/$1$cmd$2/g;
	$out .= $str;
	($out, $leftover);
}

sub include() {
	my $leftover, my $out = "";
	$_[0] =~ /\s*"(.*?)"/ 
		or die 'Invalid include format: ".inc ' . &getline($_[0]) . '"';
	#my $filename = $1;
	$leftover = $';
	if(!exists($included{$1})) {
		$included{$1} = 1;
		open INC, $1 or die "Cannot open file $1";
		my $src = <INC>;
		#change and restore curfile info
		my $orifile = $curfile;
		$curfile = $1;
		$out .= &process($src);
		$curfile = $orifile;
		close INC;
	} 
	($out, $leftover);
}

sub addFuncDecl() {
	my $leftover = $_[0];
	my $func_type = "global|local";
	$leftover =~ /^\s*(?:($func_type)\s+)?((\w+):.*?)\.end_decl/s 
		or die "invalid function declaration format \"" . &getline($leftover) . '"';
	$leftover = $';
	$func{$3} = [$curfile, &process($2), (defined($1) ? ($1 eq "local") : 0)];
	#print("$3: " . (defined($1) ? ($1 eq "local") : 0) . "\n");
	("", $leftover);
}

sub addVarDecl() {
	my $leftover = $_[0];
	$leftover =~ /^(.*?)\.end_decl/s or die "invalid static variable declaration format \"" . &getline($leftover) . '"';
	$leftover = $';
	$var .= &process($1);
	("", $leftover)
}

sub addDecl() {
	my $out = "", my $leftover = $_[0];
	my $tmp;
	$leftover =~ /^\s*($supported_decl)($|[^\w])/ or die "Declaration \"" . &getline($leftover) . "\" not supported";
	$leftover = $';
	given($1) {
		when('func') {
			($tmp, $leftover) = &addFuncDecl($leftover);
			$out .= $tmp;
		}
		when('var') {
			($tmp, $leftover) = &addVarDecl($leftover);
			$out .= $tmp;
		}
		default {
			die "Unexpected error";
		}
	}
	($out, $leftover);
}

sub processCmd() {
	my ($cmd, $leftover) = @_;
	my $out = "", my $tmp;
	given($cmd) {
		when('.def') {
			($tmp, $leftover) = &addDef($leftover);
			$out .= $tmp;
		}
		when('.inc') {
			($tmp, $leftover) = &include($leftover);
			$out .= $tmp;
		}
		when('.decl') {
			($tmp, $leftover) = &addDecl($leftover);
			$out .= $tmp;
		}
		when(/\.\w+/) {
			$out .= $cmd;
		}
		default {
			($tmp, $leftover) = &expandDef($cmd, $leftover);
			$out .= $tmp;
		}
	}
	($out, $leftover);
}

sub process() {
	my $src = $_[0];
	my $out = "", my $tmp;
	$src = &rmComm($src);

	while($src =~ /(^|[^\w])($keystr)($|[^\w])/) {
		$out .= ($` . $1);
		($tmp, $src) = &processCmd($2, $3 . $');
		$out .= $tmp;
	}
	$out .= $src;
}

sub addDepend() {
	my $buf = $_[0], my $out = "";
	my $tmp, my $key;
	while($buf) {
		$tmp = "";
		for $key (keys %func) {
#			print("$key\n");
			if(index($buf, $key) != -1) {
				die "local function $key(in $func{$key}->[0]) called in $curfile"
					if $func{$key}->[2] == 1 && $curfile ne $func{$key}->[0]; 
				$tmp .= $func{$key}->[1];
				delete $func{$key};	# do not delete cause it maybe used again?
			}
		}
		$out .= $buf;
		$buf = $tmp;
	}
	$out;
}

sub addVar() {
	$_[0] . $var;
}

sub format() {
	$_[0] =~ s/(\s*\n)+/\n/g;
	$_[0];
}

my $src = &readSrc();
my $res = &process($src);
$res = &addDepend($res);
$res = &addVar($res);
$res = &format($res);
print($res);
0;


