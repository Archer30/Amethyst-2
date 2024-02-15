#include "stdafx.h"
extern PatcherInstance* Z_Amethyst;

auto _alldiv__nonpatched = (int (__stdcall*) (__int64 , unsigned int, int)) 0x00618090;
int __stdcall _alldiv__patched(__int64 a1, unsigned int a2, int a3) {
	if (a2 == 0) {
		return MAXINT32 >> 2;
	}
	else {
		return _alldiv__nonpatched(a1, a2, a3);
	}
		
}

/*
_LHF_(hook_0042597A) {
	if (c->edi == 0 && c->ecx == 0) {
		// c->ecx = 1;
		c->esp += 16;
		return NO_EXEC_DEFAULT;
	}
	return EXEC_DEFAULT;
}

_LHF_(hook_00425AA5) {
	if (c->ebx == 0 && c->ecx == 0) {
		// c->ecx = 1;
		c->esp += 16;
		return NO_EXEC_DEFAULT;
	}
	return EXEC_DEFAULT;
}
*/

_LHF_(hook_00442D62) {
	if (c->ecx == 0) {
		c->ecx = 1;
		/*
		// c->return_address = 0x00442D6F;
		c->eax = 0; c->edx = 0;
		c->return_address = 0x00442D90;
		return NO_EXEC_DEFAULT;
		*/
	}
	return EXEC_DEFAULT;
}

_LHF_(hook_00435D7F) {
	if (c->ecx == 0) c->ecx = 1;

	return EXEC_DEFAULT;
}

_LHF_(hook_00426020) {
	if (c->esi == 0) c->esi = 1;

	return EXEC_DEFAULT;
}

_LHF_(hook_00435DBE) {
	if (c->edi == 0) c->edi = 1;

	return EXEC_DEFAULT;
}


_LHF_(hook_0042626A) {
	if (c->ebx == 0) c->ebx = 1;

	return EXEC_DEFAULT;
}

_LHF_(hook_00468DBD) {
	c->edx = *(int*)(c->eax + 0x128);
	if (c->edx) c->return_address = 0x00468DC3;
	else		c->return_address = 0x00468DDB;
	return NO_EXEC_DEFAULT;
}

_LHF_(hook_00468A1B) {
	c->ecx = *(int*)(c->esi + 0x128);
	if (c->ecx) c->return_address = 0x00468A21;
	else		c->return_address = 0x00468A39;
	return NO_EXEC_DEFAULT;
}

_LHF_(hook_005A6BD3) {
	long tmp = c->eax <= 0 ;
	c->eax = *(int*) (c->esi );

	if (!c->eax) c->return_address = 0x005A6C75;
	else if (tmp /* && c->eax */) c->return_address = 0x005A6C24;
	else /* if (!tmp && c->eax) */ c->return_address = 0x005A6BD9;

	return NO_EXEC_DEFAULT;
}

_LHF_(hook_005A6D89) {
	c->eax = *(int*)(c->esi + 0x124);
	c->ecx = *(int*)(c->esi - 0x4);
	if (c->eax) c->return_address = 0x005A6D92;
	else c->return_address = 0x005A6DAA;
	return NO_EXEC_DEFAULT;
}

_LHF_(hook_004D9508) {
	if (c->edx < 0x1000) {
		c->return_address = 0x004D9515;
		return NO_EXEC_DEFAULT;
	}
	else
	{
		if (c->esi == *(int*)c->edx) {
			c->return_address = 0x004D951A;
			return NO_EXEC_DEFAULT;
		}
		else
		{
			++(c->eax);
			c->return_address = 0x004D950D;
			return NO_EXEC_DEFAULT;
		}
	}
}

void apply_fixes() {

	Z_Amethyst->WriteLoHook(0x005A6BD3, hook_005A6BD3);
	Z_Amethyst->WriteLoHook(0x005A6D89, hook_005A6D89);
	Z_Amethyst->WriteLoHook(0x004D9508, hook_004D9508);

	Z_Amethyst->WriteLoHook(0x00468DBD, hook_00468DBD);
	Z_Amethyst->WriteLoHook(0x00468A1B, hook_00468A1B);

	//Z_Amethyst->WriteLoHook(0x0042597A, hook_0042597A);
	//Z_Amethyst->WriteLoHook(0x00425AA5, hook_00425AA5);
	Z_Amethyst->WriteDword(0x0042597A + 1, ((long) _alldiv__patched) - 0x0042597A - 5);
	Z_Amethyst->WriteDword(0x00425AA5 + 1, ((long) _alldiv__patched) - 0x00425AA5 - 5);

	Z_Amethyst->WriteLoHook(0x0042626A, hook_0042626A);
	Z_Amethyst->WriteLoHook(0x00435DBE, hook_00435DBE);
	Z_Amethyst->WriteLoHook(0x00426020, hook_00426020);
	Z_Amethyst->WriteLoHook(0x00435D7F, hook_00435D7F);
	Z_Amethyst->WriteLoHook(0x00442D62, hook_00442D62);
	
}