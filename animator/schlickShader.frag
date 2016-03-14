
varying vec3 N;
varying vec3 v;

uniform float N1;
uniform float N2;
void main()
{

	float roughness = 0.3; // fresnel reflectance at normal incidence
	float F0 = pow(((N1 - N2) / (N1 + N2)), 2.0);
	float k = 0.2; // fraction of diffuse reflection

	vec3 lightColor = vec3(0.9, 0.1, 0.1);

	vec3 normal = normalize(N);

	vec3 lightDir  = normalize(gl_LightSource[0].position.xyz);

	float NdotL = max(dot(normal, lightDir), 0.0);

	float specular = 0.0;

	if(NdotL > 0.0)
	{
		vec3 Viewer = -normalize(v);

		vec3 H = normalize(lightDir + Viewer);

		float NdotH = max(dot(normal, H), 0.0);
		float NdotV = max(dot(normal, Viewer), 0.0);

		float VdotH = max(dot(Viewer, H), 0.0);

		float mSquared = roughness * roughness;

		float NH2 = 2.0 * NdotH;
		float g1 = (NH2 * NdotV) / VdotH;
		float g2 = (NH2 * NdotL) / VdotH;
		float atten = min(1.0, min(g1, g2));

		float r1 = 1.0 / (4.0 * mSquared * pow(NdotH, 4.0));
		float r2 = (NdotH * NdotH - 1.0) / (mSquared * NdotH * NdotH);
		float rough = r1 * exp(r2);

		float fresnel = F0 + (1.0 - F0) * pow(1.0 - NdotV, 5.0);

		specular = (fresnel * atten * rough) / (NdotV * NdotL * 3.14);
	}

	vec4 final = (gl_LightSource[0].specular + gl_LightSource[0].ambient + gl_LightSource[0].diffuse) * NdotL * (k + specular * (1.0 - k));
	gl_FragColor = final;
}