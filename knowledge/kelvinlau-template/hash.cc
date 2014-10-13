#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

const int N = 1000;
const int L = 11;
const int HZ = 1999;

int next_id;
char name[N][L];
int hash_key[HZ];
int hash_id[HZ];

int elfhash(char *str) {
  int result = 0, g;
  while (*str) {
    result = (result << 4) + ((*str++) - 97 + 1);
    g = result & 0xf0000000;
    if (g) result ^= g>>24;
    result &= ~g;
  }
  return result;
}

int id(char *str) {
  int key = elfhash(str);
  int t = key % HZ;
  int k = 1;
  while (1) {
    if (hash_key[t] == -1) {
      hash_key[t] = key;
      hash_id[t] = next_id;
      strcpy(name[next_id], str);
      return next_id++;
    }
    if (hash_key[t] == key && !strcmp(name[hash_id[t]], str)) {
      return hash_id[t];
    }
    t = (t + k) % HZ;
    k += 2;
  }
}

void init() {
  next_id = 0;
  memset(hash_key, -1, sizeof(hash_key));
}
