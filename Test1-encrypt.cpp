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



vector<string> getPlaintexts();
vector<int> shuffleNumVector();
map<char, vector<int>> createCipherMap(const map<char, int>& charFreq, const vector<int>& vNum);
string encryptPlaintext(const string& plaintext, map<char, int> charFreq, map<char, vector<int>> cipherMap);
void writeCiphertextToFile(const string& ciphertext);



int main() {
	map<char, int> charFreq = { { ' ', 19 },{ 'a', 7 },{ 'b', 1 },{ 'c', 2 },{ 'd', 4 },{ 'e', 10 },
	{ 'f', 2 },{ 'g', 2 },{ 'h', 5 },{ 'i', 6 },{ 'j', 1 },{ 'k', 1 },
	{ 'l', 3 },{ 'm', 2 },{ 'n', 6 },{ 'o', 6 },{ 'p', 2 },{ 'q', 1 },
	{ 'r', 5 },{ 's', 5 },{ 't', 7 },{ 'u', 2 },{ 'v', 1 },{ 'w', 2 },
	{ 'x', 1 },{ 'y', 2 },{ 'z', 1 } };

	vector<int> vNum = shuffleNumVector();
	map<char, vector<int>> cipherMap = createCipherMap(charFreq, vNum);
	vector<string> vPlaintexts = getPlaintexts();
	// get random plaintext 
	int rNum = rand() % 5;
	cout << "This is encrypting plaintext " << rNum + 1 << endl; // for testing
	string plaintext = vPlaintexts[rNum];
	string ciphertext = encryptPlaintext(plaintext, charFreq, cipherMap);
	writeCiphertextToFile(ciphertext);

	return 0;
}



vector<string> getPlaintexts() {
	ifstream plainttextFile("test1_candidate_5_plaintexts.txt");
	//Check if file can be opened
	if (!plainttextFile) {
		cerr << "File failed to open\n";
		exit(1);
	}
	// get each plaintext and put it in vector
	vector<string> vPlaintexts;
	string word;
	string line;
	while (plainttextFile >> word) {
		if (word == "Candidate") {
			for (int i = 0; i < 3; ++i) {
				getline(plainttextFile, line);
			}
			vPlaintexts.push_back(line);
		}
	}
	plainttextFile.close();
	return vPlaintexts;
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



string encryptPlaintext(const string& plaintext, map<char, int> charFreq, map<char, vector<int>> cipherMap) {
	string ciphertext;
	int index = 1;
	for (const char& c : plaintext) {
		int freq = charFreq[c];
		ciphertext += to_string(cipherMap[c][rand() % freq]);
		if (index != 500) {
			ciphertext += ",";
		}
		++index;
	}
	return ciphertext;
}



void writeCiphertextToFile(const string& ciphertext) {
	ofstream ciphertextFile;
	ciphertextFile.open("Test1-ciphertext.txt", fstream::out);
	ciphertextFile << ciphertext;
	ciphertextFile.close();
}
