#include "gui.hh"
#include <vector>
#include <iostream>
#include "board.hh"

#include "SDL/SDL.h"
#include <GL/glut.h>
#include "GL/gl.h"
#include "GL/glu.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_opengl.h"
#include <sstream>

Gui::~Gui()
{
}

void
Gui::play()
{
  int argc = 0;
  const char *argv = "";
  glutInit(&argc, const_cast<char**>(&argv));
  sdl_init(600, 800, false);

  text_.push_back(load_text("../share/R.png"));
  text_.push_back(load_text("../share/B.png"));
  text_.push_back(load_text("../share/G.png"));
  text_.push_back(load_text("../share/V.png"));
  text_.push_back(load_text("../share/Y.png"));


  while (!board_->is_finished())
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    display();

    if (ask_next_move())
      break;

    std::ostringstream s;

    s << "Score : " << board_->score_get();
    bitmap_output(0, 0, s.str().c_str(), GLUT_BITMAP_HELVETICA_12);
    SDL_GL_SwapBuffers();
  }

  std::cout << "Score : " << board_->score_get() << std::endl;

  SDL_Quit();
}

// Private part

void
Gui::display()
{
  Board::board_t& map = board_->map_get();

  int QUAD_WIDTH = 600 / std::max(board_->height_, board_->width_);

  for (unsigned int i = 0; i < board_->height_; ++i)
    for (unsigned int j = 0; j < board_->width_; ++j)
    {

      glEnable(GL_TEXTURE_2D);

      glColor3f(1, 1, 1);
      if (map[i][j] != 0)
      {
	switch (map[i][j]->color_get())
	{
	  case RED:
	    glBindTexture(GL_TEXTURE_2D, text_[0]);
	    break;
	  case BLUE:
	    glBindTexture(GL_TEXTURE_2D, text_[1]);
	    break;
	  case GREEN:
	    glBindTexture(GL_TEXTURE_2D, text_[2]);
	    break;
	  case VIOLET:
	    glBindTexture(GL_TEXTURE_2D, text_[3]);
	    break;
	  case YELLOW:
	    glBindTexture(GL_TEXTURE_2D, text_[4]);
	    break;
	}

	if (map[i][j]->is_selected())
	  switch (map[i][j]->color_get())
	  {
	    case RED:
	      glColor3f(1, 0, 0);
	      break;
	    case BLUE:
	      glColor3f(0, 0, 1);
	      break;
	    case GREEN:
	      glColor3f(0, 1, 0);
	      break;
	    case VIOLET:
	      glColor3f(1, 0, 1);
	      break;
	    case YELLOW:
	      glColor3f(1, 1, 0);
	      break;
	  }

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex2d(QUAD_WIDTH * j, 600 - i * QUAD_WIDTH);
	glTexCoord2d(1, 0);
	glVertex2d(QUAD_WIDTH * (j + 1), 600 - i * QUAD_WIDTH);
	glTexCoord2d(1, 1);
	glVertex2d(QUAD_WIDTH * (j + 1), 600 - (i + 1) * QUAD_WIDTH);
	glTexCoord2d(0, 1);
	glVertex2d(QUAD_WIDTH * j, 600 - (i + 1) * QUAD_WIDTH);
	glEnd();

	glColor3f(1, 1, 1);
	glDisable(GL_TEXTURE_2D);
      }
    }
}

bool
Gui::ask_next_move()
{
  SDL_Event	event;

  int QUAD_WIDTH = 600 / std::max(board_->height_, board_->width_);

  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
      case SDL_KEYDOWN :
 	switch (event.key.keysym.sym)
 	{
 	  case SDLK_ESCAPE :
 	    SDL_Quit();
 	    exit(0);
 	    break;
 	  default :
 	    break;
 	}
 	break;
      case SDL_MOUSEBUTTONDOWN:
	board_->pick(event.button.x / QUAD_WIDTH, event.button.y / QUAD_WIDTH);
	break;
      case SDL_QUIT :
 	SDL_Quit();
 	exit(0);
 	break;
    }
  }

  return false;
}

bool
Gui::sdl_init(size_t	height,
	      size_t	width,
	      int	fullscreen)
{
  SDL_Surface	*screen = NULL;

  if (SDL_Init(SDL_INIT_VIDEO) == -1)
  {
    std::cout << "SDL_Init: " << SDL_GetError() << std::endl;
    return false;
  }

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  screen = SDL_SetVideoMode(width, height, 32, SDL_OPENGL | fullscreen);

  if (!screen)
  {
    std::cout << "SDL_SetVideoMode: %s\n" << SDL_GetError() << std::endl;
    SDL_Quit();
    return false;
  }

  glClearColor(0, 0, 0, 0);
  gluOrtho2D(0, width, 0, height);
  SDL_ShowCursor(SDL_ENABLE);
  SDL_EnableKeyRepeat(10, 10);

  return true;
}


int
Gui::load_text(std::string name)
{
  SDL_Surface *i;
  GLuint img;
  i = IMG_Load(name.c_str());

  if (i != NULL)
  {
    GLenum	format = GL_RGBA;

    if (i->format->BitsPerPixel <= 24)
      format = GL_RGB;

    glGenTextures(1, &img);
    glBindTexture(GL_TEXTURE_2D, img);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    gluBuild2DMipmaps (GL_TEXTURE_2D, i->format->BytesPerPixel, i->w,
		       i->h, format, GL_UNSIGNED_BYTE, i->pixels);
  }
  return (img);
}

void
Gui::bitmap_output(int x, int y, const char *string, void *font)
{
  int	len = 0;
  int	i = 0;

  glRasterPos2f(x,y);
  len = strlen(string);
  for (i = 0; i < len; i++)
  {
    glutBitmapCharacter(font,string[i]);
  }
}
