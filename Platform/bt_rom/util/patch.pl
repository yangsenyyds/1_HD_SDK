#!/usr/bin/perl



@dirs = (".");
while(@dirs){
   $d = $dirs[0];
   opendir folder, $d || die "Can not open this directory";
   @filelist = readdir folder; 
   closedir folder;
   foreach (@filelist) {
      $f = $d . "/" . $_;
      next if($_ eq "." || $_ eq "..");
      push(@dirs, $f) if(-d $f) ;
      push(@files,$f) if(-f $f);
    }
   shift @dirs;
}

for($i = 0;$i < @files;$i++) {
	if($files[$i] =~ /bt\.prog/) {
		($files[$i],$files[0]) = ($files[0],$files[$i]);
		last;
	}
}


for($i = $byte = $bit = 0;$i < @files;$i++) {
	next if($files[$i] =~ /patch\.prog|sim\.prog/ || $files[$i] !~ /\.prog/);
	open(FILE, $files[$i]) or next;
	@file = <FILE>;
	close FILE;
	open(FILE, "> $files[$i]");
	for($j = 0;$j < @file;$j++) {
		$_ = $file[$j];
		if(/bpatch.*,.*/) {
			printf FILE "\t%s patch%02x_%d,mem_patch%02x\n", $byte > 31 ? "bpatchx" : "bpatch" , $byte, $bit, $byte;
			if(++$bit > 7)
			{
				$bit = 0;
				$byte++;
			}
		} else {
			print FILE $_;
		}
	}
	close(FILE);
}
printf "Total patches : %d, last patch number is:%02x bit %d\n", $byte*8+$bit, $byte, $bit;
