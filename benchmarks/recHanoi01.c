/*
  The C* port of recHanoi01.c from github.com/sosy-lab/sv-benchmarks
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

uint64_t counter;

/*
 * This function returns the optimal amount of steps,
 * needed to solve the problem for n-disks
 */
uint64_t hanoi(uint64_t n) {
	if (n == 1) {
		return 1;
	}

  return 2 * (hanoi(n-1)) + 1;
}

/*
 * This applies the known algorithm, without executing it (so no arrays).
 * But the amount of steps is counted in a global variable.
 */
void applyHanoi(uint64_t n, uint64_t from, uint64_t to, uint64_t via) {
	if (n == 0) {
		return;
	}

	// increment the number of steps
	counter = counter + 1;

  applyHanoi(n-1, from, via, to);
	applyHanoi(n-1, via, to, from);
}

uint64_t main() {
  uint64_t n;
  uint64_t result;

  n = input(0, -1, 1);
  if (n < 1)
    return 0;
  else if (n > 31)
    return 0;

  counter = 0;
  applyHanoi(n, 1, 3, 2);
  result = hanoi(n);

  // result and the counter should be the same!
  VERIFIER_assert(result == counter);

  return 0;
}