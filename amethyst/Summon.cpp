//#include "..\__include\heroes.h"
#include "stdafx.h"
typedef HERO _Hero_;


#include "H3CombatMonster.hpp"
typedef H3CombatMonster _BattleStack_;

void  PEr__Del(void) {

}

#define RETURN(x) {/*PEr::Del();*/ PEr__Del(); return(x);}
#define RETURNV {/*PEr::Del();*/ PEr__Del(); return;}


int M2B_BFPos[11][15 + 15] = {
  {0,0x01,0,0x02,0,0x03,0,0x04,0,0x05,0,0x06,0,0x07,0,0x08,0,0x09,0,0x0A,0,0x0B,0,0x0C,0,0x0D,0,0x0E,0,0x0F},
  {0x12,0,0x13,0,0x14,0,0x15,0,0x16,0,0x17,0,0x18,0,0x19,0,0x1A,0,0x1B,0,0x1C,0,0x1D,0,0x1E,0,0x1F,0,0x20,0},
  {0,0x23,0,0x24,0,0x25,0,0x26,0,0x27,0,0x28,0,0x29,0,0x2A,0,0x2B,0,0x2C,0,0x2D,0,0x2E,0,0x2F,0,0x30,0,0x31},
  {0x34,0,0x35,0,0x36,0,0x37,0,0x38,0,0x39,0,0x3A,0,0x3B,0,0x3C,0,0x3D,0,0x3E,0,0x3F,0,0x40,0,0x41,0,0x42,0},
  {0,0x45,0,0x46,0,0x47,0,0x48,0,0x49,0,0x4A,0,0x4B,0,0x4C,0,0x4D,0,0x4E,0,0x4F,0,0x50,0,0x51,0,0x52,0,0x53},
  {0x56,0,0x57,0,0x58,0,0x59,0,0x5A,0,0x5B,0,0x5C,0,0x5D,0,0x5E,0,0x5F,0,0x60,0,0x61,0,0x62,0,0x63,0,0x64,0},
  {0,0x67,0,0x68,0,0x69,0,0x6A,0,0x6B,0,0x6C,0,0x6D,0,0x6E,0,0x6F,0,0x70,0,0x71,0,0x72,0,0x73,0,0x74,0,0x75},
  {0x78,0,0x79,0,0x7A,0,0x7B,0,0x7C,0,0x7D,0,0x7E,0,0x7F,0,0x80,0,0x81,0,0x82,0,0x83,0,0x84,0,0x85,0,0x86,0},
  {0,0x89,0,0x8A,0,0x8B,0,0x8C,0,0x8D,0,0x8E,0,0x8F,0,0x90,0,0x91,0,0x92,0,0x93,0,0x94,0,0x95,0,0x96,0,0x97},
  {0x9A,0,0x9B,0,0x9C,0,0x9D,0,0x9E,0,0x9F,0,0xA0,0,0xA1,0,0xA2,0,0xA3,0,0xA4,0,0xA5,0,0xA6,0,0xA7,0,0xA8,0},
  {0,0xAB,0,0xAC,0,0xAD,0,0xAE,0,0xAF,0,0xB0,0,0xB1,0,0xB2,0,0xB3,0,0xB4,0,0xB5,0,0xB6,0,0xB7,0,0xB8,0,0xB9}
};


int M2B_FindPos(int Code, int* x, int* y)
{
    int i, j;

    //#include "templ.h"
    for (i = 0; i < 30; i++)
        for (j = 0; j < 11; j++)
            if (M2B_BFPos[j][i] == Code) { *x = i; *y = j; RETURN(0) }
    RETURN(-1)
}

int M2B_GetPosCode(int x, int y)
{
//#include "templ.h"
    int ret;
    if (x < 0) x = 0; if (x > 29) x = 29;
    if (y < 0) y = 0; if (y > 10) y = 10;
    ret = M2B_BFPos[y][x];
    if (ret != 0) RETURN(ret)
        if (x == 0) { x = 1; RETURN(M2B_BFPos[y][x]) }
    if (x == 29) { x = 28; RETURN(M2B_BFPos[y][x]) }
    if (y == 0) { if (x < 15) x += 1; else x -= 1; RETURN(M2B_BFPos[y][x]) }
    if (y == 10) { if (x < 15) x += 1; else x -= 1; RETURN(M2B_BFPos[y][x]) }
    RETURN(0)
}

int M2B_CheckPosCode(int x, int y)
{
//#include "templ.h"
    if (x < 0)  RETURN(-1)
        if (x > 29) RETURN(-1)
            if (y < 0)  RETURN(-1)
                if (y > 10) RETURN(-1)
                    RETURN(0)
}

static int SG_Shift[6][2] = { {2,0},{-2,0},{-1,1},{-1,-1},{1,1},{1,-1} };
static int SC_Type, SC_Num, SC_Pos, SC_Side, SC_Slot, SC_Redraw, SC_Flags;
static Byte* SC_MStruct;
Byte* SammonCreatureEx(int type, int num, int pos, int side, int slot, int redraw, int Flags)
{
//#include "templ.h"
    SC_Type = type; // тип существа
    SC_Num = num; // число существ
    SC_Pos = pos; // позиция на поле
    SC_Side = side; // сторона игрока (0,1)
    SC_Slot = slot; // слот в армии героя
    SC_Redraw = redraw; // перерисовывать
    SC_Flags = Flags; // флаги вызванного существа
    __asm {
        //005A775B 8B83F4000000   mov    eax,[ebx+000000F4] ebx -> генерирующий монстр
        mov    ecx, 0x699420
        mov    ecx, [ecx]
        push   SC_Redraw
        push   SC_Flags
        push   SC_Pos     //edx  = координата клетки поля
        push   SC_Num     //ecx  = количество
        push   SC_Type    //00000030 = номер монстра
        push   SC_Side    //eax = номер игрока (0,1)
    //005A776D 8BCF           mov    ecx,edi -> комбат менеждер
    mov    eax, 0x479A30
    call   eax
    //005A776F E8BC22EDFF     call   H3WOG.00479A30
    mov    edx, eax
    mov    SC_MStruct, eax  //-> ук. на нового монстра
    or edx, edx
    je     l_notset
    mov    eax, SC_Slot     //[ebp+0C] = номер слота в армии героя
//005A7779 8BCF           mov    ecx,edi
mov[edx + 0x5C], eax
////    mov    ecx,0x699420
////    mov    ecx,[ecx]
////    mov    eax,0x493290
////    call   eax
//005A777E E80DBBEEFF     call   H3WOG.00493290
    }
    //CrExpBon::ClearCrExpBonBFStartStat(MonIndex(SC_MStruct));
l_notset:
    RETURN(SC_MStruct)
}

Byte* SammonCreature(int type, int num, int pos, int side, int slot, int redraw) {
//#include "templ.h"
    RETURN(SammonCreatureEx(type, num, pos, side, slot, redraw, 0));
}

int SG_GuardsSet = 0;
struct _SG_GuardsSetUp_ {
    int sg_stack;
    int own_stack;
    int own_type;
    int own_speed;
} SG_GuardsSetUp[42];
void MakeMonGuards(Byte* man, Byte* mon, int SG_stack, int type)
{
//#include "templ.h"
    _Hero_* hpo = (_Hero_*)*(Dword*)&man[0x53CC];
    _Hero_* hpe = (_Hero_*)*(Dword*)&man[0x53D0];
    int(*SG_shpo)[2];
    //  if(*((int *)&[0x34])==172){        // Santa Gremlin
    //   if((*(Dword *)&man[0x13D6C])==1){
    //    if(man[0x13D68]==0){
    Byte* gmon;
    int i, j, n, /* type, */ gm, num, x, y, x1, y1;
    int Code = *(int*)&mon[0x38];
    //*(int *)&mon[0x3C]=2;
    *(int*)&mon[0x4E0] = 16; // frost ring
    num = *(int*)&mon[0x60];
    gm = *(int*)&mon[0xF4];
    _Hero_* hp;
    if (gm) hp = hpe; else hp = hpo;
    n = 6;
    //      SG_Shift[2][0]=SG_Shift[3][0]=-1;
    //      SG_Shift[1][0]=-2; SG_Shift[0][0]=2;
    SG_shpo = SG_Shift;
    /*
    if (hp != 0) {
        type = (hp->Spec / 2) * 14;
        if (type == 14) type = 16; // Кентавры на Гномов
        if (type == 112) type = 118; // Элементали на Пикселей
        if ((hp->Spec / 2) == 1) { // Рампарт
            num = num / 2;
            if (num == 0) num = 1;
        }
    }
    else {
        type = 28; // гремлин
    }*/
    //CrExpBon::SGBonus(mon, &type, &num);
    if (M2B_FindPos(Code, &x, &y) == -1) { /* Error(); */ RETURNV }
    for (i = 0; i < n; i++) {
        if ((SG_shpo[i][0] == 0) && (SG_shpo[i][1] == 0)) continue;
        x1 = x + SG_shpo[i][0];
        y1 = y + SG_shpo[i][1];
        if (M2B_CheckPosCode(x1, y1) == 0) {
            if ((Code = M2B_GetPosCode(x1, y1)) != 0) {
                gmon = SammonCreature(type, num, Code, gm, -1, 0);
                if (gmon != 0) {
                    SG_GuardsSetUp[SG_GuardsSet].sg_stack = SG_stack;
                    for (j = 0; j < (21 * 2); j++) {
                        // 3.58 Tower skip
                        if (j == 20) continue; // 3.58
                        if (j == 41) continue; // 3.58
                        if (&man[0x54CC + 0x548 * j] == gmon) break;
                    }
                    if (j == 42) j = -1;
                    SG_GuardsSetUp[SG_GuardsSet].own_stack = j;
                    SG_GuardsSetUp[SG_GuardsSet].own_type = type;
                    SG_GuardsSetUp[SG_GuardsSet].own_speed = *(int*)&gmon[0xC4];
                    ++SG_GuardsSet;
                    *(int*)&gmon[0xC4] = -1;
                }
            }
        }
    }
    RETURNV
}

int DalionType = -1; int Dalion_Stack = 0;
void(__stdcall* MakeSGGuards)(Byte*, Byte*, int) = (void(__stdcall*)(Byte*, Byte*, int))0x0075CDA2;
//extern DWORD DalionsGuards[MONSTERS_AMOUNT];
//extern char hasSantaGuards[MONSTERS_AMOUNT];
extern DWORD* DalionsGuards;
extern char*  hasSantaGuards;

void MakeMonGuards_reverse(int type, int SG_stack, Byte* mon, Byte* man ) {
    if(mon && man)
        MakeMonGuards( man, mon, SG_stack, type);
}


void MakeSGGuards_unstack(Byte* man, Byte* mon, int SG_stack) {
}


void MakeMonGuards_unstack(Byte* man, Byte* mon, int SG_stack, int zType) {
}

namespace MakeMonGuards_proxy {

    int z_type; Byte* man; Byte* mon; int SG_stack; 
    int Z_ESP; int Z_EAX;

    __declspec(naked) void /* __stdcall */ MakeMonGuards_proxy() {
        __asm {
            mov Z_ESP, esp
            mov Z_EAX, eax
            mov eax, [esp+0x04]
            mov z_type, eax
            mov eax, [esp+0x08]
            mov man, eax
            mov eax, [esp+0x0C]
            mov mon, eax
            mov eax, [esp+0x10]
            mov SG_stack, eax
            mov eax, Z_EAX
        }
        MakeMonGuards(man, mon, SG_stack, z_type);
        __asm {
            mov esp, Z_ESP
            push 0x0075D10F
            ret
        }
    }
}
__declspec(naked) void /* __stdcall */ MakeMonGuards_proxy_bad(int type, Byte* man, Byte* mon, int SG_stack) {

    if (mon && ((_BattleStack_*)mon)->type >= 0
        && hasSantaGuards[((_BattleStack_*)mon)->type])
        MakeMonGuards(man, mon, SG_stack, type);
    else MakeMonGuards_unstack(man, mon, SG_stack, type);
}



__declspec(naked) void __stdcall MakeDalionsGuards(Byte* man, Byte* mon, int SG_stack) {


    __asm
    {
        mov eax, [esp + 4]
        cmp eax, 0
        je Bad
        mov eax, [eax + 0x34]
        cmp eax, 0xffffffff
        je Bad
        mov eax, [DalionsGuards + eax * 4]
        //mov DalionType, eax
        //cmp DalionType, 0xffffffff
        cmp eax, 0xffffffff

        je Santa

        Dalion :
            push eax
            call MakeMonGuards_proxy::MakeMonGuards_proxy
            push 0x0075D10F
            ret

            Santa :
        call MakeSGGuards
            push 0x0075D10F
            ret

            Bad :

        call MakeSGGuards_unstack
            push 0x0075D10F
            ret
    }
}

__declspec(naked) void /* __stdcall */ MakeDalionsGuards_old(Byte* man, Byte* mon, int SG_stack) {
   
    
    __asm
    {
        //pop SG_stack
        //pop mon
        //pop man
        mov eax, [esp + 4]
        mov Dalion_Stack, eax
        cmp Dalion_Stack, 0
        je Bad
        mov eax, [Dalion_Stack +0x34]
        cmp eax, 0xffffffff
        je Bad
        mov eax, [DalionsGuards+eax*4]
        mov DalionType, eax

        //cmp man, 0
        cmp DalionType, 0xffffffff
        je Santa

        Dalion:
        //push man
        //push mon
        //push SG_stack
        push DalionType
        call MakeMonGuards_proxy::MakeMonGuards_proxy
        push 0x0075D10F
        ret
        
        Santa:
        //push man
        //push mon
        //push SG_stack

        call MakeSGGuards
        push 0x0075D10F
        ret

        Bad:

        call MakeSGGuards_unstack
        push 0x0075D10F
        ret
    }
}


__declspec(naked) void __stdcall  MakeDalionsGuards_bad(Byte* man, Byte* mon, int SG_stack) {
    //_asm pusha
    if (man && mon) {
        // /*int type*/ DalionType = DalionsGuards[((_BattleStack_*)mon)->type];

        // /*int type*/ DalionType = DalionsGuards[((_BattleStack_*)mon)->type];
        if (/*type*/ DalionType < 0 || /*type*/ DalionType >= MONSTERS_AMOUNT)
            MakeSGGuards(man, mon, SG_stack);
        else MakeMonGuards(man, mon, SG_stack, /* type */ DalionType);
    }
    __asm
    {
        //popa

        //push 0x0075D10F
        ret
    }
}