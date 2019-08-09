/*
  The C* port of id2_b5_o10.c from github.com/sosy-lab/sv-benchmarks
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

uint64_t id(uint64_t x);
uint64_t id2(uint64_t x);

uint64_t id(uint64_t x) {
  uint64_t ret;

  if (x == 0) return 0;

  ret = id2(x-1) + 1;

  if (ret > 5) return 5;

  return ret;
}

uint64_t id2(uint64_t x) {
  uint64_t ret;

  if (x==0) return 0;

  ret = id(x-1) + 1;

  if (ret > 5) return 5;

  return ret;
}

uint64_t main() {
  uint64_t input;
  uint64_t result;

  input  = input(0, 2500, 1); // input(0, -1, 1);
  result = id(input);

  if (result == 10) {
    VERIFIER_error();
  }
}
