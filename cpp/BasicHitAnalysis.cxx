/*************************************************************
 *
 * HitAna program
 *
 * This is a simple demonstration of reading a LArSoft file
 * and accessing recob::Hit and recob::Wire information in gallery
 *
 * Wesley Ketchum (wketchum@fnal.gov), May22, 2017
 *
 *************************************************************/

// Our class definition
#include "BasicHitAnalysis.h"

//some standard C++ includes
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <chrono>

//some ROOT includes
#include "TInterpreter.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"

//"art" includes (canvas, and gallery)
#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"
#include "gallery/ValidHandle.h"
#include "canvas/Persistency/Common/FindMany.h"
#include "canvas/Persistency/Common/FindOne.h"

//"larsoft" object includes
#include "lardataobj/RecoBase/Wire.h"
#include "lardataobj/RecoBase/Hit.h"

//our own includes!
#include "hist_utilities.h"

namespace HitAnalysis
{

BasicHitAnalysis::BasicHitAnalysis(const art::InputTag& inputTag) :
        fHitTag(inputTag)
{
    return;
}

BasicHitAnalysis::~BasicHitAnalysis()
{
    return;
}

void BasicHitAnalysis::setupAnalysis(const std::string& branchName)
{
    //Create a Tree to store event information
    fEventTreeObj = std::make_unique<EventTreeObject>();
    fEventTree    = std::make_unique<TTree>("tree_ev_hit","HitAna: Hits per event tree");
    fEventTree->Branch("",fEventTreeObj.get(),fEventTreeObj->BranchString().c_str());
    
    //Create a Tree to store hit information
    fHitTreeObj = std::make_unique<HitTreeObject>();
    fHitTree = std::make_unique<TTree>("tree_hit","HitAna: Hit tree");
    fHitTree->Branch("hit",fHitTreeObj.get(),fHitTreeObj->BranchString().c_str());
    
    return;
}

void BasicHitAnalysis::analyzeEvent(gallery::Event& event)
{
    //clear out event tree objects
    fEventTreeObj->Init();
    
    //time the loop
    auto t_begin = std::chrono::high_resolution_clock::now();
    
    //to get run and event info, you use this "eventAuxillary()" object.
    std::cout << "Processing "
    << "Run " << event.eventAuxiliary().run() << ", "
    << "Event " << event.eventAuxiliary().event() << std::endl;
    
    fHitTreeObj->run   = event.eventAuxiliary().run();
    fHitTreeObj->ev    = event.eventAuxiliary().event();
    fEventTreeObj->run = event.eventAuxiliary().run();
    fEventTreeObj->ev  = event.eventAuxiliary().event();
    
    //get handle to hits
    auto const& hit_handle = event.getValidHandle<std::vector<recob::Hit>>(fHitTag);
    auto const& hit_vec(*hit_handle);
    
    //get the associations to the recob::Wire objects
    art::FindOne<recob::Wire> wire_per_hit(hit_handle,event,fHitTag);
    
    //For good measure, print out the number of hits
    std::cout << "\tThere are " << hit_vec.size() << " hits in this event." << std::endl;
    
    //loop over the hits
    //for(auto const& hit : hit_vec){
    for(size_t i_h=0; i_h<hit_vec.size(); ++i_h){
        
        auto const& hit = hit_vec[i_h];
        
        //get associated wire object
        auto const& wire = (wire_per_hit.at(i_h)).ref();
        auto const& roi  = wire.SignalROI().find_range((int)(hit.PeakTime()));
        
        //do the event tree info
        if(hit.View()==geo::View_t::kU){
            fEventTreeObj->nhits_u++;
            fEventTreeObj->hit_amp_mean_u += hit.PeakAmplitude();
            fEventTreeObj->hit_integral_mean_u += hit.Integral();
            fEventTreeObj->hit_sumadc_mean_u += hit.SummedADC();
            fEventTreeObj->hit_mult_mean_u += hit.Multiplicity();
        }
        else if(hit.View()==geo::View_t::kV){
            fEventTreeObj->nhits_v++;
            fEventTreeObj->hit_amp_mean_v += hit.PeakAmplitude();
            fEventTreeObj->hit_integral_mean_v += hit.Integral();
            fEventTreeObj->hit_sumadc_mean_v += hit.SummedADC();
            fEventTreeObj->hit_mult_mean_v += hit.Multiplicity();
        }
        else if(hit.View()==geo::View_t::kW){
            fEventTreeObj->nhits_w++;
            fEventTreeObj->hit_amp_mean_w += hit.PeakAmplitude();
            fEventTreeObj->hit_integral_mean_w += hit.Integral();
            fEventTreeObj->hit_sumadc_mean_w += hit.SummedADC();
            fEventTreeObj->hit_mult_mean_w += hit.Multiplicity();
        }
        
        //now fill the hit info
        fHitTreeObj->start_tick = hit.StartTick();
        fHitTreeObj->end_tick   = hit.EndTick();
        fHitTreeObj->roi_start = roi.begin_index();
        fHitTreeObj->roi_end = roi.end_index();
        fHitTreeObj->roi_size = roi.size();
        fHitTreeObj->ch = hit.Channel();
        fHitTreeObj->time = hit.PeakTime();
        fHitTreeObj->time_err = hit.SigmaPeakTime();
        fHitTreeObj->rms = hit.RMS();
        fHitTreeObj->amp = hit.PeakAmplitude();
        fHitTreeObj->amp_err = hit.SigmaPeakAmplitude();
        fHitTreeObj->sumadc = hit.SummedADC();
        fHitTreeObj->integral = hit.Integral();
        fHitTreeObj->integral_err = hit.SigmaIntegral();
        fHitTreeObj->gof = hit.GoodnessOfFit();
        fHitTreeObj->mult = hit.Multiplicity();
        fHitTreeObj->idx = hit.LocalIndex();
        fHitTreeObj->view = hit.View();
        
        fHitTree->Fill();
    }//end first loop over hits.
    
    
    //mean calculations...
    fEventTreeObj->hit_amp_mean_u = fEventTreeObj->hit_amp_mean_u / fEventTreeObj->nhits_u;
    fEventTreeObj->hit_amp_mean_v = fEventTreeObj->hit_amp_mean_v / fEventTreeObj->nhits_v;
    fEventTreeObj->hit_amp_mean_w = fEventTreeObj->hit_amp_mean_w / fEventTreeObj->nhits_w;
    
    fEventTreeObj->hit_integral_mean_u = fEventTreeObj->hit_integral_mean_u / fEventTreeObj->nhits_u;
    fEventTreeObj->hit_integral_mean_v = fEventTreeObj->hit_integral_mean_v / fEventTreeObj->nhits_v;
    fEventTreeObj->hit_integral_mean_w = fEventTreeObj->hit_integral_mean_w / fEventTreeObj->nhits_w;
    
    fEventTreeObj->hit_sumadc_mean_u = fEventTreeObj->hit_sumadc_mean_u / fEventTreeObj->nhits_u;
    fEventTreeObj->hit_sumadc_mean_v = fEventTreeObj->hit_sumadc_mean_v / fEventTreeObj->nhits_v;
    fEventTreeObj->hit_sumadc_mean_w = fEventTreeObj->hit_sumadc_mean_w / fEventTreeObj->nhits_w;
    
    fEventTreeObj->hit_mult_mean_u = fEventTreeObj->hit_mult_mean_u / fEventTreeObj->nhits_u;
    fEventTreeObj->hit_mult_mean_v = fEventTreeObj->hit_mult_mean_v / fEventTreeObj->nhits_v;
    fEventTreeObj->hit_mult_mean_w = fEventTreeObj->hit_mult_mean_w / fEventTreeObj->nhits_w;
    
    //second loop over hits to do rms calculations...
    for(auto const& hit : hit_vec){
        
        if(hit.View()==geo::View_t::kU){
            fEventTreeObj->hit_amp_rms_u += (hit.PeakAmplitude()-fEventTreeObj->hit_amp_mean_u)*(hit.PeakAmplitude()-fEventTreeObj->hit_amp_mean_u);
            fEventTreeObj->hit_integral_rms_u += (hit.Integral()-fEventTreeObj->hit_integral_mean_u)*(hit.Integral()-fEventTreeObj->hit_integral_mean_u);
            fEventTreeObj->hit_sumadc_rms_u += (hit.SummedADC()-fEventTreeObj->hit_sumadc_mean_u)*(hit.SummedADC()-fEventTreeObj->hit_sumadc_mean_u);
            fEventTreeObj->hit_mult_rms_u += (hit.Multiplicity()-fEventTreeObj->hit_mult_mean_u)*(hit.Multiplicity()-fEventTreeObj->hit_mult_mean_u);
        }
        else if(hit.View()==geo::View_t::kV){
            fEventTreeObj->hit_amp_rms_v += (hit.PeakAmplitude()-fEventTreeObj->hit_amp_mean_v)*(hit.PeakAmplitude()-fEventTreeObj->hit_amp_mean_v);
            fEventTreeObj->hit_integral_rms_v += (hit.Integral()-fEventTreeObj->hit_integral_mean_v)*(hit.Integral()-fEventTreeObj->hit_integral_mean_v);
            fEventTreeObj->hit_sumadc_rms_v += (hit.SummedADC()-fEventTreeObj->hit_sumadc_mean_v)*(hit.SummedADC()-fEventTreeObj->hit_sumadc_mean_v);
            fEventTreeObj->hit_mult_rms_v += (hit.Multiplicity()-fEventTreeObj->hit_mult_mean_v)*(hit.Multiplicity()-fEventTreeObj->hit_mult_mean_v);
        }
        else if(hit.View()==geo::View_t::kW){
            fEventTreeObj->hit_amp_rms_w += (hit.PeakAmplitude()-fEventTreeObj->hit_amp_mean_w)*(hit.PeakAmplitude()-fEventTreeObj->hit_amp_mean_w);
            fEventTreeObj->hit_integral_rms_w += (hit.Integral()-fEventTreeObj->hit_integral_mean_w)*(hit.Integral()-fEventTreeObj->hit_integral_mean_w);
            fEventTreeObj->hit_sumadc_rms_w += (hit.SummedADC()-fEventTreeObj->hit_sumadc_mean_w)*(hit.SummedADC()-fEventTreeObj->hit_sumadc_mean_w);
            fEventTreeObj->hit_mult_rms_w += (hit.Multiplicity()-fEventTreeObj->hit_mult_mean_w)*(hit.Multiplicity()-fEventTreeObj->hit_mult_mean_w);
        }
        
    }//end second loop over hits;
    
    //rms calculations...
    fEventTreeObj->hit_amp_rms_u = std::sqrt(fEventTreeObj->hit_amp_rms_u/(fEventTreeObj->nhits_u-1));
    fEventTreeObj->hit_integral_rms_u = std::sqrt(fEventTreeObj->hit_integral_rms_u/(fEventTreeObj->nhits_u-1));
    fEventTreeObj->hit_sumadc_rms_u = std::sqrt(fEventTreeObj->hit_sumadc_rms_u/(fEventTreeObj->nhits_u-1));
    fEventTreeObj->hit_mult_rms_u = std::sqrt(fEventTreeObj->hit_mult_rms_u/(fEventTreeObj->nhits_u-1));
    
    fEventTreeObj->hit_amp_rms_v = std::sqrt(fEventTreeObj->hit_amp_rms_v/(fEventTreeObj->nhits_v-1));
    fEventTreeObj->hit_integral_rms_v = std::sqrt(fEventTreeObj->hit_integral_rms_v/(fEventTreeObj->nhits_v-1));
    fEventTreeObj->hit_sumadc_rms_v = std::sqrt(fEventTreeObj->hit_sumadc_rms_v/(fEventTreeObj->nhits_v-1));
    fEventTreeObj->hit_mult_rms_v = std::sqrt(fEventTreeObj->hit_mult_rms_v/(fEventTreeObj->nhits_v-1));
    
    fEventTreeObj->hit_amp_rms_w = std::sqrt(fEventTreeObj->hit_amp_rms_w/(fEventTreeObj->nhits_w-1));
    fEventTreeObj->hit_integral_rms_w = std::sqrt(fEventTreeObj->hit_integral_rms_w/(fEventTreeObj->nhits_w-1));
    fEventTreeObj->hit_sumadc_rms_w = std::sqrt(fEventTreeObj->hit_sumadc_rms_w/(fEventTreeObj->nhits_w-1));
    fEventTreeObj->hit_mult_rms_w = std::sqrt(fEventTreeObj->hit_mult_rms_w/(fEventTreeObj->nhits_w-1));
    
    fEventTree->Fill();
    
    auto t_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double,std::milli> time_total_ms(t_end-t_begin);
    std::cout << "\tEvent took " << time_total_ms.count() << " ms to process." << std::endl;
    
    return;
}


} // namespace
