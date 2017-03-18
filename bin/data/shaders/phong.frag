#version 410 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
} fs_in;

//uniform sampler2D tex;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 u_color;


void main()
{           
    //vec3 color = texture(tex, fs_in.TexCoord).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.3);
    
    // Ambient
    vec3 ambient = 0.15 * vec3(1.0);
    
    // Diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    
    
    float dist = length(lightPos - fs_in.FragPos) * 0.007;
    float attenuation = (0.01 * dist) + (0.01 * dist * dist);
    
    vec3 lighting = (ambient + diffuse + specular) * attenuation;
    

    
    
    FragColor = vec4(u_color, 1.0f);
}
