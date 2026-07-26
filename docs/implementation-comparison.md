---
title: Implementation Comparison
description: Planned comparison of the Java, C++ and Python calculator implementations
---

# Implementation Comparison

This page will compare the three implementations after they have been added and validated.

## Comparison goals

The comparison will focus on the same small set of concerns:

- program entry point
- input handling
- validation strategy
- structured result representation
- 32-bit IPv4 arithmetic
- interactive terminal loop
- direct command-line mode
- error handling
- build and run workflow
- test approach

## Planned structure

| Area | Java 21 | C++20 | Python 3.12 |
|---|---|---|---|
| Result type | Record | Struct | Dataclass |
| Build | `javac` | CMake | None |
| Runtime | JVM | Native executable | Python interpreter |
| Dependencies | None | None | Standard library only |
| Interactive mode | Planned | Planned | Planned |
| Direct mode | Planned | Planned | Planned |
| Tests | Planned | Planned | Planned |

The page will describe meaningful language differences without presenting one implementation as universally superior.

## Current status

The shared specification and repository foundation are complete. Implementation-specific details will be documented in Phases 2 through 4.
