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
my ($func_name, $func_filename, $func_code, $func_islocal, $func_depends) = (0, 1, 2, 3, 4);
my %func = ();
my $var = "";
my $label_cnt = 0;
my $error_len = 30;
my $supported_decl = "func|var";
my $keystr = '\.\w+';
my $curfile = "main file";

#key proc
sub readSrc() {
	$/ = undef;
	chomp(my $src = <STDIN>);
	$src;
}

#tools
sub rmComm() {
	$_[0] =~ s/#.*?(\n|$)/\n/g;
	$_[0];
}

sub getline() {
	$_[0] =~ /.*/;
	$&;
}

sub format() {
    $_[0] =~ s/(\s*\n)+/\n/g;
    $_[0];
}

sub calcDep() {
    my $funcinfo = $_[0], my $key;
    my @dep = ();
    for $key (keys %func) {
        if(index($funcinfo->[$func_code], $key) != -1) {
            if($func{$key}->[$func_islocal] == 1 && $curfile ne $func{$key}->[$func_filename]) {
                push @dep, "__error__ '$func{$key}->[$func_name]' '$funcinfo->[$func_name]' '$curfile'";
            } else {
                push @dep, $key;
            }
        }
    }
    \@dep;
}

sub genDepByArr() {
    my $depname, my $out = "", my $tmp;
    for $depname (@{$_[0]}) {
        if(index($depname, "__error__") != -1) {
			$depname =~ /__error__\s*'(.*)'\s*'(.*)'\s*'(.*)'\s*$/
				or die "internal error info mismatch";
        	die "call of local function '$1' in function '$2' in file '$3'"
		}
        if(exists($func{$depname})) {
            $out .= $func{$depname}->[$func_code];
            $tmp = $func{$depname}->[$func_depends];
            delete $func{$depname};
            $out .= &genDepByArr($tmp);
        }
    }
    $out;
}

#.def subroutines
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

#.inc subroutines
sub include() {
	my $leftover, my $out = "", my $localout;
	$_[0] =~ /\s*"(.*?)"/ 
		or die 'Invalid include format: ".inc ' . &getline($_[0]) . '"';
	$leftover = $';

	if(!exists($included{$1})) {
		#change curfile info
		my $orifile = $curfile;
		$curfile = $1;
		
		$included{$1} = 1;
		open INC, $1 or die "Cannot open file $1";
		my $src = <INC>;
		($out, $localout) = &process($src);
		$out .= &localDep($localout);
		close INC;
		
		#restore curfile info
		$curfile = $orifile;
	}
	($out, $leftover);
}

#.decl subroutines
sub localDep() {
    my $funcinfo = ["main chunk", $curfile, $_[0], 0];
    my $curdep = &calcDep($funcinfo);
    &genDepByArr($curdep);
}

sub addFuncDecl() {
	my $leftover = $_[0];
	my $func_type = "global|local";
	$leftover =~ /^\s*(?:($func_type)\s+)?((\w+):.*?)\.end_decl/s 
		or die "invalid function declaration format \"" . &getline($leftover) . '"';
	$leftover = $';
	my ($processed) = &process($2);
	my $funcinfo = [$3, $curfile, $processed, (defined($1) ? ($1 eq "local") : 0)];
	push @{$funcinfo}, &calcDep($funcinfo);
	$func{$funcinfo->[$func_name]} = $funcinfo;
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
	my $out, my $tmp;
	my $localout; #without .inc
	given($cmd) {
		when('.def') {
			($tmp, $leftover) = &addDef($leftover);
			$out = $tmp;
			$localout = $tmp;
		}
		when('.inc') {
			($tmp, $leftover) = &include($leftover);
			$out = $tmp;
			$localout = "";
		}
		when('.decl') {
			($tmp, $leftover) = &addDecl($leftover);
			$out = $tmp;
			$localout = $tmp;
		}
		when(/\.\w+/) {
			$out = $cmd;
			$localout = $cmd;
		}
		default {
			($tmp, $leftover) = &expandDef($cmd, $leftover);
			$out = $tmp;
			$localout = $tmp;
		}
	}
	($out, $leftover, $localout);
}

sub process() {
	my $src = $_[0];
	my $out = "";
	my $localout = ""; #without .inc
	my $tmpout, my $tmplocal;
	$src = &rmComm($src);

	while($src =~ /(^|[^\w])($keystr)($|[^\w])/) {
		$out .= ($` . $1);
		$localout .= ($` . $1);
		($tmpout, $src, $tmplocal) = &processCmd($2, $3 . $');
		$out .= $tmpout;
		$localout .= $tmplocal;
	}
	$out .= $src;
	$localout .= $src;
	($out, $localout);
}

sub addVar() {
	$_[0] . $var;
}

my $src = &readSrc();
my ($res, $localout) = &process($src);
$res .= &localDep($localout);
$res = &addVar($res);
$res = &format($res);
print($res);
0;

