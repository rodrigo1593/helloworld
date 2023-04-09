#include <stdio.h>

#include <kernel.h>
#include <sifrpc.h>
#include <iopcontrol.h>
#include <sbv_patches.h>
#include <loadfile.h>

int fileXioInit();

extern unsigned char iomanX_irx[] __attribute__((aligned(16)));
extern unsigned int size_iomanX_irx;

extern unsigned char fileXio_irx[] __attribute__((aligned(16)));
extern unsigned int size_fileXio_irx;


static void reset_IOP() {
   SifExitRpc();
	SifInitRpc(0);

	while(!SifIopReset(NULL, 0x80000000)){};

	while(!SifIopSync()){};
	SifInitRpc(0);
	sbv_patch_enable_lmb();
	sbv_patch_disable_prefix_check();
}

static void init_drivers() {
   SifExecModuleBuffer(&iomanX_irx, size_iomanX_irx, 0, NULL, NULL);
   SifExecModuleBuffer(&fileXio_irx, size_fileXio_irx, 0, NULL, NULL);

   fileXioInit();
}

int main()
{
   reset_IOP();
	init_drivers();
	printf("Hello world with fileXio loaded!\n");
	reset_IOP();
	printf("Hello world after reset IOP!\n");
   return 0;
}