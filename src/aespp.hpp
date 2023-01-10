#ifndef AESPP_HPP
#define AESPP_HPP

#include "aespp_math.hpp"
#include <string>

#define BLOCK_LEN 16
#define BLOCK_SIDE 4

namespace aespp {

	enum class aesRounds {
		AES_128 = 10,
		AES_192 = 12,
		AES_256 = 14
	};

	enum class aesMode {
		AES_ECB = 0,
		AES_CBC = 1,
	};

	/*
	Substitution box
	*/
	extern const unsigned char sBox[256];

	/*
	Inverse Substitution Box
	*/
	extern const unsigned char inv_sBox[256];

	/*
	Mix Column Matrix for Mix Column step
	*/
	extern const unsigned char mixColumnMatrix[BLOCK_SIDE][BLOCK_SIDE];

	/*
	Inverse Mix Column Matrix for Inverse Mix Column step
	*/
	extern const unsigned char inv_mixColumnMatrix[BLOCK_SIDE][BLOCK_SIDE];

	/*
	Encryption layers
	*/
	void addRoundKey(unsigned char state[BLOCK_SIDE][BLOCK_SIDE], unsigned char subkey[BLOCK_SIDE][BLOCK_SIDE]);

	void byteSub(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]); // Full name: Byte substitution

	void shiftRows(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]);

	void mixColumns(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]);

	void encryptBlock(unsigned char inputText[], unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE], unsigned int nr, unsigned char iv[BLOCK_LEN], unsigned char* output);

	extern void keyExpansionAES128(unsigned char inputKey[], unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE], unsigned int nr);

	extern void keyExpansion(unsigned char inputKey[], unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE], unsigned int numberBlocks, unsigned int nr);

	std::size_t encrypt(unsigned char inputText[], unsigned int inputLen, unsigned char inputKey[], unsigned char iv[BLOCK_LEN], unsigned  char** output, aesRounds keyLen, aesMode mode);

	std::size_t encryptWithECB(unsigned char inputText[], unsigned int inputLen, unsigned char inputKey[], unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE], unsigned int nr, unsigned char** output);

	std::size_t encryptWithCBC(unsigned char inputText[], unsigned int inputLen, unsigned char inputKey[], unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE], unsigned int nr, unsigned char iv[BLOCK_LEN], unsigned char* output[]);

	/*
	Decryption layers
	*/
	void inv_addRoundKey(unsigned char state[BLOCK_SIDE][BLOCK_SIDE], unsigned char subkey[BLOCK_SIDE][BLOCK_SIDE]);

	void inv_byteSub(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]); // Full name: Byte substitution

	void inv_shiftRows(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]);

	void inv_mixColumns(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]);

	void decryptBlock(unsigned char inputText[], unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE], unsigned int nr, unsigned char iv[BLOCK_LEN], unsigned char output[BLOCK_LEN]);

	std::size_t decrypt(unsigned char inputText[], unsigned int inputLen, unsigned char inputKey[], unsigned char iv[BLOCK_LEN], unsigned  char** output, aesRounds keyLen, aesMode mode);

	std::size_t decryptWithECB(unsigned char inputText[], unsigned int inputLen, unsigned char inputKey[], unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE], unsigned int nr, unsigned char** output);

	std::size_t decryptWithCBC(unsigned char inputText[], unsigned int inputLen, unsigned char inputKey[], unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE], unsigned int nr, unsigned char iv[BLOCK_LEN], unsigned char** output);

	/*
	Utils
	*/
	unsigned char* charToUnsignedChar(std::string input)
	{
		std::size_t i;
		unsigned char* output = new unsigned char[input.length()];

		for (i = 0; i < input.length(); i++) {
			output[i] = static_cast<unsigned char>(input[i]);
		}

		return output;
	}

	unsigned char* charToUnsignedChar(char input[], std::size_t inputLen)
	{
		std::size_t i;
		unsigned char* output = new unsigned char[inputLen];
		for (i = 0; i < inputLen; i++) {
			output[i] = static_cast<unsigned char>(input[i]);
		}

		return output;
	}
	
	char* unsignedCharToChar(unsigned char input[], std::size_t inputLen)
	{
		std::size_t i;
		char* output = new char[inputLen + 1];
		
		for (i = 0; i < inputLen; i++) {
			output[i] = static_cast<char>(input[i]);
		}
		
		return output;
	}	

	void printHex(unsigned char* array, std::size_t length)
	{
		char hex[17] = "0123456789ABCDEF";
		printf("{ ");
		for (std::size_t i = 0; i < length; i++) {
			printf("%c%c", hex[array[i] >> 4], hex[array[i] & 0x0f]);
		}
		printf(" }\n");
	}
};

#endif // AESPP_HPP
