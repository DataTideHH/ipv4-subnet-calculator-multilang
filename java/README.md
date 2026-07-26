# Java Implementation

Java 21 implementation of the shared IPv4 subnet calculator specification.

## Build

From the repository root:

```text
javac -d java/out java/src/SubnetCalculator.java
```

## Interactive mode

```text
java -cp java/out SubnetCalculator
```

Enter IPv4/CIDR values repeatedly and use `q` or `Q` to quit.

## Direct command-line mode

```text
java -cp java/out SubnetCalculator 192.168.10.42/24
```

## Help

```text
java -cp java/out SubnetCalculator --help
```

## Tests

Compile the application and the standard-library test runner:

```text
javac -d java/out java/src/SubnetCalculator.java java/test/SubnetCalculatorTest.java
```

Run the shared contract and CLI smoke tests:

```text
java -cp java/out SubnetCalculatorTest tests/cases.tsv
```

## Implementation notes

- uses a Java record for the structured calculation result
- separates calculation, validation and terminal interaction
- uses explicit IPv4/CIDR validation
- compares bounded decimal text before `int` conversion so oversized numeric values produce the same range errors as C++ and Python
- uses 64-bit `long` arithmetic for the `/0` address count
- has no external dependencies
- follows `docs/behavior-specification.md`
