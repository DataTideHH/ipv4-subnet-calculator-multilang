---
title: IPv4 Subnet Calculator Multilang
description: Java, C++ and Python implementations of the same terminal-based IPv4 subnet calculator
---

# IPv4 Subnet Calculator Multilang

**A small terminal-based IPv4 subnet calculator implemented in Java 21, C++20 and Python 3.12 from one shared behavior specification.**

[View repository](https://github.com/DataTideHH/ipv4-subnet-calculator-multilang) · [Read the full README](https://github.com/DataTideHH/ipv4-subnet-calculator-multilang/blob/main/README.md) · [DataTideHH portfolio](https://datatidehh.de/)

---

## Project purpose

This project compares how the same clearly bounded networking calculation can be implemented in three programming languages.

The focus is intentionally narrow:

- read an IPv4 address with a CIDR prefix
- validate the complete input
- calculate subnet information
- display a consistent terminal result
- handle important IPv4 edge cases
- keep the implementations readable and directly comparable

The project connects programming fundamentals with practical IPv4 subnetting without turning a small calculator into an unnecessarily large application.

---

## Current implementations

| Implementation | Version / Standard | Tooling | Status |
|---|---|---|---|
| Java | Java 21 | `javac` and `java` | Implemented |
| C++ | C++20 | CMake | Implemented |
| Python | Python 3.12 | Python interpreter | Implemented |

All three versions share the same input contract, output fields, validation categories, special-case behavior and exit rules.

---

## Shared behavior

The canonical input format is:

```text
IPv4/CIDR
```

Example:

```text
192.168.10.42/24
```

Every implementation produces:

```text
Input IP:          192.168.10.42
CIDR prefix:       /24
Subnet mask:       255.255.255.0
Wildcard mask:     0.0.0.255
Network address:   192.168.10.0
Broadcast address: 192.168.10.255
Total addresses:   256
Usable hosts:      254
First usable host: 192.168.10.1
Last usable host:  192.168.10.254
Note:              Standard subnet with network and broadcast addresses excluded.
```

The complete normative rules are documented in the [behavior specification](behavior-specification.md).

---

## Terminal modes

### Interactive mode

Starting an implementation without a subnet argument opens:

```text
IPv4 Subnet Calculator

Enter IPv4/CIDR or q to quit:
> 192.168.10.42/24
```

The calculator continues after valid results and validation errors. Enter `q` or `Q` to quit.

### Direct command-line mode

```text
<program> 192.168.10.42/24
```

This mode supports repeatable manual checks and later automation. Invalid input returns a non-zero exit code.

---

## Run examples

### Java 21

```text
javac -d java/out java/src/SubnetCalculator.java
java -cp java/out SubnetCalculator
```

### C++20

```text
cmake -S cpp -B cpp/build
cmake --build cpp/build
./cpp/build/ipv4_subnet_calculator
```

Windows multi-configuration generators place the executable in a configuration directory such as `cpp\build\Debug` or `cpp\build\Release`.

### Python 3.12

```text
python python/subnet_calculator.py
python python/subnet_calculator.py 192.168.10.42/24
```

On Windows, `py -3.12` can be used to select Python 3.12 explicitly.

---

## Validation focus

The calculator rejects malformed or unsupported input, including:

- missing or multiple CIDR separators
- invalid IPv4 octet counts
- empty or non-decimal octets
- octets outside `0-255`
- leading zeros in multi-digit octets
- missing or invalid prefixes
- prefixes outside `0-32`
- whitespace inside the IPv4/CIDR token

All versions report concrete validation categories instead of one generic invalid-input message.

---

## Deliberate scope limits

The current project does not include:

- IPv6
- a graphical user interface
- a web application
- VLSM planning
- subnet splitting
- database storage
- export functions
- external command-line frameworks

These limits keep the repository understandable and ensure that the language implementations remain directly comparable.

---

## Current status

Phase 3 is complete:

- Java implementation consolidated
- C++ implementation consolidated
- Python implementation added
- output structure unified across all three languages
- validation categories aligned
- interactive, direct and help modes aligned
- `/0`, `/31` and `/32` behavior aligned
- language-specific documentation and comparison updated

Formal automated tests and GitHub Actions remain planned for Phase 4.

---

## Portfolio context

The project supports practical learning in Java, C++, Python, IPv4 subnetting, input validation, reproducible workflows and technical documentation.

It is a supporting IT-foundations project within the broader DataTideHH portfolio and complements CCNA-oriented networking work without changing the main Data/BI and process-analysis direction.
