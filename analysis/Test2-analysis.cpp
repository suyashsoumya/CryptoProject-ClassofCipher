#include <iostream> 
#include <fstream>
#include <map>
#include <vector>     
#include <string>
#include <algorithm>  
#include <random>       
#include <chrono>   
#include <cstdlib>
using namespace std;



vector<string> getEnglishWords();
vector<int> shuffleNumVector();
map<char, vector<int>> createCipherMap(const map<char, int>& charFreq, const vector<int>& vNum);
void plaintextCiphertextToFile(ofstream& oFile, const string& plaintext, map<char, int> charFreq, map<char, vector<int>> cipherMap);
string generatePlaintext(const vector<string>& vEnglishWords);


int main() {
	map<char, int> charFreq = { { ' ', 19 },{ 'a', 7 },{ 'b', 1 },{ 'c', 2 },{ 'd', 4 },{ 'e', 10 },
	{ 'f', 2 },{ 'g', 2 },{ 'h', 5 },{ 'i', 6 },{ 'j', 1 },{ 'k', 1 },
	{ 'l', 3 },{ 'm', 2 },{ 'n', 6 },{ 'o', 6 },{ 'p', 2 },{ 'q', 1 },
	{ 'r', 5 },{ 's', 5 },{ 't', 7 },{ 'u', 2 },{ 'v', 1 },{ 'w', 2 },
	{ 'x', 1 },{ 'y', 2 },{ 'z', 1 } };

	vector<string> vEnglishWords = getEnglishWords();
	string plaintext = generatePlaintext(vEnglishWords);
	
	// file is for analysis of encryption scheme
	ofstream oFile;
	oFile.open("Test2-analysis.csv", fstream::out);
	for (int i = 0; i < 100; ++i){
		vector<int> vNum = shuffleNumVector();
		map<char, vector<int>> cipherMap = createCipherMap(charFreq, vNum);
		oFile << "plainChar" << "," << "cipherChar" << endl;
		plaintextCiphertextToFile(oFile, plaintext, charFreq, cipherMap);
	}
	oFile.close();
	
	return 0;
}



vector<string> getEnglishWords() {
	ifstream englishWordsFile("test2_candidate_70_english_words.txt");
	//Check if file can be opened
	if (!englishWordsFile) {
		cerr << "File failed to open\n";
		exit(1);
	}
	// get each plaintext and put it in vector
	vector<string> vEnglishWords;
	string word;
	while (englishWordsFile >> word) {
		vEnglishWords.push_back(word);
	}
	englishWordsFile.close();
	
	// get time-based seed:
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine e(seed);
	// shuffle english words in vector
	shuffle(vEnglishWords.begin(), vEnglishWords.end(), e);

	return vEnglishWords;
}



vector<int> shuffleNumVector() {
	// get time-based seed:
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine e(seed);

	// randomize numbers in vector
	vector<int> vNum;
	for (size_t i = 0; i <= 105; ++i) {
		vNum.push_back(i);
	}

	shuffle(vNum.begin(), vNum.end(), e);

	return vNum;
}



map<char, vector<int>> createCipherMap(const map<char, int>& charFreq, const vector<int>& vNum) {
	// map each character to a list of numbers
	map<char, vector<int>> cipherMap;
	int index = 0;
	for (auto const& x : charFreq) {
		vector<int> cipherValues;
		for (int i = 0; i < x.second; ++i) {
			cipherValues.push_back(vNum[index]);
			++index;
		}
		cipherMap.insert({ x.first, cipherValues });
	}
	return cipherMap;
}



string generatePlaintext(const vector<string>& vEnglishWords) {
	string plaintext;
	int length = 0;
	for (const string& word : vEnglishWords){
		if (length + word.length() > 500) {
			int difference = (length + word.length()) - 500;
			plaintext += word.substr(0,word.length() - difference);
			break;
		}
		else if (length + word.length() + 1 > 500){
			plaintext += word;
			length += word.length();
		}
		else{
			plaintext += word + " ";
			length += word.length() + 1;
		}
	}
	return plaintext;
}



void plaintextCiphertextToFile(ofstream& oFile, const string& plaintext, map<char, int> charFreq, map<char, vector<int>> cipherMap) {
	string ciphertext;
	int index = 1;
	for (const char& p : plaintext) {
		int freq = charFreq[p];
		string cipherChar = to_string(cipherMap[p][rand() % freq]);
		ciphertext += cipherChar;
		if (index != 500) {
			ciphertext += ",";
		}
		++index;
		oFile << p << "," << cipherChar << endl;
	}
	
	
}
