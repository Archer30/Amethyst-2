#pragma once
// * information about H3 spells
struct H3Spell
{
	// * +0
	// * -1 enemy
	// * 0 area
	// * 1 friendly
	INT32 type;
	// * +4
	// * the soundname to use
	LPCSTR soundName;
protected:
	// * +8
	UINT32 animationRelated;
public:
	// * +C
	// * bitfield of spell data
	struct SpellFlags
	{
		// * flag 1
		unsigned battlefield_spell : 1;
		// * flag 2
		unsigned map_spell : 1;
		// * flag 4
		unsigned time_scale : 1;
		// * flag 8
		unsigned creature_spell : 1;
		// * flag 10
		unsigned single_target : 1;
		// * flag 20
		unsigned single_shooting_stack : 1;
		// * flag 40
		unsigned expert_mass_version : 1;
		// * flag 80
		unsigned target_anywhere : 1;
		// * flag 100
		unsigned remove_obstacle : 1;
		// * flag 200
		// * All damage spells
		unsigned damage_spell : 1;
		// * flag 400
		unsigned mind_spell : 1;
		// * flag 800
		unsigned friendly_mass : 1;
		// * flag 1000
		unsigned cannot_target_siege : 1;
		// * flag 2000
		unsigned spell_from_artifact : 1;
		// * flag 4000
		// * Air/Fire Shield, Protections From, Anti-Magic, Magic Mirror, Stone Skin, Counterstrike
		unsigned defensive : 1;
		// * flag 8000
		// * All damage spells except INFERNO and CHAING LIGHTNING
		unsigned AI_damage_spells : 1;
		// * flag 10000
		// * Inferno and Chaing Lightning
		unsigned AI_area_effect : 1;
		// * flag 20000
		// * Death Ripple, Destroy Undead and Armageddon
		unsigned AI_mass_damage_spells : 1;
		// * flag 40000
		// * Shield, Air Shield, ... Berserk, Teleport, Clone
		// * NO SUMMON SPELLS
		unsigned AI_non_damage_spells : 1;
		// * flag 80000
		// * Resurrection, Animate Dead
		// * Hypnotize
		// * 4 Summon Spells
		unsigned AI_creatures : 1;
		// * flag 100000
		// * Summon Boat, Fly, WW, DD, TP
		// * Earthquake, Titan's Lightning Bolt (not sure why these are here???)
		unsigned AI_adventure_map : 1;
		// * flag 200000+
		// * there are no spells with these flags
		unsigned _unused : 11;
	}flags;
	// * +10
	// * full name
	LPCSTR name;
	// * +14
	// * short name
	LPCSTR shortName;
	// * +18
	// * 0~5
	INT32 level;
	enum eSchool : INT32
	{
		AIR = 1,
		FIRE = 2,
		WATER = 4,
		EARTH = 8
	};
	// * +1C
	eSchool school;
	// * +20
	// * mana cost at each spell expertise
	INT32 mana_cost[4];
	// * +30
	// * value multiplied by spell power
	INT32 sp_effect;
	// * +34
	// * base value of spell for calculations
	INT32 base_value[4];
	// * +44
	// * change for each class?
	INT32 chance_to_get[9];
	// * +68
	UINT32 ai_value[4];
	// * 78
	// * description of spell based on secondary skill level
	LPCSTR description[4];

	enum eSpells
	{
		SUMMON_BOAT = 0,
		SCUTTLE_BOAT = 1,
		VISIONS = 2,
		VIEW_EARTH = 3,
		DISGUISE = 4,
		VIEW_AIR = 5,
		FLY = 6,
		WATER_WALK = 7,
		DIMENSION_DOOR = 8,
		TOWN_PORTAL = 9,
		QUICK_SAND = 10,
		LAND_MINE = 11,
		FORCE_FIELD = 12,
		FIRE_WALL = 13,
		EARTHQUAKE = 14,
		MAGIC_ARROW = 15,
		ICE_BOLT = 16,
		LIGHTNING_BOLT = 17,
		IMPLOSION = 18,
		CHAIN_LIGHTNING = 19,
		FROST_RING = 20,
		FIREBALL = 21,
		INFERNO = 22,
		METEOR_SHOWER = 23,
		DEATH_RIPPLE = 24,
		DESTROY_UNDEAD = 25,
		ARMAGEDDON = 26,
		SHIELD = 27,
		AIR_SHIELD = 28,
		FIRE_SHIELD = 29,
		PROTECTION_FROM_AIR = 30,
		PROTECTION_FROM_FIRE = 31,
		PROTECTION_FROM_WATER = 32,
		PROTECTION_FROM_EARTH = 33,
		ANTI_MAGIC = 34,
		DISPEL = 35,
		MAGIC_MIRROR = 36,
		CURE = 37,
		RESURRECTION = 38,
		ANIMATE_DEAD = 39,
		SACRIFICE = 40,
		BLESS = 41,
		CURSE = 42,
		BLOODLUST = 43,
		PRECISION = 44,
		WEAKNESS = 45,
		STONE_SKIN = 46,
		DISRUPTING_RAY = 47,
		PRAYER = 48,
		MIRTH = 49,
		SORROW = 50,
		FORTUNE = 51,
		MISFORTUNE = 52,
		HASTE = 53,
		SLOW = 54,
		SLAYER = 55,
		FRENZY = 56,
		TITANS_LIGHTNING_BOLT = 57,
		COUNTERSTRIKE = 58,
		BERSERK = 59,
		HYPNOTIZE = 60,
		FORGETFULNESS = 61,
		BLIND = 62,
		TELEPORT = 63,
		REMOVE_OBSTACLE = 64,
		CLONE = 65,
		FIRE_ELEMENTAL = 66,
		EARTH_ELEMENTAL = 67,
		WATER_ELEMENTAL = 68,
		AIR_ELEMENTAL = 69,
		/* These abilities are not available to heroes */
		STONE = 70,
		POISON = 71,
		BIND = 72,
		DISEASE = 73,
		PARALYZE = 74,
		AGING = 75,
		DEATH_CLOUD = 76,
		THUNDERBOLT = 77,
		DRAGONFLY_DISPEL = 78,
		DEATH_STARE = 79,
		ACID_BREATH = 80,
	};

	// _H3API_ INT32 GetBaseEffect(INT32 level, INT32 spellPower);
};