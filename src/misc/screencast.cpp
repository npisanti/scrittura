
// Nicola Pisanti, GPLv3 License, 2016

#include "screencast.h"
    
void np::ffmpeg_record(int x, int y, int w, int h){
    
    string command = "ffmpeg -thread_queue_size 2048 -video_size ";
    command+= to_string(w);
    command+="x";
    command+= to_string(h);
    command+= " -framerate 60  -f x11grab -i :0.0+";
    command+= to_string(x);
    command+=",";
    command+= to_string(y);
    command+= " -f pulse -ac 2 -sample_rate 44100 -thread_queue_size 2048 -i alsa_output.pci-0000_00_1b.0.analog-stereo.monitor -vcodec libx264 -b:v 5000k -b:a 320k screencast";
    command+=ofGetTimestampString();
    command+=".mp4 &";
    
    cout<<"[nplib] executing record command: "<<command<<"\n";
    
    system ( command.c_str() );
}

    
void np::ffmpeg_record( int w, int h){
    np::ffmpeg_record( 0, 0, w, h );
}

void np::ffmpeg_stop(){
    
    cout<<"[nplib] executing stopping command: pkill ffmpeg &\n";
    system ("pkill ffmpeg &");
}
