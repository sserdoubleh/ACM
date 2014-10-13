#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

const int N = 1000010;

int kmp(char *s, int n, char *t, int m) {
  static int next[N];
  int i, j;
  
  next[0] = -1;
  i = 0; j = -1;
  while (i < m) {
    if (j == -1 || t[i] == t[j]) {
      i++; j++;
      next[i] = (t[i] == t[j] ? next[j] : j);
    } else {
      j = next[j];
    }
  }
  
  i = j = 0;
  while (i < n && j < m) {
    if (j == -1 || s[i] == t[j]) {
      i++; j++;
    } else {
      j = next[j];
    }
  }
  return (j >= m ? i - m : -1);
}

void Z_algo(const char *s, int n, const char *t, int m, int ext[]) {
  static int next[N];
  int i, j, k;

  next[0] = m;
  for (next[1] = 0; 1 + next[1] < m && t[next[1]] == t[1 + next[1]]; ++next[1]);
  for (i = 2, k = 1; i < m; ++i)
    if (next[i - k] < k + next[k] - i) next[i] = next[i - k];
    else {
      j = max(k + next[k] - i, 0);
      while (i + j < m && t[i + j] == t[j]) ++j;
      next[k = i] = j;
    }

  for (ext[0] = 0; ext[0] < n && ext[0] < m && s[ext[0]] == t[ext[0]]; ++ext[0]);
  for (i = 1, k = 0; i < n; ++i)
    if (next[i - k] < k + ext[k] - i) ext[i] = next[i - k];
    else {
      j = max(k + ext[k] - i, 0);
      while (i + j < n && j < m && s[i + j] == t[j]) ++j;
      ext[k = i] = j;
    }
}
