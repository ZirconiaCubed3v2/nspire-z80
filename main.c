#include <os.h>
#include <stdint.h>
#include "drz80.h"
#include "lcd.h"
#include "keypad.h"
//#include "mmap.h"

void cpu_init();
unsigned int cpu_rebasePC(unsigned short x);
unsigned int cpu_rebaseSP(unsigned short x);
unsigned short cpu_read16(unsigned short idx);
unsigned char cpu_read8(unsigned short idx);
void cpu_write16(unsigned short val, unsigned short idx);
void cpu_write8(unsigned char val, unsigned short idx);
unsigned char cpu_in(unsigned short port);
//static unsigned char cpu_in_(unsigned short port);
void cpu_out(unsigned short port, unsigned char val);
void cpu_irq_callback();

//#define printf(...)


uint8_t *flash;
struct DrZ80 ZCpu;

int main(void){
	FILE *romfile;
	if(!(romfile = fopen("rom.tns", "rb"))){
		show_msgbox("Error", "Could not open rom");
		return 1;
	}
	fseek(romfile, 0, SEEK_END);
	int romsize = ftell(romfile);
	fseek(romfile, 0, 0);
	//printf("%d\n", romsize);
	//mmap_init();
	flash = calloc(0x20000, 1);
	fread(flash, sizeof(char), romsize, romfile);
	
	lcd_init();
	cpu_init();
	//asm(" b .");
	printf("%08x\n", ZCpu.Z80PC_BASE);
	//printf("%p\n", mmap_bank_for_addr(0));
	/*for(i = 0; i < 0x100; i++){
		printf("%02x", cpu_read8(i));
	}*/
	while(1){
		DrZ80Run(&ZCpu, 1000);
		if(isKeyPressed(KEY_NSPIRE_ESC)) break;
	}
	//mmap_end();
	free(flash);
	return 0;
}

void cpu_init(){
	memset(&ZCpu, 0, sizeof(ZCpu));
	ZCpu.z80_rebasePC=cpu_rebasePC;
	ZCpu.z80_rebaseSP=cpu_rebaseSP;
	ZCpu.z80_read8   =cpu_read8;
	ZCpu.z80_read16  =cpu_read16;
	ZCpu.z80_write8  =cpu_write8;
	ZCpu.z80_write16 =cpu_write16;
	ZCpu.z80_in      =cpu_in;
	ZCpu.z80_out     =cpu_out;
	ZCpu.z80_irq_callback = cpu_irq_callback;
	ZCpu.Z80PC = cpu_rebasePC(0);
	ZCpu.Z80SP = cpu_rebaseSP(0);
}

void cpu_irq_callback(){
	printf("irq\n");
}

//void pdb(uint32_t pc){
//	printf("pc(a) %02x\n", pc);
//}

unsigned int cpu_rebasePC(unsigned short x){
	//printf("rebasePC 0x%x\n", x);
	ZCpu.Z80PC_BASE = (unsigned int)flash;//mmap_bank_for_addr(x);
	return ZCpu.Z80PC_BASE + x;//(x & 0x3FFF);
}

unsigned int cpu_rebaseSP(unsigned short x){
	//printf("rebaseSP 0x%x\n", x);
	ZCpu.Z80SP_BASE = (unsigned int)flash;//mmap_bank_for_addr(x);
	return ZCpu.Z80SP_BASE + x;//(x & 0x3FFF);
}

unsigned short cpu_read16(unsigned short idx){
	//printf("read16 0x%x\n", idx);
	//asm(" b .");
	uint8_t *p = flash+idx;//mmap_z80_to_arm(idx);
	return *p | *(p+1) << 8;
}
unsigned char cpu_read8(unsigned short idx){
	//printf("read8 0x%x\n", idx);
	return flash[idx];//*(mmap_z80_to_arm(idx));
}

void cpu_write16(unsigned short val, unsigned short idx){
	//printf("write16 0x%x 0x%x\n", idx, val);
	uint8_t *p = flash+idx;//mmap_z80_to_arm(idx);
	*p++ = val & 0xff;
	*p = val >> 8;
}

void cpu_write8(unsigned char val, unsigned short idx){
	//printf("write8 0x%x 0x%x\n", idx, val);
	//*(mmap_z80_to_arm(idx)) = val;
	flash[idx] = val;
}
/*unsigned char cpu_in(unsigned short port){
	port &= 0xff;
	uint8_t x = cpu_in_(port);
	//if(port == 1) printf("in %x %x\n", port, x);
	return x;
}*/
unsigned char cpu_in(unsigned short port){
	port &= 0xff;
	switch(port){
		case 0x01:
		return keypad_read();
		//case 0x05:
		//case 0x06:
		//case 0x07:
		//return mmap_in(port);
		case 0x10:
		case 0x12:
		return lcd_cmd_read();
		case 0x11:
		case 0x13:
		return lcd_data_read();
	}
	return 0;
}

void cpu_out(unsigned short port, unsigned char val){
	port &= 0xff;
	//if(port == 1) printf("out %x %x\n", port, val);
	switch(port){
		case 0x01:
		keypad_write(val);
		break;
		//case 0x05:
		//case 0x06:
		//case 0x07:
		//mmap_out(port, val);
		//break;
		case 0x10:
		case 0x12:
		lcd_cmd(val);
		break;
		case 0x11:
		case 0x13:
		lcd_data(val);
		break;
	}
}