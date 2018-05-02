/*!
 * @file 
 * @brief This file contains forward declarations and constants.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include<stdlib.h>
#include<stdint.h>

/**
 * @brief pi constant
 */
#define MY_PI 3.14159265359f



struct Vec2;//forward declaration
struct Vec3;//forward declaration
struct Vec4;//forward declaration
struct Mat4;//forward declaration

typedef struct Vec2 Vec2;///< shortcut
typedef struct Vec3 Vec3;///< shortcut
typedef struct Vec4 Vec4;///< shortcut
typedef struct Mat4 Mat4;///< shortcut

/**
 * @brief Instance of this type contains index to vertex.
 */
typedef uint32_t VertexIndex;

#if !defined(MAKE_STUDENT_RELEASE)

/// This define activates implementation of all tasks.
#define IMPLEMENT_SOLUTION

/// This define activates implementation of first task.
#define IMPLEMENT_TASK1

/// This define activates implementation of second task.
#define IMPLEMENT_TASK2

/// This define activates implementation of third task.
#define IMPLEMENT_TASK3

#endif
