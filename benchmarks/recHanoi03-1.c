/*
  The C* port of recHanoi03-1.c from github.com/sosy-lab/sv-benchmarks
  done by Alireza Abyaneh
  for any information about the LICENCE see github.com/sosy-lab/sv-benchmarks

  termination : true
  unreach-call: true
*/

void VERIFIER_error() {
  uint64_t x;
  x = 10 / 0;
}

uint64_t  SIZEOFUINT64 = 8;
uint64_t* power_of_two_table;

void init_library() {
  uint64_t i;

  power_of_two_table = malloc(64 * SIZEOFUINT64);

  *power_of_two_table = 1;

  i = 1;
  while (i < 64) {
    *(power_of_two_table + i) = *(power_of_two_table + (i - 1)) * 2;

    i = i + 1;
  }
}

uint64_t two_to_the_power_of(uint64_t p) {
  if (p == 0)
    return *(power_of_two_table + 0);
  else if (p == 1)
    return *(power_of_two_table + 1);
  else if (p == 2)
    return *(power_of_two_table + 2);
  else if (p == 3)
    return *(power_of_two_table + 3);
  else if (p == 4)
    return *(power_of_two_table + 4);
  else if (p == 5)
    return *(power_of_two_table + 5);
  else if (p == 6)
    return *(power_of_two_table + 6);
  else if (p == 7)
    return *(power_of_two_table + 7);
  else if (p == 8)
    return *(power_of_two_table + 8);
  else if (p == 9)
    return *(power_of_two_table + 9);
  else if (p == 10)
    return *(power_of_two_table + 10);
  else if (p == 11)
    return *(power_of_two_table + 11);
  else if (p == 12)
    return *(power_of_two_table + 12);
  else if (p == 13)
    return *(power_of_two_table + 13);
  else if (p == 14)
    return *(power_of_two_table + 14);
  else if (p == 15)
    return *(power_of_two_table + 15);
  else if (p == 16)
    return *(power_of_two_table + 16);
  else if (p == 17)
    return *(power_of_two_table + 17);
  else if (p == 18)
    return *(power_of_two_table + 18);
  else if (p == 19)
    return *(power_of_two_table + 19);
  else if (p == 20)
    return *(power_of_two_table + 20);
  else if (p == 21)
    return *(power_of_two_table + 21);
  else if (p == 22)
    return *(power_of_two_table + 22);
  else if (p == 23)
    return *(power_of_two_table + 23);
  else if (p == 24)
    return *(power_of_two_table + 24);
  else if (p == 25)
    return *(power_of_two_table + 25);
  else if (p == 26)
    return *(power_of_two_table + 26);
  else if (p == 27)
    return *(power_of_two_table + 27);
  else if (p == 28)
    return *(power_of_two_table + 28);
  else if (p == 29)
    return *(power_of_two_table + 29);
  else if (p == 30)
    return *(power_of_two_table + 30);
  else if (p == 31)
    return *(power_of_two_table + 31);
  else if (p == 32)
    return *(power_of_two_table + 32);
  else if (p == 33)
    return *(power_of_two_table + 33);
  else if (p == 34)
    return *(power_of_two_table + 34);
  else if (p == 35)
    return *(power_of_two_table + 35);
  else if (p == 36)
    return *(power_of_two_table + 36);
  else if (p == 37)
    return *(power_of_two_table + 37);
  else if (p == 38)
    return *(power_of_two_table + 38);
  else if (p == 39)
    return *(power_of_two_table + 39);
  else if (p == 40)
    return *(power_of_two_table + 40);
  else if (p == 41)
    return *(power_of_two_table + 41);
  else if (p == 42)
    return *(power_of_two_table + 42);
  else if (p == 43)
    return *(power_of_two_table + 43);
  else if (p == 44)
    return *(power_of_two_table + 44);
  else if (p == 45)
    return *(power_of_two_table + 45);
  else if (p == 46)
    return *(power_of_two_table + 46);
  else if (p == 47)
    return *(power_of_two_table + 47);
  else if (p == 48)
    return *(power_of_two_table + 48);
  else if (p == 49)
    return *(power_of_two_table + 49);
  else if (p == 50)
    return *(power_of_two_table + 50);
  else if (p == 51)
    return *(power_of_two_table + 51);
  else if (p == 52)
    return *(power_of_two_table + 52);
  else if (p == 53)
    return *(power_of_two_table + 53);
  else if (p == 54)
    return *(power_of_two_table + 54);
  else if (p == 55)
    return *(power_of_two_table + 55);
  else if (p == 56)
    return *(power_of_two_table + 56);
  else if (p == 57)
    return *(power_of_two_table + 57);
  else if (p == 58)
    return *(power_of_two_table + 58);
  else if (p == 59)
    return *(power_of_two_table + 59);
  else if (p == 60)
    return *(power_of_two_table + 60);
  else if (p == 61)
    return *(power_of_two_table + 61);
  else if (p == 62)
    return *(power_of_two_table + 62);
  else if (p == 63)
    return *(power_of_two_table + 63);
  else
    VERIFIER_error();
}

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


uint64_t main() {
  uint64_t n;
  uint64_t result;

  init_library();

  n = input(0, 63, 1);

  if (n < 1)
  	return 0;

  result = hanoi(n);

  if (result + 1 > 0) {
    if (result + 1 == two_to_the_power_of(n))
      return 0;
    else
      VERIFIER_error();
  } else {
    VERIFIER_error();
  }

}


