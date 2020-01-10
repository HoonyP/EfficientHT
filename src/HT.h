#include <vector>
#include <iostream>
#include <unordered_map>
#include <time.h>
#include <list>

using namespace std;

class Node {
public:
    int Key;
    int Value;
    Node(int key, int val) : Key(key), Value(val) {}
};
class Linear_HashTable {
public:
    Linear_HashTable(int size) {
        table.assign(size, NULL);
        current_sz = 0;
        insert_collision = get_collision = 0;
    }
    bool isexist(const int index) {
        return table[index] != NULL ? true : false;
    }
    void insert(const int key, const int val) {
        int _key = hash(key);
        while (isexist(_key)) {
            _key++;
            insert_collision++;
            if (_key == table.size()) _key = 0;
        }
        table[_key] = new Node(key, val);
        current_sz++;
    }
    int get_hash(const int key) {
        int _key = hash(key);
        while (isexist(_key)) {
            if (table[_key]->Key == key) {
                break;
            }
            _key++;
            get_collision++;
            if (_key == table.size()) _key = 0;
        }
        return table[_key]->Value;
    }
    int hash(int K) {
        return K % prime;
    }
    void setprime(const int p) {
        prime = p;
    }
    int getsize() {
        return table.size();
    }
    void show_table() {
        for (int i = 0; i < table.size(); i++) {
            if (table[i] != NULL) {
                cout << i << " // " << table[i]->Key << " : " << table[i]->Value << endl;
            }
        }
    }
    void show_result_param() {
        cout << "Insert Collision : " << insert_collision << endl;
        cout << "Get Collision : " << get_collision << endl;
    }
private:
    vector< Node* > table; // Key : index
    int current_sz;
    int prime;
    int insert_collision, get_collision;
};


class Double_HashTable {
public:
    Double_HashTable(int size = 1) {
        table.assign(2 * size, NULL);
        current_sz = 0;
        get_collision = insert_collision = 0;
    }
    bool isexist(const int index) {
        return table[index] != NULL ? true : false;
    }
    void insert(const int key, const int val) {
        int _key = hash(key);
        int origin_key = _key;
        int jump = hash2(key);
        int i = 1;
        while (isexist(_key)) {
            _key = hash(origin_key + i++ * jump);
            insert_collision++;
            if (_key >= table.size()) _key %= table.size();
        }
        table[_key] = new Node(key, val);
        current_sz++;
    }
    int get_hash(const int key) {
        int _key = hash(key);
        int jump = hash2(key);
        int i = 1;
        while (isexist(_key)) {
            if (table[_key]->Key == key) {
                break;
            }
            _key = hash(_key + i++ * jump);
            get_collision++;
            if (_key >= table.size()) _key %= table.size();
        }
        return table[_key]->Value;
    }
    int hash(int K) {
        return K % prime1;
    }
    int hash2(int K) {
        return prime2 - (K % prime2);
    }
    void setprime(const int p1, const int p2) {
        prime1 = p1;
        prime2 = p2;
    }
    int getsize() {
        return table.size();
    }
private:
    vector< Node* > table; // Key : index
    int current_sz;
    int prime1, prime2;
    int insert_collision, get_collision;
};


class Chaining_HashTable {
public:
    Chaining_HashTable(int size) {
        table.resize(size);
        prime = size;
        current_sz = 0;
        insert_collision = get_collision = 0;
    }
    void insert(const int key, const int val) {
        int _key = hash(key);
        Node ins(key, val);
        table[_key].push_back(ins);
        current_sz++;
    }
    int get_hash(const int key) {
        int _key = hash(key);
        list<Node>::iterator iter;
        for (iter = table[_key].begin(); iter != table[_key].end(); ++iter) {
            if (iter->Key == key)
                return iter->Value;
        }
    }
    int hash(int K) {
        return K % prime;
    }
    int getsize() {
        return table.size();
    }
private:
    vector< list<Node> > table; // Key : index
    int current_sz;
    int prime;
    int insert_collision, get_collision;
};