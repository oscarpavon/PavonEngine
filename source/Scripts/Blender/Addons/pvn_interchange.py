bl_info = {
    "name": "Pavon Engine Addon",
    "category": "Object",
}
import socket
import sys
import bpy

path = "/home/pavon/PavonTheGame/Content/"

        
def export_current_collection():
    bpy.ops.object.select_all(action='DESELECT')
    collection = bpy.context.collection
    all_object = collection.all_objects
    for object in all_object:
        object.select_set(True)
    #new_file_path += bpy.context.active_object.name
    new_file_path = path
    new_file_path += collection.name
    export_selects_objects(new_file_path)
    send_to_PavonEditor(collection.name)
    return

def export_selects_objects(path):
    
    bpy.ops.export_scene.gltf(export_format='GLB', \
    filepath=path, \
    export_texcoords=True, \
    export_normals=False, export_materials=False, \
    export_selected=True, export_animations=False, \
    export_colors=False, \
    export_skins=False, \
    export_morph=False, \
    export_yup=False
    )
    return

def send_to_PavonEditor(send_path):

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   
    s.connect(('localhost', 7654))    

    message = bytes(send_path, 'utf-8')

    s.sendall(message)
    return

class PavonEngineAddon(bpy.types.Operator):
    """Pavon Engine Addons Test"""
    bl_idname = "object.pavon_engine_addon"
    bl_label = "Pavon Engine"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
#       export_and_send_to_PavonEditor()
        export_current_collection()

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