#version 330 core
//in type in_variable_name;
//out type out_variable_name;
//uniform type uniform_name;
// vecn - floats
//bvecn - booleans
//ivecn - integers
//uvecn - unsigned integers
//dvecn - double floats

// Outputs for the fragment shader can be multiple vectors
// Locations should be explicit, if you only have one output it's okay to leave it unset
  
void main()
{
    // set the output variable!
    // Inputs and Outputs depend on shader type and what you configure for attributes
    // https://www.khronos.org/opengl/wiki/Vertex_Shader
    // https://www.khronos.org/opengl/wiki/Fragment_Shader
    // swizzling: allows you to construct new vectors from components of another vector
    vec2 someVec = vec2(1.0, 2.0);
    vec4 differentVec = someVec.xyxx;
    vec3 anotherVec = differentVec.zyw;
    vec4 otherVec = someVec.xxxx + anotherVec.yxzy;
    // make sure the components actually exist! (e.g. vec2 only has xy/rg/st)
    // vectors used as parameters are expanded
    vec2 vect = vec2(0.5, 0.7);
    vec4 result = vec4(vect, 0.0, 0.0);
    vec4 otherResult = vec4(result.xyz, 1.0);
}
