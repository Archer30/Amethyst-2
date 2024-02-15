
#include "stdafx.h"

extern MONSTER_PROP new_monsters[MONSTERS_AMOUNT];
extern char Monster_Ban_Table[MONSTERS_AMOUNT];

#define _GameMgr_ void
int __stdcall GameMgr_GetRandMonster_ByLevel(HiHook* hook, _GameMgr_* gm, int mon_lvl_Lo, int mon_lvl_Hi)
{
	if (mon_lvl_Lo < 0 || mon_lvl_Lo > 6) {
		v_MsgBox((char*)"Please use the monster level 0-6", 1);
		return 0;
	}

	if (mon_lvl_Hi < 0 || mon_lvl_Hi > 6) {
		v_MsgBox((char*)"Please use the monster level 0-6", 1);
		return 0;
	}

	if (mon_lvl_Lo > mon_lvl_Hi) {
		v_MsgBox((char*)"Are you kidding?!", 1);
		return 0;
	}

	const int count = MONSTERS_AMOUNT;
	char monArray[count];

	for (int i = 0; i < count; i++)
		monArray[i] = 0;

	for (int i = 0; i < count; i++) {
		int lvl = new_monsters[i].level;

		if (lvl >= mon_lvl_Lo && lvl <= mon_lvl_Hi
			&& !Monster_Ban_Table[i])
			monArray[i] = 1; 
	}



	int result;

	do { 
		result = Randint(0, count - 1);
	} while (!monArray[result]);

	return result;
}
