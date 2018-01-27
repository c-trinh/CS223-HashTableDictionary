#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class Word {
	private:
		string word;
		string def;
	public:
		/*If no word/defintion is given, word/def is set empty*/
		Word (void) {
			this->word = "";
			this->def = "";
		}
		
		/*If word/def is given, stores those variables*/
		Word(string word, string def) {
			this->word = word;
			this->def = def;
		}
		
		/*Converts String -> ascii value (int)*/
		unsigned int key(void) {
			unsigned int hashVal = 0;
			for (char ch : word)
				hashVal = 37 * hashVal + ch;
			return hashVal;	//Convert Word -> ascii int
		}
		
		/*Replaces previous word/definition with new values*/
		void overwrite(Word &w) {
			this->word = w.getWord();
			this->def = w.getDef();
		}
		
		/*Prints the definition of the word to the screen*/
		void print (void) {
			cout << "[*]" << this->word << ": " << this->def << endl;
		}
		
		/*Returns the word*/
		string getWord(void) {
			return this->word;
		}
		
		/*Returns the definition*/
		string getDef(void) {
			return this->def;
		}
};

class Dictionary {
	private:
		vector<vector<Word>> table;
		int totalWords;	//"N Size" variable; Total words in the table (dictionary)
		int tableSize;	//Size of the table
		ifstream input;	//File for parsing in words+definitons from file
	public:
		/*Initilizes when new Dictionary is made*/
		Dictionary() {
			tableSize = 101;	//Initilizes table size to 101
			totalWords = 0;		//Initilizes 0 words in the dictionary
			makeEmpty(table);	//Makes a new empty list
		}
		
		void makeEmpty(vector<vector<Word>> &table) {
			table.clear();		//Clears the table
			Word empty;	//Inserts blank strings into the table
			
			for (int x = 0; x < tableSize; x++) {	//Scans down the table vector based on the tableSize cap
				vector<Word> temp;					//Creates a temp vector to branch off first vector
				for (int y = 0; y < 1; y++) {			//Scans through second vector (Cap: 10)
					temp.push_back(empty);				//Inserts 10 blank Words into the branching vector (Only 10 to save memory and Rehashing will un-clump from sharing same vector)
				}
				table.push_back(temp);				//Inserts branching vector into the table vector
			}
		}
		
		/*Inserts new word into the table (Dictionary)*/
		void insert(Word &w, bool newWord) {
			int x = hash(w.key()), y = 0;	//Hashes the index value into 'x', y initilized at 0
			while ((table[x][y].getWord() != "") && (table[x][y].getWord() != w.getWord())) {	//While TableNode != Blank OR TableNode == Word
				y++;					//Increases the y value (Scans through branching vector until empty space/existing word is found)
			}
			if (table[x][y].getWord() != w.getWord() && newWord == true)	//If word does not already exist
				totalWords++;			//Updates new word count in the dictionary
			for (int x = 0; x < tableSize; x++) {	//Scans down the table vector based on the tableSize cap
				vector<Word> temp;					//Creates a temp vector to branch off first vector
				for (int y = 0; y < 1; y++) {			//Scans through second vector (Cap: 10)
					temp.push_back(w);				//Inserts 10 blank Words into the branching vector (Only 10 to save memory and Rehashing will un-clump from sharing same vector)
				}
				table.push_back(temp);				//Inserts branching vector into the table vector
			}
			//table[x][y].overwrite(w);	//When empty TableNode or existing word is found, replace with new word/definition
			
			if (getLoadFactor() > 1)	//If the load factor goes above 1, rehash
				rehash();				//Rehashes table to unclump words
		}
		
		/*Hashes for index if given a key*/
		unsigned int hash (unsigned int key) {
			return key % tableSize;	//Returns index to store in table
		}
		
		/*Hashes for index if given a string*/
		unsigned int hash (const string &word) {
			unsigned int hashVal = 0;
			for (char ch : word)
				hashVal = 37 * hashVal + ch;	//Converts to ascii hashVal
			return hashVal % tableSize;	//Returns index to store in table
		}
		
		/*Expands the table size and rehashes the words in a bigger table (Double Size rounded to nearest Prime)*/
		void rehash(void) {
			vector<vector<Word>> oldTable = this->table;	//Creates a temp holding old table
			
			tableSize *= 2;				//Doubles table size
			if (!isPrime(tableSize))	//If current tableSize is not a prime value
				tableSize = findPrime(tableSize);	//Finds the next prime value
			
			makeEmpty(table);	//Makes new table empty w/ new size
			
			for (unsigned int x = 0; x < oldTable.size(); x++) {
				for (unsigned int y = 0; y < oldTable[x].size(); y++) {
					Word temp(oldTable[x][y].getWord(), oldTable[x][y].getDef());	//Creates a new Word from the old table
					insert(temp, false);											//Inserts the word from the old table -> new table
				}
			}
		}
		
		/*Removes existing word from the table (Returns null if none is found)*/
		Word* remove(string w) {
			Word empty;			//Empty word
			int i = hash(w);	//Hashes to find the index location
			for (unsigned int x = 0; x < table[i].size(); x++)	//Scans through the branching vector for the word
				if (table[i][x].getWord() == w) {				//If word is found in the table
					Word *temp = new Word(table[i][x].getWord(), table[i][x].getDef());	//Creates a temp word
					table[i][x].overwrite(empty);	//Overwrites work with blank word (Doesn't use erase because my set up is set differently)
					totalWords--;					//Updates one less word in the dictionary
					return temp;
				}
			return nullptr;		//Returns null
		}
		
		/*Returns true if word is found in the table (False otherwise)*/
		bool contains(string &word) {
			int i = hash(word);	//FInds index where word would be held
			for (unsigned int x = 0; x < table[i].size(); x++) {	//Scans through the branhcing vector
				if (table[i][x].getWord() == word) {					//If word is found
					table[i][x].print();								//Prints definition when found
					return true;										//Returns true that word was found
				}
			}
			return false;	//Returns false if no word was found
		}
		
		/*Checks if a given int is prime or not*/
		bool isPrime(int val) {
			for (int x = 2; x <= val/2; x++) {	//Checks that value is not divisible by any number besides 1 & itself
				if (val % x == 0)	//If value is not prime
					return false;	//Return false
			}
			return true;	//Returns true if prime is found
		}
		
		/*Returns the next prime of a given value*/
		int findPrime(int val) {
			//loop continuously until isPrime returns true for a number above n
			for (;;) {	//Infinite loop
				val++;	//Increases value
				if (isPrime(val))	//Checks if current value is prime
					return val;		//Returns prime value
			}
		}
		
		/*Returns the load factor*/
		double getLoadFactor(void) {
			return (double)totalWords/(double)tableSize;	//Returns load factor
		}
		
		/*Allows user to input word to search and prints the definition*/
		void query (void){
			string w = "\0";
			cout << "[To exit, input \"Ctrl+D\"]\n" << endl;
			cout << ">Word to define: ";
			while (getline(cin, w)) {	//Requests for word to search before entering the loop
				cout << endl;
				for (unsigned int x = 0; x < w.size(); x++)	//Scans through each individual letter in input
					w[x] = toupper(w[x]);					//Converts input char to Upper
				
				if (contains(w) != true || w == "")		//If word is not found OR is empty (If found, will print definition from inside Contains)
					cout << "[!] Word not found." << endl;	//Prints word was not found
				cout << "______________________________________________________________________"<< endl;	//Seperator
				cout << ">Word to define: ";	//New word to define
			}
			cout << "\n\n[Program Terminated.]";	//When user exits, prints program has ended
		}
		
		/*Reads input file and stores in the table*/
		void parser (void) {
			cout << "Processing..." << endl;	//When parsing a large file, lets user know words are currently parsing
			input.open("dictionary.json", ios::in);	//Inputs words from specified file, opens file
			while (!input.eof()) {
				string word = "", def = "";		//Stores the word/definition here
				
				getline(input, word, '"');		//Scans between the first and second quatations
				getline(input, word, '"');
				
				getline(input, def, '}');		//Scans from the second quotation to the '}' symbol
				def.erase(0, 3);				//Trims from index 1-3
				def[def.size()-1] = '\0';		//Makes last index a null
				
				for (unsigned int x = 0; x < word.size(); x++)	//Scans through each individual letter in input
					word[x] = toupper(word[x]);	//Security check: Converts word to all caps
				
				Word temp(word, def);			//Stores inputted word/def into a Word class variable
				
				if (temp.getWord() != "") {		//Error check, make sure word is not scanned empty
					insert(temp, true);			//Inserts the word into the table
				}
			}
			input.close();	//Closes file
		}
		
		/*Prints the totalWords, tableSize and load factor*/
		void printInfo(void) {
			cout << "______________________________________________________________________" << endl << endl;	//Seperator
			cout << "-Words in Dictionary:\t" << totalWords;			//Prints total words in dictionary
			cout << endl << "-Current table size:\t" << tableSize;		//Prints the current table size
			cout << endl << "-Current load factor:\t" << getLoadFactor() << "\n\n";	//Prints the current load factor
		}
		
		/*(FOR TESTING PURPOSES) Prints the word, definition, indexes and totalWord count*/
		void printTable() {
			int count = 1;	//Variable to count words
			for (unsigned int x = 0; x < table.size(); x++) {
				for (unsigned int y = 0; y < table[x].size(); y++) {
					if (table[x][y].getWord() != "") {
						cout << count << ".) " << "[" << x << "][" << y << "]= ";	//Prints indexes
						table[x][y].print();										//Prints word + definition
						count++;													//Updates total words displayed
					}
				}
			}
			cout << endl;
		}
};
