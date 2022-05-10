#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

// upper bound on hashtable size
const int MAXN = 105;
// actual used hashtable size
int m;

// number of tables
const int VER = 2;

int hashtable[VER][MAXN];

// possible positions for a key
int pos[VER];

/* function to fill hash table with dummy value 
* dummy value: -1
* number of hashtables: ver */
void initTable()
{
    memset(hashtable, -1, sizeof(hashtable));
}

/* return hashed value for a key 
* function: ID of hash function
* key: item to be hashed */
int myhash(int function, int key, int n)
{
    switch (function)
    {
    case 0:
        return key % m;
    case 1:
        return (key / m) % m;
    }
}

void place(int key, int tableID, int n)
{
    int nxt = key;
    if (hashtable[0][myhash(0, key, n)] == key ||
        hashtable[1][myhash(1, key, n)] == key)
        return;
    do
    {
        int &hash_result = hashtable[tableID][myhash(tableID, nxt, n)];
        swap(nxt, hash_result);
        tableID ^= 1;
    } while (nxt != -1 && !(nxt == key && tableID == 0));
    if (nxt != -1)
        puts("Failed!"), exit(0);
}

/* function to print hash table contents */
void printTable(int n)
{
    for (int i = 0; i < VER; i++, std::cout << endl)
        for (int j = 0; j < m; j++)
            (hashtable[i][j] == -1) ? std::cout << "- " : std::cout << hashtable[i][j] << " ";
}

/* function for Cuckoo-hashing keys 
* keys[]: input array of keys 
* n: size of input array */
void cuckoo(int keys[], int n)
{
    //init
    initTable();

    // start with placing every key at its position in
    // the first hash table according to first hash
    // function
    for (int i = 0; i < n; i++)
        place(keys[i], 0, n);

    //print the final hash tables
    printTable(n);
}

int main()
{
    // freopen("in", "r", stdin);
    int keys[MAXN];
    //input array size
    int n;
    std::cin >> n;
    //m in hash function
    m = n + 1;
    int i = 0;
    while (i < n)
    {
        std::cin >> keys[i++];
    }
    cuckoo(keys, n);

    return 0;
}
