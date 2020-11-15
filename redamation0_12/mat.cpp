//-----------------------------------------------------------
#include "stdafx.h"
#include "mat.h"

unsigned short _sqrt_table[256] =
{
   0x2D4,   0x103F,  0x16CD,  0x1BDB,  0x201F,  0x23E3,  0x274B,  0x2A6D, 
   0x2D57,  0x3015,  0x32AC,  0x3524,  0x377F,  0x39C2,  0x3BEE,  0x3E08, 
   0x400F,  0x4207,  0x43F0,  0x45CC,  0x479C,  0x4960,  0x4B19,  0x4CC9, 
   0x4E6F,  0x500C,  0x51A2,  0x532F,  0x54B6,  0x5635,  0x57AE,  0x5921, 
   0x5A8D,  0x5BF4,  0x5D56,  0x5EB3,  0x600A,  0x615D,  0x62AB,  0x63F5, 
   0x653B,  0x667D,  0x67BA,  0x68F5,  0x6A2B,  0x6B5E,  0x6C8D,  0x6DBA, 
   0x6EE3,  0x7009,  0x712C,  0x724C,  0x7369,  0x7484,  0x759C,  0x76B1, 
   0x77C4,  0x78D4,  0x79E2,  0x7AEE,  0x7BF7,  0x7CFE,  0x7E04,  0x7F07, 
   0x8007,  0x8106,  0x8203,  0x82FF,  0x83F8,  0x84EF,  0x85E5,  0x86D9, 
   0x87CB,  0x88BB,  0x89AA,  0x8A97,  0x8B83,  0x8C6D,  0x8D56,  0x8E3D, 
   0x8F22,  0x9007,  0x90E9,  0x91CB,  0x92AB,  0x938A,  0x9467,  0x9543, 
   0x961E,  0x96F8,  0x97D0,  0x98A8,  0x997E,  0x9A53,  0x9B26,  0x9BF9, 
   0x9CCA,  0x9D9B,  0x9E6A,  0x9F39,  0xA006,  0xA0D2,  0xA19D,  0xA268, 
   0xA331,  0xA3F9,  0xA4C1,  0xA587,  0xA64D,  0xA711,  0xA7D5,  0xA898, 
   0xA95A,  0xAA1B,  0xAADB,  0xAB9A,  0xAC59,  0xAD16,  0xADD3,  0xAE8F, 
   0xAF4B,  0xB005,  0xB0BF,  0xB178,  0xB230,  0xB2E8,  0xB39F,  0xB455, 
   0xB50A,  0xB5BF,  0xB673,  0xB726,  0xB7D9,  0xB88A,  0xB93C,  0xB9EC, 
   0xBA9C,  0xBB4B,  0xBBFA,  0xBCA8,  0xBD55,  0xBE02,  0xBEAE,  0xBF5A, 
   0xC005,  0xC0AF,  0xC159,  0xC202,  0xC2AB,  0xC353,  0xC3FA,  0xC4A1, 
   0xC548,  0xC5ED,  0xC693,  0xC737,  0xC7DC,  0xC87F,  0xC923,  0xC9C5, 
   0xCA67,  0xCB09,  0xCBAA,  0xCC4B,  0xCCEB,  0xCD8B,  0xCE2A,  0xCEC8, 
   0xCF67,  0xD004,  0xD0A2,  0xD13F,  0xD1DB,  0xD277,  0xD312,  0xD3AD, 
   0xD448,  0xD4E2,  0xD57C,  0xD615,  0xD6AE,  0xD746,  0xD7DE,  0xD876, 
   0xD90D,  0xD9A4,  0xDA3A,  0xDAD0,  0xDB66,  0xDBFB,  0xDC90,  0xDD24, 
   0xDDB8,  0xDE4C,  0xDEDF,  0xDF72,  0xE004,  0xE096,  0xE128,  0xE1B9, 
   0xE24A,  0xE2DB,  0xE36B,  0xE3FB,  0xE48B,  0xE51A,  0xE5A9,  0xE637, 
   0xE6C5,  0xE753,  0xE7E1,  0xE86E,  0xE8FB,  0xE987,  0xEA13,  0xEA9F, 
   0xEB2B,  0xEBB6,  0xEC41,  0xECCB,  0xED55,  0xEDDF,  0xEE69,  0xEEF2, 
   0xEF7B,  0xF004,  0xF08C,  0xF114,  0xF19C,  0xF223,  0xF2AB,  0xF332, 
   0xF3B8,  0xF43E,  0xF4C4,  0xF54A,  0xF5D0,  0xF655,  0xF6DA,  0xF75E, 
   0xF7E3,  0xF867,  0xF8EA,  0xF96E,  0xF9F1,  0xFA74,  0xFAF7,  0xFB79, 
   0xFBFB,  0xFC7D,  0xFCFF,  0xFD80,  0xFE02,  0xFE82,  0xFF03,  0xFF83
};

short _sincos[]={
0x4000,0x3FFF,0x3FFF,0x3FFF,0x3FFF,0x3FFF,0x3FFF,0x3FFE,0x3FFE,0x3FFD,
0x3FFD,0x3FFC,0x3FFC,0x3FFB,0x3FFB,0x3FFA,0x3FF9,0x3FF8,0x3FF7,0x3FF6,
0x3FF6,0x3FF4,0x3FF3,0x3FF2,0x3FF1,0x3FF0,0x3FEF,0x3FED,0x3FEC,0x3FEB,
0x3FE9,0x3FE8,0x3FE6,0x3FE4,0x3FE3,0x3FE1,0x3FDF,0x3FDD,0x3FDB,0x3FDA,
0x3FD8,0x3FD6,0x3FD4,0x3FD1,0x3FCF,0x3FCD,0x3FCB,0x3FC8,0x3FC6,0x3FC4,
0x3FC1,0x3FBF,0x3FBC,0x3FB9,0x3FB7,0x3FB4,0x3FB1,0x3FAE,0x3FAC,0x3FA9,
0x3FA6,0x3FA3,0x3FA0,0x3F9D,0x3F99,0x3F96,0x3F93,0x3F90,0x3F8C,0x3F89,
0x3F85,0x3F82,0x3F7E,0x3F7B,0x3F77,0x3F73,0x3F70,0x3F6C,0x3F68,0x3F64,
0x3F60,0x3F5C,0x3F58,0x3F54,0x3F50,0x3F4C,0x3F47,0x3F43,0x3F3F,0x3F3A,
0x3F36,0x3F31,0x3F2D,0x3F28,0x3F23,0x3F1F,0x3F1A,0x3F15,0x3F10,0x3F0C,
0x3F07,0x3F02,0x3EFD,0x3EF7,0x3EF2,0x3EED,0x3EE8,0x3EE3,0x3EDD,0x3ED8,
0x3ED2,0x3ECD,0x3EC7,0x3EC2,0x3EBC,0x3EB7,0x3EB1,0x3EAB,0x3EA5,0x3E9F,
0x3E99,0x3E93,0x3E8D,0x3E87,0x3E81,0x3E7B,0x3E75,0x3E6F,0x3E68,0x3E62,
0x3E5C,0x3E55,0x3E4F,0x3E48,0x3E41,0x3E3B,0x3E34,0x3E2D,0x3E27,0x3E20,
0x3E19,0x3E12,0x3E0B,0x3E04,0x3DFD,0x3DF6,0x3DEE,0x3DE7,0x3DE0,0x3DD9,
0x3DD1,0x3DCA,0x3DC2,0x3DBB,0x3DB3,0x3DAC,0x3DA4,0x3D9C,0x3D94,0x3D8D,
0x3D85,0x3D7D,0x3D75,0x3D6D,0x3D65,0x3D5D,0x3D55,0x3D4C,0x3D44,0x3D3C,
0x3D34,0x3D2B,0x3D23,0x3D1A,0x3D12,0x3D09,0x3D01,0x3CF8,0x3CEF,0x3CE6,
0x3CDE,0x3CD5,0x3CCC,0x3CC3,0x3CBA,0x3CB1,0x3CA8,0x3C9F,0x3C95,0x3C8C,
0x3C83,0x3C7A,0x3C70,0x3C67,0x3C5D,0x3C54,0x3C4A,0x3C41,0x3C37,0x3C2D,
0x3C23,0x3C1A,0x3C10,0x3C06,0x3BFC,0x3BF2,0x3BE8,0x3BDE,0x3BD4,0x3BCA,
0x3BBF,0x3BB5,0x3BAB,0x3BA0,0x3B96,0x3B8B,0x3B81,0x3B76,0x3B6C,0x3B61,
0x3B56,0x3B4C,0x3B41,0x3B36,0x3B2B,0x3B20,0x3B15,0x3B0A,0x3AFF,0x3AF4,
0x3AE9,0x3ADE,0x3AD3,0x3AC7,0x3ABC,0x3AB1,0x3AA5,0x3A9A,0x3A8E,0x3A83,
0x3A77,0x3A6B,0x3A60,0x3A54,0x3A48,0x3A3C,0x3A30,0x3A24,0x3A19,0x3A0D,
0x3A00,0x39F4,0x39E8,0x39DC,0x39D0,0x39C3,0x39B7,0x39AB,0x399E,0x3992,
0x3985,0x3979,0x396C,0x3960,0x3953,0x3946,0x3939,0x392C,0x3920,0x3913,
0x3906,0x38F9,0x38EC,0x38DF,0x38D1,0x38C4,0x38B7,0x38AA,0x389C,0x388F,
0x3882,0x3874,0x3867,0x3859,0x384C,0x383E,0x3830,0x3823,0x3815,0x3807,
0x37F9,0x37EB,0x37DD,0x37CF,0x37C1,0x37B3,0x37A5,0x3797,0x3789,0x377B,
0x376C,0x375E,0x3750,0x3741,0x3733,0x3724,0x3716,0x3707,0x36F9,0x36EA,
0x36DB,0x36CD,0x36BE,0x36AF,0x36A0,0x3691,0x3682,0x3673,0x3664,0x3655,
0x3646,0x3637,0x3628,0x3618,0x3609,0x35FA,0x35EA,0x35DB,0x35CB,0x35BC,
0x35AC,0x359D,0x358D,0x357D,0x356E,0x355E,0x354E,0x353E,0x352E,0x351E,
0x350E,0x34FE,0x34EE,0x34DE,0x34CE,0x34BE,0x34AE,0x349E,0x348D,0x347D,
0x346C,0x345C,0x344C,0x343B,0x342B,0x341A,0x3409,0x33F9,0x33E8,0x33D7,
0x33C6,0x33B6,0x33A5,0x3394,0x3383,0x3372,0x3361,0x3350,0x333F,0x332E,
0x331C,0x330B,0x32FA,0x32E9,0x32D7,0x32C6,0x32B4,0x32A3,0x3291,0x3280,
0x326E,0x325D,0x324B,0x3239,0x3228,0x3216,0x3204,0x31F2,0x31E0,0x31CE,
0x31BC,0x31AA,0x3198,0x3186,0x3174,0x3162,0x3150,0x313D,0x312B,0x3119,
0x3106,0x30F4,0x30E2,0x30CF,0x30BD,0x30AA,0x3097,0x3085,0x3072,0x305F,
0x304D,0x303A,0x3027,0x3014,0x3001,0x2FEE,0x2FDB,0x2FC8,0x2FB5,0x2FA2,
0x2F8F,0x2F7C,0x2F69,0x2F56,0x2F42,0x2F2F,0x2F1C,0x2F08,0x2EF5,0x2EE1,
0x2ECE,0x2EBA,0x2EA7,0x2E93,0x2E80,0x2E6C,0x2E58,0x2E45,0x2E31,0x2E1D,
0x2E09,0x2DF5,0x2DE1,0x2DCD,0x2DB9,0x2DA5,0x2D91,0x2D7D,0x2D69,0x2D55,
0x2D41,0x2D2D,0x2D18,0x2D04,0x2CF0,0x2CDB,0x2CC7,0x2CB2,0x2C9E,0x2C89,
0x2C75,0x2C60,0x2C4C,0x2C37,0x2C22,0x2C0E,0x2BF9,0x2BE4,0x2BCF,0x2BBA,
0x2BA5,0x2B90,0x2B7B,0x2B66,0x2B51,0x2B3C,0x2B27,0x2B12,0x2AFD,0x2AE8,
0x2AD3,0x2ABD,0x2AA8,0x2A93,0x2A7D,0x2A68,0x2A52,0x2A3D,0x2A28,0x2A12,
0x29FC,0x29E7,0x29D1,0x29BC,0x29A6,0x2990,0x297A,0x2965,0x294F,0x2939,
0x2923,0x290D,0x28F7,0x28E1,0x28CB,0x28B5,0x289F,0x2889,0x2873,0x285D,
0x2846,0x2830,0x281A,0x2804,0x27ED,0x27D7,0x27C0,0x27AA,0x2794,0x277D,
0x2767,0x2750,0x2739,0x2723,0x270C,0x26F5,0x26DF,0x26C8,0x26B1,0x269B,
0x2684,0x266D,0x2656,0x263F,0x2628,0x2611,0x25FA,0x25E3,0x25CC,0x25B5,
0x259E,0x2587,0x2570,0x2558,0x2541,0x252A,0x2513,0x24FB,0x24E4,0x24CC,
0x24B5,0x249E,0x2486,0x246F,0x2457,0x2440,0x2428,0x2410,0x23F9,0x23E1,
0x23C9,0x23B2,0x239A,0x2382,0x236A,0x2353,0x233B,0x2323,0x230B,0x22F3,
0x22DB,0x22C3,0x22AB,0x2293,0x227B,0x2263,0x224B,0x2232,0x221A,0x2202,
0x21EA,0x21D2,0x21B9,0x21A1,0x2189,0x2170,0x2158,0x213F,0x2127,0x210E,
0x20F6,0x20DD,0x20C5,0x20AC,0x2094,0x207B,0x2062,0x204A,0x2031,0x2018,
0x2000,0x1FE7,0x1FCE,0x1FB5,0x1F9C,0x1F83,0x1F6B,0x1F52,0x1F39,0x1F20,
0x1F07,0x1EEE,0x1ED5,0x1EBC,0x1EA2,0x1E89,0x1E70,0x1E57,0x1E3E,0x1E25,
0x1E0B,0x1DF2,0x1DD9,0x1DC0,0x1DA6,0x1D8D,0x1D74,0x1D5A,0x1D41,0x1D27,
0x1D0E,0x1CF4,0x1CDB,0x1CC1,0x1CA8,0x1C8E,0x1C75,0x1C5B,0x1C41,0x1C28,
0x1C0E,0x1BF4,0x1BDA,0x1BC1,0x1BA7,0x1B8D,0x1B73,0x1B59,0x1B40,0x1B26,
0x1B0C,0x1AF2,0x1AD8,0x1ABE,0x1AA4,0x1A8A,0x1A70,0x1A56,0x1A3C,0x1A22,
0x1A08,0x19ED,0x19D3,0x19B9,0x199F,0x1985,0x196A,0x1950,0x1936,0x191C,
0x1901,0x18E7,0x18CD,0x18B2,0x1898,0x187E,0x1863,0x1849,0x182E,0x1814,
0x17F9,0x17DF,0x17C4,0x17AA,0x178F,0x1774,0x175A,0x173F,0x1724,0x170A,
0x16EF,0x16D4,0x16BA,0x169F,0x1684,0x1669,0x164F,0x1634,0x1619,0x15FE,
0x15E3,0x15C8,0x15AE,0x1593,0x1578,0x155D,0x1542,0x1527,0x150C,0x14F1,
0x14D6,0x14BB,0x14A0,0x1485,0x1469,0x144E,0x1433,0x1418,0x13FD,0x13E2,
0x13C7,0x13AB,0x1390,0x1375,0x135A,0x133E,0x1323,0x1308,0x12ED,0x12D1,
0x12B6,0x129B,0x127F,0x1264,0x1248,0x122D,0x1212,0x11F6,0x11DB,0x11BF,
0x11A4,0x1188,0x116D,0x1151,0x1136,0x111A,0x10FF,0x10E3,0x10C7,0x10AC,
0x1090,0x1075,0x1059,0x103D,0x1022,0x1006,0xFEA,0xFCE,0xFB3,0xF97,
0xF7B,0xF60,0xF44,0xF28,0xF0C,0xEF0,0xED5,0xEB9,0xE9D,0xE81,
0xE65,0xE49,0xE2E,0xE12,0xDF6,0xDDA,0xDBE,0xDA2,0xD86,0xD6A,
0xD4E,0xD32,0xD16,0xCFA,0xCDE,0xCC2,0xCA6,0xC8A,0xC6E,0xC52,
0xC36,0xC1A,0xBFE,0xBE2,0xBC6,0xBA9,0xB8D,0xB71,0xB55,0xB39,
0xB1D,0xB01,0xAE4,0xAC8,0xAAC,0xA90,0xA74,0xA57,0xA3B,0xA1F,
0xA03,0x9E6,0x9CA,0x9AE,0x992,0x975,0x959,0x93D,0x921,0x904,
0x8E8,0x8CC,0x8AF,0x893,0x877,0x85A,0x83E,0x822,0x805,0x7E9,
0x7CC,0x7B0,0x794,0x777,0x75B,0x73E,0x722,0x706,0x6E9,0x6CD,
0x6B0,0x694,0x677,0x65B,0x63F,0x622,0x606,0x5E9,0x5CD,0x5B0,
0x594,0x577,0x55B,0x53E,0x522,0x505,0x4E9,0x4CC,0x4B0,0x493,
0x477,0x45A,0x43E,0x421,0x404,0x3E8,0x3CB,0x3AF,0x392,0x376,
0x359,0x33D,0x320,0x304,0x2E7,0x2CA,0x2AE,0x291,0x275,0x258,
0x23C,0x21F,0x202,0x1E6,0x1C9,0x1AD,0x190,0x173,0x157,0x13A,
0x11E,0x101,0xE4,0xC8,0xAB,0x8F,0x72,0x56,0x39,0x1C,0x0
};

/* The use of some short working variables allows this code to run   */
/* faster on 16-bit computers, but is not essential.  It should not  */
/* affect operation on 32-bit computers.  The short working variables*/
/* to not restrict the range of valid input values, as these were    */
/* constrained in any case, due to algorithm restrictions.           */
int lines_intersect(long x1,long y1,long x2,long y2,long x3,long y3,long x4,long y4,long *x,long *y) 
{
long Ax,Bx,Cx,Ay,By,Cy,d,e,f,num,offset;
short x1lo,x1hi,y1lo,y1hi;
Ax = x2-x1;
Bx = x3-x4;
if(Ax<0) { x1lo=(short)x2; x1hi=(short)x1; } else { x1hi=(short)x2; x1lo=(short)x1; }
if(Bx>0) { 
  if(x1hi < (short)x4 || (short)x3 < x1lo) return DONT_INTERSECT;
  } else {
  if(x1hi < (short)x3 || (short)x4 < x1lo) return DONT_INTERSECT;
  }
Ay = y2-y1;
By = y3-y4;
if(Ay<0) { y1lo=(short)y2; y1hi=(short)y1; } else { y1hi=(short)y2; y1lo=(short)y1; }
if(By>0) {
  if(y1hi < (short)y4 || (short)y3 < y1lo) return DONT_INTERSECT;
  } else {
  if(y1hi < (short)y3 || (short)y4 < y1lo) return DONT_INTERSECT;
  }
Cx = x1-x3;Cy = y1-y3;
d = By*Cx - Bx*Cy;f = Ay*Bx - Ax*By;
if(f>0) {						/* alpha tests*/
  if(d<0 || d>f) return DONT_INTERSECT;
  } else {
  if(d>0 || d<f) return DONT_INTERSECT;
  }
e = Ax*Cy - Ay*Cx;					/* beta numerator*/
if(f>0) {						/* beta tests*/
  if(e<0 || e>f) return DONT_INTERSECT;
  } else {
  if(e>0 || e<f) return DONT_INTERSECT;
  }
/*compute intersection coordinates*/
if(f==0) return COLLINEAR;
num = d*Ax;						/* numerator */
offset = SAME_SIGNS(num,f) ? f/2 : -f/2;		/* round direction*/
*x = x1 + (num+offset) / f;				/* intersection x */
num = d*Ay;
offset = SAME_SIGNS(num,f) ? f/2 : -f/2;
*y = y1 + (num+offset) / f;				/* intersection y */
return DO_INTERSECT;
}
