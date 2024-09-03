#shader vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoords;

out vec4 v_Color;
out vec2 v_TexCoords;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProj;
};

void main()
{
    gl_Position = u_ViewProj * vec4(a_Position, 1.0);
    v_Color = a_Color;
    v_TexCoords = a_TexCoords;
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

layout(binding = 0) uniform sampler2D u_Tex;

in vec4 v_Color;
in vec2 v_TexCoords;

float screenPxRange() {
    const float pxRange = 2.0; // set to distance field's pixel range
    vec2 unitRange = vec2(pxRange)/vec2(textureSize(u_Tex, 0));
    vec2 screenTexSize = vec2(1.0)/fwidth(v_TexCoords);
    return max(0.5*dot(unitRange, screenTexSize), 1.0);
}

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main()
{
    vec4 texColor = v_Color * texture(u_Tex, v_TexCoords);

    vec3 msd = texture(u_Tex, v_TexCoords).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
    if (opacity == 0.0)
        discard;

    vec4 bgColor = vec4(0.0);
    o_Color = mix(bgColor, v_Color, opacity);
    if (o_Color.a == 0.0)
        discard;
}
