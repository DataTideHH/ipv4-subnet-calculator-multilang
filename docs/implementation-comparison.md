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
| Test runner | Plain Java process-based runner | CTest executable | `unittest` |
| Shared cases | `tests/cases.tsv` | `tests/cases.tsv` | `tests/cases.tsv` |
| CI check | `Java 21` | `C++20` | `Python 3.12` |

## Java design

The Java version keeps the beginner-readable implementation in one source file. A record stores the calculated values, while dedicated methods handle modes, input, output, validation and subnet arithmetic.

The Java contract runner executes the real command-line class in a child JVM. This verifies:

- public terminal output
- direct-mode exit codes
- exact validation messages
- all shared valid and invalid domain cases

Using the public CLI boundary avoids exposing internal methods solely for testing.

## C++ design

The C++ version separates the terminal interface from the calculation module:

```text
cpp/src/main.cpp
cpp/src/subnet.cpp
cpp/include/subnet.h
```

The `Calculation` struct represents the result. IPv4 values use `std::uint32_t`; address counts use `std::uint64_t` so `/0` can represent `2^32` addresses.

The C++ test executable links directly against the calculation module and verifies every result field and `std::invalid_argument` message. CTest integrates it into both local builds and GitHub Actions.

## Python design

The Python version keeps the implementation in one module and represents results with a frozen, slotted `dataclass`.

The standard-library `unittest` suite verifies the shared calculation and validation cases. Additional subprocess checks cover direct-mode output, success and failure exit codes, and help mode.

The explicit ASCII digit check matches the specification rather than Python's broader Unicode-aware `str.isdigit()` behavior.

## Shared contract design

The tab-separated file `tests/cases.tsv` is intentionally simple enough to parse without third-party libraries in all three languages.

Valid rows define:

- subnet and wildcard masks
- network and broadcast addresses
- total and usable address counts
- first and last usable hosts
- the explanatory note

Invalid rows define the exact expected validation message.

This makes the fachliche expectations independent from any one implementation language.

## Deliberate differences

Equivalent behavior does not require identical source code:

- Java tests the public CLI through a child JVM.
- C++ tests the reusable calculation module directly.
- Python combines direct function tests with CLI smoke tests.
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
- out-of-range values
- leading zeros

The GitHub Actions workflow runs all three language checks independently on pull requests and pushes to `main`.
