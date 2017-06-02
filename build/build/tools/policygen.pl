#!/usr/bin/perl
use Getopt::Long;
use warnings;
use strict;

my $relPackageName= "";
my $company= "";
my $project= "";
my $baseProject = "";
my $rputil = "device/mediatek/build/build/tools/rputil";

&GetOptions(
    "rel=s" => \$relPackageName,
    "c=s" => \$company,
    "p=s" => \$project,
    "base=s" => \$baseProject,
);

if($relPackageName =~ /^\s*$/){
    print "release package is empty $relPackageName\n";
    &usage();
}

my $relPackages = $relPackageName =~ s/,/ /gr;
#print "relPackages: $relPackages\n";
my $rputilCmd = $rputil . " gen";

$rputilCmd .= " -c $company" if ($company);
$rputilCmd .= " -p $project" if ($project);
$rputilCmd .= " -b $baseProject" if ($baseProject);
$rputilCmd .= " -q $relPackages";
#print "Redirect to rputil: $rputilCmd\n";
system($rputilCmd) == 0 or die "Failed: $rputilCmd\n";

sub usage {
  warn << "__END_OF_USAGE";
Usage: perl policygen.pl [options]

Options:
  -rel      : release policy file.
  -p        : project to release.
  -c        : customer to release.

Example:
  perl policygen.pl -rel rel_customer_basic,rel_customer_platform_mt6752,rel_customer_modem -p k2v1 -c mediatek

__END_OF_USAGE

  exit 1;

}
