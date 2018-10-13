#include <iostream> 
#include <fstream>
#include <map>
#include <vector>     
#include <string>
#include <cstdlib>
#include <algorithm>
using namespace std;



// global charFreq map
map<char, int> charFreq = { { ' ', 19 },{ 'a', 7 },{ 'b', 1 },{ 'c', 2 },{ 'd', 4 },{ 'e', 10 },
							{ 'f', 2 },{ 'g', 2 },{ 'h', 5 },{ 'i', 6 },{ 'j', 1 },{ 'k', 1 },
							{ 'l', 3 },{ 'm', 2 },{ 'n', 6 },{ 'o', 6 },{ 'p', 2 },{ 'q', 1 },
							{ 'r', 5 },{ 's', 5 },{ 't', 7 },{ 'u', 2 },{ 'v', 1 },{ 'w', 2 },
							{ 'x', 1 },{ 'y', 2 },{ 'z', 1 } };



class EWord {
public:
	//EWord Constructor
	EWord(const string& word) : word(word) {}
	void updateScore(int iScore) { score = iScore; }
	int returnScore() const { return score; }
	const string& returnWord() const { return word; }
	void displayEWord() const { cout << word << " " << score << endl; }
	bool operator<(const EWord& other) { return score < other.score; }
private:
	string word;
	int score = 0;
};



bool compare(EWord* lhs, EWord* rhs) {return lhs->returnScore() < rhs->returnScore();}
vector<EWord*> getEnglishWords();
int calculateScoreOfWord(EWord* wPtr);
void clearEWords(vector<EWord*>& vEWords);
void orderEWordsByScore(vector<EWord*>& vEWords);
void writeDictFile(const vector<string>& vWords);



int main() {
	// get vector with english word and respective score
	vector<EWord*> vEWords = getEnglishWords();
	orderEWordsByScore(vEWords);
	// vector with words sorted by score
	vector<string> vWords;
	for (EWord* wPtr : vEWords) {
		vWords.push_back(wPtr->returnWord());
	}
	clearEWords(vEWords);
	writeDictFile(vWords);
	return 0;
}



vector<EWord*> getEnglishWords() {
	ifstream englishWordsFile("test2_candidate_70_english_words.txt");
	//Check if file can be opened
	if (!englishWordsFile) {
		cerr << "File failed to open\n";
		exit(1);
	}
	// get each plaintext and put it in vector
	vector<EWord*> vEnglishWords;
	string word;
	while (englishWordsFile >> word) {
		EWord* wPtr = new EWord(word);
		vEnglishWords.push_back(wPtr);
		wPtr->updateScore(calculateScoreOfWord(wPtr));
	}
	englishWordsFile.close();
	
	return vEnglishWords;
}



int calculateScoreOfWord(EWord* wPtr) {
	int score = 0;
	for (int i = 0; i < wPtr->returnWord().length(); ++i) {
		score += charFreq[wPtr->returnWord()[i]];
	}
	return score;
}



void orderEWordsByScore(vector<EWord*>& vEWords) {
	sort(vEWords.begin(), vEWords.end(), compare);
}



void clearEWords(vector<EWord*>& vEWords) {
	//Take care of EWord pointers
	for (size_t ind = 0; ind < vEWords.size(); ++ind) {
		delete vEWords[ind];
	}

	//Reset size of vector
	vEWords.clear();
}



void writeDictFile(const vector<string>& vWords) {
	ofstream dictFile;
	dictFile.open("dict.txt", fstream::out);
	for (const string& word : vWords){
		dictFile << word << endl;
	}
	dictFile.close();
}
