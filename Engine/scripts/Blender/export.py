import bpy
bpy.ops.export_scene.gltf(export_format='GLTF_EMBEDDED', \
filepath="../assets/HLOD/out.gltf", \
export_texcoords=True, \
export_normals=False, export_materials=False, \
export_selected=True, export_animations=False, \
export_colors=False, \
export_skins=False, \
export_morph=False, \
)

