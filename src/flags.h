

#ifndef __ARM_NEON__
    #define PULSE
#endif

#define MASTER_PITCH 29


//#define USE_PROJECTOR

#ifdef USE_PROJECTOR
    #define PROJECTOR_WIDTH 720
    #define PROJECTOR_HEIGHT 480
#else
    #define PROJECTOR_WIDTH 0
#endif


//#define SCREENCAST

#ifdef SCREENCAST
    #define WIDTH 1400
    #define HEIGHT 1040
#else
    #ifdef __ARM_NEON__
        #define WIDTH 640
        #define HEIGHT 480       
    #else
        #define WIDTH 1600
        #define HEIGHT 900    
    #endif
#endif

#define SUBSONICS
