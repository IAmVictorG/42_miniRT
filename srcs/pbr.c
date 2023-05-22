#include "../includes/miniRT.h"


t_vec3 reinhard_tone_mapping(t_vec3 color)
{
	color.x = color.x / (1 + color.x);
	color.y = color.y / (1 + color.y);
	color.z = color.z / (1 + color.z);
    return color;
}

/*static float G1(float alpha, t_vec3 N, t_vec3 X)
{
	float numerator = fmax(vec3_dot_product(N, X), 0.0f);

	float k = alpha / 2.0f;
	float denominator = fmax(vec3_dot_product(N, X), 0.0f) *(1.0f * k) + k;
	denominator = fmax(denominator, 0.0000001f); 

	return numerator / fmax(denominator, 0.00000001);
}*/

static float G1(float alpha, t_vec3 N, t_vec3 X)
{
	float NdX = vec3_dot_product(N, X);
	float numerator = 2.0f * NdX;

	float denominator = NdX + sqrtf(alpha * alpha + (1.0f - alpha * alpha) * NdX * NdX);

	return numerator / fmax(denominator, 0.00000001f);
}


static float G(float alpha, t_vec3 N, t_vec3 V, t_vec3 L)
{
	/*float m = fmin(G1(alpha, N, V), G1(alpha, N, L));
	return (fmin(1.0f, m));*/
	return (min(1, G1(alpha, N, V) * G1(alpha, N, L)));
}

static float D(float alpha, t_vec3 N, t_vec3 H)
{
	float numerator = powf(alpha, 2.0f);

	float Ndoth = fmax(vec3_dot_product(N, H), 0.0f);
	float denominator = PI * powf(powf(Ndoth, 2.0f) * (powf(alpha, 2.0f) - 1.0f) + 1.0f, 2.0f);
	denominator = fmax(denominator, 0.0000001f);

	return numerator / denominator;
}

static t_vec3 F(t_vec3 F0, t_vec3 V, t_vec3 H)
{
	return (vec3_add(F0 ,vec3_multiply_scalar(vec3_subtract((t_vec3){1.0f, 1.0f, 1.0f}, F0), powf(1 - fmax(vec3_dot_product(V, H), 0.0f), 5.0f))));
}

t_color PBR(t_utils *utils, t_vec3 F0, t_vec3 V, t_vec3 H, t_payload payload)
{
	t_vec3 light_color = (t_vec3) {(float) utils->scene->lights->color.r, (float) utils->scene->lights->color.g, (float) utils->scene->lights->color.b};

	t_vec3 L = payload.light_direction;
	t_vec3 N = payload.normal;

	float VdN;
	float LdN;

	VdN = vec3_dot_product(V, N);
	LdN = vec3_dot_product(L, N);

	float roughness = 0.7f;
	float alpha = roughness * roughness;
	t_vec3 Ks = F(F0, V, H);
	t_vec3 Kd = vec3_subtract((t_vec3) {1.0f, 1.0f, 1.0f}, Ks);

	t_vec3 color = (t_vec3) {(float) payload.object_color.r, (float) payload.object_color.g, (float) payload.object_color.b};
	t_vec3 lambert = vec3_multiply_scalar(color, 1.0f / PI);
	
	t_vec3 cookTorranceNumerator = vec3_multiply_scalar(F(F0, V, H), D(alpha, N, H) * G(alpha, N, V, L));
	float cookTorranceDenominator = 4.0f * fmax(VdN, 0.0f) * fmax(LdN, 0.0f);
	cookTorranceDenominator = fmax(cookTorranceDenominator, 0.0000001);


	//t_vec3 FR = F(F0, V, H);
	//printf("F %f, %f, %f\n", FR.x, FR.y, FR.z);
	//printf("D %f\n", D(alpha, N, H));
	//printf("G %f\n", G(alpha, N, V, L));

	t_vec3 cookTorrance = vec3_multiply_scalar(cookTorranceNumerator, 1.0f / cookTorranceDenominator);
	//printf("Cook Torrence Numerator %f,%f,%f\n", cookTorranceNumerator.x, cookTorranceNumerator.y, cookTorranceNumerator.z);
	//printf("Cook Torrence denominator %f\n", cookTorranceDenominator);
	//printf("Cook Torrence %f,%f,%f\n", cookTorrance.x, cookTorrance.y, cookTorrance.z);


	t_vec3 BRDF = vec3_add(vec3_multiply(Kd, lambert), cookTorrance);
	//printf("BRDF %f,%f,%f\n", BRDF.x, BRDF.y, BRDF.z);

	t_vec3 result = vec3_multiply_scalar(vec3_multiply(BRDF, light_color), fmax(LdN, 0.0f));

	//printf("result %f,%f,%f\n", result.x, result.y, result.z);
	//printf("Color rendering float %f, %f, %f\n", result.x, result.y, result.z);
	result = reinhard_tone_mapping(result);
	t_color final_color = (t_color){
    	(int)(((result).x) * 255.0f),
    	(int)(((result).y) * 255.0f),
    	(int)(((result).z) * 255.0f)
	};
	//printf("Color rendering int %d, %d, %d\n", (int) final_color.r, (int) final_color.g, (int) final_color.b);

	//return (t_color) {(int) result.x, (int) result.y, (int) result.z};
	return (final_color);
}
