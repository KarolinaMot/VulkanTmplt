#version 450
layout(location = 0) in vec3 forwards;

 layout( set = 1, binding = 1 ) uniform samplerCube Cubemap;

 layout( location = 0 ) out vec4 frag_color;

 void main() {
	//frag_color = texture(material, forwards);
	frag_color = vec4(1.f);
 }