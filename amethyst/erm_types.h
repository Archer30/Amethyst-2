#pragma once
/*
struct _Mes_ {
	char* s;
	long  l;
};
*/
struct VarNum {
	//  signed     Num  : 21; // флаг, переменная (1...1000) или число
	//#define VN_MAXVAL 1048575
	//#define VN_MINVAL -1048576
	signed     Num : 32; // флаг, переменная  или число
// #define VN_MAXVAL 0x7FFFFFFF// 2147483647
// #define VN_MINVAL ((int)0x80000000)//-2147483647
	unsigned   Type : 4; // тип перем
  // 0=число, 1=флаг, 2=f...t, 3=v1...1000, 4=w1...100, 5=x1...100, 6=y1...100
  // 7=z1...500,8=e1...e100
  // ******* 8=Scope v1...1000, 9=Scope z1...z500
	unsigned   IType : 4;
	// indexed
	// 0=нет, 1=флаг, 2=f...t, 3=v1...1000, 4=w1...100, 5=x1...100, 6=y1...100
	// ******* 8=Scope v1...1000 float
	unsigned   Check : 3; // тип проверки
  // 0=nothing, 1?, 2=, 3<>, 4>, 5<, 6>=, 7<=
  
};

struct Mes {
	long  i;    // первым для быстрого доступа
	_Mes_ m;
	//  VarNum Efl [16][2];
	//  VarNum Ofl [16][2];      // до 16 флагов зависимости |
	VarNum Efl[2][16][2];      // до 16 флагов зависимости & и |
	VarNum VarI[16];
	char  c[16];
	char  f[16];
	int   n[16];
};

struct _ToDo_ {
	Word   Type;            // +0 тип объекта изменения
	Byte   Disabled;        // запрещен приемник
	Byte   DisabledPrev;    // пред состояние запрета
	VarNum Efl[2][16][2];      // до 16 флагов зависимости & и |
	Dword  Pointer;         // указатель на структуру
	VarNum Par[16];         // указатель на структуру свойств этого объекта
	int    ParSet;          // сколько Pointers установлено
	_Mes_  Self;            // HE#:
	_Mes_  Com;             // X#/#/#/#/#
};


inline char* ERM2String(char* mes, int zstr, int* len) {
	return CALL_3(char*,__cdecl,0x0073DF05,mes,zstr,len);
}


inline void Mess(Mes* m) {

}