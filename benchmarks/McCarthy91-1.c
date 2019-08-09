/*
  The C* port of McCarthy91-1.c from github.com/sosy-lab/sv-benchmarks
  done by Alireza Abyaneh
  for any information about the LICENCE see github.com/sosy-lab/sv-benchmarks

  termination : true
  unreach-call: false
*/

void VERIFIER_error() {
  uint64_t x;
  x = 10 / 0;
}

uint64_t f91(uint64_t x) {
  if (x > 100)
    return x - 10;
  else
    return f91(f91(x+11));
}

uint64_t main() {
  uint64_t x;
  uint64_t result;

  x = input(0, -1, 1);
  result = f91(x);

  if (result == 91) {
    return 0;
  } else if (x > 102) {
    if (result == x - 10)
      return 0;
    else VERIFIER_error();
  } else {
      VERIFIER_error();
  }
}
