#version 460 core

#ifndef PREDEFINED_SHADER
#define PREDEFINED_SHADER

vec2 PixelateFrame(vec2 UV, float noise)
{
    vec2 p = UV.st;
    p.x -= mod(p.x, 1.0 / noise);
    p.y -= mod(p.y, 1.0 / noise);

    return p;
}

vec3 StaticTVNoise(vec2 texcoords, float _clock) {

    vec2 uv = texcoords.st;
    
    float t = _clock +123.; // tweak the start moment
    float ta = t*.654321;
    float tb = t*(ta*.123456);
    
    float c = fract(sin(uv.x*ta+uv.y*tb)*5678.);
    vec3 col = vec3(c);

    return col;
}

vec4 TVScreenEffect(vec2 texcoords, sampler2D texture)
{
    vec2 uv = texcoords.xy;
    uv = uv * 2.0-1.0;
    vec2 offset = abs(uv.yx) / vec2(3, 3);
    uv = uv + uv * offset * offset;
    uv = uv * 0.5 + 0.5;	
    vec4 baseColor = texture2D(texture, uv);
    if (uv.x < 0.0 || uv.y < 0.0 || uv.x > 1.0 || uv.y > 1.0){
        return vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        return baseColor;
    }
}

vec4 KernelEffect(sampler2D uTexture, vec2 vTexcoords, float noise) {
    float offset = 1.0 / noise;

    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0),   // center-left
        vec2( 0.0,    0.0),   // center-center
        vec2( offset,  0.0),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(uTexture, vTexcoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    return vec4(col, 1.0);
}

void GrayScale(out vec4 FragColor)
{
    float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    FragColor = vec4(average, average, average, 1.0);
}

vec3 DrawPointLight_raw(vec3 vNormal, vec3 lightPos, vec3 lightColor, vec3 viewPos, vec3 FragPos, float ambientStrength, float specularStrength, float impactValue) {
    const float constant = 1.0;
    const float linear = 0.09;
    const float quadratic = 0.32;
    float distance = length(lightPos - FragPos);
    float attenuation = 1.0 / (constant + linear * distance +
        quadratic * (distance * distance));

    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), impactValue);
    vec3 specular = specularStrength * spec * lightColor;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 DrawDirectionalLight_raw(vec3 vNormal, vec3 lightColor, vec3 viewPos, vec3 FragPos, vec3 direction, float ambientStrength, float specularStrength, float impactValue) {
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(-direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), impactValue);
    vec3 specular = specularStrength * spec * lightColor;

    return (ambient + diffuse + specular);
}

vec3 DrawSpotLight_raw(vec3 vNormal, vec3 lightColor, vec3 viewPos, vec3 FragPos, vec3 Lposition, vec3 direction, float ambientStrength, float specularStrength, float impactValue){
    vec3 lightDir = normalize(Lposition - FragPos);
    
    // check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-direction)); 
    float cutOff = cos(radians(12.0));
    float epsilon = cutOff - 13.0;
    float intensity = clamp((theta - 13.0) / epsilon, 0.0, 1.0);
    
    vec3 ambient = ambientStrength * lightColor;

    if(theta > cutOff) // remember that we're working with angles as cosines instead of degrees so a '>' is used.
    {    
    
        vec3 norm = normalize(vNormal);
        vec3 lightDir = normalize(-direction);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), impactValue);
        vec3 specular = specularStrength * spec * lightColor; 
        
        // attenuation
        float distance    = length(Lposition - FragPos);

        const float constant = 1.0;
        const float linear = 0.09;
        const float quadratic = 0.32;
        float distance1 = length(Lposition - FragPos);
        float attenuation = 1.0 / (constant + linear * distance1 +
            quadratic * (distance1 * distance)); 

        // ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branche
        diffuse   *= attenuation;
        specular *= attenuation;  

        diffuse *= intensity; 
        specular *= intensity; 
            
        vec3 result = ambient + diffuse + specular;
        return result;
    }
    else 
    {
        // else, use ambient light so scene isn't completely dark outside the spot
        return ambient;
    }
}


vec3 DrawPointLight(sampler2D normalMap, sampler2D diffuseMap, sampler2D specularMap, vec2 texcoord, vec3 lightPos, vec3 lightColor, vec3 viewPos, vec3 FragPos, float ambientStrength, float specularStrength, float impactValue) {
    const float constant = 1.0;
    const float linear = 0.09;
    const float quadratic = 0.32;
    float distance = length(lightPos - FragPos);
    float attenuation = 1.0 / (constant + linear * distance +
        quadratic * (distance * distance));

    vec3 ambient = ambientStrength * vec3(texture(diffuseMap, texcoord)) * lightColor;

    vec3 norm = vec3(texture(normalMap, texcoord));
    norm = normalize(norm * 2.0 - 1.0);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(texture(diffuseMap, texcoord)) * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), impactValue);
    vec3 specular = specularStrength * spec * vec3(texture(specularMap, texcoord)) * lightColor;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 DrawDirectionalLight(sampler2D normalMap, sampler2D diffuseMap, sampler2D specularMap, vec2 texcoord, vec3 lightColor, vec3 viewPos, vec3 FragPos, vec3 direction, float ambientStrength, float specularStrength, float impactValue) {
    vec3 ambient = ambientStrength * vec3(texture(diffuseMap, texcoord)) * lightColor;

    vec3 norm = texture(normalMap, texcoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    vec3 lightDir = normalize(-direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(texture(diffuseMap, texcoord)) * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), impactValue);
    vec3 specular = specularStrength * spec * vec3(texture(specularMap, texcoord)) * lightColor;

    return (ambient + diffuse + specular);
}

#endif