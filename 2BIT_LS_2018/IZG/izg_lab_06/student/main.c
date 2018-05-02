/**
 * @file 
 * @brief This file contains main function.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<SDL2/SDL.h>

#include<student/linearAlgebra.h>
#include<student/student.h>
#include<examples/quadExample.h>
#include<student/mouseCamera.h>
#include<opengl/opengl.h>

/**
 * @brief This function is entry point of application
 *
 * @param argc   number of command line arguments
 * @param argv[] command line arguments
 *
 * @return return non zero on error
 */
int main(int argc,char*argv[]){
  (void)argc;
  (void)argv;
  int32_t windowWidth  = 500;
  int32_t windowHeight = 500;
  char const* applicationName = "izg2017_lab6";


  // enable logging
  SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

  // initialize SDL
  if(SDL_Init(SDL_INIT_VIDEO) != 0){
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"SDL_Init fail: %s\n",SDL_GetError());
    exit(1);
  }

  // create window
  SDL_Window*window = SDL_CreateWindow(
      applicationName       ,
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      windowWidth           ,
      windowHeight          ,
      SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
  if(!window){
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"SDL_CreateWindow fail: %s\n",SDL_GetError());
    exit(1);
  }

  if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE)<0){
    SDL_LogError(
        SDL_LOG_CATEGORY_APPLICATION,
        "SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,...) fail: %s\n",
        SDL_GetError());
    exit(1);
  }

  if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,SDL_GL_CONTEXT_DEBUG_FLAG)<0){
    SDL_LogError(
        SDL_LOG_CATEGORY_APPLICATION,
        "SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,...) fail: %s\n",
        SDL_GetError());
    exit(1);
  }

  if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3)<0){
    SDL_LogError(
        SDL_LOG_CATEGORY_APPLICATION,
        "SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,...) fail: %s\n",
        SDL_GetError());
    exit(1);
  }

  if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3)<0){
    SDL_LogError(
        SDL_LOG_CATEGORY_APPLICATION,
        "SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,...) fail: %s\n",
        SDL_GetError());
    exit(1);
  }

  SDL_GLContext context = SDL_GL_CreateContext(window);
  if(!context){
    SDL_LogError(
        SDL_LOG_CATEGORY_APPLICATION,
        "SDL_GL_CreateContext() fail: %s\n",
        SDL_GetError());
    exit(1);
  }

  loadOpenGLFunction((LOADING_FUNCTION)SDL_GL_GetProcAddress);

  typedef void(*ON_INIT)(int32_t,int32_t);
  typedef void(*ON_EXIT)();
  typedef void(*ON_DRAW)();

  // pointers to methods
  ON_INIT const onInits[] = {
    phong_onInit,
    quadExample_onInit,
  };

  ON_EXIT const onExits[] = {
    phong_onExit,
    quadExample_onExit,
  };

  ON_DRAW const onDraws[] = {
    phong_onDraw,
    quadExample_onDraw,
  };

  size_t const nofMethods = sizeof(onInits)/sizeof(onInits[0]);

  //id of method
  size_t method = 0;
  //id of last method (for detection of changes)
  size_t lastMethod = method;

  //inits first method
  onInits[method](windowWidth,windowHeight);

  SDL_Event event;
  int32_t running = 1;
  //main loop
  while(running){
    //get event
    while(SDL_PollEvent(&event)){
      if(event.type == SDL_QUIT)
        running = 0;

      if(event.type == SDL_KEYDOWN){
        if(event.key.keysym.sym == SDLK_n){//next method
          method++;
          if(method >= nofMethods)
            method = 0;
        }
        if(event.key.keysym.sym == SDLK_p){//previous method
          if(method == 0)
            method = nofMethods-1;
          else
            method--;
        }
      }

      if(event.type == SDL_MOUSEMOTION)
        onMouseMotion(event.motion.x,event.motion.y,event.motion.xrel,event.motion.yrel);

      if(event.type == SDL_MOUSEBUTTONDOWN)
        onMouseButtonDown(event.button.button);

      if(event.type == SDL_MOUSEBUTTONUP)
        onMouseButtonUp(event.button.button);
    }

    //is method switched?
    if(lastMethod != method){
      //destroy last method
      onExits[lastMethod]();
      onInits[method](windowWidth,windowHeight);
      lastMethod = method;
    }

    onDraws[method]();

    SDL_GL_SwapWindow(window);
    SDL_UpdateWindowSurface(window);
  }
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  //destroy method
  onExits[method]();

  return EXIT_SUCCESS;
}


