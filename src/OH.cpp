#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdlib>
#include<ctime>
#include<iomanip>
#include<cmath>
#include<string>
#include<fstream>

using namespace std;
int a = 0;
#define MAX_COUNT 2000
#define MAX_LEN 20
#define TEXT_SIZE 1048575

int matching = 0;

#define min(a,b) a<b?a:b
#define max(a,b) a<b?b:a

class Hash_T {
public:
	int* pattern;
	Hash_T* next;
	int p_num;
	Hash_T();
};
Hash_T::Hash_T() {

	this->pattern = NULL;
	this->next = NULL;
	this->p_num = -1;
}

int factorial(int n) {
	return (n == 1 || n == 0) ? 1 : factorial(n - 1)*n;
}
void preprocessing_table(int** p, int* shift, Hash_T** hash, int B_size, int arr_size, int max_len);
void make_shift(int** p, int* shift, int B_size, int arr_size, int max_len, Hash_T** hash);
int find_shortest_pattern(int** p, int arr_size, int max_len);
int* q_gram(int* p, int m, int q);
int find_k(int val, int** arr, int size, int arr_size, int q);

int find_len(int* p, int max_len) {

	int ret = max_len;
	for (int i = 0; i < max_len; i++) {
		if (p[i] < 0 || p[i] == 0) {
			ret = i;
			break;
		}
	}

	return ret;
}
int find_max(int* p, int len) {

	int ret = 0;

	for (int i = 0; i < len; i++) {
		if (p[i] > ret)
			ret = p[i];
	}

	return ret;

}
void merge(int first, int mid, int last, int* arr) {

	int idx = first;
	int temp_arr[MAX_COUNT];


	int i = first, j = mid + 1;

	while (i <= mid && j <= last) {
		if (arr[i] <= arr[j]) {
			temp_arr[idx] = arr[i];
			idx++;
			i++;
		}
		else if (arr[i] > arr[j]) {
			temp_arr[idx] = arr[j];
			idx++;
			j++;
		}
	}

	if (i > mid) {
		for (int m = j; m <= last; m++) {
			temp_arr[idx] = arr[m];
			idx++;
		}
	}
	else {
		for (int m = i; m <= mid; m++) {
			temp_arr[idx] = arr[m];
			idx++;
		}
	}

	for (int m = first; m <= last; m++) {
		arr[m] = temp_arr[m];
	}

}
void mergeSort(int first, int last, int* arr) {

	if (first < last) {
		int mid = (first + last) / 2;
		mergeSort(first, mid, arr);
		mergeSort(mid + 1, last, arr);
		merge(first, mid, last, arr);
	}

}
void make_phi(int* temp_p, int* p, int* phi, int len);
void make_phi_inv(int* phi, int* phi_inv, int len);
void make_E(int* p, int* phi_inv, int* E, int len);
void preprocessing_phi(int** p, int** phi, int** phi_inv, int** E, int arr_size, int max_len);

int finger_printing(int* p, int s, int m, int q);
bool Check_OP(int* T, int* P, int s, int len, int* phi_inv, int* E);
void Search(int* Text, int** p, int* shift, Hash_T** hash, int** phi_inv, int** E, int arr_size, int max_len, int q);
int cal_table_size(int n) {

	int ret = 0;
	for (int i = 0; i < n; i++) {
		ret += i * factorial(i);
	}

	return ret;
}
int cal_q(int* p, int m, int q, int s) {

	int count;
	int result = 0;
	for (int j = s; j < s + q; j++) {
		count = 0;
		for (int k = s; k < j; k++) {
			if (p[k] <= p[j])
				count++;
		}
		result += count * factorial(j - s);
	}

	return result;
}

int main(int argc, char* argv[]) {

	a = 0;
	int** PATTERN_SET;
	int** phi;
	int** phi_inv;
	int** E;
	int* shift_table;
	Hash_T** hash;
	int table_size;
	int* TEXT;

	clock_t pre_stime, pre_etime;
	clock_t search_stime, search_etime;
	clock_t total_stime, total_etime;

	// Calculated Table Size
	int TABLE_SIZE[10] = { 0, 0, 0, 6, 24, 120, 720, 5040, 40320, 362880 }; // Q : 3 ~ 9

	// Set Files Name and Folder Name
	string TC_FOLDER = "./TESTCASE/";
	string TEXT_FILE = "POWER.txt";
	string PATTERN_FILE = "PATTERN";

	int BLOCK_SIZE = atoi(argv[1]);
	int HOW = atoi(argv[2]);
	// int half = atoi(argv[3]);
	// int PATTERN_LEN = half;
	clock_t pre, src, total;
	pre = src = total = 0.0;

	clock_t s, e, ts, te;
	// PATTERN_COUNT : 패턴 개수 ( k )
	// PATTERN_LEN : 패턴 길이 ( m )
	for (int PATTERN_COUNT = 200; PATTERN_COUNT <= MAX_COUNT; PATTERN_COUNT += 200) {
		for (int PATTERN_LEN = 8; PATTERN_LEN <= MAX_LEN; /*PATTERN_LEN += 20*/) {

			pre = src = total = 0.0;
			// Read Pattern Information - 패턴개수와 패턴길이에 맞게 패턴 파일 읽음
			string pattern_filename = TC_FOLDER + PATTERN_FILE + "_" + to_string(PATTERN_COUNT) + "_" + to_string(PATTERN_LEN) + ".txt";
			ifstream pattern(pattern_filename);
			PATTERN_SET = new int*[PATTERN_COUNT];
			for (int i = 0; i < PATTERN_COUNT; i++) {
				PATTERN_SET[i] = new int[PATTERN_LEN];
			}
			for (int i = 0; i < PATTERN_COUNT; i++) {
				for (int j = 0; j < PATTERN_LEN; j++) {
					pattern >> PATTERN_SET[i][j];
				}
			}
			pattern.close();

			// Read Text Information - 텍스트 파일 읽음
			string text_filename = TC_FOLDER + TEXT_FILE;
			ifstream text(text_filename);
			TEXT = new int[TEXT_SIZE];
			for (int i = 0; i < TEXT_SIZE; i++) {
				text >> TEXT[i];
			}
			text.close();

			/****************************************/
			for (int _HOW = 0; _HOW < HOW; _HOW++) {
				table_size = TABLE_SIZE[BLOCK_SIZE];
				shift_table = new int[table_size];
				hash = new Hash_T *[table_size];
				for (int i = 0; i < table_size; i++)
					hash[i] = NULL;

				ts = clock();
				phi = new int *[PATTERN_COUNT];
				phi_inv = new int *[PATTERN_COUNT];
				E = new int *[PATTERN_COUNT];
				for (int i = 0; i < PATTERN_COUNT; i++)
				{
					phi[i] = new int[PATTERN_LEN];
					phi_inv[i] = new int[PATTERN_LEN];
					E[i] = new int[PATTERN_LEN];
				}

				s = clock();
				// PatternSet을 전처리하여 순위동형을 확인하는데 사용되는 phi_inverse, E 계산
				preprocessing_phi(PATTERN_SET, phi, phi_inv, E, PATTERN_COUNT, PATTERN_LEN);

				// PatternSet을 이용하여 Shift Table 생성
				preprocessing_table(PATTERN_SET, shift_table, hash, BLOCK_SIZE, PATTERN_COUNT, PATTERN_LEN);
				e = clock();
				pre += (float)(e - s) / CLOCKS_PER_SEC * 1000;

				s = clock();
				// 생성된 shift table 과 hash table을 이용하여, search를 진행함
				Search(TEXT, PATTERN_SET, shift_table, hash, phi_inv, E, PATTERN_COUNT, PATTERN_LEN, BLOCK_SIZE);
				e = clock();
				src += (float)(e - s) / CLOCKS_PER_SEC * 1000;
				te = clock();
				total += (float)(te - ts) / CLOCKS_PER_SEC * 1000;
				delete[] hash;
				delete[] shift_table;
				for (int i = 0; i < PATTERN_COUNT; i++)
				{
					delete[] phi[i];
					delete[] phi_inv[i];
					delete[] E[i];
				}
				delete[] phi;
				delete[] phi_inv;
				delete[] E;
			}

			printf("%3.2f, %3.2f, %3.2f\n", (float)pre / HOW, (float)src / HOW, (float)total / HOW);
			for (int i = 0; i < PATTERN_COUNT; i++)
			{
				delete[] PATTERN_SET[i];
			}
			delete[] TEXT;
			delete[] PATTERN_SET;

			if (PATTERN_LEN == 15) PATTERN_LEN += 5;
			else PATTERN_LEN++;
		}
		cout << endl;
	}
	cout << a << endl;
	cout << endl;
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
void preprocessing_table(int** p, int* shift, Hash_T** hash, int B_size, int arr_size, int max_len) {

	make_shift(p, shift, B_size, arr_size, max_len, hash);

}
void make_shift(int** p, int* shift, int B_size, int arr_size, int max_len, Hash_T** hash) {

	int m = find_shortest_pattern(p, arr_size, max_len); // 가장 짧은 패턴 찾음
	int range = m - B_size + 1;

	for (int i = 0; i < cal_table_size(B_size) + 1; i++) {
		shift[i] = range; // shift table 초기화
	}

	int x, k;
	int* y = new int[arr_size]; // 패턴의 마지막 q그램의 값들을 저장하는 임시 배열 <- Hash Table 계산할때 사용
	for (int i = 0; i < arr_size; i++) {
		for (int j = 0; j < range; j++) {
			// 각 패턴의 처음부터 m - q + 1 까지의 q그램을 전부 계산하며, shift table의 값을 갱신
			x = cal_q(p[i], m, B_size, j);
			if (j == range - 1)
				y[i] = x;
			shift[x] = min(shift[x], m - (B_size + j));
		}
	}

	// 패턴의 마지막 q그램 값을 사용하여 Hash Table 생성
	// 같은 q그램 값을 가지는것들은 pointer로 연결
	int idx;
	Hash_T* temp_ptr = NULL;
	for (int i = 0; i < arr_size; i++) {
		idx = y[i];
		temp_ptr = hash[idx];
		if (temp_ptr != NULL) {
			while (temp_ptr->next != NULL) {
				temp_ptr = temp_ptr->next;
			}
			temp_ptr->next = new Hash_T();
			temp_ptr->next->pattern = p[i];
			temp_ptr->next->p_num = i;
		}
		else {
			temp_ptr = new Hash_T();
			temp_ptr->pattern = p[i];
			temp_ptr->p_num = i;
			hash[idx] = temp_ptr;
		}
	}
	delete[] y;
}
int find_shortest_pattern(int** p, int arr_size, int max_len) {

	int ret = max_len;

	for (int i = 0; i < arr_size; i++) {
		for (int j = 0; j < max_len; j++) {
			if (p[i][j] == 0) {
				if (j < ret) {
					ret = j;
					break;
				}
				else
					break;
			}
		}
	}

	return ret;
}
int* q_gram(int* p, int m, int q) {

	int size = m - q + 1;
	int* ret = new int[size];
	int result;
	int count;

	for (int i = 0; i < size; i++) {


		result = 0;

		for (int j = i; j < i + q; j++) {
			count = 0;
			for (int k = i; k < j; k++) {
				if (p[k] <= p[j])
					count++;
			}
			result += count * factorial(j - i);
		}

		ret[i] = result;
	}
	return ret;
}
int find_k(int val, int** arr, int size, int arr_size, int q) {

	int ret = 0;

	for (int i = 0; i < arr_size; i++) {
		for (int j = 0; j < size; j++) {
			int temp = j;
			if (val == arr[i][j]) {
				if (ret < temp + q)
					ret = temp + q;
			}
		}
	}

	return ret;
}
/////////////////////////////////////////////////////////////////
void make_phi(int* temp_p, int* p, int* phi, int len) {

	int max_val = find_max(p, len);
	int* flag = new int[max_val + 1];
	for (int i = 0; i <= max_val; i++) {
		flag[i] = 0;
	}

	int temp;

	for (int i = 0; i < len; i++) {
		temp = p[i];
		for (int j = flag[temp]; j < len; j++) {
			if (temp_p[j] == temp) {
				flag[temp] = j + 1;
				phi[i] = j;
				break;
			}
		}
	}

	delete[]flag;

}
void make_phi_inv(int* phi, int* phi_inv, int len) {

	for (int i = 0; i < len; i++) {
		phi_inv[phi[i]] = i;
	}

}
void make_E(int* p, int* phi_inv, int* E, int len) {

	for (int i = 0; i < len - 1; i++) {
		if (p[phi_inv[i]] == p[phi_inv[i + 1]])
			E[i] = 1;
		else
			E[i] = 0;
	}

}
void preprocessing_phi(int** p, int** phi, int** phi_inv, int** E, int arr_size, int max_len) {

	int len;
	int* temp_arr;
	for (int i = 0; i < arr_size; i++) {

		len = find_len(p[i], max_len);
		temp_arr = new int[len];

		for (int j = 0; j < len; j++) {
			temp_arr[j] = p[i][j];
		}
		mergeSort(0, len - 1, temp_arr);

		make_phi(temp_arr, p[i], phi[i], len);

		make_phi_inv(phi[i], phi_inv[i], len);

		make_E(p[i], phi_inv[i], E[i], len);

		delete[]temp_arr;
	}

}
////////////////////////////////////////////////////////////////
int finger_printing(int* p, int s, int m, int q) {

	int ret = 0;
	int count;
	for (int j = s; j < s + q; j++) {
		count = 0;
		for (int k = s; k < j; k++) {
			if (p[k] <= p[j])
				count++;
		}
		ret += count * factorial(j - s);
	}

	return ret;
}
bool Check_OP(int* T, int* P, int s, int len, int* phi_inv, int* E) {

	bool ret = true;

	for (int i = 0; i < len - 1; i++) {

		if (E[i] == 0) {
			if (T[s + phi_inv[i]] >= T[s + phi_inv[i + 1]]) {
				ret = false;
				break;
			}
		}
		else {
			if (T[s + phi_inv[i]] != T[s + phi_inv[i + 1]]) {
				ret = false;
				break;
			}
		}
	}
	return ret;
}
void Search(int* Text, int** p, int* shift, Hash_T** hash, int** phi_inv, int** E, int arr_size, int max_len, int q) {

	int n = TEXT_SIZE;
	int m = max_len;
	int j = m;

	while (j <= n) {

		int x = finger_printing(Text, j - q, m, q);
		int l = shift[x];

		// shift table의 값이 0 이면, 후보 패턴이 있다는 의미이므로 실제 일치하는지 확인을 진행
		if (l == 0) {
			matching++;
			Hash_T* temp_Hash = hash[x];
			int* P;
			P = temp_Hash->pattern;
			while (P != NULL) {
				//P = hash[x].pattern;
				int p_num = temp_Hash->p_num;
				int P_len = max_len;
				if (Check_OP(Text, P, j - m, P_len, phi_inv[p_num], E[p_num])) {
					a++;
				}
				if (temp_Hash->next != NULL) {
					P = temp_Hash->next->pattern;
					temp_Hash = temp_Hash->next;
				}
				else
					P = NULL;
				l = 1;
			}
		}
		j = j + l;

	}

}
