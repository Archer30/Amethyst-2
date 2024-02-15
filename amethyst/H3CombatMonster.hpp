#pragma once
//////////////////////////////////////////////////////////////////////
//                                                                  //
//                     Created by RoseKavalier:                     //
//                     rosekavalierhc@gmail.com                     //
//                       Created: 2019-12-06                        //
//        ***You may use or distribute these files freely           //
//            so long as this notice remains present.***            //
//                                                                  //
//                     modified by majaczek                         //
//                                                                  //
//////////////////////////////////////////////////////////////////////


#pragma once
#include <windows.h>

typedef  char h3unk;
typedef  int  INT32;
typedef  unsigned int  UINT32;

typedef  short  INT16;
typedef  unsigned short  UINT16;

typedef char H3Vector_Blank[16];
struct H3Resources
{
	enum H3ResType
	{
		RT_Wood = 0,
		RT_Mercury = 1,
		RT_Ore = 2,
		RT_Sulfur = 3,
		RT_Crystal = 4,
		RT_Gems = 5,
		RT_Gold = 6,
	};

	INT32 wood;
	INT32 mercury;
	INT32 ore;
	INT32 sulfur;
	INT32 crystal;
	INT32 gems;
	INT32 gold;
};
// * bitfield flags for creatures
struct H3CreatureFlags
{
	union
	{
		struct
		{
			unsigned DOUBLE_WIDE : 1; // 1
			unsigned FLYER : 1; // 2
			unsigned SHOOTER : 1; // 4
			unsigned EXTENDED_ATTACK : 1; // 8 ~ aka dragon breath
			unsigned ALIVE : 1; // 10
			unsigned DESTROY_WALLS : 1; // 20
			unsigned SIEGE_WEAPON : 1; // 40
			unsigned KING1 : 1; // 80 ~ all creatures of 7th level and neutral dragons that do not belong to the KING2 or KING3
			unsigned KING2 : 1; // 100
			unsigned KING3 : 1; // 200
			unsigned MIND_IMMUNITY : 1; // 400
			unsigned NO_OBSTACLE_PENALTY : 1; // 800
			unsigned NO_MELEE_PENALTY : 1; // 1000
			unsigned unk2000 : 1; // 2000
			unsigned FIRE_IMMUNITY : 1; // 4000
			unsigned DOUBLE_ATTACK : 1; // 8000
			unsigned NO_RETALIATION : 1; // 10000
			unsigned NO_MORALE : 1; // 20000
			unsigned UNDEAD : 1; // 40000
			unsigned ATTACK_ALL_AROUND : 1; // 80000
			unsigned MAGOG : 1; // 100000
			unsigned CANNOT_MOVE : 1; // 200000 ~21
			unsigned SUMMON : 1; // 400000
			unsigned CLONE : 1; // 800000
			unsigned MORALE : 1; // 1000000
			unsigned WAITING : 1; // 2000000 ~25
			unsigned DONE : 1; // 4000000
			unsigned DEFENDING : 1; // 8000000
			unsigned SACRIFICED : 1; // 10000000
			unsigned NO_COLORING : 1; // 20000000
			unsigned GRAY : 1; // 40000000
			unsigned DRAGON : 1; // 80000000
		};
		struct /* deprecated names */
		{
			/*
			_H3API_DEPRECATED_("Use EXTENDED_ATTACK")     unsigned EXTENDED : 1;
			_H3API_DEPRECATED_("Use DESTROY_WALLS")       unsigned DESTROYWALLS : 1;
			_H3API_DEPRECATED_("Use SIEGE_WEAPON")        unsigned SIEGEWEAPON : 1;
			_H3API_DEPRECATED_("Use MIND_IMMUNITY")       unsigned MINDIMMUNITY : 1;
			_H3API_DEPRECATED_("Use NO_OBSTACLE_PENALTY") unsigned NOOBSTACLEPENALTY : 1;
			_H3API_DEPRECATED_("Use NO_MELEE_PENALTY")    unsigned NOMELEEPENALTY : 1;
			_H3API_DEPRECATED_("Use FIRE_IMMUNITY")       unsigned FIREIMMUNITY : 1;
			_H3API_DEPRECATED_("Use DOUBLE_ATTACK")       unsigned DOUBLEATTACK : 1;
			_H3API_DEPRECATED_("Use NO_RETALIATION")      unsigned NORETALIATION : 1;
			_H3API_DEPRECATED_("Use NO_MORALE")           unsigned NOMORALE : 1;
			_H3API_DEPRECATED_("Use ATTACK_ALL_AROUND")   unsigned ATTACKALLAROUND : 1;
			_H3API_DEPRECATED_("Use CANNOT_MOVE")         unsigned CANNOTMOVE : 1;
			_H3API_DEPRECATED_("Use NO_COLORING")         unsigned NOCOLORING : 1;
			*/
			unsigned flags;
		};
	};
};

// * a substructure of H3CombatMonster related to spells
struct H3CombatMonsterSpellsData
{
	INT32 bless_damage;            // 0x458
	INT32 curse_damage;            // 0x45C
	INT32 anti_magic;              // 0x460
	INT32 bloodlust_effect;        // 0x464
	INT32 precision_effect;        // 0x468
	INT32 weakness_effect;         // 0x46C
	INT32 stone_skin_effect;       // 0x470
	INT32 unknown13;               // 0x474
	INT32 prayer_effect;           // 0x478
	INT32 mirth_effect;            // 0x47C
	INT32 sorrow_effect;           // 0x480
	INT32 fortune_effect;          // 0x484
	INT32 misfortune_effect;       // 0x488
	INT32 slayer_type;             // 0x48C - called KING_1/2/3
	INT32 unknown14;               // 0x490 - Max traversed cells before hitting?
	INT32 counterstrike_effect;    // 0x494
	FLOAT frenzyMultiplier;        // 0x498
	INT32 blind_effect;            // 0x49C - for calculating damage retaliation damage?
	FLOAT fire_shield_effect;      // 0x4A0
	INT32 unknown16;               // 0x4A4
	FLOAT protection_air_effect;   // 0x4A8 - in % as below
	FLOAT protection_fire_effect;  // 0x4AC
	FLOAT protection_water_effect; // 0x4B0
	FLOAT protection_earth_effect; // 0x4B4
	INT32 shield_effect;           // 0x4B8
	INT32 air_shield_effect;       // 0x4BC
	INT8 blinded;                  // 0x4C0 - to reduce damage?
	INT8 paralyzed;                // 0x4C1 - to reduce damage?
	h3unk _f_4C2[2];               // 0x4C2-0x4C3
	INT32 forgetfulness_level;     // 0x4C4
	FLOAT slow_effect;             // 0x4C8
	INT32 haste_effect;            // 0x4CC - value added/removed
	INT32 disease_attack_effect;   // 0x4D0
	INT32 disease_defense_effect;  // 0x4D4
	h3unk _f_4D8[8];               // 0x4D8-0x4DC
	INT32 faerie_dragon_spell;     // +4E0
	INT32 magic_mirror_effect;     // 0x4E4
	INT32 morale;                  // +4E8
	INT32 luck;                    // +4EC
	h3unk _f_4F0[4];
	/*H3Vector<H3CombatMonster*> */ H3Vector_Blank dendroidBinder;  // +4F4 which dendroids have binded the current target (used for animation requirement)
	/*H3Vector<H3CombatMonster*> */ H3Vector_Blank dendroidBinds;   // +504 a list of H3CombatMonsters binded by this dendroid
	h3unk _f_514[20];
	INT32 Hypnotize_528;
	INT32 Hypnotize_52C;
	h3unk _f_530[24];
};


// * Cranim.txt
struct H3MonsterAnimation
{
	enum eMissiles
	{
		M_UPPER_RIGHT,
		M_RIGHT,
		M_LOWER_RIGHT
	};

	struct H3MissileOffets
	{
		INT16 offset_x;
		INT16 offset_y;
	}missiles[3];

	INT32 missileFrameAngles[12]; // from high to low (90 to -90)
	INT32 troopCountLocationOffset;
	INT32 attackClimaxFrame;
	INT32 timeBetweenFidgets;
	INT32 walkAnimationTime;
	INT32 attackAnimationTime;
	INT32 flightAnimationTime;
};


// * hardcoded creature information in heroes3.exe
struct H3CreatureInformation
{
	//  * +0
	// -1 means neutral
	INT32 town;
	//  * +4
	// 0 ~ 6
	INT32 level;
	//  * +8
	LPCSTR soundName;
	//  * +C
	LPCSTR defName;
	//  * +10
	H3CreatureFlags flags;
	//  * +14
	LPCSTR nameSingular;
	//  * +18
	LPCSTR namePlural;
	//  * +1C
	LPCSTR description;
	//  * +20
	H3Resources cost;
	//  * +3C
	INT32  fightValue;
	// * +40
	INT32  aiValue;
	//  * +44
	INT32  grow;
	// * +48
	INT32  hGrow;
	// * +4C
	INT32  hitPoints;
	// * +50
	INT32  speed;
	// * +54
	INT32  attack;
	// * +58
	INT32  defence;
	// * +5C
	INT32  damageLow;
	// * +60
	INT32  damageHigh;
	// * +64
	INT32  numberShots;
	// * +68
	INT32  spellCharges;
	// * +6C
	INT32  advMapLow;
	// * +70
	INT32  advMapHigh;
	/*
	_H3API_ LPCSTR GetCreatureName(INT32 count);
	_H3API_ H3Resources UpgradeCost(H3CreatureInformation* upg, INT32 count);
	*/
};

typedef void H3LoadedDef;

// * monster information on battlefield
struct H3CombatMonster
{
protected:
	h3unk _f_000[52];
public:
	// * +34
	INT32 type;
	// * +38
	// * position on battlefield
	INT32 position;
	// * +3C
	INT32 animation;
	// * +40
	INT32 animationFrame;
	// * +44
	// * left or right
	INT32 secondHexOrientation;
protected:
	h3unk _f_048[4];
public:
	// * +4C
	// the number of creatures that are currently alive
	INT32 numberAlive;
	// * +50
	INT32 previousNumber;
protected:
	h3unk _f_054[4];
public:
	// * +58
	// * the number of lost hitpoints of top creature in stack
	INT32 healthLost;
	// * +5C
	// * ?reference to position on side?
	INT32 slotIndex;
	// * +60
	// * the number of creatures in this stack to compare against resurrection
	INT32 numberAtStart;
protected:
	h3unk _f_064[8];
public:
	// * +6C
	// * maximum hit points
	INT32 baseHP;
	// * +70
	INT32 isLucky;
public:
	// * +74
	// * a copy of H3CreatureInformation using combat values in some places
	H3CreatureInformation info;
protected:
	h3unk _f_0E8[4];
public:
	// * +EC
	// * set in After-Hit spell subroutine 0x440220
	INT32 spellToApply;
protected:
	h3unk _f_0F0[4];
public:
	// * +F4
	// * left or right
	INT32 side;
	// * +F8
	//  * reference to position on side
	INT32 sideIndex;
protected:
	// * +FC
	UINT32 last_animation_time;
	// * +100
	INT32 yOffset;
	// * +104
	INT32 xOffset;
	h3unk _f_108[8];
	// * +110 from cranim
	H3MonsterAnimation cranim;
public:
	// * +164
	H3LoadedDef* def;
protected:
	// * +168
	H3LoadedDef* shootingDef;
	h3unk _f_16C[4];
	// * +170
	UINT32 moveSound;
	// * +174
	UINT32 attackSound;
	// * +178
	UINT32 getHitSound;
	// * +17C
	UINT32 shotSound;
	// * +180
	UINT32 deathSound;
	// * +184
	UINT32 defendSound;
	// * +188
	UINT32 extraSound1;
	// * +18C
	UINT32 extraSound2;
	h3unk _f_190[4];
public:
	// * +194
	// * the number of spells currently active
	INT32 activeSpellsNumber;
	// * +198
	// * the remaining number of turns of any spells
	INT32 activeSpellsDuration[81];
	// * +2DC
	// * the secondary skill level of applied spells
	INT32 activeSpellsLevel[81];
protected:
	h3unk _f_420[52];
public:
	// * +454
	// * number of retaliations left
	INT32 retaliations;
	// * +458
	// * information about some spell effects
	H3CombatMonsterSpellsData spellsData;
	/*
	// * returns appropriate name of stack
	_H3API_ LPCSTR GetCreatureName();
	// * returns second square if creature occupies 2 squares
	_H3API_ INT32 GetSecondSquare();
	// * returns actual speed of creature
	_H3API_ INT32 GetStackSpeed();
	_H3API_ BOOL IsDone();
	_H3API_ BOOL IsClone();
	_H3API_ BOOL IsSiege();
	_H3API_ BOOL IsSummon();
	_H3API_ BOOL HadMorale();
	_H3API_ BOOL IsWaiting();
	_H3API_ BOOL HasMoved();
	// * index 0 ~ 41
	_H3API_ INT32 Index();
	// * show creature information dialog
	_H3API_ VOID ShowStatsDialog(BOOL RightClick);
	// * Checks if hypnotized
	_H3API_ INT32 GetSide();
	// * Checks if hypnotized
	_H3API_ H3Hero* GetOwner();
	// * the bonus/decreased effect on a spell from targeting a creature
	_H3API_ INT32 GetProtectiveSpellEffect(INT32 damage, INT32 spellID);
	// * odds of magic resisting
	_H3API_ INT32 MagicMirrorEffect();

	_H3API_ INT32 ApplyPhysicalDamage(INT32 amount);
	_H3API_ VOID  ApplySpell(INT32 spellId, INT32 spellPower, INT32 schoolLevel, H3Hero* hero);
	_H3API_ BOOL8 CanReceiveSpell(INT32 spellId);
	_H3API_ BOOL  CanCastSpellAtEmptyHex(INT32 hexId);
	_H3API_ BOOL8 MoveToHex(INT32 hexId);
	_H3API_ BOOL8 UseEnchanters();
	_H3API_ INT32 GetX() const;
	_H3API_ INT32 GetY() const;
	*/
};