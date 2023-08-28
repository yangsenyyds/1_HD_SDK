$a = __FILE__;
$a =~ s/[\\\/][^\\^\/]+$//;
$a =~ s/\\/\//g;
unshift(@INC, $a);

sub crc16_ccitt2
{
	my($crc, $c) = @_;

	$crc  = ($crc >> 8) | ($crc << 8);
	$crc ^= $c;
	$crc ^= ($crc & 0xff) >> 4;
	$crc ^= $crc << 12;
	$crc ^= ($crc & 0xff) << 5;
	$crc &= 0xffff;

	return $crc;
}

@txt = <STDIN>;
$offset = 0;
$offset = $ARGV[0] if @ARGV > 0;
$init = 0xffff;
$init = hex($ARGV[1]) if @ARGV > 1;
open(file_crc,">flash_crc.dat") or die "Can't open out file : $!";

for($i = 0, $crc = $init; $i < @txt; $i++) {
	$_ = $txt[$i];
	s/\s//g;
	print file_crc $_, "\n";
	$crc = crc16_ccitt2($crc, (hex($_))) if($i >= $offset);
}

printf file_crc "%02x\n",($crc >> 8);
printf file_crc "%02x\n",($crc & 0xff);

close(file_crc);
