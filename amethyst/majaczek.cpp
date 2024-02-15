//#include "majaczek.h"
const int sizeof__CrExpBonStr = 0x11;
const int sizeof__CrExpBon =    0x154; //= 0x11 *20 = 340
const int sizeof__CrExpModStr = 0x14;
const int sizeof__CrExpMod =    0x14*(256)+0x14+44 +0x14*42+0x14;
const int sizeof__CrExpo =		0x10;


int CRExpBon__BFBody = 0x841c48;
int CRExpBon__Cur =    0x847d98; //??? 0x845410; // 0x841c48 + 0x154 * 42
int CRExpBon__Body =   0x847f08;
int CRExpBon__Dummy =  0x847428;
//int CrexpBon__Body = 0x00847F08;

int CrExpMod__BFBody = 0x85ff50;
int CrExpMod__Body =   0x85eb50;
int CrExpMod__Cur =    0x847eec;
int CrExpMod__Dummy =  0x791e80;

int CrExpoSet__Body =  0x860550;
int LoadExpTXT__ptr =  0x71a545;

int CrExpoSet__Clear__ptr = 0x7186c1;
int CrExpMod__Clear__ptr  = 0x71a69e;
int CrExpBon__Clear__ptr  = 0x71ad24;

int H3ERA__Base__ptr =  0x700000; 
int H3ERA__Entry__ptr = 0x701000;
int CrExpBonFileLoaded__ptr = 0x008602a4;

int UnloadTxtFun__ptr = 0x55D300;
int LoadTxtFun__ptr = 0x55C2B0;