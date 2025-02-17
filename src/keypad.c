#include <os.h>
#include "keypad.h"
#define K(x) &(KEY_NSPIRE_##x)
#define J(x) &(KEY_84_##x)
#define KEY_TABLE n_key_ar
uint8_t mon = 0;

const t_key *n_legacy_key_ar[8][7] = {
	{K(DOWN),	K(ENTER),	K(NEGATIVE),	K(PERIOD),  	K(0),   NULL,		K(EQU)},
	{K(LEFT),	K(PLUS),	K(3),		K(2),		K(1),	K(FLAG),	K(TRIG)},
	{K(RIGHT),	K(MINUS),	K(6),		K(5),		K(4),	K(eEXP),	K(EE)},
	{K(UP),		K(MULTIPLY),	K(9),		K(8),		K(7),	K(TENX),	K(FRAC)},
	{NULL,		K(DIVIDE),	K(RP),		K(LP),		K(COMMA),K(SQU),	K(A)},
	{NULL,		K(EXP),		K(TAN),		K(COS),		K(SIN),	K(QUESEXCL),	K(CTRL)},
	{NULL,		K(RET),		K(VAR),		K(SCRATCHPAD),	K(DOC),	K(MENU),	K(TAB)},
	{NULL,		NULL,		NULL,		K(PI),		K(X),	K(SHIFT),	K(DEL)}
};

const t_key *n_key_ar[8][7] = {
	{K(DOWN),	K(ENTER),	K(NEGATIVE),	K(PERIOD),	K(0),	NULL,		K(CAT)},
	{K(LEFT),	K(PLUS),	K(3),		K(2),		K(1),	K(FLAG),	K(FRAC)},
	{K(RIGHT),	K(MINUS),	K(6),		K(5),		K(4),	K(eEXP),	K(EE)},
	{K(UP),		K(MULTIPLY),	K(9),		K(8),		K(7),	K(TENX),	K(TRIG)},
	{NULL,		K(DIVIDE),	K(RP),		K(LP),		K(COMMA),K(SQU),	K(EQU)},
	{NULL,		K(EXP),		K(G),		K(F),		K(E),	K(QUESEXCL),	K(CTRL)},
	{NULL,		K(RET),		K(VAR),		K(SCRATCHPAD),	K(DOC),	K(MENU),	K(TAB)},
	{NULL,		NULL,		NULL,		K(PI),		K(X),	K(SHIFT),	K(DEL)}
};

const t_key *n_84_key_ar[8][7] = {
	{J(DOWN),	J(ENTER),	J(NEGATIVE),	J(PERIOD),	J(0),	NULL,		J(GRAPH)},
	{J(LEFT),	J(PLUS),	J(3),		J(2),		J(1),	J(STO),		J(TRACE)},
	{J(RIGHT),	J(MINUS),	J(6),		J(5),		J(4),	J(LN),		J(ZOOM)},
	{J(UP),		J(MULTIPLY),	J(9),		J(8),		J(7),	J(LOG),		J(WIND)},
	{NULL,		J(DIVIDE),	J(RP),		J(LP),		J(COMMA),J(SQU),	J(YEQU)},
	{NULL,		J(EXP),		J(TAN),		J(COS),		J(SIN),	J(INV),		J(2ND)},
	{NULL,		J(CLEAR),	J(VARS),	J(PRGM),	J(APPS),J(MATH),	J(MODE)},
	{NULL,		NULL,		NULL,		J(STAT),	J(X),	J(ALPHA),	J(DEL)}
};

const t_key* (*key_ar[])[7] = {
	n_key_ar,
	n_84_key_ar,
	n_legacy_key_ar
};

static enum z80_keypad_type key_type = 0;

void keypad_set_type(enum z80_keypad_type val) {
	key_type = val;
}

void keypad_write(uint8_t val){
	//printf("keypad_write %02x\n", val);
	mon = ~val;
}

uint8_t keypad_read(){
	const t_key * const(* c_key_ar)[7] = key_ar[(int)key_type];
	int i;
	uint8_t o = 0;
	for(i = 0; i < 7; i++){
		if(mon & 1<<i){
			int j;
			for(j = 0; j < 8; j++){
				const t_key *k = c_key_ar[j][i];
				if(k != NULL) o |= isKeyPressed(*k) << j;
			}
		}
	}
	return ~o;
}

void keypad_save(FILE *f) {
	fputc(mon, f);
}

void keypad_restore(FILE *f) {
	mon = fgetc(f);
}

