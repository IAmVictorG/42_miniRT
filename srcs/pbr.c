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

/*static float G1(float alpha, t_vec3 N, t_vec3 X)
{
	float NdX = vec3_dot_product(N, X);
	float numerator = 2.0f * NdX;

	float denominator = NdX + sqrtf(alpha * alpha + (1.0f - alpha * alpha) * NdX * NdX);

	return numerator / fmax(denominator, 0.00000001f);
}*/

static float G1(float k, t_vec3 n, t_vec3 v)
{
	float ndv;
	float numerator;
	float denominator;

	ndv = vec3_dot_product(n, v);
	numerator = ndv;
	denominator = ndv * (1.0f - k) + k;
	return (numerator / denominator);
}

static float G(float k, t_vec3 N, t_vec3 V, t_vec3 L)
{
	/*float m = fmin(G1(alpha, N, V), G1(alpha, N, L));
	return (fmin(1.0f, m));*/
	return (fmin(1.0f, G1(k, N, V) * G1(k, N, L)));
}

static float D(float alpha, t_vec3 N, t_vec3 H)
{
	float numerator = powf(alpha, 2.0f);

	float Ndoth = fmax(vec3_dot_product(N, H), 0.0f);
	float denominator = (float) PI * powf(powf(Ndoth, 2.0f) * (powf(alpha, 2.0f) - 1.0f) + 1.0f, 2.0f);
	denominator = fmax(denominator, 0.0000001f);

	return numerator / denominator;
}

static t_vec3 F(t_vec3 F0, t_vec3 V, t_vec3 H) // F Schlick
{
	return (vec3_add(F0 ,vec3_multiply_scalar(vec3_subtract((t_vec3){1.0f, 1.0f, 1.0f}, F0), powf(1.0f - fmax(vec3_dot_product(H, V), 0.0f), 5.0f))));
}

t_vec3 PBR(t_utils *utils, t_material material, t_vec3 V, t_vec3 H, t_payload payload)
{
	(void) material;
	t_vec3 light_color = utils->scene->lights->color;
	t_vec3 ambient = utils->scene->alight->color;
	t_vec3 L = payload.light_direction;
	t_vec3 N = payload.normal;
	(void) ambient;

	float VdN;
	float LdN;

	t_vec3 F0 = (t_vec3) {0.03f, 0.03f, 0.03f};
	VdN = vec3_dot_product(V, N);
	LdN = vec3_dot_product(L, N);

	float roughness = 0.2f;
	float alpha = roughness * roughness;
	t_vec3 Ks = F(F0, V, H);
	t_vec3 Kd = vec3_subtract((t_vec3) {1.0f, 1.0f, 1.0f}, Ks);

	t_vec3 color = payload.object_color;
	t_vec3 lambert = vec3_multiply_scalar(color, 1.0f / (float) PI);
	
	t_vec3 cookTorranceNumerator = vec3_multiply_scalar(F(F0, V, H), D(alpha, N, H) * G(alpha, N, V, L));
	float cookTorranceDenominator = 4.0f * fmax(VdN, 0.0f) * fmax(LdN, 0.0f);
	cookTorranceDenominator = fmax(cookTorranceDenominator, 0.0000001f);

	t_vec3 cookTorrance = vec3_multiply_scalar(cookTorranceNumerator, 1.0f / cookTorranceDenominator);



	t_vec3 BRDF = vec3_add(vec3_multiply(Kd, lambert), cookTorrance);

	t_vec3 result = vec3_multiply_scalar(vec3_multiply(BRDF, light_color), fmax(LdN, 0.0f));

	return (result);
}


t_vec3 mix(t_vec3 x, t_vec3 y, float a) {
    t_vec3 result;

    result.x = (1.0f - a) * x.x + a * y.x;
    result.y = (1.0f - a) * x.y + a * y.y;
    result.z = (1.0f - a) * x.z + a * y.z;

    return result;
}


/*t_vec3 PBR(t_utils *utils, t_vec3 F0, t_vec3 V, t_vec3 H, t_payload payload)
{
	t_vec3 ambient = (t_vec3) {(float) utils->scene->alight->color.r / 255.0f, (float) utils->scene->alight->color.g / 255.0f, (float) utils->scene->alight->color.b / 255.0f};
	F0 = mix(F0, (t_vec3) {(float) payload.object_color.r / 255.0f, (float) payload.object_color.g / 255.0f, (float) payload.object_color.b / 255.0f}, 0.0f);
	float intensity = 1.0f;
	float l_dist = payload.hit_distance;
	intensity = utils->scene->lights->intensity / (l_dist*l_dist);


}*/
