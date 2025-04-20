import json
import glob
import sys


def main(files: list[str]):
    for f in files:
        with open(f, "r") as i:
            content = json.load(i)
        with open(f, "w", newline="\n") as o:
            json.dump(
                content,
                o,
                indent=4,
            )


if __name__ == "__main__":
    main(sum([glob.glob(arg) for arg in sys.argv[1:]], []))
