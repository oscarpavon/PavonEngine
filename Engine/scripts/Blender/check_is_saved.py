import bpy
import sys

def run():
    print("hello from test")
    if bpy.data.is_dirty:        
        print("Dirty")
    else:
        print("No dirty")
        bpy.wm.quit_blender()
        

    if not bpy.data.is_saved:
        print("Saved")
try:
    run()
except Exception:
    print("Test exception")
    sys.exit(1)

