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
# static:
# 	format: .static {<BODY>}
# 		static data will be moved to the end of the code
# declare:
# 	format: .decl <type> {BODY}
# 	supported types:
# 		func:(scope|global)
# 			function being declared will not be coded until it is called
# 			scope decides the scope in which functions can be called

use strict;
use warnings;
use utf8;
use 5.010;

my %macros = (), my %included = ();
my ($func_name, $func_filename, $func_code, $func_scope, $func_depends) = (0, 1, 2, 3, 4);
my %func = ();
my $static = "";
my $label_cnt = 0;
my $error_len = 30;
my $supported_decl = "func";
my $keystr = '\.\w+';
my $curfile = "main file";

#key proc
sub readSrc() {
	$/ = undef;
	my $src = <STDIN>;
	$/ = "\n";
	chomp($src);
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
            if($func{$key}->[$func_scope] ne "__global__" 
				&& !($func{$key}->[$func_scope] =~ /(^|[^\w])$curfile($|[^\w])/)) {
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
	$leftover =~ /^\s*(?:([^{]+?)\s|{)\s*(.*?)(\1|})/s 
		or die "Invalid macro definition: \".def $macro_name " . &getline($leftover) . '"';
	$macros{$macro_name}->[0] = $2;
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
		@args = defined($1) ? (grep { $_ ne "" } split(/\s|,/, $1)) : ();

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
		$included{$1} = 1;
		open INC, $1 or die "Cannot open file $1 in $curfile";
		
		#change curfile info and curdir
		my $orifile = $curfile;
		chomp(my $oridir = `pwd`);
		chomp(my $newdir = `dirname $1`);
		$curfile = $1;
		chdir $newdir or die $!; 
	
		$/ = undef;
		my $src = <INC>;
		$/ = "\n";
		($out, $localout) = &process($src);
		$out .= &localDep($localout);
		
		#restore curfile info and curdir
		$curfile = $orifile;
		chdir $oridir or die $!;
		
		close INC;
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
	my $func_type = "global|scope";
	$leftover =~ /^\s*(?:($func_type)(?:\s*\((.*?)\))?\s+)?(?:([^{]+?)\s|{)\s*((\w+):.*?)(\3|})/s 
		or die "invalid function declaration format \"" . &getline($leftover) . '"';
	$leftover = $';
	my ($processed) = &process($4);
	my $scope = (defined($1) && $1 eq "scope") ? ("$curfile, " . (defined($2) ? $2 : "")) : "__global__";
	my $funcinfo = [$5, $curfile, $processed, $scope];
	push @{$funcinfo}, &calcDep($funcinfo);
	$func{$funcinfo->[$func_name]} = $funcinfo;
	("", $leftover);
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
		default {
			die "Unexpected error";
		}
	}
	($out, $leftover);
}

#.static subroutines
sub addStatic() {
	my $leftover = $_[0];
	$leftover =~ /^\s*(?:([^{]+?)\s|{)\s*(.*?)(\1|})/s 
		or die "invalid static variable declaration format \"" . &getline($leftover) . '"';
	$leftover = $';
	my ($processed) = &process($2);
	$static .= $processed;
	("", $leftover)
}

#.loop subroutines
sub loop() {
	my $leftover = $_[0];
	my $loopvar, my $tmp;
	my $out = "";
	$leftover =~ /^\s*([x\da-fA-F]+)\s+([x\da-fA-F]+)(?:\s+([x\da-fA-F]+))?\s*(?:([^{]+)\s|{)(.*?)(\4|})/s
		or die "invalid loop format\"" . &getline($leftover) . '"';
	$leftover = $';
	my $num1 = index($1, "0x") == 0 ? hex($1) : $1;
	my $num2 = index($2, "0x") == 0 ? hex($2) : $2;
	my $num3 = (defined($3) ? (index($3, "0x") == 0 ? hex($3) : $3) : 1);
	for($loopvar = $num1;
		$loopvar <= $num2; 
		$loopvar += $num3) {
		$tmp = $5;
		$tmp =~ s/<LOOPVAR>/$loopvar/g;
		$out .= $tmp;
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
		when('.static') {
			($tmp, $leftover) = &addStatic($leftover);
			$out = $tmp;
			$localout = $tmp;
		}
		when('.loop') {
			($tmp, $leftover) = &loop($leftover);
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

my $src = &readSrc();
my ($res, $localout) = &process($src);
$res .= &localDep($localout);
$res .= "STATIC_BEG:\n${static}STATIC_END:\n";
$res = &format($res);
print($res);
0;

