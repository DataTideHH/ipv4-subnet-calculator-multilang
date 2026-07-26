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
- the same six-character ASCII outer-whitespace rule
- the same structural validation priority
- rejection of multi-digit CIDR prefixes with leading zeros
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
| Outer trim | explicit ASCII helper | explicit ASCII helper | `strip(" \t\n\r\f\v")` helper |
| Bounded parsing | text comparison before conversion | text comparison before conversion | text comparison before conversion |
| Test runner | Plain Java process-based runner | CTest executable plus CMake CLI script | `unittest` plus subprocess checks |
| Shared cases | `tests/cases.tsv` | `tests/cases.tsv` | `tests/cases.tsv` |
| Process parity | `tests/cross_language_parity.py` | `tests/cross_language_parity.py` | `tests/cross_language_parity.py` |
| CI check | `Java 21` | `C++20` | `Python 3.12` |

## Explicit ASCII trimming

Language defaults are intentionally not used as the contract:

- Java `String.trim()` accepts a broader legacy control-character range than the project requires.
- Python `str.strip()` without arguments is Unicode-aware and removes characters that C++ would retain.
- C++ does not provide one universal trim operation with the desired semantics.

Each implementation therefore removes only ASCII space, tab, line feed, carriage return, form feed and vertical tab. This rule also applies to interactive input and quit handling.

## Validation priority

After exactly one slash is found, both structural components are checked before detailed parsing:

1. empty IPv4 component
2. empty prefix component
3. detailed IPv4 validation
4. detailed prefix validation

This makes mixed-invalid inputs deterministic. For example, `192.168.1.999/` reports an empty prefix in every language rather than allowing the C++ implementation to fail first on the invalid octet.

## Prefix representation

A single `/0` is valid. Multi-digit prefixes with a leading zero, including `/00`, `/08` and `/0032`, are rejected. This removes the earlier asymmetry between strict IPv4-octet representation and implicitly normalized prefix representation.

## Java design

The Java version keeps the beginner-readable implementation in one source file. A record stores the calculated values, while dedicated methods handle modes, input, output, validation and subnet arithmetic.

The Java contract runner executes the real command-line class in child JVMs. This verifies public terminal output, direct-mode exit codes, exact validation messages, shared domain cases, help and incorrect-usage behavior.

## C++ design

The C++ version separates the terminal interface from the calculation module:

```text
cpp/src/main.cpp
cpp/src/subnet.cpp
cpp/include/subnet.h
```

The `Calculation` struct represents the result. IPv4 values use `std::uint32_t`; address counts use `std::uint64_t` so `/0` can represent `2^32` addresses.

The C++ contract executable links directly against the calculation module. A separate CMake script executes the CLI smoke modes.

## Python design

The Python version keeps the implementation in one module and represents results with a frozen, slotted `dataclass`.

The standard-library `unittest` suite verifies shared calculation and validation cases. Subprocess checks cover valid and invalid direct mode, help and incorrect usage.

The explicit ASCII digit check matches the specification rather than Python's broader Unicode-aware `str.isdigit()` behavior.

## Cross-language bounded parsing

The implementations follow the same numeric sequence:

1. verify that the token contains only ASCII digits
2. reject a forbidden leading-zero representation where applicable
3. compare the decimal text with `255` or `32`
4. reject oversized values with the shared out-of-range message
5. convert only values already known to fit

This keeps the domain contract independent from language-specific integer behavior.

## Shared contract and parity design

The tab-separated file `tests/cases.tsv` is simple enough to parse without third-party libraries in all three languages. It defines seven valid and twenty-three invalid cases.

The parity runner then executes the real programs with control-character, Unicode-whitespace, validation-priority and interactive-mode inputs. It compares complete `stdout`, `stderr` and exit codes rather than only checking selected message fragments.

Equivalent behavior does not require identical source code, but it does require identical observable results at the command-line boundary.

## Verification status

The GitHub Actions workflow runs four checks independently:

- `Java 21`
- `C++20`
- `Python 3.12`
- `Cross-language parity`

This separates implementation-specific failures from cross-language conformance failures.
