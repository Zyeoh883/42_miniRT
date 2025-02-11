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

float3 to_float3(float x)
{
    return (float3)(x, x, x);
}


float NDF_GGX(float alphaSqr, float NdotH)
{
  float denom;

  denom = NdotH * NdotH * (alphaSqr + 1.0f) + 1.0f;


  // printf("%f %f %f\n", denom, NdotHSqr, TanNdotHSqr);

  return (alphaSqr * INV_PI / (denom * denom));
  // denom = NdotHSqr * (alphaSqr - 1.0f) + 1.0f;
  
  // return (alphaSqr / (M_PI * denom * denom));
}

float geometric_smith(float3 in, float NdotL, float alphaSqr, float3 normal, float NdotV)
{
  float k;
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

 float3 cook_torrance_BRDF(float3 in, float3 out, float3 normal, float NdotH, U __constant t_object *hit_object)
 {
    float D;
    float3 F;
    float G;
    // float NdotH;
    float NdotL;
    float NdotV;

   // Calculate NDF (Normal Distribution Function - GGX)
   // NdotH = max(dot(normal, halfDir), 0.0f);
   NdotL = max(dot(normal, out), 0.0f);
   NdotV = -max(dot(normal, in), 0.0f); 

   D = NDF_GGX(hit_object->roughness_sqr, NdotH);

   // Calculate Geometry term (Smith's method with Schlick-GGX)
   G = geometric_smith(out, NdotL, hit_object->roughness_sqr, normal, NdotV);

   // Calculate Fresnel term (Schlick's approximation)
   F = freshnel_schlick(NdotH, hit_object->F_0);

  // printf("D = %f, G = %f, F = %f %f %f\n", D, G, F[0], F[1], F[2]);

   // Calculate final BRDF
   return ((D * G) / (4.0f * NdotL * NdotV + 1e-5f));   
   // return (D * G);   
 }

 float3 lambertian_BRDF(float3 albedo)
 {
   return (albedo * INV_PI); //dividing by (float)M_PI gets too dark
 }

// {in : incoming ray}, {out : outgoing ray}, {normal : normal vector of surface}
 float3 BRDF(float3 in, float3 out, float3 normal, U __constant t_object *hit_object)
 {
  float3 halfDir;
  float3 diffuse;
  float3 specular;
  float3 freshnel;
  float NdotH;
  
  in = fast_normalize(in);  // Normalize incoming ray
  out = fast_normalize(out); // Normalize outgoing ray
  // hit_object->F_0 = mix((float3)(0.04f), hit_object->diffuse_albedo, hit_object->metallic);
  // printf("%f %f %f\n", in[0], in[1], in[2]);
  diffuse = lambertian_BRDF((1 - hit_object->metallic) * hit_object->diffuse_albedo);
   // if (hit_object->mat_type == 'D')
   //   return (lambertian_BRDF(hit_object->albedo));
  /*printf("Metallic: %f, Roughness: %f, F0: (%f, %f, %f)\n",*/
  /*     hit_object->metallic, hit_object->roughness_sqr,*/
  /*     hit_object->F_0.x, hit_object->F_0.y, hit_object->F_0.z);*/
   if (hit_object->mat_type == 'M')
  {
    halfDir = fast_normalize(in + out);
    NdotH = max(dot(normal, halfDir), 0.0f);
    freshnel = freshnel_schlick(NdotH, hit_object->F_0);
    specular = cook_torrance_BRDF(in, out, normal, NdotH, hit_object);

    // printf("%f %f %f\n", freshnel[0], freshnel[1], freshnel[2]);

    // return (specular);
    float3 kD = (1.0f - freshnel) * (1.0f - hit_object->metallic);
    return (kD * diffuse + freshnel * specular);
    return (mix(diffuse, specular, freshnel));
    // return ((1.0f - freshnel) * diffuse + freshnel * specular);
    // ambert (diffuse) + Cook-Torrance (specular)
  }
   return (diffuse);  

 }


