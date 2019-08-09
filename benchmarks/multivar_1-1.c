/*
  The C* port of multivar_1-1.c from github.com/sosy-lab/sv-benchmarks
  done by Alireza Abyaneh
  for any information about the LICENCE see github.com/sosy-lab/sv-benchmarks

  termination : true
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

  x = input(0, -1, 1);
  y = x;

  while (x < 1024) {
    x = x + 1;
    y = y + 1;
  }

  VERIFIER_assert(x == y);
}
