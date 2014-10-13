#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

typedef int T;

const int N = 10000 + 10;

T value[N]; 
int n;
int heap[N], pos[N], hn;

void heap_init() {
  hn = 0;
}

int heap_size() {
  return hn;
}

void heap_up(int x) {
  int p;
  while (x && value[heap[x]] < value[heap[p = (x - 1) >> 1]]) {
    swap(heap[x], heap[p]);
    swap(pos[heap[x]], pos[heap[p]]);
    x = p;
  }
}

void heap_down(int x) {
  int c;
  while ((c = (x << 1) + 1) < hn) {
    if (c + 1 < hn && value[heap[c + 1]] < value[heap[c]]) c++;
    if (value[heap[c]] < value[heap[x]]) {
      swap(heap[x], heap[c]);
      swap(pos[heap[x]], pos[heap[c]]);
      x = c;
    } else break;
  }
}

void heap_push(int i) {
  pos[heap[hn] = i] = hn;
  heap_up(hn++);
}

void heap_remove(int i) {
  int x = pos[i];
  pos[heap[x] = heap[--hn]] = x;
  heap_down(x);
}

int heap_top() {
  return heap[0];
}

int heap_pop() {
  int t;
  heap_remove(t = heap[0]);
  return t;
}
