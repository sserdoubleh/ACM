#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
using namespace std;

typedef unsigned char byte;
const byte mask[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
#define tget(i) ( (t[(i) >> 3] & mask[(i) & 7]) ? 1 : 0 )
#define tset(i, b) t[(i) >> 3] = (b) ? (mask[(i) & 7]|t[(i) >> 3]) : ((~mask[(i) & 7]) & t[(i) >> 3])
#define chr(i) (cs == sizeof(int) ? ((int*)s)[i] : ((byte *)s)[i])
#define isLMS(i) (i > 0 && tget(i) && !tget(i - 1))

void getBuckets(byte *s, int *bkt, int n, int K, int cs, bool end) {
  int i, sum = 0;
  for (i = 0; i <= K; i++)
    bkt[i] = 0; 
  for (i = 0; i < n; i++)
    bkt[chr(i)]++;
  for (i = 0; i <= K; i++) {
    sum += bkt[i];
    bkt[i] = end ? sum : sum - bkt[i];
  }
}

void induceSAl(byte *t, int *SA, byte *s, int *bkt, int n, int K, int cs, bool end) {
  int i, j;
  getBuckets(s, bkt, n, K, cs, end);  
  for (i = 0; i < n; i++) {
    j = SA[i] - 1;
    if (j >= 0 && !tget(j))
      SA[bkt[chr(j)]++] = j;
  }
}

void induceSAs(byte *t, int *SA, byte *s, int *bkt, int n, int K, int cs, bool end) {
  int i, j;
  getBuckets(s, bkt, n, K, cs, end);  
  for (i = n - 1; i >= 0; i--) {
    j = SA[i] - 1;
    if (j >= 0 && tget(j))
      SA[--bkt[chr(j)]] = j;
  }
}

void issa(byte *s, int *SA, int n, int K, int cs) {
  int i, j;
  byte *t = (byte *) malloc(n / 8 + 1);  

  tset(n - 2, 0);
  tset(n - 1, 1);        
  for (i = n - 3; i >= 0; i--)
    tset(i, (chr(i) < chr(i + 1) || (chr(i) == chr(i + 1) && tget(i + 1) == 1)) ? 1 : 0);

  int *bkt = (int *) malloc(sizeof(int) * (K + 1));  
  getBuckets(s, bkt, n, K, cs, true);  
  for (i = 0; i < n; i++)
    SA[i] = -1;
  for (i = 1; i < n; i++)
    if (isLMS(i))
      SA[--bkt[chr(i)]] = i;
  induceSAl(t, SA, s, bkt, n, K, cs, false);
  induceSAs(t, SA, s, bkt, n, K, cs, true);
  free(bkt);
  
  int n1 = 0;
  for (i = 0; i < n; i++)
    if (isLMS(SA[i]))
      SA[n1++] = SA[i];
  
  for (i = n1; i < n; i++)
    SA[i] = -1;        
  int name = 0, prev = -1;
  for (i = 0; i < n1; i++) {
    int pos = SA[i];
    bool diff = false;
    for (int d = 0; d < n; d++)
      if (prev == -1 || chr(pos + d) != chr(prev + d) || tget(pos + d) != tget(prev + d)) {
        diff = true;
        break;
      } else if (d > 0 && (isLMS(pos + d) || isLMS(prev + d)))
        break;
      if (diff) {
        name++;
        prev = pos;
      }
      pos = (pos % 2 == 0) ? pos / 2 : (pos - 1) / 2;
      SA[n1 + pos] = name - 1;
  }
  for (i = n - 1, j = n - 1; i >= n1; i--)
    if (SA[i] >= 0)
      SA[j--] = SA[i];

  int *SA1 = SA, *s1 = SA + n - n1;
  if (name < n1)
    issa((byte *) s1, SA1, n1, name - 1, sizeof(int));
  else            
    for (i = 0; i < n1; i++)
      SA1[s1[i]] = i;

  bkt = (int *) malloc(sizeof(int) * (K + 1));  

  getBuckets(s, bkt, n, K, cs, true);  
  for (i = 1, j = 0; i < n; i++)
    if (isLMS(i))
      s1[j++] = i;    
  for (i = 0; i < n1; i++)
    SA1[i] = s1[SA1[i]];  
  for (i = n1; i < n; i++)
    SA[i] = -1;        
  for (i = n1 - 1; i >= 0; i--) {
    j = SA[i];

    SA[i] = -1;
    SA[--bkt[chr(j)]] = j;
  }
  induceSAl(t, SA, s, bkt, n, K, cs, false);
  induceSAs(t, SA, s, bkt, n, K, cs, true);
  free(bkt);
  free(t);
}

void get_ht(byte *str, int *sa, int *rk, int *ht, int n) {
  int i, j, k = 0;
  for (i = 1; i < n; i++)
    rk[sa[i]] = i;
  for (i = 0; i < n - 1; ht[rk[i++]] = k)
    for (k ? k-- : 0, j = sa[rk[i] - 1]; str[i + k] == str[j + k]; k++);
}

const int N = 1000010;

int n;
char str[N];
int sa[N], rk[N], ht[N];

int main() {
  scanf("%s", str);
  n = strlen(str);
  str[n++] = 0;

  issa((byte *)str, sa, n, 256, 1);
  get_ht((byte *)str, sa, rk, ht, n);

  for (int i = 1; i < n; i++)
    printf("%2d %s\n", ht[i], str + sa[i]);

  return 0;
}
