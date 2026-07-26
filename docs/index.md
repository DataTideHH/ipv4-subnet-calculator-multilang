---
title: IPv4 Subnet Calculator Multilang
description: Tested Java, C++ and Python implementations of the same IPv4 subnet calculator
---

# IPv4 Subnet Calculator Multilang

**One IPv4/CIDR specification, three implementations, one shared contract-test suite.**

[View repository](https://github.com/DataTideHH/ipv4-subnet-calculator-multilang) · [View CI](https://github.com/DataTideHH/ipv4-subnet-calculator-multilang/actions/workflows/ci.yml) · [Read the full README](https://github.com/DataTideHH/ipv4-subnet-calculator-multilang/blob/main/README.md) · [DataTideHH portfolio](https://datatidehh.de/)

---

## Project purpose

This project compares how a clearly bounded networking calculation can be implemented and verified in Java 21, C++20 and Python 3.12.

The deliberately narrow scope makes the comparison credible:

- one accepted `IPv4/CIDR` format
- explicit validation
- consistent terminal output
- interactive and direct modes
- correct `/0`, `/31` and `/32` handling
- no external runtime dependencies
- one shared set of expected domain results

The project connects programming fundamentals, test design and practical IPv4 subnetting without presenting a small calculator as a production network platform.

---

## Implementations

| Implementation | Structure | Verification | Status |
|---|---|---|---|
| Java 21 | Single application class with a record result | Plain Java CLI contract runner | Implemented and tested |
| C++20 | Header, calculation module and terminal entry point | CTest executable | Implemented and tested |
| Python 3.12 | Single module with a frozen, slotted dataclass | Standard-library `unittest` | Implemented and tested |

All three versions share the same input contract, output fields, validation categories, special-case behavior and exit rules.

---

## Example

Input:

```text
192.168.10.42/24
```

Output:

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

## Shared verification

Every language-specific runner reads the same tab-separated contract file.

The current set covers:

- standard `/24`, `/30` and `/16` subnets
- all IPv4 addresses through `/0`
- `/31` point-to-point semantics
- `/32` host-route semantics
- surrounding-whitespace trimming
- malformed separators and octet counts
- empty and non-decimal values
- signs, range violations and leading zeros
- whitespace inside the token

Valid cases verify every result field. Invalid cases verify the exact validation reason.

Read [Testing and CI](testing-and-ci.md) for commands and design details.

---

## GitHub Actions

The repository runs three independent checks on pull requests and pushes to `main`:

- `Java 21`
- `C++20`
- `Python 3.12`

Separate jobs make failures easy to locate and provide stable status checks for protected-branch rules.

---

## Run examples

### Java

```text
javac -d java/out java/src/SubnetCalculator.java
java -cp java/out SubnetCalculator 192.168.10.42/24
```

### C++

```text
cmake -S cpp -B cpp/build
cmake --build cpp/build --config Release
```

### Python

```text
python python/subnet_calculator.py 192.168.10.42/24
```

---

## Related portfolio projects

- [Cisco Switching Lab](https://datatidehh.github.io/cisco-switching-lab/) provides the physical switching and CCNA-oriented context behind IPv4 addressing and subnetting.
- [Spring Boot Process API Basics](https://datatidehh.github.io/spring-boot-process-api-basics/) shows the Java progression from a compact command-line implementation to a layered REST API.

Together, the projects show a controlled learning progression across networking fundamentals, multi-language implementation, automated verification and backend structure.

---

## Scope limits

The project intentionally excludes IPv6, VLSM planning, subnet splitting, a GUI, a web frontend, persistence, exports and external CLI frameworks.

These limits keep the source understandable and the cross-language comparison direct.

---

## Portfolio assessment

The strongest signal is not the calculator alone. It is the disciplined workflow around it:

- explicit behavioral specification
- three independently structured implementations
- shared fachliche test cases
- reproducible local commands
- GitHub Actions
- public project documentation
- honest scope boundaries

That makes the repository a credible supporting project for a Data/BI and process-analysis portfolio with additional networking and software-engineering foundations.
