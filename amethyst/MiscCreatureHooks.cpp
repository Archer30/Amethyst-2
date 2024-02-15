#include "stdafx.h"
// #include "..\__include\era.h"

/*
//using Era::;
#define PEvent Era::TEvent*
#define ErmV Era::v
using Era::ExecErmCmd;
using Era::ConnectEra;
using Era::RegisterHandler;
using Era::ReadSavegameSection;
using Era::WriteSavegameSection;
*/
namespace Era {
	typedef char  TErmZVar[512];
	extern TErmZVar* z;
}
extern Patcher* globalPatcher;
extern PatcherInstance* Z_Amethyst;


extern char names[MONSTERS_AMOUNT][64];
extern char names_pl[MONSTERS_AMOUNT][64];
extern char descripts[MONSTERS_AMOUNT][256];

char default_names[MONSTERS_AMOUNT][64];
char default_names_pl[MONSTERS_AMOUNT][64];
char default_descripts[MONSTERS_AMOUNT][256];

extern MONSTER_PROP new_monsters[MONSTERS_AMOUNT];
extern int all_creatures;

/*
_LHF_(hook_UN_G1_get) {
	long info_type = *(int*)(c->ebp-0xd4);
	long z_var_target = *(int*)(c->ebp - 0x0c);
	long creature_type = *(int*)(c->ebp - 0x44);

	char* output = Era::z[z_var_target];
	MONSTER_PROP* monster = new_monsters + creature_type;

	switch (info_type) {
	case 0:
		strcpy_s(output, 512, monster->name);
		break;
	case 1:
		strcpy_s(output, 512, monster->pluralname);
		break;
	case 2:
		strcpy_s(output, 512, monster->features);
		break;
	}

	c->return_address = 0x007334C7;
	return NO_EXEC_DEFAULT;
}
*/

_LHF_(hook_UN_G1_set) {

	long info_type = *(int*)(c->ebp - 0xd4);
	long z_var_source = *(int*)(c->ebp - 0x0c);
	long creature_type = *(int*)(c->ebp - 0x44);

	char* input = Era::z[z_var_source];
	MONSTER_PROP* monster = new_monsters + creature_type;

	if (input)
	switch (info_type) {
	case 0:
		strcpy_s(monster->name, 64, input);
		break;
	case 1:
		strcpy_s(monster->pluralname, 64, input);
		break;
	case 2:
		strcpy_s(monster->features, 256, input);
		break;
	}


	//c->return_address = 0x00733577;
	
	c->return_address = 0x007334C7;
	return NO_EXEC_DEFAULT;

}


_LHF_(hook_UN_G1_reset) {

	long info_type = *(int*)(c->ebp - 0xd8);
	// long z_var_source = *(int*)(c->ebp - 0x0c);
	long creature_type = *(int*)(c->ebp - 0x44);

	// char* input = Era::z[z_var_source];
	MONSTER_PROP* monster = new_monsters + creature_type;


	switch (info_type) {
	case 0:
		strcpy_s(monster->name, 64, default_names[creature_type]);
		break;
	case 1:
		strcpy_s(monster->pluralname, 64, default_names_pl[creature_type]);
		break;
	case 2:
		strcpy_s(monster->features, 256, default_descripts[creature_type]);
		break;
	}

	c->return_address = 0x00733577;
	return NO_EXEC_DEFAULT;

}

void MiscCreatureHooks_apply() {
	Z_Amethyst->WriteLoHook(0x00733409, hook_UN_G1_set);
	Z_Amethyst->WriteLoHook(0x007334D5, hook_UN_G1_reset);

	// Z_Amethyst->WriteLoHook(0x00733409, hook_00733409);
	// Z_Amethyst->WriteLoHook(0x007334D5, hook_007334D5);

}


void MiscCreatureHooks_apply_strings() {
	for (long creature_type = 0; creature_type <= all_creatures; ++creature_type) {

		MONSTER_PROP* monster = new_monsters + creature_type;
		strcpy_s(default_names[creature_type], monster->name);
		strcpy_s(default_names_pl[creature_type], monster->pluralname);
		strcpy_s(default_descripts[creature_type], monster->features);

	}

	/*
	memcpy(names, default_names, sizeof(names));
	memcpy(names_pl, default_names_pl, sizeof(names_pl));
	memcpy(descripts, default_descripts, sizeof(descripts));
	*/
}