MEMORY
{
  /* Define each memory region */
  MFlash32 (rx) : ORIGIN = 0x0, LENGTH = 0x7000 /* 32k but last 0x2000 is MothionArea*/
  FlashMap (rx) : ORIGIN = 0x7000, LENGTH = 0x1000 /* 32k but last 0x2000 is MothionArea*/
  RamLoc8 (rwx) : ORIGIN = 0x10000000, LENGTH = 0x2000 /* 8k */

}
  /* Define a symbol for the top of each memory region */
  __top_MFlash32 = 0x0 + 0x7000;	
  __top_FlashMap = 0x7000 + 0x1000;	
  __top_RamLoc8 = 0x10000000 + 0x2000;

