bl_info = {
    "name": "Pavon Engine Addon",
    "category": "Object",
}
import socket
import sys
import bpy

path = "/home/pavon/PavonTheGame/Content/"

def export_current_collection():
    return

def export_current_object():
    new_file_path = path
    new_file_path += bpy.context.active_object.name
    bpy.ops.export_scene.gltf(export_format='GLB', \
    filepath=new_file_path, \
    export_texcoords=True, \
    export_normals=False, export_materials=False, \
    export_selected=True, export_animations=False, \
    export_colors=False, \
    export_skins=False, \
    export_morph=False, \
    export_yup=False
    )
    return

class PavonEngineAddon(bpy.types.Operator):
    """Pavon Engine Addons Test"""
    bl_idname = "object.pavon_engine_addon"
    bl_label = "Pavon Engine"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        print("Fuck yeah")
        export_current_object()
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
       
        s.connect(('localhost', 7654))
        
        message = b'Load the fucking model Pavon Engine'
        message = bytes(bpy.context.active_object.name, 'utf-8')

        s.sendall(message)

        return {'FINISHED'}

addon_keymaps = []

def register():
    bpy.utils.register_class(PavonEngineAddon)
    
    wm = bpy.context.window_manager
    km = wm.keyconfigs.addon.keymaps.new(name='Object Mode', space_type='EMPTY')

    kmi = km.keymap_items.new(PavonEngineAddon.bl_idname, 'E', 'PRESS', shift=True, alt=True)

    addon_keymaps.append(km)

def unregister():
    bpy.utils.unregister_class(PavonEngineAddon)
    
     # handle the keymap
    wm = bpy.context.window_manager
    for km in addon_keymaps:
        wm.keyconfigs.addon.keymaps.remove(km)
    # clear the list
    addon_keymaps.clear()


if __name__ == "__main__":
    register()