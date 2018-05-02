/*!
 * @file 
 * @brief This file contains function declarations important for exercise.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 *
 */

#pragma once

#include<SDL2/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This function is called at start of application.
 * It should contain initialization of CPU side.
 * @param width width of window in pixels
 * @param height height of window in pixels
 */
void phong_onInit(int32_t width,int32_t height);

/**
 * @brief This function is called at end of application.
 */
void phong_onExit();

/**
 * @brief This function is called every frame and is responsible for drawing.
 */
void phong_onDraw();

#ifdef __cplusplus
}
#endif

/**
 * \mainpage Izg 2017 - 2018, cvičení 6.
 * 
 * \section zadani Zadání cvičení 6. do předmětu IZG 2017 - 2018.
 *
 * \tableofcontents
 *
 *
 * Vašim úkolem je naimplementovat phongův osvětlovací model pomocí OpenGL.
 * Úkol je složen ze tří částí:
 *  - Inicializace dat na GPU a spuštění kreslení \link task1 první úkol \endlink.
 * \image html images/task1.png "Výstup po prvním úkolu" width=5cm
 *  - Implementace transformace vrcholů pomocí vertex shaderu \link task2 druhý úkol \endlink.
 * \image html images/task2.png "Výstup po druhém úkolu" width=5cm
 *  - Implementace phongova osvětlení pomocí fragment shaderu \link task3 třetí úkol \endlink.
 * \image html images/task3.png "Výstup cvičení" width=5cm
 * Seznam všech úkolů naleznete zde \link todo.html \endlink.
 *
 * Ve cvičení se seznámíte se základními funkcemi OpenGL a napíšete si jednoduchý kód ve jazyce GLSL.
 * Prerekvizita cvičení je 11. přednáška o osvětlování a základech OpenGL.
 * Cvičení taktéž kopíruje zadání projektu, ve kterém je úkol totožný, ale nevyužívá se OpenGL.
 *
 * V projektu je přítomen i příklad vykreslení jednoho čtverce: v \link quadExample Quad Example\endlink.
 * Tento příklad můžete využít pro inspiraci a návod jak napsat cvičení.
 * Všechny principy OpenGL, které budete ve cvičení používat jsou tam použity (tvorba bufferů, tvorba vertex arrays objektu, indexování, uniformní proměnné, vertex atributy, přeposílání dat mezi vertex shaderem a fragment shaderem).
 *
 * Všechny úkoly jsou v souboru <b>student/student.c</b>.
 * Model králička je v souboru <b>student/bunny.h</b>.
 *
 * \section teorie Teorie
 *
 * Typické grafické API (OpenGL/Vulkan/DirectX) je složeno ze 2 částí: CPU a GPU strany.
 *
 * CPU strana se obvykle stará o tyto úkoly:
 *  - Příprava dat pro kreslení (modely, textury, matice, ...)
 *  - Upload dat na GPU a nastavení GPU
 *  - Spuštění vykreslení
 *
 * GPU strana je složena ze dvou částí: video paměti a zobrazovacího řetězce.
 * Vykreslovací řetězec se skládá z několika částí:
 * - Vertex Puller
 * - Vertex Processor
 * - Primitive Assembly
 * - Clipping
 * - Rasterize
 * - Fragment Processor
 * - Per-Fragment Operations
 *
 * \subsection terminologie Terminologie
 *
 * \b Vertex je kolekce několika vertex atributů.
 * Tyto atributy mají svůj typ a počet komponent.
 * Každý vertex atribut má nějaký význam (pozice, hmotnost, texturovací koordináty), které mu přiřadí programátor/modelátor.
 * Z několika vrcholů je složeno primitivum (trojúhelník, úsečka, ...)
 *
 * <b>Vertex atribut</b> je jedna vlastnost vrcholu (pozice, normála, texturovací koordináty, hmotnost, ...).
 * Atribut je složen z 1,2,3 nebo 4 komponent daného typu (FLOAT, INT, ...).
 * Sémantika atributu není pevně stanovena (atributy mají pouze pořadové číslo - attribIndex) a je na každém programátorovi/modelátorovi, jakou sémantiku atributu přidělí.
 * \image html images/primitive.svg "Vertex attributes" width=10cm
 *
 * \b Fragment je kolekce několika atributů (podobně jako Vertex).
 * Tyto atributy mají svůj typ a počet komponent.
 * Fragmenty jsou produkovány resterizací, kde jsou atributy fragmetů vypočítány z vertex atributů pomocí interpolace.
 * Fragment si lze představit jako útržek původního primitiva.
 *
 * <b>Fragment atribut</b> je jedna vlastnost fragmentu (podobně jako vertex atribut).
 *
 * <b>Interpolace</b> Při přechodu mezi vertex atributem a fragment atributem dochází k interpolaci atributů.
 * Atributy jsou váhovány podle pozice fragmentu v trojúhelníku (barycentrické koordináty).
 * \image html images/interpolation.svg "Vertex attribute interpolation" width=10cm
 *
 * <b>Vertex Processor</b> (často označován za Vertex Shader) je funkční blok, který je vykonáván nad každým vertexem.
 * Jeho vstup i výstup je Vertex. Výstupní vertex má obvykle jiné vertex atributy než vstupní vertex.
 * Výstupní vertex má vždy atribut - gl_Position (pozice vertexu v clip-space).
 * Vertex Processor se obvykle stará o transformace vrcholů modelu (posuny, rotace, projekce).
 * Jelikož Vertex Processor pracuje po vrcholech, je vhodný pro efekty jako vlnění na vodní hladině, displacement mapping apod.
 * Vertex Processor má informace pouze o jednom vrcholu v daném čase (neví nic o sousednostech vrcholů).
 * Vertex processor je programovatelný.
 *
 * <b>Fragment Processor</b> (často označován za Fragment Shader/Pixel Shader) je funkční blok, který je vykonáván nad každým fragmentem.
 * Jeho vstup i výstup je Fragment. Výstupní fragment má obykle jiné attributy.
 * Fragment processor je programovatelný.
 *
 * <b>Shader</b> je program/funkce, který běží na některé z programovatelných částí zobrazovacího řetezce.
 * Shader má vstupy a výstupy, které se mění s každou jeho invokací.
 * Shader má také vstupy, které zůstávají konstantní a nejsou závislé na číslu invokace shaderu.
 * Shaderů je několik typů, v tomto projektu se používají pouze 2 - vertex shader a fragment shader.
 * V tomto projektu jsou shadery reprezentovány pomocí standardních Cčkovských funkcí.
 *
 * <b>Vertex Shader</b> je program, který běží na vertex processoru.
 * Jeho vstupní interface obsahuje: vertex, uniformní proměnné a další proměnné (číslo vrcholu gl_VertexID, ...).
 * Jeho výstupní inteface je vertex, který vždy obsahuje proměnnou gl_Position - pozici vertexu v clip-space.
 *
 * <b>Fragment Shader</b> je program, který běží na fragment processoru.
 * Jeho vstupní interface obsahuje: fragment, uniformní proměnné a proměnné (souřadnici fragmentu ve screen-space gl_FragCoord, ...).
 * Jeho výstupní interface je fragment.
 *
 * <b>Shader Program</b> je kolekce programů, které běží na programovatelných částech zobrazovacího řetězce.
 * Obsahuje vždy maximálně jeden shader daného typu.
 * V tompto projektu je program reprezentován pomocí dvou ukazatelů na funkce.
 * \image html images/shader_program.svg "This shader program is composed of vertex shader and fragment shader" width=10cm
 *
 * <b>Buffer</b> je lineární pole dat ve video paměti na GPU.
 * Do bufferů se ukládají vertex attributy vextexů modelů nebo indexy na vrcholy pro indexované vykreslování.
 * Buffer je na CPU straně reprezentování celočíselným identifikátorem.
 *
 * <b>Binding point</b> je místo, kam lze připojit buffer.
 * Některé binding pointy slouží pro indexování, jiné pro vertex atributy.
 *
 * <b>Uniformní proměnná</b> je proměná uložená v konstantní paměti GPU. Všechny programovatelné bloky zobrazovacího řetězce z nich mohou pouze číst.
 * Jejich hodnota zůstává stejná v průběhu kreslení (nemění se v závislosti na číslu vertexu nebo fragmentu). Jejich hodnodu lze změnit z CPU strany pomocí funkcí jako je
 * glUniform1f, glUniform1i, glUniform2f, glUniformMatrix4fv apod. Uniformní proměnné jsou vhodné například pro uložení transformačních matic nebo uložení času.
 *
 * <b>Vertex Puller</b> se stará o přípravů vrcholů.
 * K tomuto účelu má tabulku s nastavením (vertex arrays object).
 * Vertex puller si můžete představit jako sadu čtecích hlav.
 * Každá čtecí hlava se stará o přípravu jednoho vertex atributu.
 * Mezi nastavení čtecí hlavy patří: ukazatel na začátek bufferu, offset a krok.
 * Vertex puller může obsahovat indexování.
 *
 * <b>Vertex Arrays Object (VAO)</b> je tabulka s nastavením vertex pulleru.
 * Obsahuje seznam nastavení čtecích hlav pro jednotlivé vertex atributy.
 *
 * <b>Zobrazovací řetězec</b> je obvykle kus hardware na grafické kartě, který se stará o vyreslování.
 * Grafická karta je složena ze dvou částí: paměti a zobrazovacího řetězce.
 * V paměti se nacházejí buffery, textury, uniformní proměnné, programy, nastavení vertex pulleru a framebuffery.
 * Pokud se spustí kreslení N vrcholů, je vertex puller spuštěn N krát a sestaví N vrcholů.
 * Nad každým vrcholem je puštěn vertex shader.
 * Výstupem vertex shaderu je nový vrchol.
 * Blok sestavení primitiv "si počká" na 3 vrcholy z vertex shaderu (pro trojúhelník) a vloží je do jedné struktury.
 * Blok clipping ořeže trojúhelníky pohledovým jehlanem.
 * Následuje perspektivní dělení, které vydělí pozice vertexů homogenní složkou.
 * Poté následuje viewport transformace, která podělené vrcholy transformuje do rozlišení obrazovky.
 * Rasterizace trojúhelníky nařeže na fragmenty a interpoluje vertex atributy.
 * Nad každým fragmentem je spuštěn fragment shader.
 * Než jsou fragmenty zapsány zpět do paměti GPU (framebufferu) jsou provedeny per-fragment operace (zjištění viditelnosti fragmentů podle hloubky uchované v depth bufferu).
 * \image html images/rendering_pipeline.svg "Simplified rendering pipeline" width=10cm
 *
 * <b>Uniformní lokace</b> je číslo, které reprezentuje jednu uniformní proměnnou.
 *
 * <b>Vertex atribut lokace</b> je číslo, které reprezentuje jeden vertex atribut.
 *
 * <b>gl_VertexID</b> je číslo vrcholu, které je vypočítáno pomocí indexování a pořadového čísla vyvolání vertex shaderu.
 *
 * <b>Indexované kreslení</b> je způsob snížení redundance dat s využitím indexů na vrcholy.
 * \image html images/drawElements.svg "Difference between indexed and non-indexed drawing" width=10cm
 *
 * \section glsl Jazyk GLSL
 * Jazyk GLSL slouží pro implementaci algoritmů (shader programů), které běží na grafické kartě.
 * Jazyk je odvozen z jazyka C a je rozšířen o vektorové a maticové typy:
 * \verbatim
 * vec3 a;//vektor třísložkový, typ float
 * ivec4 b;//vektor čtyřsložkový, typ int
 * mat4 c;//matice 4x4, typ float
 * \endverbatim
 * Jazyk má přetížené operátory a funkce pro práci s těmito novými typy:
 * \verbatim
 * vec3 a = vec3(1,2,3);
 * vec3 b = vec3(2,3,2);
 * b += a;//pricte k "b" "a" po komponentách
 * b.x = 10;//nahraje do x složky 10
 * b[1] = 11;//nahraje do y složky 11
 * b.xy = vec2(13,14);//nahraje do x 13 a do y 14
 * b = a.xxy;// je akvivalentni k b.x = a.x; b.y = a.x; b.z = a.y;
 * float s = dot(a,b);//skalární součin
 * b *= 10;//vynásobí všechny složky vektoru 10
 * mat3 m;
 * b = m*a;//násobení maticí
 * \endverbatim
 * Jazyk vynucuje, aby první řádek zdrojového kódu obsahoval verzi
 * \verbatim
 * #version 450
 * \endverbatim
 * Zdrojový kód shaderu je složen z interface a funkce main.
 * Interface obsahuje deklarace globálních proměnných, pomocí kterých shader komunikuje s okolím.
 * Funkce main je vykonána nad každým vertexem/fragmentem.
 * Kvalifikátor proměnné "uniform" říká, že data proměnné budou uložena v konstantní paměti GPU, která lze změnit z CPU.
 * \verbatim
 * uniform mat4 viewMatrix;
 * \endverbatim
 * Kvalifikátor "in" a "out" říkají, že data proměnné přijdou z předcházejícího programovatelného bloku nebo vertex pulleru nebo
 * odejdou do následujícího programovatelného bloku nebo do per-fragment operací.
 * Lokaci lze získat pouze z uniformních proměnných a "in" proměnných vertex shaderu.
 * Takto dekorované proměnné mění svoji hodnotu per-vertex nebo per-fragment.
 * Jazyk obsahuje spoustu vestavěnách funkcí:
 * \verbatim
 * vec3 a,b;
 * dot(a,b);//skalární součin
 * cross(a,b);//cross product
 * reflect(a,b);//a je odražen podle b
 * normalize(a);//normalizace
 * clamp(f,a,b) ořez proměnné f do rozsahu [a,b]
 * min(a,b) návrat menší hodnoty
 * max(a,b) návrat větší hodnoty
 * \endverbatim
 *
 * \section sestaveni Sestavení
 *
 * Linux v CVT: pustit skript linux_create_makefile.sh - to vám vytvoří makefile ve složce build
 * Postup:
 *  -# $ ./linux_create_makefile.sh
 *  -# $ cd build
 *  -# $ make
 *
 * Windows:
 * V CVT je naistalována stará verze CMake, stáhněte <b>novou</b> verzi CMake z: 
 * <a href="http://www.fit.vutbr.cz/~imilet/shared/cmake-3.11.1-win64-x64.zip">cmake na kazi</a>, nebo z 
 * <a href="https://cmake.org/files/v3.11/cmake-3.11.1-win64-x64.zip">CMAKE</a>.
 * Rozbalte cmake a spusťte program <b>bin/cmake-gui.exe</b>.
 * \image html images/64build.png "sestavení v CVT" width=5cm
 * Cvičení bylo testováno na Ubuntu 14.04, Ubuntu 16.04, Visual Studio 2013, Visual Studio 2015, Visual Studio 2017.
 * Cvičení vyžaduje 64 bitové sestavení.
 * Cvičení využívá build systém <a href="https://cmake.org/">CMAKE</a>.
 * CMake je program, který na základně konfiguračních souborů "CMakeLists.txt" vytvoří "makefile" v daném vývojovém prostředí.
 * Dokáže generovat makefile pro Linux, mingw, solution file pro Microsoft Visual Studio, a další.
 * Pro Visual Studio 2017 je potřeba nový CMAKE 3.10.1 a vyšší.
 * Postup:
 * -# stáhnout projekt
 * -# rozbalit projekt
 * -# ve složce build spusťte "cmake-gui .." případně "ccmake .."
 * -# vyberte si překladovou platformu (64 bit).
 * -# configure
 * -# generate
 * -# make nebo otevřete vygenerovnou Microsoft Visual Studio Solution soubor.
 * 
 *
 * Cvičení vyžaduje pro sestavení knihovnu <a href="https://www.libsdl.org/download-2.0.php">SDL2</a>.
 * Ta je pro 64bit build ve Visual Studiu přibalena.
 * Cesty na hlavičkové soubory a libs jsou nastaveny pomocí checkboxu USE_PREBUILD_LIB_PACKAGE.
 * Pod Linuxem si stáhněte SDL2 zdrojáky, zkompilujte je (s pomocí CMAKE) a nainstalujte knihovnu.
 *
 * \section spousteni Spouštění
 *
 * Cvičení je možné po úspěšném přeložení pustit přes aplikaci <b>izg2017Lab6</b>.
 * 
 * \section ovladani Ovládání
 * Program se ovládá pomocí myši a klávesnice:
 * - stisknuté levé tlačítko myši + pohyb myší - rotace kamery
 * - stisknuté pravé tlačítko myši + pohyb myší - přiblížení kamery
 * - "n" - přepne na další scénu/metodu
 *   "p" - přepne na předcházející scénu/metodu
 *
 */


