/**
 * @file 
 * @brief This file contains function declarations for simple quad example.
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
 * It contains initialization at CPU side.
 * @param width width of window in pixels
 * @param height height of window in pixels
 */
void quadExample_onInit(int32_t width,int32_t height);

/**
 * @brief This function is called at end of application.
 */
void quadExample_onExit();

/**
 * @brief This function is called every frame and is responsible for drawing.
 */
void quadExample_onDraw();

#ifdef __cplusplus
}
#endif


