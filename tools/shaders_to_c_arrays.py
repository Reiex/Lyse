#!/usr/bin/env python3

import os
import sys


def main(argv):

	assert len(argv) == 3

	shader_folder = argv[1]
	output_path = argv[2]

	output = open(output_path, "w")

	output.write("""\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \\file
//! \\author Reiex
//! \\copyright The MIT License (MIT)
//! \\date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

""")

	for filename in os.listdir(shader_folder):

		assert len(filename.split(".")) == 2

		if filename.split(".")[1] == "vert":
			variable_name = filename.split(".")[0] + "Vert"
		elif filename.split(".")[1] == "tesc":
			variable_name = filename.split(".")[0] + "Tesc"
		elif filename.split(".")[1] == "tese":
			variable_name = filename.split(".")[0] + "Tese"
		elif filename.split(".")[1] == "geom":
			variable_name = filename.split(".")[0] + "Geom"
		elif filename.split(".")[1] == "frag":
			variable_name = filename.split(".")[0] + "Frag"
		elif filename.split(".")[1] == "comp":
			variable_name = filename.split(".")[0] + "Comp"
		else:
			continue

		filepath = os.path.join(shader_folder, filename)

		with open(filepath, "r") as file:

			shader_source = file.read()

			output.write("static constexpr char {}[] = {{".format(variable_name))

			for i, c in enumerate(shader_source):
				if i % 20 == 0:
					output.write("\n\t")
				output.write("0x" + hex(ord(c))[2:].zfill(2) + ", ")

			output.write("\n};\n\n")


if __name__ == "__main__":
	main(sys.argv)