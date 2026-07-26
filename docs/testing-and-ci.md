---
title: Testing and CI
description: Shared contract cases, language-specific test runners and GitHub Actions
---

# Testing and CI

The project verifies equivalent behavior through one shared set of domain cases and one small test runner per language.

## Shared contract cases

The file [`tests/cases.tsv`](https://github.com/DataTideHH/ipv4-subnet-calculator-multilang/blob/main/tests/cases.tsv) is the language-neutral source of truth.

It contains seven valid and sixteen invalid IPv4/CIDR examples. Valid cases define the complete expected result. Invalid cases define the exact validation category and message.

The contract covers:

- standard subnets
- `/0`, `/31` and `/32`
- surrounding whitespace
- malformed separators and octet counts
- empty and non-decimal values
- signs and internal whitespace
- leading zeros
- ordinary out-of-range values
- oversized numeric octets and prefixes

The oversized numeric cases prevent a cross-language parser difference: Java and C++ use fixed-width integers, while Python supports arbitrary-precision integers. Every implementation now performs the same bounded decimal text comparison before conversion.

## Language-specific tests

| Language | Test runner | Contract scope | CLI smoke scope |
|---|---|---|---|
| Java 21 | `java/test/SubnetCalculatorTest.java` | Executes the command-line program and checks all shared result fields, errors and exit codes | valid direct call, invalid direct call, help and incorrect usage |
| C++20 | `cpp/tests/subnet_test.cpp` with CTest | Calls the calculation module and checks every structured result field and exception message | CMake script executes the built CLI for the same four modes |
| Python 3.12 | `python/test_subnet_calculator.py` | Uses `unittest` for every shared calculation and validation case | subprocess checks for the same four modes |

No external test framework is required. The repository stays reproducible with the Java, C++ and Python standard toolchains.

## Local commands

### Java

```text
javac -d java/out java/src/SubnetCalculator.java java/test/SubnetCalculatorTest.java
java -cp java/out SubnetCalculatorTest tests/cases.tsv
```

### C++

```text
cmake -S cpp -B cpp/build -DBUILD_TESTING=ON
cmake --build cpp/build --config Release
ctest --test-dir cpp/build --build-config Release --output-on-failure
```

### Python

```text
python -m unittest discover -s python -p "test_*.py" -v
```

On Windows, `py -3.12` can replace `python`.

## GitHub Actions

The workflow under `.github/workflows/ci.yml` runs on pull requests, pushes to `main` and manual dispatches.

It exposes three independent checks:

- `Java 21`
- `C++20`
- `Python 3.12`

The workflow uses read-only repository permissions, cancels superseded runs and applies a ten-minute timeout to every job. Current official action major versions are used for checkout and language setup.

Keeping the jobs separate makes failures easy to locate and allows branch protection to require all three checks before a pull request can be merged.
