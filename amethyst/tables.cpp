
#include "stdafx.h"
// #include "H3CombatMonster.hpp"
#include "../../H3API/single_header/H3API.hpp"
using namespace h3;

#include "erm_types.h"

extern PatcherInstance* Z_Amethyst;

extern bool FilterNecromancyCreatures;
extern bool DisableNecromancyFeatures;
extern int all_creatures;
DWORD demonolgy_Temp1;

DWORD GhostMultiplyer_028460f8[MONSTERS_AMOUNT + 64];

DWORD CreatureSpellPowerMultiplier[MONSTERS_AMOUNT + 64];
DWORD CreatureSpellPowerDivider[MONSTERS_AMOUNT + 64];
DWORD CreatureSpellPowerAdder[MONSTERS_AMOUNT + 64];
char CreatureSpellPowerScaled[MONSTERS_AMOUNT + 64];

DWORD CreatureMimicArtifact[MONSTERS_AMOUNT + 64];
DWORD CreatureMimicArtifact2[MONSTERS_AMOUNT + 64];

BYTE addditional_moves[42];
BYTE addditional_shots[42];
BYTE addditional_melees[42];

/////
char Spell_Immunities[MONSTERS_AMOUNT + 64][16];
bool has_wog_resist(const char* str, int Spell) {
	Byte* SpTable;
	__asm {
		mov   eax, 0x687FA8
		mov   eax, [eax]
		mov   SpTable, eax
	}

	for (int i = 0; i < 16; ++i) {
		switch (str[i]) {
		case '6':
		case '7':
		case '8':
		case '9':
		case ':':
			if (*(int*)&SpTable[0x88 * Spell + 0x18] < str[i] - '6' + 2)  return true;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5': // immune to spell of levels
			if (*(int*)&SpTable[0x88 * Spell + 0x18] < str[i] - '1' + 2) return true;
			break;
		case 'C': // immune to cold spells (Ice Bolt, Frost Ring)
			if (Spell == 16 || Spell == 20) return true;
			break;
		case 'L': // immune to Lightning spells (Lightning Bolt, Chain Lightning and Titan's Lightning Bolt)
			if (Spell == 17 || Spell == 19 || Spell == 57) return true;
			break;
		case 'R': // immune to Armageddon
			if (Spell == 26) return true;
			break;
		case 'M': // immune to   Meteor Shower
			if (Spell == 23) return true;
			break;
		case 'I': // immune to Implosion
			if (Spell == 18) return true;
			break;
			//          case 'P': // immune to Any Dispell
			//            if(Spell==35) val=100.1;
			//            break;
			//          case 'p': // immune to Hostile Dispell
			//            if(Spell==35) val=100.1;
			//            break;
			//          case 'N': // immune to Dispell Benefic. Spells
			//            if(Spell==78) val=100.1;
			//            break;
		case 'B': // immune to Blind
			if (Spell == 62) return true;
			break;
		case 'K': // immune to Berserk
			if (Spell == 59) return true;
			break;
		case 'H': // immune to Hypnotize
			if (Spell == 60) return true;
			break;
		case 'S': // immune to Slow
			if (Spell == 54) return true;
			break;
		case 'O': // immune to Fire Direct Damage Spell
			//10. Immune to all Fire Damage spells (but NOT Magic Arrow). The
			//specific spells are: Fire Wall, Fireball, Land Mine, Armageddon, Fire
			//Shield, Inferno
			if (Spell == 13 || Spell == 21 || Spell == 11 || Spell == 26 || Spell == 29 || Spell == 22) return true;
			break;
		case 'D': // immune to All Direct Damage Spell
			//11. And an Immune to All Direct Damage Spells might also be useful if
			//it's not too much trouble. The list of spells would be: Magic Arrow,
			//Lightning Bolt, Chain Lightning, Meteor Shower, Implosion, Fireball,
			//Armageddon, Inferno, Ice Bolt, Frost Ring.
			if (Spell == 15 || Spell == 17 || Spell == 19 || Spell == 23 || Spell == 18 || Spell == 21 || Spell == 26 || Spell == 22 || Spell == 16 || Spell == 20) return true;
			break;
		case 'm': // immune to some mind spells (Blind, Berserk, Sorrow, Hypnotize and Forgetfulness)
			if (Spell == 62 || Spell == 59 || Spell == 50 || Spell == 60 || Spell == 61) return true;
			break;
		case 'A': // immune to Air schools
		case 'a': // immune to Air schools
			if (*(int*)&SpTable[0x88 * Spell + 0x1C] & 0x1) return true;
			break;
		case 'F': // immune to Fire schools
		case 'f': // immune to Fire schools
			if (*(int*)&SpTable[0x88 * Spell + 0x1C] & 0x2) return true;
			break;
		case 'W': // immune to Water schools
		case 'w': // immune to Water schools
			if (*(int*)&SpTable[0x88 * Spell + 0x1C] & 0x4) return true;
			break;
		case 'E': // immune to Earth schools
		case 'e': // immune to Earth schools
			if (*(int*)&SpTable[0x88 * Spell + 0x1C] & 0x8) return true;
			break;

		case 'r': if (Spell == 24) return true; break;
		case 'u': if (Spell == 25) return true; break;

		default:
			continue;
		}
	}
	return false;
}

_LHF_(z_hook_0071CC52) {
	H3CombatMonster* mon = (H3CombatMonster*)*(int*)(c->ebp + 0x08);
	int Spell = *(int*)(c->ebp + 0x10);
	int mon_id = mon->type;
	int mon_stack = mon->side * 21 + mon->sideIndex;

	if (has_wog_resist(&Spell_Immunities[mon_id][0], Spell)
		|| has_wog_resist(&Spell_Immunities[MONSTERS_AMOUNT+mon_stack][0], Spell)
		) {
		c->return_address = 0x0071CFB4; c->eax = 100;
	}

	return NO_EXEC_DEFAULT;
}

///// MOP Area Begin
DWORD RegenerationHitPoints_Table[MONSTERS_AMOUNT + 64];
char  RegenerationChance_Table[MONSTERS_AMOUNT + 64];
WORD  ManaDrain_Table[MONSTERS_AMOUNT + 64];
char  SpellsCostDump_Table[MONSTERS_AMOUNT + 64];
char  SpellsCostLess_Table[MONSTERS_AMOUNT + 64];
char  Counterstrike_Table[MONSTERS_AMOUNT + 64];
char  AlwaysPositiveMorale_Table[MONSTERS_AMOUNT + 64];
char  AlwaysPositiveLuck_Table[MONSTERS_AMOUNT + 64];
char  ThreeHeadedAttack_Table[MONSTERS_AMOUNT + 64];

char  DeathBlow_Table[MONSTERS_AMOUNT + 64];
char  Fear_Table[MONSTERS_AMOUNT + 64];
char  Fearless_Table[MONSTERS_AMOUNT + 64];
char  NoWallPenalty_Table[MONSTERS_AMOUNT + 64];
char  MagicAura_Table[MONSTERS_AMOUNT + 64];
char  StrikeAndReturn_Table[MONSTERS_AMOUNT + 64];
char  Spells_Table[MONSTERS_AMOUNT + 64];
char  Hate_Table[MONSTERS_AMOUNT + 64];
char  JoustingBonus_Table[MONSTERS_AMOUNT + 64];
char  ImmunToJoustingBonus_Table[MONSTERS_AMOUNT + 64];
char  MagicChannel_Table[MONSTERS_AMOUNT + 64];
char  MagicMirror_Table[MONSTERS_AMOUNT + 64];
char  Sharpshooters_Table[MONSTERS_AMOUNT + 64];
char  ShootingAdjacent_Table[MONSTERS_AMOUNT + 64];
char  ReduceTargetDefense_Table[MONSTERS_AMOUNT + 64];
Word  Demonology_Table[MONSTERS_AMOUNT + 64];
char  ImposedSpells_Table[MONSTERS_AMOUNT + 64][6];
char* ImposedSpells_ptr = (char*) ImposedSpells_Table;

char PreventiveCounterstrikeTable [MONSTERS_AMOUNT + 64];
char RangeRetaliation_Table[MONSTERS_AMOUNT + 64];
///// MOP Area End

DWORD PersonalHate[MONSTERS_AMOUNT + 64][MONSTERS_AMOUNT] = {};
char isDragonSlayer_Table[MONSTERS_AMOUNT + 64];
char isFaerieDragon_Table[MONSTERS_AMOUNT + 64];
char isAimedCaster_Table[MONSTERS_AMOUNT + 64];
char isAmmoCart_Table[MONSTERS_AMOUNT + 64];
char isPassive_Table[MONSTERS_AMOUNT + 64];
char isTeleporter[MONSTERS_AMOUNT + 64];

DWORD Necromancy_without_artifacts[16];
DWORD Necromancy_with_artifacts[16];

char hasSantaGuards[MONSTERS_AMOUNT + 64];
//DWORD SantaGuardsType[MONSTERS_AMOUNT + 64];
//DWORD UpgradedSantaGuardsType[MONSTERS_AMOUNT + 64];
DWORD DalionsGuards[MONSTERS_AMOUNT + 64];

float ghost_fraction[MONSTERS_AMOUNT + 64];
char isGhost[MONSTERS_AMOUNT + 64];
char isRogue[MONSTERS_AMOUNT + 64];

char isEnchanter[MONSTERS_AMOUNT + 64];
char isSorceress[MONSTERS_AMOUNT + 64];

char FireWall_Table[MONSTERS_AMOUNT + 64];
char isHellSteed[MONSTERS_AMOUNT + 64];
char isHellSteed2[MONSTERS_AMOUNT + 64];
char isHellSteed3[MONSTERS_AMOUNT + 64];

char NoGolemOverflow[MONSTERS_AMOUNT + 64];
char isDragonResistant[MONSTERS_AMOUNT + 64];

char isHellHydra[MONSTERS_AMOUNT + 64];
char MovesTwice_Table[MONSTERS_AMOUNT + 64];
char Receptive_Table[MONSTERS_AMOUNT + 64];
char isLord_Table[MONSTERS_AMOUNT + 64];
char isHeroic_Table[MONSTERS_AMOUNT + 64];

char* SharedStats[MONSTERS_AMOUNT + 64];

typedef enum 
{ACAST_BIND,
ACAST_BLIND,
ACAST_DISEASE,
ACAST_CURSE,
ACAST_AGE, 
ACAST_STONE,
ACAST_PARALIZE, 
ACAST_POIZON, 
ACAST_ACID, 
ACAST_DEFAULT}
AFTERCAST_ABILITY;

char aftercast_abilities_table[MONSTERS_AMOUNT + 64];

long poison_table[MONSTERS_AMOUNT + 64];
long aging_table[MONSTERS_AMOUNT + 64];
long paralyze_table[MONSTERS_AMOUNT + 64];
long paralyze_chance[MONSTERS_AMOUNT + 64];

//=============================================
typedef enum
{ATT_VAMPIRE,
ATT_THUNDER,
ATT_DEATHSTARE,
ATT_DISPEL,
ATT_DISRUPT,
ATT_DEFAULT}
ATTACK_ABILITY;

char attack_abilities_table[MONSTERS_AMOUNT + 64];
//=============================================
typedef enum
{RESIST_DWARF20,
 RESIST_DWARF40,
 RESIST_123LVL,
 RESIST_1234LVL,
 RESIST_MAGICIMMUNE,
 RESIST_ASAIR,
 RESIST_ASEARTH,
 RESIST_ASFIRE,
 RESIST_DEFAULT,
 RESIST_DWARF60,
 RESIST_DWARF80,
 RESIST_DWARF100,
 RESIST_1LVL,
 RESIST_12LVL,
 RESIST_SPEED,
 RESIST_TOXIC,
 RESIST_WILL,
 RESIST_NO_EYES,
 RESIST_MASS_DAMAGE,
 RESIST_TO_DISPEL,
 RESIST_TO_DEBUFF

}
MAGIC_RESISTANCE;

char magic_resistance_table[MONSTERS_AMOUNT + 64];
//=============================================

typedef enum
{
	VULN_HALF,
	VULN_QUATER,
	VULN_LIGHTING,
	VULN_SHOWER,
	VULN_ICE,
	VULN_FIRE,
	VULN_GOLD,
	VULN_DIAMOND,
	VULN_DEFAULT,
	VULN_GOLEM_125,
	VULN_GOLEM_150,
	VULN_GOLEM_200,
	VULN_GOLEM_300,
	VULN_GOLEM_400,
	VULN_GOLEM_500,
	VULN_GOLEM_600,
	VULN_GOLEM_700,
	VULN_GOLEM_900
} 
MAGIC_VULNERABILITY;

char magic_vulnerability_table[MONSTERS_AMOUNT + 64];
//=============================================
char missiles_table[MONSTERS_AMOUNT + 64];
//=============================================



char spell_1_table[MONSTERS_AMOUNT]; int  spell_1_table_ptr = (int)spell_1_table;
char spell_2_table[MONSTERS_AMOUNT]; int  spell_2_table_ptr = (int)spell_2_table;
char spell_3_table[MONSTERS_AMOUNT]; int  spell_3_table_ptr = (int)spell_3_table;
//=============================================

int skeltrans[MONSTERS_AMOUNT];
//=============================================
// int upgtable_spacer[4] = {-2,-2,-2,-2};
int upgtable[MONSTERS_AMOUNT];

char special_missiles_table[MONSTERS_AMOUNT + 64];
float fire_shield_table[MONSTERS_AMOUNT + 64];
// float respawn_table[MONSTERS_AMOUNT + 64];
float respawn_table_chance[MONSTERS_AMOUNT + 64];
float respawn_table_fraction[MONSTERS_AMOUNT + 64];
float respawn_table_sure[MONSTERS_AMOUNT + 64];
/*char mana_decrease[MONSTERS_AMOUNT];
char mana_increase[MONSTERS_AMOUNT];

char no_wall_table[MONSTERS_AMOUNT];
char no_range_table[MONSTERS_AMOUNT];


char mana_channel_table[MONSTERS_AMOUNT];
char mana_leak_table[MONSTERS_AMOUNT];

char enchanted[MONSTERS_AMOUNT];


char resource_type_table[MONSTERS_AMOUNT];
char resource_amount_table[MONSTERS_AMOUNT];

char block_table[MONSTERS_AMOUNT];

char double_strike[MONSTERS_AMOUNT];*/



char  resource_type_table[MONSTERS_AMOUNT];
DWORD resource_amount_table[MONSTERS_AMOUNT];
DWORD resource_tax_table[MONSTERS_AMOUNT];

char isConstruct_Table[MONSTERS_AMOUNT + 64];
//=============================================


extern "C" __declspec(dllexport) int Creature2Artifact(int target) {
	if (target < 0) return 0;
	return CreatureMimicArtifact[target];
}

extern "C" __declspec(dllexport) int Creature2Artifact_2(int target) {
	if (target < 0) return 0;
	return CreatureMimicArtifact2[target];
}


//===================================

extern MONSTER_PROP new_monsters[MONSTERS_AMOUNT];


extern "C" __declspec(dllexport) int isCreatureConstruct(int creature) {
	return isConstruct_Table[creature] || (new_monsters[creature].flags & 64);
}

char Monster_Ban_Table[MONSTERS_AMOUNT] = {};

int  after_wound__spell[MONSTERS_AMOUNT + 64] = {};
int  after_melee__spell[MONSTERS_AMOUNT + 64] = {};
int  after_shoot__spell[MONSTERS_AMOUNT + 64] = {};
int  after_defend_spell[MONSTERS_AMOUNT + 64] = {};
char after_action_spell_mastery[MONSTERS_AMOUNT + 64] = {};

int  after_wound__spell2[MONSTERS_AMOUNT + 64] = {};
int  after_melee__spell2[MONSTERS_AMOUNT + 64] = {};
int  after_shoot__spell2[MONSTERS_AMOUNT + 64] = {};
int  after_defend_spell2[MONSTERS_AMOUNT + 64] = {};

float shooting_resistance[MONSTERS_AMOUNT + 64] = {};
float melee_resistance[MONSTERS_AMOUNT + 64] = {};

float mana_regen_table[MONSTERS_AMOUNT + 64] = {};
const long Hero_GetIntelligencePower = 0x004E4B20;

int shooting_resistance_hook(Byte* Mon, int Dam) {
	auto StackBlockPartial = (int(*)(Byte*,int))0x0071C710;
	auto the_shooting_resistance = shooting_resistance[*(int*)(Mon + 0x34)]
		+ shooting_resistance[MONSTERS_AMOUNT + *(int*)(Mon + 0xF8) + 21* *(int*)(Mon + 0xF4)];
	auto the_melee_resistance		= melee_resistance		[*(int*)(Mon + 0x34)]
		+ melee_resistance[MONSTERS_AMOUNT + *(int*)(Mon + 0xF8) + 21 * *(int*)(Mon + 0xF4)];
	int last_action = *(int*)((*(int*)0x699420) + 0x3c);

	if (last_action == 7 && the_shooting_resistance != 0) {

		if(the_shooting_resistance>0)
			__asm { 
				mov    ecx, 0x699420
				mov    ecx, [ecx] // bat man
				mov    eax, Mon
				mov    edx, 0x59
				push   0
				push   100
				push   eax // ->monster
					push   edx
				mov    eax, 0x4963C0
				call   eax
			}
		else
			__asm {
				mov    ecx, 0x699420
				mov    ecx, [ecx] // bat man
				mov    eax, Mon
				mov    edx, 0x5a
				push   0
				push   100
				push   eax // ->monster
				push   edx
				mov    eax, 0x4963C0
				call   eax
			}

		double ReturnDamage = (double)Dam *((double)1.0 - (double)the_shooting_resistance);
		return StackBlockPartial(Mon, ReturnDamage);
	} else if (last_action == 6 && the_melee_resistance != 0) {


		if (the_melee_resistance> 0)
			__asm {
				mov    ecx, 0x699420
				mov    ecx, [ecx] // bat man
				mov    eax, Mon
				mov    edx, 0x57
				push   0
				push   100
				push   eax // ->monster
				push   edx
				mov    eax, 0x4963C0
				call   eax
			}
		else 
			__asm {
				mov    ecx, 0x699420
				mov    ecx, [ecx] // bat man
				mov    eax, Mon
				mov    edx, 0x58
				push   0
				push   100
				push   eax // ->monster
				push   edx
				mov    eax, 0x4963C0
				call   eax
			}

		double ReturnDamage = (double)Dam * ((double)1.0 - (double)the_melee_resistance);
		return StackBlockPartial(Mon, ReturnDamage);
	}
	else return StackBlockPartial(Mon, Dam);
}

auto WoG_CastSpell = (void(*)(int, int, int, int, int))0x007157F6;
void WoG_CastSpell_Monster(int Spell, int heroMONSTER, int Pos, int HSkill, int MSkill, int CasterMonPtr) {
	int CombatManager = (*(int*)0x699420);
	
	int tmp_currentMonSide = *(int*)(CombatManager + 0x132B8);
	int tmp_currentMonIndex = *(int*)(CombatManager + 0x132BC);
	int tmp_currentActiveSide = *(int*)(CombatManager + 0x132C0);

	int StackNumber = ((CasterMonPtr - (CombatManager + 0x54cc)) / 0x548);
	// *(int*)(CombatManager + 0x132B8) = *(int*)(CasterMonPtr + 0xF4);
	*(int*)(CombatManager + 0x132B8) = StackNumber / 21;
	*(int*)(CombatManager + 0x132BC) = StackNumber % 21;
	*(int*)(CombatManager + 0x132C0) = StackNumber / 21;

	// *(int*)(CombatManager + 0x132C0) = *(int*)(CasterMonPtr + 0xF4);
	WoG_CastSpell(Spell, heroMONSTER, Pos, HSkill, MSkill);



	*(int*)(CombatManager + 0x132B8) = tmp_currentMonSide;
	*(int*)(CombatManager + 0x132BC) = tmp_currentMonIndex;
	*(int*)(CombatManager + 0x132C0) = tmp_currentActiveSide;
}


int __stdcall NPCDeathStare_hook(HiHook* hook, int a1, signed int a2) {
	if (a1 > 0x10000 && a2 > 0x10000) return
		CALL_2(int, __cdecl, hook->GetDefaultFunc(), a1, a2);
	else return (-1);
}

int __stdcall CheckCreature2Leave_hook(HiHook* hook, int CreatureID) {
	if (isLord_Table[CreatureID]) {
		v_MsgBox( (char*) "This Noble Creature Refuses To Leave You.", 1);
		return 1;
	}
	return CALL_1(int, __cdecl, hook->GetDefaultFunc(), CreatureID);
}

extern "C" __declspec(dllexport) void ChangeCreatureTable(int target, char* buf) {
	if (*buf == 0) return;

	char* c; char pattern[256];
	if (target < 0) target = MONSTERS_AMOUNT - target -1;

	for (int i = 0; i < MONSTERS_AMOUNT; ++i) {
		sprintf(pattern, "Hate%04d=", i);
		c = strstr(buf, pattern);
		if (c != NULL) PersonalHate[target][i] = atoi(c + strlen(pattern));
	}

	c = strstr(buf, "Wog_Spell_Immunites=\"");
	if (c != NULL) {
		c += strlen("Wog_Spell_Immunites=\"");
		if(target < MONSTERS_AMOUNT)
		{
			memset(Spell_Immunities[target], 0, 16);
			
			for (int i = 0; i < 16; ++i) {
				if (c[i] == '\"') break;
				Spell_Immunities[target][i] = c[i];
			}
		}
		else {
			char letter_used[256]; int j = 0; 
			memset(letter_used,0,256);
			for (j = 0; j < 16; ++j) {
				if (Spell_Immunities[target][j] == 0) { break; }
				letter_used[Spell_Immunities[target][j]] = 1;
			}
			for (int k = 0; k < 16 && j < 16; ++k) {
				if (c[k] == '\"') break;
				if (letter_used[c[k]] == 0) {
					Spell_Immunities[target][j] = c[k];
					letter_used[c[k]] = 1;  ++j;
				}
			}

		}
	}

	c = strstr(buf, "isDragonResistant=");
	if (c != NULL) isDragonResistant[target] = atoi(c + strlen("isDragonResistant="));

	c = strstr(buf, "NoGolemOverflow=");
	if (c != NULL) NoGolemOverflow[target] = atoi(c + strlen("NoGolemOverflow="));

	c = strstr(buf, "isHeroic=");
	if (c != NULL) isHeroic_Table[target] = atoi(c + strlen("isHeroic="));

	c = strstr(buf, "isLord=");
	if (c != NULL) 	isLord_Table[target] = atoi(c + strlen("isLord="));

	c = strstr(buf, "isHellHydra=");
	if (c != NULL) isHellHydra[target] = atoi(c + strlen("isHellHydra="));


	c = strstr(buf, "Aging=");
	if (c != NULL) aging_table[target] = atoi(c + strlen("Aging="));

	c = strstr(buf, "Poison=");
	if (c != NULL) poison_table[target] = atoi(c + strlen("Poison="));


	c = strstr(buf, "Paralyze=");
	if (c != NULL) paralyze_table[target] = atoi(c + strlen("Paralyze="));

	c = strstr(buf, "ParalyzeChance=");
	if (c != NULL) paralyze_chance[target] = atoi(c + strlen("ParalyzeChance="));


	c = strstr(buf, "DoNotGenerate=");
	if (c != NULL) Monster_Ban_Table[target] = atoi(c + strlen("DoNotGenerate="));


	c = strstr(buf, "isTeleporter=");
	if (c != NULL) isTeleporter[target] = atoi(c + strlen("isTeleporter="));

	c = strstr(buf, "GhostMultiplier=");
	if (c != NULL) GhostMultiplyer_028460f8[target] = atoi(c + strlen("GhostMultiplier="));


	c = strstr(buf, "MeleeResistance=");
	if (c != NULL) melee_resistance[target] = atof(c + strlen("MeleeResistance="));
	c = strstr(buf, "MeleeResistance+=");
	if (c != NULL) melee_resistance[target] += atof(c + strlen("MeleeResistance+="));

	c = strstr(buf, "ShootingResistance=");
	if (c != NULL) shooting_resistance[target] = atof(c + strlen("ShootingResistance="));
	c = strstr(buf, "ShootingResistance+=");
	if (c != NULL) shooting_resistance[target] += atof(c + strlen("ShootingResistance+="));

	c = strstr(buf, "AfterMeleeSpell=");
	if (c != NULL) after_melee__spell[target] = atoi(c + strlen("AfterMeleeSpell="));

	c = strstr(buf, "AfterShootSpell=");
	if (c != NULL) after_shoot__spell[target] = atoi(c + strlen("AfterShootSpell="));

	c = strstr(buf, "AfterDefendSpell=");
	if (c != NULL) after_defend_spell[target] = atoi(c + strlen("AfterDefendSpell="));

	c = strstr(buf, "AfterWoundSpell=");
	if (c != NULL) after_wound__spell[target] = atoi(c + strlen("AfterWoundSpell="));

	c = strstr(buf, "AfterActionSpellMastery=");
	if (c != NULL) after_action_spell_mastery[target] = atoi(c + strlen("AfterActionSpellMastery="));

	c = strstr(buf, "AfterMeleeSpell2=");
	if (c != NULL) after_melee__spell2[target] = atoi(c + strlen("AfterMeleeSpell2="));

	c = strstr(buf, "AfterShootSpell2=");
	if (c != NULL) after_shoot__spell2[target] = atoi(c + strlen("AfterShootSpell2="));

	c = strstr(buf, "AfterDefendSpell2=");
	if (c != NULL) after_defend_spell2[target] = atoi(c + strlen("AfterDefendSpell2="));

	c = strstr(buf, "AfterWoundSpell2=");
	if (c != NULL) after_wound__spell2[target] = atoi(c + strlen("AfterWoundSpell2="));


	c = strstr(buf, "RetaliateRanged=");
	if (c != NULL) RangeRetaliation_Table[target] = atoi(c + strlen("RetaliateRanged="));

	c = strstr(buf, "PreventiveStrike=");
	if (c != NULL) PreventiveCounterstrikeTable[target] = atoi(c + strlen("PreventiveStrike="));

	c = strstr(buf, "ReduceTargetDefense=");
	if (c != NULL) ReduceTargetDefense_Table[target] = atoi(c + strlen("ReduceTargetDefense="));

	c = strstr(buf, "isDragonSlayer=");
	if (c != NULL) isDragonSlayer_Table[target] = atoi(c + strlen("isDragonSlayer="));

	c = strstr(buf, "ResGen_Tax=");
	if (c != NULL) resource_tax_table[target] = atoi(c + strlen("ResGen_Tax="));


	c = strstr(buf, "ResGen_Amount=");
	if (c != NULL) resource_amount_table[target] = atoi(c + strlen("ResGen_Amount="));

	c = strstr(buf, "ResGen_Type=");
	if (c != NULL) resource_type_table[target] = atoi(c + strlen("ResGen_Type="));


	c = strstr(buf, "UpgradesTo=");
	if (c != NULL) upgtable[target] = atoi(c + strlen("UpgradesTo="));


	c = strstr(buf, "isCerberus=");
	if (c != NULL) ThreeHeadedAttack_Table[target] = atoi(c + strlen("isCerberus="));

	c = strstr(buf, "Demonology=");
	if (c != NULL) Demonology_Table[target] = atoi(c + strlen("Demonology="));

	c = strstr(buf, "CreatureSpellPowerMultiplier=");
	if (c != NULL) CreatureSpellPowerMultiplier[target] = atoi(c + strlen("CreatureSpellPowerMultiplier="));

	c = strstr(buf, "CreatureSpellPowerDivider=");
	if (c != NULL) CreatureSpellPowerDivider[target] = atoi(c + strlen("CreatureSpellPowerDivider="));

	c = strstr(buf, "CreatureSpellPowerAdder=");
	if (c != NULL) CreatureSpellPowerAdder[target] = atoi(c + strlen("CreatureSpellPowerAdder="));

	c = strstr(buf, "CreatureSpellPowerScaled=");
	if (c != NULL) CreatureSpellPowerScaled[target] = atoi(c + strlen("CreatureSpellPowerScaled="));

	c = strstr(buf, "CreatureMimicArtifact=");
	if (c != NULL) CreatureMimicArtifact[target] = atoi(c + strlen("CreatureMimicArtifact="));

	c = strstr(buf, "CreatureMimicArtifact2=");
	if (c != NULL) CreatureMimicArtifact2[target] = atoi(c + strlen("CreatureMimicArtifact2="));

	c = strstr(buf, "isFaerie=");
	if (c != NULL) isFaerieDragon_Table[target] = atoi(c + strlen("isFaerie="));

	c = strstr(buf, "isAimedCaster=");
	if (c != NULL) isAimedCaster_Table[target] = atoi(c + strlen("isAimedCaster="));

	c = strstr(buf, "isPassive=");
	if (c != NULL) isPassive_Table[target] = atoi(c + strlen("isPassive="));
	c = strstr(buf, "isAmmoCart=");
	if (c != NULL) isAmmoCart_Table[target] = atoi(c + strlen("isAmmoCart="));

	c = strstr(buf, "ImposedSpell1Number=");
	if (c != NULL) ImposedSpells_Table[target][0] = atoi(c + strlen("ImposedSpell1Number="));
	c = strstr(buf, "ImposedSpell1Level=");
	if (c != NULL) ImposedSpells_Table[target][3] = atoi(c + strlen("ImposedSpell1Level="));

	c = strstr(buf, "ImposedSpell2Number=");
	if (c != NULL) ImposedSpells_Table[target][1] = atoi(c + strlen("ImposedSpell2Number="));
	c = strstr(buf, "ImposedSpell2Level=");
	if (c != NULL) ImposedSpells_Table[target][4] = atoi(c + strlen("ImposedSpell2Level="));

	c = strstr(buf, "ImposedSpell3Number=");
	if (c != NULL) ImposedSpells_Table[target][2] = atoi(c + strlen("ImposedSpell3Number="));
	c = strstr(buf, "ImposedSpell3Level=");
	if (c != NULL) ImposedSpells_Table[target][5] = atoi(c + strlen("ImposedSpell3Level="));


	c = strstr(buf, "DeathBlow=");
	if (c != NULL) DeathBlow_Table[target] = atoi(c + strlen("DeathBlow="));
	c = strstr(buf, "DeathBlow+=");
	if (c != NULL) DeathBlow_Table[target] += atoi(c + strlen("DeathBlow+="));

	c = strstr(buf, "Fear=");
	if (c != NULL) Fear_Table[target] = atoi(c + strlen("Fear="));
	c = strstr(buf, "Fearless=");
	if (c != NULL) Fearless_Table[target] = atoi(c + strlen("Fearless="));

	c = strstr(buf, "NoWallPenalty=");
	if (c != NULL) NoWallPenalty_Table[target] = atoi(c + strlen("NoWallPenalty="));
	c = strstr(buf, "Sharpshooters=");
	if (c != NULL) Sharpshooters_Table[target] = atoi(c + strlen("Sharpshooters="));
	c = strstr(buf, "ShootingAdjacent=");
	if (c != NULL) ShootingAdjacent_Table[target] = atoi(c + strlen("ShootingAdjacent="));


	c = strstr(buf, "MagicAura=");
	if (c != NULL) MagicAura_Table[target] = atoi(c + strlen("MagicAura="));
	c = strstr(buf, "MagicMirror=");
	if (c != NULL) MagicMirror_Table[target] = atoi(c + strlen("MagicMirror="));
	c = strstr(buf, "MagicChannel=");
	if (c != NULL) MagicChannel_Table[target] = atoi(c + strlen("MagicChannel="));


	c = strstr(buf, "StrikeAndReturn=");
	if (c != NULL) StrikeAndReturn_Table[target] = atoi(c + strlen("StrikeAndReturn="));
	c = strstr(buf, "JoustingBonus=");
	if (c != NULL) JoustingBonus_Table[target] = atoi(c + strlen("JoustingBonus="));
	c = strstr(buf, "JoustingImmune=");
	if (c != NULL) ImmunToJoustingBonus_Table[target] = atoi(c + strlen("JoustingImmune="));

	c = strstr(buf, "RegenerationChance=");
	if (c != NULL) RegenerationChance_Table[target] = atoi(c + strlen("RegenerationChance="));

	c = strstr(buf, "RegenerationHitPoints=");
	if (c != NULL) RegenerationHitPoints_Table[target] = atoi(c + strlen("RegenerationHitPoints="));


	c = strstr(buf, "Retaliations=");
	if (c != NULL) Counterstrike_Table[target] = atoi(c + strlen("Retaliations="));
	c = strstr(buf, "Retaliations=+");
	if (c != NULL) Counterstrike_Table[target] += atoi(c + strlen("Retaliations+="));



	c = strstr(buf, "CastsSpell=");
	if (c != NULL)Spells_Table[target] = atoi(c + strlen("CastsSpell="));


	c = strstr(buf, "ManaDrain=");
	if (c != NULL) ManaDrain_Table[target] = atoi(c + strlen("ManaDrain="));



	c = strstr(buf, "SpellsCostLess=");
	if (c != NULL) SpellsCostLess_Table[target] = atoi(c + strlen("SpellsCostLess="));

	c = strstr(buf, "SpellsCostDump=");
	if (c != NULL) SpellsCostDump_Table[target] = atoi(c + strlen("SpellsCostDump="));


	c = strstr(buf, "AlwaysPositiveMorale=");
	if (c != NULL) AlwaysPositiveMorale_Table[target] = atoi(c + strlen("AlwaysPositiveMorale="));

	c = strstr(buf, "AlwaysPositiveLuck=");
	if (c != NULL) AlwaysPositiveLuck_Table[target] = atoi(c + strlen("AlwaysPositiveLuck="));



	c = strstr(buf, "hasSantaGuards=");
	if (c != NULL) hasSantaGuards[target] = atoi(c + strlen("hasSantaGuards="));
	c = strstr(buf, "DalionsGuards=");
	if (c != NULL) DalionsGuards[target] = atoi(c + strlen("DalionsGuards="));
	else DalionsGuards[target] = -1;

	c = strstr(buf, "isRogue=");
	if (c != NULL) isRogue[target] = atoi(c + strlen("isRogue="));

	c = strstr(buf, "isGhost=");
	if (c != NULL) isGhost[target] = atoi(c + strlen("isGhost="));
	c = strstr(buf, "GhostFraction=");
	if (c != NULL) ghost_fraction[target] = atof(c + strlen("GhostFraction="));


	c = strstr(buf, "isEnchanter=");
	if (c != NULL) isEnchanter[target] = atoi(c + strlen("isEnchanter="));

	c = strstr(buf, "isSorceress=");
	if (c != NULL) isSorceress[target] = atoi(c + strlen("isSorceress="));

	/*
	c = strstr(buf, "isHellSteed=");
	if (c != NULL) isHellSteed[target] = atoi(c + strlen("isHellSteed="));
	c = strstr(buf, "isHellSteed2=");
	if (c != NULL) isHellSteed2[target] = atoi(c + strlen("isHellSteed2="));
	c = strstr(buf, "isHellSteed3=");
	if (c != NULL) isHellSteed3[target] = atoi(c + strlen("isHellSteed3="));
	*/

	c = strstr(buf, "hasFirewall=");
	if (c != NULL) FireWall_Table[target] = atoi(c + strlen("hasFirewall="));

	//end of majaczek
	c = strstr(buf, "Level=");
	if (c != NULL)
		new_monsters[target].level = atoi(c + strlen("Level="));

	c = strstr(buf, "Flags=");
	if (c != NULL)
		new_monsters[target].flags = atoi(c + strlen("Flags="));

	c = strstr(buf, "Flags|=");
	if (c != NULL)
		new_monsters[target].flags |= atoi(c + strlen("Flags|="));

	c = strstr(buf, "Flags&=");
	if (c != NULL)
		new_monsters[target].flags &= atoi(c + strlen("Flags&="));


	c = strstr(buf, "Town=");
	if (c != NULL)
		new_monsters[target].town = atoi(c + strlen("Town="));
	//			
	///*
	//Singular	Plural	Wood	Mercury	Ore	Sulfur	Crystal	Gems	Gold	Fight Value	AI Value	Growth	Horde Growth	
	//Hit Points	Speed	Attack	Defense	Low	High	
	//Shots	Spells	Low	High	Ability Text	Attributes */
	//
				c = strstr(buf,"Adv.high=" );
				if (c!=NULL) new_monsters[target].adv_high=
							atoi(c+strlen("Adv.high=" ));
	//			else new_monsters[target].adv_high = 0;
	//
				c = strstr(buf,"Adv.low=" );
				if (c!=NULL) new_monsters[target].adv_low=
							atoi(c+strlen("Adv.low=" ));
	//			else new_monsters[target].i_AdvLow = 0;
	//
				c = strstr(buf,"AI_value=" );
				if (c!=NULL) new_monsters[target].ai_value=
							atoi(c+strlen("AI_value=" ));
	//
				c = strstr(buf,"Attack=" );
				if (c!=NULL) new_monsters[target].attack =
							atoi(c+strlen("Attack=" ));
	//
				c = strstr(buf,"CostCrystal=" );
				if (c!=NULL) new_monsters[target].cost_crystal=
							atoi(c+strlen("CostCrystal=" ));
	//
				c = strstr(buf,"CostGems=" );
				if (c!=NULL) new_monsters[target].cost_gems =
							atoi(c+strlen("CostGems=" ));
	//
				c = strstr(buf,"CostGold=" );
				if (c!=NULL) new_monsters[target].cost_gold=
							atoi(c+strlen("CostGold=" ));
	//
				c = strstr(buf,"CostMercury=" );
				if (c!=NULL) new_monsters[target].cost_mercury=
							atoi(c+strlen("CostMercury=" ));
	//
				c = strstr(buf,"CostOre=" );
				if (c!=NULL) new_monsters[target].cost_ore=
							atoi(c+strlen("CostOre=" ));
	//
				c = strstr(buf,"CostSulfur=" );
				if (c!=NULL) new_monsters[target].cost_sulfur=
							atoi(c+strlen("CostSulfur=" ));
	//
				c = strstr(buf,"CostWood=" );
				if (c!=NULL) new_monsters[target].cost_wood=
							atoi(c+strlen("CostWood=" ));
	//
				c = strstr(buf,"DamageHigh=" );
				if (c!=NULL) new_monsters[target].damage_high =
							atoi(c+strlen("DamageHigh=" ));
	//
				c = strstr(buf,"DamageLow=" );
				if (c!=NULL) new_monsters[target].damage_low=
							atoi(c+strlen("DamageLow=" ));
	//
				c = strstr(buf,"Defence=" );
				if (c!=NULL) new_monsters[target].defence=
							atoi(c+strlen("Defence=" ));
	//
				c = strstr(buf,"FightValue=" );
				if (c!=NULL) new_monsters[target].fight_value=
							atoi(c+strlen("FightValue=" ));
	//
				c = strstr(buf,"Growth=" );
				if (c!=NULL) new_monsters[target].growth=
							atoi(c+strlen("Growth=" ));
	//
				c = strstr(buf,"HP=" );
				if (c!=NULL) new_monsters[target].hp=
							atoi(c+strlen("HP=" ));
	//
				c = strstr(buf,"Horde=" );
				if (c!=NULL) new_monsters[target].horde_growth=
							atoi(c+strlen("Horde=" ));
	//
				c = strstr(buf,"Shots=" );
				if (c!=NULL) new_monsters[target].shots=
							atoi(c+strlen("Shots=" ));
	//
				c = strstr(buf,"Speed=" );
				if (c!=NULL) new_monsters[target].speed=
							atoi(c+strlen("Speed=" ));
	//
				c = strstr(buf,"Spells=" );
				if (c!=NULL) new_monsters[target].spells=
							atoi(c+strlen("Spells=" ));
	//
	///*Time between fidgets	Walk Animation Time	Attack Animation Time	
	//Flight Animation Distance	Upper-right Missile Offset		
	//Right Missile Offset		Lower-right Missile Offset		Missile Frame Angles												
	//Troop Count Location Offset	Attack Climax Frame*/
	//
	///*
	//			c = strstr(buf,"TBF=" );
	//			if (c!=NULL) new_monsters[target].i_Spells=
	//						atoi(c+strlen("TBF=" ));
	//*/


	c = strstr(buf, "Spell effect=");
	if (c != NULL) aftercast_abilities_table[target] =
		atoi(c + strlen("Spell effect="));

	c = strstr(buf, "Attack effect=");
	if (c != NULL) attack_abilities_table[target] =
		atoi(c + strlen("Attack effect="));

	c = strstr(buf, "Resistance effect=");
	if (c != NULL) magic_resistance_table[target] =
		atoi(c + strlen("Resistance effect="));

	c = strstr(buf, "Vulnerability effect="); // majaczek
	if (c != NULL) magic_vulnerability_table[target] =
		atoi(c + strlen("Vulnerability effect="));




	c = strstr(buf, "Spell 1=");
	if (c != NULL) spell_1_table[target] =
		atoi(c + strlen("Spell 1="));


	c = strstr(buf, "Spell 2=");
	if (c != NULL) spell_2_table[target] =
		atoi(c + strlen("Spell 2="));


	c = strstr(buf, "Spell 3=");
	if (c != NULL) spell_3_table[target] =
		atoi(c + strlen("Spell 3="));

	c = strstr(buf, "Sktransformer=");
	if (c != NULL) skeltrans[target] =
		atoi(c + strlen("Sktransformer="));

	c = strstr(buf, "Shot type=");
	if (c != NULL) special_missiles_table[target] =
		atoi(c + strlen("Shot type="));

	c = strstr(buf, "Fire shield=");
	if (c != NULL) fire_shield_table[target] =
		(float)atof(c + strlen("Fire shield="));
	c = strstr(buf, "Fire shield+=");
	if (c != NULL) fire_shield_table[target] +=
		(float)atof(c + strlen("Fire shield+="));

	// deprecated but left for backwards compatibility
	c = strstr(buf, "Self-resurrection=");
	if (c != NULL) respawn_table_fraction[target] =
		respawn_table_chance[target] = respawn_table_sure[target] =
		(float)atof(c + strlen("Self-resurrection="));

	c = strstr(buf, "rebirth chance=");
	if (c != NULL) 	respawn_table_chance[target] =
		(float)atof(c + strlen("rebirth chance="));

	c = strstr(buf, "rebirth fraction=");
	if (c != NULL) 	respawn_table_fraction[target] =
		(float)atof(c + strlen("rebirth fraction="));

	c = strstr(buf, "rebirth sure=");
	if (c != NULL) 	respawn_table_sure[target] =
		(float)atof(c + strlen("rebirth sure="));

	c = strstr(buf, "isConstruct=");
	if (c != NULL) isConstruct_Table[target] =
		atoi(c + strlen("isConstruct="));

	c = strstr(buf, "MovesTwice=");
	if (c != NULL) MovesTwice_Table[target] =
		atoi(c + strlen("MovesTwice="));
	if (target >= MONSTERS_AMOUNT) {
		c = strstr(buf, "AdditionalMove+=");
		if (c != NULL) addditional_moves[target - MONSTERS_AMOUNT]
			+= atoi(c + strlen("AdditionalMove+="));

		c = strstr(buf, "AdditionalShot+=");
		if (c != NULL) addditional_shots[target - MONSTERS_AMOUNT]
			+= atoi(c + strlen("AdditionalShot+="));

		c = strstr(buf, "AdditionalMelee+=");
		if (c != NULL) addditional_melees[target - MONSTERS_AMOUNT]
			+= atoi(c + strlen("AdditionalMelee+="));
	}

	c = strstr(buf, "ManaRegen=");
	if (c != NULL) mana_regen_table[target] =
		atoi(c + strlen("ManaRegen="));

	c = strstr(buf, "Receptive=");
	if (c != NULL) Receptive_Table[target] =
		atoi(c + strlen("Receptive="));


}

void ApplySharedStats(int mon_type, int j) {
	if(SharedStats[mon_type])
		ChangeCreatureTable(-1-j, SharedStats[mon_type]);
}

void LoadCreatureConfig(int target)
{
		char *buf, *c, fname[256];
        FILE *fdesc;
		
		/*
		sprintf(fname,"Mods\\Amethyst Upgrades\\Data\\creatures\\%u.cfg",target);
        if(!FileExists(fname)) */
	    /*    sprintf(fname,"Mods\\Knightmare Kingdoms\\Data\\creatures\\%u.cfg",target);
		if(!FileExists(fname))*/ 
			sprintf(fname,"Data\\creatures\\%u.cfg",target);
        if(fdesc=fopen(fname,"r"))
        {
            //----------
            fseek (fdesc , 0 , SEEK_END);
            int fdesc_size=ftell(fdesc);
            rewind(fdesc);
            //----------
            buf=(char*)malloc(fdesc_size+1);
            fread(buf,1,fdesc_size,fdesc);
            buf[fdesc_size]=0;
            fclose(fdesc);
            //begin of majaczek

			ChangeCreatureTable(target, buf);


            free(buf);
        }
}

char only_infernals_can_be_gated[512] = "You can only Gate units from Inferno faction.";
char only_Machines_can_be_repaired[512] = "Only War Machines and Constructs can be repaied that way.";
char only_Undead_can_be_raised[512] = "Only Undead can be raised that way.";
long BattleHex_GetMonAtBFPosition = 0x004E7230;
long BattleMgr_CreatureMagicResist = 0x005A83A0;
long BattleStack_GetSecondOccupedGexNum = 0x004463C0;
long CanCastSpellAtCoord = 0x005A3CD0;
long BattleMgr_Play_MagicAnimation_onGEX = 0x00496590;
long BattleMgr_CastSpell = 0x005A0140;
long BattleMgr_ResurectArchangel = 0x005A7870;


__declspec(naked) void get_can_gate(void) {
	__asm {
		push ebp
		mov ebp, esp
		push ecx
		mov eax, dword ptr ss : [ebp + 0xC]
		push ebx
		mov ebx, ecx
		push esi
		test eax, eax
		push edi

		//check if proper hex
		mov dword ptr ss : [ebp - 0x4] , ebx
		jl h3era_005A413A
		cmp eax, 0xBB
		jge h3era_005A413A

		lea ecx, dword ptr ds : [eax * 8]
		sub ecx, eax
		shl ecx, 0x4
		lea esi, dword ptr ds : [ecx + ebx]
		mov dword ptr ss : [ebp + 0xC] , esi
		add esi, 0x1C4
		mov al, byte ptr ds : [esi + 0x18]
		test al, al
		jl h3era_005A4078 
		mov ecx, esi

		//call <h3era hd.BattleHex_GetMonAtBFPosition>
		call  BattleHex_GetMonAtBFPosition

		mov esi, eax
		mov eax, dword ptr ss : [ebp + 0x8]
		cmp dword ptr ds : [esi + 0xF4] , eax
		jne h3era_005A413A


		// check inferno faction
		mov eax, dword ptr ds : [esi + 0x34]
		cdq
		imul eax, MON_Sizeof
		cmp dword ptr ds : [eax + new_monsters] , 3
		jne h3era_005A413A


		// return esi
		mov eax, esi
		pop edi
		pop esi
		pop ebx
		mov esp, ebp
		pop ebp
		ret 0xC

		// something with doublewide
		h3era_005A4078:
		test byte ptr ds : [esi + 0x10] , 0x2
		jne h3era_005A413A
		mov edi, dword ptr ds : [esi + 0x1C]
		dec edi
		js h3era_005A413A


			h3era_005A408C :
		movsx edx, byte ptr ds : [esi + edi + 0x20]
			movsx eax, byte ptr ds : [esi + edi + 0x2E]
			lea ecx, dword ptr ds : [edx * 8]
			sub ecx, edx
			lea ecx, dword ptr ds : [ecx + ecx * 2]
			add eax, ecx
			lea ecx, dword ptr ds : [eax * 8]
			sub ecx, eax
			lea ecx, dword ptr ds : [ecx + ecx * 2]
			lea eax, dword ptr ds : [eax + ecx * 8]
			mov ecx, dword ptr ss : [ebp + 0x8]
			cmp edx, ecx
			lea ebx, dword ptr ds : [ebx + eax * 8 + 0x54CC]
			jne check_inferno // je h3era_005A413A // jne h3era_005A4124


			/*
			// check inferno faction again
			mov eax, dword ptr ds : [esi + 0x34]
			cdq
			imul eax, MON_Sizeof
			cmp byte ptr ds : [eax + new_monsters] , 3
			je h3era_005A4124
			*/


			mov al, byte ptr ds : [edi + esi + 0x3C]
			mov edx, dword ptr ss : [ebp + 0xC]
			test al, al
			jne h3era_005A40EC
			cmp byte ptr ds : [edx + 0x24C] , 0x0
			jge h3era_005A4124
			test byte ptr ds : [edx + 0x244] , 0x2
			jne h3era_005A4124

			h3era_005A40EC :


		cmp al, 0x1
			jne h3era_005A4103
			mov al, byte ptr ds : [edx + 0x16C]
			test al, al
			jge h3era_005A4124
			test byte ptr ds : [edx + 0x164] , 0x2
			jne h3era_005A4124

			h3era_005A4103 :
		mov eax, dword ptr ss : [ebp + 0x10]
			jmp h3era_005A412F

			h3era_005A4124 :
		dec edi
			js h3era_005A413A
			mov ebx, dword ptr ss : [ebp - 0x4]
			jmp h3era_005A408C

			h3era_005A412F :

		// return ebx
		pop edi
		mov eax, ebx
		pop esi
		pop ebx
		mov esp, ebp
		pop ebp
		ret 0xC


		h3era_005A413A: // return zero
			pop edi
			pop esi
			xor eax, eax
			pop ebx
			mov esp, ebp
			pop ebp
			ret 0xC

		check_inferno:
			mov eax, dword ptr ds : [ebx + 0x34]
			cdq
			imul eax, MON_Sizeof
			cmp byte ptr ds : [eax + new_monsters] , 3
			je  h3era_005A412F
			jmp h3era_005A413A
	}
}

__declspec(naked) void get_raise_undead(void) {
	__asm {
		push ebp
		mov ebp, esp
		push ecx
		mov eax, dword ptr ss : [ebp + 0xC]
		push ebx
		mov ebx, ecx
		push esi
		test eax, eax
		push edi
		mov dword ptr ss : [ebp - 0x4] , ebx
		jl h3era_005A413A
		cmp eax, 0xBB
		jge h3era_005A413A
		lea ecx, dword ptr ds : [eax * 8]
		sub ecx, eax
		shl ecx, 0x4
		lea esi, dword ptr ds : [ecx + ebx]
		mov dword ptr ss : [ebp + 0xC] , esi
		add esi, 0x1C4
		mov al, byte ptr ds : [esi + 0x18]
		test al, al
		jl h3era_005A4078
		mov ecx, esi

		//call <h3era hd.BattleHex_GetMonAtBFPosition>
		call  BattleHex_GetMonAtBFPosition

		mov esi, eax
		mov eax, dword ptr ss : [ebp + 0x8]
		cmp dword ptr ds : [esi + 0xF4] , eax
		jne h3era_005A413A
		mov edx, dword ptr ds : [esi + 0x84]
		shr edx, 0x12 // undead, not living
		test dl, 0x1
		je h3era_005A413A
		mov ecx, dword ptr ds : [esi + 0x4C]
		mov edx, dword ptr ds : [esi + 0x60]
		cmp ecx, edx
		jge h3era_005A413A
		mov edx, dword ptr ss : [ebp + 0x10]
		mov ecx, ebx
		push edx
		push 0x1
		push 0x0
		push esi
		push eax
		push 0x27 // animate dead

		//call <h3era hd.BattleMgr_CreatureMagicResist>
		call BattleMgr_CreatureMagicResist

		//fcomp st(0), qword ptr ds : [0x63AC38]
		fcomp qword ptr ds : [0x63AC38]

		fnstsw ax
		test ah, 0x41
		jne h3era_005A413A
		mov eax, esi
		pop edi
		pop esi
		pop ebx
		mov esp, ebp
		pop ebp
		ret 0xC

		h3era_005A4078:
		test byte ptr ds : [esi + 0x10] , 0x2
			jne h3era_005A413A
			mov edi, dword ptr ds : [esi + 0x1C]
			dec edi
			js h3era_005A413A

			h3era_005A408C :
		movsx edx, byte ptr ds : [esi + edi + 0x20]
			movsx eax, byte ptr ds : [esi + edi + 0x2E]
			lea ecx, dword ptr ds : [edx * 8]
			sub ecx, edx
			lea ecx, dword ptr ds : [ecx + ecx * 2]
			add eax, ecx
			lea ecx, dword ptr ds : [eax * 8]
			sub ecx, eax
			lea ecx, dword ptr ds : [ecx + ecx * 2]
			lea eax, dword ptr ds : [eax + ecx * 8]
			mov ecx, dword ptr ss : [ebp + 0x8]
			cmp edx, ecx
			lea ebx, dword ptr ds : [ebx + eax * 8 + 0x54CC]
			jne h3era_005A4124
			mov edx, dword ptr ds : [ebx + 0x84]
			shr edx, 0x12 // instead of 0x4
			test dl, 0x1
			je h3era_005A4124
			mov al, byte ptr ds : [edi + esi + 0x3C]
			mov edx, dword ptr ss : [ebp + 0xC]
			test al, al
			jne h3era_005A40EC
			cmp byte ptr ds : [edx + 0x24C] , 0x0
			jge h3era_005A4124
			test byte ptr ds : [edx + 0x244] , 0x2
			jne h3era_005A4124

			h3era_005A40EC :
		cmp al, 0x1
			jne h3era_005A4103
			mov al, byte ptr ds : [edx + 0x16C]
			test al, al
			jge h3era_005A4124
			test byte ptr ds : [edx + 0x164] , 0x2
			jne h3era_005A4124

			h3era_005A4103 :
		mov eax, dword ptr ss : [ebp + 0x10]
			push eax
			push 0x1
			push 0x0
			push ebx
			push ecx
			mov ecx, dword ptr ss : [ebp - 0x4]
			push 0x27 //animate dead

			//call <h3era hd.BattleMgr_CreatureMagicResist>
			call  BattleMgr_CreatureMagicResist

			//fcomp st(0), qword ptr ds : [0x63AC38]
			fcomp qword ptr ds : [0x63AC38]

			fnstsw ax
			test ah, 0x41
			je h3era_005A412F

			h3era_005A4124 :
		dec edi
			js h3era_005A413A
			mov ebx, dword ptr ss : [ebp - 0x4]
			jmp h3era_005A408C

			h3era_005A412F :
		pop edi
			mov eax, ebx
			pop esi
			pop ebx
			mov esp, ebp
			pop ebp
			ret 0xC

			h3era_005A413A:
		pop edi
			pop esi
			xor eax, eax
			pop ebx
			mov esp, ebp
			pop ebp
			ret 0xC
	}
}


__declspec(naked) void get_repair_constructs(void) {
	__asm {
		push ebp
		mov ebp, esp
		push ecx
		mov eax, dword ptr ss : [ebp + 0xC]
		push ebx
		mov ebx, ecx
		push esi
		test eax, eax
		push edi
		mov dword ptr ss : [ebp - 0x4] , ebx
		jl h3era_005A413A
		cmp eax, 0xBB
		jge h3era_005A413A
		lea ecx, dword ptr ds : [eax * 8]
		sub ecx, eax
		shl ecx, 0x4
		lea esi, dword ptr ds : [ecx + ebx]
		mov dword ptr ss : [ebp + 0xC] , esi
		add esi, 0x1C4
		mov al, byte ptr ds : [esi + 0x18]
		test al, al
		jl h3era_005A4078
		mov ecx, esi

		//call <h3era hd.BattleHex_GetMonAtBFPosition>
		call  BattleHex_GetMonAtBFPosition

		mov esi, eax
		mov eax, dword ptr ss : [ebp + 0x8]
		cmp dword ptr ds : [esi + 0xF4] , eax
		jne h3era_005A413A

		mov edx, dword ptr ds : [esi + 0x34]
		
		cmp byte ptr ds : [edx+isConstruct_Table], 0
		//lea eax, isConstruct_Table
		//mov dl, [edx + eax]
		//cmp dl, 0
		
		jne possible1

		mov edx, dword ptr ds : [esi + 0x84]
		shr edx, 0x6 // machines, not living
		test dl, 0x1
		je h3era_005A413A

		possible1:
		mov ecx, dword ptr ds : [esi + 0x4C]
		mov edx, dword ptr ds : [esi + 0x60]
		cmp ecx, edx
		jge h3era_005A413A
		mov edx, dword ptr ss : [ebp + 0x10]
		mov ecx, ebx

	
		push edx
		push 0x1
		push 0x0
		push esi
		push eax
		push 0x25 // cure

		//call <h3era hd.BattleMgr_CreatureMagicResist>
		call BattleMgr_CreatureMagicResist

		//fcomp st(0), qword ptr ds : [0x63AC38]
		fcomp qword ptr ds : [0x63AC38]

		fnstsw ax
		test ah, 0x41
		//jne h3era_005A413A
		

		mov eax, esi
		pop edi
		pop esi
		pop ebx
		mov esp, ebp
		pop ebp
		ret 0xC

		h3era_005A4078:
		test byte ptr ds : [esi + 0x10] , 0x2
			jne h3era_005A413A
			mov edi, dword ptr ds : [esi + 0x1C]
			dec edi
			js h3era_005A413A

			h3era_005A408C :
		movsx edx, byte ptr ds : [esi + edi + 0x20]
			movsx eax, byte ptr ds : [esi + edi + 0x2E]
			lea ecx, dword ptr ds : [edx * 8]
			sub ecx, edx
			lea ecx, dword ptr ds : [ecx + ecx * 2]
			add eax, ecx
			lea ecx, dword ptr ds : [eax * 8]
			sub ecx, eax
			lea ecx, dword ptr ds : [ecx + ecx * 2]
			lea eax, dword ptr ds : [eax + ecx * 8]
			mov ecx, dword ptr ss : [ebp + 0x8]
			cmp edx, ecx
			lea ebx, dword ptr ds : [ebx + eax * 8 + 0x54CC]
			jne check_constructs// jne h3era_005A4124


			mov edx, dword ptr ds : [ebx + 0x34]
			
			cmp byte ptr ds : [edx + isConstruct_Table] , 0
			//lea eax, isConstruct_Table
			//mov dl, [edx + eax]
			//cmp dl, 0

			jne possible2


			mov edx, dword ptr ds : [ebx + 0x84]
			shr edx, 0x6 // instead of 0x4
			test dl, 0x1
			je h3era_005A4124
			
			possible2:
			mov al, byte ptr ds : [edi + esi + 0x3C]
			mov edx, dword ptr ss : [ebp + 0xC]
			test al, al
			jne h3era_005A40EC
			cmp byte ptr ds : [edx + 0x24C] , 0x0
			jge h3era_005A4124
			test byte ptr ds : [edx + 0x244] , 0x2
			jne h3era_005A4124

			h3era_005A40EC :
		cmp al, 0x1
			jne h3era_005A4103
			mov al, byte ptr ds : [edx + 0x16C]
			test al, al
			jge h3era_005A4124
			test byte ptr ds : [edx + 0x164] , 0x2
			jne h3era_005A4124

			h3era_005A4103 :
			mov eax, dword ptr ss : [ebp + 0x10]
				
			push eax
			push 0x1
			push 0x0
			push ebx
			push ecx
			mov ecx, dword ptr ss : [ebp - 0x4]
			push 0x25 //cure

			//call <h3era hd.BattleMgr_CreatureMagicResist>
			call  BattleMgr_CreatureMagicResist

			//fcomp st(0), qword ptr ds : [0x63AC38]
			fcomp qword ptr ds : [0x63AC38]

			fnstsw ax
			test ah, 0x41
			// je h3era_005A412F
				

			//inserted
				//mov ecx, dword ptr ss : [ebp - 0x4]
				jmp h3era_005A412F


			h3era_005A4124 :
		dec edi
			js h3era_005A413A
			mov ebx, dword ptr ss : [ebp - 0x4]
			jmp h3era_005A408C

			h3era_005A412F :
		pop edi
			mov eax, ebx
			pop esi
			pop ebx
			mov esp, ebp
			pop ebp
			ret 0xC

			h3era_005A413A:
		pop edi
			pop esi
			xor eax, eax
			pop ebx
			mov esp, ebp
			pop ebp
			ret 0xC


		check_constructs:
			mov eax, dword ptr ds : [ebx + 0x34]
			cmp byte ptr ds : [eax + isConstruct_Table], 0
			jnz h3era_005A412F
			mov eax, dword ptr ds : [ebx + 0x84]
			shr eax, 0x6 // machines, not living
			test al, 0x1
			jnz h3era_005A412F
			jmp h3era_005A413A
	}
}

long Battle_Stack_Get_Resurrect_Count = 0x00447050;
long BattleMgr_CanCast = 0x0041FA10;

void FillTables()
{
   WIN32_FIND_DATAA ffd;
   HANDLE hFind = INVALID_HANDLE_VALUE;

   /*
       hFind = FindFirstFile(_T("Mods\\Amethyst Upgrades\\Data\\creatures\\*.cfg"), &ffd);  
   if (INVALID_HANDLE_VALUE == hFind) 
       hFind = FindFirstFile(_T("Mods\\Knightmare Kingdoms\\Data\\creatures\\*.cfg"), &ffd);
   if (INVALID_HANDLE_VALUE == hFind)
       hFind = FindFirstFile(_T("Data\\creatures\\*.cfg"), &ffd);
	*/
   hFind = FindFirstFileA("Data\\creatures\\*.cfg", &ffd);

   if (INVALID_HANDLE_VALUE == hFind)  {return;} 

   do
   {
      if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      {
		 int creature = atoi(ffd.cFileName);
		 if(creature!=0)
		 {
			LoadCreatureConfig(creature);
		 }
      }
   }
   while (FindNextFileA(hFind, &ffd) != 0);
   FindClose(hFind);

   //if()
	LoadCreatureConfig(0);
}
//=============================================
//fear


_declspec(naked) void Get_Address_Stack_structure_of_Action()
{
	__asm {
		mov ecx, [699420h]
		imul eax, [ecx + 000132B8h], 15h
		add eax, [ecx + 000132BCh]
		imul eax, 00000548h
		lea eax, [eax + ecx + 000054CCh]
		retn
	}
}

_declspec(naked) void Get_address_of_stack_structure_to_which_action_directed()
{
	__asm {
		push ebx
		mov ebx, [699420h]
		push dword ptr [ebx + 44h]
		mov eax, 715872h
		call eax
		pop ecx
		mov edx, eax
		test edx, edx
		jnz L006369B0
		or eax, -1
		pop ebx
		ret
		L006369B0 :
		movsx eax, byte ptr[edx + 18h]
			cmp eax, -1
			jnz L006369BB
			pop ebx
			retn
			L006369BB :
		imul eax, 15h
			movsx ecx, byte ptr[edx + 19h]
			add eax, ecx
			imul eax, 548h
			lea eax, [eax + ebx + 000054CCh]
			pop ebx
			retn
	}
}


_declspec(naked) void OpenCreatureWindow()
{
	__asm {
		push ebp
		mov ebp, esp
		push -1
		push 62D90Bh
		mov eax, fs: [00000000h]
		push eax
		mov fs : [00000000h], esp
		sub esp, 000000B8h
		push ebx
		push esi
		push edi
		push dword ptr[ebp + 14h]
		push dword ptr[ebp + 10h]
		push dword ptr[ebp + 0Ch]
		push dword ptr[ebp + 08h]
		lea ecx, [ebp - 000000C4h]
		mov eax, 764AD6h
		call eax
		push -1 
		push -1
		lea ecx, [ebp - 000000C4h]
		mov dword ptr[ebp - 04h], 00000000h
		mov eax, 5FF800h
		call eax
		cmp dword ptr[ebp + 18h], 00000000h
		lea ecx, [ebp - 000000C4h]
		jz L00732C75
		mov eax, 5F4B90h
		call eax
		jmp L00732C7A
		L00732C75 :
		mov eax, 5F4BA0h
			call eax
			L00732C7A :
		lea ecx, [ebp - 000000C4h]
			or dword ptr[ebp - 04h], 0FFFFFFFFh
			mov eax, 5F4980h
			call eax
			mov ecx, [ebp - 0Ch]
			pop edi
			pop esi
			pop ebx
			mov fs : [00000000h], ecx
			leave
			retn 14h
	}
}

long zzz_0x27F996C = 0x27F996C; long zzz_0x887658 = 0x887658;
long Alternate_parameter_address_for_alternate_construction_dialog;
long Stack_Stack_structure_Address;
long Action_type_in_fight;
long Address_of_stack_structure_to_which_action;
long zzz_F9964h = 0xF9964; long zzz_27F996Ch = 0x27F996C; long zzz_27F9964h = 0x27F9964;
long Stack_structure_address_to_which_action_directed;
_declspec(naked) void CallTrigger()
{
	__asm {
		// get the trigger number
		mov esi, [27C1950h]
		
		//.if esi = 30371 & dword[27F9964h] = 1986 // dialogue number 1986
		zzz_if_esi:
		cmp esi, 30371
		jne zzz_elseif_esi_1
		cmp dword ptr[zzz_27F9964h], 1986
		jne zzz_elseif_esi_1

		mov eax, dword ptr [27F9968h]
		
		//.if signed eax > 4 & signed eax < 9 & dword[27F996Ch] < > 13
		zzz__if_eax:
		cmp eax, 4
		jle zzz__elseif_eax_1
		cmp eax, 9
		jge zzz__elseif_eax_1
		cmp dword ptr [zzz_27F996Ch], 13
		je  zzz__elseif_eax_1

		sub eax, 5
		mov ecx, [Alternate_parameter_address_for_alternate_construction_dialog]
		mov eax, [eax * 4 + ecx]
		xor ecx, ecx
		xor edx, edx
		inc edx
		cmp dword ptr[zzz_0x27F996C], 12
		jnz right_click
		xchg ecx, edx
		 right_click:


		////stdcall OpenCreatureWindow, eax, 119, 20, ecx, edx
		// probably
		push edx
		push ecx
		push 20
		push 119
		push eax
		call OpenCreatureWindow

			
			//.elseif(eax = 11 | eax = 13)& dword[27F996Ch] = 13
			zzz__elseif_eax_1:
			cmp eax, 11
			je zzz__ifor_eax
			cmp eax, 13
			jne zzz__endif_eax
			zzz__ifor_eax:
			cmp dword ptr[zzz_27F996Ch], 13
			jne zzz__endif_eax


			mov dword ptr[zzz_0x887658], 1 //  CloseDialog
			
			//.endif
			zzz__endif_eax:

			//.elseif esi = 30303//  Battle _!_ Before_action
			zzz_elseif_esi_1:
			cmp esi, 30303
			jne zzz_elseif_esi_2

			mov ebx, dword ptr [699420h]//  COMBAT_MANAGER; ebx now stores CombatManager
			mov eax, [ebx + 3Ch] //  Action_type
			mov[Action_type_in_fight], eax
			call Get_Address_Stack_structure_of_Action
			mov[Stack_Stack_structure_Address], eax
			call Get_address_of_stack_structure_to_which_action_directed
			mov[Stack_structure_address_to_which_action_directed], eax
			
			//.elseif esi = 30304//  Battle _!_ After_action
			zzz_elseif_esi_2:
			cmp esi, 30304
			jne zzz__endif_esi

			mov ebx, dword ptr[699420h] //  COMBAT_MANAGER; ebx now stores CombatManager
			mov edi, [Stack_Stack_structure_Address]
			mov esi, [Stack_structure_address_to_which_action_directed]

			//.if[Action_type_in_fight] = 7 & signed esi > -1 & signed[esi + 0x4c] > 0 & [edi + 34h] < > 149// CR_Shot_Tower
			cmp[Action_type_in_fight], 7
			jne zzz__improper_target
			cmp esi, -1
			jle zzz__improper_target
			cmp[esi + 0x4c], 0
			jle zzz__improper_target
			cmp[edi + 34h], 149
			je  zzz__improper_target

			mov eax, [esi + 34h]
			cmp byte ptr[eax + RangeRetaliation_Table], 0
			je no_ability
			have_ability:
			push 0
			mov ecx, esi
			mov eax, 442610h //  firing test
			call eax
			test al, al
			je no_ability
			push edi
			mov ecx, esi
			mov eax, 43F620h//  Shooting_squad_on_squad
			call eax
			//  fixed shooting bug already in a dead state if Morale fell on the stack
			
			//.if signed[edi + 0x4c] < 1 //  stack killed
			cmp[edi + 0x4c], 1
			jge zzz__stack_killed


			bt[edi + 84h], 24//  Flag_Order_morality fell; morale roll
			jnb no_morality
			mov dword ptr[ebx + 3Ch], 12 //  Cancel_action_of_dead
			no_morality:

			//.endif
			zzz__stack_killed:
		
		
			no_ability :
			
			//.endif
			zzz__improper_target:

			//.endif
			zzz__endif_esi:

			//  running overwritten code
			mov ecx, 28AAFD0h
			ret
	}
}


_declspec(naked) void CallTrigger2()
{
	__asm {
		pushad 
			call CallTrigger 
		popad
		ret
	}
}

long zzz_0x767f85 = 0x767f85;
_declspec(naked) void try_ranged_retaliation()
{
	__asm {
		//call zzz_0x767f85

		//pusha

		/*
		mov ebx, dword ptr[699420h]//  COMBAT_MANAGER; ebx now stores CombatManager
		mov eax, [ebx + 3Ch] //  Action_type
		mov Action_type_in_fight, eax
		call Get_Address_Stack_structure_of_Action
		mov Stack_Stack_structure_Address, eax
		call Get_address_of_stack_structure_to_which_action_directed
		mov Stack_structure_address_to_which_action_directed, eax


		mov ebx, dword ptr[699420h] //  COMBAT_MANAGER; ebx now stores CombatManager
		mov edi, Stack_Stack_structure_Address
		mov esi, Stack_structure_address_to_which_action_directed
		*/

		mov edi, ecx
		mov esi, ebx

		cmp esi, -1
		jle zzz__improper_target
		cmp[esi + 0x4c], 0
		jle zzz__improper_target
		cmp[edi + 34h], 149
		je  zzz__improper_target

		mov eax, [esi + 34h]
		cmp byte ptr[eax + RangeRetaliation_Table], 0
		je no_ability
		have_ability :

			mov ebx, dword ptr[699420h] //  COMBAT_MANAGER
			push 0
			mov ecx, esi
			mov eax, 442610h //  firing test
			call eax
			test al, al
			je no_ability
			push edi
			mov ecx, esi
			mov eax, 43F620h//  Shooting_squad_on_squad
			call eax
			//  fixed shooting bug already in a dead state if Morale fell on the stack

			//.if signed[edi + 0x4c] < 1 //  stack killed
			cmp[edi + 0x4c], 1
			jge zzz__stack_killed


			bt[edi + 84h], 24//  Flag_Order_morality fell; morale roll
			jnb no_morality
			// mov ebx, dword ptr[699420h] //  COMBAT_MANAGER
			mov dword ptr[ebx + 3Ch], 12 //  Cancel_action_of_dead
			no_morality:

		//.endif
	zzz__stack_killed:


	no_ability:

		//.endif
	zzz__improper_target:
		
	//popa 

	// call zzz_0x767f85


	mov ecx, edi
	mov ebx, esi

	call zzz_0x767f85
	ret

	}
}

long fix_hp_overflow_tmp;
__declspec(naked) void fix_hp_overflow()

{
	__asm
	{
		pusha

		mov esi, 42
		mov	 ebx, 0x699420
		mov	 ebx, [ebx]
		lea  edi, [ebx + 0x54cc + 0x58]
		z_loop:

		//z_again:
		cmp dword ptr [edi], 0
		jge z_next

		mov edx, [edi - 0x58 + 0x60]
		mov fix_hp_overflow_tmp, edx

		mov ecx, [edi - 0x58 + 0x6c]
		//add [edi], ecx
		//inc dword ptr [edi -0x58 +0x4c]
		//jmp z_again
		cmp ecx, 0
		je z_next

		mov eax, [edi]
		cdq
		idiv ecx
		dec eax
		sub dword ptr[edi - 0x58 + 0x4c], eax
		neg edx
		mov [edi], edx
		cmp edx, 0
		jne z_NoOverflow
		dec dword ptr[edi - 0x58 + 0x4c]

		z_NoOverflow:
		mov ecx, [edi - 0x58 + 0x34]
		// cmp byte ptr [ecx + NoGolemOverflow], 0
		mov dl, [ecx + NoGolemOverflow]
		cmp dl, 0
		je z_next

		//mov ecx, [edi - 0x58 + 0x60]
		//cmp ecx, [edi - 0x58 + 0x4C]
		mov ecx, fix_hp_overflow_tmp
		cmp ecx, [edi - 0x58 + 0x4C]

		jge z_next
		mov [edi - 0x58 + 0x4C], ecx
		mov [edi - 0x58 + 0x60], ecx

		// mov ecx, [edi - 0x58 + 0x34]

		z_next :
		dec esi
		add edi, 0x548
		cmp esi, 0
		jnz z_loop

		popa
		ret
	}
}

// BYTE addditional_moves[42];
//constexpr bool* erm_flags = (bool*)0x91F2DF;
constexpr long sizeof_H3CombatMonster = 0x548; // 1344L;
long last_action, last_target, last_current_mon, last_source_type, before_mon, before_action, last_target_pos, before_target_pos;
void ranged_retaliation_event(int trigger, int action, int target, int current_mon, int current_target_pos) {

	int COMBAT_MANAGER = *(int*)0x699420;
	int combat_stacks = COMBAT_MANAGER + 0x54cc;
	bool* erm_flags = (bool*)0x91F2DF;
	bool remote = erm_flags[997] && erm_flags[998] && (!(erm_flags[999]));
	bool true_online = erm_flags[997] && erm_flags[998];
	if (trigger == 30303){
		before_mon = last_current_mon;
		before_action = last_action;
		before_target_pos = last_target_pos;

		//Action_type_in_fight = action;
		last_action = action; last_target = target; last_current_mon = current_mon; last_target_pos = current_target_pos;

		int Stack_Stack_structure_Address = combat_stacks + last_current_mon * sizeof_H3CombatMonster;
		last_source_type = *(int*)(Stack_Stack_structure_Address + 0x34);
		int last_flags = *(int*)(Stack_Stack_structure_Address + 0x84);

		/*
		if (*(int*)(Stack_Stack_structure_Address+0x170)==0)
			*(int*)(COMBAT_MANAGER + 0x3c) = 12;
		*/
		if (last_source_type >= 0 && last_source_type < MONSTERS_AMOUNT
			&& isPassive_Table[last_source_type]
			&& last_action != 1 && last_action != 8 && last_action != 12
			&& last_action != 0 && last_action != 3) {
				if(true_online)  *(int*)(last_source_type + 0x74 + 0x50) = 0;
				else *(int*)(COMBAT_MANAGER + 0x3c) = 3;
		}

	/*
		{
			int mon_type = *(int*)(Stack_Stack_structure_Address + 0x34);
			if (mon_type >= 0 && MovesTwice_Table[mon_type])
				addditional_moves[current_mon] = 1;
		}
	*/
		/*
		if ((last_flags & 0x04000000)  && last_action == 10 ) {
			*(int*)(COMBAT_MANAGER + 0x3c) = 12;
			CALL_2(int, __thiscall, 0x464C60, COMBAT_MANAGER, 1);
		}
		*/
		/*
		Stack_Stack_structure_Address=combat_stacks + current_mon *sizeof_H3CombatMonster ;

		if (target != -1)	Stack_structure_address_to_which_action_directed = combat_stacks + target * sizeof_H3CombatMonster;
		else				Stack_structure_address_to_which_action_directed = 0;
		*/
	}
	else  if (trigger == 30304 && last_action == 7 && last_target >= 0) {
		int Stack_Stack_structure_Address = combat_stacks + last_current_mon * sizeof_H3CombatMonster;
		int Stack_structure_address_to_which_action_directed = (last_target != -1) ?
			(combat_stacks + last_target * sizeof_H3CombatMonster) : 0;

		int last_target_type		= *(int*)(Stack_structure_address_to_which_action_directed + 0x34);
		int last_source_type		= *(int*)(Stack_Stack_structure_Address + 0x34);
		int last_target_numberAlive = *(int*)(Stack_structure_address_to_which_action_directed + 0x4C);

		int last_flags = *(int*)(Stack_Stack_structure_Address + 0x84);

		if (last_source_type != 149 && last_target_type >= 0 && last_target_numberAlive >0 && 
			(RangeRetaliation_Table[last_target_type] || RangeRetaliation_Table[last_target + MONSTERS_AMOUNT])) {
			__asm {
				mov edi, Stack_Stack_structure_Address
				mov esi, Stack_structure_address_to_which_action_directed


				push 0
				mov ecx, esi
				mov eax, 442610h //  firing test
				call eax
				test al, al
				je no_ability
				push edi
				mov ecx, esi
				mov eax, 43F620h//  Shooting_squad_on_squad
				call eax
				//  fixed shooting bug already in a dead state if Morale fell on the stack

				//.if signed[edi + 0x4c] < 1 //  stack killed
				cmp[edi + 0x4c], 1
				jge zzz__stack_killed


				bt[edi + 84h], 24//  Flag_Order_morality fell; morale roll
				jnb no_morality
				mov ebx, COMBAT_MANAGER
				mov dword ptr[ebx + 3Ch], 12 //  Cancel_action_of_dead
				no_morality:
				nop
					zzz__stack_killed :
				nop
					no_ability :
				nop
					zzz__improper_target :
				nop
			}
		}

	} 
	if (trigger == 30304 && last_action != 4 && last_action != 5) fix_hp_overflow();
	if (trigger == 30304 && last_action != 1 && last_action != 4 && last_action != 5 && last_action != 8) {
		int side = last_current_mon > 21;  HERO* myhero = (HERO*) *(int*) (COMBAT_MANAGER + 0x53CC + (side << 2));
		if(myhero)
		{
			float my_inteligence = CALL_1(float, __thiscall, Hero_GetIntelligencePower, myhero);
			long maxmana = 10 * myhero->PSkill[3] * my_inteligence; WORD& mananow = myhero->SpPoints;
			if (mananow < maxmana) {

				// int Stack_Stack_structure_Address = combat_stacks + last_current_mon * sizeof_H3CombatMonster;
				mananow += (mana_regen_table[MONSTERS_AMOUNT + last_current_mon] + mana_regen_table[last_source_type])
					;// **(int*)(Stack_Stack_structure_Address + 0x4c);
				if (mananow > maxmana) mananow = maxmana;
			}
		}
	}

	/*
	if (trigger == 30304 && addditional_moves[before_mon]
		
		&& last_action != 4 && last_action != 5
		// && before_action != 8
		
		) {
		--addditional_moves[before_mon];
		*(int*)(COMBAT_MANAGER + 0x132BC) = before_mon;
		*(int*)(COMBAT_MANAGER + 0x132C8) = combat_stacks + before_mon * sizeof_H3CombatMonster;
		if (before_mon != last_current_mon) *(int*)(COMBAT_MANAGER + 0x3C) = 0;
	}
	*/

	/*
	if (trigger == 30304 && addditional_moves[last_current_mon]) {
		--addditional_moves[last_current_mon];
		int Stack_Stack_structure_Address = combat_stacks + last_current_mon * sizeof_H3CombatMonster;
		*(int*)(Stack_Stack_structure_Address +0x74 +0x10) &= ~0x04000000;
	}*/
	// else if (trigger == 30304) {
	if (trigger == 30304 && !addditional_moves[last_current_mon]) {

		int Stack_Stack_structure_Address = combat_stacks + last_current_mon * sizeof_H3CombatMonster;
		int last_flags = *(int*)(Stack_Stack_structure_Address + 0x84);
		// int mon_type = *(int*)(Stack_Stack_structure_Address + 0x34);
		if ((last_flags & 0x04000000) && last_action == 10 && last_current_mon == current_mon)
		{
			*(int*)(COMBAT_MANAGER + 0x3c) = 12;
			last_action = 12;
			// CALL_2(int, __thiscall, 0x464C60, COMBAT_MANAGER, 1);
		} 
		/*
		else {
			if (mon_type >= 0 && MovesTwice_Table[mon_type])
				addditional_moves[current_mon] = 1;
		}*/
	}

	/*
	if (trigger == 30304 && (last_action == 7 || last_action == 6 || last_action == 3)) {
		int Stack_Stack_structure_Address = combat_stacks + last_current_mon * sizeof_H3CombatMonster;
		//int source_type = *(int*)(Stack_Stack_structure_Address + 0x34);
		int last_source_pos = *(int*)(Stack_Stack_structure_Address + 0x38);

		int Stack_structure_address_to_which_action_directed = (target != -1) ?
			(combat_stacks + target * sizeof_H3CombatMonster) : 0;

		int target_numberAlive = (target != -1) ? *(int*)(Stack_structure_address_to_which_action_directed + 0x4C) : 0;
		int target_type = (target != -1) ? *(int*)(Stack_structure_address_to_which_action_directed + 0x34) : 0;

		{
			int SpellPower = *(int*)(Stack_Stack_structure_Address + 0x4c); // number of alive creatures
			SpellPower *= CreatureSpellPowerMultiplier[last_source_type];
			SpellPower /= CreatureSpellPowerDivider[last_source_type];
			SpellPower += CreatureSpellPowerAdder[last_source_type];
			if (last_action == 7 && after_shoot__spell[last_source_type] && target_numberAlive) 
				WoG_CastSpell_Monster(after_shoot__spell[last_source_type], 1, current_target_pos, SpellPower, after_action_spell_mastery[last_source_type], Stack_Stack_structure_Address);
			if (last_action == 6 && after_melee__spell[last_source_type] && target_numberAlive)
				WoG_CastSpell_Monster(after_melee__spell[last_source_type], 1, current_target_pos, SpellPower, after_action_spell_mastery[last_source_type], Stack_Stack_structure_Address);
			if (last_action == 3 && after_defend_spell[last_source_type])
				WoG_CastSpell_Monster(after_defend_spell[last_source_type], 1, last_source_pos, SpellPower, after_action_spell_mastery[last_source_type], Stack_Stack_structure_Address);
		}

		if (target_numberAlive && after_wound__spell[target_type])
		{
			int SpellPower = *(int*)(Stack_structure_address_to_which_action_directed + 0x4c); // number of alive creatures
			SpellPower *= CreatureSpellPowerMultiplier[target_type];
			SpellPower /= CreatureSpellPowerDivider[target_type];
			SpellPower += CreatureSpellPowerAdder[target_type];
			if ((last_action == 7 || last_action == 6) && after_wound__spell[target_type])
				WoG_CastSpell_Monster(after_wound__spell[target_type], 1, current_target_pos, SpellPower, after_action_spell_mastery[target_type], Stack_structure_address_to_which_action_directed);
		}
	}
	*/
		
}

_LHF_(z_hook_004476E9) {
	if (c->esi < 0x00010000 || c->edi < 0x00010000) return EXEC_DEFAULT;

	int from_ID = *(int*)(c->esi + 0x34) ;
	int from_side = *(int*)(c->esi + 0xF4) ;
	
	if (spell_1_table[from_ID] < 10 ) return EXEC_DEFAULT;
	
	int to_ID = *(int*)(c->edi + 0x34) ;
	int to_side = *(int*)(c->edi + 0xF4) ;

	if (from_ID >= 0 && to_ID >= 0 && from_side == to_side) {
		if (spell_2_table[from_ID] == 2 && Receptive_Table[to_ID]) {
			c->return_address = 0x004476DD;
			return NO_EXEC_DEFAULT;
		}
		if (spell_2_table[from_ID] == 11) {
			c->return_address = 0x004476DD;
			return NO_EXEC_DEFAULT;
		}
	}
	else return EXEC_DEFAULT;

}

_LHF_(z_hook_00756D06) {
	long mon = *(long*) (c->eax+0x34);
	if (isHellHydra[mon])	c->return_address = 0x00756D0F;
	else					c->return_address = 0X00756D51;
	

	return NO_EXEC_DEFAULT;
}

_LHF_(z_hook_004645BA) {
	if (c->eax <= *(int*) c->ebx ) {
		c->return_address = 0x004645C0;
	}
	else {
		c->return_address = 0x00464708;
		// long stack_number = (*(int*)(c->ebp+0xC)) + (*(int*)(c->ebp + 0x8))*21;
		long stack_number = *(int*)(c->esi + 0x132BC) + *(int*)(c->esi + 0x132B8) *21;
		long active_stack = *(int*)(c->esi + 0x132C8);
		long current_mon = active_stack ? *(int*)(active_stack + 0x34) : -1;
		if (addditional_moves[stack_number]) {
			*(unsigned*)(c->edi + 0x84) &= 0xF2FFFFFF;// 0x0FBFFFFFFu && ~0x09000000;
			--addditional_moves[stack_number];
		}
		else if (current_mon>=0 && 
			(MovesTwice_Table[current_mon] || MovesTwice_Table[stack_number + MONSTERS_AMOUNT])) {
			addditional_moves[stack_number] += 1;
		}
	}

return NO_EXEC_DEFAULT;
}

_LHF_(z_hook_00478C9E) {
	long batman = c->esi; if (batman < 0x10000 || batman >0x30000000) {
		
		batman = (*(int*)0x699420);
		c->esi = batman;
		
		if (!batman) v_MsgBox("Very bad bug at z_hook_00478C9E, prepare to crash",1);
	}

	return EXEC_DEFAULT;
}

_LHF_(z_hook_0046453E) {
	long batman = c->esi; if (batman < 0x10000 || batman >0x30000000) {
		// c->esi = 
		batman = (*(int*)0x699420);
		c->esi /*= c->ecx*/ = batman;
		/*
		c->esp += 8;
		c->return_address = 0x00464708;
		return NO_EXEC_DEFAULT;
		*/
		if (!batman) v_MsgBox("Very bad bug at z_hook_0046453E, prepare to crash",1);
	}
	c->edi = (c->esi + c->eax * 8 + 0x54CC);
	if (c->edi < 0x10000 || c->edi > 0x30000000) {
		v_MsgBox("Debug Warning: No current monster at z_hook_0046453E.",1);
		long source = (*(long*)(batman + 0x132b8))*21 + *(long*)(batman + 0x132bc);
		if (source<0 || source >41) {
			v_MsgBox("Debug Warning: Wrong Source monster at z_hook_0046453E.", 1);
		}

		c->edi = (batman + 0x54cc + source * sizeof(H3CombatMonster));
		*(long*)(batman + 0x132C8) = c->edi;

		c->return_address = 0x00464545;
		return NO_EXEC_DEFAULT;
	}

	long target = *(long*)(batman + 0x44);
	long action = *(long*)(batman + 0x3c);

	H3CombatMonster* me = (H3CombatMonster*) c->edi;
	// H3CombatMonster* it = (H3CombatMonster*)(batman + 0x54cc + target * sizeof(H3CombatMonster));
	if (target >= 0 && target <= 186 && me && (action == 6 || action == 7)) {


		if (aging_table[me->type] && CALL_7(char, __thiscall, CanCastSpellAtCoord, batman, 75, 1, target, me->side, 1, 0))
			CALL_7(void, __thiscall, BattleMgr_CastSpell, batman, 75, target, 1, -1, 1, aging_table[me->type]);


		if(poison_table[me->type] && CALL_7(char,__thiscall,CanCastSpellAtCoord,batman,71,1,target,me->side,1,0))
			CALL_7(void, __thiscall, BattleMgr_CastSpell, batman ,71,target,1,-1,1, poison_table[me->type]);


		if (paralyze_table[me->type] && (paralyze_chance[me->type] > Randint(0,100))
			&& CALL_7(char, __thiscall, CanCastSpellAtCoord, batman, 74, 1, target, me->side, 1, 0))
				CALL_7(void, __thiscall, BattleMgr_CastSpell, batman, 74, target, 1, -1, 1, paralyze_table[me->type]);


		// __asm nop;
	}

	c->return_address = 0x00464545;
	return NO_EXEC_DEFAULT;
}

int __stdcall CrExpBon_DwarfResistFriendly_WoG_hook(HiHook *hook,int a1, signed int a2, int a3) {
	if (a1 && Receptive_Table[*(int*)(a1+0x34)]) return 0;
	return CALL_3(int, __cdecl, hook->GetDefaultFunc(), a1,a2,a3);
}



/*
_declspec(naked) void zzz()
{
	__asm {}
}
*/

_declspec(naked) void Before_strike()
{
	__asm {
		//mov esi, [Attacker_structure]
		//mov edi, [Defender_structure]
		cmp dword ptr[edi + 454h], 0
		jle no_ability
		cmp dword ptr [edi + (62 * 4) + 198h], 0 // Blindness
		jnz no_ability
		cmp dword ptr[edi + (70 * 4) + 198h], 0//  Petrification
		jnz no_ability
		cmp dword ptr[edi + (74 * 4) + 198h], 0//  Paralysis
		jnz no_ability
		mov ecx, 2860244h//  Defensive_hero_structure
		cmp dword ptr[edi + 0xF4], 0
		jnz defender
		add ecx, 4
		defender:
		bt dword ptr[esi + 84h], 16 // Flag_No Reply
		jc no_ability
		
		mov eax, [edi + 0xf4]
		cmp eax, 0
		je left
		mov eax, 0x15
		left:
		add eax, [edi + 0xf8]
		cmp byte ptr[eax + PreventiveCounterstrikeTable + MONSTERS_AMOUNT], 0
		jne is_ability

		mov eax, [edi + 34h]
		cmp byte ptr[eax + PreventiveCounterstrikeTable], 0
		je no_ability
		is_ability:
		mov ecx, [esi + 38h] //  get attacker's position
			mov eax, [edi + 38h]
			bt[edi + 84h], 0 //  Flag_2_cellularity
			jnc zzz_not_carry//IF CARRY ?
				
				cmp [edi + 0xF4], 0
				//IF[edi + F4h] = 0
				jne zzz_else

				inc eax
				jmp zzz_endif

				zzz_else:///ELSE
				dec eax

				zzz_endif:///ENDIF
			zzz_not_carry: //ENDIF
			imul eax, 12//  get a defensive position
			add eax, [699420h]//  COMBAT_MANAGER
			add eax, 13468h//  combatManager.Cells_circle_monster
			xor edx, edx
			zzz_repeat://.REPEAT
			cmp word ptr [eax + edx * 2], cx
			je position_found
			inc edx

			cmp edx, 5 //.UNTIL signed edx > 5
			jle zzz_repeat

			position_found:
			
			push edx 
			push esi

			mov ecx, edi
			mov eax, 441330h
			call eax
			dec dword ptr[edi + 454h]
			cmp dword ptr[esi + (62 * 4) + 198h], 0
			jnz strike_cancel
			cmp dword ptr[esi + (70 * 4) + 198h], 0
			jnz strike_cancel
			cmp dword ptr[esi + (74 * 4) + 198h], 0
			jnz strike_cancel
			cmp dword ptr[esi + 4ch], 0
			sete al
			ret
			strike_cancel:
		mov al, 1
			no_ability :
			xor al, al
			ret
	}
}

_declspec(naked) void Before_Melee()
{
	__asm {
		// push esi edi

		xchg esi, edi
		call Before_strike
		xchg esi, edi

		test al, al
		je lab
		push 441C85h
		ret
		lab:
		// overwritten code :
		push dword ptr [ebp + 08h] 
		
		push esi
		push 441AE3h
		ret
	}
}





//=============================================
//special shots

int __fastcall GetShotType(int creature, int* slot)
{
   return special_missiles_table[creature];
}

__declspec(naked) void f43F72C_Hook()
{
__asm
{
	mov ecx, eax
	mov edx, esi
	call GetShotType
	cmp eax, 1
	je j43F735
	cmp eax, 2
	je j43FB27

	mov eax, 0x43FA31
	jmp eax
	
	j43FB27:
	mov eax, 0x43FB27
	jmp eax

	j43F735:
	mov eax, 0x0043F735
	jmp eax
}
}

__declspec(naked) void f41ED5A_Hook()
{
__asm
{
	push ecx
	push edx
	mov ecx, eax
	mov edx, -1
	call GetShotType
	cmp eax, 0
	jne j41ED69
	mov eax, 0x41ED6D
	pop edx
	pop ecx
	jmp eax
	j41ED69:
	mov eax, 0x41ED69
	pop edx
	pop ecx
	jmp eax

}
}


__declspec(naked) void isAmmoCart_on_BF()
{
	__asm
	{
		pusha
		mov esi, 20
		cmp eax, 0 
		je left

		right:
		add eax, 0x15 * 0x548
		left:
		
		//imul  eax, 0x548
		mov	 ebx, 0x699420
		mov	 ebx, [ebx]
		lea  eax, [ebx + eax + 0x54cc]
			z_loop:

			cmp dword ptr[eax + 0x4c], 0
			jle z_next

		mov ebx, [eax + 0x34]
		cmp byte ptr [isAmmoCart_Table +ebx ], 0

		jnz z_yes

		z_next:
		dec esi
		add eax, 0x548
		cmp esi, 0
		jnz z_loop
		jmp z_no

		z_yes :
			popa
			mov eax, 1
			ret
		z_no:
			popa
			xor eax, eax
			ret
	}
}

long h3era_Hero_HasArt = 0x004D9460;
__declspec(naked) void AmmoCart_43F718_Hook()
{
	__asm
	{
		pushad

		/*
		push 0x5
		call h3era_Hero_HasArt
		test al, al
		jne loc_0043F729
		*/
		// here is changed part begin

		//mov eax, dword ptr ds : [0x00699420] // Battle Manager
		mov eax, [esi+0xf4] //eax set to current side
		call isAmmoCart_on_BF
		cmp eax, 0
		popad 
		jne loc_0043f729

		// here is changed part end

	loc_0043F723:
		dec dword ptr[esi + 0D8h]

	loc_0043F729:
		push 0x0043F729
		ret
	}
}


__declspec(naked) void Passive_474541_Hook()
{
	__asm
	{
		cmp		byte ptr [ecx + isPassive_Table], 0
		jnz		loc_0047460F

		add     ecx, 0FFFFFF6Fh
		push	0x00474547
		ret

		loc_0047460F:
		push 0x0047460F
		ret
	}
}

__declspec(naked) void AmmoCart_422A86_Hook()
{
	__asm
	{
		cmp byte ptr[eax+isAmmoCart_Table], 0
		jnz lab1
		push 0x00422A91
		ret 

		lab1:
		push 0x00422B96
		ret
	}
}

DWORD rebirth_temp;
/// 75E108h
__declspec(naked) void  Rebirth_Plus(void) {
	__asm {
		pushad
		mov eax, dword ptr ds : [esi + 0xF8]
		mov ebx, dword ptr ds : [esi + 0xF4]
		cmp ebx, 0
		je left_side
		add eax, 21
		left_side:
		mov eax, dword ptr ds : [respawn_table_chance + MONSTERS_AMOUNT*4 + eax * 4]
		mov rebirth_temp, eax
		cmp rebirth_temp, 0.0f
		ja battle_only

		mov eax, dword ptr ds : [esi + 0x34]
		mov eax, dword ptr ds : [respawn_table_chance + eax*4]
		mov rebirth_temp, eax

		battle_only:
		popad
		cmp rebirth_temp, 0.0f
		jbe zero_chance

		cmp rebirth_temp, 1.0f
		jae all_chance

		cmp rebirth_temp, 0.2f
		je fifth_chance 

		some_chance:
		push 75E149h
		ret

		//todo

		fifth_chance:
		push 75E149h
		ret

		all_chance:
		push 75E136h
		ret

		zero_chance:
		push 75E11Ah
		ret
	}
}

_LHF_(rebirth_004690B8) {
	c->ebx = c->eax * respawn_table_sure[*(int*)(c->esi + 0x34)];
	int ret = c-> eax * respawn_table_fraction[*(int*)(c->esi+0x34)];
	c->eax = c->edx = ret; 
	c->return_address = 0x004690C2;
	return NO_EXEC_DEFAULT;
}
_LHF_(rebirth_004690CF) {
	long dice = c->eax; 
	float coin = double (dice) / double (0x8000);
	float chance = respawn_table_chance[*(int*)(c->esi + 0x34)];

	if (coin < chance) ++(c->ebx);

	c->return_address = 0x004690DC;
	return NO_EXEC_DEFAULT;
}


//============================================
//MOP

__declspec(naked) void ReduceTargetDefense(void) {
	__asm {
		mov edx, [2846BCCh]
		mov dword ptr [ebp - 14h], 64h
		movzx eax, byte ptr [ecx + ReduceTargetDefense_Table]
		mov[ebp - 10h], eax
		mov dword ptr[ebp - 0Ch], 0
		fild qword ptr[ebp - 10h]
		fidiv dword ptr[ebp - 14h]
		fstp qword ptr[ebp - 10h]
		fild dword ptr[edx + 000000CCh]
		fmul qword ptr[ebp - 10h]
		push 75D4F2h
		ret

	}
}

short RTD2_Temp1; long RTD2_Temp2;
__declspec(naked) void ReduceTargetDefense2(void) {
	__asm {
		mov word ptr [RTD2_Temp1], 64h
		movzx eax, byte ptr[eax + ReduceTargetDefense_Table]
		mov[RTD2_Temp2], eax
		fild dword ptr[RTD2_Temp2]
		fidiv word ptr[RTD2_Temp1]
		fstp dword ptr[RTD2_Temp2]
		fild dword ptr[ebp + 0Ch]
		fstp dword ptr[ebp + 0Ch]
		fld dword ptr[ebp + 0Ch]
		fmul dword ptr[RTD2_Temp2]
		push 44230Dh
		ret
	}
}

__declspec(naked) void  ThreeHeadedAttack(void) {
	__asm {
		cmp byte ptr[eax + ThreeHeadedAttack_Table], 0
		jnz lab1
		push 4414A4h
		ret
		lab1:
		push 44138Fh
			ret
	}
}


__declspec(naked) void  ThreeHeadedAttack2(void) {
	__asm {
		cmp byte ptr[eax + ThreeHeadedAttack_Table], 0
		jnz lab1
		push 4488DFh
		ret
		lab1:
		push 4487F3h
			ret
	}
}

__declspec(naked) void ImposedSpellsGeneral(void) {
	__asm {
		imul edi, [esp + 4], 6
		add edi, ImposedSpells_ptr
		xor ebx, ebx
		lab1:
		movsx eax, byte ptr[edi + ebx] /// Spell Number
			test eax, eax
			jz lab2
			push 0
			movsx ecx, byte ptr[edi + ebx + 3] /// Spell Level
			push ecx
			push 1000 /// number of rounds
			push eax
			push dword ptr[ebp - 4]
			mov eax, 0x71465A
			call eax
			add esp, 0x14
			lab2:
		inc ebx
			cmp ebx, 3
			jl lab1
			ret 4
	}
}


__declspec(naked) void ImposedSpells1(void) {
	__asm {
		push dword ptr[ebp - 10h]
		call ImposedSpellsGeneral
		push 75E9F9h
		ret
	}
}

__declspec(naked) void ImposedSpells2(void) {
	__asm {
		push dword ptr[ebp - 0Ch]
		call ImposedSpellsGeneral
		push 7608B1h
		ret
	}
}

__declspec(naked) void  Spells(void) {
	__asm {
		mov eax, [esi + 0x34]
		cmp eax, 0xAE
		jl L004483C4
		cmp eax, 0xBF
		jg L004483C4
		push esi
		mov eax, 0x76BEEA
		call eax
		pop ecx
		push eax
		push 2
		push -1
		push 1
		push dword ptr[ebp + 0x08]
		mov eax, [esi + 0x34]
		movzx eax, byte ptr[eax + Spells_Table]
		push eax
		push 0x4483D2
		ret
		L004483C4:
		mov eax, [ebp + 0x08]
		push 6
		push 2
		push -1
		mov ecx, [esi + 0x34]
		push 1
		push eax
		movzx eax, byte ptr[ecx + Spells_Table]
		push eax
		push 0x4483D2
		ret
	}
}


__declspec(naked) void  Spells2(void) {
	__asm {
		push 1 
		push edi 
		push eax
		mov eax, [esi + 0x34]
		movzx eax, byte ptr[eax + Spells_Table]
		push eax
		push 0x4476CE
		ret
	}
}


__declspec(naked) void  Spells3(void) {
	__asm {
		mov eax, [ebp - 0x0C]
		mov bl, byte ptr[eax + Spells_Table]
		test bl, bl
		jnz lab1

		mov bl, byte ptr[eax + hasSantaGuards]
		test bl, bl
		jnz lab1

		push 0x75D112
		ret
		lab1:
		push 0x75D0FB
		ret
	}
}


__declspec(naked) void  Spells3Minus(void) {
	__asm {
		mov eax, [ebp - 0x0C]
		mov bl, byte ptr[eax + Spells_Table]
		test bl, bl
		jnz lab1
		//push 0x75D112
		//ret
		lab1 :
		push 0x75D0FB
			ret
	}
}

__declspec(naked) void  Spells4(void) {
	__asm {
		mov edx, [ebp + 0x20]
		cmp edx, MONSTERS_AMOUNT
		jge Default

		movzx ecx, byte ptr [edx + Spells_Table]
		mov[eax + 0x000004E0], ecx
		push 0x75CDEC
		ret

		Default:
		mov[eax + 0x000004E0], 0
		push 0x75CDEC
		ret

	}
}


__declspec(naked) void  Sharpshooters(void) {
	__asm {
		cmp byte ptr[eax + Sharpshooters_Table], 0
		jnz lab1
		push 0x75DD6C
		ret
		lab1:
		push 0x75DDD2
			ret
	}
}

__declspec(naked) int zShootAnyTime(Byte* Mon) {
	__asm {
		push 0x0071ED4B
		ret
	}
}

static long SAT_MonID;
static Byte* SAT_MonPTR;
static char  SAT_Ret;
char __stdcall ShootAnyTime(int Art)
{
	__asm mov   SAT_MonPTR, esi
	__asm mov	EAX, dword ptr DS:[esi + 0x34] //added line
	__asm mov	SAT_MonID, EAX	//added line
	__asm push  Art
	__asm mov   eax, 0x4D9460
	__asm call  eax
	__asm mov   SAT_Ret, al
	__asm pusha
	if (SAT_Ret == 0) SAT_Ret = (char)zShootAnyTime(SAT_MonPTR);
	if (SAT_Ret == 0) SAT_Ret = ShootingAdjacent_Table[SAT_MonID]; //addded line
	__asm popa
	//  __asm mov   al,SAT_Ret
	return SAT_Ret;
}

char __stdcall ShootAnyTime2(int Art)
{
	__asm mov   SAT_MonPTR, edi
	__asm mov	EAX, dword ptr DS:[edi + 0x34] //added line
	__asm mov	SAT_MonID, EAX //added line
	__asm push  Art
	__asm mov   eax, 0x4D9460
	__asm call  eax
	__asm mov   SAT_Ret, al
	__asm pusha
	if (SAT_Ret == 0) SAT_Ret = (char)zShootAnyTime(SAT_MonPTR);
	if (SAT_Ret == 0) SAT_Ret = ShootingAdjacent_Table[SAT_MonID]; //addded line
	__asm popa
	//  __asm mov   al,SAT_Ret
	return SAT_Ret;
}

int ShootingAdjacent_temp; char ShootingAdjacent_return;
__declspec(naked) void  ShootingAdjacent(void) {
	__asm {
		mov DS:[0x2846414], esi
		
		mov ShootingAdjacent_return, 0
		mov eax, [esi+0x34]
		mov ShootingAdjacent_temp, eax

		test ecx, ecx
		jz L007671A5
		push dword ptr[ebp + 0x08]
		mov eax, 0x4D9460
		call eax
		test al, al
		mov ShootingAdjacent_return, al
		jnz L007671CD
		L007671A5:
		mov esi, [0x2846414]
			//mov eax, [esi + 0x34]
			mov eax, ShootingAdjacent_temp
			mov al, [eax + ShootingAdjacent_Table]
			mov ShootingAdjacent_return, al
			test al, al
			jnz L007671CD
			pushad
			push  dword ptr[2846414h]
			mov eax, 0x71ED4B
			call eax
			mov ShootingAdjacent_return, al
			pop ecx
			popad

			mov al, ShootingAdjacent_return
			test al, al
			jnz L007671CD

			mov ShootingAdjacent_return, 0
			// xor al, al
			L007671CD:
			movsx eax, ShootingAdjacent_return
			pop edi
			pop esi
			pop ebx
			pop ebp
			retn 4
	}
}


__declspec(naked) void  MagicChannel(void) {
	__asm {
		mov ecx, [eax - 0x000000C0]
		cmp byte ptr DS:[ecx + MagicChannel_Table], 0
		jnz lab1
		push 0x5A250A
		ret
		lab1:
		push 0x5A24E5
			ret
	}
}


__declspec(naked) void  MagicMirror(void) {
	__asm {
		mov ecx, [ecx + 0x34]
		cmp byte ptr[ecx + MagicMirror_Table], 0
		jnz lab1
		push 0x44854F
		ret
		lab1:
		push 0x448531
		ret
	}
}

__declspec(naked) void JoustingBonus(void) {
	__asm {
		mov eax, DS:[0x28460C0]
		cmp byte ptr[eax + JoustingBonus_Table], 0
		jnz lab1
		push 0x75D84D
		ret
		
		lab1:
		mov dword ptr DS:[0x2846420], 0x0B
			push 0x75D860
		ret}
}


__declspec(naked) void   ImmunToJoustingBonus(void) {
	__asm {
		cmp byte ptr[eax + ImmunToJoustingBonus_Table], 0
		jnz lab1
		push 0x443083
		ret
	
			lab1:
		push 0x4430A3
		ret
	}
}

extern "C" __declspec(dllexport) int check_harpy(H3CombatMonster* arg) {
	return StrikeAndReturn_Table[arg->type] 
		|| StrikeAndReturn_Table[MONSTERS_AMOUNT + 
		arg->side * 21 + arg->sideIndex];
}

_LHF_(harpy_hook_0075E063) {
	H3CombatMonster* source = (H3CombatMonster*)c->ebx;
	if (StrikeAndReturn_Table[c->eax] 
		|| StrikeAndReturn_Table[MONSTERS_AMOUNT  
		+ source->side *21 + source->sideIndex] ) {
		c->return_address = 0x0075E08B;
		return NO_EXEC_DEFAULT;
	}
	else {
		c->return_address = 0x0075E06A;
		return NO_EXEC_DEFAULT;

	}

}
_LHF_(harpy_hook_00762940) {

	H3CombatMonster* source = (H3CombatMonster*)(*(int*)(c->ebp - 8));
	if (StrikeAndReturn_Table[*(int*)(c->ebp-4)]
		|| StrikeAndReturn_Table[MONSTERS_AMOUNT
		+ source->side * 21 + source->sideIndex]) {
		c->return_address = 0x00762969;
		return NO_EXEC_DEFAULT;
	}
	else {
		c->return_address = 0x00762958;
		return NO_EXEC_DEFAULT;

	}
}

long StrikeAndReturn_temp;
__declspec(naked) void  StrikeAndReturn(void) {
	__asm {
		mov StrikeAndReturn_temp, eax
		mov eax, [ebx + 0xF8]
		cmp [ebx + 0xF4], 0
		je left
		add eax, 21
		left:
		movsx eax, byte ptr[eax + StrikeAndReturn_Table + MONSTERS_AMOUNT]
		cmp eax, 1
		mov eax, StrikeAndReturn_temp
		je lab4

		//mov eax, StrikeAndReturn_temp
		movsx eax, byte ptr[eax + StrikeAndReturn_Table]
		
		//.if ~eax
		cmp eax, 0
		jne lab1

		push 0x75E06A
		ret

		//.elseif eax = 1
		lab1 :
		cmp eax,1
		jne lab2
	
		push 0x75E0BB
		ret
		//.else
		lab2:
		push 0x75E08B
		ret
		//.endif

		lab4:
		push 0x75E0BB
		ret
	}
}

__declspec(naked) void  StrikeAndReturn2(void) {
	__asm {
		mov edx, [ebp - 18h]
		add edx, MONSTERS_AMOUNT
		cmp byte ptr[edx + StrikeAndReturn_Table], 0
		jnz lab1

		mov edx, [ebp - 4]
		cmp byte ptr[edx + StrikeAndReturn_Table], 0
		jnz lab1

		mov edx, [ebp - 4]
		push 0x762955
		ret

		lab1 :
		mov edx, [ebp - 4]
		push 0x762969
			ret
	}
}

long StrikeAndReturn3_temp;
__declspec(naked) void  StrikeAndReturn3(void) {
	__asm {

		mov ecx, [esi + 0xF8]
		mov StrikeAndReturn3_temp, ecx
		mov ecx, [esi + 0xF4]
		cmp ecx, 0
		je left_side
		add StrikeAndReturn3_temp, 21
		left_side:
		mov ecx, StrikeAndReturn3_temp 
		cmp byte ptr[ecx + StrikeAndReturn_Table + MONSTERS_AMOUNT], 0
		jnz lab1

		mov ecx, [esi + 34h]
		cmp byte ptr[ecx + StrikeAndReturn_Table], 0
		jnz lab1

		mov ecx, [esi + 34h]
		push 0x421C17
		ret
		lab1 :

		mov ecx, [esi + 34h]
		push 0x421C36
			ret
	}
}

__declspec(naked) void  NoWallPenalty(void) {
	__asm {
		cmp byte ptr[eax + NoWallPenalty_Table], 0
		jnz lab1
		push 0x7605C5
		ret
		lab1:
		push 0x760622
		ret
	}
}

__declspec(naked) void  MagicAura(void) {
	__asm {
		mov eax, [eax + 0x34]
		cmp byte ptr[eax + MagicAura_Table], 0
		jnz lab1
		push 0x7672B1
		ret
			lab1 :
		push 0x7672A5
		ret
	}
}


__declspec(naked) void  Fear(void) {
	__asm {
		mov edi, dword ptr ds : [0x02846900]
		mov esi, [edi + 0xf4]
		mov edi, [edi + 0xf8]
		cmp esi, 0
		je left
		add edi, 21
		left:
		cmp byte ptr[edi + Fear_Table + MONSTERS_AMOUNT], 0
		jnz lab1

		cmp byte ptr[ebx + Fear_Table], 0
		jnz lab1
		push 0x760509
		ret
			lab1 :
		push 0x76056E 
			ret
	}
}


__declspec(naked) void  Fearless(void) {
	__asm {

		mov eax, [edi + 0xF8]
		cmp [edi + 0xF4], 0
		je left
		add eax, 21
		left:
		cmp byte ptr[eax + Fearless_Table + MONSTERS_AMOUNT], 0
		jnz lab1

		mov eax, [edi + 0x34]
		cmp byte ptr[eax + Fearless_Table], 0
		jnz lab1
		push 0x76049C 
		ret
			lab1 :
		push 0x7604D0
			ret
	}
}

__declspec(naked) void  DeathBlow(void) {
	__asm {
		mov edi, [eax + 0xf8]
		mov esi, [eax + 0xf4]
		cmp esi, 0
		je left
		add edi, 0x15
		left:
		mov ecx, [eax + 0x34]
		xor edx, edx
		mov dword ptr[ebp - 4], 0
		mov dl, [ecx + DeathBlow_Table]
		add dl, [edi + DeathBlow_Table + MONSTERS_AMOUNT]
		cmp edx, 100
		jl not_always
		mov edx, 100
		not_always:
		add[ebp - 4], edx
		push 766CF5h
		ret
	}
}


__declspec(naked) void DeathBlowCorrect(void) {
	__asm {
		cmp eax, DS:[2860220h]
		jg lab1
		push 4436E7h
		ret
		lab1:
		push 44388Eh
			ret
	}
}



constexpr int sizeof_Creature_ = sizeof(MONSTER_PROP);
char* PlusLuckEngText = "\n %s are always lucky.";
__declspec(naked) void AlwaysPositiveLuck(void) {
	__asm {
		cmp byte ptr[ebx + AlwaysPositiveLuck_Table], 0
		jnz lab1
		push 044C18Ah// no bonus
		ret
		lab1:
		mov esi, 1
		cmp dword ptr[ebp + 0x14], esi
		jl lab2
		push 44C18Ah
		ret
		lab2:
			//imul ebx, sizeof._Creature_
			imul ebx, sizeof_Creature_
			//push dword[ebx + MonTable + _Creature_.Mon_name]
			push dword ptr[ebx + new_monsters + 0x18]
		lea eax, [ebp - 30h]
			//.if[Language] = 0
		push PlusLuckEngText
			//.else
			//push PlusLuckRusText
			//.endif
		push    eax
		mov eax, 0x50C7F0
		call eax
		push 0x44C161
		ret
	}
}


__declspec(naked) void  AlwaysPositiveLuck2(void) {
	__asm {
		mov ebx, [edi + 0x34]
		cmp byte ptr[ebx + AlwaysPositiveLuck_Table], 0
		jnz lab1
		push 0x43DCFF // no bonus
		ret
		lab1:
		push 0x43DCE8
			ret
	}
}


__declspec(naked) void  AlwaysPositiveLuck3(void) {
	__asm {
		mov ecx, [esi + edi * 4]
		cmp byte ptr [ecx + AlwaysPositiveLuck_Table], 0
		jnz lab1
		push 0x44B12E // no bonus
		ret
		lab1:
		push 0x44B124
			ret
	}
}

__declspec(naked) void AlwaysPositiveMorale(void) {
	__asm {
		cmp byte ptr DS:[edi + AlwaysPositiveMorale_Table], 0
		jnz lab1
		push 0x44AF0B 
		ret
		lab1:
		push 0x44AF01 
		ret
	}
}
__declspec(naked) void AlwaysPositiveMorale2(void) {
	__asm {
		cmp byte ptr DS:[eax + AlwaysPositiveMorale_Table], 0
		jnz lab1
		push 0x44BCBD
		ret
		lab1:
		push 0x44BC58
		ret
	}
}
__declspec(naked) void AlwaysPositiveMorale3(void) {
	__asm {
		cmp byte ptr DS:[ecx + AlwaysPositiveMorale_Table], 0
		jnz lab1
		push 0x76714B
		ret
		lab1:
		push 0x76713F
		ret
	}
}

long CounterStrike_temp;
__declspec(naked) void CounterStrike(void) {
	__asm {

		movzx eax, byte ptr[eax + Counterstrike_Table]
		mov [esi + 0x454], eax

		mov eax, [esi + 0xF8]
		cmp [esi + 0xF4],0
		je left
		add eax, 21
		left:
		movzx eax, byte ptr[eax + MONSTERS_AMOUNT + Counterstrike_Table]
		add [esi + 0x454], eax

		push 0x43D6CB
		ret
	}
}
__declspec(naked) void CounterStrike2(void) {
	__asm {
		movzx eax, byte ptr[eax + Counterstrike_Table]
		mov[esi + 0x454], eax

		mov eax, [esi + 0xF8]
		cmp[esi + 0xF4], 0
		je left
		add eax, 21
		left:
		movzx eax, byte ptr[eax + MONSTERS_AMOUNT + Counterstrike_Table]
		add[esi + 0x454], eax

		push 0x446E9C
		ret
	}
}


int lab_2846BC8 = 0x2846BC8;
__declspec(naked) void Regeneration(void) {
	__asm {
		mov esi, [RegenerationHitPoints_Table + eax * 4]
		//mov[lab_2846BC8], esi
		mov DS:[0x2846BC8], esi
		movsx ebx, byte ptr[eax + RegenerationChance_Table]
		test ebx, ebx
		jnz lab1
		push 0x75DE8E
		ret
		lab1:
		mov eax, 0x7563A0
		call eax
		cmp eax, ebx
		push 0x75DF28
		ret
	}
}


__declspec(naked) void ManaDrain(void) {
	__asm {
		cmp word ptr[eax * 2 + ManaDrain_Table], 0
		lea eax, [eax - 0x3D]
		jnz lab1
		push 0x4650D5
		ret
		lab1:
		push 0x465128
		ret
	}
}


__declspec(naked) void ManaDrainPlus(void) {
	__asm {
		cmp word ptr[eax * 2 + ManaDrain_Table], 0
		// lea eax, [eax - 0x3D]
		jnz lab1
		cmp byte ptr [eax + isFaerieDragon_Table], 0
		jnz lab2
		push 0x4650DD
		ret
		lab1:
		push 0x465128
		ret
		lab2:
		push 0x0046511C
		ret
	}
}

__declspec(naked) void ManaDrain2(void) {
	__asm {
		mov edx, [ebx + 0x34]
		sub ax, [edx * 2 + ManaDrain_Table]
		test ax, ax
		push 0x46517A
		ret
	}
}

__declspec(naked) void SpellsCostDump(void) {
	__asm {
		push ebx
		xor ebx, ebx
		xor ecx, ecx
		cycle:
		mov eax, [edi + ecx * 4]
			test eax, eax
			jl lab1
			movzx eax, byte ptr[eax + SpellsCostDump_Table]
			cmp eax, ebx
			jl lab1
			mov ebx, eax
			lab1:
		inc ecx
			cmp ecx, 7
			jl cycle
			add esi, ebx
			pop ebx
			push 4E5569h
			ret
	}
}


__declspec(naked) void SpellsCostLess(void) {
	__asm {
		cmp dword ptr DS:[0x803288], 0
		jz L00766BDE
		cmp dword ptr DS:[0x2772730], 0
		jnz L00766BF3
		L00766BDE:
		mov eax, [ebp - 0x18]
			movzx eax, byte ptr[eax + SpellsCostLess_Table]
			//.if eax
			cmp eax, 0
			je L_endif_1

			mov[ebp - 0x14], eax
			//.endif
			L_endif_1:
			push 0x766B90
			ret
			L00766BF3:
		mov dword ptr[ebp - 0x0C], 0
			mov eax, [ebp - 0x18]
			movzx eax, byte ptr[eax + SpellsCostLess_Table]
			//.if eax
			cmp eax, 0
			je L_endif_2

			mov[ebp - 0x0C], eax
			//.endif
			L_endif_2:
			push 0x766C0D
			ret
	}
}
//============================================
//majaczek


__declspec(naked) void   Dem1(void)
{
	__asm {
		cmp byte ptr[eax + spell_1_table], 2
		mov dword ptr[ebp - 04h], 0
		je lab1
		push 421218h
		ret
		lab1:
		push 4211F9h
			ret
	}
}

__declspec(naked) void   Dem2(void)
{
	__asm {
		mov ecx, [ecx + 34h]
		cmp byte ptr[ecx + spell_1_table], 2
		jnz lab1
		push 421280h
		ret
		lab1:

		cmp byte ptr[ecx + spell_2_table], 9
		je z_necro

		cmp byte ptr[ecx + spell_2_table], 10
		je z_construct

		cmp byte ptr[ecx + spell_2_table], 11
		je z_gating

		push 4212CDh
		ret

	z_necro:
		mov ecx, dword ptr ss : [ebp - 0x10]
		push 0x1
		push edi
		push edx
		call get_raise_undead
			cmp eax, esi
			je lab_421306
			test byte ptr ds : [esi + 0x84] , 0x1
			je lab_421382
			mov ecx, esi
			call BattleStack_GetSecondOccupedGexNum
			mov ecx, dword ptr ss : [ebp - 0x10]
			mov edi, eax
			mov eax, dword ptr ds : [ebx + 0x20]
			push 0x1
			push edi
			push eax
		call get_raise_undead

		jmp lab_421302

	z_construct:
		mov ecx, dword ptr ss : [ebp - 0x10]
		push 0x1
		push edi
		push edx
		call get_repair_constructs

			cmp eax, esi
			je lab_421306
			test byte ptr ds : [esi + 0x84] , 0x1
			je lab_421382
			mov ecx, esi
			call BattleStack_GetSecondOccupedGexNum
			mov ecx, dword ptr ss : [ebp - 0x10]
			mov edi, eax
			mov eax, dword ptr ds : [ebx + 0x20]
			push 0x1
			push edi
			push eax

		call get_repair_constructs
		jmp lab_421302


		z_gating :
		mov ecx, dword ptr ss : [ebp - 0x10]
			push 0x1
			push edi
			push edx
			call get_repair_constructs

			cmp eax, esi
			je lab_421306
			test byte ptr ds : [esi + 0x84] , 0x1
			je lab_421382
			mov ecx, esi
			call BattleStack_GetSecondOccupedGexNum
			mov ecx, dword ptr ss : [ebp - 0x10]
			mov edi, eax
			mov eax, dword ptr ds : [ebx + 0x20]
			push 0x1
			push edi
			push eax

			call get_can_gate
			jmp lab_421302


	lab_004212D9:
		push 0x004212D9
		ret

	lab_421302:
		push 0x00421302
		ret

	lab_421306:
		push 0x00421306
		ret

	lab_421382:
		push 0x00421382
		ret

	}
}

__declspec(naked) void  Dem3(void)
{
	__asm {
		mov eax, [eax + 34h]
		cmp byte ptr[eax + spell_1_table], 2
		jnz lab1
		push 421323h
		ret
		lab1:
		push 421340h
			ret
	}
}


__declspec(naked) void Dem4(void)
{
	__asm {

		mov eax, [esi + 34h]

		cmp byte ptr[eax + spell_2_table], 11
		je z_gating

		cmp byte ptr[eax + spell_2_table], 10
		je z_construct

		cmp byte ptr[eax + spell_2_table], 9
		je z_necro

		cmp byte ptr[eax + spell_1_table], 2
		jnz lab1
		push 44715Ah
		ret
		lab1:
		push 44716Ch
			ret

		z_necro:
			mov ecx, dword ptr ss : [ebp + 0x8]
			push 0x1
			push ecx
			mov ecx, dword ptr ds : [0x00699420] // Battle Manager
			push edi
			call get_raise_undead

			push 0x0044717E
			ret

		z_construct:
			mov ecx, dword ptr ss : [ebp + 0x8]
				push 0x1
				push ecx
				mov ecx, dword ptr ds : [0x00699420] // Battle Manager
				push edi
				call get_repair_constructs

				push 0x0044717E
				ret


		z_gating :
			mov ecx, dword ptr ss : [ebp + 0x8]
				push 0x1
				push ecx
				mov ecx, dword ptr ds : [0x00699420] // Battle Manager
				push edi
				call get_can_gate

				push 0x0044717E
				ret
	}
}



__declspec(naked) void Dem5(void)
{
	__asm {
		mov eax, [esi + 34h]
		cmp byte ptr[eax + spell_1_table], 2
		jnz lab1
		push 4474D7h
		ret
		lab1:
		push 4474E6h
			ret
	}
}


__declspec(naked) void Dem6(void)
{
	__asm {
		mov eax, [ecx + 34h]
		cmp byte ptr[eax + spell_1_table], 2
		je lab1
		push 491F48h
		ret
		lab1:
		push 491EE5h
			ret
	}
}

__declspec(naked) void   Demon(void)
{
	__asm {
		mov edi, demonolgy_Temp1
		movsx edi, word ptr[edi * 2 + Demonology_Table]
		//imul edi, sizeof._Creature_
		imul edi, sizeof_Creature_
		//idiv[esi + edi + _Creature_.Health]
		idiv dword ptr[esi + edi + 0x4c]
		push 44720Ah
		ret
	}
}


__declspec(naked) void   Demon2(void)
{
	__asm {
		push edi
		mov edi, demonolgy_Temp1
		movsx edi, word ptr[edi * 2 + Demonology_Table]
		//imul edi, sizeof._Creature_
		imul edi, sizeof_Creature_
		//idiv[esi + edi + _Creature_.Health]
		idiv dword ptr[esi + edi + 0x4c]
		pop edi
		push 4470D3h
		ret
	}
}


__declspec(naked) void  Demon3(void)
{
	__asm {
		mov ecx, [ebx + 34h]
		movsx ecx, word ptr[ecx * 2 + Demonology_Table]
		push ecx
		push eax
		mov ecx, edi
		push 5A776Fh
		ret
	}
}



__declspec(naked) void Gate2(void)
{
	__asm {
		pusha
		push eax
		mov eax, [ebx + 0x132C8]
		test eax, eax
		jz z_cast_by_hero

		//mov edx, [eax + 0x34]
		mov edx, demonolgy_Temp1

		/*
		mov ecx, ebx
		mov ebx, [ecx + 0x132BC]
		mov edx, [ecx + 0x132B8]
		cmp edx, 0
		je defender
		add ebx, 21
		defender:
		push eax
		mov eax, ebx
		mov edx, 0
		mul sizeof_H3CombatMonster
		add eax, ecx
		lea ebx, [eax + 0x54CC]
		pop eax
		mov edx, [ebx + 0x34]
		*/

		cmp [spell_2_table + edx], 11
		je z_gate
		
		z_cast_by_hero:
		pop eax
 		popa

		cmp eax, ecx
		mov dword ptr ds : [esi + 0x58] , edx
		jle h3era_5A78C6

		push 0x5A78BC
		ret

		h3era_5A78C6:
		push 0x5A78C6
		ret

		z_gate:
		pop eax
		popa
		mov dword ptr ds : [esi + 0x58] , edx
		push 0x5A78C6
		ret
	}
}

__declspec(naked) void Gate1(void)
{
	__asm {
		
		push edx
		push eax
		mov eax, demonolgy_Temp1
		cmp byte ptr [spell_2_table + eax], 11
		je Gating
		pop eax
		pop edx

		cmp eax, ecx
		lea eax, dword ptr ss : [ebp - 0x4]
		jl h3era_4470C2

		push 0x4470BF
		ret

		h3era_4470C2:
		push 0x4470C2
		ret

		// ---------------

		Gating:

		pop eax
		pop edx

		lea eax, dword ptr ss : [ebp + 0x8]
		mov eax, dword ptr ds : [eax]
		//mov esi, dword ptr ds : [<o_pCreatureInfo>]
		mov esi, dword ptr ds : [0x006747B0]
		mov ecx, edx
		cdq
		idiv    dword ptr[esi + 160Ch]
			mov dword ptr ss : [ebp - 0x8] , ecx
			mov dword ptr ss : [ebp + 0x8] , eax
			cmp ecx, eax
			lea eax, dword ptr ss : [ebp - 0x8]
			jl h3era_4470E3
			lea eax, dword ptr ss : [ebp + 0x8]
		h3era_4470E3:
			mov eax, dword ptr ds : [eax]
			pop esi
			mov esp, ebp
			pop ebp
			ret 0x4

	}
}

/*
__declspec(naked) inline void Ang1_sub1(void) {
	__asm {
		push edi
		push ecx
		mov edi, dword ptr ss : [ebp + 0x8]
		mov esi, dword ptr ds : [edi + 0x60]
		mov eax, dword ptr ds : [edi + 0x4C]
		sub esi, eax
		mov eax, dword ptr ds : [ecx + 0x4C]
		mov dword ptr ss : [ebp + 0x8] , esi

		mov ecx, demonolgy_Temp1
		add eax, [CreatureSpellPowerAdder + ecx * 4]

		lea eax, dword ptr ds : [eax + eax * 4]
		lea eax, dword ptr ds : [eax + eax * 4]
		shl eax, 0x2

		imul eax, [CreatureSpellPowerMultiplier + ecx * 4]
		cdq
		idiv[CreatureSpellPowerDivider + ecx * 4]

		cdq
		idiv dword ptr ds : [edi + 0xC0]
		pop ecx
		pop edi
		mov dword ptr ss : [ebp - 0x4] , eax
	}
}
*/

/*
long Ang1_temp;
__declspec(naked) void Ang1(void)
{
	__asm {
		mov demonolgy_Temp1, eax



		//push 44705Fh
		//ret
		
		push edi
		push ecx
		
		mov edi, dword ptr ss : [ebp + 0x8]
		mov esi, dword ptr ds : [edi + 0x60]
		mov eax, dword ptr ds : [edi + 0x4C]
		sub esi, eax
		mov eax, dword ptr ds : [ecx + 0x4C]
		mov dword ptr ss : [ebp + 0x8] , esi

		mov ecx, demonolgy_Temp1
		add eax, [CreatureSpellPowerAdder + ecx * 4]

		lea eax, dword ptr ds : [eax + eax * 4]
		lea eax, dword ptr ds : [eax + eax * 4]
		shl eax, 0x2

		imul eax, [CreatureSpellPowerMultiplier + ecx * 4]
		cdq
		idiv [CreatureSpellPowerDivider + ecx * 4]

		cdq
		idiv dword ptr ds : [edi + 0xC0]
		
		mov Ang1_temp, edi
		pop ecx
		pop edi
		
		mov dword ptr ss : [ebp - 0x4] , eax
		

		lab5:
		mov eax, demonolgy_Temp1
		cmp byte ptr[eax + spell_2_table], 11
		jnz lab4

		//cmp esi, eax
		//lea eax, dword ptr ss : [ebp + 0x8]
		//jl h3era hd.4470E3
		push 0x0044708C
		ret

		lab4:
		cmp byte ptr[eax + spell_2_table], 10
		jnz lab3
			// jmp check_overflow
			// push 0x0044708C 
			push 44705Fh
		ret

		lab3:
		cmp byte ptr[eax + spell_2_table], 9
		jnz lab2
			// jmp check_overflow
			// push 0x0044708C 
			push 44705Fh
		ret

		lab2:
		cmp byte ptr[eax + spell_1_table], 0
		jnz lab1

			// jmp check_overflow
			// push 0x0044708C 
			push 44705Fh
		ret

		lab1:
		cmp byte ptr[eax + spell_1_table], 21
		jnz lab0

		// jmp check_overflow
		// push 0x0044708C 
		push 44705Fh

		lab0:
		push 447098h
		ret

		check_overflow:
		push ecx
		push edi
		mov edi, Ang1_temp
		mov ecx, [edi + 0x60]
		sub ecx, [edi + 0x4c]
		mov edi, [ebp - 0x04]
		cmp edi, ecx
		jl no_overflow
		mov[ebp - 0x04], ecx
		pop edi
		pop ecx
		push 0x00447098
		ret
		no_overflow :
		pop edi
		pop ecx
		push 0x0044708C
		ret
	}
}
*/

__declspec(naked) void Ang1(void)
{
	__asm {
		mov demonolgy_Temp1, eax


		cmp byte ptr[eax + spell_2_table], 11
		jz lab_yes // jnz lab4
		//push 44705Fh
		//ret
		lab4 :
		cmp byte ptr[eax + spell_2_table], 10
			jz lab_yes //	jnz lab3
			
			//push 44705Fh
			//ret

			lab3 :
		cmp byte ptr[eax + spell_2_table], 9
			jz lab_yes //jnz lab2
			//push 44705Fh
			//ret

			//lab2 :
		cmp byte ptr[eax + spell_1_table], 0
			jz lab_yes //jnz lab1
			//push 44705Fh
			//ret

		jmp lab_no

		lab_yes:
		push edi
		push ecx
		mov edi, dword ptr ss : [ebp + 0x8]
		mov esi, dword ptr ds : [edi + 0x60]
		mov eax, dword ptr ds : [edi + 0x4C]
		sub esi, eax
		mov eax, dword ptr ds : [ecx + 0x4C]
		mov dword ptr ss : [ebp + 0x8] , esi

		mov ecx, demonolgy_Temp1
		add eax, [CreatureSpellPowerAdder + ecx * 4]

		lea eax, dword ptr ds : [eax + eax * 4]
		lea eax, dword ptr ds : [eax + eax * 4]
		shl eax, 0x2

		imul eax, [CreatureSpellPowerMultiplier + ecx * 4]
		cdq
		idiv[CreatureSpellPowerDivider + ecx * 4]

		cdq
		idiv dword ptr ds : [edi + 0xC0]
		pop ecx
		pop edi
		mov dword ptr ss : [ebp - 0x4] , eax
		//cmp esi, eax
		//lea eax, dword ptr ss : [ebp + 0x8]
		//jl h3era hd.4470E3
		push 0x0044708C
		ret

		

			/*
			lab1 :
		push 447098h
			ret
			*/
		lab_no:
		push 447098h
			ret
	}
}


__declspec(naked) void  Ang2(void)
{
	__asm {
		mov eax, [esi + 34h]
		cmp byte ptr[eax + spell_1_table], 0
		jnz lab1
		push 447194h
		ret
		lab1:

		cmp byte ptr[eax + spell_2_table], 9
		jnz lab2
		push 447194h
		ret

		lab2:
		cmp byte ptr[eax + spell_2_table], 10
		jnz lab3
		push 447194h
		ret

		lab3:
		cmp byte ptr[eax + spell_2_table], 11
		jnz lab4
		push 447194h
		ret


		lab4:
		push 4471D2h
		ret
	}
}



__declspec(naked) void Ang3(void)
{
	__asm {
		mov eax, [esi + 34h]
		mov esi, [esi + 4Ch]
		cmp byte ptr[eax + spell_1_table], 0
		je lab1
		ret
		lab1:
		mov eax, 0Dh
			ret
	}
}

__declspec(naked) void  AngDem1(void)
{
	__asm {

		cmp byte ptr[eax + spell_2_table], 11
		je zret

		cmp byte ptr[eax + spell_2_table], 10
		je zret

		cmp byte ptr[eax + spell_2_table], 9
		je zret

		cmp byte ptr[eax + spell_1_table], 0
		je zret
		cmp byte ptr[eax + spell_1_table], 2

		// je zret
		// cmp byte ptr[eax + spell_2_table], 9

		jnz zno
		zret:
		push 0x4211BD
		ret
		zno :
		push 0x4211C7
		ret
		//endp
	}
}

__declspec(naked) void AngDem2(void)
{
	__asm {
		//mov[Temp1], eax // remember for later use in the calculation of demonology(proc Demon)
		mov demonolgy_Temp1, eax

		cmp byte ptr[eax + spell_2_table], 11
		je z_gating

		cmp byte ptr[eax + spell_2_table], 10
		je z_repair

		cmp byte ptr[eax + spell_2_table], 9
		je z_necro

		cmp byte ptr[eax + spell_1_table], 0
		je zret
		cmp byte ptr[eax + spell_1_table], 2

		//je zret
		//cmp byte ptr[eax + spell_2_table], 9

		jnz zno

		zret:
		push 0x447107
		ret
		zno :
		push 0x447111
		ret
			//endp
		z_necro:

		mov eax, dword ptr ds : [esi + 0xDC]
			test eax, eax
			jg nero_h3era_44711B
			pop edi
			xor al, al
			pop esi
			mov esp, ebp
			pop ebp
			ret 0x4

			nero_h3era_44711B:

		mov eax, dword ptr ds : [esi + 0x288]
			test eax, eax
			je necro_h3era_447134
			mov eax, dword ptr ds : [esi + 0xF4]
			mov edi, 0x1
			sub edi, eax
			jmp necro_h3era_44713A
			necro_h3era_447134:
			mov edi, dword ptr ds : [esi + 0xF4]
			necro_h3era_44713A:
			mov ecx, dword ptr ds : [0x00699420] // Battle Manager
			push 0x27 // raise undead
			push edi
			call BattleMgr_CanCast // h3era_41FA10
			test al, al
			jne h3era_447154
			pop edi
			pop esi
			mov esp, ebp
			pop ebp
			ret 0x4


			z_repair:

			mov eax, dword ptr ds : [esi + 0xDC]
				test eax, eax
				jg repair_h3era_44711B
				pop edi
				xor al, al
				pop esi
				mov esp, ebp
				pop ebp
				ret 0x4

			repair_h3era_44711B:

			mov eax, dword ptr ds : [esi + 0x288]
				test eax, eax
				je repair_h3era_447134
				mov eax, dword ptr ds : [esi + 0xF4]
				mov edi, 0x1
				sub edi, eax
				jmp repair_h3era_44713A
				repair_h3era_447134 :
			mov edi, dword ptr ds : [esi + 0xF4]
				repair_h3era_44713A :
				mov ecx, dword ptr ds : [0x00699420] // Battle Manager
				push 0x25 // cure
				push edi
				call BattleMgr_CanCast // h3era_41FA10
				test al, al
				jne h3era_447154
				pop edi
				pop esi
				mov esp, ebp
				pop ebp
				ret 0x4





			z_gating:

			mov eax, dword ptr ds : [esi + 0xDC]
				test eax, eax
				jg gating_h3era_44711B
				pop edi
				xor al, al
				pop esi
				mov esp, ebp
				pop ebp
				ret 0x4

				gating_h3era_44711B:

			mov eax, dword ptr ds : [esi + 0x288]
				test eax, eax
				je gating_h3era_447134
				mov eax, dword ptr ds : [esi + 0xF4]
				mov edi, 0x1
				sub edi, eax
				jmp gating_h3era_44713A
				gating_h3era_447134 :
			mov edi, dword ptr ds : [esi + 0xF4]
				gating_h3era_44713A :
				
				//mov ecx, dword ptr ds : [0x00699420] // Battle Manager
				//push 0x25 // cure
				//push edi
				//call BattleMgr_CanCast // h3era_41FA10
				//test al, al
				//jne h3era_447154

				mov     ecx, [ebp + 8]
				push    1
				push    ecx
				mov ecx, dword ptr ds : [0x00699420] // Battle Manager
				push edi
				call get_can_gate
				test al, al
				jne h3era_44717E

				pop edi
				pop esi
				mov esp, ebp
				pop ebp
				ret 0x4


		h3era_44717E:
			push 0x44717E
			ret

		h3era_447154:
			push 0x447154
			ret
	}
}

__declspec(naked) void AngDem3(void)
{
	__asm {

		cmp byte ptr[eax + spell_2_table], 11
		je z_gating

		cmp byte ptr[eax + spell_2_table], 10
		je z_repair

		cmp byte ptr[eax + spell_2_table], 9
		je z_necro


		cmp byte ptr[eax + spell_1_table], 0
		je zret
		cmp byte ptr[eax + spell_1_table], 2
		
		//je zret
		//cmp byte ptr[eax + spell_2_table], 9

		jnz zno

		zret:
		push 0x44748E
		ret
		zno :
		push 0x4476F2
		ret
		
		//endp
		z_necro:
		

		mov     eax, [esi + 0DCh]
		test    eax, eax
		jle		zno

			mov eax, dword ptr ds : [esi + 0x288]
			test eax, eax
			je necro_h3era_4474B5
			mov eax, dword ptr ds : [esi + 0xF4]
			mov edi, 0x1
			sub edi, eax
			jmp necro_loc_004474BB
		
			necro_h3era_4474B5:
			mov     edi, [esi + 0F4h]

				necro_loc_004474BB:

				mov ecx, dword ptr ds : [0x00699420] // Battle Manager
				push 0x0
				push edi
				call BattleMgr_CanCast
				test al, al
				jz zno

			jmp necro_loc_004474E6 // shortcut
			
				necro_loc_004474E6:
			mov ecx, dword ptr ds : [0x00699420] // Battle Manager

			push 0x1
			push ebx
			push edi
			call get_raise_undead
			// test eax, eax
			push 0x004474F5
			ret


			z_repair :


			mov     eax, [esi + 0DCh]
				test    eax, eax
				jle		zno

				mov eax, dword ptr ds : [esi + 0x288]
				test eax, eax
				je repair_h3era_4474B5
				mov eax, dword ptr ds : [esi + 0xF4]
				mov edi, 0x1
				sub edi, eax
				jmp repair_loc_004474BB

				repair_h3era_4474B5 :
			mov     edi, [esi + 0F4h]

				repair_loc_004474BB :

				mov ecx, dword ptr ds : [0x00699420] // Battle Manager
				push 0x0
				push edi
				call BattleMgr_CanCast
				test al, al
				jz zno

				jmp repair_loc_004474E6 // shortcut

				repair_loc_004474E6 :
			mov ecx, dword ptr ds : [0x00699420] // Battle Manager

				push 0x1
				push ebx
				push edi
				call get_repair_constructs
				// test eax, eax
				push 0x004474F5
				ret



			z_gating:


			mov     eax, [esi + 0DCh]
				test    eax, eax
				jle		zno

				mov eax, dword ptr ds : [esi + 0x288]
				test eax, eax
				je gating_h3era_4474B5
				mov eax, dword ptr ds : [esi + 0xF4]
				mov edi, 0x1
				sub edi, eax
				jmp gating_loc_004474BB

				gating_h3era_4474B5 :
			mov     edi, [esi + 0F4h]

				gating_loc_004474BB :

				mov ecx, dword ptr ds : [0x00699420] // Battle Manager
				push 0x0
				push edi
				call BattleMgr_CanCast
				test al, al
				jz zno

				jmp gating_loc_004474E6 // shortcut

				gating_loc_004474E6 :
			mov ecx, dword ptr ds : [0x00699420] // Battle Manager

				push 0x1
				push ebx
				push edi
				call get_can_gate
				// test eax, eax
				push 0x004474F5
				ret
	}
}



__declspec(naked) void AngDem4(void)
{
	__asm {

		cmp byte ptr[ecx + spell_2_table], 11
		je z_gating

		cmp byte ptr[ecx + spell_2_table], 10
		je z_repair


		cmp byte ptr[ecx + spell_2_table], 9
		je z_necro

		cmp byte ptr[ecx + spell_1_table], 0
		je zret
		cmp byte ptr[ecx + spell_1_table], 2

		//je zret
		//cmp byte ptr[eax + spell_2_table], 9

		jnz zno

		zret:
		push 491EA2h
			ret
		zno :
			push 491E9Dh
			ret
			//endp

		z_necro:
		push 491EA2h
		ret


		z_repair:
		push 491EA2h
			ret



		z_gating:
		push 491EA2h
			ret
	}
}



	__declspec(naked) void AngDem5(void)
{
	__asm {

		cmp byte ptr[eax + spell_2_table], 11
		je gating

		cmp byte ptr[eax + spell_2_table], 10
		je constructs

		cmp byte ptr[eax + spell_2_table], 9
		je necro

		cmp byte ptr[eax + spell_1_table], 0
		jnz lab1
		push 0x492233
		ret
		lab1:
		cmp byte ptr[eax + spell_1_table], 2
		jnz lab2
		push 0x492136
		ret
		lab2:
		push 0x492077
		ret
			//endp



		gating:

		mov ecx, dword ptr ds : [esi + 0x60]
			mov eax, dword ptr ds : [esi + 0x4C]
			cmp ecx, eax
			jle h3era_491EE5


			mov eax, dword ptr ds : [esi + 0x34]

			cdq
			imul eax, MON_Sizeof
			cmp byte ptr ds : [eax + new_monsters] , 3

			//cmp byte ptr ds : [edx + isConstruct_Table] , 0
			//lea edi, isConstruct_Table
			//mov dl, [edx + edi]
			//cmp dl, 0

			je h3era_492286

			mov edx, dword ptr ds : [esi + 0x84]
			shr edx, 0x6 // machines - not living
			test dl, 0x1
			//push 0x0049224D
			jne h3era_492286
			lea edi, only_infernals_can_be_gated

			push 0x0049225D
			ret


			constructs:

		mov ecx, dword ptr ds : [esi + 0x60]
			mov eax, dword ptr ds : [esi + 0x4C]
			cmp ecx, eax
			jle h3era_491EE5


			mov edx, dword ptr ds : [esi + 0x34]

			cmp byte ptr ds : [edx + isConstruct_Table] , 0
			//lea edi, isConstruct_Table
			//mov dl, [edx + edi]
			//cmp dl, 0

			jne h3era_492286

			mov edx, dword ptr ds : [esi + 0x84]
			shr edx, 0x6 // machines - not living
			test dl, 0x1
			//push 0x0049224D
			jne h3era_492286
			lea edi, only_Machines_can_be_repaired

			push 0x0049225D
			ret

		necro:
		mov ecx, dword ptr ds : [esi + 0x60]
		mov eax, dword ptr ds : [esi + 0x4C]
		cmp ecx, eax
		jle h3era_491EE5
		mov edx, dword ptr ds : [esi + 0x84]
		shr edx, 0x12 // undead - not living
		test dl, 0x1
		//push 0x0049224D
		jne h3era_492286

		//mov eax 
		//push 0x0049224F
		lea edi, only_Undead_can_be_raised
		push 0x0049225D
		ret

		h3era_491EE5:
		push 0x491EE5
		ret

		h3era_492286:
		push 0x492286
		ret
	}
}



__declspec(naked) void IsAngel7(void)
{
	__asm {
		push eax
		mov eax, [esi +0x34]
		cmp[eax + spell_1_table_ptr], 0
		jne not_angel
		cmp[eax + spell_2_table_ptr], 0
		jne not_angel
		cmp[eax + spell_3_table_ptr], 0
		jne not_angel
		pop eax
		push 0x00447194
		ret

		not_angel :
		pop eax
		push 0x004471D2
		ret
	}
}

__declspec(naked) void IsAngel6_CastEmptyField(void)
{
	__asm {
		cmp[eax + spell_1_table_ptr], 0
		jne not_angel
		cmp[eax + spell_2_table_ptr], 0
		jne not_angel
		cmp[eax + spell_3_table_ptr], 0
		jne not_angel
		push 0x00447107
		ret

		not_angel :
		push 0x00447102
			ret
	}
}

__declspec(naked) void IsAngel5_AI(void)
{
	__asm {
		cmp[eax + spell_1_table_ptr], 0
		jne not_angel
		cmp[eax + spell_2_table_ptr], 0
		jne not_angel
		cmp[eax + spell_3_table_ptr], 0
		jne not_angel
		push 0x004211BD
		ret

		not_angel:
		push 0x004211B8
		ret
	}
}

// 0x0076735D
__declspec(naked) void IsAngel3_Plus(void)
{
	__asm {
		mov    eax, [esi + 0x34]
		//cmp    eax, 0x0D
		//je     l_Ok
		//cmp    eax, 150
		//je     l_Ok

		push ebx
		cmp eax, 0
		jl l_bad
		cmp eax, MONSTERS_AMOUNT
		jge l_bad

		mov ebx, spell_1_table_ptr
		add ebx, eax
		mov ebx, DS : [EBX]
		and ebx, 0xff
		cmp ebx, 0
		jne l_bad
		mov ebx, spell_2_table_ptr
		add ebx, eax
		mov ebx, DS : [EBX]
		and ebx, 0xff
		cmp ebx, 0
		jne l_bad
		mov ebx, spell_3_table_ptr
		add ebx, eax
		mov ebx, DS : [EBX]
		and ebx, 0xff
		cmp ebx, 0
		jne l_bad
		jmp l_Ok
	}
l_bad:
	__asm {
		pop ebx

		mov    esi, [esi + 0x4C]
		lea    esi, [esi + esi * 4]
		lea    esi, [esi + esi * 4]

		push  0x005A87C3
		ret
	}
l_Ok:
	__asm {
		mov ebx, eax
		//shl ebx, 2

		//mov    eax, 0x0D
		mov    esi, [esi + 0x4C]
		lea    esi, [esi + esi * 4]
		lea    esi, [esi + esi * 4]
		shl    esi, 0x02
		mov    eax, esi



		push edx
		mul    CreatureSpellPowerMultiplier[ebx * 4]
		div    dword ptr CreatureSpellPowerDivider[ebx * 4]
		add    eax, CreatureSpellPowerAdder[ebx * 4]
		pop edx

		mov esi, eax

		mov eax, ebx
		pop ebx
		//mov    eax, 0x0D

		push 0x005A8812
		ret
	}
}


// 0x0076735D
__declspec(naked) void IsAngel3_Minus(void)
{
	__asm {
		mov    eax, [esi + 0x34]
		//cmp    eax, 0x0D
		//je     l_Ok
		//cmp    eax, 150
		//je     l_Ok

		push ebx
		cmp eax, 0
		jl l_bad
		cmp eax, MONSTERS_AMOUNT
		jge l_bad
		/*
		mov ebx, spell_1_table_ptr
		add ebx, eax
		mov ebx, DS : [EBX]
		and ebx, 0xff
		cmp ebx, 0
		jne l_bad
		*/
		mov ebx, spell_2_table_ptr
		add ebx, eax
		mov ebx, DS : [EBX]
		and ebx, 0xff
		cmp ebx, 0
		jne l_bad
		/*
		mov ebx, spell_3_table_ptr
		add ebx, eax
		mov ebx, DS : [EBX]
		and ebx, 0xff
		cmp ebx, 0
		jne l_bad
		*/
		jmp l_Ok
	}
l_bad:
	__asm {
		pop ebx

		mov    esi, [esi + 0x4C]
		ret
	}
l_Ok:
	__asm {
		mov ebx, eax
		//shl ebx, 2

		//mov    eax, 0x0D
		mov    esi, [esi + 0x4C]
		mov    eax, esi

		push edx

		mul    CreatureSpellPowerMultiplier[ebx * 4]
		div    dword ptr CreatureSpellPowerDivider[ebx * 4]
		add    eax, CreatureSpellPowerAdder[ebx * 4]

		pop edx

		mov esi, eax
		pop ebx
		mov    eax, 0x0D
		ret
	}
}

// 0x0076735D
__declspec(naked) void IsAngel3(void)
{
	__asm {
		mov    eax, [esi + 0x34]
		//cmp    eax, 0x0D
		//je     l_Ok
		//cmp    eax, 150
		//je     l_Ok

		push ebx
		cmp eax, 0
		jl l_bad
		cmp eax, MONSTERS_AMOUNT
		jge l_bad

		mov ebx, spell_1_table_ptr
		add ebx, eax
		mov ebx, DS : [EBX]
		and ebx, 0xff
		cmp ebx, 0
		jne l_bad
		mov ebx, spell_2_table_ptr
		add ebx, eax
		mov ebx, DS : [EBX]
		and ebx, 0xff
		cmp ebx, 0
		jne l_bad
		mov ebx, spell_3_table_ptr
		add ebx, eax
		mov ebx, DS : [EBX]
		and ebx, 0xff
		cmp ebx, 0
		jne l_bad
		jmp l_Ok
	}
l_bad:
	__asm {
		pop ebx

		mov    esi, [esi + 0x4C]
		ret
	}
l_Ok:
	__asm {
		mov ebx, eax
		//shl ebx, 2

		//mov    eax, 0x0D
		mov    esi, [esi + 0x4C]
		mov    eax, esi
		
		push edx

		mul    CreatureSpellPowerMultiplier[ebx * 4]
		div    dword ptr CreatureSpellPowerDivider[ebx * 4]
		add    eax, CreatureSpellPowerAdder[ebx * 4]

		pop edx

		mov esi, eax
		pop ebx
		mov    eax, 0x0D
		ret
	}
}

int ret_004482DC = 0x004482DC;
__declspec(naked) void Angel4_004482D3(void)
{
	__asm {
		lea eax, dword ptr ds : [eax + eax * 4]
		lea eax, dword ptr ds : [eax + eax * 4]
		shl eax, 2

		push edx
		push ebx

		//mov ebx, [ecx + 0x132c8] //activeStack
		//cmp ebx, 0
		//je z_null
		//mov ebx, [ebx + 0x34] //monster type
		//mov ebx, demonolgy_Temp1

		mov ebx, [ecx + 0x132BC]
		mov edx, [ecx + 0x132B8]
		cmp edx, 0
		je defender
		add ebx, 21
		defender:
		push eax
		mov eax, ebx
		mov edx, 0 
		mul sizeof_H3CombatMonster
		add eax, ecx
		lea ebx, [eax + 0x54CC]
		pop eax
		mov ebx,[ebx + 0x34]

		mul    CreatureSpellPowerMultiplier[ebx * 4]
		div    dword ptr CreatureSpellPowerDivider[ebx * 4]
		add    eax, CreatureSpellPowerAdder[ebx * 4]

		
		z_null:
		pop ebx
		pop edx

		jmp ret_004482DC
	}
}
/*
__declspec(naked) void f_num_Hook()
{
__asm
{
	
}
}
*/

//Creature SpellPower
DWORD f_75D2BB_temp;
__declspec(naked) void f_75D2BB_Hook()
{
	__asm
	{
		pop edi
		pop esi
		pop ebx
		pop ebp
		pusha

		mov ebx, DS:[esi + 0x34]
		
		//mov ecx, CreatureSpellPowerMultiplier[ebx*4]
		//mul ecx
		mul CreatureSpellPowerMultiplier[ebx * 4]

		//mov ecx, dword ptr CreatureSpellPowerDivider[ebx*4]
		//mov edx, 0
		//div ecx
		div dword ptr CreatureSpellPowerDivider[ebx * 4]

		//mov ecx, CreatureSpellPowerAdder[ebx*4]
		//add eax, ecx
		add eax, CreatureSpellPowerAdder[ebx * 4]

		mov f_75D2BB_temp, EAX

		popa
		mov EAX, f_75D2BB_temp
		retn

	}
}

_LHF_(Creature_SpellPower_5A0145) {
	int casterKind = *(int*)(c->ebp+0x10);
	if(casterKind != 1)
		return EXEC_DEFAULT;
	// int CombatManager = (*(int*)0x699420);
	auto bm = H3CombatManager::Get();
	int Spell = *(int*)(c->ebp + 0x08);
	if(Spell == 74 || Spell ==71) return EXEC_DEFAULT;

	auto caster = &bm->stacks[bm->currentMonSide][bm->currentMonIndex];
	auto mon_type = caster->type; auto mon_count = caster->numberAlive;

	int& Spellpower = *(int*)(c->ebp + 0x1c);

	Spellpower = mon_count; 
	if ((mon_type >= 174) && (mon_type <= 191))
		Spellpower = CALL_1(int, __cdecl, 0x0076BEEA, caster);

	Spellpower *= CreatureSpellPowerMultiplier[mon_type];
	Spellpower /= CreatureSpellPowerDivider[mon_type];
	Spellpower += CreatureSpellPowerAdder[mon_type];
	return EXEC_DEFAULT;
}

//hooked at 0x005A0140
int lab_5A0145 = 0x005A0145;
__declspec(naked) void Creature_SpellPower_hook() {

	__asm {
		//pushad
		push ebx 
		push edx
		push esi
		push edi

		mov ecx, [0x699420]
		//cmp[esp + 0x14], 1 //0x0c+ 0x08
		cmp[ecx + 0x3c], 0xA
		jne non_monster_spell

		////mov eax, [esp + 0x20] //0x18 + 0x08
		//mov ebx, [ecx + 0x132c8] //activeStack
		//mov ebx, [ebx + 0x34] //monster type
		mov ebx, [ecx+ 0x132BC] // INT32 currentMonIndex;
		cmp [ecx+0x132b8], 0
		je attacker
		add ebx, 21 // are we sure it should be here ?
		attacker:
		mov eax, 0x548 // sizeof H3CombatMonster
		mul ebx
		lea ebx, [ecx + 0x54CC + eax]
		mov esi, [ebx + 0x4c] //monster count
		mov ebx, [ebx + 0x34] //monster type

		mov eax, [esp + 0x28] //0x18 + 0x10
		cmp CreatureSpellPowerScaled[ebx],0
		je non_scaled
		imul esi
		non_scaled:

		mul    CreatureSpellPowerMultiplier[ebx * 4]
		div    dword ptr CreatureSpellPowerDivider[ebx * 4]
		add    eax, CreatureSpellPowerAdder[ebx * 4]

		mov edx, [ebp - 0x30]
		cmp edx, 74 // paralyze
		je limit_spellpower
		cmp edx, 71 // poison
		je limit_spellpower

		jmp apply_spell_power
		limit_spellpower:
		mov eax, 5
		apply_spell_power:
		mov [ebp - 0x1c], eax //0x18 + 0x10

		non_monster_spell:
		
		exit:
		pop edi
		pop esi
		pop edx 
		pop ebx
		//popad

		//
		push ebp
		mov ebp, esp
		push 0xFFFFFFFF
		//
		
			//	mov eax, 0x005A0145
			//	jmp eax
			jmp lab_5A0145
	}
}

//Necromancy
__declspec(naked) void f_4e3ed0_Hook()
{
	__asm
	{

	PUSH ESI
	MOV ESI, ECX
	XOR EAX, EAX
	LEA ECX, [ESI + 0x12D]
		
	pusha
		
	mov al, [esi + 0xd5]
	
	//mov edx Necromancy_without_artifacts[eax]
	lea ebx, [Necromancy_without_artifacts + eax*4]

	mov edx, [ebx]
	mov DS:[0x4E3F3A], edx
	
	
	XOR EAX, EAX

	mov edx, Necromancy_with_artifacts[1]
	mov DS : [0x004e3f33], edx

	mov edx, Necromancy_with_artifacts[2]
	mov DS : [0x004e3f2a], edx

	mov edx, Necromancy_with_artifacts[3]
	mov DS : [0x004e3f1f], edx

	popa

	_unknown_1: // loc_004E3EDB
	CMP  DS : [ECX], 0x82 //  Cloak of the Undead King
	je _unknown_3 // loc_004E3F14  
	inc eax
	add ecx, 8
	CMP EAX, 0x13
	jl _unknown_1 // loc_004E3EDB
	MOV EAX,  DS : [0x660B68] // o_ArtSetup
	MOV EAX,  DS : [EAX + 0x1058]
	cmp eax, -1
	je _unknown_2 // loc_004E3F39  
	MOV EDX,  DS : [0x660B6C] // ComboArtSetUpPo
	LEA ECX, [EAX * 2 + EAX]
	MOV EAX,  DS : [ECX * 8 + EDX]
	MOV ECX, ESI
	push eax
	CALL CS: 0x004D9460 // Hero_HasArt
	mov eax, 0x4e3f39;
	jmp eax

	_unknown_2: // loc_004E3F39  
	mov eax, 0x4e3f39
	jmp eax
	
	_unknown_3: // loc_004E3F14  
	xor eax, eax
	mov al, [esi + 0D5h] // Necromancy
	lea eax, [Necromancy_with_artifacts + eax * 4]
	mov eax, [eax]
	pop esi
	retn
	//mov eax, 0x4e3f14
	//jmp eax

	//// @ 0x4e3f14
	//// this part don't work in my dissassembler
	//// dissassembled by hand from old 8086 instructions
	//mov al, 0xd5
	//add al, [ebx+esi] // or reverse ???
	//cmp al, 0x03

	}
}
__declspec(naked) void f_433438_Hook()
{
	__asm
	{
		movzx	 eax, al
		mov		 eax, Necromancy_with_artifacts[eax*4]
		push	 0x433458
		ret
	}
}



__declspec(naked) void Firewall_bit(void) {
	__asm {
		mov eax, [ebp - 2Ch]
		mov ecx, eax
		and eax, 1Fh
		shr ecx, 5
		bt dword ptr[FireWall_Table + ecx * 4], eax
		jc lab1
		push 75CA76h
		ret
		lab1 :
		push 75CA42h
			ret
	}
}


__declspec(naked) void Firewall_byte(void) {
	__asm {
		mov eax, SS:[ebp - 2Ch]
		//mov ecx, eax
		//and eax, 1Fh
		//shr ecx, 5
		//bt dword ptr[FireWall_Table + ecx * 4], eax
		cmp byte ptr [FireWall_Table+eax], 0
		//jc lab1
		jnz lab1
		push 75CA76h
		ret
		lab1 :
		push 75CA42h
			ret
	}
}

//Hell Steed
__declspec(naked) void f_75CA39_Hook()
{
__asm
{
	pusha
	mov  eax , SS:[EBP-0x2c]
	mov  bh , isHellSteed[eax]
	cmp  bh , 0
	je _isNotHellSteed
	
	_isHellSteed:
	popa
	MOV EAX, SS : [EBP - 0x20]
	CMP DWORD PTR DS : [EAX], 6
	JNE _another_action
	mov eax, 0x0075ca4a
	jmp eax

	_another_action:
	mov eax, 0x0075CA76
	jmp eax
	//mov eax, 0x0075ca42
	//jmp eax
	
	_isNotHellSteed:
	popa
	mov eax, 0x0075ca76
	jmp eax
}
}
//
__declspec(naked) void f_75E8BA_Hook()
{
__asm
{
	
	pusha
	mov  eax , SS:[EBP-0x10]
	mov  bh , isHellSteed2[eax]
	cmp  bh , 0
	je _isNotHellSteed
	
	_isHellSteed:
	popa	
	mov eax, 0x0075e8c3
	jmp eax
	
	_isNotHellSteed:
	popa
	mov eax, 0x0075e8da
	jmp eax
}
}
//
__declspec(naked) void f_760723_Hook()
{
__asm
{
	
	pusha
	mov  eax , SS:[EBP-0x0c]
	mov  bh , isHellSteed3[eax]
	cmp  bh , 0
	je _isNotHellSteed
	
	_isHellSteed:
	popa	
		mov eax, 0x0076072c
		jmp eax
	_isNotHellSteed:
	popa
		mov eax, 0x00760743
		jmp eax
}
}


//Sorceress
__declspec(naked) void f_75C96C_Hook()
{
__asm
{
	pusha
	mov ecx, SS:[EBP-0x2c]
	mov dh, isSorceress[ecx]
	cmp dh, 0
	je _isNotSorceress
	
	_isSorceress:
	popa
		mov eax, 0x0075c979
		jmp eax
	_isNotSorceress:
	popa
		mov eax, 0x0075ca39
		jmp eax
}
}


//Enchanter bad
__declspec(naked) void f_447ED9_Hook()
{
__asm
{   

	pusha
	mov EBX, DS:[EAX+0x34]
	mov ch, isEnchanter[EBX]
	cmp ch, 0
	je _isNotEnchanter
	
	_isEnchanter:
	popa
	//mov DS:[eax+0x34], 0x88
	mov eax, 0x00447ee2
	jmp eax
	//push 0x00447ee2
	//retf

	_isNotEnchanter:
	popa
	mov eax, 0x00447f1b
	jmp eax
	//push  0x00447f1b
	//retf
	

}
}

//Enchanter
__declspec(naked) void f_4650dd_Hook()
{
	__asm
	{
		pusha
		//mov eax, DS:[EBX+0x34]
		//add eax, 0x88
		mov ch, isEnchanter[EAX]
		cmp ch, 0
		je _isNotEnchanter

		_isEnchanter:
		popa
		MOV ECX, DS : [ESI + 0x132C0]
		mov eax, 0x004650e9
		jmp eax
	
		_isNotEnchanter:
		popa
		mov eax, 0x004653e2
		jmp eax
	}
}

//Santa Gremlin
__declspec(naked) void f_75D0F2_Hook()
{
__asm
{
	pusha
	mov dh, hasSantaGuards[ecx]
	cmp dh, 0
	je  _hasntSantaGuards
	_hasSantaGuards:
	popa
	mov eax, 0x0075d0fb
	jmp eax

	_hasntSantaGuards:
	popa	
	mov eax, 0x0075d112
	jmp eax
}
}

int MakeSGGuards = 0x0075CDA2;
void __stdcall MakeDalionsGuards(Byte* man, Byte* mon, int SG_stack);
//Dalions Guards
__declspec(naked) void f_75D10A_Hook() {
	__asm
	{
		//lea eax, []
		call MakeDalionsGuards
		push 0x0075D10F
		ret
	}
}

__declspec(naked) void SantaFix ()
{
	__asm
	{
		cmp dword ptr[ebp + 0x20], 0x000000AD
		jnz L0075D03E
		mov ecx, [ebp - 0x10]
		cmp ecx, [ebp - 0x0C]
		push  0x0075CEE3
		ret
		L0075D03E:
		push 0x75D03E
		ret
	}
}
 
__declspec(naked) void SantaFixPlus()
{
	__asm
	{
		push eax
		//cmp dword ptr[ebp + 0x20], 0x000000AD
		//jnz L0075D03E
		mov eax, [ebp + 0x20]

		cmp eax, MONSTERS_AMOUNT
		jge L0075D03E

		cmp hasSantaGuards[eax], 0
		je L0075D03E
		pop eax
		mov ecx, [ebp - 0x10]
		cmp ecx, [ebp - 0x0C]
		push  0x0075CEE3
		ret
		L0075D03E :
		pop eax
		push 0x75D03E
			ret
	}
}

long last_ghost;
//Ghost
__declspec(naked) void f_756B1F_Hook()
{

__asm
{   
	pusha
	mov EBX, DS:[EAX+0x34]

	mov CH, isGhost[EBX]
	cmp ch, 0
	je _isNotGhost
	
	_isGhost:
	mov last_ghost, ebx
	mov DS:[0x756C9A], EBX
	popa
	mov eax, 0x00756b2c
	jmp eax
	_isNotGhost:
	popa
	mov eax, 0x00756cad
	jmp eax
}
}

_LHF_(z_756BA0_hook) {
	//long dword_2860224 = *(int*)0x2860224;
	//c->eax *= ghost_fraction[*(_DWORD*)(dword_2860224 + 52)];
	
	c->eax *= ghost_fraction[last_ghost];
	return EXEC_DEFAULT;
}


__declspec(naked) void fun_4E609E_Rogue_Hook()
{
	__asm {
		push    ebp
		mov     ebp, esp
		push    esi
		push	edi
		mov     esi, [ebp + 8]
		xor eax, eax
		mov     edx, 7
		
		NextMon:
		cmp		dword ptr [ecx], -1
		je      NotThis
		mov		edi, [ecx]
		cmp     byte ptr [isRogue + edi], 0
		je      NotThis
	    add     eax, [ecx + 0x1c]

		NotThis:
		add     ecx, 4
		dec     edx 
		jnz     NextMon
		pop		edi
		pop     esi
		pop     ebp
		retn    4
	}
}

__declspec(naked) void f_MoP_Rogue_Hook()
{
__asm{
	//	; Change function :

	mop_007572FA:
		PUSH ESI
		MOV ESI,ECX
		CMP DWORD PTR DS : [ESI + 0x129],3 // - Scouting level
		PUSH EDX
		PUSH EBX
		JGE SHORT mop_00757332
		LEA ECX,DWORD PTR DS : [ESI + 0x91] // - creatures slots
		MOV EDX,7
	mop_00757313 :
		MOV EBX,DWORD PTR DS : [ECX]
		CMP EBX,-1
		JE SHORT mop_0075732C
		CMP BYTE PTR DS : [EBX + isRogue],0 // - check ability
		MOV EAX,3
		JE SHORT mop_0075732C
	mop_00757328 :
		POP EBX
		POP EDX
		POP ESI
		RETN
	mop_0075732C :
		ADD ECX,4
		DEC EDX
		JNZ SHORT mop_00757313
	mop_00757332 :
		MOV EAX,DWORD PTR DS : [ESI + 0x129]
		JMP SHORT mop_00757328

	}
}
/*
DWORD temp_rogue;
__declspec(naked) void f_004E6050_Hook()
{
__asm
{
label_004E6050:
PUSH ESI
MOV ESI, ECX
CMP DWORD PTR DS : [ESI + 0x129], 3
JGE SHORT label_004E6075

//PUSH 0x8F
jmp label_majaczek1
label_majaczek3:

LEA ECX, [ESI + 0x91]
mov eax, 0x0044AA90
Call eax
TEST EAX, EAX
MOV EAX, 3
JNZ SHORT label_004E607B
label_004E6075:
MOV EAX, DWORD PTR DS : [ESI + 0x129]
label_004E607B:
POP ESI
RETN

label_majaczek1:
pusha
label_majaczek2:

mov temp_rogue, 0x08f

popa
push temp_rogue;
jmp label_majaczek3
}
}
*/

//============================================
int __stdcall GetFireShield(int creature, int* slot)
{
	/*
	char t[256];
	sprintf(t,"%i %x %f", creature, slot, respawn_table[creature]);
	*/

	//MessageBoxA(0,(LPCSTR)t,(LPCSTR)t,0);
	if (fire_shield_table[creature]>0.01)
	{
		*(int*)0x442E69 = creature*4 + (int)fire_shield_table;
		return 1;
	}
	else
	{
		return 0;
	}
}

__declspec(naked) void f442E61_Hook()
{
__asm
{
	push ecx
	push [ecx+0x34]
	call GetFireShield
	cmp eax, 0
	je j442e6e
	mov eax, 0x442e67
	jmp eax

	j442e6e:
	mov eax, 0x442e6e
	jmp eax

}
}


__declspec(naked) void f4225D6_Hook()
{
__asm
{
	push esi
	push [esi+0x34]
	call GetFireShield
	cmp eax, 0
	je j4225dc
	mov eax, 0x4225e6
	jmp eax
j4225dc:
	mov eax, 0x4225dc
	jmp eax

}
}

__declspec(naked) void Hill_Fort_47AA50_Hook()
{
	__asm
	{
		cmp ecx, -1
		je empty_slot

		lea eax, ds: [ecx * 8]
		push 0x0047AA57
		ret

		empty_slot:
		mov eax, 0
		ret
	}
}

__declspec(naked) void Dlg_RecruitMonster_Hook_5502EE() {
	__asm {
		mov ecx, 6
		push eax
		push edx
		z_loop:
		dec ecx
		// mov eax, 0
		mov eax, [ebx + 0x50]

		imul eax, 0x74
		lea eax, [new_monsters+ eax]
		lea eax, [eax+0x20+ecx*4]
		cmp [eax], 0
		jnz z_fin

		cmp ecx, -1
		jne z_loop
		z_fin:
		pop edx
		pop eax

		mov [ebx + 0x88], ecx
		// mov ecx, dword ptr ds : [ebx + 0x88]
		push 0x005502F4
		ret
	}
}


int PH_MonA = 0x02832714; int amtype; int amflags;
int PH_MonD = 0x028601FC; int dmtype; int dmflags; int dmpos;
int _PersonalHate = 0x00766E14; int hate_percent = 0;
constexpr int hatecluster = sizeof(int[MONSTERS_AMOUNT]);
const int hateoffset = (int)(char*)&(PersonalHate[0][0]);
__declspec(naked) void Hate_Hook_766DFF() {

	__asm {
		call _PersonalHate
		push eax
		pusha
	}
	hate_percent = 0;

	amtype  = *(int*)(*(int*)PH_MonA + 0x34);
	dmtype  = *(int*)(*(int*)PH_MonD + 0x34);
	dmpos   = *(int*)(*(int*)PH_MonD + 0x38);
	amflags = *(int*)(*(int*)PH_MonA + 0x74 + 0x10);
	dmflags = *(int*)(*(int*)PH_MonD + 0x74 + 0x10);

	if (isDragonSlayer_Table[amtype] && (dmflags & 0x80000000)) {
		hate_percent += 100;
		// CALL_5(char,__thiscall,BattleMgr_Play_MagicAnimation_onGEX,*(int*)0x699420,0x5B,dmpos,0x64,0);
	}
	if (isDragonResistant[dmtype] && (amflags & 0x80000000)) {
		hate_percent -= 50;
	}
	hate_percent += PersonalHate[amtype][dmtype];

	__asm{
		popa
		pop eax
		add eax, hate_percent
		push 0x00766E04
		ret
	}
}
/*
__declspec(naked) void Hate_Hook_766DFF() {

	__asm {
		call _PersonalHate
		pusha
		mov hate_percent, 0

		mov ebx, [PH_MonA]
		lea ebx, [ebx + 0x34]
		movsx edx, byte ptr [ebx]
		cmp edx, 0
		je no_dragons

		mov ebx, [PH_MonD]
		mov ebx, [ebx + 0x74 + 0x10]
		and ebx, 0x80000000
		cmp ebx, 0
		je no_dragons
		add hate_percent, 100
		no_dragons:

		mov ebx, [PH_MonA]
		mov ebx, [ebx + 0x34]
		mov ebx, [ebx]
		imul ebx, hatecluster
		add ebx, hateoffset
		mov edx, [PH_MonD]
		mov edx, [edx + 0x34]
		//mov edx, [edx]
		
		mov eax, [ebx + edx*4]
		add hate_percent, eax

		popa
		add eax, hate_percent
		push 0x00766E04
		ret
	}
}
*/


long Spell_1_mode_exact_expert_ebx;
__declspec(naked) void   Spell_1_mode_exact_expert(void)
{
	__asm {

		mov Spell_1_mode_exact_expert_ebx, ebx
		//mov		ebx, [ebp + 4] // _BattleStack_ *actStack
		mov		ebx, [esi + 0x34] // stack type
		// mov		edx, [esi + 0x4c] // stack count
		/*
		mul		[CreatureSpellPowerMultiplier	+ 4 * ebx]
		div		dword ptr [CreatureSpellPowerDivider + 4 * ebx]
		add		eax, [CreatureSpellPowerAdder + 4 * ebx]
		mov		edx, eax
		*/

		// mov		edx, eax
		mov		eax, [ebp + 8] //target

		push[esi + 0x4c] // edx // power
		push    3 // skill
		push    0FFFFFFFFh
		push    1
		push	eax // target

		mov		bl, [Spells_Table + ebx]
		movzx	ebx, bl
		push    ebx //spell


		mov		ebx, Spell_1_mode_exact_expert_ebx

		push     0x004483D2
		ret
	}
}

long Spell_1_mode_exact_ebx;
__declspec(naked) void   Spell_1_mode_exact(void)
{
	__asm {

		mov Spell_1_mode_exact_ebx, ebx
		//mov		ebx, [ebp + 4] // _BattleStack_ *actStack
		mov		ebx, [esi + 0x34] // stack type
		// mov		edx, [esi + 0x4c] // stack count
		/*
		mul		[CreatureSpellPowerMultiplier	+ 4 * ebx]
		div		dword ptr [CreatureSpellPowerDivider + 4 * ebx]
		add		eax, [CreatureSpellPowerAdder + 4 * ebx]
		mov		edx, eax
		*/

		// mov		edx, eax
		mov		eax, [ebp + 8] //target

		push	[esi + 0x4c] // edx // power
		push    2 // skill
		push    0FFFFFFFFh
		push    1
		push	eax // target

		mov		bl, [Spells_Table + ebx]
		movzx	ebx, bl
		push    ebx //spell


		mov		ebx, Spell_1_mode_exact_ebx

		push     0x004483D2
		ret
	}
}


long Spell_1_mode_exact_twice_ebx;
__declspec(naked) void   Spell_1_mode_exact_twice(void)
{
	__asm {

		mov Spell_1_mode_exact_twice_ebx, ebx
		//mov		ebx, [ebp + 4] // _BattleStack_ *actStack
		mov		ebx, [esi + 0x34] // stack type
		// mov		edx, [esi + 0x4c] // stack count
		/*
		mul		[CreatureSpellPowerMultiplier	+ 4 * ebx]
		div		dword ptr [CreatureSpellPowerDivider + 4 * ebx]
		add		eax, [CreatureSpellPowerAdder + 4 * ebx]
		mov		edx, eax
		*/

		// mov		edx, eax
		mov		eax, [ebp + 8] //target

		push[esi + 0x4c] // edx // power
		push    2 // skill
		push    0FFFFFFFFh
		push    1
		push	eax // target

		mov		bl, [Spells_Table + ebx]
		movzx	ebx, bl
		push    ebx //spell


		mov ecx, dword ptr ds : [0x00699420] // Battle Manager
		call	BattleMgr_CastSpell


		push 0x7f
		call Sleep

		mov		ebx, [esi + 0x34] // stack type
		mov		eax, [ebp + 8] //target

		push[esi + 0x4c] // edx // power
		push    2 // skill
		push    0FFFFFFFFh
		push    1
		push	eax // target

		mov		bl, [Spells_Table + ebx]
		movzx	ebx, bl
		push    ebx //spell


		mov		ebx, Spell_1_mode_exact_twice_ebx

		push     0x004483D2
		ret
	}
}

long default_elementals[4] = {0x72,0x71,0x73,0x70};
long Spell_1_mode_elemental_ebx;
__declspec(naked) void   Spell_1_mode_elemental(void)
{
	__asm {

		mov Spell_1_mode_elemental_ebx, ebx
		mov		ebx, [esi + 0x34] // stack type

		mov		eax, [ebp + 8] //target

		/*
		push[esi + 0x4c] // edx // power
		push    2 // skill
		push    0FFFFFFFFh
		push    1
		push	eax // target
		*/

		push 3
		mov eax, [esi + 0x4c]
		push edx
		cdq
		mul    CreatureSpellPowerMultiplier[ebx * 4]
		div    dword ptr ds:CreatureSpellPowerDivider[ebx * 4]
		add    eax, CreatureSpellPowerAdder[ebx * 4]
		pop edx
		push eax

		call homm3_rand
		and	eax, 3
		
		//push 0
		mov ebx, [eax*4 + default_elementals]
		push ebx

		lea		ebx , [0x42+eax]
		push ebx

		mov ecx, dword ptr ds : [0x00699420] // Battle Manager
		call CombatMan_SummonMonster


		mov		ebx, Spell_1_mode_elemental_ebx

		push     0x004483DD
		ret
	}
}

__declspec(naked) void   Spell_1_mode_Heavenly_Inspiration(void)
{
	__asm {

		mov		eax, [ebp + 8] //target
		pusha

		//check if proper hex
		test eax, eax
		jl z_exit
		cmp eax, 0xBB
		jge z_exit

		cdq
		imul eax, 112
		//add eax, 0x1c4

		mov ebx, DS: [699420h]//  COMBAT_MANAGER; ebx now stores CombatManager
		lea ecx, [eax + ebx + 0x1c4]
		call BattleHex_GetMonAtBFPosition
		mov ebx, eax // ebx is now target H3CombatMonster pointer

		mov dword ptr[ebx + 0x454 ], 5000
		add dword ptr[ebx + 0x74 + 0x50], 5


		mov	ecx, [esi + 0x34] // caster stack type
		mov eax, [esi + 0x4c] // caster stack count

		push edx
		cdq
		mul    dword ptr [ebx + 0x6c]
		mul    CreatureSpellPowerMultiplier[ecx * 4]
		div    dword ptr CreatureSpellPowerDivider[ecx * 4]
		mov edx, [ecx * 4 + CreatureSpellPowerAdder]
		add eax, edx
		xor edx, edx
		mov ecx, 100
		div ecx
		pop edx
		add		[ebx + 0x6c], eax
		add		[ebx + 0x4c + 0x74], eax

		popa

		pusha
		push 0
		push 0x64
		push eax
		push 0x5F
		mov ecx, DS : [699420h]//  COMBAT_MANAGER; ebx now stores CombatManager
		call BattleMgr_Play_MagicAnimation_onGEX

		z_exit :
		popa

		push     0x004483DD
		ret
	}
}


__declspec(naked) void   Spell_1_mode_enchant(void)
{
	__asm {

		mov		eax, [ebp + 8] //target
		pusha

		//check if proper hex
		test eax, eax
		jl z_exit
		cmp eax, 0xBB
		jge z_exit

		cdq
		imul eax, 112
		//add eax, 0x1c4

		mov ebx, DS:[699420h]//  COMBAT_MANAGER; ebx now stores CombatManager
		lea ecx, [eax + ebx + 0x1c4]
		call BattleHex_GetMonAtBFPosition
		mov ebx, eax // ebx is now target H3CombatMonster pointer

		mov eax, [ebx + 0x34] // eax is now target type
		
		cdq
		imul eax, MON_Sizeof
		//add eax, 0x68
		lea edi, new_monsters
		cmp dword ptr [edi + eax + 0x68], 0
		je skip_caster

		add dword ptr[ebx + 0xDC], 2 // increase spell target's spell charges

		skip_caster:

		cmp dword ptr[edi + eax + 0x64], 0
		je skip_shooter

		add dword ptr[ebx + 0xD8], 5 // increase spell target's shoot charges

		skip_shooter:


		mov	ecx, [esi + 0x34] // caster stack type
		mov eax, [esi + 0x4c] // caster stack count

		push edx
		cdq
		mul    CreatureSpellPowerMultiplier[ecx * 4]
		div    dword ptr CreatureSpellPowerDivider[ecx * 4]
		mov edx, [ecx*4 + CreatureSpellPowerAdder]
		pop edx
		//sar [ebx + 0x58], 1 // heal half of HP
		sub [ebx + 0x58], eax
		cmp [ebx + 0x58], 0

		jge skip_overhealed
		mov dword ptr [ebx + 0x58], 0
		skip_overhealed:

		popa

		pusha
		push 0
		push 0x64
		push eax
		push 0x5C
		mov ecx, DS : [699420h]//  COMBAT_MANAGER; ebx now stores CombatManager
		call BattleMgr_Play_MagicAnimation_onGEX

		push 0x7f
		call Sleep
		popa

			pusha
			push 0
			push 0x64
			push eax
			push 0x5C
			mov ecx, DS : [699420h]//  COMBAT_MANAGER; ebx now stores CombatManager
			call BattleMgr_Play_MagicAnimation_onGEX

			z_exit :
			popa

		push     0x004483DD
		ret
	}
}


__declspec(naked) void   Spell_1_mode_enchant_minor(void)
{
	__asm {

		mov		eax, [ebp + 8] //target
		pusha

		//check if proper hex
		test eax, eax
		jl z_exit
		cmp eax, 0xBB
		jge z_exit

		cdq
		imul eax, 112
		//add eax, 0x1c4

		mov ebx, DS: [699420h]//  COMBAT_MANAGER; ebx now stores CombatManager
		lea ecx, [eax + ebx + 0x1c4]
		call BattleHex_GetMonAtBFPosition
		mov ebx, eax // ebx is now target H3CombatMonster pointer

		mov eax, [ebx + 0x34] // eax is now target type

		cdq
		imul eax, MON_Sizeof
		//add eax, 0x68
		lea edi, new_monsters
		cmp dword ptr[edi + eax + 0x68], 0
		je skip_caster

		add dword ptr[ebx + 0xDC], 1 // increase spell target's spell charges

		skip_caster:

		cmp dword ptr[edi + eax + 0x64], 0
			je skip_shooter

			add dword ptr[ebx + 0xD8], 3 // increase spell target's shoot charges

			skip_shooter :


			mov	ecx, [esi + 0x34] // caster stack type
			mov eax, [esi + 0x4c] // caster stack count

			push edx
			cdq
			mul    CreatureSpellPowerMultiplier[ecx * 4]
			div    dword ptr CreatureSpellPowerDivider[ecx * 4]
			mov edx, [ecx * 4 + CreatureSpellPowerAdder]
			pop edx
			//sar [ebx + 0x58], 1 // heal half of HP
			sub[ebx + 0x58], eax
			cmp[ebx + 0x58], 0

			jge skip_overhealed
			mov dword ptr[ebx + 0x58], 0
			skip_overhealed:

		popa

			pusha
			push 0
			push 0x64
			push eax
			push 0x5C
			mov ecx, DS : [699420h]//  COMBAT_MANAGER; ebx now stores CombatManager
			call BattleMgr_Play_MagicAnimation_onGEX

			z_exit :
			popa

			push     0x004483DD
			ret
	}
}



__declspec(naked) void   Spell_1_mode_enchant_major(void)
{
	__asm {

		mov		eax, [ebp + 8] //target
		pusha

		//check if proper hex
		test eax, eax
		jl z_exit
		cmp eax, 0xBB
		jge z_exit

		cdq
		imul eax, 112
		//add eax, 0x1c4

		mov ebx, DS: [699420h]//  COMBAT_MANAGER; ebx now stores CombatManager
		lea ecx, [eax + ebx + 0x1c4]
		call BattleHex_GetMonAtBFPosition
		mov ebx, eax // ebx is now target H3CombatMonster pointer

		mov eax, [ebx + 0x34] // eax is now target type

		cdq
		imul eax, MON_Sizeof
		//add eax, 0x68
		lea edi, new_monsters
		cmp dword ptr[edi + eax + 0x68], 0
		je skip_caster

		add dword ptr[ebx + 0xDC], 3 // increase spell target's spell charges

		skip_caster:

		cmp dword ptr[edi + eax + 0x64], 0
			je skip_shooter

			add dword ptr[ebx + 0xD8], 9 // increase spell target's shoot charges

			skip_shooter :


			mov	ecx, [esi + 0x34] // caster stack type
			mov eax, [esi + 0x4c] // caster stack count

			push edx
			cdq
			mul    CreatureSpellPowerMultiplier[ecx * 4]
			div    dword ptr CreatureSpellPowerDivider[ecx * 4]
			mov edx, [ecx * 4 + CreatureSpellPowerAdder]
			pop edx
			//sar [ebx + 0x58], 1 // heal half of HP
			sub[ebx + 0x58], eax
			cmp[ebx + 0x58], 0

			jge skip_overhealed
			mov dword ptr[ebx + 0x58], 0
			skip_overhealed:

		popa

			pusha
			push 0
			push 0x64
			push eax
			push 0x5C
			mov ecx, DS : [699420h]//  COMBAT_MANAGER; ebx now stores CombatManager
			call BattleMgr_Play_MagicAnimation_onGEX

			push 0x7f
			call Sleep
			popa

			pusha
			push 0
			push 0x64
			push eax
			push 0x5C
			mov ecx, DS : [699420h]//  COMBAT_MANAGER; ebx now stores CombatManager
			call BattleMgr_Play_MagicAnimation_onGEX

			push 0x7f
			call Sleep
			popa

			pusha
			push 0
			push 0x64
			push eax
			push 0x5C
			mov ecx, DS : [699420h]//  COMBAT_MANAGER; ebx now stores CombatManager
			call BattleMgr_Play_MagicAnimation_onGEX


			z_exit :
			popa

			push     0x004483DD
			ret
	}
}


long Spell_1_mode_rush_ebx;
long Spell_1_mode_rush_ecx;
long Spell_1_mode_rush_edx;
__declspec(naked) void   Spell_1_mode_rush(void)
{
	__asm {

		mov		Spell_1_mode_rush_ebx, ebx
		mov		Spell_1_mode_rush_ecx, ecx
		mov		Spell_1_mode_rush_edx, edx
		mov		ebx, [esi + 0x34] // stack type

		mov		eax, [ebp + 8] //target


		test eax, eax
		jl z_exit
		cmp eax, 0xBB
		jge z_exit

		pusha

		cdq
		imul eax, 112

		push ebx
		mov ebx, DS: [699420h]//  COMBAT_MANAGER; ebx now stores CombatManager
		lea ecx, [eax + ebx + 0x1c4]
		call BattleHex_GetMonAtBFPosition
		sub eax, ebx
		sub eax, 0x54cc
		cdq
		idiv sizeof_H3CombatMonster

		pop ebx
		mov ecx, eax

		//mul    dword ptr DS : [CreatureSpellPowerMultiplier + ebx * 4]
		//div    dword ptr DS : [CreatureSpellPowerDivider + ebx * 4]
		
		mov    eax, [CreatureSpellPowerAdder + ebx * 4]
		cmp	   eax, 0
		jle	   z_exit
		cmp	   eax, 63
		jge	   z_exit
		lea		edx, addditional_moves 
		add	   DS:[edx + ecx], al
		cmp	   byte ptr DS : [edx + ecx] , 63
		jle	   z_okay
		mov	   byte ptr DS : [edx + ecx] , 63

		z_okay:
		
		popa

		pusha
		push 0
		push 0x64
		push eax
		push 0x62
		mov ecx, DS : [699420h]//  COMBAT_MANAGER; ebx now stores CombatManager
		call BattleMgr_Play_MagicAnimation_onGEX
		popa


		z_exit:
		mov		ebx, Spell_1_mode_rush_ebx
		mov		ecx, Spell_1_mode_rush_ecx
		mov		edx, Spell_1_mode_rush_edx

		push     0x004483DD
		ret
	}
}

__declspec(naked) void   Spell_1_mode_1_extended(void) {

	__asm {
		cmp byte ptr[eax + spell_2_table], 9
		je z_necro

		cmp byte ptr[eax + spell_2_table], 10
		je z_constructs


		cmp byte ptr[eax + spell_2_table], 11
		je z_gating


		//original mode 1
		push 0x0044826A
		ret 

		z_necro:
		mov eax, dword ptr ds : [esi + 0x288]
			test eax, eax
			je necro_h3era_448283
			mov ecx, dword ptr ds : [esi + 0xF4]
			mov eax, 0x1
			sub eax, ecx
			jmp necro_h3era_448289
			necro_h3era_448283:
			mov eax, dword ptr ds : [esi + 0xF4]
			
		necro_h3era_448289:
			mov edx, dword ptr ss : [ebp + 0x8]
			mov ecx, dword ptr ds : [0x00699420] // Battle Manager
			push 0x1
			push edx
			push eax
			call get_raise_undead
			push 0x0044829B
			ret

		z_constructs:

			mov eax, dword ptr ds : [esi + 0x288]
				test eax, eax
				je constructs_h3era_448283
				mov ecx, dword ptr ds : [esi + 0xF4]
				mov eax, 0x1
				sub eax, ecx
				jmp constructs_h3era_448289
				constructs_h3era_448283 :
			mov eax, dword ptr ds : [esi + 0xF4]

				constructs_h3era_448289 :
				mov edx, dword ptr ss : [ebp + 0x8]
				mov ecx, dword ptr ds : [0x00699420] // Battle Manager
				push 0x1
				push edx
				push eax
				call get_repair_constructs
				push 0x0044829B
				ret



			z_gating:
			mov eax, dword ptr ds : [esi + 0x288]
				test eax, eax
				je gating_h3era_448283
				mov ecx, dword ptr ds : [esi + 0xF4]
				mov eax, 0x1
				sub eax, ecx
				jmp gating_h3era_448289
				gating_h3era_448283 :
			mov eax, dword ptr ds : [esi + 0xF4]

				gating_h3era_448289 :
				mov edx, dword ptr ss : [ebp + 0x8]
				mov ecx, dword ptr ds : [0x00699420] // Battle Manager
				push 0x1
				push edx
				push eax
				call get_can_gate
				push 0x0044829B
				ret
	} 
}


long Spell_1_mode_Dragon_Will_ebx;
long Spell_1_mode_Dragon_Will_ecx;
long Spell_1_mode_Dragon_Will_edx;
__declspec(naked) void   Spell_1_mode_Dragon_Will(void)
{
	__asm {

		mov		Spell_1_mode_Dragon_Will_ebx, ebx
		mov		Spell_1_mode_Dragon_Will_ecx, ecx
		mov		Spell_1_mode_Dragon_Will_edx, edx
		mov		ebx, [esi + 0x34] // stack type

		mov		eax, [ebp + 8] //target


		test eax, eax
		jl z_exit
		cmp eax, 0xBB
		jge z_exit

		pusha

		cdq
		imul eax, 112

		push ebx
		mov ebx, DS: [699420h]//  COMBAT_MANAGER; ebx now stores CombatManager
		lea ecx, [eax + ebx + 0x1c4]
		call BattleHex_GetMonAtBFPosition
		//sub eax, ebx
		//sub eax, 0x54cc
		//cdq
		//idiv sizeof_H3CombatMonster

		pop ebx
		mov ecx, eax

		mov		eax, [esi + 0x4c] // caster stack count
		mul     [CreatureSpellPowerMultiplier + ebx * 4]
		div     dword ptr[CreatureSpellPowerDivider + ebx * 4]
		add		eax, [CreatureSpellPowerAdder + ebx * 4]
		add		eax, 100
		mov		edi, 100

		push eax
		imul dword ptr DS : [ecx+ 0x6c]
		idiv edi
		mov [ecx + 0x6c], eax
		pop eax

		push eax
		imul dword ptr DS : [ecx + 0x74 + 0x4c]
		idiv edi
		mov[ecx + 0x74 + 0x4c], eax
		pop eax

		push eax
		imul dword ptr DS : [ecx + 0x74 + 0x50]
		idiv edi
		mov[ecx + 0x74 + 0x50], eax
		pop eax

		push eax
		imul dword ptr DS : [ecx + 0x74 + 0x5c]
		idiv edi
		mov[ecx + 0x74 + 0x5c], eax
		pop eax

		push eax
		imul dword ptr DS : [ecx + 0x74 + 0x60]
		idiv edi
		mov[ecx + 0x74 + 0x60], eax
		pop eax

		or dword ptr DS : [ecx + 0x74 + 0x10], 0x80000000

		z_okay :

		popa

		pusha
		push 0
		push 0x64
		push eax
		push 0x5D
		mov ecx, DS : [699420h]//  COMBAT_MANAGER; ebx now stores CombatManager
		call BattleMgr_Play_MagicAnimation_onGEX
		popa


		z_exit :
		mov		ebx, Spell_1_mode_Dragon_Will_ebx
		mov		ecx, Spell_1_mode_Dragon_Will_ecx
		mov		edx, Spell_1_mode_Dragon_Will_edx

			push     0x004483DD
			ret
	}
}


__declspec(naked) void   Spell_1_mode_overclock(void)
{
	__asm {

		mov		eax, [ebp + 8] //target
		push	eax
		pusha

		//check if proper hex
		test eax, eax
		jl z_exit
		cmp eax, 0xBB
		jge z_exit

		cdq
		imul eax, 112
		//add eax, 0x1c4

		mov ebx, DS: [699420h]//  COMBAT_MANAGER; ebx now stores CombatManager
		lea ecx, [eax + ebx + 0x1c4]
		call BattleHex_GetMonAtBFPosition
		mov ebx, eax // ebx is now target H3CombatMonster pointer

			stage_0:
		mov eax, [ebx + 0x84] // eax is now target flags
		and eax, 0x00008000 // attack twice
		jnz stage_1
		mov eax, [ebx + 0x84] // eax is now target flags
		or  eax, 0x00008000 // attack twice
		mov [ebx + 0x84], eax
		jmp z_casted

			stage_1:
		mov eax, [ebx + 0x84] // eax is now target flags
		and eax, 0x00000004 // shooter
		jnz stage_1_shooter
		jmp stage_1_melee
			stage_1_shooter:
		mov eax, [ebx + 0xf8] // eax is now target Index
		cmp[ebx + 0xF4], 0
		je stage_1_shooter_left
		add eax, 21
			stage_1_shooter_left:
		mov cl, RangeRetaliation_Table[eax+ MONSTERS_AMOUNT]
		and cl, 0xff
		jnz stage_2
		mov RangeRetaliation_Table[eax+ MONSTERS_AMOUNT], 1
		jmp z_casted
			stage_1_melee:
		
		mov cl, [eax + isPassive_Table]
		cmp cl, 0
		jnz stage_2

		mov eax, [ebx + 0xf8] // eax is now target index
			cmp[ebx + 0xF4], 0
			je stage_1_melee_left
			add eax, 21
			stage_1_melee_left:
		mov cl, PreventiveCounterstrikeTable[eax+ MONSTERS_AMOUNT]
		and cl, 0xff
		jnz stage_2
		mov PreventiveCounterstrikeTable[eax+ MONSTERS_AMOUNT], 1
		jmp z_casted
	
			stage_2:

		mov ecx, [ebx + 0x84]
		and ecx, 0x40 // siege weapon
		cmp ecx, 0
		jnz z_casted

		mov eax, [ebx + 0xC4] // eax is now target speed
		cmp eax, 0
		je z_exit
		add eax, 6
		mov [ebx + 0xC4], eax


			z_casted:
		popa
		pop eax

			pusha
			push 0
			push 0xC8
			push eax
			push 0x61
			mov ecx, DS : [699420h]//  COMBAT_MANAGER; ebx now stores CombatManager
			call BattleMgr_Play_MagicAnimation_onGEX

			z_exit :
		popa

			push     0x004483DD
			ret
	}
}


__declspec(naked) void   Spell_1_mode_repair_and_overclock(void)
{
	__asm {

		pushad
		mov eax, dword ptr ds : [esi + 0x288]
		test eax, eax
		je constructs_h3era_448283
		mov ecx, dword ptr ds : [esi + 0xF4]
		mov eax, 0x1
		sub eax, ecx
		jmp constructs_h3era_448289
		constructs_h3era_448283 :
		mov eax, dword ptr ds : [esi + 0xF4]

			constructs_h3era_448289 :
			mov edx, dword ptr ss : [ebp + 0x8]
			mov ecx, dword ptr ds : [0x00699420] // Battle Manager
			push 0x1
			push edx
			push eax
			call get_repair_constructs
			
			test eax, eax
			je skip_repair
			/*
			push eax
			mov ecx, esi
			call Battle_Stack_Get_Resurrect_Count
			cmp eax, 0
			je skip_repair
			*/
			mov ecx, eax
			mov eax, [ecx + 0x4c]
			mov ebx, [ecx + 0x34]

			add eax, [CreatureSpellPowerAdder + ebx*4]

			lea     eax, [eax + eax * 4]
			lea     eax, [eax + eax * 4]
			shl     eax, 2

			imul [CreatureSpellPowerMultiplier + ebx * 4]
			cdq
			idiv [CreatureSpellPowerDivider + ebx*4]

			push 0 // a4
			push eax // power
			push ecx // monpo
			mov ecx, dword ptr ds : [0x00699420] // Battle Manager
			call BattleMgr_ResurectArchangel

		skip_repair:

		popad
		jmp Spell_1_mode_overclock
	}
}


unsigned long spell_1_jumptable[256] = {
	(unsigned long) Spell_1_mode_1_extended
	/*0x0044826A*/, 0x00448351, 0x00448308, 0x004483C4, 0x00448394,
	0x004483B4, 0x00448384, 0x004483A4, 0x0044835E, 0x004483DD,

	(unsigned long) Spell_1_mode_exact,
	(unsigned long) Spell_1_mode_elemental,
	(unsigned long) Spell_1_mode_enchant,
	(unsigned long) Spell_1_mode_enchant_minor,
	(unsigned long) Spell_1_mode_enchant_major,
	(unsigned long) Spell_1_mode_Heavenly_Inspiration,
	(unsigned long) Spell_1_mode_exact_twice,
	(unsigned long) Spell_1_mode_rush,
	(unsigned long) Spell_1_mode_Dragon_Will,
	(unsigned long) Spell_1_mode_exact_expert,
	(unsigned long) Spell_1_mode_overclock,
	(unsigned long) Spell_1_mode_repair_and_overclock,

};

__declspec(naked) void   Spell_2_raise_undead(void)
{
	__asm {
		mov     eax, [esi + 0DCh]
		test    eax, eax // Logical Compare
		jle     loc_004476F2 // default
		mov     eax, [esi + 288h]
		test    eax, eax // Logical Compare
		jz      loc_004474B5 // Jump if Zero(ZF = 1)
		mov     eax, [esi + 0F4h]
		mov     edi, 1
		sub     edi, eax // Integer Subtraction
		
		//jmp     short loc_004474BB // Jump
		jmp loc_004474BB

		loc_004474B5:
		mov edi, dword ptr ds : [esi + 0xF4]

		loc_004474BB:
		mov ecx, dword ptr ds : [0x00699420] // Battle Manager
		push 0x0
		push edi
		
		//call DS:0x41FA10 //BattleMgr_CanCast  
		call BattleMgr_CanCast

		test al, al
		je loc_004476F2

		//mov ecx, dword ptr ds : [<o_BattleMgr>]
		mov ecx, dword ptr ds : [0x00699420] // Battle Manager
		
		push 0x1
		push ebx
		push edi
		call get_raise_undead
		test eax, eax
		je loc_004476F2
		push eax
		mov ecx, esi
		call Battle_Stack_Get_Resurrect_Count // h3era hd.447050 // Battle_Stack_Get_Resurrect_Count
		test eax, eax
		pop edi
		pop esi
		setg al
		pop ebx
		pop ebp
		ret 0x4


		loc_004476F2: // h3era hd.spell2_default
		push 0x004476F2
		ret

	}
}


__declspec(naked) void   Spell_2_repair_constructs(void)
{
	__asm {
		mov     eax, [esi + 0DCh]
		test    eax, eax // Logical Compare
		jle     loc_004476F2 // default
		mov     eax, [esi + 288h]
		test    eax, eax // Logical Compare
		jz      loc_004474B5 // Jump if Zero(ZF = 1)
		mov     eax, [esi + 0F4h]
		mov     edi, 1
		sub     edi, eax // Integer Subtraction

		//jmp     short loc_004474BB // Jump
		jmp loc_004474BB

		loc_004474B5 :
		mov edi, dword ptr ds : [esi + 0xF4]

			loc_004474BB :
			mov ecx, dword ptr ds : [0x00699420] // Battle Manager
			push 0x0
			push edi

			//call DS:0x41FA10 //BattleMgr_CanCast  
			call BattleMgr_CanCast

			test al, al
			je loc_004476F2

			mov ecx, [esi + 0x34]
			mov cl, [ecx + spell_1_table]
			cmp cl, 21
			jne do_not_overclock
			mov ecx, dword ptr ds : [0x00699420] // Battle Manager
			imul ebx, 0x70
			lea ecx, [ecx + ebx + 0x1c4]
			call BattleHex_GetMonAtBFPosition
			mov ecx, [eax + 0xf4]
			mov ebx, [esi + 0xF4]
			cmp ebx, ecx
			jne loc_004476F2
			mov ebx, [eax + 0x74 + 0x10]
			test ebx, 0x40
			jnz skip_resurrect_count
			mov ebx, [eax + 0x34]
			movsx eax, [ebx + isConstruct_Table]
			jmp skip_resurrect_count
			do_not_overclock:

			//mov ecx, dword ptr ds : [<o_BattleMgr>]
			mov ecx, dword ptr ds : [0x00699420] // Battle Manager

			push 0x1
			push ebx
			push edi
			call get_repair_constructs
			test eax, eax
			je loc_004476F2
			push eax
			mov ecx, esi
			call Battle_Stack_Get_Resurrect_Count // h3era hd.447050 // Battle_Stack_Get_Resurrect_Count
			skip_resurrect_count:
			test eax, eax
			pop edi
			pop esi
			setg al
			pop ebx
			pop ebp
			ret 0x4


			loc_004476F2: // h3era hd.spell2_default
		push 0x004476F2
			ret

	}
}



__declspec(naked) void   Spell_2_gate_infernals(void)
{
	__asm {
		mov     eax, [esi + 0DCh]
		test    eax, eax // Logical Compare
		jle     loc_004476F2 // default
		mov     eax, [esi + 288h]
		test    eax, eax // Logical Compare
		jz      loc_004474B5 // Jump if Zero(ZF = 1)
		mov     eax, [esi + 0F4h]
		mov     edi, 1
		sub     edi, eax // Integer Subtraction

		//jmp     short loc_004474BB // Jump
		jmp loc_004474BB

		loc_004474B5 :
		mov edi, dword ptr ds : [esi + 0xF4]

			loc_004474BB :
			mov ecx, dword ptr ds : [0x00699420] // Battle Manager
			push 0x0
			push edi

			//call DS:0x41FA10 //BattleMgr_CanCast  
			call BattleMgr_CanCast

			test al, al
			je loc_004476F2

			//mov ecx, dword ptr ds : [<o_BattleMgr>]
			mov ecx, dword ptr ds : [0x00699420] // Battle Manager

			push 0x1
			push ebx
			push edi
			call get_can_gate
			test eax, eax
			je loc_004476F2
			push eax
			mov ecx, esi
			call Battle_Stack_Get_Resurrect_Count // h3era hd.447050 // Battle_Stack_Get_Resurrect_Count
			test eax, eax
			pop edi
			pop esi
			setg al
			pop ebx
			pop ebp
			ret 0x4


			loc_004476F2: // h3era hd.spell2_default
		push 0x004476F2
			ret

	}
}



//long Spell_2_damage_spell_temp_1, Spell_2_damage_spell_temp_2;
__declspec(naked) void   Spell_2_damage_spell(void)
{
	__asm {
		//mov Spell_2_damage_spell_temp_1, eax
		//mov Spell_2_damage_spell_temp_2, ecx
		mov eax, [esi + 0x34]
		cmp byte ptr DS: [eax + spell_1_table], 10
		je Spell_Exact

		push 0x0044756E
		ret
		
		Spell_Exact:
		test edi, edi
		jz Spell_Return

		mov al, [eax + Spells_Table]
		and eax, 0xFF

		/*
		mov ecx, dword ptr ds : [0x00699420] // Battle Manager
		push ?   // noHero
		push 0x1 // a6
		push 0x0 // a5
		push edi //target
		push ?	//side
		push eax // spell
		call BattleMgr_CreatureMagicResist
		*/

		// push eax
		// mov eax, Spell_2_damage_spell_temp_1
		// push edi

		//call DS:0x41FA10 //BattleMgr_CanCast  
		// call BattleMgr_CanCast
		// call BattleMgr_CreatureMagicResist

		// test al, al
		//mov eax, Spell_2_damage_spell_temp_1
		//mov ecx, Spell_2_damage_spell_temp_2
		// je Spell_Return
		// jmp Spell_Return

		cmp [edi + 0x1C4], 0 // antimagic
		jg Spell_Return

		mov ecx, dword ptr ds : [0x00699420] // Battle Manager
			push 0
			push 1
			push [ecx + 0x132c0]
			push [edi + 0x38]
			push 2
			push eax
		call CanCastSpellAtCoord
		test al, al
		je Spell_Return

		Spell_Can_Cast:
		pop edi
		pop esi
		mov eax, 0x1
		pop ebx
		pop ebp
		ret 0x4

		//mov eax, Spell_2_damage_spell_temp_1
		//mov ecx, Spell_2_damage_spell_temp_2


		// push 0x0044756E
		// ret
		
		Spell_Return:
		push 0x004476E9
		ret

		Spell_None:
		push 0x004483DD
		ret
	}

}

__declspec(naked) void  hook_004E6307(void)
{
	__asm {
		lea ecx, dword ptr ds : [edx + 0x1]
		cdq
		
		test ecx, ecx
		jz	not_div_zero

		idiv ecx
	not_div_zero:
		push 0x004E630D
		ret
	}
}


__declspec(naked) void hook_004755E7(void)
{
	__asm {
		cmp byte ptr [ecx+isAimedCaster_Table], 0
		lea eax, dword ptr ds : [ebx + edx * 8 + 0x54CC]
		je NotAimed

		push 0x004755FA
		ret

		NotAimed:
		push 0x00475764
		ret
	}
}

__declspec(naked) void   Spell_2_FriendlyNonDragons(void) {
	__asm {
		test    edi, edi
		jz loc_004476E9
		mov     eax, [esi + 0x0DC]
		test    eax, eax
		jz      loc_004476F2
		
		mov		eax, [esi + 0x0F4]
		cmp		eax, [edi + 0x0F4]
		jnz	z_no

		//push eax
		mov		eax, [edi + 0x84]
		and		eax, 0x80000000 //dragon
		//pop		eax
		jz	z_yes
		jmp z_no
		z_yes:
		push 0x004476DD
		ret
		z_no:
		jmp loc_004476E9

		loc_004476E9:
		push 0x004476E9
		ret

		loc_004476F2:
		push 0x004476F2
		ret
	}
}

__declspec(naked) void   Spell_2_FriendlyConstructs(void) {
	__asm {
		test    edi, edi
		jz loc_004476E9
		mov     eax, [esi + 0x0DC]
		test    eax, eax
		jz      loc_004476F2

		mov		eax, [esi + 0x0F4]
		cmp		eax, [edi + 0x0F4]
		jnz	z_no

		//push eax
		mov		eax, [edi + 0x84]
		and eax, 0x00000040 //Siege Weapon
		//pop		eax
		jnz	z_yes

		mov		eax, [edi + 0x34] // target type
		mov		al, isConstruct_Table[eax]
		and		al, 0xff
		jnz z_yes

		jmp z_no
		z_yes :
		push 0x004476DD
			ret
			z_no :
		jmp loc_004476E9

			loc_004476E9 :
		push 0x004476E9
			ret

			loc_004476F2 :
		push 0x004476F2
			ret
	}
}
unsigned long spell_2_jumptable[256] = {
	0x00447480, 0x00447511, 0x004476A3, 0x004475F6, 0x0044766E,
	0x004475B7, 0x00447635, /*0x0044756E*/ (unsigned long) Spell_2_damage_spell , 0x004476F2,

	(unsigned long) Spell_2_raise_undead,
	(unsigned long) Spell_2_repair_constructs,
	(unsigned long) Spell_2_gate_infernals,
	(unsigned long) Spell_2_FriendlyNonDragons,
	(unsigned long) Spell_2_FriendlyConstructs,
};

typedef H3CombatMonster _BattleStack_;
constexpr int AI_BattleStack_Get_Value = 0x00442B80;
constexpr int AI_AICastInfo_Construct = 0x00436610;
constexpr int AI_AICastInfo_Destruct2 = 0x00436880;
constexpr int BattleStack_CheckCanCastToGex = 0x004473E0;

char AI_Battle_GetStackDeBuffValue(void* _this_, int mon, int* weight, int a4) {
	char result;

	int *v4 = (int*)a4;
	int v6 = *(int*)(a4 + 32);
	int v7 = *(int*)(a4 + 4);
	char thisa[0x430];
	int& v19 = *(int*)(thisa + 0x410);
	int& v20 = *(int*)(thisa + 0x414);
	int& v21 = *(int*)(thisa + 0x418);
	void* a2 = _this_;
	int v5 = *(int*)((int)_this_ + (1-v6)*4 + /*5423*/ 0x54BC);
	int v8 = *(int*)a4;
	v21 = -1;
		v19 = CALL_3(int,__thiscall,AI_BattleStack_Get_Value,mon, v8, v7);
	CALL_4(int, __thiscall, AI_AICastInfo_Construct,thisa, a2, *(int*)(a4 + 32), 1);
	int v23 = 0;

	if (*weight && CALL_2(int,__fastcall, 0x0050C7C0,1, 100) <= 30)
	{
		v23 = -1;
		CALL_1(int, __thiscall,AI_AICastInfo_Destruct2,thisa);
		return 0;
	}


	int v10 = v5;
	int v11 = v5 - 1;
	if (v10 <= 0)
		goto LABEL_21;

	v20 = v11 + 1;
	// _BattleStack_ *o_Mon = (_BattleStack_*)((int)a2 + 1352 * (v11 + 21 * v6) + 21708);
	_BattleStack_* o_Mon = (_BattleStack_*)((int)a2 + 0x548 * (v11 + 21 * (1-v6)) + 0x54CC);

do{

	if ((int)o_Mon == mon)
		goto LABEL_19;
	if (o_Mon->type == 149)
		goto LABEL_19;

	/*
	if(o_Mon->side == ((_BattleStack_*)mon)->side)
		goto LABEL_19;
	*/
	
	if (!CALL_2(char,__thiscall,BattleStack_CheckCanCastToGex,mon, o_Mon->position))
		goto LABEL_19;
	
	if (*weight > 0)
	{
		int v13 = CALL_3(int,__thiscall,AI_BattleStack_Get_Value,o_Mon, *v4, v4[1]);
		if (v19 > v13)
			goto LABEL_19;
	}


	int v15 = a4;
LABEL_16:
	if (v15 > 0 && v15 > *weight)
	{
		*weight = v15;
		v21 = o_Mon->position;
	}
LABEL_19:
	--o_Mon;
	--v20;
}
while (v20);
int v16 = v21;
if (v21 >= 0)
{
	//int *v17 = (int*) a2;
	v23 = -1;
	*(int*)((int)a2 + 0x3c) = 10; // *(int*)((int)a2 + 15) = 10;
	*(int*)((int)a2 + 0x44) = v16;// v17[17] = v16;
	CALL_1(int, __thiscall, AI_AICastInfo_Destruct2, thisa); // AI_AICastInfo_Destruct2(&thisa);
	result = 1;
}
else
{
LABEL_21:
	v23 = -1;
	CALL_1(int, __thiscall, AI_AICastInfo_Destruct2, thisa); // AI_AICastInfo_Destruct2(&thisa);
	result = 0;
}
return result;

}

char AI_Battle_GetStackDeBuffValue_0(void* _this_, int mon, int* weight, int a4) {
	return 0;

	if (*weight && CALL_2(int, __fastcall, 0x0050C7C0, 1, 100) <= 30)
		return 0;

	return 1;
}
//const long AI_Battle_GetStackDeBuffValue_address = (long)AI_Battle_GetStackDeBuffValue;

__declspec(naked) void   Spell_3_debuff(void) {
	__asm {
		mov ecx, [ebp + 0x10]
		mov edx, [ebp + 0xC]
		//jmp cannot
		push ecx
		push edx
		push eax
		push esi
		mov ecx, esi
		call AI_Battle_GetStackDeBuffValue
		add esp, 0x10

		//push 0x004214AF
		//ret

		test al, al
		je cannot

		//push 0x004214CE
		//ret

		mov al, 0x1
		pop esi
		pop ebp
		ret //ret 0xC

		cannot:
		//push 0x004214D5
		//ret
		
		xor al, al
		pop esi
		pop ebp
		ret // ret 0xC
	}
}


char AI_Battle_GetStackCustomBuffValue(void* _this_, int mon, int* weight, int a4) {
	if(!mon) return 0;

	char allowed_type = spell_2_table[*(int*)(mon + 0x34)];
	if (*weight && allowed_type != 11 && CALL_2(int, __fastcall, 0x0050C7C0, 1, 100) <= 20)
	{
		// v23 = -1;
		// CALL_1(int, __thiscall, AI_AICastInfo_Destruct2, thisa);
		return 0;
	}


	char result = 0;
	int v6 = *(int*)(a4 + 32);
	int v5 = *(int*)((int)_this_ + v6 * 4 + /*5423*/ 0x54BC);

	int v11 = v5 - 1;
	int v20 = v5;

	if (v5 <= 0)
		goto LABEL_21;
	
	// _BattleStack_* caster = (_BattleStack_*)*(int*)((int)_this_ + 0x132C8) ;
	int casterindex = *(int*)((int)_this_ + 0x132BC); int casterside = *(int*)((int)_this_ + 0x132B8);
	_BattleStack_* caster = (_BattleStack_*)((int)_this_ + 0x548 * (casterindex + 21 * casterside) + 0x54CC);
	_BattleStack_* o_Mon = (_BattleStack_*)((int)_this_ + 0x548 * (v11 + 21 * v6) + 0x54CC);
	// char allowed_type = spell_2_table[*(int*)(mon + 0x34)];
	int	v21 = -1; int target_stack_value = 0;
	do {
		if (o_Mon->type < 0)
			goto LABEL_19;

		if ((int)o_Mon == mon)
			goto LABEL_19;
		if (o_Mon->type == 149)
			goto LABEL_19;
		switch (allowed_type) {
		case 13: // friendly constructs
			if (isPassive_Table[o_Mon->type]) goto LABEL_19;
			if (!o_Mon->info.siegeWeapon// SIEGE_WEAPON 
				&& !isConstruct_Table[o_Mon->type]
				&& o_Mon->numberAlive) goto LABEL_19;
			break;
		case 12: // friendly non-dragons
			if (o_Mon->info.dragon // DRAGON
				|| !o_Mon->numberAlive)goto LABEL_19;
			break;
		case 11: // Gate Infernals
			if (o_Mon->info.town != 3)		goto LABEL_19;
			if (o_Mon->info.summon)	goto LABEL_19;
			if(caster )
			{
				long CID = caster->type;
				if (o_Mon->baseHP /* - o_Mon->healthLost */ >=
					(caster->numberAlive + CreatureSpellPowerAdder[CID])
					* 100 * CreatureSpellPowerMultiplier[CID] / CreatureSpellPowerDivider[CID])
					goto LABEL_19;
			}
			break;

		case 2: // Friendly Spell
			if (!o_Mon->numberAlive)goto LABEL_19;
			break;

		default:
			goto LABEL_19;
		}
		int tmp_value = o_Mon->numberAlive * o_Mon->info.fightValue;
		if (target_stack_value < tmp_value) {
			target_stack_value = tmp_value;
			v21 = v21 = o_Mon->position;
		}

	LABEL_19:
		--o_Mon;
		--v20;
	} while (v20);

	if (v21 >= 0)
	{
		//int *v17 = (int*) a2;
		//v23 = -1;
		*(int*)((int)_this_ + 0x3c) = 10; // *(int*)((int)a2 + 15) = 10;
		*(int*)((int)_this_ + 0x44) = v21;// v17[17] = v16;
		//CALL_1(int, __thiscall, AI_AICastInfo_Destruct2, thisa); // AI_AICastInfo_Destruct2(&thisa);
		result = 1;

		*(int*)((int)_this_ + 0x132c8) = mon;
	}
	else
	{
	LABEL_21:
		// v23 = -1;
		// CALL_1(int, __thiscall, AI_AICastInfo_Destruct2, thisa); // AI_AICastInfo_Destruct2(&thisa);
		result = 0;
	}
	return result;

}

__declspec(naked) void   Spell_3_custom_buff(void) {
	__asm {
		mov ecx, [ebp + 0x10]
		mov edx, [ebp + 0xC]
		//jmp cannot
		push ecx
		push edx
		push eax
		push esi
		mov ecx, esi
		call AI_Battle_GetStackCustomBuffValue
		add esp, 0x10

		//push 0x004214AF
		//ret

		test al, al
		je cannot

		//push 0x004214CE
		//ret

		mov al, 0x1
		pop esi
		pop ebp
		ret //ret 0xC

		cannot :
		//push 0x004214D5
		//ret

		xor al, al
			pop esi
			pop ebp
			ret // ret 0xC
	}
}

unsigned long spell_3_jumptable[256] = {  
	0x00421484, 0x0042149F, 0x004214BA, 0x004214D5,
	(unsigned long)Spell_3_debuff,
	(unsigned long)Spell_3_custom_buff,
};

long h3era_4E4950 = 0x4E4950;

__declspec(naked) void resistance_dwarf60(void) {
	__asm{
		mov dword ptr ss : [ebp + 0xC], 0x3ecccccd
		test ebx,ebx
		je h3era_44A581
		mov ecx,ebx
		call h3era_4E4950
		fsubr /* st(0), */dword ptr ds : [0x63B6E0]
		fsubr /* st(0), */dword ptr ss : [ebp + 0xC]
		fstp dword ptr ss : [ebp + 0xC] // ,st(0)
		
		h3era_44A581:
		//jmp h3era hd.44A581
		push 0x44A581
		ret
	}

}

__declspec(naked) void resistance_dwarf80(void) {
	__asm {
		mov dword ptr ss : [ebp + 0xC] , 0x3e4ccccd
		test ebx, ebx
		je h3era_44A581
		mov ecx, ebx
		call h3era_4E4950
		fsubr /* st(0), */dword ptr ds : [0x63B6E0]
		fsubr /* st(0), */dword ptr ss : [ebp + 0xC]
		fstp dword ptr ss : [ebp + 0xC] // ,st(0)

		h3era_44A581 :
		//jmp h3era hd.44A581
		push 0x44A581
		ret
	}

}


__declspec(naked) void resistance_dwarf100(void) {
	__asm {
		mov dword ptr ss : [ebp + 0xC] , 0x00000000
		test ebx, ebx
		je h3era_44A581
		mov ecx, ebx
		call h3era_4E4950
		fsubr /* st(0), */dword ptr ds : [0x63B6E0]
		fsubr /* st(0), */dword ptr ss : [ebp + 0xC]
		fstp dword ptr ss : [ebp + 0xC] // ,st(0)

		h3era_44A581 :
		//jmp h3era hd.44A581
		push 0x44A581
		ret
	}

}


__declspec(naked) void resistance_level_1(void) {
	__asm {
		cmp dword ptr ds : [esi + 0x18] , 0x1
		jg h3era_44A4C5
		fld /*st(0),*/ dword ptr ds : [0x63AC64]
		pop edi
		pop esi
		pop ebx
		mov esp, ebp
		pop ebp
		ret 0x8

		h3era_44A4C5:
		push 0x44A4C5
		ret
	}
}


__declspec(naked) void resistance_level_1_2(void) {
	__asm {
		cmp dword ptr ds : [esi + 0x18] , 0x2
		jg h3era_44A4C5
		fld /*st(0),*/ dword ptr ds : [0x63AC64]
		pop edi
		pop esi
		pop ebx
		mov esp, ebp
		pop ebp
		ret 0x8

		h3era_44A4C5:
		push 0x44A4C5
		ret
	}
}


__declspec(naked) void resistance_speed(void) {
	__asm {
		mov eax, dword ptr ss : [ebp - 0x10]
		cmp eax, 10
		je h3era_44A521
		cmp eax, 53
		je h3era_44A521
		cmp eax, 54
		je h3era_44A521
		cmp eax, 72
		jne h3era_44A4C5
		
		h3era_44A521:
		push 0x44A521
		ret

		h3era_44A4C5:
		push 0x44A4C5
		ret
	}
}


__declspec(naked) void resistance_toxic(void) {
	__asm {
		mov eax, dword ptr ss : [ebp - 0x10]
		cmp eax, 71
		je h3era_44A521
		cmp eax, 73
		je h3era_44A521
		cmp eax, 74
		je h3era_44A521
		cmp eax, 80
		jne h3era_44A4C5

		h3era_44A521 :
		push 0x44A521
			ret

			h3era_44A4C5 :
		push 0x44A4C5
			ret
	}
}


__declspec(naked) void resistance_will(void) {
	__asm {
		mov eax, dword ptr ss : [ebp - 0x10]
		cmp eax, 56
		je h3era_44A521
		cmp eax, 59
		je h3era_44A521
		cmp eax, 60
		je h3era_44A521
		cmp eax, 61
		jne h3era_44A4C5

		h3era_44A521 :
		push 0x44A521
			ret

			h3era_44A4C5 :
		push 0x44A4C5
			ret
	}
}


__declspec(naked) void resistance_no_eyes(void) {
	__asm {
		mov eax, dword ptr ss : [ebp - 0x10]
		cmp eax, 62
		je h3era_44A521
		cmp eax, 70
		je h3era_44A521
		cmp eax, 79
		jne h3era_44A4C5

		h3era_44A521 :
		push 0x44A521
			ret

			h3era_44A4C5 :
		push 0x44A4C5
			ret
	}
}


__declspec(naked) void resistance_mass_damage(void) {
	__asm {
		mov eax, dword ptr ss : [ebp - 0x10]
		cmp eax, 24
		je h3era_44A521
		cmp eax, 25
		je h3era_44A521
		cmp eax, 26
		jne h3era_44A4C5

		h3era_44A521 :
		push 0x44A521
			ret

			h3era_44A4C5 :
		push 0x44A4C5
			ret
	}
}


__declspec(naked) void resistance_to_dispel(void) {
	__asm {
		mov eax, dword ptr ss : [ebp - 0x10]
		cmp eax, 35
		je h3era_44A521
		cmp eax, 78
		jne h3era_44A4C5

		h3era_44A521 :
		push 0x44A521
			ret

			h3era_44A4C5 :
		push 0x44A4C5
			ret
	}
}

__declspec(naked) void resistance_to_debuff(void) {
	__asm {
		mov eax, dword ptr ss : [ebp - 0x10]
		cmp eax, 42
		je h3era_44A521
		cmp eax, 45
		je h3era_44A521
		cmp eax, 47
		je h3era_44A521
		cmp eax, 50
		je h3era_44A521
		cmp eax, 52
		je h3era_44A521
		cmp eax, 54
		je h3era_44A521
		cmp eax, 61
		je h3era_44A521
		cmp eax, 62
		je h3era_44A521
		cmp eax, 70
		je h3era_44A521
		cmp eax, 71
		je h3era_44A521
		cmp eax, 72
		je h3era_44A521
		cmp eax, 73
		je h3era_44A521
		cmp eax, 74
		je h3era_44A521
		cmp eax, 75
		je h3era_44A521
		cmp eax, 76
		je h3era_44A521
		cmp eax, 77
		je h3era_44A521
		cmp eax, 78
		jne h3era_44A4C5

		h3era_44A521 :
		push 0x44A521
			ret

			h3era_44A4C5 :
		push 0x44A4C5
			ret
	}
}

unsigned long resistance_jumptable[256] = {
	0x0044a4be, 0x0044a4e5, 0x0044a54c, 0x0044a565,
	0x0044a56f, 0x0044a4ee, 0x0044A50A, 0x0044a530,
	0x0044a4c5,
	(unsigned long)resistance_dwarf60,
	(unsigned long)resistance_dwarf80,
	(unsigned long)resistance_dwarf100,
	(unsigned long)resistance_level_1 ,
	(unsigned long)resistance_level_1_2,
	(unsigned long)resistance_speed,
	(unsigned long)resistance_toxic,
	(unsigned long)resistance_will,
	(unsigned long)resistance_no_eyes,
	(unsigned long)resistance_mass_damage,
	(unsigned long)resistance_to_dispel,
	(unsigned long)resistance_to_debuff

};


__declspec(naked) void vulnerability_125(void) {
	__asm {
		mov eax, ecx
		pop esi
		cdq
		sub eax, edx
		sar eax, 0x2
		neg eax
		mov ecx, eax
		pop ebp
		ret 0x4
	}
}


__declspec(naked) void vulnerability_150(void) {
	__asm {
		mov eax, ecx
		pop esi
		cdq
		sub eax, edx
		sar eax, 0x1
		neg eax
		mov ecx, eax
		pop ebp
		ret 0x4
	}
}


__declspec(naked) void vulnerability_200(void) {
	__asm {
		mov eax, ecx
		pop esi
		cdq
		sub eax, edx
		neg eax
		mov ecx, eax
		pop ebp
		ret 0x4
	}
}


__declspec(naked) void vulnerability_300(void) {
	__asm {
		mov eax, ecx
		pop esi
		cdq
		sub eax, edx
		sal eax, 0x1
		neg eax
		mov ecx, eax
		pop ebp
		ret 0x4
	}
}



__declspec(naked) void vulnerability_400(void) {
	__asm {
		mov eax, ecx
		pop esi
		cdq
		sub eax, edx
		//sal eax, 0x1
		lea eax, [eax*2+eax]
		neg eax
		mov ecx, eax
		pop ebp
		ret 0x4
	}
}

__declspec(naked) void vulnerability_500(void) {
	__asm {
		mov eax, ecx
		pop esi
		cdq
		sub eax, edx
		sal eax, 0x2
		neg eax
		mov ecx, eax
		pop ebp
		ret 0x4
	}
}


__declspec(naked) void vulnerability_600(void) {
	__asm {
		mov eax, ecx
		pop esi
		cdq
		sub eax, edx
		lea eax, [eax * 4 + eax]
		neg eax
		mov ecx, eax
		pop ebp
		ret 0x4
	}
}

__declspec(naked) void vulnerability_700(void) {
	__asm {
		mov eax, ecx
		pop esi
		cdq
		sub eax, edx
		sal eax, 0x1
		lea eax, [eax * 2 + eax]
		neg eax
		mov ecx, eax
		pop ebp
		ret 0x4
	}
}


__declspec(naked) void vulnerability_900(void) {
	__asm {
		mov eax, ecx
		pop esi
		cdq
		sub eax, edx
		sal eax, 0x3
		neg eax
		mov ecx, eax
		pop ebp
		ret 0x4
	}
}



unsigned long vulnerability_jumptable[256] = {
	0x0044b212, 0x0044b200, 0x0044b1a2, 0x0044b1da,
	0x0044b1c3, 0x0044b1e8, 0x0044b220, 0x0044b240,
	0x0044b253,
	(unsigned long)vulnerability_125,
	(unsigned long)vulnerability_150,
	(unsigned long)vulnerability_200,
	(unsigned long)vulnerability_300,
	(unsigned long)vulnerability_400,
	(unsigned long)vulnerability_500,
	(unsigned long)vulnerability_600,
	(unsigned long)vulnerability_700,
	(unsigned long)vulnerability_900
};

_LHF_(z_Hook_after_melee) {
	begin:


	long creature = *(int*)(c->edi + 0x34);// c->eax; 
	long battle_monster = *(int*)(c->edi+0xf8) + ((*(int*)(c->edi + 0xf4)) ? 21 : 0 );
	
	if (creature < 0 || creature >= MONSTERS_AMOUNT) {
		return EXEC_DEFAULT;
		/*
		c->eax = creature;
		c->return_address = 0x00440233;
		return NO_EXEC_DEFAULT; 
		*/
	}
	


	// long BatMan = *(int*)0x00699420;
	// long action = *(int*)(BatMan + 0x3c);
	{
		
		// long me = c->ecx; long it = *(int*)(c->ebp + 8);
		long me = c->edi;  long it = c->esi;

		long dmflags = *(int*)(it + 0x84); long dmpos = *(int*)(it + 0x38);
		if (isDragonSlayer_Table[creature] && (dmflags & 0x80000000)) {
			//hate_percent += 100;
			CALL_5(char,__thiscall,BattleMgr_Play_MagicAnimation_onGEX,*(int*)0x699420,0x5B,dmpos,0x64,0);
		}

		long spell = after_melee__spell[creature];
		if (spell) {
			long mastery = after_action_spell_mastery[creature];
			// long me = c->ecx; long it = *(int*) (c->ebp + 8);
			// WoG_CastSpell_Monster(spell,1,*(int*)(it + 0x38),1,mastery, me );
			if (0 < *(int*)(it + 0x4c))
				WoG_CastSpell(spell, 1, *(int*)(it + 0x38), 1, mastery);
			spell = after_melee__spell2[creature];
			if (spell && 0 < *(int*)(it + 0x4c))
				WoG_CastSpell(spell, 1, *(int*)(it + 0x38), 1, mastery);

		}
	}
	
	long creature2 = battle_monster + MONSTERS_AMOUNT;
	{

		// long me = c->ecx; long it = *(int*)(c->ebp + 8);
		long me = c->edi;  long it = c->esi;

		long dmflags = *(int*)(it + 0x84); long dmpos = *(int*)(it + 0x38);
		if (isDragonSlayer_Table[creature2] && (dmflags & 0x80000000)) {
			//hate_percent += 100;
			CALL_5(char, __thiscall, BattleMgr_Play_MagicAnimation_onGEX, *(int*)0x699420, 0x5B, dmpos, 0x64, 0);
		}

		long spell = after_melee__spell[creature2];
		if (spell) {
			long mastery = after_action_spell_mastery[creature2];
			// long me = c->ecx; long it = *(int*)(c->ebp + 8);
			// WoG_CastSpell_Monster(spell,1,*(int*)(it + 0x38),1,mastery, me );
			if (0 < *(int*)(it + 0x4c))
				WoG_CastSpell(spell, 1, *(int*)(it + 0x38), 1, mastery);
			spell = after_melee__spell2[creature2];
			if (spell && 0 < *(int*)(it + 0x4c))
				WoG_CastSpell(spell, 1, *(int*)(it + 0x38), 1, mastery);

		}
	}


	if (addditional_melees[battle_monster]) {
		--addditional_melees[battle_monster];
		long me = c->edi;  long it = c->esi;
		h3::H3CombatCreature* target = (h3::H3CombatCreature*)it;
		int& target_alive = target->numberAlive; // former *(int*) (it + 0x4c)
		if (target_alive) {
			CALL_3(void, __thiscall, 0x0000441330, me, it, *(int*)(c->ebp+8) );
			goto begin;
		}
	}

	return EXEC_DEFAULT;
	/*
	c->eax = creature;
	c->return_address = 0x00440233;
	return NO_EXEC_DEFAULT;
	*/
}
_LHF_(z_Hook_after_shot) {
	begin:
	// long BatMan = *(int*)0x00699420;
	// long action = *(int*)(BatMan + 0x3c);

	// long me = (*(int*)(BatMan + 0x132BC) + 21* *(int*)(BatMan + 0x132C0))* 0x548 + BatMan + 0x54cc;
	long me = c->esi; // *(int*)(c->ebp + 0x14);
	if (me<=0) return EXEC_DEFAULT;
	long creature = *(int*)(me+0x34);
	long battle_monster = *(int*)(me + 0xf8) + ((*(int*)(me + 0xf4)) ? 21 : 0);

	if (creature < 0 || creature >= MONSTERS_AMOUNT) return EXEC_DEFAULT;
	// long it = *(int*)(c->ebp + 8);
	h3::H3CombatCreature* target = (h3::H3CombatCreature*)*(int*)(c->ebp -4); // *(int*)(c->ebp + 0x1c);
	long it = *(int*)(c->ebp - 4);// *(int*)(c->ebp + 0x1c);

	//long dmflags = *(int*)(it + 0x84); long dmpos = *(int*)(it + 0x38);
	long dmflags = target->info.flags; long dmpos = target->position;

	if (isDragonSlayer_Table[creature] && (dmflags & 0x80000000)) {
		//hate_percent += 100;
		CALL_5(char, __thiscall, BattleMgr_Play_MagicAnimation_onGEX, *(int*)0x699420, 0x5B, dmpos, 0x64, 0);
	}


	long spell = after_shoot__spell[creature];
	if (spell) {
		int &target_alive = target->numberAlive; // former *(int*) (it + 0x4c) 
		long mastery = after_action_spell_mastery[creature];
		// long me = c->ecx; 
		// long it = *(int*)(c->ebp + 0x1c);
		// WoG_CastSpell_Monster(spell,1,*(int*)(it + 0x38),1,mastery, me );
		if( 0 < target_alive )
			WoG_CastSpell(spell, 1, dmpos, 1, mastery);
		spell = after_shoot__spell2[creature];
		if (spell && 0 < target_alive)
			WoG_CastSpell(spell, 1, dmpos, 1, mastery);
	}


	if(battle_monster<0 || battle_monster > 44) return EXEC_DEFAULT;

	long creature2 = battle_monster + MONSTERS_AMOUNT;
	if (isDragonSlayer_Table[creature2] && (dmflags & 0x80000000)) {
		//hate_percent += 100;
		CALL_5(char, __thiscall, BattleMgr_Play_MagicAnimation_onGEX, *(int*)0x699420, 0x5B, dmpos, 0x64, 0);
	}

	{
		long spell = after_shoot__spell[creature2];
		if (spell) {
			int& target_alive = target->numberAlive; // former *(int*) (it + 0x4c) 
			long mastery = after_action_spell_mastery[creature2];
			// long me = c->ecx; 
			// long it = *(int*)(c->ebp + 0x1c);
			// WoG_CastSpell_Monster(spell,1,*(int*)(it + 0x38),1,mastery, me );
			if (0 < target_alive)
				WoG_CastSpell(spell, 1, dmpos, 1, mastery);
			spell = after_shoot__spell2[creature2];
			if (spell && 0 < target_alive)
				WoG_CastSpell(spell, 1, dmpos, 1, mastery);
		}

	}

	if (addditional_shots[battle_monster]) {
		--addditional_shots[battle_monster];
		int& target_alive = target->numberAlive; // former *(int*) (it + 0x4c)
		if (target_alive) {
			CALL_2(void,__thiscall,0x0043F620,me,it);
			goto begin;
		}
	}

	return EXEC_DEFAULT;
}

_LHF_(z_Hook_00479231) {
	c->return_address = 0x00479503;
	long me = c->ebx;
	long creature = *(int*)(me + 0x34);
	if (creature < 0 || creature >= MONSTERS_AMOUNT) return NO_EXEC_DEFAULT;
	long battle_monster = *(int*)(me + 0xf8) + ((*(int*)(me + 0xf4)) ? 21 : 0);

	long spell = after_defend_spell[creature];
	if (spell) {
		long mastery = after_action_spell_mastery[creature];
		// WoG_CastSpell_Monster(spell,1,*(int*)(me + 0x38),1,mastery, me );
		if (0 < *(int*)(me + 0x4c))
			WoG_CastSpell(spell, 1, *(int*)(me + 0x38), 1, mastery);
		spell = after_defend_spell2[creature];
		if (spell && 0 < *(int*)(me + 0x4c))
			WoG_CastSpell(spell, 1, *(int*)(me + 0x38), 1, mastery);

	}



	creature = battle_monster + MONSTERS_AMOUNT;
	{
		long spell = after_defend_spell[creature];
		if (spell) {
			long mastery = after_action_spell_mastery[creature];
			// WoG_CastSpell_Monster(spell,1,*(int*)(me + 0x38),1,mastery, me );
			if (0 < *(int*)(me + 0x4c))
				WoG_CastSpell(spell, 1, *(int*)(me + 0x38), 1, mastery);
			spell = after_defend_spell2[creature];
			if (spell && 0 < *(int*)(me + 0x4c))
				WoG_CastSpell(spell, 1, *(int*)(me + 0x38), 1, mastery);

		}
	}

	return NO_EXEC_DEFAULT;
}

_LHF_(z_Hook_00443E36) {
	long me = c->esi; if (me <= 0) return EXEC_DEFAULT;
	long creature = *(int*)(me + 0x34);
	if (creature < 0 || creature >= MONSTERS_AMOUNT) return EXEC_DEFAULT;
	long battle_monster = *(int*)(me + 0xf8) + ((*(int*)(me + 0xf4)) ? 21 : 0);
	long spell = after_wound__spell[creature];
	
	if (spell) {
		long mastery = after_action_spell_mastery[creature];
		// WoG_CastSpell_Monster(spell,1,*(int*)(me + 0x38),1,mastery, me );
		if (0 < *(int*)(me + 0x4c))
			WoG_CastSpell_Monster(spell, 1, *(int*)(me + 0x38), 1, mastery, me);
		long spell = after_wound__spell2[creature];
		if (spell && 0 < *(int*)(me + 0x4c))
			WoG_CastSpell_Monster(spell, 1, *(int*)(me + 0x38), 1, mastery, me);

	}


	creature = battle_monster + MONSTERS_AMOUNT;
	{
		long spell = after_wound__spell[creature];
		if (spell) {
			long mastery = after_action_spell_mastery[creature];
			// WoG_CastSpell_Monster(spell,1,*(int*)(me + 0x38),1,mastery, me );
			if (0 < *(int*)(me + 0x4c))
				WoG_CastSpell_Monster(spell, 1, *(int*)(me + 0x38), 1, mastery, me);
			long spell = after_wound__spell2[creature];
			if (spell && 0 < *(int*)(me + 0x4c))
				WoG_CastSpell_Monster(spell, 1, *(int*)(me + 0x38), 1, mastery, me);

		}
	}

	return EXEC_DEFAULT;
}

_LHF_(z_hook_007609E0) {
	c->flags.ZF = isTeleporter[c->eax];
	c->return_address = 0x007609F1;
	return NO_EXEC_DEFAULT;
}

_LHF_(z_hook_00760412) {
	if (isTeleporter[c->eax]) {
		c->return_address = 0x00760443;
		return NO_EXEC_DEFAULT;
	} else {
		c->return_address = 0x0076041C;
		return NO_EXEC_DEFAULT;
	}

}

_LHF_(z_Hook_004403A7) {
	long me = c->esi;	long it = c->edi;
	if (magic_resistance_table[ *(int*)(it + 0x34 ) ] == RESIST_SPEED ) {
		c->return_address = 0x004402AE;
		return NO_EXEC_DEFAULT;
	}

	return EXEC_DEFAULT;
}

char missing_string[512] = "Ooops !!!";

_LHF_(z_Hook_00492A7D) {
	if (c->eax) return EXEC_DEFAULT;

	c->Push((int)missing_string);

	c->return_address = 0x00492A99;
	return NO_EXEC_DEFAULT;
}

//============================================
extern "C" __declspec(dllexport) void InitNecromancy15() {

	bool good = true;
	FILE* str_necro;
	char* filename;

	/*if (FileExists(filename = "Mods\\Knightmare Kingdoms\\Data\\necromancy15.cfg")) str_necro = fopen(filename, "r");

	else*/ if (FileExists(filename = "Data\\necromancy15.cfg")) str_necro = fopen(filename, "r");
	else { good = false; }

	bool monster_out_of_range = false;
	if (good) for (int i = 0; i <= 15; i++) {
		fscanf(str_necro, "%ld%ld",
			(Necromancy_without_artifacts + i),
			(Necromancy_with_artifacts + i));
		if (all_creatures < Necromancy_without_artifacts[i] 
			|| all_creatures < Necromancy_with_artifacts[i])
				monster_out_of_range = true;
	}

	if (!good || monster_out_of_range)
		MessageBoxA(0, "necromancy15.cfg", "Wrong config: ", 0);

	if (good)fclose(str_necro);
}

void InitNecromancy() {

	Necromancy_with_artifacts[0] = 56;
	/*
	std::ifstream str_necro;
	
	if (FileExists("Mods\\Amethyst Upgrades\\Data\\necromancy.cfg"))
		str_necro.open("Mods\\Amethyst Upgrades\\Data\\necromancy.cfg", std::ios_base::in);
	else if (FileExists("Mods\\Knightmare Kingdoms\\Data\\necromancy.cfg"))
		str_necro.open("Mods\\Knightmare Kingdoms\\Data\\necromancy.cfg", std::ios_base::in);
	else str_necro.open("Data\\necromancy.cfg", std::ios_base::in);
	*/
	bool good = true;
	FILE* str_necro;
	char *filename;

	/*if (FileExists(filename = "Data\\necromancy.cfg")) str_necro = fopen(filename, "r");
	else*/ /*if(FileExists(filename = "Mods\\Amethyst Upgrades\\Data\\necromancy.cfg")) str_necro = fopen(filename, "r");
	else*/ /*if (FileExists(filename = "Mods\\Knightmare Kingdoms\\Data\\necromancy.cfg")) str_necro = fopen(filename, "r");

	else*/ if (FileExists(filename = "Data\\necromancy.cfg")) str_necro = fopen(filename, "r");
	else { good = false; }
	//str_necro=fopen(filename, "r");
	/*
	str_necro
		>> (Necromancy_without_artifacts[1])
		>> (Necromancy_without_artifacts[2])
		>> (Necromancy_without_artifacts[3])

		>> (Necromancy_with_artifacts[1])
		>> (Necromancy_with_artifacts[2])
		>> (Necromancy_with_artifacts[3]);
	*/
	if (good)
	fscanf(str_necro,"%ld%ld%ld%ld%ld%ld",
		(Necromancy_without_artifacts+1),
		(Necromancy_without_artifacts+2),
		(Necromancy_without_artifacts+3),

		(Necromancy_with_artifacts+1),
		(Necromancy_with_artifacts+2),
		(Necromancy_with_artifacts+3));
	
	if ((!good)
		|| all_creatures < Necromancy_without_artifacts[1] || all_creatures < Necromancy_without_artifacts[2] || all_creatures < Necromancy_without_artifacts[3]
		|| all_creatures < Necromancy_with_artifacts[1]    || all_creatures < Necromancy_with_artifacts[2]    || all_creatures < Necromancy_with_artifacts[3] ) {

		MessageBoxA(0, "necromancy.cfg", "Wrong config: ", 0);
		char log_[1024]; sprintf(log_, "%ld %ld %ld %ld %ld %ld",
			(Necromancy_without_artifacts[1]),
			(Necromancy_without_artifacts[2]),
			(Necromancy_without_artifacts[3]),

			(Necromancy_with_artifacts[1]),
			(Necromancy_with_artifacts[2]),
			(Necromancy_with_artifacts[3]));

		MessageBoxA(0,log_, "necromancy.cfg", 0);

		Necromancy_without_artifacts[1] = 56;
		Necromancy_without_artifacts[2] = 56;
		Necromancy_without_artifacts[3] = 56;

		Necromancy_with_artifacts[1] = 58;
		Necromancy_with_artifacts[2] = 60;
		Necromancy_with_artifacts[3] = 64;

		good = false;
	}

	///str_necro.close();
	if(good)fclose(str_necro);

	//majaczek necromancy
	//WriteHook((void*)0x4e3f14, (void*)f_4e3f14_Hook, HOOKTYPE_JUMP);
	WriteHook((void*)0x4e3ed0, (void*)f_4e3ed0_Hook, HOOKTYPE_JUMP);
	WriteHook((void*)0x433438, (void*)f_433438_Hook, HOOKTYPE_JUMP);
}
_LHF_(z_hook_00476F67) {
	MONSTER_PROP* mon = new_monsters+ *(int*) c->ebx;
	if ((mon->flags & 0x00040010) == 0) {
		c->return_address = 0x00476FA4;
		return NO_EXEC_DEFAULT;
	}
	
	if (c->eax > c->ecx) {
		c->eax = c->ecx;
	}

	c->return_address = 0x00476F6D;
	return NO_EXEC_DEFAULT;
}


void ResetAdditionalTables_BattleOnly(){
	//int CombatManager = (*(int*)0x699420);
	//H3CombatMonster* monsters = (H3CombatMonster*) (CombatManager+0x54CC);

	for (int i = MONSTERS_AMOUNT; i < MONSTERS_AMOUNT + 42; ++i) {
		memset(Spell_Immunities[i], 0, 16);

		respawn_table_fraction[i] = 0.0; respawn_table_chance[i] = 0.0;
		StrikeAndReturn_Table[i] = 0; respawn_table_sure[i] = 0.0;
		Counterstrike_Table[i] = 0; after_action_spell_mastery[i] = 2;
		after_wound__spell[i] = after_wound__spell2[i] = 0;
		after_defend_spell[i] = after_defend_spell2[i] = 0;
		after_melee__spell[i] = after_melee__spell2[i] = 0;
		after_shoot__spell[i] = after_shoot__spell2[i] = 0;
		Fearless_Table[i] = 0; Fear_Table[i] = 0;
		DeathBlow_Table[i] = 0;
		PreventiveCounterstrikeTable[i] = 0;
		RangeRetaliation_Table[i] = 0;
		MovesTwice_Table[i] = 0;

		/*
		long mon_type = monsters[(i-MONSTERS_AMOUNT)].type;
		if (mon_type>=0 && MovesTwice_Table[mon_type]) 
			addditional_moves[i-MONSTERS_AMOUNT] = 1;
			*/
	}

	/*
	for (int i = 0; i < 42; ++i) {
		long mon_type = monsters[i].type;
		if (mon_type >= 0 && MovesTwice_Table[mon_type])
			addditional_moves[i] = 1;
	}
	*/
}


_LHF_(hook_next_creature) {
	// THISCALL_2(int, 0x464C60, this, 1);
	int bm = *(int*)(0x699420);
	CALL_2(int,__thiscall, 0x464C60, bm, 1);
	return EXEC_DEFAULT;
}

_LHF_(hook_004470B5_Demonology) {
	c->eax *= CreatureSpellPowerMultiplier[demonolgy_Temp1];
	c->eax /= CreatureSpellPowerDivider[demonolgy_Temp1];
	c->eax += CreatureSpellPowerAdder[demonolgy_Temp1];
	return EXEC_DEFAULT;
}

// #include"H3Spell.h"
#define o_Spell ( *reinterpret_cast<H3Spell**>(0x687FA8))
double __stdcall ResistanceHook(HiHook* h, int spell, int creature, HERO* caster, HERO* victim)
{
	if (Receptive_Table[creature] && spell > 0 && 
		(o_Spell[spell].defensive || o_Spell[spell].friendlyMass)) return 1.01;
	// if ();
	return  CALL_4(double, __fastcall, h->GetDefaultFunc(), spell, creature, caster, victim);
}

extern "C" __declspec(dllexport) int isReceptive(int creature) {
	return Receptive_Table[creature];
}

_LHF_(z_hook_0075642C) { // !!MA
	char letter = c->eax;
	int mn = *(int*)(c->ebp - 4);
	Mes* Mp = (Mes*)*(int*)(c->ebp + 0x14);
	int i;
	switch (letter)
	{
	case 'W':
	{
		char* str = "";
		str = ERM2String(&Mp->m.s[/*Mp->i*/ 1], 0, &i);
		// Mp->i += i;
		// Mess(Mp);

		// v_MsgBox(str, 1);

		ChangeCreatureTable(mn, str);
		break;
	}
	default:
		return EXEC_DEFAULT;
		break;
	}

	c->return_address = 0x00756912;
	return NO_EXEC_DEFAULT;
}

_LHF_(z_hook_0075F840) { // !!BM
	char letter = 'A' + *(char*)(c->ebp - 0x44);
	int mn = *(int*)(c->ebp - 0xc);
	int i;
	Mes *Mp = (Mes*) *(int*)(c->ebp+0x14);
	_ToDo_* sp = (_ToDo_*)*(int*)(c->ebp + 0x10);
	switch (letter)
	{
	case 'W':
	{
		char* str = "";
		str = ERM2String(&Mp->m.s[/*Mp->i*/ 1], 0, &i);
		// Mp->i += i;
		// Mess(Mp);

		// v_MsgBox(str, 1);

		ChangeCreatureTable(-1 - mn, str);
		break;
	}
	default:
		return EXEC_DEFAULT;
		break;
	}
	c->return_address = 0x0075F85D;
	return NO_EXEC_DEFAULT;
}

_LHF_(hook_00440BDB) {
	h3::H3CombatCreature* target = (h3::H3CombatCreature*) c->ecx;
	if (isHeroic_Table[target->type]
		|| isHeroic_Table[target->Index()+MONSTERS_AMOUNT]
		) {
		c->return_address = 0x004412AB;
		return NO_EXEC_DEFAULT;
	}

	return EXEC_DEFAULT;
}

_LHF_(hook_00440245) {
	h3::H3CombatCreature* target = (h3::H3CombatCreature*)c->edi;

	if (isHeroic_Table[target->type]
		|| isHeroic_Table[target->Index() + MONSTERS_AMOUNT]
		) {
		c->return_address = 0x004402AE;
		return NO_EXEC_DEFAULT;
	}

	return EXEC_DEFAULT;
}

_LHF_(hook_005A03A8) {
	if (!c->edi) return EXEC_DEFAULT;
	auto targetMon = (h3::H3CombatCreature*) c->edi;
	int spell = *(int*)(c->ebp+8);

	if (spell == 75 || spell == 79) {

		if (isHeroic_Table[targetMon->type]
			|| isHeroic_Table[targetMon->Index() + MONSTERS_AMOUNT]
			) {
			c->edi = 0; return EXEC_DEFAULT;
		}
	}

	return EXEC_DEFAULT;
}

void CreateAdditionalTables()
{
	static bool first_time = true;

	memset(SharedStats, 0, sizeof(SharedStats) );
	for (int i = 0; i < MONSTERS_AMOUNT; ++i) {
		FILE* filedesc;	char filename[256];
		sprintf(filename, "Data\\creatures\\%us.cfg", i);
		if (filedesc = fopen(filename, "r"))
		{
			//----------
			fseek(filedesc, 0, SEEK_END);
			int fdesc_size = ftell(filedesc);
			rewind(filedesc);
			//----------
			char *buf = (char*)malloc(fdesc_size + 1);
			fread(buf, 1, fdesc_size, filedesc);
			buf[fdesc_size] = 0;
			fclose(filedesc);

			SharedStats[i] = buf;//free(buf);
		}
	}

	/* Heroic - disabed by Archer30 8 Jun 2024
	Z_Amethyst->WriteLoHook(0x005A03A8, hook_005A03A8); // Archer: Cast spell
	Z_Amethyst->WriteLoHook(0x00440245, hook_00440245); // Archer: After Hit spells
	Z_Amethyst->WriteLoHook(0x00440BDB, hook_00440BDB); // Archer: After attack abilities
	*/

	memset(Spell_Immunities,0,sizeof(Spell_Immunities));
	Z_Amethyst->WriteLoHook(0x0071CC52,z_hook_0071CC52); // Archer: dwarf resistance

	// Z_Amethyst->WriteLoHook(0x005A7B77,hook_next_creature);
	// 00478CC3

	Z_Amethyst->WriteLoHook(0x0075F840, z_hook_0075F840); // Archer: erm patches
	Z_Amethyst->WriteLoHook(0x0075642C, z_hook_0075642C);

	//majaczek tables
	memset(resource_type_table, -1, MONSTERS_AMOUNT);

	memset(hasSantaGuards,false,MONSTERS_AMOUNT); hasSantaGuards[173]=true;
	memset(isGhost,false,MONSTERS_AMOUNT); isGhost[159]=true;
	for (int i = 0; i < MONSTERS_AMOUNT;++i) ghost_fraction[i] = 1.0;

	/*
	for (long i = 0; i < MONSTERS_AMOUNT; i++)
		GhostMultiplyer_028460f8[i] = 100;
	*/
	*(int*)(0x00756B97 + 4) = (int) GhostMultiplyer_028460f8;
	*(int*)(0x00756A4F + 3) = (int) GhostMultiplyer_028460f8;
	*(int*)(0x00756AD2 + 3) = (int) GhostMultiplyer_028460f8;

	memset(isEnchanter,false,MONSTERS_AMOUNT); isEnchanter[136]=true;
	memset(isSorceress,false,MONSTERS_AMOUNT); isSorceress[193]=true;

	memset(isHellSteed,false,MONSTERS_AMOUNT);  isHellSteed[195]=true;
	memset(isHellSteed2,false,MONSTERS_AMOUNT); isHellSteed2[195]=true;
	memset(isHellSteed3,false,MONSTERS_AMOUNT); isHellSteed3[195]=true;

	FireWall_Table[195] = true;
	WriteHook((void*)0x75CA39, (void*)Firewall_byte, HOOKTYPE_JUMP);


	memset(isRogue, false, MONSTERS_AMOUNT); isRogue[143] = true;
	memset(after_action_spell_mastery, 2, MONSTERS_AMOUNT);

	// Archer: Don't disable this otherwise the game crashes when casting a spell on AA's turn
	for (long i = 0; i < MONSTERS_AMOUNT; i++) {
		CreatureSpellPowerMultiplier[i] = 1;
		CreatureSpellPowerDivider[i] = 1;
		CreatureSpellPowerAdder[i] = 0;
	}

	memset(DalionsGuards,-1,MONSTERS_AMOUNT*4);
	//DalionsGuards[230] = 29;

	//majaczek hooks	

	WriteHook((void*)0x00766AC8,(void*)shooting_resistance_hook,HOOKTYPE_CALL);
	
	WriteHook((void*)0x766DFF,(void*) Hate_Hook_766DFF, HOOKTYPE_JUMP);

	WriteHook((void*) 0x5502EE , (void*) Dlg_RecruitMonster_Hook_5502EE, HOOKTYPE_JUMP);

	//WriteHook((void*)0x004E6050, (void*)f_004E6050_Hook, HOOKTYPE_JUMP); //rogue_bad
	//WriteHook((void*), (void*)f_MoP_Rogue_Hook, HOOKTYPE_CALL);
	
	/*
	WriteHook((void*) 0x4165c3, (void*)f_MoP_Rogue_Hook, HOOKTYPE_CALL);
	WriteHook((void*) 0x4165de, (void*)f_MoP_Rogue_Hook, HOOKTYPE_CALL);
	WriteHook((void*) 0x4167e8, (void*)f_MoP_Rogue_Hook, HOOKTYPE_CALL);
	WriteHook((void*) 0x416803, (void*)f_MoP_Rogue_Hook, HOOKTYPE_CALL);
	WriteHook((void*) 0x416fb4, (void*)f_MoP_Rogue_Hook, HOOKTYPE_CALL);
	WriteHook((void*) 0x416fcf, (void*)f_MoP_Rogue_Hook, HOOKTYPE_CALL);
	WriteHook((void*) 0x41719c, (void*)f_MoP_Rogue_Hook, HOOKTYPE_CALL);
	*/
	WriteHook((void*) 0x4E609E, (void*)fun_4E609E_Rogue_Hook, HOOKTYPE_CALL);
	WriteHook((void*) 0x4E6067, (void*)fun_4E609E_Rogue_Hook, HOOKTYPE_CALL);


	//WriteHook((void*)0x75D2BB, (void*)f_75D2BB_Hook, HOOKTYPE_JUMP);
	
	/// debug
	// WriteHook((void*)0x005A0140, (void*)Creature_SpellPower_hook, HOOKTYPE_JUMP);
	// Z_Amethyst->WriteLoHook(0x005A0145, Creature_SpellPower_5A0145); // Most important spell power hook - Disabled by Archer 16 Feb 2024

	//WriteHook((void*)0x75D0F2,(void*)f_75D0F2_Hook, HOOKTYPE_JUMP);	//hasSantaGuards
	WriteHook((void*)0x756B1F,(void*)f_756B1F_Hook, HOOKTYPE_JUMP); // Archer: Monster Kickback
	
	//WriteHook((void*)0x447ED9,(void*)f_447ED9_Hook, HOOKTYPE_JUMP);
	WriteHook((void*)0x4650dd,(void*)f_4650dd_Hook, HOOKTYPE_JUMP); // Archer: Enchanter
	WriteHook((void*)0x75C96C,(void*)f_75C96C_Hook, HOOKTYPE_JUMP); // Archer: Sorceress
	
	
	//WriteHook((void*)0x75CA39,(void*)f_75CA39_Hook, HOOKTYPE_JUMP);	
	//WriteHook((void*)0x75E8BA,(void*)f_75E8BA_Hook, HOOKTYPE_JUMP);
	//WriteHook((void*)0x760723,(void*)f_760723_Hook, HOOKTYPE_JUMP);

	
	//spells
	// Archer: This bit is important. It prevents the game from crashing on mouse hovering
	*(int*) (0x00448263 +3) = (int) spell_1_jumptable;
	  memset(spell_1_table,0x09,MONSTERS_AMOUNT);
	  if(first_time)
		memcpy(spell_1_table+0x0D,(void*)(*(int*)0x44825F),197-0x0D);
	
	  *(char*)0x44824A = 0;
	   *(int*)0x44825F = (int)spell_1_table;
	   


	   *(int*)(0x00447479 + 3) = (int)spell_2_jumptable;
	  memset(spell_2_table,0x08,MONSTERS_AMOUNT);
	  if (first_time)
		  memcpy(spell_2_table+0x0D,(void*)(*(int*)0x447475),197-0x0D);
	
	  *(char*)0x447467 = 0;
	   *(int*)0x447475 = (int)spell_2_table;

	   *(int*)(0x0042147D + 3) = (int)spell_3_jumptable;
	  memset(spell_3_table,0x03,MONSTERS_AMOUNT);
	  if (first_time)
		  memcpy(spell_3_table+0x0D,(void*)(*(int*)0x421479),197-0x0D);
	
	  *(char*)0x42146F = 0;
	   *(int*)0x421479 = (int)spell_3_table;
	

	//skel
		/* Disabled by Archer 16 Feb 2024
		for (int i=196; i!=MONSTERS_AMOUNT; i++) skeltrans[i]=56;
	    memcpy((char*)skeltrans,(void*)(0x7C3D00),197*4);
	    
	  	*(int*)(0x5664B5+3) = (int)skeltrans;
		*(int*)(0x56689D+3) = (int)skeltrans;
		*(int*)(0x566D79+3) = (int)skeltrans;
		*(int*)(0x566F3C+3) = (int)skeltrans;
		*(int*)(0x566FA4+3) = (int)skeltrans;
		*(int*)(0x566FD1+3) = (int)skeltrans;

		*(int*)(0x5664C9) = MONSTERS_AMOUNT;
		*/

	//upgtable

		WriteHook((void*)0x47AA50, (void*)Hill_Fort_47AA50_Hook, HOOKTYPE_JUMP);

		memset((char*)upgtable, 0xFF, MONSTERS_AMOUNT*sizeof(int));
		
	  	*(int*)(0x724A92+3) = (int)upgtable;
		*(int*)(0x724AA8+3) = (int)upgtable;
		*(int*)(0x74EC7A+3) = (int)upgtable;
		*(int*)(0x74EC94+3) = (int)upgtable;
		*(int*)(0x74ED17+3) = (int)upgtable;
		*(int*)(0x74ED33+3) = (int)upgtable;
		*(int*)(0x75137D+1) = (int)upgtable;
		*(int*)(0x751EC4+1) = (int)upgtable;
		*(int*)(0x752E33+3) = (int)upgtable;
		*(int*)(0x7568F1+3) = (int)upgtable;
		

		*(int*)(0x751378+1) = (int)MONSTERS_AMOUNT*sizeof(int);
		*(int*)(0x751EB9+3) = (int)MONSTERS_AMOUNT*sizeof(int);
	//cast after attack ablilties
	  memset(aftercast_abilities_table,ACAST_DEFAULT,MONSTERS_AMOUNT);
	  memcpy(aftercast_abilities_table+0x16,(void*)(*(int*)0x440237),197-0x16);
	
	  *(char*)0x44022D = 0;
	   *(int*)0x440237 = (int)aftercast_abilities_table;
	  //memset((void*)0x440231,0x90,3);
	   // Z_Amethyst->WriteLoHook(0x0044022B, z_Hook_after_melee);
	   Z_Amethyst->WriteLoHook(0x0070A13F, z_Hook_after_melee);
	   Z_Amethyst->WriteLoHook(0x00441C01, z_Hook_after_melee);
	   // Z_Amethyst->WriteLoHook(0x0043FB1E, z_Hook_after_shot);
	   Z_Amethyst->WriteLoHook(0x0043FF9A, z_Hook_after_shot);
	   Z_Amethyst->WriteLoHook(0x00479231, z_Hook_00479231);
	   Z_Amethyst->WriteLoHook(0x00443E36, z_Hook_00443E36);
	   Z_Amethyst->WriteLoHook(0x004403A7, z_Hook_004403A7);

	//special attacks
	  memset(attack_abilities_table,ATT_DEFAULT,MONSTERS_AMOUNT);
	  memcpy(attack_abilities_table+0x3F,(void*)(*(int*)0x440916),197-0x3F);
	
	  *(char*)0x440908 = 0;
	   *(int*)0x440916 = (int)attack_abilities_table;
	  memset((void*)0x440909,0x90,9);


    //magic resistance
	  memset(magic_resistance_table,RESIST_DEFAULT,MONSTERS_AMOUNT);
	  memcpy(magic_resistance_table+0x10,(void*)(*(int*)0x44A4B3),118);
	
	  *(char*)0x44A4A9 = 0;
	  *(unsigned short*)0x0044A4AD = 0x9090;
	   *(int*)0x44A4B3 = (int)magic_resistance_table;
	   *(int*)0x44a4ba = (int)resistance_jumptable;

	 //magic vulnerability by majaczek
	   memset(magic_vulnerability_table, VULN_DEFAULT, MONSTERS_AMOUNT);
	   memcpy(magic_vulnerability_table+0x20, (void*)(*(int*)0x44B197),0x62);

	   *(char*)0x44B189 = 0; 
	    *(unsigned short*)0x0044B18D = 0x9090;
	    *(unsigned long*) 0x0044B18F = 0x90909090;
		*(int*)0x44B197 = (int)magic_vulnerability_table;
		*(int*)0x44b19e = (int)vulnerability_jumptable;

	//shot
		memset(special_missiles_table, 0, MONSTERS_AMOUNT);
		special_missiles_table[CREATURE_MAGOG] = 1;
		special_missiles_table[CREATURE_LICH] = 2;
		special_missiles_table[CREATURE_POWERLICH] = 2;
		special_missiles_table[CREATURE_DRACOLICH] = 2;

		WriteHook((void*)0x41ED5A,(void*)f41ED5A_Hook, HOOKTYPE_JUMP);

		WriteHook((void*)0x43F72C,(void*)f43F72C_Hook,HOOKTYPE_JUMP);

	//fire shield
		for (int i=0; i!=MONSTERS_AMOUNT; i++) fire_shield_table[i]=0;

		fire_shield_table[CREATURE_EFREET_SULTAN] = 0.2;
		//fire_shield_table[CREATURE_CHAMPION] = 1;

		WriteHook((void*)0x4225D6,(void*)f4225D6_Hook,HOOKTYPE_JUMP);
		WriteHook((void*)0x442E61,(void*)f442E61_Hook,HOOKTYPE_JUMP);
	//pegasi

	   
	//aftercast_abilities_table[197] = ACAST_ACID;
	//attack_abilities_table[197]=ATT_VAMPIRE;

	WriteHook((void*)0x0076735D, (void*)IsAngel3_Minus, HOOKTYPE_JUMP); //2021-03-14
	WriteHook((void*)0x004482D3, (void*)Angel4_004482D3, HOOKTYPE_JUMP);
	//WriteHook((void*)0x004211b3, (void*)IsAngel5_AI, HOOKTYPE_JUMP);
	//WriteHook((void*)0x004470FD, (void*)IsAngel6_CastEmptyField, HOOKTYPE_JUMP);
	//WriteHook((void*)0x0044718E, (void*)IsAngel7, HOOKTYPE_JUMP);

	WriteHook((void*)0x0044705A, (void*)Ang1, HOOKTYPE_JUMP);
	WriteHook((void*)0x0044718E, (void*)Ang2, HOOKTYPE_JUMP);
	//WriteHook((void*)0x005A87AD, (void*)Ang3, HOOKTYPE_CALL);

	WriteHook((void*)0x004211B3, (void*)AngDem1, HOOKTYPE_JUMP);
	WriteHook((void*)0x004470FD, (void*)AngDem2, HOOKTYPE_JUMP);
	WriteHook((void*)0x00447480, (void*)AngDem3, HOOKTYPE_JUMP);
	WriteHook((void*)0x00491E93, (void*)AngDem4, HOOKTYPE_JUMP); // Anchangel battle hint proc. Disabling it won't fix the hint. 
	WriteHook((void*)0x00492065, (void*)AngDem5, HOOKTYPE_JUMP);

	Demonology_Table[51] = 48;
	WriteHook((void*)0x4211ED, (void*)Dem1, HOOKTYPE_JUMP);
	WriteHook((void*)0x42127A, (void*)Dem2, HOOKTYPE_JUMP);
	WriteHook((void*)0x42131D, (void*)Dem3, HOOKTYPE_JUMP);
	WriteHook((void*)0x447154, (void*)Dem4, HOOKTYPE_JUMP);
	WriteHook((void*)0x4474D1, (void*)Dem5, HOOKTYPE_JUMP);
	WriteHook((void*)0x491F42, (void*)Dem6, HOOKTYPE_JUMP);
	WriteHook((void*)0x447204, (void*)Demon, HOOKTYPE_JUMP);
	WriteHook((void*)0x4470CD, (void*)Demon2, HOOKTYPE_JUMP);
	WriteHook((void*)0x5A776A, (void*)Demon3, HOOKTYPE_JUMP);
	Z_Amethyst->WriteLoHook(0x004470B5, hook_004470B5_Demonology);

	//WriteHook((void*)0x4470B8, (void*)Gate1, HOOKTYPE_JUMP);
	// WriteHook((void*)0x5A78B5, (void*)Gate2, HOOKTYPE_JUMP); // Disabled by Archer30 6 Jun 2024

	/* Disabled by Archer30 6 Jun 2024
	WriteHook((void*)0x75DE5B, (void*) Regeneration,HOOKTYPE_JUMP);
	RegenerationChance_Table[60]  = 100; RegenerationHitPoints_Table[60]  = 50;
	RegenerationChance_Table[61]  = 100; RegenerationHitPoints_Table[61]  = 50;
	RegenerationChance_Table[144] = 100; RegenerationHitPoints_Table[144] = 50;
	RegenerationChance_Table[157] = 40;  RegenerationHitPoints_Table[157] = 50;
	for (int i =174; i<= 191;i++)		 RegenerationHitPoints_Table[i]   = 50;
	*/


	WriteHook((void*)0x4650D0, (void*)ManaDrainPlus,  HOOKTYPE_JUMP);
	WriteHook((void*)0x465174, (void*)ManaDrain2, HOOKTYPE_JUMP);
	ManaDrain_Table[61] = 2;

	WriteHook((void*)0x4E554C, (void*) SpellsCostDump, HOOKTYPE_JUMP);
	SpellsCostDump_Table[20] = SpellsCostDump_Table[21] = 2;
	WriteHook((void*)0x766BCC, (void*) SpellsCostLess, HOOKTYPE_JUMP);
	SpellsCostLess_Table[34] = SpellsCostLess_Table[35] = 2;


	WriteHook((void*)0x43D6A6, (void*)CounterStrike, HOOKTYPE_JUMP);
	WriteHook((void*)0x446E77, (void*)CounterStrike2, HOOKTYPE_JUMP);
	for (int i = 0; i < MONSTERS_AMOUNT; i++) Counterstrike_Table[i] = 1;
	Counterstrike_Table[4] = 2; Counterstrike_Table[5] = 255;
	Counterstrike_Table[145] = Counterstrike_Table[146] = Counterstrike_Table[147] 
		= Counterstrike_Table[148] = Counterstrike_Table[149] = 0;


	WriteHook((void*)0x44AEF7, (void*)AlwaysPositiveMorale, HOOKTYPE_JUMP);
	WriteHook((void*)0x44BC4E, (void*)AlwaysPositiveMorale2, HOOKTYPE_JUMP);
	WriteHook((void*)0x767133, (void*)AlwaysPositiveMorale3, HOOKTYPE_JUMP);
	/* AlwaysPositiveMorale_Table[145] = AlwaysPositiveMorale_Table[146] = AlwaysPositiveMorale_Table[147]
		= AlwaysPositiveMorale_Table[148] = AlwaysPositiveMorale_Table[149] = 1; */
	AlwaysPositiveMorale_Table[78] = AlwaysPositiveMorale_Table[79] = 1;

	WriteHook((void*)0x44C132, (void*)AlwaysPositiveLuck,  HOOKTYPE_JUMP);
	WriteHook((void*)0x43DCDF, (void*)AlwaysPositiveLuck2, HOOKTYPE_JUMP);
	WriteHook((void*)0x44B11B, (void*)AlwaysPositiveLuck3, HOOKTYPE_JUMP);
	AlwaysPositiveLuck_Table[138] = 1;

	WriteHook((void*)0x766CE8, (void*)DeathBlow, HOOKTYPE_JUMP);
	WriteHook((void*)0x4436DE, (void*)DeathBlowCorrect, HOOKTYPE_JUMP);
	DeathBlow_Table[67] = 20;

	WriteHook((void*)0x7604F1, (void*)Fear, HOOKTYPE_JUMP);
	WriteHook((void*)0x760481, (void*)Fearless, HOOKTYPE_JUMP);
	Fear_Table[132] = Fearless_Table[132] = 1;
	Fearless_Table[150] = Fearless_Table[151] = Fearless_Table[152] = 1;
	Fear_Table[153] = Fearless_Table[153] = 1;
	Fear_Table[155] = Fearless_Table[155] = 1;
	Fearless_Table[154] = Fearless_Table[156] = Fearless_Table[157] = Fearless_Table[158] = 1;
	Fearless_Table[159] = Fearless_Table[164] = Fearless_Table[165] = Fearless_Table[166] = 1;
	Fearless_Table[167] = Fearless_Table[168] = Fearless_Table[169] = Fearless_Table[170] = 1;
	Fearless_Table[171] = Fearless_Table[172] = 1; 
	for (int i=174;i<=191;i++) Fearless_Table[i] = 1;


	WriteHook((void*)0x760590, (void*)NoWallPenalty, HOOKTYPE_JUMP);
	NoWallPenalty_Table[34] = NoWallPenalty_Table[35] = 1;
	NoWallPenalty_Table[136] = NoWallPenalty_Table[137] = 1;
	NoWallPenalty_Table[149] = 1;
	NoWallPenalty_Table[170] = NoWallPenalty_Table[171] = 1;

	WriteHook((void*)0x767244, (void*)MagicAura, HOOKTYPE_JUMP);
	WriteHook((void*)0x76729F, (void*)MagicAura, HOOKTYPE_JUMP);
	MagicAura_Table[24] = MagicAura_Table[25] = 1;

	/*
	WriteHook((void*) 0x75E059, (void*)StrikeAndReturn, HOOKTYPE_JUMP);
	WriteHook((void*) 0x762940, (void*)StrikeAndReturn2, HOOKTYPE_JUMP);
	WriteHook((void*) 0x421C0D, (void*)StrikeAndReturn3, HOOKTYPE_JUMP);
	*/

	// These Strike and Retunr hooks are important - Archer30
	Z_Amethyst->WriteLoHook(0x00762940, harpy_hook_00762940);
	Z_Amethyst->WriteLoHook(0x0075E063, harpy_hook_0075E063);
	StrikeAndReturn_Table[72] = StrikeAndReturn_Table[73] = 1;
	StrikeAndReturn_Table[155] = 1;

	WriteHook((void*) 0x75D823, (void*)JoustingBonus, HOOKTYPE_JUMP);
	WriteHook((void*) 0x44307A, (void*)ImmunToJoustingBonus, HOOKTYPE_JUMP);
	ImmunToJoustingBonus_Table[0] = ImmunToJoustingBonus_Table[1] = 1;
	JoustingBonus_Table[10] = JoustingBonus_Table[11] = 1;

	WriteHook((void*)0x5A24DC, (void*)MagicChannel, HOOKTYPE_JUMP);
	WriteHook((void*)0x448528, (void*)MagicMirror, HOOKTYPE_JUMP);
	MagicChannel_Table[43] = 1; MagicMirror_Table[134] = 1;

	WriteHook((void*)0x75DD50, (void*)Sharpshooters, HOOKTYPE_JUMP);
	
	// WriteHook((void*)0x767195, (void*)ShootingAdjacent, HOOKTYPE_JUMP);
	WriteHook((void*)0x0076718F, (void*)ShootAnyTime, HOOKTYPE_JUMP);
	WriteHook((void*)0x007671D7, (void*)ShootAnyTime2, HOOKTYPE_JUMP);

	Sharpshooters_Table[137] = Sharpshooters_Table[149] = 1;
	Sharpshooters_Table[170] = Sharpshooters_Table[171] = 1;

	//WriteHook((void*)0x4483C4, (void*)Spells, HOOKTYPE_JUMP);
	//WriteHook((void*)0x4476C8, (void*)Spells2, HOOKTYPE_JUMP);

	/* Disabled by Archer 8 Jun 2024
	WriteHook((void*)0x75D0F2, (void*)Spells3, HOOKTYPE_JUMP);
	WriteHook((void*)0x75CDE2, (void*)Spells4, HOOKTYPE_JUMP); // todo
	*/

	Spells_Table[173] = 16;
	Spells_Table[174] = Spells_Table[183] = 37;
	Spells_Table[175] = Spells_Table[184] = 27;
	Spells_Table[176] = Spells_Table[185] = 44;
	Spells_Table[177] = Spells_Table[186] = 29;
	Spells_Table[178] = Spells_Table[187] = 39;
	Spells_Table[179] = Spells_Table[188] = 43;
	Spells_Table[180] = Spells_Table[189] = 46;
	Spells_Table[181] = Spells_Table[190] = 53;
	Spells_Table[182] = Spells_Table[191] = 58;

	/* Disabled by Archer 8 Jun 2024
	WriteHook((void*)0x75CEDD, (void*)SantaFixPlus, HOOKTYPE_JUMP);
	//WriteHook((void*)0x75D10A, (void*)f_75D10A_Hook, HOOKTYPE_JUMP);
	//*((int*)(0x75D10A + 1)) = (int)(void*)MakeDalionsGuards;
	//WriteHook((void*)0x75D10A, (void*)MakeDalionsGuards, HOOKTYPE_CALL);
	WriteHook((void*)0x75D10A, (void*)MakeDalionsGuards, HOOKTYPE_JUMP);
	*/

	WriteHook((void*)0x4755E7, (void*)hook_004755E7, HOOKTYPE_JUMP); // Archer: Proc action user action? Not sure what
	isAimedCaster_Table[134] = 1;
	isFaerieDragon_Table[134] = 1;
	isAmmoCart_Table[148] = 1;
	isPassive_Table[148] = 1;
	//WriteHook((void*)0x422A86, (void*)AmmoCart_422A86_Hook, HOOKTYPE_JUMP);
	// WriteHook((void*)0x43F718, (void*)AmmoCart_43F718_Hook, HOOKTYPE_JUMP); // Disabled by Archer 7 May 2024 as it results in a crash
	WriteHook((void*)0x474541, (void*)Passive_474541_Hook, HOOKTYPE_JUMP);

	WriteHook((void*)0x75E8F9, (void*) ImposedSpells1, HOOKTYPE_JUMP);
	WriteHook((void*)0x7607B1, (void*) ImposedSpells2, HOOKTYPE_JUMP);
	ImposedSpells_Table[152][0] = 28; ImposedSpells_Table[152][3] = 3;
	ImposedSpells_Table[158][0] = 29; ImposedSpells_Table[158][3] = 1;
	ImposedSpells_Table[158][1] = 55; ImposedSpells_Table[158][4] = 3;
	ImposedSpells_Table[164][0] = 31; ImposedSpells_Table[164][3] = 3;
	ImposedSpells_Table[165][0] = 33; ImposedSpells_Table[165][3] = 3;
	ImposedSpells_Table[166][0] = 30; ImposedSpells_Table[166][3] = 3;
	ImposedSpells_Table[167][0] = 32; ImposedSpells_Table[167][3] = 3;
	ImposedSpells_Table[169][0] = 36; ImposedSpells_Table[169][3] = 3;
	ImposedSpells_Table[170][0] = 28; ImposedSpells_Table[170][3] = 3;
	ImposedSpells_Table[171][0] = 29; ImposedSpells_Table[171][3] = 3;
	ImposedSpells_Table[192][0] = 49; ImposedSpells_Table[192][3] = 3;
	ImposedSpells_Table[195][0] = 29; ImposedSpells_Table[195][3] = 1;

	ThreeHeadedAttack_Table[47] = 1;
	WriteHook((void*)0x441386, (void*)ThreeHeadedAttack, HOOKTYPE_JUMP);
	WriteHook((void*)0x4487E9, (void*)ThreeHeadedAttack2, HOOKTYPE_JUMP);

	/* Disabled by Archer 16 Feb 2024
	Z_Amethyst->WriteLoHook(0x004690B8, rebirth_004690B8);
	Z_Amethyst->WriteLoHook(0x004690CF, rebirth_004690CF);
	WriteHook((void*)0x75E108, (void*)Rebirth_Plus, HOOKTYPE_JUMP);
	respawn_table_chance[0x83] = 0.2f; respawn_table_chance[0x9e] = 1.0f;
	respawn_table_fraction[0x83] = 0.2f; respawn_table_fraction[0x9e] = 0.2f; 
	respawn_table_sure[0x83] = 0.2f; respawn_table_sure[0x9e] = 0.0f;

	WriteHook((void*)0x75D4A0, (void*)ReduceTargetDefense, HOOKTYPE_JUMP);
	WriteHook((void*)0x4422E3, (void*)ReduceTargetDefense2, HOOKTYPE_JUMP);
	ReduceTargetDefense_Table[96]  = 40;
	ReduceTargetDefense_Table[97]  = 60;
	ReduceTargetDefense_Table[156] = 80;
	*/

	WriteHook((void*) 0x441ADE, (void*) Before_Melee, HOOKTYPE_JUMP);
	// WriteHook((void*) 0x74C82A, (void*) CallTrigger, HOOKTYPE_CALL);
	// WriteHook((void*)0x0043ff7e, (void*)try_ranged_retaliation, HOOKTYPE_CALL);

	isConstruct_Table[30] = isConstruct_Table[31] = isConstruct_Table[32] = isConstruct_Table[33] = 1;
	isConstruct_Table[116] = isConstruct_Table[117] = isConstruct_Table[133] = 1;

	// WriteHook((void*) 0x004E6307, (void*)hook_004E6307, HOOKTYPE_JUMP); // Disabled by Archer 8 Jun 2024: hero spell specialty effect


	for (int i = 0; i < MONSTERS_AMOUNT; ++i)
		Monster_Ban_Table[i] = 1;

	
	for (int i = 0; i <= 144; ++i)
		Monster_Ban_Table[i] = 0;

	Monster_Ban_Table[122] = Monster_Ban_Table[124] = Monster_Ban_Table[126] = Monster_Ban_Table[128] = 1;

	for (int i = 150; i < 160; ++i)
		Monster_Ban_Table[i] = 0;
	for (int i = 168; i < 174; ++i)
		Monster_Ban_Table[i] = 0;
	for (int i = 192; i <= all_creatures; ++i)
		Monster_Ban_Table[i] = 0;

	isTeleporter[54] = isTeleporter[55] = isTeleporter[153] = 1;
	Z_Amethyst->WriteLoHook(0x007609E0, z_hook_007609E0);
	Z_Amethyst->WriteLoHook(0x00760412, z_hook_00760412);

	FillTables();

	/* Disabled by Archer 16 Feb 2024
	if(!DisableNecromancyFeatures)
		InitNecromancy();

	if (FilterNecromancyCreatures)
		Z_Amethyst->WriteLoHook(0x00476F67, z_hook_00476F67);
	*/

	Z_Amethyst->WriteLoHook(0x00492A7D, z_Hook_00492A7D); // Anchangel battle hint proc. Disabling it won't fix the hint. 

	Z_Amethyst->WriteLoHook(0x004645BA, z_hook_004645BA); // Archer: Check good morale
	Z_Amethyst->WriteLoHook(0x0046453E, z_hook_0046453E); // same

	Z_Amethyst->WriteHiHook(0x0071D04E, SPLICE_, EXTENDED_, CDECL_, CrExpBon_DwarfResistFriendly_WoG_hook);
	
	Z_Amethyst->WriteLoHook(0x00756D06, z_hook_00756D06);
	isHellHydra[157] = 1;

	// Z_Amethyst->WriteLoHook(0x004476E9, z_hook_004476E9); // Disabled by Archer 16 Feb 2024. Disabling this hook unlocks BM:U and Santa Gremlin's damage

	Z_Amethyst->WriteHiHook(0x0076DB0C, SPLICE_, EXTENDED_, CDECL_, NPCDeathStare_hook);

	Z_Amethyst->WriteHiHook(0x00757045, SPLICE_, EXTENDED_, CDECL_, CheckCreature2Leave_hook);

	Z_Amethyst->WriteHiHook(0x44A1A0, SPLICE_, EXTENDED_, FASTCALL_, (void*)ResistanceHook);

	Z_Amethyst->WriteLoHook(0x756BA0, z_756BA0_hook);

	Z_Amethyst->WriteLoHook(0x00478C9E, z_hook_00478C9E);

	first_time = false;
}

extern "C" __declspec(dllexport) int _hasSantaGuards(int creature) {
	return hasSantaGuards[creature];
}