/*
  The C* port of phases_2-2.c from github.com/sosy-lab/sv-benchmarks
  done by Alireza Abyaneh
  for any information about the LICENCE see github.com/sosy-lab/sv-benchmarks

  termination : false
  unreach-call: true
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
  uint64_t x;
  uint64_t y;

  x = 2;
  y = input(0, 4000, 1); // input(0, -1, 1);

  if (y <= 0) return 0;

  while (x < y) {
    if (x < y / x) {
      x = x * x;
    } else {
      x = x + 1;
    }
  }

  VERIFIER_assert(x == y);
}
