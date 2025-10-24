#!/usr/bin/env python3

from pathlib import Path
import re

def process(hex_bytes: bytes) -> bytes:
    current_index: int = 0
    finished: bool = False
    passed_header: bool = False
    passed_palettes: bool = False
    passed_stats: bool = False
    while not finished:
        if not passed_header:
            current_index += 2
            passed_header = True
        elif not passed_palettes:
            palette_count: int = int.from_bytes(hex_bytes[current_index:current_index + 2], "big")
            current_index += 2
            color_count: int = int.from_bytes(hex_bytes[current_index:current_index + 2], "big")
            current_index += 2
            current_index += 3 * color_count * palette_count
            passed_palettes = True
        elif not passed_stats:
            current_index += 28
            passed_stats = True
        else:
            current_index += 2
            sprite_count = int.from_bytes(hex_bytes[current_index:current_index + 2], "big")
            current_index += 2
            for _ in range(sprite_count):
                sprite_length: int = int.from_bytes(hex_bytes[current_index:current_index + 2], "big")
                current_index += 2
                if sprite_length > 0xFF00:
                    current_index += 4
                    copy_attrs: int = sprite_length % 0x100
                    bits: str = format(copy_attrs, '#010b')[2:]
                    for i, bit in enumerate(bits):
                        if bit == '0':
                            match i:
                                case 0:
                                    current_index += 2
                                case 1:
                                    current_index += 8
                                case 2:
                                    current_index += 4
                                case _:
                                    box_count: int = int.from_bytes(hex_bytes[current_index:current_index + 2], "big")
                                    current_index += 2
                                    current_index += box_count * 8
                else:
                    current_index += 12
                    current_box: int = int.from_bytes(hex_bytes[current_index:current_index + 2], "big")
                    current_index += 2
                    while current_box != 0:
                        box_count: int = int.from_bytes(hex_bytes[current_index:current_index + 2], "big")
                        current_index += 2
                        for _ in range(box_count):
                            current_index += 8
                        current_box: int = int.from_bytes(hex_bytes[current_index:current_index + 2], "big")
                        current_index += 2
            finished = True
    return hex_bytes[:current_index]

def convert(data: Path) -> str:
    result: str = process(data.read_bytes()).hex().upper()
    result = re.sub("([0-9A-F]{2})", "\\1 ", result)
    result = re.sub("(([0-9A-F]{2} ){16})", "\\1\\n", result)
    result = result.replace(" \n", "\n")
    result = result.strip()
    result2 = "$$\n"
    for line in result.split("\n"):
        result2 += f"\\texttt{{{line}}}\\\n"
    result2 += "$$"
    return result2

def main() -> None:
    print(convert(Path("data/characters/Debuggy.ff")))

if __name__ == "__main__":
    main()