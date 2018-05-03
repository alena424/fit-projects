/*!
 * @file
 * @brief This file contains implementation of exercise.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 *
 */

#include<assert.h>
#include<math.h>

#include<student/student.h>
#include<student/linearAlgebra.h>
#include<student/camera.h>
#include<student/bunny.h>
#include<student/mouseCamera.h>
#include<opengl/opengl.h>
#include<student/program.h>

///This variable contains projection matrix.
extern Mat4 projectionMatrix;
///This variable contains view matrix.
extern Mat4 viewMatrix;
///This variable contains camera position in world-space.
extern Vec3 cameraPosition;

/**
 * @brief This structure contains all global variables for this method.
 */
struct PhongVariables{
  ///This variable contains light poistion in world-space.
  Vec3 lightPosition;
  ///This variable contains a program id.
  GLuint program;//a program id
  ///This variable contains a location of projection matrix uniform.
  GLint  projectionMatrixUniform;//a uniform location
  ///This variable contains a location of view matrix uniform.
  GLint  viewMatrixUniform;//a uniform location
  ///This variable contains a location of camera position uniform.
  GLint  cameraPositionUniform;
  ///This variable contains a location of light position uniform.
  GLint  lightPositionUniform;
  ///This variable contains a vertex arrays object id.
  GLuint vao;//a vertex array id
  ///This variable contains a buffer id for vertex attributes.
  GLuint vbo;//vertex buffer object id
  ///This variable contains a buffer id for vertex indices.
  GLuint ebo;//vertex indices

  //projMatrixUniform
}phong;///<instance of all global variables for triangle example.

/// \addtogroup vs Vertex Shader
/// @{

/// \addtogroup task2 Druhý úkol
/// @{
/// \todo 2.1.) Doimplementujte vertex shader.
/// Vašim úkolem je přidat uniformní proměnné pro view a projekční matici.
/// Dále pronásobte pozici vrcholu těmito maticemi a zapište výsledek do gl_Position.
/// Nezapomeňte, že píšete v jazyce GLSL, který umožňuje práci s maticovými a vektorovými typy.
/// Upravujte phongVertexShaderSource proměnnou.
/// @}


/// \addtogroup task3 Třetí úkol
/// @{
/// \todo 3.1.) Upravte vertex shader.
/// Vašim úkolem je přidat druhý vertex atribut - normálu vrcholu.
/// Dále přidejte dvě výstupní proměnné typu vec3 a zapište do nich pozici a normálu vrcholu ve world-space.
/// Tyto proměnné budete potřebovat pro výpočet osvětlení.
/// Upravujte phongVertexShaderSource proměnnou.
/// @}

/// This variable contains vertex shader source for phong shading/lighting.
char const*phongVertexShaderSource =
"#version 330\n"
"layout(location=0)in vec3 position;\n"
"layout(location=1)in vec3 normal;\n"
"uniform mat4 projectionMatrix;\n"
"uniform mat4 viewMatrix;\n"
"out vec3 vPos;\n"
"out vec3 vNormal;\n"
"void main(){\n"
"	gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.f);\n"
"	vPos = vec3(viewMatrix * vec4(position, 1.f));\n"
"	vNormal = normal;\n"
"}\n";


/// @}

/// \addtogroup fs Fragment Shader
/// @{

/// \addtogroup task3 Třetí úkol
/// @{
/// \todo 3.2.) Upravte fragment shader (proměnná phongFragmentShaderSource).
/// Vašim úkolem je implementovat phongův osvětlovací model.
/// Přidejte dvě vstupní proměnné (typ vec3) stejného názvu, jako nově přidané výstupní proměnné ve vertex shaderu.
/// V jedné obdržíte pozice fragmentu ve world-space.
/// V druhé obdržíte normálu fragmentu ve world-space.
/// Dále přidejte dvě uniformní proměnné (typ vec3) pro pozici kamery a pro pozici světla.
/// Difuzní barvu materiálu nastave v základu na vec3(0.f,1.f,0.f) - zelená.<br/>
/// V případě, že normála povrchu směřuje vzhůru bude difuzní barva vec3(1.f,1.f,1.f) - bílá.<br/>
/// Zelenou a bílou difuzní barvu míchejte pomocí y komponenty normály umocněné na druhou.<br/>
/// Samozřejmě berte v potaz záporné hodnoty a nepřidávejte sníh (bílou barvu) ze spodu.<br/>
/// Spekulání barvu materiálu nastavte na vec3(1.f,1.f,1.f) - bílá.<br/>
/// Shininess faktor nastavte na 40.f.<br/>
/// Předpokládejte, že světlo má bílou barvu.<br/>
/// Barva se vypočíta podle vzorce dF*dM*dL + sF*sM*sL.<br/>
/// dM,sM jsou difuzní/spekulární barvy materiálu - vektory.<br/>
/// dL,sL jsou difuzní/spekulární barvy světla - vektory.<br/>
/// dF,sF jsou difuzní/spekulární faktory - skaláry.<br/>
/// dF lze vypočíst pomocí vztahu clamp(dot(N,L),0.f,1.f) - skalární součin a ořez do rozsahu [0,1].<br/>
/// N je normála fragmentu (nezapomeňte ji normalizovat).<br/>
/// L je normalizovaný vektor z pozice fragmentu směrem ke světlu.<br/>
/// sF lze vypočíst pomocí vztahu pow((clamp(dot(R,L),0.f,1.f)),s) - skalární součin, ořez do rozsahu [0,1] a umocnění.<br/>
/// s je shininess faktor.<br/>
/// R je odražený pohledový vektor V; R = reflect(V,N).<br/>
/// V je normalizovaný pohledový vektor z pozice kamery do pozice fragmentu.<br/>
/// <br/>
/// Nezapomeňte, že programujete v jazyce GLSL, který zvládá vektorové operace.<br/>
/// <b>Seznam užitečných funkcí:</b>
///  - dot(x,y) - funkce vrací skalární součit dvou vektorů x,y stejné délky
///  - clamp(x,a,b) - funkce vrací ořezanou hodnotu x do intervalu [a,b]
///  - normalize(x) - funkce vrací normalizovaný vektor x
///  - reflect(I,N) - funkce vrací odražený vektor I podle normály N
///  - pow(x,y) - funkce vrací umocnění x na y - x^y
/// @}

/// This variable contains fragment shader source for phong shading/linghting.
char const*phongFragmentShaderSource =
"#version 330\n"
"layout(location=0)out vec4 fColor;\n"
"in vec3 vPos, vNormal;\n"
"uniform vec3 cameraPos, lightPos;\n"
"vec3 dM, sM, dL, sL, N, R, V, L, color;\n"
"float s, dF, sF;\n"
"void main(){\n"
"  dM = vec3(0.f, 1.f, 0.f);\n"
"  sM = vec3(1.f, 1.f, 1.f);\n"
"  fColor = vec4(1.f);\n"
"  s = 40.0;\n"
"  dL = vec3(1.f, 1.f, 1.f);\n"
"  sL = vec3(1.f, 1.f, 1.f);\n"
"  N = normalize(vNormal);\n"
"  L = normalize(lightPos - vPos);\n" // z pozie fragmentu smerem ke svetlu
"  R = reflect(V, N);\n"
"  V = normalize(cameraPos - vPos);\n"
"  dF = clamp(dot(N,L), 0.f, 1.f);\n"
"  sF = pow( clamp(dot(R,L), 0.f, 1.f), s);\n"
"  color = dF*dM*dL + sF*sM*sL;\n"
"  fColor = vec4(color, 1.f);\n"
"}\n";
/// @}

/// \addtogroup init Inicializace
/// @{
void phong_onInit(int32_t width,int32_t height){
  //init matrices
  cpu_initMatrices(width,height);
  //init lightPosition
  init_Vec3(&phong.lightPosition,100.f,100.f,100.f);

  GLuint const vertexId = compileShader(
      GL_VERTEX_SHADER       , //a type of shader
      phongVertexShaderSource);//a shader source
  GLuint const fragmentId = compileShader(
      GL_FRAGMENT_SHADER       , //a type of shader
      phongFragmentShaderSource);//a shader source
  phong.program = linkProgram(
      vertexId,fragmentId);

  /// \addtogroup task1 První úkol
  /// @{
  /// \todo 1.1.) Doprogramujte inicializační funkci phong_onInit().
  /// Zde byste měli vytvořit buffery na GPU, nahrát data do bufferů, vytvořit vertex arrays object a správně jej nakonfigurovat.
  /// Do bufferů nahrajte vrcholy králička (pozice, normály) a indexy na vrcholy ze souboru bunny.h.
  /// V konfiguraci vertex arrays objektu zatím nastavte pouze jeden vertex atribut - pro pozici.
  /// Využijte proměnné ve struktuře PhongVariables (vbo, ebo, vao).
  /// Do proměnné phong.vbo zapište id bufferu obsahující vertex atributy.
  /// Do proměnné phong.ebo zapište id bufferu obsahující indexy na vrcholy.
  /// Do proměnné phong.vao zapište id vertex arrays objektu.
  /// Data vertexů naleznete v proměnné bunny.h/bunnyVertices - ty překopírujte do bufferu phong.vbo.
  /// Data indexů naleznete v proměnné bunny.h/bunnyIndices - ty překopírujte do bufferu phong.ebo.
  /// Dejte si pozor, abyste správně nastavili stride a offset ve funkci glVertexAttribPointer.
  /// Vrchol králička je složen ze dvou vertex atributů: pozice a normála.<br/>
  /// Buffer indexů nabindujte při nahrávání nastavení do VAO na GL_ELEMENT_ARRAY_BUFFER binding point.<br/>
  /// <b>Seznam funkcí, které jistě využijete:</b>
  ///  - glGenBuffers
  ///  - glBindBuffer
  ///  - glBufferData
  ///  - glGenVertexArrays
  ///  - glGetAttribLocation
  ///  - glBindVertexArray
  ///  - glVertexAttribPointer
  ///  - glEnableVertexAttribArray
  ///  @}
	// vytvorime buffery na vrcholy kralicka a na indexy na vrcholy
	glGenBuffers(1, &phong.vbo); // vrcholy
	glGenBuffers(1, &phong.ebo); // indexy

	glBindBuffer(GL_ARRAY_BUFFER, phong.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bunnyVertices), bunnyVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, phong.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(VertexIndex) * 3 * 2092, bunnyIndices, GL_STATIC_DRAW);

	// nastavime jeden atribut
	glGenVertexArrays(1, &phong.vao);
	glBindVertexArray(phong.vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, phong.ebo);
	glBindBuffer(GL_ARRAY_BUFFER, phong.vbo);
//	glBindBuffer(GL_ARRAY_BUFFER, phong.vbo);

	GLint const posAttr  = glGetAttribLocation(phong.program, "position");

	glVertexAttribPointer((GLuint)posAttr, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, NULL);

	glEnableVertexAttribArray((GLuint) posAttr);



  /// \addtogroup task2 Druhý úkol
  /// @{
  /// \todo 2.2.) Ve funkci phong_onInit() získejte lokace přidaných uniformních proměnných pro projekční a view matice.
  /// Zapište lokace do příslušných položek ve struktuře PhongVariables.
  /// Nezapomeňte, že lokace získáte pomocí jména proměnné v jazyce GLSL, které jste udělali v předcházejícím kroku.
  /// @}
	phong.projectionMatrixUniform = glGetUniformLocation(phong.program, "projectionMatrix");
	phong.viewMatrixUniform = glGetUniformLocation(phong.program, "viewMatrix");

  /// \addtogroup task3 Třetí úkol
  /// @{
  /// \todo 3.3.) Ve funkci phong_onInit() získejte lokace přidaných uniformních proměnných pro pozici světla a pro pozice kamery.
  /// Zapište lokace do příslušných položek ve struktuře PhongVariables.
  /// Nezapomeňte, že lokace získáte pomocí jména proměnné v jazyce GLSL, které jste udělali v předcházejícím kroku.<br/>
  /// <b>Seznam funkcí, které jistě využijete:</b>
  ///  - glGetUniformLocation
  /// @}
	phong.lightPositionUniform = glGetUniformLocation(phong.program, "lightPos");
	phong.cameraPositionUniform = glGetUniformLocation(phong.program, "cameraPos");


  /// \addtogroup task3 Třetí úkol
  /// @{
  /// \todo 3.4.) Ve funkci phong_onInit() nastavte druhý vertex atribut pro normálu.
  /// Musíte získat lokaci vstupní proměnné ve vertex shaderu, kterou jste přidali v předcházejícím kroku.
  /// Musíte správně nastavit stride a offset - normála nemá nulový offset.<br/>
  /// <b>Seznam funkcí, které jistě využijete:</b>
  ///  - glGetAttribLocation
  ///  - glVertexAttribPointer
  ///  - glEnableVertexAttribArray
  /// @}

	GLuint const normalAttr  = glGetAttribLocation(phong.program, "normal");
	glVertexAttribPointer(normalAttr, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, sizeof(GLfloat)*3);
	glEnableVertexAttribArray((GLuint) normalAttr);



  glClearColor(.1f,.1f,.1f,1.f);
  glEnable(GL_DEPTH_TEST);

}

/// @}

void phong_onExit(){
  glDeleteBuffers(1,&phong.vbo);
  glDeleteBuffers(1,&phong.ebo);
  glDeleteVertexArrays(1,&phong.vao);
  glDeleteProgram(phong.program);
}

/// \addtogroup draw Kreslení
/// @{

void phong_onDraw(){
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glUseProgram(phong.program);

  /// \addtogroup task1 První úkol
  /// @{
  /// \todo 1.2.) Doprogramujte kreslící funkci phong_onDraw().
  /// Zde byste měli aktivovat vao a spustit kreslení.
  /// Funcke glDrawElements kreslí indexovaně, vyžaduje 4 parametry: mode - typ primitia, počet indexů,
  /// typ indexů (velikost indexu), a offset.
  /// Kreslíte trojúhelníky, počet vrcholů odpovídá počtu indexů viz proměnná bunny.h/bunnyIndices.<br/>
  /// <b>Seznam funkcí, které jistě využijete:</b>
  ///  - glBindVertexArray
  ///  - glDrawElements
  /// @}
	glBindVertexArray(phong.vao);
	glDrawElements(GL_TRIANGLES, 2092 * 3 * sizeof(VertexIndex), GL_UNSIGNED_INT, NULL);


  /// \addtogroup task2 Druhý úkol
  /// @{
  /// \todo 2.3.) Upravte funkci phong_onDraw().
  /// Nahrajte data matic na grafickou kartu do uniformních proměnných.
  /// Aktuální data matic naleznete v externích proměnných viewMatrix a projectionMatrix.
  /// <b>Seznam funkcí, které jistě využijete:</b>
  ///  - glUniformMatrix4fv
  /// @}
	glUniformMatrix4fv(phong.projectionMatrixUniform, 1, GL_FALSE, &projectionMatrix);
	glUniformMatrix4fv(phong.viewMatrixUniform, 1, GL_FALSE, &viewMatrix);

  /// \addtogroup task3 Třetí úkol
  /// @{
  /// \todo 3.5.) Ve funkci phong_onDraw() nahrajte pozici světla a pozici kamery na GPU.
  /// Pozice světla a pozice kamery je v proměnných phong.lightPosition a cameraPosition.<br/>
  /// <b>Seznam funkcí, které jistě využijete:</b>
  ///  - glUniform3f nebo glUniform3fv
  /// @}
	glUniform3fv(phong.cameraPositionUniform, 1, &cameraPosition);
	glUniform3fv(phong.lightPositionUniform, 1, &phong.lightPosition);



}

/// @}


