#version 450
 layout( location = 0 ) in vec3 vert_texcoord;

 layout( set = 1, binding = 1 ) uniform samplerCube Cubemap;

 layout( location = 0 ) out vec4 frag_color;

 void main() {
 //frag_color = texture( Cubemap, vert_texcoord );
 frag_color = vec4(1.f);
 }