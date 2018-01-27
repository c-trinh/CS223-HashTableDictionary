/* Name:	Cong Trinh
 * Class:	Cpts 223
 * Project:	Hashtable Dictionary
 * Date finished: 24 November 2016
 * 
 * Description: This program uses a hashtable and chaining in order to store large quantities of words into
 * 				while using quick and efficient sorting. The program sorts words into the table by hashing
 * 				the strings into index values, and storing those words in the appropriate index in the table.
 * 				When the word to tableSize ratio gets too large, the hashTable is expanded and words are rehashed
 * 				to prevent words from clumping.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include "Hashing.h"

int main(int argc, char* argv[]) {
	Dictionary dict;
	
	dict.parser();
	
	//dict.remove("WINCE");
	//dict.printTable();
	dict.printInfo();
	
	dict.query();
	return(0);
}
