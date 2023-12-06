:: 下载代码
e pu&e p

:: close wdt&enable xtal
e 804c 00000011
e 8019 80

e 8042 4



:: flash io setting
SET SourceFile="./flash_io_setting.bat"
if exist %SourceFile% (
	@call %SourceFile%
) else (
	e 8090 020506
	e 8094 040307
)



:: enable flash, avoid power work.
e fa

:: unlock flashlock
e fs 0
:: clear flash first
e fc
:: read flash for check
e fr 0 30
:: 烧录
e fp flash_burn.dat 1000
:: 代码加载地址
e fw 0 131000
:: 重启
e k
