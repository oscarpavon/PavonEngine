import bpy
import sys
import traceback
import math
def import_mesh():
    bpy.ops.object.delete(use_global=False)
    bpy.ops.import_scene.gltf(filepath="../assets/HLOD/out.gltf")
    bpy.ops.transform.rotate(value=math.radians(90),orient_axis='X')
    bpy.ops.object.transform_apply(rotation=True)
    bpy.ops.object.editmode_toggle()
    bpy.ops.mesh.remove_doubles()
    bpy.ops.mesh.tris_convert_to_quads()

try:
    import_mesh()    
except Exception as e:
    traceback.print_exc() 
    print(e)
    print("Error, exit")
    print(Exception)
    sys.exit(1)