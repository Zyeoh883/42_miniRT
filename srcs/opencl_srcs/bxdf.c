/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bxdf.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:38 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/19 17:24:35 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// unsigned int	wang_hash(unsigned int x)
// {
// 	x = (x ^ 61) ^ (x >> 16);
// 	x = x + (x << 3);
// 	x = x ^ (x >> 4);
// 	x = x * 0x27d4eb2d;
// 	x = x ^ (x >> 15);
// 	return (x);
// }

// LCG step

// TODO: reduce variable count
/* create a local orthogonal coordinate frame centered at the hitpoint */
float3	sample_diffuse(float3 *out, float *pdf, t_args_sample a)
{
	t_sample_diffuse	x;

	x.phi = 2 * (float)M_PI * a.s.x;
	x.sin_theta = sqrt(a.s.y);
	x.cos_theta = sqrt(1.0f - a.s.y);
	*pdf = x.cos_theta * (float) M_1_PI;
	if (*pdf <= 0.0f)
	{
		*pdf = 0.0f;
		return ((float3)(0.0f));
	}
	x.dir = (float3)(cos(x.phi) * x.sin_theta, sin(x.phi) * x.sin_theta,
			x.cos_theta);
	x.axis = select((float3)(1.0f, 0.0f, 0.0f), (float3)(0.0f, 1.0f, 0.0f),
			(int3)(fabs(a.n.x) > 0.001f));
	x.u = normalize(cross(x.axis, a.n));
	x.v = cross(a.n, x.u);
	*out = fast_normalize(x.v * x.dir.x + x.u * x.dir.y + a.n * x.dir.z);
	return (a.diffuse_albedo * (float) M_1_PI);
}

// n_dot_o = dot(*out, normal);
// if (n_dot_o <= 0.0f)
// {
//   *pdf = 0;
//   return (float3)(0.0f);
// }
// Don't apply fresnel here, it's applied in the external function

float3	specular_rough(t_dot x, t_object *hit_object, float *pdf)
{
	float	d;
	float	g;
	float	denominator;

	if (x.n_dot_o <= 0.0f || x.n_dot_h <= 0.0f || x.wi_dot_h <= 0.0f
		|| x.n_dot <= 0.0f)
	{
		*pdf = 0.0f;
		return ((float3)(0.0f));
	}
	d = ndf_ggx(hit_object->roughness_sqr, x.n_dot_h);
	g = g_smith_ggx_correlated(x.n_dot, x.n_dot_o,
			hit_object->roughness_sqr);
	*pdf = d * x.n_dot_h / (4.0f * x.wi_dot_h);
	denominator = 4.0f * x.n_dot * x.n_dot_o + 1e-5f;
	return (d * g * hit_object->specular_albedo / denominator);
}

float3	sample_specular(float3 *out, t_object *hit_object,
			float *pdf, t_args_sample a)
{
	t_dot	x;
	float	d;
	float	g;

	if (hit_object->roughness_sqr <= 1e-4f)
	{
		*out = reflect(a.in, a.n);
		*pdf = 1.0f;
		return (hit_object->specular_albedo);
	}
	x.wh = ggx_sample(a.n, hit_object->roughness_sqr, a.s);
	*out = reflect(a.in, x.wh);
	x.n_dot = dot(a.n, -a.in);
	x.n_dot_o = dot(a.n, *out);
	x.n_dot_h = dot(a.n, x.wh);
	x.h_dot_o = dot(*out, x.wh);
	x.wi_dot_h = dot(-a.in, x.wh);
	return (specular_rough(x, hit_object, pdf));
}

// float3	sample_bxdf(float3 in, float3 *out, float3 normal,
// 		t_object *hit_object, float *pdf, float2 seed)

// if (isnan(x.bxdf.x) || isnan(x.bxdf.y) || isnan(x.bxdf.z) || isinf(x.bxdf.x)
// || isinf(x.bxdf.y) || isinf(x.bxdf.z))
// {
// 	printf("freshnel: %f %f %f, %f %f %f, %f\n", x.freshnel.x, x.freshnel.y,
// 		x.freshnel.z, hit_object->f_0.x, hit_object->f_0.y,
// 		hit_object->f_0.z, dot(normal, -in));
// 	// printf("type: %d\n", seed <= x.specular_sampling_pdf);
// 	printf("bxdf %f %f %f\n", x.bxdf.x, x.bxdf.y, x.bxdf.z);
// }

t_bxdf	choose_sample_type(t_bxdf x, t_args_bxdf a)
{
	if (a.seed.x <= x.specular_sampling_pdf)
		x.bxdf = x.freshnel * sample_specular(&x.out_dir, a.hit_object,
				&x.pdf_proposal, x.args_sample);
	else
		x.bxdf = (1.0f - x.freshnel) * sample_diffuse(&x.out_dir,
				&x.pdf_proposal, x.args_sample);
	x.pdf_proposal *= select(x.specular_sampling_pdf,
			x.diffuse_sampling_pdf, a.seed.x > x.specular_sampling_pdf);
	return (x);
}

float3	sample_bxdf(t_args_bxdf a)
{
	t_bxdf	x;

	x.freshnel = freshnel_schlick(a.hit_object->f_0, dot(a.normal, -a.in));
	x.specular_weight = luma(x.freshnel * a.hit_object->specular_albedo);
	x.diffuse_weight = luma((1.0f - x.freshnel) * a.hit_object->diffuse_albedo);
	x.inv_weight_sum = 1.0f / (x.specular_weight + x.diffuse_weight + 1e-5f);
	x.specular_sampling_pdf = x.specular_weight * x.inv_weight_sum;
	x.diffuse_sampling_pdf = x.diffuse_weight * x.inv_weight_sum;
	x.args_sample = (t_args_sample){a.in, a.normal,
		a.hit_object->diffuse_albedo, a.seed};
	x = choose_sample_type(x, a);
	if (x.pdf_proposal <= 0.0f)
	{
		*a.pdf = 0.0f;
		return ((float3)(0.0f));
	}
	*a.out = x.out_dir;
	*a.pdf = x.pdf_proposal;
	return (x.bxdf);
}

// float3 sample_bxdf(float seed, float2 s, float3 in, float3 *out,
// float3 normal, t_object *hit_object, float *pdf)
// {
//   float3 freshnel;
//   float specular_weight;
//   float diffuse_weight;
//   float inv_weight_sum;
//
//   float specular_sampling_pdf;
//   float diffuse_sampling_pdf;
//
//   float3 bxdf;
//
//   // if (hit_object->obj_type == SPHERE && check_checkerboard(normal))
//   // {
//   //   *pdf = -1;
//   //   return (to_float3(0));
//   // }
//
//   in = fast_normalize(in);
//
//   freshnel = freshnel_schlick(hit_object->f_0, dot(normal, -in))
// * hit_object->specular_albedo;
//
//   specular_weight = luma(hit_object->specular_albedo * freshnel);
//   diffuse_weight = luma(hit_object->diffuse_albedo * (1 - freshnel));
//   inv_weight_sum = 1 / (specular_weight + diffuse_weight);
//
//   specular_sampling_pdf = specular_weight * inv_weight_sum;
//   diffuse_sampling_pdf = diffuse_weight * inv_weight_sum;
//
//   // int condition = seed <= specular_sampling_pdf;
//   // //                         // seed <= specular_sampling_pdf,
//   // //                         // seed <= specular_sampling_pdf);
//   // //
//   // // // condition.x = seed <= specular_sampling_pdf;
//   // //
//   // bxdf = select(1 - freshnel, freshnel, (int3) condition);
//   // bxdf *= select(sample_diffuse(s, out, normal,
// hit_object->diffuse_albedo, pdf)
//   //             , sample_specular(s, in, out, normal, hit_object, pdf)
//   //             , (int3)condition);
//   // *pdf *= select(diffuse_sampling_pdf, specular_sampling_pdf, condition);
//
//   if (seed <= specular_sampling_pdf)
//   {
//     bxdf = freshnel * sample_specular(s, in, out, normal, hit_object, pdf);
//     *pdf *= specular_sampling_pdf;
//   }
//   else
//   {
//     bxdf = (1 - freshnel) * sample_diffuse(s, out, normal,
// hit_object->diffuse_albedo, pdf);
//     *pdf *= diffuse_sampling_pdf;
//   }
//
//   *pdf = select(*pdf, 1e-5f, *pdf == 0);
//   // if (*pdf == 0)
//   //   *pdf = 1e-5f;
//   return (bxdf * fmax(dot(*out, normal), 0.0f));
// }
