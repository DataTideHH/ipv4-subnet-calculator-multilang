from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
import subprocess
import sys


REPOSITORY_ROOT = Path(__file__).resolve().parents[1]


@dataclass(frozen=True)
class Observation:
    returncode: int
    stdout: str
    stderr: str


@dataclass(frozen=True)
class DirectCase:
    name: str
    input_text: str
    expected_returncode: int
    expected_fragment: str
    fragment_stream: str


DIRECT_CASES = (
    DirectCase(
        "empty prefix before invalid octet range",
        "192.168.1.999/",
        1,
        "Error: CIDR prefix is empty.",
        "stderr",
    ),
    DirectCase(
        "empty prefix before wrong octet count",
        "1.2.3/",
        1,
        "Error: CIDR prefix is empty.",
        "stderr",
    ),
    DirectCase(
        "empty prefix before nondigit octet",
        "1.2.3.a/",
        1,
        "Error: CIDR prefix is empty.",
        "stderr",
    ),
    DirectCase(
        "empty prefix before leading-zero octet",
        "192.168.001.1/",
        1,
        "Error: CIDR prefix is empty.",
        "stderr",
    ),
    DirectCase(
        "leading-zero prefix",
        "192.168.1.1/0032",
        1,
        "Error: CIDR prefix must not have leading zeros: 0032",
        "stderr",
    ),
    DirectCase(
        "non-whitespace ASCII control character",
        "\x01192.168.1.1/24",
        1,
        "Error: IPv4 octet must contain only digits:",
        "stderr",
    ),
    DirectCase(
        "non-breaking space is not trimmed",
        "\u00A0192.168.1.1/24",
        1,
        "Error: IPv4 octet must contain only digits:",
        "stderr",
    ),
    DirectCase(
        "ideographic space is not trimmed",
        "\u3000192.168.1.1/24",
        1,
        "Error: IPv4 octet must contain only digits:",
        "stderr",
    ),
    DirectCase(
        "ASCII tab and carriage return are trimmed",
        "\t192.168.1.1/24\r",
        0,
        "Network address:   192.168.1.0",
        "stdout",
    ),
)


INTERACTIVE_CASES = (
    ("ASCII whitespace around quit", "\tq\r\n", ""),
    (
        "Unicode whitespace does not create a quit command",
        "\u00A0q\u00A0\nq\n",
        "Error: Missing '/' separator.",
    ),
)


def cpp_program() -> Path:
    candidates = (
        REPOSITORY_ROOT / "cpp" / "build" / "ipv4_subnet_calculator",
        REPOSITORY_ROOT / "cpp" / "build" / "Release" / "ipv4_subnet_calculator.exe",
        REPOSITORY_ROOT / "cpp" / "build" / "Debug" / "ipv4_subnet_calculator.exe",
    )

    for candidate in candidates:
        if candidate.is_file():
            return candidate

    raise FileNotFoundError(
        "C++ calculator was not found. Build it under cpp/build before running parity tests."
    )


def commands() -> dict[str, list[str]]:
    return {
        "Java 21": [
            "java",
            "-cp",
            str(REPOSITORY_ROOT / "java" / "out"),
            "SubnetCalculator",
        ],
        "C++20": [str(cpp_program())],
        "Python 3.12": [
            sys.executable,
            str(REPOSITORY_ROOT / "python" / "subnet_calculator.py"),
        ],
    }


def observe(
    command: list[str],
    *,
    argument: str | None = None,
    stdin: str | None = None,
) -> Observation:
    arguments = [*command]
    if argument is not None:
        arguments.append(argument)

    completed = subprocess.run(
        arguments,
        input=stdin,
        capture_output=True,
        check=False,
        text=True,
        encoding="utf-8",
    )
    return Observation(completed.returncode, completed.stdout, completed.stderr)


def assert_equal_observations(
    name: str,
    observations: dict[str, Observation],
) -> Observation:
    iterator = iter(observations.items())
    baseline_language, baseline = next(iterator)

    for language, observation in iterator:
        if observation != baseline:
            raise AssertionError(
                f"{name}: {language} diverged from {baseline_language}\n"
                f"{baseline_language}: {baseline!r}\n"
                f"{language}: {observation!r}"
            )

    return baseline


def verify_direct_cases(programs: dict[str, list[str]]) -> None:
    for case in DIRECT_CASES:
        observations = {
            language: observe(command, argument=case.input_text)
            for language, command in programs.items()
        }
        baseline = assert_equal_observations(case.name, observations)

        if baseline.returncode != case.expected_returncode:
            raise AssertionError(
                f"{case.name}: expected exit code {case.expected_returncode}, "
                f"got {baseline.returncode}"
            )

        stream = baseline.stdout if case.fragment_stream == "stdout" else baseline.stderr
        if case.expected_fragment not in stream:
            raise AssertionError(
                f"{case.name}: expected {case.expected_fragment!r} in "
                f"{case.fragment_stream}, got {stream!r}"
            )


def verify_interactive_cases(programs: dict[str, list[str]]) -> None:
    for name, stdin, expected_stderr_fragment in INTERACTIVE_CASES:
        observations = {
            language: observe(command, stdin=stdin)
            for language, command in programs.items()
        }
        baseline = assert_equal_observations(name, observations)

        if baseline.returncode != 0:
            raise AssertionError(f"{name}: expected exit code 0, got {baseline.returncode}")

        if expected_stderr_fragment not in baseline.stderr:
            raise AssertionError(
                f"{name}: expected {expected_stderr_fragment!r} in stderr, "
                f"got {baseline.stderr!r}"
            )


def main() -> int:
    programs = commands()
    verify_direct_cases(programs)
    verify_interactive_cases(programs)
    print(f"Cross-language direct parity cases passed: {len(DIRECT_CASES)}")
    print(f"Cross-language interactive parity cases passed: {len(INTERACTIVE_CASES)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
