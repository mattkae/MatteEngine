struct Light {
    vec3 direction;
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
    float cosineCutOff;
    float dropOff;
    bool usesShadows;
    mat4 shadowMatrix;
};

vec3 getDiffuse(vec3 diffuseProperty, 
    vec3 normal, 
    vec3 lightDir, 
    vec3 color, 
    vec3 diffuse) 
{
    float cosTheta = clamp(dot(normal, lightDir), 0, 1);
    vec3 diffuseFactor = diffuse * diffuseProperty * cosTheta;
    return color * diffuseFactor;
}

vec3 getSpecular(float shininess, 
    vec3 normal, 
    vec3 lightDir, 
    vec3 viewDir, 
    vec3 color, 
    vec3 specular) 
{
    vec3 reflection = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflection), 0.0), shininess);
	vec3 specularFactor = specular * spec;
    return color * spec;
}

vec3 getColorFromLight(const in Light light,
    vec3 normal, 
    vec3 viewDir, 
    vec3 diffuse,
	vec3 diffuseProperty,
    vec3 specular,
	float shininess,
    vec4 fragPos) 
{
    // Calculate the direction, and early out at certain points
    vec3 direction = -light.direction;
    float angleBetween = 1.0;
	vec3 posToFrag = light.position - fragPos.xyz;
    float delta = length(posToFrag);
    posToFrag = normalize(posToFrag);
    if (light.direction == vec3(0)) {
        direction = posToFrag;
	}

	// Check if frag is within spot. Point and Direction have cutoff = 0.0 (so never less)
	angleBetween = min(dot(posToFrag, direction), 1.0);
	if (angleBetween < light.cosineCutOff) {
	   return vec3(0.f);
	}
    
    float normalDotDir = max(0, dot(normal, direction));
    if (normalDotDir == 0) {
        return vec3(0.0);
    }

    // Calculate intensity of the light (Directional and Point have no drop off)
    float attenuation = (light.constant + delta * light.linear + delta * delta * light.quadratic);
    vec3 intensity = (light.color * pow(angleBetween, light.dropOff)) / attenuation;

    // Calculate diffuse and specular
    vec3 diffuseOut = getDiffuse(diffuseProperty, normal, direction, intensity, diffuse);
    vec3 specularOut = getSpecular(shininess, normal, direction, viewDir, intensity, specular);

    return diffuseOut + specularOut;
}

float getOmniVisibility(const in Light light, vec4 fragPos, vec2 uFarNear) {
    vec3 lightToFrag = fragPos.xyz - light.position;

    // Basicaly, we're moving our depth values into the
    // range of depth expected by the light's projection,
    // and then translating those values into device
    // space. See: https://en.wikipedia.org/wiki/Z-buffering#Mathematics
    vec3 absltof = abs(lightToFrag);
    float z = max(absltof.x, max(absltof.y, absltof.z));
    float depth = uFarNear.x + uFarNear.y / z;

    return 0.0; // texture(uOmniShadows[lightIndex], vec4(lightToFrag, depth));
}

float getDirVisibility(const in int lightIndex, vec4 fragPosInLightSpace, sampler2DShadow uDirShadow[MAX_LIGHTS]) {
    float retval = 0;
    int N = 8;
    int twoN = N * 2;

    vec3 lightPosition = (vec3(fragPosInLightSpace.x, 
        fragPosInLightSpace.y, 
        fragPosInLightSpace.z) / fragPosInLightSpace.w) * 0.5 + 0.5;

    if (false) {
       return texture(uDirShadow[lightIndex], lightPosition);
    }

    vec3 transformedVec = vec3(lightPosition);
    float divisor = 0;
    vec2 texelSize = 1.0 / textureSize(uDirShadow[lightIndex], 0);

    for (int x = -N; x <= N; x++) {
        for (int y = -N; y <= N; y++) {
            vec2 transform = vec2(x, y) * texelSize;
            transformedVec.x = lightPosition.x + transform.x;
            transformedVec.y = lightPosition.y + transform.y;
            divisor += 1;
            retval += texture(uDirShadow[lightIndex], transformedVec);
        }
    }
    return retval / divisor;
}