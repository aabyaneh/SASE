/*
  The C* port of Fibonacci05.c from github.com/sosy-lab/sv-benchmarks
  done by Alireza Abyaneh
  for any information about the LICENCE see github.com/sosy-lab/sv-benchmarks

  termination : true
  unreach-call: false
*/

void VERIFIER_error() {
  uint64_t x;
  x = 10 / 0;
}

uint64_t fibonacci(uint64_t n) {
  if (n < 1) {
    return 0;
  } else if (n == 1) {
    return 1;
  } else {
    return fibonacci(n-1) + fibonacci(n-2);
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

  x = input(0, -1, 1);

  if (x > 25)
    return 0;

  result = fibonacci(x);
  if (x < 19) {
    return 0;
  } else if (result >= 6765) {
    return 0;
  } else {
    VERIFIER_error();
  }

  return 0;
}
