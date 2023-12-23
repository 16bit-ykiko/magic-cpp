import os.path as path
import argparse

from script.struct_bind import *
from script.template import *

parser = argparse.ArgumentParser(description='Description of your script.')
parser.add_argument('--fields', '-f', type=int, default=32,
                    help="the max of supported fields")
parser.add_argument('--templates', '-t', type=int, default=4,
                    help="the max of supported template parameters")

args = parser.parse_args()
src = path.join(path.dirname(__file__), "include/magic/generate")


def main():
    with open(path.join(src, "struct_bind_of_field_types.code"), "w") as f:
        f.write(generate_struct_bind_of_field_types(args.fields))

    with open(path.join(src, "struct_bind_of_field_access.code"), "w") as f:
        f.write(generate_struct_bind_of_field_access(args.fields))

    with open(path.join(src, "template_parse.code"), "w") as f:
        f.write(generate_template_parse(args.templates))


if __name__ == "__main__":
    main()
