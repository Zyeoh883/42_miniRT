/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:38 by zyeoh             #+#    #+#             */
/*   Updated: 2024/08/22 20:32:42 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


float NDF_GGX(float alphaSqr, float NdotH)
{
  float denom;
  float NdotHSqr;
  float TanNdotHSqr;

  NdotHSqr = NdotH * NdotH;
  TanNdotHSqr = (1 - NdotHSqr) / NdotHSqr;
  denom = NdotHSqr * (alphaSqr + TanNdotHSqr);
  return (alphaSqr / (M_PI * denom * denom));
  // denom = NdotHSqr * (alphaSqr - 1.0f) + 1.0f;
  
  // return (alphaSqr / (M_PI * denom * denom));
}

float geometric_smith(float3 in, float NdotL, float alphaSqr, float3 normal, float NdotH)
{
  float k;
  float NdotV;
  float G1;
  float G2;

  k = alphaSqr / 2.0f;
  // G1 = 2.0f * NdotH / (NdotH + k);
  G1 = NdotL / (NdotL * (1.0f - k) + k);
  G2 = NdotV / (NdotV * (1.0f - k) + k);
  return (G1 * G2);
}

float3 freshnel_schlick(float NdotH, float3 F_0)
{
  return (F_0 + (1.0f - F_0) * pow(1.0f - NdotH, 5.0f));
}

 float3 cook_torrance_BRDF(float3 in, float3 out, float3 normal, float3 halfDir, U __global t_object *hit_object)
 {
    float D;
    float3 F;
    float G;
    float NdotH;
    float NdotL;
    float NdotV;

   // Calculate NDF (Normal Distribution Function - GGX)
   NdotH = max(dot(normal, halfDir), 0.0f);
   NdotL = max(dot(normal, out), 0.0f);
   NdotV = max(dot(normal, in), 0.0f); 

   D = NDF_GGX(hit_object->roughness_sqr, NdotH);

   // Calculate Geometry term (Smith's method with Schlick-GGX)
   G = geometric_smith(out, NdotL, hit_object->roughness_sqr, normal, NdotH);

   // Calculate Fresnel term (Schlick's approximation)
   F = freshnel_schlick(NdotH, hit_object->F_0);

   // Calculate final BRDF
   return (D * F * G / (4.0f * NdotL * NdotV));   
 }

 float3 lambertian_BRDF(float3 albedo)
 {
   return (albedo); //dividing by (float)M_PI gets too dark
 }

// {in : incoming ray}, {out : outgoing ray}, {normal : normal vector of surface}
 float3 BRDF(float3 in, float3 out, float3 normal, U __global t_object *hit_object)
 {
  float3 halfDir;
  float3 diffuse;
  float3 specular;

  diffuse = lambertian_BRDF(hit_object->albedo);
   // if (hit_object->mat_type == 'D')
   //   return (lambertian_BRDF(hit_object->albedo));

   halfDir = fast_normalize(in + out);
   if (hit_object->mat_type == 'M')
  {
    specular = cook_torrance_BRDF(in, out, normal, halfDir, hit_object);
    return ((1 - hit_object->metallic) * diffuse + specular * hit_object->metallic);
    // ambert (diffuse) + Cook-Torrance (specular)
  }
   return (diffuse);  

 }


