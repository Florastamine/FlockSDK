//
// Copyright (c) 2008-2017 Flock SDK developers & contributors. 
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "../Precompiled.h"

#include "../Core/StringUtils.h"

#include <cstdio>
#include <string>
#include <vector>

namespace TinyAES {
/*
This is an implementation of the AES algorithm, specifically ECB and CBC mode.
Block size can be chosen in aes.h - available choices are AES128, AES192, AES256.

The implementation is verified against the test vectors in:
  National Institute of Standards and Technology Special Publication 800-38A 2001 ED

ECB-AES128
----------

  plain-text:
    6bc1bee22e409f96e93d7e117393172a
    ae2d8a571e03ac9c9eb76fac45af8e51
    30c81c46a35ce411e5fbc1191a0a52ef
    f69f2445df4f9b17ad2b417be66c3710

  key:
    2b7e151628aed2a6abf7158809cf4f3c

  resulting cipher
    3ad77bb40d7a3660a89ecaf32466ef97 
    f5d3d58503b9699de785895a96fdbaaf 
    43b1cd7f598ece23881b00e3ed030688 
    7b0c785e27e8ad3f8223207104725dd4 


NOTE:   String length must be evenly divisible by 16byte (str_len % 16 == 0)
        You should pad the end of the string with zeros if this is not the case.
        For AES192/256 the block size is proportionally larger.
*/

#define AES256 1
#define ECB    1

#include <cstdint>
#include <cstring> // CBC mode, for memset

/*****************************************************************************/
/* Defines:                                                                  */
/*****************************************************************************/
// The number of columns comprising a state in AES. This is a constant in AES. Value=4
#define Nb 4
#define BLOCKLEN 16 //Block length in bytes AES is 128b block only

#if defined(AES256) && (AES256 == 1)
    #define Nk 8
    #define KEYLEN 32
    #define Nr 14
    #define keyExpSize 240
#elif defined(AES192) && (AES192 == 1)
    #define Nk 6
    #define KEYLEN 24
    #define Nr 12
    #define keyExpSize 208
#else
    #define Nk 4        // The number of 32 bit words in a key.
    #define KEYLEN 16   // Key length in bytes
    #define Nr 10       // The number of rounds in AES Cipher.
    #define keyExpSize 176
#endif

// jcallan@github points out that declaring Multiply as a function 
// reduces code size considerably with the Keil ARM compiler.
// See this link for more information: https://github.com/kokke/tiny-AES128-C/pull/3
#ifndef MULTIPLY_AS_A_FUNCTION
  #define MULTIPLY_AS_A_FUNCTION 0
#endif

/*****************************************************************************/
/* Private variables:                                                        */
/*****************************************************************************/
// state - array holding the intermediate results during decryption.
typedef uint8_t state_t[4][4];
static state_t* state;

// The array that stores the round keys.
static uint8_t RoundKey[keyExpSize];

// The Key input to the AES Program
static const uint8_t* Key;

#if defined(CBC) && CBC
  // Initial Vector used only for CBC mode
  static uint8_t* Iv;
#endif

// The lookup-tables are marked const so they can be placed in read-only storage instead of RAM
// The numbers below can be computed dynamically trading ROM for RAM - 
// This can be useful in (embedded) bootloader applications, where ROM is often limited.
static constexpr const uint8_t sbox[256] = {
  //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

static constexpr const uint8_t rsbox[256] = {
  0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };

// The round constant word array, Rcon[i], contains the values given by 
// x to th e power (i-1) being powers of x (x is denoted as {02}) in the field GF(2^8)
static constexpr const uint8_t Rcon[11] = {
  0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };

/*
 * Jordan Goulder points out in PR #12 (https://github.com/kokke/tiny-AES128-C/pull/12),
 * that you can remove most of the elements in the Rcon array, because they are unused.
 *
 * From Wikipedia's article on the Rijndael key schedule @ https://en.wikipedia.org/wiki/Rijndael_key_schedule#Rcon
 * 
 * "Only the first some of these constants are actually used – up to rcon[10] for AES-128 (as 11 round keys are needed), 
 *  up to rcon[8] for AES-192, up to rcon[7] for AES-256. rcon[0] is not used in AES algorithm."
 *
 * ... which is why the full array below has been 'disabled' below.
 */
#if 0
static const uint8_t Rcon[256] = {
  0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
  0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
  0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
  0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
  0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
  0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
  0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
  0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
  0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
  0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
  0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
  0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
  0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
  0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
  0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
  0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d };
#endif

/*****************************************************************************/
/* Private functions:                                                        */
/*****************************************************************************/
static uint8_t getSBoxValue(uint8_t num)
{
  return sbox[num];
}

static uint8_t getSBoxInvert(uint8_t num)
{
  return rsbox[num];
}

// This function produces Nb(Nr+1) round keys. The round keys are used in each round to decrypt the states. 
static void KeyExpansion(void)
{
  uint32_t i, k;
  uint8_t tempa[4]; // Used for the column/row operations
  
  // The first round key is the key itself.
  for (i = 0; i < Nk; ++i)
  {
    RoundKey[(i * 4) + 0] = Key[(i * 4) + 0];
    RoundKey[(i * 4) + 1] = Key[(i * 4) + 1];
    RoundKey[(i * 4) + 2] = Key[(i * 4) + 2];
    RoundKey[(i * 4) + 3] = Key[(i * 4) + 3];
  }

  // All other round keys are found from the previous round keys.
  //i == Nk
  for (; i < Nb * (Nr + 1); ++i)
  {
    {
      tempa[0]=RoundKey[(i-1) * 4 + 0];
      tempa[1]=RoundKey[(i-1) * 4 + 1];
      tempa[2]=RoundKey[(i-1) * 4 + 2];
      tempa[3]=RoundKey[(i-1) * 4 + 3];
    }

    if (i % Nk == 0)
    {
      // This function shifts the 4 bytes in a word to the left once.
      // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]

      // Function RotWord()
      {
        k = tempa[0];
        tempa[0] = tempa[1];
        tempa[1] = tempa[2];
        tempa[2] = tempa[3];
        tempa[3] = k;
      }

      // SubWord() is a function that takes a four-byte input word and 
      // applies the S-box to each of the four bytes to produce an output word.

      // Function Subword()
      {
        tempa[0] = getSBoxValue(tempa[0]);
        tempa[1] = getSBoxValue(tempa[1]);
        tempa[2] = getSBoxValue(tempa[2]);
        tempa[3] = getSBoxValue(tempa[3]);
      }

      tempa[0] =  tempa[0] ^ Rcon[i/Nk];
    }
#if defined(AES256) && (AES256 == 1)
    if (i % Nk == 4)
    {
      // Function Subword()
      {
        tempa[0] = getSBoxValue(tempa[0]);
        tempa[1] = getSBoxValue(tempa[1]);
        tempa[2] = getSBoxValue(tempa[2]);
        tempa[3] = getSBoxValue(tempa[3]);
      }
    }
#endif
    RoundKey[i * 4 + 0] = RoundKey[(i - Nk) * 4 + 0] ^ tempa[0];
    RoundKey[i * 4 + 1] = RoundKey[(i - Nk) * 4 + 1] ^ tempa[1];
    RoundKey[i * 4 + 2] = RoundKey[(i - Nk) * 4 + 2] ^ tempa[2];
    RoundKey[i * 4 + 3] = RoundKey[(i - Nk) * 4 + 3] ^ tempa[3];
  }
}

// This function adds the round key to state.
// The round key is added to the state by an XOR function.
static void AddRoundKey(uint8_t round)
{
  uint8_t i,j;
  for (i=0;i<4;++i)
  {
    for (j = 0; j < 4; ++j)
    {
      (*state)[i][j] ^= RoundKey[round * Nb * 4 + i * Nb + j];
    }
  }
}

// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
static void SubBytes(void)
{
  uint8_t i, j;
  for (i = 0; i < 4; ++i)
  {
    for (j = 0; j < 4; ++j)
    {
      (*state)[j][i] = getSBoxValue((*state)[j][i]);
    }
  }
}

// The ShiftRows() function shifts the rows in the state to the left.
// Each row is shifted with different offset.
// Offset = Row number. So the first row is not shifted.
static void ShiftRows(void)
{
  uint8_t temp;

  // Rotate first row 1 columns to left  
  temp           = (*state)[0][1];
  (*state)[0][1] = (*state)[1][1];
  (*state)[1][1] = (*state)[2][1];
  (*state)[2][1] = (*state)[3][1];
  (*state)[3][1] = temp;

  // Rotate second row 2 columns to left  
  temp           = (*state)[0][2];
  (*state)[0][2] = (*state)[2][2];
  (*state)[2][2] = temp;

  temp           = (*state)[1][2];
  (*state)[1][2] = (*state)[3][2];
  (*state)[3][2] = temp;

  // Rotate third row 3 columns to left
  temp           = (*state)[0][3];
  (*state)[0][3] = (*state)[3][3];
  (*state)[3][3] = (*state)[2][3];
  (*state)[2][3] = (*state)[1][3];
  (*state)[1][3] = temp;
}

static uint8_t xtime(uint8_t x)
{
  return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

// MixColumns function mixes the columns of the state matrix
static void MixColumns(void)
{
  uint8_t i;
  uint8_t Tmp,Tm,t;
  for (i = 0; i < 4; ++i)
  {  
    t   = (*state)[i][0];
    Tmp = (*state)[i][0] ^ (*state)[i][1] ^ (*state)[i][2] ^ (*state)[i][3] ;
    Tm  = (*state)[i][0] ^ (*state)[i][1] ; Tm = xtime(Tm);  (*state)[i][0] ^= Tm ^ Tmp ;
    Tm  = (*state)[i][1] ^ (*state)[i][2] ; Tm = xtime(Tm);  (*state)[i][1] ^= Tm ^ Tmp ;
    Tm  = (*state)[i][2] ^ (*state)[i][3] ; Tm = xtime(Tm);  (*state)[i][2] ^= Tm ^ Tmp ;
    Tm  = (*state)[i][3] ^ t ;              Tm = xtime(Tm);  (*state)[i][3] ^= Tm ^ Tmp ;
  }
}

// Multiply is used to multiply numbers in the field GF(2^8)
#if MULTIPLY_AS_A_FUNCTION
static uint8_t Multiply(uint8_t x, uint8_t y)
{
  return (((y & 1) * x) ^
       ((y>>1 & 1) * xtime(x)) ^
       ((y>>2 & 1) * xtime(xtime(x))) ^
       ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^
       ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))));
  }
#else
#define Multiply(x, y)                                \
      (  ((y & 1) * x) ^                              \
      ((y>>1 & 1) * xtime(x)) ^                       \
      ((y>>2 & 1) * xtime(xtime(x))) ^                \
      ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^         \
      ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))))   \

#endif

// MixColumns function mixes the columns of the state matrix.
// The method used to multiply may be difficult to understand for the inexperienced.
// Please use the references to gain more information.
static void InvMixColumns(void)
{
  int i;
  uint8_t a, b, c, d;
  for (i = 0; i < 4; ++i)
  { 
    a = (*state)[i][0];
    b = (*state)[i][1];
    c = (*state)[i][2];
    d = (*state)[i][3];

    (*state)[i][0] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
    (*state)[i][1] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
    (*state)[i][2] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
    (*state)[i][3] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
  }
}


// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
static void InvSubBytes(void)
{
  uint8_t i,j;
  for (i = 0; i < 4; ++i)
  {
    for (j = 0; j < 4; ++j)
    {
      (*state)[j][i] = getSBoxInvert((*state)[j][i]);
    }
  }
}

static void InvShiftRows(void)
{
  uint8_t temp;

  // Rotate first row 1 columns to right  
  temp = (*state)[3][1];
  (*state)[3][1] = (*state)[2][1];
  (*state)[2][1] = (*state)[1][1];
  (*state)[1][1] = (*state)[0][1];
  (*state)[0][1] = temp;

  // Rotate second row 2 columns to right 
  temp = (*state)[0][2];
  (*state)[0][2] = (*state)[2][2];
  (*state)[2][2] = temp;

  temp = (*state)[1][2];
  (*state)[1][2] = (*state)[3][2];
  (*state)[3][2] = temp;

  // Rotate third row 3 columns to right
  temp = (*state)[0][3];
  (*state)[0][3] = (*state)[1][3];
  (*state)[1][3] = (*state)[2][3];
  (*state)[2][3] = (*state)[3][3];
  (*state)[3][3] = temp;
}


// Cipher is the main function that encrypts the PlainText.
static void Cipher(void)
{
  uint8_t round = 0;

  // Add the First round key to the state before starting the rounds.
  AddRoundKey(0); 
  
  // There will be Nr rounds.
  // The first Nr-1 rounds are identical.
  // These Nr-1 rounds are executed in the loop below.
  for (round = 1; round < Nr; ++round)
  {
    SubBytes();
    ShiftRows();
    MixColumns();
    AddRoundKey(round);
  }
  
  // The last round is given below.
  // The MixColumns function is not here in the last round.
  SubBytes();
  ShiftRows();
  AddRoundKey(Nr);
}

static void InvCipher(void)
{
  uint8_t round=0;

  // Add the First round key to the state before starting the rounds.
  AddRoundKey(Nr); 

  // There will be Nr rounds.
  // The first Nr-1 rounds are identical.
  // These Nr-1 rounds are executed in the loop below.
  for (round = (Nr - 1); round > 0; --round)
  {
    InvShiftRows();
    InvSubBytes();
    AddRoundKey(round);
    InvMixColumns();
  }
  
  // The last round is given below.
  // The MixColumns function is not here in the last round.
  InvShiftRows();
  InvSubBytes();
  AddRoundKey(0);
}

/*****************************************************************************/
/* Public functions:                                                         */
/*****************************************************************************/
#if defined(ECB) && (ECB == 1)
void AES_ECB_encrypt(const uint8_t* input, const uint8_t* key, uint8_t* output, const uint32_t length)
{
  // Copy input to output, and work in-memory on output
  memcpy(output, input, length);
  state = (state_t*)output;

  Key = key;
  KeyExpansion();

  // The next function call encrypts the PlainText with the Key using AES algorithm.
  Cipher();
}

void AES_ECB_decrypt(const uint8_t* input, const uint8_t* key, uint8_t *output, const uint32_t length)
{
  // Copy input to output, and work in-memory on output
  memcpy(output, input, length);
  state = (state_t*)output;

  // The KeyExpansion routine must be called before encryption.
  Key = key;
  KeyExpansion();

  InvCipher();
}
#endif // #if defined(ECB) && (ECB == 1)

#if defined(CBC) && (CBC == 1)
static void XorWithIv(uint8_t* buf)
{
  uint8_t i;
  for (i = 0; i < BLOCKLEN; ++i) //WAS for(i = 0; i < KEYLEN; ++i) but the block in AES is always 128bit so 16 bytes!
  {
    buf[i] ^= Iv[i];
  }
}

void AES_CBC_encrypt_buffer(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv)
{
  uintptr_t i;
  uint8_t extra = length % BLOCKLEN; /* Remaining bytes in the last non-full block */

  // Skip the key expansion if key is passed as 0
  if (0 != key)
  {
    Key = key;
    KeyExpansion();
  }

  if (iv != 0)
  {
    Iv = (uint8_t*)iv;
  }

  for (i = 0; i < length; i += BLOCKLEN)
  {
    XorWithIv(input);
    memcpy(output, input, BLOCKLEN);
    state = (state_t*)output;
    Cipher();
    Iv = output;
    input += BLOCKLEN;
    output += BLOCKLEN;
    //printf("Step %d - %d", i/16, i);
  }

  if (extra)
  {
    memcpy(output, input, extra);
    state = (state_t*)output;
    Cipher();
  }
}

void AES_CBC_decrypt_buffer(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv)
{
  uintptr_t i;
  uint8_t extra = length % BLOCKLEN; /* Remaining bytes in the last non-full block */

  // Skip the key expansion if key is passed as 0
  if (0 != key)
  {
    Key = key;
    KeyExpansion();
  }

  // If iv is passed as 0, we continue to encrypt without re-setting the Iv
  if (iv != 0)
  {
    Iv = (uint8_t*)iv;
  }

  for (i = 0; i < length; i += BLOCKLEN)
  {
    memcpy(output, input, BLOCKLEN);
    state = (state_t*)output;
    InvCipher();
    XorWithIv(output);
    Iv = input;
    input += BLOCKLEN;
    output += BLOCKLEN;
  }

  if (extra)
  {
    memcpy(output, input, extra);
    state = (state_t*)output;
    InvCipher();
  }
}
#endif // #if defined(CBC) && (CBC == 1)
}

static const FlockSDK::String base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

namespace FlockSDK
{

unsigned CountElements(const char* buffer, char separator)
{
    if (!buffer)
        return 0;

    const char* endPos = buffer + String::CStringLength(buffer);
    const char* pos = buffer;
    unsigned ret = 0;

    while (pos < endPos)
    {
        if (*pos != separator)
            break;
        ++pos;
    }

    while (pos < endPos)
    {
        const char* start = pos;

        while (start < endPos)
        {
            if (*start == separator)
                break;

            ++start;
        }

        if (start == endPos)
        {
            ++ret;
            break;
        }

        const char* end = start;

        while (end < endPos)
        {
            if (*end != separator)
                break;

            ++end;
        }

        ++ret;
        pos = end;
    }

    return ret;
}

bool ToBool(const String &source)
{
    return ToBool(source.CString());
}

bool ToBool(const char* source)
{
    unsigned length = String::CStringLength(source);

    for (auto i = 0u; i < length; ++i)
    {
        char c = (char)tolower(source[i]);
        if (c == 't' || c == 'y' || c == '1')
            return true;
        else if (c != ' ' && c != '\t')
            break;
    }

    return false;
}

int ToInt(const String &source, int base)
{
    return ToInt(source.CString(), base);
}

int ToInt(const char* source, int base)
{
    if (!source)
        return 0;

    // Shield against runtime library assert by converting illegal base values to 0 (autodetect)
    if (base < 2 || base > 36)
        base = 0;

    return (int)strtol(source, 0, base);
}

long long ToInt64(const char* source, int base)
{
    if (!source)
        return 0;

    // Shield against runtime library assert by converting illegal base values to 0 (autodetect)
    if (base < 2 || base > 36)
        base = 0;

    return strtoll(source, 0, base);
}

long long ToInt64(const String &source, int base)
{
    return ToInt64(source.CString(), base);
}

unsigned ToUInt(const String &source, int base)
{
    return ToUInt(source.CString(), base);
}

unsigned long long ToUInt64(const char* source, int base)
{
    if (!source)
        return 0;

    // Shield against runtime library assert by converting illegal base values to 0 (autodetect)
    if (base < 2 || base > 36)
        base = 0;

    return strtoull(source, 0, base);
}

unsigned long long ToUInt64(const String &source, int base)
{
    return ToUInt64(source.CString(), base);
}

unsigned ToUInt(const char* source, int base)
{
    if (!source)
        return 0;

    if (base < 2 || base > 36)
        base = 0;

    return (unsigned)strtoul(source, 0, base);
}

float ToFloat(const String &source)
{
    return ToFloat(source.CString());
}

float ToFloat(const char* source)
{
    if (!source)
        return 0;

    return (float)strtod(source, 0);
}

double ToDouble(const String &source)
{
    return ToDouble(source.CString());
}

double ToDouble(const char* source)
{
    if (!source)
        return 0;

    return strtod(source, 0);
}

Color ToColor(const String &source)
{
    return ToColor(source.CString());
}

Color ToColor(const char* source)
{
    Color ret;

    unsigned elements = CountElements(source, ' ');
    if (elements < 3)
        return ret;

    char* ptr = (char*)source;
    ret.r_ = (float)strtod(ptr, &ptr);
    ret.g_ = (float)strtod(ptr, &ptr);
    ret.b_ = (float)strtod(ptr, &ptr);
    if (elements > 3)
        ret.a_ = (float)strtod(ptr, &ptr);

    return ret;
}

IntRect ToIntRect(const String &source)
{
    return ToIntRect(source.CString());
}

IntRect ToIntRect(const char* source)
{
    IntRect ret(IntRect::ZERO);

    unsigned elements = CountElements(source, ' ');
    if (elements < 4)
        return ret;

    char* ptr = (char*)source;
    ret.left_ = (int)strtol(ptr, &ptr, 10);
    ret.top_ = (int)strtol(ptr, &ptr, 10);
    ret.right_ = (int)strtol(ptr, &ptr, 10);
    ret.bottom_ = (int)strtol(ptr, &ptr, 10);

    return ret;
}

IntVector2 ToIntVector2(const String &source)
{
    return ToIntVector2(source.CString());
}

IntVector2 ToIntVector2(const char* source)
{
    IntVector2 ret(IntVector2::ZERO);

    unsigned elements = CountElements(source, ' ');
    if (elements < 2)
        return ret;

    char* ptr = (char*)source;
    ret.x_ = (int)strtol(ptr, &ptr, 10);
    ret.y_ = (int)strtol(ptr, &ptr, 10);

    return ret;
}

IntVector3 ToIntVector3(const String &source)
{
    return ToIntVector3(source.CString());
}

IntVector3 ToIntVector3(const char* source)
{
    IntVector3 ret(IntVector3::ZERO);

    unsigned elements = CountElements(source, ' ');
    if (elements < 3)
        return ret;

    char* ptr = (char*)source;
    ret.x_ = (int)strtol(ptr, &ptr, 10);
    ret.y_ = (int)strtol(ptr, &ptr, 10);
    ret.z_ = (int)strtol(ptr, &ptr, 10);

    return ret;
}

Rect ToRect(const String &source)
{
    return ToRect(source.CString());
}

Rect ToRect(const char* source)
{
    Rect ret(Rect::ZERO);

    unsigned elements = CountElements(source, ' ');
    if (elements < 4)
        return ret;

    char* ptr = (char*)source;
    ret.min_.x_ = (float)strtod(ptr, &ptr);
    ret.min_.y_ = (float)strtod(ptr, &ptr);
    ret.max_.x_ = (float)strtod(ptr, &ptr);
    ret.max_.y_ = (float)strtod(ptr, &ptr);

    return ret;
}

Quaternion ToQuaternion(const String &source)
{
    return ToQuaternion(source.CString());
}

Quaternion ToQuaternion(const char* source)
{
    unsigned elements = CountElements(source, ' ');
    char* ptr = (char*)source;

    if (elements < 3)
        return Quaternion::IDENTITY;
    else if (elements < 4)
    {
        // 3 coords specified: conversion from Euler angles
        float x, y, z;
        x = (float)strtod(ptr, &ptr);
        y = (float)strtod(ptr, &ptr);
        z = (float)strtod(ptr, &ptr);

        return Quaternion(x, y, z);
    }
    else
    {
        // 4 coords specified: full quaternion
        Quaternion ret;
        ret.w_ = (float)strtod(ptr, &ptr);
        ret.x_ = (float)strtod(ptr, &ptr);
        ret.y_ = (float)strtod(ptr, &ptr);
        ret.z_ = (float)strtod(ptr, &ptr);

        return ret;
    }
}

Vector2 ToVector2(const String &source)
{
    return ToVector2(source.CString());
}

Vector2 ToVector2(const char* source)
{
    Vector2 ret(Vector2::ZERO);

    unsigned elements = CountElements(source, ' ');
    if (elements < 2)
        return ret;

    char* ptr = (char*)source;
    ret.x_ = (float)strtod(ptr, &ptr);
    ret.y_ = (float)strtod(ptr, &ptr);

    return ret;
}

Vector3 ToVector3(const String &source)
{
    return ToVector3(source.CString());
}

Vector3 ToVector3(const char* source)
{
    Vector3 ret(Vector3::ZERO);

    unsigned elements = CountElements(source, ' ');
    if (elements < 3)
        return ret;

    char* ptr = (char*)source;
    ret.x_ = (float)strtod(ptr, &ptr);
    ret.y_ = (float)strtod(ptr, &ptr);
    ret.z_ = (float)strtod(ptr, &ptr);

    return ret;
}

Vector4 ToVector4(const String &source, bool allowMissingCoords)
{
    return ToVector4(source.CString(), allowMissingCoords);
}

Vector4 ToVector4(const char* source, bool allowMissingCoords)
{
    Vector4 ret(Vector4::ZERO);

    unsigned elements = CountElements(source, ' ');
    char* ptr = (char*)source;

    if (!allowMissingCoords)
    {
        if (elements < 4)
            return ret;

        ret.x_ = (float)strtod(ptr, &ptr);
        ret.y_ = (float)strtod(ptr, &ptr);
        ret.z_ = (float)strtod(ptr, &ptr);
        ret.w_ = (float)strtod(ptr, &ptr);

        return ret;
    }
    else
    {
        if (elements > 0)
            ret.x_ = (float)strtod(ptr, &ptr);
        if (elements > 1)
            ret.y_ = (float)strtod(ptr, &ptr);
        if (elements > 2)
            ret.z_ = (float)strtod(ptr, &ptr);
        if (elements > 3)
            ret.w_ = (float)strtod(ptr, &ptr);

        return ret;
    }
}

Variant ToVectorVariant(const String &source)
{
    return ToVectorVariant(source.CString());
}

Variant ToVectorVariant(const char* source)
{
    Variant ret;
    unsigned elements = CountElements(source, ' ');

    switch (elements)
    {
    case 1:
        ret.FromString(VAR_FLOAT, source);
        break;

    case 2:
        ret.FromString(VAR_VECTOR2, source);
        break;

    case 3:
        ret.FromString(VAR_VECTOR3, source);
        break;

    case 4:
        ret.FromString(VAR_VECTOR4, source);
        break;

    case 9:
        ret.FromString(VAR_MATRIX3, source);
        break;

    case 12:
        ret.FromString(VAR_MATRIX3X4, source);
        break;

    case 16:
        ret.FromString(VAR_MATRIX4, source);
        break;

    default:
        // Illegal input. Return variant remains empty
        break;
    }

    return ret;
}

Matrix3 ToMatrix3(const String &source)
{
    return ToMatrix3(source.CString());
}

Matrix3 ToMatrix3(const char* source)
{
    Matrix3 ret(Matrix3::ZERO);

    unsigned elements = CountElements(source, ' ');
    if (elements < 9)
        return ret;

    char* ptr = (char*)source;
    ret.m00_ = (float)strtod(ptr, &ptr);
    ret.m01_ = (float)strtod(ptr, &ptr);
    ret.m02_ = (float)strtod(ptr, &ptr);
    ret.m10_ = (float)strtod(ptr, &ptr);
    ret.m11_ = (float)strtod(ptr, &ptr);
    ret.m12_ = (float)strtod(ptr, &ptr);
    ret.m20_ = (float)strtod(ptr, &ptr);
    ret.m21_ = (float)strtod(ptr, &ptr);
    ret.m22_ = (float)strtod(ptr, &ptr);

    return ret;
}

Matrix3x4 ToMatrix3x4(const String &source)
{
    return ToMatrix3x4(source.CString());
}

Matrix3x4 ToMatrix3x4(const char* source)
{
    Matrix3x4 ret(Matrix3x4::ZERO);

    unsigned elements = CountElements(source, ' ');
    if (elements < 12)
        return ret;

    char* ptr = (char*)source;
    ret.m00_ = (float)strtod(ptr, &ptr);
    ret.m01_ = (float)strtod(ptr, &ptr);
    ret.m02_ = (float)strtod(ptr, &ptr);
    ret.m03_ = (float)strtod(ptr, &ptr);
    ret.m10_ = (float)strtod(ptr, &ptr);
    ret.m11_ = (float)strtod(ptr, &ptr);
    ret.m12_ = (float)strtod(ptr, &ptr);
    ret.m13_ = (float)strtod(ptr, &ptr);
    ret.m20_ = (float)strtod(ptr, &ptr);
    ret.m21_ = (float)strtod(ptr, &ptr);
    ret.m22_ = (float)strtod(ptr, &ptr);
    ret.m23_ = (float)strtod(ptr, &ptr);

    return ret;
}

Matrix4 ToMatrix4(const String &source)
{
    return ToMatrix4(source.CString());
}

Matrix4 ToMatrix4(const char* source)
{
    Matrix4 ret(Matrix4::ZERO);

    unsigned elements = CountElements(source, ' ');
    if (elements < 16)
        return ret;

    char* ptr = (char*)source;
    ret.m00_ = (float)strtod(ptr, &ptr);
    ret.m01_ = (float)strtod(ptr, &ptr);
    ret.m02_ = (float)strtod(ptr, &ptr);
    ret.m03_ = (float)strtod(ptr, &ptr);
    ret.m10_ = (float)strtod(ptr, &ptr);
    ret.m11_ = (float)strtod(ptr, &ptr);
    ret.m12_ = (float)strtod(ptr, &ptr);
    ret.m13_ = (float)strtod(ptr, &ptr);
    ret.m20_ = (float)strtod(ptr, &ptr);
    ret.m21_ = (float)strtod(ptr, &ptr);
    ret.m22_ = (float)strtod(ptr, &ptr);
    ret.m23_ = (float)strtod(ptr, &ptr);
    ret.m30_ = (float)strtod(ptr, &ptr);
    ret.m31_ = (float)strtod(ptr, &ptr);
    ret.m32_ = (float)strtod(ptr, &ptr);
    ret.m33_ = (float)strtod(ptr, &ptr);

    return ret;
}

String ToString(void* value)
{
    return ToStringHex((unsigned)(size_t)value);
}

String ToStringHex(unsigned value)
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    std::sprintf(tempBuffer, "%08x", value);
    return String(tempBuffer);
}

void BufferToString(String &dest, const void* data, unsigned size)
{
    // Precalculate needed string size
    const unsigned char* bytes = (const unsigned char*)data;
    unsigned length = 0;
    for (auto i = 0u; i < size; ++i)
    {
        // Room for separator
        if (i)
            ++length;

        // Room for the value
        if (bytes[i] < 10)
            ++length;
        else if (bytes[i] < 100)
            length += 2;
        else
            length += 3;
    }

    dest.Resize(length);
    unsigned index = 0;

    // Convert values
    for (auto i = 0u; i < size; ++i)
    {
        if (i)
            dest[index++] = ' ';

        if (bytes[i] < 10)
        {
            dest[index++] = '0' + bytes[i];
        }
        else if (bytes[i] < 100)
        {
            dest[index++] = (char)('0' + bytes[i] / 10);
            dest[index++] = (char)('0' + bytes[i] % 10);
        }
        else
        {
            dest[index++] = (char)('0' + bytes[i] / 100);
            dest[index++] = (char)('0' + bytes[i] % 100 / 10);
            dest[index++] = (char)('0' + bytes[i] % 10);
        }
    }
}

void StringToBuffer(PODVector<unsigned char>& dest, const String &source)
{
    StringToBuffer(dest, source.CString());
}

void StringToBuffer(PODVector<unsigned char>& dest, const char* source)
{
    if (!source)
    {
        dest.Clear();
        return;
    }

    unsigned size = CountElements(source, ' ');
    dest.Resize(size);

    bool inSpace = true;
    unsigned index = 0;
    unsigned value = 0;

    // Parse values
    const char* ptr = source;
    while (*ptr)
    {
        if (inSpace && *ptr != ' ')
        {
            inSpace = false;
            value = (unsigned)(*ptr - '0');
        }
        else if (!inSpace && *ptr != ' ')
        {
            value *= 10;
            value += *ptr - '0';
        }
        else if (!inSpace && *ptr == ' ')
        {
            dest[index++] = (unsigned char)value;
            inSpace = true;
        }

        ++ptr;
    }

    // Write the final value
    if (!inSpace && index < size)
        dest[index] = (unsigned char)value;
}

unsigned GetStringListIndex(const String &value, const String* strings, unsigned defaultIndex, bool caseSensitive)
{
    return GetStringListIndex(value.CString(), strings, defaultIndex, caseSensitive);
}

unsigned GetStringListIndex(const char* value, const String* strings, unsigned defaultIndex, bool caseSensitive)
{
    unsigned i = 0;

    while (!strings[i].Empty())
    {
        if (!strings[i].Compare(value, caseSensitive))
            return i;
        ++i;
    }

    return defaultIndex;
}

unsigned GetStringListIndex(const char* value, const char** strings, unsigned defaultIndex, bool caseSensitive)
{
    unsigned i = 0;

    while (strings[i])
    {
        if (!String::Compare(value, strings[i], caseSensitive))
            return i;
        ++i;
    }

    return defaultIndex;
}

String ToString(const char* formatString, ...)
{
    String ret;
    va_list args;
    va_start(args, formatString);
    ret.AppendWithFormatArgs(formatString, args);
    va_end(args);
    return ret;
}

bool IsAlpha(unsigned ch)
{
    return ch < 256 ? isalpha(ch) != 0 : false;
}

bool IsDigit(unsigned ch)
{
    return ch < 256 ? isdigit(ch) != 0 : false;
}

unsigned ToUpper(unsigned ch)
{
    return (unsigned)toupper(ch);
}

unsigned ToLower(unsigned ch)
{
    return (unsigned)tolower(ch);
}

String GetFileSizeString(unsigned long long memorySize)
{
    static const char* memorySizeStrings = "kMGTPE";

    String output;

    if (memorySize < 1024)
    {
        output = String(memorySize) + " b";
    }
    else
    {
        const int exponent = (int)(log((double)memorySize) / log(1024.0));
        const double majorValue = ((double)memorySize) / pow(1024.0, exponent);
        char buffer[64];
        memset(buffer, 0, 64);
        std::sprintf(buffer, "%.1f", majorValue);
        output = buffer;
        output += " ";
        output += memorySizeStrings[exponent - 1];
    }

    return output;
}

// Implementation of Base64 decoding originally by Ren� Nyffenegger.
// Modified by Konstantin Guschin and Lasse Oorni.
/*
base64.cpp and base64.h

Copyright (C) 2004-2017 Ren� Nyffenegger

This source code is provided 'as-is', without any express or implied
warranty. In no event will the author be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this source code must not be misrepresented; you must not
claim that you wrote the original source code. If you use this source code
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original source code.

3. This notice may not be removed or altered from any source distribution.

Ren� Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/
static inline bool IsBase64(char c)
{
    return (::isalnum(c) || (c == '+') || (c == '/'));
}

PODVector<unsigned char> DecodeBase64(const String &encodedString)
{
    int i = 0, j = 0, in_ = 0;
    auto inLen = encodedString.Length();
    unsigned char charArray4[4]{}, charArray3[3]{};
    PODVector<unsigned char> ret;

    while (inLen-- && (encodedString[in_] != '=') && IsBase64(encodedString[in_]))
    {
        charArray4[i++] = encodedString[in_]; 
        in_++;

        if (i == 4)
        {
            for (i = 0; i < 4; i++)
                charArray4[i] = base64_chars.Find(charArray4[i]);

            charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
            charArray3[1] = ((charArray4[1] & 0xf) << 4) + ((charArray4[2] & 0x3c) >> 2);
            charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];

            for (i = 0; i < 3; i++)
                ret.Push(charArray3[i]);
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 4; j++)
            charArray4[j] = 0;

        for (j = 0; j < 4; j++)
            charArray4[j] = base64_chars.Find(charArray4[j]);

        charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
        charArray3[1] = ((charArray4[1] & 0xf) << 4) + ((charArray4[2] & 0x3c) >> 2);
        charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];

        for (j = 0; j < i - 1; j++) 
            ret.Push(charArray3[j]);
    }

    return ret;
}

static std::vector<uint8_t> InputToAESPaddedBuffer (const std::string &s)
{
    std::vector<uint8_t> v(s.begin(), s.end());
    auto length = s.size();

    // If the input's length isn't divisible by 16 (which is required by AES), then pad zeros
    // at the end of the buffer.
    // https://stackoverflow.com/questions/17773450/why-must-all-inputs-to-aes-be-multiples-of-16
    if((length % 16) != 0)
    {
        // Round to the nearest number which is divisible by 16.
        int rounded = [] (int n, int d) -> int {
            return ((n < 0) ^ (d < 0)) ? ((n - d/2)/d) : ((n + d/2)/d);
        } (length, 16);
        length = rounded * 16;

        const auto j = (length - s.size()) * 2; // Since each character in the input takes two bytes on the buffer.
        for (int i = 0; i < j; ++i)
            v.push_back(0x00);
    }

    return v;
}

String AESEncryptToString(const String &s, const char *key)
{
    return [] (const std::string &s, const char *key) -> String {
        auto v = InputToAESPaddedBuffer(s);
        uint8_t buffer[v.size()];
        std::string o;
    
        TinyAES::AES_ECB_encrypt(v.data(), (const uint8_t *) key, buffer, v.size());
        for (const auto &i : buffer)
        {
            char b[2];
            std::sprintf(b, "%x", i);
            if (std::isalnum(b[0]))
                o += b[0];
            if (std::isalnum(b[1]))
                o += b[1];
        }
        
        return o.c_str();
    }(s.CString(), key);
}

String AESEncrypt(const String &s, const char *key)
{
    // This lambda can be easily refactored into a separate function which works with std::string.
    return [] (const std::string &s, const char *key) -> String {
        auto v = InputToAESPaddedBuffer(s);
        std::string o;
        
        uint8_t buffer[v.size()];
        std::memset(buffer, 0, sizeof(uint8_t) * v.size());
        TinyAES::AES_ECB_encrypt(v.data(), (const uint8_t *) key, buffer, v.size());
        for (const auto &i : buffer)
            o += i;
        
        return o.c_str();
    }(s.CString(), key);
}

String AESDecrypt(const String &v, const char *key)
{
    return [] (const std::string &t, const char *key) -> String {
        auto s = InputToAESPaddedBuffer(t);
        uint8_t buffer_out[s.size()];
        std::string o;
    
        TinyAES::AES_ECB_decrypt(s.data(), (const uint8_t *) key, buffer_out, s.size());
        for (const auto &i : buffer_out)
            o += i;
    
        return o.c_str();
    }(v.CString(), key);
}

}
