# blender 2.6x
import bpy

current_obj = bpy.context.active_object

out = open('./out.obj', 'w')

for vert in current_obj.data.vertices:
    out.write( 'v %f %f %f\n' % (vert.co.x, vert.co.y, vert.co.z) )
        
for face in current_obj.data.faces:
    out.write('f')
                
    for vert in face.vertices:
            out.write( ' %i' % (vert + 1) )
    out.write('\n')

out.close()