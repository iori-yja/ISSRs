/***********************************************************************/
/*  LPC2388-ROM.ld:  Linker Script File                                */
/***********************************************************************/
OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY("_startup")

/* 
   Based on scripts found in the LPC yahoo-group and the Keil examples 
   modified by Martin Thomas, Kaiserslautern, Germany
   http://www.siwawi.arubi.uni-kl.de/avr_projects
   
   Rev 1
   20090309 Nemui Trinomious adopted to LPC2388
   Rev 2
   20090508 Nemui Changed ".text ALIGN 4->8"(RAMSTART workaround)
   Rev 3
   20090607 Nemui added entry potint (LPCUSB workaround)
   Rev 4
   20090804 Nemui Fixed entry potint
   Rev 5
   200911xx Nemui Fixed ".text ALIGN 8->4"(adopted to Sourcery G++ Lite 2009q3-68 (GCC4.4.1))
   Rev 6
   20091220 Nemui Fixed more FreeRTOS's Consideration
*/

/* LPC2388 Memory Definitions */
MEMORY
{
  ROM (rx) : ORIGIN = 0x00000000, LENGTH = (512k-8k)	/* Flash: 512k - boot code 8kbyte*/
  RAM (w)  : ORIGIN = 0x40000000, LENGTH = (64k-32)	    /* SRAM: 64k - IAP work 32byte */
  URAM (w) : ORIGIN = 0x7FD00000, LENGTH = (16k)	    /* USB RAM: 16k */
  ERAM (w) : ORIGIN = 0x7FE00000, LENGTH = (16k)	    /* Ethernet RAM: 16k */
  BRAM (w) : ORIGIN = 0xE0084000, LENGTH = (2k)	    	/* Battery RAM: 2k */
}

_ram_base  = 0x40000000;
_stack_end = _ram_base + 64k-32-4;
__stack_end__ = _stack_end ;

/* Section Definitions */
SECTIONS
{
  /* first section is .text which is used for code */
   .text :
	{
		KEEP(*(.VECTOR))
		KEEP(*(.vectorg))
		KEEP(*(.RESET))
		KEEP(*(.startup))
		KEEP(*(.SWI_HANDLER))
		. = ALIGN(4);
		KEEP(*(.init))             /* Startup code from .init-section */
		*(.text .text.*)           /* remaining code */
		*(.gnu.linkonce.t.*)
		*(.glue_7)
		*(.glue_7t)
		*(.vfp11_veneer)
		*(.janus_2cc_veneer)
		*(.v4_bx)
		*(.gcc_except_table)
		*(.rodata)                 /* read-only data (constants) */
		*(.rodata*)
		*(.gnu.linkonce.r.*)
		. = ALIGN(4);
	} > ROM

	. = ALIGN(4);
  
	/* .ctors .dtors are used for c++ constructors/destructors */
	/* added by Martin Thomas 4/2005 based on Anglia Design example */
	.ctors :
	{
		PROVIDE(__ctors_start__ = .);
		KEEP(*(SORT(.ctors.*)))
		KEEP(*(.ctors))
		. = ALIGN(4);
		PROVIDE(__ctors_end__ = .);
		. = ALIGN(4);
	} >ROM
	
	.dtors :
	{
		PROVIDE(__dtors_start__ = .); 
		KEEP(*(SORT(.dtors.*)))
		KEEP(*(.dtors))
		. = ALIGN(4);
		PROVIDE(__dtors_end__ = .);
		. = ALIGN(4);
	} >ROM
    
    . = ALIGN(4);
	/* mthomas - end */
	
	.ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > ROM

	__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} >ROM
	__exidx_end = .;
	
	.eh_frame_hdr : { *(.eh_frame_hdr) } >ROM
	.eh_frame       :{ KEEP (*(.eh_frame)) } >ROM
	
	. = ALIGN(4);
	/* nemui - end */
	
	__end_of_text__ = .;
	_etext = . ;
	PROVIDE (etext = .);


	/* .data section which is used for initialized data */
	.data : AT (_etext)
	{
		_data = .;
		__data_beg__ = .;
		__data_beg_src__ = __end_of_text__;
		KEEP(*(.vectmapped)) /* mthomas: when code in ROM and vectors remapped */
		. = ALIGN(4);
		*(.fastrun) /* mthomas: for "RAMFUNC" */
		. = ALIGN(4);
		SORT(CONSTRUCTORS) /* mt 4/2005 */
		. = ALIGN(4);
		*(.data)
		*(.data.*)
		*(.gnu.linkonce.d*)
		. = ALIGN(4);
	} > RAM
 
	. = ALIGN(4);
	__data_end__ = .;
	_edata = . ;
	PROVIDE (edata = .);
  
		
	/* .bss section which is used for uninitialized data */
	.bss (NOLOAD) :
	{
		__bss_start = . ;
		_bss_start = . ;
		__bss_beg__ = .;
		PROVIDE (_bss_start = .);
		__bss_start__ = . ;
		*(.bss) 
		*(.bss.*) /* mthomas 7/2006 */
		*(.gnu.linkonce.b*)
		*(COMMON)
		. = ALIGN(4);
	} > RAM
	
	. = ALIGN(4);
	__bss_end__ = . ;
	PROVIDE (__bss_end = .);
	_bss_end = . ;
	PROVIDE (_bss_end = .);
	__end__ = . ;
	. = ALIGN(4);

	.heap (NOLOAD) :
	{
		_end = . ;
		PROVIDE (_end = .);
		PROVIDE (end = .);
		PROVIDE (_heap = .);
		*(.heap)
		*(.heap*)
		*(.HEAP)
		. = ALIGN(4);
	} > RAM
	
	. = ALIGN(4);
	_heap_end = . ;
	PROVIDE (_heap_end = .);
  
	.stack ALIGN(4) :
	{
		_stack_top = . ;
		PROVIDE (_stack_top = .);
		*(.stack)
		*(.STACK)
		_stack = . ;
		PROVIDE (_stack = .);
		. = ALIGN(4);
	} > RAM
  
	.usbram (NOLOAD) :
	{
		__usbram_start = . ;
		__usbram_start__ = . ;
		*(.usbram)
		. = ALIGN(4);
	} > URAM

	.etherram (NOLOAD) :
	{
		__etherram_start = . ;
		__etherram_start__ = . ;
		*(.etherram)
		. = ALIGN(4);
	} > ERAM

	.batteryram (NOLOAD) :
	{
		__batteryram_start = . ;
		__batteryram_start__ = . ;
		*(.batteryram)
		. = ALIGN(4);
	} > BRAM
 
	/* Stabs debugging sections.  */
	.stab          0 : { *(.stab) }
	.stabstr       0 : { *(.stabstr) }
	.stab.excl     0 : { *(.stab.excl) }
	.stab.exclstr  0 : { *(.stab.exclstr) }
	.stab.index    0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment       0 : { *(.comment) }
	/* DWARF debug sections.
	 Symbols in the DWARF debugging sections are relative to the beginning
	 of the section so we begin them at 0.  */
	/* DWARF 1 */
	.debug          0 : { *(.debug) }
	.line           0 : { *(.line) }
	/* GNU DWARF 1 extensions */
	.debug_srcinfo  0 : { *(.debug_srcinfo) }
	.debug_sfnames  0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges  0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	/* DWARF 2 */
	.debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }
	.debug_macinfo  0 : { *(.debug_macinfo) }
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }
}
