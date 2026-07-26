# Contributing

This is a small personal learning and portfolio project. Changes should remain focused on the documented IPv4 subnet calculator scope.

## Workflow

1. Start from the current `main` branch.
2. Create a short-lived branch with a descriptive name.
3. Make one coherent change.
4. Update the shared contract cases when observable behavior changes.
5. Run the affected language test and, where practical, all three test runners.
6. Open a pull request using the repository template.
7. Merge with **Squash and merge** only after all required CI checks pass.
8. Delete the merged branch.

Suggested branch prefixes:

```text
chore/
feat/
fix/
test/
ci/
docs/
```

## Pull Requests

A pull request should explain:

- what changed
- why the change is needed
- whether the shared behavior changed
- which contract cases were added or updated
- how the change was validated

Changes must not claim support or behavior that has not been implemented and tested.

## Shared Contract

`docs/behavior-specification.md` defines the normative observable behavior.

`tests/cases.tsv` defines the automated language-neutral examples. Equivalent inputs must be accepted or rejected consistently by Java, C++ and Python.

When changing validation or calculation behavior:

1. update the behavior specification when necessary
2. add or update the shared TSV case
3. update all affected language implementations
4. run all three language checks

## Local Tests

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

## Code Style

- Use English for identifiers, comments and technical documentation.
- Keep comments limited to non-obvious implementation details.
- Keep calculation logic separate from terminal input and output.
- Preserve the common behavior defined in `docs/behavior-specification.md`.
- Avoid external dependencies unless they provide a clear and documented benefit.

## Merge Policy

The repository uses pull requests, squash-only merging and linear history. The intended protected-branch policy requires the three CI checks `Java 21`, `C++20` and `Python 3.12` before merge.
