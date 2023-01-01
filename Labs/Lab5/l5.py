#!/usr/bin/env python3
   
# Example usage: ./l5.py int rand inc

import sys
import subprocess
from enum import Enum
import tempfile


class SortingType(str, Enum):
    rand = "rand"
    inc = "inc"
    dec = "dec"


class DataType(str, Enum):
    char = "char"
    int = "int"
    double = "double"


def generate_text_filename(sorting_type: SortingType, data_type: DataType) -> str:
    return f"{sorting_type.value}_{data_type.value}.txt"


def main(data_type: DataType, orig_sort: SortingType, dest_sort: SortingType) -> int:
    fn = {DataType.char: str, DataType.int: int, DataType.double: float}
    input_fn = generate_text_filename(orig_sort, data_type)
    input_lines = open(input_fn).read().rstrip().split("\n")
    input_dict = {}
    for line in input_lines:
        k, v = line.split("=")
        input_dict[k] = fn[data_type](v)
    output_fn = generate_text_filename(dest_sort, data_type)
    try:
        print(
            "$",
            " ".join(["./l5", input_fn, dest_sort.value]),
            file=sys.stderr,
            flush=True,
        )
        tf = tempfile.SpooledTemporaryFile()
        subprocess.run(
            ["./l5", input_fn, dest_sort.value],
            check=True,
            stdout=tf,
            stderr=sys.stderr,
        )
        tf.seek(0)
        open(output_fn, "wb").write(tf.read())
    except subprocess.CalledProcessError as e:
        print(f"Program exit with non zero value {e.returncode}!")
        return e.returncode
    except FileNotFoundError:
        print(f"Binary file ./l5 not found!")
        return -1
    try:
        output_lines = open(output_fn).read().rstrip().split("\n")
    except FileNotFoundError:
        print(f"Output file {output_fn} not found!")
        return -1
    if len(input_lines) != len(output_lines):
        print(f"Line number {len(input_lines)} vs {len(output_lines)} not match!")
        return 0
    output_dict = {}
    for line in output_lines:
        try:
            k, v = line.split("=")
            output_dict[k] = fn[data_type](v)
        except:
            print(f"Line '{line}' wrong format")
            return 0
    if dest_sort == SortingType.rand:
        for k, v in input_dict.items():
            if output_dict.get(k) != v:
                print(f"{k}={v} not match!")
                return 0
        shuffled = False
        for k1, k2 in zip(input_dict, output_dict):
            if k1 != k2:
                shuffled = True
                break
        if not shuffled:
            print(f"File content not change!")
            return 0
    else:
        input_dict = {
            k: v
            for k, v in sorted(
                input_dict.items(),
                key=lambda item: item[1],
                reverse=dest_sort == SortingType.dec,
            )
        }
        for k1, k2 in zip(input_dict, output_dict):
            if k1 != k2 or input_dict[k1] != output_dict.get(k2):
                print(f"{k1}={input_dict[k1]} not match!")
                return 0
    print("Correct!")
    return 0

if __name__ == "__main__":
    if len(sys.argv) == 1:
        print("Free 10 points to make the total score 100.", file=sys.stderr)
        print("Correct!")
        exit(0)
    data_type = DataType(sys.argv[1])
    orig_sort = SortingType(sys.argv[2])
    dest_sort = SortingType(sys.argv[3])
    exit_code = main(data_type, orig_sort, dest_sort)
    exit(exit_code)