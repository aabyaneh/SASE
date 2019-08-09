/*
  This file is a C* translation of the original implementation
  done by Alireza Abyaneh.
*/

uint64_t FNM_NOMATCH =	1;
uint64_t EOS = 0;

uint64_t CHAR_BACKSLASH = 92;

uint64_t* rangematch(uint64_t* pattern, uint64_t test, uint64_t flags);

uint64_t fnmatch(uint64_t* pattern, uint64_t* string, uint64_t flags) {
	uint64_t* stringstart;
	uint64_t c;
  uint64_t test;

  while (1) {
    c = *pattern;
    pattern = pattern + 1;
    if (c == EOS) {
      if (*string == EOS) {
        return 0;
      } else {
        return FNM_NOMATCH;
      }

    } else if (c == '?') {
      if (*string == EOS) {
				return FNM_NOMATCH;
      }
			string = string + 1;

    } else if (c == '*') {
      c = *pattern;

			while (c == '*') {
        pattern = pattern + 1;
				c = *pattern;
      }

			if (c == EOS) {
				return 0;
      }

      test = *string;
      while (test != EOS) {
        if (fnmatch(pattern, string, 0) == 0) {
          return 0;
        }

        string = string + 1;
        test = *string;
      }

      return FNM_NOMATCH;

    } else if (c == '[') {
      if (*string == EOS) {
				return FNM_NOMATCH;
      }
      pattern = rangematch(pattern, *string, 0);
			if (pattern == (uint64_t*) 0) {
				return FNM_NOMATCH;
      }
			string = string + 1;

    } else if (c == CHAR_BACKSLASH) {
      c = *pattern;
      pattern = pattern + 1;
			if (c == EOS) {
				c = CHAR_BACKSLASH;
				pattern = pattern -  1;
			}

      if (c != *string) {
        return FNM_NOMATCH;
      }
			string = string + 1;

    } else {
      if (c != *string) {
        return FNM_NOMATCH;
      }
			string = string + 1;
		}

  }
}

uint64_t* rangematch(uint64_t* pattern, uint64_t test, uint64_t flags) {
	uint64_t negate;
  uint64_t ok;
	uint64_t c;
  uint64_t c2;

  if (*pattern == '!') {
    negate = 1;
  } else if (*pattern == '^') {
    negate = 1;
  } else {
    negate = 0;
  }

	if (negate) {
		pattern = pattern + 1;
  }

  ok = 0;
  c = *pattern;
  pattern = pattern + 1;
  while (c != ']') {
    if (c == CHAR_BACKSLASH) {
		  c = *pattern;
      pattern = pattern + 1;
    }

		if (c == EOS) {
			return (uint64_t*) 0;
    }

    if (*pattern == '-') {
      c2 = *(pattern+1);
      if (c2 != EOS) {
        if (c2 != ']') {
          pattern = pattern + 2;
    			if (c2 == CHAR_BACKSLASH) {
            c2 = *pattern;
            pattern = pattern + 1;
          }

    			if (c2 == EOS) {
    				return (uint64_t*) 0;
          }

    			if (c <= test) {
            if (test <= c2) {
              ok = 1;
            }
          }
        }
      }
    } else if (c == test) {
			ok = 1;
    }

    c = *pattern;
    pattern = pattern + 1;
  }

  if (ok == negate) {
    return (uint64_t*) 0;
  } else {
    return pattern;
  }
}

uint64_t main() {
  uint64_t* pattern_str;
  uint64_t* str;
  uint64_t  res;
  uint64_t  cnt;
  uint64_t  i;

  pattern_str = malloc(19 * 8);
  *(pattern_str + 0)  = '[';
  *(pattern_str + 1)  = 'a';
  *(pattern_str + 2)  = '-';
  *(pattern_str + 3)  = 'z';
  *(pattern_str + 4)  = ']';
	*(pattern_str + 5)  = '[';
  *(pattern_str + 6)  = '0';
  *(pattern_str + 7)  = '-';
  *(pattern_str + 8)  = '9';
  *(pattern_str + 9)  = ']';
  *(pattern_str + 10) = '*';
  *(pattern_str + 11) = '?';
  *(pattern_str + 12) = '*';
  *(pattern_str + 13) = '[';
  *(pattern_str + 14) = 'a';
  *(pattern_str + 15) = '-';
  *(pattern_str + 16) = 'z';
  *(pattern_str + 17) = ']';
  *(pattern_str + 18) = 0;

  cnt = 12;
  str = malloc(cnt * 8);
  i = 0;
  while (i < cnt-1) {
    *(str + i) = input(33, 125, 1);
    i = i + 1;
  }
  *(str + cnt-1) = 0;

  res = fnmatch(pattern_str, str, 0);

  return 0;
}