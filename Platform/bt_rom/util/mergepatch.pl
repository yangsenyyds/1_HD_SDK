#!/usr/bin/perl
$defs = "";

sub alloc {
	my($end, %mc) = @_;
	my(%eadr) = ();
	my($tstr, $str, $i, $j, $addr);
	my($tstr) = "";
	
	foreach $i (sort keys %mc) {
		if($i =~ /(.+)_(.+)/ && exists($eadr{$1})) {
			$addr = $eadr{$1};
		} else {
			$addr = $end;
		}
		for($j = 0;$j <= $#{$mc{$i}};$j++) {
			if($mc{$i}[$j] =~ /^\s*([0-9]+)\s+([a-zA-Z0-9_]+)\s*(.*)$/) {
				$str = sprintf "0x%04x %s%s\n", $addr, $2, $3;
				$var{$2} = $addr;
				$addr += $1;
				$tstr .= $str;
			}
		}
		$eadr{$i} = $addr;
	}
	if($addr > 0x4000) {
		$xend = $addr if($addr > $xend);
	} else {
		$bend = $addr if($addr > $bend);
	}
	return $tstr . "\n";
}


sub malloc {
	$path="output/bt_format.meta";
	open file, $path;
	@f = <file>;
	close file;
	open file, ">$path";
	$path =~ s/\/.*//;
	open file1, ">output/memmap.format";
	$xaddr = 0x4000;
	$addr = 0;
	if($#ARGV == 1) {
		$addr = hex($ARGV[0]);
		$xaddr = hex($ARGV[1]);
	}
	for($i = 0, $ocnt = 0;$i < @f;$i++) { 
		$_ = $f[$i];
		$start = 1 if(/^\s*memalloc\s*\(\s*$/);		# allocate from 0
		$start = 2 if(/^\s*xmemalloc\s*\(\s*$/);	# allocate from 4000
		$start = 3 if(/^\s*omemalloc\s*\(\s*$/);	# allocate after all xmemalloc, mutex from each other(obsolete, for compatibility)
		if(/^\s*([ax]*)memalloc\s+([a-zA-Z_0-9]+)\s*\(\s*$/) {
			$start = $1 eq 'a' ? 5 : $1 eq 'x' ? 6 : 7;
			$basev = $2;
		}
		if($start) {
			if(/^\s*([a-zA-Z0-9_]+)\s+([a-zA-Z0-9_]+)\s*(.*)$/ && exists($def2{$1})) {		# macro for size
				$_ = sprintf("%s %s %s", $def2{$1}, $2, $3);
			}
			if(/^\s*([0-9]+)\s+([a-zA-Z0-9_]+)\s*(.*)$/) {
				if($start == 1) {
					$str = sprintf "0x%04x %s%s\n", $addr, $2, $3;
					$var{$2} = $addr;
					$addr += $1;
					$bstr .= $str;
				} elsif($start == 2) {
					$str = sprintf "0x%04x %s%s\n", $xaddr, $2, $3;
					$var{$2} = $xaddr;
					$xaddr += $1;
					$xstr .= $str;
				} else {
					push @ostr, $_;
				}
			}
			if(/^\s*\)\s*$/) {
				if(@ostr > 0) {
					if($start == 3) {
						push @{$xmalloc{$ocnt}}, @ostr;
						$ocnt++;
					} elsif($start == 5) {
						push @aaddr,$basev;
						push @amalloc,[@ostr];
					} elsif($start == 6) {
						push @{$xmalloc{$basev}}, @ostr;
					} elsif($start == 7) {
						push @{$malloc{$basev}}, @ostr;
					} 
				}
				$start = 0;
				@ostr = ();
			}
		} else {
			$def2{$2} = $1 if(/^\s*([0-9]+)\s+([a-zA-Z0-9_]+)\s*(.*)$/);
			$sstr .= $_;
		}
	}
	
#--------------------- o/x/memalloc process -----------------------
	$bend = $addr, $xend = $xaddr;
	$bstr .= alloc($addr, %malloc);
	$xstr .= alloc($xaddr, %xmalloc);

#--------------------- amemalloc process -----------------------
	for($i = 0;$i <= $#aaddr;$i++) {
		for($j = 0, $addr = $aaddr[$i] =~ /^0x([0-9a-f]+)/ ? hex($1) : $var{$aaddr[$i]};$addr != 0 && $j <= $#{$amalloc[$i]};$j++) {
			if($amalloc[$i][$j] =~ /^\s*([0-9]+)\s+([a-zA-Z0-9_]+)\s*(.*)$/) {
				$str = sprintf "0x%04x %s%s\n", $addr, $2, $3;
				$addr += $1;
				if($addr < 0x4000) {
					$bstr .= $str;
				} else  {
					$xstr .= $str;
				}
			}
		}
	}

	
	print file $bstr, $xstr, $sstr;
	print file1 $bstr, $xstr;
	close file;
	close file1;
	printf "\nLast allocated address is %04x\n", $bend;
	printf "\nLast allocated xmem address is %04x\n", $xend;
}


sub parseif {
	my($fname) = @_;
	open file, $fname;
	@f = <file>;
	close file;
	open file, ">$fname";
	@valid = (1);
	for($i = 0;$i < @f;$i++) { 
		$_ = $f[$i];
		s/\/\*.*\*\///g;
		$comment = 1 if(/\/\*/ && !/\/\//);
		if($comment) {
			$comment = 0 if(/\*\//);
			next;
		}
		$defs .= $1 . " " if(/^define\s+(\w+)\s*/);
		if(/^\s*ifdef\s+(\w+)\s*/) {
			push @valid, ($defs =~ /(^|\s)$1(\s|$)/ ? 1 : 0) & $valid[$#valid];
		} elsif(/^\s*ifndef\s+(\w+)\s*/) {
			push @valid, ($defs =~ /(^|\s)$1(\s|$)/ ? 0 : 1) & $valid[$#valid];
		} elsif(/^\s*else/) {
			printf "%s\nelse without if at line %d\n", $f[$i - 3], $i if($#valid == 0);
			if($#valid == 0) {
				for($j = -10;$j < 10;$j++) {
					print $f[$i + $j];
				}
				last;
 				$i = $i;
			}
			$valid[$#valid] = (1 - $valid[$#valid]) & $valid[$#valid - 1];
		} elsif(/^\s*endif/) {
			printf "%s\nendif without if at line %d\n", $_, $i if($#valid == 0);
			if($#valid == 0) {
				for($j = -10;$j < 10;$j++) {
					print $f[$i + $j];
				}
				last;
 				$i = $i;
			}
			pop @valid;
		} elsif($valid[$#valid]) {
			if(/^include\s+(.+)\s*/) {
				open file2, "program/$1";
				@f2 = <file2>;
				close file2;
				splice(@f, $i + 1, 0, @f2);
			} else {
				print file $_;
			}
		}
	}
	close file;
}

sub genpatch
{
	print "genpatch\n";
	$patched = 0;
	open file,"program/patch.prog";
	while(<file>) {
		if(/^\s*beq\s+patch([0-9a-f]+)_([0-7]),/) {
			$a = hex($1), $b = hex($2);
			$bits[$a] |= 1 << $b;
			$patched = 1;
		}
	}
	close file;
	return if(!$patched);
	open file,"output/sched.rom";
	@sched = <file>;
	close file;
	for($j = 0;$j < 0x40;$j++) { 
		$s .= sprintf ("%02x   #mem_patch%02x\n", $bits[$j], $j);
	}
	$s .= "\n\n";
	for($i = $skip = 0;$i < @sched;$i++) {
		$_ = $sched[$i];
		if($skip > 0 && /:/) {
			splice(@sched, $skip, $i - $skip, $s);
			last;
		}
		$skip = $i + 1 if(/^mem_patch00:\s*$/);
	}
	if($skip == 0) {
		$s = "mem_patch00:\n" . $s;
		splice(@sched, 0, 0, $s);
	}
	open file,">output/sched.rom";
	for($i = 0;$i < @sched;$i++) { print file $sched[$i]; }
	close file;
}

sub zcode {
	open file, "output/bt_program23.meta";
	@f = <file>;
	close file;
	for($i = $label = $line = 0, $z = 1;$i < @f;$i++) {
		$_ = $f[$i];
		$line = $i - 1 if(/^org\s+0x[23]000/ && $line == 0);
		if(/^org z\s*$/) {
			$z += 0x10000;
			$f[$i] = sprintf("org 0x%x\n", $z);
			$label = 1;
			$line = $i - 1 if($line == 0);
		}
		if(/^(\w+):\s*$/ && $label == 1) {
			$lab{$1} = $z;
			$label = 2;
		}
	}
	return if($z == 1);
	for($i = 0;$i < @f;$i++) {
		$_ = $f[$i];
		$label = "";
		if(/^\s*(branch|nbranch|call|ncall)\s+(\w+)\s*$/ || /^\s*(branch|nbranch|call|ncall)\s+(\w+)\s*,\s*(\w+)\s*$/) {
			$label = $2;
		}
		if(/^\s*(bmark|beq|bne|bbit[01])\s+(\w+)\s*,\s*(\w+)\s*$/) {
			$label = $3;
		}
		if(exists($lab{$label})) {
			$nlabel = "jmpz_" . $label;
			$f[$i] =~ s/$label/eval "$nlabel"/e;
			if($lab{$label} > 100) {
				$f[$line] .= sprintf("%s:\n\tsetarg 0x%x\n\tbranch p_zcode_entrance_2Bytes_common\n\n", $nlabel, int($lab{$label}/0x1000));
				$lab{$label} = 1;
			}
		}
	}
	open file, ">output/bt_program23.meta";
	for($i = 0;$i < @f;$i++) {
		if($f[$i] =~ /^\s*bbit1 8,pf_patch_ext\s*$/) {
			printf file ("p_start:\n\tbranch p_patch_array\n\np_zcode:\n");
			for($j = 0;$j < 63;$j++) {
				printf file "\tnop %d\n", $j + 1;
			}
			printf file "p_patch_array:\n";
		}
		print file $f[$i];
	}
	close file;
}

parseif("output/bt_program23.meta");
parseif("output/bt_format.meta");
genpatch() if($defs !~ / SIM /);
malloc();
zcode();
