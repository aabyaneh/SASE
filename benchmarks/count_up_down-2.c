/*
  The C* port of count_by_nondet-2.c from github.com/sosy-lab/sv-benchmarks
  done by Alireza Abyaneh
  for any information about the LICENCE see github.com/sosy-lab/sv-benchmarks

  termination : true
  unreach-call: false
*/

void VERIFIER_error() {
  uint64_t x;
  x = 10 / 0;
}

void VERIFIER_assert(uint64_t cond) {
  if (cond == 0) {
    VERIFIER_error();
  }
  return;
}

uint64_t main() {
  uint64_t n;
  uint64_t x;
  uint64_t y;

  n = input(0, 2000, 1); // input(0, -1, 1);
  x = n;
  y = 0;

  while (x > 0) {
    x = x - 1;
    y = y + 1;
  }

  VERIFIER_assert(y != n);
}
