import sys
import bpy

argv = sys.argv
argv = argv[argv.index("--") + 1:]  # get all args after "--"

print(argv)  # --> ['example', 'args', '123']
new_file_path = "../generated_glTF_files/"
new_file_path += argv[0]
print(new_file_path)

bpy.ops.export_scene.gltf(export_format='GLTF_EMBEDDED', \
filepath=new_file_path, \
export_texcoords=True, \
export_normals=False, export_materials=False, \
export_selected=True, export_animations=False, \
export_colors=False, \
export_skins=False, \
export_morph=False, \
export_yup=False
)

