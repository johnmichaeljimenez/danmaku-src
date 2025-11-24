
import os

sprite_h = ""
sprite_c = ""
def code_h():
	return f"""
#pragma once
#include "main.h"

{sprite_h}
""".strip()

def code_c():
	return f"""
#include "gen_assets.h"

{sprite_c}
""".strip()

def write():
	print(code_c())
	with open("src/gen_assets.c", 'w', encoding='utf-8') as file:
		file.write(code_c())

	with open("include/gen_assets.h", 'w', encoding='utf-8') as file:
		file.write(code_h())

def process_textures(list):
	global sprite_h, sprite_c
	sprite_h = f"#define SPRITE_PATH_COUNT {len(list)}\n\n"
	for i in list:
		print(f"{i}: {list[i]}")
		sprite_h += f"#define SPRITE_{i} \"{list[i]}\" \n"
		pass

	sprite_h += f"""

	"""

	sprite_c += f"""

const char* SPRITE_PATHS[] =
{{
"""

	n = 0
	for	i in list:
		sprite_c += f"	SPRITE_{i}, \n"
		n += 1

	sprite_c += "};"

def main():
	texture_id = {}

	for path, subdirs, files in os.walk("assets"):
		for name in files:
			p = os.path.join(path, name)
			id = p.replace("assets\\", "")
			id, ext = os.path.splitext(id)
			id = id.replace("\\", "_").replace(" ", "_").replace("(", "").replace(")", "").replace("-", "_").upper()

			if name.endswith(".png"):
				texture_id[id] = p.replace("\\", "/")

	process_textures(texture_id)

if __name__ == "__main__":
	main()
	write()