/*
  The C* port of sum01-1.c from github.com/sosy-lab/sv-benchmarks
  done by Alireza Abyaneh
  for any information about the LICENCE see github.com/sosy-lab/sv-benchmarks

  termination : true
  unreach-call: false
*/

void VERIFIER_error() {
  uint64_t x;
  x = 10 / 0;
}

uint64_t a = 2;

uint64_t main() {
  uint64_t i;
  uint64_t n;
  uint64_t sn;

  n = input(0, -1, 1);
  sn = 0;
  i = 1;
  while (i <= n) {
    if (i < 10)
      sn = sn + a;
    i = i + 1;
  }

  if (sn == n*a)
    return 0;
  else if (sn == 0)
    return 0;
  else
    VERIFIER_error();
}
