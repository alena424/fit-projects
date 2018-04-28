/*!
 * @file
 * @brief This file contains implemenation of phong vertex and fragment shader.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <assert.h>
#include <math.h>

#include <student/gpu.h>
#include <student/student_shader.h>
#include <student/uniforms.h>

/// \addtogroup shader_side Úkoly v shaderech
/// @{

void phong_vertexShader(GPUVertexShaderOutput *const      output,
                        GPUVertexShaderInput const *const input,
                        GPU const                         gpu) {
  /// \todo Naimplementujte vertex shader, který transformuje vstupní vrcholy do
  /// clip-space.<br>
  /// <b>Vstupy:</b><br>
  /// Vstupní vrchol by měl v nultém atributu obsahovat pozici vrcholu ve
  /// world-space (vec3) a v prvním
  /// atributu obsahovat normálu vrcholu ve world-space (vec3).<br>
  /// <b>Výstupy:</b><br>
  /// Výstupní vrchol by měl v nultém atributu obsahovat pozici vrcholu (vec3)
  /// ve world-space a v prvním
  /// atributu obsahovat normálu vrcholu ve world-space (vec3).
  /// Výstupní vrchol obsahuje pozici a normálu vrcholu proto, že chceme počítat
  /// osvětlení ve world-space ve fragment shaderu.<br>
  /// <b>Uniformy:</b><br>
  /// Vertex shader by měl pro transformaci využít uniformní proměnné obsahující
  /// view a projekční matici.
  /// View matici čtěte z uniformní proměnné "viewMatrix" a projekční matici
  /// čtěte z uniformní proměnné "projectionMatrix".
  /// Zachovejte jména uniformních proměnných a pozice vstupních a výstupních
  /// atributů.
  /// Pokud tak neučiníte, akceptační testy selžou.<br>
  /// <br>
  /// Využijte vektorové a maticové funkce.
  /// Nepředávajte si data do shaderu pomocí globálních proměnných.
  /// Pro získání dat atributů použijte příslušné funkce vs_interpret*
  /// definované v souboru program.h.
  /// Pro získání dat uniformních proměnných použijte příslušné funkce
  /// shader_interpretUniform* definované v souboru program.h.
  /// Vrchol v clip-space by měl být zapsán do proměnné gl_Position ve výstupní
  /// struktuře.<br>
  /// <b>Seznam funkcí, které jistě použijete</b>:
  ///  - gpu_getUniformsHandle()
  ///  - getUniformLocation()
  ///  - shader_interpretUniformAsMat4()
  ///  - vs_interpretInputVertexAttributeAsVec3()
  ///  - vs_interpretOutputVertexAttributeAsVec3()

  // uniform handler
  Uniforms gpuuniformHandler = gpu_getUniformsHandle(gpu);

  // projekcni matice
  UniformLocation projectMatrix = getUniformLocation(gpu, "projectionMatrix");
   // view matice
  UniformLocation viewMatrix = getUniformLocation(gpu, "viewMatrix");

  // interpretujeme matice
  Mat4 const * const projectM = shader_interpretUniformAsMat4(gpuuniformHandler, projectMatrix);
  Mat4 const * const viewM = shader_interpretUniformAsMat4(gpuuniformHandler, viewMatrix);

  // interpretujeme vrcholy atributu na v3
  Vec3 const*const attr_position = vs_interpretInputVertexAttributeAsVec3(gpu, input, 0);
  Vec3 const*const attr_normala = vs_interpretInputVertexAttributeAsVec3(gpu, input, 1);

  Vec3 *const attr_positionOut = vs_interpretOutputVertexAttributeAsVec3(gpu, output, 0);
  Vec3 *const attr_normalaOut = vs_interpretOutputVertexAttributeAsVec3(gpu, output, 1);

  // transformace projektcni a  view matice, vysledek ve matici vysl
  Mat4 vysl;
  multiply_Mat4_Mat4(&vysl, projectM, viewM);

  // vynasobime matici vektorem position, musime ale pridat z souradnici
  Vec4 novy_vec4;
  copy_Vec3Float_To_Vec4(&novy_vec4, attr_position, 1);
  //Vec4 glpos;

  multiply_Mat4_Vec4(&output->gl_Position, &vysl, &novy_vec4);
  //output->gl_Position = glpos;

  // iniclaizuji vystupni atributy
  init_Vec3(attr_normalaOut, attr_normala->data[0], attr_normala->data[1], attr_normala->data[2]);
  init_Vec3(attr_positionOut, attr_position->data[0], attr_position->data[1], attr_position->data[2]);

  //(void)output;
  //(void)input;
  //(void)gpu;
}

void phong_fragmentShader(GPUFragmentShaderOutput *const      output,
                          GPUFragmentShaderInput const *const input,
                          GPU const                           gpu) {
  /// \todo Naimplementujte fragment shader, který počítá phongův osvětlovací
  /// model s phongovým stínováním.<br>
  /// <b>Vstup:</b><br>
  /// Vstupní fragment by měl v nultém fragment atributu obsahovat
  /// interpolovanou pozici ve world-space a v prvním
  /// fragment atributu obsahovat interpolovanou normálu ve world-space.<br>
  /// <b>Výstup:</b><br>
  /// Barvu zapište do proměnné color ve výstupní struktuře.<br>
  /// <b>Uniformy:</b><br>
  /// Pozici kamery přečtěte z uniformní proměnné "cameraPosition" a pozici
  /// světla přečtěte z uniformní proměnné "lightPosition".
  /// Zachovejte jména uniformních proměnný.
  /// Pokud tak neučiníte, akceptační testy selžou.<br>
  /// <br>
  /// Dejte si pozor na velikost normálového vektoru, při lineární interpolaci v
  /// rasterizaci může dojít ke zkrácení.
  /// Zapište barvu do proměnné color ve výstupní struktuře.
  /// Shininess faktor nastavte na 40.f
  /// Difuzní barvu materiálu nastavte podle normály povrchu.
  /// V případě, že normála směřuje kolmo vzhůru je difuzní barva čistě bílá.
  /// V případě, že normála směřuje vodorovně nebo dolů je difuzní barva čiště zelená.
  /// Difuzní barvu spočtěte lineární interpolací zelené a bíle barvy pomocí interpolačního parameteru t.
  /// Interpolační parameter t spočtěte z y komponenty normály pomocí t = y*y (samozřejmě s ohledem na negativní čísla).
  /// Spekulární barvu materiálu nastavte na čistou bílou.
  /// Barvu světla nastavte na bílou.
  /// Nepoužívejte ambientní světlo.<br>
  /// <b>Seznam funkcí, které jistě využijete</b>:
  ///  - shader_interpretUniformAsVec3()
  ///  - fs_interpretInputAttributeAsVec3()

  // opet budu mit uniform handler
  Uniforms gpuuniformHandler = gpu_getUniformsHandle(gpu);

  // vektor pozice kamery
  UniformLocation const camera = getUniformLocation(gpu, "cameraPosition");
   // vektor pozice svetla
  UniformLocation const light = getUniformLocation(gpu, "lightPosition");

  // interpretujeme vektory
  Vec3 const * const cameraVec = shader_interpretUniformAsVec3(gpuuniformHandler, camera);
  Vec3 const * const lightVec = shader_interpretUniformAsMat4(gpuuniformHandler, light);

  // vytahneme si atributy
  Vec3 const * const position = fs_interpretInputAttributeAsVec3(gpu, input, 0);
  Vec3 const * const normala = fs_interpretInputAttributeAsVec3(gpu, input, 1);

   // osvetlovaci faktor
  float shine = 40.0;

  // potrebujeme vypocitat vektor L - svetla, N - normaly, V - vektor pozorovatele a R - reflexe
  Vec3 L;
  Vec3 N;
  Vec3 V;
  Vec3 R;

  // difuzni barvu - podle normaly povrchu, pokud normala vzhuru => bila, pokud vodorovne => zelena, pocitani pomoci linearni interpolace
  // budeme potrebovala interpolacni parametr t = y*y, kde y je y komponenta normaly
  // spekularni barva => bila
  Vec3 zelBarva;
  init_Vec3(&zelBarva, 0.0, 1.0, 0.0);

  // barva svetla => bila
  Vec3 bilaBarva;
  init_Vec3(&bilaBarva, 1.0, 1.0, 1.0);

  Vec3 lighttmp;
  sub_Vec3(&lighttmp, lightVec, position);
  normalize_Vec3(&L, &lighttmp);

  normalize_Vec3(&N, normala);

  sub_Vec3(&V, cameraVec, position);
  normalize_Vec3(&V, &V);

  // koeficiant difuze
  float dif = clamp( dot_Vec3(&N, &L), 0.f, 1.f );
  Vec3 Id;
  multiply_Vec3_Float(&Id, &zelBarva, dif); // jednickama samyma uz nemusime nasobit


  // budeme pocitat koeficient R
  reflect(&R, &L, &N);
  //normalize_Vec3(&R, &R);

  // naposled budeme pocitat Is pomoci koeficient s = 2 * L *N
  // Is = Il * rs(V * R)^shine
  float pom = pow( clamp(dot_Vec3(&R, &L), 0.f, 1.f), shine );
  Vec3 Is;
  multiply_Vec3_Float(&Is, &bilaBarva, pom);

  Vec3 final_vec;
  add_Vec3(&final_vec, &Is, &Id);


  // vystup, zapisu barvy

output->color.data[0] = final_vec.data[0];
output->color.data[1] = final_vec.data[1];
output->color.data[2] = final_vec.data[2];
output->color.data[3] = 1.0;


  //(void)output;
  //(void)input;
  //(void)gpu;

}

/// @}
