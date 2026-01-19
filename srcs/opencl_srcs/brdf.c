/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brdf.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:38 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/17 16:25:23 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Original formulation of G_SmithGGX Correlated
// lambda_v = ( -1 + sqrt ( alpha_g2 * (1 - n_dot_l2 ) / n_dot_l2 + 1)) * 0.5 f;
// lambda_l = ( -1 + sqrt ( alpha_g2 * (1 - n_dot_v2 ) / n_dot_v2 + 1)) * 0.5 f;
// g_smith_ggx_correlated = 1 / (1 + lambda_v + lambda_l );
// V_SmithGGXCorrelated = g_smith_ggx_correlated / (4.0 f * n_dot_l * n_dot_v );

// This is the optimize version
// float alpha_g2 = alpha_g * alpha_g;
// // Caution : the " n_dot_l *" and " n_dot_v *" are explicitely inversed ,
// this is not a mistake .
// float lambda_ggxv = n_dot_o * sqrt((-n_dot_i * alpha_g2 + n_dot_i) * n_dot_i
// + alpha_g2);
// float lambda_ggxl = n_dot_i * sqrt((-n_dot_o * alpha_g2 + n_dot_o) * n_dot_o
// + alpha_g2);
//
// return (0.5f / (lambda_ggxv + lambda_ggxl));
float	g_smith_ggx_correlated(float n_dot_i, float n_dot_o, float alpha_g)
{
	float	alpha_g2;
	float	lambda_ggxv;
	float	lambda_ggxl;

	alpha_g2 = alpha_g * alpha_g;
	lambda_ggxv = n_dot_i * sqrt(alpha_g2 + n_dot_o * n_dot_o * (1.0f
				- alpha_g2));
	lambda_ggxl = n_dot_o * sqrt(alpha_g2 + n_dot_i * n_dot_i * (1.0f
				- alpha_g2));
	return (0.5f / (lambda_ggxv + lambda_ggxl));
}

// printf("%f %f %f\n", denom, n_dot_hSqr, Tann_dot_hSqr);
// denom = n_dot_hSqr * (alpha_sqr - 1.0f) + 1.0f;
// return (alpha_sqr / (M_PI * denom * denom));
float	ndf_ggx(float alpha_sqr, float n_dot_h)
{
	float	denom;

	denom = n_dot_h * n_dot_h * (alpha_sqr - 1.0f) + 1.0f;
	return (alpha_sqr * (float) M_1_PI / (denom * denom));
}

// g1 = 2.0f * n_dot_h / (n_dot_h + k);
float	geometric_smith(float3 in, float n_dot_l, float alpha_sqr,
		float3 normal, float n_dot_v)
{
	float	k;
	float	g1;
	float	g2;

	k = alpha_sqr;
	g1 = n_dot_l / (n_dot_l * (1.0f - k) + k);
	g2 = n_dot_v / (n_dot_v * (1.0f - k) + k);
	return (g1 * g2);
}

float3	freshnel_schlick(float3 f_0, float n_dot_h)
{
	return (f_0 + (1.0f - f_0) * pow(1.0f - n_dot_h, 5.0f));
}

// float n_dot_h;
// Calculate NDF (Normal Distribution Function - GGX)
// n_dot_h = max(dot(normal, halfDir), 0.0f);
// printf("D = %f, G = %f, F = %f %f %f\n", D, G, F[0], F[1], F[2]);
// return (D * G);
float3	cook_torrance_brdf(float3 in, float3 out, float3 normal, float n_dot_h,
		t_object *hit_object)
{
	float	d;
	float	g;
	float3	f;
	float	n_dot_l;
	float	n_dot_v;

	n_dot_l = max(dot(normal, out), 0.0f);
	n_dot_v = -max(dot(normal, in), 0.0f);
	d = ndf_ggx(hit_object->roughness_sqr, n_dot_h);
	g = geometric_smith(out, n_dot_l, hit_object->roughness_sqr, normal,
			n_dot_v);
	f = freshnel_schlick(hit_object->f_0, n_dot_h);
	return ((d * g) / (4.0f * n_dot_l * n_dot_v + 1e-5f));
}

float3	lambertian_brdf(float3 albedo)
{
	return (albedo * (float) M_1_PI);
}

// {in : incoming ray}, {out : outgoing ray},
// {normal : normal vector of surface}
// float3 BRDF(float3 in, float3 out, float3 normal, t_object *hit_object)
// {
//  float3 halfDir;
//  float3 diffuse;
//  float3 specular;
//  float3 freshnel;
//  float n_dot_h;
//
//  in = fast_normalize(in);  // Normalize incoming ray
//  out = fast_normalize(out); // Normalize outgoing ray
//  // hit_object->f_0 = mix((float3)(0.04f), hit_object->diffuse_albedo,
// hit_object->metallic);
//  // printf("%f %f %f\n", in[0], in[1], in[2]);
//  if (hit_object->mat_type == 'D')
//    return (lambertian_brdf(hit_object->diffuse_albedo));
//  diffuse = lambertian_brdf((1 - hit_object->metallic)
// * hit_object->diffuse_albedo);
//   // if (hit_object->mat_type == 'D')
//   //   return (lambertian_brdf(hit_object->albedo));
//  /*printf("Metallic: %f, Roughness: %f, F0: (%f, %f, %f)\n",*/
//  /*     hit_object->metallic, hit_object->roughness_sqr,*/
//  /*     hit_object->f_0.x, hit_object->f_0.y, hit_object->f_0.z);*/
//   if (hit_object->mat_type == 'M')
//  {
//    halfDir = fast_normalize(in + out);
//    n_dot_h = max(dot(normal, halfDir), 0.0f);
//    freshnel = freshnel_schlick(hit_object->f_0, n_dot_h);
//    specular = cook_torrance_brdf(in, out, normal, n_dot_h, hit_object);
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
