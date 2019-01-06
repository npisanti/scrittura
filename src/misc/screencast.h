
#pragma once

#include "ofMain.h"
#include <stdlib.h>

namespace np{
    
    void ffmpeg_record(int w, int h);
    void ffmpeg_record(int x, int y, int w, int h);
    
    void ffmpeg_stop();
    
    
}
