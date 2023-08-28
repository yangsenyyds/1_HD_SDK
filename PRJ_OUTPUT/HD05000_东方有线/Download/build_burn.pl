use strict;

open(file_ori,"flash.dat") or die "Can't open out file : $!";
open(file_name,"serial_name.dat") or die "Can't open out file : $!"; 
open(file_burn_out,">flash_burn.dat") or die "Can't open out file : $!";

my @ori_lines = <file_ori>;
my $ori_length = @ori_lines;
my @name_lines = <file_name>;
my $name_length = @name_lines;
my $num = 0;


for($num = 0; $num < $name_length; $num++)
{
	print file_burn_out $name_lines[$num];
}

print file_burn_out "\n";

for($num = 3; $num < $ori_length; $num++)
{
	print file_burn_out $ori_lines[$num];
}


close(file_ori);
close(file_name);
close(file_burn_out);
