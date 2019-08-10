import bpy
import sys
try:
    bpy.ops.import_scene.gltf(filepath="../assets/HLOD/out.gltf")
except Exception:
    sys.exit(1)