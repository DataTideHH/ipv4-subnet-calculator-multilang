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

## Implementation notes

- uses a Java record for the structured calculation result
- separates calculation, validation and terminal interaction
- uses explicit IPv4/CIDR validation
- uses 64-bit `long` arithmetic for the `/0` address count
- has no external dependencies
- follows `docs/behavior-specification.md`
