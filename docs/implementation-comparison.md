---
title: Implementation Comparison
description: Comparison of the Java, C++ and Python calculator implementations
---

# Implementation Comparison

The Java, C++ and Python implementations follow the same observable behavior while using idiomatic structures for each language.

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
| Status | Implemented | Implemented | Implemented |
| Result type | Record | Struct | Frozen, slotted dataclass |
| Build | `javac` | CMake | None |
| Runtime | JVM | Native executable | Python interpreter |
| Dependencies | Java standard library | C++ standard library | Python standard library |
| Interactive mode | Implemented | Implemented | Implemented |
| Direct mode | Implemented | Implemented | Implemented |
| Help options | `-h`, `--help` | `-h`, `--help` | `-h`, `--help` |
| Validation errors | `IllegalArgumentException` | `std::invalid_argument` | `ValueError` |
| IPv4 storage | 64-bit `long` calculation values | `std::uint32_t` | Arbitrary-precision `int`, masked to 32 bits |
| Address count | 64-bit `long` | `std::uint64_t` | `int` |
| Automated tests | Phase 4 | Phase 4 | Phase 4 |

## Java design

The Java version keeps the complete beginner-readable implementation in one source file.

A private record stores the calculated values. Dedicated methods handle:

- program modes
- user input
- output formatting
- IPv4 validation
- CIDR validation
- subnet arithmetic
- IPv4 text conversion

The implementation uses a `long` for address counts so `/0` can represent all `4,294,967,296` IPv4 addresses.

## C++ design

The C++ version separates the terminal interface from the calculation module:

```text
cpp/src/main.cpp
cpp/src/subnet.cpp
cpp/include/subnet.h
```

The `Calculation` struct represents the shared result. IPv4 values use `std::uint32_t`, while total and usable address counts use `std::uint64_t`.

Validation failures use `std::invalid_argument` with a specific reason, allowing the terminal layer to report errors equivalent to the Java and Python versions.

## Python design

The Python version keeps the implementation in one module:

```text
python/subnet_calculator.py
```

A frozen, slotted `dataclass` represents the shared calculation result. Separate functions handle:

- argument and mode selection
- interactive input
- output formatting
- IPv4 and CIDR validation
- subnet arithmetic
- integer-to-IPv4 conversion

Python integers do not overflow at 32 or 64 bits. The implementation still applies an explicit `0xFFFFFFFF` mask where required so that the bitwise behavior remains equivalent to fixed-width IPv4 arithmetic.

The digit check deliberately accepts only ASCII `0-9`, matching the shared specification instead of Python's broader Unicode-aware `str.isdigit()` behavior.

## Deliberate differences

The goal is equivalent behavior, not identical source code.

- Java uses a record and `IllegalArgumentException`.
- C++ uses a struct, fixed-width unsigned integers and `std::invalid_argument`.
- Python uses a dataclass, functions and `ValueError`.
- Java and Python remain single-file implementations because the program is small.
- C++ keeps a header and source split because calculation logic and the executable are separate compilation units.

These differences demonstrate language conventions while preserving the same user-facing contract.

## Validation status

The implementations were manually checked for:

- standard subnet output
- `/0`
- `/31`
- `/32`
- all invalid examples in the shared specification
- interactive error recovery
- quit and EOF behavior
- help behavior
- direct-mode exit codes

Formal test suites and continuous integration remain intentionally reserved for Phase 4.
