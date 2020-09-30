# Snum
Simple parser for numbers

## Features
At now Snum supports 4 bases for conversions: binary, octal, hexadecimal and decimal.

Base | Format | Range (of digits)
-----|--------|------------------
Binary | 0b... | 0s and 1s
Octal | 0... | \[0-7\]
Hexadecimal | 0x... | \[0-9A-Fa-f\]
Decimal | ... | \[0-9\]

_**note: Snum is case insensitive so 0b101 is equal to 0B101**_

Moreover, reals and signed numbers are available. For reals you have to put a dot (.) between decimal and integral sections. +/- as signs (obviusly).

## Usage
To use Snum you have to include `snum.h` into your C or C++ project and add `snum.c` for compilation.
```C++
#include <iostream>
#include "snum.h"

int main(int argc, char** argv) {
  if (argc < 2) std::cerr << "[usage]> " << argv[0] << " [number]" << std::endl;
  else {
    num4_t num;
    snum_ston(&num, argv[1]);
    if (num.flags & SNUM_INF)
      std::cout << "you typed an infinite value" << std::endl;
    else if (num.flags & SNUM_NAN)
      std::cout << "you typed a not-a-number (NAN) value" << std::endl;
    else if (num.flags & SNUM_REAL)
      std::cout << "you typed " << snum_ntov(&num).r4 << std::endl;
    else if (num.flags & SNUM_SIGN)
      std::cout << "you typed " << snum_ntov(&num).s4 << std::endl;
    else
      std::cout << "you typed " << snum_ntov(&num).u4 << std::endl;
  }
  return 0;
}
```
