/*
  The C* port of count_by_nondet.c from github.com/sosy-lab/sv-benchmarks
  done by Alireza Abyaneh
  for any information about the LICENCE see github.com/sosy-lab/sv-benchmarks

  termination : true
  unreach-call: true
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

uint64_t LARGE_INT = 2000;

uint64_t main() {
  uint64_t i;
  uint64_t k;
  uint64_t j;

  i = 0;
  k = 0;

  while(i < LARGE_INT) {
    j = input(0, -1, 1);
    if (1 > j) {
      return 0;
    } else if (j >= LARGE_INT) {
      return 0;
    }

    i = i + j;
    k = k + 1;
  }

  VERIFIER_assert(k <= LARGE_INT);

  return 0;
}