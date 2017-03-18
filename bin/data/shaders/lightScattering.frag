#version 410

uniform sampler2D u_src;
uniform vec2 u_screenSpaceLightPos;

uniform float u_density;
uniform float u_weight;
uniform float u_decay;
uniform float u_exposure;
uniform int u_sampleNum;

in vec2 v_texcoord;
in vec2 vUv;

out vec4 fragcolor;


vec3 LightScattering(
             float density,
             float weight,
             float decay,
             float exposure,
             int numSamples,
             sampler2D occlusionTexture,
             vec2 screenSpaceLightPos,
             vec2 uv
             ) {
    
    vec3 fragColor = vec3(0.0);
    
    screenSpaceLightPos = vec2(screenSpaceLightPos.x, 1.0 - screenSpaceLightPos.y);
    vec2 deltaTextCoord = vec2( uv - screenSpaceLightPos );
    
    vec2 textCoo = uv.xy ;
    deltaTextCoord *= (1.0 /  float(numSamples)) * density;
    float illuminationDecay = 1.0;
    
    vec3 samp = vec3(0.0);
    for(int i = 0; i < numSamples ; i++) {
        
        
        textCoo -= deltaTextCoord;
        samp = texture(occlusionTexture, textCoo).xyz;
        samp *= illuminationDecay * weight;
        fragColor += samp;
        illuminationDecay *= decay;
    }
    
    
    vec3 result = fragColor * exposure;
    
    return result;
}

void main() {
    vec3 godray = LightScattering(u_density, u_weight, u_decay, u_exposure, u_sampleNum, u_src, u_screenSpaceLightPos, v_texcoord);
    fragcolor = vec4(godray, 1.0);
}
