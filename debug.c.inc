#define DEBUG 0


#if DEBUG

# define DEBUG_GETPIXELAVG                                                                                             \
  printf("\e[1;1H\e[2J") ; printf("GetPixelAvg() xwin_w=%d xwin_h=%d\n" , xwin_w , xwin_h) ;                           \
  printf("GetPixelAvg() RGB TL=(%3d %3d %3d)" , xcolors[0].red/256 , xcolors[0].green/256 , xcolors[0].blue/256) ;     \
  printf(" TR=(%2X %2X %2X)\n"                , xcolors[1].red/256 , xcolors[1].green/256 , xcolors[1].blue/256) ;     \
  printf("GetPixelAvg()     BL=(%3d %3d %3d)" , xcolors[2].red/256 , xcolors[2].green/256 , xcolors[2].blue/256) ;     \
  printf(" BR=(%2X %2X %2X)\n"                , xcolors[3].red/256 , xcolors[3].green/256 , xcolors[3].blue/256) ;     \
  printf("GetPixelAvg() pixels[]=[ %3d %3d\n" , ( (xcolors[0].red + xcolors[0].green + xcolors[0].blue) / 3) / 256 ,   \
                                                ( (xcolors[1].red + xcolors[1].green + xcolors[1].blue) / 3) / 256 ) ; \
  printf("                         %3d %3d ]" , ( (xcolors[2].red + xcolors[2].green + xcolors[2].blue) / 3) / 256 ,   \
                                                ( (xcolors[3].red + xcolors[3].green + xcolors[3].blue) / 3) / 256 ) ; \
  printf(" pixel_avg=%d\n" , pixel_avg) ;
# define DEBUG_MAIN_LOOP                                                                          \
  printf("main() %d " , pixel_avg) ;                                                              \
  if (pixel_avg > PIXEL_AVG_THRESHOLD) { printf("> PIXEL_AVG_THRESHOLD\n") ; system("mybeep") ; } \
  else                                   printf("< PIXEL_AVG_THRESHOLD\n") ;                      \
  printf("main() is_bright=%d is_inverted=%d\n" , is_bright , is_inverted) ;

#else

# define DEBUG_GETPIXELAVG ;
# define DEBUG_MAIN_LOOP   ;

#endif
