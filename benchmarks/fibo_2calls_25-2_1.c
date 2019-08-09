/*
  The C* port of fibo_2calls_25-2.c from github.com/sosy-lab/sv-benchmarks
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

uint64_t fibo1(uint64_t n);
uint64_t fibo2(uint64_t n);

uint64_t fibo1(uint64_t n) {
  if (n < 1) {
    return 0;
  } else if (n == 1) {
    return 1;
  } else {
    return fibo2(n-1) + fibo2(n-2);
  }
}

uint64_t fibo2(uint64_t n) {
  if (n < 1) {
    return 0;
  } else if (n == 1) {
    return 1;
  } else {
    return fibo1(n-1) + fibo1(n-2);
  }
}

// fibo 1-30
// 1, 1, 2, 3, 5,
// 8, 13, 21, 34, 55,
// 89, 144, 233, 377, 610,
// 987, 1597, 2584, 4181, 6765,
// 10946, 17711, 28657, 46368, 75025,
// 121393, 196418, 317811, 514229, 832040

uint64_t main() {
  uint64_t x;
  uint64_t result;

  x = input(0, 25, 1);
  result = fibo1(x);

  if (x < 15) {
    return 0;
  } else if (result > 610) {
    return 0;
  } else if (x != 15) {
    VERIFIER_error();
  }

  return 0;
}
