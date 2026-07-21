//  $Id$
//
//  SuperTux -  A Jump'n Run
//  Copyright (C) 2000 Bill Kendrick <bill@newbreedsoftware.com>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <SDL.h>
#include <SDL_image.h>

#ifndef WIN32
#include <sys/types.h>
#include <ctype.h>
#endif

#include "defines.h"
#include "globals.h"
#include "screen.h"
#include "setup.h"
#include "type.h"

/* Needed for line calculations */
#define SGN(x) ((x)>0 ? 1 : ((x)==0 ? 0:(-1)))
#define ABS(x) ((x)>0 ? (x) : (-x))

/* --- CLEAR SCREEN --- */

void clearscreen(int r, int g, int b)
{
#ifndef NOOPENGL
  if(use_gl)
  {
    glClearColor(r/256, g/256, b/256, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
  }
  else
  {
#endif
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, r, g, b));
#ifndef NOOPENGL
  }
#endif
}

/* --- DRAWS A VERTICAL GRADIENT --- */

void drawgradient(Color top, Color bottom)
{
  if (use_gl)
  {
    // Milestone 1 uses a hardcoded 640x480 virtual frame buffer target space
    GLfloat vertices[] = {
        0.0f,   0.0f,
        640.0f, 0.0f,
        0.0f,   480.0f,
        640.0f, 480.0f
    };

    // Milestone 1 color values are floats (red, green, blue) from 0.0 to 1.0
    GLfloat colors[] = {
        top.red,    top.green,    top.blue,    1.0f,
        top.red,    top.green,    top.blue,    1.0f,
        bottom.red, bottom.green, bottom.blue, 1.0f,
        bottom.red, bottom.green, bottom.blue, 1.0f
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
  }
  else
  {
    for (int y = 0; y < screen->h; y++)
    {
      float t = (float)y / (float)(screen->h > 1 ? (screen->h - 1) : 1);

      const Uint8 r = (Uint8)(top.red + t * (bottom.red - top.red));
      const Uint8 g = (Uint8)(top.green + t * (bottom.green - top.green));
      const Uint8 b = (Uint8)(top.blue + t * (bottom.blue - top.blue));

      const Uint32 color = SDL_MapRGB(screen->format, r, g, b);

      SDL_Rect lineRect = { 0, (Sint16)y, (Uint16)screen->w, 1 };
      SDL_FillRect(screen, &lineRect, color);
    }
  }
}

/* --- FADE IN --- */

/** Fades the given surface into a black one. If fade_out is true, it will fade out, else
it will fade in */

void fade(Surface *surface, int seconds, bool fade_out);

void fade(const std::string& surface, int seconds, bool fade_out)
{
  Surface* sur = new Surface(datadir + surface, IGNORE_ALPHA);
  fade(sur, seconds, fade_out);
  delete sur;
}

void fade(Surface *surface, int seconds, bool fade_out)
{
  float alpha;
  if (fade_out)
    alpha = 0;
  else
    alpha = 255;

  int cur_time, old_time;
  cur_time = SDL_GetTicks();

  while(alpha >= 0 && alpha < 256)
    {
    surface->draw(0,0,(int)alpha);
    flipscreen();

    old_time = cur_time;
    cur_time = SDL_GetTicks();

    /* Calculate the next alpha value */
    float calc = (float) ((cur_time - old_time) / seconds);
    if(fade_out)
      alpha += 255 * calc;
    else
      alpha -= 255 * calc;
    }
}

/* 'Stolen' from the SDL documentation.
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
  int bpp = surface->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to set */
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp)
    {
    case 1:
      *p = pixel;
      break;

    case 2:
      *(Uint16 *)p = pixel;
      break;

    case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
          p[0] = (pixel >> 16) & 0xff;
          p[1] = (pixel >> 8) & 0xff;
          p[2] = pixel & 0xff;
        }
      else
        {
          p[0] = pixel & 0xff;
          p[1] = (pixel >> 8) & 0xff;
          p[2] = (pixel >> 16) & 0xff;
        }
      break;

    case 4:
      *(Uint32 *)p = pixel;
      break;
    }
}

/* Draw a single pixel on the screen. */
void drawpixel(int x, int y, Uint32 pixel)
{
  /* Lock the screen for direct access to the pixels */
  if ( SDL_MUSTLOCK(screen) )
    {
      if ( SDL_LockSurface(screen) < 0 )
        {
          fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
          return;
        }
    }

  if(!(x < 0 || y < 0 || x > screen->w || y > screen->h))
    putpixel(screen, x, y, pixel);

  if ( SDL_MUSTLOCK(screen) )
    {
      SDL_UnlockSurface(screen);
    }
  /* Update just the part of the display that we've changed */
  SDL_UpdateRect(screen, x, y, 1, 1);
}

void drawline(int x1, int y1, int x2, int y2, int r, int g, int b, int a)
{
  if (use_gl)
  {
    GLfloat vertices[] = {
        (GLfloat)x1, (GLfloat)y1,
        (GLfloat)x2, (GLfloat)y2
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glColor4f((GLfloat)r / 255.0f, (GLfloat)g / 255.0f, (GLfloat)b / 255.0f, (GLfloat)a / 255.0f);

    glDrawArrays(GL_LINES, 0, 2);

    glDisableClientState(GL_VERTEX_ARRAY);
  }
  else
  {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    if ( SDL_MUSTLOCK(screen) )
    {
      if ( SDL_LockSurface(screen) < 0 )
        {
          fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
          return;
        }
    }

    while (true)
    {
        if (x1 >= 0 && x1 < screen->w && y1 >= 0 && y1 < screen->h) {
            putpixel(screen, x1, y1, SDL_MapRGBA(screen->format, r, g, b, a));
        }
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }

    if ( SDL_MUSTLOCK(screen) )
    {
      SDL_UnlockSurface(screen);
    }
  }
}

/* --- FILL A RECT --- */

void fillrect(float x, float y, float w, float h, int r, int g, int b, int a)
{
  if (use_gl)
  {
    GLfloat vertices[] = {
        x,     y,         // Top-Left
        x + w, y,         // Top-Right
        x,     y + h,     // Bottom-Left
        x + w, y + h      // Bottom-Right
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glColor4f((GLfloat)r / 255.0f, (GLfloat)g / 255.0f, (GLfloat)b / 255.0f, (GLfloat)a / 255.0f);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
  }
  else
  {
    SDL_Surface* temp = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 
                                             screen->format->BitsPerPixel,
                                             screen->format->Rmask, 
                                             screen->format->Gmask, 
                                             screen->format->Bmask, 
                                             screen->format->Amask);
    if (!temp) return;

    Uint32 color = SDL_MapRGB(temp->format, r, g, b);
    SDL_FillRect(temp, NULL, color);

    SDL_SetAlpha(temp, SDL_SRCALPHA, a);

    SDL_Rect dest = { (Sint16)x, (Sint16)y, (Uint16)w, (Uint16)h };
    SDL_BlitSurface(temp, NULL, screen, &dest);

    SDL_FreeSurface(temp);
  }
}


/* --- UPDATE SCREEN --- */

void updatescreen(void)
{
  if(use_gl)  /*clearscreen(0,0,0);*/
    SDL_GL_SwapBuffers();
  else
    SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);
}

void flipscreen(void)
{
  if(use_gl)
    SDL_GL_SwapBuffers();
  else
    SDL_Flip(screen);
}

void fadeout()
{
  clearscreen(0, 0, 0);
  white_text->draw_align("Loading...", screen->w/2, screen->h/2, A_HMIDDLE, A_TOP);
  flipscreen();
}

void update_rect(SDL_Surface *scr, Sint32 x, Sint32 y, Sint32 w, Sint32 h)
{
  if(!use_gl)
    SDL_UpdateRect(scr, x, y, w, h);
}

