#ifndef __VARIABLES_H
#define __VARIABLES_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
#include <stdint.h>

   typedef union {
   uint8_t Bytes[2];
   int16_t Word;
   }
   _TwoBytesS;
   typedef union {
   uint8_t Bytes[2];
   int16_t Word;
   }
   _TwoBytesU;
   
   typedef union {
   uint8_t Bytes[4];
   uint16_t Words[2];
   int32_t DWord;
   }
   _FourBytesS;
   
   typedef union {
   uint8_t Bytes[4];
   uint16_t Words[2];
   uint32_t DWord;
   }
   _FourBytesU;
   
typedef struct 
{
  int16_t X;
  int16_t Y;
}Point, * pPoint;  

 typedef struct { 
  Point LeftTop;           // the left top corner coords of zone
  Point RightBottom;       //the right bottom one
  uint8_t PagesActivities; // on which pages this zone is active like 0b00010101 - means, that it is active on 0-th AND 2-nd AND 4-th pages
  uint8_t kbdCode1;         // the code 1
  uint8_t kbdCode2;         // an addition code for a key
} ZoneKBD;   

// define zones for Touh Screen pressing detection
typedef struct { 
  Point LeftTop;   // the left top corner coords of zone
  Point RightBottom; //the right bottom one
  uint8_t PagesActivities; // on which pages this zone is active like 0b00010101 - means, that it is active on 0-th AND 2-nd AND 4-th pages
} Zone; 
  
// binaries emulation

#define ob00000000	0
#define ob00000001	1
#define ob00000010	2
#define ob00000011	3
#define ob00000100	4
#define ob00000101	5
#define ob00000110	6
#define ob00000111	7
#define ob00001000	8
#define ob00001001	9
#define ob00001010	10
#define ob00001011	11
#define ob00001100	12
#define ob00001101	13
#define ob00001110	14
#define ob00001111	15
#define ob00010000	16
#define ob00010001	17
#define ob00010010	18
#define ob00010011	19
#define ob00010100	20
#define ob00010101	21
#define ob00010110	22
#define ob00010111	23
#define ob00011000	24
#define ob00011001	25
#define ob00011010	26
#define ob00011011	27
#define ob00011100	28
#define ob00011101	29
#define ob00011110	30
#define ob00011111	31
#define ob00100000	32
#define ob00100001	33
#define ob00100010	34
#define ob00100011	35
#define ob00100100	36
#define ob00100101	37
#define ob00100110	38
#define ob00100111	39
#define ob00101000	40
#define ob00101001	41
#define ob00101010	42
#define ob00101011	43
#define ob00101100	44
#define ob00101101	45
#define ob00101110	46
#define ob00101111	47
#define ob00110000	48
#define ob00110001	49
#define ob00110010	50
#define ob00110011	51
#define ob00110100	52
#define ob00110101	53
#define ob00110110	54
#define ob00110111	55
#define ob00111000	56
#define ob00111001	57
#define ob00111010	58
#define ob00111011	59
#define ob00111100	60
#define ob00111101	61
#define ob00111110	62
#define ob00111111	63
#define ob01000000	64
#define ob01000001	65
#define ob01000010	66
#define ob01000011	67
#define ob01000100	68
#define ob01000101	69
#define ob01000110	70
#define ob01000111	71
#define ob01001000	72
#define ob01001001	73
#define ob01001010	74
#define ob01001011	75
#define ob01001100	76
#define ob01001101	77
#define ob01001110	78
#define ob01001111	79
#define ob01010000	80
#define ob01010001	81
#define ob01010010	82
#define ob01010011	83
#define ob01010100	84
#define ob01010101	85
#define ob01010110	86
#define ob01010111	87
#define ob01011000	88
#define ob01011001	89
#define ob01011010	90
#define ob01011011	91
#define ob01011100	92
#define ob01011101	93
#define ob01011110	94
#define ob01011111	95
#define ob01100000	96
#define ob01100001	97
#define ob01100010	98
#define ob01100011	99
#define ob01100100	100
#define ob01100101	101
#define ob01100110	102
#define ob01100111	103
#define ob01101000	104
#define ob01101001	105
#define ob01101010	106
#define ob01101011	107
#define ob01101100	108
#define ob01101101	109
#define ob01101110	110
#define ob01101111	111
#define ob01110000	112
#define ob01110001	113
#define ob01110010	114
#define ob01110011	115
#define ob01110100	116
#define ob01110101	117
#define ob01110110	118
#define ob01110111	119
#define ob01111000	120
#define ob01111001	121
#define ob01111010	122
#define ob01111011	123
#define ob01111100	124
#define ob01111101	125
#define ob01111110	126
#define ob01111111	127
#define ob10000000	128
#define ob10000001	129
#define ob10000010	130
#define ob10000011	131
#define ob10000100	132
#define ob10000101	133
#define ob10000110	134
#define ob10000111	135
#define ob10001000	136
#define ob10001001	137
#define ob10001010	138
#define ob10001011	139
#define ob10001100	140
#define ob10001101	141
#define ob10001110	142
#define ob10001111	143
#define ob10010000	144
#define ob10010001	145
#define ob10010010	146
#define ob10010011	147
#define ob10010100	148
#define ob10010101	149
#define ob10010110	150
#define ob10010111	151
#define ob10011000	152
#define ob10011001	153
#define ob10011010	154
#define ob10011011	155
#define ob10011100	156
#define ob10011101	157
#define ob10011110	158
#define ob10011111	159
#define ob10100000	160
#define ob10100001	161
#define ob10100010	162
#define ob10100011	163
#define ob10100100	164
#define ob10100101	165
#define ob10100110	166
#define ob10100111	167
#define ob10101000	168
#define ob10101001	169
#define ob10101010	170
#define ob10101011	171
#define ob10101100	172
#define ob10101101	173
#define ob10101110	174
#define ob10101111	175
#define ob10110000	176
#define ob10110001	177
#define ob10110010	178
#define ob10110011	179
#define ob10110100	180
#define ob10110101	181
#define ob10110110	182
#define ob10110111	183
#define ob10111000	184
#define ob10111001	185
#define ob10111010	186
#define ob10111011	187
#define ob10111100	188
#define ob10111101	189
#define ob10111110	190
#define ob10111111	191
#define ob11000000	192
#define ob11000001	193
#define ob11000010	194
#define ob11000011	195
#define ob11000100	196
#define ob11000101	197
#define ob11000110	198
#define ob11000111	199
#define ob11001000	200
#define ob11001001	201
#define ob11001010	202
#define ob11001011	203
#define ob11001100	204
#define ob11001101	205
#define ob11001110	206
#define ob11001111	207
#define ob11010000	208
#define ob11010001	209
#define ob11010010	210
#define ob11010011	211
#define ob11010100	212
#define ob11010101	213
#define ob11010110	214
#define ob11010111	215
#define ob11011000	216
#define ob11011001	217
#define ob11011010	218
#define ob11011011	219
#define ob11011100	220
#define ob11011101	221
#define ob11011110	222
#define ob11011111	223
#define ob11100000	224
#define ob11100001	225
#define ob11100010	226
#define ob11100011	227
#define ob11100100	228
#define ob11100101	229
#define ob11100110	230
#define ob11100111	231
#define ob11101000	232
#define ob11101001	233
#define ob11101010	234
#define ob11101011	235
#define ob11101100	236
#define ob11101101	237
#define ob11101110	238
#define ob11101111	239
#define ob11110000	240
#define ob11110001	241
#define ob11110010	242
#define ob11110011	243
#define ob11110100	244
#define ob11110101	245
#define ob11110110	246
#define ob11110111	247
#define ob11111000	248
#define ob11111001	249
#define ob11111010	250
#define ob11111011	251
#define ob11111100	252
#define ob11111101	253
#define ob11111110	254
#define ob11111111	255
//---------------------------
#ifdef __cplusplus
}
#endif

#endif /* __VARIABLES_H */   
