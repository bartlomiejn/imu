MEMORY
{
	rom (rx) : ORIGIN = 0x08000000, LENGTH = 256K
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 40K
}

_estack = ORIGIN(ram) + LENGTH(ram);

ENTRY(Reset_Handler)

SECTIONS
{
	.isr_vector :
	{
		. = ALIGN(4);
		KEEP(*(.isr_vector))
		. = ALIGN(4);
	} >rom

	.text : {
		. = ALIGN(4);
		_stext = .;

		*(.text)
		*(.text*)
		*(.rodata)
		*(.rodata*)
		*(.eh_frame)

		KEEP(*(.init))
		KEEP(*(.fini))

		. = ALIGN(4);
		_etext = .;
	} >rom

	.ARM.extab : {
		*(.ARM.extab*)
	} >rom
	.ARM : {
		__exidx_start = .;
		*(.ARM.exidx*)
		__exidx_end = .;
	} >rom

	.preinit_array : {
		__preinit_array_start = .;
		KEEP (*(.preinit_array))
		__preinit_array_end = .;
	} >rom
	.init_array : {
		__init_array_start = .;
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array))
		__init_array_end = .;
	} >rom
	.fini_array : {
		__fini_array_start = .;
		KEEP (*(SORT(.fini_array.*)))
		KEEP (*(.fini_array))
		__fini_array_end = .;
	} >rom

	_sidata = LOADADDR(.data);

	.data : {
		. = ALIGN(4);
		_sdata = .;

		*(.data)
		*(.data*)

		. = ALIGN(4);
		_edata = .;
	} >ram AT >rom

	.bss : {
		_sbss = .;
		__bss_start__ = _sbss;

		*(.bss)
		*(.bss*)
		*(COMMON)

		. = ALIGN(4);
		_ebss = .;
		__bss_end__ = _ebss;
	} >ram

	. = ALIGN(4);
	end = .;
	_end = .;
}