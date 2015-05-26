void mmap_init();
void mmap_end();
uint8_t *mmap_z80_to_arm(uint16_t z80addr);
uint8_t *mmap_bank_for_addr(uint16_t z80addr);
uint8_t *mmap_base_addr(uint16_t z80addr);
void mmap_out(uint8_t port, uint8_t val);
uint8_t mmap_in(uint8_t port);
extern uint8_t *flash;
extern uint8_t *ram;
