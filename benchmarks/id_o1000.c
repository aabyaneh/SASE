/*
  The C* port of id_o1000.c from github.com/sosy-lab/sv-benchmarks
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

uint64_t id(uint64_t x) {
  if (x == 0) return 0;
  return id(x-1) + 1;
}

uint64_t main() {
  uint64_t input;
  uint64_t result;

  input = input(0, -1, 1);
  result = id(input);

  if (result == 1000) {
    VERIFIER_error();
  }
}
