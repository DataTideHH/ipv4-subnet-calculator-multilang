# Python Implementation

The Python 3.12 implementation follows the same observable behavior as the Java and C++ calculators.

## Requirements

- Python 3.12 or newer
- no external packages

## Run from the repository root

Interactive mode:

```text
python python/subnet_calculator.py
```

Direct mode:

```text
python python/subnet_calculator.py 192.168.10.42/24
```

Help:

```text
python python/subnet_calculator.py --help
```

On Windows, `py -3.12` can be used instead of `python` when multiple Python versions are installed:

```text
py -3.12 python/subnet_calculator.py 192.168.10.42/24
```

## Tests

Run the shared contract and CLI smoke tests:

```text
python -m unittest discover -s python -p "test_*.py" -v
```

On Windows, the equivalent explicit Python 3.12 command is:

```text
py -3.12 -m unittest discover -s python -p "test_*.py" -v
```

## Design

The implementation uses only the Python standard library:

- a frozen, slotted `dataclass` stores the calculation result
- terminal interaction is separated from calculation and validation functions
- IPv4 addresses are converted to 32-bit integer values for bitwise arithmetic
- explicit ASCII digit checks avoid accepting Unicode numerals
- bounded decimal text is compared before `int` conversion so oversized values produce the same range errors as Java and C++
- `ValueError` communicates the shared validation categories

## Program behavior

- no arguments start interactive mode
- one IPv4/CIDR argument performs one calculation
- `-h` and `--help` display usage information
- successful direct execution returns exit code `0`
- invalid direct input or incorrect usage returns exit code `1`
- interactive validation errors do not end the session
- `q`, `Q`, or end-of-file ends the interactive session normally

The normative rules are defined in [`docs/behavior-specification.md`](../docs/behavior-specification.md). Automated tests and GitHub Actions verify the current implementation.
