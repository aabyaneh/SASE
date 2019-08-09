/*
  The C* port of array_3-2.c from github.com/sosy-lab/sv-benchmarks
  done by Alireza Abyaneh
  for any information about the LICENCE see github.com/sosy-lab/sv-benchmarks

  termination : true
  unreach-call: false
*/

void  VERIFIER_error() {
  uint64_t x;
  x = 10 / 0;
}

void VERIFIER_assert(uint64_t cond) {
  if (cond == 0) {
    VERIFIER_error();
  }
  return;
}

uint64_t N = 1024;
uint64_t SIZEOFUINT64 = 8;

uint64_t main() {
  uint64_t* A;
  uint64_t  i;

  A = malloc(N * SIZEOFUINT64);

  i = 0;
  while (i < N) {
    *(A + i) = input(0, -1, 1);
    i = i + 1;
  }

  i = 0;
  while (i < N) {
    if (*(A + i) != 0) {
      i = i + 1;
    }
  }

  VERIFIER_assert(i <= N / 2);
}