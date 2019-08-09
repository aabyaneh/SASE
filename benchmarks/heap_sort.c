/*
  This file is a C* translation of the original implementation
  done by Alireza Abyaneh.
*/

uint64_t* malloc(uint64_t size);

void swap(uint64_t* op1, uint64_t* op2) {
  uint64_t temp;

  temp = *op1;
  *op1 = *op2;
  *op2 = temp;
}

void heapify(uint64_t* arr, uint64_t n, uint64_t i) {
  uint64_t largest;
  uint64_t l;
  uint64_t r;

  largest = i;
  l = 2*i + 1;
  r = 2*i + 2;

  if (l < n)
    if (*(arr + l) > *(arr + largest))
      largest = l;

  if (r < n)
    if (*(arr + r) > *(arr + largest))
      largest = r;

  if (largest != i) {
    swap((arr + i), (arr + largest));

    heapify(arr, n, largest);
  }
}

void heap_sort(uint64_t* arr, uint64_t n) {
  uint64_t i;

  i = n / 2;
  while (i > 0) {
    heapify(arr, n, i - 1);

    i = i - 1;
  }

  i = n;
  while (i > 0) {
    swap(arr, (arr + (i-1)));

    heapify(arr, i-1, 0);

    i = i - 1;
  }
}

uint64_t main(uint64_t argc, uint64_t* argv) {
  uint64_t v1;
  uint64_t cnt;
  uint64_t* arr;

  cnt = 250;
  arr = malloc(cnt * 8);

  v1 = 0;
  while (v1 < cnt) {
    if (v1 != cnt/2)
      *(arr + v1) = cnt - v1;
    v1 = v1 + 1;
  }

  *(arr + cnt/2) = input(0, 2*cnt-1, 1);

  heap_sort(arr, cnt);

  return 0;
}