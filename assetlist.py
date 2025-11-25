import os
from collections import defaultdict

sprite_h = ""
sprite_c = ""
sfx_h = ""
sfx_c = ""

def code_h():
	return f"""
#pragma once
#include "main.h"

{sprite_h}

{sfx_h}

""".strip()

def code_c():
	return f"""
#include "gen_assets.h"

{sprite_c}

{sfx_c}

""".strip()

def sanitize(i) -> str:
	i = i.replace("assets/", "")
	i, ext = os.path.splitext(i)
	return i.replace("/", "_").replace(" ", "_").replace("(", "").replace(")", "").replace("-", "_").upper(), ext

def write():
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

def process_sounds(list):
	global sfx_h, sfx_c
	sfx_h = f"#define SFX_PATH_COUNT {len(list)}\n\n"

	n = 0
	for id in list:
		sfx_h += f"#define SFX_{id} {n} \n"
		n += 1
	pass

def main():
	texture_id = {}
	sfx_id = defaultdict(list)

	for path, subdirs, files in os.walk("assets"):
		for name in files:
			fullpath = os.path.join(path, name).replace("\\", "/")
			id, ext = sanitize(fullpath)
			id2, ext2 = sanitize(os.path.dirname(fullpath))

			if name.endswith(".png"):
				texture_id[id] = fullpath
			elif name.endswith(".wav") or name.endswith(".ogg"):
				sfx_id[id2].append(fullpath)

	process_textures(texture_id)
	process_sounds(sfx_id)

if __name__ == "__main__":
	main()
	write()