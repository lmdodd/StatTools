#include "TCanvas.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h" 
#include <math.h>
#include <boost/algorithm/string.hpp>
#include "HiggsAnalysis/CombinedLimit/interface/th1fmorph.h"
#include <TEfficiency.h>
#include <TMath.h>
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
//#include "FineBins.h"

struct BkgOutput {
    float DATA;
    float W, dW;
    float QCD, dQCD;
    float ZLFT, dZLFT;
    float ZJFT, dZJFT;
    float TOP, dTOP;
    float VV, dVV;
    float EWKZ, dEWKZ;
    float ZTT, dZTT;

    BkgOutput(){};

    BkgOutput(float i) {
        // This assumes that a-d are all of the same type and all in order
        std::fill(&DATA, &dZTT+1, i);
    }

};



class DataCardCreatorControlRegion {
    public:

        DataCardCreatorControlRegion(optutl::CommandLineParser parser) {
            channel_ = parser.stringValue("channel");
            shifts_  = parser.stringVector("shifts");
            energy_ = parser.stringValue("energy");
            samesign_ = parser.doubleValue("samesign");


            std::string name_=channel_;
            if (channel_=="eleTau") filelabel_="et";
            if (channel_=="muTau") filelabel_="mt";


            //create the name you need to add to the histograms 
            //in the root file 
            for(unsigned int i=0;i<shifts_.size();++i) {
                std::string shiftL = shifts_.at(i);
                shiftL.resize(1);
                boost::to_lower(shiftL);
                std::cout<<"shiftL: "<<shiftL<<std::endl;
                std::string chan = "mt";
                if(channel_=="eleTau") chan = "et";
                if (shifts_[i]=="Jet"){
                    for (unsigned int j=0;j<jetshifts_.size();j++){ 
                        if (jetshifts_[j]=="Total") shiftsPostFix_.push_back("CMS_scale_"+shiftL+"_"+energy_);
                        else shiftsPostFix_.push_back("CMS_scale_"+shiftL+"_"+jetshifts_[j]+"_"+energy_);
                    }
                }
                else { 
                    shiftsPostFix_.push_back("CMS_scale_"+shiftL+"_"+energy_);
                    for (unsigned int t=0;t<taushifts_.size();t++)
                        shiftsPostFix_.push_back("CMS_scale_"+shiftL+"_"+taushifts_[t]+"_"+energy_);
                }
            }

            std::cout<<"shiftsPostFix: ";
            for (unsigned int i=0;i<shiftsPostFix_.size();i++){ std::cout<<shiftsPostFix_[i]<<",";}
            std::cout<<std::endl;

            //read input files
            zttFile_  = parser.stringValue("zttFile");
            zllFile_  = parser.stringValue("zllFile");
            wFile_    = parser.stringValue("wFile");
            vvFile_   = parser.stringValue("vvFile");
            ewkFile_   = parser.stringValue("ewkFile");
            topFile_  = parser.stringValue("topFile");
            qcdFile_  = parser.stringValue("qcdFile");
            dataFile_  = parser.stringValue("dataFile");

            //read control and signal regions
            preSelection_ = parser.stringValue("preselection");

            signalSelection_      = parser.stringValue("signalselection");
            wSelection_           = parser.stringValue("wselection");
            relaxedSelection_     = parser.stringValue("relaxedSelection");
            trigSelection_        = parser.stringValue("trigSelection");
            trigSelectionData_        = parser.stringValue("trigSelectionData");
            blinding_             = parser.stringValue("blinding");
            charge_               = parser.stringValue("charge");
            if (samesign_) charge_="abs(charge)==2";


            ZTT_genTauSel_        = "gen_match_2==5"; //Zttyield
            ZLFT_genLSel_         = "gen_match_2<5";
            ZJFT_genLReject_      = "gen_match_2==6";
            ZLL_genLLSel_        = "(gen_match_2<5||gen_match_2==6)"; //Zttyield

            //
            if(samesign_>0)
                osSignalSelection_    = signalSelection_+"&&abs(charge)==2";
            else
                osSignalSelection_    = signalSelection_+"&&charge==0";

            ssSignalSelection_    = signalSelection_+"&&abs(charge)==2";
            if(samesign_>0)
                osWSelection_         = wSelection_+"&&abs(charge)==2";
            else
                osWSelection_         = wSelection_+"&&charge==0";

            osWSelectionData_         = osWSelection_+"&&EffCSVWeight0==1";
            ssWSelection_         = wSelection_+"&&abs(charge)==2";
            ssWSelectionData_         = ssWSelection_+"&&EffCSVWeight0==1";
            verbose_              = parser.integerValue("verbose");

            //read systematic uncertainties 
            luminosity_    = parser.doubleValue("luminosity");
            luminosityErr_ = parser.doubleValue("luminosityErr");
            muID_          = parser.doubleValue("muID");
            muIDErr_       = parser.doubleValue("muIDErr");
            bID_           = parser.doubleValue("bID");
            bIDErr_        = parser.doubleValue("bIDErr");
            bMisID_        = parser.doubleValue("bMisID");
            bMisIDErr_     = parser.doubleValue("bMisIDErr");
            eleID_         = parser.doubleValue("eleID");
            eleIDErr_      = parser.doubleValue("eleIDErr");
            tauID_         = parser.doubleValue("tauID");
            tauIDHigh_     = parser.doubleValue("tauIDHigh");
            tauIDErr_      = parser.doubleValue("tauIDErr");
            zttScale_      = parser.doubleValue("zttScale");
            zttScaleErr_   = parser.doubleValue("zttScaleErr");

            //read the basic varibale you will put in the histogram
            variable_      = parser.stringValue("variable");

            //read the event weight for MC and embedded
            weight_        = parser.stringValue("weight");
            Zweight_        = parser.stringValue("Zweight");
            ZUnrollweight_        = parser.stringValue("ZUnrollweight");
            TTweight_        = parser.stringValue("TTweight");
            embWeight_     = parser.stringValue("embWeight");
            ggHWeight_     = parser.stringValue("ggHWeight");

            //define the histogram binning
            bins_ = parser.integerValue("bins");
            max_  = parser.doubleValue("max");
            min_  = parser.doubleValue("min");


            //Define background uncertainty Errors
            topErr_     = parser.doubleValue("topErr");
            qcdErr_     = parser.doubleValue("qcdErr");
            vvErr_      = parser.doubleValue("vvErr");
            zlftErr_    = parser.doubleValue("zlftErr");
            zlftFactor_ = parser.stringValue("zlftFactor");

            zjftErr_      = parser.doubleValue("zjftErr");
            wFactorErr_   = parser.doubleValue("wFactorErr");
            qcdFactor_    = parser.doubleValue("qcdFactor");
            qcdFactorErr_ = parser.doubleValue("qcdFactorErr");

            dir_ = parser.stringValue("dir");

            scaleUp_ = parser.doubleValue("scaleUp");

            fout_ = new TFile(parser.stringValue("outputfile").c_str(),"RECREATE");

        }


        void makeHiggsShape(std::string preselection, std::string categoryselection, std::string prefix) {

            std::pair<float,float> tmp;
            float legCorr=1.0;

            if(muID_!=0&&eleID_!=0) {legCorr*=muID_*eleID_;}
            if(muID_!=0&&eleID_==0) {legCorr*=muID_*tauID_;}
            if(muID_==0&&eleID_!=0) {legCorr*=eleID_*tauID_;}
            std::cout<<"Make Higgs Shape"<<std::endl;
            tmp= createHistogramAndShifts(dir_+"ggH110.root","ggH110","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_,luminosity_*legCorr*57.9*0.0791,prefix);
            tmp= createHistogramAndShifts(dir_+"ggH110.root","ggH110_CMS_scale_gg_13TeVUp","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_+"*"+ggHWeight_,luminosity_*legCorr*57.9*0.0791,prefix);
            tmp= createHistogramAndShifts(dir_+"ggH110.root","ggH110_CMS_scale_gg_13TeVDown","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_+"*(2-"+ggHWeight_+")",luminosity_*legCorr*57.9*0.0791,prefix);

            tmp= createHistogramAndShifts(dir_+"ggH120.root","ggH120","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_,luminosity_*legCorr*52.22*0.0698,prefix);
            tmp= createHistogramAndShifts(dir_+"ggH120.root","ggH120_CMS_scale_gg_13TeVUp","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_+"*"+ggHWeight_,luminosity_*legCorr*52.22*0.0698,prefix);
            tmp= createHistogramAndShifts(dir_+"ggH120.root","ggH120_CMS_scale_gg_13TeVDown","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_+"*(2-"+ggHWeight_+")",luminosity_*legCorr*52.22*0.0698,prefix);
            tmp= createHistogramAndShifts(dir_+"ggH125.root","ggH125","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_,luminosity_*legCorr*48.58*0.0627,prefix);
            tmp= createHistogramAndShifts(dir_+"ggH125.root","ggH125_CMS_scale_gg_13TeVUp","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_+"*"+ggHWeight_,luminosity_*legCorr*48.58*0.0627,prefix);
            tmp= createHistogramAndShifts(dir_+"ggH125.root","ggH125_CMS_scale_gg_13TeVDown","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_+"*(2-"+ggHWeight_+")",luminosity_*legCorr*48.58*0.0627,prefix);
            tmp= createHistogramAndShifts(dir_+"ggH130.root","ggH130","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_,luminosity_*legCorr*45.31*0.0541,prefix);
            tmp= createHistogramAndShifts(dir_+"ggH130.root","ggH130_CMS_scale_gg_13TeVUp","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_+"*"+ggHWeight_,luminosity_*legCorr*45.31*0.0541,prefix);
            tmp= createHistogramAndShifts(dir_+"ggH130.root","ggH130_CMS_scale_gg_13TeVDown","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_+"*(2-"+ggHWeight_+")",luminosity_*legCorr*45.31*0.0541,prefix);


            tmp= createHistogramAndShifts(dir_+"ggH140.root","ggH140","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_,luminosity_*legCorr*36.0*0.0360,prefix);
            tmp= createHistogramAndShifts(dir_+"ggH140.root","ggH140_CMS_scale_gg_13TeVUp","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_+"*"+ggHWeight_,luminosity_*legCorr*36.0*0.0360,prefix);
            tmp= createHistogramAndShifts(dir_+"ggH140.root","ggH140_CMS_scale_gg_13TeVDown","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_+"*(2-"+ggHWeight_+")",luminosity_*legCorr*36.0*0.0360,prefix);


            tmp= createHistogramAndShifts(dir_+"vbfH120.root","qqH120","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_,luminosity_*legCorr*3.935*0.0698,prefix);
            tmp= createHistogramAndShifts(dir_+"vbfH125.root","qqH125","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_,luminosity_*legCorr*3.782*0.0627,prefix);
            tmp= createHistogramAndShifts(dir_+"vbfH130.root","qqH130","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_,luminosity_*legCorr*3.637*0.0541,prefix);
            tmp= createHistogramAndShifts(dir_+"ZH120.root","ZH120","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_,luminosity_*legCorr*0.994*0.0698,prefix);
            tmp= createHistogramAndShifts(dir_+"ZH125.root","ZH125","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_,luminosity_*legCorr*0.884*0.0627,prefix);
            tmp= createHistogramAndShifts(dir_+"ZH130.root","ZH130","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_,luminosity_*legCorr*0.790*0.0541,prefix);
            tmp= createHistogramAndShifts(dir_+"WpH120.root","WH120","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_,luminosity_*legCorr*0.9558*0.0698,prefix);
            tmp= createHistogramAndShifts(dir_+"WpH125.root","WH125","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_,luminosity_*legCorr*0.840*0.0627,prefix);
            tmp= createHistogramAndShifts(dir_+"WpH130.root","WH130","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_,luminosity_*legCorr*0.7414*0.0541,prefix);
            tmp= createHistogramAndShifts(dir_+"WmH120.root","WmH120","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_,luminosity_*legCorr*0.6092*0.0698,prefix);
            tmp= createHistogramAndShifts(dir_+"WmH125.root","WmH125","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_,luminosity_*legCorr*0.5328*0.0627,prefix);
            tmp= createHistogramAndShifts(dir_+"WmH130.root","WmH130","("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")",weight_,luminosity_*legCorr*0.4676*0.0541,prefix);


            std::cout<<"Made Higgs Shape"<<std::endl;
            addHistogramShifts(filelabel_+prefix,"WH120","WmH120"); 
            addHistogramShifts(filelabel_+prefix,"WH125","WmH125"); 
            addHistogramShifts(filelabel_+prefix,"WH130","WmH130"); 
            std::cout<<"Added WH samples"<<std::endl;
        }

        /*
         * Run the full extrapolation in the Btag category
         * Needed: BtagSF, categorySelection
         *
         */

        BkgOutput runFullExtrapBtag(std::string relaxedSelection, std::string wSel, std::string preSelection, std::string categorySelection_, std::string prefix, std::string zShape, float topExtrap, float zExtrap, float zExtrapErr, std::string bTagSF) {

            std::cout<<"======================"<<std::endl;
            std::cout<<"Run Full Extrapolation"<<std::endl;
            std::cout<<"categorySelection: "<<categorySelection_<<std::endl;
            //TODO: Fix this category seleciton business, should be more elegant
            std::string categorySelection = categorySelection_;
            std::string dummySelection_   = osSignalSelection_;


            BkgOutput output;    

            float leg1Corr=1.0;
            if(muID_!=0) leg1Corr*=muID_;
            if(eleID_!=0) leg1Corr*=eleID_;

            printf("Tau ID Scale Factor is %.3f \n",tauID_);

            std::cout<<"Select DATA"<<std::endl;
            std::pair<float,float> dataY         = createHistogramAndShifts(dataFile_,"data_obs","("+preSelection+"&&"+trigSelectionData_+"&&"+osSignalSelection_+"&&"+categorySelection+")",scaleUp_,prefix);
            output.DATA = dataY.first;
            std::cout<<"      DATA Yield: "<< output.DATA<<std::endl;
            std::cout<<"      DATA Selection: "<<preSelection<<"&&"<<trigSelectionData_<<"&&"<<osSignalSelection_<<"&&"<<categorySelection<<std::endl; 



            std::cout<<"Create DiBoson"<<std::endl;
            //Create Diboson
            std::pair<float,float> vvYield      = createHistogramAndShifts(vvFile_,"VV","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")",weight_,luminosity_*leg1Corr*tauID_,prefix);
            std::pair<float,float> vvJYield      = createHistogramAndShifts(vvFile_,"VVJ","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&gen_match_2!=5)",weight_,luminosity_*leg1Corr,prefix);
            std::pair<float,float> vvTYield      = createHistogramAndShifts(vvFile_,"VVT","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&gen_match_2==5)",weight_,luminosity_*leg1Corr*tauID_,prefix);

            printf("      Diboson events in signal region = %f + %f \n",vvYield.first,vvYield.second);
            output.VV  = vvYield.first;
            output.dVV = vvYield.second;

            std::cout<<"Create EWKZ"<<std::endl;
            //Create Diboson
            std::pair<float,float> ewkYield      = createHistogramAndShifts(ewkFile_,"EWKZ","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")",weight_+"*"+Zweight_+"*"+ZUnrollweight_,luminosity_*leg1Corr*zttScale_,prefix);
            printf("      EWKZ events in signal region = %f + %f \n",ewkYield.first,ewkYield.second);
            output.EWKZ  = ewkYield.first;
            output.dEWKZ = ewkYield.second;


            //Create ZL and ZJ

            std::cout<<"Create ZLFT"<<std::endl;
            //ZL Yield
            std::pair<float,float> zlftYield   = createHistogramAndShifts(zllFile_,"ZL","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")",weight_+"*"+Zweight_+"*"+zlftFactor_+"*"+ZUnrollweight_,luminosity_*leg1Corr*zttScale_,prefix);
            //ZLShape
            std::pair<float,float> zlftShapeUp   = createHistogramAndShifts(zllFile_,"ZL_CMS_htt_dyShape_13TeVUp","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")",weight_+"*"+Zweight_+"Up*"+zlftFactor_+"*"+ZUnrollweight_,luminosity_*leg1Corr*zttScale_,prefix);
            std::pair<float,float> zlftShapeDown   = createHistogramAndShifts(zllFile_,"ZL_CMS_htt_dyShape_13TeVDown","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")",weight_+"*"+Zweight_+"Down*"+zlftFactor_+"*"+ZUnrollweight_,luminosity_*leg1Corr*zttScale_,prefix);


            printf("      Z (l->tau) in signal region = %f + %f \n",zlftYield.first,zlftYield.second);

            output.ZLFT  = zlftYield.first;
            output.dZLFT  = zlftYield.second;

            std::cout<<"create histogram shifted"<<std::endl;
            if (prefix!="_0jet"){
                if(shifts_.size()>0&&channel_=="eleTau"){
                    std::pair<float,float> zlShiftUp      = createHistogramShifted(zllFile_,"ZL_CMS_htt_ZLShape_et_13TeVUp",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_+"*"+ZUnrollweight_),"TauUp",luminosity_*leg1Corr*zttScale_*zExtrap,prefix);
                    std::pair<float,float> zlShiftDown     = createHistogramShifted(zllFile_,"ZL_CMS_htt_ZLShape_et_13TeVDown",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_+"*"+ZUnrollweight_),"TauDown",luminosity_*leg1Corr*zttScale_*zExtrap,prefix);
                }
                else if(shifts_.size()>0&&channel_=="muTau"){
                    std::pair<float,float> zlShiftUp      = createHistogramShifted(zllFile_,"ZL_CMS_htt_ZLShape_mt_13TeVUp",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_+"*"+ZUnrollweight_),"TauUp",luminosity_*leg1Corr*zttScale_*zExtrap,prefix);
                    std::pair<float,float> zlShiftDown     = createHistogramShifted(zllFile_,"ZL_CMS_htt_ZLShape_mt_13TeVDown",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_+"*"+ZUnrollweight_),"TauDown",luminosity_*leg1Corr*zttScale_*zExtrap,prefix);
                }
            }
            else{ //tauDEcayMode unroll/
                if(shifts_.size()>0&&channel_=="eleTau"){
                    std::pair<float,float> zlShift0      = createHistogramShifted(zllFile_,"ZL_CMS_htt_ZLShape_et_0jet_1prong_13TeV",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_+"*(tauDecayMode==0)*"+ZUnrollweight_),"",luminosity_*leg1Corr*zttScale_*zExtrap,prefix);
                    std::pair<float,float> zlShiftUp0      = createHistogramShifted(zllFile_,"ZL_CMS_htt_ZLShape_et_0jet_1prong_13TeVUp",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_+"*(tauDecayMode==0)*"+ZUnrollweight_),"TauUp",luminosity_*leg1Corr*zttScale_*zExtrap,prefix);
                    std::pair<float,float> zlShiftDown0     = createHistogramShifted(zllFile_,"ZL_CMS_htt_ZLShape_et_0jet_1prong_13TeVDown",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_+"*(tauDecayMode==0)*"+ZUnrollweight_),"TauDown",luminosity_*leg1Corr*zttScale_*zExtrap,prefix);
                    std::pair<float,float> zlShift1      = createHistogramShifted(zllFile_,"ZL_CMS_htt_ZLShape_et_0jet_1prong1pizero_13TeV",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_+"*(tauDecayMode==1)*"+ZUnrollweight_),"",luminosity_*leg1Corr*zttScale_*zExtrap,prefix);
                    std::pair<float,float> zlShiftUp1      = createHistogramShifted(zllFile_,"ZL_CMS_htt_ZLShape_et_0jet_1prong1pizero_13TeVUp",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_+"*(tauDecayMode==1)*"+ZUnrollweight_),"TauUp",luminosity_*leg1Corr*zttScale_*zExtrap,prefix);
                    std::pair<float,float> zlShiftDown1     = createHistogramShifted(zllFile_,"ZL_CMS_htt_ZLShape_et_0jet_1prong1pizero_13TeVDown",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_+"*(tauDecayMode==1)*"+ZUnrollweight_),"TauDown",luminosity_*leg1Corr*zttScale_*zExtrap,prefix);
                    subtractHistogram(filelabel_+prefix,"ZL_CMS_htt_ZLShape_et_0jet_1prong_13TeVUp","ZL_CMS_htt_ZLShape_et_0jet_1prong_13TeV"); 
                    subtractHistogram(filelabel_+prefix,"ZL_CMS_htt_ZLShape_et_0jet_1prong_13TeVDown","ZL_CMS_htt_ZLShape_et_0jet_1prong_13TeV",false); 
                    subtractHistogram(filelabel_+prefix,"ZL_CMS_htt_ZLShape_et_0jet_1prong1pizero_13TeVUp","ZL_CMS_htt_ZLShape_et_0jet_1prong1pizero_13TeV"); 
                    subtractHistogram(filelabel_+prefix,"ZL_CMS_htt_ZLShape_et_0jet_1prong1pizero_13TeVDown","ZL_CMS_htt_ZLShape_et_0jet_1prong1pizero_13TeV",false); 
                    addHistogram(filelabel_+prefix,"ZL_CMS_htt_ZLShape_et_0jet_1prong_13TeVUp","ZL"); 
                    addHistogram(filelabel_+prefix,"ZL_CMS_htt_ZLShape_et_0jet_1prong_13TeVDown","ZL"); 
                    addHistogram(filelabel_+prefix,"ZL_CMS_htt_ZLShape_et_0jet_1prong1pizero_13TeVUp","ZL"); 
                    addHistogram(filelabel_+prefix,"ZL_CMS_htt_ZLShape_et_0jet_1prong1pizero_13TeVDown","ZL"); 
                }
                else if(shifts_.size()>0&&channel_=="muTau"){
                    std::pair<float,float> zlShift0      = createHistogramShifted(zllFile_,"ZL_CMS_htt_ZLShape_mt_0jet_1prong_13TeV",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_+"*(tauDecayMode==0)*"+ZUnrollweight_),"",luminosity_*leg1Corr*zttScale_*zExtrap,prefix);
                    std::pair<float,float> zlShiftUp0      = createHistogramShifted(zllFile_,"ZL_CMS_htt_ZLShape_mt_0jet_1prong_13TeVUp",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_+"*(tauDecayMode==0)*"+ZUnrollweight_),"TauUp",luminosity_*leg1Corr*zttScale_*zExtrap,prefix);
                    std::pair<float,float> zlShiftDown0     = createHistogramShifted(zllFile_,"ZL_CMS_htt_ZLShape_mt_0jet_1prong_13TeVDown",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_+"*(tauDecayMode==0)*"+ZUnrollweight_),"TauDown",luminosity_*leg1Corr*zttScale_*zExtrap,prefix);
                    std::pair<float,float> zlShift1      = createHistogramShifted(zllFile_,"ZL_CMS_htt_ZLShape_mt_0jet_1prong1pizero_13TeV",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_+"*(tauDecayMode==1)*"+ZUnrollweight_),"",luminosity_*leg1Corr*zttScale_*zExtrap,prefix);
                    std::pair<float,float> zlShiftUp1      = createHistogramShifted(zllFile_,"ZL_CMS_htt_ZLShape_mt_0jet_1prong1pizero_13TeVUp",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_+"*(tauDecayMode==1)*"+ZUnrollweight_),"TauUp",luminosity_*leg1Corr*zttScale_*zExtrap,prefix);
                    std::pair<float,float> zlShiftDown1     = createHistogramShifted(zllFile_,"ZL_CMS_htt_ZLShape_mt_0jet_1prong1pizero_13TeVDown",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_+"*(tauDecayMode==1)*"+ZUnrollweight_),"TauDown",luminosity_*leg1Corr*zttScale_*zExtrap,prefix);
                    subtractHistogram(filelabel_+prefix,"ZL_CMS_htt_ZLShape_mt_0jet_1prong_13TeVUp","ZL_CMS_htt_ZLShape_mt_0jet_1prong_13TeV"); 
                    subtractHistogram(filelabel_+prefix,"ZL_CMS_htt_ZLShape_mt_0jet_1prong_13TeVDown","ZL_CMS_htt_ZLShape_mt_0jet_1prong_13TeV"); 
                    subtractHistogram(filelabel_+prefix,"ZL_CMS_htt_ZLShape_mt_0jet_1prong1pizero_13TeVUp","ZL_CMS_htt_ZLShape_mt_0jet_1prong1pizero_13TeV"); 
                    subtractHistogram(filelabel_+prefix,"ZL_CMS_htt_ZLShape_mt_0jet_1prong1pizero_13TeVDown","ZL_CMS_htt_ZLShape_mt_0jet_1prong1pizero_13TeV",false); 
                    addHistogram(filelabel_+prefix,"ZL_CMS_htt_ZLShape_mt_0jet_1prong_13TeVUp","ZL"); 
                    addHistogram(filelabel_+prefix,"ZL_CMS_htt_ZLShape_mt_0jet_1prong_13TeVDown","ZL"); 
                    addHistogram(filelabel_+prefix,"ZL_CMS_htt_ZLShape_mt_0jet_1prong1pizero_13TeVUp","ZL"); 
                    addHistogram(filelabel_+prefix,"ZL_CMS_htt_ZLShape_mt_0jet_1prong1pizero_13TeVDown","ZL"); 
                }
            }



            if (prefix=="_vbf"){
                std::pair<float,float> zlftZMShapeUp   = createHistogramAndShifts(zllFile_,"ZL_CMS_htt_zmumuShape_VBF_13TeVUp","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")",weight_+"*"+Zweight_+"*"+ZUnrollweight_+"*"+ZUnrollweight_,luminosity_*leg1Corr*zttScale_,prefix);
                std::pair<float,float> zlftZMShapeDown   = createHistogramAndShifts(zllFile_,"ZL_CMS_htt_zmumuShape_VBF_13TeVDown","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")",weight_+"*"+Zweight_+"*"+zlftFactor_,luminosity_*leg1Corr*zttScale_,prefix);
            }
            else if (prefix=="_0jet"){
                std::pair<float,float> zlftZMShape0Up   = createHistogramAndShifts(zllFile_,"ZL_CMS_tauDMReco_1prong_13TeVUp","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")",weight_+"*"+Zweight_+"*(1+((tauDecayMode==0)*0.03))",luminosity_*leg1Corr*zttScale_,prefix);
                std::pair<float,float> zlftZMShape1Up   = createHistogramAndShifts(zllFile_,"ZL_CMS_tauDMReco_1prong1pizero_13TeVUp","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")",weight_+"*"+Zweight_+"*(1+((tauDecayMode==1)*0.03))",luminosity_*leg1Corr*zttScale_,prefix);
                std::pair<float,float> zlftZMShape10Up   = createHistogramAndShifts(zllFile_,"ZL_CMS_tauDMReco_3prong_13TeVUp","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")",weight_+"*"+Zweight_+"*(1+((tauDecayMode==10)*0.03))",luminosity_*leg1Corr*zttScale_,prefix);

                std::pair<float,float> zlftZMShape0Down   = createHistogramAndShifts(zllFile_,"ZL_CMS_tauDMReco_1prong_13TeVDown","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")",weight_+"*"+Zweight_+"*(1-((tauDecayMode==0)*0.03))",luminosity_*leg1Corr*zttScale_,prefix);
                std::pair<float,float> zlftZMShape1Down   = createHistogramAndShifts(zllFile_,"ZL_CMS_tauDMReco_1prong1pizero_13TeVDown","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")",weight_+"*"+Zweight_+"*(1-((tauDecayMode==1)*0.03))",luminosity_*leg1Corr*zttScale_,prefix);
                std::pair<float,float> zlftZMShape10Down   = createHistogramAndShifts(zllFile_,"ZL_CMS_tauDMReco_3prong_13TeVDown","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")",weight_+"*"+Zweight_+"*(1-((tauDecayMode==10)*0.03))",luminosity_*leg1Corr*zttScale_,prefix);

                renormalizeHistogram(filelabel_+prefix,"ZL_CMS_tauDMReco_1prong_13TeVUp",zlftYield.first);
                renormalizeHistogram(filelabel_+prefix,"ZL_CMS_tauDMReco_1prong1pizero_13TeVUp",zlftYield.first);
                renormalizeHistogram(filelabel_+prefix,"ZL_CMS_tauDMReco_3prong_13TeVUp",zlftYield.first);
                renormalizeHistogram(filelabel_+prefix,"ZL_CMS_tauDMReco_1prong_13TeVDown",zlftYield.first);
                renormalizeHistogram(filelabel_+prefix,"ZL_CMS_tauDMReco_1prong1pizero_13TeVDown",zlftYield.first);
                renormalizeHistogram(filelabel_+prefix,"ZL_CMS_tauDMReco_3prong_13TeVDown",zlftYield.first);

            }




            std::cout<<"Create ZJFT"<<std::endl;
            //ZJ Yield
            std::pair<float,float> zjftYield      = createHistogramAndShifts(zllFile_,"ZJ","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZJFT_genLReject_+")",weight_+"*"+Zweight_+"*"+ZUnrollweight_,luminosity_*leg1Corr*zttScale_,prefix);    

            std::cout<<"Create ZJFT 2"<<std::endl;
            std::pair<float,float> zjftShapeUp      = createHistogramAndShifts(zllFile_,"ZJ_CMS_htt_dyShape_13TeVUp","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZJFT_genLReject_+")",weight_+"*"+Zweight_+"Up*"+ZUnrollweight_,luminosity_*leg1Corr*zttScale_,prefix);    
            std::pair<float,float> zjftShapeDown      = createHistogramAndShifts(zllFile_,"ZJ_CMS_htt_dyShape_13TeVDown","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZJFT_genLReject_+")",weight_+"*"+Zweight_+"Down*"+ZUnrollweight_,luminosity_*leg1Corr*zttScale_,prefix);    

            std::cout<<"Create ZJFT 3"<<std::endl;
            std::pair<float,float> zjftFTShapeUp      = createHistogramAndShifts(zllFile_,"ZJ_CMS_htt_jetToTauFake_13TeVUp","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZJFT_genLReject_+")",weight_+"*"+Zweight_+"*fakeTauEffiUp"+"*"+ZUnrollweight_,luminosity_*leg1Corr*zttScale_,prefix);    
            std::pair<float,float> zjftFTShapeDown      = createHistogramAndShifts(zllFile_,"ZJ_CMS_htt_jetToTauFake_13TeVDown","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZJFT_genLReject_+")",weight_+"*"+Zweight_+"*fakeTauEffiDown"+"*"+ZUnrollweight_,luminosity_*leg1Corr*zttScale_,prefix);    

            //
            std::cout<<"Create ZJFT 4"<<std::endl;
            if (prefix=="_vbf"){
                std::pair<float,float> zjftZMShapeUp   = createHistogramAndShifts(zttFile_,"ZJ_CMS_htt_zmumuShape_VBF_13TeVUp","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZJFT_genLReject_+")",weight_+"*"+Zweight_+"*"+ZUnrollweight_+"*"+ZUnrollweight_,luminosity_*leg1Corr*zttScale_,prefix);
                std::pair<float,float> zjftZMShapeDown   = createHistogramAndShifts(zttFile_,"ZJ_CMS_htt_zmumuShape_VBF_13TeVDown","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZJFT_genLReject_+")",weight_+"*"+Zweight_,luminosity_*leg1Corr*zttScale_,prefix);
            }

            printf("      Z (j->tau) in signal region = %f + %f \n",zjftYield.first,zjftYield.second);

            output.ZJFT  = zjftYield.first;
            output.dZJFT = zjftYield.second;

            std::cout<<"Create ZJFT 5"<<std::endl;
            std::cout<<"Create ZTT"<<std::endl;
            //TODO: Check me, previous Btag ZTT shape correction had a special normalization method
            if(!runZTT(preSelection, prefix, zShape, topExtrap, output, categorySelection)){
                std::cout<<"Error Creating Ztt"<<std::endl;
                return output;
            }

            std::cout<<"Create TOP"<<std::endl;
            //Create ttbar
            //Last argument is a dummy argument
            if(!runTOP(preSelection, prefix, zShape, topExtrap, output, categorySelection, relaxedSelection)){
                std::cout<<"Error Creating TOP"<<std::endl;
                return output;
            }



            std::cout<<"Create W"<<std::endl;
            //Create W
            if(!runW(preSelection, prefix, zShape, topExtrap, output, categorySelection, relaxedSelection, wSel,bTagSF)){
                std::cout<<"Error Creating W"<<std::endl;
                return output;
            }


            std::cout<<"Create QCD"<<std::endl;
            //Create QCD
            if(!runQCD(preSelection, prefix, zShape, topExtrap, output, categorySelection, relaxedSelection)){
                std::cout<<"Error Creating QCD"<<std::endl;
                return output;
            }



            FixNegativeBinsMC(filelabel_+prefix,"QCD",false);
            //FixNegativeBinsMC(filelabel_+prefix,"W",false);
            //FixNegativeBinsMC(filelabel_+prefix,"TOP",false);
            FixNegativeBinsMC(filelabel_+prefix,"VV",false);
            //FixNegativeBinsMC(filelabel_+prefix,"ZL",false);
            //FixNegativeBinsMC(channel_+prefix,"ZJ",false);
            //FixNegativeBinsMC(channel_+prefix,"ZTT",false);
            //FixNegativeBinsMC(channel_+prefix,"EWKZ",false);




            std::cout<<std::endl;
            std::cout<<"=============Data Yields============="<<std::endl;
            std::cout<<"DATA: "<< output.DATA<<std::endl;
            std::cout<<std::endl;


            std::cout<<std::endl;
            std::cout<<"BKGD Yields "<<std::endl;
            std::cout<<"QCD: "<< output.QCD<<std::endl;
            std::cout<<"W: "<< output.W<<std::endl;
            std::cout<<"TOP: "<< output.TOP<<std::endl;
            std::cout<<"VV: "<< output.VV<<std::endl;
            std::cout<<"EWKZ: "<< output.EWKZ<<std::endl;
            std::cout<<"ZLFT: "<< output.ZLFT<<std::endl;
            std::cout<<"ZJFT: "<< output.ZJFT<<std::endl;
            std::cout<<"ZTT: "<< output.ZTT<<std::endl;


            float background    = output.QCD  + output.W  + output.TOP  + output.VV  + output.ZLFT  + output.ZJFT  + output.ZTT;
            float backgroundErr = sqrt( pow(output.dQCD,2) + pow(output.dW,2) + pow(output.dTOP,2) + pow(output.dVV,2) + pow(output.dZLFT,2) + pow(output.dZJFT,2) + pow(output.dZTT,2));
            printf("BACKGROUND=%f +-%f \n",background,backgroundErr);

            ///LATEX->Here since we want it for the note add all errors , even those that will go separate in the datacard

            //printf("LATEX ------------------------------------\n");
            //printf("Total & %.2f & %.2f & %.2f & %.2f \\\\ \n", dataYield.first, dataYieldSdb.first, dataSSYield.first, dataSSYieldSdb.first);
            //printf("Di-Boson & %.2f $\\pm$ %.2f & %.2f $\\pm$ %.2f & - & - \\\\ \n", vvYield.first, quadrature(vvYield.first,vvYield.second,muIDErr_,eleIDErr_,zttScaleErr_,tauIDErr_), vvYieldSdb.first, quadrature(vvYieldSdb.first,vvYieldSdb.second,muIDErr_,eleIDErr_,tauIDErr_));
            //printf("$t\\bar{t}$ & %.2f $\\pm$ %.2f & %.2f $\\pm$ %.2f & - & - \\\\ \n", topYield.first,quadrature(topYield.first,topYield.second,muIDErr_,eleIDErr_,tauIDErr_), topYieldSdb.first, quadrature(topYieldSdb.first,topYield.second,muIDErr_,eleIDErr_,tauIDErr_));
            //printf("$Z^{l+jet}$ & %.2f $\\pm$ %.2f & - & %.2f $\\pm$ %.2f & - \\\\ \n", zjftYield.first, quadrature(zjftYield.first,zjftYield.second,muIDErr_,eleIDErr_,zttScaleErr_), zjftInflSSYield.first,quadrature(zjftInflSSYield.first,zjftInflSSYield.second,muIDErr_,eleIDErr_,zttScaleErr_));
            //printf("$Z^{ll}$ & %.2f $\\pm$ %.2f & - & %.2f $\\pm$ %.2f & - \\\\ \n", zlftYield.first, quadrature(zlftYield.first,zlftYield.second,muIDErr_,eleIDErr_,zttScaleErr_),zlftInflSSYield.first,quadrature(zlftInflSSYield.first,zlftInflSSYield.second,muIDErr_,eleIDErr_,zttScaleErr_));
            //printf("$W+jets$ & %.2f $\\pm$ %.2f & %.2f $\\pm$ %.2f & %.2f $\\pm$ %.2f & %.2f $\\pm$ %.2f  \\\\ \n", osWLow.first, osWLow.second, osWHigh.first, osWHigh.second, ssWLow.first, ssWLow.second, dataSSYieldSdb.first, dataSSYieldSdb.second);
            //printf("QCD & %.2f $\\pm$ %.2f & - & %.2f $\\pm$ %.2f & - \\\\ \n", osQCD.first, osQCD.second, ssQCD.first, ssQCD.second);
            //printf("$Z\\rightarrow\\tau\\tau$ & %.2f $\\pm$ %.2f & - & - & - \\\\ \n", zttYield.first,quadrature(zttYield.first,zttYield.second,muIDErr_,eleIDErr_,zttScaleErr_,tauIDErr_));


            float fullBackgroundErr = sqrt(pow(quadrature(output.VV,output.dVV,muIDErr_,eleIDErr_,zttScaleErr_,tauIDErr_),2)
                    +pow(quadrature(output.TOP,output.dTOP,muIDErr_,eleIDErr_,tauIDErr_),2)
                    +pow(quadrature(output.ZJFT,output.dZJFT,muIDErr_,eleIDErr_,zttScaleErr_),2)
                    +pow(quadrature(output.ZLFT,output.dZLFT,muIDErr_,eleIDErr_,zttScaleErr_),2)
                    +pow(output.dQCD,2)
                    +pow(output.dW,2)
                    +pow(quadrature(output.ZTT,output.dZTT,muIDErr_,eleIDErr_,zttScaleErr_,tauIDErr_),2));

            printf("Total Background & %.2f $\\pm$ %.2f & - & - & - \\\\ \n",background,sqrt(pow(quadrature(output.VV,    output.dVV,   muIDErr_,eleIDErr_,zttScaleErr_,tauIDErr_),2)
                        +pow(quadrature(output.TOP,  output.dTOP,  muIDErr_,eleIDErr_,tauIDErr_),2)
                        +pow(quadrature(output.ZJFT, output.dZJFT, muIDErr_,eleIDErr_,zttScaleErr_),2)
                        +pow(quadrature(output.ZLFT, output.dZLFT, muIDErr_,eleIDErr_,zttScaleErr_),2)
                        +pow(output.dQCD,2)
                        +pow(output.dW,2)
                        +pow(quadrature(output.ZTT,output.dZTT,muIDErr_,eleIDErr_,zttScaleErr_,tauIDErr_),2)));



            //create a histogram with the error for plotting reasons and only
            TH1F *err = new TH1F("BKGErr","",1,0,1);
            err->SetBinContent(1,fullBackgroundErr/background);
            fout_->cd((filelabel_+prefix).c_str());
            err->Write();

            return output;

        }


        /*
         * For Inclusive take the Ztt normalization directly from MC (zttFile_)
         * For a category the Ztt normalization is as follows,
         * ZttCategoryYield = ZttMCInclusiveYield * (ZttEmbedded_Category) / *Ztt(
         */

        bool runZTT(std::string preSelection,std::string prefix,std::string zShape, float topExtrap, BkgOutput &output,
                std::string categorySelection = "pt_1>-100", std::string relaxedSelection = "pt_1>-100"){

            float leg1Corr=1.0;
            if(muID_!=0) leg1Corr*=muID_;
            if(eleID_!=0) leg1Corr*=eleID_;


            std::pair<float,float> ztt  = createHistogramAndShifts(zttFile_,"ZTT","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZTT_genTauSel_+")",weight_+"*"+Zweight_+"*"+ZUnrollweight_,luminosity_*zttScale_*leg1Corr*tauID_,prefix);
            std::pair<float,float> zttDyUp  = createHistogramAndShifts(zttFile_,"ZTT_CMS_htt_dyShape_13TeVUp","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZTT_genTauSel_+")",weight_+"*"+Zweight_+"Up*"+ZUnrollweight_,luminosity_*zttScale_*leg1Corr*tauID_,prefix);
            std::pair<float,float> zttDyDown  = createHistogramAndShifts(zttFile_,"ZTT_CMS_htt_dyShape_13TeVDown","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZTT_genTauSel_+")",weight_+"*"+Zweight_+"Down*"+ZUnrollweight_,luminosity_*zttScale_*leg1Corr*tauID_,prefix);

            if (prefix=="_vbf"){
                std::pair<float,float> zttZMShapeUp   = createHistogramAndShifts(zttFile_,"ZTT_CMS_htt_zmumuShape_VBF_13TeVUp","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZTT_genTauSel_+")",weight_+"*"+Zweight_+"*"+ZUnrollweight_+"*"+ZUnrollweight_,luminosity_*leg1Corr*zttScale_,prefix);
                std::pair<float,float> zttZMShapeDown   = createHistogramAndShifts(zttFile_,"ZTT_CMS_htt_zmumuShape_VBF_13TeVDown","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZTT_genTauSel_+")",weight_+"*"+Zweight_,luminosity_*leg1Corr*zttScale_,prefix);
            }

            std::cout<<"      ZTT Selection: "<<preSelection<<"&&"<<trigSelection_<<"&&"<<osSignalSelection_<<"&&"<<categorySelection<<std::endl;
            output.ZTT  = ztt.first;
            output.dZTT = ztt.second;

            printf("      Selection ZTT events in signal region = %f + %f \n",ztt.first,ztt.second);
            return true;
        }


        bool runTOP( std::string preSelection, std::string prefix, std::string zShape, float topExtrap, BkgOutput &output, 
                std::string categorySelection, std::string relaxedSelection ) {
            std::string genTauSel_="gen_match_2==5";
            std::string genTauRej_="gen_match_2!=5";

            float leg1Corr=1.0;
            if(muID_!=0) leg1Corr*=muID_;
            if(eleID_!=0) leg1Corr*=eleID_;

            std::pair<float,float> topYield      = createHistogramAndShifts(topFile_,"TT","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")",weight_+"*"+TTweight_, luminosity_*leg1Corr*topExtrap, prefix);

            std::pair<float,float> topJShape      = createHistogramAndShifts(topFile_,"TTJ","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+genTauRej_+")",weight_+"*"+TTweight_, luminosity_*leg1Corr*topExtrap, prefix);
            std::pair<float,float> topJShapeUp      = createHistogramAndShifts(topFile_,"TTJ_CMS_htt_ttbarShape_13TeVUp","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+genTauRej_+")",weight_+"*"+TTweight_+"Up", luminosity_*leg1Corr*topExtrap, prefix);
            std::pair<float,float> topJShapeDown      = createHistogramAndShifts(topFile_,"TTJ_CMS_htt_ttbarShape_13TeVDown","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+genTauRej_+")",weight_, luminosity_*leg1Corr*topExtrap, prefix);

            std::pair<float,float> topJFTShapeUp      = createHistogramAndShifts(topFile_,"TTJ_CMS_htt_jetToTauFake_13TeVUp","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+genTauRej_+")",weight_+"*"+TTweight_+"*fakeTauEffiUp", luminosity_*leg1Corr*topExtrap, prefix);
            std::pair<float,float> topJFTShapeDown      = createHistogramAndShifts(topFile_,"TTJ_CMS_htt_jetToTauFake_13TeVDown","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+genTauRej_+")",weight_+"*"+TTweight_+"*fakeTauEffiDown", luminosity_*leg1Corr*topExtrap, prefix);

            //renormalizeHistogram(filelabel_+prefix,"TTJ_CMS_htt_jetToTauFake_13TeVUp",topJShape.first);
            //renormalizeHistogram(filelabel_+prefix,"TTJ_CMS_htt_jetToTauFake_13TeVDown",topJShape.first);


            std::pair<float,float> topTShape      = createHistogramAndShifts(topFile_,"TTT","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+genTauSel_+")",weight_+"*"+TTweight_, luminosity_*leg1Corr*tauID_*topExtrap, prefix);
            std::pair<float,float> topTShapeUp      = createHistogramAndShifts(topFile_,"TTT_CMS_htt_ttbarShape_13TeVUp","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+genTauSel_+")",weight_+"*"+TTweight_+"Up", luminosity_*leg1Corr*tauID_*topExtrap, prefix);
            std::pair<float,float> topTShapeDown      = createHistogramAndShifts(topFile_,"TTT_CMS_htt_ttbarShape_13TeVDown","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+genTauSel_+")",weight_, luminosity_*leg1Corr*tauID_*topExtrap, prefix);


            output.TOP  = topYield.first;
            output.dTOP = topYield.second;

            printf("      TTbar events in signal region = %f + %f \n",topYield.first,topYield.second);

            return true;}




        bool runQCD(std::string preSelection, std::string prefix, std::string zShape, float topExtrap, BkgOutput &output, std::string categorySelection, std::string relaxedSelection) 
        {
            float leg1Corr=1.0;
            if(muID_!=0) leg1Corr*=muID_;
            if(eleID_!=0) leg1Corr*=eleID_;
            std::string chan = "mt";
            if(channel_ == "eleTau")
                chan = "et";

            std::pair<float,float> dataQCDShape = createHistogramAndShiftsQCD(dataFile_,"QCD","("+relaxedSelection+"&&"+trigSelectionData_+"&&"+ssSignalSelection_+"&&"+categorySelection+")",scaleUp_,prefix); 
            std::pair<float,float> dataQCDShapeUp = createHistogramAndShifts(dataFile_,"QCD_WSFUncert_"+chan+prefix+"_13TeVUp","("+relaxedSelection+"&&"+trigSelectionData_+"&&"+ssSignalSelection_+"&&"+categorySelection+")",scaleUp_,prefix); 
            std::pair<float,float> dataQCDShapeDown = createHistogramAndShifts(dataFile_,"QCD_WSFUncert_"+chan+prefix+"_13TeVDown","("+relaxedSelection+"&&"+trigSelectionData_+"&&"+ssSignalSelection_+"&&"+categorySelection+")",scaleUp_,prefix); 

            printf("      Data events in SS Signal QCDShape sideband region = %f + %f \n",dataQCDShape.first,dataQCDShape.second);
            std::pair<float,float> ZQCDShape = createHistogramAndShifts(zttFile_,"ZQCD","("+relaxedSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")",weight_+"*"+Zweight_+"*"+ZUnrollweight_,luminosity_*zttScale_,prefix); 
            printf("      Z events in SS Signal QCDShape sideband region = %f + %f \n",ZQCDShape.first,ZQCDShape.second);
            std::pair<float,float> TopQCDShape = createHistogramAndShifts(topFile_,"TopQCD","("+relaxedSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")",weight_+"*"+TTweight_,luminosity_,prefix); 
            printf("      TOP events in SS Signal QCDShape sideband region = %f + %f \n",TopQCDShape.first,TopQCDShape.second);
            std::pair<float,float> VVQCDShape       = createHistogramAndShifts(vvFile_,"VVQCD","("+relaxedSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")",weight_,luminosity_,prefix);
            printf("      VV events in SS Signal QCDShape sideband region = %f + %f \n",VVQCDShape.first,VVQCDShape.second);
            std::pair<float,float> WQCDLowShape       = createHistogramAndShifts(wFile_,"WQCD","("+relaxedSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")",weight_,luminosity_*leg1Corr*wFactorQCD_,prefix);
            std::pair<float,float> WQCDLowShapeUp       = createHistogramAndShifts(wFile_,"WQCDUp","("+relaxedSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")",weight_,luminosity_*leg1Corr*(wFactorQCD_+wFactorErr_),prefix);
            std::pair<float,float> WQCDLowShapeDown       = createHistogramAndShifts(wFile_,"WQCDDown","("+relaxedSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")",weight_,luminosity_*leg1Corr*(wFactorQCD_-wFactorErr_),prefix);
            //CHECKME add scaling to W
            printf("      W events in SS Signal QCDShape sideband region = %f*%f =%f + %f \n",WQCDLowShape.first/wFactorQCD_,wFactorQCD_,WQCDLowShape.first,WQCDLowShape.second);


            std::cout<<"QCD Selections= "<<"("<<relaxedSelection<<"&&"<<trigSelectionData_<<"&&"<<ssSignalSelection_<<")"<<std::endl;
            std::pair<float,float> dataQCD = createHistogramAndShiftsQCD(dataFile_,"QCDYield","("+preSelection+"&&"+trigSelectionData_+"&&"+ssSignalSelection_+"&&"+categorySelection+")",scaleUp_,prefix); 
            std::pair<float,float> dataQCDUp = createHistogramAndShiftsQCD(dataFile_,"QCDYieldUp","("+preSelection+"&&"+trigSelectionData_+"&&"+ssSignalSelection_+"&&"+categorySelection+")",scaleUp_,prefix); 
            std::pair<float,float> dataQCDDown = createHistogramAndShiftsQCD(dataFile_,"QCDYieldDown","("+preSelection+"&&"+trigSelectionData_+"&&"+ssSignalSelection_+"&&"+categorySelection+")",scaleUp_,prefix); 

            printf("      Data events in SS Signal QCD sideband region = %f + %f \n",dataQCD.first,dataQCD.second);
            std::pair<float,float> ZQCD = createHistogramAndShifts(zttFile_,"ZQCDYield","("+preSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")",weight_+"*"+Zweight_+"*"+ZUnrollweight_,luminosity_*zttScale_,prefix); 
            printf("      Z events in SS Signal QCD sideband region = %f + %f \n",ZQCD.first,ZQCD.second);
            std::pair<float,float> TopQCD = createHistogramAndShifts(topFile_,"TopQCDYield","("+preSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")",weight_+"*"+TTweight_,luminosity_,prefix); 
            printf("      TOP events in SS Signal QCD sideband region = %f + %f \n",TopQCD.first,TopQCD.second);
            std::pair<float,float> VVQCD       = createHistogramAndShifts(vvFile_,"VVQCDYield","("+preSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")",weight_,luminosity_,prefix);
            printf("      VV events in SS Signal QCD sideband region = %f + %f \n",VVQCD.first,VVQCD.second);

            //CHECK
            std::pair<float,float> ssWLow       = createHistogramAndShifts(wFile_,"WQCDYield","("+preSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")",weight_,luminosity_*leg1Corr*wFactorQCD_,prefix);
            std::pair<float,float> ssWLowUp       = createHistogramAndShifts(wFile_,"WQCDYieldUp","("+preSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")",weight_,luminosity_*leg1Corr*(wFactorQCD_+wFactorErr_),prefix);
            std::pair<float,float> ssWLowDown       = createHistogramAndShifts(wFile_,"WQCDYieldDown","("+preSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")",weight_,luminosity_*leg1Corr*(wFactorQCD_-wFactorErr_),prefix);
            printf("      (MonteCarlo) W events in SS region = %f + %f \n",ssWLow.first,ssWLow.second);


            //if (ssWLow.first<0.1) wFactor_=std::make_pair(1.0,1.0);
            /*
             * DATADRIVEN W FOUND 
             */


            subtractHistogramShifts(filelabel_+prefix,"QCD","ZQCD");
            subtractHistogramShifts(filelabel_+prefix,"QCD","VVQCD");
            subtractHistogramShifts(filelabel_+prefix,"QCD","TopQCD");
            subtractHistogramShifts(filelabel_+prefix,"QCD","WQCD");


            //Now subtracting off bkgd shapes from data ss shape
            //iWSF

            std::cout<<"QCD_WSFUncert_"<<chan<<prefix<<"_13TeVUp"<<std::endl;
            subtractHistogram(filelabel_+prefix,"QCD_WSFUncert_"+chan+prefix+"_13TeVUp","ZQCD");
            subtractHistogram(filelabel_+prefix,"QCD_WSFUncert_"+chan+prefix+"_13TeVUp","VVQCD");
            subtractHistogram(filelabel_+prefix,"QCD_WSFUncert_"+chan+prefix+"_13TeVUp","TopQCD");
            subtractHistogram(filelabel_+prefix,"QCD_WSFUncert_"+chan+prefix+"_13TeVUp","WQCDUp");
            subtractHistogram(filelabel_+prefix,"QCD_WSFUncert_"+chan+prefix+"_13TeVDown","ZQCD");
            subtractHistogram(filelabel_+prefix,"QCD_WSFUncert_"+chan+prefix+"_13TeVDown","VVQCD");
            subtractHistogram(filelabel_+prefix,"QCD_WSFUncert_"+chan+prefix+"_13TeVDown","TopQCD");
            subtractHistogram(filelabel_+prefix,"QCD_WSFUncert_"+chan+prefix+"_13TeVDown","WQCDDown");


            std::pair<float,float> ssQCD = std::make_pair(TMath::Nint(dataQCD.first
                        -ssWLow.first
                        -TopQCD.first
                        -VVQCD.first
                        -ZQCD.first),
                    sqrt(dataQCD.second*dataQCD.second
                        +ssWLow.second*ssWLow.second
                        +TopQCD.second*TopQCD.second
                        +VVQCD.second*VVQCD.second
                        +ZQCD.second*ZQCD.second));

            std::pair<float,float> ssQCDUp = std::make_pair(TMath::Nint(dataQCD.first
                        -ssWLowUp.first
                        -TopQCD.first
                        -VVQCD.first
                        -ZQCD.first),
                    sqrt(dataQCD.second*dataQCD.second
                        +ssWLowUp.second*ssWLowUp.second
                        +TopQCD.second*TopQCD.second
                        +VVQCD.second*VVQCD.second
                        +ZQCD.second*ZQCD.second));
            std::pair<float,float> ssQCDDown = std::make_pair(TMath::Nint(dataQCD.first
                        -ssWLowDown.first
                        -TopQCD.first
                        -VVQCD.first
                        -ZQCD.first),
                    sqrt(dataQCD.second*dataQCD.second
                        +ssWLowDown.second*ssWLowDown.second
                        +TopQCD.second*TopQCD.second
                        +VVQCD.second*VVQCD.second
                        +ZQCD.second*ZQCD.second));


            if(ssQCD.first<0) {
                ssQCD.first=0.0000001;
                ssQCD.second=1.2;
                ssQCDUp.first=0.0000001;
                ssQCDUp.second=1.2;
                ssQCDDown.first=0.0000001;
                ssQCDDown.second=1.2;
            }

            printf("SS QCD in  core  =%f -%f -%f -%f -%f= %f +- %f \n",
                    dataQCD.first,
                    ssWLow.first,
                    TopQCD.first,
                    ZQCD.first,
                    VVQCD.first,
                    ssQCD.first,
                    ssQCD.second);


            printf("5. Extrapolate SS QCD -> OS QCD \n");
            std::pair<float,float> osQCD = std::make_pair(ssQCD.first*qcdFactor_,
                    sqrt(ssQCD.second*ssQCD.second*qcdFactor_*qcdFactor_
                        +qcdFactorErr_*qcdFactorErr_*ssQCD.first*ssQCD.first));
            std::pair<float,float> osQCDUp = std::make_pair(ssQCDUp.first*qcdFactor_,
                    sqrt(ssQCDUp.second*ssQCDUp.second*qcdFactor_*qcdFactor_
                        +qcdFactorErr_*qcdFactorErr_*ssQCDUp.first*ssQCDUp.first));
            std::pair<float,float> osQCDDown = std::make_pair(ssQCDDown.first*qcdFactor_,
                    sqrt(ssQCDDown.second*ssQCDDown.second*qcdFactor_*qcdFactor_
                        +qcdFactorErr_*qcdFactorErr_*ssQCDDown.first*ssQCDDown.first));



            //Now Renormalize
            renormalizeHistogram(filelabel_+prefix,"QCD",osQCD.first);
            //renormalizeHistogram(filelabel_+prefix,"QCD_WSFUncert_"+chan+prefix+"_13TeVUp",osQCDUp.first);
            //renormalizeHistogram(filelabel_+prefix,"QCD_WSFUncert_"+chan+prefix+"_13TeVDown",osQCDDown.first);
            renormalizeHistogram(filelabel_+prefix,"QCD_WSFUncert_"+chan+prefix+"_13TeVUp",osQCD.first);
            renormalizeHistogram(filelabel_+prefix,"QCD_WSFUncert_"+chan+prefix+"_13TeVDown",osQCD.first);



            printf("OS QCD in  core  =%f *%f = %f +- %f \n",ssQCD.first,qcdFactor_,osQCD.first,osQCD.second);

            output.QCD  = osQCD.first;    
            output.dQCD = osQCD.second;

            if(shifts_.size()>0){
                qcdSyst(filelabel_, prefix, "QCD", 1.0, qcdFactorErr_);
            }

            return true;


        }

        bool runW(std::string preSelection,std::string prefix,std::string zShape, float topExtrap, BkgOutput &output, std::string categorySelection, std::string relaxedSelection, std::string wSel, std::string btagsf="1.0") {
            std::string chan = "mt";
            if(channel_ == "eleTau")
                chan = "et";


            float leg1Corr=1.0;
            if(muID_!=0) leg1Corr*=muID_;
            if(eleID_!=0) leg1Corr*=eleID_;

            //temp yield
            //MC yield
            std::pair<float,float> wYield         = createHistogramAndShifts(wFile_,"WMC","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")",weight_,luminosity_*leg1Corr,prefix);
            //MC relaxed shape
            std::pair<float,float> wShape         = createHistogramAndShifts(wFile_,"W","("+relaxedSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")",weight_,luminosity_*leg1Corr,prefix);

            std::pair<float,float> wShapeFTUp        = createHistogramAndShifts(wFile_,"W_CMS_htt_jetToTauFake_13TeVUp","("+relaxedSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")",weight_+"*fakeTauEffiUp",luminosity_*leg1Corr,prefix);
            std::pair<float,float> wShapeFTDown        = createHistogramAndShifts(wFile_,"W_CMS_htt_jetToTauFake_13TeVDown","("+relaxedSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")",weight_+"*fakeTauEffiDown",luminosity_*leg1Corr,prefix);
            /*
             * SUBTRACT MC IN HIGH OS SDB
             */
            //First get data in Sideband
            //old
            std::pair<float,float> dataYSdb     = createHistogramAndShifts(dataFile_,"data_obs_sdb","("+preSelection+"&&"+trigSelectionData_+"&&"+categorySelection+"&&"+osWSelectionData_+")",scaleUp_,prefix);

            std::pair<float,float> dataYieldSdb = convertToPoisson(dataYSdb);

            //then get ttbar in sideband
            std::pair<float,float> topYieldSdb      = createHistogramAndShifts(topFile_,"TT_SDB","("+preSelection+"&&"+trigSelection_+"&&"+categorySelection+"&&"+osWSelection_+")",weight_+"*"+TTweight_+"*"+btagsf,luminosity_*leg1Corr*topExtrap,prefix);
            printf("TTbar events in sideband region = %f + %f \n",topYieldSdb.first,topYieldSdb.second);
            //Next get Diboson in sideband
            std::pair<float,float> vvYieldSdb      = createHistogramAndShifts(vvFile_,"VV_SDB","("+preSelection+"&&"+trigSelection_+"&&"+categorySelection+"&&"+osWSelection_+")",weight_+"*"+btagsf,luminosity_*leg1Corr,prefix);
            printf("Diboson events in sideband region = %f + %f \n",vvYieldSdb.first,vvYieldSdb.second);
            //next get Z in sideband
            std::pair<float,float> zSDB        = createHistogramAndShifts(zttFile_,"Z_SDB","("+preSelection+"&&"+trigSelection_+"&&"+categorySelection+"&&"+osWSelection_+")",weight_+"*"+Zweight_+"*"+ZUnrollweight_+"*"+btagsf,luminosity_*zttScale_*leg1Corr,prefix);
            printf("Z events in sideband region = %f + %f \n",zSDB.first,zSDB.second);

            std::pair<float,float> wSDB        = createHistogramAndShifts(wFile_,"W_SDB","("+preSelection+"&&"+trigSelection_+"&&"+categorySelection+"&&"+osWSelection_+")",weight_+"*"+btagsf,luminosity_*leg1Corr,prefix);
            printf("Expected W events in sideband MC region btag  = %f + %f \n",wSDB.first,wSDB.second);
            std::pair<float,float> osWMChigh        = createHistogramAndShifts(wFile_,"W_SDB_nobtag","("+preSelection+"&&"+trigSelection_+"&&"+categorySelection+"&&"+osWSelection_+")",weight_,luminosity_*leg1Corr,prefix);
            printf("Expected W events in sideband MC region  = %f + %f \n",osWMChigh.first,osWMChigh.second);

            /*
             * also want to subtract QCD 
             * NOW GET QCD IN HIGH MT SS SDB
             */
            //NEXT GET SS QCD 
            //next get QCD in sideband
            //old function
            std::pair<float,float> dataYSdbSS     = createHistogramAndShifts(dataFile_,"QCD_SS_HMT_SDB","("+preSelection+"&&"+trigSelectionData_+"&&"+categorySelection+"&&"+ssWSelectionData_+")",scaleUp_,prefix);
            //then get ttbar in sideband
            //
            std::pair<float,float> topYieldSdbSS      = createHistogramAndShifts(topFile_,"TT_SS_HMT_SDB","("+preSelection+"&&"+trigSelection_+"&&"+categorySelection+"&&"+ssWSelection_+")",weight_+"*"+TTweight_+"*"+btagsf,luminosity_*leg1Corr*topExtrap,prefix);
            //Next get Diboson in sideband
            std::pair<float,float> vvYieldSdbSS      = createHistogramAndShifts(vvFile_,"VV_SS_HMT_SDB","("+preSelection+"&&"+trigSelection_+"&&"+categorySelection+"&&"+ssWSelection_+")",weight_+"*"+btagsf,luminosity_*leg1Corr,prefix);
            //Next get W in sideband
            std::pair<float,float> wSdbSS        = createHistogramAndShifts(wFile_,"W_SS_HMT_SDB","("+preSelection+"&&"+trigSelection_+"&&"+categorySelection+"&&"+ssWSelection_+")",weight_+"*"+btagsf,luminosity_*leg1Corr,prefix);
            //next get Z in sideband
            std::pair<float,float> zSdbSS        = createHistogramAndShifts(zttFile_,"Z_SS_HMT_SDB","("+preSelection+"&&"+trigSelection_+"&&"+categorySelection+"&&"+ssWSelection_+")",weight_+"*"+Zweight_+"*"+ZUnrollweight_+"*"+btagsf,luminosity_*zttScale_*leg1Corr,prefix);

            subtractHistogram(filelabel_+prefix,"QCD_SS_HMT_SDB","TT_SS_HMT_SDB");
            subtractHistogram(filelabel_+prefix,"QCD_SS_HMT_SDB","VV_SS_HMT_SDB");
            subtractHistogram(filelabel_+prefix,"QCD_SS_HMT_SDB","Z_SS_HMT_SDB");
            subtractHistogram(filelabel_+prefix,"QCD_SS_HMT_SDB","W_SS_HMT_SDB");

            std::pair<float,float> ssQCDhigh = std::make_pair(TMath::Nint(dataYSdbSS.first
                        -topYieldSdbSS.first
                        -vvYieldSdbSS.first
                        -wSdbSS.first
                        -zSdbSS.first),
                    sqrt(dataYSdbSS.second*dataYSdbSS.second
                        +topYieldSdbSS.second*topYieldSdbSS.second
                        +vvYieldSdbSS.second*vvYieldSdbSS.second
                        +wSdbSS.second*wSdbSS.second
                        +zSdbSS.second*zSdbSS.second));

            if(ssQCDhigh.first<0) {
                ssQCDhigh.first=0.0000001;
                ssQCDhigh.second=1.8;
            }
            printf("5. Extrapolate SS QCD -> OS QCD \n");
            std::pair<float,float> osQCDhigh = std::make_pair(ssQCDhigh.first*qcdFactor_,
                    sqrt(ssQCDhigh.second*ssQCDhigh.second*qcdFactor_*qcdFactor_
                        +qcdFactorErr_*qcdFactorErr_*ssQCDhigh.first*ssQCDhigh.first));

            renormalizeHistogram(filelabel_+prefix,"QCD_SS_HMT_SDB",osQCDhigh.first);
            printf("OS QCD estimation in high Mt sideband in wJets  =%f *%f = %f +- %f \n",ssQCDhigh.first,qcdFactor_,osQCDhigh.first,osQCDhigh.second);

            /*
             * NOW ACTUALLY SUBRACT THE BACKGORUNDS FROM THE W YIELD
             */
            //Run OS+LS + MT method
            //
            std::pair<float,float> osWHigh = std::make_pair(TMath::Nint(dataYieldSdb.first
                        -topYieldSdb.first
                        -vvYieldSdb.first
                        -zSDB.first
                        -osQCDhigh.first),
                    sqrt(dataYieldSdb.second*dataYieldSdb.second
                        +topYieldSdb.second*topYieldSdb.second
                        +vvYieldSdb.second*vvYieldSdb.second
                        +osQCDhigh.second*osQCDhigh.second
                        +zSDB.second*zSDB.second));

            if (osWMChigh.first<0.1||wYield.first<0.1) wFactorQCD_=1.0;

            wFactorQCD_ =  osWHigh.first/osWMChigh.first;

            if (channel_=="eleTau" && prefix=="_vbf") wFactorQCD_= 0.937;
            if (channel_=="muTau" &&prefix=="_vbf") wFactorQCD_= 0.937;

            printf("OS W Datadriven/Straight MC Estimation  = %f \n", wFactorQCD_);

            std::pair<float,float> osWLow = std::make_pair(wYield.first*wFactorQCD_,
                    sqrt(wYield.first*wYield.first*wFactorQCD_*wFactorQCD_+wYield.second*wYield.second*wFactorQCD_*wFactorQCD_));

            printf("OS W Datadriven Estimation  =%f  \n",osWHigh.first);
            printf("OS W Estimation =%f *%f  = %f +- %f \n",wYield.first,wFactorQCD_,osWLow.first,osWLow.second);

            if (osWLow.first<0.0000001) osWLow.first=0.0000001; 
            if(shifts_.size()>0){
                WSyst(filelabel_, prefix, "W", 1.0, wFactorErr_);
            }

            renormalizeHistogram(filelabel_+prefix,"W",osWLow.first);
            renormalizeHistogram(filelabel_+prefix,"W_CMS_htt_jetToTauFake_13TeVUp",osWLow.first);
            renormalizeHistogram(filelabel_+prefix,"W_CMS_htt_jetToTauFake_13TeVDown",osWLow.first);

            printf("OS W Straight MC Estimation  = %f +- %f \n", wYield.first, wYield.second);
            //use wFactorError instead!!!
            wFactorStatErr_ = (osWLow.first+osWLow.second)/wYield.first;

            printf("OS W Straight MC -> Full Sideband Estimation  = %f +- (stat)%f +- full %f \n",osWLow.first, wFactorStatErr_,wFactorErr_*osWLow.first);

            output.W  = osWLow.first;
            output.dW = osWLow.second;
            return true;
        }

        //-------------Helper Functions------------------


        void addHistogramShifts(std::string folder, std::string histo1, std::string histo2)
        {
            addHistogram(folder,histo1,histo2);
            for(unsigned int i=0;i<shiftsPostFix_.size();++i) {
                TH1F * h1 =(TH1F*) fout_->Get((folder+"/"+histo1+"_"+shiftsPostFix_[i]+"Up").c_str());
                if(h1!=0) 
                {
                    addHistogram(folder,(histo1+"_"+shiftsPostFix_[i]+"Up").c_str(),histo2);
                }
                else
                {
                    printf("Shift not found = %s\n",(folder+"/"+histo1+"_"+shiftsPostFix_[i]+"Up").c_str());
                }
                TH1F * h2 =(TH1F*) fout_->Get((folder+"/"+histo1+"_"+shiftsPostFix_[i]+"Down").c_str());
                if(h2!=0)
                {
                    addHistogram(folder,(histo1+"_"+shiftsPostFix_[i]+"Down").c_str(),histo2);
                }		
                else
                {
                    printf("Shift not found = %s\n",(folder+"/"+histo1+"_"+shiftsPostFix_[i]+"Down").c_str());
                }
            }//end for


        }//end addHistogram ZTT
        std::pair<float,float> createHistogramAndShifts(std::string file,std::string name, std::string cut,float scaleFactor = 1, std::string postfix = "",bool normUC  = true, bool keys=false,bool ShapeUncertainty=true) {
            TFile *f  = new TFile(file.c_str());
            if(f==0) printf("Not file Found\n");
            //get the nominal tree first
            TTree *t= (TTree*)f->Get((channel_+"EventTree/eventTree").c_str());
            if(t==0) printf("Not Tree Found in file %s\n",file.c_str());
            std::pair<float,float> yield;

            yield =makeHistogram(t,filelabel_+postfix,name,cut,scaleFactor,"",postfix);
            //yield =makeHistogram(t,filelabel_+postfix,name,cut,scaleFactor,shift,postfix);

            //now the shifts
            std::pair<float,float> tmpYield;
            for(unsigned int i=0;i<shifts_.size();++i) {

                TTree *ts= (TTree*)f->Get((channel_+"EventTree"+shifts_[i]+"Up/eventTree").c_str());
                if(ts!=0) {
                    if(!keys)
                        tmpYield = makeHistogram(ts,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",cut,scaleFactor);

                    if(!normUC)
                        scaleHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",yield.first/tmpYield.first);

                }
                TTree *td= (TTree*)f->Get((channel_+"EventTree"+shifts_[i]+"Down/eventTree").c_str());
                if(td!=0) {
                    if(!keys)
                        tmpYield = makeHistogram(td,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",cut,scaleFactor);

                    if(!normUC)
                        scaleHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",yield.first/tmpYield.first);

                }
            }
            f->Close();
            return yield;
        }


        std::pair<float,float> createHistogramAndShifts(std::string file,std::string name, std::string cut,std::string tmpWeight, float scaleFactor = 1, std::string postfix = "",bool normUC  = true, bool keys=false,bool ShapeUncertainty=true) {
            TFile *f  = new TFile(file.c_str());
            if(f==0) printf("Not file Found\n");
            //get the nominal tree first
            TTree *t= (TTree*)f->Get((channel_+"EventTree/eventTree").c_str());
            if(t==0) printf("Not Tree Found in file %s\n",file.c_str());
            std::pair<float,float> yield;
            std::string tmpCut = cut+"*"+tmpWeight;

            yield =makeHistogram(t,filelabel_+postfix,name,tmpCut,scaleFactor,"",postfix);

            //now the shifts
            std::pair<float,float> tmpYield;
            for(unsigned int i=0;i<shifts_.size();++i) {
                if (shifts_[i]=="Jet"){
                    for (unsigned int j=0;j<jetshifts_.size();j++){ //shiftsPostFix should be correctly set up
                        tmpYield = makeHistogram(t,filelabel_+postfix,name+"_"+shiftsPostFix_[i+j+3]+"Up",tmpCut,scaleFactor,shifts_[i]+"Up",postfix,j);
                        tmpYield = makeHistogram(t,filelabel_+postfix,name+"_"+shiftsPostFix_[i+j+3]+"Down",tmpCut,scaleFactor,shifts_[i]+"Down",postfix,j);
                    }
                }
                else if (shifts_[i]=="Tau"){
                    std::vector<std::string> dm = { cut+"*"+tmpWeight+"*(tauDecayMode==0&&gen_match_2==5)",cut+"*"+tmpWeight+"*(tauDecayMode==1&&gen_match_2==5)",cut+"*"+tmpWeight+"*(tauDecayMode==10&&gen_match_2==5)"};

                    for (unsigned int s=0;s<taushifts_.size();s++){
                        tmpYield = makeHistogram(t,filelabel_+postfix,name+"_"+shiftsPostFix_[i+s+1],dm[s],scaleFactor,shifts_[i],postfix);
                        tmpYield = makeHistogram(t,filelabel_+postfix,name+"_"+shiftsPostFix_[i+s+1]+"Up",dm[s],scaleFactor,shifts_[i]+"Up",postfix);
                        subtractHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i+s+1]+"Up",name+"_"+shiftsPostFix_[i+s+1]); //Up is only change and positive
                        tmpYield = makeHistogram(t,filelabel_+postfix,name+"_"+shiftsPostFix_[i+s+1]+"Down",dm[s],scaleFactor,shifts_[i]+"Down",postfix);
                        subtractHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i+s+1]+"Down",name+"_"+shiftsPostFix_[i+s+1], false);//down is only change and negative
                    }

                    //make total Up. 
                    //void renameHist(std::string folder, std::string name ,std::string histo1)///folder, new name, old name
                    //void subtractHistogram(std::string folder, std::string histo1, std::string histo2)
                    //shiftsPostFix_[i]==tautotal
                    //shiftsPostFix_[i+1]==1prong
                    //shiftsPostFix_[i+2]==1prong1pizero
                    //shiftsPostFix_[i+3]==3prong
                    renameHist(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",name); //clone and rename nominal  as total UP
                    addHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",name+"_"+shiftsPostFix_[i+1]+"Up"); //add 1prong up
                    addHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",name+"_"+shiftsPostFix_[i+2]+"Up"); //add 1p1p0 up
                    addHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",name+"_"+shiftsPostFix_[i+3]+"Up"); //add 3p ip

                    //make total Down. 
                    renameHist(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",name); //clone and rename nominal  as total DOWN
                    addHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",name+"_"+shiftsPostFix_[i+1]+"Down");
                    addHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",name+"_"+shiftsPostFix_[i+2]+"Down");
                    addHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",name+"_"+shiftsPostFix_[i+3]+"Down");

                    //make DM0 Up
                    addHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i+1]+"Up",name); //Up change 0 + total 
                    //make DM0 Down
                    addHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i+1]+"Down",name); //Down 0 + total 

                    //make DM1 Up
                    addHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i+2]+"Up",name); //Up 1 + Nom total
                    //make DM1 Down
                    addHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i+2]+"Down",name); //Down 1 + Nom 0

                    //make DM10 Up
                    addHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i+3]+"Up",name); //Up 10 + Nom 0
                    //make DM10 Down
                    addHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i+3]+"Down",name); //Down 10 + Nom 0


                }

                if(!normUC){
                    scaleHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",yield.first/tmpYield.first);
                    scaleHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",yield.first/tmpYield.first);
                }

            }
            f->Close();
            return yield;
        }


        std::pair<float,float> createHistogramAndShiftsQCD(std::string file,std::string name, std::string cut,float scaleFactor = 1, std::string postfix = "",bool normUC  = true, bool keys=false,bool ShapeUncertainty=true) {
            TFile *f  = new TFile(file.c_str());
            if(f==0) printf("Not file Found\n");
            //get the nominal tree first
            TTree *t= (TTree*)f->Get((channel_+"EventTree/eventTree").c_str());
            if(t==0) printf("Not Tree Found in file %s\n",file.c_str());
            std::pair<float,float> yield;

            yield =makeHistogram(t,filelabel_+postfix,name,cut,scaleFactor);
            //yield =makeHistogram(t,filelabel_+postfix,name,cut,scaleFactor,shift,postfix);

            //now the shifts
            std::pair<float,float> tmpYield;
            for(unsigned int i=0;i<shiftsPostFix_.size();++i) {

                tmpYield = makeHistogram(t,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",cut,scaleFactor);
                //tmpYield = makeHistogram(t,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",cut,scaleFactor,shifts_[i]+"Up",postfix);

                tmpYield = makeHistogram(t,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",cut,scaleFactor);
                //tmpYield = makeHistogram(t,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",cut,scaleFactor,shifts_[i]+"Down",postfix);

            }
            f->Close();
            return yield;
        }



        void setBinning(const std::vector<double>& binning) {
            binning_ = binning;
        }

        std::pair<float,float> inflateError(std::pair<float,float> input,float error, float error2 =0.0, float error3 = 0.0 ) {
            float value = input.first;
            float err = sqrt(input.second*input.second+(input.first*error)*(input.first*error)+(input.first*error2)*(input.first*error2)+(input.first*error3)*(input.first*error3));

            if(err>value) {
                err=0.99*value;
                printf("Found error above value -> Setting error to value\n");
            }

            return std::make_pair(value,err);
        }

        void renormalizeHistogram(std::string folder, std::string histo, float yield)
        {

            TH1F * h =(TH1F*) fout_->Get((folder+"/"+histo).c_str());
            double scaleFactor = yield/h->Integral();


            h->Scale(scaleFactor);
            fout_->cd(folder.c_str());
            h->Write(h->GetName(),TObject::kOverwrite);

            for(unsigned int i=0;i<shiftsPostFix_.size();++i) {
                TH1F * hh =(TH1F*) fout_->Get((folder+"/"+histo+"_"+shiftsPostFix_[i]+"Up").c_str());
                if(hh!=0) {
                    hh->Scale(scaleFactor);
                    fout_->cd(folder.c_str());
                    hh->Write(hh->GetName(),TObject::kOverwrite);
                }
                else
                {
                    printf("Shift not found = %s\n",(folder+"/"+histo+"_"+shiftsPostFix_[i]+"Up").c_str());
                }


                TH1F * hhh =(TH1F*) fout_->Get((folder+"/"+histo+"_"+shiftsPostFix_[i]+"Down").c_str());
                if(hhh!=0) {
                    hhh->Scale(scaleFactor);
                    fout_->cd(folder.c_str());
                    hhh->Write(hhh->GetName(),TObject::kOverwrite);
                }
                else
                {
                    printf("Shift not found\n");
                }
            }
        }

        float quadrature(float value, float error, float e0 = 0.0, float e1 = 0.0 ,float e2 = 0.0, float e3 = 0.0 ,float e4 = 0.0, float e5 = 0.0 ){
            float returnValue = sqrt(error*error+(e0*e0+e1*e1+e2*e2+e3*e3+e4*e4+e5*e5)*value*value);
            return returnValue;
        }


        void ReplaceStringInPlace(std::string& subject, const std::string& search,
                const std::string& replace) {
            size_t pos = 0;
            while ((pos = subject.find(search, pos)) != std::string::npos) {
                subject.replace(pos, search.length(), replace);
                pos += replace.length();
            }
        }

        std::pair<float,float> makeHistogram(TTree* tree,std::string folder,std::string name,std::string cut,float scaleFactor = 1.,std::string shft="",std::string prefix="",int j=0) {

            if(fout_->Get(folder.c_str())==0)
                fout_->mkdir(folder.c_str());
            TH1F *h=0;

            TString variableOld_=variable_;
            std::string newCuts_=cut;

            std::string zl="ZL";
            std::string qcd="QCD";
            bool isQCD = (name.compare(0, qcd.length(), qcd) == 0);
            bool isZL = (name.compare(0, zl.length(), zl) == 0);

            //std::cout<<"############# Make Histo ###########"<<std::endl;
            //std::cout<<"\t name: "<<name<<std::endl;
            //std::cout<<"\t isQCD: "<<isQCD<<std::endl;

            if((name=="data_obs"||name=="data_obs_ss"||name=="data_obs_sdb"||name=="data_obs_ss_sdb"||name=="QCD"||isQCD)){
                //std::cout<<"Replacing DATA cut: "<<cut<<std::endl;
                if (variable_=="taupt") variable_="pt_2";
                else if (variable_=="m_vis_REDO") variable_="m_vis";

                ReplaceStringInPlace(newCuts_, "taupt", "pt_2");
                std::cout<<"Replacing cut for data! "<<newCuts_<<std::endl;
            }
            else if (isZL && prefix=="_0jet"){
                variable_="m_vis_ZL";
                if (shft=="TauUp")  variable_="m_vis_ZL_UP";
                if (shft=="TauDown")  variable_="m_vis_ZL_DOWN";
            }
            else if (shft=="TauUp"){
                if (variable_=="m_sv") variable_="m_sv_UP";
                else if (variable_=="m_vis_REDO") variable_="m_vis_UP";
                else if (variable_=="mtRecoil_1") variable_="mtRecoil_1UP";
                ReplaceStringInPlace(newCuts_, "pt_sv", "pt_sv_UP");
                ReplaceStringInPlace(newCuts_, "taupt", "taupt_UP");
                ReplaceStringInPlace(newCuts_, "mtRecoil_1", "mtRecoil_1UP");
            }
            else if (shft=="TauDown"){
                if (variable_=="m_sv") variable_="m_sv_DOWN";
                else if (variable_=="m_vis_REDO") variable_="m_vis_DOWN";
                else if (variable_=="mtRecoil_1") variable_="mtRecoil_1DOWN";
                ReplaceStringInPlace(newCuts_, "pt_sv", "pt_sv_DOWN");
                ReplaceStringInPlace(newCuts_, "taupt", "taupt_DOWN");
                ReplaceStringInPlace(newCuts_, "mtRecoil_1", "mtRecoil_1DOWN");
            }
            else if (shft=="JetUp"){
                std::string vbfvar = "vbfMass_"+jetshifts_[j]+"Up";
                std::string njvar = "njet_"+jetshifts_[j]+"Up";
                ReplaceStringInPlace(newCuts_, "njets", njvar);
                ReplaceStringInPlace(newCuts_, "vbfMass", vbfvar);
            }
            else if (shft=="JetDown"){
                std::string vbfvar = "vbfMass_"+jetshifts_[j]+"Down";
                std::string njvar = "njet_"+jetshifts_[j]+"Down";
                ReplaceStringInPlace(newCuts_, "njets", njvar);
                ReplaceStringInPlace(newCuts_, "vbfMass", vbfvar);
            }

            if(binning_.size()==0)
                h= new TH1F(name.c_str(),name.c_str(),bins_,min_,max_);
            else 
                h = new TH1F(name.c_str(),name.c_str(),binning_.size()-1,&binning_[0]);
            h->Sumw2();

            tree->Draw((variable_+">>"+name).c_str(),newCuts_.c_str());

            variable_=variableOld_;//

            h->Scale(scaleFactor);

            if(verbose_>0)
                std::cout<< " " <<name<<": "<<h->Integral()<<std::endl;

            //     printf("Created Histogram %s with entries=%f\n",name.c_str(),h->Integral());
            fout_->cd(folder.c_str());

            Double_t error=0.0;
            //LD
            float yield = h->IntegralAndError(1,h->GetNbinsX(),error,"");
            //float yield = h->IntegralAndError(0,h->GetNbinsX()+1,error,"");
            if(yield == 0){
                h->SetBinContent(1,1,0.000001);
            }
            h->Write(h->GetName(),TObject::kOverwrite);

            return std::make_pair(yield,error);
        }

        void close() {
            fout_->Close();
        }

        std::pair<float,float> createHistogramShifted(std::string file,std::string name, std::string cut,std::string shift,float scaleFactor = 1, std::string postfix = "") {
            TFile *f  = new TFile(file.c_str());
            if(f==0) printf("Not file Found\n");
            //get the nominal tree first
            TTree *t= (TTree*)f->Get((channel_+"EventTree/eventTree").c_str());
            if(t==0) printf("Not Tree Found in file %s\n",file.c_str());

            std::pair<float,float> yield;

            yield =makeHistogram(t,filelabel_+postfix,name,cut,scaleFactor,shift,postfix);

            f->Close();
            return yield;
        }

        std::pair<float,float> makeHistogramShifted(TTree* tree,std::string folder,std::string name,std::string cut, std::string shift,float scaleFactor = 1.) {

            if(fout_->Get(folder.c_str())==0)
                fout_->mkdir(folder.c_str());
            TH1F *h=0;

            if(binning_.size()==0)
                h= new TH1F(name.c_str(),name.c_str(),bins_,min_,max_);
            else 
                h = new TH1F(name.c_str(),name.c_str(),binning_.size()-1,&binning_[0]);
            h->Sumw2();


            tree->Draw((variable_+"*"+shift+">>"+name).c_str(),cut.c_str());

            h->Scale(scaleFactor);
            fout_->cd(folder.c_str());

            Double_t error=0.0;
            float yield = h->IntegralAndError(1,h->GetNbinsX(),error,"");
            if(yield == 0){
                h->SetBinContent(1,0.00001);
            }
            h->Write(h->GetName(),TObject::kOverwrite);

            return std::make_pair(yield,error);
        }

        void scaleHistogram(std::string folder, std::string histo, float scaleFactor)
        {
            TH1F * h =(TH1F*) fout_->Get((folder+"/"+histo).c_str());
            h->Scale(scaleFactor);
            fout_->cd(folder.c_str());
            h->Write(h->GetName(),TObject::kOverwrite);
            fout_->cd();

            for(unsigned int i=0;i<shiftsPostFix_.size();++i) {
                TH1F * hh =(TH1F*) fout_->Get((folder+"/"+histo+"_"+shiftsPostFix_[i]+"Up").c_str());
                if(hh!=0) {
                    hh->Scale(scaleFactor);
                    fout_->cd(folder.c_str());
                    hh->Write(hh->GetName(),TObject::kOverwrite);
                    fout_->cd();
                }

                TH1F * hhh =(TH1F*) fout_->Get((folder+"/"+histo+"_"+shiftsPostFix_[i]+"Down").c_str());
                if(hhh!=0) {
                    hhh->Scale(scaleFactor);
                    fout_->cd(folder.c_str());
                    hhh->Write(hhh->GetName(),TObject::kOverwrite);
                    fout_->cd();
                }

            }
        }






        std::pair<float,float> extractWFactor(std::string file,std::string preselection,std::string postfix, std::string Wweight = "1") {
            TFile *f  = new TFile (file.c_str());
            TTree *t = (TTree*)f->Get((channel_+"EventTree/eventTree").c_str());

            std::pair<float,float> high;
            std::pair<float,float> low;
            high = makeHistogram(t,filelabel_+postfix,"W_High",("("+preselection+"&&"+wSelection_+")*"+weight_+"*"+Wweight).c_str());
            low = makeHistogram(t,filelabel_+postfix,"W_Low",("("+preselection+"&&"+signalSelection_+")*"+weight_+"*"+Wweight).c_str());

            std::cout<<"W + Jets High: "<< high.first<<" Low: "<< low.first<<std::endl;

            float factor = low.first/high.first;
            float factorerrStat = sqrt(low.second*low.second + high.second*high.second);
            float factorerrSyst = factor*wFactorErr_;
            float factorErr = sqrt(factorerrStat*factorerrStat+factorerrSyst*factorerrSyst);

            if(high.first==0.){ 
                factor=0.;
                factorErr=0.;
            }
            return std::make_pair(factor,factorErr);   

        }

        std::pair<float,float> extractWFactor(std::string file, std::string preselection, std::string postfix, std::string wSel, std::string sigSel) {
            TFile *f  = new TFile (file.c_str());
            TTree *t = (TTree*)f->Get((channel_+"EventTree/eventTree").c_str());
            //TTree *t = (TTree*)f->Get((channel_+"EventTreeFinal/eventTree").c_str());

            std::pair<float,float> high;
            std::pair<float,float> low;


            high = makeHistogram(t,filelabel_+postfix,"W_High",("("+preselection+"&&"+wSel+")*"+weight_).c_str(),luminosity_);
            low = makeHistogram(t,filelabel_+postfix,"W_Low",("("+preselection+"&&"+sigSel+")*"+weight_).c_str(),luminosity_);

            std::cout<<"      Cuts in extract WFactor "<<"      OS W Mt>80: "<<high.first<<"      OS W Mt Low: "<<low.first<<std::endl;
            std::cout<<"      Extract WFactor Second"<<"      OS W Mt>80: "<<high.second<<"      OS W Mt Low: "<<low.second<<std::endl;
            float factor = low.first/high.first;
            float factorerrStat = sqrt(low.second*low.second + high.second*high.second);
            float factorerrSyst = factor*wFactorErr_;
            float factorErr = sqrt(factorerrStat*factorerrStat+factorerrSyst*factorerrSyst);


            std::cout<<"      WFactor Stat Error "<<"      factor: "<<factor<<"      StatError: "<<factorerrStat<<std::endl;
            std::cout<<"      WFactor Syst Error "<<"      factor*wFactorErr_: "<<factor<<"*"<<wFactorErr_<<" = "<<factorerrSyst<<std::endl; 
            std::cout<<"      WFactor Error "<<"      factorError = sqrt(Syst Error^2+Stat Error^2)= "<<factorErr<<std::endl; 
            if(high.first==0.){
                factor=0.;
                factorErr=0.;
            }
            return std::make_pair(factor,factorErr);   

        }


        void mergeHistogram(std::string folder, std::string histo1, std::string histo2, std::string newName)
        {
            TH1F * h1 =(TH1F*) fout_->Get((folder+"/"+histo1).c_str());
            TH1F * h2 =(TH1F*) fout_->Get((folder+"/"+histo2).c_str());
            TH1F *hNew =(TH1F*) h1->Clone(TString(newName));
            hNew->Add(h2,1.);

            for(int i=1;i<h1->GetNbinsX();++i){
                if(h1->GetBinContent(i)<0){
                    h1->SetBinContent(i,0.00001);
                    h1->SetBinError(i,1.0);
                }
            }

            fout_->cd(folder.c_str());
            hNew->Write(TString(newName),TObject::kOverwrite);
        }

        std::pair<float,float> convertToPoisson(std::pair<float,float> measurement) {
            float yield = measurement.first;
            float CLHi = TMath::ChisquareQuantile(1-0.32/2,2*yield+2)/2.;
            float CLLo = TMath::ChisquareQuantile(0.32/2,2*yield)/2.;
            printf("Yield =%f Lo=%f Hi=%f\n",measurement.first,CLLo,CLHi);
            return std::make_pair(measurement.first,(CLHi-CLLo)/2.);

        }

        void subtractHistogramShifts(std::string folder, std::string histo1, std::string histo2, bool floor = false )
        {
            TH1F * h1 =(TH1F*) fout_->Get((folder+"/"+histo1).c_str());
            TH1F * h2 =(TH1F*) fout_->Get((folder+"/"+histo2).c_str());
            h1->Add(h2,-1.);

            if (floor) {
                for(int i=1;i<h1->GetNbinsX();++i){
                    if(h1->GetBinContent(i)<0){
                        h1->SetBinContent(i,0.00001);
                        h1->SetBinError(i,1.0);
                    }
                }
            }

            for(unsigned int i=0;i<shiftsPostFix_.size();++i) {
                TH1F * hhu1 =(TH1F*) fout_->Get((folder+"/"+histo1+"_"+shiftsPostFix_[i]+"Up").c_str());
                TH1F * hhu2 =(TH1F*) fout_->Get((folder+"/"+histo2+"_"+shiftsPostFix_[i]+"Up").c_str());

                TH1F * hhd1=(TH1F*) fout_->Get((folder+"/"+histo1+"_"+shiftsPostFix_[i]+"Down").c_str());
                TH1F * hhd2 =(TH1F*) fout_->Get((folder+"/"+histo2+"_"+shiftsPostFix_[i]+"Down").c_str());
                hhu1->Add(hhu2,-1.);
                hhd1->Add(hhd2,-1.);
                fout_->cd(folder.c_str());
                hhu1->Write(hhu1->GetName(),TObject::kOverwrite);
                hhd1->Write(hhd1->GetName(),TObject::kOverwrite);
            }

            fout_->cd(folder.c_str());
            h1->Write(h1->GetName(),TObject::kOverwrite);
        }


        void subtractHistogram(std::string folder, std::string histo1, std::string histo2, bool floor=false)
        {
            TH1F * h1 =(TH1F*) fout_->Get((folder+"/"+histo1).c_str());
            TH1F * h2 =(TH1F*) fout_->Get((folder+"/"+histo2).c_str());
            h1->Add(h2,-1.);
            if (floor){
                for(int i=1;i<h1->GetNbinsX();++i){
                    if(h1->GetBinContent(i)<0){
                        h1->SetBinContent(i,0.00001);
                        h1->SetBinError(i,1.0);
                    }
                }
            }

            fout_->cd(folder.c_str());
            h1->Write(h1->GetName(),TObject::kOverwrite);
        }

        void addHistogram(std::string folder, std::string histo1, std::string histo2)
        {
            TH1F * h1 =(TH1F*) fout_->Get((folder+"/"+histo1).c_str());
            TH1F * h2 =(TH1F*) fout_->Get((folder+"/"+histo2).c_str());
            h1->Add(h2,1.);

            fout_->cd(folder.c_str());
            h1->Write(h1->GetName(),TObject::kOverwrite);
        }

        void renameHist(std::string folder, std::string name ,std::string histo1)///folder, new name, old name
        {
            TH1F * h1 =(TH1F*) fout_->Get((folder+"/"+histo1).c_str());
            TH1F * h2 = (TH1F*) h1->Clone(TString(name));

            h2->Write(h2->GetName(),TObject::kOverwrite);
        }


        void FixNegativeBins(std::string folder, std::string histo, bool verbose=false) {

            TH1F * hist =(TH1F*) fout_->Get((folder+"/"+histo).c_str());
            unsigned bins = hist->GetNbinsX();
            for (unsigned i = 1; i <= bins; ++i) {
                if (hist->GetBinContent(i) < 0.){
                    hist->SetBinContent(i,0);
                    if(verbose)
                        std::cout<<"Bin "<< i << " is negative, setting to 0"<<std::endl;
                }
            }
            hist->Write(hist->GetName(),TObject::kOverwrite);
        }

        void FixNegativeBinsMC(std::string folder, std::string histo,  bool verbose=false){

            FixNegativeBins(folder, histo, verbose);

            for(unsigned int i=0;i<shiftsPostFix_.size();++i) {
                TH1F * hh =(TH1F*) fout_->Get((folder+"/"+histo+"_"+shiftsPostFix_[i]+"Up").c_str());
                if(hh!=0) {
                    FixNegativeBins(folder, (histo+"_"+shiftsPostFix_[i]+"Up").c_str(), verbose);
                }

                TH1F * hhh =(TH1F*) fout_->Get((folder+"/"+histo+"_"+shiftsPostFix_[i]+"Down").c_str());
                if(hhh!=0) {
                    FixNegativeBins(folder, (histo+"_"+shiftsPostFix_[i]+"Down").c_str(), verbose);
                }

            }
        }


        void FixEmptyBins(std::string folder, std::string histo, bool is_qcd, bool verbose=false) {

            TH1F * hist =(TH1F*) fout_->Get((folder+"/"+histo).c_str());
            unsigned bins = hist->GetNbinsX();
            unsigned first_populated = 0;
            unsigned last_populated = 0;
            for (unsigned i = 1; i <= bins; ++i) {
                if (hist->GetBinContent(i) > 0. && first_populated == 0) first_populated = i;
                if (hist->GetBinContent(bins-(i-1)) > 0. && last_populated == 0) last_populated = bins-(i-1);
            }
            if (last_populated <= first_populated) {
                if (verbose) std::cout << "Cannot correct this distribution!" << std::endl;
                return;
            }
            if (verbose) std::cout << "First populated bin: " << first_populated << std::endl;
            if (verbose) std::cout << "Last populated bin: " << last_populated << std::endl;
            double av_weight = ( hist->Integral() / double(hist->GetEntries()));
            if (!is_qcd) {
                if (verbose) std::cout << "Integral: " << hist->Integral() << std::endl;
                if (verbose) std::cout << "Entries: " << hist->GetEntries() << std::endl;
                if (verbose) std::cout << "Av. Weight: " << av_weight << std::endl;
            }
            for (unsigned i = first_populated+1; i < last_populated; ++i) {
                if (hist->GetBinContent(i) == 0.) {
                    if (verbose) std::cout << "Bin " << i << " is empty!" << std::endl;
                    if (is_qcd) {
                        hist->SetBinError(i, 1.0);
                    } else {
                        if (verbose) std::cout << "Set weight to 1.0 * av_weight = " << (1.0 * av_weight) << std::endl;
                        hist->SetBinError(i, av_weight);    
                    }
                }
            }
            hist->Write(hist->GetName(),TObject::kOverwrite);
        }


        bool WSyst(std::string channel, std::string prefix, std::string histo1, float corr, float unc)
        {

            std::string chan = "mt";
            if(channel_ == "eleTau")
                chan = "et";

            TH1F * h1 =(TH1F*) fout_->Get((channel+prefix+"/"+histo1).c_str());
            TH1F * h2 = (TH1F*)h1->Clone(TString(histo1+std::string("_WSFUncert_"+chan+prefix+"_"+energy_+"Up")));
            TH1F * h3 = (TH1F*)h1->Clone(TString(histo1+std::string("_WSFUncert_"+chan+prefix+"_"+energy_+"Down")));
            std::cout << "Channel label is " << chan << std::endl;
            float x,y;
            float cUp = corr+unc;
            float cDown = corr-unc;
            //printf("Creating QCD Shape Systematic \n");
            float scale = h1->Integral();
            for(int i=1;i<h1->GetNbinsX();++i){
                x = h1->GetXaxis()->GetBinCenter(i);
                y = h1->GetBinContent(i);
                h1->SetBinContent(i,y*corr);
                h2->SetBinContent(i,y*cUp);
                h3->SetBinContent(i,y*cDown);

            }
            //h1->Scale(scale/h1->Integral());
            //h2->Scale(scale/h2->Integral());
            //h3->Scale(scale/h3->Integral());
            h1->Write(h1->GetName(),TObject::kOverwrite);
            h2->Write(h2->GetName(),TObject::kOverwrite);
            h3->Write(h3->GetName(),TObject::kOverwrite);        

            return true;
        }


        bool qcdSyst(std::string channel, std::string prefix, std::string histo1, float corr, float unc)
        {

            std::string chan = "mt";
            if(channel_ == "eleTau")
                chan = "et";

            TH1F * h1 =(TH1F*) fout_->Get((channel+prefix+"/"+histo1).c_str());
            TH1F * h2 = (TH1F*)h1->Clone(TString(histo1+std::string("_QCDSFUncert_"+chan+prefix+"_"+energy_+"Up")));
            TH1F * h3 = (TH1F*)h1->Clone(TString(histo1+std::string("_QCDSFUncert_"+chan+prefix+"_"+energy_+"Down")));
            std::cout << "Channel label is " << chan << std::endl;
            float x,y;
            float cUp = corr+unc;
            float cDown = corr-unc;
            //printf("Creating QCD Shape Systematic \n");
            float scale = h1->Integral();
            for(int i=1;i<h1->GetNbinsX();++i){
                x = h1->GetXaxis()->GetBinCenter(i);
                y = h1->GetBinContent(i);
                h1->SetBinContent(i,y*corr);
                h2->SetBinContent(i,y*cUp);
                h3->SetBinContent(i,y*cDown);
            }
            //h1->Scale(scale/h1->Integral());
            //h2->Scale(scale/h2->Integral());
            //h3->Scale(scale/h3->Integral());
            h1->Write(h1->GetName(),TObject::kOverwrite);
            h2->Write(h2->GetName(),TObject::kOverwrite);
            h3->Write(h3->GetName(),TObject::kOverwrite);        

            return true;
        }



    private:
        std::string channel_;
        std::string filelabel_;
        std::vector<std::string> shifts_;
        std::vector<std::string> shiftsPostFix_;
        std::string energy_;
        int samesign_;

        //files
        TFile *fout_;
        int verbose_;
        std::string zttFile_;
        std::string zllFile_;
        std::string wFile_;
        std::string vvFile_;
        std::string ewkFile_;
        std::string topFile_;
        std::string qcdFile_;
        std::string dataFile_;
        ///////////////////////
        std::vector<std::string> taushifts_ = {"1prong","1prong1pizero","3prong"};
        std::vector<std::string> jetshifts_ = {"Total","RelativeBal"};
        //std::vector<std::string> jetshifts_ ={"Total","AbsoluteFlavMap", "AbsoluteMPFBias", "AbsoluteScale", "AbsoluteStat", "FlavorQCD", "Fragmentation", "PileUpDataMC", "PileUpPtBB", "PileUpPtEC1", "PileUpPtEC2", "PileUpPtHF", "PileUpPtRef", "RelativeBal", "RelativeFSR", "RelativeJEREC1", "RelativeJEREC2", "RelativeJERHF", "RelativePtBB", "RelativePtEC1", "RelativePtEC2", "RelativePtHF", "RelativeStatEC", "RelativeStatFSR", "RelativeStatHF", "SinglePionECAL", "SinglePionHCAL", "SubTotalAbsolute", "SubTotalMC", "SubTotalPileUp", "SubTotalPt", "SubTotalRelative", "SubTotalScale", "TimePtEta", "Closure"};
        //selections of regions
        std::string preSelection_;
        std::string osSignalSelection_;
        std::string ssSignalSelection_;
        std::string signalSelection_;
        std::string osWSelection_;
        std::string osWSelectionData_;
        std::string ssWSelection_;
        std::string ssWSelectionData_;
        std::string wSelection_;
        std::string qcdSelection_;
        std::string relaxedSelection_;
        std::string trigSelection_;
        std::string trigSelectionData_;
        std::string blinding_;
        std::string charge_;
        std::string ZLL_genLLSel_;
        std::string ZTT_genTauSel_;
        std::string ZJFT_genLReject_;
        std::string ZLFT_genLSel_;
        std::string genTSel_;
        std::string dataSelection_;

        //Luminosity and efficiency corrections
        float luminosity_;
        float luminosityErr_;
        float  muID_   ;      
        float muIDErr_;      
        float eleID_ ;       
        float eleIDErr_;     
        float tauID_  ;
        float tauIDHigh_;      
        float tauIDErr_;     
        float zttScale_;     
        float zttScaleErr_;  

        float scaleUp_;

        //histogram options
        std::string variable_;
        int bins_;
        float min_;
        float max_;
        std::vector<double> binning_;
        std::string weight_;
        std::string Zweight_;
        std::string ZUnrollweight_;
        std::string TTweight_;
        std::string embWeight_;
        std::string ggHWeight_;

        std::string zlftFactor_;
        //external parameters
        float topErr_;
        float qcdErr_;//tmp
        float vvErr_;
        float zlftErr_;
        float zjftErr_;
        float wFactorQCD_;
        float wFactorStatErr_;
        float wFactorErr_;
        float qcdFactor_;
        float qcdFactorErr_;



        float bID_;
        float bIDErr_;
        float bMisID_;
        float bMisIDErr_;

        std::string dir_;


};
