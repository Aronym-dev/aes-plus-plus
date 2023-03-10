#include "aespp.hpp"

namespace aespp
{
	/*
	Substitution Box
	*/
	const unsigned char sBox[256] = {
		0x63,   0x7c,   0x77,   0x7b,   0xf2,   0x6b,   0x6f,   0xc5,   0x30,   0x01,   0x67,   0x2b,   0xfe,   0xd7,   0xab,   0x76,
		0xca,   0x82,   0xc9,   0x7d,   0xfa,   0x59,   0x47,   0xf0,   0xad,   0xd4,   0xa2,   0xaf,   0x9c,   0xa4,   0x72,   0xc0,
		0xb7,   0xfd,   0x93,   0x26,   0x36,   0x3f,   0xf7,   0xcc,   0x34,   0xa5,   0xe5,   0xf1,   0x71,   0xd8,   0x31,   0x15,
		0x04,   0xc7,   0x23,   0xc3,   0x18,   0x96,   0x05,   0x9a,   0x07,   0x12,   0x80,   0xe2,   0xeb,   0x27,   0xb2,   0x75,
		0x09,   0x83,   0x2c,   0x1a,   0x1b,   0x6e,   0x5a,   0xa0,   0x52,   0x3b,   0xd6,   0xb3,   0x29,   0xe3,   0x2f,   0x84,
		0x53,   0xd1,   0x00,   0xed,   0x20,   0xfc,   0xb1,   0x5b,   0x6a,   0xcb,   0xbe,   0x39,   0x4a,   0x4c,   0x58,   0xcf,
		0xd0,   0xef,   0xaa,   0xfb,   0x43,   0x4d,   0x33,   0x85,   0x45,   0xf9,   0x02,   0x7f,   0x50,   0x3c,   0x9f,   0xa8,
		0x51,   0xa3,   0x40,   0x8f,   0x92,   0x9d,   0x38,   0xf5,   0xbc,   0xb6,   0xda,   0x21,   0x10,   0xff,   0xf3,   0xd2,
		0xcd,   0x0c,   0x13,   0xec,   0x5f,   0x97,   0x44,   0x17,   0xc4,   0xa7,   0x7e,   0x3d,   0x64,   0x5d,   0x19,   0x73,
		0x60,   0x81,   0x4f,   0xdc,   0x22,   0x2a,   0x90,   0x88,   0x46,   0xee,   0xb8,   0x14,   0xde,   0x5e,   0x0b,   0xdb,
		0xe0,   0x32,   0x3a,   0x0a,   0x49,   0x06,   0x24,   0x5c,   0xc2,   0xd3,   0xac,   0x62,   0x91,   0x95,   0xe4,   0x79,
		0xe7,   0xc8,   0x37,   0x6d,   0x8d,   0xd5,   0x4e,   0xa9,   0x6c,   0x56,   0xf4,   0xea,   0x65,   0x7a,   0xae,   0x08,
		0xba,   0x78,   0x25,   0x2e,   0x1c,   0xa6,   0xb4,   0xc6,   0xe8,   0xdd,   0x74,   0x1f,   0x4b,   0xbd,   0x8b,   0x8a,
		0x70,   0x3e,   0xb5,   0x66,   0x48,   0x03,   0xf6,   0x0e,   0x61,   0x35,   0x57,   0xb9,   0x86,   0xc1,   0x1d,   0x9e,
		0xe1,   0xf8,   0x98,   0x11,   0x69,   0xd9,   0x8e,   0x94,   0x9b,   0x1e,   0x87,   0xe9,   0xce,   0x55,   0x28,   0xdf,
		0x8c,   0xa1,   0x89,   0x0d,   0xbf,   0xe6,   0x42,   0x68,   0x41,   0x99,   0x2d,   0x0f,   0xb0,   0x54,   0xbb,   0x16
	};

	/*
	Mix Column Matrix for Mix Column step
	*/
	const unsigned char mixColumnMatrix[BLOCK_SIDE][BLOCK_SIDE] = {
		{ 0x02, 0x03, 0x01, 0x01 },
		{ 0x01, 0x02, 0x03, 0x01 },
		{ 0x01, 0x01, 0x02, 0x03 },
		{ 0x03, 0x01, 0x01, 0x02 }
	};

	/*
	Encryption layers
	*/
	void addRoundKey(unsigned char state[BLOCK_SIDE][BLOCK_SIDE], unsigned char subkey[BLOCK_SIDE][BLOCK_SIDE])
	{
		unsigned int i, j;
		for (i = 0; i < BLOCK_SIDE; i++) {
			for (j = 0; j < BLOCK_SIDE; j++) {
				state[i][j] ^= subkey[i][j];
			}
		}
	}

	void byteSub(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]) // Full name: Byte substitution
	{
		unsigned int i, j;
		for (i = 0; i < BLOCK_SIDE; i++) {
			for (j = 0; j < BLOCK_SIDE; j++) {
				state[i][j] = sBox[state[i][j]];
			}
		}
	}

	void shiftRows(unsigned char state[BLOCK_SIDE][BLOCK_SIDE])
	{
		/*
			The state array before shift rows

			00	01	02	03
			04	05	06	07
			08	09	10	11
			12	13	14	15

			The state array after shift rows

			00	01	02	03	No shift
			07	04	05	06	One shift
			11	10	08	09	Two shifts
			15	14	13	12	Three shifts
		*/
		unsigned int i, j, n = 0;
		for (i = 0; i < BLOCK_SIDE; i++) {
			for (j = 0; j < n; j++) {
				unsigned char temp;
				int k;
				temp = state[i][0];

				for (k = 0; k < BLOCK_SIDE - 1; k++) {
					state[i][k] = state[i][k + 1];
				}

				state[i][k] = temp;
			}

			n++;
		}
	}
	
	void mixColumns(unsigned char state[BLOCK_SIDE][BLOCK_SIDE])
	{
		unsigned int i, j, k;
		unsigned char output[BLOCK_SIDE][BLOCK_SIDE];
		
		for (i = 0; i < 4; ++i)
		{
			std::memset(output[i], 0, 4);
		}
		
		for (i = 0; i < BLOCK_SIDE; i++)
		{
			for (j = 0; j < BLOCK_SIDE; j++)
			{
				for (k = 0; k < BLOCK_SIDE; k++)
				{
					if (CMDS[i][j] == 1) {
						output[i][k] ^= state[j][k];
					} else {
						output[i][k] ^= galoisFieldMultiplicationTable[CMDS[i][j]][state[j][k]];
					}
				}
			}
		}
		
		for (i = 0; i < BLOCK_SIDE; i++) {
			std::memcpy(state[i], output[i], (BLOCK_SIDE + 1));
			}
	}

	void encryptBlock(unsigned char inputText[], unsigned int inputLen, unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE], int nr, unsigned char iv[BLOCK_LEN], unsigned char output[BLOCK_LEN])
	{
		unsigned int i, j, k;
		unsigned char state[BLOCK_SIDE][BLOCK_SIDE];

		k = 0;
		for (i = 0; i < BLOCK_SIDE; i++) {
			for (j = 0; j < BLOCK_SIDE; j++) {
				state[j][i] = (k < inputLen) ? inputText[k] : BLOCK_LEN - inputLen;
				if (iv) {
					state[j][i] ^= iv[k];
				}
				k++;
			}
		}

		// Round 0 takes in original key
		addRoundKey(state, subkeys[0]);

		// The other rounds
		for (i = 1; i < nr; i++) {
			byteSub(state);
			shiftRows(state);
			mixColumns(state);
			addRoundKey(state, subkeys[i]);
		}

		// Round NR
		byteSub(state);
		shiftRows(state);
		addRoundKey(state, subkeys[nr]);

		k = 0;
		for (i = 0; i < BLOCK_SIDE; i++) {
			for (j = 0; j < BLOCK_SIDE; j++) {
				output[k++] = state[j][i];
			}
		}
	}

	std::size_t encrypt(unsigned char inputText[], unsigned int inputLen, unsigned char inputKey[], unsigned char iv[BLOCK_LEN], unsigned  char** output, aesRounds keyLen, aesMode mode)
	{
		unsigned int nr;
		std::size_t outputSize = 0;

		if (inputText == 0) {
			throw "Invalid input_text.";
		}

		switch (keyLen) {
		case aesRounds::AES_128:
			nr = 10;
			break;
		case aesRounds::AES_192:
			nr = 12;
			break;
		case aesRounds::AES_256:
			nr = 14;
			break;
		default:
			throw "Valid arguments are AES_256, AES_192 and AES_128.";
		}

		unsigned char(*subkeys)[BLOCK_SIDE][BLOCK_SIDE] = new unsigned char[nr + 1][BLOCK_SIDE][BLOCK_SIDE];

		switch (mode) {
		case aesMode::AES_ECB:
			outputSize = encryptWithECB(inputText, inputLen, inputKey, subkeys, nr, output);
			break;
		case aesMode::AES_CBC:
			outputSize = encryptWithCBC(inputText, inputLen, inputKey, subkeys, nr, iv, output);
			break;
		default:
			throw "Valid arguments are AES_ECB, AES_CBC.";
		}

		delete[] subkeys;
		return outputSize;
	}

	void keyExpansionAES128(unsigned char inputKey[], unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE], unsigned int nr)
	{
		unsigned int i, j, k;

		i = 0;
		for (j = 0; j < BLOCK_SIDE; j++) {
			for (k = 0; k < BLOCK_SIDE; k++) {
				subkeys[0][k][j] = inputKey[i++];
			}
		}

		unsigned char roundCoefficient = 0x01;

		for (i = 1; i <= nr; i++) {
			unsigned char g[4] = {
				(unsigned char)(sBox[subkeys[i - 1][1][3]] ^ roundCoefficient),
				sBox[subkeys[i - 1][2][3]],
				sBox[subkeys[i - 1][3][3]],
				sBox[subkeys[i - 1][0][3]]
			};

			for (j = 0; j < BLOCK_SIDE; j++) {
				subkeys[i][j][0] = subkeys[i - 1][j][0] ^ g[j];
			}

			for (j = 1; j < BLOCK_SIDE; j++) {
				for (k = 0; k < BLOCK_SIDE; k++) {
					subkeys[i][k][j] = subkeys[i - 1][k][j] ^ subkeys[i][k][j - 1];
				}
			}

			roundCoefficient = AES_MATH::galoisMultiplication(roundCoefficient, 0x02);
		}
	}

	void keyExpansion(unsigned char inputKey[], unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE], unsigned int numberBlocks, unsigned int nr)
	{
		unsigned int i = 0, j, k, round = 0, column;
		for (j = 0; j < numberBlocks; j++)
		{
			column = j % 4;
			round = j / 4;
			for (k = 0; k < BLOCK_SIDE; k++)
			{
				subkeys[round][k][column] = inputKey[i++];
			}
		}

		unsigned char roundCoefficient = 0x01;
		int previousRound, previousColumn;
		switch (nr) {
		case 12:
			previousRound = 1, previousColumn = 1;
			for (i = 6; i < 52; i++) {
				round = i / 4;
				column = i % 4;
				if (i % numberBlocks == 0) {
					unsigned char g[4] = {
						(unsigned char)(sBox[subkeys[previousRound][1][previousColumn]] ^ roundCoefficient),
						sBox[subkeys[previousRound][2][previousColumn]],
						sBox[subkeys[previousRound][3][previousColumn]],
						sBox[subkeys[previousRound][0][previousColumn]]
					};

					for (j = 0; j < BLOCK_SIDE; j++) {
						subkeys[round][j][column] = subkeys[(i - 6) / 4][j][(i - 6) % 4] ^ g[j];
					}

					roundCoefficient = AES_MATH::galoisMultiplication(roundCoefficient, 0x02);
				}
				else {
					for (j = 0; j < BLOCK_SIDE; j++) {
						subkeys[round][j][column] = subkeys[(i - 6) / 4][j][(i - 6) % 4] ^ subkeys[previousRound][j][previousColumn];
					}
				}
				previousRound = round;
				previousColumn = column;
			}
			break;
		case 14:
			previousRound = 1, previousColumn = 3;
			for (i = 8; i < 60; i++) {
				round = i / 4;
				column = i % 4;
				if (!(i % 8)) {
					unsigned char g[4] = {
						(unsigned char)(sBox[subkeys[previousRound][1][previousColumn]] ^ roundCoefficient),
						sBox[subkeys[previousRound][2][previousColumn]],
						sBox[subkeys[previousRound][3][previousColumn]],
						sBox[subkeys[previousRound][0][previousColumn]]
					};

					for (j = 0; j < BLOCK_SIDE; j++) {
						subkeys[round][j][column] = subkeys[round - 2][j][column] ^ g[j];
					}
					roundCoefficient = AES_MATH::galoisMultiplication(roundCoefficient, 0x02);
				}
				else if (!(i % 4)) {
					for (j = 0; j < BLOCK_SIDE; j++) {
						subkeys[round][j][column] = subkeys[round - 2][j][column] ^ sBox[subkeys[previousRound][j][previousColumn]];
					}
				}
				else {
					for (j = 0; j < BLOCK_SIDE; j++) {
						subkeys[round][j][column] = subkeys[round - 2][j][column] ^ subkeys[previousRound][j][previousColumn];
					}
				}

				previousRound = round, previousColumn = column;
			}
			break;
		};
	}

	std::size_t encryptWithECB(unsigned char inputText[], unsigned int inputLen, unsigned char inputKey[], unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE], unsigned int nr, unsigned char** output)
	{
		unsigned int i, numberBlocks, extra;
		numberBlocks = (inputLen >> 4) + 1;
		extra = inputLen & 0x0f;
		std::size_t outputLen = numberBlocks * BLOCK_LEN;

		*output = new unsigned char[outputLen];

		switch (nr) {
		case 10:
			keyExpansionAES128(inputKey, subkeys, nr);
			break;
		case 12:
			keyExpansion(inputKey, subkeys, 6, nr);
			break;
		case 14:
			keyExpansion(inputKey, subkeys, 8, nr);
		};

		for (i = 0; i < numberBlocks; i++) {
			unsigned int len = (i < numberBlocks - 1) ? BLOCK_LEN : extra;
			encryptBlock(inputText + (i << 4), len, subkeys, nr, NULL, *output + (i << 4));
		}

		return outputLen;
	}

	std::size_t encryptWithCBC(unsigned char inputText[], unsigned inputLen, unsigned char inputKey[], unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE], unsigned int nr, unsigned char iv[BLOCK_LEN], unsigned char** output)
	{
		if (!iv) {
			throw "Need a valid initialization vector (IV)";
		};
		
		unsigned int i;
		unsigned int numberBlocks = (inputLen >> 4) + 1; // For example if input_len is 32 the return value is 2 + 1
		unsigned int extra = inputLen & 0x0f;
		std::size_t outputLen = numberBlocks * BLOCK_LEN;
		*output = new unsigned char[outputLen];
		
		switch (nr) {
		case 10:
			keyExpansionAES128(inputKey, subkeys, nr);
			break;
		case 12:
			keyExpansion(inputKey, subkeys, 6, nr);
			break;
		case 14:
			keyExpansion(inputKey, subkeys, 8, nr);
		};
		
		for (i = 0; i < numberBlocks; i++) {
			if (!i) {
				unsigned int len = (i < numberBlocks - 1) ? BLOCK_LEN : extra;
				encryptBlock(inputText + (i << 4), len, subkeys, nr, iv, *output + (i << 4));
			} else {
				unsigned int len = (i < numberBlocks - 1) ? BLOCK_LEN : extra;
				encryptBlock(inputText + (i << 4), len, subkeys, nr, *output + ((i - 1) << 4), *output + (i << 4));
			}
		}
		
		return outputLen;
	}
};
