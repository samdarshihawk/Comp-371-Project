#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;
layout (location = 3) in vec2 aUV;


uniform mat4 light_view_proj_matrix;
uniform mat4 model_matrix;

void main()
{
    mat4 scale_bias_matrix = mat4(vec4(0.5, 0.0, 0.0, 0.0),
                                    vec4(0.0, 0.5, 0.0, 0.0),
                                    vec4(0.0, 0.0, 0.5, 0.0),
                                    vec4(0.5, 0.5, 0.5, 1.0));
    gl_Position = 
//                    scale_bias_matrix * // bias the depth map coordinates
                    light_view_proj_matrix * model_matrix * vec4(position, 1.0);
}