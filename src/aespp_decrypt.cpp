#include "aespp.hpp"

namespace aespp {
	const unsigned char inv_sBox[256] = {
		0x52,	0x09,	0x6a,	0xd5,	0x30,	0x36,	0xa5,	0x38,	0xbf,   0x40,	0xa3,	0x9e,	0x81,	0xf3,	0xd7,	0xfb,
		0x7c,	0xe3,	0x39,	0x82,	0x9b,	0x2f,	0xff,	0x87,	0x34,   0x8e,	0x43,	0x44,	0xc4,	0xde,	0xe9,	0xcb,
		0x54,	0x7b,	0x94,	0x32,	0xa6,	0xc2,	0x23,	0x3d,	0xee,   0x4c,	0x95,	0x0b,	0x42,	0xfa,	0xc3,	0x4e,
		0x08,	0x2e,	0xa1,	0x66,	0x28,	0xd9,	0x24,	0xb2,	0x76,   0x5b,	0xa2,	0x49,	0x6d,	0x8b,	0xd1,	0x25,
		0x72,	0xf8,	0xf6,	0x64,	0x86,	0x68,	0x98,	0x16,	0xd4,   0xa4,	0x5c,	0xcc,	0x5d,	0x65,	0xb6,	0x92,
		0x6c,	0x70,	0x48,	0x50,	0xfd,	0xed,	0xb9,	0xda,	0x5e,   0x15,	0x46,	0x57,	0xa7,	0x8d,	0x9d,	0x84,
		0x90,	0xd8,	0xab,	0x00,	0x8c,	0xbc,	0xd3,	0x0a,	0xf7,   0xe4,	0x58,	0x05,	0xb8,	0xb3,	0x45,	0x06,
		0xd0,	0x2c,	0x1e,	0x8f,	0xca,	0x3f,	0x0f,	0x02,	0xc1,   0xaf,	0xbd,	0x03,	0x01,	0x13,	0x8a,	0x6b,
		0x3a,	0x91,	0x11,	0x41,	0x4f,	0x67,	0xdc,	0xea,	0x97,   0xf2,	0xcf,	0xce,	0xf0,	0xb4,	0xe6,	0x73,
		0x96,	0xac,	0x74,	0x22,	0xe7,	0xad,	0x35,	0x85,	0xe2,   0xf9,	0x37,	0xe8,	0x1c,	0x75,	0xdf,	0x6e,
		0x47,	0xf1,	0x1a,	0x71,	0x1d,	0x29,	0xc5,	0x89,	0x6f,   0xb7,	0x62,	0x0e,	0xaa,	0x18,	0xbe,	0x1b,
		0xfc,	0x56,	0x3e,	0x4b,	0xc6,	0xd2,	0x79,	0x20,	0x9a,   0xdb,	0xc0,	0xfe,	0x78,	0xcd,	0x5a,	0xf4,
		0x1f,	0xdd,	0xa8,	0x33,	0x88,	0x07,	0xc7,	0x31,	0xb1,   0x12,	0x10,	0x59,	0x27,	0x80,	0xec,	0x5f,
		0x60,	0x51,	0x7f,	0xa9,	0x19,	0xb5,	0x4a,	0x0d,	0x2d,   0xe5,	0x7a,	0x9f,	0x93,	0xc9,	0x9c,	0xef,
		0xa0,	0xe0,	0x3b,	0x4d,	0xae,	0x2a,	0xf5,	0xb0,	0xc8,   0xeb,	0xbb,	0x3c,	0x83,	0x53,	0x99,	0x61,
		0x17,	0x2b,	0x04,	0x7e,	0xba,	0x77,	0xd6,	0x26,	0xe1,   0x69,	0x14,	0x63,	0x55,	0x21,	0x0c,	0x7d
	};

	const unsigned char inv_mixColumnMatrix[BLOCK_SIDE][BLOCK_SIDE] = {
		{ 0x0E, 0x0B, 0x0D, 0x09 },
		{ 0x09, 0x0E, 0x0B, 0x0D },
		{ 0x0D, 0x09, 0x0E, 0x0B },
		{ 0x0B, 0x0D, 0x09, 0x0E }
	};

	void inv_addRoundKey(unsigned char state[BLOCK_SIDE][BLOCK_SIDE], unsigned char subkey[BLOCK_SIDE][BLOCK_SIDE])
	{
		unsigned int i, j;
		for (i = 0; i < BLOCK_SIDE; i++) {
			for (j = 0; j < BLOCK_SIDE; j++) {
				state[i][j] ^= subkey[i][j];
			}
		}
	}

	void inv_byteSub(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]) // Full name: Inverse byte substitution
	{
		unsigned int i, j;

		for (i = 0; i < BLOCK_SIDE; i++) {
			for (j = 0; j < BLOCK_SIDE; j++) {
				state[i][j] = inv_sBox[state[i][j]];
			}
		}
	}
	
	void inv_mixColumns(unsigned char state[BLOCK_SIDE][BLOCK_SIDE])
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
					output[i][k] ^= galoisFieldMultiplicationTable[inv_CMDS[i][j]][state[j][k]];
				}
			}
		}
		
		for (i = 0; i < BLOCK_SIDE; i++) {
			std::memcpy(state[i], output[i], (BLOCK_SIDE + 1));
			}
	}
	
	void inv_shiftRows(unsigned char state[BLOCK_SIDE][BLOCK_SIDE])
	{
		unsigned int i, j, n = 0;
		for (i = 0; i < BLOCK_SIDE; i++)
		{
			for (j = 0; j < n; j++)
			{
				int k;
				unsigned char temp;

				temp = state[i][3];
				for (k = BLOCK_SIDE - 1; k > 0; k--)
				{
					state[i][k] = state[i][k - 1];
				}
				state[i][k] = temp;
			}
			n++;
		}
	}

	void decryptBlock(unsigned char inputText[], unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE], unsigned int nr, unsigned char iv[BLOCK_LEN], unsigned char output[BLOCK_LEN])
	{
		unsigned int i, j, k;
		unsigned char state[BLOCK_SIDE][BLOCK_SIDE];

		k = 0;
		for (i = 0; i < BLOCK_SIDE; i++) {
			for (j = 0; j < BLOCK_SIDE; j++) {
				state[j][i] = inputText[k++];
			}
		}

		inv_addRoundKey(state, subkeys[nr]);
		inv_shiftRows(state);
		inv_byteSub(state);

		for (i = nr - 1; i > 0; i--) {
			inv_addRoundKey(state, subkeys[i]);
			inv_mixColumns(state);
			inv_shiftRows(state);
			inv_byteSub(state);
		}

		inv_addRoundKey(state, subkeys[0]);

		k = 0;
		for (i = 0; i < BLOCK_SIDE; i++) {
			for (j = 0; j < BLOCK_SIDE; j++) {
				output[k] = state[j][i];

				if (iv) {
					output[k] ^= iv[k];
				}
				k++;
			}
		}
	}

	std::size_t decrypt(unsigned char inputText[], unsigned int inputLen, unsigned char inputKey[], unsigned char iv[BLOCK_LEN], unsigned  char** output, aesRounds keyLen, aesMode mode)
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
			outputSize = decryptWithECB(inputText, inputLen, inputKey, subkeys, nr, output);
			break;
		case aesMode::AES_CBC:
			outputSize = decryptWithCBC(inputText, inputLen, inputKey, subkeys, nr, iv, output);
			break;
		default:
			throw "Valid arguments are AES_ECB, AES_CBC.";
		}

		delete[] subkeys;
		return outputSize;
	}

	std::size_t decryptWithECB(unsigned char inputText[], unsigned int inputLen, unsigned char inputKey[], unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE], unsigned int nr, unsigned char** output)
	{
		int i, numberBlocks, extra;
		unsigned char* tempOutput = NULL, numberPadding;

		numberBlocks = inputLen >> 4;
		extra = inputLen & 4;

		if (extra) {
			numberBlocks++;
		}

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

		tempOutput = new unsigned char[numberBlocks * BLOCK_LEN + 1];
		for (i = 0; i < numberBlocks; i++) {
			decryptBlock(inputText + (i << 4), subkeys, nr, NULL, tempOutput + (i << 4));
		}

		numberPadding = tempOutput[numberBlocks * BLOCK_LEN - 1];
		*output = new unsigned char[numberBlocks * BLOCK_LEN - numberPadding];
		std::memcpy(*output, tempOutput, (numberBlocks * BLOCK_LEN - numberPadding));

		delete[] tempOutput;

		return numberBlocks * BLOCK_LEN - numberPadding;
	}

	std::size_t decryptWithCBC(unsigned char inputText[], unsigned int inputLen, unsigned char inputKey[], unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE], unsigned int nr, unsigned char iv[BLOCK_LEN], unsigned char** output)
	{
		if (!iv) {
			throw "Need a valid initialization vector (IV)";
		};
		
		unsigned int i, numberBlocks, extra;
		unsigned char* tempOutput = NULL, numberPadding;
		
		numberBlocks = inputLen >> 4;
		extra = inputLen & 4;

		if (extra) {
			numberBlocks++;
		}

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
		
		tempOutput = new unsigned char[numberBlocks * BLOCK_LEN + 1];
		
		for (i = 0; i < numberBlocks; i++) {
			if (!i) {
				decryptBlock(inputText + (i << 4), subkeys, nr, iv, tempOutput + (i << 4));
			}
			else {
				decryptBlock(inputText + (i << 4), subkeys, nr, inputText + ((i - 1) << 4), tempOutput + (i << 4));
			}
		}

		numberPadding = tempOutput[numberBlocks * BLOCK_LEN - 1];
		*output = new unsigned char[numberBlocks * BLOCK_LEN - numberPadding];
		
		std::memcpy(*output, tempOutput, (numberBlocks * BLOCK_LEN - numberPadding));
		
		delete[] tempOutput;

		return numberBlocks * BLOCK_LEN - numberPadding;
	}
};
