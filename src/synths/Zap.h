
#pragma once

#include "ofxPDSP.h"

namespace np{

class Zap : public pdsp::Patchable{ 
public:
        Zap(){ patch(); }
        Zap(const Zap & other){ patch(); }

        void patch();
        
        void setGain(float dBvalue);

        float meter_env() const;

private:
        pdsp::PatchNode     triggers;
        pdsp::FMOperator    osc;
        pdsp::AHR           aEnv;
        pdsp::AHR           pEnv;
        pdsp::Amp           amp;
        pdsp::Amp           pModAmt;
        pdsp::Amp           fbModAmt;
        
};

}
