/*
  The C* port of half.c from github.com/sosy-lab/sv-benchmarks
  done by Alireza Abyaneh
  for any information about the LICENCE see github.com/sosy-lab/sv-benchmarks

  termination : true
  unreach-call: true
*/

uint64_t LARGE_INT = 2000;

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

uint64_t  INT64_MAX;
uint64_t  INT64_MIN;
uint64_t  SIZEOFUINT64 = 8;
uint64_t* power_of_two_table;

void init_library() {
  uint64_t i;

  power_of_two_table = malloc(64 * SIZEOFUINT64);

  *power_of_two_table = 1; // 2^0 == 1

  i = 1;
  while (i < 64) {
    *(power_of_two_table + i) = *(power_of_two_table + (i - 1)) * 2;

    i = i + 1;
  }
}

uint64_t two_to_the_power_of(uint64_t p) {
  return *(power_of_two_table + p);
}

uint64_t signed_less_than(uint64_t a, uint64_t b) {
  return a + INT64_MIN < b + INT64_MIN;
}

uint64_t main() {
  uint64_t i;
  uint64_t n;
  uint64_t k;

  init_library();
  INT64_MAX = two_to_the_power_of(64 - 1) - 1;
  INT64_MIN = INT64_MAX + 1;

  n = 0;
  k = input(0, -1, 1);

  if (k > LARGE_INT) {
    return 0;
  } else if ( signed_less_than(k, -LARGE_INT) ) {
    return 0;
  }

  i = 0;
  while ( signed_less_than(i, 2 * k) ) {
    if (i % 2 == 0)
      n = n + 1;
    i = i + 1;
  }

  if (k < 0) {
    VERIFIER_assert(n == 0);
    return 0;
  } else if (n == k) {
    return 0;
  } else {
    VERIFIER_error();
  }

  return 0;
}
