from __future__ import annotations

import csv
from pathlib import Path
import subprocess
import sys
import unittest

from subnet_calculator import calculate


CASES_PATH = Path(__file__).resolve().parents[1] / "tests" / "cases.tsv"
SCRIPT_PATH = Path(__file__).resolve().with_name("subnet_calculator.py")


def load_cases() -> list[list[str]]:
    with CASES_PATH.open(encoding="utf-8", newline="") as source:
        return [
            row
            for row in csv.reader(source, delimiter="\t")
            if row and not row[0].startswith("#")
        ]


class SubnetCalculatorContractTest(unittest.TestCase):
    def test_shared_calculation_cases(self) -> None:
        for case in load_cases():
            self.assertEqual(12, len(case), case)
            kind, name, input_text = case[:3]

            with self.subTest(name=name):
                if kind == "valid":
                    result = calculate(input_text)
                    trimmed_input = input_text.strip()
                    input_ip, prefix_text = trimmed_input.split("/", maxsplit=1)

                    self.assertEqual(input_ip, result.input_ip)
                    self.assertEqual(int(prefix_text), result.prefix)
                    self.assertEqual(case[3], result.subnet_mask)
                    self.assertEqual(case[4], result.wildcard_mask)
                    self.assertEqual(case[5], result.network_address)
                    self.assertEqual(case[6], result.broadcast_address)
                    self.assertEqual(int(case[7]), result.total_addresses)
                    self.assertEqual(int(case[8]), result.usable_hosts)
                    self.assertEqual(case[9], result.first_usable_host)
                    self.assertEqual(case[10], result.last_usable_host)
                    self.assertEqual(case[11], result.note)
                elif kind == "invalid":
                    with self.assertRaises(ValueError) as raised:
                        calculate(input_text)
                    self.assertEqual(case[11], str(raised.exception))
                else:
                    self.fail(f"Unknown case kind: {kind}")

    def test_direct_mode_output_and_exit_codes(self) -> None:
        success = subprocess.run(
            [sys.executable, str(SCRIPT_PATH), "192.168.10.42/24"],
            capture_output=True,
            check=False,
            text=True,
        )
        self.assertEqual(0, success.returncode)
        self.assertIn("Network address:   192.168.10.0", success.stdout)
        self.assertEqual("", success.stderr)

        failure = subprocess.run(
            [sys.executable, str(SCRIPT_PATH), "192.168.1.300/24"],
            capture_output=True,
            check=False,
            text=True,
        )
        self.assertEqual(1, failure.returncode)
        self.assertIn(
            "Error: IPv4 octet out of range (0-255): 300",
            failure.stderr,
        )

    def test_help_mode(self) -> None:
        result = subprocess.run(
            [sys.executable, str(SCRIPT_PATH), "--help"],
            capture_output=True,
            check=False,
            text=True,
        )
        self.assertEqual(0, result.returncode)
        self.assertIn("Usage:", result.stdout)
        self.assertEqual("", result.stderr)

    def test_incorrect_usage(self) -> None:
        result = subprocess.run(
            [
                sys.executable,
                str(SCRIPT_PATH),
                "192.168.10.42/24",
                "extra",
            ],
            capture_output=True,
            check=False,
            text=True,
        )
        self.assertEqual(1, result.returncode)
        self.assertIn("Error: Expected zero or one argument.", result.stderr)
        self.assertIn("Usage:", result.stdout)


if __name__ == "__main__":
    unittest.main()
