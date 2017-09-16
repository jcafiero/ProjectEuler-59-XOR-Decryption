/*
 * xordecryption.cpp
 *
 *  Created on: Mar 25, 2017
 *      Authors: Jennifer Cafiero, Catherine Javadian, Jordana Approvato
 *      I pledge my honor that I have abided by the Stevens Honor System.
 *      Project Euler #59 - XOR Decryption
 *
 */

#include <iostream>

const int BUFFER = 2048;
char correctKey[3];
int maxSpaceCount = 0;
int len = 0; // length of encrypted text

// param: char
// return: bool
// returns true if character is whitespace
// returns false otherwise
bool whitespace(char c) {
	if (c ==' ' || c == '\0'){
		return true;
	}
	return false;
}

// param: char
// return: char
// returns true if character is a valid english character
// returns false otherwise
bool englishChar(char c) {
	if (c < 32 || c > 126){
		return false;
	}
	return true;
}

// param: const char* cipher (block of encrypted text from file)
// param: char c1 - first char key
// param: char c2 - second char key
// param: char c3 - third char key
// return: bool
// takes in possible key combination (c1,c2,c3) and uses it to calculate a
// decrypted version of the encrypted char * (cipher)
// keeping track of the whitespace count and exiting function when a Decrypted
// character is found that is not a valid english character
// stores a key combination as a possible correctKey if its whitespace count
// is greater than any previous whitespace counts because the most common char
// to find in English is a space character (although 'e' is fairly common too)
bool check(const char* cipher, char c1, char c2, char c3) {
	char result[BUFFER + 1];
	char key[3] = {c1, c2, c3};
	int spaceCount = 0;

	for (int i = 0; i < len; i++) {
		result[i] = cipher[i] ^ key[i % 3];

		if (whitespace(result[i])) {
			spaceCount++;
		}
		if(!englishChar(result[i])){
			return 0;
		}

	}
	if(spaceCount > maxSpaceCount){
		maxSpaceCount = spaceCount;
		correctKey[0] = c1;
		correctKey[1] = c2;
		correctKey[2] = c3;
	}
	return 1;
}


// main function does three things:
// 1. Reads cipher.txt file into a buffer
// 2. Calls check function on each possible key [aaa - zzz] and finds correct key
// 3. Calculates and prints result from check function
int main(int argc, char* argv[]){
	char encrypt_str[BUFFER];
	memset(encrypt_str, 0, BUFFER); //memset fills encrypt_str with 0s to size of BUFF_SIZE
	int n;
	unsigned long long correctSum = 0;
	char correctResult[BUFFER + 1];

	// reads cipher file into encrypt_str
	FILE* cipher = fopen("cipher.txt", "r");//r denotes read-only of file
	if (cipher){
		bool atEnd = false;
		while (len < BUFFER) {
			n = fgetc(cipher);//fgetc will get the next character from the file and advance to next position
			if (feof(cipher)) {
				atEnd = true;
				break;
			}
			if (n == ','){
				len++;
			}
			else if (n >= '0' && n <= '9') {
				encrypt_str[len] = (encrypt_str[len] * 10) + (n - '0');
			}
		}
		fclose(cipher);
		cipher = NULL;
		if (atEnd == false) {
			printf("File is too big for buffer set");
			return -1;
		}
	}
	else {
		printf("Error: Unable to open cipher.txt");
		return -1;
	}
	if (encrypt_str[len] > 0){
		len++;
	}

	// iterates through all possible combinations [aaa - zzz]
	// and calls check function on them
	for (char c1 = 'a'; c1 <= 'z'; c1++){
		for (char c2 = 'a'; c2 <= 'z'; c2++){
			for (char c3 = 'a'; c3 <= 'z'; c3++){
				check(encrypt_str, c1, c2, c3);
			}
		}

	}

	// prints results: message, key, and sum of ASCII values
	printf("Decrypted Message:\n\n");

	for(int i=0;i<len;i++) {
		correctSum += encrypt_str[i] ^ correctKey[i % 3];
		printf("%c", (encrypt_str[i] ^ correctKey[i % 3]));
	}
	printf("\n\n");
	printf("The key is: %c%c%c\n\n", correctKey[0], correctKey[1], correctKey[2]);
	printf("The sum of ASCII values is: %llu\n\n", correctSum);

	return 0;
}
