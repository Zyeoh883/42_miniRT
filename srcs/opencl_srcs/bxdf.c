/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bxdf.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:38 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/19 15:20:04 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

unsigned int	wang_hash(unsigned int x)
{
	x = (x ^ 61) ^ (x >> 16);
	x = x + (x << 3);
	x = x ^ (x >> 4);
	x = x * 0x27d4eb2d;
	x = x ^ (x >> 15);
	return (x);
}

// LCG step
uint	pcg_hash(uint input)
{
	uint	state;
	uint	word;

	state = input * 747796405u + 2891336453u;
	word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
	return ((word >> 22u) ^ word);
}

// return (dot(rgb, (float3)(1.0f, 1.0f, 1.0f)));
float	luma(float3 rgb)
{
	return (dot(rgb, (float3)(0.2126f, 0.7152f, 0.0722f)));
}

float3	reflect(float3 v, float3 n)
{
	return (normalize(v - 2.0f * dot(v, n) * n));
}

float3	ggx_sample(float3 n, float alpha, float2 s)
{
	float phi = 2.0f * (float)M_PI * s.x;
	float tan_theta_sq = alpha * alpha * s.y / (1.0f - s.y);
	float cos_theta = 1.0f / sqrt(1.0f + tan_theta_sq);
	float sin_theta = sqrt(max(0.0f, 1.0f - cos_theta * cos_theta));

	// Optimized tangent space calculation
	float3 axis = select((float3)(1.0f, 0.0f, 0.0f), (float3)(0.0f, 1.0f, 0.0f),
			(int3)(fabs(n.x) > 0.1f)); // Increased threshold for stability
	float3 t = normalize(cross(axis, n));
	float3 b = cross(n, t);

	float cos_phi = cos(phi);
	float sin_phi = sin(phi);

	return normalize(b * cos_phi * sin_theta +
	                t * sin_phi * sin_theta +
	                n * cos_theta);
}

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

float3	sample_specular(float3 *out, t_object *hit_object, float *pdf, t_args_sample a)
{
	t_dot	x;
	float	d;
	float	g;
	float	denominator;

	if (hit_object->roughness_sqr <= 1e-4f)
	{
		*out = reflect(a.in, a.n);
		*pdf = 1.0f;
		return (hit_object->specular_albedo);
	}
	else
	{
		x.wh = ggx_sample(a.n, hit_object->roughness_sqr, a.s);
		*out = reflect(a.in, x.wh);
		x.n_dot = dot(a.n, -a.in);
		x.n_dot_o = dot(a.n, *out);
		x.n_dot_h = dot(a.n, x.wh);
		x.h_dot_o = dot(*out, x.wh);
		x.wi_dot_h = dot(-a.in, x.wh); // Incident dot Halfway (Incident is -in)
		if (x.n_dot_o <= 0.0f || x.n_dot_h <= 0.0f || x.wi_dot_h <= 0.0f
			|| x.n_dot <= 0.0f)
		{
			// printf("here\n");
			*pdf = 0.0f;
			return ((float3)(0.0f));
		}
		d = ndf_ggx(hit_object->roughness_sqr, x.n_dot_h);
		g = g_smith_ggx_correlated(x.n_dot, x.n_dot_o,
				hit_object->roughness_sqr);
		*pdf = d * x.n_dot_h / (4.0f * x.wi_dot_h);
		denominator = 4.0f * x.n_dot_o + 1e-5f;
		return (d * g * hit_object->specular_albedo / denominator);
	}
}

int	check_checkerboard(float3 normal)
{
	t_checkerboard	x;

	x.theta = acos(-normal.y);
	x.phi = atan2(-normal.z, normal.x) + (float)M_PI;
	x.u = x.phi * 0.5f * (float) M_1_PI;
	x.v = x.theta * (float) M_1_PI;
	x.tile_u = floor(x.u * 12.0f);
	x.tile_v = floor(x.v * 12.0f);
	if ((x.tile_u + x.tile_v) % 2 == 0)
		return (1);
	return (0);
}

float3	sample_bxdf(float3 in, float3 *out, float3 normal,
		t_object *hit_object, float *pdf, float2 seed)
{
	t_bxdf	x;

	x.freshnel = freshnel_schlick(hit_object->f_0, dot(normal,-in));
	x.specular_weight = luma(x.freshnel * hit_object->specular_albedo);
	x.diffuse_weight = luma((1.0f - x.freshnel) * hit_object->diffuse_albedo);
	x.inv_weight_sum = 1.0f / (x.specular_weight + x.diffuse_weight + 1e-5f);
	x.specular_sampling_pdf = x.specular_weight * x.inv_weight_sum;
	x.diffuse_sampling_pdf = x.diffuse_weight * x.inv_weight_sum;
	x.args_sample = (t_args_sample){in, normal, hit_object->diffuse_albedo, seed};
	if (seed.x <= x.specular_sampling_pdf)
		// x.bxdf = x.freshnel * sample_specular(in, &x.out_dir, normal,
		// 		hit_object, &x.pdf_proposal);
		x.bxdf = x.freshnel * sample_specular(&x.out_dir, hit_object, &x.pdf_proposal, x.args_sample);
		// x.pdf_proposal *= x.specular_sampling_pdf;
	else

		x.bxdf = (1.0f - x.freshnel) * sample_diffuse(&x.out_dir, &x.pdf_proposal, x.args_sample);
		// x.pdf_proposal *= x.diffuse_sampling_pdf;

	x.pdf_proposal *= select(x.specular_sampling_pdf, x.diffuse_sampling_pdf, seed.x > x.specular_sampling_pdf);
	// if (isnan(x.bxdf.x) || isnan(x.bxdf.y) || isnan(x.bxdf.z) || isinf(x.bxdf.x)
	// || isinf(x.bxdf.y) || isinf(x.bxdf.z))
	// {
	// 	printf("freshnel: %f %f %f, %f %f %f, %f\n", x.freshnel.x, x.freshnel.y,
	// 		x.freshnel.z, hit_object->f_0.x, hit_object->f_0.y,
	// 		hit_object->f_0.z, dot(normal, -in));
	// 	// printf("type: %d\n", seed <= x.specular_sampling_pdf);
	// 	printf("bxdf %f %f %f\n", x.bxdf.x, x.bxdf.y, x.bxdf.z);
	// }
	if (x.pdf_proposal <= 0.0f)
	{
		*pdf = 0.0f;
		return ((float3)(0.0f));
	}
	*out = x.out_dir;
	*pdf = x.pdf_proposal;
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
