#pragma once
#include"patcher_x86_commented.hpp"

extern int sizeof__CrExpBonStr;
extern int sizeof__CrExpBon;
extern int sizeof__CrExpModStr;
extern int sizeof__CrExpMod;

extern int CRExpBon__BFBody;
extern int CRExpBon__Cur;
extern int CRExpBon__Body;
extern int CRExpBon__Dummy;

extern int CRExpMod__BFBody;
extern int CRExpMod__Cur;
extern int CRExpMod__Body;
extern int CRExpMod__Dummy;


extern int CrExpoSet__Body;
extern int LoadExpTXT__ptr;
typedef unsigned int(*LoadExpTXT__type)(void);


extern int CrExpoSet__Clear__ptr;
extern int CrExpMod__Clear__ptr;
extern int CrExpBon__Clear__ptr;

extern int H3ERA__Base__ptr;
extern int H3ERA__Entry__ptr;
extern int CrExpBonFileLoaded__ptr;

typedef	void(__fastcall *CrExpoSet__Clear__type)(void);
typedef unsigned int(*CrExpMod__Clear__type)(void);
typedef unsigned int(*CrExpBon__Clear__type)(void);


struct replace_byte {
	unsigned int address;
	unsigned char new_byte;
};

struct replace_dword {
	unsigned int address;
	unsigned int new_dword;
};

struct replace_long {
	unsigned int address;
	long int new_long;
};


/*
struct TTxtFile {
	unsigned char dummy[18];
	int ref_count;
};
typedef TTxtFile* PTxtFile;
*/

extern int UnloadTxtFun__ptr;
extern int LoadTxtFun__ptr;

// typedef void (__fastcall *UnloadTxtFun__type)(PTxtFile);
// typedef PTxtFile(__fastcall *LoadTxtFun__type)(char*);

/*
#define DIID_OK 30725
inline bool b_MsgBox(char* text, int style)
{
	CALL_12(void, __fastcall, 0x4F6C00, text, style, -1, -1, -1, 0, -1, 0, -1, 0, -1, 0);
	return (o_WndMgr->result_dlg_item_id == DIID_OK);
}
*/

inline void v_MsgBox(char* text, int style)
{
	CALL_12(void, __fastcall, 0x4F6C00, text, style, -1, -1, -1, 0, -1, 0, -1, 0, -1, 0);
	return;
}


#define Randint(Low, High) CALL_2(int, __fastcall, 0x50C7C0, (int)Low, (int)High)