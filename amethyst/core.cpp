#include "stdafx.h"
#include "majaczek.h"

extern int conf_creatures;
extern int all_creatures;
extern void creexpo1024(void);

MONSTER_PROP new_monsters_back[MONSTERS_AMOUNT];
MONSTER_PROP new_monsters[MONSTERS_AMOUNT];
CRANIM new_cranim[MONSTERS_AMOUNT];


extern Patcher* globalPatcher;
extern PatcherInstance* Z_Amethyst;

struct _CrState_ {
	int   Index;
	long  Group;     //  0...8,-1 - neutral
	long  SubGroup;  //  0...6,-1 - not available (423D87, 42A0FC)
	Dword Flags;     //  (424354, 42C6C0)
	long  CostRes[7];//  (42B73A)
	long  Fight;     //
	long  AIvalue;   //
	long  Grow;      //  начальное количество монстров для найма
	long  HGrow;     //
	long  HitPoints; //
	long  Speed;     //
	long  Attack;    //
	long  Defence;   //
	long  DamageL;   //
	long  DamageH;   //
	long  NShots;    //
	long  HasSpell;  //  - сколько раз колдует
	long  AdvMapL;   //
	long  AdvMapH;   //
} CrState[MONSTERS_AMOUNT];
const Dword p_CrState = (Dword) (void*) &CrState[0];
constexpr long sizeof_CrState = sizeof(_CrState_);

replace_dword replace_CrState[] = {
	// SendCreatures
	{0x007505D6 + 2, p_CrState + 4}, //0x277386C
	{0x007505EE + 2, p_CrState + 8},
	{0x00750606 + 2, p_CrState + 12},
	{0x0075063D + 3, p_CrState + 16},
	{0x00750658 + 2, p_CrState + 44},
	{0x00750670 + 2, p_CrState + 48},
	{0x00750688 + 2, p_CrState + 52},
	{0x007506A0 + 2, p_CrState + 56},
	{0x007506B8 + 2, p_CrState + 60},
	{0x007506D0 + 2, p_CrState + 64},
	{0x007506E8 + 2, p_CrState + 68},
	{0x00750700 + 2, p_CrState + 72},
	{0x00750718 + 2, p_CrState + 76},
	{0x00750730 + 2, p_CrState + 80},
	{0x00750748 + 2, p_CrState + 84},
	{0x00750760 + 2, p_CrState + 88},
	{0x00750778 + 2, p_CrState + 92},
	{0x00750790 + 2, p_CrState + 96},

	// ReceiveCreatures
	{0x007508D8 + 2, p_CrState + 4}, // 0x277386C
	{0x007508F0 + 2, p_CrState + 8},
	{0x00750908 + 2, p_CrState + 12}, // 0x2773874
	{0x0075093E + 3, p_CrState + 16},
	{0x0075095A + 2, p_CrState + 44},
	{0x00750972 + 2, p_CrState + 48},
	{0x0075098A + 2, p_CrState + 52},
	{0x007509A2 + 2, p_CrState + 56},
	{0x007509BA + 2, p_CrState + 60},
	{0x007509D2 + 2, p_CrState + 64},
	{0x007509EA + 2, p_CrState + 68},
	{0x00750A02 + 2, p_CrState + 72},
	{0x00750A1A + 2, p_CrState + 76},
	{0x00750A32 + 2, p_CrState + 80},
	{0x00750A4A + 2, p_CrState + 84},
	{0x00750A62 + 2, p_CrState + 88},
	{0x00750A7A + 2, p_CrState + 92},
	{0x00750A92 + 2, p_CrState + 96},

	{0,0}
};
const Dword p_new_monsters = (Dword)(void*)&new_monsters[0];
replace_dword replace_CrStateNetwork[] = {
	// SendCreatures
	{0x007505D0 + 2, p_new_monsters + 0},
	{0x007505E8 + 2, p_new_monsters + 4},
	{0x00750600 + 2, p_new_monsters + 16},
	{0x00750636 + 3, p_new_monsters + 32},
	{0x00750652 + 2, p_new_monsters + 60},
	{0x0075066A + 2, p_new_monsters + 64},
	{0x00750682 + 2, p_new_monsters + 68},
	{0x0075069A + 2, p_new_monsters + 72},
	{0x007506B2 + 2, p_new_monsters + 76},
	{0x007506CA + 2, p_new_monsters + 80},
	{0x007506E2 + 2, p_new_monsters + 84},
	{0x007506FA + 2, p_new_monsters + 88},
	{0x00750712 + 2, p_new_monsters + 92},
	{0x0075072A + 2, p_new_monsters + 96},
	{0x00750742 + 2, p_new_monsters + 100},
	{0x0075075A + 2, p_new_monsters + 104},
	{0x00750772 + 2, p_new_monsters + 108},
	{0x0075078A + 2, p_new_monsters + 112},
	// ReceiveCreatures
	{0x007508DE + 2, p_new_monsters + 0},
	{0x007508F6 + 2, p_new_monsters + 4},
	{0x0075090E + 2, p_new_monsters + 16},
	{0x00750945 + 3, p_new_monsters + 32},
	{0x00750960 + 2, p_new_monsters + 60},
	{0x00750978 + 2, p_new_monsters + 64},
	{0x00750990 + 2, p_new_monsters + 68},
	{0x007509A8 + 2, p_new_monsters + 72},
	{0x007509C0 + 2, p_new_monsters + 76},
	{0x007509D8 + 2, p_new_monsters + 80},
	{0x007509F0 + 2, p_new_monsters + 84},
	{0x00750A08 + 2, p_new_monsters + 88},
	{0x00750A20 + 2, p_new_monsters + 92},
	{0x00750A38 + 2, p_new_monsters + 96},
	{0x00750A50 + 2, p_new_monsters + 100},
	{0x00750A68 + 2, p_new_monsters + 104},
	{0x00750A80 + 2, p_new_monsters + 108},
	{0x00750A98 + 2, p_new_monsters + 112},
	{0,0}
};


//// majaczek's commment
// useful adressses:
// 0x00791e80 // CrExpMod::CrExpModStr CrExpMod::Dummy
// 0x00791e94 // int CrExpMod::Ranks[11]
// 
//
//
int __dummy_int = 0;
int __dummy_ptr = (int) &__dummy_int;


void* (__fastcall* FindDefOrMskInAllLods)(const char* name) =
(void* (__fastcall*)(const char*)) (0x0055D160);



/*
char *names[MONSTERS_AMOUNT];
char *names_pl[MONSTERS_AMOUNT];
char *descripts[MONSTERS_AMOUNT];
*/

char names[MONSTERS_AMOUNT][64] = {};
char names_pl[MONSTERS_AMOUNT][64] = {};
char descripts[MONSTERS_AMOUNT][256] = {};

char* ptr_names[MONSTERS_AMOUNT];
char* ptr_names_pl[MONSTERS_AMOUNT];
char* ptr_descripts[MONSTERS_AMOUNT];

char* ptr_names_back[MONSTERS_AMOUNT];
char* ptr_names_pl_back[MONSTERS_AMOUNT];
char* ptr_descripts_back[MONSTERS_AMOUNT];

// CrexpBon__Body
char  experience_0x00847f08_table[(MONSTERS_AMOUNT+64)*0x154]; //sizeof__CrExpBon


// CrExpMod__Body
char  experience_0x0085EB50_table[(MONSTERS_AMOUNT + 64)*0x14]; //sizeof__crexpmodstr
char* experience_0x0085EB54_table = experience_0x0085EB50_table + 0x04;
char* experience_0x0085EB58_table = experience_0x0085EB50_table + 0x08;
char* experience_0x0085EB5C_table = experience_0x0085EB50_table + 0x0c;
char* experience_0x0085EB60_table = experience_0x0085EB50_table + 0x10;
// CrExpoSet__Body
//char experience_0x00860550_table[(MAXCRINFONUM+64)*0x10]; //sizeof__crexpo


//--------------------------------------------------------------------------------------

//char  experience_misc_table[ 24 + (MONSTERS_AMOUNT + 1 + 64) * 20 + 864 + 672 + 65535];
//char* experience_0x0085E950_table = nullptr;

/*
char* experience_0x0085EB48_table = experience_misc_table + 0;
char* experience_0x0085EB50_table = experience_0x0085EB48_table + 8 + (0*20);
char* experience_0x0085EB54_table = experience_0x0085EB50_table + 4;
char* experience_0x0085EB58_table = experience_0x0085EB54_table + 4;
char* experience_0x0085EB5C_table = experience_0x0085EB58_table + 4;
char* experience_0x0085EB60_table = experience_0x0085EB5C_table + 4;
//char experience_0x0085FF50_table[(42 +1) * 20];
char* experience_0x0085FF50_table = experience_0x0085EB50_table + (MONSTERS_AMOUNT*20);

char* experience_0x00860298_table = experience_0x0085FF50_table + 840; // 42 * 20 ??? // 0x00860298
char* experience_0x00860299_table = experience_0x00860298_table + 1;
char* experience_0x0086029A_table = experience_0x00860299_table + 1;
char* experience_0x0086029B_table = experience_0x0086029A_table + 1;
char* experience_0x0086029C_table = experience_0x0086029B_table + 1;
char* experience_0x0086029D_table = experience_0x0086029C_table + 1;
char* experience_0x008602A0_table = experience_0x0086029D_table + 3;
char* experience_0x008602A4_table = experience_0x008602A0_table + 4;
char* experience_0x008602A8_table = experience_0x008602A4_table + 4;
char* experience_0x008602A9_table = experience_0x008602A8_table + 1;
char* experience_0x008602AA_table = experience_0x008602A9_table + 1;

char* experience_0x008602B0_table = experience_0x0085FF50_table + 864; //why such constant?
char* experience_0x00860550_table = experience_0x008602B0_table + 672; //why such constant?
*/

//char experience__table[(MAXCRINFONUM+1) * 16];
//char experience_modifier_table[(MONSTERS_AMOUNT + 1 + 42 + 1) * 20];


int i_ExistingMonstr = 197;                  /* кол-во стандартных монстров */

//адреса где встречаются указатели на структуру монстров
int p_AddressOfPatchStrMonstr[] = {
0x0047ADD1, 0x006747B0, 0x007505D2, 0x007508E0, 
0x00756455, 0x00756473, 0x00756482, 0x00756492, 
0x007564A1, 0x00760F55, 0x00760F55, 0x007613AE, 
0x007613CA, 0x0076150C, 0x0076150C, 0x007626E0, 
0x0076C05C, 0x0076DBED, 0x0078A350, 0x0078A35C, 
0x0078BFAC
};

// адреса где встречаются указатели на структуру CrAnim
int p_AddressOfPatchCrAnim [] = {0x50CC04, 0x67FF74};

// адреса где встречаются указатели на структуру св-в при атаке
int p_AddressOfPatchAtack [] = {0x440237};

// адреса где встречаются указатели на структуру св-в при защите
int p_AddressOfPatchDefend [] = {0x440916};

// адреса где встречаются указатели на структуру св-в 
int p_AddressOfPatchOption [] = {0x447475};

// адрес таблицы снарядов
//int p_AddressOfShell [] = {0x3DA9F};


int p_AddressOfERMLimit[] = { 0x00756410, 0x007504F8, 0x00732CA9, 0x00733326,
									0x007324BD, 0x00714ECA, 0x0071EF50, 0x0072476B,
									0x00724A89, 0x00724B15, 0x005213E5, 0x00721818,
									0x004A1657, 0x004A189C, 0x0047ADED, 0x0047AE7D,
									0x0047B04E, 0x0047B0E6, 0x0047B106, 0x0047B126,
									0x0040AC92, 0x0040ACF3, 0x0040C2B1, 0x00416949,
									0x0043F9B3, 0x0043FA89, 0x0043FDA9, 0x0043FE26,
									0x004401B6, 0x004409D8, 0x00440A22, 0x00440A86,
									0x00440C9B, 0x00440CD6, 0x00440D4A, 0x00440D87,
									0x00440F5C, 0x004418D1, 0x0044192F, 0x00443266,
									0x0044329B, 0x00443308, 0x0044333D, 0x0044372C,
									0x00443798, 0x00446C87, 0x00446CF5, 0x0044730F,
									0x0044BB15, 0x0044BC65, 0x0044C062, 0x00464671,
									0x0046487B, 0x00464A92, 0x00465243, 0x004652E5,
									0x00478594, 0x004785CC, 0x004790D8, 0x00479143,
									0x00479280, 0x0047931C, 0x004840B3, 0x004863FE,
									0x00491F66, 0x00491FB6, 0x004920AB, 0x004921A3,
									0x004921EE, 0x004922B7, 0x004922EE, 0x0049233F,
									0x00492376, 0x00492522, 0x004925BF, 0x00492630,
									0x004926CA, 0x0049275E, 0x004927E9, 0x0049295A,
									0x00492D0C, 0x00492DAE, 0x0049E707, 0x0049E72A,
									0x0049EE74, 0x0049EFC1, 0x004A0437, 0x004A04D9,
									0x004A17DA, 0x004A1804, 0x004A1829, 0x004A184E,
									0x004A19FC, 0x004A1AA1, 0x004A42F2, 0x004A6A7F,
									0x004A6C98, 0x004A6F59, 0x004A6FC7, 0x004AB895,
									0x004ABC6D, 0x004ABF69, 0x004AE280, 0x004AE2CE,
									0x004CC86E, 0x004CC8CA, 0x004CC989, 0x004DBA62,
									0x004DBA9F, 0x004DBAE2, 0x004DBB2E, 0x004DBB75,
									0x004DBB98, 0x004F1F89, 0x004F2011, 0x004F20D7,
									0x004F2139, 0x004F479C, 0x004F5BD2, 0x004F5C4C,
									0x0051402F, 0x00514079, 0x005213E6, 0x0052141E,
									0x005217C8, 0x00521813, 0x00521837, 0x0052191D,
									0x0052195D, 0x005219BE, 0x005219CF, 0x005219F9,
									0x0052FEF9, 0x0052FF5D, 0x0052FFBB, 0x0055036F,
									0x00550826, 0x0055113F, 0x00551D90, 0x005632DC,
									0x00565366, 0x0056637C, 0x005664C9, 0x0056EBD4,
									0x0056ECD5, 0x0056F116, 0x005706A6, 0x00570A6F,
									0x00570D4E, 0x0059F90C, 0x005A21EE, 0x005A22E7,
									0x005A2CC4, 0x005A2DCF, 0x005A2E29, 0x005A753B,
									0x005A77C2, 0x005A77FD, 0x005A79B3, 0x005A79EE,
									0x005A7D4D, 0x005A7DBD, 0x005A8A0A, 0x005A8A6B,
									0x005A8B54, 0x005A9379, 0x005B0E0B, 0x005C08EC,
									0x005C605D, 0x005C656B, 0x005C789B, 0x005C7947,
									0x005C79BD, 0x005C7A1E, 0x005C7AB1, 0x005C7B27,
									0x005C7B49, 0x005C7FD5, 0x005C8047, 0x005C84B1,
									0x005C9C3B, 0x005D0C57, 0x005D105B, 0x005DD0A5,
									0x005DD977, 0x005DDA4B, 0x005EF08D, 0x005F393A,
									0x005F40CD,

	/*0x718151+2,*/ 0x71A9F5 + 3, /*0x71B02A+3, 0x723970+3,
	0x723B63+3, 0x723D55+3, 0x726124+3, 0x726174+3,
	0x726363+3, 0x7263CD+3, 0x73A847+3, 0x7401CB+3,
	0x74027F+3, 0x7402F8+3,*/ 0x74EC69 + 6, 0x74ED06 + 6,/*
	0x750490+3, 0x750548+3, */ 0x750599 + 3, 0x75081E + 3,
	0x7508A5 + 3, /*  0x750D8A+3, 0x7521A4+3, */0x752E27 + 3,/*,
	0x752F08+3, */

	0x756A40 + 3, 0x756A9A + 3, //responsible for zghost.txt

	0x760F3E + 3, 0x7614DD + 3, //responsible for wog saver/loader
	// SadnessPower
	0x7504F5 + 3, 0x750490 + 3, 0x750548 + 3
};

int p_AddressOfERMLimit2[]=
{

								/*
									// should fix something 2021-07-15
									0x00723970+3, 0x00723B63+3, 0x00723D55+3, 0x00726124+3,
									0x00726174+3, 0x00726363+3, 0x007263CD+3,
									0x007402F8+3, 0x00750490+3, 0x00750548+3, 0x00750D8A+3,
									0x007401CB+3, 0x0074027F+3, 0x00752F08+3,
									0x00718151+2, 0x007521A4+3, 
									0x00749E4E+1, 0x0076B2DD+1,
									0x004288E3+1, 0x004288A3+3,
								*/

									0x756A40 + 3, 0x756A9A + 3, //responsible for zghost.txt

									0x760F3E + 3, 0x7614DD + 3 //responsible for wog saver/loader
									
							};

void set_erm_limits(void) {
	for (int i = 0; i != (sizeof(p_AddressOfERMLimit) / 4); i++)
	{
		*(int*)(p_AddressOfERMLimit[i]) = (int)MONSTERS_AMOUNT;
	}

	for (int i = 0; i != (sizeof(p_AddressOfERMLimit2) / 4); i++)
	{
		*(int*)(p_AddressOfERMLimit2[i]) = all_creatures + 1;
	}
}
                                 
int p_AddressOfERM [] = {   0x00756455, 0x007564CA, 0x007568C5, 0x00733440, 
							0x0071F252, 0x00724E05, 0x00733507, 0x00750DA5, 
							0x00733475, 0x0071F26E, 0x00733533, 0x00750DBB, 
							0x007334AB, 0x0073355F, 0x00750DD1, 0x00756579, 
							0x00750639, 0x00750948, 0x00760FD2, 0x00761589, 
							0x00760FFC, 0x007615B3, 0x00761025, 0x007615DC, 
							0x0076104F, 0x00761606, 0x00761079, 0x00761630, 
							0x007610A2, 0x00761659, 0x007610CC, 0x00761683, 
							0x007565AF, 0x00750654, 0x00750962, 0x007610F6, 
							0x007616AD, 0x007565E5, 0x0075066C, 0x0075097A, 
							0x0076111F, 0x007616D6, 0x0075661B, 0x00750684, 
							0x00750992, 0x00761149, 0x00761700, 0x00756651, 
							0x0075069C, 0x007509AA, 0x00761173, 0x0076172A, 
							0x00756687, 0x007506B4, 0x007509C2, 0x0076119C, 
							0x00761753, 0x007566BD, 0x007506CC, 0x007509DA, 
							0x007566DB, 0x007566EA, 0x007611C6, 0x0076177D, 
							0x00756713, 0x007506E4, 0x007509F2, 0x007611F0, 
							0x007617A7, 0x0075674A, 0x007506FC, 0x00750A0A, 
							0x00761219, 0x007617D0, 0x00756781, 0x00750714, 
							0x00750A22, 0x00761243, 0x007617FA, 0x007567B8, 
							0x0075072C, 0x00750A3A, 0x0076126D, 0x00761824, 
							0x007567EF, 0x00750744, 0x00750A52, 0x00761296, 
							0x0076184D, 0x00756826, 0x0075075C, 0x00750A6A, 
							0x007612C0, 0x00761877, 0x0075685D, 0x00750774, 
							0x00750A82, 0x007612EA, 0x007618A1, 0x00756894, 
							0x0075078C, 0x00750A9A, 0x00761310, 0x007618CA, 
							0x0070B54C, 0x00718EC4, 0x00718F2B, 0x0071AA0F, 
							0x0071AFAC, 0x0071AFBB, 0x0071AFC7, 0x0071AFD9, 
							0x0071AFE8, 0x0071B054, 0x0071B480, 0x0071B48F, 
							0x0071B49B, 0x0071B4AD, 0x0071B4BC, 0x00721826, 
							0x007219BA, 0x00723981, 0x00723B74, 0x00723D66, 
							0x007275ED, 0x007505EA, 0x00750602, 0x007508F8, 
							0x00750910, 0x00752267, 0x0075229D, 0x007522D2, 
							0x007564E8, 0x007564F7, 0x00756507, 0x00756516, 
							0x00757341, 0x0075735D, 0x00757382, 0x0075A0B5, 
							0x0075A7D3, 0x0075A83C, 0x0075DB00, 0x0075DB20, 
							0x0075DB4A, 0x00760F7F, 0x00760FA8, 0x00761536, 
							0x0076155F, 0x00767679, 0x0076768A, 0x0076798B, 
							0x0076C99C, 0x0076C9AB};

                            
int i_OffERM [] = {0x00, 0x04, 0x10, 0x14, 0x14, 0x14, 0x14, 0x14,
				   0x18, 0x18, 0x18, 0x18, 0x1C, 0x1C, 0x1C, 0x20,
				   0x20, 0x20, 0x20, 0x20, 0x24, 0x24, 0x28, 0x28,
				   0x2C, 0x2C, 0x30, 0x30, 0x34, 0x34, 0x38, 0x38,
				   0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x40, 0x40, 0x40,
				   0x40, 0x40, 0x44, 0x44, 0x44, 0x44, 0x44, 0x48,
				   0x48, 0x48, 0x48, 0x48, 0x4C, 0x4C, 0x4C, 0x4C,
				   0x4C, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50,
				   0x54, 0x54, 0x54, 0x54, 0x54, 0x58, 0x58, 0x58,
				   0x58, 0x58, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x60,
				   0x60, 0x60, 0x60, 0x60, 0x64, 0x64, 0x64, 0x64,
				   0x64, 0x68, 0x68, 0x68, 0x68, 0x68, 0x6C, 0x6C,
				   0x6C, 0x6C, 0x6C, 0x70, 0x70, 0x70, 0x70, 0x70,
				   0x4C, 0x04, 0x04, 0x04, 0x10, 0x10, 0x64, 0x10,
				   0x10, 0x04, 0x10, 0x10, 0x64, 0x10, 0x10, 0x18,
				   0x18, 0x18, 0x18, 0x18, 0x04, 0x04, 0x04, 0x04,
				   0x10, 0x14, 0x18, 0x1C, 0x04, 0x04, 0x04, 0x04,
				   0x14, 0x18, 0x18, 0x40, 0x4C, 0x4C, 0x10, 0x10,
				   0x10, 0x04, 0x10, 0x04, 0x10, 0x14, 0x18, 0x4C,
				   0x10, 0x10
};


//новая таблица ссылок на имена существ в  ед. числе (29834C0h/43529408):
int p_AddressOfNames[] =   {0x00733459, 0x0073351B, 0x00750C72, 0x00750D14, 
							0x00750D9F, 0x00752280, 0x0047AE2B, 0x0047ADF6, 
							0x0047B12D}; 
//новая таблица ссылок на имена существ во мн. числе (29834C0h/43529408):

int p_AddressOfPlNames[] = {0x0073348E, 0x00733547, 0x00750CA1, 0x00750D43, 
							0x00750DB5, 0x007522B5, 0x0076C532, 0x0076C5AC, 
							0x0047AEBF, 0x0047AE86, 0x0047B10D};

//новая таблица ссылок на описания существ (2984460h/43533408):
int p_AddressOfDesc [] = {  0x007334C3, 0x00733573, 0x00750CD0, 0x00750D72, 
							0x00750DCB, 0x007522EB, 0x0047B094, 0x0047B058, 
							0x0047B0ED};

int p_Experience_0x00847f08_Table [] = // CrexpBon__Body
{
0x71AB36+1,//  PUSH OFFSET Era_1_6.00847F08
0x71AB9E+1,//  PUSH OFFSET Era_1_6.00847F08
0x71AD82+1,//  PUSH OFFSET Era_1_6.00847F08
0x71B040+2,//  ADD ECX,OFFSET Era_1_6.00847F08
0x71B24F+2,//  ADD EDX,OFFSET Era_1_6.00847F08
0x7280FD+1,//  ADD EAX,OFFSET Era_1_6.00847F08
0x728137+1,//  ADD EAX,OFFSET Era_1_6.00847F08
0x728186+2 // ADD ECX,OFFSET Era_1_6.00847F08
}; // CRExpBon__Body

int p_CURMON_table[] = { 
// from wog sources: #define CURMON 1000
// by majaczek: 1989 (anything above MONSTERS_AMOUNT works)


						// function entry 0x0071c16d
						// static int Prepare(Byte *Mon,int checknum=1)
	0x0071c259 + 1,		// PUSH 3E8


						// function entry 0x0071d4c8
						// static int Prepare2(_Hero_ *Hp,int Stack){
	0x0071d599 + 1,		// PUSH 3E8

						// function entry 0x0071ef2b	-	5 occurencies
						// void CrExpBon::PrepareInfo(int Mon,int Num,int Expo,CrExpo *Cr,int Changing)
	0x0071f19b + 1,		// PUSH 3E8
	0x0071f34d + 1,		// PUSH 3E8
	0x0071f4d3 + 1,		// PUSH 3E8
	0x0071f519 + 1,		// PUSH 3E8
	0x0071f56f + 1,		// PUSH 3E8
	


						// function entry 0x00724c0e	-	14 occurencies
						// int OnControlClick(int ContorIndex,int MouseButton,int ShiftState)
	0x00724c58 + 1,		// PUSH 3E8
	0x00724c98 + 1,		// PUSH 3E8
	0x00724cc5 + 1,		// PUSH 3E8
	0x00724ce6 + 1,		// PUSH 3E8
	0x00724d00 + 1,		// PUSH 3E8
	0x00724d13 + 1,		// PUSH 3E8
	0x00724d2d + 1,		// PUSH 3E8
	0x00724d4e + 1,		// PUSH 3E8
	0x00724d5c + 1,		// PUSH 3E8
	0x00724d6c + 1,		// PUSH 3E8
	0x00724d87 + 1,		// PUSH 3E8
	0x00724d96 + 1,		// PUSH 3E8
	0x00724daf + 1,		// PUSH 3E8
	0x00724dc6 + 1,		// PUSH 3E8

						// function entry 0x00727fb0

	0x007278b3 + 3,		// CMP DWORD PTR SS:[EBP+8],3E8
						// function entry 0x007278b0


	0x00728093 + 3		// CMP DWORD PTR SS:[EBP+8],3E8
						// function entry 0x00728090


};

/*
int p_experience_0x0085EB48_table[] = {
	// by majaczek
	0x0071ef74 + 2, // MOV DWORD PTR DS : [85EB48],EDX
	0x00724c52 + 1, // MOV EAX,DWORD PTR DS:[85EB48]
	0x00724cb2 + 2, // MOV ESI,DWORD PTR DS:[85EB48]
	0x00724dd3 + 2, // SUB EAX,DWORD PTR DS:[85EB48]
	0x00724dda + 1  // MOV EAX,DWORD PTR DS:[85EB48]
};
*/

int p_experience_0x0085EB50_table[] = { // CrExpMod__Body
// by majaczek
	0x0071a60d +1, // PUSH OFFSET 0085EB50
	0x0071a675 +1, // PUSH OFFSET 0085EB50
	0x0071a727 +1, // PUSH OFFSET 0085EB50
	//
	 0x0071a8d9 +2, // MOV DWORD PTR DS:[EDX+85EB50],EAX
	 0x0071aa39 +2, // MOV DWORD PTR DS:[EAX+85EB50],EDX
	//
	0x007278f7 +1  // ADD EAX,OFFSET 0085EB50
};


int p_experience_0x0085EB54_table[] = {
	// by majaczek
	0x0071a907 + 2, // MOV DWORD PTR DS:[EDX+85EB54],EAX
	0x0071aa4f + 2  // MOV DWORD PTR DS:[EAX+85EB54],EDX
};
int p_experience_0x0085EB58_table[] = {
	// by majaczek
	0x0071a935 + 2, // MOV DWORD PTR DS : [EDX + 85EB58],EAX
	0x0071a9ea + 2, // CMP DWORD PTR DS:[EAX+85EB58],0
	0x0071aa67 + 2  // MOV DWORD PTR DS:[ECX+85EB58],EDX
};
int p_experience_0x0085EB5C_table[] = {
	// by majaczek
	0x0071a991 + 2, // MOV DWORD PTR DS:[EDX+85EB5C],EAX
	0x0071aa97 + 2  // MOV DWORD PTR DS:[ECX+85EB5C],EDX
};
int p_experience_0x0085EB60_table[] = {
	// by majaczek
	0x0071a963 + 2, // MOV DWORD PTR DS:[EDX+85EB60],EAX
	0x0071aa7f + 2  // MOV DWORD PTR DS : [ECX + 85EB60],EDX
};


/*
int p_experience_0x0085FF50_table[] = { //CrExpMod__BFBody
	// by majaczek
	0x00727182 + 1, // PUSH OFFSET 0085FF50
	0x00727796 + 1, // PUSH OFFSET 0085FF50
	0x00762dec + 1, // PUSH OFFSET 0085FF50
	//
	0x007272b7 + 2, // ADD EDX,OFFSET 0085FF50
	0x00727445 + 2, // ADD EDX,OFFSET 0085FF50
	0x007278ea + 1, // ADD EAX,OFFSET 0085FF50
	0x0072824c + 1, // ADD EAX,OFFSET 0085FF50
	//
	0x00762d96 + 2  // MOV DWORD PTR DS:[ECX],OFFSET 0085FF50
};
*/


/*
int p_experience_0x00860298_table[] = {
	// by majaczek
	0x00717363 + 3 // MOV DWORD PTR SS:[EBP-20],860298
};


int p_experience_0x00860299_table[] = {
	// by majaczek
	0x007173ab + 3 // MOV DWORD PTR SS:[EBP-4],860299
};


// from 0x0086029A to 0x0086029D 
int p_experience_0x0086029A_table[] = {
	// by majaczek
	0x00717470 + 3 // MOV DWORD PTR SS:[EBP-4],86029a
};

int p_experience_0x0086029B_table[] = {
	// by majaczek
	0x00717477 + 3 // MOV DWORD PTR SS:[EBP-4],86029b
};
int p_experience_0x0086029C_table[] = {
	// by majaczek
	0x00717505 + 3 // MOV DWORD PTR SS:[EBP-4],86029c
};
int p_experience_0x0086029D_table[] = {
	// by majaczek
	0x00717510 + 3 // MOV DWORD PTR SS:[EBP-4],86029d
};
*/


/*
// from 0x008602A8 to 0x008602AA 
int p_experience_0x008602A8_table[] = {
	// by majaczek
	0x0071F1c7 + 1 //PUSH 8602A8 
};
int p_experience_0x008602A9_table[] = {
	// by majaczek
	0x00724843 + 2 // MOV DWORD PTR DS:[8458BC],8602A9
};
int p_experience_0x008602AA_table[] = {
	// by majaczek
	0x00724b95 + 2 // MOV DWORD PTR DS:[8458BC],8602AA
};
*/

/*
int p_experience_0x008602A0_table[] = {
	// by majaczek
	0x00727136 + 2, // MOV DWORD PTR DS:[8602A0],1 
	0x007277c8 + 2, // MOV DWORD PTR DS:[8602A0],0
	0x0076486e + 2  // CMP DWORD PTR DS:[8602A0],0
};


int p_experience_0x008602A4_table[] = {
	// by majaczek
	0x0071ad43 + 2, // CMP DWORD PTR DS:[8602A4],0 
	0x0071ad71 + 2  // MOV DWORD PTR DS:[8602A4],1
};


int p_experience_0x008602B0_table[] = {
	// by majaczek
	0x00726c9b + 2, // ADD EDX,8602B0
	0x0072715a + 1, // PUSH 8602B0
	0x00727328 + 2, // ADD ECX,8602B0
	0x0072736a + 2, // ADD ECX,8602B0
	0x007274b3 + 2, // ADD ECX,8602B0
	0x007274e0 + 2, // ADD ECX,8602B0
	0x0072776e + 1, // PUSH 8602B0
	0x00728119 + 1, // ADD EAX,8602B0
	0x007620ea + 2, // ADD EDX,8602B0
	0x00762c72 + 2, // MOV DWORD PTR DS:[ECX],8602B0
	0x00762cc8 + 1, // PUSH 8602B0
	0x007843cc + 1, // PUSH 8602B0
	0x0078457c + 1  // PUSH 8602B0

};
*/

/*
int p_experience_modifier_table[] = {
	0x0071a734 + 1, // PUSH OFFSET 00840f8c
	0x0071a77a + 1, // PUSH OFFSET 00840f8c
	0x0071a7b6 + 1, // PUSH OFFSET 00840f8c
	0x0071a7e2 + 1, // PUSH OFFSET 00840f8c
	0x0071a80e + 1, // PUSH OFFSET 00840f8c
	0x0071a83c + 1, // PUSH OFFSET 00840f8c
	0x0071a86a + 1, // PUSH OFFSET 00840f8c
	0x0071a8b1 + 1, // PUSH OFFSET 00840f8c
	0x0071a8df + 1, // PUSH OFFSET 00840f8c
	0x0071a90d + 1, // PUSH OFFSET 00840f8c
	0x0071a93b + 1, // PUSH OFFSET 00840f8c
	0x0071a969 + 1, // PUSH OFFSET 00840f8c
	0x0071a99c + 1  // PUSH OFFSET 00840f8c
};
*/

/*
int* Exe2Mem (int adr)
{
    if (adr < 0x29C000)	return ((int*)(adr + 0x400000));
    else				return ((int*)(adr + 0x465000));        
}
*/

struct _CrChanged_ {
	int DayOfSet;
} CrChanged[MONSTERS_AMOUNT];

const int newNetworkBufferSize = 1000000;
char networkBuffer[newNetworkBufferSize + 5000];

void install_network_remappings()
{
	// CrTraits 
	for (int i = 0; replace_CrStateNetwork[i].address; ++i)
		*(int*)replace_CrStateNetwork[i].address = replace_CrStateNetwork[i].new_dword;

	memcpy((void*)CrChanged, (void*)0x8A3210, sizeof(_CrChanged_) * 197);

	int p_AddressOfCrChanged[] = { 0x008A3210, 0x0075049C + 3, 0x007504AE + 3, 0x007504BD + 3,
								0x00750521 + 3, 0x00750554 + 3, 0x007505A9 + 3 };


	for (int i = 0; i != (sizeof(p_AddressOfCrChanged) / 4); i++)
	{
		*(int*)(p_AddressOfCrChanged[i]) = (int)CrChanged;
	}

	/*
	// CrExpBon::BFBody - Send -> this mapping is WRONG, the addresses are correct but the table is wrong. Anyway, there is no need to be mapped.
	*(int*)(0x00762CFB + 2) = (int)sizeof(experience_0x00847f08_table);
	*(int*)(0x00762D04 + 2) = (int)experience_0x00847f08_table;
	// CrExpBon::BFBody - Receive -> this mapping is WRONG, the addresses are correct but the table is wrong. Anyway, there is no need to be mapped.
	*(int*)(0x00762D2D + 3) = (int)sizeof(experience_0x00847f08_table);
	*(int*)(0x00762D51 + 1) = (int)sizeof(experience_0x00847f08_table);
	*(int*)(0x00762D5A + 1) = (int)experience_0x00847f08_table;

	// Buffer size increase, for further use (not needed now)

	int bufferSizeRemapping[] = {
								  0x00763208 + 2,
								  0x0076327D + 1,
								  0x00763316 + 2,
								  0x0076338B + 1,
								  0x007633FB + 1,
								  0x00763495 + 2,
								  0x00763506 + 2,
								  0x00763577 + 2,
								  0x007635E8 + 2,
								  0x00763659 + 2,
								  0x007636CA + 2,
								  0x0076373B + 2,
								  0x00763C0E + 3   // Prepare2Send
	};

	for (int i = 0; i != (sizeof(bufferSizeRemapping) / 4); i++)
	{
		Z_Amethyst->WriteDword(bufferSizeRemapping[i], newNetworkBufferSize);
	}

	int bufferRemapping[] = {
							  0x00763CF2 + 2,
							  0x00763C1A + 2,
							  0x007637AE + 2,
							  0x007632C6 + 2,
							  0x007633D4 + 2,
							  0x00763444 + 2,
							  0x00763C4A + 2,
							  0x00763252 + 2,
							  0x00763360 + 2,
							  0x007634DF + 2,
							  0x00763550 + 2,
							  0x007635C1 + 2,
							  0x00763632 + 2,
							  0x007636A3 + 2,
							  0x00763714 + 2,
							  0x00763785 + 2,
							  0x0076321B + 2,
							  0x00763329 + 2,
							  0x007634A8 + 2,
							  0x00763519 + 2,
							  0x0076358A + 2,
							  0x007635FB + 2,
							  0x0076366C + 2,
							  0x007636DD + 2,
							  0x0076374E + 2,
							  0x0076379C + 2,
							  0x007631A9 + 2,
							  0x0076328F + 2,
							  0x0076339D + 2,
							  0x0076340D + 2,
							  0x00763193 + 2
							};

	for (int i = 0; i != (sizeof(bufferRemapping) / 4); i++)
	{
		Z_Amethyst->WriteDword(bufferRemapping[i], (int)networkBuffer);
	}

	// Remapping buffer
	*(int*)(0x02846C60) = (int)networkBuffer;  */
}


void install_CrState() {
	for (int i = 0; replace_CrState[i].address; ++i)
		*(int*)replace_CrState[i].address = replace_CrState[i].new_dword;

	*(int*)(0x007505BE + 2) = p_CrState; //0x2773868

	*(unsigned char*)0x00750868 = 0x8B;
	*(unsigned char*)0x0075086A = 0x89;
	*(int*)(0x0075086A + 2) = p_CrState;

	*(int*)(0x00750896 + 2) = p_CrState;


	*(int*)(0x007507A7 + 2) = p_CrState;
}

const char *dummydef="CHALF.def";
const char *dummysnd="lich";

char fake_new_buffer[3][512];
int __cdecl fake_new(int Size2Allocate){
	static int i = -1; (++i) %= 3;
	return (int) fake_new_buffer[i];
}
_LHF_(fake_new_hook) {
	c->eax = fake_new(c->Pop());
	return NO_EXEC_DEFAULT;
}
int  __stdcall  LoadCrTraitsRow_hook(HiHook* h, int mon, int a2) {
	__asm pushad
	if (!*(char*)0x0696690 & 7) {

	}

	*(char*)0x0696690 |= 7;
	int ret = CALL_2(int, __fastcall, h->GetDefaultFunc(),mon,a2);
	strcpy_s(names[mon], new_monsters[mon].name);
	strcpy_s(names_pl[mon], new_monsters[mon].pluralname);
	strcpy_s(descripts[mon], new_monsters[mon].features);
	new_monsters[mon].name = ptr_names[mon] = names[mon];
	new_monsters[mon].pluralname= ptr_names_pl[mon] = names_pl[mon];
	new_monsters[mon].features = ptr_descripts[mon] = descripts[mon];

	__asm popad
	return ret;
}
int LoadCrTraitsRow_lastmon = -1;
_LHF_(Hook_0047AE12) {
	LoadCrTraitsRow_lastmon = c -> esi;
	// c->edx = c->eax = (int) names[LoadCrTraitsRow_lastmon];
	// c->return_address = 0x0047AE26;
	// c->return_address = 0x0047AE5C;
	
	c->ecx = -1; c->esi = *(int*)*(int*)(c->ebx + 4);
	c->edx = c->edi = (int)names[LoadCrTraitsRow_lastmon];
	c->return_address = 0x0047AE3F;
	return NO_EXEC_DEFAULT;
}
_LHF_(Hook_0047AEA2) {
	// c->edx = c->eax = (int)names_pl[c->esi];
	
	//c->return_address = 0x0047AEB7; //= 0x0047AEBA;
	// c->return_address = 0x0047AEEB;

	c->ecx = -1;
	c->edx = c->edi = (int)names_pl[LoadCrTraitsRow_lastmon];
	c->esi = *(int*)(*(int*)(c->ebx + 4) + 4);
	c->return_address = 0x0047AED1;

	return NO_EXEC_DEFAULT;
}
_LHF_(Hook_0047B074) {
	// c->edx = c->eax = (int)descripts[LoadCrTraitsRow_lastmon];
	// c->return_address = 0x0047B089;

	c->eax = *(int*)(c->ebp-4);
	// c->return_address = 0x0047B0C3;
	c->esi = *(int*)(*(int*)(c->ebx + 4) + 92);
	c->edx = c->edi = (int)descripts[LoadCrTraitsRow_lastmon];
	c->return_address = 0x0047B0A5;
	return NO_EXEC_DEFAULT;
}


struct _MonNames_ {
	int Var[3];
};

_MonNames_ MonNames_Var[MONSTERS_AMOUNT];

void CreateNewTable()
{    

	//copying old tables
		for(int i=197; i!=MONSTERS_AMOUNT;i++)
		{
			new_monsters[i].defname = (char*)malloc(16);
			new_monsters[i].soundname = (char*)malloc(5);

			sprintf(new_monsters[i].defname,"CR%03i.def\0",i);
			sprintf(new_monsters[i].soundname,"s%03i\0",i);


			new_monsters[i].adv_high = 0;
			new_monsters[i].adv_low  = 0;
		

			new_monsters[i].level = 2;
			new_monsters[i].flags = 0;
			new_monsters[i].town = -1;


		}


		memcpy((void*)new_monsters,(void*)0x7D0C90,sizeof(MONSTER_PROP)*197);
		memcpy((void*)new_cranim,(void*)0x7B8610,sizeof(CRANIM)*197);
	//table transfer

		// werewolf fix
		*(int*)(0x00767619 + 1) = ((int)new_monsters) + 0x5834;

		// arrow towers fix
		// *(int*)(0x004C69AF + 3) = ((int)new_monsters) + 0x4384;

		/*
		for (int i = 0; i < 197;++i) {
			char z_defname[16] = {};
			char z_soundname[5] = {};

			sprintf(z_defname, "CR%03i.def\0", i);
			sprintf(z_soundname, "s%03i\0", i);

			if (FindDefOrMskInAllLods(z_defname)) {
				new_monsters[i].defname = (char*)malloc(16);
				strcpy(new_monsters[i].defname,z_defname);
			}

			char zz_soundname[16] = {};
			sprintf(zz_soundname,"%s%s",z_soundname,"MOVE");
			if (FindDefOrMskInAllLods(zz_soundname)) {
				new_monsters[i].soundname = (char*)malloc(5);
				strcpy(new_monsters[i].soundname, z_soundname);
			}
		}
		*/

		for(int i=0; i!=(sizeof(p_AddressOfPatchStrMonstr)/4); i++)
		{
			*(int*)(p_AddressOfPatchStrMonstr[i]) = (int)new_monsters;
		}

		for(int i=0; i!=(sizeof(p_AddressOfPatchCrAnim)/4); i++)
		{
			*(int*)(p_AddressOfPatchCrAnim[i]) = (int)new_cranim;
		}

	//removal of restrictions from ERM
		for(int i=0; i!=(sizeof(p_AddressOfERM)/4); i++)
		{
			*(int*)(p_AddressOfERM[i]) = (((int)new_monsters) + i_OffERM[i]);
		}

		for(int i=0; i!=(sizeof(p_AddressOfERMLimit)/4); i++)
		{
			*(int*)(p_AddressOfERMLimit[i]) =  (int)MONSTERS_AMOUNT;
		}
	//new tables for names and such
		
		/*
		for(int i=0; i!=(sizeof(names)/4); i++)
		{
			new_monsters[i].name =  
				names[i] = (char*)malloc(64);
			new_monsters[i].pluralname = 
				names_pl[i] = (char*)malloc(64);
			new_monsters[i].features =
				descripts[i] = (char*)malloc(256);
		}
		*/

		// Archer: LoadCrTraitsRow
		Z_Amethyst->WriteLoHook(0x0047AE21, fake_new_hook);
		Z_Amethyst->WriteLoHook(0x0047AEB2, fake_new_hook);
		Z_Amethyst->WriteLoHook(0x0047B084, fake_new_hook);
		Z_Amethyst->WriteHiHook(0x0047ADB0, SPLICE_, EXTENDED_, FASTCALL_, LoadCrTraitsRow_hook);
		// Z_Amethyst->WriteLoHook(0x0047AE12, Hook_0047AE12);
		// Z_Amethyst->WriteLoHook(0x0047AEA2, Hook_0047AEA2);
		// Z_Amethyst->WriteLoHook(0x0047B074, Hook_0047B074);
		// *(BYTE*)(0x0047ade0) = *(BYTE*)(0x0047ae6e) = *(BYTE*)(0x0047b03f) = 0xEB;
		for (int i = 0; i < MONSTERS_AMOUNT; i++)
		{
			/*
			new_monsters[i].name =
				names[i];
			new_monsters[i].pluralname =
				names_pl[i];
			new_monsters[i].features =
				descripts[i];
			*/
			ptr_names[i] = names[i];
			ptr_names_pl[i] = names_pl[i];
			ptr_descripts[i] = descripts[i];
		}

		for(int i=0; i!=(sizeof(p_AddressOfPlNames)/4); i++)
		{
			*(int*)(p_AddressOfPlNames[i]) =  (int)ptr_names_pl;
		}

		for(int i=0; i!=(sizeof(p_AddressOfNames)/4); i++)
		{
			*(int*)(p_AddressOfNames[i]) =  (int)ptr_names;
		}

		for(int i=0; i!=(sizeof(p_AddressOfDesc)/4); i++)
		{
			*(int*)(p_AddressOfDesc[i]) =  (int)ptr_descripts;
		}

	
		/*
		for(int i=0; i!=(sizeof(p_Experience_0x00847f08_Table)/4); i++)
		{
			*(int*)(p_Experience_0x00847f08_Table[i]) =  (int)experience_0x00847f08_table;
		}
		*/


	//loading from config
		*(int*)0x47AD78 = conf_creatures; //zcraits
		*(int*)0x50CBB8 = conf_creatures;

	//ResetMonTable
		*(int*)(0x007613A8 + 1) = (int)new_monsters_back;
		*(int*)(0x007613CE + 1) = (int)new_monsters_back;
		*(int*)(0x007613A3 + 1) = sizeof(new_monsters);
		*(int*)(0x007613C4 + 1) = sizeof(new_monsters);

		Z_Amethyst->WriteDword(0x0073338D + 3, (long)MonNames_Var);
		Z_Amethyst->WriteDword(0x007333C0 + 3, (long)MonNames_Var);
		Z_Amethyst->WriteDword(0x007514F3 + 1, (long)MonNames_Var);
		Z_Amethyst->WriteDword(0x00752184 + 1, (long)MonNames_Var);
		Z_Amethyst->WriteDword(0x007521EE + 3, (long)MonNames_Var);
		Z_Amethyst->WriteDword(0x00752F32 + 3, (long)MonNames_Var);
}

void fix_amethyst_crexpmod() {

	creexpo1024();


	//return; // disabled temporary @2017-07-24_23-13
	//        // confirmed that this function is buggy

	//////
	//majaczek crexpo

	// *(char*) 0x00608cd0 = (char) 4; // rather not :(
	// *(char*)0x0060cf74 = (char)4; // Unknown
	// *(char*)0x006191cf = (char)4; // Unknown
	// *(char*)0x0077f4dd = (char)4; // Unknown

	// *(char*) 0x006058f9 = (char)4;
	// *(char*) 0x00605df6 = (char)4;
	// *(char*) 0x00608a7d = (char)4; // ??? probably bad
	// *(char*) 0x0078123b = (char)4; // ??? probably bad

	
	//// 
	//*(char*)0x0071a9db = (char)4; // Unknown (some loop inside crexpmod::Clear) (probably good)
	//*(char*)0x00761ac5 = (char)4; // Unknown (some loop) (probably good)
	//// *(char*)0x0077472d = (char)4; //seriously not this line
	//// //disabled 2017-07-24 23-35

	//

	//// 
	
	//some of them are buggy but some are needed 
	/*
	// *(char*)0x0071a8a7 = (char)4; //legit //suspicious 2017-07-25 00-28
	*(char*)0x0071acdf = (char)4; //legit
	// *(char*)0x0071b23f = (char)4; //legit //suspicious 2017-07-25 00-28
	*(char*)0x0071b555 = (char)4; //legit
	*(char*)0x0071b611 = (char)4; //legit
	// *(char*)0x00726512 = (char)4; //legit //suspicious 2017-07-25 00-20
	// *(char*)0x00726a01 = (char)4; //legit //suspicious 2017-07-25 00-25
	*(char*)0x007278cd = (char)4; //legit
	*(char*)0x00727e50 = (char)4; //legit
	*(char*)0x007280ad = (char)4; //legit
	//disabled 2017-07-24 23-35 //reenabled 2017-07-24 23-56
	*/
	//// *(char*)0x0071a8a7 = (char)4; //legit //suspicious 2017-07-25 00-28 //one of them should work 2017-07-25 00-37
    //*(char*)0x0071b23f = (char)4; //legit //suspicious 2017-07-25 00-28 //one of them should work 2017-07-25 00-37


	////
	// *(char*)0x005baf5e = (char)4; // Unknown

	////
	/*
	*(DWORD32*)0x0071ab9a = (DWORD32) (1024 * 340); //???
	*(DWORD32*)0x0071ab32 = (DWORD32) (1024 * 340); //???
	*(DWORD32*)0x0071ad7e = (DWORD32) (1024 * 340); //???
	*/

	////

	/*
	*(DWORD32*)0x0071b02d = (DWORD32)all_creatures;
	*(DWORD32*)0x00723973 = (DWORD32)all_creatures;
	*(DWORD32*)0x00723b66 = (DWORD32)all_creatures;
	*(DWORD32*)0x00723d58 = (DWORD32)all_creatures;


	*(DWORD32*)0x00750493 = (DWORD32)all_creatures;
	*(DWORD32*)0x0075054b = (DWORD32)all_creatures;
	*(DWORD32*)0x0075059c = (DWORD32)all_creatures;
	*(DWORD32*)0x00750d8d = (DWORD32)all_creatures;

	*(DWORD32*)0x00752f0b = (DWORD32)all_creatures;
	// *(DWORD32*)0x00756a43 = (DWORD32)all_creatures; // ZGHOST.TXT handling
	// *(DWORD32*)0x00760f41 = (DWORD32)all_creatures; //// this seriously NOT (why?)

	////
	*(DWORD32*) 0x718153 = (DWORD32)all_creatures;
	*(DWORD32*) 0x71a9f8 = (DWORD32)all_creatures;

	// *(DWORD32*) 0x726266 = (DWORD32) something; //it's originaly (char)4
	
	*(DWORD32*) 0x73a84a = (DWORD32)all_creatures;
	*/

	/*
	*(DWORD32*)0x726127 = (DWORD32)all_creatures;
	*(DWORD32*)0x726177 = (DWORD32)all_creatures;
	*(DWORD32*)0x726366 = (DWORD32)all_creatures;
	*(DWORD32*)0x7263d0 = (DWORD32)all_creatures;
	*(DWORD32*)0x756a9d = (DWORD32)all_creatures;

	*(DWORD32*)0x71a671 = (DWORD32)1024 * 20;
	*(DWORD32*)0x71a609 = (DWORD32)1024 * 20;
	*(DWORD32*)0x71a721 = (DWORD32)1024 * 20;
	*/ //disabled 2017-07-24 23-35

	////////////

	// majaczek
	
	
	for (int i = 0; i != (sizeof(p_CURMON_table) / 4); i++)
	{
			*(int*)(p_CURMON_table[i]) = 1989; // anything greater than CURMON in wog sources
	}
	

	for (int i = 0; i != (sizeof(p_experience_0x0085EB50_table) / 4); i++)
	{
		//CrExpMod__Body
		*(int*)(p_experience_0x0085EB50_table[i]) = (int)experience_0x0085EB50_table;
	}

	for (int i = 0; i != (sizeof(p_Experience_0x00847f08_Table) / 4); i++)
	{	
		// CRExpBon__Body
		*(int*)(p_Experience_0x00847f08_Table[i]) = (int)experience_0x00847f08_table;
	}

	/*
	// 
	for (int i = 0; i != (sizeof(p_experience_0x0085EB48_table) / 4); i++)
	{
		*(int*)(p_experience_0x0085EB48_table[i]) = (int)experience_0x0085EB48_table;
	}

	// Why doesn't work ?
	for (int i = 0; i != (sizeof(p_experience_0x0085EB50_table) / 4); i++)
	{
		*(int*)(p_experience_0x0085EB50_table[i]) = (int)experience_0x0085EB50_table;
	}
	*/

	//MessageBoxA(0, (LPCSTR)"amethyst::crexpo BUMP", (LPCSTR)"amethyst::crexpo BUMP", 0);
	for (int i = 0; i != (sizeof(p_experience_0x0085EB54_table) / 4); i++)
	{
		*(int*)(p_experience_0x0085EB54_table[i]) = (int)experience_0x0085EB54_table;
	}
	for (int i = 0; i != (sizeof(p_experience_0x0085EB58_table) / 4); i++)
	{
		*(int*)(p_experience_0x0085EB58_table[i]) = (int)experience_0x0085EB58_table;
	}
	for (int i = 0; i != (sizeof(p_experience_0x0085EB5C_table) / 4); i++)
	{
		*(int*)(p_experience_0x0085EB5C_table[i]) = (int)experience_0x0085EB5C_table;
	}
	for (int i = 0; i != (sizeof(p_experience_0x0085EB60_table) / 4); i++)
	{
		*(int*)(p_experience_0x0085EB60_table[i]) = (int)experience_0x0085EB60_table;
	}
	//MessageBoxA(0, (LPCSTR)"amethyst::crexpo BUMP", (LPCSTR)"amethyst::crexpo BUMP", 0);
	
	/*
	
	// probably not THE THING
	for (int i = 0; i != (sizeof(p_experience_0x0085FF50_table) / 4); i++)
	{
		*(int*)(p_experience_0x0085FF50_table[i]) = ((int)experience_0x0085FF50_table); // + (int)1024 * 20;
	}
	

	//MessageBoxA(0, (LPCSTR)"amethyst::crexpo BUMP", (LPCSTR)"amethyst::crexpo BUMP", 0);

	//
	for (int i = 0; i != (sizeof(p_experience_0x00860298_table) / 4); i++)
	{
		*(int*)(p_experience_0x00860298_table[i]) = ((int)experience_0x00860298_table);
	}	// MessageBoxA(0, (LPCSTR)"amethyst::crexpo BUMP 01", (LPCSTR)"amethyst::crexpo BUMP", 0);
	for (int i = 0; i != (sizeof(p_experience_0x00860299_table) / 4); i++)
	{
		*(int*)(p_experience_0x00860299_table[i]) = ((int)experience_0x00860299_table);
	}	// MessageBoxA(0, (LPCSTR)"amethyst::crexpo BUMP 02", (LPCSTR)"amethyst::crexpo BUMP", 0);
	 for (int i = 0; i != (sizeof(p_experience_0x0086029A_table) / 4); i++) //problem fixed?
	{
		*(int*)(p_experience_0x0086029A_table[i]) = ((int)experience_0x0086029A_table);
	}	// MessageBoxA(0, (LPCSTR)"amethyst::crexpo BUMP 03", (LPCSTR)"amethyst::crexpo BUMP", 0); 
	 for (int i = 0; i != (sizeof(p_experience_0x0086029B_table) / 4); i++) //problem fixed?
	{
		*(int*)(p_experience_0x0086029B_table[i]) = ((int)experience_0x0086029B_table);
	}	// MessageBoxA(0, (LPCSTR)"amethyst::crexpo BUMP 04", (LPCSTR)"amethyst::crexpo BUMP", 0); 
	for (int i = 0; i != (sizeof(p_experience_0x0086029C_table) / 4); i++)
	{
		*(int*)(p_experience_0x0086029C_table[i]) = ((int)experience_0x0086029C_table);
	}	// MessageBoxA(0, (LPCSTR)"amethyst::crexpo BUMP 05", (LPCSTR)"amethyst::crexpo BUMP", 0);
	for (int i = 0; i != (sizeof(p_experience_0x0086029D_table) / 4); i++)
	{
		*(int*)(p_experience_0x0086029D_table[i]) = ((int)experience_0x0086029D_table);
	}	// MessageBoxA(0, (LPCSTR)"amethyst::crexpo BUMP 06", (LPCSTR)"amethyst::crexpo BUMP", 0);
	for (int i = 0; i != (sizeof(p_experience_0x008602A0_table) / 4); i++)
	{
		*(int*)(p_experience_0x008602A0_table[i]) = ((int)experience_0x008602A0_table);
	}	// MessageBoxA(0, (LPCSTR)"amethyst::crexpo BUMP 07", (LPCSTR)"amethyst::crexpo BUMP", 0);
	for (int i = 0; i != (sizeof(p_experience_0x008602A4_table) / 4); i++)
	{
		*(int*)(p_experience_0x008602A4_table[i]) = ((int)experience_0x008602A4_table);
	}	// MessageBoxA(0, (LPCSTR)"amethyst::crexpo BUMP 08", (LPCSTR)"amethyst::crexpo BUMP", 0);
	for (int i = 0; i != (sizeof(p_experience_0x008602A8_table) / 4); i++)
	{
		*(int*)(p_experience_0x008602A8_table[i]) = ((int)experience_0x008602A8_table);
	}	// MessageBoxA(0, (LPCSTR)"amethyst::crexpo BUMP 09", (LPCSTR)"amethyst::crexpo BUMP", 0);
	for (int i = 0; i != (sizeof(p_experience_0x008602A9_table) / 4); i++)
	{
		*(int*)(p_experience_0x008602A9_table[i]) = ((int)experience_0x008602A9_table);
	}	// MessageBoxA(0, (LPCSTR)"amethyst::crexpo BUMP 10", (LPCSTR)"amethyst::crexpo BUMP", 0);
	for (int i = 0; i != (sizeof(p_experience_0x008602AA_table) / 4); i++)
	{
		*(int*)(p_experience_0x008602AA_table[i]) = ((int)experience_0x008602AA_table);
	}	// MessageBoxA(0, (LPCSTR)"amethyst::crexpo BUMP 11", (LPCSTR)"amethyst::crexpo BUMP", 0);
	//


	//MessageBoxA(0, (LPCSTR)"amethyst::crexpo BUMP", (LPCSTR)"amethyst::crexpo BUMP", 0);

	for (int i = 0; i != (sizeof(p_experience_0x008602B0_table) / 4); i++)
	{
		*(int*)(p_experience_0x008602B0_table[i]) = ((int)experience_0x008602B0_table);
	}

	*/

	/*
	// seems evil
	for (int i = 0; i != (sizeof(p_experience_modifier_table) / 4); i++)
	{
		*(int*)(p_experience_modifier_table[i]) = ((int)experience_modifier_table); //+(int)1024 * 20;
	}
	*/
	
	///////////

	*(int*)(0x0071a609) = 20 * MONSTERS_AMOUNT;
	*(int*)(0x0071a671) = 20 * MONSTERS_AMOUNT;
	*(int*)(0x0071a721) = 20 * MONSTERS_AMOUNT;

	*(int*)(0x0071ad7e) = 340 * MONSTERS_AMOUNT;

	*(int*)(0x0071a8a6) =  MONSTERS_AMOUNT;
	*(int*)(0x0071a9da) =  MONSTERS_AMOUNT;
	*(int*)(0x007278cc) =  MONSTERS_AMOUNT;

	*(int*)(0x0071b23e) = MONSTERS_AMOUNT;
	*(int*)(0x00761ac4) = MONSTERS_AMOUNT;
	*(int*)(0x007280ac) = MONSTERS_AMOUNT;

	// MessageBoxA(0, (LPCSTR)"amethyst::crexpo loaded", (LPCSTR)"amethyst::crexpo loaded", 0);
	// memcpy(experience_0x00860298_table, (void*)0x00860298, experience_0x00860550_table - experience_0x00860298_table);
	// MessageBoxA(0, (LPCSTR)"amethyst::crexpo memcpy", (LPCSTR)"amethyst::crexpo memcpy",0);

	//creexpo1024();
/*
	LoadExpTXT__type LoadExpTXT = (LoadExpTXT__type) LoadExpTXT__ptr;
	//LoadExpTXT();

	CrExpoSet__Clear__type CrExpoSet__Clear= (CrExpoSet__Clear__type)CrExpoSet__Clear__ptr;
	CrExpMod__Clear__type CrExpMod__Clear= (CrExpMod__Clear__type)CrExpMod__Clear__ptr;
	CrExpBon__Clear__type CrExpBon__Clear= (CrExpBon__Clear__type)CrExpBon__Clear__ptr;

	//ForceTxtUnload("ZCREXP.TXT"); LoadExpTXT(); CrExpoSet__Clear();
	ForceTxtUnload("CREXPMOD.TXT"); CrExpMod__Clear();
	ForceTxtUnload("CREXPBON.TXT");	CrExpBon__Clear();
*/
	*(int*)(0x00733326) = MONSTERS_AMOUNT;
}

//====================================================================
char tmp_missile[16]; int tmp_missle_ret = 0;

int (__fastcall *LoadDef)(char *defname) =
(int (__fastcall*)(char*))(0x55C9C0);

unsigned char(__fastcall* SearchFileFromLod)(void* FILE, int, char*) =
(unsigned char(__fastcall* )(void* FILE, int, char*))(0x004FB100);


int _edi() {__asm{mov eax, edi}}
int _esi() {__asm{mov eax, esi}}
int _ebx() {__asm{mov eax, ebx}}

int __fastcall NewMissiles_Body(char* missile_def, int edx)
{
	int creature = *(int*)(_ebx()+0x34);
	
	if (creature <= 0 || creature > MONSTERS_AMOUNT/*&& (new_monsters[creature].i_Flags & 2048)*/)
	{
		sprintf(tmp_missile, "shot%i.DEF", creature);
		MessageBoxA(0,(LPCSTR)tmp_missile,(LPCSTR)tmp_missile,0);
		return(LoadDef("Phalf.DEF"));
	}

	if (creature > 196 /*&& (new_monsters[creature].i_Flags & 2048)*/)
	{
		sprintf(tmp_missile, "shot%i.DEF", creature);
		//MessageBoxA(0,(LPCSTR)tmp_missile,(LPCSTR)tmp_missile,0);
		return(LoadDef(tmp_missile));
	}
	else {
		sprintf(tmp_missile, "shot%i.DEF", creature);
		//if(SearchFileFromLod(,0,tmp_missile))
		if (FindDefOrMskInAllLods(tmp_missile))
		{
			tmp_missle_ret = LoadDef(tmp_missile);
			if (tmp_missle_ret) return tmp_missle_ret;
		}
		else {
			sprintf(tmp_missile, "shot%03i.DEF", creature);
			if (FindDefOrMskInAllLods(tmp_missile))
			{
				tmp_missle_ret = LoadDef(tmp_missile);
				if (tmp_missle_ret) return tmp_missle_ret;
			}
		}
	}

	return LoadDef(missile_def);
}


void NewMonstersMissiles()
{
	//*(int*)0x43DA95 = MONSTERS_AMOUNT;
  
	WriteHook((void*)0x43DB1D,(void*)NewMissiles_Body, HOOKTYPE_CALL);
} 
//=============================================

/*
char(__cdecl *LoadCranimDefault)() =
      (char(__cdecl *)())(0x50CA00);

char(__cdecl *LoadCrtraitsDefault)() =
      (char(__cdecl *)())(0x47ABC0);


extern void FillTables();

char __cdecl LoadCrtraits()
{
	
	char ret = LoadCrtraitsDefault();
	     ret|= LoadCranimDefault();

		 
	FillTables();

	return ret;
}

char __cdecl LoadCranim()
{
	//char ret = LoadCranimDefault();

	return 1;
}


void LoadTxtHook()
{
	WriteHook((void*)0x4EDE90,(void*)LoadCrtraits, HOOKTYPE_CALL);
	WriteHook((void*)0x4EDF4B,(void*)LoadCranim  , HOOKTYPE_CALL);
}*/
//==============================================