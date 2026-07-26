---
title: IPv4 Subnet Calculator Multilang
description: Java, C++ and Python implementations of the same terminal-based IPv4 subnet calculator
---

# IPv4 Subnet Calculator Multilang

**A small terminal-based IPv4 subnet calculator implemented in Java 21, C++20 and Python 3.12 according to one shared behavior specification.**

[View repository](https://github.com/DataTideHH/ipv4-subnet-calculator-multilang) · [Read the full README](https://github.com/DataTideHH/ipv4-subnet-calculator-multilang/blob/main/README.md) · [DataTideHH portfolio](https://datatidehh.de/)

---

## Project purpose

This project compares how the same clearly bounded networking calculation can be implemented in three programming languages.

The focus is intentionally narrow:

- read an IPv4 address with a CIDR prefix
- validate the input
- calculate the subnet information
- display a consistent terminal result
- handle important IPv4 edge cases
- keep the implementations readable and comparable

The project connects programming fundamentals with practical IPv4 subnetting without turning a small calculator into an unnecessarily large application.

---

## Shared behavior

All three implementations will use the same canonical input format:

```text
IPv4/CIDR
```

Example:

```text
192.168.10.42/24
```

The expected result structure is:

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

## Planned terminal modes

### Interactive mode

```text
IPv4 Subnet Calculator

Enter IPv4/CIDR or q to quit:
> 192.168.10.42/24
```

The application will continue accepting input until the user quits.

### Direct command-line mode

```text
<program> 192.168.10.42/24
```

This mode supports repeatable manual tests and later automation.

---

## Implementation comparison

| Implementation | Version / Standard | Tooling | External dependencies |
|---|---|---|---|
| Java | Java 21 | `javac` and `java` | None |
| C++ | C++20 | CMake | None |
| Python | Python 3.12 | Python interpreter | None |

The detailed comparison will be expanded as each implementation is added. See [implementation comparison](implementation-comparison.md).

---

## Validation focus

The calculator will reject malformed or unsupported input, including:

- missing CIDR prefixes
- invalid IPv4 octet counts
- empty or non-decimal octets
- octets outside `0-255`
- leading zeros in multi-digit octets
- prefixes outside `0-32`

Invalid input will not terminate the interactive session. Direct command-line mode will return a non-zero exit code.

---

## Deliberate scope limits

The initial project does not include:

- IPv6
- a graphical user interface
- a web application
- VLSM planning
- subnet splitting
- database storage
- export functions
- external command-line frameworks

These limits keep the repository understandable and ensure that the three language implementations remain directly comparable.

---

## Current status

Phase 1 is complete and provides:

- repository documentation
- shared behavior specification
- language-directory foundations
- GitHub Pages foundation
- pull-request template
- squash-oriented contribution workflow

The Java, C++ and Python implementations will be added in subsequent phases.

---

## Portfolio context

The project supports practical learning in Java, C++, Python, IPv4 subnetting, input validation, reproducible workflows and technical documentation.

It is a supporting IT-foundations project within the broader DataTideHH portfolio and complements CCNA-oriented networking work without changing the main Data/BI and process-analysis direction.
