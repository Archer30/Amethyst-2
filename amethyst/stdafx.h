// stdafx.h: ���������� ���� ��� ����������� ��������� ���������� ������
// ��� ���������� ������ ��� ����������� �������, ������� ����� ������������, ��
// �� ����� ����������
//

#pragma once

#include "targetver.h"

// #pragma pack (push, 1)

#define WIN32_LEAN_AND_MEAN             // ��������� ����� ������������ ���������� �� ���������� Windows
// ����� ���������� Windows:
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include "Tchar.h"

#include <io.h>

#include <fstream>

// #include "..\__include\era.h"

// TODO. ���������� ����� ������ �� �������������� ���������, ����������� ��� ���������

#include "..\__Include\heroes.h"


inline bool FileExists(const char* chPath) {
	return(_access(chPath, 0) == 0);
}

#define MONSTERS_AMOUNT 1024
// were 197

#define MAXCRINFONUM 96000L
//were 10000L

constexpr long homm3_rand = 0x0061842C;
constexpr long CombatMan_SummonMonster = 0x005A7390;

#include "majaczek.h"