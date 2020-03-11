// FB Alpha IGS PolyGameMaster driver module
// Original FB Alpha port by OopsWare
// Based on information from ElSemi, Haze (David Haywood), and XingXing

#include "pgm.h"
#include "ics2115.h"

static struct BurnRomInfo emptyRomDesc[] = {
	{ "", 0, 0, 0 },
};

static struct BurnInputInfo pgmInputList[] = {
	{"P1 Coin",		BIT_DIGITAL,	PgmBtn1 + 0,	"p1 coin"	},
	{"P1 Start",		BIT_DIGITAL,	PgmJoy1 + 0,	"p1 start"	},
	{"P1 Up",		BIT_DIGITAL,	PgmJoy1 + 1,	"p1 up"		},
	{"P1 Down",		BIT_DIGITAL,	PgmJoy1 + 2,	"p1 down"	},
	{"P1 Left",		BIT_DIGITAL,	PgmJoy1 + 3,	"p1 left"	},
	{"P1 Right",		BIT_DIGITAL,	PgmJoy1 + 4,	"p1 right"	},
	{"P1 Button 1",		BIT_DIGITAL,	PgmJoy1 + 5,	"p1 fire 1"	},
	{"P1 Button 2",		BIT_DIGITAL,	PgmJoy1 + 6,	"p1 fire 2"	},
	{"P1 Button 3",		BIT_DIGITAL,	PgmJoy1 + 7,	"p1 fire 3"	},
	{"P1 Button 4",		BIT_DIGITAL,	PgmBtn2 + 0,	"p1 fire 4"	},

	{"P2 Coin",		BIT_DIGITAL,	PgmBtn1 + 1,	"p2 coin"	},
	{"P2 Start",		BIT_DIGITAL,	PgmJoy2 + 0,	"p2 start"	},
	{"P2 Up",		BIT_DIGITAL,	PgmJoy2 + 1,	"p2 up"		},
	{"P2 Down",		BIT_DIGITAL,	PgmJoy2 + 2,	"p2 down"	},
	{"P2 Left",		BIT_DIGITAL,	PgmJoy2 + 3,	"p2 left"	},
	{"P2 Right",		BIT_DIGITAL,	PgmJoy2 + 4,	"p2 right"	},
	{"P2 Button 1",		BIT_DIGITAL,	PgmJoy2 + 5,	"p2 fire 1"	},
	{"P2 Button 2",		BIT_DIGITAL,	PgmJoy2 + 6,	"p2 fire 2"	},
	{"P2 Button 3",		BIT_DIGITAL,	PgmJoy2 + 7,	"p2 fire 3"	},
	{"P2 Button 4",		BIT_DIGITAL,	PgmBtn2 + 1,	"p2 fire 4"	},

	{"P3 Coin",		BIT_DIGITAL,	PgmBtn1 + 2,	"p3 coin"	},
	{"P3 Start",		BIT_DIGITAL,	PgmJoy3 + 0,	"p3 start"	},
	{"P3 Up",		BIT_DIGITAL,	PgmJoy3 + 1,	"p3 up"		},
	{"P3 Down",		BIT_DIGITAL,	PgmJoy3 + 2,	"p3 down"	},
	{"P3 Left",		BIT_DIGITAL,	PgmJoy3 + 3,	"p3 left"	},
	{"P3 Right",		BIT_DIGITAL,	PgmJoy3 + 4,	"p3 right"	},
	{"P3 Button 1",		BIT_DIGITAL,	PgmJoy3 + 5,	"p3 fire 1"	},
	{"P3 Button 2",		BIT_DIGITAL,	PgmJoy3 + 6,	"p3 fire 2"	},
	{"P3 Button 3",		BIT_DIGITAL,	PgmJoy3 + 7,	"p3 fire 3"	},
	{"P3 Button 4",		BIT_DIGITAL,	PgmBtn2 + 2,	"p3 fire 4"	},

	{"P4 Coin",		BIT_DIGITAL,	PgmBtn1 + 3,	"p4 coin"	},
	{"P4 Start",		BIT_DIGITAL,	PgmJoy4 + 0,	"p4 start"	},
	{"P4 Up",		BIT_DIGITAL,	PgmJoy4 + 1,	"p4 up"		},
	{"P4 Down",		BIT_DIGITAL,	PgmJoy4 + 2,	"p4 down"	},
	{"P4 Left",		BIT_DIGITAL,	PgmJoy4 + 3,	"p4 left"	},
	{"P4 Right",		BIT_DIGITAL,	PgmJoy4 + 4,	"p4 right"	},
	{"P4 Button 1",		BIT_DIGITAL,	PgmJoy4 + 5,	"p4 fire 1"	},
	{"P4 Button 2",		BIT_DIGITAL,	PgmJoy4 + 6,	"p4 fire 2"	},
	{"P4 Button 3",		BIT_DIGITAL,	PgmJoy4 + 7,	"p4 fire 3"	},
	{"P4 Button 4",		BIT_DIGITAL,	PgmBtn2 + 3,	"p4 fire 4"	},

	{"Reset",		BIT_DIGITAL,	&PgmReset,	"reset"		},
	{"Diagnostics 1",	BIT_DIGITAL,	PgmBtn1 + 4,	"diag"		},
	{"Diagnostics 2",	BIT_DIGITAL,	PgmBtn1 + 6,	""		},
	{"Service 1",		BIT_DIGITAL,	PgmBtn1 + 5,	"service"	},
	{"Service 2",		BIT_DIGITAL,	PgmBtn1 + 7,	"service2"	},

	{"Dip A",		BIT_DIPSWITCH,	PgmInput + 6,	"dip"		},
	{"Dip B",		BIT_DIPSWITCH,	PgmInput + 7,	"dip"		},
	{"Dip C",		BIT_DIPSWITCH,  PgmInput + 8,   "dip"		},
};

STDINPUTINFO(pgm)

static struct BurnDIPInfo pgmDIPList[] = {
	{0x2D,	0xFF, 0xFF,	0x00, NULL				},
	{0x2F,  0xFF, 0x01, 0x01, NULL					},

	{0,	0xFE, 0,	2,    "Test mode"			},
	{0x2D,	0x01, 0x01,	0x00, "Off"				},
	{0x2D,	0x01, 0x01,	0x01, "On"				},

	{0,	0xFE, 0,	2,    "Music"				},
	{0x2D,	0x01, 0x02,	0x02, "Off"				},
	{0x2D,	0x01, 0x02,	0x00, "On"				},

	{0,	0xFE, 0,	2,    "Voice"				},
	{0x2D,	0x01, 0x04,	0x04, "Off"				},
	{0x2D,	0x01, 0x04,	0x00, "On"				},

	{0,	0xFE, 0,	2,    "Free play"			},
	{0x2D,	0x01, 0x08,	0x00, "Off"				},
	{0x2D,	0x01, 0x08,	0x08, "On"				},

	{0,	0xFE, 0,	2,    "Stop mode"			},
	{0x2D,	0x01, 0x10,	0x00, "Off"				},
	{0x2D,	0x01, 0x10,	0x10, "On"				},

	{0,     0xFE, 0,        4,    "Bios select (Fake)"		},
	{0x2F,  0x01, 0x0f,     0x00, "Older"				},
	{0x2F,  0x01, 0x0f,     0x01, "Newer"				},
	{0x2F,  0x01, 0x0f,     0x02, "Newer (no intro, calendar)" 	},
	{0x2F,  0x01, 0x0f,     0x03, "Newer (no intro)"		},
};

STDDIPINFO(pgm)

static struct BurnDIPInfo jammaDIPList[] = {
	{0x2D,	0xFF, 0xFF,	0x00, NULL				},
	{0x2F,  0xFF, 0x01, 0x00, NULL					},

	{0,	0xFE, 0,	2,    "Test mode"			},
	{0x2D,	0x01, 0x01,	0x00, "Off"				},
	{0x2D,	0x01, 0x01,	0x01, "On"				},

	{0,	0xFE, 0,	2,    "Music"				},
	{0x2D,	0x01, 0x02,	0x02, "Off"				},
	{0x2D,	0x01, 0x02,	0x00, "On"				},

	{0,	0xFE, 0,	2,    "Voice"				},
	{0x2D,	0x01, 0x04,	0x04, "Off"				},
	{0x2D,	0x01, 0x04,	0x00, "On"				},

	{0,	0xFE, 0,	2,    "Free play"			},
	{0x2D,	0x01, 0x08,	0x00, "Off"				},
	{0x2D,	0x01, 0x08,	0x08, "On"				},

	{0,	0xFE, 0,	2,    "Stop mode"			},
	{0x2D,	0x01, 0x10,	0x00, "Off"				},
	{0x2D,	0x01, 0x10,	0x10, "On"				},
};

STDDIPINFO(jamma)

static struct BurnDIPInfo orlegendDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x00, NULL				},

	{0,	0xFE, 0,	2,    "Disable Ba-Chieh"		},
	{0x2E,	0x02, 0x01,	0x00, "Off"				},
	{0x2E,	0x02, 0x01,	0x01, "On"				},

	{0,	0xFE, 0,	3,    "Region (Fake)"			},
	{0x2E,	0x01, 0x03,	0x00, "World"				},
	{0x2E,	0x01, 0x03,	0x02, "Korea"				},
	{0x2E,	0x01, 0x03,	0x03, "China"				},
};

static struct BurnDIPInfo orld111cDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x02, NULL				},

	{0,	0xFE, 0,	2,    "Region (Fake)"			},
	{0x2E,	0x01, 0x03,	0x00, "Hong Kong"			},
	{0x2E,	0x01, 0x03,	0x02, "China"				},
};

static struct BurnDIPInfo orld111tDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x04, NULL				},
};

static struct BurnDIPInfo orld105kDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x02, NULL				},
};

static struct BurnDIPInfo orld112cDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x03, NULL				},

	{0,	0xFE, 0,	3,    "Region (Fake)"			},
	{0x2E,	0x01, 0x03,	0x00, "World"				},
	{0x2E,	0x01, 0x03,	0x02, "Korea"				},
	{0x2E,	0x01, 0x03,	0x03, "China"				},
};

static struct BurnDIPInfo killbldDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x17, NULL				},

	{0,	0xFE, 0,	6,    "Region (Fake)"			},
	{0x2E,	0x01, 0xFF,	0x16, "Taiwan"				},
	{0x2E,	0x01, 0xFF,	0x17, "China"				},
	{0x2E,	0x01, 0xFF,	0x18, "Hong Kong"			},
	{0x2E,	0x01, 0xFF,	0x19, "Japan"				},
	{0x2E,	0x01, 0xFF,	0x20, "Korea"				},
	{0x2E,	0x01, 0xFF,	0x21, "World"				},
};

static struct BurnDIPInfo killbld104DIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x21, NULL				},

	{0,	0xFE, 0,	6,    "Region (Fake)"			},
	{0x2E,	0x01, 0xFF,	0x16, "Taiwan"				},
	{0x2E,	0x01, 0xFF,	0x17, "China"				},
	{0x2E,	0x01, 0xFF,	0x18, "Hong Kong"			},
	{0x2E,	0x01, 0xFF,	0x19, "Japan"				},
	{0x2E,	0x01, 0xFF,	0x20, "Korea"				},
	{0x2E,	0x01, 0xFF,	0x21, "World"				},
};

static struct BurnDIPInfo photoy2kDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x03, NULL				},

	{0,	0xFE, 0,	7,    "Region (Fake)"			},
	{0x2E,	0x01, 0x0F,	0x00, "Taiwan"				},
	{0x2E,	0x01, 0x0F,	0x01, "China"				},
	{0x2E,	0x01, 0x0F,	0x02, "Japan"				},
	{0x2E,	0x01, 0x0F,	0x03, "World"				},
	{0x2E,	0x01, 0x0F,	0x04, "Korea"				},
	{0x2E,	0x01, 0x0F,	0x05, "Hong Kong"			},
	{0x2E,	0x01, 0x0F,	0x06, "Singapore / Malaysia"		},
};

static struct BurnDIPInfo py2k2DIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x00, NULL				},

	{0,	0xFE, 0,	7,    "Region (Fake)"			},
	{0x2E,	0x01, 0x0F,	0x00, "Taiwan"				},
	{0x2E,	0x01, 0x0F,	0x01, "China"				},
	{0x2E,	0x01, 0x0F,	0x02, "Japan"				},
	{0x2E,	0x01, 0x0F,	0x03, "World"				},
	{0x2E,	0x01, 0x0F,	0x04, "Korea"				},
	{0x2E,	0x01, 0x0F,	0x05, "Hong Kong"			},
	{0x2E,	0x01, 0x0F,	0x06, "Singapore / Malaysia"		},
};

static struct BurnDIPInfo oldsDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x04, NULL				},

	{0,	0xFE, 0,	6,    "Region (Fake)"			},
	{0x2E,	0x01, 0x0F,	0x01, "Taiwan"				},
	{0x2E,	0x01, 0x0F,	0x02, "China"				},
	{0x2E,	0x01, 0x0F,	0x03, "Japan"				},
	{0x2E,	0x01, 0x0F,	0x04, "Korea"				},
	{0x2E,	0x01, 0x0F,	0x05, "Hong Kong"			},
	{0x2E,	0x01, 0x0F,	0x06, "World"				},
};

static struct BurnDIPInfo olds100DIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x06, NULL				},

	{0,	0xFE, 0,	6,    "Region (Fake)"			},
	{0x2E,	0x01, 0x0F,	0x01, "Taiwan"				},
	{0x2E,	0x01, 0x0F,	0x02, "China"				},
	{0x2E,	0x01, 0x0F,	0x03, "Japan"				},
	{0x2E,	0x01, 0x0F,	0x04, "Korea"				},
	{0x2E,	0x01, 0x0F,	0x05, "Hong Kong"			},
	{0x2E,	0x01, 0x0F,	0x06, "World"				},
};

static struct BurnDIPInfo olds103tDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x02, NULL				},
};

static struct BurnDIPInfo puzzli2DIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x05, NULL				},

	{0,	0xFE, 0,	6,    "Region (Fake)"			},
	{0x2E,	0x01, 0x0F,	0x00, "Taiwan"				},
	{0x2E,	0x01, 0x0F,	0x01, "China"				},
	{0x2E,	0x01, 0x0F,	0x02, "Japan"				},
	{0x2E,	0x01, 0x0F,	0x03, "Korea"				},
	{0x2E,	0x01, 0x0F,	0x04, "Hong Kong"			},
	{0x2E,	0x01, 0x0F,	0x05, "World"				},
};

static struct BurnDIPInfo kovDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x05, NULL				},

	{0,	0xFE, 0,	6,    "Region (Fake)"			},
	{0x2E,	0x01, 0x0F,	0x00, "China"				},
	{0x2E,	0x01, 0x0F,	0x01, "Taiwan"				},
	{0x2E,	0x01, 0x0F,	0x02, "Japan"				},
	{0x2E,	0x01, 0x0F,	0x03, "Korea"				},
	{0x2E,	0x01, 0x0F,	0x04, "Hong Kong"			},
	{0x2E,	0x01, 0x0F,	0x05, "World"				},
};

static struct BurnDIPInfo kov111DIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x02, NULL				},

	{0,	0xFE, 0,	6,    "Region (Fake)"			},
	{0x2E,	0x01, 0x0F,	0x00, "China"				},
	{0x2E,	0x01, 0x0F,	0x01, "Taiwan"				},
	{0x2E,	0x01, 0x0F,	0x02, "Japan"				},
	{0x2E,	0x01, 0x0F,	0x03, "Korea"				},
	{0x2E,	0x01, 0x0F,	0x04, "Hong Kong"			},
	{0x2E,	0x01, 0x0F,	0x05, "World"				},
};

static struct BurnDIPInfo kov114DIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x04, NULL				},

	{0,	0xFE, 0,	6,    "Region (Fake)"			},
	{0x2E,	0x01, 0x0F,	0x00, "China"				},
	{0x2E,	0x01, 0x0F,	0x01, "Taiwan"				},
	{0x2E,	0x01, 0x0F,	0x02, "Japan"				},
	{0x2E,	0x01, 0x0F,	0x03, "Korea"				},
	{0x2E,	0x01, 0x0F,	0x04, "Hong Kong"			},
	{0x2E,	0x01, 0x0F,	0x05, "World"				},
};

static struct BurnDIPInfo kov2DIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x04, NULL				},

	{0,	0xFE, 0,	6,    "Region (Fake)"			},
	{0x2E,	0x01, 0x0F,	0x00, "China"				},
	{0x2E,	0x01, 0x0F,	0x01, "Taiwan"				},
	{0x2E,	0x01, 0x0F,	0x02, "Japan (Alta license)"				},
	{0x2E,	0x01, 0x0F,	0x03, "Korea"				},
	{0x2E,	0x01, 0x0F,	0x04, "Hong Kong"			},
	{0x2E,	0x01, 0x0F,	0x05, "World"				},
};

static struct BurnDIPInfo martmastDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x06, NULL				},

	{0,	0xFE, 0,	7,    "Region (Fake)"			},
	{0x2E,	0x01, 0x07,	0x00, "China"				},
	{0x2E,	0x01, 0x07,	0x01, "Taiwan"				},
	{0x2E,	0x01, 0x07,	0x02, "Japan"				},
	{0x2E,	0x01, 0x07,	0x03, "Korea"				},
	{0x2E,	0x01, 0x07,	0x04, "Hong Kong"			},
	{0x2E,	0x01, 0x07,	0x05, "World"				},
	{0x2E,	0x01, 0x07,	0x06, "USA (Andamiro USA  license)"				},
};

static struct BurnDIPInfo martmastc102DIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x00, NULL				},

	{0,	0xFE, 0,	6,    "Region (Fake)"			},
	{0x2E,	0x01, 0x07,	0x00, "China"				},
	{0x2E,	0x01, 0x07,	0x01, "Taiwan"				},
	{0x2E,	0x01, 0x07,	0x02, "Japan"				},
	{0x2E,	0x01, 0x07,	0x03, "Korea"				},
	{0x2E,	0x01, 0x07,	0x04, "Hong Kong"			},
	{0x2E,	0x01, 0x07,	0x05, "World"				},
//	{0x2E,  0x01, 0x07,     0x06, "USA"				},
};

static struct BurnDIPInfo martmasttwDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x01, NULL				},

	{0,	0xFE, 0,	6,    "Region (Fake)"			},
	{0x2E,	0x01, 0x07,	0x00, "China"				},
	{0x2E,	0x01, 0x07,	0x01, "Taiwan"				},
	{0x2E,	0x01, 0x07,	0x02, "Japan"				},
	{0x2E,	0x01, 0x07,	0x03, "Korea"				},
	{0x2E,	0x01, 0x07,	0x04, "Hong Kong"			},
	{0x2E,	0x01, 0x07,	0x05, "World"				},
//	{0x2E,  0x01, 0x07,     0x06, "USA"				},
};

static struct BurnDIPInfo thegladDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x06, NULL				},

	{0,	0xFE, 0,	7,    "Region (Fake)"			},
	{0x2E,	0x01, 0x07,	0x00, "China"				},
	{0x2E,	0x01, 0x07,	0x01, "Taiwan"				},
	{0x2E,	0x01, 0x07,	0x02, "Japan"				},
	{0x2E,	0x01, 0x07,	0x03, "Korea"				},
	{0x2E,	0x01, 0x07,	0x04, "Hong Kong"			},
	{0x2E,	0x01, 0x07,	0x05, "Spanish Territories"		},
	{0x2E,  0x01, 0x07, 0x06, "World"				},
};

static struct BurnDIPInfo theglad100DIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x01, NULL				},

	{0,	0xFE, 0,	7,    "Region (Fake)"			},
	{0x2E,	0x01, 0x07,	0x00, "China"				},
	{0x2E,	0x01, 0x07,	0x01, "Taiwan"				},
	{0x2E,	0x01, 0x07,	0x02, "Japan"				},
	{0x2E,	0x01, 0x07,	0x03, "Korea"				},
	{0x2E,	0x01, 0x07,	0x04, "Hong Kong"			},
	{0x2E,	0x01, 0x07,	0x05, "Spanish Territories"		},
	{0x2E,  0x01, 0x07, 0x06, "World"				},
};

static struct BurnDIPInfo thegladpcbDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x02, NULL				},

	{0,	0xFE, 0,	7,    "Region (Fake)"			},
	{0x2E,	0x01, 0x07,	0x00, "China"				},
	{0x2E,	0x01, 0x07,	0x01, "Taiwan"				},
	{0x2E,	0x01, 0x07,	0x02, "Japan"				},
	{0x2E,	0x01, 0x07,	0x03, "Korea"				},
	{0x2E,	0x01, 0x07,	0x04, "Hong Kong"			},
	{0x2E,	0x01, 0x07,	0x05, "Spanish Territories"		},
	{0x2E,  0x01, 0x07, 0x06, "World"				},
};

static struct BurnDIPInfo dmnfrntpcbDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x03, NULL				},

	{0,	0xFE, 0,	6,    "Region (Fake)"			},
	{0x2E,	0x01, 0x0F,	0x00, "China"				},
	{0x2E,	0x01, 0x0F,	0x01, "Taiwan"				},
	{0x2E,	0x01, 0x0F,	0x02, "Japan"				},
	{0x2E,	0x01, 0x0F,	0x03, "Korea"				},
	{0x2E,	0x01, 0x0F,	0x04, "Hong Kong"			},
	{0x2E,	0x01, 0x0F,	0x05, "World"				},
};

static struct BurnDIPInfo ddp2DIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x05, NULL				},
};

static struct BurnDIPInfo ddp2hkDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x04, NULL				},
};

static struct BurnDIPInfo ddp2kDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x03, NULL				},
};

static struct BurnDIPInfo ddp2jDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x02, NULL				},
};

static struct BurnDIPInfo ddp2tDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x01, NULL				},
};

static struct BurnDIPInfo ddp2cDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x00, NULL				},
};

static struct BurnDIPInfo kovshxasDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x00, NULL				},

	{0,	0xFE, 0,	6,    "Region (Fake)"			},
	{0x2E,	0x01, 0x0F,	0x00, "China"				},
	{0x2E,	0x01, 0x0F,	0x01, "Taiwan"				},
	{0x2E,	0x01, 0x0F,	0x02, "Japan"				},
	{0x2E,	0x01, 0x0F,	0x03, "Korea"				},
	{0x2E,	0x01, 0x0F,	0x04, "Hong Kong"			},
	{0x2E,	0x01, 0x0F,	0x05, "World"				},
};

static struct BurnDIPInfo happy6DIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x00, NULL				},

	{0,	0xFE, 0,	5,    "Region (Fake)"			},
	{0x2E,	0x01, 0x0F,	0x00, "China"				},
	{0x2E,	0x01, 0x0F,	0x01, "Taiwan"				},
	{0x2E,	0x01, 0x0F,	0x02, "Hong Kong"			},
	{0x2E,	0x01, 0x0F,	0x03, "Singapore"			},
	{0x2E,	0x01, 0x0F,	0x04, "World"				},
};

static struct BurnDIPInfo svgDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x00, NULL				},

	{0,	0xFE, 0,	7,    "Region (Fake)"			},
	{0x2E,	0x01, 0x07,	0x00, "China"				},
	{0x2E,	0x01, 0x07,	0x01, "Taiwan"				},
	{0x2E,	0x01, 0x07,	0x02, "Japan"				},
	{0x2E,	0x01, 0x07,	0x03, "Korea"				},
	{0x2E,	0x01, 0x07,	0x04, "Hong Kong"			},
	{0x2E,	0x01, 0x07,	0x05, "Spanish Territories"	},
	{0x2E,  0x01, 0x07, 0x06, "World"				},
};

static struct BurnDIPInfo svgtwDIPList[] = {
	{0x2E,	0xFF, 0xFF,	0x01, NULL				},

	{0,	0xFE, 0,	8,    "Region (Fake)"			},
	{0x2E,	0x01, 0x07,	0x00, "China"				},
	{0x2E,	0x01, 0x07,	0x01, "Taiwan"				},
	{0x2E,	0x01, 0x07,	0x02, "Japan"				},
	{0x2E,	0x01, 0x07,	0x03, "Korea"				},
	{0x2E,	0x01, 0x07,	0x04, "Hong Kong"			},
	{0x2E,	0x01, 0x07,	0x05, "Spanish Territories"	},
	{0x2E,  0x01, 0x07, 0x06, "World"				},
	{0x2E,  0x01, 0x07, 0xff, "Don't Change"		},
};

STDDIPINFOEXT(orlegend,		pgm,	orlegend		)
STDDIPINFOEXT(orld111c, 	pgm,	orld111c		)
STDDIPINFOEXT(orld111t, 	pgm,	orld111t		)
STDDIPINFOEXT(orld105k, 	pgm,	orld105k		)
STDDIPINFOEXT(orld112c, 	pgm,	orld112c		)
STDDIPINFOEXT(kov,       	pgm,	kov		    	)
STDDIPINFOEXT(kov111,       pgm,	kov111			)
STDDIPINFOEXT(kov114,     	pgm,	kov114			)
STDDIPINFOEXT(kov2,       	pgm,	kov2			)
STDDIPINFOEXT(kovshxas,    	pgm,	kovshxas		)
STDDIPINFOEXT(killbld,	 	pgm,	killbld			)
STDDIPINFOEXT(killbld104,	pgm,	killbld104		)
STDDIPINFOEXT(photoy2k, 	pgm,	photoy2k		)
STDDIPINFOEXT(py2k2, 		pgm,	py2k2			)
STDDIPINFOEXT(puzzli2,  	pgm,	puzzli2			)
STDDIPINFOEXT(martmast, 	pgm,	martmast		)
STDDIPINFOEXT(martmastc102, pgm,	martmastc102	)
STDDIPINFOEXT(martmasttw, 	pgm,	martmasttw		)
STDDIPINFOEXT(olds,     	pgm,	olds			)
STDDIPINFOEXT(olds100,     	pgm,	olds100			)
STDDIPINFOEXT(olds103t,     pgm,	olds103t		)
STDDIPINFOEXT(ddp2,     	pgm,	ddp2			)
STDDIPINFOEXT(ddp2hk,     	pgm,	ddp2hk			)
STDDIPINFOEXT(ddp2k,     	pgm,	ddp2k			)
STDDIPINFOEXT(ddp2j,     	pgm,	ddp2j			)
STDDIPINFOEXT(ddp2t,     	pgm,	ddp2t			)
STDDIPINFOEXT(ddp2c,     	pgm,	ddp2c			)
STDDIPINFOEXT(theglad,	 	pgm,	theglad 		)
STDDIPINFOEXT(theglad100,	pgm,	theglad100 		)
STDDIPINFOEXT(happy6,		pgm,	happy6	 		)
STDDIPINFOEXT(svg,			pgm,	svg	 			)
STDDIPINFOEXT(svgtw,		pgm,	svgtw	 		)
STDDIPINFOEXT(dmnfrntpcb,   jamma,	dmnfrntpcb		)
STDDIPINFOEXT(thegladpcb,   jamma,	thegladpcb		)

// -----------------------------------------------------------------------------
// BIOS


// PGM (Polygame Master) System BIOS

static struct BurnRomInfo pgmRomDesc[] = {
	{ "pgm_t01s.rom", 		0x200000, 0x1a7123a0, BRF_GRA | BRF_BIOS },	// 0x80 - 8x8 Text Layer Tiles
	{ "pgm_m01s.rom", 		0x200000, 0x45ae7159, BRF_SND | BRF_BIOS },	// 0x81 - Samples

	{ "pgm_p01s.u20", 		0x020000, 0xe42b166e, BRF_PRG | BRF_BIOS },	// 0x82 - 68K BIOS (V0001,     older  - 02/26/97 - 11:14:09)
	{ "pgm_p02s.u20", 		0x020000, 0x78c15fa2, BRF_PRG | BRF_BIOS },	// 0x83 - 68K BIOS (V0001,     newer  - 07/10/97 - 16:36:08)
#if !defined (ROM_VERIFY)
	{ "ddp3_bios.u37",		0x080000, 0xb3cc5c8f, BRF_PRG | BRF_BIOS }, // 0x84 - 68K BIOS (V0001,     custom - 07/17/97 - 19:44:59)
	{ "bios.u42",     		0x020000, 0x517cf7a2, BRF_PRG | BRF_BIOS },	// 0x85 - 68K BIOS (V0001-01J, custom - 02/25/03 - 17:51:01)
#else
	{ "",             		0x000000, 0x00000000, 0                  },	// 0x84
	{ "",             		0x000000, 0x00000000, 0                  },	// 0x85
#endif
};

STD_ROM_PICK(pgm)
STD_ROM_FN(pgm)

struct BurnDriver BurnDrvPgm = {
	"pgm", NULL, NULL, NULL, "1997",
	"PGM (Polygame Master) System BIOS\0", "BIOS only", "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_BOARDROM, 0, HARDWARE_IGS_PGM, GBF_BIOS, 0,
	NULL, pgmRomInfo, pgmRomName, NULL, NULL, NULL, NULL, pgmInputInfo, pgmDIPInfo,
	pgmInit, pgmExit, pgmFrame, pgmDraw, pgmScan,  &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

static struct BurnRomInfo thegladBIOSRomDesc[] = {
	{ "pgm_t01s.u72", 		0x200000, 0x1a7123a0, BRF_GRA | BRF_BIOS }, // 0x80 - 8x8 Text Layer Tiles

	{ "pgm_m01s.u4", 		0x200000, 0x45ae7159, BRF_SND | BRF_BIOS },	// 0x81 - Samples

	{ "bios.42",      		0x020000, 0x517cf7a2, BRF_PRG | BRF_BIOS },	// 0x82 - 68K BIOS (V0001-01J, custom 02/25/03 17:51:01)
};

static struct BurnRomInfo dmnfrntBIOSRomDesc[] = {
	{ "pgm_t01s.rom", 		0x200000, 0x1a7123a0, BRF_GRA | BRF_BIOS }, // 0x80 - 8x8 Text Layer Tiles

	{ "pgm_m01s.rom", 		0x200000, 0x45ae7159, BRF_SND | BRF_BIOS },	// 0x81 - Samples

	{ "pgm_p02s.u42", 		0x020000, 0x78c15fa2, BRF_PRG | BRF_BIOS },	// 0x82 - 68K BIOS (V0001, newer - 07/10/97 - 16:36:08)
};

static struct BurnRomInfo svgpcbBIOSRomDesc[] = {
	{ "pgm_t01s.rom", 		0x200000, 0x1a7123a0, BRF_GRA | BRF_BIOS }, // 0x80 - 8x8 Text Layer Tiles

	{ "pgm_m01s.rom", 		0x200000, 0x45ae7159, BRF_SND | BRF_BIOS },	// 0x81 - Samples

	{ "svg_bios.u49", 		0x020000, 0x3346401f, BRF_PRG | BRF_BIOS },	// 0x82 - 68K BIOS (V0001, custom 05/13/05 16:00:00)
};

static struct BurnRomInfo ketsuiBiosRomDesc[] = {
	{ "pgm_t01s.rom", 		0x200000, 0x1a7123a0, BRF_GRA | BRF_BIOS }, // 0x80 - 8x8 Text Layer Tiles
	{ "",		  		0,        0,          0 },			// 0x81 - Samples

	{ "",		  		0,        0,          0 },			// 0x82 - 68K BIOS
	{ "",		  		0,        0,          0 },			// 0x83 - 68K BIOS
};

static struct BurnRomInfo espgalBiosRomDesc[] = {
	{ "t01s.u18", 			0x200000, 0x1a7123a0, BRF_GRA | BRF_BIOS }, // 0x80 - 8x8 Text Layer Tiles
	{ "",		  		0,        0,          0 },			// 0x81 - Samples

	{ "",		  		0,        0,          0 },			// 0x82 - 68K BIOS
	{ "",		  		0,        0,          0 },			// 0x83 - 68K BIOS
};

static struct BurnRomInfo ddp3BiosRomDesc[] = {
	{ "pgm_t01s.rom", 		0x200000, 0x1a7123a0, BRF_GRA | BRF_BIOS }, // 0x80 - 8x8 Text Layer Tiles

	{ "pgm_m01s.rom", 		0x200000, 0x45ae7159, BRF_SND | BRF_BIOS },	// 0x81 - Samples

	{ "ddp3_bios.u37",		0x080000, 0xb3cc5c8f, BRF_PRG | BRF_BIOS },	// 0x82 - 68K BIOS (V0001, custom? 07/17/97 19:44:59)
};


// -----------------------------------------------------------------------------
// Normal Games


// Oriental Legend / Xi Yo Gi Shi Re Zuang (V126)

static struct BurnRomInfo orlegendRomDesc[] = {
	{ "p0103.rom",			0x200000, 0xd5e93543, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t0100.u8",			0x400000, 0x61425e1e, 2 | BRF_GRA },			//  1 Tile data

	{ "a0100.u5",			0x400000, 0x8b3bd88a, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0101.u6",			0x400000, 0x3b9e9644, 3 | BRF_GRA },			//  3	  
	{ "a0102.u7",			0x400000, 0x069e2c38, 3 | BRF_GRA },			//  4
	{ "a0103.u8",			0x400000, 0x4460a3fd, 3 | BRF_GRA },			//  5
	{ "a0104.u11",			0x400000, 0x5f8abb56, 3 | BRF_GRA },			//  6
	{ "a0105.u12",			0x400000, 0xa17a7147, 3 | BRF_GRA },			//  7

	{ "b0100.u9",			0x400000, 0x69d2e48c, 4 | BRF_GRA },			//  8 Sprite Masks & Color Indexes
	{ "b0101.u10",			0x400000, 0x0d587bf3, 4 | BRF_GRA },			//  9
	{ "b0102.u15",			0x400000, 0x43823c1e, 4 | BRF_GRA },			// 10

	{ "m0100.u1",			0x200000, 0xe5c36c83, 5 | BRF_SND },			// 11 Samples
};

STDROMPICKEXT(orlegend, orlegend, pgm)
STD_ROM_FN(orlegend)

static INT32 orlegendInit()
{
	pPgmProtCallback = install_protection_asic3_orlegend;

	return pgmInit();
}

struct BurnDriver BurnDrvOrlegend = {
	"orlegend", NULL, "pgm", NULL, "1997",
	"Oriental Legend - Xi Yo Gi Shi Re Zuang (V126)\0", NULL, "IGS", "PolyGameMaster",
	L"Oriental Legend\0\u897F\u6E38\u91CA\u5384\u4F20 (V126)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM, GBF_SCRFIGHT, 0,
	NULL, orlegendRomInfo, orlegendRomName, NULL, NULL, NULL, NULL, pgmInputInfo, orlegendDIPInfo,
	orlegendInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Oriental Legend / Xi Yo Gi Shi Re Zuang (V112)

static struct BurnRomInfo orlegend112eRomDesc[] = {
	{ "pgm_p0102.u2",		0x200000, 0x4d0f6cc5, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "pgm_t0100.u8",		0x400000, 0x61425e1e, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a0100-1.u5",		0x400000, 0x8b3bd88a, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a0101-1.u6",		0x400000, 0x3b9e9644, 3 | BRF_GRA },			//  3	  
	{ "pgm_a0102-1.u7",		0x400000, 0x069e2c38, 3 | BRF_GRA },			//  4
	{ "pgm_a0103-1.u8",		0x400000, 0x4460a3fd, 3 | BRF_GRA },			//  5
	{ "pgm_a0104-1.u11",	0x400000, 0x5f8abb56, 3 | BRF_GRA },			//  6
	{ "pgm_a0105-1.u12",	0x400000, 0xa17a7147, 3 | BRF_GRA },			//  7

	{ "pgm_b0100-1.u9",		0x400000, 0x69d2e48c, 4 | BRF_GRA },			//  8 Sprite Masks & Color Indexes
	{ "pgm_b0101-1.u10",	0x400000, 0x0d587bf3, 4 | BRF_GRA },			//  9
	{ "pgm_b0102-1.u15",	0x400000, 0x43823c1e, 4 | BRF_GRA },			// 10

	{ "pgm_m0100.u1",		0x200000, 0xe5c36c83, 5 | BRF_SND },			// 11 Samples
};

STDROMPICKEXT(orlegend112e, orlegend112e, pgm)
STD_ROM_FN(orlegend112e)

struct BurnDriver BurnDrvOrlegend112e = {
	"orlegende", "orlegend", "pgm", NULL, "1997",
	"Oriental Legend - Xi Yo Gi Shi Re Zuang (V112)\0", NULL, "IGS", "PolyGameMaster",
	L"Oriental Legend\0\u897F\u6E38\u91CA\u5384\u4F20 (V112)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_SCRFIGHT, 0,
	NULL, orlegend112eRomInfo, orlegend112eRomName, NULL, NULL, NULL, NULL, pgmInputInfo, orlegendDIPInfo,
	orlegendInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Oriental Legend / Xi Yo Gi Shi Re Zuang (V112, China)

static struct BurnRomInfo orlegend112cRomDesc[] = {
	{ "p0101.160",			0x200000, 0xb24f0c1e, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t0100.u8",			0x400000, 0x61425e1e, 2 | BRF_GRA },			//  1 Tile data

	{ "a0100.u5",			0x400000, 0x8b3bd88a, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0101.u6",			0x400000, 0x3b9e9644, 3 | BRF_GRA },			//  3	  
	{ "a0102.u7",			0x400000, 0x069e2c38, 3 | BRF_GRA },			//  4
	{ "a0103.u8",			0x400000, 0x4460a3fd, 3 | BRF_GRA },			//  5
	{ "a0104.u11",			0x400000, 0x5f8abb56, 3 | BRF_GRA },			//  6
	{ "a0105.u12",			0x400000, 0xa17a7147, 3 | BRF_GRA },			//  7

	{ "b0100.u9",			0x400000, 0x69d2e48c, 4 | BRF_GRA },			//  8 Sprite Masks & Color Indexes
	{ "b0101.u10",			0x400000, 0x0d587bf3, 4 | BRF_GRA },			//  9
	{ "b0102.u15",			0x400000, 0x43823c1e, 4 | BRF_GRA },			// 10

	{ "m0100.u1",			0x200000, 0xe5c36c83, 5 | BRF_SND },			// 11 Samples
};

STDROMPICKEXT(orlegend112c, orlegend112c, pgm)
STD_ROM_FN(orlegend112c)

struct BurnDriver BurnDrvOrlegend112c = {
	"orlegendc", "orlegend", "pgm", NULL, "1997",
	"Oriental Legend - Xi Yo Gi Shi Re Zuang (V112, China)\0", NULL, "IGS", "PolyGameMaster",
	L"Oriental Legend\0\u897F\u6E38\u91CA\u5384\u4F20 (V112, China)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_SCRFIGHT, 0,
	NULL, orlegend112cRomInfo, orlegend112cRomName, NULL, NULL, NULL, NULL, pgmInputInfo, orld112cDIPInfo,
	orlegendInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Oriental Legend / Xi Yo Gi Shi Re Zuang (V112 alt,China)

static struct BurnRomInfo orlegend112caRomDesc[] = {
	{ "p0101.102",			0x200000, 0x7a22e1cb, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t0100.u8",			0x400000, 0x61425e1e, 2 | BRF_GRA },			//  1 Tile data

	{ "a0100.u5",			0x400000, 0x8b3bd88a, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0101.u6",			0x400000, 0x3b9e9644, 3 | BRF_GRA },			//  3	  
	{ "a0102.u7",			0x400000, 0x069e2c38, 3 | BRF_GRA },			//  4
	{ "a0103.u8",			0x400000, 0x4460a3fd, 3 | BRF_GRA },			//  5
	{ "a0104.u11",			0x400000, 0x5f8abb56, 3 | BRF_GRA },			//  6
	{ "a0105.u12",			0x400000, 0xa17a7147, 3 | BRF_GRA },			//  7

	{ "b0100.u9",			0x400000, 0x69d2e48c, 4 | BRF_GRA },			//  8 Sprite Masks & Color Indexes
	{ "b0101.u10",			0x400000, 0x0d587bf3, 4 | BRF_GRA },			//  9
	{ "b0102.u15",			0x400000, 0x43823c1e, 4 | BRF_GRA },			// 10

	{ "m0100.u1",			0x200000, 0xe5c36c83, 5 | BRF_SND },			// 11 Samples
};

STDROMPICKEXT(orlegend112ca, orlegend112ca, pgm)
STD_ROM_FN(orlegend112ca)

struct BurnDriver BurnDrvOrlegend112ca = {
	"orlegendca", "orlegend", "pgm", NULL, "1997",
	"Oriental Legend - Xi Yo Gi Shi Re Zuang (V112 alt, China)\0", NULL, "IGS", "PolyGameMaster",
	L"Oriental Legend\0\u897F\u6E38\u91CA\u5384\u4F20 (V112 alt, China)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_SCRFIGHT, 0,
	NULL, orlegend112caRomInfo, orlegend112caRomName, NULL, NULL, NULL, NULL, pgmInputInfo, orld111cDIPInfo,
	orlegendInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Oriental Legend / Xi Yo Gi Shi Re Zuang (V111, China)

static struct BurnRomInfo orlegend111cRomDesc[] = {
	{ "olv111ch.u6",		0x080000, 0x5fb86373, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code
	{ "olv111ch.u9",		0x080000, 0x83cf09c8, 1 | BRF_PRG | BRF_ESS },	//  1
	{ "olv111ch.u7",		0x080000, 0x6ee79faf, 1 | BRF_PRG | BRF_ESS },	//  2
	{ "olv111ch.u11",		0x080000, 0xb80ddd3c, 1 | BRF_PRG | BRF_ESS },	//  3

	{ "t0100.u8",			0x400000, 0x61425e1e, 2 | BRF_GRA },			//  1 Tile data

	{ "a0100.u5",			0x400000, 0x8b3bd88a, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0101.u6",			0x400000, 0x3b9e9644, 3 | BRF_GRA },			//  3	  
	{ "a0102.u7",			0x400000, 0x069e2c38, 3 | BRF_GRA },			//  4
	{ "a0103.u8",			0x400000, 0x4460a3fd, 3 | BRF_GRA },			//  5
	{ "a0104.u11",			0x400000, 0x5f8abb56, 3 | BRF_GRA },			//  6
	{ "a0105.u12",			0x400000, 0xa17a7147, 3 | BRF_GRA },			//  7

	{ "b0100.u9",			0x400000, 0x69d2e48c, 4 | BRF_GRA },			//  8 Sprite Masks & Color Indexes
	{ "b0101.u10",			0x400000, 0x0d587bf3, 4 | BRF_GRA },			//  9
	{ "b0102.u15",			0x400000, 0x43823c1e, 4 | BRF_GRA },			// 10

	{ "m0100.u1",			0x200000, 0xe5c36c83, 5 | BRF_SND },			// 11 Samples
};

STDROMPICKEXT(orlegend111c, orlegend111c, pgm)
STD_ROM_FN(orlegend111c)

struct BurnDriver BurnDrvOrlegend111c = {
	"orlegend111c", "orlegend", "pgm", NULL, "1997",
	"Oriental Legend - Xi Yo Gi Shi Re Zuang (V111, China)\0", NULL, "IGS", "PolyGameMaster",
	L"Oriental Legend\0\u897F\u6E38\u91CA\u5384\u4F20 (V111, China)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_SCRFIGHT, 0,
	NULL, orlegend111cRomInfo, orlegend111cRomName, NULL, NULL, NULL, NULL, pgmInputInfo, orld111cDIPInfo,
	orlegendInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Oriental Legend / Xi Yo Gi Shi Re Zuang (V111, Taiwan)

static struct BurnRomInfo orlegend111tRomDesc[] = {
	{ "olv111tw.u6",		0x080000, 0xb205a733, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code
	{ "olv111tw.u9",		0x080000, 0x6d9d29b4, 1 | BRF_PRG | BRF_ESS },	//  1
	{ "olv111tw.u7",		0x080000, 0x27628e87, 1 | BRF_PRG | BRF_ESS },	//  2
	{ "olv111tw.u11",		0x080000, 0x23f33bc9, 1 | BRF_PRG | BRF_ESS },	//  3

	{ "pgm_t0100.u8",		0x400000, 0x61425e1e, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a0100b.u5",		0x400000, 0x8b3bd88a, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a0101b.u8",		0x400000, 0x3b9e9644, 3 | BRF_GRA },			//  3	  
	{ "pgm_a0102b.u6",		0x400000, 0x069e2c38, 3 | BRF_GRA },			//  4
	{ "pgm_a0103b.u9",		0x400000, 0x4460a3fd, 3 | BRF_GRA },			//  5
	{ "pgm_a0104b.u7",		0x400000, 0x5f8abb56, 3 | BRF_GRA },			//  6
	{ "pgm_a0105b.u10",		0x400000, 0xa17a7147, 3 | BRF_GRA },			//  7

	{ "pgm_b0100b.u11",		0x400000, 0x69d2e48c, 4 | BRF_GRA },			//  8 Sprite Masks & Color Indexes
	{ "pgm_b0101b.u12",		0x400000, 0x0d587bf3, 4 | BRF_GRA },			//  9
	{ "pgm_b0102.u2",		0x400000, 0x43823c1e, 4 | BRF_GRA },			// 10

	{ "pgm_m0100.u1",		0x200000, 0xe5c36c83, 5 | BRF_SND },			// 11 Samples
};

STDROMPICKEXT(orlegend111t, orlegend111t, pgm)
STD_ROM_FN(orlegend111t)

struct BurnDriver BurnDrvOrlegend111t = {
	"orlegend111t", "orlegend", "pgm", NULL, "1997",
	"Oriental Legend - Xi Yo Gi Shi Re Zuang (V111, Taiwan)\0", NULL, "IGS", "PolyGameMaster",
	L"Oriental Legend\0\u897F\u6E38\u91CA\u5384\u4F20 (V111, Taiwan)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_SCRFIGHT, 0,
	NULL, orlegend111tRomInfo, orlegend111tRomName, NULL, NULL, NULL, NULL, pgmInputInfo, orld111tDIPInfo,
	orlegendInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Oriental Legend / Xi Yo Gi Shi Re Zuang (V111, Korea)

static struct BurnRomInfo orlegend111kRomDesc[] = {
	{ "olv111ko.u6",		0x080000, 0x1ff35baa, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code
	{ "olv111ko.u9",		0x080000, 0x87b6d202, 1 | BRF_PRG | BRF_ESS },	//  1
	{ "olv111ko.u7",		0x080000, 0x27628e87, 1 | BRF_PRG | BRF_ESS },	//  2
	{ "olv111ko.u11",		0x080000, 0x23f33bc9, 1 | BRF_PRG | BRF_ESS },	//  3

	{ "t0100.u8",			0x400000, 0x61425e1e, 2 | BRF_GRA },			//  1 Tile data

	{ "a0100.u5",			0x400000, 0x8b3bd88a, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0101.u6",			0x400000, 0x3b9e9644, 3 | BRF_GRA },			//  3	  
	{ "a0102.u7",			0x400000, 0x069e2c38, 3 | BRF_GRA },			//  4
	{ "a0103.u8",			0x400000, 0x4460a3fd, 3 | BRF_GRA },			//  5
	{ "a0104.u11",			0x400000, 0x5f8abb56, 3 | BRF_GRA },			//  6
	{ "a0105.u12",			0x400000, 0xa17a7147, 3 | BRF_GRA },			//  7

	{ "b0100.u9",			0x400000, 0x69d2e48c, 4 | BRF_GRA },			//  8 Sprite Masks & Color Indexes
	{ "b0101.u10",			0x400000, 0x0d587bf3, 4 | BRF_GRA },			//  9
	{ "b0102.u15",			0x400000, 0x43823c1e, 4 | BRF_GRA },			// 10

	{ "m0100.u1",			0x200000, 0xe5c36c83, 5 | BRF_SND },			// 11 Samples
};

STDROMPICKEXT(orlegend111k, orlegend111k, pgm)
STD_ROM_FN(orlegend111k)

struct BurnDriver BurnDrvOrlegend111k = {
	"orlegend111k", "orlegend", "pgm", NULL, "1997",
	"Oriental Legend - Xi Yo Gi Shi Re Zuang (V111, Korea)\0", NULL, "IGS", "PolyGameMaster",
	L"Oriental Legend\0\u897F\u6E38\u91CA\u5384\u4F20 (V111, Korea)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_SCRFIGHT, 0,
	NULL, orlegend111kRomInfo, orlegend111kRomName, NULL, NULL, NULL, NULL, pgmInputInfo, orld105kDIPInfo,
	orlegendInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Oriental Legend (V105, Korea)

static struct BurnRomInfo orlegend105kRomDesc[] = {
	{ "olv105ko.u6",		0x080000, 0xb86703fe, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code
	{ "olv105ko.u9",		0x080000, 0x5a108e39, 1 | BRF_PRG | BRF_ESS },	//  1
	{ "olv105ko.u7",		0x080000, 0x5712facc, 1 | BRF_PRG | BRF_ESS },	//  2
	{ "olv105ko.u11",		0x080000, 0x40ae4d9e, 1 | BRF_PRG | BRF_ESS },	//  3

	{ "pgm_t0100.u8",		0x400000, 0x61425e1e, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a0100.u5",		0x400000, 0x8b3bd88a, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a0101.u6",		0x400000, 0x3b9e9644, 3 | BRF_GRA },			//  3	  
	{ "pgm_a0102.u7",		0x400000, 0x069e2c38, 3 | BRF_GRA },			//  4
	{ "pgm_a0103.u8",		0x400000, 0x4460a3fd, 3 | BRF_GRA },			//  5
	{ "pgm_a0104.u11",		0x400000, 0x5f8abb56, 3 | BRF_GRA },			//  6
	{ "pgm_a0105.u12",		0x400000, 0xa17a7147, 3 | BRF_GRA },			//  7

	{ "pgm_b0100.u9",		0x400000, 0x69d2e48c, 4 | BRF_GRA },			//  8 Sprite Masks & Color Indexes
	{ "pgm_b0101.u10",		0x400000, 0x0d587bf3, 4 | BRF_GRA },			//  9
	{ "pgm_b0102.u15",		0x400000, 0x43823c1e, 4 | BRF_GRA },			// 10

	{ "pgm_m0100.u1",		0x200000, 0xe5c36c83, 5 | BRF_SND },			// 11 Samples
};

STDROMPICKEXT(orlegend105k, orlegend105k, pgm)
STD_ROM_FN(orlegend105k)

struct BurnDriver BurnDrvOrlegend105k = {
	"orlegend105k", "orlegend", "pgm", NULL, "1997",
	"Oriental Legend (V105, Korea)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_SCRFIGHT, 0,
	NULL, orlegend105kRomInfo, orlegend105kRomName, NULL, NULL, NULL, NULL, pgmInputInfo, orld105kDIPInfo,
	orlegendInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Dragon World II (V110X, World)

static struct BurnRomInfo drgw2RomDesc[] = {
	{ "dragon_ii_v-110x.u2",	0x080000, 0x1978106b, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "pgm_t0200.u7",		0x400000, 0xb0f6534d, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a0200.u5",		0x400000, 0x13b95069, 3 | BRF_GRA },			//  2 Sprite Color Data

	{ "pgm_b0200.u9",		0x400000, 0x932d0f13, 4 | BRF_GRA },			//  3 Sprite Masks & Color Indexes
};

STDROMPICKEXT(drgw2, drgw2, pgm)
STD_ROM_FN(drgw2)

static void drgw2_patch()
{
	pgm_decrypt_dw2();

	UINT16 *mem16 = (UINT16*)PGM68KROM;

	// This is not a hack! The protection chip actually applies patches at three specific locations.
	for (INT32 i = 0; i < 0x80000/2; i++) {
		if (mem16[i] == 0x4e90 && mem16[i+1] == 0x207c && (mem16[i+2] & 0xfff8) == 0x0010) {
			mem16[i] = 0x4e93;
		}
	}
}

static INT32 drgw2Init()
{
	pPgmInitCallback = drgw2_patch;
	pPgmProtCallback = install_protection_asic25_asic12_dw2;

	return pgmInit();
}

struct BurnDriver BurnDrvDrgw2 = {
	"drgw2", NULL, "pgm", NULL, "1997",
	"Dragon World II (V110X, World)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM, GBF_PUZZLE, 0,
	NULL, drgw2RomInfo, drgw2RomName, NULL, NULL, NULL, NULL, pgmInputInfo, pgmDIPInfo,
	drgw2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Zhong Guo Long II (V100C, China)

static struct BurnRomInfo drgw2cRomDesc[] = {
	{ "v-100c.u2",			0x080000, 0x67467981, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "pgm_t0200.u7",		0x400000, 0xb0f6534d, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a0200.u5",		0x400000, 0x13b95069, 3 | BRF_GRA },			//  2 Sprite Color Data

	{ "pgm_b0200.u9",		0x400000, 0x932d0f13, 4 | BRF_GRA },			//  3 Sprite Masks & Color Indexes
};

STDROMPICKEXT(drgw2c, drgw2c, pgm)
STD_ROM_FN(drgw2c)

struct BurnDriver BurnDrvDrgw2c = {
	"drgw2c", "drgw2", "pgm", NULL, "1997",
	"Zhong Guo Long II (V100C, China)\0", NULL, "IGS", "PolyGameMaster",
	L"\u4E2D\u570B\u9F8D II (V100C, China)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_PUZZLE, 0,
	NULL, drgw2cRomInfo, drgw2cRomName, NULL, NULL, NULL, NULL, pgmInputInfo, pgmDIPInfo,
	drgw2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Chuugokuryuu II (V100J, Japan)

static struct BurnRomInfo drgw2jRomDesc[] = {
	{ "dragon_ii_v100j.u2",	0x080000, 0xf8f8393e, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "pgm_t0200.u7",		0x400000, 0xb0f6534d, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a0200.u5",		0x400000, 0x13b95069, 3 | BRF_GRA },			//  2 Sprite Color Data

	{ "pgm_b0200.u9",		0x400000, 0x932d0f13, 4 | BRF_GRA },			//  3 Sprite Masks & Color Indexes
};

STDROMPICKEXT(drgw2j, drgw2j, pgm)
STD_ROM_FN(drgw2j)

struct BurnDriver BurnDrvDrgw2j = {
	"drgw2j", "drgw2", "pgm", NULL, "1997",
	"Chuugokuryuu II (V100J, Japan)\0", NULL, "IGS", "PolyGameMaster",
	L"\u4E2D\u570B\u9F8D II (V100J, Japan)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_PUZZLE, 0,
	NULL, drgw2jRomInfo, drgw2jRomName, NULL, NULL, NULL, NULL, pgmInputInfo, pgmDIPInfo,
	drgw2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Dragon World II (V100X, World)

static struct BurnRomInfo drgw2xRomDesc[] = {
	{ "dragon_ii_v-100x.bin",	0x080000, 0x5e71851d, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "pgm_t0200.u7",		0x400000, 0xb0f6534d, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a0200.u5",		0x400000, 0x13b95069, 3 | BRF_GRA },			//  2 Sprite Color Data

	{ "pgm_b0200.u9",		0x400000, 0x932d0f13, 4 | BRF_GRA },			//  3 Sprite Masks & Color Indexes
};

STDROMPICKEXT(drgw2x, drgw2x, pgm)
STD_ROM_FN(drgw2x)

struct BurnDriver BurnDrvDrgw2x = {
	"dw2v100x", "drgw2", "pgm", NULL, "1997",
	"Dragon World II (V100X, World)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_PUZZLE, 0,
	NULL, drgw2xRomInfo, drgw2xRomName, NULL, NULL, NULL, NULL, pgmInputInfo, pgmDIPInfo,
	drgw2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Dragon World II (V100H, Hong Kong)

static struct BurnRomInfo drgw2hkRomDesc[] = {
	{ "dragon_ii_v-100-h.u2",	0x080000, 0xc6e2e6ec, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "pgm_t0200.u7",		0x400000, 0xb0f6534d, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a0200.u5",		0x400000, 0x13b95069, 3 | BRF_GRA },			//  2 Sprite Color Data

	{ "pgm_b0200.u9",		0x400000, 0x932d0f13, 4 | BRF_GRA },			//  3 Sprite Masks & Color Indexes
};

STDROMPICKEXT(drgw2hk, drgw2hk, pgm)
STD_ROM_FN(drgw2hk)

struct BurnDriver BurnDrvDrgw2hk = {
	"drgw2hk", "drgw2", "pgm", NULL, "1997",
	"Dragon World II (V100H, Hong Kong)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_PUZZLE, 0,
	NULL, drgw2hkRomInfo, drgw2hkRomName, NULL, NULL, NULL, NULL, pgmInputInfo, pgmDIPInfo,
	drgw2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// The Killing Blade (V109, China)

static struct BurnRomInfo killbldRomDesc[] = {
	{ "p0300_v109.u9",		0x200000, 0x2fcee215, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "pgm_t0300.u14",		0x400000, 0x0922f7d9, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a0300.u9",		0x400000, 0x3f9455d3, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a0301.u10",		0x400000, 0x92776889, 3 | BRF_GRA },			//  3
	{ "pgm_a0303.u11",		0x400000, 0x33f5cc69, 3 | BRF_GRA },			//  4
	{ "pgm_a0306.u12",		0x400000, 0xcc018a8e, 3 | BRF_GRA },			//  5
	{ "pgm_a0307.u2",		0x400000, 0xbc772e39, 3 | BRF_GRA },			//  6

	{ "pgm_b0300.u13",		0x400000, 0x7f876981, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b0302.u14",		0x400000, 0xeea9c502, 4 | BRF_GRA },			//  8
	{ "pgm_b0303.u15",		0x200000, 0x77a9652e, 4 | BRF_GRA },			//  9

	{ "pgm_m0300.u1",		0x400000, 0x93159695, 5 | BRF_SND },			// 10 Samples

	{ "kb_u2.rom",			0x010000, 0xde3eae63, 9 | BRF_PRG | BRF_ESS },	// 11 Protection Data
};

STDROMPICKEXT(killbld, killbld, pgm)
STD_ROM_FN(killbld)

static INT32 killbldInit()
{
	pPgmInitCallback = pgm_decrypt_killbld;
	pPgmProtCallback = install_protection_asic25_asic22_killbld;

	return pgmInit();
}

struct BurnDriver BurnDrvKillbld = {
	"killbld", NULL, "pgm", NULL, "1998",
	"The Killing Blade (V109, China)\0", NULL, "IGS", "PolyGameMaster",
	L"The Killing Blade\0\u50B2\u5251\u72C2\u5200 (V109, China)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM, GBF_VSFIGHT, 0,
	NULL, killbldRomInfo, killbldRomName, NULL, NULL, NULL, NULL, pgmInputInfo, killbldDIPInfo,
	killbldInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// The Killing Blade (V106)

static struct BurnRomInfo killbld106RomDesc[] = {
	{ "kb_u3_v106.u3",		0x080000, 0x33b9111a, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code
	{ "kb_u6_v106.u6",		0x080000, 0x1c957bd7, 1 | BRF_PRG | BRF_ESS },	//  1
	{ "kb_u4_v106.u4",		0x080000, 0x169bbaaf, 1 | BRF_PRG | BRF_ESS },	//  2
	{ "kb_u5_v106.u5",		0x080000, 0xdf85abd4, 1 | BRF_PRG | BRF_ESS },	//  3

	{ "pgm_t0300.u14",		0x400000, 0x0922f7d9, 2 | BRF_GRA },			//  4 Tile data

	{ "pgm_a0300.u9",		0x400000, 0x3f9455d3, 3 | BRF_GRA },			//  5 Sprite Color Data
	{ "pgm_a0301.u10",		0x400000, 0x92776889, 3 | BRF_GRA },			//  6
	{ "pgm_a0303.u11",		0x400000, 0x33f5cc69, 3 | BRF_GRA },			//  7
	{ "pgm_a0306.u12",		0x400000, 0xcc018a8e, 3 | BRF_GRA },			//  8
	{ "pgm_a0307.u2",		0x400000, 0xbc772e39, 3 | BRF_GRA },			//  9

	{ "pgm_b0300.u13",		0x400000, 0x7f876981, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "pgm_b0302.u14",		0x400000, 0xeea9c502, 4 | BRF_GRA },			// 11
	{ "pgm_b0303.u15",		0x200000, 0x77a9652e, 4 | BRF_GRA },			// 12

	{ "pgm_m0300.u1",		0x400000, 0x93159695, 5 | BRF_SND },			// 13 Samples

	{ "kb_u2_v106.u2",		0x010000, 0x5df8cf51, 9 | BRF_PRG | BRF_ESS },	// 14 Protection Data
};

STDROMPICKEXT(killbld106, killbld106, pgm)
STD_ROM_FN(killbld106)

struct BurnDriver BurnDrvKillbld106 = {
	"killbld106", "killbld", "pgm", NULL, "1998",
	"The Killing Blade (V106)\0", NULL, "IGS", "PolyGameMaster",
	L"The Killing Blade\0\u50B2\u5251\u72C2\u5200 (V106)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_VSFIGHT, 0,
	NULL, killbld106RomInfo, killbld106RomName, NULL, NULL, NULL, NULL, pgmInputInfo, killbld104DIPInfo,
	killbldInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// The Killing Blade (V104)

static struct BurnRomInfo killbld104RomDesc[] = {
	{ "kb_u3_v104.u3",		0x080000, 0x6db1d719, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code
	{ "kb_u6_v104.u6",		0x080000, 0x31ecc978, 1 | BRF_PRG | BRF_ESS },	//  1
	{ "kb_u4_v104.u4",		0x080000, 0x1ed8b2e7, 1 | BRF_PRG | BRF_ESS },	//  2
	{ "kb_u5_v104.u5",		0x080000, 0xa0bafc29, 1 | BRF_PRG | BRF_ESS },	//  3

	{ "pgm_t0300.u14",		0x400000, 0x0922f7d9, 2 | BRF_GRA },			//  4 Tile data

	{ "pgm_a0300.u9",		0x400000, 0x3f9455d3, 3 | BRF_GRA },			//  5 Sprite Color Data
	{ "pgm_a0301.u10",		0x400000, 0x92776889, 3 | BRF_GRA },			//  6
	{ "pgm_a0303.u11",		0x400000, 0x33f5cc69, 3 | BRF_GRA },			//  7
	{ "pgm_a0306.u12",		0x400000, 0xcc018a8e, 3 | BRF_GRA },			//  8
	{ "pgm_a0307.u2",		0x400000, 0xbc772e39, 3 | BRF_GRA },			//  9

	{ "pgm_b0300.u13",		0x400000, 0x7f876981, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "pgm_b0302.u14",		0x400000, 0xeea9c502, 4 | BRF_GRA },			// 11
	{ "pgm_b0303.u15",		0x200000, 0x77a9652e, 4 | BRF_GRA },			// 12

	{ "pgm_m0300.u1",		0x400000, 0x93159695, 5 | BRF_SND },			// 13 Samples

	{ "kb_u2_v104.u2",		0x010000, 0xc970f6d5, 9 | BRF_PRG | BRF_ESS },	// 14 Protection Data
};

STDROMPICKEXT(killbld104, killbld104, pgm)
STD_ROM_FN(killbld104)

struct BurnDriver BurnDrvKillbld104 = {
	"killbld104", "killbld", "pgm", NULL, "1998",
	"The Killing Blade (V104)\0", NULL, "IGS", "PolyGameMaster",
	L"The Killing Blade\0\u50B2\u5251\u72C2\u5200 (V104)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_VSFIGHT, 0,
	NULL, killbld104RomInfo, killbld104RomName, NULL, NULL, NULL, NULL, pgmInputInfo, killbld104DIPInfo,
	killbldInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Dragon World 3 (V106, Korea)

static struct BurnRomInfo drgw3RomDesc[] = {
	{ "dw3_v106.u12",  		0x080000, 0xc3f6838b, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code
	{ "dw3_v106.u13",  		0x080000, 0x28284e22, 1 | BRF_PRG | BRF_ESS },  //  1

	{ "pgm_t0400.u18",  	0x400000, 0xb70f3357, 2 | BRF_GRA },	        //  2 Tile data

	{ "pgm_a0400.u9",   	0x400000, 0xdd7bfd40, 3 | BRF_GRA },	        //  3 Sprite Color Data
	{ "pgm_a0401.u10",  	0x400000, 0xcab6557f, 3 | BRF_GRA },	        //  4

	{ "pgm_b0400.u13",  	0x400000, 0x4bb87cc0, 4 | BRF_GRA },	        //  5 Sprite Masks & Color Indexes

	{ "pgm_m0400.u1",   	0x400000, 0x031eb9ce, 5 | BRF_SND },	        //  6 Samples

	{ "dw3_v100_u15.u15",  	0x010000, 0x03dc4fdf, 9 | BRF_PRG | BRF_ESS },  //  7 Protection data
};

STDROMPICKEXT(drgw3, drgw3, pgm)
STD_ROM_FN(drgw3)

static INT32 drgw3Init()
{
	pPgmInitCallback = pgm_decrypt_dw3;
	pPgmProtCallback = install_protection_asic25_asic22_drgw3;

	return pgmInit();
}

struct BurnDriverD BurnDrvDrgw3 = {
	"drgw3", NULL, "pgm", NULL, "1998",
	"Dragon World 3 (V106, Korea)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	0, 4, HARDWARE_IGS_PGM, GBF_PUZZLE, 0,
	NULL, drgw3RomInfo, drgw3RomName, NULL, NULL, NULL, NULL, pgmInputInfo, pgmDIPInfo,
	drgw3Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Dragon World 3 (ver. 105)

static struct BurnRomInfo drgw3105RomDesc[] = {
	{ "dw3_v105.u12",  		0x080000, 0xc5e24318, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code
	{ "dw3_v105.u13",  		0x080000, 0x8d6c9d39, 1 | BRF_PRG | BRF_ESS },  //  1

	{ "pgm_t0400.u18",  	0x400000, 0xb70f3357, 2 | BRF_GRA },	        //  2 Tile data

	{ "pgm_a0400.u9",   	0x400000, 0xdd7bfd40, 3 | BRF_GRA },	        //  3 Sprite Color Data
	{ "pgm_a0401.u10",  	0x400000, 0xcab6557f, 3 | BRF_GRA },	        //  4

	{ "pgm_b0400.u13",  	0x400000, 0x4bb87cc0, 4 | BRF_GRA },	        //  5 Sprite Masks & Color Indexes

	{ "pgm_m0400.u1",   	0x400000, 0x031eb9ce, 5 | BRF_SND },	        //  6 Samples

	{ "dw3_v100_u15.u15",  	0x010000, 0x03dc4fdf, 9 | BRF_PRG | BRF_ESS },  //  7 Protection data
};

STDROMPICKEXT(drgw3105, drgw3105, pgm)
STD_ROM_FN(drgw3105)

struct BurnDriverD BurnDrvDrgw3105 = {
	"drgw3105", "drgw3", "pgm", NULL, "1998",
	"Dragon World 3 (ver. 105)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_PUZZLE, 0,
	NULL, drgw3105RomInfo, drgw3105RomName, NULL, NULL, NULL, NULL, pgmInputInfo, pgmDIPInfo,
	drgw3Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Chuugokuryuu 3 Special (ver. 103, Japan)
// Japan only, has an extra game mode option!

static struct BurnRomInfo drgw3103RomDesc[] = {
	{ "dragon_iii_v103j_u12.u12",  	0x080000, 0x275b39a2, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code
	{ "dragon_iii_v103j_u13.u13",  	0x080000, 0x9aa56e8f, 1 | BRF_PRG | BRF_ESS },  //  1

	{ "pgm_t0400.u18",  	0x400000, 0xb70f3357, 2 | BRF_GRA },	        //  2 Tile data

	{ "pgm_a0400.u9",   	0x400000, 0xdd7bfd40, 3 | BRF_GRA },	        //  3 Sprite Color Data
	{ "pgm_a0401.u10",  	0x400000, 0xcab6557f, 3 | BRF_GRA },	        //  4

	{ "pgm_b0400.u13",  	0x400000, 0x4bb87cc0, 4 | BRF_GRA },	        //  5 Sprite Masks & Color Indexes

	{ "pgm_m0400.u1",   	0x400000, 0x031eb9ce, 5 | BRF_SND },	        //  6 Samples

	{ "dw3_v100_u15.u15",  	0x010000, 0x03dc4fdf, 9 | BRF_PRG | BRF_ESS },  //  7 Protection data
};

STDROMPICKEXT(drgw3103, drgw3103, pgm)
STD_ROM_FN(drgw3103)

struct BurnDriverD BurnDrvDrgw3103 = {
	"drgw3103", "drgw3", "pgm", NULL, "1998",
	"Chuugokuryuu 3 Special (ver. 103, japan)\0", NULL, "IGS (Alta Co., LTD License)", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_PUZZLE, 0,
	NULL, drgw3103RomInfo, drgw3103RomName, NULL, NULL, NULL, NULL, pgmInputInfo, pgmDIPInfo,
	drgw3Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Chuugokuryuu 3 Special (ver. 100, Japan)
// Japan only, has an extra game mode option!

static struct BurnRomInfo drgw3100RomDesc[] = {
	{ "dw3_v100.u12",  		0x080000, 0x47243906, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code
	{ "dw3_v100.u13",  		0x080000, 0xb7cded21, 1 | BRF_PRG | BRF_ESS },  //  1

	{ "pgm_t0400.u18",  	0x400000, 0xb70f3357, 2 | BRF_GRA },	        //  2 Tile data

	{ "pgm_a0400.u9",   	0x400000, 0xdd7bfd40, 3 | BRF_GRA },	        //  3 Sprite Color Data
	{ "pgm_a0401.u10",  	0x400000, 0xcab6557f, 3 | BRF_GRA },	        //  4

	{ "pgm_b0400.u13",  	0x400000, 0x4bb87cc0, 4 | BRF_GRA },	        //  5 Sprite Masks & Color Indexes

	{ "pgm_m0400.u1",   	0x400000, 0x031eb9ce, 5 | BRF_SND },	        //  6 Samples

	{ "dw3_v100_u15.u15",  	0x010000, 0x03dc4fdf, 9 | BRF_PRG | BRF_ESS },  //  7 Protection data
};

STDROMPICKEXT(drgw3100, drgw3100, pgm)
STD_ROM_FN(drgw3100)

struct BurnDriverD BurnDrvDrgw3100 = {
	"drgw3100", "drgw3", "pgm", NULL, "1998",
	"Chuugokuryuu 3 Special (ver. 100, Japan)\0", NULL, "IGS (Alta Co., LTD License)", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_PUZZLE, 0,
	NULL, drgw3100RomInfo, drgw3100RomName, NULL, NULL, NULL, NULL, pgmInputInfo, pgmDIPInfo,
	drgw3Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Dragon World 3 EX (ver. 100)

static struct BurnRomInfo dwexRomDesc[] = {
	{ "ex_v100.u12",   		0x080000, 0xbc171799, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code
	{ "ex_v100.u13",   		0x080000, 0x7afe6322, 1 | BRF_PRG | BRF_ESS },  //  1

	{ "pgm_t0400.u18",  	0x200000, 0x9ecc950d, 2 | BRF_GRA },	        //  2 Tile data

	{ "pgm_a0400.u9",   	0x400000, 0xdd7bfd40, 3 | BRF_GRA },	        //  3 Sprite Color Data
	{ "pgm_a0401.u10",  	0x200000, 0xd36c06a4, 3 | BRF_GRA },	        //  4

	{ "pgm_b0400.u13",  	0x400000, 0x4bb87cc0, 4 | BRF_GRA },	        //  5 Sprite Masks & Color Indexes

	{ "pgm_m0400.u1",   	0x200000, 0x42d54fd5, 5 | BRF_SND },	        //  6 Samples

	{ "data_u15.u15",   	0x010000, 0x03dc4fdf, 9 | BRF_PRG | BRF_ESS },  //  7 Protection data
};

STDROMPICKEXT(dwex, dwex, pgm)
STD_ROM_FN(dwex)

struct BurnDriverD BurnDrvDwex = {
	"dwex", NULL, "pgm", NULL, "1998",
	"Dragon World 3 EX (ver. 100)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	0, 4, HARDWARE_IGS_PGM, GBF_PUZZLE, 0,
	NULL, dwexRomInfo, dwexRomName, NULL, NULL, NULL, NULL, pgmInputInfo, pgmDIPInfo,
	drgw3Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Dragon World 3 EX (ver. 101, China)

static struct BurnRomInfo dwex101cnRomDesc[] = {
	{ "ex_v101cn.u12",   	0x080000, 0x4f951f42, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code
	{ "ex_v101cn.u13",   	0x080000, 0x7afe6322, 1 | BRF_PRG | BRF_ESS },  //  1

	{ "pgm_t0400.u18",  	0x200000, 0x9ecc950d, 2 | BRF_GRA },	        //  2 Tile data

	{ "pgm_a0400.u9",   	0x400000, 0xdd7bfd40, 3 | BRF_GRA },	        //  3 Sprite Color Data
	{ "pgm_a0401.u10",  	0x200000, 0xd36c06a4, 3 | BRF_GRA },	        //  4

	{ "pgm_b0400.u13",  	0x400000, 0x4bb87cc0, 4 | BRF_GRA },	        //  5 Sprite Masks & Color Indexes

	{ "pgm_m0400.u1",   	0x200000, 0x42d54fd5, 5 | BRF_SND },	        //  6 Samples

	{ "data_u15.u15",   	0x010000, 0x03dc4fdf, 9 | BRF_PRG | BRF_ESS },  //  7 Protection data
};

STDROMPICKEXT(dwex101cn, dwex101cn, pgm)
STD_ROM_FN(dwex101cn)

struct BurnDriverD BurnDrvDwex101cn = {
	"dwex101cn", "dwex", "pgm", NULL, "1998",
	"Dragon World 3 EX (ver. 101, China)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_PUZZLE, 0,
	NULL, dwex101cnRomInfo, dwex101cnRomName, NULL, NULL, NULL, NULL, pgmInputInfo, pgmDIPInfo,
	drgw3Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Oriental Legend Super (V101, Korea)

static struct BurnRomInfo oldsRomDesc[] = {
	{ "sp_v101_u2.u2",	   	0x080000, 0x08eb9661, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code
	{ "sp_v101_u3.u3",	   	0x080000, 0x0a358c1e, 1 | BRF_PRG | BRF_ESS },  //  1
	{ "sp_v101_u4.u4",	   	0x080000, 0x766570e0, 1 | BRF_PRG | BRF_ESS },  //  2
	{ "sp_v101_u5.u5",	   	0x080000, 0x58662e12, 1 | BRF_PRG | BRF_ESS },  //  3
	{ "sp_v101_u1.u1",	   	0x080000, 0x2b2f4f1e, 1 | BRF_PRG | BRF_ESS },  //  4

	{ "pgm_t0500.u18",	  	0x400000, 0xd881726c, 2 | BRF_GRA },			//  5 Tile data
	{ "pgm_t0501.u19",	   	0x200000, 0xd2106864, 2 | BRF_GRA },			//  6

	{ "pgm_a0500.u5",	   	0x400000, 0x80a59197, 3 | BRF_GRA },			//  7 Sprite Color Data
	{ "pgm_a0501.u6",	   	0x400000, 0x98c931b1, 3 | BRF_GRA },			//  8
	{ "pgm_a0502.u7",	   	0x400000, 0xc3fcdf1d, 3 | BRF_GRA },			//  9
	{ "pgm_a0503.u8",	   	0x400000, 0x066dffec, 3 | BRF_GRA },			// 10
	{ "pgm_a0504.u11",	   	0x400000, 0x45337583, 3 | BRF_GRA },			// 11
	{ "pgm_a0505.u12",	   	0x400000, 0x5b8cf3a5, 3 | BRF_GRA },			// 12
	{ "pgm_a0506.u13",	   	0x400000, 0x087ac60c, 3 | BRF_GRA },			// 13

	{ "pgm_b0500.u9",	   	0x400000, 0xcde07f74, 4 | BRF_GRA },			// 14 Sprite Masks & Color Indexes
	{ "pgm_b0501.u10",	   	0x400000, 0x1546c2e9, 4 | BRF_GRA },			// 15
	{ "pgm_b0502.u15",	   	0x400000, 0xe97b31c3, 4 | BRF_GRA },			// 16
	{ "pgm_b0503.u16",	   	0x400000, 0xe41d98e4, 4 | BRF_GRA },			// 17

	{ "pgm_m0500.u1",	   	0x200000, 0x37928cdd, 5 | BRF_SND },			// 18 Samples

	{ "sp_v101_u6.u6",	   	0x010000, 0x097046bc, 9 | BRF_PRG | BRF_ESS },  // 19 Protection Rom
	
#if defined (ROM_VERIFY)
	{ "ram_dump",	   		0x040000, 0x280cfb4e, 0 | BRF_OPT },
#endif
};

STDROMPICKEXT(olds, olds, pgm)
STD_ROM_FN(olds)

static INT32 oldsInit()
{
	pPgmProtCallback = install_protection_asic25_asic28_olds;

	return pgmInit();
}

struct BurnDriver BurnDrvOlds = {
	"olds", NULL, "pgm", NULL, "1999",
	"Oriental Legend Super (V101, Korea)\0", "Imperfect Protection Emulation", "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM, GBF_SCRFIGHT, 0,
	NULL, oldsRomInfo, oldsRomName, NULL, NULL, NULL, NULL, pgmInputInfo, oldsDIPInfo,
	oldsInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Oriental Legend Special / Xi Yo Gi Shi Re Zuang Super (V100)

static struct BurnRomInfo olds100RomDesc[] = {
	{ "sp_v100_u2.u2",   	0x080000, 0x517c2a06, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code
	{ "sp_v100_u3.u3",   	0x080000, 0xd0e2b741, 1 | BRF_PRG | BRF_ESS },  //  1
	{ "sp_v100_u4.u4",   	0x080000, 0x32a6bdbd, 1 | BRF_PRG | BRF_ESS },  //  2
	{ "sp_v100_u5.u5",   	0x080000, 0xb4a1cafb, 1 | BRF_PRG | BRF_ESS },  //  3
	{ "sp_v100_u1.u1",   	0x080000, 0x37ea4e75, 1 | BRF_PRG | BRF_ESS },  //  4

	{ "pgm_t0500.u18",	  	0x400000, 0xd881726c, 2 | BRF_GRA },			//  5 Tile data
	{ "pgm_t0501.u19",	   	0x200000, 0xd2106864, 2 | BRF_GRA },			//  6

	{ "pgm_a0500.u5",	   	0x400000, 0x80a59197, 3 | BRF_GRA },			//  7 Sprite Color Data
	{ "pgm_a0501.u6",	   	0x400000, 0x98c931b1, 3 | BRF_GRA },			//  8
	{ "pgm_a0502.u7",	   	0x400000, 0xc3fcdf1d, 3 | BRF_GRA },			//  9
	{ "pgm_a0503.u8",	   	0x400000, 0x066dffec, 3 | BRF_GRA },			// 10
	{ "pgm_a0504.u11",	   	0x400000, 0x45337583, 3 | BRF_GRA },			// 11
	{ "pgm_a0505.u12",	   	0x400000, 0x5b8cf3a5, 3 | BRF_GRA },			// 12
	{ "pgm_a0506.u13",	   	0x400000, 0x087ac60c, 3 | BRF_GRA },			// 13

	{ "pgm_b0500.u9",	   	0x400000, 0xcde07f74, 4 | BRF_GRA },			// 14 Sprite Masks & Color Indexes
	{ "pgm_b0501.u10",	   	0x400000, 0x1546c2e9, 4 | BRF_GRA },			// 15
	{ "pgm_b0502.u15",	   	0x400000, 0xe97b31c3, 4 | BRF_GRA },			// 16
	{ "pgm_b0503.u16",	   	0x400000, 0xe41d98e4, 4 | BRF_GRA },			// 17

	{ "pgm_m0500.u1",	   	0x200000, 0x37928cdd, 5 | BRF_SND },			// 18 Samples

	{ "sp_v100_u6.u6",	   	0x010000, 0xe7613dda, 9 | BRF_PRG | BRF_ESS },  // 19 Protection Rom
	
#if defined (ROM_VERIFY)
	{ "ram_dump",	   		0x040000, 0x280cfb4e, 0 | BRF_OPT },
#endif
};

STDROMPICKEXT(olds100, olds100, pgm)
STD_ROM_FN(olds100)

struct BurnDriver BurnDrvOlds100 = {
	"olds100", "olds", "pgm", NULL, "1998",
	"Oriental Legend Special - Xi Yo Gi Shi Re Zuang Super (V100)\0", "Imperfect Protection Emulation", "IGS", "PolyGameMaster",
	L"Oriental Legend Super\0\u897F\u6E38\u91CA\u5384\u4F20 Super (V100)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_SCRFIGHT, 0,
	NULL, olds100RomInfo, olds100RomName, NULL, NULL, NULL, NULL, pgmInputInfo, olds100DIPInfo,
	oldsInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Oriental Legend Special / Xi Yo Gi Shi Re Zuang Super (V100 alt)

static struct BurnRomInfo olds100aRomDesc[] = {
	{ "pgm_p0500_v100.u24",	0x400000, 0x8981fc87, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t0500.u18",	   	0x400000, 0xd881726c, 2 | BRF_GRA },			//  1 Tile data
	{ "pgm_t0501.u19",	   	0x200000, 0xd2106864, 2 | BRF_GRA },			//  2

	{ "pgm_a0500.u5",	   	0x400000, 0x80a59197, 3 | BRF_GRA },			//  3 Sprite Color Data
	{ "pgm_a0501.u6",	   	0x400000, 0x98c931b1, 3 | BRF_GRA },			//  4
	{ "pgm_a0502.u7",	   	0x400000, 0xc3fcdf1d, 3 | BRF_GRA },			//  5
	{ "pgm_a0503.u8",	   	0x400000, 0x066dffec, 3 | BRF_GRA },			//  6
	{ "pgm_a0504.u11",	   	0x400000, 0x45337583, 3 | BRF_GRA },			//  7
	{ "pgm_a0505.u12",	   	0x400000, 0x5b8cf3a5, 3 | BRF_GRA },			//  8
	{ "pgm_a0506.u13",	   	0x400000, 0x087ac60c, 3 | BRF_GRA },			//  9

	{ "pgm_b0500.u9",	   	0x400000, 0xcde07f74, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "pgm_b0501.u10",	   	0x400000, 0x1546c2e9, 4 | BRF_GRA },			// 11
	{ "pgm_b0502.u15",	   	0x400000, 0xe97b31c3, 4 | BRF_GRA },			// 12
	{ "pgm_b0503.u16",	   	0x400000, 0xe41d98e4, 4 | BRF_GRA },			// 13

	{ "pgm_m0500.u1",	   	0x200000, 0x37928cdd, 5 | BRF_SND },			// 14 Samples
	
#if !defined (ROM_VERIFY)
	{ "kd-u6.512",	   		0x010000, 0xe7613dda, 9 | BRF_PRG | BRF_ESS },  // 15 Protection Rom
#else
	{ "ram_dump",	   		0x040000, 0x280cfb4e, 0 | BRF_OPT },
#endif
};

STDROMPICKEXT(olds100a, olds100a, pgm)
STD_ROM_FN(olds100a)

struct BurnDriver BurnDrvOlds100a = {
	"olds100a", "olds", "pgm", NULL, "1998",
	"Oriental Legend Special - Xi Yo Gi Shi Re Zuang Super (V100 alt)\0", "Imperfect Protection Emulation", "IGS", "PolyGameMaster",
	L"Oriental Legend Super\0\u897F\u6E38\u91CA\u5384\u4F20 Super (V100 alt)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_SCRFIGHT, 0,
	NULL, olds100aRomInfo, olds100aRomName, NULL, NULL, NULL, NULL, pgmInputInfo, olds100DIPInfo,
	oldsInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Oriental Legend Special / Xi Yo Gi Shi Re Zuang Super (V103, China, Tencent) (unprotected)

static struct BurnRomInfo olds103tRomDesc[] = {
	{ "p0500.v103",	   		0x400000, 0x17e32e14, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t0500.u18",	   	0x400000, 0xd881726c, 2 | BRF_GRA },			//  1 Tile data
	{ "pgm_t0501.u19",	   	0x200000, 0xd2106864, 2 | BRF_GRA },			//  2

	{ "pgm_a0500.u5",	   	0x400000, 0x80a59197, 3 | BRF_GRA },			//  3 Sprite Color Data
	{ "pgm_a0501.u6",	   	0x400000, 0x98c931b1, 3 | BRF_GRA },			//  4
	{ "pgm_a0502.u7",	   	0x400000, 0xc3fcdf1d, 3 | BRF_GRA },			//  5
	{ "pgm_a0503.u8",	   	0x400000, 0x066dffec, 3 | BRF_GRA },			//  6
	{ "pgm_a0504.u11",	   	0x400000, 0x45337583, 3 | BRF_GRA },			//  7
	{ "pgm_a0505.u12",	   	0x400000, 0x5b8cf3a5, 3 | BRF_GRA },			//  8
	{ "pgm_a0506.u13",	   	0x400000, 0x087ac60c, 3 | BRF_GRA },			//  9

	{ "pgm_b0500.u9",	   	0x400000, 0xcde07f74, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "pgm_b0501.u10",	   	0x400000, 0x1546c2e9, 4 | BRF_GRA },			// 11
	{ "pgm_b0502.u15",	   	0x400000, 0xe97b31c3, 4 | BRF_GRA },			// 12
	{ "pgm_b0503.u16",	   	0x400000, 0xe41d98e4, 4 | BRF_GRA },			// 13

	{ "pgm_m0500.u1",	   	0x200000, 0x37928cdd, 5 | BRF_SND },			// 14 Samples
};

STDROMPICKEXT(olds103t, olds103t, pgm)
STD_ROM_FN(olds103t)

struct BurnDriver BurnDrvOlds103t = {
	"olds103t", "olds", "pgm", NULL, "2003",
	"Oriental Legend Special - Xi Yo Gi Shi Re Zuang Super (V103, China, Tencent) (unprotected)\0", NULL, "IGS", "PolyGameMaster",
	L"Oriental Legend Super\0\u897F\u6E38\u91CA\u5384\u4F20 Super (V103, China, Tencent) (unprotected)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_SCRFIGHT, 0,
	NULL, olds103tRomInfo, olds103tRomName, NULL, NULL, NULL, NULL, pgmInputInfo, olds103tDIPInfo,
	pgmInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour / Sangoku Senki (V117)

static struct BurnRomInfo kovRomDesc[] = {
	{ "p0600.117",     		0x400000, 0xc4d19fe6, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t0600.u11",     	0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data
	
	{ "pgm_a0600.u2",     	0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a0601.u4",     	0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "pgm_a0602.u6",     	0x800000, 0xe7a32959, 3 | BRF_GRA },			//  4
	{ "pgm_a0603.u8",     	0x400000, 0xec31abda, 3 | BRF_GRA },			//  5
	
	{ "pgm_b0600.u5",     	0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "pgm_b0601.u7",     	0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			//  7
	
	{ "pgm_m0600.u3",     	0x400000, 0x3ada4fd6, 5 | BRF_SND },			//  8 Samples
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kov, kov, pgm)
STD_ROM_FN(kov)

static INT32 kovInit()
{
	pPgmInitCallback = pgm_decrypt_kov;
	pPgmProtCallback = install_protection_asic27_kov; // simulation

	return pgmInit();
}

struct BurnDriver BurnDrvKov = {
	"kov", NULL, "pgm", NULL, "1999",
	"Knights of Valour - Sangoku Senki (V117)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour\0\u4E09\u56FD\u6226\u7D00\0\u4E09\u56FD\u6218\u7EAA (V117)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kovRomInfo, kovRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour / Sangoku Senki (V115)

static struct BurnRomInfo kov115RomDesc[] = {
	{ "p0600.115",     		0x400000, 0x527a2924, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t0600.u11",     	0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data
	
	{ "pgm_a0600.u2",     	0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a0601.u4",     	0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "pgm_a0602.u6",     	0x800000, 0xe7a32959, 3 | BRF_GRA },			//  4
	{ "pgm_a0603.u8",     	0x400000, 0xec31abda, 3 | BRF_GRA },			//  5
	
	{ "pgm_b0600.u5",     	0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "pgm_b0601.u7",     	0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			//  7
	
	{ "pgm_m0600.u3",     	0x400000, 0x3ada4fd6, 5 | BRF_SND },			//  8 Samples
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	//  9 Internal ARM7 Rom
};

STDROMPICKEXT(kov115, kov115, pgm)
STD_ROM_FN(kov115)

struct BurnDriver BurnDrvKov115 = {
	"kov115", "kov", "pgm", NULL, "1999",
	"Knights of Valour - Sangoku Senki (V115)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour\0\u4E09\u56FD\u6226\u7D00\0\u4E09\u56FD\u6218\u7EAA (V115)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kov115RomInfo, kov115RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour Plus / Sangoku Senki (V114, Hong Kong)

static struct BurnRomInfo kov114RomDesc[] = {
	{ "sav114.u4",     		0x080000, 0x4db3d4d3, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code
	{ "sav114.u7",     		0x080000, 0xb8d12b0c, 1 | BRF_PRG | BRF_ESS },  //  1
	{ "sav114.u5",     		0x080000, 0x9e586dab, 1 | BRF_PRG | BRF_ESS },  //  2
	{ "sav114.u8",     		0x080000, 0xab129997, 1 | BRF_PRG | BRF_ESS },  //  3
	{ "sav114.u10",    		0x080000, 0x8f84ecfd, 1 | BRF_PRG | BRF_ESS },  //  4

	{ "pgm_t0600.u11",     	0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  5 Tile data
	
	{ "pgm_a0600.u2",     	0x800000, 0xd8167834, 3 | BRF_GRA },			//  6 Sprite Color Data
	{ "pgm_a0601.u4",     	0x800000, 0xff7a4373, 3 | BRF_GRA },			//  7
	{ "pgm_a0602.u6",     	0x800000, 0xe7a32959, 3 | BRF_GRA },			//  8
	{ "pgm_a0603.u9",     	0x400000, 0xec31abda, 3 | BRF_GRA },			//  9
	
	{ "pgm_b0600.u5",     	0x800000, 0x7d3cd059, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "pgm_b0601.u7",     	0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			// 11
	
	{ "pgm_m0600.u9",     	0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 12 Samples
	
	// IGS 027 55857E 100 9901 HONG KONG
	{ "kov_igs027.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	// 13 Internal ARM7 Rom
};

STDROMPICKEXT(kov114, kov114, pgm)
STD_ROM_FN(kov114)

struct BurnDriver BurnDrvKov114 = {
	"kov114", "kov", "pgm", NULL, "1999",
	"Knights of Valour - Sangoku Senki (V114, Hong Kong)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour\0\u4E09\u56FD\u6226\u7D00\0\u4E09\u56FD\u6218\u7EAA (V114, Hong Kong)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kov114RomInfo, kov114RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kov114DIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour Plus / Sangoku Senki (V111, Japan)

static struct BurnRomInfo kov111RomDesc[] = {
	{ "sav111_u4.u4",     	0x080000, 0xae2f1b4e, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code
	{ "sav111_u7.u7",     	0x080000, 0x95eedf0e, 1 | BRF_PRG | BRF_ESS },  //  1
	{ "sav111_u5.u5",     	0x080000, 0x5fdd4aa8, 1 | BRF_PRG | BRF_ESS },  //  2
	{ "sav111_u8.u8",     	0x080000, 0x003cbf49, 1 | BRF_PRG | BRF_ESS },  //  3
	{ "sav111_u10.u10",    	0x080000, 0xd5536107, 1 | BRF_PRG | BRF_ESS },  //  4

	{ "pgm_t0600.u11",     	0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  5 Tile data
	
	{ "pgm_a0600.u2",     	0x800000, 0xd8167834, 3 | BRF_GRA },			//  6 Sprite Color Data
	{ "pgm_a0601.u4",     	0x800000, 0xff7a4373, 3 | BRF_GRA },			//  7
	{ "pgm_a0602.u6",     	0x800000, 0xe7a32959, 3 | BRF_GRA },			//  8
	{ "pgm_a0603.u9",     	0x400000, 0xec31abda, 3 | BRF_GRA },			//  9
	
	{ "pgm_b0600.u5",     	0x800000, 0x7d3cd059, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "pgm_b0601.u7",     	0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			// 11
	
	{ "pgm_m0600.u9",     	0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 12 Samples	
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	// 13 Internal ARM7 Rom
};

STDROMPICKEXT(kov111, kov111, pgm)
STD_ROM_FN(kov111)

struct BurnDriver BurnDrvKov111 = {
	"kov111", "kov", "pgm", NULL, "1999",
	"Knights of Valour - Sangoku Senki (V111, Japan)\0", NULL, "IGS (Alta Co., LTD License)", "PolyGameMaster",
	L"Knights of Valour\0\u4E09\u56FD\u6226\u7D00\0\u4E09\u56FD\u6218\u7EAA (V111, Japan)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kov111RomInfo, kov111RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kov111DIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour Plus / Sangoku Senki Plus (V119)

static struct BurnRomInfo kovplusRomDesc[] = {
	{ "p0600.119",     		0x400000, 0xe4b0875d, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t0600.u11",     	0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data
	
	{ "pgm_a0600.u2",     	0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a0601.u4",     	0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "pgm_a0602.u6",     	0x800000, 0xe7a32959, 3 | BRF_GRA },			//  4
	{ "pgm_a0603.u9",     	0x400000, 0xec31abda, 3 | BRF_GRA },			//  5
	
	{ "pgm_b0600.u5",     	0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "pgm_b0601.u7",     	0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			//  7
	
	{ "pgm_m0600.u3",     	0x400000, 0x3ada4fd6, 5 | BRF_SND },			//  8 Samples
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovplus, kovplus, pgm)
STD_ROM_FN(kovplus)

struct BurnDriver BurnDrvKovplus = {
	"kovplus", NULL, "pgm", NULL, "1999",
	"Knights of Valour Plus - Sangoku Senki Plus (V119)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour Plus\0\u4E09\u56FD\u6226\u7D00 Plus\0\u4E09\u56FD\u6218\u7EAA Plus (V119)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kovplusRomInfo, kovplusRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour Plus / Sangoku Senki Plus (V119 alt)

static struct BurnRomInfo kovplusaRomDesc[] = {
	{ "v119.u3",       		0x080000, 0x6750388f, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code
	{ "v119.u5",       		0x080000, 0xd4101ffd, 1 | BRF_PRG | BRF_ESS },  //  1
	{ "v119.u4",       		0x080000, 0x8200ece6, 1 | BRF_PRG | BRF_ESS },  //  2
	{ "v119.u6",       		0x080000, 0x71e28f27, 1 | BRF_PRG | BRF_ESS },  //  3
	{ "v119.u2",	   		0x080000, 0x29588ef2, 1 | BRF_PRG | BRF_ESS },  //  4

	{ "pgm_t0600.u11",     	0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  5 Tile data
	
	{ "pgm_a0600.u2",     	0x800000, 0xd8167834, 3 | BRF_GRA },			//  6 Sprite Color Data
	{ "pgm_a0601.u4",     	0x800000, 0xff7a4373, 3 | BRF_GRA },			//  7
	{ "pgm_a0602.u6",     	0x800000, 0xe7a32959, 3 | BRF_GRA },			//  8
	{ "pgm_a0603.u9",     	0x400000, 0xec31abda, 3 | BRF_GRA },			//  9
	
	{ "pgm_b0600.u5",     	0x800000, 0x7d3cd059, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "pgm_b0601.u7",     	0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			// 11
	
	{ "pgm_m0600.u3",     	0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 12 Samples	
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  // 13 Internal ARM7 Rom
};

STDROMPICKEXT(kovplusa, kovplusa, pgm)
STD_ROM_FN(kovplusa)

struct BurnDriver BurnDrvKovplusa = {
	"kovplusa", "kovplus", "pgm", NULL, "1999",
	"Knights of Valour Plus - Sangoku Senki Plus (V119 alt)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour Plus\0\u4E09\u56FD\u6226\u7D00 Plus\0\u4E09\u56FD\u6218\u7EAA Plus (V119 alt)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kovplusaRomInfo, kovplusaRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour Superheroes / Sangoku Senki Superheroes (V104, China)

static struct BurnRomInfo kovshRomDesc[] = {
	{ "p0600.322",	   		0x400000, 0x7c78e5f3, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code
//	This is the same as above, but with the last 1mb 0-filled. 
//	{ "p0600.322",			0x400000, 0x4e2ba39b, 1 | BRF_PRG | BRF_ESS },	//  0 68k Code

	{ "pgm_t0600.u2",     	0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a0600.u3",	   	0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a0601.u4",	   	0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "pgm_a0602.u6",	   	0x800000, 0xe7a32959, 3 | BRF_GRA },			//  4
	{ "pgm_a0603.u8",	   	0x400000, 0xec31abda, 3 | BRF_GRA },			//  5
	{ "pgm_a0604.u10",	   	0x400000, 0x26b59fd3, 3 | BRF_GRA },			//  6

	{ "pgm_b0600.u7",	   	0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b0601.u9",	   	0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			//  8
	{ "pgm_b0602.u11",	   	0x100000, 0x9df77934, 4 | BRF_GRA },			//  9

	{ "pgm_m0600.u5",	   	0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 10 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS },  // 11 Internal ARM7 Rom
};

STDROMPICKEXT(kovsh, kovsh, pgm)
STD_ROM_FN(kovsh)

static INT32 kovshInit()
{
	pPgmInitCallback = pgm_decrypt_kovsh;
	pPgmProtCallback = install_protection_asic27a_kovsh;

	nPgmAsicRegionHackAddress = 0x3f0d;

	INT32 nRet = pgmInit();

	Arm7SetIdleLoopAddress(0x00000260);

	return nRet;
}

struct BurnDriver BurnDrvKovsh = {
	"kovsh", NULL, "pgm", NULL, "1999",
	"Knights of Valour Superheroes / Sangoku Senki Superheroes (V104, China)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour Superheroes\0\u4E09\u56FD\u6218\u7EAA\0\u98CE\u4E91\u518D\u8D77 (V104, China)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshRomInfo, kovshRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovshInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour Superheroes / Sangoku Senki Superheroes (V103)

static struct BurnRomInfo kovsh103RomDesc[] = {
	{ "p0600.103",	   		0x400000, 0xf0b3da82, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t0600.u2",     	0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a0600.u3",	   	0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a0601.u4",	   	0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "pgm_a0602.u6",	   	0x800000, 0xe7a32959, 3 | BRF_GRA },			//  4
	{ "pgm_a0603.u8",	   	0x400000, 0xec31abda, 3 | BRF_GRA },			//  5
	{ "pgm_a0604.u10",	   	0x400000, 0x26b59fd3, 3 | BRF_GRA },			//  6

	{ "pgm_b0600.u7",	   	0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b0601.u9",	   	0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			//  8
	{ "pgm_b0602.u11",	   	0x100000, 0x9df77934, 4 | BRF_GRA },			//  9

	{ "pgm_m0600.u5",	   	0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 10 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS },  // 11 Internal ARM7 Rom
};

STDROMPICKEXT(kovsh103, kovsh103, pgm)
STD_ROM_FN(kovsh103)

struct BurnDriver BurnDrvKovsh103 = {
	"kovsh103", "kovsh", "pgm", NULL, "1999",
	"Knights of Valour Superheroes / Sangoku Senki Superheroes (V103)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovsh103RomInfo, kovsh103RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour Superheroes / Sangoku Senki Superheroes (V102)

static struct BurnRomInfo kovsh102RomDesc[] = {
	{ "p0600.102",	   		0x400000, 0xfdd4fb0f, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t0600.u2",     	0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a0600.u3",	   	0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a0601.u4",	   	0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "pgm_a0602.u6",	   	0x800000, 0xe7a32959, 3 | BRF_GRA },			//  4
	{ "pgm_a0603.u8",	   	0x400000, 0xec31abda, 3 | BRF_GRA },			//  5
	{ "pgm_a0604.u10",	   	0x400000, 0x26b59fd3, 3 | BRF_GRA },			//  6

	{ "pgm_b0600.u7",	   	0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b0601.u9",	   	0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			//  8
	{ "pgm_b0602.u11",	   	0x100000, 0x9df77934, 4 | BRF_GRA },			//  9

	{ "pgm_m0600.u5",	   	0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 10 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS },  // 11 Internal ARM7 Rom
};

STDROMPICKEXT(kovsh102, kovsh102, pgm)
STD_ROM_FN(kovsh102)

struct BurnDriver BurnDrvKovsh102 = {
	"kovsh102", "kovsh", "pgm", NULL, "1999",
	"Knights of Valour Superheroes / Sangoku Senki Superheroes (V102)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovsh102RomInfo, kovsh102RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour Superheroes / Sangoku Senki Superheroes (V101)

static struct BurnRomInfo kovsh101RomDesc[] = {
	{ "p0600.101",	   		0x400000, 0x517a9bcf, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t0600.u2",     	0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a0600.u3",	   	0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a0601.u4",	   	0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "pgm_a0602.u6",	   	0x800000, 0xe7a32959, 3 | BRF_GRA },			//  4
	{ "pgm_a0603.u8",	   	0x400000, 0xec31abda, 3 | BRF_GRA },			//  5
	{ "pgm_a0604.u10",	   	0x400000, 0x26b59fd3, 3 | BRF_GRA },			//  6

	{ "pgm_b0600.u7",	   	0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b0601.u9",	   	0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			//  8
	{ "pgm_b0602.u11",	   	0x100000, 0x9df77934, 4 | BRF_GRA },			//  9

	{ "pgm_m0600.u5",	   	0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 10 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS },  // 11 Internal ARM7 Rom
};

STDROMPICKEXT(kovsh101, kovsh101, pgm)
STD_ROM_FN(kovsh101)

struct BurnDriver BurnDrvKovsh101 = {
	"kovsh101", "kovsh", "pgm", NULL, "1999",
	"Knights of Valour Superheroes / Sangoku Senki Superheroes (V101)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovsh101RomInfo, kovsh101RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour Superheroes / Sangoku Senki Superheroes (V100)

static struct BurnRomInfo kovsh100RomDesc[] = {
	{ "kovsh-v0100-u1.bin", 0x400000, 0xd145c1ca, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t0600.u2",     	0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a0600.u3",	   	0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a0601.u4",	   	0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "pgm_a0602.u6",	   	0x800000, 0xe7a32959, 3 | BRF_GRA },			//  4
	{ "pgm_a0603.u8",	   	0x400000, 0xec31abda, 3 | BRF_GRA },			//  5
	{ "pgm_a0604.u10",	   	0x400000, 0x26b59fd3, 3 | BRF_GRA },			//  6

	{ "pgm_b0600.u7",	   	0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b0601.u9",	   	0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			//  8
	{ "pgm_b0602.u11",	   	0x100000, 0x9df77934, 4 | BRF_GRA },			//  9

	{ "pgm_m0600.u5",	   	0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 10 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS },  // 11 Internal ARM7 Rom
};

STDROMPICKEXT(kovsh100, kovsh100, pgm)
STD_ROM_FN(kovsh100)

struct BurnDriver BurnDrvKovsh100 = {
	"kovsh100", "kovsh", "pgm", NULL, "1999",
	"Knights of Valour Superheroes / Sangoku Senki Superheroes (V100)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovsh100RomInfo, kovsh100RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Photo Y2K (ver. 105)

static struct BurnRomInfo photoy2kRomDesc[] = {
	{ "pgm_p0701_v105.u2", 	0x200000, 0xfab142e0, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t0700.u11",     	0x080000, 0x93943b4d, 2 | BRF_GRA },			//  1 Tile data
/*	PGM A700, PGM A701 and PGM B0700 ROMs needs to be redump. After checking PCB scans, it appears they are 8MB size.
	Maybe actual dumps are good and ROMs .l and .h have be joined and/or reordered.
	{ "pgm_a0700.u2",       0x800000, 0x00000000, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a0701.u4",       0x800000, 0x00000000, 3 | BRF_GRA },			//  3
	{ "pgm_a0702.u3",     	0x080000, 0x42239e1b, 3 | BRF_GRA },			//  4

	{ "pgm_b0700.u7",       0x800000, 0x00000000, 4 | BRF_GRA },			//  5 Sprite Masks & Color Indexes */
	{ "a0700.l",       		0x400000, 0x26a9ae9c, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0700.h",       		0x400000, 0x79bc1fc1, 3 | BRF_GRA },			//  3
	{ "a0701.l",       		0x400000, 0x23607f81, 3 | BRF_GRA },			//  4
	{ "a0701.h",       		0x400000, 0x5f2efd37, 3 | BRF_GRA },			//  5
	{ "pgm_a0702.u3",     	0x080000, 0x42239e1b, 3 | BRF_GRA },			//  6

	{ "b0700.l",       		0x400000, 0xaf096904, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "b0700.h",       		0x400000, 0x6d53de26, 4 | BRF_GRA },			//  8
	{ "photo_y2k_cg_v101_u6.u6",    0x020000, 0xda02ec3e, 4 | BRF_GRA },	//  9

	{ "pgm_m0700.u5",     	0x080000, 0xacc7afce, 5 | BRF_SND },			// 10 Samples

	{ "igs027a_photoy2k_v100_china.asic",	0x004000, 0x1a0b68f6, 7 | BRF_PRG | BRF_ESS },  //  8 Internal ARM7 Rom
};

STDROMPICKEXT(photoy2k, photoy2k, pgm)
STD_ROM_FN(photoy2k)

static INT32 photoy2kInit()
{
	pPgmInitCallback = pgm_decrypt_photoy2k;
	pPgmProtCallback = install_protection_asic27a_kovsh;

	nPgmAsicRegionHackAddress = 0x17b1;

	INT32 nRet = pgmInit();
	
	Arm7SetIdleLoopAddress(0x000001f4);

	return nRet;
}

struct BurnDriver BurnDrvPhotoy2k = {
	"photoy2k", NULL, "pgm", NULL, "1999",
	"Photo Y2K / Real and Fake (V105)\0", NULL, "IGS", "PolyGameMaster",
	L"Photo Y2K\0\u30EA\u30A2\u30EB\u30A2\u30F3\u30C9 \u30D5\u30A7\u30A4\u30AF\0\u5927\u5BB6\u6765 \u627E\u78B4\0\u8D85\u7EA7 \u6BD4\u4E00\u6BD4 (V105)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_PUZZLE, 0,
	NULL, photoy2kRomInfo, photoy2kRomName, NULL, NULL, NULL, NULL, pgmInputInfo, photoy2kDIPInfo,
	photoy2kInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Photo Y2K / Real and Fake (V104)

static struct BurnRomInfo photoy2k104RomDesc[] = {
	{ "v104.16m",      		0x200000, 0xe051070f, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t0700.u11",     	0x080000, 0x93943b4d, 2 | BRF_GRA },			//  1 Tile data

	{ "a0700.l",       		0x400000, 0x26a9ae9c, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0700.h",       		0x400000, 0x79bc1fc1, 3 | BRF_GRA },			//  3
	{ "a0701.l",       		0x400000, 0x23607f81, 3 | BRF_GRA },			//  4
	{ "a0701.h",       		0x400000, 0x5f2efd37, 3 | BRF_GRA },			//  5
	{ "pgm_a0702.u3",     	0x080000, 0x42239e1b, 3 | BRF_GRA },			//  6

	{ "b0700.l",       		0x400000, 0xaf096904, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "b0700.h",       		0x400000, 0x6d53de26, 4 | BRF_GRA },			//  8
	{ "photo_y2k_cg_v101_u6.u6",    0x020000, 0xda02ec3e, 4 | BRF_GRA },	//  9

	{ "pgm_m0700.u5",     	0x080000, 0xacc7afce, 5 | BRF_SND },			// 10 Samples

	{ "igs027a_photoy2k_v100_china_alt.asic",	0x004000, 0x6dd7f257, 7 | BRF_PRG | BRF_ESS },  // 11 Internal ARM7 Rom
};

STDROMPICKEXT(photoy2k104, photoy2k104, pgm)
STD_ROM_FN(photoy2k104)

struct BurnDriver BurnDrvPhotoy2k104 = {
	"photoy2k104", "photoy2k", "pgm", NULL, "1999",
	"Photo Y2K / Real and Fake (V104)\0", NULL, "IGS", "PolyGameMaster",
	L"Photo Y2K\0\u30EA\u30A2\u30EB\u30A2\u30F3\u30C9 \u30D5\u30A7\u30A4\u30AF\0\u5927\u5BB6\u6765 \u627E\u78B4\0\u8D85\u7EA7 \u6BD4\u4E00\u6BD4 (V104)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_PUZZLE, 0,
	NULL, photoy2k104RomInfo, photoy2k104RomName, NULL, NULL, NULL, NULL, pgmInputInfo, photoy2kDIPInfo,
	photoy2kInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Real and Fake / Photo Y2K (V103, Japan)

static struct BurnRomInfo photoy2k103jRomDesc[] = {
	{ "photo_y2k_v103_u4.u4",	0x080000, 0xc16dc699, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code
	{ "photo_y2k_v103_u6.u6",   0x080000, 0x2e2671a4, 1 | BRF_PRG | BRF_ESS },  //  1
	{ "photo_y2k_v103_u5.u5",   0x080000, 0x97839a61, 1 | BRF_PRG | BRF_ESS },  //  2
	{ "photo_y2k_v103_u8.u8",   0x080000, 0x43af9664, 1 | BRF_PRG | BRF_ESS },  //  3

	{ "pgm_t0700.u11",		0x080000, 0x93943b4d, 2 | BRF_GRA },				//  4 Tile data

	{ "a0700.l",       		0x400000, 0x26a9ae9c, 3 | BRF_GRA },				//  5 Sprite Color Data
	{ "a0700.h",       		0x400000, 0x79bc1fc1, 3 | BRF_GRA },				//  6
	{ "a0701.l",       		0x400000, 0x23607f81, 3 | BRF_GRA },				//  7
	{ "a0701.h",       		0x400000, 0x5f2efd37, 3 | BRF_GRA },				//  8
	{ "photo_yk2_cg_v101_u3.u3",	0x080000, 0x42239e1b, 3 | BRF_GRA },		//  9

	{ "b0700.l",       		0x400000, 0xaf096904, 4 | BRF_GRA },				// 10 Sprite Masks & Color Indexes
	{ "b0700.h",       		0x400000, 0x6d53de26, 4 | BRF_GRA },				// 11
	{ "photo_y2k_cg_v101_u6.u6",    0x020000, 0xda02ec3e, 4 | BRF_GRA },		// 12

	{ "photo_y2k_sp_v102_u5.u5",     0x080000, 0xacc7afce, 5 | BRF_SND },		// 13 Samples

	{ "igs027a_photoy2k_v100_china.asic",	0x004000, 0x1a0b68f6, 7 | BRF_PRG | BRF_ESS }, // 14 Internal ARM7 Rom
};

STDROMPICKEXT(photoy2k103j, photoy2k103j, pgm)
STD_ROM_FN(photoy2k103j)

struct BurnDriver BurnDrvphotoy2k103j = {
	"photoy2k103j", "photoy2k", "pgm", NULL, "1999",
	"Photo Y2K / Real and Fake (V103, Japan)\0", NULL, "IGS", "PolyGameMaster",
	L"\u30EA\u30A2\u30EB\u30A2\u30F3\u30C9 \u30D5\u30A7\u30A4\u30AF\0Photo Y2K\0\u5927\u5BB6\u6765 \u627E\u78B4\0\u8D85\u7EA7 \u6BD4\u4E00\u6BD4 (V103, Japan)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING  | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_PUZZLE, 0,
	NULL, photoy2k103jRomInfo, photoy2k103jRomName, NULL, NULL, NULL, NULL, pgmInputInfo, photoy2kDIPInfo,
	photoy2kInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Real and Fake / Photo Y2K (V102, Japan)

static struct BurnRomInfo photoy2k102RomDesc[] = {
	{ "photo_y2k_v102_u4.u4",	0x080000, 0xa65eda9f, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code
	{ "photo_y2k_v102_u6.u6",   0x080000, 0xb9ca5504, 1 | BRF_PRG | BRF_ESS },  //  1
	{ "photo_y2k_v102_u5.u5",   0x080000, 0x9201621b, 1 | BRF_PRG | BRF_ESS },  //  2
	{ "photo_y2k_v102_u8.u8",   0x080000, 0x3be22b8f, 1 | BRF_PRG | BRF_ESS },  //  3

	{ "pgm_t0700.u11",		0x080000, 0x93943b4d, 2 | BRF_GRA },				//  4 Tile data

	{ "a0700.l",       		0x400000, 0x26a9ae9c, 3 | BRF_GRA },				//  5 Sprite Color Data
	{ "a0700.h",       		0x400000, 0x79bc1fc1, 3 | BRF_GRA },				//  6
	{ "a0701.l",       		0x400000, 0x23607f81, 3 | BRF_GRA },				//  7
	{ "a0701.h",       		0x400000, 0x5f2efd37, 3 | BRF_GRA },				//  8
	{ "photo_yk2_cg_v101_u3.u3",	0x080000, 0x42239e1b, 3 | BRF_GRA },		//  9

	{ "b0700.l",       		0x400000, 0xaf096904, 4 | BRF_GRA },				// 10 Sprite Masks & Color Indexes
	{ "b0700.h",       		0x400000, 0x6d53de26, 4 | BRF_GRA },				// 11
	{ "photo_y2k_cg_v101_u6.u6",    0x020000, 0xda02ec3e, 4 | BRF_GRA },		// 12

	{ "photo_y2k_sp_v102_u5.u5",     0x080000, 0xacc7afce, 5 | BRF_SND },				// 13 Samples

	{ "igs027a_photoy2k_v100_china_alt.asic",	0x0004000, 0x6dd7f257, 7 | BRF_PRG | BRF_ESS }, // 14 Internal ARM7 Rom
};

STDROMPICKEXT(photoy2k102, photoy2k102, pgm)
STD_ROM_FN(photoy2k102)

struct BurnDriver BurnDrvphotoy2k102 = {
	"photoy2k102", "photoy2k", "pgm", NULL, "1999",
	"Photo Y2K / Real and Fake (V102, Japan)\0", NULL, "IGS", "PolyGameMaster",
	L"\u30EA\u30A2\u30EB\u30A2\u30F3\u30C9 \u30D5\u30A7\u30A4\u30AF\0Photo Y2K\0\u5927\u5BB6\u6765 \u627E\u78B4\0\u8D85\u7EA7 \u6BD4\u4E00\u6BD4 (V102, Japan)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING  | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_PUZZLE, 0,
	NULL, photoy2k102RomInfo, photoy2k102RomName, NULL, NULL, NULL, NULL, pgmInputInfo, photoy2kDIPInfo,
	photoy2kInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Puzzle Star (V100MG)

static struct BurnRomInfo puzlstarRomDesc[] = {
	{ "PuzzleStar_u2_v100mg.u2",	0x080000, 0x4c79d979, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code
	{ "PuzzleStar_u1_v100mg.u1",	0x080000, 0x5788b77d, 1 | BRF_PRG | BRF_ESS },  //  1

	{ "pgm_t0800.u5",	   	0x200000, 0xf9d84e59, 2 | BRF_GRA }, 	        //  2 Tile data

	{ "pgm_a0800.u1",	   	0x400000, 0xe1e6ec40, 3 | BRF_GRA },			//  3 Sprite Color Data

	{ "pgm_b0800.u3",	   	0x200000, 0x52e7bef5, 4 | BRF_GRA }, 	  		//  4 Sprite Masks & Color Indexes

	{ "pgm_m0800.u2",	   	0x400000, 0xe1a46541, 5 | BRF_SND },			//  5 Samples

	{ "puzlstar_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  6 Internal ARM7 Rom
};

STDROMPICKEXT(puzlstar, puzlstar, pgm)
STD_ROM_FN(puzlstar)

static INT32 puzlstarInit()
{
	pPgmInitCallback = pgm_decrypt_puzlstar;
	pPgmProtCallback = install_protection_asic27a_puzlstar; // simulation
//	pPgmProtCallback = install_protection_asic27a_kovsh;

	return pgmInit();
}

struct BurnDriver BurnDrvPuzlstar = {
	"puzlstar", NULL, "pgm", NULL, "1999",
	"Puzzle Star (V100MG)\0", "Incomplete dump", "IGS", "PolyGameMaster",
	L"Puzzle Star\0\u30D1\u30BA\u30EB\u30B9\u30BF\u30FC\0\u9B54\u5E7B\u661F\u5EA7 (V100MG)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_PUZZLE, 0,
	NULL, puzlstarRomInfo, puzlstarRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	puzlstarInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Puzzli 2 (V100)

static struct BurnRomInfo puzzli2RomDesc[] = {
	{ "v100_u5.u5",   		0x200000, 0x1abb4595, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t0900.u9",	   	0x200000, 0x70615611, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a0900.u3",	   	0x400000, 0x14911251, 3 | BRF_GRA },			//  2 Sprite Color Data

	{ "pgm_b0900.u4",	   	0x200000, 0x6f0638b6, 4 | BRF_GRA },			//  3 Sprite Masks & Color Indexes

	{ "pgm_m0900.u2",	   	0x400000, 0x9ea7af2e, 5 | BRF_SND },			//  4 Samples

	{ "puzzli2_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  5 Internal ARM7 Rom
};

STDROMPICKEXT(puzzli2, puzzli2, pgm)
STD_ROM_FN(puzzli2)

static INT32 puzzli2Init()
{
	nPGMDisableIRQ4 = 1;
	pPgmInitCallback = pgm_decrypt_puzzli2;
	pPgmProtCallback = install_protection_asic27a_puzzli2; // simulation
//	pPgmProtCallback = install_protection_asic27a_kovsh;

	return pgmInit();
}

struct BurnDriver BurnDrvPuzzli2 = {
	"puzzli2", NULL, "pgm", NULL, "1999",
	"Puzzli 2 (V100)\0", "Incomplete dump", "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_PUZZLE, 0,
	NULL, puzzli2RomInfo, puzzli2RomName, NULL, NULL, NULL, NULL, pgmInputInfo, puzzli2DIPInfo,
	puzzli2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Puzzli 2 Super (V200)

static struct BurnRomInfo puzzli2sRomDesc[] = {
	{ "2sp_v200.u4",   		0x080000, 0xfa5c86c1, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code
	{ "2sp_v200.u3",   		0x080000, 0x2a5ba8a6, 1 | BRF_PRG | BRF_ESS },  //  1

	{ "pgm_t0900.u9",	   	0x200000, 0x70615611, 2 | BRF_GRA },			//  2 Tile data

	{ "pgm_a0900.u3",	   	0x400000, 0x14911251, 3 | BRF_GRA },			//  3 Sprite Color Data

	{ "pgm_b0900.u4",	   	0x200000, 0x6f0638b6, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "pgm_m0900.u2",	   	0x400000, 0x9ea7af2e, 5 | BRF_SND },			//  5 Samples

	{ "puzzli2_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  6 Internal ARM7 Rom
};

STDROMPICKEXT(puzzli2s, puzzli2s, pgm)
STD_ROM_FN(puzzli2s)

struct BurnDriver BurnDrvPuzzli2s = {
	"puzzli2s", "puzzli2", "pgm", NULL, "2001",
	"Puzzli 2 Super (V200)\0", "Incomplete dump", "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_PUZZLE, 0,
	NULL, puzzli2sRomInfo, puzzli2sRomName, NULL, NULL, NULL, NULL, pgmInputInfo, puzzli2DIPInfo,
	puzzli2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Martial Masters / Xing Yi (V104, 102, 102, USA)

static struct BurnRomInfo martmastRomDesc[] = {
	{ "v104_32m.u9",   		0x400000, 0xcfd9dff4, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t1000.u3",	 	0x800000, 0xbbf879b5, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a1000.u3",    	0x800000, 0x43577ac8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a1001.u4",    	0x800000, 0xfe7a476f, 3 | BRF_GRA },			//  3
	{ "pgm_a1002.u6",    	0x800000, 0x62e33d38, 3 | BRF_GRA },			//  4
	{ "pgm_a1003.u8",    	0x800000, 0xb2c4945a, 3 | BRF_GRA },			//  5
	{ "pgm_a1004.u10",   	0x400000, 0x9fd3f5fd, 3 | BRF_GRA },			//  6

	{ "pgm_b1000.u9",	  	0x800000, 0xc5961f6f, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b1001.u11",	  	0x800000, 0x0b7e1c06, 4 | BRF_GRA },			//  8

	{ "pgm_m1000.u5",     	0x800000, 0xed407ae8, 5 | BRF_SND },			//  9 Samples
	{ "pgm_m1001.u7",      	0x400000, 0x662d2d48, 5 | BRF_SND },			// 10

	{ "martial_masters_v102_usa.asic",	0x004000, 0xa6c0828c, 7 | BRF_PRG | BRF_ESS },  // 11 Internal ARM7 Rom

	{ "v102_16m.u10",  		0x200000, 0x18b745e6, 8 | BRF_PRG | BRF_ESS },  // 12 External ARM7 Rom
};

STDROMPICKEXT(martmast, martmast, pgm)
STD_ROM_FN(martmast)

static INT32 martmastInit()
{
	pPgmInitCallback = pgm_decrypt_martmast;
	pPgmProtCallback = install_protection_asic27a_martmast;

	nPgmAsicRegionHackAddress = 0x37c1;
	
	INT32 nRet = pgmInit();
	
	Arm7SetIdleLoopAddress(0x800039e);

	return nRet;
}

struct BurnDriver BurnDrvMartmast = {
	"martmast", NULL, "pgm", NULL, "2001",
	"Martial Masters (V104, 102, 102, USA)\0", NULL, "IGS", "PolyGameMaster",
	L"Martial Masters\0\u5f62\u610f\u62f3 (V104, 102, 102, USA)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VSFIGHT, 0,
	NULL, martmastRomInfo, martmastRomName, NULL, NULL, NULL, NULL, pgmInputInfo, martmastDIPInfo,
	martmastInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Martial Masters / Xing Yi (V104, 102, 101, China)

static struct BurnRomInfo martmastcRomDesc[] = {
	{ "v104_32m.u9",   		0x400000, 0xcfd9dff4, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t1000.u3",	 	0x800000, 0xbbf879b5, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a1000.u3",    	0x800000, 0x43577ac8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a1001.u4",    	0x800000, 0xfe7a476f, 3 | BRF_GRA },			//  3
	{ "pgm_a1002.u6",    	0x800000, 0x62e33d38, 3 | BRF_GRA },			//  4
	{ "pgm_a1003.u8",    	0x800000, 0xb2c4945a, 3 | BRF_GRA },			//  5
	{ "pgm_a1004.u10",   	0x400000, 0x9fd3f5fd, 3 | BRF_GRA },			//  6

	{ "pgm_b1000.u9",	  	0x800000, 0xc5961f6f, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b1001.u11",	  	0x800000, 0x0b7e1c06, 4 | BRF_GRA },			//  8

	{ "pgm_m1000.u5",     	0x800000, 0xed407ae8, 5 | BRF_SND },			//  9 Samples
	{ "pgm_m1001.u7",      	0x400000, 0x662d2d48, 5 | BRF_SND },			// 10

	{ "martial_masters_v101_cn.asic",	0x004000, 0xb3e25b7d, 7 | BRF_PRG | BRF_ESS },  // 11 Internal ARM7 Rom

	{ "v102_16m.u10",  		0x200000, 0x18b745e6, 8 | BRF_PRG | BRF_ESS },  // 12 External ARM7 Rom
};

STDROMPICKEXT(martmastc, martmastc, pgm)
STD_ROM_FN(martmastc)

static INT32 martmastcInit()
{
	pPgmInitCallback = pgm_decrypt_martmast;
	pPgmProtCallback = install_protection_asic27a_martmast;

	nPgmAsicRegionHackAddress = 0x2cb5;
	
	INT32 nRet = pgmInit();
	
	Arm7SetIdleLoopAddress(0x800039e);

	return nRet;
}

struct BurnDriver BurnDrvmartmastc = {
	"martmastc", "martmast", "pgm", NULL, "2001",
	"Martial Masters (V104, 102, 101, China)\0", NULL, "IGS", "PolyGameMaster",
	L"Martial Masters\0\u5f62\u610f\u62f3 (V104, 102, 101, China)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VSFIGHT, 0,
	NULL, martmastcRomInfo, martmastcRomName, NULL, NULL, NULL, NULL, pgmInputInfo, martmastDIPInfo,
	martmastcInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Martial Masters / Xing Yi (V103, 102, 101, China)
// Needs proper martial_masters_v102_cn.asic to be dumped

static struct BurnRomInfo martmastc103RomDesc[] = {
	{ "v103_32m.u9",   		0x400000, 0xdf5ffbe9, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t1000.u3",	 	0x800000, 0xbbf879b5, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a1000.u3",    	0x800000, 0x43577ac8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a1001.u4",    	0x800000, 0xfe7a476f, 3 | BRF_GRA },			//  3
	{ "pgm_a1002.u6",    	0x800000, 0x62e33d38, 3 | BRF_GRA },			//  4
	{ "pgm_a1003.u8",    	0x800000, 0xb2c4945a, 3 | BRF_GRA },			//  5
	{ "pgm_a1004.u10",   	0x400000, 0x9fd3f5fd, 3 | BRF_GRA },			//  6

	{ "pgm_b1000.u9",	  	0x800000, 0xc5961f6f, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b1001.u11",	  	0x800000, 0x0b7e1c06, 4 | BRF_GRA },			//  8

	{ "pgm_m1000.u5",     	0x800000, 0xed407ae8, 5 | BRF_SND },			//  9 Samples
	{ "pgm_m1001.u7",      	0x400000, 0x662d2d48, 5 | BRF_SND },			// 10
	
	{ "martial_masters_v101_cn.asic",	0x004000, 0xb3e25b7d, 7 | BRF_PRG | BRF_ESS },  // 11 Internal ARM7 Rom

	{ "v102_16m.u10",  		0x200000, 0x18b745e6, 8 | BRF_PRG | BRF_ESS },  // 12 External ARM7 Rom
};

STDROMPICKEXT(martmastc103, martmastc103, pgm)
STD_ROM_FN(martmastc103)

struct BurnDriver BurnDrvmartmastc103 = {
	"martmastc103", "martmast", "pgm", NULL, "2001",
	"Martial Masters (V103, 102, 101, China)\0", NULL, "IGS", "PolyGameMaster",
	L"Martial Masters\0\u5f62\u610f\u62f3 (V103, 102, 101, China)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VSFIGHT, 0,
	NULL, martmastc103RomInfo, martmastc103RomName, NULL, NULL, NULL, NULL, pgmInputInfo, martmastc102DIPInfo,
	martmastInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Martial Masters / Xing Yi (V102, 101, 101, China)

static struct BurnRomInfo martmastc102RomDesc[] = {
	{ "martmast_u9-v102.322",	0x400000, 0xbb24b92a, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t1000.u3",	 	0x800000, 0xbbf879b5, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a1000.u3",    	0x800000, 0x43577ac8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a1001.u4",    	0x800000, 0xfe7a476f, 3 | BRF_GRA },			//  3
	{ "pgm_a1002.u6",    	0x800000, 0x62e33d38, 3 | BRF_GRA },			//  4
	{ "pgm_a1003.u8",    	0x800000, 0xb2c4945a, 3 | BRF_GRA },			//  5
	{ "pgm_a1004.u10",   	0x400000, 0x9fd3f5fd, 3 | BRF_GRA },			//  6

	{ "pgm_b1000.u9",	  	0x800000, 0xc5961f6f, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b1001.u11",	  	0x800000, 0x0b7e1c06, 4 | BRF_GRA },			//  8

	{ "pgm_m1000.u5",     	0x800000, 0xed407ae8, 5 | BRF_SND },			//  9 Samples
	{ "pgm_m1001.u7",      	0x400000, 0x662d2d48, 5 | BRF_SND },			// 10

	{ "martial_masters_v101_cn.asic",	0x004000, 0xb3e25b7d, 7 | BRF_PRG | BRF_ESS },  // 11 Internal ARM7 Rom

	{ "martmast_u10-v101.160",	0x200000, 0xd5d93294, 8 | BRF_PRG | BRF_ESS },  // 12 External ARM7 Rom
};

STDROMPICKEXT(martmastc102, martmastc102, pgm)
STD_ROM_FN(martmastc102)

struct BurnDriver BurnDrvmartmastc102 = {
	"martmastc102", "martmast", "pgm", NULL, "2001",
	"Martial Masters (V102, 101, 101, China)\0", NULL, "IGS", "PolyGameMaster",
	L"Martial Masters\0\u5f62\u610f\u62f3 (V102, 101, 101, China)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VSFIGHT, 0,
	NULL, martmastc102RomInfo, martmastc102RomName, NULL, NULL, NULL, NULL, pgmInputInfo, martmastc102DIPInfo,
	martmastcInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Martial Masters / Xing Yi (V102, 101, 101, Taiwan)
// Only the ROMs at U9 and U10 were dumped. 
// Internal ROM martial_masters_v101_tw.asic was not dumped

static struct BurnRomInfo martmasttwRomDesc[] = {
	{ "mm_v102_u9.bin",   	0x400000, 0x7eb41ed4, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t1000.u3",	 	0x800000, 0xbbf879b5, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a1000.u3",    	0x800000, 0x43577ac8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a1001.u4",    	0x800000, 0xfe7a476f, 3 | BRF_GRA },			//  3
	{ "pgm_a1002.u6",    	0x800000, 0x62e33d38, 3 | BRF_GRA },			//  4
	{ "pgm_a1003.u8",    	0x800000, 0xb2c4945a, 3 | BRF_GRA },			//  5
	{ "pgm_a1004.u10",   	0x400000, 0x9fd3f5fd, 3 | BRF_GRA },			//  6

	{ "pgm_b1000.u9",	  	0x800000, 0xc5961f6f, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b1001.u11",	  	0x800000, 0x0b7e1c06, 4 | BRF_GRA },			//  8

	{ "pgm_m1000.u5",     	0x800000, 0xed407ae8, 5 | BRF_SND },			//  9 Samples
	{ "pgm_m1001.u7",      	0x400000, 0x662d2d48, 5 | BRF_SND },			// 10
	
	{ "martial_masters_v101_tw.asic",	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  // 11 Internal ARM7 Rom

	// double size wrt to other sets
	{ "mm_v101_u10.bin",  	0x400000, 0x917beb91, 8 | BRF_PRG | BRF_ESS },  // 12 External ARM7 Rom
};

STDROMPICKEXT(martmasttw, martmasttw, pgm)
STD_ROM_FN(martmasttw)

struct BurnDriver BurnDrvmartmasttw = {
	"martmasttw", "martmast", "pgm", NULL, "2001",
	"Martial Masters (V102, 101, 101, Taiwan)\0", NULL, "IGS", "PolyGameMaster",
	L"Martial Masters\0\u5f62\u610f\u62f3 (V102, 101, 101, Taiwan)\0", NULL, NULL, NULL,
	BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VSFIGHT, 0,
	NULL, martmasttwRomInfo, martmasttwRomName, NULL, NULL, NULL, NULL, pgmInputInfo, martmasttwDIPInfo,
	martmastInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Photo Y2K 2 (VM101XX, Taiwan)

static struct BurnRomInfo py2k2RomDesc[] = {
	{ "m-101xx.u1",			0x200000, 0xc47795f1, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_a1100.u6", 		0x800000, 0xe32ce499, 3 | BRF_GRA },			//  1 Sprite Color Data
	{ "pgm_a1101.u7", 		0x800000, 0x4e7568bc, 3 | BRF_GRA },			//  2
	{ "pgm_a1102.u8", 		0x800000, 0x6da7c143, 3 | BRF_GRA },			//  3
	{ "pgm_a1103.u9", 		0x800000, 0x0ebebfdc, 3 | BRF_GRA },			//  4

	{ "pgm_b1100.u4", 		0x800000, 0xfa53d6f6, 4 | BRF_GRA },			//  5 Sprite Masks & Color Indexes
	{ "pgm_b1101.u5", 		0x800000, 0x001e4c81, 4 | BRF_GRA },			//  6

	{ "pgm_m1100.u3", 		0x200000, 0xfb1515f8, 5 | BRF_SND },			//  7 Samples

	{ "igs027a_photoy2k2.asic",	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	//  8 Internal ARM7 Rom
};

STDROMPICKEXT(py2k2, py2k2, pgm)
STD_ROM_FN(py2k2)

static INT32 py2k2Init()
{
	pPgmInitCallback = pgm_decrypt_py2k2;
	pPgmProtCallback = install_protection_asic27a_py2k2;
//	pPgmProtCallback = install_protection_asic27a_kovsh;

	return pgmInit();
}

struct BurnDriverD BurnDrvPy2k2 = {
	"py2k2", NULL, "pgm", NULL, "2001",
	"Photo Y2K 2 (VM101XX, Taiwan)\0", "Incomplete dump", "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_PUZZLE, 0,
	NULL, py2k2RomInfo, py2k2RomName, NULL, NULL, NULL, NULL, pgmInputInfo, py2k2DIPInfo,
	py2k2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Photo Y2K 2 (V100, Japan)

static struct BurnRomInfo py2k2100RomDesc[] = {
	{ "y2kii_v100.u1",		0x100000, 0x7a1e36ac, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_a1100.u6", 		0x800000, 0xe32ce499, 3 | BRF_GRA },			//  1 Sprite Color Data
	{ "pgm_a1101.u7", 		0x800000, 0x4e7568bc, 3 | BRF_GRA },			//  2
	{ "pgm_a1102.u8", 		0x800000, 0x6da7c143, 3 | BRF_GRA },			//  3
	{ "pgm_a1103.u9", 		0x800000, 0x0ebebfdc, 3 | BRF_GRA },			//  4

	{ "pgm_b1100.u4", 		0x800000, 0xfa53d6f6, 4 | BRF_GRA },			//  5 Sprite Masks & Color Indexes
	{ "pgm_b1101.u5", 		0x800000, 0x001e4c81, 4 | BRF_GRA },			//  6

	{ "pgm_m1100.u3", 		0x200000, 0xfb1515f8, 5 | BRF_SND },			//  7 Samples

	{ "igs027a_photoy2k2.asic",	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	//  8 Internal ARM7 Rom
};

STDROMPICKEXT(py2k2100, py2k2100, pgm)
STD_ROM_FN(py2k2100)

struct BurnDriverD BurnDrvPy2k2100 = {
	"py2k2100", "py2k2", "pgm", NULL, "2001",
	"Photo Y2K 2 (V100, japan)\0", "Incomplete dump", "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_PUZZLE, 0,
	NULL, py2k2100RomInfo, py2k2100RomName, NULL, NULL, NULL, NULL, pgmInputInfo, py2k2DIPInfo,
	py2k2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour 2 (V107, 102, 100, Hong Kong)

static struct BurnRomInfo kov2RomDesc[] = {
	{ "v107_u18.u18",	  		0x400000, 0x661a5b2c, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t1200.u21",	   	0x800000, 0xd7e26609, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a1200.u1",	   	0x800000, 0xceeb81d8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a1201.u4",   	0x800000, 0x21063ca7, 3 | BRF_GRA },			//  3
	{ "pgm_a1202.u6",	   	0x800000, 0x4bb92fae, 3 | BRF_GRA },			//  4
	{ "pgm_a1203.u8",	   	0x800000, 0xe73cb627, 3 | BRF_GRA },			//  5
	{ "pgm_a1204.u10",   	0x200000, 0x14b4b5bb, 3 | BRF_GRA },			//  6

	{ "pgm_b1200.u5",	   	0x800000, 0xbed7d994, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b1201.u7",	   	0x800000, 0xf251eb57, 4 | BRF_GRA },			//  8

	{ "pgm_m1200.u3",	   	0x800000, 0xb0d88720, 5 | BRF_SND },			//  9 Samples

	{ "kov2_v100_hongkong.asic", 	0x004000, 0xe0d7679f, 7 | BRF_PRG | BRF_ESS },  // 10 Internal ARM7 Rom

	{ "v102_u19.u19",	   		0x200000, 0x462e2980, 8 | BRF_PRG | BRF_ESS },  // 11 External ARM7 Rom
};

STDROMPICKEXT(kov2, kov2, pgm)
STD_ROM_FN(kov2)

static INT32 kov2Init()
{
	pPgmInitCallback = pgm_decrypt_kov2;
	pPgmProtCallback = install_protection_asic27a_martmast;

	nPgmAsicRegionHackAddress = 0x23e5;

	INT32 nRet = pgmInit();
	
	Arm7SetIdleLoopAddress(0x80000ae);

	return nRet;
}

struct BurnDriver BurnDrvKov2 = {
	"kov2", NULL, "pgm", NULL, "2000",
	"Knights of Valour 2 (V107, 102, 100, Hong Kong)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour 2\0\u4e09\u56fd\u6218\u7eaa 2 (V107, 102, 100, Hong Kong)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kov2RomInfo, kov2RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kov2DIPInfo,
	kov2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour 2 (V106, 102, 100 Hong Kong)

static struct BurnRomInfo kov2106RomDesc[] = {
	{ "v106_u18.u18",	   	0x400000, 0x40051ad9, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t1200.u21",	   	0x800000, 0xd7e26609, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a1200.u1",	   	0x800000, 0xceeb81d8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a1201.u4",   	0x800000, 0x21063ca7, 3 | BRF_GRA },			//  3
	{ "pgm_a1202.u6",	   	0x800000, 0x4bb92fae, 3 | BRF_GRA },			//  4
	{ "pgm_a1203.u8",	   	0x800000, 0xe73cb627, 3 | BRF_GRA },			//  5
	{ "pgm_a1204.u10",   	0x200000, 0x14b4b5bb, 3 | BRF_GRA },			//  6

	{ "pgm_b1200.u5",	   	0x800000, 0xbed7d994, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b1201.u7",	   	0x800000, 0xf251eb57, 4 | BRF_GRA },			//  8

	{ "pgm_m1200.u3",	   	0x800000, 0xb0d88720, 5 | BRF_SND },			//  9 Samples

	{ "kov2_v100_hongkong.asic", 	0x004000, 0xe0d7679f, 7 | BRF_PRG | BRF_ESS },  // 10 Internal ARM7 Rom

	{ "v102_u19.u19",	   	0x200000, 0x462e2980, 8 | BRF_PRG | BRF_ESS },  // 11 External ARM7 Rom
};

STDROMPICKEXT(kov2106, kov2106, pgm)
STD_ROM_FN(kov2106)

struct BurnDriver BurnDrvKov2106 = {
	"kov2106", "kov2", "pgm", NULL, "2000",
	"Knights of Valour 2 (V106, 102, 100 Hong Kong)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour 2\0\u4e09\u56fd\u6218\u7eaa 2 (V106, 102, 100 Hong Kong)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kov2106RomInfo, kov2106RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kov2DIPInfo,
	kov2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour 2 (V104, 102, 100, Hong Kong)

static struct BurnRomInfo kov2104RomDesc[] = {
	{ "v104_u18.u18",		0x400000, 0x0fdb050b, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t1200.u27",		0x800000, 0xd7e26609, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a1200.u1",	   	0x800000, 0xceeb81d8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a1201.u4",   	0x800000, 0x21063ca7, 3 | BRF_GRA },			//  3
	{ "pgm_a1202.u6",	   	0x800000, 0x4bb92fae, 3 | BRF_GRA },			//  4
	{ "pgm_a1203.u8",	   	0x800000, 0xe73cb627, 3 | BRF_GRA },			//  5
	{ "pgm_a1204.u10",   	0x200000, 0x14b4b5bb, 3 | BRF_GRA },			//  6

	{ "pgm_b1200.u5",		0x800000, 0xbed7d994, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b1201.u7",		0x800000, 0xf251eb57, 4 | BRF_GRA },			//  8

	{ "pgm_m1200.u3",		0x800000, 0xb0d88720, 5 | BRF_SND },			//  9 Samples

	{ "kov2_v100_hongkong.asic",	0x004000, 0xe0d7679f, 7 | BRF_PRG | BRF_ESS },  // 10 Internal ARM7 Rom

	{ "v102_u19.u19",		0x200000, 0x462e2980, 8 | BRF_PRG | BRF_ESS },  // 11 External ARM7 Rom
};

STDROMPICKEXT(kov2104, kov2104, pgm)
STD_ROM_FN(kov2104)

struct BurnDriver BurnDrvKov2104 = {
	"kov2104", "kov2", "pgm", NULL, "2000",
	"Knights of Valour 2 (V104, 102, 100, Hong Kong)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour 2\0\u4e09\u56fd\u6218\u7eaa 2 (V104, 102, 100, Hong Kong)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kov2104RomInfo, kov2104RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kov2DIPInfo,
	kov2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour 2 (V103, 101, 100, Hong Kong)

static struct BurnRomInfo kov2103RomDesc[] = {
	{ "v103_u18.u18",		0x400000, 0x98c32f76, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t1200.u21",	   	0x800000, 0xd7e26609, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a1200.u1",	   	0x800000, 0xceeb81d8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a1201.u4",   	0x800000, 0x21063ca7, 3 | BRF_GRA },			//  3
	{ "pgm_a1202.u6",	   	0x800000, 0x4bb92fae, 3 | BRF_GRA },			//  4
	{ "pgm_a1203.u8",	   	0x800000, 0xe73cb627, 3 | BRF_GRA },			//  5
	{ "pgm_a1204.u10",   	0x200000, 0x14b4b5bb, 3 | BRF_GRA },			//  6

	{ "pgm_b1200.u5",	   	0x800000, 0xbed7d994, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b1201.u7",	   	0x800000, 0xf251eb57, 4 | BRF_GRA },			//  8

	{ "pgm_m1200.u3",	   	0x800000, 0xb0d88720, 5 | BRF_SND },			//  9 Samples

	{ "kov2_v100_hongkong.asic",	0x004000, 0xe0d7679f, 7 | BRF_PRG | BRF_ESS },  // 10 Internal ARM7 Rom

	{ "v101_u19.u19",		0x200000, 0x8c35f2fe, 8 | BRF_PRG | BRF_ESS },  // 11 External ARM7 Rom
};

STDROMPICKEXT(kov2103, kov2103, pgm)
STD_ROM_FN(kov2103)

struct BurnDriver BurnDrvKov2103 = {
	"kov2103", "kov2", "pgm", NULL, "2000",
	"Knights of Valour 2 (V103, 101, 100, Hong Kong)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour 2\0\u4e09\u56fd\u6218\u7eaa 2 (V103, 101, 100, Hong Kong)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kov2103RomInfo, kov2103RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kov2DIPInfo,
	kov2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour 2 (V102, 101, 100, Hong Kong)

static struct BurnRomInfo kov2102RomDesc[] = {
	{ "v102_u18.u18",	   	0x400000, 0xa2489c37, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t1200.u21",	   	0x800000, 0xd7e26609, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a1200.u1",	   	0x800000, 0xceeb81d8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a1201.u4",   	0x800000, 0x21063ca7, 3 | BRF_GRA },			//  3
	{ "pgm_a1202.u6",	   	0x800000, 0x4bb92fae, 3 | BRF_GRA },			//  4
	{ "pgm_a1203.u8",	   	0x800000, 0xe73cb627, 3 | BRF_GRA },			//  5
	{ "pgm_a1204.u10",   	0x200000, 0x14b4b5bb, 3 | BRF_GRA },			//  6

	{ "pgm_b1200.u5",	   	0x800000, 0xbed7d994, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b1201.u7",	   	0x800000, 0xf251eb57, 4 | BRF_GRA },			//  8

	{ "pgm_m1200.u3",	   	0x800000, 0xb0d88720, 5 | BRF_SND },			//  9 Samples

	{ "kov2_v100_hongkong.asic",	0x004000, 0xe0d7679f, 7 | BRF_PRG | BRF_ESS },  // 10 Internal ARM7 Rom

	{ "v101_u19.u19",	   	0x200000, 0x8c35f2fe, 8 | BRF_PRG | BRF_ESS },  // 11 External ARM7 Rom
};

STDROMPICKEXT(kov2102, kov2102, pgm)
STD_ROM_FN(kov2102)

struct BurnDriver BurnDrvKov2102 = {
	"kov2102", "kov2", "pgm", NULL, "2000",
	"Knights of Valour 2 (V102, 101, 100, Hong Kong)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour 2\0\u4e09\u56fd\u6218\u7eaa 2 (V102, 101, 100, Hong Kong)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kov2102RomInfo, kov2102RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kov2DIPInfo,
	kov2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour 2 (V101, 101, 100, Hong Kong)

static struct BurnRomInfo kov2101RomDesc[] = {
	{ "v101_u18.u18",	   	0x400000, 0xc9926f35, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t1200.u21",	   	0x800000, 0xd7e26609, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a1200.u1",	   	0x800000, 0xceeb81d8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a1201.u4",   	0x800000, 0x21063ca7, 3 | BRF_GRA },			//  3
	{ "pgm_a1202.u6",	   	0x800000, 0x4bb92fae, 3 | BRF_GRA },			//  4
	{ "pgm_a1203.u8",	   	0x800000, 0xe73cb627, 3 | BRF_GRA },			//  5
	{ "pgm_a1204.u10",   	0x200000, 0x14b4b5bb, 3 | BRF_GRA },			//  6

	{ "pgm_b1200.u5",	   	0x800000, 0xbed7d994, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b1201.u7",	   	0x800000, 0xf251eb57, 4 | BRF_GRA },			//  8

	{ "pgm_m1200.u3",	   	0x800000, 0xb0d88720, 5 | BRF_SND },			//  9 Samples

	{ "kov2_v100_hongkong.asic",	0x004000, 0xe0d7679f, 7 | BRF_PRG | BRF_ESS },  // 10 Internal ARM7 Rom

	{ "v101_u19.u19",	   	0x200000, 0x8c35f2fe, 8 | BRF_PRG | BRF_ESS },  // 11 External ARM7 Rom
};

STDROMPICKEXT(kov2101, kov2101, pgm)
STD_ROM_FN(kov2101)

struct BurnDriver BurnDrvKov2101 = {
	"kov2101", "kov2", "pgm", NULL, "2000",
	"Knights of Valour 2 (V101, 101, 100, Hong Kong)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour 2\0\u4e09\u56fd\u6218\u7eaa 2 (V101, 101, 100, Hong Kong)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kov2101RomInfo, kov2101RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kov2DIPInfo,
	kov2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour 2 (V100, 100, 100, Hong Kong)

static struct BurnRomInfo kov2100RomDesc[] = {
	{ "v100_u18.u18",   	0x400000, 0x86205879, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t1200.u21",	   	0x800000, 0xd7e26609, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a1200.u1",	   	0x800000, 0xceeb81d8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a1201.u4",   	0x800000, 0x21063ca7, 3 | BRF_GRA },			//  3
	{ "pgm_a1202.u6",	   	0x800000, 0x4bb92fae, 3 | BRF_GRA },			//  4
	{ "pgm_a1203.u8",	   	0x800000, 0xe73cb627, 3 | BRF_GRA },			//  5
	{ "pgm_a1204.u10",   	0x200000, 0x14b4b5bb, 3 | BRF_GRA },			//  6

	{ "pgm_b1200.u5",	   	0x800000, 0xbed7d994, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b1201.u7",	   	0x800000, 0xf251eb57, 4 | BRF_GRA },			//  8

	{ "pgm_m1200.u3",	   	0x800000, 0xb0d88720, 5 | BRF_SND },			//  9 Samples

	{ "kov2_v100_hongkong.asic",	0x004000, 0xe0d7679f, 7 | BRF_PRG | BRF_ESS },  // 10 Internal ARM7 Rom

	{ "v100_u19.u19",	   	0x200000, 0xedd59922, 8 | BRF_PRG | BRF_ESS },  // 11 External ARM7 Rom
};

STDROMPICKEXT(kov2100, kov2100, pgm)
STD_ROM_FN(kov2100)

struct BurnDriver BurnDrvKov2100 = {
	"kov2100", "kov2", "pgm", NULL, "2000",
	"Knights of Valour 2 (V100, 100, 100, Hong Kong)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour 2\0\u4e09\u56fd\u6218\u7eaa 2 (V100, 100, 100, Hong Kong)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kov2100RomInfo, kov2100RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kov2DIPInfo,
	kov2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour 2 Plus - Nine Dragons (VM205XX, China)

static struct BurnRomInfo kov2pRomDesc[] = {
	{ "v205_32m.u8",  		0x400000, 0x3a2cc0de, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t1200.u21",	   	0x800000, 0xd7e26609, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a1200.u1",	   	0x800000, 0xceeb81d8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a1201.u4",   	0x800000, 0x21063ca7, 3 | BRF_GRA },			//  3
	{ "pgm_a1202.u6",	   	0x800000, 0x4bb92fae, 3 | BRF_GRA },			//  4
	{ "pgm_a1203.u8",	   	0x800000, 0xe73cb627, 3 | BRF_GRA },			//  5
	{ "pgm_a1204.u10",   	0x200000, 0x14b4b5bb, 3 | BRF_GRA },			//  6

	{ "pgm_b1200.u5",	   	0x800000, 0xbed7d994, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b1201.u7",	   	0x800000, 0xf251eb57, 4 | BRF_GRA },			//  8

	{ "pgm_m1200.u3",	   	0x800000, 0xb0d88720, 5 | BRF_SND },			//  9 Samples

	{ "kov2p_igs027a_china.bin",	0x004000, 0x19a0bd95, 7 | BRF_PRG | BRF_ESS },  // 10 Internal ARM7 Rom

	{ "v200_16m.u23",   	0x200000, 0x16a0c11f, 8 | BRF_PRG | BRF_ESS },  // 11 External ARM7 Rom
};

STDROMPICKEXT(kov2p, kov2p, pgm)
STD_ROM_FN(kov2p)

static INT32 kov2pInit()
{
	pPgmInitCallback = pgm_decrypt_kov2p;
	pPgmProtCallback = install_protection_asic27a_martmast;

	nPgmAsicRegionHackAddress = 0x26ed;

	INT32 nRet = pgmInit();
	
	Arm7SetIdleLoopAddress(0x80000a6);

	return nRet;
}

struct BurnDriver BurnDrvKov2p = {
	"kov2p", NULL, "pgm", NULL, "2001",
	"Knights of Valour 2 Plus - Nine Dragons (VM205XX, China)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour 2 Plus\0\u4e09\u56fd\u6218\u7eaa 2 - \u7fa4\u96c4\u4e89\u9738 (VM205XX, China)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kov2pRomInfo, kov2pRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kov2pInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour 2 Plus - Nine Dragons (VM204XX)

static struct BurnRomInfo kov2p204RomDesc[] = {
	{ "v204_32m.u8",  		0x400000, 0x583e0650, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t1200.u21",	   	0x800000, 0xd7e26609, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a1200.u1",	   	0x800000, 0xceeb81d8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a1201.u4",   	0x800000, 0x21063ca7, 3 | BRF_GRA },			//  3
	{ "pgm_a1202.u6",	   	0x800000, 0x4bb92fae, 3 | BRF_GRA },			//  4
	{ "pgm_a1203.u8",	   	0x800000, 0xe73cb627, 3 | BRF_GRA },			//  5
	{ "pgm_a1204.u10",   	0x200000, 0x14b4b5bb, 3 | BRF_GRA },			//  6

	{ "pgm_b1200.u5",	   	0x800000, 0xbed7d994, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b1201.u7",	   	0x800000, 0xf251eb57, 4 | BRF_GRA },			//  8

	{ "pgm_m1200.u3",	   	0x800000, 0xb0d88720, 5 | BRF_SND },			//  9 Samples

	{ "kov2p_igs027a_china.bin",	0x004000, 0x19a0bd95, 7 | BRF_PRG | BRF_ESS },  // 10 Internal ARM7 Rom

	{ "v200_16m.u23",		0x200000, 0x16a0c11f, 8 | BRF_PRG | BRF_ESS },  // 11 External ARM7 Rom
};

STDROMPICKEXT(kov2p204, kov2p204, pgm)
STD_ROM_FN(kov2p204)

struct BurnDriver BurnDrvKov2p204 = {
	"kov2p204", "kov2p", "pgm", NULL, "2001",
	"Knights of Valour 2 Plus - Nine Dragons (VM204XX)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour 2 Plus\0\u4e09\u56fd\u6218\u7eaa 2 - \u7fa4\u96c4\u4e89\u9738 (VM204XX)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kov2p204RomInfo, kov2p204RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kov2pInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour 2 Plus - Nine Dragons (VM202XX)

static struct BurnRomInfo kov2p202RomDesc[] = {
	{ "v202_32m.u8",  		0x400000, 0xe9b5aa0c, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t1200.u21",	   	0x800000, 0xd7e26609, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a1200.u1",	   	0x800000, 0xceeb81d8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a1201.u4",   	0x800000, 0x21063ca7, 3 | BRF_GRA },			//  3
	{ "pgm_a1202.u6",	   	0x800000, 0x4bb92fae, 3 | BRF_GRA },			//  4
	{ "pgm_a1203.u8",	   	0x800000, 0xe73cb627, 3 | BRF_GRA },			//  5
	{ "pgm_a1204.u10",   	0x200000, 0x14b4b5bb, 3 | BRF_GRA },			//  6

	{ "pgm_b1200.u5",	   	0x800000, 0xbed7d994, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b1201.u7",	   	0x800000, 0xf251eb57, 4 | BRF_GRA },			//  8

	{ "pgm_m1200.u3",	   	0x800000, 0xb0d88720, 5 | BRF_SND },			//  9 Samples

	{ "kov2p_igs027a_china.bin",	0x004000, 0x19a0bd95, 7 | BRF_PRG | BRF_ESS },  // 10 Internal ARM7 Rom

	{ "v200_16m.u23",		0x200000, 0x16a0c11f, 8 | BRF_PRG | BRF_ESS },  // 11 External ARM7 Rom
};

STDROMPICKEXT(kov2p202, kov2p202, pgm)
STD_ROM_FN(kov2p202)

struct BurnDriver BurnDrvKov2p202 = {
	"kov2p202", "kov2p", "pgm", NULL, "2001",
	"Knights of Valour 2 Plus - Nine Dragons (VM202XX)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour 2 Plus\0\u4e09\u56fd\u6218\u7eaa 2 - \u7fa4\u96c4\u4e89\u9738 (VM202XX)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kov2p202RomInfo, kov2p202RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kov2pInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour 2 Plus - Nine Dragons (VM200XX)

static struct BurnRomInfo kov2p200RomDesc[] = {
	{ "v200_32m.u8",   	    0x400000, 0x9a09fd61, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t1200.u21",	   	0x800000, 0xd7e26609, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a1200.u1",	   	0x800000, 0xceeb81d8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a1201.u4",   	0x800000, 0x21063ca7, 3 | BRF_GRA },			//  3
	{ "pgm_a1202.u6",	   	0x800000, 0x4bb92fae, 3 | BRF_GRA },			//  4
	{ "pgm_a1203.u8",	   	0x800000, 0xe73cb627, 3 | BRF_GRA },			//  5
	{ "pgm_a1204.u10",   	0x200000, 0x14b4b5bb, 3 | BRF_GRA },			//  6

	{ "pgm_b1200.u5",	   	0x800000, 0xbed7d994, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b1201.u7",	   	0x800000, 0xf251eb57, 4 | BRF_GRA },			//  8

	{ "pgm_m1200.u3",	   	0x800000, 0xb0d88720, 5 | BRF_SND },				//  9 Samples

	{ "kov2p_igs027a_china.bin",	0x004000, 0x19a0bd95, 7 | BRF_PRG | BRF_ESS },  // 10 Internal ARM7 Rom

	{ "v200_16m.u23",		0x200000, 0x16a0c11f, 8 | BRF_PRG | BRF_ESS },  // 11 External ARM7 Rom
};

STDROMPICKEXT(kov2p200, kov2p200, pgm)
STD_ROM_FN(kov2p200)

struct BurnDriver BurnDrvKov2p200 = {
	"kov2p200", "kov2p", "pgm", NULL, "2001",
	"Knights of Valour 2 Plus - Nine Dragons (VM200XX)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour 2 Plus\0\u4e09\u56fd\u6218\u7eaa 2 - \u7fa4\u96c4\u4e89\u9738 (VM200XX)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kov2p200RomInfo, kov2p200RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kov2pInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Bee Storm - DoDonPachi II definitions

#define DDP2_ASIC027_CHINA \
	{ "ddp2_igs027a_china.bin",		0x004000, 0x8c566319, 7 | BRF_PRG | BRF_ESS },

#define DDP2_ASIC027_TAIWAN \
	{ "ddp2_igs027a_taiwan.bin",	0x004000, 0x1dd34bdc, 7 | BRF_PRG | BRF_ESS },

#define DDP2_ASIC027_JAPAN \
	{ "ddp2_igs027a_japan.bin",		0x004000, 0x742d34d2, 7 | BRF_PRG | BRF_ESS },

#define DDP2_ASIC027_KOREA \
	{ "ddp2_igs027a_korea.bin",		0x004000, 0xe5a81c17, 7 | BRF_PRG | BRF_ESS },

#define DDP2_ASIC027_HONG_KONG \
	{ "ddp2_igs027a_hong_kong.bin",	0x004000, 0xa7d1cace, 7 | BRF_PRG | BRF_ESS },

#define DDP2_ASIC027_WORLD \
	{ "ddp2_igs027a_world.bin",		0x004000, 0x3654e20b, 7 | BRF_PRG | BRF_ESS },
	
#define DDP2_COMMON_ROMS \
	{ "pgm_t1300.u21",	   	0x800000, 0xe748f0cb, 2 | BRF_GRA },			\
																			\
	{ "pgm_a1300.u1",	   	0x800000, 0xfc87a405, 3 | BRF_GRA },			\
	{ "pgm_a1301.u2",	   	0x800000, 0x0c8520da, 3 | BRF_GRA },			\
																			\
	{ "pgm_b1300.u7",	   	0x800000, 0xef646604, 4 | BRF_GRA },			\
																			\
	{ "pgm_m1300.u5",	   	0x400000, 0x82d4015d, 5 | BRF_SND },			\
																			\
	{ "v100_210.u23", 	   	0x020000, 0x06c3dd29, 8 | BRF_PRG | BRF_ESS },
	
#define DDP2_PROGRAM_102 \
	{ "v102.u8",       		0x200000, 0x5a9ea040, 1 | BRF_PRG | BRF_ESS },

#define DDP2_PROGRAM_101 \
	{ "v101_16m.u8",   		0x200000, 0x5e5786fd, 1 | BRF_PRG | BRF_ESS },

#define DDP2_PROGRAM_100 \
	{ "v100.u8",	   		0x200000, 0x0c8aa8ea, 1 | BRF_PRG | BRF_ESS },

// Bee Storm - DoDonPachi II (V102, World)

static struct BurnRomInfo ddp2RomDesc[] = {
	DDP2_PROGRAM_102
	DDP2_ASIC027_WORLD
	DDP2_COMMON_ROMS
};

STDROMPICKEXT(ddp2, ddp2, pgm)
STD_ROM_FN(ddp2)

static INT32 ddp2Init()
{
	pPgmInitCallback = pgm_decrypt_ddp2;
	pPgmProtCallback = install_protection_asic27a_martmast;

//	nPgmAsicRegionHackAddress = 0x2882; // 2883?

	INT32 nRet = pgmInit();

	if (!nRet) {
		Arm7SetIdleLoopAddress(0x8010998);
	}

	return nRet;
}

struct BurnDriver BurnDrvDdp2 = {
	"ddp2", NULL, "pgm", NULL, "2001",
	"Bee Storm - DoDonPachi II (V102, World)\0", NULL, "IGS", "PolyGameMaster",
	L"Bee Storm - DoDonPachi II\0\u6012\u9996\u9886\u8702\uff0d\u8702\u66b4 (V102, World)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp2RomInfo, ddp2RomName, NULL, NULL, NULL, NULL, pgmInputInfo, ddp2DIPInfo,
	ddp2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224,448,3,4
};


// Bee Storm - DoDonPachi II (V101, World)

static struct BurnRomInfo ddp2101RomDesc[] = {
	DDP2_PROGRAM_101
	DDP2_ASIC027_WORLD
	DDP2_COMMON_ROMS
};

STDROMPICKEXT(ddp2101, ddp2101, pgm)
STD_ROM_FN(ddp2101)

struct BurnDriver BurnDrvDdp2101 = {
	"ddp2101", "ddp2", "pgm", NULL, "2001",
	"Bee Storm - DoDonPachi II (V101, World)\0", NULL, "IGS", "PolyGameMaster",
	L"Bee Storm - DoDonPachi II\0\u6012\u9996\u9886\u8702\uff0d\u8702\u66b4 (V101, World)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp2101RomInfo, ddp2101RomName, NULL, NULL, NULL, NULL, pgmInputInfo, ddp2DIPInfo,
	ddp2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224,448,3,4
};


// Bee Storm - DoDonPachi II (V100, World)

static struct BurnRomInfo ddp2100RomDesc[] = {
	DDP2_PROGRAM_100
	DDP2_ASIC027_WORLD
	DDP2_COMMON_ROMS
};

STDROMPICKEXT(ddp2100, ddp2100, pgm)
STD_ROM_FN(ddp2100)

struct BurnDriver BurnDrvDdp2100 = {
	"ddp2100", "ddp2", "pgm", NULL, "2001",
	"Bee Storm - DoDonPachi II (V100, World)\0", NULL, "IGS", "PolyGameMaster",
	L"Bee Storm - DoDonPachi II\0\u6012\u9996\u9886\u8702\uff0d\u8702\u66b4 (V100, World)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp2100RomInfo, ddp2100RomName, NULL, NULL, NULL, NULL, pgmInputInfo, ddp2DIPInfo,
	ddp2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224,448,3,4
};


// Bee Storm - DoDonPachi II (V102, Hong Kong)

static struct BurnRomInfo ddp2hkRomDesc[] = {
	DDP2_PROGRAM_102
	DDP2_ASIC027_HONG_KONG
	DDP2_COMMON_ROMS
};

STDROMPICKEXT(ddp2hk, ddp2hk, pgm)
STD_ROM_FN(ddp2hk)

struct BurnDriver BurnDrvDdp2hk = {
	"ddp2hk", "ddp2", "pgm", NULL, "2001",
	"Bee Storm - DoDonPachi II (V102, Hong Kong)\0", NULL, "IGS", "PolyGameMaster",
	L"Bee Storm - DoDonPachi II\0\u6012\u9996\u9886\u8702\uff0d\u8702\u66b4 (V102, Hong Kong)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp2hkRomInfo, ddp2hkRomName, NULL, NULL, NULL, NULL, pgmInputInfo, ddp2hkDIPInfo,
	ddp2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224,448,3,4
};


// Bee Storm - DoDonPachi II (V101, Hong Kong)

static struct BurnRomInfo ddp2101hkRomDesc[] = {
	DDP2_PROGRAM_101
	DDP2_ASIC027_HONG_KONG
	DDP2_COMMON_ROMS
};

STDROMPICKEXT(ddp2101hk, ddp2101hk, pgm)
STD_ROM_FN(ddp2101hk)

struct BurnDriver BurnDrvDdp2101hk = {
	"ddp2101hk", "ddp2", "pgm", NULL, "2001",
	"Bee Storm - DoDonPachi II (V101, Hong Kong)\0", NULL, "IGS", "PolyGameMaster",
	L"Bee Storm - DoDonPachi II\0\u6012\u9996\u9886\u8702\uff0d\u8702\u66b4 (V101, Hong Kong)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp2101hkRomInfo, ddp2101hkRomName, NULL, NULL, NULL, NULL, pgmInputInfo, ddp2hkDIPInfo,
	ddp2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224,448,3,4
};


// Bee Storm - DoDonPachi II (V100, Hong Kong)

static struct BurnRomInfo ddp2100hkRomDesc[] = {
	DDP2_PROGRAM_100
	DDP2_ASIC027_HONG_KONG
	DDP2_COMMON_ROMS
};

STDROMPICKEXT(ddp2100hk, ddp2100hk, pgm)
STD_ROM_FN(ddp2100hk)

struct BurnDriver BurnDrvDdp2100hk = {
	"ddp2100hk", "ddp2", "pgm", NULL, "2001",
	"Bee Storm - DoDonPachi II (V100, Hong Kong)\0", NULL, "IGS", "PolyGameMaster",
	L"Bee Storm - DoDonPachi II\0\u6012\u9996\u9886\u8702\uff0d\u8702\u66b4 (V100, Hong Kong)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp2100hkRomInfo, ddp2100hkRomName, NULL, NULL, NULL, NULL, pgmInputInfo, ddp2hkDIPInfo,
	ddp2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224,448,3,4
};


// Bee Storm - DoDonPachi II (V102, Korea)

static struct BurnRomInfo ddp2kRomDesc[] = {
	DDP2_PROGRAM_102
	DDP2_ASIC027_KOREA
	DDP2_COMMON_ROMS
};

STDROMPICKEXT(ddp2k, ddp2k, pgm)
STD_ROM_FN(ddp2k)

struct BurnDriver BurnDrvDdp2k = {
	"ddp2k", "ddp2", "pgm", NULL, "2001",
	"Bee Storm - DoDonPachi II (V102, Korea)\0", NULL, "IGS", "PolyGameMaster",
	L"Bee Storm - DoDonPachi II\0\u6012\u9996\u9886\u8702\uff0d\u8702\u66b4 (V102, Korea)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp2kRomInfo, ddp2kRomName, NULL, NULL, NULL, NULL, pgmInputInfo, ddp2kDIPInfo,
	ddp2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224,448,3,4
};


// Bee Storm - DoDonPachi II (V101, Korea)

static struct BurnRomInfo ddp2101kRomDesc[] = {
	DDP2_PROGRAM_101
	DDP2_ASIC027_KOREA
	DDP2_COMMON_ROMS
};

STDROMPICKEXT(ddp2101k, ddp2101k, pgm)
STD_ROM_FN(ddp2101k)

struct BurnDriver BurnDrvDdp2101k = {
	"ddp2101k", "ddp2", "pgm", NULL, "2001",
	"Bee Storm - DoDonPachi II (V101, Korea)\0", NULL, "IGS", "PolyGameMaster",
	L"Bee Storm - DoDonPachi II\0\u6012\u9996\u9886\u8702\uff0d\u8702\u66b4 (V101, Korea)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp2101kRomInfo, ddp2101kRomName, NULL, NULL, NULL, NULL, pgmInputInfo, ddp2kDIPInfo,
	ddp2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224,448,3,4
};


// Bee Storm - DoDonPachi II (V100, Korea)

static struct BurnRomInfo ddp2100kRomDesc[] = {
	DDP2_PROGRAM_100
	DDP2_ASIC027_KOREA
	DDP2_COMMON_ROMS
};

STDROMPICKEXT(ddp2100k, ddp2100k, pgm)
STD_ROM_FN(ddp2100k)

struct BurnDriver BurnDrvDdp2100k = {
	"ddp2100k", "ddp2", "pgm", NULL, "2001",
	"Bee Storm - DoDonPachi II (V100, Korea)\0", NULL, "IGS", "PolyGameMaster",
	L"Bee Storm - DoDonPachi II\0\u6012\u9996\u9886\u8702\uff0d\u8702\u66b4 (V100, Korea)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp2100kRomInfo, ddp2100kRomName, NULL, NULL, NULL, NULL, pgmInputInfo, ddp2kDIPInfo,
	ddp2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224,448,3,4
};


// Bee Storm - DoDonPachi II (V102, Japan)

static struct BurnRomInfo ddp2jRomDesc[] = {
	DDP2_PROGRAM_102
	DDP2_ASIC027_JAPAN
	DDP2_COMMON_ROMS
};

STDROMPICKEXT(ddp2j, ddp2j, pgm)
STD_ROM_FN(ddp2j)

struct BurnDriver BurnDrvDdp2j = {
	"ddp2j", "ddp2", "pgm", NULL, "2001",
	"Bee Storm - DoDonPachi II (V102, Japan)\0", NULL, "IGS", "PolyGameMaster",
	L"Bee Storm - DoDonPachi II\0\u6012\u9996\u9886\u8702\uff0d\u8702\u66b4 (V102, Japan)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp2jRomInfo, ddp2jRomName, NULL, NULL, NULL, NULL, pgmInputInfo, ddp2jDIPInfo,
	ddp2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224,448,3,4
};


// Bee Storm - DoDonPachi II (V101, Japan)

static struct BurnRomInfo ddp2101jRomDesc[] = {
	DDP2_PROGRAM_101
	DDP2_ASIC027_JAPAN
	DDP2_COMMON_ROMS
};

STDROMPICKEXT(ddp2101j, ddp2101j, pgm)
STD_ROM_FN(ddp2101j)

struct BurnDriver BurnDrvDdp2101j = {
	"ddp2101j", "ddp2", "pgm", NULL, "2001",
	"Bee Storm - DoDonPachi II (V101, Japan)\0", NULL, "IGS", "PolyGameMaster",
	L"Bee Storm - DoDonPachi II\0\u6012\u9996\u9886\u8702\uff0d\u8702\u66b4 (V101, Japan)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp2101jRomInfo, ddp2101jRomName, NULL, NULL, NULL, NULL, pgmInputInfo, ddp2jDIPInfo,
	ddp2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224,448,3,4
};


// Bee Storm - DoDonPachi II (V100, Japan)

static struct BurnRomInfo ddp2100jRomDesc[] = {
	DDP2_PROGRAM_100
	DDP2_ASIC027_JAPAN
	DDP2_COMMON_ROMS
};

STDROMPICKEXT(ddp2100j, ddp2100j, pgm)
STD_ROM_FN(ddp2100j)

struct BurnDriver BurnDrvDdp2100j = {
	"ddp2100j", "ddp2", "pgm", NULL, "2001",
	"Bee Storm - DoDonPachi II (V100, Japan)\0", NULL, "IGS", "PolyGameMaster",
	L"Bee Storm - DoDonPachi II\0\u6012\u9996\u9886\u8702\uff0d\u8702\u66b4 (V100, Japan)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp2100jRomInfo, ddp2100jRomName, NULL, NULL, NULL, NULL, pgmInputInfo, ddp2jDIPInfo,
	ddp2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224,448,3,4
};


// Bee Storm - DoDonPachi II (V102, Taiwan)

static struct BurnRomInfo ddp2tRomDesc[] = {
	DDP2_PROGRAM_102
	DDP2_ASIC027_TAIWAN
	DDP2_COMMON_ROMS
};

STDROMPICKEXT(ddp2t, ddp2t, pgm)
STD_ROM_FN(ddp2t)

struct BurnDriver BurnDrvDdp2t = {
	"ddp2t", "ddp2", "pgm", NULL, "2001",
	"Bee Storm - DoDonPachi II (V102, Taiwan)\0", NULL, "IGS", "PolyGameMaster",
	L"Bee Storm - DoDonPachi II\0\u6012\u9996\u9886\u8702\uff0d\u8702\u66b4 (V102, Taiwan)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp2tRomInfo, ddp2tRomName, NULL, NULL, NULL, NULL, pgmInputInfo, ddp2tDIPInfo,
	ddp2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224,448,3,4
};


// Bee Storm - DoDonPachi II (V101, Taiwan)

static struct BurnRomInfo ddp2101tRomDesc[] = {
	DDP2_PROGRAM_101
	DDP2_ASIC027_TAIWAN
	DDP2_COMMON_ROMS
};

STDROMPICKEXT(ddp2101t, ddp2101t, pgm)
STD_ROM_FN(ddp2101t)

struct BurnDriver BurnDrvDdp2101t = {
	"ddp2101t", "ddp2", "pgm", NULL, "2001",
	"Bee Storm - DoDonPachi II (V101, Taiwan)\0", NULL, "IGS", "PolyGameMaster",
	L"Bee Storm - DoDonPachi II\0\u6012\u9996\u9886\u8702\uff0d\u8702\u66b4 (V101, Taiwan)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp2101tRomInfo, ddp2101tRomName, NULL, NULL, NULL, NULL, pgmInputInfo, ddp2tDIPInfo,
	ddp2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224,448,3,4
};


// Bee Storm - DoDonPachi II (V100, Taiwan)

static struct BurnRomInfo ddp2100tRomDesc[] = {
	DDP2_PROGRAM_100
	DDP2_ASIC027_TAIWAN
	DDP2_COMMON_ROMS
};

STDROMPICKEXT(ddp2100t, ddp2100t, pgm)
STD_ROM_FN(ddp2100t)

struct BurnDriver BurnDrvDdp2100t = {
	"ddp2100t", "ddp2", "pgm", NULL, "2001",
	"Bee Storm - DoDonPachi II (V100, Taiwan)\0", NULL, "IGS", "PolyGameMaster",
	L"Bee Storm - DoDonPachi II\0\u6012\u9996\u9886\u8702\uff0d\u8702\u66b4 (V100, Taiwan)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp2100tRomInfo, ddp2100tRomName, NULL, NULL, NULL, NULL, pgmInputInfo, ddp2tDIPInfo,
	ddp2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224,448,3,4
};


// Bee Storm - DoDonPachi II (V102, China)

static struct BurnRomInfo ddp2cRomDesc[] = {
	DDP2_PROGRAM_102
	DDP2_ASIC027_CHINA
	DDP2_COMMON_ROMS
};

STDROMPICKEXT(ddp2c, ddp2c, pgm)
STD_ROM_FN(ddp2c)

struct BurnDriver BurnDrvDdp2c = {
	"ddp2c", "ddp2", "pgm", NULL, "2001",
	"Bee Storm - DoDonPachi II (V102, China)\0", NULL, "IGS", "PolyGameMaster",
	L"Bee Storm - DoDonPachi II\0\u6012\u9996\u9886\u8702\uff0d\u8702\u66b4 (V102, China)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp2cRomInfo, ddp2cRomName, NULL, NULL, NULL, NULL, pgmInputInfo, ddp2cDIPInfo,
	ddp2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224,448,3,4
};


// Bee Storm - DoDonPachi II (V101, China)

static struct BurnRomInfo ddp2101cRomDesc[] = {
	DDP2_PROGRAM_101
	DDP2_ASIC027_CHINA
	DDP2_COMMON_ROMS
};

STDROMPICKEXT(ddp2101c, ddp2101c, pgm)
STD_ROM_FN(ddp2101c)

struct BurnDriver BurnDrvDdp2101c = {
	"ddp2101c", "ddp2", "pgm", NULL, "2001",
	"Bee Storm - DoDonPachi II (V101, China)\0", NULL, "IGS", "PolyGameMaster",
	L"Bee Storm - DoDonPachi II\0\u6012\u9996\u9886\u8702\uff0d\u8702\u66b4 (V101, China)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp2101cRomInfo, ddp2101cRomName, NULL, NULL, NULL, NULL, pgmInputInfo, ddp2cDIPInfo,
	ddp2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224,448,3,4
};


// Bee Storm - DoDonPachi II (V100, Taiwan)

static struct BurnRomInfo ddp2100cRomDesc[] = {
	DDP2_PROGRAM_100
	DDP2_ASIC027_CHINA
	DDP2_COMMON_ROMS
};

STDROMPICKEXT(ddp2100c, ddp2100c, pgm)
STD_ROM_FN(ddp2100c)

struct BurnDriver BurnDrvDdp2100c = {
	"ddp2100c", "ddp2", "pgm", NULL, "2001",
	"Bee Storm - DoDonPachi II (V100, China)\0", NULL, "IGS", "PolyGameMaster",
	L"Bee Storm - DoDonPachi II\0\u6012\u9996\u9886\u8702\uff0d\u8702\u66b4 (V100, China)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp2100cRomInfo, ddp2100cRomName, NULL, NULL, NULL, NULL, pgmInputInfo, ddp2cDIPInfo,
	ddp2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224,448,3,4
};


// Dragon World 2001 (V100, Japan)

static struct BurnRomInfo dw2001RomDesc[] = {
	{ "2001_u22.u22",	   	0x080000, 0x5cabed92, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "2001_u11.u11",	   	0x200000, 0xb27cf093, 2 | BRF_GRA },			//  1 Tile data

	{ "2001_u2.u2",	   		0x200000, 0xd11c733c, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "2001_u3.u3",	   		0x200000, 0x1435aef2, 3 | BRF_GRA },			//  4

	{ "2001_u9.u9",	   		0x200000, 0xccbca572, 4 | BRF_GRA },			//  5 Sprite Masks & Color Indexes

	{ "2001_u7.u7",	   		0x200000, 0x4ea62f21, 5 | BRF_SND },			//  6 Samples

	{ "dw2001_igs027a_japan.bin",	0x004000, 0x3a79159b, 7 | BRF_PRG },	//  7 Internal ARM7 rom

	{ "2001_u12.u12",	   	0x080000, 0x973db1ab, 8 | BRF_PRG | BRF_ESS },	// 8 External ARM7 rom
};

STDROMPICKEXT(dw2001, dw2001, pgm)
STD_ROM_FN(dw2001)

static void dw2001Callback()
{
	pgm_decrypt_martmast();

	// add proper fix to rom loading routines at a later date
	memcpy (ICSSNDROM + 0x200000, ICSSNDROM + 0x400000, 0x200000);
}

static INT32 dw2001Init()
{
	pPgmInitCallback = dw2001Callback;
	pPgmProtCallback = install_protection_asic27a_martmast;

	INT32 nRet = pgmInit();

	Arm7SetIdleLoopAddress(0x8000228);

	return nRet;
}

struct BurnDriver BurnDrvDw2001 = {
	"dw2001", NULL, "pgm", NULL, "2001",
	"Dragon World 2001 (V100, Japan)\0", "Bad sound?", "IGS (Alta Co., LTD License)", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_PUZZLE, 0,
	NULL, dw2001RomInfo, dw2001RomName, NULL, NULL, NULL, NULL, pgmInputInfo, pgmDIPInfo,
	dw2001Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Dragon World Pretty Chance (V101, Japan)

static struct BurnRomInfo dwpcRomDesc[] = {
	{ "dwpc_v101jp_u22.u22",	0x080000, 0xb93027c0, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "dwpc_v100jp_u11.u11",	0x400000, 0x3aa5a787, 2 | BRF_GRA },			//  1 Tile data

	{ "dwpc_v100jp_u2.u2",		0x200000, 0xe7115763, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "dwpc_v100jp_u3.u3",		0x200000, 0x49c184a4, 3 | BRF_GRA },			//  4

	{ "dwpc_v100jp_u9.u9",		0x200000, 0x412b9913, 4 | BRF_GRA },			//  5 Sprite Masks & Color Indexes

	{ "dwpc_v100jp_u7.u7",		0x200000, 0x5cf9bada, 5 | BRF_SND },			//  6 Samples

	{ "dw2001_igs027a_japan.bin",	0x004000, 0x3a79159b, 7 | BRF_PRG },  		//  7 Internal ARM7 rom

	{ "dwpc_v100jp_u12.u12",	0x080000, 0x0d112126, 8 | BRF_PRG | BRF_ESS },	// 8 External ARM7 rom
};

STDROMPICKEXT(dwpc, dwpc, pgm)
STD_ROM_FN(dwpc)

struct BurnDriver BurnDrvdwpc = {
	"dwpc", NULL, "pgm", NULL, "2001",
	"Dragon World Pretty Chance (V101, Japan)\0", "Bad sound?", "IGS (Alta Co., LTD License)", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_PUZZLE, 0,
	NULL, dwpcRomInfo, dwpcRomName, NULL, NULL, NULL, NULL, pgmInputInfo, pgmDIPInfo,
	dw2001Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Demon Front (V105)

static struct BurnRomInfo dmnfrntRomDesc[] = {
	// 1st dump (01/01/2006) : v105_32m.u26 size 4 194 304 crc 877cac84
	// - 1st and 2nd half identical crc 7c0690e1
	// 2nd dump (15/04/2009) : v105_32m.u26 size 4 194 304 crc d200ee63
	// - 1st half crc 341f0f22
	// - 2nd half crc 3ce2fe8a
	// 3rd dump (21/11/2009) : chinese-v105.u62 size 4 194 304 crc c798c2ef
	// - 1st half crc 7c0690e1
	// - 2nd half crc 3ce2fe8a
	// Regarding these 3 dumps, only the 3rd is correct. Others are confirmed bad dumps.
	{ "v105_16m.u5",   		0x200000, 0xbda083bd, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "igs_t04501w064.u29",	0x800000, 0x900eaaac, 2 | BRF_GRA },			//  1 Tile data

	{ "igs_a04501w064.u3",  0x800000, 0x9741bea6, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "igs_a04502w064.u4",  0x800000, 0xe104f405, 3 | BRF_GRA },			//  3
	{ "igs_a04503w064.u6",  0x800000, 0xbfd5cfe3, 3 | BRF_GRA },			//  4

	{ "igs_b04501w064.u9",	0x800000, 0x29320b7d, 4 | BRF_GRA },			//  5 Sprite Masks & Color Indexes
	{ "igs_b04502w016.u11",	0x200000, 0x578c00e9, 4 | BRF_GRA },			//  6

	{ "igs_w04501b064.u5",  0x800000, 0x3ab58137, 5 | BRF_SND },			//  7 Samples

	{ "dmnfrnt_igs027a.bin",	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  8 Internal ARM7 Rom

	{ "v105_32m.u26",	 	0x400000, 0xc798c2ef, 8 | BRF_PRG | BRF_ESS },  //  9 External ARM7 Rom
};

STDROMPICKEXT(dmnfrnt, dmnfrnt, pgm)
STD_ROM_FN(dmnfrnt)

static void pgm_fake_internal_asic()
{
	UINT16 *mem16 = (UINT16 *)PGMARMROM;

	for (INT32 i = 0; i < 0x4000/2; i+=2) {
		mem16[i + 0] = BURN_ENDIAN_SWAP_INT16(0xff1e);
		mem16[i + 1] = BURN_ENDIAN_SWAP_INT16(0xe12f);
	}

	mem16[0x000000 / 2] = BURN_ENDIAN_SWAP_INT16(0xd088);
	mem16[0x000002 / 2] = BURN_ENDIAN_SWAP_INT16(0xe59f);
	mem16[0x000004 / 2] = BURN_ENDIAN_SWAP_INT16(0x0680);
	mem16[0x000006 / 2] = BURN_ENDIAN_SWAP_INT16(0xe3a0);
	mem16[0x000008 / 2] = BURN_ENDIAN_SWAP_INT16(0xff10);
	mem16[0x00000a / 2] = BURN_ENDIAN_SWAP_INT16(0xe12f);
	mem16[0x000090 / 2] = BURN_ENDIAN_SWAP_INT16(0x0400);
	mem16[0x000092 / 2] = BURN_ENDIAN_SWAP_INT16(0x1000);
}

static void dmfrntCallback()
{
	pgm_decrypt_dfront();
	pgm_fake_internal_asic();
}

static INT32 dmnfrntInit()
{
	pPgmInitCallback = dmfrntCallback; //pgm_decrypt_dfront;
	pPgmProtCallback = install_protection_asic27a_svg;

	INT32 nRet = pgmInit();

	Arm7SetIdleLoopAddress(0x8000fb4);

	return nRet;
}

struct BurnDriver BurnDrvDmnfrnt = {
	"dmnfrnt", NULL, "pgm", NULL, "2002",
	"Demon Front (V105)\0", NULL, "IGS", "PolyGameMaster",
	L"Demon Front\0\u9B54\u57DF\u6218\u7EBF\0\u9B54\u57DF\u6230\u7DDA (V105)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_RUNGUN, 0,
	NULL, dmnfrntRomInfo, dmnfrntRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	dmnfrntInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Demon Front (V103)

static struct BurnRomInfo dmnfrntbRomDesc[] = {
	{ "v103_16m.u5",   		0x200000, 0x2ddafb3d, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "igs_t04501w064.u29",	0x800000, 0x900eaaac, 2 | BRF_GRA },			//  1 Tile data

	{ "igs_a04501w064.u3",  0x800000, 0x9741bea6, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "igs_a04502w064.u4",  0x800000, 0xe104f405, 3 | BRF_GRA },			//  3
	{ "igs_a04503w064.u6",  0x800000, 0xbfd5cfe3, 3 | BRF_GRA },			//  4

	{ "igs_b04501w064.u9",	0x800000, 0x29320b7d, 4 | BRF_GRA },			//  5 Sprite Masks & Color Indexes
	{ "igs_b04502w016.u11",	0x200000, 0x578c00e9, 4 | BRF_GRA },			//  6

	{ "igs_w04501b064.u5",  0x800000, 0x3ab58137, 5 | BRF_SND },			//  7 Samples

	{ "dmnfrnt_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  8 Internal ARM7 Rom

	{ "v103_32m.u26",  		0x400000, 0xe78383a3, 8 | BRF_PRG | BRF_ESS },  //  9 External ARM7 Rom
};

STDROMPICKEXT(dmnfrntb, dmnfrntb, pgm)
STD_ROM_FN(dmnfrntb)

struct BurnDriver BurnDrvdmnfrntb = {
	"dmnfrntb", "dmnfrnt", "pgm", NULL, "2002",
	"Demon Front (V103)\0", NULL, "IGS", "PolyGameMaster",
	L"Demon Front\0\u9B54\u57DF\u6218\u7EBF\0\u9B54\u57DF\u6230\u7DDA (V103)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_RUNGUN, 0,
	NULL, dmnfrntbRomInfo, dmnfrntbRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	dmnfrntInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Demon Front (V102, China)

static struct BurnRomInfo dmnfrntaRomDesc[] = {
	{ "v102_16m.u5",   		0x200000, 0x3d4d481a, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "igs_t04501w064.u29",	0x800000, 0x900eaaac, 2 | BRF_GRA },			//  1 Tile data

	{ "igs_a04501w064.u3",  0x800000, 0x9741bea6, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "igs_a04502w064.u4",  0x800000, 0xe104f405, 3 | BRF_GRA },			//  3
	{ "igs_a04503w064.u6",  0x800000, 0xbfd5cfe3, 3 | BRF_GRA },			//  4

	{ "igs_b04501w064.u9",	0x800000, 0x29320b7d, 4 | BRF_GRA },			//  5 Sprite Masks & Color Indexes
	{ "igs_b04502w016.u11",	0x200000, 0x578c00e9, 4 | BRF_GRA },			//  6

	{ "igs_w04501b064.u5",  0x800000, 0x3ab58137, 5 | BRF_SND },			//  7 Samples

	{ "dmnfrnt_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  8 Internal ARM7 Rom

	{ "v101_32m.u26",  		0x400000, 0x93965281, 8 | BRF_PRG | BRF_ESS },  //  9 External ARM7 Rom
};

STDROMPICKEXT(dmnfrnta, dmnfrnta, pgm)
STD_ROM_FN(dmnfrnta)

struct BurnDriver BurnDrvdmnfrnta = {
	"dmnfrnta", "dmnfrnt", "pgm", NULL, "2002",
	"Demon Front (V102, China)\0", NULL, "IGS", "PolyGameMaster",
	L"Demon Front\0\u9B54\u57DF\u6218\u7EBF\0\u9B54\u57DF\u6230\u7DDA (V102, China)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_RUNGUN, 0,
	NULL, dmnfrntaRomInfo, dmnfrntaRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	dmnfrntInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// The Gladiator - Road Of The Sword / Shen Jian (V101)

static struct BurnRomInfo thegladRomDesc[] = {
	{ "v101_u6.u6",			0x080000, 0xf799e866, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "igs_t04601w64m.u33",	0x800000, 0xe5dab371, 2 | BRF_GRA },			//  1 Tile data

	{ "igs_a04601w64m.u2",	0x800000, 0xd9b2e004, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "igs_a04602w64m.u4",	0x800000, 0x14f22308, 3 | BRF_GRA },			//  3
	{ "igs_a04603w64m.u6",	0x800000, 0x8f621e17, 3 | BRF_GRA },			//  4

	{ "igs_b04601w64m.u11",	0x800000, 0xee72bccf, 4 | BRF_GRA },			//  5 Sprite Masks & Color Indexes
	{ "igs_b04602w64m.u12",	0x400000, 0x7dba9c38, 4 | BRF_GRA },			//  6

	{ "igs_w04601b64m.u1",	0x800000, 0x5f15ddb3, 5 | BRF_SND },			//  7 Samples

//	{ "theglad_igs027a.bin",					0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  8 Internal ARM7 Rom
	{ "theglad_igs027a_execute_only_area",	    0x000188, 0x00000000, 0 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  8 Internal ARM7 Rom
	{ "theglad_igs027a_v100_overseas.bin",      0x003e78, 0x02fe6f52, 7 | BRF_PRG | BRF_ESS },  //  9

	{ "v107_u26.u26",		0x200000, 0xf7c61357, 8 | BRF_PRG | BRF_ESS },	// 10 External ARM7 Rom
};

STDROMPICKEXT(theglad, theglad, pgm)
STD_ROM_FN(theglad)

static void pgm_create_theglad_EO_data()
{
	// Replace undumpable area of the arm7 internal rom with a custom-built
	// version created by David Haywood in order to make game playable
	static UINT16 thegladEOHackData[0x188/2] = {
		0x000a, 0xea00, 0xfffe, 0xeaff, 0xfffe, 0xeaff, 0xfffe, 0xeaff,
		0xfffe, 0xeaff, 0xfffe, 0xeaff, 0xfffe, 0xeaff, 0xf000, 0xe59f,
		0x0010, 0x0800, 0x0010, 0x0800, 0xfffe, 0xeaff, 0xfffe, 0xeaff,
		0x00d2, 0xe3a0, 0xf000, 0xe121, 0x4001, 0xe3a0, 0x4b06, 0xe284,
		0x0cfa, 0xe3a0, 0xd804, 0xe080, 0x00d1, 0xe3a0, 0xf000, 0xe121,
		0x0cf6, 0xe3a0, 0xd804, 0xe080, 0x00d7, 0xe3a0, 0xf000, 0xe121,
		0x0cff, 0xe3a0, 0xd804, 0xe080, 0x00db, 0xe3a0, 0xf000, 0xe121,
		0x4140, 0xe1c4, 0x0cfe, 0xe3a0, 0xd804, 0xe080, 0x00d3, 0xe3a0,
		0xf000, 0xe121, 0x4a01, 0xe3a0, 0x0b01, 0xe3a0, 0xd804, 0xe080,
		0x5a0f, 0xe3a0, 0x0008, 0xe3a0, 0x8805, 0xe080, 0x0010, 0xe3a0,
		0x0000, 0xe5c8, 0x7805, 0xe1a0, 0x6a01, 0xe3a0, 0x0012, 0xe3a0,
		0x0a02, 0xe280, 0x6806, 0xe080, 0x6000, 0xe587, 0x00d3, 0xe3a0,
		0xf000, 0xe121, 0x4001, 0xe3a0, 0x4b06, 0xe284, 0x0cf2, 0xe3a0,
		0xd804, 0xe080, 0x0013, 0xe3a0, 0xf000, 0xe121, 0x0028, 0xea00,
		0xfffe, 0xeaff, 0xfffe, 0xeaff, 0xe004, 0xe52d, 0x00d3, 0xe3a0,
		0xf000, 0xe121, 0xe004, 0xe49d, 0xff1e, 0xe12f, 0xe004, 0xe52d,
		0x0013, 0xe3a0, 0xf000, 0xe121, 0xe004, 0xe49d, 0xff1e, 0xe12f,
		0x00d1, 0xe3a0, 0xf000, 0xe121, 0xd0b8, 0xe59f, 0x00d3, 0xe3a0,
		0xf000, 0xe121, 0xd0b0, 0xe59f, 0x10b8, 0xe59f, 0x0000, 0xe3a0,
		0x0000, 0xe581, 0xf302, 0xe3a0, 0xfffe, 0xeaff, 0xfffe, 0xeaff,
		0xfffe, 0xeaff, 0xfffe, 0xeaff, 0xfffe, 0xeaff, 0xfffe, 0xeaff,
		0xff1e, 0xe12f, 0xfffe, 0xeaff, 0xfffe, 0xeaff, 0xfffe, 0xeaff,
		0xfffe, 0xeaff, 0xfffe, 0xeaff, 0xfffe, 0xeaff, 0xfffe, 0xeaff,
		0xfffe, 0xeaff, 0xfffe, 0xeaff, 0xfffe, 0xeaff, 0xfffe, 0xeaff,
		0xfffe, 0xeaff, 0x105c, 0xe59f
	};

	for (int i = 0; i < 0x188/2; i++) {
		thegladEOHackData[i] = BURN_ENDIAN_SWAP_INT16(thegladEOHackData[i]);
	}

	memcpy  (PGMARMROM, thegladEOHackData, 0x188);
}

static void thegladPatch()
{
	pgm_decrypt_theglad();
	pgm_create_theglad_EO_data();
}

static INT32 thegladInit()
{
	nPgmAsicRegionHackAddress = 0x3316;
	pPgmInitCallback = thegladPatch;
	pPgmProtCallback = install_protection_asic27a_svg;

	INT32 nRet = pgmInit();

	Arm7SetIdleLoopAddress(0x000007c4);

	return nRet;
}

struct BurnDriver BurnDrvTheglad = {
	"theglad", NULL, "pgm", NULL, "2003",
	"The Gladiator - Road Of The Sword / Shen Jian (V101)\0", "Incomplete Dump", "IGS", "PolyGameMaster",
	L"The Gladiator - Road of The Sword\0\u795E\u5251\u98CE\u4E91\0\u795E\u528D\u98A8\u96F2 (V101)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, thegladRomInfo, thegladRomName, NULL, NULL, NULL, NULL, pgmInputInfo, thegladDIPInfo,
	thegladInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// The Gladiator - Road Of The Sword / Shen Jian (V100)

static struct BurnRomInfo theglad101RomDesc[] = {
	{ "v100.u6",			0x080000, 0xbcf3b172, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "igs_t04601w64m.u33",	0x800000, 0xe5dab371, 2 | BRF_GRA },			//  1 Tile data

	{ "igs_a04601w64m.u2",	0x800000, 0xd9b2e004, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "igs_a04602w64m.u4",	0x800000, 0x14f22308, 3 | BRF_GRA },			//  3
	{ "igs_a04603w64m.u6",	0x800000, 0x8f621e17, 3 | BRF_GRA },			//  4

	{ "igs_b04601w64m.u11",	0x800000, 0xee72bccf, 4 | BRF_GRA },			//  5 Sprite Masks & Color Indexes
	{ "igs_b04602w64m.u12",	0x400000, 0x7dba9c38, 4 | BRF_GRA },			//  6

	{ "igs_w04601b64m.u1",	0x800000, 0x5f15ddb3, 5 | BRF_SND },			//  7 Samples

//	{ "theglad_igs027a.bin",		    0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  8 Internal ARM7 Rom
	{ "theglad_igs027a_execute_only_area",	    0x000188, 0x00000000, 0 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  8 Internal ARM7 Rom
	{ "theglad_igs027a_v100_overseas.bin",      0x003e78, 0x02fe6f52, 7 | BRF_PRG | BRF_ESS },  //  9

	{ "v101.u26",			0x200000, 0x23faec02, 8 | BRF_PRG | BRF_ESS },	// 10 External ARM7 Rom
};

STDROMPICKEXT(theglad101, theglad101, pgm)
STD_ROM_FN(theglad101)

struct BurnDriver BurnDrvTheglad101 = {
	"theglad101", "theglad", "pgm", NULL, "2003",
	"The Gladiator - Road Of The Sword / Shen Jian (V100)\0", "Incomplete Dump", "IGS", "PolyGameMaster",
	L"The Gladiator - Road of The Sword\0\u795E\u5251\u98CE\u4E91\0\u795E\u528D\u98A8\u96F2 (V100)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, theglad101RomInfo, theglad101RomName, NULL, NULL, NULL, NULL, pgmInputInfo, thegladDIPInfo,
	thegladInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// The Gladiator - Road of The Sword / Shen Jian (V100, Taiwan)

static struct BurnRomInfo theglad100RomDesc[] = {
	{ "u6.rom",				0x080000, 0x14c85212, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "igs_t04601w64m.u33",	0x800000, 0xe5dab371, 2 | BRF_GRA },			//  1 Tile data

	{ "igs_a04601w64m.u2",	0x800000, 0xd9b2e004, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "igs_a04602w64m.u4",	0x800000, 0x14f22308, 3 | BRF_GRA },			//  3
	{ "igs_a04603w64m.u6",	0x800000, 0x8f621e17, 3 | BRF_GRA },			//  4

	{ "igs_b04601w64m.u11",	0x800000, 0xee72bccf, 4 | BRF_GRA },			//  5 Sprite Masks & Color Indexes
	{ "igs_b04602w64m.u12",	0x400000, 0x7dba9c38, 4 | BRF_GRA },			//  6

	{ "igs_w04601b64m.u1",	0x800000, 0x5f15ddb3, 5 | BRF_SND },			//  7 Samples

//	{ "theglad_igs027a.bin",		    0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  8 Internal ARM7 Rom
	{ "theglad_igs027a_execute_only_area",	    0x000188, 0x00000000, 0 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  8 Internal ARM7 Rom
	{ "theglad_igs027a_older.bin",              0x003e78, 0x02fe6f52, 7 | BRF_PRG | BRF_ESS },  //  9

	{ "u2.rom",				0x200000, 0xc7bcf2ae, 8 | BRF_PRG | BRF_ESS },	//  9 External ARM7 Rom
};

STDROMPICKEXT(theglad100, theglad100, pgm)
STD_ROM_FN(theglad100)

static void theglad100Patch()
{
	pgm_decrypt_theglad();
	pgm_create_theglad_EO_data();

	// Hack the jump table in the external rom to work correctly with the internal rom we have...
	static const UINT16 subroutine_addresses[] = {
		0x00FC, 0x00E8, 0x0110, 0x0150, 0x0194, 0x06C8, 0x071C, 0x0728,
		0x0734, 0x0740, 0x0784, 0x0794, 0x07FC, 0x0840, 0x086C, 0x0988,
		0x0A54, 0x0AA8, 0x0AD4, 0x0EB8, 0x0EF8, 0x0F2C, 0x0F3C, 0x0F78,
		0x0FA8, 0x0FD8, 0x1028, 0x1038, 0x1048, 0x1058, 0x1068, 0x1070,
		0x1090, 0x10B0, 0x10D4, 0x1100, 0x113C, 0x1198, 0x1234, 0x1258,
		0x127C, 0x12A8, 0x12E4, 0x1368, 0x142C, 0x0B10, 0x0B54, 0x0B74,
		0x0C08, 0x0C90, 0x0D18, 0x0D90, 0x1570, 0x1600, 0x1640, 0x1694,
		0x1730, 0x176C, 0x17AC, 0x17D8, 0x18C4, 0x18E0, 0x1904, 0x1930,
		0x19D8, 0x1A38, 0x1950, 0x1970, 0x1990, 0x19B8, 0x19C8, 0x1A9C,
		0x1AC4, 0x1AE8, 0x1B20, 0x1B48, 0x1B70, 0x1B8C, 0x1BB4, 0x1BD8,
		0x1BFC, 0x1C10, 0x1C24, 0x1CA0, 0x1D5C, 0x1D7C, 0x1D8C, 0x1DAC,
		0x1DCC, 0x1DE0, 0x1DF4, 0x1E1C, 0x1E2C, 0x1E60, 0x1E94, 0x1EA4,
		0x1ECC, 0x1ED8, 0x1EE4, 0x1F14, 0x1F44, 0x1FB4, 0x1FC4, 0x2040,
		0x20BC, 0x2140, 0x21C4, 0x2240, 0x22BC, 0x2340, 0x23C4, 0x23D0,
		0x2400, 0x2430, 0x244C, 0x245C, 0x246C, 0x2FCC, 0x3000, 0x3028,
		0x3050, 0x30A4, 0x30F8, 0x3120, 0x249C, 0x24C0, 0x27BC, 0x2B40,
		0x2BF4, 0x2CD8, 0x2E2C
	};

	UINT16 *extprot = (UINT16 *)PGMUSER0;

	for (int i = 0; i < 131; i++)
	{
		extprot[((0x82078 + (i * 4)) / 2)] = (subroutine_addresses[i]);
	}
}

static INT32 theglad100Init()
{
	nPgmAsicRegionHackAddress = 0x3316;
	pPgmInitCallback = theglad100Patch;
	pPgmProtCallback = install_protection_asic27a_svg;

	INT32 nRet = pgmInit();

	Arm7SetIdleLoopAddress(0x000007c4);

	return nRet;
}

struct BurnDriver BurnDrvtheglada = {
	"theglad100", "theglad", "pgm", NULL, "2003",
	"The Gladiator - Road Of The Sword / Shen Jian (V100, Taiwan)\0", "Incomplete Dump", "IGS", "PolyGameMaster",
	L"The Gladiator - Road Of The Sword\0\u795E\u5251\u98CE\u4E91\0\u795E\u528D\u98A8\u96F2 (V100, Taiwan)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, theglad100RomInfo, theglad100RomName, NULL, NULL, NULL, NULL, pgmInputInfo, theglad100DIPInfo,
	theglad100Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Photo Y2K 2 (3-in-1)

static struct BurnRomInfo pgm3in1RomDesc[] = {
	{ "v-100cn.u3",			0x200000, 0xa39f59b4, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code
	
	{ "u5.u5",	 	        0x200000, 0xda375a50, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a1100.u4", 		0x800000, 0xe32ce499, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a1101.u5", 		0x800000, 0x4e7568bc, 3 | BRF_GRA },			//  3
	{ "pgm_a1102.u6", 		0x800000, 0x6da7c143, 3 | BRF_GRA },			//  4
	{ "pgm_a1103.u7", 		0x800000, 0x0ebebfdc, 3 | BRF_GRA },			//  4
	{ "u20.u20", 			0x400000, 0xfe314754, 3 | BRF_GRA },			//  6

	{ "pgm_b1100.u8", 		0x800000, 0xfa53d6f6, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b1101.u9", 		0x800000, 0x001e4c81, 4 | BRF_GRA },			//  8
	{ "u21.u21", 			0x200000, 0xfe31dca6, 4 | BRF_GRA },			//  9

	{ "pgm_m1100.u17", 		0x200000, 0xfb1515f8, 5 | BRF_SND },			// 10 Samples
	{ "u16.u16", 		   	0x800000, 0x714c33e5, 5 | BRF_SND },			// 11 

	{ "igs027a_pgm3in1.asic",	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	// 12 Internal ARM7 Rom
};

STDROMPICKEXT(pgm3in1, pgm3in1, pgm)
STD_ROM_FN(pgm3in1)

static INT32 pgm3in1Init()
{
	pPgmInitCallback = pgm_decrypt_pgm3in1;
	pPgmProtCallback = install_protection_asic27a_py2k2;
//	pPgmProtCallback = install_protection_asic27a_kovsh;

	return pgmInit();
}

struct BurnDriverD BurnDrvPgm3in1 = {
	"pgm3in1", NULL, "pgm", NULL, "2001",
	"Photo Y2K 2 (3-in-1)\0", "Incomplete dump", "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_PUZZLE, 0,
	NULL, pgm3in1RomInfo, pgm3in1RomName, NULL, NULL, NULL, NULL, pgmInputInfo, pgmDIPInfo,
	pgm3in1Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Happy 6-in-1 (V102 - V101MK, China)
// All ROMs labels are on stickers

static struct BurnRomInfo happy6RomDesc[] = {
	{ "v101cn_u5.u5",		0x080000, 0xaa4646e3, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t01w64m_u29.u29",   	0x800000, 0x2d3feb8b, 2 | BRF_GRA },			//  1 Tile data

	{ "a01w64m_u5.u5",	  	0x800000, 0xbbaa3df3, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a02w64m_u6.u6",	  	0x800000, 0xf8c9cd36, 3 | BRF_GRA },			//  3

	{ "b01w64m_u19.u19", 	0x800000, 0x73f5f225, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "w01w64m_u17.u17",	0x800000, 0x7e23e2be, 5 | BRF_SND },			//  5 Samples

//	{ "happy6_igs027a.bin",			0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  8 Internal ARM7 Rom
	{ "happy6_igs027a_execute_only_area",	0x000188, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP }, 	//  6 Internal ARM7 Rom
	{ "happy6_igs027a_v100_china.bin",	0x003e78, 0xed530445, 7 | BRF_PRG | BRF_ESS }, 	//  7

	{ "v102cn_u26.u26",		0x400000, 0x310510fb, 8 | BRF_PRG | BRF_ESS },	// 8 External ARM7 Rom
};

STDROMPICKEXT(happy6, happy6, pgm)
STD_ROM_FN(happy6)

static void happy6Patch()
{
	pgm_decrypt_happy6();
	pgm_create_theglad_EO_data();
	pgm_descramble_happy6_data(PGMSPRMaskROM,        0x800000);
	pgm_descramble_happy6_data(ICSSNDROM + 0x400000, 0x800000);
}

static INT32 happy6in1Init()
{
	pPgmInitCallback = happy6Patch;
	pPgmProtCallback = install_protection_asic27a_svg;

	nPgmAsicRegionHackAddress = 0x3586;

	INT32 nRet = pgmInit();

	Arm7SetIdleLoopAddress(0x00000a08);

	return nRet;
}

struct BurnDriver BurnDrvhappy6 = {
	"happy6", NULL, "pgm", NULL, "2004",
	"Happy 6-in-1 (V102 - V101MK, China)\0", NULL, "IGS", "PolyGameMaster",
	L"Happy 6-in-1\0\u6B22\u4E50\u516D\u5408\u4E00 (V102 - V101MK, China)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_CASINO, 0,
	NULL, happy6RomInfo, happy6RomName, NULL, NULL, NULL, NULL, pgmInputInfo, happy6DIPInfo,
	happy6in1Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Happy 6-in-1 (V101 - V100MK, China)
// All ROMs labels are on stickers

static struct BurnRomInfo happy6101RomDesc[] = {
	{ "v100cn_u5.u5",		0x080000, 0xa25418e8, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t01w64m_u29.u29",   	0x800000, 0x2d3feb8b, 2 | BRF_GRA },				//  1 Tile data

	{ "a01w64m_u5.u5",	  	0x800000, 0xbbaa3df3, 3 | BRF_GRA },				//  2 Sprite Color Data
	{ "a02w64m_u6.u6",	  	0x800000, 0xf8c9cd36, 3 | BRF_GRA },				//  3

	{ "b01w64m_u19.u19", 	0x800000, 0x73f5f225, 4 | BRF_GRA },				//  4 Sprite Masks & Color Indexes

	{ "w01w64m_u17.u17",	0x800000, 0x7e23e2be, 5 | BRF_SND },				//  5 Samples

//	{ "happy6_igs027a.bin",	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  8 Internal ARM7 Rom
	{ "happy6_igs027a_execute_only_area",	0x000188, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP }, 	//  6 Internal ARM7 Rom
	{ "happy6_igs027a_v100_china.bin",	0x003e78, 0xed530445, 7 | BRF_PRG | BRF_ESS }, 	//  7

	{ "v101cn_u26.u26",		0x400000, 0x4a48ca1c, 8 | BRF_PRG | BRF_ESS },	// 8 External ARM7 Rom
};

STDROMPICKEXT(happy6101, happy6101, pgm)
STD_ROM_FN(happy6101)

struct BurnDriver BurnDrvhappy6101 = {
	"happy6101", "happy6", "pgm", NULL, "2004",
	"Happy 6-in-1 (V101 - V100MK, China)\0", "Incomplete Dump", "IGS", "PolyGameMaster",
	L"Happy 6-in-1\0\u6B22\u4E50\u516D\u5408\u4E00 (V101 - V100MK, China)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_CASINO, 0,
	NULL, happy6101RomInfo, happy6101RomName, NULL, NULL, NULL, NULL, pgmInputInfo, happy6DIPInfo,
	happy6in1Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Happy 6-in-1 (V100HK - V100HK, Hong Kong)
// All ROMs labels are on stickers

static struct BurnRomInfo happy6100hkRomDesc[] = {
	{ "v100hk_u5.u5",		0x080000, 0xa25418e8, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t01w64m_u29.u29",   	0x800000, 0x2d3feb8b, 2 | BRF_GRA },				//  1 Tile data

	{ "a01w64m_u5.u5",	  	0x800000, 0xbbaa3df3, 3 | BRF_GRA },				//  2 Sprite Color Data
	{ "a02w64m_u6.u6",	  	0x800000, 0xf8c9cd36, 3 | BRF_GRA },				//  3

	{ "b01w64m_u19.u19", 	0x800000, 0x73f5f225, 4 | BRF_GRA },				//  4 Sprite Masks & Color Indexes

	{ "w01w64m_u17.u17",	0x800000, 0x7e23e2be, 5 | BRF_SND },				//  5 Samples

//	{ "happy6_igs027a.bin",	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  8 Internal ARM7 Rom
	{ "happy6_igs027a_execute_only_area",	0x000188, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP }, 	//  6 Internal ARM7 Rom
	{ "happy6_igs027a_v100_china.bin",	0x003e78, 0xed530445, 7 | BRF_PRG | BRF_ESS }, 	//  7

	{ "v100hk_u26.u26",		0x400000, 0x8f2feb1f, 8 | BRF_PRG | BRF_ESS },	// 8 External ARM7 Rom
};

STDROMPICKEXT(happy6100hk, happy6100hk, pgm)
STD_ROM_FN(happy6100hk)

struct BurnDriver BurnDrvhappy6100hk = {
	"happy6100hk", "happy6", "pgm", NULL, "2004",
	"Happy 6-in-1 (V100HK - V100MK, Hong Kong)\0", "Incomplete Dump", "IGS", "PolyGameMaster",
	L"Happy 6-in-1\0\u6B22\u4E50\u516D\u5408\u4E00 (V100HK - V100MK, Hong Kong)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_CASINO, 0,
	NULL, happy6100hkRomInfo, happy6100hkRomName, NULL, NULL, NULL, NULL, pgmInputInfo, happy6DIPInfo,
	happy6in1Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour Super Heroes Plus / Sangoku Senki Super Heroes Plus (ver. 101)

static struct BurnRomInfo kovshpRomDesc[] = {
	{ "p0600h_101.rom",		0x400000, 0xe1d89a19, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",    		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0xff7a4373, 3 | BRF_GRA }, 	        //  3 
	{ "a0602.rom",	   		0x800000, 0xe7a32959, 3 | BRF_GRA }, 	        //  4
	{ "a0540.rom",	   		0x800000, 0x4fd3413e, 3 | BRF_GRA }, 	        //  5

	{ "b0600.rom",	   		0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b0540.rom",	   		0x800000, 0x60999757, 4 | BRF_GRA },			//  7

	{ "m0600.rom",	   		0x400000, 0x3ada4fd6, 5 | BRF_SND },			//  8 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovshp, kovshp, pgm)
STD_ROM_FN(kovshp)

static void kovshp_asic_patch()
{
	*((UINT16*)(PGMARMROM + 0x2892)) = BURN_ENDIAN_SWAP_INT16(0x0101);
	*((UINT16*)(PGMARMROM + 0x289e)) = BURN_ENDIAN_SWAP_INT16(0x0107);
	*((UINT16*)(PGMARMROM + 0x28a4)) = BURN_ENDIAN_SWAP_INT16(0x0108);
	*((UINT16*)(PGMARMROM + 0x28a8)) = BURN_ENDIAN_SWAP_INT16(0x0101);
	*((UINT16*)(PGMARMROM + 0x2bf2)) = BURN_ENDIAN_SWAP_INT16(0x4810);
	*((UINT16*)(PGMARMROM + 0x2bf4)) = BURN_ENDIAN_SWAP_INT16(0x800e);
	*((UINT16*)(PGMARMROM + 0x2c92)) = BURN_ENDIAN_SWAP_INT16(0x400f);
	*((UINT16*)(PGMARMROM + 0x2ce0)) = BURN_ENDIAN_SWAP_INT16(0x6c1e);
	*((UINT16*)(PGMARMROM + 0x2ce2)) = BURN_ENDIAN_SWAP_INT16(0x0048);

	for (INT32 i = 0x2ce8; i < 0x2e48; i+=8) { // fix z80 data offsets (offset - 0x9e000)
		UINT16 d = ((PGMARMROM[i+4] << 8) + (PGMARMROM[i+7] << 0)) - 0x09e0;

		PGMARMROM[i+4] = d >> 8;
		PGMARMROM[i+7] = d & 0xff;
	};
}

static void kovshpCallback()
{
	pgm_decrypt_kovshp();
	kovshp_asic_patch();
}

static INT32 kovshpInit()
{
	pPgmInitCallback = kovshpCallback; // pgm_decrypt_kovshp;
	pPgmProtCallback = install_protection_asic27a_kovshp;

	nPgmAsicRegionHackAddress = 0x3f0d;

	INT32 nRet = pgmInit();

	Arm7SetIdleLoopAddress(0x00000260);

	return nRet;
}

struct BurnDriver BurnDrvkovshp = {
	"kovshp", NULL, "pgm", NULL, "2004",
	"Knights of Valour Super Heroes Plus / Sangoku Senki Super Heroes Plus (ver. 101)\0", "Imperfect Protection Emulation", "IGS", "PolyGameMaster",
	L"Knights of Valour Super Heroes Plus\0\u4E09\u56FD\u6218\u7EAA\0\u4E71\u4E16\u67AD\u96C4 (ver. 101)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshpRomInfo, kovshpRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshpInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour Super Heroes Plus / Sangoku Senki Super Heroes Plus (ver. 100)

static struct BurnRomInfo kovshpaRomDesc[] = {
	{ "p0600h.rom",			0x400000, 0xe251e8e4, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",    		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0xff7a4373, 3 | BRF_GRA }, 	        //  3 
	{ "a0602.rom",	   		0x800000, 0xe7a32959, 3 | BRF_GRA }, 	        //  4
	{ "a0540.rom",	   		0x800000, 0x4fd3413e, 3 | BRF_GRA }, 	        //  5

	{ "b0600.rom",	   		0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b0540.rom",	   		0x800000, 0x60999757, 4 | BRF_GRA },			//  7

	{ "m0600.rom",	   		0x400000, 0x3ada4fd6, 5 | BRF_SND },			//  8 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovshpa, kovshpa, pgm)
STD_ROM_FN(kovshpa)

struct BurnDriver BurnDrvkovshpa = {
	"kovshpa", "kovshp", "pgm", NULL, "2004",
	"Knights of Valour Super Heroes Plus / Sangoku Senki Super Heroes Plus (ver. 100)\0", "Imperfect Protection Emulation", "IGS", "PolyGameMaster",
	L"Knights of Valour Super Heroes Plus\0\u4E09\u56FD\u6218\u7EAA\0\u4E71\u4E16\u67AD\u96C4 (ver. 100)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshpaRomInfo, kovshpaRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshpInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour: Yi Tong Zhong Yuan  / Sangoku Senki: Yi Tong Zhong Yuan (ver. 201, China)

static struct BurnRomInfo kovytzyRomDesc[] = {
	{ "ytzy_v201cn.rom",		0x400000, 0xf3705ea0, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",    		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0xff7a4373, 3 | BRF_GRA }, 	        //  3 
	{ "a0602.rom",	   		0x800000, 0xe7a32959, 3 | BRF_GRA }, 	        //  4
	{ "a0540.rom",	   		0x800000, 0x4fd3413e, 3 | BRF_GRA }, 	        //  5

	{ "b0600.rom",	   		0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b0540.rom",	   		0x800000, 0x60999757, 4 | BRF_GRA },			//  7

	{ "m0600.rom",	   		0x400000, 0x3ada4fd6, 5 | BRF_SND },			//  8 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovytzy, kovytzy, pgm)
STD_ROM_FN(kovytzy)

struct BurnDriver BurnDrvkovytzy = {
	"kovytzy", NULL, "pgm", NULL, "1999",
	"Knights of Valour: Yi Tong Zhong Yuan / Sangoku Senki: Yi Tong Zhong Yuan (ver. 201, China)\0", "Imperfect Protection Emulation", "IGS", "PolyGameMaster",
	L"Knights of Valour: Yi Tong Zhong Yuan\0\u4E09\u56FD\u6218\u7EAA\0\u4E00\u7EDF\u4E2D\u539F (ver. 201, China)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovytzyRomInfo, kovytzyRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovshpInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Oriental Legend 2 (Korea) / (World, China, Japan, Hong Kong, Taiwan) (ver. 205) 

static struct BurnRomInfo oldsplusRomDesc[] = {
	{ "p03501.rom",			0x400000, 0x923f7246, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "igs_t05301w064.u2",	0x800000, 0x8257bbb0, 2 | BRF_GRA },			//  1 Tile data

	{ "igs_a05301w064.u3",	0x800000, 0x57946fd2, 3 | BRF_GRA },	 		//  2 Sprite Color Data
	{ "igs_a05302w064.u4",	0x800000, 0x3459a0b8, 3 | BRF_GRA },			//  3
	{ "igs_a05303w064.u6",	0x800000, 0x13475d85, 3 | BRF_GRA },			//  4
	{ "igs_a05304w064.u8",	0x800000, 0xf03ef7a6, 3 | BRF_GRA },			//  5

	{ "igs_b05301w064.u9",	0x800000, 0xfd98f503, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "igs_b05302w064.u11",	0x800000, 0x9f6094a8, 4 | BRF_GRA },			//  7

	{ "igs_w05301b032.u5",	0x400000, 0x86ec83bc, 5 | BRF_SND },			//  8 Samples

	{ "oldsplus_igs027a.bin",	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	//  9 Internal ARM7 Rom
};

STDROMPICKEXT(oldsplus, oldsplus, pgm)
STD_ROM_FN(oldsplus)

static INT32 oldsplusInit()
{
	pPgmInitCallback = pgm_decrypt_oldsplus;
	pPgmProtCallback = install_protection_asic27a_oldsplus; // simulation
//	pPgmProtCallback = install_protection_asic27a_kovsh;

	return pgmInit();
}

struct BurnDriver BurnDrvoldsplus = {
	"oldsplus", NULL, "pgm", NULL, "2004",
	"Oriental Legend 2 (Korea) / Xi You Shi E Zhuan Super Plus (World, China, Japan, Hong Kong, Taiwan) (ver. 205)\0", "Incomplete Dump", "IGS", "PolyGameMaster",
	L"Oriental Legend 2 (Korea)\0\u897F\u6E38\u91CA\u5384\u4F20\0\u7FA4\u9B54\u4E71\u821E (World, China, Japan, Hong Kong, Taiwan) (ver. 205)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, oldsplusRomInfo, oldsplusRomName, NULL, NULL, NULL, NULL, pgmInputInfo, puzzli2DIPInfo,
	oldsplusInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// The Killing Blade Plus (V300)

static struct BurnRomInfo killbldpRomDesc[] = {
	{ "v300xx_u6.u6",       0x080000, 0xb7fb8ec9, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code
	
	{ "igs_t05701w032.u33", 0x400000, 0x567c714f, 2 | BRF_GRA },			//  1 Tile data

	{ "igs_a05701w064.u3",	0x800000, 0x8c0c992c, 3 | BRF_GRA },			//  3 Sprite Color Data
	{ "igs_a05702w064.u4",	0x800000, 0x7e5b0f27, 3 | BRF_GRA },			//  4
	{ "igs_a05703w064.u6",	0x800000, 0xaccbdb44, 3 | BRF_GRA },			//  5
	
	{ "igs_b05701w064.u9",	0x800000, 0xa20cdcef, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "igs_b05702w016.u11", 0x200000, 0xfe7457df, 4 | BRF_GRA },			//  7
	
	{ "igs_w05701b032.u5",	0x400000, 0x2d3ae593, 5 | BRF_SND },			//  8 Samples

// this is the original rom with the first 0x268 bytes from the bootleg - but it doesn't work?
//	{ "killbldp_igs027a.bin",	0x004000, 0xc7868d90, 7 | BRF_PRG }, 		//  9 Internal ARM7 Rom
// from a bootleg
	{ "killbldp_igs027a_alt.bin",	0x004000, 0x98316b06, 7 | BRF_PRG }, 	//  9 Internal ARM7 Rom
	
	{ "v300xx_u26.u26",		0x200000, 0x144388c8, 8 | BRF_PRG | BRF_ESS },	// 10 External ARM7 Rom
};

STDROMPICKEXT(killbldp, killbldp, pgm)
STD_ROM_FN(killbldp)

INT32 killbldpInit()
{
	pPgmInitCallback = pgm_decrypt_killbldp;
	pPgmProtCallback = install_protection_asic27a_svg;
	
	INT32 nRet = pgmInit();
	
	Arm7SetIdleLoopAddress(0x00007c4);

	return nRet;
}

struct BurnDriver BurnDrvKillbldp = {
	"killbldp", NULL, "pgm", NULL, "2005",
	"The Killing Blade Plus (V300)\0", NULL, "IGS", "PolyGameMaster",
	L"The Killing Blade Plus \0\u50B2\u5251\u72C2\u5200\u00A0\u52A0\u5F3A\u7248 (V300)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VSFIGHT, 0,
	NULL, killbldpRomInfo, killbldpRomName, NULL, NULL, NULL, NULL, pgmInputInfo, pgmDIPInfo,
	killbldpInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// S.V.G. - Spectral vs Generation (V200, China)

static struct BurnRomInfo svgRomDesc[] = {
	{ "u30.bin",			0x080000, 0x34c18f3f, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t05601w016.bin",		0x200000, 0x03e110dc, 2 | BRF_GRA },		//  1 Tile data

	{ "a05601w064.bin",		0x800000, 0xea6453e4, 3 | BRF_GRA },		//  2 Sprite Color Data
	{ "a05602w064.bin",		0x800000, 0x6d00621b, 3 | BRF_GRA },		//  3
	{ "a05603w064.bin",		0x800000, 0x7b71c64f, 3 | BRF_GRA },		//  4
	{ "a05604w032.bin",		0x400000, 0x9452a567, 3 | BRF_GRA },		//  5

	{ "b05601w064.bin",		0x800000, 0x35c0a489, 4 | BRF_GRA },		//  6 Sprite Masks & Color Indexes
	{ "b05602w064.bin",		0x800000, 0x8aad3f85, 4 | BRF_GRA },		//  7

	{ "w05601b064.bin",		0x800000, 0xbfe61a71, 5 | BRF_SND },		//  8 Samples
	{ "w05602b032.bin",		0x400000, 0x0685166d, 5 | BRF_SND },		//  9

//	{ "svg_igs027a.bin",			0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	// 10 Internal ARM7 Rom
	{ "svg_igs027a_execute_only_area", 	0x000188, 0x00000000, 0 | BRF_OPT | BRF_NODUMP },	// 10 Internal ARM7 Rom
	{ "svg_igs027a_v200_china.bin", 	0x003e78, 0x72b73169, 7 | BRF_PRG | BRF_ESS },		// 11 Internal ARM7 Rom

	{ "u26.bin",			0x400000, 0x46826ec8, 8 | BRF_PRG | BRF_ESS },	// 12 External ARM7 Rom
	{ "u29.bin",			0x400000, 0xfa5f3901, 8 | BRF_PRG | BRF_ESS },	// 13
};

STDROMPICKEXT(svg, svg, pgm)
STD_ROM_FN(svg)

static void pgm_create_svg_EO_data()
{
	// Replace undumpable area of the arm7 internal rom with a custom-built
	// version created by David Haywood in order to make game playable
	static UINT16 svgEOHackData[0x188/2] = {
		0x000a, 0xea00, 0xfffe, 0xeaff, 0xfffe, 0xeaff, 0xfffe, 0xeaff,
		0xfffe, 0xeaff, 0xfffe, 0xeaff, 0xfffe, 0xeaff, 0xf000, 0xe59f,
		0x0038, 0x0800, 0x0038, 0x0800, 0xfffe, 0xeaff, 0xfffe, 0xeaff,
		0x00d2, 0xe3a0, 0xf000, 0xe121, 0x4001, 0xe3a0, 0x4b06, 0xe284,
		0x0cfa, 0xe3a0, 0xd804, 0xe080, 0x00d1, 0xe3a0, 0xf000, 0xe121,
		0x0cf6, 0xe3a0, 0xd804, 0xe080, 0x00d7, 0xe3a0, 0xf000, 0xe121,
		0x0cff, 0xe3a0, 0xd804, 0xe080, 0x00db, 0xe3a0, 0xf000, 0xe121,
		0x4140, 0xe1c4, 0x0cfe, 0xe3a0, 0xd804, 0xe080, 0x00d3, 0xe3a0,
		0xf000, 0xe121, 0x4a01, 0xe3a0, 0x0b01, 0xe3a0, 0xd804, 0xe080,
		0x5a0f, 0xe3a0, 0x0008, 0xe3a0, 0x8805, 0xe080, 0x0010, 0xe3a0,
		0x0000, 0xe5c8, 0x7805, 0xe1a0, 0x6a01, 0xe3a0, 0x0012, 0xe3a0,
		0x0a02, 0xe280, 0x6806, 0xe080, 0x6000, 0xe587, 0x00d3, 0xe3a0,
		0xf000, 0xe121, 0x4001, 0xe3a0, 0x4b06, 0xe284, 0x0cf2, 0xe3a0,
		0xd804, 0xe080, 0x0013, 0xe3a0, 0xf000, 0xe121, 0x0028, 0xea00,
		0xfffe, 0xeaff, 0xfffe, 0xeaff, 0xe004, 0xe52d, 0x00d3, 0xe3a0,
		0xf000, 0xe121, 0xe004, 0xe49d, 0xff1e, 0xe12f, 0xe004, 0xe52d,
		0x0013, 0xe3a0, 0xf000, 0xe121, 0xe004, 0xe49d, 0xff1e, 0xe12f,
		0x00d1, 0xe3a0, 0xf000, 0xe121, 0xd0b8, 0xe59f, 0x00d3, 0xe3a0,
		0xf000, 0xe121, 0xd0b0, 0xe59f, 0x10b8, 0xe59f, 0x0000, 0xe3a0,
		0x0000, 0xe581, 0xf0b0, 0xe59f, 0xfffe, 0xeaff, 0xfffe, 0xeaff,
		0xfffe, 0xeaff, 0xfffe, 0xeaff, 0xfffe, 0xeaff, 0xfffe, 0xeaff,
		0xff1e, 0xe12f, 0xfffe, 0xeaff, 0xfffe, 0xeaff, 0xfffe, 0xeaff,
		0xfffe, 0xeaff, 0xfffe, 0xeaff, 0xfffe, 0xeaff, 0xfffe, 0xeaff,
		0xfffe, 0xeaff, 0xfffe, 0xeaff, 0xfffe, 0xeaff, 0xfffe, 0xeaff,
		0xfffe, 0xeaff, 0x105c, 0xe59f
	};

	for (int i = 0; i < 0x188/2; i++) {
		svgEOHackData[i] = BURN_ENDIAN_SWAP_INT16(svgEOHackData[i]);
	}

	memcpy  (PGMARMROM, svgEOHackData, 0x188);
}

static void svgPatch()
{
	pgm_decrypt_svg();
	pgm_create_svg_EO_data();
}

static INT32 svgInit()
{
	pPgmInitCallback = svgPatch;
	pPgmProtCallback = install_protection_asic27a_svg;

	nPgmAsicRegionHackAddress = 0x3c3e;

	INT32 nRet = pgmInit();
	
	Arm7SetIdleLoopAddress(0x0000B90);

	return nRet;
}

struct BurnDriver BurnDrvSvg = {
	"svg", NULL, "pgm", NULL, "2005",
	"S.V.G. - Spectral vs Generation (V200, China)\0", NULL, "IGS", "PolyGameMaster",
	L"S.V.G. - Spectral vs Generation\0\u5723\u9B54\u4E16\u7EAA (V200, China)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, svgRomInfo, svgRomName, NULL, NULL, NULL, NULL, pgmInputInfo, svgDIPInfo,
	svgInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// S.V.G. - Spectral vs Generation (V100, Taiwan)

static struct BurnRomInfo svgtwRomDesc[] = {
	{ "v101tw_u30.u30",		0x080000, 0x8d0405e4, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "igs_t05601w016.u29",	0x200000, 0x03e110dc, 2 | BRF_GRA },		//  1 Tile data

	{ "a05601w064.bin",		0x800000, 0xea6453e4, 3 | BRF_GRA },		//  2 Sprite Color Data
	{ "a05602w064.bin",		0x800000, 0x6d00621b, 3 | BRF_GRA },		//  3
	{ "a05603w064.bin",		0x800000, 0x7b71c64f, 3 | BRF_GRA },		//  4
	{ "a05604w032.bin",		0x400000, 0x9452a567, 3 | BRF_GRA },		//  5

	{ "b05601w064.bin",		0x800000, 0x35c0a489, 4 | BRF_GRA },		//  6 Sprite Masks & Color Indexes
	{ "b05602w064.bin",		0x800000, 0x8aad3f85, 4 | BRF_GRA },		//  7

	{ "w05601b064.bin",		0x800000, 0xbfe61a71, 5 | BRF_SND },		//  8 Samples
	{ "w05602b032.bin",		0x400000, 0x0685166d, 5 | BRF_SND },		//  9

//	{ "svg_igs027a.bin",			0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	// 10 Internal ARM7 Rom
	{ "svgpcb_igs027a_execute_only_area", 	0x000188, 0x00000000, 0 | BRF_OPT | BRF_NODUMP },	// 10 Internal ARM7 Rom
	{ "svgcpb_igs027a_v100_japan.bin", 	0x003e78, 0x7a59da5d, 7 | BRF_PRG | BRF_ESS },		// 11 Internal ARM7 Rom

	{ "v101tw_u26.u26",		0x400000, 0xcc24f542, 8 | BRF_PRG | BRF_ESS },	// 12 External ARM7 Rom
	{ "v101tw_u36.u36",		0x400000, 0xf18283e2, 8 | BRF_PRG | BRF_ESS },	// 13
};

STDROMPICKEXT(svgtw, svgtw, pgm)
STD_ROM_FN(svgtw)

static void svgtwPatch()
{
	pgm_decrypt_svgpcb();
	pgm_create_theglad_EO_data();
}

static INT32 svgtwInit()
{
	pPgmInitCallback = svgtwPatch;
	pPgmProtCallback = install_protection_asic27a_svg;

	nPgmAsicRegionHackAddress = 0x3a8e;

	INT32 nRet = pgmInit();
	
	Arm7SetIdleLoopAddress(0x00009e0);

	return nRet;
}

struct BurnDriverD BurnDrvSvgtw = {
	"svgtw", "svg", "pgm", NULL, "2005",
	"S.V.G. - Spectral vs Generation (V100, Taiwan)\0", "Incomplete dump", "IGS", "PolyGameMaster",
	L"S.V.G. - Spectral vs Generation\0\u5723\u9B54\u4E16\u7EAA (V100, Taiwan)\0", NULL, NULL, NULL,
	BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, svgtwRomInfo, svgtwRomName, NULL, NULL, NULL, NULL, pgmInputInfo, svgtwDIPInfo,
	svgtwInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// -----------------------------------------------------------------------------
// PCB Versions


// DoDonPachi III (World, 2002.05.15 Master Ver)

static struct BurnRomInfo ddp3RomDesc[] = {
	{ "ddp3_v101_16m.u36",		0x200000, 0xfba2180e, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t04401w064.u19",			0x800000, 0x3a95f19c, 2 | BRF_GRA },			//  1 Tile data

	{ "a04401w064.u7",			0x800000, 0xed229794, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a04402w064.u8",			0x800000, 0x752167b0, 3 | BRF_GRA },			//  3

	{ "b04401w064.u1",			0x800000, 0x8cbff066, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "m04401b032.u17",			0x400000, 0x5a0dbd76, 5 | BRF_SND },			//  5 Samples

	{ "ddp3_igs027a.bin",		0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  6 Internal ARM7 Rom
	
	{ "ddp3_defaults.nv",		0x020000, 0x571e96c0, 0 | BRF_OPT },			//  7 NV RAM
};

STDROMPICKEXT(ddp3, ddp3, ddp3Bios) // custom bios
STD_ROM_FN(ddp3)

static INT32 ddp3Init()
{
	pPgmInitCallback = pgm_decrypt_py2k2;
	pPgmProtCallback = install_protection_asic27a_ddp3; // simulation
	pgm_cave_refresh = 1;

	return pgmInit();
}

struct BurnDriver BurnDrvDdp3 = {
	"ddp3", NULL, NULL,  NULL, "2002",
	"DoDonPachi III (World, 2002.05.15 Master Ver)\0", NULL, "Cave (AMI license)", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL, 4, HARDWARE_IGS_PGM /* | HARDWARE_IGS_USE_ARM_CPU */, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp3RomInfo, ddp3RomName, NULL, NULL, NULL, NULL, pgmInputInfo, jammaDIPInfo,
	ddp3Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224, 448, 3, 4
};


// DoDonPachi Dai-Ou-Jou (V101, Japan)

static struct BurnRomInfo ddp3aRomDesc[] = {
	{ "ddp3_v101.u36",			0x200000, 0x195b5c1e, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t04401w064.u19",			0x800000, 0x3a95f19c, 2 | BRF_GRA },			//  1 Tile data

	{ "a04401w064.u7",			0x800000, 0xed229794, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a04402w064.u8",			0x800000, 0x752167b0, 3 | BRF_GRA },			//  3

	{ "b04401w064.u1",			0x800000, 0x8cbff066, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "m04401b032.u17",			0x400000, 0x5a0dbd76, 5 | BRF_SND },			//  5 Samples

	{ "ddp3_igs027a.bin",		0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  6 Internal ARM7 Rom
	
	{ "ddp3_defaults.nv",		0x020000, 0x571e96c0, 0 | BRF_OPT },			//  7 NV RAM
};

STDROMPICKEXT(ddp3a, ddp3a, ddp3Bios) // custom bios
STD_ROM_FN(ddp3a)

struct BurnDriver BurnDrvDdp3a = {
	"ddpdoj", "ddp3", NULL,  NULL, "2002",
	"DoDonPachi Dai-Ou-Jou (V101, Japan)\0", NULL, "Cave (AMI license)", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE | BDF_ORIENTATION_VERTICAL, 4, HARDWARE_IGS_PGM /* | HARDWARE_IGS_USE_ARM_CPU */, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp3aRomInfo, ddp3aRomName, NULL, NULL, NULL, NULL, pgmInputInfo, jammaDIPInfo,
	ddp3Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224, 448, 3, 4
};


// DoDonPachi Dai-Ou-Jou (V100 (second version), Japan)

static struct BurnRomInfo ddp3bRomDesc[] = {
	{ "ddp3_d_d_1_0.u36",		0x200000, 0x5D3F85BA, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t04401w064.u19",			0x800000, 0x3a95f19c, 2 | BRF_GRA },			//  1 Tile data

	{ "a04401w064.u7",			0x800000, 0xed229794, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a04402w064.u8",			0x800000, 0x752167b0, 3 | BRF_GRA },			//  3

	{ "b04401w064.u1",			0x800000, 0x8cbff066, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "m04401b032.u17",			0x400000, 0x5a0dbd76, 5 | BRF_SND },	 		//  5 Samples

	{ "ddp3_igs027a.bin",		0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	//  6 Internal ARM7 Rom
	
	{ "ddp3_defaults.nv",		0x020000, 0x571e96c0, 0 | BRF_OPT },			//  7 NV RAM
};

STDROMPICKEXT(ddp3b, ddp3b, ddp3Bios) // custom bios
STD_ROM_FN(ddp3b)

struct BurnDriver BurnDrvDdp3b = {
	"ddpdoja", "ddp3", NULL,  NULL, "2002",
	"DoDonPachi Dai-Ou-Jou (V100 (second version), Japan)\0", NULL, "Cave (AMI license)", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE | BDF_ORIENTATION_VERTICAL, 4, HARDWARE_IGS_PGM /* | HARDWARE_IGS_USE_ARM_CPU */, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp3bRomInfo, ddp3bRomName, NULL, NULL, NULL, NULL, pgmInputInfo, jammaDIPInfo,
	ddp3Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224, 448, 3, 4
};


// DoDonPachi Dai-Ou-Jou (V100 (first version), Japan)

static struct BurnRomInfo ddp3cRomDesc[] = {
	{ "dd v100.bin",			0x200000, 0x7da0c1e4, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t04401w064.u19",			0x800000, 0x3a95f19c, 2 | BRF_GRA },			//  1 Tile data

	{ "a04401w064.u7",			0x800000, 0xed229794, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a04402w064.u8",			0x800000, 0x752167b0, 3 | BRF_GRA },			//  3

	{ "b04401w064.u1",			0x800000, 0x8cbff066, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "m04401b032.u17",			0x400000, 0x5a0dbd76, 5 | BRF_SND },			//  5 Samples

	{ "ddp3_igs027a.bin",		0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  6 Internal ARM7 Rom
	
	{ "ddp3_defaults.nv",		0x020000, 0x571e96c0, 0 | BRF_OPT },			//  7 NV RAM
};

STDROMPICKEXT(ddp3c, ddp3c, ddp3Bios) // custom bios
STD_ROM_FN(ddp3c)

struct BurnDriver BurnDrvDdp3c = {
	"ddpdojb", "ddp3", NULL,  NULL, "2002",
	"DoDonPachi Dai-Ou-Jou (V100 (first version), Japan)\0", NULL, "Cave (AMI license)", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE | BDF_ORIENTATION_VERTICAL, 4, HARDWARE_IGS_PGM /* | HARDWARE_IGS_USE_ARM_CPU */, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp3cRomInfo, ddp3cRomName, NULL, NULL, NULL, NULL, pgmInputInfo, jammaDIPInfo,
	ddp3Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224, 448, 3, 4
};


// DoDonPachi Dai-Ou-Jou Black Label (V100 (2002.10.07.Black Ver), Japan)

static struct BurnRomInfo ddp3blkRomDesc[] = {
	{ "ddb_1dot.u45",			0x200000, 0x265f26cd, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t04401w064.u19",			0x800000, 0x3a95f19c, 2 | BRF_GRA },			//  1 Tile data

	{ "a04401w064.u7",			0x800000, 0xed229794, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a04402w064.u8",			0x800000, 0x752167b0, 3 | BRF_GRA },			//  3

	{ "b04401w064.u1",			0x800000, 0x8cbff066, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "m04401b032.u17",			0x400000, 0x5a0dbd76, 5 | BRF_SND },			//  5 Samples

	{ "ddp3_igs027a.bin",		0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  6 Internal ARM7 Rom

	{ "ddp3blk_defaults.nv",	0x020000, 0xa1651904, 0 | BRF_OPT },			//  7 NV RAM (patch below instead)
};

STDROMPICKEXT(ddp3blk, ddp3blk, ddp3Bios) // custom bios
STD_ROM_FN(ddp3blk)

static void ddp3blkPatchRAM()
{
	SekOpen(0);
	SekWriteLong(0x803800, 0x95804803);
	SekWriteLong(0x803804, 0x23879065);
	SekClose();

	// enable asic test
//	*((UINT16*)(PGM68KROM + 0x03c0f4)) = BURN_ENDIAN_SWAP_INT16(0x0012);
}

static INT32 ddp3blkInit()
{
	pPgmInitCallback = pgm_decrypt_py2k2;
	pPgmProtCallback = install_protection_asic27a_ddp3; // simulation
	pgm_cave_refresh = 1;

	INT32 nRet = pgmInit();

	if (nRet == 0) {
		ddp3blkPatchRAM();
	}

	return nRet;
}

struct BurnDriver BurnDrvDdp3blk = {
	"ddpdojblk", "ddp3", NULL, NULL, "2002",
	"DoDonPachi Dai-Ou-Jou Black Label (V100, (2002.10.07.Black Ver), Japan)\0", NULL, "Cave (AMI license)", "PolyGameMaster",
	L"DoDonPachi Dai-Ou-Jou Black Label\0\u6012\u9996\u9818\u8702 \u5927\u5F80\u751F Black Label (V100 (2002.10.07.Black Ver), Japan)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL | BDF_CLONE, 4, HARDWARE_IGS_PGM /* | HARDWARE_IGS_USE_ARM_CPU */, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp3blkRomInfo, ddp3blkRomName, NULL, NULL, NULL, NULL, pgmInputInfo, jammaDIPInfo,
	ddp3blkInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224, 448, 3, 4
};


// DoDonPachi Dai-Ou-Jou Black Label (V100 (2002.10.07 Black Ver), Japan)

static struct BurnRomInfo ddp3blkaRomDesc[] = {
	{ "ddb10.u45",				0x200000, 0x72b35510, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t04401w064.u19",			0x800000, 0x3a95f19c, 2 | BRF_GRA },			//  1 Tile data

	{ "a04401w064.u7",			0x800000, 0xed229794, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a04402w064.u8",			0x800000, 0x752167b0, 3 | BRF_GRA },			//  3

	{ "b04401w064.u1",			0x800000, 0x8cbff066, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "m04401b032.u17",			0x400000, 0x5a0dbd76, 5 | BRF_SND },			//  5 Samples

	{ "ddp3_igs027a.bin",		0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  6 Internal ARM7 Rom

	{ "ddp3blk_defaults.nv",	0x020000, 0xa1651904, 0 | BRF_OPT },			//  7 NV RAM (patch below instead)
};

STDROMPICKEXT(ddp3blka, ddp3blka, ddp3Bios) // custom bios
STD_ROM_FN(ddp3blka)

struct BurnDriver BurnDrvDdp3blka = {
	"ddpdojblka", "ddp3", NULL, NULL, "2002",
	"DoDonPachi Dai-Ou-Jou Black Label (V100 (2002.10.07 Black Ver), Japan)\0", NULL, "Cave (AMI license)", "PolyGameMaster",
	L"DoDonPachi Dai-Ou-Jou Black Label\0\u6012\u9996\u9818\u8702 \u5927\u5F80\u751F Black Label (V100 (2002.10.07 Black Ver), Japan)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL | BDF_CLONE, 4, HARDWARE_IGS_PGM /* | HARDWARE_IGS_USE_ARM_CPU */, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddp3blkaRomInfo, ddp3blkaRomName, NULL, NULL, NULL, NULL, pgmInputInfo, jammaDIPInfo,
	ddp3blkInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224, 448, 3, 4
};


// Ketsui Kizuna Jigoku Tachi (Revision 2?, Japan)

static struct BurnRomInfo ketRomDesc[] = {
	{ "ketsui_v100.u38",		0x200000, 0xdfe62f3b, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code
	
	{ "t04701w064.u19", 		0x800000, 0x2665b041, 2 | BRF_GRA },			//  1 Tile data

	{ "a04701w064.u7", 			0x800000, 0x5ef1b94b, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a04702w064.u8", 			0x800000, 0x26d6da7f, 3 | BRF_GRA },			//  3

	{ "b04701w064.u1",			0x800000, 0x1bec008d, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "m04701b032.u17",			0x400000, 0xb46e22d1, 5 | BRF_SND },			//  5 Samples

	{ "ket_igs027a.bin",		0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	//  6 Internal ARM7 Rom
	
	{ "ket_defaults.nv",		0x020000, 0x3ca892d8, 0 | BRF_OPT },			//  7 NV RAM
};

STDROMPICKEXT(ket, ket, ketsuiBios) // custom bios
STD_ROM_FN(ket)

static INT32 ketsuiInit()
{
	pPgmInitCallback = pgm_decrypt_ketsui;
	pPgmProtCallback = install_protection_asic27a_ketsui;  // simulation

	return pgmInit();
}

struct BurnDriver BurnDrvKet = {
	"ket", NULL, NULL, NULL, "2002",
	"Ketsui Kizuna Jigoku Tachi (Revision 2?, Japan)\0", NULL, "CAVE / AMI", "PolyGameMaster based",
	L"Ketsui Kizuna Jigoku Tachi\0\u30B1\u30C4\u30A4~\u7D46\u5730\u7344\u305F\u3061 (Revision 2?, Japan)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_JAMMAPCB/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_VERSHOOT, 0,
	NULL, ketRomInfo, ketRomName, NULL, NULL, NULL, NULL, pgmInputInfo, jammaDIPInfo,
	ketsuiInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224, 448, 3, 4
};


// Ketsui Kizuna Jigoku Tachi (Revision 2? alt, Japan)

static struct BurnRomInfo ket1RomDesc[] = {
	// alt version with 0xff fill in the unused area after 0x1443bc rather than random data, there are no code changes.
	{ "ketsui_v100_alt_fill.u38",	0x200000, 0xe140f8a4, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code
	
	{ "t04701w064.u19", 		0x800000, 0x2665b041, 2 | BRF_GRA },				//  1 Tile data

	{ "a04701w064.u7", 			0x800000, 0x5ef1b94b, 3 | BRF_GRA },				//  2 Sprite Color Data
	{ "a04702w064.u8", 			0x800000, 0x26d6da7f, 3 | BRF_GRA },				//  3

	{ "b04701w064.u1",			0x800000, 0x1bec008d, 4 | BRF_GRA },				//  4 Sprite Masks & Color Indexes

	{ "m04701b032.u17",			0x400000, 0xb46e22d1, 5 | BRF_SND },				//  5 Samples

	{ "ket_igs027a.bin",		0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	//  6 Internal ARM7 Rom
	
	{ "ket_defaults.nv",		0x020000, 0x3ca892d8, 0 | BRF_OPT },				//  7 NV RAM
};

STDROMPICKEXT(ket1, ket1, ketsuiBios) // custom bios
STD_ROM_FN(ket1)

struct BurnDriver BurnDrvKet1 = {
	"ket1", "ket", NULL, NULL, "2002",
	"Ketsui Kizuna Jigoku Tachi (Revision 2? alt, Japan)\0", NULL, "CAVE / AMI", "PolyGameMaster based",
	L"Ketsui Kizuna Jigoku Tachi\0\u30B1\u30C4\u30A4~\u7D46\u5730\u7344\u305F\u3061 (Revision 2? alt, Japan)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE | BDF_ORIENTATION_VERTICAL, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_JAMMAPCB/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_VERSHOOT, 0,
	NULL, ket1RomInfo, ket1RomName, NULL, NULL, NULL, NULL, pgmInputInfo, jammaDIPInfo,
	ketsuiInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224, 448, 3, 4
};


// Ketsui Kizuna Jigoku Tachi (Revision 1?, Japan)

static struct BurnRomInfo ketaRomDesc[] = {
	{ "ketsui_prg_revised.bin",	0x200000, 0x69fcf5eb, 1 | BRF_PRG | BRF_ESS },		//  0 68K Code

	{ "t04701w064.u19", 		0x800000, 0x2665b041, 2 | BRF_GRA },				//  1 Tile data

	{ "a04701w064.u7", 			0x800000, 0x5ef1b94b, 3 | BRF_GRA },				//  2 Sprite Color Data
	{ "a04702w064.u8", 			0x800000, 0x26d6da7f, 3 | BRF_GRA },				//  3

	{ "b04701w064.u1",			0x800000, 0x1bec008d, 4 | BRF_GRA },				//  4 Sprite Masks & Color Indexes

	{ "m04701b032.u17",			0x400000, 0xb46e22d1, 5 | BRF_SND },				//  5 Samples

	{ "ket_igs027a.bin",		0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	//  6 Internal ARM7 Rom
	
	{ "ket_defaults.nv",		0x020000, 0x3ca892d8, 0 | BRF_OPT },				//  7 NV RAM
};

STDROMPICKEXT(keta, keta, ketsuiBios) // custom bios
STD_ROM_FN(keta)

struct BurnDriver BurnDrvKeta = {
	"keta", "ket", NULL, NULL, "2002",
	"Ketsui Kizuna Jigoku Tachi (Revision 1?, Japan)\0", NULL, "CAVE / AMI", "PolyGameMaster based",
	L"Ketsui Kizuna Jigoku Tachi\0\u30B1\u30C4\u30A4~\u7D46\u5730\u7344\u305F\u3061 (Revision 1?, Japan)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE | BDF_ORIENTATION_VERTICAL, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_JAMMAPCB/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_VERSHOOT, 0,
	NULL, ketaRomInfo, ketaRomName, NULL, NULL, NULL, NULL, pgmInputInfo, jammaDIPInfo,
	ketsuiInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224, 448, 3, 4
};


// Ketsui Kizuna Jigoku Tachi (Original?, Japan)

static struct BurnRomInfo ketbRomDesc[] = {
	{ "ketsui_prg_original.bin",	0x200000, 0xcca5e153, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t04701w064.u19", 		0x800000, 0x2665b041, 2 | BRF_GRA },				//  1 Tile data

	{ "a04701w064.u7", 			0x800000, 0x5ef1b94b, 3 | BRF_GRA },				//  2 Sprite Color Data
	{ "a04702w064.u8", 			0x800000, 0x26d6da7f, 3 | BRF_GRA },				//  3

	{ "b04701w064.u1",			0x800000, 0x1bec008d, 4 | BRF_GRA },				//  4 Sprite Masks & Color Indexes

	{ "m04701b032.u17",			0x400000, 0xb46e22d1, 5 | BRF_SND },				//  5 Samples

	{ "ket_igs027a.bin",		0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	//  6 Internal ARM7 Rom
	
	{ "ket_defaults.nv",		0x020000, 0x3ca892d8, 0 | BRF_OPT },				//  7 NV RAM
};

STDROMPICKEXT(ketb, ketb, ketsuiBios) // custom bios
STD_ROM_FN(ketb)

struct BurnDriver BurnDrvKetb = {
	"ketb", "ket", NULL, NULL, "2002",
	"Ketsui Kizuna Jigoku Tachi (Original?, Japan)\0", NULL, "CAVE / AMI", "PolyGameMaster based",
	L"Ketsui Kizuna Jigoku Tachi\0\u30B1\u30C4\u30A4~\u7D46\u5730\u7344\u305F\u3061 (Original?, Japan)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE | BDF_ORIENTATION_VERTICAL, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_JAMMAPCB/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_VERSHOOT, 0,
	NULL, ketbRomInfo, ketbRomName, NULL, NULL, NULL, NULL, pgmInputInfo, jammaDIPInfo,
	ketsuiInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224, 448, 3, 4
};


// Ketsui Kizuna Jigoku Tachi (Arrange Mode version 1.7, hack by Trap15)

static struct BurnRomInfo ketarrRomDesc[] = {
	{ "ketarr17_v100.u38",		0x200000, 0x2cb80b89, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t04701w064.u19", 		0x800000, 0x2665b041, 2 | BRF_GRA },			//  1 Tile data

	{ "a04701w064.u7", 			0x800000, 0x5ef1b94b, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a04702w064.u8", 			0x800000, 0x26d6da7f, 3 | BRF_GRA },			//  3

	{ "b04701w064.u1",			0x800000, 0x1bec008d, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "m04701b032.u17",			0x400000, 0xb46e22d1, 5 | BRF_SND },			//  5 Samples

	{ "ket_igs027a.bin",		0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	//  6 Internal ARM7 Rom 
	
	{ "ket_defaults.nv",		0x020000, 0x3ca892d8, 0 | BRF_OPT },			//  7 NV RAM
};

STDROMPICKEXT(ketarr, ketarr, ketsuiBios) // custom bios
STD_ROM_FN(ketarr)

struct BurnDriver BurnDrvketarr = {
	"ketarr", "ket", NULL, NULL, "2014",
	"Ketsui Kizuna Jigoku Tachi (Arrange Mode version 1.7, hack by Trap15)\0", NULL, "hack / Trap15", "PolyGameMaster based",
	L"Ketsui Kizuna Jigoku Tachi\0\u30B1\u30C4\u30A4~\u7D46\u5730\u7344\u305F\u3061 (Arrange Mode version 1.7, hack by Trap15)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE | BDF_ORIENTATION_VERTICAL | BDF_HACK, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_JAMMAPCB/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_VERSHOOT, 0,
	NULL, ketarrRomInfo, ketarrRomName, NULL, NULL, NULL, NULL, pgmInputInfo, jammaDIPInfo,
	ketsuiInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224, 448, 3, 4
};


// Ketsui Kizuna Jigoku Tachi (Arrange Mode version 1.0, hack by Trap15)

static struct BurnRomInfo ketarr10RomDesc[] = {
	{ "ketarr_v100.u38",		0x200000, 0xd4c7a8ab, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t04701w064.u19", 		0x800000, 0x2665b041, 2 | BRF_GRA },			//  1 Tile data

	{ "a04701w064.u7", 			0x800000, 0x5ef1b94b, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a04702w064.u8", 			0x800000, 0x26d6da7f, 3 | BRF_GRA },			//  3

	{ "b04701w064.u1",			0x800000, 0x1bec008d, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "m04701b032.u17",			0x400000, 0xb46e22d1, 5 | BRF_SND },			//  5 Samples

	{ "ket_igs027a.bin",		0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	//  6 Internal ARM7 Rom
	
	{ "ket_defaults.nv",		0x020000, 0x3ca892d8, 0 | BRF_OPT },			//  7 NV RAM
};

STDROMPICKEXT(ketarr10, ketarr10, ketsuiBios) // custom bios
STD_ROM_FN(ketarr10)

struct BurnDriver BurnDrvketarr10 = {
	"ketarr10", "ket", NULL, NULL, "2012",
	"Ketsui Kizuna Jigoku Tachi (Arrange Mode version 1.0, hack by Trap15)\0", NULL, "hack / Trap15", "PolyGameMaster based",
	L"Ketsui Kizuna Jigoku Tachi\0\u30B1\u30C4\u30A4~\u7D46\u5730\u7344\u305F\u3061 (Arrange Mode version 1.0, hack by Trap15)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE | BDF_ORIENTATION_VERTICAL | BDF_HACK, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_JAMMAPCB/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_VERSHOOT, 0,
	NULL, ketarr10RomInfo, ketarr10RomName, NULL, NULL, NULL, NULL, pgmInputInfo, jammaDIPInfo,
	ketsuiInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224, 448, 3, 4
};


// Ketsui Kizuna Jigoku Tachi (Arrange Mode version 1.5, hack by Trap15)

static struct BurnRomInfo ketarr15RomDesc[] = {
	{ "ketarr15_v100.u38",		0x200000, 0x552a7d95, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t04701w064.u19", 		0x800000, 0x2665b041, 2 | BRF_GRA },			//  1 Tile data

	{ "a04701w064.u7", 			0x800000, 0x5ef1b94b, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a04702w064.u8", 			0x800000, 0x26d6da7f, 3 | BRF_GRA },			//  3

	{ "b04701w064.u1",			0x800000, 0x1bec008d, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "m04701b032.u17",			0x400000, 0xb46e22d1, 5 | BRF_SND },			//  5 Samples

	{ "ket_igs027a.bin",		0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	//  6 Internal ARM7 Rom
	
	{ "ket_defaults.nv",		0x020000, 0x3ca892d8, 0 | BRF_OPT },			//  7 NV RAM
};

STDROMPICKEXT(ketarr15, ketarr15, ketsuiBios) // custom bios
STD_ROM_FN(ketarr15)

struct BurnDriver BurnDrvketarr15 = {
	"ketarr15", "ket", NULL, NULL, "2012",
	"Ketsui Kizuna Jigoku Tachi (Arrange Mode version 1.5, hack by Trap15)\0", NULL, "hack / Trap15", "PolyGameMaster based",
	L"Ketsui Kizuna Jigoku Tachi\0\u30B1\u30C4\u30A4~\u7D46\u5730\u7344\u305F\u3061 (Arrange Mode version 1.5, hack by Trap15)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE | BDF_ORIENTATION_VERTICAL | BDF_HACK, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_JAMMAPCB/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_VERSHOOT, 0,
	NULL, ketarr15RomInfo, ketarr15RomName, NULL, NULL, NULL, NULL, pgmInputInfo, jammaDIPInfo,
	ketsuiInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224, 448, 3, 4
};


// Ketsui Kizuna Jigoku Tachi (Arrange Mode version 1.51, hack by Trap15)

static struct BurnRomInfo ketarr151RomDesc[] = {
	{ "ketarr151_v100.u38",		0x200000, 0x2b7c030d, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t04701w064.u19", 		0x800000, 0x2665b041, 2 | BRF_GRA },			//  1 Tile data

	{ "a04701w064.u7", 			0x800000, 0x5ef1b94b, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a04702w064.u8", 			0x800000, 0x26d6da7f, 3 | BRF_GRA },			//  3

	{ "b04701w064.u1",			0x800000, 0x1bec008d, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "m04701b032.u17",			0x400000, 0xb46e22d1, 5 | BRF_SND },			//  5 Samples

	{ "ket_igs027a.bin",		0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	//  6 Internal ARM7 Rom
	
	{ "ket_defaults.nv",		0x020000, 0x3ca892d8, 0 | BRF_OPT },			//  7 NV RAM
};

STDROMPICKEXT(ketarr151, ketarr151, ketsuiBios) // custom bios
STD_ROM_FN(ketarr151)

struct BurnDriver BurnDrvketarr151 = {
	"ketarr151", "ket", NULL, NULL, "2012",
	"Ketsui Kizuna Jigoku Tachi (Arrange Mode version 1.51, hack by Trap15)\0", NULL, "hack / Trap15", "PolyGameMaster based",
	L"Ketsui Kizuna Jigoku Tachi\0\u30B1\u30C4\u30A4~\u7D46\u5730\u7344\u305F\u3061 (Arrange Mode version 1.51, hack by Trap15)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE | BDF_ORIENTATION_VERTICAL | BDF_HACK, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_JAMMAPCB/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_VERSHOOT, 0,
	NULL, ketarr151RomInfo, ketarr151RomName, NULL, NULL, NULL, NULL, pgmInputInfo, jammaDIPInfo,
	ketsuiInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224, 448, 3, 4
};


// Ketsui Kizuna Jigoku Tachi (MR. Stoic version 1.5, hack by Trap15)

static struct BurnRomInfo ketarrs15RomDesc[] = {
	{ "ketarrs15_v100.u38",		0x200000, 0xa95e71e0, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t04701w064.u19", 		0x800000, 0x2665b041, 2 | BRF_GRA },			//  1 Tile data

	{ "a04701w064.u7", 			0x800000, 0x5ef1b94b, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a04702w064.u8", 			0x800000, 0x26d6da7f, 3 | BRF_GRA },			//  3

	{ "b04701w064.u1",			0x800000, 0x1bec008d, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "m04701b032.u17",			0x400000, 0xb46e22d1, 5 | BRF_SND },			//  5 Samples

	{ "ket_igs027a.bin",		0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	//  6 Internal ARM7 Rom
	
	{ "ket_defaults.nv",		0x020000, 0x3ca892d8, 0 | BRF_OPT },			//  7 NV RAM
};

STDROMPICKEXT(ketarrs15, ketarrs15, ketsuiBios) // custom bios
STD_ROM_FN(ketarrs15)

struct BurnDriver BurnDrvketarrs15 = {
	"ketarrs15", "ket", NULL, NULL, "2012",
	"Ketsui Kizuna Jigoku Tachi (MR. Stoic version 1.5, hack by Trap15)\0", NULL, "hack / Trap15", "PolyGameMaster based",
	L"Ketsui Kizuna Jigoku Tachi\0\u30B1\u30C4\u30A4~\u7D46\u5730\u7344\u305F\u3061 (MR. Stoic version 1.5, hack by Trap15)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE | BDF_ORIENTATION_VERTICAL | BDF_HACK, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_JAMMAPCB/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_VERSHOOT, 0,
	NULL, ketarrs15RomInfo, ketarrs15RomName, NULL, NULL, NULL, NULL, pgmInputInfo, jammaDIPInfo,
	ketsuiInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224, 448, 3, 4
};


// Ketsui Kizuna Jigoku Tachi (MR. Stoic version 1.51, hack by Trap15)

static struct BurnRomInfo ketarrs151RomDesc[] = {
	{ "ketarrs151_v100.u38",	0x200000, 0x35c984e4, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t04701w064.u19", 		0x800000, 0x2665b041, 2 | BRF_GRA },			//  1 Tile data

	{ "a04701w064.u7", 			0x800000, 0x5ef1b94b, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a04702w064.u8", 			0x800000, 0x26d6da7f, 3 | BRF_GRA },			//  3

	{ "b04701w064.u1",			0x800000, 0x1bec008d, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "m04701b032.u17",			0x400000, 0xb46e22d1, 5 | BRF_SND },			//  5 Samples

	{ "ket_igs027a.bin",		0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	//  6 Internal ARM7 Rom
	
	{ "ket_defaults.nv",		0x020000, 0x3ca892d8, 0 | BRF_OPT },			//  7 NV RAM
};

STDROMPICKEXT(ketarrs151, ketarrs151, ketsuiBios) // custom bios
STD_ROM_FN(ketarrs151)

struct BurnDriver BurnDrvketarrs151 = {
	"ketarrs151", "ket", NULL, NULL, "2012",
	"Ketsui Kizuna Jigoku Tachi (MR. Stoic version 1.51, hack by Trap15)\0", NULL, "hack / Trap15", "PolyGameMaster based",
	L"Ketsui Kizuna Jigoku Tachi\0\u30B1\u30C4\u30A4~\u7D46\u5730\u7344\u305F\u3061 (MR. Stoic version 1.51, hack by Trap15)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE | BDF_ORIENTATION_VERTICAL | BDF_HACK, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_JAMMAPCB/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_VERSHOOT, 0,
	NULL, ketarrs151RomInfo, ketarrs151RomName, NULL, NULL, NULL, NULL, pgmInputInfo, jammaDIPInfo,
	ketsuiInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224, 448, 3, 4
};


// Ketsui Kizuna Jigoku Tachi (Fast version, hack by Trap15)

static struct BurnRomInfo ketarrfRomDesc[] = {
	{ "ketarrf_v100.u38",		0x200000, 0x6ad17aa4, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t04701w064.u19", 		0x800000, 0x2665b041, 2 | BRF_GRA },			//  1 Tile data

	{ "a04701w064.u7", 			0x800000, 0x5ef1b94b, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a04702w064.u8", 			0x800000, 0x26d6da7f, 3 | BRF_GRA },			//  3

	{ "b04701w064.u1",			0x800000, 0x1bec008d, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "m04701b032.u17",			0x400000, 0xb46e22d1, 5 | BRF_SND },			//  5 Samples

	{ "ket_igs027a.bin",		0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	//  6 Internal ARM7 Rom
	
	{ "ket_defaults.nv",		0x020000, 0x3ca892d8, 0 | BRF_OPT },			//  7 NV RAM
};

STDROMPICKEXT(ketarrf, ketarrf, ketsuiBios) // custom bios
STD_ROM_FN(ketarrf)

struct BurnDriver BurnDrvketarrf = {
	"ketarrf", "ket", NULL, NULL, "2012",
	"Ketsui Kizuna Jigoku Tachi (Fast version, hack by Trap15)\0", NULL, "hack / Trap15", "PolyGameMaster based",
	L"Ketsui Kizuna Jigoku Tachi\0\u30B1\u30C4\u30A4~\u7D46\u5730\u7344\u305F\u3061 (Fast version, hack by Trap15)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE | BDF_ORIENTATION_VERTICAL | BDF_HACK, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_JAMMAPCB/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_VERSHOOT, 0,
	NULL, ketarrfRomInfo, ketarrfRomName, NULL, NULL, NULL, NULL, pgmInputInfo, jammaDIPInfo,
	ketsuiInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224, 448, 3, 4
};


// Espgaluda (V100, Japan)

static struct BurnRomInfo espgalRomDesc[] = {
	{ "espgaluda_v100.u38",		0x200000, 0x08ecec34, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t04801w064.u19",	   		0x800000, 0x6021c79e, 2 | BRF_GRA },				//  1 Tile data

	{ "a04801w064.u7",			0x800000, 0x26dd4932, 3 | BRF_GRA },				//  2 Sprite Color Data
	{ "a04802w064.u8",	   		0x800000, 0x0e6bf7a9, 3 | BRF_GRA },				//  3

	{ "b04801w064.u1",	 		0x800000, 0x98dce13a, 4 | BRF_GRA },				//  4 Sprite Masks & Color Indexes

	{ "w04801b032.u17",			0x400000, 0x60298536, 5 | BRF_SND },				//  5 Samples

	{ "espgal_igs027a.bin",		0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	//  6 Internal ARM7 Rom
};

STDROMPICKEXT(espgal, espgal, espgalBios) // custom bios
STD_ROM_FN(espgal)

static INT32 espgalInit()
{
	pPgmInitCallback = pgm_decrypt_espgaluda;
	pPgmProtCallback = install_protection_asic27a_ketsui; // simulation

	return pgmInit();
}

struct BurnDriver BurnDrvEspgal = {
	"espgal", NULL, NULL, NULL, "2003",
	"Espgaluda (V100, Japan)\0", NULL, "CAVE / AMI", "PolyGameMaster based",
	L"Espgaluda\0\u30A8\u30B9\u30D7\u30AC\u30EB\u30FC\u30C0 (V100, Japan)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_JAMMAPCB/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_VERSHOOT, 0,
	NULL, espgalRomInfo, espgalRomName, NULL, NULL, NULL, NULL, pgmInputInfo, jammaDIPInfo,
	espgalInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224, 448, 3, 4
};


// The Gladiator - Road Of The Sword / Shen Jian (V100, Japan, PCB Version)

static struct BurnRomInfo thegladpcbRomDesc[] = {
	{ "v100_u43.u43",		0x080000, 0xbcf3b172, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "igs_t04601w64m.u71",	0x800000, 0xe5dab371, 2 | BRF_GRA },			//  1 Tile data

	{ "igs_a04601w64m.u30",	0x800000, 0xd9b2e004, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "igs_a04602w64m.u31",	0x800000, 0x14f22308, 3 | BRF_GRA },			//  3
	{ "igs_a04603w64m.u32",	0x800000, 0x8f621e17, 3 | BRF_GRA },			//  4

	{ "igs_b04601w64m.u40",	0x800000, 0xee72bccf, 4 | BRF_GRA },			//  5 Sprite Masks & Color Indexes
	{ "igs_b04602w32m.u41",	0x400000, 0x7dba9c38, 4 | BRF_GRA },			//  6

	{ "igs_w04601b64m.u8",	0x800000, 0x5f15ddb3, 5 | BRF_SND },			//  7 Samples
	// these contain samples for the japan region
	{ "wave_u29.u29",     	0x200000, 0x51acb395, 5 | BRF_SND },			//  8

//	{ "thegladpcb_igs027a.bin",		    		0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  8 Internal ARM7 Rom
	{ "thegladpcb_igs027a_execute_only_area",   0x000188, 0x00000000, 0 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  8 Internal ARM7 Rom
	{ "thegladpcb_igs027a_v100_japan.bin",      0x003e78, 0xd7f06e2d, 7 | BRF_PRG | BRF_ESS },  //  9

	{ "v100_u62.u62",	    0x200000, 0x0f3f511e, 8 | BRF_PRG | BRF_ESS },	// 10 External ARM7 Rom
};

STDROMPICKEXT(thegladpcb, thegladpcb, thegladBIOS) // custom bios
STD_ROM_FN(thegladpcb)

struct BurnDriver BurnDrvThegladpcb = {
	"thegladpcb", "theglad", "pgm", NULL, "2003",
	"The Gladiator - Road Of The Sword / Shen Jian (V100, Japan, Single PCB Version)\0", NULL, "IGS", "PolyGameMaster",
	L"The Gladiator - Road Of The Sword\0\u795E\u5251\u98CE\u4E91\0\u795E\u528D\u98A8\u96F2 (V100, Japan, PCB Version)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, thegladpcbRomInfo, thegladpcbRomName, NULL, NULL, NULL, NULL, pgmInputInfo, thegladpcbDIPInfo,
	theglad100Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Demon Front (V107, Korea, PCB Version)

static struct BurnRomInfo dmnfrntpcbRomDesc[] = {
	{ "v107kr_u43.u43",	0x200000, 0x671d8a31, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "igs_t04501w064.u71",		0x800000, 0x900eaaac, 2 | BRF_GRA },					//  1 Tile data

	{ "igs_a04501w064.u30",		0x800000, 0x9741bea6, 3 | BRF_GRA },					//  2 Sprite Color Data
	{ "igs_a04502w064.u31",		0x800000, 0xe104f405, 3 | BRF_GRA },					//  3
	{ "igs_a04503w064.u32",		0x800000, 0xbfd5cfe3, 3 | BRF_GRA },					//  4

	{ "igs_b04501w064.u40",		0x800000, 0x29320b7d, 4 | BRF_GRA },					//  5 Sprite Masks & Color Indexes
	{ "igs_b04502w016.u41",		0x200000, 0x578c00e9, 4 | BRF_GRA },					//  6

	{ "igs_w04501b064.u8",		0x800000, 0x3ab58137, 5 | BRF_SND },					//  7 Samples

	{ "dmnfrnt_igs027a.bin",	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  8 Internal ARM7 Rom

	{ "v107kr_u62.u62",	0x400000, 0xcb94772e, 8 | BRF_PRG | BRF_ESS },  //  9 External ARM7 Rom
};

STDROMPICKEXT(dmnfrntpcb, dmnfrntpcb, dmnfrntBIOS) // custom bios
STD_ROM_FN(dmnfrntpcb)

struct BurnDriver BurnDrvDmnfrntpcb = {
	"dmnfrntpcb", "dmnfrnt", "pgm", NULL, "2002",
	"Demon Front (V107, Korea, Single PCB Version)\0", "Insert coin to get past ERROR", "IGS", "PolyGameMaster",
	L"Demon Front\0\u9B54\u57DF\u6218\u7EBF\0\u9B54\u57DF\u6230\u7DDA (V107, Korea, Single PCB Version)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_RUNGUN, 0,
	NULL, dmnfrntpcbRomInfo, dmnfrntpcbRomName, NULL, NULL, NULL, NULL, pgmInputInfo, dmnfrntpcbDIPInfo,
	dmnfrntInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// S.V.G. - Spectral vs Generation (V100, Japan, Single PCB Version)

static struct BurnRomInfo svgpcbRomDesc[] = {
	{ "svg_v100jp_u50.u50",		0x080000, 0x8d0405e4, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "igs_t05601w016.u70",		0x200000, 0x03e110dc, 2 | BRF_GRA },			//  1 Tile data

	{ "igs_a05601w064.u44",		0x800000, 0xea6453e4, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "igs_a05602w064.u45",		0x800000, 0x6d00621b, 3 | BRF_GRA },			//  3
	{ "igs_a05603w064.u46",		0x800000, 0x7b71c64f, 3 | BRF_GRA },			//  4
	{ "igs_a05604w032.u47",		0x400000, 0x9452a567, 3 | BRF_GRA },			//  5

	{ "igs_b05601w064.u61",		0x800000, 0x35c0a489, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "igs_b05602w064.u62",		0x800000, 0x8aad3f85, 4 | BRF_GRA },			//  7

	{ "igs_w05601b064.u30",		0x800000, 0xbfe61a71, 5 | BRF_SND },			//  8 Samples
	{ "igs_w05602b032.u31",		0x400000, 0x0685166d, 5 | BRF_SND },			//  9

//	{ "svgpcb_igs027a.bin",		      0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	// 10 Internal ARM7 Rom
	{ "svgpcb_igs027a_execute_only_area", 0x000188, 0x00000000, 0 | BRF_OPT | BRF_NODUMP },	// 10 Internal ARM7 Rom
	{ "svgcpb_igs027a_v100_japan.bin",    0x003e78, 0x7a59da5d, 7 | BRF_PRG | BRF_ESS },		// 11 Internal ARM7 Rom

	{ "svg_v100jp_u64.u64",		0x400000, 0x399d4a8b, 8 | BRF_PRG | BRF_ESS },	// 12 External ARM7 Rom
	{ "svg_v100jp_u65.u65",		0x400000, 0x6e1c33b1, 8 | BRF_PRG | BRF_ESS },	// 13
};

STDROMPICKEXT(svgpcb, svgpcb, svgpcbBIOS) // custom bios
STD_ROM_FN(svgpcb)

static void svgpcbPatch()
{
	pgm_decrypt_svgpcb();
	pgm_create_theglad_EO_data();
}

static INT32 svgpcbInit()
{
	pPgmInitCallback = svgpcbPatch;
	pPgmProtCallback = install_protection_asic27a_svg;

	nPgmAsicRegionHackAddress = 0x3a8e;

	INT32 nRet = pgmInit();
	
	Arm7SetIdleLoopAddress(0x00009e0);

	return nRet;
}

struct BurnDriver BurnDrvSvgpcb = {
	"svgpcb", "svg", "pgm", NULL, "2005",
	"S.V.G. - Spectral vs Generation (V100, Japan, Single PCB Version)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, svgpcbRomInfo, svgpcbRomName, NULL, NULL, NULL, NULL, pgmInputInfo, thegladpcbDIPInfo,
	svgpcbInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// -----------------------------------------------------------------------------
// Homebrew


// Chaos (PGM Demo)

static struct BurnRomInfo pgmdemoRomDesc[] = {
	{ "p0103.rom",     		0x200000, 0xd3f6ec45, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0100.rom",     		0x400000, 0x0596a59a, 2 | BRF_GRA },			//  1 Tile data

	{ "a0100.rom",     		0x400000, 0x5d0e8fa1, 3 | BRF_GRA },			//  2 Sprite Color Data

	{ "b0100.rom",     		0x400000, 0x15dd191f, 4 | BRF_GRA },			//  3 Sprite Masks & Color Indexes

	{ "m0100.rom",     		0x200000, 0x8d89877e, 5 | BRF_SND },			//  4 Samples
};

STDROMPICKEXT(pgmdemo, pgmdemo, pgm)
STD_ROM_FN(pgmdemo)

struct BurnDriver BurnDrvPgmdemo = {
	"pgmdemo", NULL, "pgm", NULL, "2005",
	"Chaos (PGM Demo)\0", "Demo Game", "Raster", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING  | BDF_DEMO, 4, HARDWARE_IGS_PGM, GBF_MISC, 0,
	NULL, pgmdemoRomInfo, pgmdemoRomName, NULL, NULL, NULL, NULL, pgmInputInfo, pgmDIPInfo,
	pgmInit,pgmExit,pgmFrame,pgmDraw,NULL, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Frog Feast (PGM)

static struct BurnRomInfo pgmfrogRomDesc[] = {
	{ "p0103.rom",    		0x200000, 0xcdec9e8d, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0100.rom",     		0x400000, 0x8f58b6d8, 2 | BRF_GRA },			//  1 Tile data

	{ "a0100.rom",     		0x400000, 0xdc1eafe6, 3 | BRF_GRA },			//  2 Sprite Color Data

	{ "b0100.rom",     		0x400000, 0x3d44b66f, 4 | BRF_GRA },			//  3 Sprite Masks & Color Indexes

	{ "m0100.rom",     		0x200000, 0x05e2f761, 5 | BRF_SND },			//  4 Samples
};

STDROMPICKEXT(pgmfrog, pgmfrog, pgm)
STD_ROM_FN(pgmfrog)

struct BurnDriver BurnDrvPgmfrog = {
	"pgmfrog", NULL, "pgm", NULL, "2006",
	"Frog Feast (PGM)\0", NULL, "RasterSoft", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING  | BDF_HOMEBREW, 4, HARDWARE_IGS_PGM, GBF_ACTION, 0,
	NULL, pgmfrogRomInfo, pgmfrogRomName, NULL, NULL, NULL, NULL, pgmInputInfo, pgmDIPInfo,
	pgmInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// P-GeMeni (060123)

static struct BurnRomInfo pgemeniRomDesc[] = {
	{ "p0103.rom",     		0x200000, 0x6cafa56b, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0100.rom",     		0x400000, 0x42b979dd, 2 | BRF_GRA },			//  1 Tile data

	{ "a0100.rom",     		0x400000, 0x105d7cee, 3 | BRF_GRA },			//  2 Sprite Color Data

	{ "b0100.rom",     		0x400000, 0xb4127373, 4 | BRF_GRA },			//  3 Sprite Masks & Color Indexes

	{ "m0100.rom",     		0x200000, 0x8d89877e, 5 | BRF_SND },			//  4 Samples
};

STDROMPICKEXT(pgemeni, pgemeni, pgm)
STD_ROM_FN(pgemeni)

struct BurnDriver BurnDrvPgemeni = {
	"pgemeni", NULL, "pgm", NULL, "2006",
	"P-GeMeni (060123)\0", NULL, "blastar@gmx.net", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING  | BDF_HOMEBREW, 4, HARDWARE_IGS_PGM, GBF_PUZZLE, 0,
	NULL, pgemeniRomInfo, pgemeniRomName, NULL, NULL, NULL, NULL, pgmInputInfo, pgmDIPInfo,
	pgmInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


//-----------------------------------------------------------------------------------------------------------------
// Bootlegs & hacks


// Knights of Valour: Quan Huang San Guo Special / Sangoku Senki: Quan Huang San Guo Special (ver. 303CN)

static struct BurnRomInfo kovqhsgsRomDesc[] = {
	{ "qhsg_c51.rom",  		0x0400000, 0xe5cbac85, 1 | BRF_PRG | BRF_ESS }, //  0 68K Code

	{ "lsqh2_t01.rom",      	0x1000000, 0xd498d97f, 2 | BRF_GRA },			//  1 Tile data

	{ "lsqh2_a01.rom",		0x1000000, 0x25ae3efd, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "lsqh2_a23.rom",		0x1000000, 0x7a779112, 3 | BRF_GRA },			//  3
	{ "lsqh2_a45.rom",		0x1000000, 0x5d7de052, 3 | BRF_GRA },			//  4

	{ "lsqh2_b01.rom",		0x1000000, 0xdf7ca696, 4 | BRF_GRA },			//  5 Sprite Masks & Color Indexes

	{ "lsqh2_m01.rom",		0x1000000, 0x01af1b65, 5 | BRF_SND },			//  6 Samples

	{ "qhsg_prot.c51",		0x0004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS },	//  7 Internal ARM7 Rom
};

STDROMPICKEXT(kovqhsgs, kovqhsgs, pgm)
STD_ROM_FN(kovqhsgs)

static INT32 kovqhsgsInit()
{
	pPgmInitCallback = pgm_decrypt_kovqhsgs;
	pPgmProtCallback = install_protection_asic27a_kovsh;

	nPgmAsicRegionHackAddress = 0x3f0d;

	INT32 nRet = pgmInit();
	
	Arm7SetIdleLoopAddress(0x00000260);

	return nRet;
}

struct BurnDriver BurnDrvkovqhsgs = {
	"kovqhsgs", "kovsh", "pgm", NULL, "1999",
	"Knights of Valour: Quan Huang San Guo Special / Sangoku Senki: Quan Huang San Guo Special (ver. 303CN)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour: Quan Huang San Guo Special\0\u4E09\u56FD\u6218\u7EAA\0\u62F3\u7687\u4E09\u56FD\u7279\u522B\u7248 (ver. 303CN)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE | BDF_BOOTLEG, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovqhsgsRomInfo, kovqhsgsRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovqhsgsInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour: Quan Huang San Guo Special / Sangoku Senki: Quan Huang San Guo Special (alt ver. 303CN)

static struct BurnRomInfo kovqhsgsaRomDesc[] = {
	{ "qhsga_c51.rom",  		0x0400000, 0x4c5ea070, 1 | BRF_PRG | BRF_ESS }, //  0 68K Code

	{ "lsqh2_t01.rom",      	0x1000000, 0xd498d97f, 2 | BRF_GRA },			//  1 Tile data

	{ "lsqh2_a01.rom",		0x1000000, 0x25ae3efd, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "lsqh2_a23.rom",		0x1000000, 0x7a779112, 3 | BRF_GRA },			//  3
	{ "lsqh2_a45.rom",		0x1000000, 0x5d7de052, 3 | BRF_GRA },			//  4

	{ "lsqh2_b01.rom",		0x1000000, 0xdf7ca696, 4 | BRF_GRA },			//  5 Sprite Masks & Color Indexes

	{ "lsqh2_m01.rom",		0x1000000, 0x01af1b65, 5 | BRF_SND },			//  6 Samples

	{ "qhsg_prot.c51",		0x0004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS },	//  7 Internal ARM7 Rom
};

STDROMPICKEXT(kovqhsgsa, kovqhsgsa, pgm)
STD_ROM_FN(kovqhsgsa)

struct BurnDriver BurnDrvkovqhsgsa = {
	"kovqhsgsa", "kovsh", "pgm", NULL, "1999",
	"Knights of Valour: Quan Huang San Guo Special / Sangoku Senki: Quan Huang San Guo Special (alt ver. 303CN)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour: Quan Huang San Guo Special\0\u4E09\u56FD\u6218\u7EAA\0\u62F3\u7687\u4E09\u56FD\u7279\u522B\u7248 (alt ver. 303CN)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE | BDF_BOOTLEG, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovqhsgsaRomInfo, kovqhsgsaRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovqhsgsInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour: Luan Shi Quan Huang 2 / Sangoku Senki: Luan Shi Quan Huang 2 (ver. 200CN)

static struct BurnRomInfo kovlsqh2RomDesc[] = {
	{ "lsqh2_prg.rom",		0x0400000, 0xd71e3d50, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "lsqh2_t01.rom",		0x1000000, 0xd498d97f, 2 | BRF_GRA },			//  1 Tile data

	{ "lsqh2_a01.rom",		0x1000000, 0x25ae3efd, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "lsqh2_a23.rom",		0x1000000, 0x7a779112, 3 | BRF_GRA },			//  3
	{ "lsqh2_a45.rom",		0x1000000, 0x5d7de052, 3 | BRF_GRA },			//  4

	{ "lsqh2_b01.rom",		0x1000000, 0xdf7ca696, 4 | BRF_GRA },			//  5 Sprite Masks & Color Indexes

	{ "lsqh2_m01.rom",		0x1000000, 0x01af1b65, 5 | BRF_SND },			//  6 Samples

#if !defined ROM_VERIFY
	{ "lsqh2_prot.c51",		0x0010000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP }, //  7 Internal ARM7 Rom
#else
	{ "kovsh_v100_china.asic",	0x0004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS }, //  7 Internal ARM7 Rom
#endif
};

STDROMPICKEXT(kovlsqh2, kovlsqh2, pgm)
STD_ROM_FN(kovlsqh2)

static INT32 kovlsqh2Init()
{
	pPgmInitCallback = pgm_decrypt_kovlsqh2;
//	pPgmProtCallback = install_protection_asic27a_kovsh;

	INT32 nRet = pgmInit();
	
//	Arm7SetIdleLoopAddress(~0);

	return nRet;
}

struct BurnDriverD BurnDrvkovlsqh2 = {
	"kovlsqh2", "kovshp", "pgm", NULL, "2009",
	"Knights of Valour: Luan Shi Quan Huang II / Sangoku Senki: Luan Shi Quan Huang II (ver. 200CN)\0", "Incomplete dump", "IGS", "PolyGameMaster",
	L"Knights of Valour: Luan Shi Quan Huang II\0\u4E09\u56FD\u6218\u7EAA\0\u4E71\u4E16\u62F3\u7687II (ver. 200CN)\0", NULL, NULL, NULL,
	BDF_CLONE | BDF_BOOTLEG, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU */, GBF_SCRFIGHT, 0,
	NULL, kovlsqh2RomInfo, kovlsqh2RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovlsqh2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour: Luan Shi Quan Huang / Sangoku Senki: Luan Shi Quan Huang (ver. 200CN)

static struct BurnRomInfo kovlsqhRomDesc[] = {
	{ "lsqh_v200cn.rom",  		0x0400000, 0x9935a27a, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "lsqh2_t01.rom",		0x1000000, 0xd498d97f, 2 | BRF_GRA },			//  1 Tile data

	{ "lsqh2_a01.rom",		0x1000000, 0x25ae3efd, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "lsqh2_a23.rom",		0x1000000, 0x7a779112, 3 | BRF_GRA },			//  3
	{ "lsqh2_a45.rom",		0x1000000, 0x5d7de052, 3 | BRF_GRA },			//  4

	{ "lsqh2_b01.rom",		0x1000000, 0xdf7ca696, 4 | BRF_GRA },			//  5 Sprite Masks & Color Indexes

	{ "lsqh2_m01.rom",		0x1000000, 0x01af1b65, 5 | BRF_SND },			//  6 Samples

#if !defined ROM_VERIFY
	{ "lsqh_prot.c51",		0x0010000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP }, //  7 Internal ARM7 Rom
#else
	{ "kovsh_v100_china.asic",	0x0004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS },	//  7 Internal ARM7 Rom
#endif
};

STDROMPICKEXT(kovlsqh, kovlsqh, pgm)
STD_ROM_FN(kovlsqh)

static INT32 kovlsqhInit()
{
	pPgmInitCallback = pgm_decrypt_kovlsqh2;
//	pPgmProtCallback = install_protection_asic27a_kovsh;

	INT32 nRet = pgmInit();
	
//	Arm7SetIdleLoopAddress(~0);

	return nRet;
}

struct BurnDriverD BurnDrvkovlsqh = {
	"kovlsqh", "kovshp", "pgm", NULL, "2009",
	"Knights of Valour: Luan Shi Quan Huang / Sangoku Senki: Luan Shi Quan Huang (ver. 200CN)\0", "Incomplete dump", "IGS", "PolyGameMaster",
	L"Knights of Valour: Luan Shi Quan Huang\0\u4E09\u56FD\u6218\u7EAA\0\u4E71\u4E16\u62F3\u7687 (ver. 200CN)\0", NULL, NULL, NULL,
	BDF_CLONE | BDF_BOOTLEG, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU */, GBF_SCRFIGHT, 0,
	NULL, kovlsqhRomInfo, kovlsqhRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovlsqhInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour: Luan Shi Jie Ba / Sangoku Senki: Luan Shi Jie Ba (ver. 200CN)

static struct BurnRomInfo kovlsjbRomDesc[] = {
	{ "lsjb_prg.rom",  		0x0400000, 0xadf06b37, 1 | BRF_PRG | BRF_ESS }, //  0 68K Code

	{ "lsqh2_t01.rom",		0x1000000, 0xd498d97f, 2 | BRF_GRA },			//  1 Tile data

	{ "lsqh2_a01.rom",		0x1000000, 0x25ae3efd, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "lsqh2_a23.rom",		0x1000000, 0x7a779112, 3 | BRF_GRA },			//  3
	{ "lsqh2_a45.rom",		0x1000000, 0x5d7de052, 3 | BRF_GRA },			//  4

	{ "lsqh2_b01.rom",		0x1000000, 0xdf7ca696, 4 | BRF_GRA },			//  5 Sprite Masks & Color Indexes

	{ "lsqh2_m01.rom",		0x1000000, 0x01af1b65, 5 | BRF_SND },			//  6 Samples

#if !defined ROM_VERIFY
	{ "lsjb_prot.c51",		0x0010000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP }, //  7 Internal ARM7 Rom
#else
	{ "kovsh_v100_china.asic",	0x0004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS }, //  7 Internal ARM7 Rom
#endif
};

STDROMPICKEXT(kovlsjb, kovlsjb, pgm)
STD_ROM_FN(kovlsjb)

struct BurnDriverD BurnDrvkovlsjb = {
	"kovlsjb", "kovshp", "pgm", NULL, "2009",
	"Knights of Valour: Luan Shi Jie Ba / Sangoku Senki: Luan Shi Jie Ba (ver. 200CN)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour: Luan Shi Jie Ba\0\u4E09\u56FD\u6218\u7EAA\0\u4E71\u4E16\u8857\u9738 (ver. 200CN)\0", NULL, NULL, NULL,
	BDF_CLONE | BDF_BOOTLEG, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kovlsjbRomInfo, kovlsjbRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovlsqh2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour: Luan Shi Jie Ba / Sangoku Senki: Luan Shi Jie Ba (alt ver. 200CN)

static struct BurnRomInfo kovlsjbaRomDesc[] = {
	{ "lsjba_prg.rom",  		0x0400000, 0x8b42f456, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "lsqh2_t01.rom",		0x1000000, 0xd498d97f, 2 | BRF_GRA },			//  1 Tile data

	{ "lsqh2_a01.rom",		0x1000000, 0x25ae3efd, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "lsqh2_a23.rom",		0x1000000, 0x7a779112, 3 | BRF_GRA },			//  3
	{ "lsqh2_a45.rom",		0x1000000, 0x5d7de052, 3 | BRF_GRA },			//  4

	{ "lsqh2_b01.rom",		0x1000000, 0xdf7ca696, 4 | BRF_GRA },			//  5 Sprite Masks & Color Indexes

	{ "lsqh2_m01.rom",		0x1000000, 0x01af1b65, 5 | BRF_SND },			//  6 Samples

#if !defined ROM_VERIFY
	{ "lsjba_prot.c51",		0x0010000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	//  7 Internal ARM7 Rom
#else
	{ "kovsh_v100_china.asic",	0x0004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS },	//  7 Internal ARM7 Rom
#endif
};

STDROMPICKEXT(kovlsjba, kovlsjba, pgm)
STD_ROM_FN(kovlsjba)

struct BurnDriverD BurnDrvkovlsjba = {
	"kovlsjba", "kovshp", "pgm", NULL, "2009",
	"Knights of Valour: Luan Shi Jie Ba / Sangoku Senki: Luan Shi Jie Ba (alt ver. 200CN)\0", "Incomplete dump", "IGS", "PolyGameMaster",
	L"Knights of Valour: Luan Shi Jie Ba\0\u4E09\u56FD\u6218\u7EAA\0\u4E71\u4E16\u8857\u9738 (alt ver. 200CN)\0", NULL, NULL, NULL,
	BDF_CLONE | BDF_BOOTLEG, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kovlsjbaRomInfo, kovlsjbaRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovlsqh2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour: Ao Shi San Guo / Sangoku Senki: Ao Shi San Guo (ver. 315CN)

static struct BurnRomInfo kovassgRomDesc[] = {
	{ "assg_v315cn.rom",  		0x0400000, 0xb9885d5c, 1 | BRF_PRG | BRF_ESS }, //  0 68K Code

	{ "lsqh2_t01.rom",		0x1000000, 0xd498d97f, 2 | BRF_GRA },			//  1 Tile data

	{ "lsqh2_a01.rom",		0x1000000, 0x25ae3efd, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "lsqh2_a23.rom",		0x1000000, 0x7a779112, 3 | BRF_GRA },			//  3
	{ "lsqh2_a45.rom",		0x1000000, 0x5d7de052, 3 | BRF_GRA },			//  4

	{ "lsqh2_b01.rom",		0x1000000, 0xdf7ca696, 4 | BRF_GRA },			//  5 Sprite Masks & Color Indexes

	{ "lsqh2_m01.rom",		0x1000000, 0x01af1b65, 5 | BRF_SND },			//  6 Samples

	{ "assg_prot.c51",		0x0010000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	// 7 Internal ARM7 Rom
};

STDROMPICKEXT(kovassg, kovassg, pgm)
STD_ROM_FN(kovassg)

static INT32 kovassgInit()
{
	pPgmInitCallback = pgm_decrypt_kovassg; // incomplete...
//	pPgmProtCallback = install_protection_asic27a_kovsh;

	INT32 nRet = pgmInit();
	
//	Arm7SetIdleLoopAddress(~0);

	return nRet;
}

struct BurnDriverD BurnDrvkovassg = {
	"kovassg", "kovshp", "pgm", NULL, "2008",
	"Knights of Valour: Ao Shi San Guo / Sangoku Senki: Ao Shi San Guo (ver. 315CN)\0", "Incomplete dump", "IGS", "PolyGameMaster",
	L"Knights of Valour: Ao Shi San Guo\0\u4E09\u56FD\u6218\u7EAA\0\u50B2\u4E16\u4E09\u56FD (ver. 315CN)\0", NULL, NULL, NULL,
	BDF_CLONE | BDF_BOOTLEG, 4, HARDWARE_IGS_PGM /* | HARDWARE_IGS_USE_ARM_CPU */, GBF_SCRFIGHT, 0,
	NULL, kovassgRomInfo, kovassgRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovassgInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour: Ao Shi San Guo / Sangoku Senki: Ao Shi San Guo (ver. 202CN)

static struct BurnRomInfo kovshxasRomDesc[] = {
	{ "de_p0609.322",  		0x400000, 0x3b7b627f, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0xff7a4373, 3 | BRF_GRA }, 	        //  3 
	{ "a0602.rom",	   		0x800000, 0xe7a32959, 3 | BRF_GRA }, 	        //  4
	{ "a0603xas.rom",  		0x800000, 0x7057b37e, 3 | BRF_GRA }, 	        //  5

	{ "b0600.rom",	  		0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b0601xas.rom",  		0x800000, 0x3784fb49, 4 | BRF_GRA },			//  7

	{ "m0600.rom",	   		0x400000, 0x3ada4fd6, 5 | BRF_SND },	        //  8 Samples

	{ "kovsh_v100_china.asic",	0x004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovshxas, kovshxas, pgm)
STD_ROM_FN(kovshxas)

static INT32 kovshxasInit()
{
	pPgmInitCallback = kovshp_asic_patch; // hack
	pPgmProtCallback = install_protection_asic27a_kovshp;

	nPgmAsicRegionHackAddress = 0x3f0d;

	INT32 nRet = pgmInit();
	
	Arm7SetIdleLoopAddress(0x00000260);

	return nRet;
}

struct BurnDriver BurnDrvkovshxas = {
	"kovshxas", "kovshp", "pgm", NULL, "2008",
	"Knights of Valour: Ao Shi San Guo / Sangoku Senki: Ao shi San Guo (ver. 202CN)\0", "Imperfect Protection Emulation", "IGS", "PolyGameMaster",
	L"Knights of Valour: Ao Shi San Guo\0\u4E09\u56FD\u6218\u7EAA\0\u50B2\u4E16\u4E09\u56FD (ver. 202CN)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE | BDF_BOOTLEG, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshxasRomInfo, kovshxasRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovshxasInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour: SanGuo QunYingZhuan / Sangoku Senki: SanGuo QunYingZhuan (set 1)

static struct BurnRomInfo kovsgqyzRomDesc[] = {
	{ "pgm_sgqyz.rom", 		0x400000, 0x18e1eed9, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t0600.u11",     	0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data
	
	{ "pgm_a0600.u2",     	0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a0601.u4",     	0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "pgm_a0602.u6",     	0x800000, 0xe7a32959, 3 | BRF_GRA },			//  4
	{ "qyza0603.rom",  		0x800000, 0xc8b92220, 3 | BRF_GRA },			//  5

	{ "pgm_b0600.u5",     	0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "qyzb0601.rom",  		0x800000, 0x64f55882, 4 | BRF_GRA },			// 11
	
	{ "pgm_m0600.u3",     	0x400000, 0x3ada4fd6, 5 | BRF_SND },			//  8 Samples	
	
	{ "kovsgqyz_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	// 13 Internal ARM7 Rom
};

STDROMPICKEXT(kovsgqyz, kovsgqyz, pgm)
STD_ROM_FN(kovsgqyz)

static INT32 kovsgqyzInit()
{
	pPgmProtCallback = install_protection_asic27_kov; // simulation

	return pgmInit();
}

struct BurnDriver BurnDrvKovsgqyz = {
	"kovsgqyz", "kovplus", "pgm", NULL, "1999",
	"Knights of Valour: SanGuo QunYingZhuan / Sangoku Senki: SanGuo QunYingZhuan (set 1)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour: SanGuo QunYingZhuan\0\u4E09\u56FD\u6218\u7EAA\0\u4E09\u56FD\u7FA4\u82F1\u4F20 (set 1)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kovsgqyzRomInfo, kovsgqyzRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovsgqyzInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour: SanGuo QunYingZhuan / Sangoku Senki: SanGuo QunYingZhuan (set 2)

static struct BurnRomInfo kovsgqyzaRomDesc[] = {
	{ "pgm_sgqyza.rom",		0x400000, 0x5a30dcb7, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t0600.u11",     	0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data
	
	{ "pgm_a0600.u2",     	0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a0601.u4",     	0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "pgm_a0602.u6",     	0x800000, 0xe7a32959, 3 | BRF_GRA },			//  4
	{ "qyza0603.rom",  		0x800000, 0xc8b92220, 3 | BRF_GRA },			//  5

	{ "pgm_b0600.u5",     	0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "qyzb0601.rom",  		0x800000, 0x64f55882, 4 | BRF_GRA },			// 11
	
	{ "pgm_m0600.u3",     	0x400000, 0x3ada4fd6, 5 | BRF_SND },			//  8 Samples	
	
	{ "kovsgqyza_igs027a.bin",	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  // 13 Internal ARM7 Rom
};

STDROMPICKEXT(kovsgqyza, kovsgqyza, pgm)
STD_ROM_FN(kovsgqyza)

struct BurnDriver BurnDrvKovsgqyza = {
	"kovsgqyza", "kovplus", "pgm", NULL, "1999",
	"Knights of Valour: SanGuo QunYingZhuan / Sangoku Senki: SanGuo QunYingZhuan (set 2)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour: SanGuo QunYingZhuan\0\u4E09\u56FD\u6218\u7EAA\0\u4E09\u56FD\u7FA4\u82F1\u4F20 (set 2)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kovsgqyzaRomInfo, kovsgqyzaRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovsgqyzInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour: SanGuo QunYingZhuan / Sangoku Senki: SanGuo QunYingZhuan (set 3)

static struct BurnRomInfo kovsgqyzbRomDesc[] = {
	{ "pgm_sgqyzb.rom",		0x400000, 0x18b8b9c0, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t0600.u11",     	0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data
	
	{ "pgm_a0600.u2",     	0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a0601.u4",     	0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "pgm_a0602.u6",     	0x800000, 0xe7a32959, 3 | BRF_GRA },			//  4
	{ "qyza0603.rom",  		0x800000, 0xc8b92220, 3 | BRF_GRA },			//  5

	{ "pgm_b0600.u5",     	0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "qyzb0601.rom",  		0x800000, 0x64f55882, 4 | BRF_GRA },			// 11
	
	{ "pgm_m0600.u3",     	0x400000, 0x3ada4fd6, 5 | BRF_SND },			//  8 Samples
	
	{ "kovsgqyzb_igs027a.bin",	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  // 13 Internal ARM7 Rom
};

STDROMPICKEXT(kovsgqyzb, kovsgqyzb, pgm)
STD_ROM_FN(kovsgqyzb)

struct BurnDriver BurnDrvKovsgqyzb = {
	"kovsgqyzb", "kovplus", "pgm", NULL, "1999",
	"Knights of Valour: SanGuo QunYingZhuan / Sangoku Senki: SanGuo QunYingZhuan (set 3)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour: SanGuo QunYingZhuan\0\u4E09\u56FD\u6218\u7EAA\0\u4E09\u56FD\u7FA4\u82F1\u4F20 (set 3)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kovsgqyzbRomInfo, kovsgqyzbRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovsgqyzInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour: SanGuo QunYingZhuan / Sangoku Senki: SanGuo QunYingZhuan (set 4)

static struct BurnRomInfo kovsgqyzcRomDesc[] = {
	{ "pgm_sgqyzc.rom",		0x400000, 0x09774a3e, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t0600.u11",     	0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data
	
	{ "pgm_a0600.u2",     	0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a0601.u4",     	0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "pgm_a0602.u6",     	0x800000, 0xe7a32959, 3 | BRF_GRA },			//  4
	{ "qyza0603.rom",  		0x800000, 0xc8b92220, 3 | BRF_GRA },			//  5

	{ "pgm_b0600.u5",     	0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "qyzb0601.rom",  		0x800000, 0x64f55882, 4 | BRF_GRA },			// 11
	
	{ "pgm_m0600.u3",     	0x400000, 0x3ada4fd6, 5 | BRF_SND },			//  8 Samples
	
	{ "kovsgqyzb_igs027a.bin",	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  // 13 Internal ARM7 Rom
};

STDROMPICKEXT(kovsgqyzc, kovsgqyzc, pgm)
STD_ROM_FN(kovsgqyzc)

struct BurnDriver BurnDrvKovsgqyzc = {
	"kovsgqyzc", "kovplus", "pgm", NULL, "1999",
	"Knights of Valour: SanGuo QunYingZhuan / Sangoku Senki: SanGuo QunYingZhuan (set 4)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour: SanGuo QunYingZhuan\0\u4E09\u56FD\u6218\u7EAA\0\u4E09\u56FD\u7FA4\u82F1\u4F20 (set 4)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kovsgqyzcRomInfo, kovsgqyzcRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovsgqyzInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour Superheroes / Sangoku Senki Superheroes (bootleg, V104, China)

static struct BurnRomInfo kovshbRomDesc[] = {
	{ "u0706.rom",	   		0x400000, 0x57c75b34, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "pgm_t0600.u2",     	0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data

	{ "pgm_a0600.u3",	   	0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "pgm_a0601.u4",	   	0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "pgm_a0602.u6",	   	0x800000, 0xe7a32959, 3 | BRF_GRA },			//  4
	{ "pgm_a0603.u8",	   	0x400000, 0xec31abda, 3 | BRF_GRA },			//  5
	{ "pgm_a0604.u10",	   	0x400000, 0x26b59fd3, 3 | BRF_GRA },			//  6

	{ "pgm_b0600.u7",	   	0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "pgm_b0601.u9",	   	0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			//  8
	{ "pgm_b0602.u11",	   	0x100000, 0x9df77934, 4 | BRF_GRA },			//  9

	{ "pgm_m0600.u5",	   	0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 10 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS },  // 11 Internal ARM7 Rom
};

STDROMPICKEXT(kovshb, kovshb, pgm)
STD_ROM_FN(kovshb)

static INT32 kovshbInit()
{
	pPgmProtCallback = install_protection_asic27a_kovsh;

	nPgmAsicRegionHackAddress = 0x3f0d;

	INT32 nRet = pgmInit();

	Arm7SetIdleLoopAddress(0x00000260);

	return nRet;
}

struct BurnDriver BurnDrvKovshb = {
	"kovshb", "kovsh", "pgm", NULL, "1999",
	"Knights of Valour Superheroes / Sangoku Senki Superheroes (bootleg, V104, China)\0", NULL, "IGS", "PolyGameMaster",
	L"Knights of Valour Superheroes\0\u4E09\u56FD\u6218\u7EAA\0\u98CE\u4E91\u518D\u8D77 (bootleg, V104, China)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshbRomInfo, kovshbRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovshbInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour: Luan Shi Quan Huang / Sangoku Senki: Luan Shi Quan Huang (ver. 203CN)

static struct BurnRomInfo kovlsqhdRomDesc[] = {
	{ "p0600kof.203",  		0x400000, 0x54f6c5d9, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0xff7a4373, 3 | BRF_GRA }, 	        //  3 
	{ "a0602.rom",	   		0x800000, 0xe7a32959, 3 | BRF_GRA }, 	        //  4
	{ "a603kof.rom",  		0x800000, 0xfe74e90a, 3 | BRF_GRA }, 	        //  5

	{ "b0600.rom",	  		0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b601kof.rom",  		0x800000, 0x188406ee, 4 | BRF_GRA },			//  7

	{ "m0600kof.rom",	   	0x400000, 0xeae5bf80, 5 | BRF_SND },	        //  8 Samples

	{ "kovsh_v100_china.asic",	0x004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovlsqhd, kovlsqhd, pgm)
STD_ROM_FN(kovlsqhd)

struct BurnDriverD BurnDrvkovlsqhd = {
	"kovlsqhd", "kovshp", "pgm", NULL, "2008",
	"Knights of Valour: Luan Shi Quan Huang / Sangoku Senki: Luan Shi Quan Huang (ver. 203CN)\0", "Imperfect Protection Emulation", "IGS", "PolyGameMaster",
	L"Knights of Valour: Luan Shi Quan Huang\0\u4E09\u56FD\u6218\u7EAA\0\u4E71\u4E16\u62F3\u7687 (ver. 203CN)\0", NULL, NULL, NULL,
	BDF_CLONE | BDF_BOOTLEG, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovlsqhdRomInfo, kovlsqhdRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovshxasInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// Knights of Valour: Luan Shi Quan Huang / Sangoku Senki: Luan Shi Quan Huang II (ver. 200CN, alt)

static struct BurnRomInfo kovlsqh2dRomDesc[] = {
	{ "p0600kof2.rom",  	0x400000, 0x6c61f80f, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0xff7a4373, 3 | BRF_GRA }, 	        //  3 
	{ "a0602.rom",	   		0x800000, 0xe7a32959, 3 | BRF_GRA }, 	        //  4
	{ "a603as.rom",  		0x800000, 0xeadc7955, 3 | BRF_GRA }, 	        //  5

	{ "b0600.rom",	  		0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b601as.rom",  		0x800000, 0xbf30ea67, 4 | BRF_GRA },			//  7

	{ "m0600kof.rom",	   	0x400000, 0xeae5bf80, 5 | BRF_SND },	        //  8 Samples

	{ "kovsh_v100_china.asic",	0x004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovlsqh2d, kovlsqh2d, pgm)
STD_ROM_FN(kovlsqh2d)

struct BurnDriverD BurnDrvkovlsqh2d = {
	"kovlsqh2d", "kovshp", "pgm", NULL, "2008",
	"Knights of Valour: Luan Shi Quan Huang / Sangoku Senki: Luan Shi Quan Huang II (ver. 200CN, alt)\0", "Imperfect Protection Emulation", "IGS", "PolyGameMaster",
	L"Knights of Valour: Luan Shi Quan Huang\0\u4E09\u56FD\u6218\u7EAA\0\u4E71\u4E16\u62F3\u7687II (ver. 200CN, alt )\0", NULL, NULL, NULL,
	BDF_CLONE | BDF_BOOTLEG, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovlsqh2dRomInfo, kovlsqh2dRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshxasInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};


// DoDonPachi Dai-Ou-Jou Black Label (2002.10.07 Black Ver., bootleg Knights of Valour Super Heroes conversion)

static struct BurnRomInfo ddpdojblkblRomDesc[] = {
	// bootleg on a converted KOVSH cart
	{ "ddp_doj_u1.bin",			0x400000, 0xeb4ab06a, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t04401w064.u19",			0x800000, 0x3a95f19c, 2 | BRF_GRA },			//  1 Tile data

	{ "a04401w064.u7",			0x800000, 0xed229794, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a04402w064.u8",			0x800000, 0x752167b0, 3 | BRF_GRA },			//  3

	{ "b04401w064.u1",			0x800000, 0x8cbff066, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "m04401b032.u17",			0x400000, 0x5a0dbd76, 5 | BRF_SND },			//  5 Samples

	{ "kovsh_v100_china.asic",	0x004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS },  //  6 Internal ARM7 Rom
};

STDROMPICKEXT(ddpdojblkbl, ddpdojblkbl, pgm) // custom bios
STD_ROM_FN(ddpdojblkbl)

struct BurnDriver BurnDrvDdpdojblkbl = {
	"ddpdojblkbl", "ddp3", NULL, NULL, "2002",
	"DoDonPachi Dai-Ou-Jou Black Label (2002.10.07 Black Ver., bootleg Knights of Valour Super Heroes conversion)\0", NULL, "bootleg", "PolyGameMaster",
	L"DoDonPachi Dai-Ou-Jou Black Label\0\u6012\u9996\u9818\u8702 \u5927\u5F80\u751F Black Label (2002.10.07 Black Ver., bootleg Knights of Valour Super Heroes conversion)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL | BDF_CLONE | BDF_BOOTLEG, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_VERSHOOT, FBF_DONPACHI,
	NULL, ddpdojblkblRomInfo, ddpdojblkblRomName, NULL, NULL, NULL, NULL, pgmInputInfo, jammaDIPInfo,
	kovshInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224, 448, 3, 4
};


// Ketsui: Kizuna Jigoku Tachi (2003/01/01. Master Ver., bootleg cartridge conversion)

static struct BurnRomInfo ketblRomDesc[] = {
	// this assumes a Dodonpachi 2 Bee Storm cart was used
	{ "ketsui_u1.bin",		    0x400000, 0x391767b4, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code
	
	{ "t04701w064.u19", 		0x800000, 0x2665b041, 2 | BRF_GRA },			//  1 Tile data

	{ "a04701w064.u7", 			0x800000, 0x5ef1b94b, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a04702w064.u8", 			0x800000, 0x26d6da7f, 3 | BRF_GRA },			//  3

	{ "b04701w064.u1",			0x800000, 0x1bec008d, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "m04701b032.u17",			0x400000, 0xb46e22d1, 5 | BRF_SND },			//  5 Samples

	{ "ddp2_igs027a_japan.bin",	0x004000, 0x742d34d2, 7 | BRF_PRG | BRF_ESS },	//  6 Internal ARM7 Rom
	
	{ "v100.u23", 	   			0x020000, 0x06c3dd29, 8 | BRF_PRG | BRF_ESS },  //  7 External ARM7 Rom
};

STDROMPICKEXT(ketbl, ketbl, pgm) // custom bios
STD_ROM_FN(ketbl)

static void ketblCallback()
{
	memcpy (PGM68KROM, PGM68KROM + 0x200000, 0x200000);
}

static INT32 ketblInit()
{
	pPgmInitCallback = pgm_decrypt_ddp2;
	pPgmProtCallback = install_protection_asic27a_martmast;
	pPgmInitCallback = ketblCallback;

//	nPgmAsicRegionHackAddress = 0x2882; // 2883?

	INT32 nRet = pgmInit();

	Arm7SetIdleLoopAddress(0x8010998);

	return nRet;
}

struct BurnDriver BurnDrvKetbl = {
	"ketbl", "ket", NULL, NULL, "2003",
	"Ketsui Kizuna Jigoku Tachi (2003/01/01. Master Ver., bootleg cartridge conversion)\0", NULL, "CAVE / AMI", "PolyGameMaster based",
	L"Ketsui Kizuna Jigoku Tachi\0\u30B1\u30C4\u30A4~\u7D46\u5730\u7344\u305F\u3061 (2003/01/01. Master Ver., bootleg cartridge conversion)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL | BDF_CLONE | BDF_BOOTLEG, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_JAMMAPCB | HARDWARE_IGS_USE_ARM_CPU, GBF_VERSHOOT, 0,
	NULL, ketblRomInfo, ketblRomName, NULL, NULL, NULL, NULL, pgmInputInfo, jammaDIPInfo,
	ketblInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224, 448, 3, 4
};


// Espgaluda (2003/10/15 Master Ver, bootleg cartridge conversion)

static struct BurnRomInfo espgalblRomDesc[] = {
	// this assumes a Dodonpachi 2 Bee Storm cart was used
	{ "espgaluda_u8.bin",		0x400000, 0x6a92dd52, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t04801w064.u19",	   		0x800000, 0x6021c79e, 2 | BRF_GRA },			//  1 Tile data

	{ "a04801w064.u7",			0x800000, 0x26dd4932, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a04802w064.u8",	   		0x800000, 0x0e6bf7a9, 3 | BRF_GRA },			//  3

	{ "b04801w064.u1",	 		0x800000, 0x98dce13a, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "w04801b032.u17",			0x400000, 0x60298536, 5 | BRF_SND },			//  5 Samples

	{ "ddp2_igs027a_japan.bin",	0x004000, 0x742d34d2, 7 | BRF_PRG | BRF_ESS },	//  6 Internal ARM7 Rom
	
	{ "v100.u23", 	   			0x020000, 0x06c3dd29, 8 | BRF_PRG | BRF_ESS },  //  7 External ARM7 Rom
};

STDROMPICKEXT(espgalbl, espgalbl, pgm) // custom bios
STD_ROM_FN(espgalbl)

struct BurnDriver BurnDrvEspgalbl = {
	"espgalbl", "espgal", NULL, NULL, "2003",
	"Espgaluda (2003/10/15 Master Ver, bootleg cartridge conversion)\0", NULL, "bootleg", "PolyGameMaster based",
	L"Espgaluda\0\u30A8\u30B9\u30D7\u30AC\u30EB\u30FC\u30C0 (2003/10/15 Master Ver, bootleg cartridge conversion)\0", NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_ORIENTATION_VERTICAL | BDF_CLONE | BDF_BOOTLEG, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_JAMMAPCB | HARDWARE_IGS_USE_ARM_CPU, GBF_VERSHOOT, 0,
	NULL, espgalblRomInfo, espgalblRomName, NULL, NULL, NULL, NULL, pgmInputInfo, jammaDIPInfo,
	ddp2Init, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	224, 448, 3, 4
};

// 西游释厄传-群魔乱舞(新版208版) Xi You Shi E Zhuan Super Plus (Qun Mo Luan Wu New 208 Revision)
// More info at: https://www.ppxclub.com/forum.php?mod=viewthread&tid=695504&fromuid=150233

static struct BurnRomInfo oldsplusnrRomDesc[] = {
	{ "p05301n_v208.rom",	0x400000, 0x97A7CF98, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t05301.rom",			0x800000, 0x8257bbb0, 2 | BRF_GRA },		//  1 Tile data

	{ "a05301n.rom",		0x800000, 0x3D3125FF, 3 | BRF_GRA },	 	//  2 Sprite Color Data
	{ "a05302n.rom",		0x800000, 0x4ED9028C, 3 | BRF_GRA },		//  3
	{ "a05303.rom",			0x800000, 0x13475d85, 3 | BRF_GRA },		//  4
	{ "a05304.rom",			0x800000, 0xf03ef7a6, 3 | BRF_GRA },		//  5

	{ "b05301n.rom",		0x800000, 0x13702BBF, 4 | BRF_GRA },		//  6 Sprite Masks & Color Indexes
	{ "b05302n.rom",		0x800000, 0xB9F75120, 4 | BRF_GRA },		//  7

	{ "m05301.rom",			0x400000, 0x86ec83bc, 5 | BRF_SND },		//  8 Samples

	{ "oldsplus_igs027a.bin",	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	//  9 Internal ARM7 Rom
};

STDROMPICKEXT(oldsplusnr, oldsplusnr, pgm)
STD_ROM_FN(oldsplusnr)

struct BurnDriver BurnDrvoldsplusnr = {
	"oldsplusnr", "oldsplus", "pgm", NULL, "2020",
	"Xi You Shi E Zhuan Super Plus (Qun Mo Luan Wu New 208 Revision)\0", "Incomplete Dump", "Hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE | BDF_HACK, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, oldsplusnrRomInfo, oldsplusnrRomName, NULL, NULL, NULL, NULL, pgmInputInfo, puzzli2DIPInfo,
	oldsplusInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};



//more hack games

//kovplus

// FBA4DROID 三国战纪 正宗Plus 举世无双 Knights of Valour - Sangoku Senki (be without a rival)
static struct BurnRomInfo kov100sRomDesc[] = {
	{ "new_kov100s.119",     	0x400000, 0xb26d7710, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data
	
	{ "a0600.rom",     		0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",     		0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "a0602.rom",     		0x800000, 0xe7a32959, 3 | BRF_GRA },			//  4
	{ "a0603.rom",     		0x400000, 0xec31abda, 3 | BRF_GRA },			//  5
	
	{ "b0600.rom",     		0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b0601.rom",     		0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			//  7
	
	{ "m0600.rom",     		0x400000, 0x3ada4fd6, 5 | BRF_SND },			//  8 Samples
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kov100s, kov100s, pgm)
STD_ROM_FN(kov100s)

struct BurnDriver BurnDrvKov100s = {
	"kov100s", "kovplus", "pgm", NULL, "2017",
	"Knights of Valour - Sangoku Senki (be without a rival)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kov100sRomInfo, kov100sRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 正宗Plus 无限能量版 Knights of Valour - Sangoku Senki (Unlimited energy)
static struct BurnRomInfo kov117maxRomDesc[] = {
	{ "new_kov117max.119",     	0x400000, 0x33fd49c7, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data
	
	{ "a0600.rom",     		0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",     		0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "a0602.rom",     		0x800000, 0xe7a32959, 3 | BRF_GRA },			//  4
	{ "a0603.rom",     		0x400000, 0xec31abda, 3 | BRF_GRA },			//  5
	
	{ "b0600.rom",     		0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b0601.rom",     		0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			//  7
	
	{ "m0600.rom",     		0x400000, 0x3ada4fd6, 5 | BRF_SND },			//  8 Samples
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kov117max, kov117max, pgm)
STD_ROM_FN(kov117max)

struct BurnDriver BurnDrvKov117max = {
	"kov117max", "kovplus", "pgm", NULL, "2017",
	"Knights of Valour - Sangoku Senki (Unlimited energy)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kov117maxRomInfo, kov117maxRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 正宗2012 芳月魔雨 Knights of Valour Plus (authentic 2012 of Fang in the rain)
static struct BurnRomInfo kov2012fymyRomDesc[] = {
	{ "new_kov2012fymy.119",       		0x400000, 0x85e2fbe8, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  5 Tile data
	
	{ "a0600.rom",     		0x800000, 0xd8167834, 3 | BRF_GRA },			//  6 Sprite Color Data
	{ "a0601.rom",     		0x800000, 0xff7a4373, 3 | BRF_GRA },			//  7
	{ "a0602.rom",     		0x800000, 0xe7a32959, 3 | BRF_GRA },			//  8
	{ "a0603.rom",     		0x400000, 0xec31abda, 3 | BRF_GRA },			//  9
	
	{ "b0600.rom",     		0x800000, 0x7d3cd059, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "b0601.rom",     		0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			// 11
	
	{ "m0600.rom",     		0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 12 Samples	
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  // 13 Internal ARM7 Rom
};

STDROMPICKEXT(kov2012fymy, kov2012fymy, pgm)
STD_ROM_FN(kov2012fymy)

struct BurnDriver BurnDrvKov2012fymy = {
	"kov2012fymy", "kovplus", "pgm", NULL, "2018",
	"Knights of Valour Plus (authentic 2012 of Fang in the rain)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kov2012fymyRomInfo, kov2012fymyRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 正宗2012 MISS版 Knights of Valour Plus (authentic 2012 of MISS)
static struct BurnRomInfo kov2012msRomDesc[] = {
	{ "new_kov2012ms.119",       		0x400000, 0xfec5eef0, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  5 Tile data
	
	{ "a0600.rom",     		0x800000, 0xd8167834, 3 | BRF_GRA },			//  6 Sprite Color Data
	{ "a0601.rom",     		0x800000, 0xff7a4373, 3 | BRF_GRA },			//  7
	{ "a0602.rom",     		0x800000, 0xe7a32959, 3 | BRF_GRA },			//  8
	{ "a0603.rom",     		0x400000, 0xec31abda, 3 | BRF_GRA },			//  9
	
	{ "b0600.rom",     		0x800000, 0x7d3cd059, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "b0601.rom",     		0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			// 11
	
	{ "m0600.rom",     		0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 12 Samples	
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  // 13 Internal ARM7 Rom
};

STDROMPICKEXT(kov2012ms, kov2012ms, pgm)
STD_ROM_FN(kov2012ms)

struct BurnDriver BurnDrvKov2012ms = {
	"kov2012ms", "kovplus", "pgm", NULL, "2018",
	"Knights of Valour Plus (authentic 2012 of MISS)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kov2012msRomInfo, kov2012msRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 正宗2012 蜀之复兴 Knights of Valour Plus (authentic 2012 of In the Renaissance)
static struct BurnRomInfo kov2012szfxRomDesc[] = {
	{ "new_kov2012szfx.119",       		0x400000, 0x4aa6737f, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  5 Tile data
	
	{ "a0600.rom",     		0x800000, 0xd8167834, 3 | BRF_GRA },			//  6 Sprite Color Data
	{ "a0601.rom",     		0x800000, 0xff7a4373, 3 | BRF_GRA },			//  7
	{ "a0602.rom",     		0x800000, 0xe7a32959, 3 | BRF_GRA },			//  8
	{ "a0603.rom",     		0x400000, 0xec31abda, 3 | BRF_GRA },			//  9
	
	{ "b0600.rom",     		0x800000, 0x7d3cd059, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "b0601.rom",     		0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			// 11
	
	{ "m0600.rom",     		0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 12 Samples	
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  // 13 Internal ARM7 Rom
};

STDROMPICKEXT(kov2012szfx, kov2012szfx, pgm)
STD_ROM_FN(kov2012szfx)

struct BurnDriver BurnDrvKov2012szfx = {
	"kov2012szfx", "kovplus", "pgm", NULL, "2018",
	"Knights of Valour Plus (authentic 2012 of In the Renaissance)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kov2012szfxRomInfo, kov2012szfxRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 正宗2012 魏吴传 Knights of Valour Plus (authentic 2012 of Wei Wu preach)
static struct BurnRomInfo kov2012wwzRomDesc[] = {
	{ "new_kov2012wwz.119",       		0x400000, 0x5681b066, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  5 Tile data
	
	{ "a0600.rom",     		0x800000, 0xd8167834, 3 | BRF_GRA },			//  6 Sprite Color Data
	{ "a0601.rom",     		0x800000, 0xff7a4373, 3 | BRF_GRA },			//  7
	{ "a0602.rom",     		0x800000, 0xe7a32959, 3 | BRF_GRA },			//  8
	{ "a0603.rom",     		0x400000, 0xec31abda, 3 | BRF_GRA },			//  9
	
	{ "b0600.rom",     		0x800000, 0x7d3cd059, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "b0601.rom",     		0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			// 11
	
	{ "m0600.rom",     		0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 12 Samples	
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  // 13 Internal ARM7 Rom
};

STDROMPICKEXT(kov2012wwz, kov2012wwz, pgm)
STD_ROM_FN(kov2012wwz)

struct BurnDriver BurnDrvKov2012wwz = {
	"kov2012wwz", "kovplus", "pgm", NULL, "2018",
	"Knights of Valour Plus (authentic 2012 of Wei Wu preach)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kov2012wwzRomInfo, kov2012wwzRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 正宗1V8 Knights of Valour - Sangoku Senki (1VS8 20170218)
// Not working due to protection?
static struct BurnRomInfo kov8RomDesc[] = {
	{ "Fzzf_v8.rom",     	0x400000, 0xf680fc22, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data
	
	{ "a0600.rom",     		0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",     		0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "a0602.rom",     		0x800000, 0xe7a32959, 3 | BRF_GRA },			//  4
	{ "a0603.rom",     		0x400000, 0xec31abda, 3 | BRF_GRA },			//  5
	
	{ "b0600.rom",     		0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b0601.rom",     		0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			//  7
	
	{ "m0600.rom",     		0x400000, 0x3ada4fd6, 5 | BRF_SND },			//  8 Samples
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kov8, kov8, pgm)
STD_ROM_FN(kov8)

struct BurnDriver BurnDrvKov8 = {
	"kov8", "kovplus", "pgm", NULL, "2017",
	"Knights of Valour Plus (1VS8 20170218)\0", "Not working!", "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kov8RomInfo, kov8RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 正宗FENG 小兵争霸 Knights of Valour Plus (Small Soldiers Fighting for Hegemony 20181206)
static struct BurnRomInfo kovplusxb1RomDesc[] = {
	{ "p0600xb.119",       		0x400000, 0x9935b99c, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  5 Tile data
	
	{ "a0600.rom",     		0x800000, 0xd8167834, 3 | BRF_GRA },			//  6 Sprite Color Data
	{ "a0601.rom",     		0x800000, 0xff7a4373, 3 | BRF_GRA },			//  7
	{ "a0602.rom",     		0x800000, 0xe7a32959, 3 | BRF_GRA },			//  8
	{ "a0603.rom",     		0x400000, 0xec31abda, 3 | BRF_GRA },			//  9
	
	{ "b0600.rom",     		0x800000, 0x7d3cd059, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "b0601.rom",     		0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			// 11
	
	{ "m0600.rom",     		0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 12 Samples	
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  // 13 Internal ARM7 Rom
};

STDROMPICKEXT(kovplusxb1, kovplusxb1, pgm)
STD_ROM_FN(kovplusxb1)

struct BurnDriver BurnDrvKovplusxb1 = {
	"kovplusxb1", "kovplus", "pgm", NULL, "2018",
	"Knights of Valour Plus (Small Soldiers Fighting for Hegemony 20181206)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kovplusxb1RomInfo, kovplusxb1RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 正宗FENG 小兵争霸2 Knights of Valour Plus (Small Soldiers Fighting for Hegemony 2 20181206)
static struct BurnRomInfo kovplusxb2RomDesc[] = {
	{ "p0600xb2.119",		0x400000, 0x0d9dce43, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  5 Tile data
	
	{ "a0600.rom",     		0x800000, 0xd8167834, 3 | BRF_GRA },			//  6 Sprite Color Data
	{ "a0601.rom",     		0x800000, 0xff7a4373, 3 | BRF_GRA },			//  7
	{ "a0602.rom",     		0x800000, 0xe7a32959, 3 | BRF_GRA },			//  8
	{ "a0603.rom",     		0x400000, 0xec31abda, 3 | BRF_GRA },			//  9
	
	{ "b0600.rom",     		0x800000, 0x7d3cd059, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "b0601.rom",     		0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			// 11
	
	{ "m0600.rom",     		0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 12 Samples	
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  // 13 Internal ARM7 Rom
};

STDROMPICKEXT(kovplusxb2, kovplusxb2, pgm)
STD_ROM_FN(kovplusxb2)

struct BurnDriver BurnDrvKovplusxb2 = {
	"kovplusxb2", "kovplus", "pgm", NULL, "2018",
	"Knights of Valour Plus (Small Soldiers Fighting for Hegemony 2 20181206)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kovplusxb2RomInfo, kovplusxb2RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 正宗FENG 小兵争霸2 大神版 Knights of Valour Plus (Small Soldiers Fighting for Hegemony 2 - Manito 20181207)
static struct BurnRomInfo kovplusxb2dsRomDesc[] = {
	{ "p0600xb2ds.119",		0x400000, 0xdabc2c66, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  5 Tile data
	
	{ "a0600.rom",     		0x800000, 0xd8167834, 3 | BRF_GRA },			//  6 Sprite Color Data
	{ "a0601.rom",     		0x800000, 0xff7a4373, 3 | BRF_GRA },			//  7
	{ "a0602.rom",     		0x800000, 0xe7a32959, 3 | BRF_GRA },			//  8
	{ "a0603.rom",     		0x400000, 0xec31abda, 3 | BRF_GRA },			//  9
	
	{ "b0600.rom",     		0x800000, 0x7d3cd059, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "b0601.rom",     		0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			// 11
	
	{ "m0600.rom",     		0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 12 Samples	
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  // 13 Internal ARM7 Rom
};

STDROMPICKEXT(kovplusxb2ds, kovplusxb2ds, pgm)
STD_ROM_FN(kovplusxb2ds)

struct BurnDriver BurnDrvKovplusxb2ds = {
	"kovplusxb2ds", "kovplus", "pgm", NULL, "2018",
	"Knights of Valour Plus (Small Soldiers Fighting for Hegemony 2 - Manito 20181207)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kovplusxb2dsRomInfo, kovplusxb2dsRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 正宗FENG 小兵争霸2 武将版 Knights of Valour Plus (Small Soldiers Fighting for Hegemony 2 - Military Commanders 20181207)
static struct BurnRomInfo kovplusxb2wjRomDesc[] = {
	{ "p0600xb2wj.119",		0x400000, 0x6da2fd46, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  5 Tile data
	
	{ "a0600.rom",     		0x800000, 0xd8167834, 3 | BRF_GRA },			//  6 Sprite Color Data
	{ "a0601.rom",     		0x800000, 0xff7a4373, 3 | BRF_GRA },			//  7
	{ "a0602.rom",     		0x800000, 0xe7a32959, 3 | BRF_GRA },			//  8
	{ "a0603.rom",     		0x400000, 0xec31abda, 3 | BRF_GRA },			//  9
	
	{ "b0600.rom",     		0x800000, 0x7d3cd059, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "b0601.rom",     		0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			// 11
	
	{ "m0600.rom",     		0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 12 Samples	
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  // 13 Internal ARM7 Rom
};

STDROMPICKEXT(kovplusxb2wj, kovplusxb2wj, pgm)
STD_ROM_FN(kovplusxb2wj)

struct BurnDriver BurnDrvKovplusxb2wj = {
	"kovplusxb2wj", "kovplus", "pgm", NULL, "2018",
	"Knights of Valour Plus (Small Soldiers Fighting for Hegemony 2 - Military Commanders 20181207)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kovplusxb2wjRomInfo, kovplusxb2wjRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 正宗Plus 洪波覆亡 2018版 Knights of Valour - Sangoku Senki (Hong Bo's Death 20180904)
static struct BurnRomInfo kovplus2017RomDesc[] = {
	{ "p0600.119",     	0x400000, 0x55131344, 1 | BRF_PRG | BRF_ESS },  ////  0 68K Code

	{ "t0600.rom",     		0x800000, 0xc70efb8c, 2 | BRF_GRA },			//  1 Tile data
	
	{ "a0600.rom",     		0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",     		0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "a0602.rom",     		0x800000, 0xe7a32959, 3 | BRF_GRA },			//  4
	{ "a0603.rom",     		0x400000, 0xec31abda, 3 | BRF_GRA },			//  5
	
	{ "b0600.rom",     		0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b0601.rom",     		0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			//  7
	
	{ "m0600.rom",     		0x400000, 0x25f4f240, 5 | BRF_SND },			//  8 Samples
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovplus2017, kovplus2017, pgm)
STD_ROM_FN(kovplus2017)

struct BurnDriver BurnDrvKovplus2017 = {
	"kovplus2017", "kovplus", "pgm", NULL, "2018",
	"Knights of Valour - Sangoku Senki (Hong Bo's Death 20180904)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kovplus2017RomInfo, kovplus2017RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 正宗2012 天下一统 Knights of Valour Plus (Unity Of China 20180515)
static struct BurnRomInfo kovplusxtRomDesc[] = {
	{ "p0600.119",       	0x400000, 0x14f0b6fc, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0xe451df7f, 2 | BRF_GRA },			//  5 Tile data
	
	{ "a0600.rom",     		0x800000, 0x783df23c, 3 | BRF_GRA },			//  6 Sprite Color Data
	{ "a0601.rom",     		0x800000, 0xff7a4373, 3 | BRF_GRA },			//  7
	{ "a0602.rom",     		0x800000, 0x71c8a46c, 3 | BRF_GRA },			//  8
	{ "a0603.rom",     		0x400000, 0x4d755da5, 3 | BRF_GRA },			//  9
	
	{ "b0600.rom",     		0x800000, 0xe3bf000f, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "b0601.rom",     		0x400000, 0x47a9af9c, 4 | BRF_GRA },			// 11
	
	{ "m0600.rom",     		0x400000, 0xd84343ec, 5 | BRF_SND },			// 12 Samples	
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  // 13 Internal ARM7 Rom
};

STDROMPICKEXT(kovplusxt, kovplusxt, pgm)
STD_ROM_FN(kovplusxt)

struct BurnDriver BurnDrvKovplusxt = {
	"kovplusxt", "kovplus", "pgm", NULL, "2018",
	"Knights of Valour Plus (Unity Of China 20180515)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kovplusxtRomInfo, kovplusxtRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 正宗DSZL 大神之路 Knights of Valour Plus (Road To Master 20180711)
static struct BurnRomInfo kovplusdszlRomDesc[] = {
	{ "p0600.119",       	0x400000, 0x404404e5, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0x954551ff, 2 | BRF_GRA },			//  5 Tile data
	
	{ "a0600.rom",     		0x800000, 0xd8167834, 3 | BRF_GRA },			//  6 Sprite Color Data
	{ "a0601.rom",     		0x800000, 0xff7a4373, 3 | BRF_GRA },			//  7
	{ "a0602.rom",     		0x800000, 0xf24258ce, 3 | BRF_GRA },			//  8
	{ "a0603.rom",     		0x400000, 0xec31abda, 3 | BRF_GRA },			//  9
	
	{ "b0600.rom",     		0x800000, 0x7d3cd059, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "b0601.rom",     		0x400000, 0xc870aebb, 4 | BRF_GRA },			// 11
	
	{ "m0600.rom",     		0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 12 Samples	
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  // 13 Internal ARM7 Rom
};

STDROMPICKEXT(kovplusdszl, kovplusdszl, pgm)
STD_ROM_FN(kovplusdszl)

struct BurnDriver BurnDrvKovplusdszl = {
	"kovplusdszl", "kovplus", "pgm", NULL, "2018",
	"Knights of Valour Plus (Road To Master 20180711)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kovplusdszlRomInfo, kovplusdszlRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 正宗DQJZ 单骑救主 Knights of Valour Plus (Single Savior 20180308)
static struct BurnRomInfo kovplusdjjzRomDesc[] = {
	{ "p0600.119",       	0x400000, 0x6be54539, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0x169b4e8b, 2 | BRF_GRA },			//  5 Tile data
	
	{ "a0600.rom",     		0x800000, 0x4f287f85, 3 | BRF_GRA },			//  6 Sprite Color Data
	{ "a0601.rom",     		0x800000, 0xff7a4373, 3 | BRF_GRA },			//  7
	{ "a0602.rom",     		0x800000, 0x11ecd0bf, 3 | BRF_GRA },			//  8
	{ "a0603.rom",     		0x400000, 0xec31abda, 3 | BRF_GRA },			//  9
	
	{ "b0600.rom",     		0x800000, 0x9783ed2c, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "b0601.rom",     		0x400000, 0x50d9c24c, 4 | BRF_GRA },			// 11
	
	{ "m0600.rom",     		0x400000, 0xb61b8214, 5 | BRF_SND },			// 12 Samples	
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  // 13 Internal ARM7 Rom
};

STDROMPICKEXT(kovplusdjjz, kovplusdjjz, pgm)
STD_ROM_FN(kovplusdjjz)

struct BurnDriver BurnDrvKovplusdjjz = {
	"kovplusdjjz", "kovplus", "pgm", NULL, "2018",
	"Knights of Valour Plus (Single Savior 20180308)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kovplusdjjzRomInfo, kovplusdjjzRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 正宗2012 无双版 Knights of Valour Plus (Unrivalled Version 20180823)
static struct BurnRomInfo kovplus2012dwRomDesc[] = {
	{ "p2012dw.119",       	0x400000, 0x2c75ac09, 1 | BRF_PRG | BRF_ESS },

	{ "t0600.rom",     		0x800000, 0x842b7711, 2 | BRF_GRA },
	
	{ "a0600.rom",     		0x800000, 0x5bd7ea9f, 3 | BRF_GRA },
	{ "a0601.rom",     		0x800000, 0xff7a4373, 3 | BRF_GRA },
	{ "a0602.rom",     		0x800000, 0xaf2c086d, 3 | BRF_GRA },
	{ "a0603.rom",     		0x400000, 0xec31abda, 3 | BRF_GRA },
	
	{ "b0600.rom",     		0x800000, 0x07d91ea3, 4 | BRF_GRA },
	{ "b0601.rom",     		0x400000, 0x4ce7b2b8, 4 | BRF_GRA },
	
	{ "m0600.rom",     		0x400000, 0x7a50501b, 5 | BRF_SND },
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovplus2012dw, kovplus2012dw, pgm)
STD_ROM_FN(kovplus2012dw)

struct BurnDriver BurnDrvKovplus2012dw = {
	"kovplus2012dw", "kovplus", "pgm", NULL, "2018",
	"Knights of Valour Plus (Unrivalled Version 20180823)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kovplus2012dwRomInfo, kovplus2012dwRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 正宗2012 噩梦版 Knights of Valour Plus (Nightmare 20180601)
// Not working properly.
static struct BurnRomInfo kovplus2012mRomDesc[] = {
	{ "p2012m.119",       	0x600000, 0xfb759b85, 1 | BRF_PRG | BRF_ESS },

	{ "t0600.rom",     		0x800000, 0xea123375, 2 | BRF_GRA },

	{ "a0600.rom",     		0x800000, 0x5bd7ea9f, 3 | BRF_GRA },
	{ "A0601.ROM",     		0x800000, 0xff7a4373, 3 | BRF_GRA },
	{ "A0602.ROM",     		0x800000, 0xe7a32959, 3 | BRF_GRA },
	{ "a0603.ROM",     		0x400000, 0xec31abda, 3 | BRF_GRA },
	
	{ "b0600.rom",     		0x800000, 0x07d91ea3, 4 | BRF_GRA },
	{ "b0601.rom",     		0x400000, 0xa0bb1c2f, 4 | BRF_GRA },
	
	{ "m0600.rom",     		0x400000, 0x7a50501b, 5 | BRF_SND },
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovplus2012m, kovplus2012m, pgm)
STD_ROM_FN(kovplus2012m)

struct BurnDriver BurnDrvKovplus2012m = {
	"kovplus2012m", "kovplus", "pgm", NULL, "2018",
	"Knights of Valour Plus (Nightmare 20180601)\0", "Not Working!", "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kovplus2012mRomInfo, kovplus2012mRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 正宗2012 战神无双 Knights of Valour Plus (Unrivalled God Of War 20180715)
static struct BurnRomInfo kovplus2012emRomDesc[] = {
	{ "p0600.em",       	0x400000, 0x7e6c3c95, 1 | BRF_PRG | BRF_ESS },

	{ "T0600.ROM",     		0x800000, 0x4acc1ad6, 2 | BRF_GRA },
	
	{ "A0600.ROM",     		0x800000, 0x24091941, 3 | BRF_GRA },
	{ "A0601.ROM",     		0x800000, 0x8998680f, 3 | BRF_GRA },
	{ "A0602.ROM",     		0x800000, 0x27a93ff6, 3 | BRF_GRA },
	{ "a0603.rom",     		0x400000, 0x5e00a709, 3 | BRF_GRA },
	
	{ "B0600.ROM",     		0x800000, 0x1ce751b1, 4 | BRF_GRA },
	{ "B0601.rom",     		0x400000, 0x912acd06, 4 | BRF_GRA },
	
	{ "m0600.rom",     		0x400000, 0x3ada4fd6, 5 | BRF_SND },
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovplus2012em, kovplus2012em, pgm)
STD_ROM_FN(kovplus2012em)

struct BurnDriver BurnDrvKovplus2012em = {
	"kovplus2012em", "kovplus", "pgm", NULL, "2018",
	"Knights of Valour Plus (Unrivalled God Of War 20180715)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kovplus2012emRomInfo, kovplus2012emRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// PPXCLUB 三国战纪 正宗2012 战神ST Knights of Valour Plus (God Of War ST 20180716)
static struct BurnRomInfo kovplus2012stRomDesc[] = {
	{ "p0600.st",       	0x400000, 0x8895f498, 1 | BRF_PRG | BRF_ESS },

	{ "t0600.rom",     		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data
	
	{ "a0600.rom",     		0x800000, 0x24091941, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",     		0x800000, 0x8998680f, 3 | BRF_GRA },			//  3
	{ "a0602.rom",     		0x800000, 0x27a93ff6, 3 | BRF_GRA },			//  4
	{ "a0603.rom",     		0x400000, 0x5e00a709, 3 | BRF_GRA },			//  5
	
	{ "b0600.rom",     		0x800000, 0x1ce751b1, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b0601.rom",     		0x400000, 0x912acd06, 4 | BRF_GRA },			//  7
	{ "b0602.rom",     		0x800000, 0x2407290e, 4 | BRF_GRA },			//  7
	
	{ "m0600.rom",     		0x400000, 0x3ada4fd6, 5 | BRF_SND },			//  8 Samples
	
	{ "kov_igs027a.bin", 	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovplus2012st, kovplus2012st, pgm)
STD_ROM_FN(kovplus2012st)

struct BurnDriver BurnDrvKovplus2012st = {
	"kovplus2012st", "kovplus", "pgm", NULL, "2018",
	"Knights of Valour Plus (God Of War ST 20180716)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, kovplus2012stRomInfo, kovplus2012stRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

//kovsh

// FBA4DROID 三国战记 逐鹿中原 Knights of Valour Superheroes (Fight For Territory In Ancient Central China 20180307)
// kovshs111 in HBMAME. Need to add this as parent ROM for other hacks.
static struct BurnRomInfo kovshzlzyRomDesc[] = {
	{ "p0600.322",	   		0x400000, 0x05cb05f4, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "a0602.rom",	   		0x800000, 0xe7a32959, 3 | BRF_GRA },			//  4
	{ "a0603.rom",	   		0x400000, 0xec31abda, 3 | BRF_GRA },			//  5
	{ "a0604.rom",	   		0x400000, 0x17a67ffa, 3 | BRF_GRA },			//  6

	{ "b0600.rom",	   		0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "b0601.rom",	   		0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			//  8
	{ "b0602.rom",	   		0x100000, 0x1f79f118, 4 | BRF_GRA },			//  9
	{ "b0603.rom",	   		0x800000, 0xd9da8cba, 4 | BRF_GRA },			//  9-2 EXPAND

	{ "m0600.rom",	   		0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 10 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0x5c1c9305, 7 | BRF_PRG | BRF_ESS },  // 11 Internal ARM7 Rom
};

STDROMPICKEXT(kovshzlzy, kovshzlzy, pgm)
STD_ROM_FN(kovshzlzy)

struct BurnDriver BurnDrvKovshzlzy = {
	"kovshzlzy", "kovsh", "pgm", NULL, "2018",
	"Knights of Valour Superheroes (Fight For Territory In Ancient Central China 20180307)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshzlzyRomInfo, kovshzlzyRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战记 逐鹿中原 决战天下 Knights of Valour Superheroes (A Decisive Battle Over The World 20181202)
static struct BurnRomInfo kovshzlzyjzRomDesc[] = {
	{ "p0600.322",	   		0x400000, 0x58e08538, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "a0602.rom",	   		0x800000, 0xe7a32959, 3 | BRF_GRA },			//  4
	{ "a0603.rom",	   		0x400000, 0xec31abda, 3 | BRF_GRA },			//  5
	{ "a0604.rom",	   		0x400000, 0x17a67ffa, 3 | BRF_GRA },			//  6

	{ "b0600.rom",	   		0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "b0601.rom",	   		0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			//  8
	{ "b0602.rom",	   		0x100000, 0x1f79f118, 4 | BRF_GRA },			//  9
	{ "b0603.rom",	   		0x800000, 0xd9da8cba, 4 | BRF_GRA },			//  9-2 EXPAND

	{ "m0600.rom",	   		0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 10 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0x5c1c9305, 7 | BRF_PRG | BRF_ESS },  // 11 Internal ARM7 Rom
};

STDROMPICKEXT(kovshzlzyjz, kovshzlzyjz, pgm)
STD_ROM_FN(kovshzlzyjz)

struct BurnDriver BurnDrvKovshzlzyjz = {
	"kovshzlzyjz", "kovsh", "pgm", NULL, "2018",
	"Knights of Valour Superheroes (A Decisive Battle Over The World 20181202)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshzlzyjzRomInfo, kovshzlzyjzRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 逐鹿中原 五剑传奇 Knights of Valour Superheroes (Legend Of Five Swords 20181203)
static struct BurnRomInfo kovshzlzywjRomDesc[] = {
	{ "p0600.322",	   		0x400000, 0xb8799030, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "a0602.rom",	   		0x800000, 0xe7a32959, 3 | BRF_GRA },			//  4
	{ "a0603.rom",	   		0x400000, 0xec31abda, 3 | BRF_GRA },			//  5
	{ "a0604.rom",	   		0x400000, 0x17a67ffa, 3 | BRF_GRA },			//  6

	{ "b0600.rom",	   		0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "b0601.rom",	   		0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			//  8
	{ "b0602.rom",	   		0x100000, 0x1f79f118, 4 | BRF_GRA },			//  9
	{ "b0603.rom",	   		0x800000, 0xd9da8cba, 4 | BRF_GRA },			//  9-2 EXPAND

	{ "m0600.rom",	   		0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 10 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0x5c1c9305, 7 | BRF_PRG | BRF_ESS },  // 11 Internal ARM7 Rom
};

STDROMPICKEXT(kovshzlzywj, kovshzlzywj, pgm)
STD_ROM_FN(kovshzlzywj)

struct BurnDriver BurnDrvKovshzlzywj = {
	"kovshzlzywj", "kovsh", "pgm", NULL, "2018",
	"Knights of Valour Superheroes (Legend Of Five Swords 20181203)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshzlzywjRomInfo, kovshzlzywjRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 逐鹿中原 莫邪传说 Knights of Valour Superheroes (Fight For Territory In Ancient Central China - Legend Of Mo Ye 20190102)
static struct BurnRomInfo kovshzlzymyRomDesc[] = {
	{ "p0600.322",	   		0x400000, 0x21bffcf1, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "a0602.rom",	   		0x800000, 0xe7a32959, 3 | BRF_GRA },			//  4
	{ "a0603.rom",	   		0x400000, 0xec31abda, 3 | BRF_GRA },			//  5
	{ "a0604.rom",	   		0x400000, 0x17a67ffa, 3 | BRF_GRA },			//  6

	{ "b0600.rom",	   		0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "b0601.rom",	   		0x400000, 0xa0bb1c2f, 4 | BRF_GRA },			//  8
	{ "b0602.rom",	   		0x100000, 0x1f79f118, 4 | BRF_GRA },			//  9
	{ "b0603.rom",	   		0x800000, 0xd9da8cba, 4 | BRF_GRA },			//  9-2 EXPAND

	{ "m0600.rom",	   		0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 10 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0x5c1c9305, 7 | BRF_PRG | BRF_ESS },  // 11 Internal ARM7 Rom
};

STDROMPICKEXT(kovshzlzymy, kovshzlzymy, pgm)
STD_ROM_FN(kovshzlzymy)

struct BurnDriver BurnDrvKovshzlzymy = {
	"kovshzlzymy", "kovsh", "pgm", NULL, "2019",
	"Knights of Valour Superheroes (Fight For Territory In Ancient Central China - Legend Of Mo Ye 20190102)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshzlzymyRomInfo, kovshzlzymyRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 风云再起 四剑传说 Knights of Valour Superheroes (Legend Of Four Swords 20151105)
static struct BurnRomInfo kovshsjcsRomDesc[] = {
	{ "p0600.322",	   		0x454f49, 0x72146042, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0x12097ed8, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0x26b3b2ee, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "a0602.rom",	   		0x800000, 0xab94d8a0, 3 | BRF_GRA },			//  4
	{ "a0603.rom",	   		0x400000, 0xec31abda, 3 | BRF_GRA },			//  5
	{ "a0604.rom",	   		0x400000, 0xd1bb8945, 3 | BRF_GRA },			//  6

	{ "b0600.rom",	   		0x800000, 0x9ca61576, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "b0601.rom",	   		0x400000, 0x1e917e36, 4 | BRF_GRA },			//  8
	{ "b0602.rom",	   		0x400000, 0x79a0c0e1, 4 | BRF_GRA },			//  9

	{ "m0600.rom",	   		0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 10 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0xd79943df, 7 | BRF_PRG | BRF_ESS },  // 11 Internal ARM7 Rom
};

STDROMPICKEXT(kovshsjcs, kovshsjcs, pgm)
STD_ROM_FN(kovshsjcs)

struct BurnDriver BurnDrvKovshsjcs = {
	"kovshsjcs", "kovsh", "pgm", NULL, "2018",
	"Knights of Valour Superheroes (Legend Of Four Swords 20151105)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshsjcsRomInfo, kovshsjcsRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

//kovshp

//FBA4DROID 三国战纪 乱世枭雄 PLUS 2019 Knights of Valour Super Heroes Plus (Plus 20190105)
static struct BurnRomInfo kovshpp19RomDesc[] = {
	{ "m6001.rom",			0x200000, 0xf238fbf5, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code
	{ "m6002.rom",			0x200000, 0xd5638f8d, 1 | BRF_PRG | BRF_ESS },  //  1 68K Code

	{ "t0600.rom",    		0x800000, 0xa919ec0f, 2 | BRF_GRA },			//  2 Tile data

	{ "a0600x.rom",	   		0x2000000, 0xa45fa949, 3 | BRF_GRA },			//  3 Sprite Color Data

	{ "b0600x.rom",	   		0x1000000, 0xb1c15d4f, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "p0600h.rom",	   		0x400000, 0x3ada4fd6, 5 | BRF_SND },			//  5 Samples

	{ "kovshp_v100_china.asic", 	0x004000, 0x7ece721c, 7 | BRF_PRG | BRF_ESS },  //  6 Internal ARM7 Rom
};

STDROMPICKEXT(kovshpp19, kovshpp19, pgm)
STD_ROM_FN(kovshpp19)

struct BurnDriver BurnDrvKovshpp19 = {
	"kovshpp19", "kovshp", "pgm", NULL, "2019",
	"Knights of Valour Super Heroes Plus (Plus 20190105)\0", "Imperfect Protection Emulation", "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshpp19RomInfo, kovshpp19RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovshxasInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 乱世枭雄 群雄再起 金版 Knights of Valour Super Heroes Plus (Rerise of Heroes Gold 20190404)
static struct BurnRomInfo kovshpqxzqjRomDesc[] = {
	{ "m6001.rom",			0x300000, 0xcd97641b, 1 | BRF_PRG | BRF_ESS },  //  0-1 68K Code
	{ "m6002.rom",			0x300000, 0x9debf14d, 1 | BRF_PRG | BRF_ESS },  //  0-2 68K Code

	{ "t0600.rom",    		0x800000, 0x355eb28d, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600x.rom",	   		0x2000000, 0x110b8d33, 3 | BRF_GRA },			//  2 Sprite Color Data

	{ "b0600x.rom",	   		0x1000000, 0x43a2406e, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes

	{ "p0600h.rom",	   		0x400000, 0x7a50501b, 5 | BRF_SND },			//  8 Samples

	{ "kovshp_v100_china.asic", 	0x004000, 0x5e6ead02, 7 | BRF_PRG | BRF_ESS },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovshpqxzqj, kovshpqxzqj, pgm)
STD_ROM_FN(kovshpqxzqj)

struct BurnDriver BurnDrvKovshpqxzqj = {
	"kovshpqxzqj", "kovshp", "pgm", NULL, "2019",
	"Knights of Valour Super Heroes Plus (Rerise of Heroes Gold 20190404)\0", "Imperfect Protection Emulation", "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshpqxzqjRomInfo, kovshpqxzqjRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovshxasInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 乱世枭雄 惊天战神 2019 Knights of Valour Super Heroes Plus (Incredible God of War 20190312)
static struct BurnRomInfo kovshp1v4xRomDesc[] = {
	{ "m0601.rom",			0x200000, 0xf513d89c, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code
	{ "m0602.rom",			0x200000, 0x505c0fb9, 1 | BRF_PRG | BRF_ESS },  //  1 68K Code

	{ "t0600.rom",    		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  2 Tile data

	{ "a0600x.rom",	   		0x2000000, 0x92444dcd, 3 | BRF_GRA },			//  3 Sprite Color Data

	{ "b0600x.rom",	   		0x1000000, 0x5d5d9512, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "p0600h.rom",	   		0x400000, 0xec3f335d, 5 | BRF_SND },			//  5 Samples

	{ "kovshp_v100_china.asic", 	0x004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS },  //  6 Internal ARM7 Rom
};

STDROMPICKEXT(kovshp1v4x, kovshp1v4x, pgm)
STD_ROM_FN(kovshp1v4x)

struct BurnDriver BurnDrvKovshp1v4x = {
	"kovshp1v4x", "kovshp", "pgm", NULL, "2019",
	"Knights of Valour Super Heroes Plus (Incredible God of War 20190312)\0", "Imperfect Protection Emulation", "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshp1v4xRomInfo, kovshp1v4xRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovshxasInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 乱世枭雄 群雄再起 Knights of Valour Super Heroes Plus / Sangoku Senki Super Heroes Plus (Rerise of Heroes 20190130)
static struct BurnRomInfo kovshpqxzqRomDesc[] = {
	{ "b0541.rom",			0x300000, 0xf1966241, 1 | BRF_PRG | BRF_ESS },  //  0-1 68K Code
	{ "b0602.rom",			0x300000, 0xe91cef2b, 1 | BRF_PRG | BRF_ESS },  //  0-2 68K Code

	{ "t0600.rom",    		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0xff7a4373, 3 | BRF_GRA }, 	        //  3 
	{ "a0602.rom",	   		0x800000, 0xe7a32959, 3 | BRF_GRA }, 	        //  4
	{ "a0540.rom",	   		0x800000, 0x2482a3aa, 3 | BRF_GRA }, 	        //  5

	{ "b0600.rom",	   		0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b0540.rom",	   		0x800000, 0xfd349e53, 4 | BRF_GRA },			//  7-1
	{ "b0601.rom",	   		0x400000, 0xa0bb1c2f, 4 | BRF_GRA }, 	        //  7-2 Expanded ROM

	{ "m0600.rom",	   		0x400000, 0x63c76ec7, 5 | BRF_SND },			//  8 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0x5e6ead02, 7 | BRF_PRG | BRF_ESS },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovshpqxzq, kovshpqxzq, pgm)
STD_ROM_FN(kovshpqxzq)

struct BurnDriver BurnDrvKovshpqxzq = {
	"kovshpqxzq", "kovshp", "pgm", NULL, "2019",
	"Knights of Valour Super Heroes Plus / Sangoku Senki Super Heroes Plus (Rerise of Heroes 20190130)\0", "Imperfect Protection Emulation", "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshpqxzqRomInfo, kovshpqxzqRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovshxasInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 一统中原 加强版 Knights of Valour - Yi Tong Zhong Yuan Plus (20190216)
static struct BurnRomInfo kovytzypRomDesc[] = {
	{ "ytzy_v201cn.rom",	0x4551ee, 0xa0ba0f05, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code , weird size!

	{ "t0600.rom",    		0x800000, 0xa919ec0f, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0xff7a4373, 3 | BRF_GRA }, 	        //  3 
	{ "a0602.rom",	   		0x800000, 0xf25b6930, 3 | BRF_GRA }, 	        //  4
	{ "a0540.rom",	   		0x800000, 0x9d30ffbc, 3 | BRF_GRA }, 	        //  5

	{ "b0600.rom",	   		0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b0540.rom",	   		0x800000, 0x2de11691, 4 | BRF_GRA },			//  7

	{ "m0600.rom",	   		0x400000, 0x1f981f2c, 5 | BRF_SND },			//  8 Samples

	{ "kovshp_v100_china.asic", 	0x004000, 0xddb852e9, 7 | BRF_PRG | BRF_ESS },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovytzyp, kovytzyp, pgm)
STD_ROM_FN(kovytzyp)

struct BurnDriver BurnDrvKovytzyp = {
	"kovytzyp", NULL, "pgm", NULL, "2019",
	"Knights of Valour  - Yi Tong Zhong Yuan (Plus 20190216)\0", "Imperfect Protection Emulation", "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovytzypRomInfo, kovytzypRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovshpInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 乱世枭雄 惊天战神 天王版 Knights of Valour Super Heroes Plus (Incredible God of War - Sky King 20190228)
static struct BurnRomInfo kovshp1v4xtwRomDesc[] = {
	{ "m0601.rom",			0x200000, 0x96f921d6, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code
	{ "m0602.rom",			0x200000, 0xa31bd590, 1 | BRF_PRG | BRF_ESS },  //  1 68K Code

	{ "t0600.rom",    		0x800000, 0xa919ec0f, 2 | BRF_GRA },			//  2 Tile data

	{ "a0600x.rom",	   		0x2000000, 0x92444dcd, 3 | BRF_GRA },			//  3 Sprite Color Data

	{ "b0600x.rom",	   		0x1000000, 0x5d5d9512, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "p0600h.rom",	   		0x400000, 0xec3f335d, 5 | BRF_SND },			//  5 Samples

	{ "kovshp_v100_china.asic", 	0x004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS },  //  6 Internal ARM7 Rom
};

STDROMPICKEXT(kovshp1v4xtw, kovshp1v4xtw, pgm)
STD_ROM_FN(kovshp1v4xtw)

struct BurnDriver BurnDrvKovshp1v4xtw = {
	"kovshp1v4xtw", "kovshp", "pgm", NULL, "2019",
	"Knights of Valour Super Heroes Plus (Incredible God of War - Sky King 20190228)\0", "Imperfect Protection Emulation", "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshp1v4xtwRomInfo, kovshp1v4xtwRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovshxasInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 乱世枭雄 太平乱世 传说版 Knights of Valour Super Heroes Plus (Peace In Chaotic Times - Legend 20190321)
static struct BurnRomInfo kovshptplscsRomDesc[] = {
	{ "m0601.rom",		0x200000, 0x86322d6a, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code
	{ "m0602.rom",		0x200000, 0xb673c954, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",    		0x800000, 0xa919ec0f, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600x.rom",	   		0x2000000, 0xdab91337, 3 | BRF_GRA },			//  2-5 Sprite Color Data

	{ "b0600x.rom",	   		0x1000000, 0x22b3beb8, 4 | BRF_GRA },			//  6-7 Sprite Masks & Color Indexes

	{ "p0600h.rom",	   		0x400000, 0x3ada4fd6, 5 | BRF_SND },			//  8 Samples

	{ "kovshp_v100_china.asic", 	0x004000, 0x0d87d4a1, 7 | BRF_PRG | BRF_ESS },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovshptplscs, kovshptplscs, pgm)
STD_ROM_FN(kovshptplscs)

struct BurnDriver BurnDrvKovshptplscs = {
	"kovshptplscs", "kovshp", "pgm", NULL, "2019",
	"Knights of Valour Super Heroes Plus (Peace In Chaotic Times - Legend 20190321)\0", "Imperfect Protection Emulation", "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshptplscsRomInfo, kovshptplscsRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovshxasInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 乱世枭雄 无双版 Knights of Valour Super Heroes Plus (Unrivalled Version 20190319)
static struct BurnRomInfo kovshpwsbRomDesc[] = {
	{ "m0601.rom",		0x200000, 0x82579022, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code
	{ "m0602.rom",		0x200000, 0x69c260ba, 1 | BRF_PRG | BRF_ESS },  //  1 68K Code

	{ "t0600.rom",    		0x800000, 0xa919ec0f, 2 | BRF_GRA },			//  2 Tile data

	{ "a0600x.rom",	   		0x2000000, 0xa3f903ca, 3 | BRF_GRA },			//  3 Sprite Color Data

	{ "b0600x.rom",	   		0x1000000, 0x7d4930d4, 4 | BRF_GRA },			//  4 Sprite Masks & Color Indexes

	{ "p0600h.rom",	   		0x400000, 0x3ada4fd6, 5 | BRF_SND },			//  8 Samples

	{ "kovshp_v100_china.asic", 	0x004000, 0x7ece721c, 7 | BRF_PRG | BRF_ESS },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovshpwsb, kovshpwsb, pgm)
STD_ROM_FN(kovshpwsb)

struct BurnDriver BurnDrvKovshpwsb = {
	"kovshpwsb", "kovshp", "pgm", NULL, "2019",
	"Knights of Valour Super Heroes Plus (Unrivalled Version 20190319)\0", "Imperfect Protection Emulation", "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshpwsbRomInfo, kovshpwsbRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovshxasInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 乱世枭雄 加强版 1.9 天王版 Knights of Valour Super Heroes Plus (Plus v1.9 - Sky King 20190320)
static struct BurnRomInfo kovshpd19RomDesc[] = {
	{ "p0600hd.rom",		0x400000, 0x7cffefac, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",    		0x800000, 0xa919ec0f, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0xeaa46772, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0xd94abe4b, 3 | BRF_GRA }, 	        //  3 
	{ "a0602.rom",	   		0x800000, 0xe7a32959, 3 | BRF_GRA }, 	        //  4
	{ "a0540.rom",	   		0x800000, 0x4f4ac911, 3 | BRF_GRA }, 	        //  5

	{ "b0600.rom",	   		0x800000, 0xf2a85ce6, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b0540.rom",	   		0x800000, 0x94eb0579, 4 | BRF_GRA },			//  7

	{ "m0600.rom",	   		0x400000, 0x3ada4fd6, 5 | BRF_SND },			//  8 Samples

	{ "kovshpd_v100_china.asic", 	0x004000, 0xb3123ccb, 7 | BRF_PRG | BRF_ESS },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovshpd19, kovshpd19, pgm)
STD_ROM_FN(kovshpd19)

struct BurnDriver BurnDrvKovshpd19 = {
	"kovshpd19", "kovshp", "pgm", NULL, "2019",
	"Knights of Valour Super Heroes Plus (Plus v1.9 - Sky King 20190320)\0", "Imperfect Protection Emulation", "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshpd19RomInfo, kovshpd19RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshpInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 乱世枭雄 血赵云版 103 Knights of Valour Super Heroes Plus (Zhao Yun Version 103 20180922)
static struct BurnRomInfo kovshpbRomDesc[] = {
	{ "p0600h.rom",			0x400000, 0x46410a40, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",    		0x800000, 0xa919ec0f, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0x39d6f300, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0xff7a4373, 3 | BRF_GRA }, 	        //  3 
	{ "a0602.rom",	   		0x800000, 0xe7a32959, 3 | BRF_GRA }, 	        //  4
	{ "a0540.rom",	   		0x800000, 0xeceac4bd, 3 | BRF_GRA }, 	        //  5

	{ "b0600.rom",	   		0x800000, 0xc9601515, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b0540.rom",	   		0x800000, 0x69c542b7, 4 | BRF_GRA },			//  7

	{ "m0600.rom",	   		0x400000, 0x3ada4fd6, 5 | BRF_SND },			//  8 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovshpb, kovshpb, pgm)
STD_ROM_FN(kovshpb)

struct BurnDriver BurnDrvKovshpb = {
	"kovshpb", "kovshp", "pgm", NULL, "2018",
	"Knights of Valour Super Heroes Plus (Zhao Yun Version 103 20180922)\0", "Imperfect Protection Emulation", "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshpbRomInfo, kovshpbRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshpInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 乱世英雄 Knights of Valour Super Heroes Plus
static struct BurnRomInfo kovlsyxRomDesc[] = {
	{ "lsyx_v500cn.rom",		0x400000, 0xdbfb1a8e, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",    		0x800000, 0xa919ec0f, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0x47794602, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0xd94abe4b, 3 | BRF_GRA }, 	        //  3 
	{ "a0602.rom",	   		0x800000, 0xe7a32959, 3 | BRF_GRA }, 	        //  4
	{ "a0540.rom",	   		0x800000, 0x6afd8e35, 3 | BRF_GRA }, 	        //  5
	{ "a0541.rom",	   		0x800000, 0x164de1a8, 3 | BRF_GRA }, 	        //  Expanded ROM

	{ "b0600.rom",	   		0x800000, 0xf48a3b3d, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b0540.rom",	   		0x800000, 0xc3eefa33, 4 | BRF_GRA },			//  7

	{ "m0600.rom",	   		0x400000, 0x459ad15f, 5 | BRF_SND },			//  8 Samples

	{ "lsyx_v100_china.asic", 	0x004000, 0x8598d603, 7 | BRF_PRG | BRF_ESS },  //  9 Internal ARM7 Rom


};

static struct BurnRomInfo kovlsyxBIOSRomDesc[] = {
	{ "pgm_t01s.rom", 		0x200000, 0x1a7123a0, BRF_GRA | BRF_BIOS },	// 0x80 - 8x8 Text Layer Tiles
	{ "lsyx_m01s.rom", 		0x200000, 0x81a6aeca, BRF_SND | BRF_BIOS },	// 0x81 - Samples

	{ "pgm_p01s.u20", 		0x020000, 0xe42b166e, BRF_PRG | BRF_BIOS },	// 0x82 - 68K BIOS (V0001,     older  - 02/26/97 - 11:14:09)
	{ "pgm_p02s.u20", 		0x020000, 0x78c15fa2, BRF_PRG | BRF_BIOS },	// 0x83 - 68K BIOS (V0001,     newer  - 07/10/97 - 16:36:08)
#if !defined (ROM_VERIFY)
	{ "ddp3_bios.u37",		0x080000, 0xb3cc5c8f, BRF_PRG | BRF_BIOS }, // 0x84 - 68K BIOS (V0001,     custom - 07/17/97 - 19:44:59)
	{ "bios.u42",     		0x020000, 0x517cf7a2, BRF_PRG | BRF_BIOS },	// 0x85 - 68K BIOS (V0001-01J, custom - 02/25/03 - 17:51:01)
#else
	{ "",             		0x000000, 0x00000000, 0                  },	// 0x84
	{ "",             		0x000000, 0x00000000, 0                  },	// 0x85
#endif
};

STDROMPICKEXT(kovlsyx, kovlsyx, kovlsyxBIOS)
STD_ROM_FN(kovlsyx)

struct BurnDriver BurnDrvKovlsyx = {
	"kovlsyx", "kovshp", "pgm", NULL, "2017",
	"Knights of Valour Super Heroes Plus\0", "Imperfect Protection Emulation", "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovlsyxRomInfo, kovlsyxRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshpInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 乱世英雄 求生之路 Knights of Valour Super Heroes Plus (Road To Survival 20180224)
static struct BurnRomInfo kovshpqszlRomDesc[] = {
	{ "p0600h.rom",		0x400000, 0x086d100b, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",    		0x800000, 0xa919ec0f, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0x72643288, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0x9c1d1bea, 3 | BRF_GRA }, 	        //  3 
	{ "a0602.rom",	   		0x800000, 0x7b437e0f, 3 | BRF_GRA }, 	        //  4
	{ "a0540.rom",	   		0x800000, 0xb038e568, 3 | BRF_GRA }, 	        //  5
	{ "a0541.rom",	   		0x800000, 0x1fa3c921, 3 | BRF_GRA }, 	        //  Expanded ROM

	{ "b0600.rom",	   		0x800000, 0x0646c5ef, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b0540.rom",	   		0x800000, 0x03a1641e, 4 | BRF_GRA },			//  7

	{ "m0600.rom",	   		0x400000, 0xec3f335d, 5 | BRF_SND },			//  8 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0x4627f36f, 7 | BRF_PRG | BRF_ESS },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovshpqszl, kovshpqszl, pgm)
STD_ROM_FN(kovshpqszl)

struct BurnDriver BurnDrvKovshpqszl = {
	"kovshpqszl", NULL, "pgm", NULL, "2018",
	"Knights of Valour Super Heroes Plus (Road To Survival 20180224)\0", "Imperfect Protection Emulation", "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshpqszlRomInfo, kovshpqszlRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshpInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 乱世英雄 求生之路 天王版 Knights of Valour Super Heroes Plus (Road To Survival - Sky King 20190319)
static struct BurnRomInfo kovshpqszltwRomDesc[] = {
	{ "p0600h.rom",		0x4006f8, 0xbac75779, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",    		0x800000, 0xa919ec0f, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0x72643288, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0x9c1d1bea, 3 | BRF_GRA }, 	        //  3 
	{ "a0602.rom",	   		0x800000, 0x7b437e0f, 3 | BRF_GRA }, 	        //  4
	{ "a0540.rom",	   		0x800000, 0xb038e568, 3 | BRF_GRA }, 	        //  5
	{ "a0541.rom",	   		0x800000, 0x1fa3c921, 3 | BRF_GRA }, 	        //  Expanded ROM

	{ "b0600.rom",	   		0x800000, 0x0646c5ef, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b0540.rom",	   		0x800000, 0x03a1641e, 4 | BRF_GRA },			//  7

	{ "m0600.rom",	   		0x400000, 0xec3f335d, 5 | BRF_SND },			//  8 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0x4627f36f, 7 | BRF_PRG | BRF_ESS },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovshpqszltw, kovshpqszltw, pgm)
STD_ROM_FN(kovshpqszltw)

struct BurnDriver BurnDrvKovshpqszltw = {
	"kovshpqszltw", "kovshpqszl", "pgm", NULL, "2019",
	"Knights of Valour Super Heroes Plus (Road To Survival - Sky King 20190319)\0", "Imperfect Protection Emulation", "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshpqszltwRomInfo, kovshpqszltwRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshpInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 乱世英雄 群雄逐鹿 Knights of Valour Super Heroes Plus (Powerful Politicians Fighting For Supremacy 20180411)
static struct BurnRomInfo kovshpd3RomDesc[] = {
	{ "p0600h.rom",			0x400000, 0xdfb1f3f8, 1 | BRF_PRG | BRF_ESS },

	{ "t0600.rom",    		0x800000, 0xa919ec0f, 2 | BRF_GRA },

	{ "a0600.rom",	   		0x800000, 0x72643288, 3 | BRF_GRA },
	{ "a0601.rom",	   		0x800000, 0x9c1d1bea, 3 | BRF_GRA },
	{ "a0602.rom",	   		0x800000, 0x7b437e0f, 3 | BRF_GRA },
	{ "a0540.rom",	   		0x800000, 0x612306ed, 3 | BRF_GRA },
	{ "a0541.rom",	   		0x800000, 0xfac5160e, 3 | BRF_GRA }, 	        //  Expanded ROM

	{ "b0600.rom",	   		0x800000, 0x0646c5ef, 4 | BRF_GRA },
	{ "b0540.rom",	   		0x800000, 0xb7032905, 4 | BRF_GRA },
	{ "b0601.rom",	   		0x800000, 0x095378f3, 4 | BRF_GRA },			//  Expanded ROM? Not sure whether it is needed or not!

	{ "m0600.rom",	   		0x400000, 0xec3f335d, 5 | BRF_SND },

	{ "kovsh_v100_china.asic", 	0x004000, 0xd9a43086, 7 | BRF_PRG | BRF_ESS },
};

STDROMPICKEXT(kovshpd3, kovshpd3, pgm)
STD_ROM_FN(kovshpd3)

struct BurnDriver BurnDrvKovshpd3 = {
	"kovshpd3", NULL, "pgm", NULL, "2018",
	"Knights of Valour Super Heroes Plus (Powerful Politicians Fighting For Supremacy 20180411)\0", "Imperfect Protection Emulation", "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshpd3RomInfo, kovshpd3RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshpInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 乱世英雄 群雄逐鹿 天王版 Knights of Valour Super Heroes Plus (Powerful Politicians Fighting For Supremacy - Sky King 20190319)
static struct BurnRomInfo kovshpd3twRomDesc[] = {
	{ "p0600h.rom",			0x4004b2, 0xc8f14031, 1 | BRF_PRG | BRF_ESS },  // Incorrect size ???

	{ "t0600.rom",    		0x800000, 0xa919ec0f, 2 | BRF_GRA },

	{ "a0600.rom",	   		0x800000, 0x72643288, 3 | BRF_GRA },
	{ "a0601.rom",	   		0x800000, 0x9c1d1bea, 3 | BRF_GRA },
	{ "a0602.rom",	   		0x800000, 0x7b437e0f, 3 | BRF_GRA },
	{ "a0540.rom",	   		0x800000, 0x612306ed, 3 | BRF_GRA },
	{ "a0541.rom",	   		0x800000, 0xfac5160e, 3 | BRF_GRA }, 	        //  Expanded ROM

	{ "b0600.rom",	   		0x800000, 0x0646c5ef, 4 | BRF_GRA },
	{ "b0540.rom",	   		0x800000, 0xb7032905, 4 | BRF_GRA },
	{ "b0601.rom",	   		0x800000, 0x095378f3, 4 | BRF_GRA },			//  Expanded ROM? Not sure whether it is needed or not!

	{ "m0600.rom",	   		0x400000, 0xec3f335d, 5 | BRF_SND },

	{ "kovsh_v100_china.asic", 	0x004000, 0xd9a43086, 7 | BRF_PRG | BRF_ESS },
};

STDROMPICKEXT(kovshpd3tw, kovshpd3tw, pgm)
STD_ROM_FN(kovshpd3tw)

struct BurnDriver BurnDrvKovshpd3tw = {
	"kovshpd3tw", "kovshpd3", "pgm", NULL, "2019",
	"Knights of Valour Super Heroes Plus (Powerful Politicians Fighting For Supremacy - Sky King 20190319)\0", "Imperfect Protection Emulation", "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshpd3twRomInfo, kovshpd3twRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshpInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 乱世英雄 神云本色 Knights of Valour Super Heroes Plus (Inherent Qualities Of Shen Yun 20180902)
static struct BurnRomInfo kovshpd3xRomDesc[] = {
	{ "p0600h.rom",			0x400000, 0xc7885c6b, 1 | BRF_PRG | BRF_ESS },

	{ "t0600.rom",    		0x800000, 0xa919ec0f, 2 | BRF_GRA },

	{ "a0600.rom",	   		0x800000, 0x72643288, 3 | BRF_GRA },
	{ "a0601.rom",	   		0x800000, 0x5334333b, 3 | BRF_GRA },
	{ "a0602.rom",	   		0x800000, 0x7b437e0f, 3 | BRF_GRA },
	{ "a0540.rom",	   		0x800000, 0x612306ed, 3 | BRF_GRA },
	{ "a0541.rom",	   		0x800000, 0xfac5160e, 3 | BRF_GRA }, 	        //  Expanded ROM

	{ "b0600.rom",	   		0x800000, 0xefd47825, 4 | BRF_GRA },
	{ "b0540.rom",	   		0x800000, 0xb7032905, 4 | BRF_GRA },
	
	{ "m0600.rom",	   		0x400000, 0xec3f335d, 5 | BRF_SND },

	{ "kovsh_v100_china.asic", 	0x004000, 0xd9a43086, 7 | BRF_PRG | BRF_ESS },
};

STDROMPICKEXT(kovshpd3x, kovshpd3x, pgm)
STD_ROM_FN(kovshpd3x)

struct BurnDriver BurnDrvKovshpd3x = {
	"kovshpd3x", "kovshpd3", "pgm", NULL, "2018",
	"Knights of Valour Super Heroes Plus (Inherent Qualities Of Shen Yun 20180902)\0", "Imperfect Protection Emulation", "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshpd3xRomInfo, kovshpd3xRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshpInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 风云再起 月华传说 Knights of Valour Superheroes (Legend Of Moonlight 20151105)
// Shrinke the ROM due to weird file size.
static struct BurnRomInfo kovshwsglRomDesc[] = {
	{ "p0600.322",	   		0x400000, 0x9c8a9890, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",     		0x800000, 0xa7cf5653, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0xa616c24d, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0xff7a4373, 3 | BRF_GRA },			//  3
	{ "a0602.rom",	   		0x800000, 0x247eb04e, 3 | BRF_GRA },			//  4
	{ "a0603.rom",	   		0x400000, 0xec31abda, 3 | BRF_GRA },			//  5
	{ "a0604.rom",	   		0x400000, 0x8e3c32bc, 3 | BRF_GRA },			//  6

	{ "b0600.rom",	   		0x800000, 0x2c5421a6, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "b0601.rom",	   		0x400000, 0x1e917e36, 4 | BRF_GRA },			//  8
	{ "b0602.rom",	   		0x400000, 0x6bfaa3db, 4 | BRF_GRA },			//  9

	{ "m0600.rom",	   		0x400000, 0x3ada4fd6, 5 | BRF_SND },			// 10 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0xd79943df, 7 | BRF_PRG | BRF_ESS }, 
};

STDROMPICKEXT(kovshwsgl, kovshwsgl, pgm)
STD_ROM_FN(kovshwsgl)

struct BurnDriver BurnDrvKovshwsgl = {
	"kovshwsgl", "kovsh", "pgm", NULL, "2015",
	"Knights of Valour Superheroes (Legend Of Moonlight 20151105)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshwsglRomInfo, kovshwsglRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 乱世英雄 血战八方 Knights of Valour Super Heroes Plus (Bloody Battle Against All Quarters 20170801)
static struct BurnRomInfo kovshpdxRomDesc[] = {
	{ "p0600h.rom",			0x400000, 0x8e22f85c, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",    		0x800000, 0xa919ec0f, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0x72643288, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0x9c1d1bea, 3 | BRF_GRA }, 	        //  3 
	{ "a0602.rom",	   		0x800000, 0x7b437e0f, 3 | BRF_GRA }, 	        //  4
	{ "a0540.rom",	   		0x800000, 0x85e9c4c0, 3 | BRF_GRA }, 	        //  5
	{ "a0541.rom",	   		0x800000, 0xeb14e314, 3 | BRF_GRA }, 	        //  Expanded ROM

	{ "b0600.rom",	   		0x800000, 0x0646c5ef, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b0540.rom",	   		0x800000, 0x9a295eb6, 4 | BRF_GRA },			//  7
	{ "b0601.rom",	   		0x800000, 0x75c127d3, 4 | BRF_GRA }, 	        //  Expanded ROM

	{ "m0600.rom",	   		0x400000, 0xec3f335d, 5 | BRF_SND },			//  8 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0x1e968c13, 7 | BRF_PRG | BRF_ESS },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovshpdx, kovshpdx, pgm)
STD_ROM_FN(kovshpdx)

struct BurnDriver BurnDrvKovshpdx = {
	"kovshpdx", "kovshp", "pgm", NULL, "2017",
	"Knights of Valour Super Heroes Plus (Bloody Battle Against All Quarters 20170801)\0", "Imperfect Protection Emulation", "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshpdxRomInfo, kovshpdxRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshpInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 乱世枭雄 诸神黄昏 Knights of Valour Super Heroes Plus (Twilight for the Gods 20170323)
static struct BurnRomInfo kovshp1v4RomDesc[] = {
	{ "p0600h.rom",			0x400000, 0x52167c43, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",    		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0xff7a4373, 3 | BRF_GRA }, 	        //  3 
	{ "a0602.rom",	   		0x800000, 0xe7a32959, 3 | BRF_GRA }, 	        //  4
	{ "a0540.rom",	   		0x800000, 0x7bfda264, 3 | BRF_GRA }, 	        //  5

	{ "b0600.rom",	   		0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b0540.rom",	   		0x800000, 0x5e9ff06a, 4 | BRF_GRA },			//  7

	{ "m0600.rom",	   		0x400000, 0x3ada4fd6, 5 | BRF_SND },			//  8 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0xe844fede, 7 | BRF_PRG | BRF_ESS },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovshp1v4, kovshp1v4, pgm)
STD_ROM_FN(kovshp1v4)

struct BurnDriver BurnDrvKovshp1v4 = {
	"kovshp1v4", "kovshp", "pgm", NULL, "2017",
	"Knights of Valour Super Heroes Plus (Twilight for the Gods 20170323)\0", "Imperfect Protection Emulation", "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshp1v4RomInfo, kovshp1v4RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshpInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 乱世枭雄 诸神黄昏 黎明版 Knights of Valour Super Heroes Plus (Twilight for the Gods - Dawn 20180506)
static struct BurnRomInfo kovshp1v4sRomDesc[] = {
	{ "p0600h.rom",			0x400000, 0x6967eb83, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",    		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0xff7a4373, 3 | BRF_GRA }, 	        //  3 
	{ "a0602.rom",	   		0x800000, 0xe7a32959, 3 | BRF_GRA }, 	        //  4
	{ "a0540.rom",	   		0x800000, 0x241cbd34, 3 | BRF_GRA }, 	        //  5

	{ "b0600.rom",	   		0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b0540.rom",	   		0x800000, 0x55cebe8c, 4 | BRF_GRA },			//  7

	{ "m0600.rom",	   		0x400000, 0xec3f335d, 5 | BRF_SND },			//  8 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovshp1v4s, kovshp1v4s, pgm)
STD_ROM_FN(kovshp1v4s)

struct BurnDriver BurnDrvKovshp1v4s = {
	"kovshp1v4s", "kovshp", "pgm", NULL, "2018",
	"Knights of Valour Super Heroes Plus (Twilight for the Gods - Dawn 20180506)\0", "Imperfect Protection Emulation", "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshp1v4sRomInfo, kovshp1v4sRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshpInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 一统中原 青版 Knights of Valour - Yi Tong Zhong Yuan (Blue Version) (20181209)
static struct BurnRomInfo kovytzyftxqRomDesc[] = {
	{ "ytzy_v201cn.rom",	0x600000, 0x8a5702ff, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code , weird size!

	{ "t0600.rom",    		0x800000, 0xa919ec0f, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0xff7a4373, 3 | BRF_GRA }, 	        //  3 
	{ "a0602.rom",	   		0x800000, 0xf25b6930, 3 | BRF_GRA }, 	        //  4
	{ "a0540.rom",	   		0x800000, 0x9d30ffbc, 3 | BRF_GRA }, 	        //  5

	{ "b0600.rom",	   		0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b0540.rom",	   		0x800000, 0x2de11691, 4 | BRF_GRA },			//  7
	{ "b0601.rom",	   		0x400000, 0x16a7afde, 4 | BRF_GRA },			//  Expanded ROM??? Maybe not necessary?

	{ "m0600.rom",	   		0x400000, 0x1f981f2c, 5 | BRF_SND },			//  8 Samples

	{ "kovshp_v100_china.asic", 	0x004000, 0x2b170e33, 7 | BRF_PRG | BRF_ESS },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovytzyftxq, kovytzyftxq, pgm)
STD_ROM_FN(kovytzyftxq)

struct BurnDriver BurnDrvKovytzyftxq = {
	"kovytzyftxq", NULL, "pgm", NULL, "2018",
	"Knights of Valour - Yi Tong Zhong Yuan (Blue Version) (20181209)\0", "Imperfect Protection Emulation", "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovytzyftxqRomInfo, kovytzyftxqRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovshxasDIPInfo,
	kovshpInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪 乱世枭雄 枭雄天下 2019 Knights of Valour Super Heroes Plus ( Ambitious Heroes Over The World 2019 20190301)
static struct BurnRomInfo kovshptxRomDesc[] = {
	{ "p0600h2019.rom",		0x400000, 0xc3579501, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0600.rom",    		0x800000, 0x4acc1ad6, 2 | BRF_GRA },			//  1 Tile data

	{ "a0600.rom",	   		0x800000, 0xd8167834, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0601.rom",	   		0x800000, 0xff7a4373, 3 | BRF_GRA }, 	        //  3 
	{ "a0602.rom",	   		0x800000, 0xe7a32959, 3 | BRF_GRA }, 	        //  4
	{ "a0540.rom",	   		0x800000, 0xc68f3b54, 3 | BRF_GRA }, 	        //  5

	{ "b0600.rom",	   		0x800000, 0x7d3cd059, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b0540.rom",	   		0x800000, 0x664adc02, 4 | BRF_GRA },			//  7

	{ "m0600.rom",	   		0x400000, 0x3ada4fd6, 5 | BRF_SND },			//  8 Samples

	{ "kovsh_v100_china.asic", 	0x004000, 0x0f09a5c1, 7 | BRF_PRG | BRF_ESS },  //  9 Internal ARM7 Rom
};

STDROMPICKEXT(kovshptx, kovshptx, pgm)
STD_ROM_FN(kovshptx)

struct BurnDriver BurnDrvKovshptx = {
	"kovshptx", "kovshp", "pgm", NULL, "2019",
	"Knights of Valour Super Heroes Plus ( Ambitious Heroes Over The World 2019 20190301)\0", "Imperfect Protection Emulation", "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kovshptxRomInfo, kovshptxRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kovshpInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

//kov2p

// FBA4DROID 三国战纪2p 苍天航路 Knights of Valour 2 Plus - Nine Dragons (Heaven Route 20190120)
static struct BurnRomInfo kov2pcthlRomDesc[] = {
	{ "b1202.rom",  		0x200000, 0x8b8d3bf4, 1 | BRF_PRG | BRF_ESS },  //  0-1 68K Code
	{ "b1203.rom",  		0x200000, 0xf2926e49, 1 | BRF_PRG | BRF_ESS },  //  0-2 68K Code

	{ "t1200.rom",	   		0x800000, 0xd7e26609, 2 | BRF_GRA },			//  1 Tile data

	{ "a1200.rom",	   		0x800000, 0xceeb81d8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a1201.rom",   		0x800000, 0x21063ca7, 3 | BRF_GRA },			//  3
	{ "a1202.rom",	   		0x800000, 0x4bb92fae, 3 | BRF_GRA },			//  4
	{ "a1203.rom",	   		0x800000, 0xe73cb627, 3 | BRF_GRA },			//  5
	{ "a1204.rom",   		0x200000, 0x14b4b5bb, 3 | BRF_GRA },			//  6

	{ "b1200.rom",	   		0x800000, 0xbed7d994, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "b1201.rom",	   		0x800000, 0xf251eb57, 4 | BRF_GRA },			//  8

	{ "m1200.rom",	   		0x800000, 0xb0d88720, 5 | BRF_SND },			//  9 Samples

	{ "kov2p_igs027a_china.bin",	0x004000, 0x19a0bd95, 7 | BRF_PRG | BRF_ESS },  // 10 Internal ARM7 Rom

	{ "v200-16.rom",   		0x200000, 0x16a0c11f, 8 | BRF_PRG | BRF_ESS },  // 11 External ARM7 Rom
};

STDROMPICKEXT(kov2pcthl, kov2pcthl, pgm)
STD_ROM_FN(kov2pcthl)

struct BurnDriver BurnDrvKov2pcthl = {
	"kov2pcthl", "kov2p", "pgm", NULL, "2019",
	"Knights of Valour 2 Plus - Nine Dragons (Heaven Route 20190120)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kov2pcthlRomInfo, kov2pcthlRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kov2pInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

//FBA4DROID 三国战纪2p 神武传奇2019 Knights of Valour 2 Plus - Nine Dragons (Legend of Conquering Generals 20190120)
static struct BurnRomInfo kov2pswcqRomDesc[] = {
	{ "b1202.rom",  		0x200000, 0x66f561e6, 1 | BRF_PRG | BRF_ESS },  //  0-1 68K Code
	{ "b1203.rom",  		0x200000, 0x85cc2079, 1 | BRF_PRG | BRF_ESS },  //  0-2 68K Code

	{ "t1200.rom",	   		0x800000, 0xd7e26609, 2 | BRF_GRA },			//  1 Tile data

	{ "a1200.rom",	   		0x800000, 0xceeb81d8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a1201.rom",   		0x800000, 0x21063ca7, 3 | BRF_GRA },			//  3
	{ "a1202.rom",	   		0x800000, 0x4bb92fae, 3 | BRF_GRA },			//  4
	{ "a1203.rom",	   		0x800000, 0xe73cb627, 3 | BRF_GRA },			//  5
	{ "a1204.rom",   		0x200000, 0x14b4b5bb, 3 | BRF_GRA },			//  6

	{ "b1200.rom",	   		0x800000, 0xbed7d994, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "b1201.rom",	   		0x800000, 0xf251eb57, 4 | BRF_GRA },			//  8

	{ "m1200.rom",	   		0x800000, 0xb0d88720, 5 | BRF_SND },			//  9 Samples

	{ "kov2p_igs027a_china.bin",	0x004000, 0x06214503, 7 | BRF_PRG | BRF_ESS },  // 10 Internal ARM7 Rom

	{ "v200-16.rom",   		0x200000, 0x28029563, 8 | BRF_PRG | BRF_ESS },  // 11 External ARM7 Rom
};

STDROMPICKEXT(kov2pswcq, kov2pswcq, pgm)
STD_ROM_FN(kov2pswcq)

struct BurnDriver BurnDrvKov2pswcq = {
	"kov2pswcq", "kov2p", "pgm", NULL, "2019",
	"Knights of Valour 2 Plus - Nine Dragons (Legend of Conquering Generals 20190120)\0", NULL, "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kov2pswcqRomInfo, kov2pswcqRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kov2pInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪2p 天火明名 Knights of Valour 2 Plus - (Skyfire name)
static struct BurnRomInfo kov2pmemmRomDesc[] = {
	{ "v204-32m.dec",  		0x400000, 0x7f4b8e0b, 1 | BRF_PRG | BRF_ESS },  ////  0 68K Code

	{ "t1200.rom",	   		0x800000, 0xd7e26609, 2 | BRF_GRA },			//  1 Tile data

	{ "a1200.rom",	   		0x800000, 0xceeb81d8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a1201.rom",   		0x800000, 0x21063ca7, 3 | BRF_GRA },			//  3
	{ "a1202.rom",	   		0x800000, 0x4bb92fae, 3 | BRF_GRA },			//  4
	{ "a1203.rom",	   		0x800000, 0xe73cb627, 3 | BRF_GRA },			//  5
	{ "a1204.rom",   		0x200000, 0x14b4b5bb, 3 | BRF_GRA },			//  6

	{ "b1200.rom",	   		0x800000, 0xbed7d994, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "b1201.rom",	   		0x800000, 0xf251eb57, 4 | BRF_GRA },			//  8

	{ "m1200.rom",	   		0x800000, 0xb0d88720, 5 | BRF_SND },			//  9 Samples

	{ "kov2p_igs027a_china.bin",	0x004000, 0x19a0bd95, 7 | BRF_PRG | BRF_ESS },  // 10 Internal ARM7 Rom

	{ "v200-16.rom",   		0x200000, 0x16a0c11f, 8 | BRF_PRG | BRF_ESS },  // 11 External ARM7 Rom
};

STDROMPICKEXT(kov2pmemm, kov2pmemm, pgm)
STD_ROM_FN(kov2pmemm)

struct BurnDriver BurnDrvKov2pmemm = {
	"kov2pmemm", "kov2p", "pgm", NULL, "2018",
	"Knights of Valour 2 Plus - (Skyfire name)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kov2pmemmRomInfo, kov2pmemmRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kov2pInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪2p 灵魂剥离 Knights of Valour 2 Plus - (Soul Stripping 20181030)
static struct BurnRomInfo kov2pmessRomDesc[] = {
	{ "v204-32m.rom",  		0x400000, 0x9b3ce7ca, 1 | BRF_PRG | BRF_ESS },  ////  0 68K Code

	{ "t1200.rom",	   		0x800000, 0xd7e26609, 2 | BRF_GRA },			//  1 Tile data

	{ "a1200.rom",	   		0x800000, 0xceeb81d8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a1201.rom",   		0x800000, 0x21063ca7, 3 | BRF_GRA },			//  3
	{ "a1202.rom",	   		0x800000, 0x4bb92fae, 3 | BRF_GRA },			//  4
	{ "a1203.rom",	   		0x800000, 0xe73cb627, 3 | BRF_GRA },			//  5
	{ "a1204.rom",   		0x200000, 0x14b4b5bb, 3 | BRF_GRA },			//  6

	{ "b1200.rom",	   		0x800000, 0xbed7d994, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "b1201.rom",	   		0x800000, 0xf251eb57, 4 | BRF_GRA },			//  8

	{ "m1200.rom",	   		0x800000, 0xb0d88720, 5 | BRF_SND },			//  9 Samples

	{ "kov2p_igs027a_china.bin",	0x004000, 0x19a0bd95, 7 | BRF_PRG | BRF_ESS },  // 10 Internal ARM7 Rom

	{ "v200-16.rom",   		0x200000, 0x16a0c11f, 8 | BRF_PRG | BRF_ESS },  // 11 External ARM7 Rom
};

STDROMPICKEXT(kov2pmess, kov2pmess, pgm)
STD_ROM_FN(kov2pmess)

struct BurnDriver BurnDrvKov2pmess = {
	"kov2pmess", "kov2p", "pgm", NULL, "2018",
	"Knights of Valour 2 Plus - (Soul Stripping 20181030)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kov2pmessRomInfo, kov2pmessRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kov2pInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪2p 天降神兵 九龙传奇 Knights of Valour 2 Plus - (Legend Of Nine Dragons 20180924)
static struct BurnRomInfo kov2pjlcqRomDesc[] = {
	{ "v206-32m.rom",  		0x400000, 0x836075b8, 1 | BRF_PRG | BRF_ESS },  ////  0 68K Code

	{ "t1200.rom",	   		0x800000, 0xd7e26609, 2 | BRF_GRA },			//  1 Tile data

	{ "a1200.rom",	   		0x800000, 0xceeb81d8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a1201.rom",   		0x800000, 0x21063ca7, 3 | BRF_GRA },			//  3
	{ "a1202.rom",	   		0x800000, 0x24a4315f, 3 | BRF_GRA },			//  4
	{ "a1203.rom",	   		0x800000, 0xe73cb627, 3 | BRF_GRA },			//  5
	{ "a1204.rom",   		0x200000, 0x14b4b5bb, 3 | BRF_GRA },			//  6

	{ "b1200.rom",	   		0x800000, 0xbed7d994, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "b1201.rom",	   		0x800000, 0x07347997, 4 | BRF_GRA },			//  8

	{ "m1200.rom",	   		0x800000, 0x4bc06a41, 5 | BRF_SND },			//  9 Samples

	{ "kov2p_igs027a_china.bin",	0x004000, 0x19a0bd95, 7 | BRF_PRG | BRF_ESS },  // 10 Internal ARM7 Rom

	{ "v200-16.rom",   		0x200000, 0x16a0c11f, 8 | BRF_PRG | BRF_ESS },  // 11 External ARM7 Rom
};

STDROMPICKEXT(kov2pjlcq, kov2pjlcq, pgm)
STD_ROM_FN(kov2pjlcq)

struct BurnDriver BurnDrvKov2pjlcq = {
	"kov2pjlcq", "kov2p", "pgm", NULL, "2018",
	"Knights of Valour 2 Plus - (Legend Of Nine Dragons 20180924)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kov2pjlcqRomInfo, kov2pjlcqRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kov2pInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪2p 凤舞龙吟 Knights of Valour 2 Plus - (Dancing Phoenix, Singing Dragon 20180411)
static struct BurnRomInfo kov2pfwllRomDesc[] = {
	{ "u8-27322.rom",  		0x600000, 0x6332d98f, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t1200.rom",	   		0x800000, 0xd7e26609, 2 | BRF_GRA },			//  1 Tile data

	{ "a1200.rom",	   		0x800000, 0xceeb81d8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a1201.rom",   		0x800000, 0x21063ca7, 3 | BRF_GRA },			//  3
	{ "a1202.rom",	   		0x800000, 0x4bb92fae, 3 | BRF_GRA },			//  4
	{ "a1203.rom",	   		0x800000, 0xe73cb627, 3 | BRF_GRA },			//  5
	{ "a1204.rom",   		0x200000, 0x14b4b5bb, 3 | BRF_GRA },			//  6

	{ "b1200.rom",	   		0x800000, 0xbed7d994, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "b1201.rom",	   		0x800000, 0xf251eb57, 4 | BRF_GRA },			//  8

	{ "m1200.rom",	   		0x800000, 0xb0d88720, 5 | BRF_SND },			//  9 Samples

	{ "kov2p_arm.rom",	0x004000, 0x06214503, 7 | BRF_PRG | BRF_ESS },  // 10 Internal ARM7 Rom

	{ "v200-16.rom",   		0x200000, 0xb77ae68e, 8 | BRF_PRG | BRF_ESS },  // 11 External ARM7 Rom
};

STDROMPICKEXT(kov2pfwll, kov2pfwll, pgm)
STD_ROM_FN(kov2pfwll)

struct BurnDriver BurnDrvKov2pfwll = {
	"kov2pfwll", "kov2p", "pgm", NULL, "2018",
	"Knights of Valour 2 Plus - (Dancing Phoenix, Singing Dragon 20180411)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kov2pfwllRomInfo, kov2pfwllRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kov2pInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪2p 三国无双 Knights of Valour 2 Plus - (Dynasty Warriors 20180614)
static struct BurnRomInfo kov2pplusRomDesc[] = {
	{ "v204-32m.rom",  		0x40032a, 0x5a050189, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t1200.rom",	   		0x800000, 0xd7e26609, 2 | BRF_GRA },			//  1 Tile data

	{ "a1200.rom",	   		0x800000, 0xceeb81d8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a1201.rom",   		0x800000, 0x21063ca7, 3 | BRF_GRA },			//  3
	{ "a1202.rom",	   		0x800000, 0x4bb92fae, 3 | BRF_GRA },			//  4
	{ "a1203.rom",	   		0x800000, 0xe73cb627, 3 | BRF_GRA },			//  5
	{ "a1204.rom",   		0x200000, 0x14b4b5bb, 3 | BRF_GRA },			//  6

	{ "b1200.rom",	   		0x800000, 0xbed7d994, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "b1201.rom",	   		0x800000, 0xf251eb57, 4 | BRF_GRA },			//  8

	{ "m1200.rom",	   		0x800000, 0xb0d88720, 5 | BRF_SND },			//  9 Samples

	{ "kov2p_igs027a_china.bin",	0x004000, 0x19a0bd95, 7 | BRF_PRG | BRF_ESS },  // 10 Internal ARM7 Rom

	{ "v200-16.rom",   		0x200000, 0x16a0c11f, 8 | BRF_PRG | BRF_ESS },  // 11 External ARM7 Rom
};

STDROMPICKEXT(kov2pplus, kov2pplus, pgm)
STD_ROM_FN(kov2pplus)

struct BurnDriver BurnDrvKov2pplus = {
	"kov2pplus", "kov2p", "pgm", NULL, "2018",
	"Knights of Valour 2 Plus - (Dynasty Warriors 20180614)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kov2pplusRomInfo, kov2pplusRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kov2pInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪2p 盖世无双 Knights of Valour 2 Plus - (Unrivaled In The World 20180819)
static struct BurnRomInfo kov2ppwsRomDesc[] = {
	{ "v204-32m.rom",  		0x400000, 0x14751691, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t1200.rom",	   		0x800000, 0xd7e26609, 2 | BRF_GRA },			//  1 Tile data

	{ "a1200.rom",	   		0x800000, 0xceeb81d8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a1201.rom",   		0x800000, 0x21063ca7, 3 | BRF_GRA },			//  3
	{ "a1202.rom",	   		0x800000, 0x4bb92fae, 3 | BRF_GRA },			//  4
	{ "a1203.rom",	   		0x800000, 0xe73cb627, 3 | BRF_GRA },			//  5
	{ "a1204.rom",   		0x200000, 0x14b4b5bb, 3 | BRF_GRA },			//  6

	{ "b1200.rom",	   		0x800000, 0xbed7d994, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "b1201.rom",	   		0x800000, 0xf251eb57, 4 | BRF_GRA },			//  8

	{ "m1200.rom",	   		0x800000, 0xb0d88720, 5 | BRF_SND },			//  9 Samples

	{ "kov2p_igs027a_china.bin",	0x004000, 0x40a75c7e, 7 | BRF_PRG | BRF_ESS },  // 10 Internal ARM7 Rom

	{ "v200-16.rom",   		0x200000, 0x76642296, 8 | BRF_PRG | BRF_ESS },  // 11 External ARM7 Rom
};

STDROMPICKEXT(kov2ppws, kov2ppws, pgm)
STD_ROM_FN(kov2ppws)

struct BurnDriver BurnDrvKov2ppws = {
	"kov2ppws", "kov2p", "pgm", NULL, "2018",
	"Knights of Valour 2 Plus - (Unrivaled In The World 20180819)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kov2ppwsRomInfo, kov2ppwsRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kov2pInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪2p 盖世无双2016 Knights of Valour 2 Plus - (Unrivaled In The World 2016 20180727)
static struct BurnRomInfo kov2ppws2016RomDesc[] = {
	{ "v204-32m.rom",  		0x400000, 0x2c59cc1c, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t1200.rom",	   		0x800000, 0xd7e26609, 2 | BRF_GRA },			//  1 Tile data

	{ "a1200.rom",	   		0x800000, 0xceeb81d8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a1201.rom",   		0x800000, 0x21063ca7, 3 | BRF_GRA },			//  3
	{ "a1202.rom",	   		0x800000, 0x4bb92fae, 3 | BRF_GRA },			//  4
	{ "a1203.rom",	   		0x800000, 0xe73cb627, 3 | BRF_GRA },			//  5
	{ "a1204.rom",   		0x200000, 0x14b4b5bb, 3 | BRF_GRA },			//  6

	{ "b1200.rom",	   		0x800000, 0xbed7d994, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "b1201.rom",	   		0x800000, 0xf251eb57, 4 | BRF_GRA },			//  8

	{ "m1200.rom",	   		0x800000, 0xb0d88720, 5 | BRF_SND },			//  9 Samples

	{ "kov2p_igs027a_china.bin",	0x004000, 0x19a0bd95, 7 | BRF_PRG | BRF_ESS },  // 10 Internal ARM7 Rom

	{ "v200-16.rom",   		0x200000, 0x16a0c11f, 8 | BRF_PRG | BRF_ESS },  // 11 External ARM7 Rom
};

STDROMPICKEXT(kov2ppws2016, kov2ppws2016, pgm)
STD_ROM_FN(kov2ppws2016)

struct BurnDriver BurnDrvKov2ppws2016 = {
	"kov2ppws2016", "kov2p", "pgm", NULL, "2018",
	"Knights of Valour 2 Plus - (Unrivaled In The World 2016 20180727)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kov2ppws2016RomInfo, kov2ppws2016RomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kov2pInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 三国战纪2p 枫叶元清 Knights of Valour 2 Plus - (Feng Ye Yuan Qing 20180728)
static struct BurnRomInfo kov2pfwlyRomDesc[] = {
	{ "u8-27322.rom",  		0x400000, 0xe0c35180, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t1200.rom",	   		0x800000, 0xd7e26609, 2 | BRF_GRA },			//  1 Tile data

	{ "a1200.rom",	   		0x800000, 0xceeb81d8, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a1201.rom",   		0x800000, 0x21063ca7, 3 | BRF_GRA },			//  3
	{ "a1202.rom",	   		0x800000, 0x4bb92fae, 3 | BRF_GRA },			//  4
	{ "a1203.rom",	   		0x800000, 0xe73cb627, 3 | BRF_GRA },			//  5
	{ "a1204.rom",   		0x200000, 0x14b4b5bb, 3 | BRF_GRA },			//  6

	{ "b1200.rom",	   		0x800000, 0xbed7d994, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "b1201.rom",	   		0x800000, 0xf251eb57, 4 | BRF_GRA },			//  8

	{ "m1200.rom",	   		0x800002, 0x05dcaf3d, 5 | BRF_SND },			//  9 Samples

	{ "kov2p_igs027a_china.bin",	0x004000, 0x06214503, 7 | BRF_PRG | BRF_ESS },  // 10 Internal ARM7 Rom

	{ "v200-16.rom",   		0x200000, 0x28029563, 8 | BRF_PRG | BRF_ESS },  // 11 External ARM7 Rom
};

STDROMPICKEXT(kov2pfwly, kov2pfwly, pgm)
STD_ROM_FN(kov2pfwly)

struct BurnDriver BurnDrvKov2pfwly = {
	"kov2pfwly", "kov2p", "pgm", NULL, "2018",
	"Knights of Valour 2 Plus - (Feng Ye Yuan Qing 20180728)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM | HARDWARE_IGS_USE_ARM_CPU, GBF_SCRFIGHT, 0,
	NULL, kov2pfwlyRomInfo, kov2pfwlyRomName, NULL, NULL, NULL, NULL, pgmInputInfo, kovDIPInfo,
	kov2pInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

//olds100a

// FBA4DROID 西游释厄传超级版 大圣归来 Xi Yo Gi Shi Re Zuang Super (Return of Monkey King 20181229)
static struct BurnRomInfo olds100adsglRomDesc[] = {
	{ "p0500.v10",	   		0x400000, 0xd63d9999, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0500.rom",	   		0x400000, 0xd881726c, 2 | BRF_GRA },			//  1 Tile data
	{ "t0501.rom",	   		0x200000, 0xd2106864, 2 | BRF_GRA },			//  2

	{ "a0500.rom",	   		0x400000, 0x80a59197, 3 | BRF_GRA },			//  3 Sprite Color Data
	{ "a0501.rom",	   		0x400000, 0x98c931b1, 3 | BRF_GRA },			//  4
	{ "a0502.rom",	   		0x400000, 0xc3fcdf1d, 3 | BRF_GRA },			//  5
	{ "a0503.rom",	   		0x400000, 0x066dffec, 3 | BRF_GRA },			//  6
	{ "a0504.rom",	   		0x400000, 0x45337583, 3 | BRF_GRA },			//  7
	{ "a0505.rom",	   		0x400000, 0x5b8cf3a5, 3 | BRF_GRA },			//  8
	{ "a0506.rom",	   		0x400000, 0x2e50561c, 3 | BRF_GRA },			//  9

	{ "b0500.rom",	   		0x400000, 0xcde07f74, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "b0501.rom",	   		0x400000, 0x1546c2e9, 4 | BRF_GRA },			// 11
	{ "b0502.rom",	   		0x400000, 0xe97b31c3, 4 | BRF_GRA },			// 12
	{ "b0503.u16",	   		0x400000, 0x069ec4d1, 4 | BRF_GRA },			// 13

	{ "m0500.rom",	   		0x200000, 0x37928cdd, 5 | BRF_SND },			// 14 Samples
	
#if !defined (ROM_VERIFY)
	{ "kd-u6.512",	   		0x010000, 0xe7613dda, 9 | BRF_PRG | BRF_ESS },  // 15 Protection Rom
#else
	{ "ram_dump",	   		0x040000, 0x280cfb4e, 0 | BRF_OPT },
#endif
};

STDROMPICKEXT(olds100adsgl, olds100adsgl, pgm)
STD_ROM_FN(olds100adsgl)

struct BurnDriver BurnDrvOlds100adsgl = {
	"olds100adsgl", "olds", "pgm", NULL, "2018",
	"Xi Yo Gi Shi Re Zuang Super (Return of Monkey King 20181229)\0", "Imperfect Protection Emulation", "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_SCRFIGHT, 0,
	NULL, olds100adsglRomInfo, olds100adsglRomName, NULL, NULL, NULL, NULL, pgmInputInfo, olds100DIPInfo,
	oldsInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 西游释厄传超级版 天下无双 Xi Yo Gi Shi Re Zuang Super (Unparalleled In The World, GOTVG 20150203)
static struct BurnRomInfo olds100afybhRomDesc[] = {
	{ "p0500.v10",	   		0x400000, 0xfc1bcd42, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0500.rom",	   		0x400000, 0x4d8063e6, 2 | BRF_GRA },			//  1 Tile data
	{ "t0501.rom",	   		0x200000, 0xd2106864, 2 | BRF_GRA },			//  2

	{ "a0500.rom",	   		0x400000, 0x4d0c685f, 3 | BRF_GRA },			//  3 Sprite Color Data
	{ "a0501.rom",	   		0x400000, 0xc352d1c0, 3 | BRF_GRA },			//  4
	{ "a0502.rom",	   		0x400000, 0xc3fcdf1d, 3 | BRF_GRA },			//  5
	{ "a0503.rom",	   		0x400000, 0x066dffec, 3 | BRF_GRA },			//  6
	{ "a0504.rom",	   		0x400000, 0x45337583, 3 | BRF_GRA },			//  7
	{ "a0505.rom",	   		0x400000, 0x5b8cf3a5, 3 | BRF_GRA },			//  8
	{ "a0506.rom",	   		0x400000, 0x212f9bec, 3 | BRF_GRA },			//  9

	{ "b0500.rom",	   		0x400000, 0xcb608609, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "b0501.rom",	   		0x400000, 0x1546c2e9, 4 | BRF_GRA },			// 11
	{ "b0502.rom",	   		0x400000, 0xe97b31c3, 4 | BRF_GRA },			// 12
	{ "b0503.u16",	   		0x400000, 0xc5beb6f8, 4 | BRF_GRA },			// 13

	{ "m0500.rom",	   		0x200000, 0x37928cdd, 5 | BRF_SND },			// 14 Samples
	
#if !defined (ROM_VERIFY)
	{ "kd-u6.512",	   		0x010000, 0xe7613dda, 9 | BRF_PRG | BRF_ESS },  // 15 Protection Rom
#else
	{ "ram_dump",	   		0x040000, 0x280cfb4e, 0 | BRF_OPT },
#endif
};

STDROMPICKEXT(olds100afybh, olds100afybh, pgm)
STD_ROM_FN(olds100afybh)

struct BurnDriver BurnDrvOlds100afybh = {
	"olds100afybh", "olds", "pgm", NULL, "2015",
	"Xi Yo Gi Shi Re Zuang Super (Unparalleled In The World, GOTVG 20150203)\0", "Imperfect Protection Emulation", "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_SCRFIGHT, 0,
	NULL, olds100afybhRomInfo, olds100afybhRomName, NULL, NULL, NULL, NULL, pgmInputInfo, olds100DIPInfo,
	oldsInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 西游释厄传超级版 元灵之息 Xi Yo Gi Shi Re Zuang Super (Breath of Yuan Ling 20190129)
static struct BurnRomInfo olds100aylzxRomDesc[] = {
	{ "p0500.v10",	   		0x400000, 0x3541e3b5, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0500.rom",	   		0x400000, 0xd881726c, 2 | BRF_GRA },			//  1 Tile data
	{ "t0501.rom",	   		0x200000, 0xd2106864, 2 | BRF_GRA },			//  2

	{ "a0500.rom",	   		0x400000, 0x80a59197, 3 | BRF_GRA },			//  3 Sprite Color Data
	{ "a0501.rom",	   		0x400000, 0x98c931b1, 3 | BRF_GRA },			//  4
	{ "a0502.rom",	   		0x400000, 0xc3fcdf1d, 3 | BRF_GRA },			//  5
	{ "a0503.rom",	   		0x400000, 0x066dffec, 3 | BRF_GRA },			//  6
	{ "a0504.rom",	   		0x400000, 0x45337583, 3 | BRF_GRA },			//  7
	{ "a0505.rom",	   		0x400000, 0x5b8cf3a5, 3 | BRF_GRA },			//  8
	{ "a0506.rom",	   		0x400000, 0x8a050a47, 3 | BRF_GRA },			//  9

	{ "b0500.rom",	   		0x400000, 0xcde07f74, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "b0501.rom",	   		0x400000, 0x1546c2e9, 4 | BRF_GRA },			// 11
	{ "b0502.rom",	   		0x400000, 0xe97b31c3, 4 | BRF_GRA },			// 12
	{ "b0503.u16",	   		0x400000, 0x659ea5d5, 4 | BRF_GRA },			// 13

	{ "m0500.rom",	   		0x200000, 0x37928cdd, 5 | BRF_SND },			// 14 Samples
	
#if !defined (ROM_VERIFY)
	{ "kd-u6.512",	   		0x010000, 0xe7613dda, 9 | BRF_PRG | BRF_ESS },  // 15 Protection Rom
#else
	{ "ram_dump",	   		0x040000, 0x280cfb4e, 0 | BRF_OPT },
#endif
};

STDROMPICKEXT(olds100aylzx, olds100aylzx, pgm)
STD_ROM_FN(olds100aylzx)

struct BurnDriver BurnDrvOlds100aylzx = {
	"olds100aylzx", "olds", "pgm", NULL, "2019",
	"Oriental Legend Special - Xi Yo Gi Shi Re Zuang Super (Breath of Yuan Ling 20190129)\0", "Imperfect Protection Emulation", "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_SCRFIGHT, 0,
	NULL, olds100aylzxRomInfo, olds100aylzxRomName, NULL, NULL, NULL, NULL, pgmInputInfo, olds100DIPInfo,
	oldsInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DR0ID 西游释厄传超级版 舞之无双版 Xi Yo Gi Shi Re Zuang Super (Unrivalled Version 20181030)
static struct BurnRomInfo olds100aemRomDesc[] = {
	{ "p0500.v10",	   		0x400000, 0xf39435b3, 1 | BRF_PRG | BRF_ESS },  ////  0 68K Code

	{ "t0500.rom",	   		0x400000, 0xd881726c, 2 | BRF_GRA },			//  1 Tile data
	{ "t0501.rom",	   		0x200000, 0xd2106864, 2 | BRF_GRA },			//  2

	{ "a0500.rom",	   		0x400000, 0x80a59197, 3 | BRF_GRA },			//  3 Sprite Color Data
	{ "a0501.rom",	   		0x400000, 0x98c931b1, 3 | BRF_GRA },			//  4
	{ "a0502.rom",	   		0x400000, 0xc3fcdf1d, 3 | BRF_GRA },			//  5
	{ "a0503.rom",	   		0x400000, 0x066dffec, 3 | BRF_GRA },			//  6
	{ "a0504.rom",	   		0x400000, 0x45337583, 3 | BRF_GRA },			//  7
	{ "a0505.rom",	   		0x400000, 0x5b8cf3a5, 3 | BRF_GRA },			//  8
	{ "a0506.rom",	   		0x400000, 0xd76bdbad, 3 | BRF_GRA },			////  9

	{ "b0500.rom",	   		0x400000, 0xcde07f74, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "b0501.rom",	   		0x400000, 0x1546c2e9, 4 | BRF_GRA },			// 11
	{ "b0502.rom",	   		0x400000, 0xe97b31c3, 4 | BRF_GRA },			// 12
	{ "b0503.u16",	   		0x400000, 0xd5c55bd3, 4 | BRF_GRA },			//// 13

	{ "m0500.rom",	   		0x200000, 0x37928cdd, 5 | BRF_SND },			// 14 Samples
	
#if !defined (ROM_VERIFY)
	{ "kd-u6.512",	   		0x010000, 0xe7613dda, 9 | BRF_PRG | BRF_ESS },  // 15 Protection Rom
#else
	{ "ram_dump",	   		0x040000, 0x280cfb4e, 0 | BRF_OPT },
#endif
};

STDROMPICKEXT(olds100aem, olds100aem, pgm)
STD_ROM_FN(olds100aem)

struct BurnDriver BurnDrvOlds100aem = {
	"olds100aem", "olds", "pgm", NULL, "2018",
	"Xi Yo Gi Shi Re Zuang Super (Unrivalled Version 20181030)\0", "Imperfect Protection Emulation", "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_SCRFIGHT, 0,
	NULL, olds100aemRomInfo, olds100aemRomName, NULL, NULL, NULL, NULL, pgmInputInfo, olds100DIPInfo,
	oldsInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 西游释厄传超级版 噩梦求生2015 Xi Yo Gi Shi Re Zuang Super (Survival In Nightmare 2015 20180823)
static struct BurnRomInfo olds100aem2015RomDesc[] = {
	{ "p0500.v10",	   		0x400000, 0xfbf9e737, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0500.rom",	   		0x400000, 0xd881726c, 2 | BRF_GRA },			//  1 Tile data
	{ "t0501.rom",	   		0x200000, 0xd2106864, 2 | BRF_GRA },			//  2

	{ "a0500.rom",	   		0x400000, 0x80a59197, 3 | BRF_GRA },			//  3 Sprite Color Data
	{ "a0501.rom",	   		0x400000, 0x98c931b1, 3 | BRF_GRA },			//  4
	{ "a0502.rom",	   		0x400000, 0xc3fcdf1d, 3 | BRF_GRA },			//  5
	{ "a0503.rom",	   		0x400000, 0x066dffec, 3 | BRF_GRA },			//  6
	{ "a0504.rom",	   		0x400000, 0x45337583, 3 | BRF_GRA },			//  7
	{ "a0505.rom",	   		0x400000, 0x5b8cf3a5, 3 | BRF_GRA },			//  8
	{ "a0506.rom",	   		0x400000, 0xd76bdbad, 3 | BRF_GRA },			//  9

	{ "b0500.rom",	   		0x400000, 0xcde07f74, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "b0501.rom",	   		0x400000, 0x1546c2e9, 4 | BRF_GRA },			// 11
	{ "b0502.rom",	   		0x400000, 0xe97b31c3, 4 | BRF_GRA },			// 12
	{ "b0503.u16",	   		0x400000, 0xd5c55bd3, 4 | BRF_GRA },			// 13

	{ "m0500.rom",	   		0x200000, 0x37928cdd, 5 | BRF_SND },			// 14 Samples
	
#if !defined (ROM_VERIFY)
	{ "kd-u6.512",	   		0x010000, 0xe7613dda, 9 | BRF_PRG | BRF_ESS },  // 15 Protection Rom
#else
	{ "ram_dump",	   		0x040000, 0x280cfb4e, 0 | BRF_OPT },
#endif
};

STDROMPICKEXT(olds100aem2015, olds100aem2015, pgm)
STD_ROM_FN(olds100aem2015)

struct BurnDriver BurnDrvOlds100aem2015 = {
	"olds100aem2015", "olds", "pgm", NULL, "2018",
	"Xi Yo Gi Shi Re Zuang Super (Survival In Nightmare 2015 20180823)\0", "Imperfect Protection Emulation", "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_SCRFIGHT, 0,
	NULL, olds100aem2015RomInfo, olds100aem2015RomName, NULL, NULL, NULL, NULL, pgmInputInfo, olds100DIPInfo,
	oldsInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 西游释厄传超级版 噩梦求生2017 Xi Yo Gi Shi Re Zuang Super (Survival In Nightmare 2017 20180927)
static struct BurnRomInfo olds100aem2016RomDesc[] = {
	{ "p0500.v10",	   		0x400000, 0xde996dd4, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0500.rom",	   		0x400000, 0xd881726c, 2 | BRF_GRA },			//  1 Tile data
	{ "t0501.rom",	   		0x200000, 0xd2106864, 2 | BRF_GRA },			//  2

	{ "a0500.rom",	   		0x400000, 0x80a59197, 3 | BRF_GRA },			//  3 Sprite Color Data
	{ "a0501.rom",	   		0x400000, 0x98c931b1, 3 | BRF_GRA },			//  4
	{ "a0502.rom",	   		0x400000, 0xc3fcdf1d, 3 | BRF_GRA },			//  5
	{ "a0503.rom",	   		0x400000, 0x066dffec, 3 | BRF_GRA },			//  6
	{ "a0504.rom",	   		0x400000, 0x45337583, 3 | BRF_GRA },			//  7
	{ "a0505.rom",	   		0x400000, 0x5b8cf3a5, 3 | BRF_GRA },			//  8
	{ "a0506.rom",	   		0x400000, 0xd76bdbad, 3 | BRF_GRA },			//  9

	{ "b0500.rom",	   		0x400000, 0xcde07f74, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "b0501.rom",	   		0x400000, 0x1546c2e9, 4 | BRF_GRA },			// 11
	{ "b0502.rom",	   		0x400000, 0xe97b31c3, 4 | BRF_GRA },			// 12
	{ "b0503.u16",	   		0x400000, 0xd5c55bd3, 4 | BRF_GRA },			// 13

	{ "m0500.rom",	   		0x200000, 0x37928cdd, 5 | BRF_SND },			// 14 Samples
	
#if !defined (ROM_VERIFY)
	{ "kd-u6.512",	   		0x010000, 0xe7613dda, 9 | BRF_PRG | BRF_ESS },  // 15 Protection Rom
#else
	{ "ram_dump",	   		0x040000, 0x280cfb4e, 0 | BRF_OPT },
#endif
};

STDROMPICKEXT(olds100aem2016, olds100aem2016, pgm)
STD_ROM_FN(olds100aem2016)

struct BurnDriver BurnDrvOlds100aem2016 = {
	"olds100aem2016", "olds", "pgm", NULL, "2018",
	"Xi Yo Gi Shi Re Zuang Super (Survival In Nightmare 2017 20180927)\0", "Imperfect Protection Emulation", "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_SCRFIGHT, 0,
	NULL, olds100aem2016RomInfo, olds100aem2016RomName, NULL, NULL, NULL, NULL, pgmInputInfo, olds100DIPInfo,
	oldsInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 西游释厄传超级版 REMIX加强版 Xi Yo Gi Shi Re Zuang Super (Remix Plus 20180927)
static struct BurnRomInfo olds100asrpRomDesc[] = {
	{ "p0500.v10",	   		0x400000, 0x5162b454, 1 | BRF_PRG | BRF_ESS },  //  0 68K Code

	{ "t0500.rom",	   		0x400000, 0xd881726c, 2 | BRF_GRA },			//  1 Tile data
	{ "t0501.rom",	   		0x200000, 0xd2106864, 2 | BRF_GRA },			//  2

	{ "a0500.rom",	   		0x400000, 0x80a59197, 3 | BRF_GRA },			//  3 Sprite Color Data
	{ "a0501.rom",	   		0x400000, 0x98c931b1, 3 | BRF_GRA },			//  4
	{ "a0502.rom",	   		0x400000, 0xc3fcdf1d, 3 | BRF_GRA },			//  5
	{ "a0503.rom",	   		0x400000, 0x066dffec, 3 | BRF_GRA },			//  6
	{ "a0504.rom",	   		0x400000, 0x45337583, 3 | BRF_GRA },			//  7
	{ "a0505.rom",	   		0x400000, 0x5b8cf3a5, 3 | BRF_GRA },			//  8
	{ "a0506.rom",	   		0x400000, 0xd76bdbad, 3 | BRF_GRA },			//  9

	{ "b0500.rom",	   		0x400000, 0xcde07f74, 4 | BRF_GRA },			// 10 Sprite Masks & Color Indexes
	{ "b0501.rom",	   		0x400000, 0x1546c2e9, 4 | BRF_GRA },			// 11
	{ "b0502.rom",	   		0x400000, 0xe97b31c3, 4 | BRF_GRA },			// 12
	{ "b0503.u16",	   		0x400000, 0xd5c55bd3, 4 | BRF_GRA },			// 13

	{ "m0500.rom",	   		0x200000, 0x37928cdd, 5 | BRF_SND },			// 14 Samples
	
#if !defined (ROM_VERIFY)
	{ "kd-u6.512",	   		0x010000, 0xe7613dda, 9 | BRF_PRG | BRF_ESS },  // 15 Protection Rom
#else
	{ "ram_dump",	   		0x040000, 0x280cfb4e, 0 | BRF_OPT },
#endif
};

STDROMPICKEXT(olds100asrp, olds100asrp, pgm)
STD_ROM_FN(olds100asrp)

struct BurnDriver BurnDrvOlds100asrp = {
	"olds100asrp", "olds", "pgm", NULL, "2018",
	"Xi Yo Gi Shi Re Zuang Super (Remix Plus 20180927)\0", "Imperfect Protection Emulation", "IGS", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_SCRFIGHT, 0,
	NULL, olds100asrpRomInfo, olds100asrpRomName, NULL, NULL, NULL, NULL, pgmInputInfo, olds100DIPInfo,
	oldsInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 西游释厄传 寻仙回梦 Xi Yo Gi Shi Re Zuang (Boss Rush Edition 20180716)
static struct BurnRomInfo orlegndcmmRomDesc[] = {
	{ "p0101.160",			0x200000, 0xb515772b, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t0100.rom",			0x400000, 0x61425e1e, 2 | BRF_GRA },			//  1 Tile data

	{ "a0100.rom",			0x400000, 0x8b3bd88a, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0101.rom",			0x400000, 0x3b9e9644, 3 | BRF_GRA },			//  3	  
	{ "a0102.rom",			0x400000, 0x069e2c38, 3 | BRF_GRA },			//  4
	{ "a0103.rom",			0x400000, 0x4460a3fd, 3 | BRF_GRA },			//  5
	{ "a0104.rom",			0x400000, 0x5f8abb56, 3 | BRF_GRA },			//  6
	{ "a0105.rom",			0x400000, 0xa17a7147, 3 | BRF_GRA },			//  7

	{ "b0100.rom",			0x400000, 0x69d2e48c, 4 | BRF_GRA },			//  8 Sprite Masks & Color Indexes
	{ "b0101.rom",			0x400000, 0x0d587bf3, 4 | BRF_GRA },			//  9
	{ "b0102.rom",			0x400000, 0x43823c1e, 4 | BRF_GRA },			// 10

	{ "m0100.rom",			0x200000, 0xe5c36c83, 5 | BRF_SND },			// 11 Samples
};

STDROMPICKEXT(orlegndcmm, orlegndcmm, pgm)
STD_ROM_FN(orlegndcmm)

struct BurnDriver BurnDrvOrlegndcmm = {
	"orlegndcmm", "orlegend", "pgm", NULL, "2018",
	"Xi Yo Gi Shi Re Zuang (Boss Rush Edition 20180716)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_SCRFIGHT, 0,
	NULL, orlegndcmmRomInfo, orlegndcmmRomName, NULL, NULL, NULL, NULL, pgmInputInfo, orld112cDIPInfo,
	orlegendInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// FBA4DROID 西游释厄传 群魔乱舞 黄帽伏魔版 Xi Yo Gi Shi Re Zuang (Yellow Hat Subduing Devils 20180802)
static struct BurnRomInfo oldsplushmfRomDesc[] = {
	{ "p05301.rom",			0x400000, 0xc2e70cae, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t05301.rom",			0x800000, 0x8257bbb0, 2 | BRF_GRA },			//  1 Tile data

	{ "a05301.rom",			0x800000, 0x57946fd2, 3 | BRF_GRA },	 		//  2 Sprite Color Data
	{ "a05302.rom",			0x800000, 0x65acdbf7, 3 | BRF_GRA },			//  3
	{ "a05303.rom",			0x800000, 0x13475d85, 3 | BRF_GRA },			//  4
	{ "a05304.rom",			0x800000, 0xb92e10a4, 3 | BRF_GRA },			//  5

	{ "b05301.rom",			0x800000, 0x7aedd210, 4 | BRF_GRA },			//  6 Sprite Masks & Color Indexes
	{ "b05302.rom",			0x800000, 0x059d9ad8, 4 | BRF_GRA },			//  7

	{ "m05301.rom",			0x400000, 0x61a1413c, 5 | BRF_SND },			//  8 Samples

	{ "oldsplus_igs027a.bin",	0x004000, 0x00000000, 7 | BRF_PRG | BRF_ESS | BRF_NODUMP },	//  9 Internal ARM7 Rom
};

STDROMPICKEXT(oldsplushmf, oldsplushmf, pgm)
STD_ROM_FN(oldsplushmf)

struct BurnDriver BurnDrvOldsplushmf = {
	"oldsplushmf", "oldsplus", "pgm", NULL, "2018",
	"Xi Yo Gi Shi Re Zuang (Yellow Hat Subduing Devils 20180802)\0", "Incomplete Dump", "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM/* | HARDWARE_IGS_USE_ARM_CPU*/, GBF_SCRFIGHT, 0,
	NULL, oldsplushmfRomInfo, oldsplushmfRomName, NULL, NULL, NULL, NULL, pgmInputInfo, puzzli2DIPInfo,
	oldsplusInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

// killbld

// FBA4DROID 傲剑狂刀 群英版 The Killing Blade (Heroes 20180622)
static struct BurnRomInfo killbldqyRomDesc[] = {
	{ "p0300_v109.u9",		0x200000, 0x98c85d0e, 1 | BRF_PRG | BRF_ESS },	//  0 68K Code

	{ "t0300.u14",			0x400000, 0x0922f7d9, 2 | BRF_GRA },			//  1 Tile data

	{ "a0300.u9",			0x400000, 0x3f9455d3, 3 | BRF_GRA },			//  2 Sprite Color Data
	{ "a0301.u10",			0x400000, 0x92776889, 3 | BRF_GRA },			//  3
	{ "a0303.u11",			0x400000, 0x33f5cc69, 3 | BRF_GRA },			//  4
	{ "a0306.u12",			0x400000, 0xcc018a8e, 3 | BRF_GRA },			//  5
	{ "a0307.u2",			0x400000, 0xbc772e39, 3 | BRF_GRA },			//  6

	{ "b0300.u13",			0x400000, 0x7f876981, 4 | BRF_GRA },			//  7 Sprite Masks & Color Indexes
	{ "b0302.u14",			0x400000, 0xeea9c502, 4 | BRF_GRA },			//  8
	{ "b0303.u15",			0x200000, 0x77a9652e, 4 | BRF_GRA },			//  9

	{ "m0300.u1",			0x400000, 0x93159695, 5 | BRF_SND },			// 10 Samples

	{ "kb_u2.rom",			0x010000, 0xde3eae63, 9 | BRF_PRG | BRF_ESS },	// 11 Protection Data
};

STDROMPICKEXT(killbldqy, killbldqy, pgm)
STD_ROM_FN(killbldqy)

struct BurnDriver BurnDrvKillbldqy = {
	"killbldqy", "killbld", "pgm", NULL, "2018",
	"The Killing Blade (Heroes 20180622)\0", NULL, "hack", "PolyGameMaster",
	NULL, NULL, NULL, NULL,
	BDF_GAME_WORKING | BDF_CLONE, 4, HARDWARE_IGS_PGM, GBF_VSFIGHT, 0,
	NULL, killbldqyRomInfo, killbldqyRomName, NULL, NULL, NULL, NULL, pgmInputInfo, killbldDIPInfo,
	killbldInit, pgmExit, pgmFrame, pgmDraw, pgmScan, &nPgmPalRecalc, 0x900,
	448, 224, 4, 3
};

