// FRAGMENT SHADER

#version 330 core

//============================================================================
// Received from rasterizer.
//============================================================================
in vec3 ecPosition;   // Fragment's 3D position in eye space.
in vec3 ecNormal;     // Fragment's normal vector in eye space.
in vec3 v2fTexCoord;  // Fragment's texture coordinates. It is 3D when it is
                      //   used as texture coordinates to a cubemap.

//============================================================================
// Indicates which object is being rendered.
// 0 -- draw skybox, 1 -- draw brick cube, 2 -- draw wooden cube.
//============================================================================
uniform int WhichObj;

//============================================================================
// View and projection matrices, etc.
//============================================================================
uniform mat4 ViewMatrix;          // View transformation matrix.
uniform mat4 ModelViewMatrix;     // ModelView matrix.
uniform mat4 ModelViewProjMatrix; // ModelView matrix * Projection matrix.
uniform mat3 NormalMatrix;        // For transforming object-space direction
                                  //   vector to eye space.

//============================================================================
// Light info.
//============================================================================
uniform vec4 LightPosition; // Given in eye space. Can be directional.
uniform vec4 LightAmbient;
uniform vec4 LightDiffuse;
uniform vec4 LightSpecular;

//============================================================================
// Material info.
//============================================================================
// Specular material of the brick surface.
const vec3 BrickSpecularMaterial = vec3(1.0, 1.0, 1.0);

// Material shininess of specular reflection on the brick surface.
const float BrickShininess = 128.0;

// Specular material of the wood surface.
const vec3 WoodSpecularMaterial = vec3(1.0, 1.0, 1.0);

// Material shininess of specular reflection on the wood surface.
const float WoodShininess = 128.0;

//============================================================================
// Environment cubemap used for skybox and reflection mapping.
//============================================================================
uniform samplerCube EnvMap;

//============================================================================
// The brick texture map whose color is used as the ambient and diffuse
// material in the lighting computation.
//============================================================================
uniform sampler2D BrickDiffuseMap;

//============================================================================
// The brick normal map whose color is used as perturbed normal vector
// in the tangent space.
//============================================================================
uniform sampler2D BrickNormalMap;

//============================================================================
// The wood texture map whose color is used as the ambient and diffuse
// material in the lighting computation.
//============================================================================
uniform sampler2D WoodDiffuseMap;

//============================================================================
// MirrorTileDensity defines the number of hemispherical mirrors across each
// dimension when the corresponding texture coordinate ranges from 0.0 to 1.0.
//============================================================================
const float MirrorTileDensity = 2.0;  // (0.0, inf)

//============================================================================
// MirrorRadius is the radius of the hemispherical mirror in each tile.
// The radius is relative to the tile size, which is considered to be 1.0 x 1.0.
//============================================================================
const float MirrorRadius = 0.4;  // (0.0, 0.5]

//============================================================================
// DeltaNormal_Z_Scale is used to exaggerate the height of bump when doing
// normal mapping. The z component of the decoded perturbed normal vector
// read from the normal map is multiplied by DeltaNormal_Z_Adj.
//============================================================================
const float DeltaNormal_Z_Scale = 1.0 / 5.0;

//============================================================================
// Output to color buffer.
//============================================================================
layout (location = 0) out vec4 FragColor;



/////////////////////////////////////////////////////////////////////////////
// Compute fragment color on skybox.
/////////////////////////////////////////////////////////////////////////////
void drawSkybox()
{
    FragColor = texture(EnvMap, v2fTexCoord);
}



/////////////////////////////////////////////////////////////////////////////
// Compute the Tangent vector T and the Binormal vector B, given the
// Normal vector N, a 3D position p, and 2D texture coordinates uv.
// Note that T, B, N and p are all in the same coordinate space.
/////////////////////////////////////////////////////////////////////////////
void compute_tangent_vectors( vec3 N, vec3 p, vec2 uv, out vec3 T, out vec3 B )
{
    // Please refer to "Followup: Normal Mapping Without Precomputed Tangents" at
    // http://www.thetenthplanet.de/archives/1180

    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );

    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    T = normalize( dp2perp * duv1.x + dp1perp * duv2.x );  // Tangent vector
    B = normalize( dp2perp * duv1.y + dp1perp * duv2.y );  // Binormal vector
}



/////////////////////////////////////////////////////////////////////////////
// Compute fragment color on brick cube.
/////////////////////////////////////////////////////////////////////////////
void drawBrickCube()
{
    if (gl_FrontFacing) {
        vec3 viewVec = -normalize(ecPosition);
        vec3 necNormal = normalize(ecNormal);

        vec3 lightVec;
        if (LightPosition.w == 0.0 )
            lightVec = normalize(LightPosition.xyz);
        else
            lightVec = normalize(LightPosition.xyz - ecPosition);

        /////////////////////////////////////////////////////////////////////////////
        // * Construct eye-space Tangent and Binormal vectors.
        // * Read and decode tangent-space perturbation vector from normal map.
        // * Transform perturbation vector to eye space.
        // * Use eye-space perturbation vector as normal vector in lighting
        //   computation using Phong Reflection Model.
        // * Write computed fragment color to FragColor.
        /////////////////////////////////////////////////////////////////////////////

        // Construct eye-space Tangent and Binormal vectors.
        vec3 N = necNormal;
        vec3 T,B;
        compute_tangent_vectors(N,ecPosition,v2fTexCoord.st,T,B);

        // Read and decode tangent-space perturbation vector from normal map.
        vec3 tanPerturbedNormal=vec3(texture(BrickNormalMap,v2fTexCoord.st));
        // Decode:from [0,1] to [-1,1].
        tanPerturbedNormal=tanPerturbedNormal*2.0-1.0;
        // Exaggerate z.
        tanPerturbedNormal.z*=DeltaNormal_Z_Scale;
        // Normalize.
        tanPerturbedNormal=normalize(tanPerturbedNormal);

        // Transform perturbation vector to eye space.
        vec3 ecPerturbedNormal = tanPerturbedNormal.x * T +
        tanPerturbedNormal.y * B +
        tanPerturbedNormal.z * N;

        // Use eye-space perturbation vector as normal vector in lighting
        // computation using Phong Reflection Model.
        vec3 reflectVec = reflect(-lightVec, ecPerturbedNormal);
        float N_dot_L = max(0.0, dot( ecPerturbedNormal, lightVec));
        float R_dot_V = max(0.0, dot( reflectVec, viewVec));
        float spec = (R_dot_V == 0.0)? 0.0 : pow(R_dot_V, BrickShininess);
        vec3 brickDiffuseMaterial = texture(BrickDiffuseMap, v2fTexCoord.st).rgb;

        // Experience value 0.1 for brick material.
        vec3 brickColor = 0.1 * LightAmbient.rgb * brickDiffuseMaterial +
                          LightDiffuse.rgb * (N_dot_L * brickDiffuseMaterial)+
                          LightSpecular.rgb * (spec * BrickSpecularMaterial); 
        // Write computed fragment color to FragColor.
        FragColor = vec4(brickColor,1);  
    }
    else discard; 
}


/////////////////////////////////////////////////////////////////////////////
// Compute fragment color on grass cube.
/////////////////////////////////////////////////////////////////////////////
void drawGrassCube()
{
    if (gl_FrontFacing) {
        vec3 viewVec = -normalize(ecPosition);
        vec3 necNormal = normalize(ecNormal);

        vec3 lightVec;
        if (LightPosition.w == 0.0 )
            lightVec = normalize(LightPosition.xyz);
        else
            lightVec = normalize(LightPosition.xyz - ecPosition);

        float PI = 3.1415926;// PI
        // Compute perturbed normal vector in tangent space of fragment.
        vec2 c = MirrorTileDensity * v2fTexCoord.xy;
        vec2 p = fract( c ) - vec2( 0.5 );
        float sqrDist = p.x * p.x + p.y * p.y;
        // Use Phong reflection model to calculate the Color.
        vec3 reflectVec=reflect(-lightVec, necNormal);
        float N_dot_L = max(0.0, dot(necNormal, lightVec));
        float R_dot_V = max(0.0, dot( reflectVec, viewVec));
        float spec = (R_dot_V == 0.0)? 0.0 : pow(R_dot_V, WoodShininess);

        // Read the wood texture color as the ambient and diffuse material.
        vec3 woodDiffuseMaterial = texture(WoodDiffuseMap, v2fTexCoord.st).rgb;
        vec3 woodColor = LightAmbient.rgb * woodDiffuseMaterial +
                            LightDiffuse.rgb * (N_dot_L * woodDiffuseMaterial)+
                            LightSpecular.rgb * (spec * WoodSpecularMaterial); 

        // Write computed fragment color to FragColor.
        FragColor = vec4(woodColor,1); 

    }
    else discard;
}

/////////////////////////////////////////////////////////////////////////////
// Compute fragment color on grass cube.
/////////////////////////////////////////////////////////////////////////////
void drawRoadCube()
{
    if (gl_FrontFacing) {
        vec3 viewVec = -normalize(ecPosition);
        vec3 necNormal = normalize(ecNormal);

        vec3 lightVec;
        if (LightPosition.w == 0.0 )
            lightVec = normalize(LightPosition.xyz);
        else
            lightVec = normalize(LightPosition.xyz - ecPosition);

        float PI = 3.1415926;// PI
        // Compute perturbed normal vector in tangent space of fragment.
        vec2 c = MirrorTileDensity * v2fTexCoord.xy;
        vec2 p = fract( c ) - vec2( 0.5 );
        float sqrDist = p.x * p.x + p.y * p.y;
        // Use Phong reflection model to calculate the Color.
        vec3 reflectVec=reflect(-lightVec, necNormal);
        float N_dot_L = max(0.0, dot(necNormal, lightVec));
        float R_dot_V = max(0.0, dot( reflectVec, viewVec));
        float spec = (R_dot_V == 0.0)? 0.0 : pow(R_dot_V, WoodShininess);

        // Read the wood texture color as the ambient and diffuse material.
        vec3 woodDiffuseMaterial = texture(BrickDiffuseMap, v2fTexCoord.st).rgb;
        vec3 woodColor = LightAmbient.rgb * woodDiffuseMaterial +
                            LightDiffuse.rgb * (N_dot_L * woodDiffuseMaterial)+
                            LightSpecular.rgb * (spec * WoodSpecularMaterial); 

        // Write computed fragment color to FragColor.
        FragColor = vec4(woodColor,1); 

    }
    else discard;
}

/////////////////////////////////////////////////////////////////////////////
// Compute fragment color on wooden cube.
/////////////////////////////////////////////////////////////////////////////
void drawWoodenCube()
{
    if (gl_FrontFacing) {
        vec3 viewVec = -normalize(ecPosition);
        vec3 necNormal = normalize(ecNormal);

        vec3 lightVec;
        if (LightPosition.w == 0.0 )
            lightVec = normalize(LightPosition.xyz);
        else
            lightVec = normalize(LightPosition.xyz - ecPosition);

        /////////////////////////////////////////////////////////////////////////////
        // * Determine whether fragment is in wood region or mirror region.
        // * If fragment is in wood region,
        //    -- Read from wood texture map.
        //    -- Perform Phong lighting computation using the wood texture
        //       color as the ambient and diffuse material.
        //    -- Write computed fragment color to FragColor.
        // * If fragment is in mirror region,
        //    -- Construct eye-space Tangent and Binormal vectors.
        //    -- Construct tangent-space perturbation vector for a
        //       hemispherical bump.
        //    -- Transform perturbation vector to eye space.
        //    -- Reflect the view vector about the eye-space perturbation vector.
        //    -- Transform reflection vector to World Space.
        //    -- Use world-space reflection vector to access environment cubemap.
        //    -- Write computed fragment color to FragColor.
        /////////////////////////////////////////////////////////////////////////////

        float PI = 3.1415926;// PI
        // Compute perturbed normal vector in tangent space of fragment.
        vec2 c = MirrorTileDensity * v2fTexCoord.xy;
        vec2 p = fract( c ) - vec2( 0.5 );
        float sqrDist = p.x * p.x + p.y * p.y;
        if ( length( p ) > MirrorRadius )
        {
            // Wood Region
            // Use Phong reflection model to calculate the Color.
            vec3 reflectVec=reflect(-lightVec, necNormal);
            float N_dot_L = max(0.0, dot(necNormal, lightVec));
            float R_dot_V = max(0.0, dot( reflectVec, viewVec));
            float spec = (R_dot_V == 0.0)? 0.0 : pow(R_dot_V, WoodShininess);

            // Read the wood texture color as the ambient and diffuse material.
            vec3 woodDiffuseMaterial = texture(WoodDiffuseMap, v2fTexCoord.st).rgb;
            vec3 woodColor = LightAmbient.rgb * woodDiffuseMaterial +
                              LightDiffuse.rgb * (N_dot_L * woodDiffuseMaterial)+
                              LightSpecular.rgb * (spec * WoodSpecularMaterial); 

            // Write computed fragment color to FragColor.
            FragColor = vec4(woodColor,1); 
        }
        else
        {
            // Mirror region
            // Construct eye-space Tangent and Binormal vectors.
            vec3 N = necNormal;
            vec3 T,B;
            compute_tangent_vectors(N,ecPosition,p,T,B);

            //Construct tangent-space perturbation vector for a hemispherical bump.
            vec3 tanPerturbedNormal = normalize( vec3(p.x, p.y, sqrt(MirrorRadius*MirrorRadius-p.x*p.x-p.y*p.y)));

            // Transform perturbation vector to eye space.
            vec3 ecPerturbedNormal = tanPerturbedNormal.x * T +
            tanPerturbedNormal.y * B +
            tanPerturbedNormal.z * N;

            // Reflect the view vector about the eye-space perturbation vector.
            vec3 ecReflectVec=reflect(normalize(ecPosition), ecPerturbedNormal);

            // Transform reflection vector to World Space.
            vec3 wcReflectVec=inverse(NormalMatrix)*ecReflectVec;

            // Use world-space reflection vector to access environment cubemap.
            vec3 reflectColor = vec3(texture(EnvMap, wcReflectVec));
            
            // Write computed fragment color to FragColor.
            FragColor = vec4(reflectColor,1.0);  

        }

    }
    else discard;
}



void main()
{
    switch(WhichObj) {
        case 0: drawSkybox(); break;
        case 1: drawBrickCube(); break;
        case 2: drawWoodenCube(); break;
        case 3: drawGrassCube(); break;
        case 4: drawRoadCube(); break;
        case 5: FragColor = vec4(1,0,0,0.8); break;
        case 6: FragColor = vec4(0,1,0,0.8); break;
        case 7: FragColor = vec4(0,0,1,0.8); break;
    }
}
