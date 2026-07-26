---
title: IPv4 Subnet Calculator Multilang
description: Java, C++ and Python implementations of the same terminal-based IPv4 subnet calculator
---

# IPv4 Subnet Calculator Multilang

**A small terminal-based IPv4 subnet calculator implemented from one shared behavior specification.**

[View repository](https://github.com/DataTideHH/ipv4-subnet-calculator-multilang) · [Read the full README](https://github.com/DataTideHH/ipv4-subnet-calculator-multilang/blob/main/README.md) · [DataTideHH portfolio](https://datatidehh.de/)

---

## Project purpose

This project compares how the same clearly bounded networking calculation can be implemented in multiple programming languages.

The focus is intentionally narrow:

- read an IPv4 address with a CIDR prefix
- validate the input
- calculate the subnet information
- display a consistent terminal result
- handle important IPv4 edge cases
- keep the implementations readable and comparable

The project connects programming fundamentals with practical IPv4 subnetting without turning a small calculator into an unnecessarily large application.

---

## Current implementations

| Implementation | Version / Standard | Tooling | Status |
|---|---|---|---|
| Java | Java 21 | `javac` and `java` | Implemented |
| C++ | C++20 | CMake | Implemented |
| Python | Python 3.12 | Python interpreter | Planned |

The Java and C++ versions currently share the same input contract, output fields, validation categories and special-case behavior.

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

Both current implementations produce:

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

Starting either implementation without a subnet argument opens:

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

## Build examples

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

Windows multi-configuration generators commonly place the executable in a configuration directory such as `cpp\build\Debug`.

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

The C++ version now reports the same concrete validation categories as Java instead of returning one generic invalid-input message.

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

Phase 2 is complete:

- Java implementation consolidated
- C++ implementation consolidated
- output structure unified
- detailed C++ validation errors added
- interactive mode added to both implementations
- direct mode and help options aligned
- build and usage documentation updated

The Python implementation is planned for Phase 3. Formal automated tests and GitHub Actions remain planned for Phase 4.

---

## Portfolio context

The project supports practical learning in Java, C++, Python, IPv4 subnetting, input validation, reproducible workflows and technical documentation.

It is a supporting IT-foundations project within the broader DataTideHH portfolio and complements CCNA-oriented networking work without changing the main Data/BI and process-analysis direction.
