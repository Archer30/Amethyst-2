// dllmain.cpp: определяет точку входа для приложения DLL.


// #include "..\__include\era.h"
#include "stdafx.h"
#include "..\__include\era.h"

//using Era::;
#define PEvent Era::TEvent*
#define ErmV Era::v
using Era::ExecErmCmd;
using Era::ConnectEra;
using Era::RegisterHandler;
using Era::ReadSavegameSection;
using Era::WriteSavegameSection;

extern void CreateNewTable(void);
extern void CreateAdditionalTables(void);
extern void NewMonstersMissiles(void);
extern void fix_amethyst_crexpmod(void);
//extern void InitNecromancy(void);

int all_creatures;
int conf_creatures;

extern char* experience_modifier_table;
extern char  resource_type_table	[MONSTERS_AMOUNT];
extern DWORD resource_amount_table	[MONSTERS_AMOUNT];
extern DWORD resource_tax_table		[MONSTERS_AMOUNT];


extern char names[MONSTERS_AMOUNT][64];
extern char names_pl[MONSTERS_AMOUNT][64];
extern char descripts[MONSTERS_AMOUNT][256];


const DWORD* CurrentPlayer = (DWORD*) 0x69CCF4;

extern MONSTER_PROP new_monsters[MONSTERS_AMOUNT];
bool save__new_monsters = false;
bool DisableNecromancyFeatures = false;
bool FilterNecromancyCreatures = false;


void drop_html_creature_list(const char const* path);
void ranged_retaliation_event(int trigger, int action, int target, int current_mon, int target_pos);

int __stdcall GameMgr_GetRandMonster_ByLevel(HiHook* hook, void* gm, int mon_lvl_Lo, int mon_lvl_Hi);


#define PINSTANCE_MAIN "Z_Amethyst"

Patcher* globalPatcher;
PatcherInstance* Z_Amethyst;

void set_erm_limits(void);

extern char spell_2_table[MONSTERS_AMOUNT];
_LHF_(hook_005A7A5A) {
	c->ebx = (*(int*)0x699420);
	int bm = c->ebx; c->edi = 0x32;
	int stack = *(int*)(bm + 0x132C8);
	if(stack <= 0) return EXEC_DEFAULT;
	int type = *(int*)(stack + 0x34);
	int zzz = spell_2_table[type];

	// resurrect animations
	if (zzz == 9)  c->edi = 0x60;
	if (zzz == 10) c->edi = 0x61;
	if (zzz == 11) c->edi = 0x5E;
	//c->edi = ;
	return EXEC_DEFAULT;
}
/*
_LHF_(hook_005A7B77) {
	int bm = c->ebx;
	int stack =  *(int*)(bm + 0x132C8);
	int &flags = *(int*)(stack + 0x84);
	flags |= 0x04000000;
	return EXEC_DEFAULT;
}
*/
_LHF_(hook_ecx_batman_dialogue_fix) {
	int CombatManager = (*(int*)0x699420);
	c->ecx = *(int*)(CombatManager + 0x132FC);
	return EXEC_DEFAULT;
}
_LHF_(hook_ecx_batman_fix) {
	int CombatManager = (*(int*)0x699420);
	c->ecx = CombatManager;
	return EXEC_DEFAULT;
}

_LHF_(hook_esi_batman_fix) {
	int CombatManager = (*(int*)0x699420);
	c->esi = CombatManager;
	return EXEC_DEFAULT;
}

/*
_LHF_(hook_004E7230) {
	int BattleGEX = c->ecx;
	if (BattleGEX <= 0
		// || *(int*)(BattleGEX+)
		) {

		c->return_address = 0x004E7269;
		return NO_EXEC_DEFAULT;
	}

	return EXEC_DEFAULT;
}
*/

_LHF_(hook_00443DBB) {
	if (c->esi <= 0) {
		c->return_address = 0x00443E36;
		c->edi = 0; return NO_EXEC_DEFAULT;
	}

	return EXEC_DEFAULT;
}

void  __stdcall load_things_late(PEvent e) {
	// Z_Amethyst->WriteLoHook(0x004E7230, hook_004E7230);
	// Z_Amethyst->WriteLoHook(0x00443DBB,hook_00443DBB);

	Z_Amethyst->WriteLoHook(0x4729E8, hook_ecx_batman_dialogue_fix);
	Z_Amethyst->WriteLoHook(0x5A9547, hook_ecx_batman_dialogue_fix);
	Z_Amethyst->WriteLoHook(0x462E42, hook_ecx_batman_fix);
	Z_Amethyst->WriteLoHook(0x4683AB, hook_ecx_batman_fix);
	Z_Amethyst->WriteLoHook(0x5A7BF5, hook_ecx_batman_fix);
	Z_Amethyst->WriteLoHook(0x462EB6, hook_esi_batman_fix);

	// Z_Amethyst->WriteLoHook(0x005A7A6E, hook_005A7A6E);
	Z_Amethyst->WriteLoHook(0x005A7A5A, hook_005A7A5A);

	// Z_Amethyst->WriteLoHook(0x005A7B77, hook_005A7B77);
}

void MiscCreatureHooks_apply_strings();
void  __stdcall drop_html_creature_list(PEvent e) {
	MiscCreatureHooks_apply_strings();
	drop_html_creature_list(".\\help\\Amethyst_Creatures.htm");
}


void install_CrState();
void  __stdcall install_CrState(PEvent e) {
	install_CrState();
	set_erm_limits();
}

void ResetAdditionalTables_BattleOnly();
extern BYTE addditional_moves[42];
extern BYTE addditional_shots[42];
extern BYTE addditional_melees[42];

void  __stdcall battle_start_event(PEvent e) {
	memset(addditional_moves, 0, 42);
	memset(addditional_shots, 0, 42);
	memset(addditional_melees, 0, 42);
	ResetAdditionalTables_BattleOnly();
}


#include "H3CombatMonster.hpp"
extern char MovesTwice_Table[MONSTERS_AMOUNT + 64];
void  __stdcall battle_late_begin(PEvent e)
{
	if (Era::v[997]) return;
	int CombatManager = (*(int*)0x699420);
	H3CombatMonster* monsters = (H3CombatMonster*)(CombatManager + 0x54CC);
	for (int i = 0; i < 42; ++i)
	{	long mon_type = monsters[(i)].type;
	if (mon_type >= 0 && 
		(MovesTwice_Table[mon_type] || MovesTwice_Table[MONSTERS_AMOUNT + i])
		&& !addditional_moves[i])
			addditional_moves[i] = 1;
}}

void __stdcall fix_vanilla_creatures(PEvent e) {

	void* (__fastcall * FindDefOrMskInAllLods)(const char* name) =
		(void* (__fastcall*)(const char*)) (0x0055D160);

	int(__fastcall * LoadDef)(char* defname) =
		(int(__fastcall*)(char*))(0x55C9C0);


	int(__fastcall * LoadWav2)(char* WavName) =
		(int(__fastcall*)(char*))(0x55C930);

	char(__fastcall * LoadWAV_FromLod)(const char* name, int* DestroyBoolAndData, DWORD * size) =
		(char(__fastcall*)(const char* , int* , DWORD * )) (0x0055C340);
/*
	//debug
	if (!FindDefOrMskInAllLods("monkmove.82m"))
		MessageBoxA(0, (LPCSTR)"monkmove.82m not found", (LPCSTR)"monkmove.82m not found", 0);
	if (!FindDefOrMskInAllLods("monkmove.wav"))
		MessageBoxA(0, (LPCSTR)"monkmove.Wav not found", (LPCSTR)"monkmove.wav not found", 0);
	if (!FindDefOrMskInAllLods("monkmove"))
		MessageBoxA(0, (LPCSTR)"monkmove not found", (LPCSTR)"monkmove not found", 0);
*/

	for (int i = 0; i < 197; ++i) {
		char z_defname[16] = {};
		char z_soundname[5] = {};

		sprintf(z_defname, "CR%03i.def\0", i);
		sprintf(z_soundname, "s%03i\0", i);

		if (FindDefOrMskInAllLods(z_defname)) {
			new_monsters[i].defname = (char*)malloc(16);
			strcpy(new_monsters[i].defname, z_defname);
		}

		char zz_soundname[16] = {}; int DestroyData = 0; int DataSize = 0;
		sprintf(zz_soundname, "%s%s", z_soundname, "move.82M");
		//if (FindDefOrMskInAllLods(zz_soundname)) 
		if (LoadWAV_FromLod(zz_soundname,&DestroyData,(DWORD*)&DataSize))
		{
			new_monsters[i].soundname = (char*)malloc(5);
			strcpy(new_monsters[i].soundname, z_soundname);
		}
	}
}

void __stdcall Daily_Res(PEvent e) {
	// return;
	// ExecErmCmd("IF:M^OnEveryDay^");
	int res[8] = {}; int tax = 0;
	int OWN = *CurrentPlayer;
	for (int N = 0;N<=155;++N) {
		HERO* h = (HERO*) GetHeroRecord(N);
		if (h->Owner == OWN) {
			for (int S = 0; S < 7;++S) {
				if (h->Cn[S] < 0) continue;
				if (h->Ct[S] < 0) continue; 
				{
					tax += resource_tax_table[h->Ct[S]] * h->Cn[S];
					int R = resource_type_table[h->Ct[S]];
					if (R < 0 || R > 7) continue;
					if (res[R] < resource_amount_table[h->Ct[S]])
						res[R] = resource_amount_table[h->Ct[S]];
				}
				
			}
		}
	}
	auto &towns = h3::H3Main::Get()->towns;
	for (auto &town : towns) {
		if (town.owner == OWN && town.garrisonHero < 0) {
			for (int S = 0; S < 7; ++S) {
				int& Ct = town.guards.type[S];
				int& Cn = town.guards.count[S];
				if (Cn < 0) continue;
				if (Ct < 0) continue;
				{
					tax += resource_tax_table[Ct] * Cn;
					int R = resource_type_table[Ct];
					if (R < 0 || R > 7) continue;
					if (res[R] < resource_amount_table[Ct])
						res[R] = resource_amount_table[Ct];
				}

			}
		}
	}

	char cmd[512]; 
	for (int R = 0; R < 8;++R) {
		sprintf(cmd, "OW:R%d/%d/d%d", OWN, R, res[R]);
		if(res[R]) ExecErmCmd(cmd);
	}
	if (tax) {
		sprintf(cmd, "OW:R%d/%d/d%d", OWN, 6 /* Gold */ , tax);
		/* if (tax) */ ExecErmCmd(cmd);
	}
}

/*
void ForceTxtUnload(char* name) {
	auto h = GetModuleHandle(L"h3era.exe"); if (!h) h = GetModuleHandle(L"h3era HD.exe");
	if (!h) MessageBoxA(0, "cannot link to heroes3 executable", "debug", MB_OK);

	//MessageBoxA(0, "ForceTxtUnload 1", "debug", MB_OK);
	PTxtFile txt;
	LoadTxtFun__type LoadTxtFun = (LoadTxtFun__type)LoadTxtFun__ptr;
	UnloadTxtFun__type UnloadTxtFun = (UnloadTxtFun__type)UnloadTxtFun__ptr;
	//MessageBoxA(0, "ForceTxtUnload 2", "debug", MB_OK);
	txt = (*LoadTxtFun)(name); 
	//MessageBoxA(0, "ForceTxtUnload 3", "debug", MB_OK);
	if (!txt) return;	txt->ref_count = 1; 
	//MessageBoxA(0, "ForceTxtUnload 4", "debug", MB_OK);
	(*UnloadTxtFun)(txt);
	//MessageBoxA(0, "ForceTxtUnload 5", "debug", MB_OK);

}
*/

void InitMainConfig(char *config_name)
{
	conf_creatures = 0x34; all_creatures = conf_creatures + 144;
	//MessageBoxA(0, config_name, "Detected Config: ",  0);
    char *buf;

        FILE *fdesc;
		if (fdesc = fopen(config_name, "r"))
		{
			//----------
			fseek(fdesc, 0, SEEK_END);
			int fdesc_size = ftell(fdesc);
			rewind(fdesc);
			//----------
			buf = (char*)malloc(fdesc_size + 1);
			fread(buf, 1, fdesc_size, fdesc);
			buf[fdesc_size] = 0;
			fclose(fdesc);
			//-----------
			{
				char* c = strstr(buf, "SavePreserve=");
				if (c && atoi(c + strlen("SavePreserve=")) == 1) save__new_monsters = true;
			}
			//-----------
			{	
				char* c = strstr(buf, "Creatures=");
				if (c == 0) { conf_creatures = 0x34; }
				else { conf_creatures = atoi(c + strlen("Creatures=")) - 144; }
			}
			//-----------
			{
				char* c = strstr(buf, "DisableNecromancyFeatures=");
				if (c && atoi(c + strlen("DisableNecromancyFeatures=")) == 1)
					DisableNecromancyFeatures = true;
			}
			//-----------
			{
				char* c = strstr(buf, "FilterNecromancyCreatures=");
				if (c && atoi(c + strlen("FilterNecromancyCreatures=")) == 1)
					FilterNecromancyCreatures = true;
			}
			//-----------
			free(buf);
        }
        else
        {
			conf_creatures = 0x34;
            MessageBoxA(0,config_name, "Missed file: ",0);
        }    
		all_creatures = conf_creatures + 144;

		if (fdesc = fopen("mods\\list.txt", "r"))
		{

			//----------
			fseek(fdesc, 0, SEEK_END);
			int fdesc_size = ftell(fdesc);
			rewind(fdesc);
			//----------
			buf = (char*)malloc(fdesc_size + 1);
			fread(buf, 1, fdesc_size, fdesc);
			buf[fdesc_size] = 0;
			fclose(fdesc);

			//-----------
			/* // disabled because demanded 
			{
				char* c = strstr(buf, "Advanced Classes Mod");
				if (c) DisableNecromancyFeatures = true;
			}
			*/

			free(buf);
		}

}


// ThisIsNewGame 0x007caf20

/* __declspec(naked) */ void Amethyst_ResetExpTables()
{
	//reset experience tables
	
	//return;
	void(*CrExpSet__Clear)(void) = reinterpret_cast<void (*)(void)> (0x007186C1);
	int(*CrExpMod__Clear)(void) = reinterpret_cast<int (*)(void)> (0x0071A69E);
	int(*CrExpBon__Clear)(void) = reinterpret_cast<int (*)(void)> (0x0071AD24);

	CrExpSet__Clear();
	CrExpMod__Clear();
	CrExpBon__Clear();
}

void Amethyst_LoadExpTables()
{
	//reset experience tables

	//return;
	int(*CrExpSet__Load)(int) = reinterpret_cast<int(*)(int)> (0x007187ba);
	int(*CrExpMod__Load)(int) = reinterpret_cast<int(*)(int)> (0x0071A58C);
	int(*CrExpBon__Load)(int) = reinterpret_cast<int(*)(int)> (0x0071AAB5);

	//CrExpMod__Load(255); //any argument

}

void Amethyst_SaveExpTables()
{
	//reset experience tables

	//return;
	int(*CrExpSet__Save)(void) = reinterpret_cast<int(*)(void)> (0x00718837);
	int(*CrExpMod__Save)(void) = reinterpret_cast<int(*)(void)> (0x0071A638);
	int(*CrExpBon__Save)(void) = reinterpret_cast<int(*)(void)> (0x0071AB61);

	//CrExpMod__Load(); //any argument

}

// CrExpMod__Dummy        0x00791E80
// CrExpoSet__StopAllBF() 0x0072774E

// void __stdcall CopyExpModTables(TEvent* Event)
/*
__declspec(naked) void CopyExpModTables()
{
	__asm {
		
	}
	// memcpy((void*)experience_modifier_table, (void*)0x0085EB50, 20 * (197 + 5)); // not copies
}
*/

char* Hook_0x00705450_error_message = "crexpo reload failed !!! \n the game will now crash :( \n bye";
char* Hook_0x00705450_BUMP_message = "Hook_0x00705450";

__declspec(naked) void Hook_0x00705450() // at end of CalledBeforeTurn1New
{
	__asm {
		pusha

		mov  eax, 0x007186C1
		CALL eax
		mov  eax, 0x0071A69E
		CALL eax
		mov  eax, 0x0071AD24
		CALL eax

		/*
		mov  edi, esp
		PUSH 0
		PUSH 1
		PUSH Hook_0x00705450_BUMP_message
		mov	 eax, 0x0070FB63
		CALL eax
		mov  esp, edi

		mov  edi, esp
		PUSH 0
		PUSH 1
		PUSH Hook_0x00705450_BUMP_message
		mov	 eax, 0x0070FB63
		CALL eax
		mov  esp, edi

		mov edi, esp
		PUSH 18
		mov  ebx, 0x007187ba
		CALL ebx
		//ADD  ESP, 4
		TEST EAX, EAX
		JNE  z_error
		mov  esp, edi


		mov  edi, esp
		PUSH 0
		PUSH 1
		PUSH Hook_0x00705450_BUMP_message
		mov	 eax, 0x0070FB63
		CALL eax
		mov  esp, edi
		

		mov  edi, esp
		PUSH 18
		mov  ebx, 0x0071A58C
		CALL ebx
		//ADD  ESP, 4
		TEST EAX, EAX
		JNE  z_error
		mov  esp, edi


		mov  edi, esp
		PUSH 0
		PUSH 1
		PUSH Hook_0x00705450_BUMP_message
		mov	 eax, 0x0070FB63
		CALL eax
		mov  esp, edi


		mov  edi, esp
		PUSH 18
		mov  ebx, 0x0071AAB5
		CALL ebx
		//ADD  ESP, 4
		TEST EAX, EAX
		JNE  z_error
		mov  esp, edi
		*/

		mov edi, esp
		PUSH 0
		PUSH 1
		PUSH Hook_0x00705450_BUMP_message
		mov	 eax, 0x0070FB63
		CALL eax
		mov  esp, edi
		

		popa

		pop edi
		pop esi
		pop ebx
		mov esp, ebp
		pop ebp
		retn

		z_error:
			PUSH 0
			PUSH 1
			PUSH Hook_0x00705450_error_message
			mov	 eax, 0x0070FB63
			CALL eax

	}
	// memcpy((void*)experience_modifier_table, (void*)0x0085EB50, 20 * (197 + 5)); // not copies
}

//void __stdcall ReloadExperienceConfig(TEvent* Event)
void __stdcall ReloadExperienceConfig(void)
{
	//return;
	auto h= GetModuleHandle(L"h3era.exe"); if(!h) h=GetModuleHandle(L"h3era HD.exe");
	if (!h) MessageBoxA(0,"cannot link to heroes3 executable","debug",MB_OK);
	
	auto hEra = GetModuleHandle(L"era.dll"); 
	if(!hEra) MessageBoxA(0, "cannot link to era.dll", "debug", MB_OK);

	extern void asm_store(void); extern void asm_write(void); extern void asm_restore(void);
	asm_store(); asm_write();

	//typedef void(__stdcall *TForceTxtUnload) (char* Name); TForceTxtUnload ForceTxtUnload = nullptr;
	//auto ForceTxtUnload_ptr = GetProcAddress(hEra, "ForceTxtUnload");
	//if (!ForceTxtUnload_ptr) { MessageBoxA(0, "cannot get address of ForceTxtUnload from era.dll", "debug", MB_OK); return; }
	
	//ForceTxtUnload = (TForceTxtUnload)*ForceTxtUnload_ptr;

	LoadExpTXT__type LoadExpTXT = (LoadExpTXT__type) LoadExpTXT__ptr;
	//LoadExpTXT();

	CrExpoSet__Clear__type CrExpoSet__Clear = (CrExpoSet__Clear__type) CrExpoSet__Clear__ptr;
	CrExpMod__Clear__type CrExpMod__Clear = (CrExpMod__Clear__type) CrExpMod__Clear__ptr;
	CrExpBon__Clear__type CrExpBon__Clear = (CrExpBon__Clear__type) CrExpBon__Clear__ptr;

	auto z_entry = GetProcAddress(h,"entry");

	char debug[999] = ""; 
	/*
	sprintf_s(debug,"entry=%X %X=%X %X=%X %X=%X ", (int)z_entry,
		(int) CrExpoSet__Clear, CrExpoSet__Clear__ptr,
		(int)CrExpMod__Clear, CrExpMod__Clear__ptr, 
		(int)CrExpBon__Clear, CrExpBon__Clear__ptr);

	MessageBoxA(0,debug,"debug",MB_OK);

	sprintf_s(debug, "&val=%X \t val=%X \t *val=%X ", (int)&CrExpBon__Clear__ptr, 
		CrExpBon__Clear__ptr, *(int*)CrExpBon__Clear__ptr);
	MessageBoxA(0, debug, "debug", MB_OK);
	*/

	//ForceTxtUnload("ZCREXP.TXT");

	//MessageBoxA(0, "stage0.1", "debug", MB_OK);
	//(*LoadExpTXT)();
	(*CrExpoSet__Clear)();
	
	//MessageBoxA(0, "stage1.0", "debug", MB_OK);
	
	//ForceTxtUnload("CREXPMOD.TXT"); 
	//MessageBoxA(0, "stage1.1", "debug", MB_OK); 

	// *(unsigned char*)0x0071a748 = 0xeb;
	(*CrExpMod__Clear)();
	// *(unsigned char*)0x0071a748 = 0x74;

	//MessageBoxA(0, "stage2.0", "debug", MB_OK);

	//*(int*)CrExpBonFileLoaded__ptr = 0;
	//MessageBoxA(0, "CrExpBonFileLoaded__ptr = 0", "debug", MB_OK);

	//ForceTxtUnload("CREXPBON.TXT"); 
	//MessageBoxA(0,"crexpbon.txt Unloaded","debug",MB_OK);
	(*CrExpBon__Clear)();
	//MessageBoxA(0, "stage3.0", "debug", MB_OK);

	asm_restore();
	//MessageBoxA(0, "done", "debug", MB_OK);
}

void __stdcall ReloadExperienceConfig2(void)
{
	//return;
	auto h = GetModuleHandle(L"h3era.exe"); if (!h) h = GetModuleHandle(L"h3era HD.exe");
	if (!h) MessageBoxA(0, "cannot link to heroes3 executable", "debug", MB_OK);

	//LoadExpTXT__type LoadExpTXT = (LoadExpTXT__type)LoadExpTXT__ptr;
	//LoadExpTXT();

	CrExpoSet__Clear__type CrExpoSet__Clear = (CrExpoSet__Clear__type)CrExpoSet__Clear__ptr;
	CrExpMod__Clear__type CrExpMod__Clear = (CrExpMod__Clear__type)CrExpMod__Clear__ptr;
	CrExpBon__Clear__type CrExpBon__Clear = (CrExpBon__Clear__type)CrExpBon__Clear__ptr;

	static bool __once__ = false;
	if (__once__) {

		(*CrExpoSet__Clear)();
		(*CrExpMod__Clear)();
		(*CrExpBon__Clear)();
		
		return;
	}
	__once__ = true;

	//MessageBoxA(0, "begin", " ReloadExperienceConfig2() ", MB_OK);
	//char debug[999] = "";

	////todo 2020-08-12 - check if needed and remove or uncomment
	//ForceTxtUnload("ZCREXP.TXT");
	//ForceTxtUnload("CREXPMOD.TXT");
	//ForceTxtUnload("CREXPBON.TXT");

	//MessageBoxA(0, "1", "debug", MB_OK);

	LoadExpTXT__type LoadExpTXT = (LoadExpTXT__type)LoadExpTXT__ptr;
	(*LoadExpTXT)();
	(*CrExpoSet__Clear)();

	(*CrExpMod__Clear)();

	*(int*)CrExpBonFileLoaded__ptr = 0;
	(*CrExpBon__Clear)();

	//asm_restore();
	//MessageBoxA(0, "done", " ReloadExperienceConfig2() ", MB_OK);
}


void __stdcall StoreData(PEvent e)
{
	/*
	if (save__new_monsters)
		WriteSavegameSection(sizeof(MONSTER_PROP) * MONSTERS_AMOUNT,(void*) new_monsters, "Amethyst__new_monsters" );
	*/
	WriteSavegameSection(sizeof(names), names, "Amethyst__names");
	WriteSavegameSection(sizeof(names_pl), names_pl, "Amethyst__names_pl");
	WriteSavegameSection(sizeof(descripts), descripts, "Amethyst__descripts");
}
void __stdcall RestoreData(PEvent e)
{

	/*
	if (save__new_monsters)
	{
		char* soundname[MONSTERS_AMOUNT];	for (int i = 0; i < MONSTERS_AMOUNT;++i) soundname[i] = new_monsters[i].soundname;
		char* defname[MONSTERS_AMOUNT];		for (int i = 0; i < MONSTERS_AMOUNT; ++i) defname[i] = new_monsters[i].defname;
		char* name[MONSTERS_AMOUNT];		for (int i = 0; i < MONSTERS_AMOUNT; ++i) name[i] = new_monsters[i].name;
		char* pluralname[MONSTERS_AMOUNT];	for (int i = 0; i < MONSTERS_AMOUNT; ++i) pluralname[i] = new_monsters[i].pluralname;
		char* features[MONSTERS_AMOUNT];	for (int i = 0; i < MONSTERS_AMOUNT; ++i) features[i] = new_monsters[i].features;
		
		ReadSavegameSection(sizeof(MONSTER_PROP) * MONSTERS_AMOUNT, (void*)new_monsters, "Amethyst__new_monsters");

		for (int i = 0; i < MONSTERS_AMOUNT; ++i) new_monsters[i].soundname = soundname[i];
		for (int i = 0; i < MONSTERS_AMOUNT; ++i) new_monsters[i].defname = defname[i];
		for (int i = 0; i < MONSTERS_AMOUNT; ++i) new_monsters[i].name = name[i];
		for (int i = 0; i < MONSTERS_AMOUNT; ++i) new_monsters[i].pluralname = pluralname[i];
		for (int i = 0; i < MONSTERS_AMOUNT; ++i) new_monsters[i].features = features[i];
	}
	*/

	ReadSavegameSection(sizeof(names), names, "Amethyst__names");
	ReadSavegameSection(sizeof(names_pl), names_pl, "Amethyst__names_pl");
	ReadSavegameSection(sizeof(descripts), descripts, "Amethyst__descripts");
}


void __stdcall ranged_retaliation_event_30303(PEvent e)
{
	int tmp = ErmV[1];
	ExecErmCmd("BG:A?v1;");	int action = ErmV[1];
	ExecErmCmd("BG:E?v1;");	int target = ErmV[1];
	ExecErmCmd("BG:N?v1;");	int current_mon = ErmV[1];
	ExecErmCmd("BG:D?v1;");	int target_pos = ErmV[1];
	ErmV[1] = tmp;

	ranged_retaliation_event(30303, action, target, current_mon, target_pos);
}


void __stdcall ranged_retaliation_event_30304(PEvent e)
{
	int tmp = ErmV[1];
	ExecErmCmd("BG:A?v1;");	int action = ErmV[1];
	ExecErmCmd("BG:E?v1;");	int target = ErmV[1];
	ExecErmCmd("BG:N?v1;");	int current_mon = ErmV[1];
	ExecErmCmd("BG:D?v1;");	int target_pos = ErmV[1];
	ErmV[1] = tmp;

	ranged_retaliation_event(30304, action, target, current_mon, target_pos);
}


extern char* ptr_names[MONSTERS_AMOUNT];
extern char* ptr_names_pl[MONSTERS_AMOUNT];
extern char* ptr_descripts[MONSTERS_AMOUNT];

extern char* ptr_names_back[MONSTERS_AMOUNT];
extern char* ptr_names_pl_back[MONSTERS_AMOUNT];
extern char* ptr_descripts_back[MONSTERS_AMOUNT];


struct _MonNames_ {
	int Var[3];   
};

extern _MonNames_ MonNames_Var[MONSTERS_AMOUNT];

void __stdcall Block_Redirection(PEvent e)
{

	Era::RedirectMemoryBlock( (void*)0x7D0C90, 197 * sizeof (MONSTER_PROP) , new_monsters );

	Era::RedirectMemoryBlock((void*)0x7C8240, 197 * 4, ptr_names);
	Era::RedirectMemoryBlock((void*)0x7B6650, 197 * 4, ptr_names_pl);
	Era::RedirectMemoryBlock((void*)0x7C4018, 197 * 4, ptr_descripts);


	Era::RedirectMemoryBlock((void*) 0xA498A8, 197 * 4, ptr_names_back);
	Era::RedirectMemoryBlock((void*) 0xA48128, 197 * 4, ptr_names_pl_back);
	Era::RedirectMemoryBlock((void*) 0xA88E78, 197 * 4, ptr_descripts_back);


	Era::RedirectMemoryBlock((void*) 0xA48440, 197 * sizeof(_MonNames_), MonNames_Var);
}


extern char Monster_Ban_Table[MONSTERS_AMOUNT];

__declspec(naked) void MonsterRandomGeneration() {
	__asm {

		cmp byte ptr DS:[ebx + Monster_Ban_Table - 1], 0
		je lab
		push 541137h
		ret
		lab:
		mov edx, [ecx + 2Ch]
			mov esi, [ecx]
			push 541102h
			ret
	}
}

__declspec(naked) void MonsterRandomGeneration2(){
	__asm {
		cmp eax, edx
		jg lab
		cmp dword ptr DS:[edi], -1
		je lab
		lea eax, [esi + esi * 4]
		push 541116h
		ret
		lab:
		push 541137h
			ret
	}
}

void __stdcall Fix_Random_Creature_Generation(PEvent e)
{
	__asm {
		mov eax, all_creatures
		//mov eax, 144
		mov dword ptr DS:[541159h + 1], eax

		//mov eax, MONSTERS_AMOUNT
		lea eax, [eax*4+20]
		mov dword ptr DS : [541013h + 2], eax
		neg eax
		mov DS : [54106Bh + 2], eax
		mov DS : [54109Ch + 3], eax
		mov DS : [5410E1h + 3], eax
		mov DS : [541174h + 3], eax

		mov eax, all_creatures
		inc eax
		mov DS : [541064h], eax
		mov DS : [5410B4h], eax


	}

	WriteHook((void*)0x5410FD, (void*)MonsterRandomGeneration, HOOKTYPE_JUMP);
	WriteHook((void*)0x54110F, (void*)MonsterRandomGeneration2, HOOKTYPE_JUMP);
}


void apply_fixes();
void MiscCreatureHooks_apply();

void ApplySharedStats(int mon_type,int j);
extern char* SharedStats[MONSTERS_AMOUNT + 64];
void __stdcall ShareStatsEvent(PEvent e) {

	int CombatManager = (*(int*)0x699420);
	H3CombatMonster* monsters = (H3CombatMonster*)(CombatManager + 0x54CC);
	for (int i = 0; i < 42; ++i)
	{
		long mon_type = monsters[(i)].type;
		if (mon_type >= 0 && SharedStats[mon_type]) {
			int mon_side = monsters[(i)].side;
			for (int j = 0; j < 42; ++j)
			{
				long target_type = monsters[(j)].type;
				if (target_type >= 0 && mon_side == monsters[(j)].side) {
					ApplySharedStats(mon_type, j);
				}
			}
		}
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{	

			globalPatcher = GetPatcher();
			Z_Amethyst = globalPatcher->CreateInstance(PINSTANCE_MAIN);
			
			apply_fixes();

			//fix_amethyst_crexpmod();
			//ConnectEra(); //added by majaczek

		
			/*if (FileExists("Mods\\Amethyst Upgrades\\Data\\amethyst.cfg"))
				InitMainConfig("Mods\\Amethyst Upgrades\\Data\\amethyst.cfg");
			else */ /* if (FileExists("Mods\\Knightmare Kingdoms\\Data\\amethyst.cfg"))
				InitMainConfig("Mods\\Knightmare Kingdoms\\Data\\amethyst.cfg"); 
			else */ InitMainConfig("Data\\amethyst.cfg");
			CreateNewTable();
			CreateAdditionalTables();

			fix_amethyst_crexpmod();
			//Amethyst_ResetExpTables();
			
			NewMonstersMissiles();
			
			// ConnectEra(); //added by majaczek
			/*
			RegisterHandler(Amethyst_ResetExpTables, "OnAfterErmInstructions");
			RegisterHandler(Amethyst_ResetExpTables, "OnAfterLoadGame");
			*/

			// RegisterHandler(CopyExpModTables, "OnAfterErmInstructions");
			/////WriteHook((void*) /*0x00705347*/ /*0x004cd5d0*/ 0x00705450, (void*)CopyExpModTables, HOOKTYPE_CALL); // at end of CalledBeforeTurn1New
			
			//WriteHook((void*)0x00705450, (void*)Hook_0x00705450, HOOKTYPE_JUMP); // at end of CalledBeforeTurn1New

			// Amethyst_ResetExpTables(); // Unknown (probably doesn't work here)
			
			//Amethyst_LoadExpTables(); // IDK why it doesn't work

			/*
			_asm{
				pushad
				mov  eax, 0x00703d57
				call eax
				popad
			};
			*/

			//ConnectEra(); //added by majaczek
			//ReloadExperienceConfig();
			//RegisterHandler(ReloadExperienceConfig, "OnCreateWindow");
			//RegisterHandler(ReloadExperienceConfig, "zzzzzzzzzzzz");

			//todo
			//ReloadExperienceConfig();
			//*(int*)CrExpBonFileLoaded__ptr = 0;
			//ReloadExperienceConfig2();

			Z_Amethyst->WriteHiHook(0x4C8F80, SPLICE_, EXTENDED_, THISCALL_, GameMgr_GetRandMonster_ByLevel);
			ConnectEra(); //added by majaczek
			MiscCreatureHooks_apply();

			RegisterHandler(Daily_Res, "OnEveryDay");

			RegisterHandler(fix_vanilla_creatures, "OnAfterLoadMedia");
			//RegisterHandler(fix_vanilla_creatures, "OnAfterCreateWindow");

			RegisterHandler(StoreData, "OnSavegameWrite");
			RegisterHandler(RestoreData, "OnSavegameRead");

			RegisterHandler(drop_html_creature_list, "OnAfterCreateWindow");
			RegisterHandler(load_things_late, "OnAfterCreateWindow");


			RegisterHandler(ranged_retaliation_event_30303, "OnBeforeBattleAction");
			RegisterHandler(ranged_retaliation_event_30304, "OnAfterBattleAction");


			RegisterHandler(battle_late_begin, "OnBattleRound");
			RegisterHandler(battle_start_event, "OnBeforeBattleUniversal");

			RegisterHandler(install_CrState, "OnAfterWoG");
			RegisterHandler(Block_Redirection, "OnAfterWoG");
			RegisterHandler(Fix_Random_Creature_Generation, "OnAfterWoG");

			RegisterHandler( ShareStatsEvent,"OnBattlefieldVisible");
			break;
		}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

