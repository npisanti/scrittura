
// Nicola Pisanti, GPLv3 License, 2016

#include "Zap.h"

void np::Zap::patch() {
        addModuleInput("trig", triggers); 
        addModuleInput("pitch", osc.in_pitch()); 
        addModuleInput("pitch_mod_amount", pModAmt.in_mod()); 
        addModuleInput("fb_mod_amount", fbModAmt.in_mod()); 
        addModuleInput("mod_env_time", pEnv.in_release()); 
        addModuleInput("amp_env_time", aEnv.in_hold()); 
        addModuleOutput("signal", amp ); 

        //patching
        //aEnv.enableDBTriggering(-18.0f, 0.0f);
        0.5f >> pEnv.in_velocity();
        
                                    triggers >> aEnv.set(3.0f, 50.0f, 20.0f)  >> amp.in_mod();
                                                                          osc >> amp;
                                                      pEnv >> fbModAmt >> osc.in_fb();
        triggers >> pEnv.set(0.0f, 0.0f,  50.0f) >> pModAmt.set(48.0f) >> osc.in_pitch();
                                                                 36.0f >> osc.in_pitch(); // overriden by ensemble
}

void np::Zap::setGain(float dBvalue){
    aEnv * dB(dBvalue) >> amp;
}

float np::Zap::meter_env() const{
    return aEnv.meter_output();
}
