/**
 * @file 
 * @brief This file contains function declarations for compiling and linking shader programs.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 *
 */

#pragma once

#include<opengl/opengl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This function creates shader.
 *
 * @param type a type of shader
 * @param source a source code of shader
 *
 * @return It returns compiled shader id.
 */
GLuint compileShader(GLenum const type,char const*source);
 
/**
 * @brief This function creates shader program.
 *
 * @param shader0 an id of compiled shader
 * @param shader1 an id of compiled shader
 *
 * @return It returns linked shader program id.
 */
GLuint linkProgram(GLuint const shader0,GLuint const shader1);

#ifdef __cplusplus
}
#endif
