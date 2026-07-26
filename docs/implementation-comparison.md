---
title: Implementation Comparison
description: Comparison of the Java, C++ and Python calculator implementations and tests
---

# Implementation Comparison

The Java, C++ and Python implementations follow the same observable behavior while using structures and test approaches appropriate to each language.

## Shared behavior

All implementations provide:

- interactive terminal mode without arguments
- direct command-line mode with one IPv4/CIDR argument
- `-h` and `--help`
- the same output fields and ordering
- the same `/0`, `/31` and `/32` behavior
- concrete validation errors
- exit code `0` for successful direct execution
- exit code `1` for invalid direct input or incorrect usage
- consistent classification of oversized numeric octets and prefixes

## Current comparison

| Area | Java 21 | C++20 | Python 3.12 |
|---|---|---|---|
| Status | Implemented and tested | Implemented and tested | Implemented and tested |
| Result type | Record | Struct | Frozen, slotted dataclass |
| Build | `javac` | CMake | None |
| Runtime | JVM | Native executable | Python interpreter |
| Runtime dependencies | Java standard library | C++ standard library | Python standard library |
| Validation errors | `IllegalArgumentException` | `std::invalid_argument` | `ValueError` |
| IPv4 storage | 64-bit `long` calculation values | `std::uint32_t` | Arbitrary-precision `int`, masked to 32 bits |
| Address count | 64-bit `long` | `std::uint64_t` | `int` |
| Bounded parsing | Text comparison before `int` conversion | Text comparison before fixed-width conversion | Text comparison before `int` conversion |
| Test runner | Plain Java process-based runner | CTest executable plus CMake CLI script | `unittest` plus subprocess checks |
| Shared cases | `tests/cases.tsv` | `tests/cases.tsv` | `tests/cases.tsv` |
| CLI smoke modes | valid, invalid, help, incorrect usage | valid, invalid, help, incorrect usage | valid, invalid, help, incorrect usage |
| CI check | `Java 21` | `C++20` | `Python 3.12` |

## Java design

The Java version keeps the beginner-readable implementation in one source file. A record stores the calculated values, while dedicated methods handle modes, input, output, validation and subnet arithmetic.

The Java contract runner executes the real command-line class in child JVMs. This verifies:

- public terminal output
- direct-mode exit codes
- exact validation messages
- all shared valid and invalid domain cases
- help and incorrect-usage behavior

Using the public CLI boundary avoids exposing internal methods solely for testing.

## C++ design

The C++ version separates the terminal interface from the calculation module:

```text
cpp/src/main.cpp
cpp/src/subnet.cpp
cpp/include/subnet.h
```

The `Calculation` struct represents the result. IPv4 values use `std::uint32_t`; address counts use `std::uint64_t` so `/0` can represent `2^32` addresses.

The C++ contract executable links directly against the calculation module and verifies every result field and `std::invalid_argument` message. A separate CMake script executes the built CLI and verifies the four shared smoke modes. CTest integrates both layers into local builds and GitHub Actions.

## Python design

The Python version keeps the implementation in one module and represents results with a frozen, slotted `dataclass`.

The standard-library `unittest` suite verifies the shared calculation and validation cases. Subprocess checks cover valid and invalid direct mode, help and incorrect usage.

The explicit ASCII digit check matches the specification rather than Python's broader Unicode-aware `str.isdigit()` behavior.

## Cross-language bounded parsing

Fixed-width integer parsers can overflow before an application reaches its intended range check. Python integers behave differently because they support arbitrary precision.

The implementations therefore follow the same sequence:

1. verify that the token contains only ASCII digits
2. normalize insignificant leading zeros for comparison
3. compare the decimal text with `255` or `32`
4. reject oversized values with the shared out-of-range message
5. convert only values already known to fit

This keeps the domain contract independent from language-specific integer behavior.

## Shared contract design

The tab-separated file `tests/cases.tsv` is intentionally simple enough to parse without third-party libraries in all three languages.

Valid rows define:

- subnet and wildcard masks
- network and broadcast addresses
- total and usable address counts
- first and last usable hosts
- the explanatory note

Invalid rows define the exact expected validation message.

This makes the domain expectations independent from any one implementation language.

## Deliberate differences

Equivalent behavior does not require identical source code:

- Java tests the public CLI through child JVMs.
- C++ tests the reusable calculation module directly and adds a separate CLI script.
- Python combines direct function tests with subprocess checks.
- Java and Python remain single-file applications because the program is small.
- C++ keeps a header/source split because that is natural for the compiled module boundary.

## Verification status

The automated contract covers:

- standard `/24`, `/30` and `/16` subnet output
- `/0`
- `/31`
- `/32`
- surrounding-whitespace trimming
- malformed separators
- malformed octet counts
- empty and non-decimal values
- signs and internal whitespace
- ordinary and oversized out-of-range values
- leading zeros

The GitHub Actions workflow runs all three language checks independently on pull requests and pushes to `main`.
