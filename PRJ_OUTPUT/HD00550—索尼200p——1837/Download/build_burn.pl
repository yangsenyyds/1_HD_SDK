use strict;

open(file_ori,"flash.dat") or die "Can't open out file : $!";
open(file_name,"serial_name.dat") or die "Can't open out file : $!"; 
open(file_burn_out,">flash_burn.dat") or die "Can't open out file : $!";

my @ori_lines = <file_ori>;
my $ori_length = @ori_lines;
my @name_lines = <file_name>;
my $name_length = @name_lines;
my $head_length = $name_length + 3;
my $num = 0;

printf file_burn_out "%02x\n",($head_length >> 16);
printf file_burn_out "%02x\n",($head_length >> 8);
printf file_burn_out "%02x\n",($head_length & 0xFF);

for($num = 0; $num < $name_length; $num++)
{
	print file_burn_out $name_lines[$num];
}

print file_burn_out "\n";

for($num = 3; $num < $ori_length-2; $num++)
{
	print file_burn_out $ori_lines[$num];
}


close(file_ori);
close(file_name);
close(file_burn_out);
