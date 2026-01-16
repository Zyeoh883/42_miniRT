/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brdf.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:38 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/16 22:44:00 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Original formulation of G_SmithGGX Correlated
// lambda_v = ( -1 + sqrt ( alphaG2 * (1 - NdotL2 ) / NdotL2 + 1)) * 0.5 f;
// lambda_l = ( -1 + sqrt ( alphaG2 * (1 - NdotV2 ) / NdotV2 + 1)) * 0.5 f;
// G_SmithGGXCorrelated = 1 / (1 + lambda_v + lambda_l );
// V_SmithGGXCorrelated = G_SmithGGXCorrelated / (4.0 f * NdotL * NdotV );

// This is the optimize version
// float alphaG2 = alphaG * alphaG;
// // Caution : the " NdotL *" and " NdotV *" are explicitely inversed ,
	// this is not a mistake .
// float Lambda_GGXV = n_dot_o * sqrt((-n_dot_i * alphaG2 + n_dot_i) * n_dot_i
		// + alphaG2);
// float Lambda_GGXL = n_dot_i * sqrt((-n_dot_o * alphaG2 + n_dot_o) * n_dot_o
		// + alphaG2);
//
// return (0.5f / (Lambda_GGXV + Lambda_GGXL));
float	G_SmithGGXCorrelated(float n_dot_i, float n_dot_o, float alphaG)
{
	float	alphaG2;
	float	Lambda_GGXV;
	float	Lambda_GGXL;

	alphaG2 = alphaG * alphaG;
	Lambda_GGXV = n_dot_i * sqrt(alphaG2 + n_dot_o * n_dot_o * (1.0f
				- alphaG2));
	Lambda_GGXL = n_dot_o * sqrt(alphaG2 + n_dot_i * n_dot_i * (1.0f
				- alphaG2));
	return (0.5f / (Lambda_GGXV + Lambda_GGXL));
}

float	NDF_GGX(float alphaSqr, float NdotH)
{
	float	denom;

	denom = NdotH * NdotH * (alphaSqr - 1.0f) + 1.0f;
	// printf("%f %f %f\n", denom, NdotHSqr, TanNdotHSqr);
	return (alphaSqr * INV_PI / (denom * denom));
	// denom = NdotHSqr * (alphaSqr - 1.0f) + 1.0f;
	// return (alphaSqr / (M_PI * denom * denom));
}

float	geometric_smith(float3 in, float NdotL, float alphaSqr, float3 normal,
		float NdotV)
{
	float	k;
	float	G1;
	float	G2;

	k = alphaSqr;
	// G1 = 2.0f * NdotH / (NdotH + k);
	G1 = NdotL / (NdotL * (1.0f - k) + k);
	G2 = NdotV / (NdotV * (1.0f - k) + k);
	return (G1 * G2);
}

float3	freshnel_schlick(float3 F_0, float NdotH)
{
	return (F_0 + (1.0f - F_0) * pow(1.0f - NdotH, 5.0f));
}

float3	cook_torrance_BRDF(float3 in, float3 out, float3 normal, float NdotH,
		t_object *hit_object)
{
	float	D;
	float3	F;
	float	G;
	float	NdotL;
	float	NdotV;

	// float NdotH;
	// Calculate NDF (Normal Distribution Function - GGX)
	// NdotH = max(dot(normal, halfDir), 0.0f);
	NdotL = max(dot(normal, out), 0.0f);
	NdotV = -max(dot(normal, in), 0.0f);
	D = NDF_GGX(hit_object->roughness_sqr, NdotH);
	// Calculate Geometry term (Smith's method with Schlick-GGX)
	G = geometric_smith(out, NdotL, hit_object->roughness_sqr, normal, NdotV);
	// Calculate Fresnel term (Schlick's approximation)
	F = freshnel_schlick(hit_object->F_0, NdotH);
	// printf("D = %f, G = %f, F = %f %f %f\n", D, G, F[0], F[1], F[2]);
	// Calculate final BRDF
	return ((D * G) / (4.0f * NdotL * NdotV + 1e-5f));
	// return (D * G);
}

float3	lambertian_BRDF(float3 albedo)
{
	return (albedo * INV_PI); // dividing by (float)M_PI gets too dark
}

// {in : incoming ray}, {out : outgoing ray},
	{normal : normal vector of surface}
// float3 BRDF(float3 in, float3 out, float3 normal, t_object *hit_object)
// {
//  float3 halfDir;
//  float3 diffuse;
//  float3 specular;
//  float3 freshnel;
//  float NdotH;
//
//  in = fast_normalize(in);  // Normalize incoming ray
//  out = fast_normalize(out); // Normalize outgoing ray
//  // hit_object->F_0 = mix((float3)(0.04f), hit_object->diffuse_albedo,
		// hit_object->metallic);
//  // printf("%f %f %f\n", in[0], in[1], in[2]);
//  if (hit_object->mat_type == 'D')
//    return (lambertian_BRDF(hit_object->diffuse_albedo));
//  diffuse = lambertian_BRDF((1 - hit_object->metallic)
		// * hit_object->diffuse_albedo);
//   // if (hit_object->mat_type == 'D')
//   //   return (lambertian_BRDF(hit_object->albedo));
//  /*printf("Metallic: %f, Roughness: %f, F0: (%f, %f, %f)\n",*/
//  /*     hit_object->metallic, hit_object->roughness_sqr,*/
//  /*     hit_object->F_0.x, hit_object->F_0.y, hit_object->F_0.z);*/
//   if (hit_object->mat_type == 'M')
//  {
//    halfDir = fast_normalize(in + out);
//    NdotH = max(dot(normal, halfDir), 0.0f);
//    freshnel = freshnel_schlick(hit_object->F_0, NdotH);
//    specular = cook_torrance_BRDF(in, out, normal, NdotH, hit_object);
//
//    // printf("%f %f %f\n", freshnel[0], freshnel[1], freshnel[2]);
//
//    // return (specular);
//    float3 kD = (1.0f - freshnel) * (1.0f - hit_object->metallic);
//    return (kD * diffuse + freshnel * specular);
//    return (mix(diffuse, specular, freshnel));
//    // return ((1.0f - freshnel) * diffuse + freshnel * specular);
//    // ambert (diffuse) + Cook-Torrance (specular)
//  }
//   return (diffuse);
//
// }
//
