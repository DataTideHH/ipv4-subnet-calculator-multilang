---
title: Testing and CI
description: Shared contract cases, language-specific runners and cross-language process parity
---

# Testing and CI

The project verifies equivalent behavior at two levels:

1. one shared set of domain cases executed by every language-specific runner
2. one process-level parity suite that compares complete `stdout`, `stderr` and exit codes across Java, C++ and Python

## Shared contract cases

The file [`tests/cases.tsv`](https://github.com/DataTideHH/ipv4-subnet-calculator-multilang/blob/main/tests/cases.tsv) is the language-neutral source of truth.

It contains seven valid and twenty-three invalid IPv4/CIDR examples. Valid cases define the complete expected result. Invalid cases define the exact validation category and message.

The contract covers:

- standard subnets
- `/0`, `/31` and `/32`
- surrounding ASCII whitespace
- malformed separators and octet counts
- empty and non-decimal values
- signs and internal whitespace
- leading zeros in octets and prefixes
- ordinary and oversized numeric ranges
- combined defects that verify validation priority

The oversized numeric cases prevent a parser difference: Java and C++ use fixed-width integers, while Python supports arbitrary-precision integers. Every implementation performs the same bounded decimal text comparison before conversion.

The combined-error cases verify that, after exactly one `/` separator is found, empty IPv4 and prefix components are checked before either component is parsed in detail.

## Language-specific tests

| Language | Test runner | Contract scope | CLI smoke scope |
|---|---|---|---|
| Java 21 | `java/test/SubnetCalculatorTest.java` | Executes the command-line program and checks all shared result fields, errors and exit codes | valid direct call, invalid direct call, help and incorrect usage |
| C++20 | `cpp/tests/subnet_test.cpp` with CTest | Calls the calculation module and checks every structured result field and exception message | CMake script executes the built CLI for the same four modes |
| Python 3.12 | `python/test_subnet_calculator.py` | Uses `unittest` for every shared calculation and validation case | subprocess checks for the same four modes |

No external test framework is required.

## Cross-language parity

[`tests/cross_language_parity.py`](https://github.com/DataTideHH/ipv4-subnet-calculator-multilang/blob/main/tests/cross_language_parity.py) executes the three real command-line programs and compares the complete observable result:

```text
(return code, stdout, stderr)
```

The current parity suite contains nine direct cases and two interactive cases. It covers:

- empty-prefix priority when the IPv4 part is also invalid
- leading-zero prefixes
- a non-whitespace ASCII control character
- non-breaking space (`U+00A0`)
- ideographic space (`U+3000`)
- allowed ASCII tab and carriage-return trimming
- ASCII-trimmed interactive quit handling
- rejection of Unicode-space-wrapped quit text

This closes gaps that ordinary happy-path and one-error-at-a-time contract cases can miss.

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

### Cross-language parity

After Java and C++ have been built:

```text
python tests/cross_language_parity.py
```

On Windows, `py -3.12` can replace `python`.

## GitHub Actions

The workflow under `.github/workflows/ci.yml` runs on pull requests, pushes to `main` and manual dispatches.

It exposes four independent checks:

- `Java 21`
- `C++20`
- `Python 3.12`
- `Cross-language parity`

The workflow uses read-only repository permissions, cancels superseded runs and applies a ten-minute timeout to every job. Keeping the checks separate makes failures easy to locate and allows branch protection to require the language checks and process-level conformance independently.
