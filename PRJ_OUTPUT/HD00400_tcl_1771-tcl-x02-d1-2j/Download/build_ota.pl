use strict;

open(file_burn,"flash_burn.dat") or die "Can't open out file : $!";
open(file_ota_out,">flash_ota.dat") or die "Can't open out file : $!";


my @lines = <file_burn>;
my $length = @lines;
my $num = 0;


print file_ota_out "4f\n54\n41\n";

for($num = 0; $num < $length; $num++)
{
	print file_ota_out $lines[$num];
}


close(file_burn);
close(file_ota_out);
