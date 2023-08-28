#第1个参数是音频文件路径
#第2个参数是output路径

use strict;
use File::Spec;
if(@ARGV < 2) {
	printf "less input \n";
	exit;
}

my $dir_voice_prompt = "$ARGV[0]";
my $dir_output =  "$ARGV[1]";
#vp_type sbc=1/pcm=2/mp3=3/wav=4
my $vp_type = 0x01;
#flag+num
my $check_header_length= 2;
#my @vp_startaddr;
my @vp_size;


my @vp_array;
$vp_array[0] = $dir_voice_prompt."/poweron.pcm.sbc";
$vp_array[1] = $dir_voice_prompt."/poweroff.pcm.sbc";
$vp_array[2] = $dir_voice_prompt."/pairing.pcm.sbc";
$vp_array[3] = $dir_voice_prompt."/connected.pcm.sbc";
$vp_array[4] = $dir_voice_prompt."/disconnect.pcm.sbc";
$vp_array[5] = $dir_voice_prompt."/low_battery.pcm.sbc";
$vp_array[6] = $dir_voice_prompt."/max_vol.pcm.sbc";
$vp_array[7] = $dir_voice_prompt."/min_vol.pcm.sbc";
$vp_array[8] = $dir_voice_prompt."/tws_connected_l.pcm.sbc";
$vp_array[9] = $dir_voice_prompt."/tws_connected_r.pcm.sbc";
$vp_array[10] = $dir_voice_prompt."/language.pcm.sbc";
$vp_array[11] = $dir_voice_prompt."/empty_voice_0.pcm.sbc";
$vp_array[12] = $dir_voice_prompt."/num_0.pcm.sbc";
$vp_array[13] = $dir_voice_prompt."/num_1.pcm.sbc";
$vp_array[14] = $dir_voice_prompt."/num_2.pcm.sbc";
$vp_array[15] = $dir_voice_prompt."/num_3.pcm.sbc";
$vp_array[16] = $dir_voice_prompt."/num_4.pcm.sbc";
$vp_array[17] = $dir_voice_prompt."/num_5.pcm.sbc";
$vp_array[18] = $dir_voice_prompt."/num_6.pcm.sbc";
$vp_array[19] = $dir_voice_prompt."/num_7.pcm.sbc";
$vp_array[20] = $dir_voice_prompt."/num_8.pcm.sbc";
$vp_array[21] = $dir_voice_prompt."/num_9.pcm.sbc";


$vp_array[22] = $dir_voice_prompt."/poweron_ch.pcm.sbc";
$vp_array[23] = $dir_voice_prompt."/poweroff_ch.pcm.sbc";
$vp_array[24] = $dir_voice_prompt."/pairing_ch.pcm.sbc";
$vp_array[25] = $dir_voice_prompt."/connected_ch.pcm.sbc";
$vp_array[26] = $dir_voice_prompt."/disconnect_ch.pcm.sbc";
$vp_array[27] = $dir_voice_prompt."/low_battery_ch.pcm.sbc";
$vp_array[28] = $dir_voice_prompt."/max_vol_ch.pcm.sbc";
$vp_array[29] = $dir_voice_prompt."/min_vol_ch.pcm.sbc";
$vp_array[30] = $dir_voice_prompt."/tws_connected_l_ch.pcm.sbc";
$vp_array[31] = $dir_voice_prompt."/tws_connected_r_ch.pcm.sbc";
$vp_array[32] = $dir_voice_prompt."/language_ch.pcm.sbc";
$vp_array[33] = $dir_voice_prompt."/empty_voice_0_ch.pcm.sbc";

$vp_array[34] = $dir_voice_prompt."/num_0_ch.pcm.sbc";
$vp_array[35] = $dir_voice_prompt."/num_1_ch.pcm.sbc";
$vp_array[36] = $dir_voice_prompt."/num_2_ch.pcm.sbc";
$vp_array[37] = $dir_voice_prompt."/num_3_ch.pcm.sbc";
$vp_array[38] = $dir_voice_prompt."/num_4_ch.pcm.sbc";
$vp_array[39] = $dir_voice_prompt."/num_5_ch.pcm.sbc";
$vp_array[40] = $dir_voice_prompt."/num_6_ch.pcm.sbc";
$vp_array[41] = $dir_voice_prompt."/num_7_ch.pcm.sbc";
$vp_array[42] = $dir_voice_prompt."/num_8_ch.pcm.sbc";
$vp_array[43] = $dir_voice_prompt."/num_9_ch.pcm.sbc";


$vp_array[44] = $dir_voice_prompt."/didi.pcm.sbc";
$vp_array[45] = $dir_voice_prompt."/ring.pcm.sbc";
$vp_array[46] = $dir_voice_prompt."/empty_fix_voice_0.pcm.sbc";
$vp_array[47] = $dir_voice_prompt."/empty_fix_voice_1.pcm.sbc";

my $vp_array_len=@vp_array;

my $schedfile = $dir_output."/sched.rom";
my $outputfile2 = $dir_output."/flash.dat";
my $allpcmfile = $dir_output."/all_pcm_data.dat";

my @args;
my $hexL,my $hexM,my $hexH,my $hexZ;

my $count,my $countIn,my $countInin;
my $length=0;
my $buff;
my $searchRes = 0;
my $temptrans,my $temptransHex;
my @vpbaseaddr;
my $vpaddr;
#find vp flash start addr
open(FLASHOUT, "$schedfile") or die "can't open $schedfile: $!";
my @lines = <FLASHOUT>;
my $length = @lines;
my $i;
for($i=0;$i<$length;$i+=1)
{
	my @string = split(':', $lines[$i]);
	#my $length_s = @string ;
	if( $string[0] eq "mem_vp_addr")
	{
		#print "mem_vp_addr is:$string[1]";
		#$regStr[5] = substr($string[1],2,2)."\n";
		$vpbaseaddr[0] = substr($string[1],0,2);
		$vpbaseaddr[1] = substr($string[1],3,2);
		$vpbaseaddr[2] = substr($string[1],6,2);
		$vpaddr=$vpbaseaddr[0]|($vpbaseaddr[1]<<8)|($vpbaseaddr[2]<<16);
		printf "\n***mem_vp_addr is:0x%x ***\n",$vpaddr;
	}
}
close(FLASHOUT);


for($count = 0; $count < $vp_array_len;$count++)
{
	@args = stat ($vp_array[$count]);
	if ("$args[7]"=="")
	{
		#print "no file $count\n";
		$vp_size[$count] = 0;
	}
	else
	{
		$vp_size[$count] = $args[7];	
	}
}


open(FLASHOUT, ">$allpcmfile") or die "can't open $allpcmfile: $!";
#write type
my $typetemp = sprintf "%02x",$vp_type&0xff;
print FLASHOUT "$typetemp\n";
my $vpflashheadlen=1+8*$vp_array_len+$vpaddr;
my $offect=$vpflashheadlen;
#print "vpflashheadlen: $vpflashheadlen\n";

#length offect
for($count=0; $count < $vp_array_len;$count++)
{
	#print "vp_size[$count]:$vp_size[$count]\n";
	$hexL = $vp_size[$count]&0xff;
	$hexM = ($vp_size[$count]&0xff00) >>8;
	$hexH = ($vp_size[$count]&0xff0000) >>16;
	$hexZ = ($vp_size[$count]&0xff000000) >>24;
	my $sizestr = sprintf "%02x",$hexL;
	print FLASHOUT "$sizestr\n";
	$sizestr = sprintf "%02x",$hexM;
	print FLASHOUT "$sizestr\n";
	$sizestr = sprintf "%02x",$hexH;
	print FLASHOUT "$sizestr\n";
	$sizestr = sprintf "%02x",$hexZ;
	print FLASHOUT "$sizestr\n";
	if($vp_size[$count]!=0)
	{
		#print "offect:$offect\n";
		$hexL = $offect&0xff;
		$hexM = ($offect&0xff00) >>8;
		$hexH = ($offect&0xff0000) >>16;
		$hexZ = ($offect&0xff000000) >>24;
		my $sizestr = sprintf "%02x",$hexL;
		print FLASHOUT "$sizestr\n";
		$sizestr = sprintf "%02x",$hexM;
		print FLASHOUT "$sizestr\n";
		$sizestr = sprintf "%02x",$hexH;
		print FLASHOUT "$sizestr\n";
		$sizestr = sprintf "%02x",$hexZ;
		print FLASHOUT "$sizestr\n";
	
		$offect+=$vp_size[$count];
	}
	else
	{
		$sizestr = sprintf "%02x",0;
		print FLASHOUT "$sizestr\n";
		print FLASHOUT "$sizestr\n";
		print FLASHOUT "$sizestr\n";
		print FLASHOUT "$sizestr\n";
	}
}

for($count=0; $count < $vp_array_len;$count++)
{
	if($vp_size[$count]!=0)
	{
		open(VPPWO, $vp_array[$count]) or die "can't open $vp_array[$count]: $!";
		binmode(VPPWO);
		while(read(VPPWO, $buff, 1))
		{
			$hexL=unpack("H*",$buff);
			print FLASHOUT "$hexL\n";
		}
		close(VPPWO);
	}
}
close(FLASHOUT);
open(FLASHOUT, "$outputfile2") or die "can't open $outputfile2: $!";
my @lines = <FLASHOUT>;
my $length = @lines;
if($length > ($vpaddr-0x6000) )
{	
	printf "%s  length:%d (0x%x)\n",$outputfile2,$length,$length;	
	printf "\n--------- %s TOO LARGE ----------\n\n",$outputfile2;	
	printf "\n--------- %s is must less than 0x%x--------\n\n",$outputfile2,$vpaddr-0x6000;	
}
else
{
	printf "%s length:%d (0x%x)\n",$outputfile2,$length,$length;
}
close(FLASHOUT);

open(FLASHOUT, "$allpcmfile") or die "can't open $allpcmfile: $!";
my @lines = <FLASHOUT>;
my $length = @lines;
if($length > (0x7F000-$vpaddr))
{	
	printf "%s  length:%d (0x%x)\n",$allpcmfile,$length,$length;	
	printf "\n--------- %s TOO LARGE ----------\n\n",$allpcmfile;	
	printf "\n--------- %s is exceed 0x7F000 ----------\n\n",$allpcmfile;	
}
else
{
	printf "%s  length:%d (0x%x)\n",$allpcmfile,$length,$length;
}
close(FLASHOUT);
