/*
  The C* port of EvenOdd01-1.c from github.com/sosy-lab/sv-benchmarks
  done by Alireza Abyaneh
  for any information about the LICENCE see github.com/sosy-lab/sv-benchmarks

  no-overflow : true
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

uint64_t is_odd(uint64_t n);
uint64_t is_even(uint64_t n);

uint64_t is_odd(uint64_t n) {
  if (n == 0) {
    return 0;
  } else if (n == 1) {
    return 1;
  } else {
    return is_even(n - 1);
  }
}

uint64_t is_even(uint64_t n) {
  if (n == 0) {
    return 1;
  } else if (n == 1) {
    return 0;
  } else {
    return is_odd(n - 1);
  }
}

uint64_t main() {
  uint64_t n;
  uint64_t result;
  uint64_t mod;

  n = input(0, 2000, 1); // input(0, -1, 1);

  if (n < 0) {
    return 0;
  }

  result = is_odd(n);
  mod    = n % 2;

  VERIFIER_assert(result == mod);
}
