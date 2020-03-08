#ifndef __RETRO_INPUT__
#define __RETRO_INPUT__

#include "burner.h"
#include "burn_gun.h"

// Stuff we don't need anymore but want to keep in the code
//#define RETRO_INPUT_DEPRECATED

struct KeyBind
{
	unsigned id;
	unsigned port;
	unsigned device;
	int index;
	unsigned position;
};

struct AxiBind
{
	unsigned id;
#ifdef RETRO_INPUT_DEPRECATED
	unsigned id_pos;
	unsigned id_neg;
#endif
	int index;
};

#define RETROPAD_CLASSIC	RETRO_DEVICE_ANALOG
#define RETROPAD_MODERN		RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_ANALOG, 1)
#define RETROMOUSE_BALL		RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_ANALOG, 2)
#define RETROMOUSE_FULL		RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_MOUSE, 1)

#define GIT_DIRECT_COORD	(0x11)

#define JOY_NEG 0
#define JOY_POS 1
#define RETRO_DEVICE_ID_JOYPAD_EMPTY 255

void SetDiagInpHoldFrameDelay(unsigned val);
void InputMake(void);
void InputInit();
void InputDeInit();
void SetControllerInfo();

extern bool bLibretroSupportsBitmasks;

#endif
