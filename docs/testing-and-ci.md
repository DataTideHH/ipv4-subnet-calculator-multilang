---
title: Testing and CI
description: Shared contract cases, language-specific test runners and GitHub Actions
---

# Testing and CI

The project verifies equivalent behavior through one shared set of domain cases and one small test runner per language.

## Shared contract cases

The file [`tests/cases.tsv`](https://github.com/DataTideHH/ipv4-subnet-calculator-multilang/blob/main/tests/cases.tsv) is the language-neutral source of truth.

It contains valid and invalid IPv4/CIDR examples. Valid cases define the complete expected result. Invalid cases define the exact validation category and message.

The contract covers standard subnets, `/0`, `/31`, `/32`, surrounding whitespace and the malformed input categories from the behavior specification.

## Language-specific tests

| Language | Test runner | Scope |
|---|---|---|
| Java 21 | `java/test/SubnetCalculatorTest.java` | Executes the command-line program, checks exit codes, full output fields and validation errors |
| C++20 | `cpp/tests/subnet_test.cpp` | Calls the calculation module and checks every structured result field and exception message |
| Python 3.12 | `python/test_subnet_calculator.py` | Uses `unittest` for calculation cases plus direct-mode and help smoke tests |

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

Keeping the jobs separate makes failures easy to locate and allows branch protection to require all three checks before a pull request can be merged.
