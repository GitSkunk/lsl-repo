#!/bin/perl

# some versions of CGI expect things from the ARGV array
$arg0 = $ARGV[0];
delete $ARGV[0];

use Digest::MD5 qw(md5 md5_hex md5_base64);
use utf8;
use CGI qw/escape unescape/;

%vars = ();
$count = 0;

%strings = ();
$scount = 0;

sub addvar($) {
   my $var = shift @_;
   $vars{$var} = "v" . chr($count/26 + ord('A')) . chr($count%26 + ord('A'));
   $count++;
}

sub protect($) {
   my $string = shift @_;
   $strings{$scount} = $string;
   my $ret = "\"_PROTECT_$scount\_\"";
   $scount++;
   return $ret;
}

$content = "";

while (<>) {
   s/(\"([^\"]|(\\\"))*\")/protect($1)/ge;
   s/\/\/.*//g;
   s/[^ -~]//g;
   $content = $content . $_;
}

$content =~ s/[ ]+/ /g;

sub addemup($) {
   my $arg = shift @_;
   $arg =~ s/[^PROTECT_0-9]//g;
   return "\"".$arg."\"";
}

# transform "string"+"string" => "stringstring"
$content =~ s/((\"(_PROTECT_[0-9]+_)\"[\s]*\+[\s]*)+\"(_PROTECT_[0-9]+_)\")/addemup($1)/ge;

$tmp = $content;
$tmp =~ s/(void|float|integer|string|list|key|rotation|vector|quaternion)[\s]+([a-zA-Z0-9_]+)/addvar($2)/ge;

# $content =~ s/quaternion//g;

foreach $var (sort { length($b) <=> length($a) } keys(%vars))
{
#   print "// $var => $vars{$var}\n";
   $content =~ s/([^a-zA-Z0-9_.])$var([^a-zA-Z0-9_])/$1 . $vars{$var} . $2/ge;
   $content =~ s/([^a-zA-Z0-9_.])$var([^a-zA-Z0-9_])/$1 . $vars{$var} . $2/ge;
   $content =~ s/([^a-zA-Z0-9_.])$var([^a-zA-Z0-9_])/$1 . $vars{$var} . $2/ge;
}


#remove unnecessary spaces
$content =~ s/[ ]*\([ ]*/(/g;
$content =~ s/[ ]*\)[ ]*/)/g;
$content =~ s/[ ]*\{[ ]*/{/g;
$content =~ s/[ ]*\}[ ]*/}/g;
$content =~ s/[ ]*\[[ ]*/[/g;
$content =~ s/[ ]*\][ ]*/]/g;
$content =~ s/[ ]*\>[ ]*/>/g;
$content =~ s/[ ]*\<[ ]*/</g;
$content =~ s/[ ]*\,[ ]*/,/g;
$content =~ s/[ ]*\;[ ]*/;/g;
$content =~ s/[ ]*\+[ ]*/+/g;
$content =~ s/[ ]*\-[ ]*/-/g;
$content =~ s/[ ]*\![ ]*/!/g;
$content =~ s/[ ]*\|[ ]*/|/g;
$content =~ s/[ ]*\*[ ]*/*/g;
$content =~ s/[ ]*\/[ ]*/\//g;
$content =~ s/[ ]*\&[ ]*/&/g;
$content =~ s/[ ]*\=[ ]*/=/g;

# add removed space between "else" and "if" back
# not entirely save because it does this everywhere
#
$content =~ s/elseif\(/else if\(/g;

# this is tempting, but breaks various order of operations rules.
#$content =~ s/([0-9]+[-0-9\+\/\*]+[0-9]+)/eval($1)/ge;

# but this is safe!
$content =~ s/\(([0-9]+[\s]*[-0-9\+\/\*]+[\s]*[0-9]+)\)/"(".eval($1).")"/ge;

# some minor optimizations
$content =~ s/llFrand\(([0-9]+)\)/llFrand($1.0)/g;

# this seems like a good idea, but breaks some things
# remove excessive zeroes at the end of floating point numbers
#$content =~ s/(\.[0-9][1-9]*)[0]*/$1/g;

# this seems like a good idea, but breaks some things
# remove unnecessary leading zero in floating point numbers
#$content =~ s/([^0-9])0\./$1./g;

sub unq($) {
   my $q = shift @_;
   $q =~ s/^\"//g;
   $q =~ s/\"$//g;
   return $q;
}

foreach $key (keys(%strings)) {
   $val = unq($strings{$key});
   $content =~ s/_PROTECT_$key\_/$val/g;
}

# "{"+("norandom")+"}" => "{norandom}"
$content =~ s/\"\{\"\+\(\"([^"]*)\"\)\+\"\}\"/"{$1}"/g;

# "{"+("err_")
$content =~ s/\"\{\"\+\(\"([^"]*)\"\)/"{$1"/g;

# utf8 optimization
$content =~ s/llUnescapeURL\(\"([^"]+)\"\)/"\"" . CGI::unescape($1) . "\""/ge;

$name = $arg0;
$name =~ s/\.i$/.o/g;

print "// =$name\n";
#print "// copyright Ratany Resident, compiled " . gmtime() . " UTC\n";
print "// compiled " . gmtime() . " UTC\n";
#print "// DO NOT MODIFY THIS FILE!\n";
#print "// This file autogenerated, changes here will be lost.\n";
#print "// Please see http://ma8p.com/~opengate for source code and license.\n";
#print "// compiled " . gmtime() . "\n";

# $md5 = md5_hex($content);
# print "// SOFTWARE_REV $md5\n";

# $content =~ s/SOFTWARE_REV/$md5/g;

($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = gmtime();
$year += 1900;
$mon++;
#print "integer DATE = 0x" . sprintf("%04d%02d%02d", $year,$mon,$mday) . ";\n";
#print "integer TIME = 0x" . sprintf("00%02d%02d%02d", $hour,$min,$sec) . ";\n";

print "$content\n";

