/* CodeWarrior ARM Runtime Support Library

 * Copyright © 2012 Freescale Semiconductors. All rights reserved.

 *

 * $Date: 2013/02/08 07:14:44 $

 * $Revision: 1.6 $

 */





/*

 *	__arm_start.c	-	entry-point for ARM programs

 *

 */

#include <string.h>

#include <stdlib.h>

#include <runtime_configuration.h>

#include <start.h>

//_EWL_BEGIN_EXTERN_C



#if SUPPORT_SEMIHOST_ARGC_ARGV

#define __MAX_CMDLINE_ARGS 10

static char *argv[__MAX_CMDLINE_ARGS] = { 0 };

#else

static char *argv[] = { 0 };

#endif



#if __GNUC__

#define __call_static_initializers __init_cpp

#endif

#if SUPPORT_SEMIHOST_ARGC_ARGV

extern int __argc_argv(int, char **);

#endif /* SUPPORT_SEMIHOST_ARGC_ARGV */



extern void __call_static_initializers(void);

extern int main(int, char **);



#ifdef __VFPV4__

extern void __fp_init(void);

#endif /* __VFPV4__ */



extern void __init_registers();

extern void __init_hardware();

extern void __init_user();



#if defined(__APCS_ROPI)

extern void __init_pic();

#endif



#if defined(__APCS_RWPI)

extern void __init_pid();

#endif



#if defined(__APCS_ROPI) || defined(__APCS_RWPI)

extern void __load_static_base();

#endif



#if defined(__SEMIHOSTING)

extern void __init_semihost(void) _EWL_WEAK;

#endif



#if SUPPORT_ROM_TO_RAM

extern void __copy_rom_sections_to_ram(void);

//extern char __S_romp[];

/* format of the ROM table info entry ... */
typedef struct RomInfo {
  unsigned long	Source;
  unsigned long	Target;
  unsigned long Size;
} RomInfo;

extern RomInfo __S_romp[] __attribute__((weak));    /* linker defined symbol */

/*
 *	Routine to copy a single section from ROM to RAM ...
 */
void __copy_rom_section(unsigned long dst, unsigned long src, unsigned long size)
{
	unsigned long len = size;

	const unsigned int size_int = sizeof(int);
	const unsigned int mask_int = sizeof(int)-1;

	const unsigned int size_short = sizeof(short);
	const unsigned int mask_short = sizeof(short)-1;

	const unsigned int size_char = sizeof(char);

	if( dst == src || size == 0)
	{
		return;
	}


	while( len > 0)
	{

		if( !(src & mask_int) && !(dst & mask_int) && len >= size_int)
		{
			*((int *)dst)  = *((int*)src);
			dst += size_int;
			src += size_int;
			len -= size_int;
		}
		else if( !(src & mask_short) && !(dst & mask_short) && len >= size_short)
		{
			*((short *)dst)  = *((short*)src);
			dst += size_short;
			src += size_short;
			len -= size_short;
		}
		else
		{
			*((char *)dst)  = *((char*)src);
			dst += size_char;
			src += size_char;
			len -= size_char;
		}
	}
}

/*
 *	Routine that copies all sections the user marked as ROM into
 *	their target RAM addresses ...
 *
 *	__S_romp is defined in the linker command file
 *  It is a table of RomInfo
 *	structures.  The final entry in the table has all-zero
 *	fields.
 */
void __copy_rom_sections_to_ram(void)
{

	int				index;

	if (__S_romp == 0L) return;

	/*
	 *	Go through the entire table, copying sections from ROM to RAM.
	 */
	for (index = 0;
		 __S_romp[index].Source != 0 ||
		 __S_romp[index].Target != 0 ||
		 __S_romp[index].Size != 0;
		 ++index)
	{
		__copy_rom_section( __S_romp[index].Target,
							__S_romp[index].Source,
							__S_romp[index].Size );

	}
}

#endif



static void zero_fill_bss(void)

{

	extern char __START_BSS[];

	extern char __END_BSS[];



	memset(__START_BSS, 0, (__END_BSS - __START_BSS));

}

#define __thumb

#ifndef __thumb // Thumb version

#error Thumb startup

#endif



// To keep iar debugger happy

//void __iar_program_start(void) _EWL_NAKED;

void __thumb_startup(void);

void __iar_program_start()

{

	__thumb_startup();

}





void __thumb_startup(void);// _EWL_NAKED;

void __thumb_startup(void)

{

		// Setup registers

		//__init_registers();



		// setup hardware

		__init_hardware();





#if defined(__APCS_ROPI) || defined(__APCS_RWPI)

		//	static base register initialization

		__load_static_base();

#endif

#if defined(__APCS_RWPI)

		//	-pid

		//	setup static base for SB relative position independent data

		//	perform runtime relocation

		__init_pid();

#endif

#if defined(__APCS_ROPI)

		//	-pic

		//	perform runtime relocation for position independent code

		__init_pic();

#endif

		//	zero-fill the .bss section

		zero_fill_bss();



#if SUPPORT_ROM_TO_RAM

		if (__S_romp != 0L)

			__copy_rom_sections_to_ram();

#endif





		//      initialize the floating-point library

#ifdef __VFPV4__

		__fp_init();

#endif



		//	call C++ static initializers

		//__call_static_initializers();



		// initializations before main, user specific

		//__init_user();



#if defined(__SEMIHOSTING)

		// semihost initializations

		__init_semihost();

#endif



		//	call main(argc, &argv)

#if SUPPORT_SEMIHOST_ARGC_ARGV

		exit(main(__argc_argv(__MAX_CMDLINE_ARGS, argv), argv));

#else

		//exit(main(0, argv));
		start();	
#endif




		//	should never get here

		while (1);



}



//_EWL_END_EXTERN_C

