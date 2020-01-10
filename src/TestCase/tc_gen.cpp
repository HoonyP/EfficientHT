#include<iostream>
#include<fstream>
#include<random>
#include<string>
#include<ctime>

using namespace std;

int getRandomNumber(int min, int max) {
	random_device rn;
	mt19937_64 rnd(rn());
	uniform_int_distribution<int> range(min, max);
	return range(rnd);
}

void Read_and_WriteData(string text, int len) {
	string TC_FOLDER = "./TESTCASE/";
	string PATTERN_FILE = "PATTERN";

	// Read From Random Data
	ifstream inFile(TC_FOLDER + text + ".txt");
	int* TEXT = new int[len];
	for (int i = 0; i < len; i++) {
		inFile >> TEXT[i];
	}
	inFile.close();

	for (int i = 200; i <= 2000; i += 200) {
		for (int j = 8; j <= 20;) {
			string pattern_filename = PATTERN_FILE
				+ "_" + to_string(i)
				+ "_" + to_string(j)
				+ ".txt";
			ofstream outP(TC_FOLDER + "/" + text + "/" + pattern_filename);

			for (int k = 0; k < i; k++) {
				int s = getRandomNumber(0,5000);
				int lens = j;
				for (int l = 0; l < lens; l++) {
					if (l == lens - 1)
						outP << TEXT[s + l] << endl;
					else
						outP << TEXT[s + l] << " ";
				}
			}
			if (j == 15) j += 5;
			else j++;

			outP.close();
		}
	}
	delete[] TEXT;
}

int main() {

	string TEXT_FILE[3] = {"DJA","TEXT","POWER"};
	int TEXT_LEN[3] = {34685, 1000000,1000000};
	for (int i = 0; i < 3; i++) {
		Read_and_WriteData(TEXT_FILE[i], TEXT_LEN[i]);
	}

	return 0;
}