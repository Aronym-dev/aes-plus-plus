# AES plus plus
<a name="how-to-use"></a>
## How to use?
```
#include "aespp_encrypt.cpp"
#include "aespp_decrypt.cpp"
#include <iostream>
	
int main()
{
	unsigned char *txt,  *key, *iv, *cipher = NULL, *decipher;
	char *result;
	
	std::string msg = "Example message";
	std::string password = "0123456789abcdefgexamplepassword";
	char* initializerVector = "0123456789abcdef";
	
	txt = aespp::charToUnsignedChar(msg); // This function casts std::string to unsigned char
	key = aespp::charToUnsignedChar(password);
	iv = aespp::charToUnsignedChar(initializerVector, 16); // This function also works with chars, see the documentation for more information
	
/*
Encryption step
*/
	std::size_t encryptSize = aespp::encrypt(txt, msg.length(), key, iv, &cipher, aespp::aesRounds::AES_256, aespp::aesMode::AES_CBC); // The encrypt function returns the size of the encrypted message
	
/*
Decryption step
*/
	std::size_t decryptSize = aespp::decrypt(cipher, encryptSize, key, iv, &decipher, aespp::aesRounds::AES_256, aespp::aesMode::AES_CBC);
	
/*
Print section
*/
	std::cout << "Plaintext: ";
	aespp::printHex(txt, msg.length());
	
	std::cout << "Encrypted message: ";
	aespp::printHex(cipher, encryptSize);
	
	std::cout << "Decrypted message (In hex): ";
	aespp::printHex(decipher, decryptSize);
	
	result = aespp::unsignedCharToChar(decipher, decryptSize); // This function casts unsigned char to char
	std::cout << result;
}
```
## Documentation
- [Encryption function](#encryption-function)
- [Decryption function](#decryption-function)
- ### Encryption modes
    - [ECB mode encryption and decryption](#ECB-mode)
    - [CBC mode encryption and decryption](#CBC-mode)
- ### AES rounds
    - [AES 128](#AES-128)
    - [AES 192](#AES-192)
    - [AES 256](#AES-256)
- ### Extra functions
    - [char to unsigned char function](#charToUnsignedChar-function)
    - [unsigned char to char](#unsignedCharToChar-function)
    - [print to hex function](#printHex-function)

<a name="encryption-function"></a>
## Encryption function
### Parameters
    1. Plaintext: The message to encrypt, doesn't have a defined or specific size.
    2. Plaintext size: The length of the plaintext, needed for padding.
    3. Key: Password used in encryption and decryption, the size dependes on the number of rounds, the key length size is 16 for AES 128, 24 for AES 192 and 32 for AES 256.
    4. Initializer vector (IV): A unsigned char array of 16 bits similar to the password, only needed in CBC mode.
    5. Output: A null pointer array where the encrypted message will be allocated.
    6. AES rounds: A enum class named aesRounds with three options AES_128, AES_192 and AES_256.
    7. AES mode: A enum class named aesMode with two options AES_ECB, AES_CBC (CBC mode needs a IV).

<a name="decryption-function"></a>
## Decryption function
### Parameters
    1. Encrypted message: The message encrypted by AES, doesn't have a defined or specific size.
    2. Encrypted message size: The length of the encrypted message, needed for padding.
    3. Key: Password used in encryption and decryption, the size dependes on the number of rounds, the key length size is 16 for AES 128, 24 for AES 192 and 32 for AES 256.
    4. Initializer vector (IV): A unsigned char array of 16 bits similar to the password, only needed in CBC mode.
    5. Output: A null pointer array where the decrypted message will be allocated.
    6. AES rounds: A enum class named aesRounds with three options AES_128, AES_192 and AES_256.
    7. AES mode: A enum class named aesMode with two options AES_ECB, AES_CBC (CBC mode needs a IV).

<a name="ECB-mode"></a>
## ECB encryption and decryption (Electronic Code Book)
You can select the encryption mode with the enum class named aesMode
```
enum class aesMode {
    AES_ECB = 0,
    AES_CBC = 1
}; // The aesMode enum is defined in aespp.hpp
```
Example of how to use ECB encryption mode using the function aespp::encrypt(...) or aespp::decrypt(...)
```
...
std::size_t cipherSize = aespp::encrypt(plaintext, plaintextLength, key, NULL, &output, aespp::aesRounds::AES_128, aespp::aesMode::AES_ECB); // Encryption using ECB mode

std::size_t messageSize = aespp::decrypt(plaintext, plaintextLength, key, NULL, &output, aespp::aesRounds::AES_128, aespp::aesMode::AES_ECB); // Decryption using ECB mode
...
```
<a name="CBC-mode"></a>
## CBC encryption and decryption (Cipher Block Chaining)
You can select the encryption mode with the enum class named aesMode
```
enum class aesMode {
    AES_ECB = 0,
    AES_CBC = 1
}; // The aesMode enum is defined in aespp.hpp
```

Example of how to use CBC encryption mode using the function aespp::encrypt(...) or aespp::decrypt(...)

**For the CBC encryption mode is needed a initialization vector (IV) as the fourth parameter, if it isn't provided it will throw an exception.
The initialization vector is a unsigned char array of 16 bits.**
```
...
unsigned char iv = charToUnsignedChar("0123456789abcdef"); // Example of IV

std::size_t cipherSize = aespp::encrypt(plaintext, plaintextLength, key, iv, &output, aespp::aesRounds::AES_128, aespp::aesMode::AES_CBC); // Encryption using CBC mode

std::size_t messageSize = aespp::decrypt(output, cipherSize, key, iv, &decipher, aespp::aesRounds::AES_128, aespp::aesMode::AES_ECB); // Decryption using CBC mode
...
```
## AES rounds
<a name="AES-128"></a>
### AES 128
You can select the number of rounds using the enum class named aesRounds.
```
enum class aesRounds {
    AES_128 = 10,
    AES_192 = 12,
    AES_256 = 14
}; // The aesRounds enum is defined in aespp.hpp
```
AES 128 requires a unsigned char array of 16 bits as the third parameter of the encryption/decryption functions.

<a name="AES-192"></a>
### AES 192
You can select the number of rounds using the enum class named aesRounds.
```
enum class aesRounds {
    AES_128 = 10,
    AES_192 = 12,
    AES_256 = 14
}; // The aesRounds enum is defined in aespp.hpp
```
AES 192 requires a unsigned char array of 24 bits as the third parameter of the encryption/decryption functions.

<a name="AES-256"></a>
### AES 256
You can select the number of rounds using the enum class named aesRounds.
```
enum class aesRounds {
    AES_128 = 10,
    AES_192 = 12,
    AES_256 = 14
}; // The aesRounds enum is defined in aespp.hpp
```
AES 256 requires a unsigned char array of 32 bits as the third parameter of the encryption/decryption functions.

## Extra
<a name="charToUnsignedChar-function"></a>
### charToUnsignedChar function
This function helps to cast char or string types to unsigned char

Parameters (Only for std::string):
1. String: The string object

Parameters:
1. Array: A char array
2. Length: Length of the array

The example of use is in [how to use][#how-to-use] example

<a name="unsignedCharToChar-function"></a>
### unsignedCharToChar function
This function helps to cast unsigned char array to char type

Parameters:
1. Array: An unsigned char array
2. Length: Length of the array

The example of use is in [how to use](#how-to-use) example

### printHex function
<a name="printHex-function"></a>
This function print in hex a unsigned char array

Parameters:
1. Array: An unsigned char array
2. Length: The length of the array or the length of characters to print

Example:
```
printHex(message, messageLength);
```