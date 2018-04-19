////////////////////////////////////////////////////////////////////////////////////////////////
// Zaklady pocitacove grafiky
// Laboratory #5
//
// Author: Michal Spanel (remixed by Lukas Marsik and Michal Kula)
//
// Laboratory outline: 3D Transformations
//   1) How does it work
//   2) Common task
//      - Discard back-faced triangles (aka back-face culling)
//   3) Classified task
//      - Draw second object symmetrically by the blue "mirror"
//
// *) Application control:
//      mouse         - rotation and zoom of whole scene
//      shift + mouse - object translation
//      ctrl + mouse  - object rotation along its center
//      "Esc", ...  - terminate application
//
// Corrections:
//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// include

#ifdef _WIN32
  #include <windows.h>
#endif

#include "main.h"
#include "base.h"

#include <SDL.h>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>

///////////////////////////////////////////////////////////////////////////////
// name spaces

using namespace std;


///////////////////////////////////////////////////////////////////////////////
// global constants

// main window title
const char         * PROGRAM_TITLE = "IZG Labs #05, 3D transformation";

// quit flag
int                 quit            = 0;

// standard input model
const char         * DEFAULT_MODEL = "data/cylinder.tri";

// maximal scene proximity to camera
const int          MAX_SCENE_MOVE = -CAMERA_DIST + 10;

///////////////////////////////////////////////////////////////////////////////
// global variables

SDL_Surface        * screen        = NULL;    // SDL surfage
S_RGBA             * frame_buffer = NULL;     // frame buffer (RGB values) memory pointer
int                frame_w = 0, frame_h = 0;  // frame buffer size

const int          default_frame_w   = 500;   // default window size
const int          default_frame_h   = 500;

///////////////////////////////////////////////////////////////////////////////
// global variables shared among main.cpp and student.cpp

int                obj_rot_x    = 0;          // camera position
int                obj_rot_y    = 0;

int                obj_move_x   = -100;       // object translation
int                obj_move_y   = 0;

int                scene_rot_x  = 0;          // object rotation
int                scene_rot_y  = 0;

int                scene_move_z = -985;       // scene translation


///////////////////////////////////////////////////////////////////////////////
// function clears frame buffer

void ClearBuffers()
{
  // buffer exists?
  assert(frame_buffer);

  // make all pixels black
  memset(frame_buffer, 0, frame_w * frame_h * sizeof(S_RGBA));
}

///////////////////////////////////////////////////////////////////////////////
// line culling with Liang-Barsky algorithm right before rendering to frame buffer
// return false if whole line is outside of window

bool LiangBarskyLineCulling(int &x1, int &y1, int &x2, int &y2)
{
  // computer parameters dx, dy
  int dx = x2 - x1;
  int dy = y2 - y1;

  // computer parameters pi and qi
  int p[4], q[4];
  p[0] = -dx; q[0] = x1 - 1;
  p[1] =  dx; q[1] = frame_w - 2 - x1;
  p[2] = -dy; q[2] = y1 - 1;
  p[3] =  dy; q[3] = frame_h - 2 - y1;

  // set parameter ui to initial value
  double u1 = 0.0f;
  double u2 = 1.0f;

  // subsequently examine parameters pi a qi
  for( int i = 0; i < 4; ++i )
  {
    // computer parameter ri
    double ri = 0.0f;
    if (p[i] != 0)
      ri = double(q[i]) / double(p[i]);

    // is line aiming to window?
    if (p[i] < 0)
    {
      if (ri > u2)        // valid bounds check
        return false;
      else if (ri > u1)   // find new start point
        u1 = ri;
    }
    else if (p[i] > 0)      // line aims out of window
    {
      if (ri < u1)        // valid bounds check
        return false;
      else if (ri < u2)   // find new end point
        u2 = ri;
    }
    else if (q[i] < 0)      // line is out of window
    {
      return false;
    }
  }

  // computer parameters u1 and u2 at the end
  // compute final line coordinates x1,y1 and x2,y2
  if (u2 < 1)
  {
    double t1 = u2 * dx, t2 = u2 * dy;
    x2 = x1 + ROUND(t1);
    y2 = y1 + ROUND(t2);
  }
  if (u1 > 0)
  {
	double t1 = u1 * dx, t2 = u1 * dy;
	x1 = x1 + ROUND(t1);
	y1 = y1 + ROUND(t2);
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
// rasterize line with given coordinates and color to frame buffer
// x1,y1,x2,y2 - line endpoints
// z1, z2 - line depth within scene
// color - drawing color

void DrawLine(int x1, int y1, int x2, int y2, double z1, double z2, const S_RGBA & color)
{
  // window culling
  if (!LiangBarskyLineCulling(x1, y1, x2, y2))
  {
    return;
  }

  // delta in x and y axis
  int dx = ABS(x2 - x1);
  int dy = ABS(y2 - y1);

  // x and y axis swap in rendering algorithm
  bool xy_swap = (dy > dx);
  if (xy_swap)
  {
    SWAP(x1, y1);
    SWAP(x2, y2);
    SWAP(dx, dy);
  }

  // appropriate point swap in x axis
  if (x1 > x2)
  {
    SWAP(x1, x2);
    SWAP(y1, y2);
  }

  // advance direction in y axis
  int step_y;
  if (y1 > y2) step_y = -1;
  else step_y = 1;

  // Bresenham algorithm constants
  int k1 = 2 * dy;
  int k2 = 2 * (dy - dx);
  int p  = 2 * dy - dx;

  // initial assignment of values
  int x = x1;
  int y = y1;

  // line rendering loop
  while (x <= x2)
  {
    if (!xy_swap)
    {
      PIXEL(x, y) = color;
    }
    else
    {
      PIXEL(y, x) = color;
    }

    x++;
    if (p < 0)
	{
      p += k1;
    }
    else
	{
      p += k2;
      y += step_y;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// functions handling window size change
// resize - resize event

void onResize(SDL_ResizeEvent *resize)
{
	if(screen == NULL || resize->w != screen->w || resize->h != screen->h) // detect changes
	{
		// delete old resources if available
		if(screen != NULL) SDL_FreeSurface(screen);
		if(frame_buffer != NULL) delete [] frame_buffer;

		// create new screen
		if(!(screen = SDL_SetVideoMode(resize->w, resize->h, 32, SDL_HWSURFACE|SDL_RESIZABLE|SDL_DOUBLEBUF)))
		{
			IZG_ERROR("Cannot realocate screen buffer");
			SDL_Quit();
		}
		// create frame buffer
		frame_buffer = new S_RGBA[resize->w * resize->h];
		assert(frame_buffer != NULL);
		frame_w = resize->w;
		frame_h = resize->h;
	}
}

///////////////////////////////////////////////////////////////////////////////
// function handling redraw of window content

void onDraw(void)
{
  // test frame buffer existence
  assert(frame_buffer != NULL);
  // Redraw scene
  DrawScene();
  // lock surface for writing
  if(SDL_MUSTLOCK(screen) != 0) SDL_LockSurface(screen);
  // copy frame buffer to SDL surface
  MEMCOPY(screen->pixels, frame_buffer, sizeof(S_RGBA) * frame_w * frame_h);
  // unlock surface
  if(SDL_MUSTLOCK(screen) != 0) SDL_UnlockSurface(screen);
  // switch buffers
  SDL_Flip(screen);
}

///////////////////////////////////////////////////////////////////////////////
// function responsive to key press
// key - key event

void onKeyboard(SDL_KeyboardEvent *key)
{
  // switch to pressed key
  switch (key->keysym.sym)
  {
    // quit application
    case SDLK_ESCAPE:
    case SDLK_q:
    case SDLK_x:
      quit = 1;
      break;
	default:
      break;
  }
}

///////////////////////////////////////////////////////////////////////////////
// function responsive to mouse move (without pressed buttons)
// motion - mouse motion event

void onMouseMotion(SDL_MouseMotionEvent *motion)
{
  Uint8 *keystate = SDL_GetKeyState(NULL);

  if (motion->state & SDL_BUTTON(1)) {
    if (keystate[SDLK_RCTRL] || keystate[SDLK_LCTRL]) {
      // control key pressed -> object rotation
      obj_rot_y += motion->xrel;
      obj_rot_x += motion->yrel;
    }
    else if (keystate[SDLK_RSHIFT] || keystate[SDLK_LSHIFT]) {
      // shift key pressed -> object translation
      obj_move_x += motion->xrel;
      obj_move_y += -motion->yrel;
    }
    else {
      // scene rotation
      scene_rot_y += motion->xrel;
      scene_rot_x += motion->yrel;
    }
  }
  else if (motion->state & SDL_BUTTON(3)) {
    // camera translation
    scene_move_z += -motion->yrel;
    if (scene_move_z < MAX_SCENE_MOVE) scene_move_z = MAX_SCENE_MOVE;
  }
}

/******************************************************************************
 ******************************************************************************
 * funkce tiskne napovedu na stdout
 */
void printHelpText()
{
	IZG_INFO("Application loaded - IZG LAB 05 - 3D Transformations. Controls:\n\n"
			 "First command line parameter: \n"
			 "    Main object model (cylinder.tri, sphere.tri, bunny.tri, ...)\n\n"
			 "Left mouse down and drag:\n"
			 "    All scene rotation around center (center of mirror)\n\n"
			 "Right mouse down and drag:\n"
			 "    Scene zoom\n\n"
			 "Left mouse down, CTRL key down and drag:\n"
			 "    Main object rotation around its center\n\n"
			 "Left mouse down, SHIFT key down and drag:\n"
			 "    Main object translation along and towards/apart the mirror\n\n"
			 "Q, X or ESC key down:\n"
			 "    Quit running application\n\n"
			 "========================================================================\n\n")
}

///////////////////////////////////////////////////////////////////////////////
// main application function
// argc - count of command line parameters
// argv - array of command line parameters

int main(int argc, char **argv)
{
  SDL_Event event;

  // load model
  const char *model = (argc == 2) ? argv[1] : DEFAULT_MODEL;
  if( !LoadModel(model) )
  {
    IZG_SDL_ERROR("Error: Failed to read input model!");
    exit(1);
  }

  // initialize SDL library
  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    IZG_SDL_ERROR("Error: Could not initialize SDL library!");
    exit(1);
  }

  // set main application window title
  SDL_WM_SetCaption(PROGRAM_TITLE, 0);

  // force calling of onResize function - necessary for buffer initialization of buffers
  SDL_ResizeEvent resize = {SDL_VIDEORESIZE, default_frame_w, default_frame_h}; 
  onResize(&resize);

  printHelpText();

  // loop until SDL_QUIT event reached
  while(!quit) 
  {

    onDraw();

    // Wait for event
    SDL_WaitEvent(&event);

    switch( event.type )
    {
      // resize event
      case SDL_VIDEORESIZE:
		onResize(&event.resize);
        break;

      // keyboard event
      case SDL_KEYDOWN:
        onKeyboard(&event.key);
        break;

      // mouse motion event
      case SDL_MOUSEMOTION:
        onMouseMotion(&event.motion);
        break;

      // SDL_QUIT event
      case SDL_QUIT:
        quit = 1;
        break;

      default:
        break;
    }

  }

  // Free memory
  if (screen != NULL) SDL_FreeSurface(screen);
  if (frame_buffer != NULL) delete [] frame_buffer;
  SDL_Quit();

  return 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
