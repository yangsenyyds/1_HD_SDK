SECTIONS
{
		. = 0x01000000;
				.text : { KEEP(*startup_rv32.o)}		
		. = 0x01001000;
				.text1 : {KEEP(*app_config.o) *(.text*) *(.rdata*) *(.rodata*) *(.srodata*)  *(.eh_frame*)   *(.srodata.cst*)}
        _sidata = ALIGN(4);
        .local 0x10002000 : {*(.local*)}
        . = 0x10010000;
        .debug 0x10010000:{*(.debug_info*)}
        .data : AT (_sidata) {_sdata = .; *(.data*) *(.sdata)  _edata = .; }
        .bss : { _sbss = .;  *(EXCLUDE_FILE (*yc_audio.o) *.bss*) *(*.scommon*) *(EXCLUDE_FILE (*yc_audio.o) COMMON)}
        _retention_end = .;
        . = 0x10011000;
        ASSERT(_retention_end < ., "retention data size overflow")

        .noretention0(NOLOAD) : {*(*.memory_protect_under_lpm*) }
        .noretention : { *(*.bss*)  *(COMMON)}
        _ebss = .;
        . = 0x1001d800;
        _stack = .;
        ASSERT(_ebss < _stack, "data size overflow")
}

/*SECTIONS
{
		. = 0x01000000;
				.text : { KEEP(*startup_rv32.o)}		
		. = 0x01001000;
				.text1 : {KEEP(*app_config.o) *(.text*) *(.rdata*) *(.rodata*) *(.srodata*)  *(.eh_frame*)   *(.srodata.cst*)}
        _sidata = ALIGN(4);
        .local 0x10002000 : {*(.local*)}
        . = 0x10010000;
        .data : AT (_sidata) {_sdata = .; *(.data*) *(.sdata)  _edata = .; }
        .bss : { _sbss = .; *(*.bss*) *(*.scommon*) }
        _ebss = .;
        . = 0x1001d800;
        _stack = .;
        ASSERT(_ebss < _stack, "data size overflow")
}*/