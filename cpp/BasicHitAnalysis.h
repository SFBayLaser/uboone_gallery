/*************************************************************
 *
 * A basic hit analysis object
 *
 * Adapted from an example main program from Wes Ketchum
 *
 *************************************************************/

#ifndef BasicHitAnalysis_h
#define BasicHitAnalysis_h

//Gallery event
#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"

namespace HitAnalysis
{
    
class BasicHitAnalysis
{
public:
    BasicHitAnalysis(const art::InputTag& inputTag);
   ~BasicHitAnalysis();
    
    void setupAnalysis(const std::string&);
    
    void analyzeEvent(gallery::Event&);
private:
    
    //TTree Structs (could make ntuples, but less extendible...)
    struct EventTreeObject
    {
        unsigned int run;
        unsigned int ev;
        unsigned int nhits_u;
        unsigned int nhits_v;
        unsigned int nhits_w;
        
        float        hit_amp_mean_u;
        float        hit_amp_rms_u;
        float        hit_amp_mean_v;
        float        hit_amp_rms_v;
        float        hit_amp_mean_w;
        float        hit_amp_rms_w;
        
        float        hit_integral_mean_u;
        float        hit_integral_rms_u;
        float        hit_integral_mean_v;
        float        hit_integral_rms_v;
        float        hit_integral_mean_w;
        float        hit_integral_rms_w;
        
        float        hit_sumadc_mean_u;
        float        hit_sumadc_rms_u;
        float        hit_sumadc_mean_v;
        float        hit_sumadc_rms_v;
        float        hit_sumadc_mean_w;
        float        hit_sumadc_rms_w;
        
        float        hit_mult_mean_u;
        float        hit_mult_rms_u;
        float        hit_mult_mean_v;
        float        hit_mult_rms_v;
        float        hit_mult_mean_w;
        float        hit_mult_rms_w;
        
        std::string BranchString(){
            std::string str("run/i:ev/i:nhits_u/i:nhits_v/i:nhits_w/i:hit_amp_mean_u/F:hit_amp_rms_u/F:hit_amp_mean_v/F:hit_amp_rms_v/F:hit_amp_mean_w/F:hit_amp_rms_w/F:hit_integral_mean_u/F:hit_integral_rms_u/F:hit_integral_mean_v/F:hit_integral_rms_v/F:hit_integral_mean_w/F:hit_integral_rms_w/F:hit_sumadc_mean_u/F:hit_sumadc_rms_u/F:hit_sumadc_mean_v/F:hit_sumadc_rms_v/F:hit_sumadc_mean_w/F:hit_sumadc_rms_w/F:hit_mult_mean_u/F:hit_mult_rms_u/F:hit_mult_mean_v/F:hit_mult_rms_v/F:hit_mult_mean_w/F:hit_mult_rms_w/F");
            return str;
        }
        
        void Init()
        {
            run=0; ev=0;
            nhits_u=0; nhits_v=0; nhits_w=0;
            
            hit_amp_mean_u=0; hit_amp_rms_u=0;
            hit_amp_mean_v=0; hit_amp_rms_v=0;
            hit_amp_mean_w=0; hit_amp_rms_w=0;
            
            hit_integral_mean_u=0; hit_integral_rms_u=0;
            hit_integral_mean_v=0; hit_integral_rms_v=0;
            hit_integral_mean_w=0; hit_integral_rms_w=0;
            
            hit_sumadc_mean_u=0; hit_sumadc_rms_u=0;
            hit_sumadc_mean_v=0; hit_sumadc_rms_v=0;
            hit_sumadc_mean_w=0; hit_sumadc_rms_w=0;
            
            hit_mult_mean_u=0; hit_mult_rms_u=0;
            hit_mult_mean_v=0; hit_mult_rms_v=0;
            hit_mult_mean_w=0; hit_mult_rms_w=0;
        }
        
    };
    
    
    struct HitTreeObject
    {
        unsigned int run;
        unsigned int ev;
        unsigned int ch;
        unsigned int start_tick;
        unsigned int end_tick;
        unsigned int roi_start;
        unsigned int roi_end;
        unsigned int roi_size;
        float        time;
        float        time_err;
        float        rms;
        float        amp;
        float        amp_err;
        float        sumadc;
        float        integral;
        float        integral_err;
        float        gof;
        int          mult;
        int          idx;
        int          view;
        
        //function to return branch list"
        std::string BranchString(){
            std::string str("run/i:ev/i:ch/i:start_tick/i:end_tick/i:roi_start/i:roi_end/i:roi_size/i:time/F:time_err/F:rms/F:amp/F:amp_err/F:sumadc/F:integral/F:integral_err/F:gof/F:mult/I:idx/I:view/I");
            return str;
        }
    };
    
    std::string                      fBranchName;
    art::InputTag                    fHitTag;
    std::unique_ptr<TTree>           fEventTree;
    std::unique_ptr<EventTreeObject> fEventTreeObj;
    std::unique_ptr<TTree>           fHitTree;
    std::unique_ptr<HitTreeObject>   fHitTreeObj;
};

}
#endif
