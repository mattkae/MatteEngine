#version 410

out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform samplerCube ourTexture;

const bool depthenabled = true;

float LinearizeDepth(in vec2 uv)
{
    float zNear = 0.5;    // TODO: Replace by the zNear of your perspective projection
    float zFar  = 1000.0; // TODO: Replace by the zFar  of your perspective projection
    vec2 mapCoord = 2.0 * uv - 1.0;
    float depth = texture(ourTexture, vec3(mapCoord.x, -1, mapCoord.y)).x;
    return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}

void main()
{
  if (depthenabled) {
    float color = LinearizeDepth(TexCoord);
    FragColor = vec4(color, color, color, 1.0);
  }
  else {
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);//texture(ourTexture, vec3(1, 1, 1));
  }
}
