# Contributing

This is a small personal learning and portfolio project. Changes should remain focused on the documented IPv4 subnet calculator scope.

## Workflow

1. Start from the current `main` branch.
2. Create a short-lived branch with a descriptive name.
3. Make one coherent change.
4. Validate the affected implementation and documentation locally.
5. Open a pull request using the repository template.
6. Merge with **Squash and merge** after validation.
7. Delete the merged branch.

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
- how it was validated

Changes should not claim support or behavior that has not been implemented and tested.

## Code Style

- Use English for identifiers, comments and technical documentation.
- Keep comments limited to non-obvious implementation details.
- Keep calculation logic separate from terminal input and output.
- Preserve the common behavior defined in `docs/behavior-specification.md`.
- Avoid external dependencies unless they provide a clear and documented benefit.

## Merge Policy

The intended repository policy is squash-only merging. Merge commits and rebase merging should remain disabled in the repository settings.
