#!/usr/bin/env python3
"""
##########################################################################################
# Created on:  rewritten from original C version
# File(s):     xor.py
# Version:     0.1 beta
# Author:      JD Durick - change to python
# Description: Simple program using XOR to encrypt/decrypt files with a rolling key.
##########################################################################################
"""

import argparse
import os
import sys


def xor_file(input_file: str, output_file: str, key: str) -> None:
    if not key:
        raise ValueError("Key cannot be empty.")

    key_bytes = key.encode("utf-8")
    key_len = len(key_bytes)
    key_index = 0

    try:
        with open(input_file, "rb") as infile, open(output_file, "wb") as outfile:
            while True:
                chunk = infile.read(4096)
                if not chunk:
                    break

                out_chunk = bytearray()
                for b in chunk:
                    out_chunk.append(b ^ key_bytes[key_index])
                    key_index += 1
                    if key_index == key_len:
                        key_index = 0

                outfile.write(out_chunk)

    except FileNotFoundError:
        print(f"Input file '{input_file}' cannot be read", file=sys.stderr)
        sys.exit(1)
    except PermissionError as e:
        print(f"Permission error: {e}", file=sys.stderr)
        sys.exit(1)
    except OSError as e:
        print(f"File error: {e}", file=sys.stderr)
        sys.exit(1)


def get_file_size(filename: str) -> int:
 
    #Return file size in bytes.
 
    try:
        return os.path.getsize(filename)
    except OSError:
        print("Read Failed", file=sys.stderr)
        sys.exit(1)


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog="xor.py",
        description="Small program to encrypt/decrypt files via a rolling XOR key."
    )
    parser.add_argument(
        "-i",
        "--input",
        required=True,
        help="File to be encrypted or decrypted via XOR key"
    )
    parser.add_argument(
        "-o",
        "--output",
        required=True,
        help="Output file"
    )
    parser.add_argument(
        "-k",
        "--key",
        required=True,
        help="Key used to encrypt or decrypt the file(s)"
    )
    return parser


def main() -> int:
    parser = build_parser()
    args = parser.parse_args()

    xor_file(args.input, args.output, args.key)
    return 0


if __name__ == "__main__":
    sys.exit(main())
