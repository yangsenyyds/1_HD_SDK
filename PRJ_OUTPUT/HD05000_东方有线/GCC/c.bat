@echo off

echo Start time: %time:~0,2%:%time:~3,2%:%time:~6,2%
set time1=%time:~0,2%%time:~3,2%%time:~6,2%

set LOCAL_PATH=%~dp0
set OUTPUT_PATH=%LOCAL_PATH%\..\Download

set LIB_PATH=%LOCAL_PATH%\..\..\..\Platform

set BT_CORE_PATH=%LIB_PATH%\RMC\BT
set RESPIN_PATH=%LIB_PATH%\RMC\RISCV
set RESPIN_ROM_UTIL_PATH=%LIB_PATH%\bt_rom\util


@echo start compile RISCV bin
set path=D:\Environment\RISC-V\msys64\mingw32\bin;%path%;

@echo clear
cd "%OUTPUT_PATH%"
del flash.dat
cd "%LOCAL_PATH%"
make clean

@echo make
make -j all

perl "%RESPIN_ROM_UTIL_PATH%\hex2rom.pl" 1000000 1ffffff 32 output\out.hex  output\out.rom
:: copy c bin for respin compile full bin.
copy output\out.rom "%OUTPUT_PATH%"

@echo compile RISCV bin ok

@echo update sched
cd "%OUTPUT_PATH%"
copy "%BT_CORE_PATH%\sched\user.dat" + rf.dat  sched.rom
perl "%RESPIN_ROM_UTIL_PATH%/mergepatch_only.pl" 128

@echo start compile full bin
cd "%LOCAL_PATH%"
copy "%RESPIN_PATH%\ramcode.rom"     "%OUTPUT_PATH%\ramcode.rom"
copy "%RESPIN_PATH%\memmap.format"   "%OUTPUT_PATH%\memmap.format"
if exist "%RESPIN_PATH%\zcode.rom" (
	copy "%RESPIN_PATH%\zcode.rom"   "%OUTPUT_PATH%\zcode.rom"
)

set input=%1
cd "%OUTPUT_PATH%"

@echo generate not enc flash 
"%RESPIN_ROM_UTIL_PATH%/geneep" -f -n -z

:end
del ramcode.rom
del memmap.format
if exist "zcode.rom" (
	del zcode.rom
)

del flash_ota.dat
del flash_burn.dat

perl build_burn.pl
perl gencrc.pl < flash_burn.dat
del flash_burn.dat
copy flash_crc.dat flash_burn.dat
del flash_crc.dat

perl build_ota.pl

cd %LOCAL_PATH%

echo End time: %time:~0,2%:%time:~3,2%:%time:~6,2%
set time2=%time:~0,2%%time:~3,2%%time:~6,2%
set /a time3=%time2%-%time1%
echo Use time: %time3%s