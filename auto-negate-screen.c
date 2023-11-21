/*\
|*| auto-negate-screen - invert screen colors when screen is more bright than dark
|*|
|*| ANSI C (ISO C90) - requires xlib, xcalib, and xlib headers
|*|   $ gcc -ansi -o auto-negate-screen -lX11 auto-negate-screen.c
\*/

/* SPDX-License-Identifier: AGPL-3.0-or-later */

/*\
|*| Copyright 2023 bill-auger <bill-auger@programmer.net>
|*|
|*| This program is free software: you can redistribute it and/or modify
|*| it under the terms of the GNU Affero General Public License as published by
|*| the Free Software Foundation, either version 3 of the License, or
|*| (at your option) any later version.
|*|
|*| This program is distributed in the hope that it will be useful,
|*| but WITHOUT ANY WARRANTY; without even the implied warranty of
|*| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|*| GNU Affero General Public License for more details.
|*|
|*| You should have received a copy of the GNU Affero General Public License
|*| along with this program.  If not, see <https://www.gnu.org/licenses/>.
\*/


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>


/* debug and tracing */

#include "debug.c.inc"


/* constants */

const uint32_t SLEEP_IVL           = 100000 ;
const uint8_t  PIXEL_AVG_THRESHOLD = 127 ;


/* global variables */

Display* XDisplay ;
Window   RootXwin ;
Colormap XColorMap ;


uint8_t GetPixelAvg(uint8_t sample_rect_offset)
{
  XWindowAttributes xwin_attrs ;
  XImage*           ximage ;
  XColor            xcolors[4] ;
  int               xwin_x = 0 ;
  int               xwin_y = 0 ;
  unsigned int      xwin_w ;
  unsigned int      xwin_h ;
  int               pixel_t ;
  int               pixel_b ;
  int               pixel_l ;
  int               pixel_r ;
  unsigned int      pixel_avg ;

  XGetWindowAttributes(XDisplay , RootXwin , &xwin_attrs) ;
  xwin_w  = xwin_attrs.width ;
  xwin_h  = xwin_attrs.height ;
  pixel_t = 0 ;
  pixel_b = xwin_h - 1 ;
  pixel_l = 0 ;
  pixel_r = xwin_w - 1 ;

  ximage           = XGetImage(XDisplay , RootXwin , xwin_x    , xwin_y ,
                               xwin_w   , xwin_h   , AllPlanes , ZPixmap) ;
  xcolors[0].pixel = XGetPixel(ximage , pixel_l , pixel_t) ;
  xcolors[1].pixel = XGetPixel(ximage , pixel_r , pixel_t) ;
  xcolors[3].pixel = XGetPixel(ximage , pixel_l , pixel_b) ;
  xcolors[2].pixel = XGetPixel(ximage , pixel_r , pixel_b) ;

  XQueryColors(XDisplay , XColorMap , xcolors , 4) ;
  pixel_avg = ( ( xcolors[0].red + xcolors[0].green + xcolors[0].blue +
                  xcolors[1].red + xcolors[1].green + xcolors[1].blue +
                  xcolors[2].red + xcolors[2].green + xcolors[2].blue +
                  xcolors[3].red + xcolors[3].green + xcolors[3].blue ) / 12 ) / 256 ;

DEBUG_GETPIXELAVG

  XDestroyImage(ximage) ;

  return(pixel_avg) ;
}

int main(int argc , char* argv[])
{
  uint8_t  pixel_avg ;
  uint8_t  sample_rect_offset = 0 ;
  bool     is_inverted        = false ;
  bool     is_bright          = false ;

  if ((XDisplay = XOpenDisplay(NULL)) != NULL)
  {
    RootXwin  = DefaultRootWindow(XDisplay) ;
    XColorMap = XDefaultColormap(XDisplay , XDefaultScreen(XDisplay)) ;
  }
  else
  {
    printf("ERROR: Could not acquire X display\n") ;
    return(1) ;
  }

  system("xcalib -alter -clear" ) ;

  while (! usleep(SLEEP_IVL))
  {
    pixel_avg = GetPixelAvg(sample_rect_offset) ;
    is_bright = ( is_inverted && (pixel_avg < PIXEL_AVG_THRESHOLD)) ||
                (!is_inverted && (pixel_avg > PIXEL_AVG_THRESHOLD)) ;

DEBUG_MAIN_LOOP

    if (is_bright)
    {
      is_inverted = ! is_inverted ; is_bright = false ;

      if (is_inverted) system("xcalib -alter -invert") ;
      else             system("xcalib -alter -clear" ) ;
    }
  }

  XCloseDisplay(XDisplay) ;
}
