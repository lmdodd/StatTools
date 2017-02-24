#include "TCanvas.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TF1.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h" 
#include <math.h>
#include <boost/algorithm/string.hpp>
#include "HiggsAnalysis/CombinedLimit/interface/th1fmorph.h"
#include "HiggsAnalysis/CombinedLimit/interface/TH1Keys.h"
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
    float EWK, dEWK;
    float SMH, dSMH;
    float VV, dVV;
    float ZVV, dZVV;
    float ZTT, dZTT;

    BkgOutput(){};

    BkgOutput(float i) {
        // This assumes that a-d are all of the same type and all in order
        std::fill(&DATA, &dZTT+1, i);
    }

};



class DataCardCreatorXTT_MC {
    public:

        DataCardCreatorXTT_MC(optutl::CommandLineParser parser) {
            channel_ = parser.stringValue("channel");
            shifts_  = parser.stringVector("shifts");
            energy_ = parser.stringValue("energy");
            samesign_ = parser.doubleValue("samesign");


            std::string name_=channel_;
            if (channel_=="eleTau") filelabel_="et";
            if (channel_=="muTau") filelabel_="mt";
            if (channel_=="diTau") filelabel_="tt";


            //create the name you need to add to the histograms 
            //in the root file 
            for(unsigned int i=0;i<shifts_.size();++i) {
                std::string shiftL = shifts_.at(i);
                shiftL.resize(1);
                boost::to_lower(shiftL);
                std::cout<<"shiftL: "<<shiftL<<std::endl;
                std::string chan = "mt";
                //Change Me
                if(channel_=="eleTau") chan = "et";
                if(channel_=="diTau") chan = "tt";
                shiftsPostFix_.push_back("CMS_scale_"+shiftL+"_"+chan+"_"+energy_);
            }

            //read input files
            zttFile_  = parser.stringValue("zttFile");
            zllFile_  = parser.stringValue("zllFile");
            wFile_    = parser.stringValue("wFile");
            zhFile_   = parser.stringValue("zhFile");
            vvFile_   = parser.stringValue("vvFile");
            zvvFile_   = parser.stringValue("zvvFile");
            ewkFile_   = parser.stringValue("ewkFile");
            topFile_  = parser.stringValue("topFile");
            qcdFile_  = parser.stringValue("qcdFile");
            dataFile_  = parser.stringValue("dataFile");

            //read control and signal regions
            preSelection_ = parser.stringValue("preselection");

            signalSelection_      = parser.stringValue("signalselection");
            wSelection_           = parser.stringValue("wselection");
            //zSelection_           = parser.stringValue("zselection");
            qcdSelection_         = parser.stringValue("qcdSelection");
            relaxedSelection_     = parser.stringValue("relaxedselection");
            trigSelection_        = parser.stringValue("trigSelection");
            trigSelectionDATA_        = parser.stringValue("trigSelectionData");
            blinding_             = parser.stringValue("blinding");
            charge_               = parser.stringValue("charge");
            if (samesign_) charge_="abs(charge)==2";

            ZTT_genTauSel_        = "gen_match_2==5"; //Zttyield
            ZLFT_genLSel_         = "gen_match_2<5";
            ZJFT_genLReject_      = "gen_match_2==6";
            ZLL_genLLSel_        = "(gen_match_2<5||gen_match_2==6)"; //Zttyield
            /*
               ZTT_genTauSel_        = "genVisPt2>15"; //Zttyield
               ZLFT_genLSel_         = "genVisPt2<15&&gen_match_2<5";
               ZJFT_genLReject_      = "genVisPt2<15&&gen_match_2==6";
               ZLL_genLLSel_        = "genVisPt2<15"; //Zttyield
               */

            if (channel_=="diTau"){
                ZTT_genTauSel_        = "genTaus==2"; //Zttyield
                ZLFT_genLSel_         = "genTaus!=2&&((isPrompt1&&isPrompt2)||(isTauDecay1&&isTauDecay2))";
                ZJFT_genLReject_      = "genTaus!=2&&!((isPrompt1&&isPrompt2)||(isTauDecay1&&isTauDecay2))";
            }

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

            ssWSelection_         = wSelection_+"&&abs(charge)==2";
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
            Zweight_       = parser.stringValue("Zweight");
            Vweight_       = parser.stringValue("Vweight");
            TTweight_      = parser.stringValue("TTweight");
            embWeight_     = parser.stringValue("embWeight");

            //define the histogram binning
            bins_ = parser.integerValue("bins");
            min_  = parser.doubleValue("min");
            max_  = parser.doubleValue("max");


            //Define background uncertainty Errors
            topErr_     = parser.doubleValue("topErr");
            qcdErr_     = parser.doubleValue("qcdErr");
            smhErr_      = parser.doubleValue("smhErr");
            vvErr_      = parser.doubleValue("vvErr");
            zlftErr_    = parser.doubleValue("zlftErr");
            zlftFactor_ = parser.stringValue("zlftFactor");

            zjftErr_      = parser.doubleValue("zjftErr");
            wFactorErr_   = parser.doubleValue("wFactorErr");
            qcdFactor_    = parser.doubleValue("qcdFactor");
            qcdFactorErr_ = parser.doubleValue("qcdFactorErr");
            bFactorZ_     = parser.doubleValue("bFactorZ");
            bFactorZErr_  = parser.doubleValue("bFactorZErr");
            bFactorW_     = parser.doubleValue("bFactorW");
            bFactorWErr_  = parser.doubleValue("bFactorWErr");

            dir_ = parser.stringValue("dir");

            scaleUp_ = parser.doubleValue("scaleUp");

            fout_ = new TFile(parser.stringValue("outputfile").c_str(),"RECREATE");

        }


        void makeHiggsShape(std::string preselection, std::string categoryselection, std::string prefix) {

            std::pair<float,float> tmp;
            float legCorr=1.0;
            float BR=0.06272;

            if(muID_!=0&&eleID_!=0) {legCorr*=muID_*eleID_;}
            if(muID_!=0&&eleID_==0) {legCorr*=muID_*tauID_;}
            if(muID_==0&&eleID_!=0) {legCorr*=eleID_*tauID_;}
            std::cout<<"Make Higgs Shape"<<std::endl;
            //600 histos
            //

            tmp= createHistogramAndShifts(dir_+"ZpBaryonic_Zp10000_MChi1000.root","ZpBaryonic_Zp10000_MChi1000",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"ZpBaryonic_Zp10000_MChi500.root","ZpBaryonic_Zp10000_MChi500",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"ZpBaryonic_Zp10000_MChi150.root","ZpBaryonic_Zp10000_MChi150",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"ZpBaryonic_Zp10000_MChi50.root","ZpBaryonic_Zp10000_MChi50",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"ZpBaryonic_Zp10000_MChi10.root","ZpBaryonic_Zp10000_MChi10",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"ZpBaryonic_Zp10000_MChi1.root","ZpBaryonic_Zp10000_MChi1",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"ZpBaryonic_Zp1000_MChi1000.root","ZpBaryonic_Zp1000_MChi1000",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"ZpBaryonic_Zp1000_MChi150.root","ZpBaryonic_Zp1000_MChi150",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"ZpBaryonic_Zp1000_MChi1.root","ZpBaryonic_Zp1000_MChi1",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"ZpBaryonic_Zp500_MChi500.root","ZpBaryonic_Zp500_MChi500",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"ZpBaryonic_Zp500_MChi150.root","ZpBaryonic_Zp500_MChi150",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);

            tmp= createHistogramAndShifts(dir_+"ZpBaryonic_Zp100_MChi10.root","ZpBaryonic_Zp100_MChi10",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"ZpBaryonic_Zp100_MChi1.root","ZpBaryonic_Zp100_MChi1",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"ZpBaryonic_Zp10_MChi1000.root","ZpBaryonic_Zp10_MChi1000",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"ZpBaryonic_Zp10_MChi10.root","ZpBaryonic_Zp10_MChi10",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"ZpBaryonic_Zp10_MChi150.root","ZpBaryonic_Zp10_MChi150",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"ZpBaryonic_Zp10_MChi1.root","ZpBaryonic_Zp10_MChi1",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"ZpBaryonic_Zp10_MChi500.root","ZpBaryonic_Zp10_MChi500",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"ZpBaryonic_Zp10_MChi50.root","ZpBaryonic_Zp10_MChi50",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);


            std::cout<<"Make Higgs Shape"<<std::endl;
            tmp= createHistogramAndShifts(dir_+"Zprime600A300h.root","Zprime600A300",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            std::cout<<"Make Higgs Shape 600300: "<<tmp.first <<std::endl;
            tmp= createHistogramAndShifts(dir_+"Zprime600A300h.root","ZprimeA300Z600",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime600A400h.root","Zprime600A400",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime600A400h.root","ZprimeA400Z600",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            //800 histos
            tmp= createHistogramAndShifts(dir_+"Zprime800A300h.root","Zprime800A300",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime800A400h.root","Zprime800A400",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime800A500h.root","Zprime800A500",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime800A600h.root","Zprime800A600",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime800A300h.root","ZprimeA300Z800",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime800A400h.root","ZprimeA400Z800",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime800A500h.root","ZprimeA500Z800",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime800A600h.root","ZprimeA600Z800",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);

            //1000 histos
            tmp= createHistogramAndShifts(dir_+"Zprime1000A300h.root","Zprime1000A300",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1000A400h.root","Zprime1000A400",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1000A500h.root","Zprime1000A500",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1000A600h.root","Zprime1000A600",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1000A700h.root","Zprime1000A700",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1000A800h.root","Zprime1000A800",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1000A300h.root","ZprimeA300Z1000",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1000A400h.root","ZprimeA400Z1000",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1000A500h.root","ZprimeA500Z1000",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1000A600h.root","ZprimeA600Z1000",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1000A700h.root","ZprimeA700Z1000",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1000A800h.root","ZprimeA800Z1000",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            //1200 histos
            tmp= createHistogramAndShifts(dir_+"Zprime1200A300h.root","ZprimeA300Z1200",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1200A400h.root","ZprimeA400Z1200",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1200A500h.root","ZprimeA500Z1200",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1200A600h.root","ZprimeA600Z1200",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1200A700h.root","ZprimeA700Z1200",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1200A800h.root","ZprimeA800Z1200",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1200A300h.root","Zprime1200A300",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1200A400h.root","Zprime1200A400",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1200A500h.root","Zprime1200A500",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1200A600h.root","Zprime1200A600",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1200A700h.root","Zprime1200A700",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1200A800h.root","Zprime1200A800",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            //1400 histos
            tmp= createHistogramAndShifts(dir_+"Zprime1400A300h.root","Zprime1400A300",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            std::cout<<"Make Higgs Shape 1400300: "<<tmp.first <<std::endl;
            tmp= createHistogramAndShifts(dir_+"Zprime1400A400h.root","Zprime1400A400",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1400A500h.root","Zprime1400A500",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1400A600h.root","Zprime1400A600",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1400A700h.root","Zprime1400A700",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1400A800h.root","Zprime1400A800",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1400A300h.root","ZprimeA300Z1400",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1400A400h.root","ZprimeA400Z1400",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1400A500h.root","ZprimeA500Z1400",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1400A600h.root","ZprimeA600Z1400",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1400A700h.root","ZprimeA700Z1400",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1400A800h.root","ZprimeA800Z1400",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);


            //1700 histos
            tmp= createHistogramAndShifts(dir_+"Zprime1700A300h.root","Zprime1700A300",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1700A400h.root","Zprime1700A400",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1700A500h.root","Zprime1700A500",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1700A600h.root","Zprime1700A600",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1700A700h.root","Zprime1700A700",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1700A800h.root","Zprime1700A800",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1700A300h.root","ZprimeA300Z1700",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1700A400h.root","ZprimeA400Z1700",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1700A500h.root","ZprimeA500Z1700",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1700A600h.root","ZprimeA600Z1700",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1700A700h.root","ZprimeA700Z1700",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime1700A800h.root","ZprimeA800Z1700",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);



            //2000 histos
            tmp= createHistogramAndShifts(dir_+"Zprime2000A300h.root","Zprime2000A300",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime2000A400h.root","Zprime2000A400",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime2000A500h.root","Zprime2000A500",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime2000A600h.root","Zprime2000A600",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime2000A700h.root","Zprime2000A700",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime2000A800h.root","Zprime2000A800",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime2000A300h.root","ZprimeA300Z2000",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime2000A400h.root","ZprimeA400Z2000",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime2000A500h.root","ZprimeA500Z2000",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime2000A600h.root","ZprimeA600Z2000",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime2000A700h.root","ZprimeA700Z2000",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime2000A800h.root","ZprimeA800Z2000",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);

            //2500 histos
            tmp= createHistogramAndShifts(dir_+"Zprime2500A300h.root","Zprime2500A300",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime2500A400h.root","Zprime2500A400",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime2500A500h.root","Zprime2500A500",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime2500A600h.root","Zprime2500A600",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime2500A700h.root","Zprime2500A700",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime2500A800h.root","Zprime2500A800",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime2500A300h.root","ZprimeA300Z2500",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime2500A400h.root","ZprimeA400Z2500",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime2500A500h.root","ZprimeA500Z2500",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime2500A600h.root","ZprimeA600Z2500",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime2500A700h.root","ZprimeA700Z2500",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
            tmp= createHistogramAndShifts(dir_+"Zprime2500A800h.root","ZprimeA800Z2500",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*BR,prefix);
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

            weight_ = weight_+"*"+bTagSF;
            std::cout<<"weight: "<<weight_<<std::endl;

            BkgOutput output;    

            float leg1Corr=1.0;
            if(muID_!=0) leg1Corr*=muID_;
            if(eleID_!=0) leg1Corr*=eleID_;

            printf("Tau ID Scale Factor is %.3f \n",tauID_);

            std::cout<<"Select DATA"<<std::endl;
            std::pair<float,float> dataY         = createHistogramAndShifts(dataFile_,"data_obs","("+preSelection+"&&"+trigSelectionDATA_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+blinding_+")",scaleUp_,prefix);
            output.DATA = dataY.first;
            std::cout<<"      DATA Yield: "<< output.DATA<<std::endl;
            std::cout<<"      DATA Selection: "<<preSelection<<"&&"<<trigSelectionDATA_<<"&&"<<osSignalSelection_<<"&&"<<categorySelection<<std::endl; 


            std::cout<<"Create DiBoson"<<std::endl;
            //Create Diboson
            std::pair<float,float> vvYield      = createHistogramAndShifts(vvFile_,"VV",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")*"+weight_),luminosity_*leg1Corr*tauID_,prefix);
            std::cout<<"      VV before error inflation: "<<vvYield.first<<std::endl;
            std::pair<float,float> vvInflYield  = inflateError(vvYield,vvErr_);
            printf("      Diboson events in signal region = %f + %f \n",vvInflYield.first,vvInflYield.second);
            std::cout<<"      VV after error inflation: "<<vvInflYield.first<<std::endl;
            output.VV  = vvInflYield.first;
            output.dVV = vvInflYield.second;

            //Create Znunu
            std::pair<float,float> zvvYield      = createHistogramAndShifts(zvvFile_,"ZVV",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*leg1Corr*tauID_,prefix);
            printf("      ZVV events before inflation = %f + %f \n",zvvYield.first,zvvYield.second);
            std::pair<float,float> zvvInflYield  = inflateError(zvvYield,vvErr_); //FIXME
            printf("      ZVV events in signal region = %f + %f \n",zvvInflYield.first,zvvInflYield.second);
            output.ZVV  = zvvInflYield.first;
            output.dZVV = zvvInflYield.second;

            //Create Znunu
            std::pair<float,float> ewkYield      = createHistogramAndShifts(ewkFile_,"EWK",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*leg1Corr*tauID_,prefix);
            printf("      EWK events before inflation = %f + %f \n",ewkYield.first,ewkYield.second);
            std::pair<float,float> ewkInflYield  = inflateError(ewkYield,vvErr_); //FIXME
            printf("      EWK events in signal region = %f + %f \n",ewkInflYield.first,ewkInflYield.second);
            output.EWK  = ewkInflYield.first;
            output.EWK = ewkInflYield.second;


            //Create ZL and ZJ

            std::cout<<"Create ZLFT with factor "<<zlftFactor_<<std::endl;
            //ZL Yield
            std::pair<float,float> zlftYield   = createHistogramAndShifts(zllFile_,"ZLTmp",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection_+"&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_+"*"+zlftFactor_),luminosity_*leg1Corr*zttScale_,prefix);
            //ZLShape
            //std::pair<float,float> zlftShape   = createHistogramAndShifts(zllFile_,"ZL",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+relaxedSelection+"&&"+ZLFT_genLSel_+")*"+weight_),luminosity_*leg1Corr**zttScale_,prefix);
            std::pair<float,float> zlftShape   = createHistogramAndShifts(zllFile_,"ZL",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_+"*"+zlftFactor_),luminosity_*leg1Corr*zttScale_,prefix);
            std::pair<float,float> zlftShapeUp   = createHistogramAndShifts(zllFile_,"ZL_CMS_xtt_dyShape_13TeVUp",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_+"*"+Zweight_+"*"+zlftFactor_),luminosity_*leg1Corr*zttScale_,prefix);
            std::pair<float,float> zlftShapeDown   = createHistogramAndShifts(zllFile_,"ZL_CMS_xtt_dyShape_13TeVDown",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")*"+weight_+"*"+zlftFactor_),luminosity_*leg1Corr*zttScale_,prefix);

            std::pair<float,float> zlftInflYield  = inflateError(zlftYield,zlftErr_);
            printf("      Z (l->tau) in signal region = %f + %f \n",zlftInflYield.first,zlftInflYield.second);

            renormalizeHistogram(filelabel_+prefix,"ZL",zlftInflYield.first);
            output.ZLFT  = zlftInflYield.first;
            output.dZLFT  = zlftInflYield.second;

            if(shifts_.size()>0&&channel_=="eleTau"){
                std::pair<float,float> zlShiftUp      = createHistogramShifted(zllFile_,"ZL_CMS_xtt_ZLScale_etau_13TeVUp",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection_+"&&abs(pdg2)==11&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_),"1.02",luminosity_*leg1Corr*zttScale_*zExtrap,prefix);
                std::pair<float,float> zlShiftDown     = createHistogramShifted(zllFile_,"ZL_CMS_xtt_ZLScale_etau_13TeVDown",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection_+"&&abs(pdg2)==11&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_),"0.98",luminosity_*leg1Corr*zttScale_*zExtrap,prefix);
            }
            else if(shifts_.size()>0&&channel_=="muTau"){
                std::pair<float,float> zlShiftUp      = createHistogramShifted(zllFile_,"ZL_CMS_xtt_ZLScale_mutau_13TeVUp",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection_+"&&abs(pdg2)==13&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_),"1.02",luminosity_*leg1Corr*zttScale_*zExtrap,prefix);
                std::pair<float,float> zlShiftDown     = createHistogramShifted(zllFile_,"ZL_CMS_xtt_ZLScale_mutau_13TeVDown",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection_+"&&abs(pdg2)==13&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_),"0.98",luminosity_*leg1Corr*zttScale_*zExtrap,prefix);
            }

            std::cout<<"Create ZJFT"<<std::endl;
            //ZJ Yield
            std::pair<float,float> zjftYield      = createHistogramAndShifts(zllFile_,"ZJTmp",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection_+"&&"+ZJFT_genLReject_+")*"+weight_+"*"+Zweight_),luminosity_*leg1Corr*zttScale_,prefix);    

            //ZJ Shape
            //std::pair<float,float> zjftShape      = createHistogramAndShifts(zllFile_,"ZJ",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+relaxedSelection+"&&"+ZJFT_genLReject_+")*"+weight_),luminosity_*leg1Corr*zttScale_,prefix);    
            std::pair<float,float> zjftShape      = createHistogramAndShifts(zllFile_,"ZJ",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZJFT_genLReject_+")*"+weight_+"*"+Zweight_),luminosity_*leg1Corr*zttScale_,prefix);    
            std::pair<float,float> zjftShapeUp      = createHistogramAndShifts(zllFile_,"ZJ_CMS_xtt_dyShape_13TeVUp",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZJFT_genLReject_+")*"+weight_+"*"+Zweight_+"*"+Zweight_),luminosity_*leg1Corr*zttScale_,prefix);    
            std::pair<float,float> zjftShapeDown      = createHistogramAndShifts(zllFile_,"ZJ_CMS_xtt_dyShape_13TeVDown",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZJFT_genLReject_+")*"+weight_),luminosity_*leg1Corr*zttScale_,prefix);    


            std::cout<<"Create ZJFT 3"<<std::endl;
            std::pair<float,float> zjftFTShapeUp      = createHistogramAndShifts(zllFile_,"ZJ_CMS_xtt_jetToTauFake_13TeVUp","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZJFT_genLReject_+")*"+weight_+"*"+Zweight_+"*fakeTauEffiUp2",luminosity_*leg1Corr*zttScale_,prefix);    
            std::pair<float,float> zjftFTShapeDown      = createHistogramAndShifts(zllFile_,"ZJ_CMS_xtt_jetToTauFake_13TeVDown","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZJFT_genLReject_+")*"+weight_+"*"+Zweight_+"*fakeTauEffiDown2",luminosity_*leg1Corr*zttScale_,prefix);    


            std::pair<float,float> zjftInflYield  = inflateError(zjftYield,zjftErr_);
            printf("      Z (j->tau) in signal region = %f + %f \n",zjftInflYield.first,zjftInflYield.second);
            renormalizeHistogram(filelabel_+prefix,"ZJ",zjftInflYield.first);
            renormalizeHistogram(filelabel_+prefix,"ZJ_CMS_xtt_jetToTauFake_13TeVUp",zjftInflYield.first);
            renormalizeHistogram(filelabel_+prefix,"ZJ_CMS_xtt_jetToTauFake_13TeVDown",zjftInflYield.first);
            output.ZJFT  = zjftInflYield.first;
            output.dZJFT = zjftInflYield.second;


            output.ZJFT  = zjftInflYield.first;
            output.dZJFT = zjftInflYield.second;

            std::cout<<"Create ZTT"<<std::endl;
            //TODO: Check me, previous Btag ZTT shape correction had a special normalization method
            if(!runZTT(preSelection, prefix, zShape, topExtrap, output, categorySelection)){
                std::cout<<"Error Creating Ztt"<<std::endl;
                return output;
            }

            std::cout<<"Create SMH"<<std::endl;
            if(!runSMH(preSelection, prefix, zShape, topExtrap, output, categorySelection)){
                std::cout<<"Error Creating SMH"<<std::endl;
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
            if(!runW(preSelection, prefix, zShape, topExtrap, output, categorySelection, relaxedSelection, wSel)){
                std::cout<<"Error Creating W"<<std::endl;
                return output;
            }

            std::cout<<"Create QCD"<<std::endl;
            //Create QCD
            if(!runQCD(preSelection, prefix, zShape, topExtrap, output, categorySelection, relaxedSelection)){
                std::cout<<"Error Creating QCD"<<std::endl;
                return output;
            }

            std::cout<<"Does QCD have negative bins?"<<std::endl;
            FixNegativeBinsMC(filelabel_+prefix,"QCD",true);
            std::cout<<"Does VV have negative bins?"<<std::endl;
            FixNegativeBinsMC(filelabel_+prefix,"VV",true);
            std::cout<<"Does ZTT have negative bins?"<<std::endl;
            FixNegativeBinsMC(filelabel_+prefix,"ZTT",true);
            std::cout<<"Does SMH have negative bins?"<<std::endl;
            FixNegativeBinsMC(filelabel_+prefix,"SMH",true);
            std::cout<<"Does ZVV have negative bins?"<<std::endl;
            FixNegativeBinsMC(filelabel_+prefix,"ZVV",true);
            std::cout<<"Does W have negative bins?"<<std::endl;
            FixNegativeBinsMC(filelabel_+prefix,"W",true);
            std::cout<<"Does TTT have negative bins?"<<std::endl;
            FixNegativeBinsMC(filelabel_+prefix,"TTT",true);
            std::cout<<"Does TTJ have negative bins?"<<std::endl;
            FixNegativeBinsMC(filelabel_+prefix,"TTJ",true);
            std::cout<<"Does TT_Up have negative bins?"<<std::endl;
            FixNegativeBins(filelabel_+prefix, "TT_CMS_xtt_ttbarShape_13TeVUp", true);
            FixNegativeBins(filelabel_+prefix, "TTT_CMS_xtt_ttbarShape_13TeVUp", true);
            FixNegativeBins(filelabel_+prefix, "TTJ_CMS_xtt_ttbarShape_13TeVUp", true);
            std::cout<<"Does TT_Down have negative bins?"<<std::endl;
            FixNegativeBins(filelabel_+prefix, "TT_CMS_xtt_ttbarShape_13TeVDown", true);
            FixNegativeBins(filelabel_+prefix, "TTT_CMS_xtt_ttbarShape_13TeVDown", true);
            FixNegativeBins(filelabel_+prefix, "TTJ_CMS_xtt_ttbarShape_13TeVDown", true);

            std::cout<<"Does ZTT_Up have negative bins?"<<std::endl;
            FixNegativeBins(filelabel_+prefix, "ZTT_CMS_xtt_dyShape_13TeVUp", true);
            std::cout<<"Does ZTT_Up have negative bins?"<<std::endl;
            FixNegativeBins(filelabel_+prefix, "ZTT_CMS_xtt_dyShape_13TeVDown", true);
            std::cout<<"Does ZL_Up have negative bins?"<<std::endl;
            FixNegativeBins(filelabel_+prefix, "ZL_CMS_xtt_dyShape_13TeVUp", true);
            std::cout<<"Does ZL_Down have negative bins?"<<std::endl;
            FixNegativeBins(filelabel_+prefix, "ZL_CMS_xtt_dyShape_13TeVDown", true);
            std::cout<<"Does ZJ_Up have negative bins?"<<std::endl;
            FixNegativeBins(filelabel_+prefix, "ZJ_CMS_xtt_dyShape_13TeVUp", true);
            std::cout<<"Does ZJ_Down have negative bins?"<<std::endl;
            FixNegativeBins(filelabel_+prefix, "ZJ_CMS_xtt_dyShape_13TeVDown", true);

            std::cout<<"Does ZJ JFT UP have negative bins?"<<std::endl;
            FixNegativeBins(filelabel_+prefix, "ZJ_CMS_xtt_jetToTauFake_13TeVUp", true);
            std::cout<<"Does ZJ JFT Down have negative bins?"<<std::endl;
            FixNegativeBins(filelabel_+prefix, "ZJ_CMS_xtt_jetToTauFake_13TeVDown", true);

            std::cout<<"Does TTJ JFT Up have negative bins?"<<std::endl;
            FixNegativeBins(filelabel_+prefix, "TTJ_CMS_xtt_jetToTauFake_13TeVUp", true);
            std::cout<<"Does TTJ JFT Down have negative bins?"<<std::endl;
            FixNegativeBins(filelabel_+prefix, "TTJ_CMS_xtt_jetToTauFake_13TeVDown", true);

            std::cout<<"Does W JFT Up have negative bins?"<<std::endl;
            FixNegativeBins(filelabel_+prefix, "W_CMS_xtt_jetToTauFake_13TeVUp", true);
            std::cout<<"Does W JFT Down have negative bins?"<<std::endl;
            FixNegativeBins(filelabel_+prefix, "W_CMS_xtt_jetToTauFake_13TeVDown", true);




            std::cout<<std::endl;
            std::cout<<"=============Data Yields============="<<std::endl;
            std::cout<<"DATA: "<< output.DATA<<std::endl;
            std::cout<<std::endl;


            std::cout<<std::endl;
            std::cout<<"BKGD Yields "<<std::endl;
            std::cout<<"QCD: "<< output.QCD<<std::endl;
            std::cout<<"EWK: "<< output.EWK<<std::endl;
            std::cout<<"W: "<< output.W<<std::endl;
            std::cout<<"TOP: "<< output.TOP<<std::endl;
            std::cout<<"SMH: "<< output.SMH<<std::endl;
            std::cout<<"VV: "<< output.VV<<std::endl;
            std::cout<<"ZVV: "<< output.ZVV<<std::endl;
            std::cout<<"ZLFT: "<< output.ZLFT<<std::endl;
            std::cout<<"ZJFT: "<< output.ZJFT<<std::endl;
            std::cout<<"ZTT: "<< output.ZTT<<std::endl;


            float background    = output.QCD  + output.W  + output.TOP  + output.VV +output.ZVV + output.ZLFT  + output.ZJFT  + output.ZTT;
            float backgroundErr = sqrt( pow(output.dQCD,2) + pow(output.dW,2) + pow(output.dTOP,2) + pow(output.dVV,2) + pow(output.dZLFT,2) + pow(output.dZJFT,2) + pow(output.dZTT,2)) + pow(output.dZVV,2);
            printf("BACKGROUND=%f +-%f \n",background,backgroundErr);

            ///LATEX->Here since we want it for the note add all errors , even those that will go separate in the datacard

            //printf("LATEX ------------------------------------\n");
            //printf("Total & %.2f & %.2f & %.2f & %.2f \\\\ \n", dataYield.first, dataYieldSdb.first, dataSSYield.first, dataSSYieldSdb.first);
            //printf("Di-Boson & %.2f $\\pm$ %.2f & %.2f $\\pm$ %.2f & - & - \\\\ \n", vvInflYield.first, quadrature(vvInflYield.first,vvInflYield.second,muIDErr_,eleIDErr_,zttScaleErr_,tauIDErr_), vvInflYieldSdb.first, quadrature(vvInflYieldSdb.first,vvInflYieldSdb.second,muIDErr_,eleIDErr_,tauIDErr_));
            //printf("$t\\bar{t}$ & %.2f $\\pm$ %.2f & %.2f $\\pm$ %.2f & - & - \\\\ \n", topInflYield.first,quadrature(topInflYield.first,topInflYield.second,muIDErr_,eleIDErr_,tauIDErr_), topInflYieldSdb.first, quadrature(topInflYieldSdb.first,topInflYield.second,muIDErr_,eleIDErr_,tauIDErr_));
            //printf("$Z^{l+jet}$ & %.2f $\\pm$ %.2f & - & %.2f $\\pm$ %.2f & - \\\\ \n", zjftInflYield.first, quadrature(zjftInflYield.first,zjftInflYield.second,muIDErr_,eleIDErr_,zttScaleErr_), zjftInflSSYield.first,quadrature(zjftInflSSYield.first,zjftInflSSYield.second,muIDErr_,eleIDErr_,zttScaleErr_));
            //printf("$Z^{ll}$ & %.2f $\\pm$ %.2f & - & %.2f $\\pm$ %.2f & - \\\\ \n", zlftInflYield.first, quadrature(zlftInflYield.first,zlftInflYield.second,muIDErr_,eleIDErr_,zttScaleErr_),zlftInflSSYield.first,quadrature(zlftInflSSYield.first,zlftInflSSYield.second,muIDErr_,eleIDErr_,zttScaleErr_));
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



            float topError,zttError, zjftError, zlftError, wError, qcdError, vvError, zvvError,smhError, ewkError;
            topError= sqrt(pow(quadrature(output.TOP,  output.dTOP,  muIDErr_,eleIDErr_,tauIDErr_,luminosityErr_),2));
            zttError= sqrt(pow(quadrature(output.ZTT,output.dZTT,muIDErr_,eleIDErr_,zttScaleErr_,tauIDErr_,luminosityErr_),2));
            zlftError= sqrt(pow(quadrature(output.ZLFT,output.dZLFT,muIDErr_,eleIDErr_,zttScaleErr_,zlftErr_,tauIDErr_,luminosityErr_),2));
            zjftError= sqrt(pow(quadrature(output.ZJFT,output.dZJFT,muIDErr_,eleIDErr_,zttScaleErr_,zjftErr_,tauIDErr_,luminosityErr_),2));
            wError= sqrt(pow(quadrature(output.W,output.dW,muIDErr_,eleIDErr_,wFactorErr_,tauIDErr_,luminosityErr_),2));
            qcdError= sqrt(pow(output.dQCD,2));
            smhError = sqrt(pow(quadrature(output.SMH,output.dSMH,muIDErr_,eleIDErr_,smhErr_,tauIDErr_),2));
            vvError = sqrt(pow(quadrature(output.VV,output.dVV,muIDErr_,eleIDErr_,vvErr_,tauIDErr_),2));
            zvvError = sqrt(pow(quadrature(output.ZVV,output.dZVV,muIDErr_,eleIDErr_,zttScaleErr_,tauIDErr_),2));
            ewkError = sqrt(pow(quadrature(output.EWK,output.dEWK,muIDErr_,eleIDErr_,zttScaleErr_,tauIDErr_),2));
            //create a histogram with the error for plotting reasons and only
            TH1F *err = new TH1F("BKGErr","",1,0,1);
            err->SetBinContent(1,fullBackgroundErr/background);
            fout_->cd((filelabel_+prefix).c_str());
            err->Write();

            TH1F *toperr = new TH1F("TOPBKGErr","",1,0,1);
            toperr->SetBinContent(1,topError/output.TOP);
            fout_->cd((filelabel_+prefix).c_str());
            toperr->Write();

            TH1F *smherr = new TH1F("SMHBKGErr","",1,0,1);
            smherr->SetBinContent(1,smhError/output.SMH);
            fout_->cd((filelabel_+prefix).c_str());
            smherr->Write();

            TH1F *werr = new TH1F("WBKGErr","",1,0,1);
            werr->SetBinContent(1,wError/output.W);
            fout_->cd((filelabel_+prefix).c_str());
            werr->Write();

            TH1F *qcderr = new TH1F("QCDBKGErr","",1,0,1);
            qcderr->SetBinContent(1,qcdError/output.QCD);
            if (output.QCD <0.0001) qcderr->SetBinContent(1,qcdError);
            fout_->cd((filelabel_+prefix).c_str());
            qcderr->Write();


            TH1F *vverr = new TH1F("VVBKGErr","",1,0,1);
            vverr->SetBinContent(1,vvError/output.VV);
            fout_->cd((filelabel_+prefix).c_str());
            vverr->Write();


            TH1F *ztterr = new TH1F("ZTTBKGErr","",1,0,1);
            ztterr->SetBinContent(1,zttError/output.ZTT);
            fout_->cd((filelabel_+prefix).c_str());
            ztterr->Write();

            TH1F *zlfterr = new TH1F("ZLFTBKGErr","",1,0,1);
            zlfterr->SetBinContent(1,zlftError/output.ZLFT);
            fout_->cd((filelabel_+prefix).c_str());
            zlfterr->Write();

            TH1F *zjfterr = new TH1F("ZJFTBKGErr","",1,0,1);
            zjfterr->SetBinContent(1,zjftError/output.ZJFT);
            fout_->cd((filelabel_+prefix).c_str());
            zjfterr->Write();

            TH1F *zvverr = new TH1F("ZVVBKGErr","",1,0,1);
            zvverr->SetBinContent(1,zvvError/output.ZVV);
            if (output.ZVV <0.0001) qcderr->SetBinContent(1,zvvError);
            fout_->cd((filelabel_+prefix).c_str());
            zvverr->Write();


            TH1F *ewkerr = new TH1F("EWKBKGErr","",1,0,1);
            ewkerr->SetBinContent(1,ewkError/output.EWK);
            if (output.EWK <0.0001) qcderr->SetBinContent(1,ewkError);
            fout_->cd((filelabel_+prefix).c_str());
            ewkerr->Write();




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


            std::pair<float,float> ztt  = createHistogramAndShifts(zttFile_,"ZTT",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZTT_genTauSel_+")*"+weight_+"*"+Zweight_),luminosity_*zttScale_*leg1Corr*tauID_,prefix);
            std::pair<float,float> zttDyUp  = createHistogramAndShifts(zttFile_,"ZTT_CMS_xtt_dyShape_13TeVUp",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZTT_genTauSel_+")*"+weight_+"*"+Zweight_+"*"+Zweight_),luminosity_*zttScale_*leg1Corr*tauID_,prefix);
            std::pair<float,float> zttDyDown  = createHistogramAndShifts(zttFile_,"ZTT_CMS_xtt_dyShape_13TeVDown",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+ZTT_genTauSel_+")*"+weight_),luminosity_*zttScale_*leg1Corr*tauID_,prefix);

            std::cout<<"      ZTT Selection: "<<preSelection<<"&&"<<trigSelection_<<"&&"<<osSignalSelection_<<"&&"<<categorySelection<<std::endl;
            std::cout<<"      ZTT Selection GenTau Se: "<<preSelection<<"&&"<<trigSelection_<<"&&"<<osSignalSelection_<<"&&"<<categorySelection<<std::endl;
            output.ZTT  = ztt.first;
            output.dZTT = ztt.second;

            printf("      Selection ZTT events in signal region = %f + %f \n",ztt.first,ztt.second);
            return true;
        }


        bool runSMH(std::string preSelection,std::string prefix,std::string zShape, float topExtrap, BkgOutput &output,
                std::string categorySelection = "pt_1>-100", std::string relaxedSelection = "pt_1>-100"){

            float leg1Corr=1.0;
            if(muID_!=0) leg1Corr*=muID_;
            if(eleID_!=0) leg1Corr*=eleID_;


            std::pair<float,float> smh  = createHistogramAndShifts(zhFile_,"SMH",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")*"+weight_),luminosity_*leg1Corr*tauID_,prefix);

            std::cout<<"      SMH Selection: "<<preSelection<<"&&"<<trigSelection_<<"&&"<<osSignalSelection_<<"&&"<<categorySelection<<std::endl;
            output.SMH  = smh.first;
            output.dSMH = smh.second;

            printf("      Selection SMH events in signal region = %f + %f \n",smh.first,smh.second);
            return true;
        }



        bool runTOP( std::string preSelection, std::string prefix, std::string zShape, float topExtrap, BkgOutput &output, 
                std::string categorySelection, std::string relaxedSelection ) {
            std::string genTauSel_="gen_match_2==5";
            std::string genTauRej_="gen_match_2!=5";

            if (channel_=="diTau"){
                genTauSel_ = "genTaus==2"; //Zttyield
                genTauRej_  = "genTaus!=2";
            }

            std::cout<<"Gen Tau Selection: "<<genTauSel_<<"    Gen Tau Rejection: "<<genTauRej_<<std::endl;
            std::cout<<"top weight: "<<TTweight_<<std::endl;


            float leg1Corr=1.0;
            if(muID_!=0) leg1Corr*=muID_;
            if(eleID_!=0) leg1Corr*=eleID_;

            std::pair<float,float> topYield      = createHistogramAndShifts(topFile_,"TT",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")*"+weight_), luminosity_*leg1Corr*tauID_*topExtrap, prefix);

            printf("      TTbar events in before inflation region = %f + %f \n",topYield.first,topYield.second);

            std::pair<float,float> topShapeUp      = createHistogramAndShifts(topFile_,"TT_CMS_xtt_ttbarShape_13TeVUp",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")*"+weight_+"*"+TTweight_), luminosity_*leg1Corr*tauID_*topExtrap, prefix);
            std::pair<float,float> topShapeDown      = createHistogramAndShifts(topFile_,"TT_CMS_xtt_ttbarShape_13TeVDown",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")*"+weight_), luminosity_*leg1Corr*tauID_*topExtrap, prefix);

            std::pair<float,float> topInflYield  = inflateError(topYield,topErr_);


            std::pair<float,float> topJShape      = createHistogramAndShifts(topFile_,"TTJ","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+genTauRej_+")*"+weight_, luminosity_*leg1Corr*tauID_*topExtrap, prefix);
            printf("      TTJ events in before inflation region = %f + %f \n",topJShape.first,topJShape.second);

            std::pair<float,float> topJShapeUp      = createHistogramAndShifts(topFile_,"TTJ_CMS_xtt_ttbarShape_13TeVUp","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+genTauRej_+")*"+weight_+"*"+TTweight_, luminosity_*leg1Corr*tauID_*topExtrap, prefix);
            std::pair<float,float> topJShapeDown      = createHistogramAndShifts(topFile_,"TTJ_CMS_xtt_ttbarShape_13TeVDown","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+genTauRej_+")*"+weight_, luminosity_*leg1Corr*tauID_*topExtrap, prefix);

            std::pair<float,float> topJFTShapeUp      = createHistogramAndShifts(topFile_,"TTJ_CMS_xtt_jetToTauFake_13TeVUp","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+genTauRej_+")*"+weight_+"*fakeTauEffiUp2", luminosity_*leg1Corr*tauID_*topExtrap, prefix);
            std::pair<float,float> topJFTShapeDown      = createHistogramAndShifts(topFile_,"TTJ_CMS_xtt_jetToTauFake_13TeVDown","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+genTauRej_+")*"+weight_+"*fakeTauEffiDown2", luminosity_*leg1Corr*tauID_*topExtrap, prefix);

            renormalizeHistogram(filelabel_+prefix,"TTJ_CMS_xtt_jetToTauFake_13TeVUp",topJShape.first);
            renormalizeHistogram(filelabel_+prefix,"TTJ_CMS_xtt_jetToTauFake_13TeVDown",topJShape.first);


            std::pair<float,float> topTShape      = createHistogramAndShifts(topFile_,"TTT","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+genTauSel_+")*"+weight_, luminosity_*leg1Corr*tauID_*topExtrap, prefix);
            printf("      TTT events in before inflation region = %f + %f \n",topTShape.first,topTShape.second);

            std::pair<float,float> topTShapeUp      = createHistogramAndShifts(topFile_,"TTT_CMS_xtt_ttbarShape_13TeVUp","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+genTauSel_+")*"+weight_+"*"+TTweight_, luminosity_*leg1Corr*tauID_*topExtrap, prefix);
            std::pair<float,float> topTShapeDown      = createHistogramAndShifts(topFile_,"TTT_CMS_xtt_ttbarShape_13TeVDown","("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+"&&"+genTauSel_+")*"+weight_, luminosity_*leg1Corr*tauID_*topExtrap, prefix);

            output.TOP  = topInflYield.first;
            output.dTOP = topInflYield.second;

            //Subtract the ztt ttbar overlap of 1.5% from the ttbar yield
            //not used for 2014 MSSM
            //output.TOP = output.TOP-(output.ZTT*0.015);   //only for  

            renormalizeHistogram(filelabel_+prefix,"TT",output.TOP);
            printf("      TTbar events in signal region = %f + %f \n",topInflYield.first,topInflYield.second);

            return true;}




        bool runQCD(std::string preSelection, std::string prefix, std::string zShape, float topExtrap, BkgOutput &output, std::string categorySelection, std::string relaxedSelection) 
        {
            float leg1Corr=1.0;
            if(muID_!=0) leg1Corr*=muID_;
            if(eleID_!=0) leg1Corr*=eleID_;
            std::pair<float,float> dataQCDShape = createHistogramAndShiftsQCD(dataFile_,"QCD","("+relaxedSelection+"&&"+trigSelectionDATA_+"&&"+ssSignalSelection_+"&&"+categorySelection+")",scaleUp_,prefix); 
            printf("      Data events in SS Signal QCDShape sideband region = %f + %f \n",dataQCDShape.first,dataQCDShape.second);
            std::pair<float,float> ZQCDShape = createHistogramAndShifts(zttFile_,"ZQCD","("+relaxedSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")*"+weight_+"*"+Zweight_,luminosity_,prefix); 
            printf("      Z events in SS Signal QCDShape sideband region = %f + %f \n",ZQCDShape.first,ZQCDShape.second);
            std::pair<float,float> TopQCDShape = createHistogramAndShifts(topFile_,"TopQCD","("+relaxedSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")*"+weight_+"*"+TTweight_,luminosity_,prefix); 
            printf("      TOP events in SS Signal QCDShape sideband region = %f + %f \n",TopQCDShape.first,TopQCDShape.second);
            std::pair<float,float> VVQCDShape       = createHistogramAndShifts(vvFile_,"VVQCD",("("+relaxedSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")*"+weight_),luminosity_,prefix);
            printf("      VV events in SS Signal QCDShape sideband region = %f + %f \n",VVQCDShape.first,VVQCDShape.second);

            std::pair<float,float> SMHQCDShape       = createHistogramAndShifts(zhFile_,"SMHQCD",("("+relaxedSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")*"+weight_),luminosity_,prefix);
            printf("      SMH events in SS Signal QCDShape sideband region = %f + %f \n",SMHQCDShape.first,SMHQCDShape.second);

            std::pair<float,float> WQCDLowShape       = createHistogramAndShifts(wFile_,"WQCD",("("+relaxedSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")*"+weight_+"*"+Vweight_),luminosity_*leg1Corr,prefix);



            std::cout<<"QCD Selections= "<<"("<<preSelection_<<"&&"<<trigSelectionDATA_<<"&&"<<ssSignalSelection_<<"&&"<<categorySelection<<")"<<std::endl;
            std::pair<float,float> dataQCD = createHistogramAndShifts(dataFile_,"QCDYield","("+preSelection+"&&"+trigSelectionDATA_+"&&"+ssSignalSelection_+"&&"+categorySelection+")",scaleUp_,prefix); 
            printf("      Data events in SS Signal QCD sideband region = %f + %f \n",dataQCD.first,dataQCD.second);
            std::pair<float,float> ZQCD = createHistogramAndShifts(zttFile_,"ZQCDYield","("+preSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")*"+weight_+"*"+Zweight_,luminosity_,prefix); 
            printf("      Z events in SS Signal QCD sideband region = %f + %f \n",ZQCD.first,ZQCD.second);
            std::pair<float,float> TopQCD = createHistogramAndShifts(topFile_,"TopQCDYield","("+preSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")*"+weight_+"*"+TTweight_,luminosity_,prefix); 
            printf("      TOP events in SS Signal QCD sideband region = %f + %f \n",TopQCD.first,TopQCD.second);
            std::pair<float,float> VVQCD       = createHistogramAndShifts(vvFile_,"VVQCDYield",("("+preSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")*"+weight_),luminosity_,prefix);

            std::pair<float,float> SMHQCD       = createHistogramAndShifts(zhFile_,"SMHQCDYield",("("+preSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")*"+weight_),luminosity_,prefix);
            printf("      SMH events in SS Signal QCD sideband region = %f + %f \n",SMHQCD.first,SMHQCD.second);

            std::pair<float,float> ssWLow       = createHistogramAndShifts(wFile_,"WQCDYield",("("+preSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")*"+weight_+"*"+Vweight_),luminosity_*leg1Corr,prefix);
            printf("      (MonteCarlo) W events in SS region = %f + %f \n",ssWLow.first,ssWLow.second);

            /*
             * DATADRIVEN W FOUND 
             */

            //Now subtracting off bkgd shapes from data ss shape
            subtractHistogram(filelabel_+prefix,"QCDYield","ZQCDYield");
            subtractHistogram(filelabel_+prefix,"QCDYield","VVQCDYield");
            subtractHistogram(filelabel_+prefix,"QCDYield","SMHQCDYield");
            subtractHistogram(filelabel_+prefix,"QCDYield","TopQCDYield");
            subtractHistogram(filelabel_+prefix,"QCDYield","WQCDYield");

            subtractHistogramShifts(filelabel_+prefix,"QCD","ZQCD");
            subtractHistogramShifts(filelabel_+prefix,"QCD","VVQCD");
            subtractHistogramShifts(filelabel_+prefix,"QCD","SMHQCD");
            subtractHistogramShifts(filelabel_+prefix,"QCD","TopQCD");
            subtractHistogramShifts(filelabel_+prefix,"QCD","WQCD");


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


            if(ssQCD.first<0) {
                ssQCD.first=0.0000001;
                ssQCD.second=1.8;
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

            //Now Renormalize
            renormalizeHistogram(filelabel_+prefix,"QCD",osQCD.first);

            printf("OS QCD in  core  =%f *%f = %f +- %f \n",ssQCD.first,qcdFactor_,osQCD.first,osQCD.second);

            output.QCD  = osQCD.first;    
            output.dQCD = osQCD.second;

            if(shifts_.size()>0){
                qcdSyst(filelabel_, prefix, "QCD", 1.2, 0.2);
            }

            std::cout<<"QCDSystematics made"<<std::endl;
            return true;


        }


        bool runW(std::string preSelection,std::string prefix,std::string zShape, float topExtrap, BkgOutput &output, std::string categorySelection, std::string relaxedSelection, std::string wSel) {


            //WShape is relaxed
            std::pair<float,float> wShape         = createHistogramAndShifts(wFile_,"W",("("+relaxedSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")*"+weight_+"*"+Vweight_),luminosity_,prefix);
            std::cout<<"Relaxed Selection: "<<relaxedSelection<<std::endl;
            std::pair<float,float> wYield         = createHistogramAndShifts(wFile_,"WYield",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")*"+weight_+"*"+Vweight_),luminosity_,prefix);
            std::cout<<"Straight W MC Yield: "<<wYield.first<<" stat error:" <<wYield.second<<std::endl;
            std::cout<<"Straight W MC Yield: "<<preSelection+"&&"<<trigSelection_<<"&&"<<categorySelection<<"&&"<<osSignalSelection_<<std::endl; 

            renormalizeHistogram(filelabel_+prefix,"W",wYield.first);

            std::pair<float,float> wShapeFTUp        = createHistogramAndShifts(wFile_,"W_CMS_xtt_jetToTauFake_13TeVUp","("+relaxedSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")*"+weight_+"*"+Vweight_+"*fakeTauEffiUp2",luminosity_,prefix);

            std::pair<float,float> wShapeFTDown        = createHistogramAndShifts(wFile_,"W_CMS_xtt_jetToTauFake_13TeVDown","("+relaxedSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")*"+weight_+"*"+Vweight_+"*fakeTauEffiDown2",luminosity_,prefix);

            renormalizeHistogram(filelabel_+prefix,"W_CMS_xtt_jetToTauFake_13TeVUp",wYield.first);
            renormalizeHistogram(filelabel_+prefix,"W_CMS_xtt_jetToTauFake_13TeVDown",wYield.first);


            std::pair<float,float> wWUp  = createHistogramAndShifts(wFile_,"W_CMS_xtt_wShape_13TeVUp",("("+relaxedSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")*"+weight_+"*"+Vweight_+"*"+Vweight_),luminosity_,prefix);
            std::pair<float,float> wWDown  = createHistogramAndShifts(wFile_,"W_CMS_xtt_wShape_13TeVDown",("("+relaxedSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")*"+weight_),luminosity_,prefix);

            std::pair<float,float> wWUpYield  = createHistogramAndShifts(wFile_,"W_CMS_xtt_wShape_13TeVUpTMP",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")*"+weight_+"*"+Vweight_+"*"+Vweight_),luminosity_,prefix);
            std::pair<float,float> wWDownYield  = createHistogramAndShifts(wFile_,"W_CMS_xtt_wShape_13TeVDownTMP",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")*"+weight_),luminosity_,prefix);


            renormalizeHistogram(filelabel_+prefix,"W_CMS_xtt_wShape_13TeVUp",wWUpYield.first);
            renormalizeHistogram(filelabel_+prefix,"W_CMS_xtt_wShape_13TeVDown",wWDownYield.first);



            output.W  = wYield.first;
            output.dW  = wYield.second;

            return true;
        }

        /*bool runW(std::string preSelection,std::string prefix,std::string zShape, float topExtrap, BkgOutput &output, std::string categorySelection, std::string relaxedSelection, std::string wSel) {

          float leg1Corr=1.0;
          if(muID_!=0) leg1Corr*=muID_;
          if(eleID_!=0) leg1Corr*=eleID_;

        //extractWfactor 
        //First get data in Sideband
        std::pair<float,float> dataYSdb     = createHistogramAndShifts(dataFile_,"data_obs_sdb","("+trigSelectionDATA_+"&&"+categorySelection+"&&"+osWSelection_+")",scaleUp_,prefix);
        std::pair<float,float> dataYieldSdb = convertToPoisson(dataYSdb);

        //then get ttbar in sideband
        std::pair<float,float> smhYieldSdb      = createHistogramAndShifts(zhFile_,"SMH_SDB",("("+trigSelection_+"&&"+categorySelection+"&&"+osWSelection_+")*"+weight_),luminosity_*leg1Corr*tauID_*topExtrap,prefix);
        std::pair<float,float> topYieldSdb      = createHistogramAndShifts(topFile_,"TT_SDB",("("+trigSelection_+"&&"+categorySelection+"&&"+osWSelection_+")*"+weight_),luminosity_*leg1Corr*tauID_*topExtrap,prefix);
        std::pair<float,float> topInflYieldSdb  = inflateError(topYieldSdb,topErr_);
        printf("TTbar events in sideband region = %f + %f \n",topYieldSdb.first,topInflYieldSdb.second);

        //Next get Diboson in sideband
        std::pair<float,float> vvYieldSdb      = createHistogramAndShifts(vvFile_,"VV_SDB",("("+trigSelection_+"&&"+categorySelection+"&&"+osWSelection_+")*"+weight_),luminosity_*leg1Corr*tauID_,prefix);
        std::pair<float,float> vvInflYieldSdb  = inflateError(vvYieldSdb,vvErr_);
        printf("Diboson events in sideband region = %f + %f \n",vvYieldSdb.first,vvInflYieldSdb.second);

        //finally get Z in sideband
        std::pair<float,float> zSDB        = createHistogramAndShifts(zttFile_,"Z_SDB",("("+trigSelection_+"&&"+categorySelection+"&&"+osWSelection_+")*"+weight_+"*"+Zweight_),luminosity_*zttScale_*leg1Corr*tauID_,prefix);
        std::pair<float,float> WSDB        = createHistogramAndShifts(wFile_,"W_SDB",("("+trigSelection_+"&&"+categorySelection+"&&"+osWSelection_+")*"+weight_+"*"+Vweight_),luminosity_*leg1Corr*tauID_,prefix);
        printf("Z events in sideband region = %f + %f \n",zSDB.first,zSDB.second);
        printf("W events in sideband region = %f + %f \n",WSDB.first,WSDB.second);

        //Run OS+LS + MT method
        printf("1. Subtract TTbar and diboson from sideband");
        std::pair<float,float> osWHigh = std::make_pair(TMath::Nint(dataYieldSdb.first
        -topInflYieldSdb.first
        -vvInflYieldSdb.first
        -smhYieldSdb.first
        //-WSDB.first
        -zSDB.first),
        sqrt(dataYieldSdb.second*dataYieldSdb.second
        +topInflYieldSdb.second*topInflYieldSdb.second
        +vvInflYieldSdb.second*vvInflYieldSdb.second
        +smhYieldSdb.second*smhYieldSdb.second
        +WSDB.second*WSDB.second
        +zSDB.second*zSDB.second));

        float WSF= osWHigh.first/WSDB.first;
        std::pair<float,float> wFactor = std::make_pair(WSF, osWHigh.first*osWHigh.second); 

        printf("OS W in sideband  =%f -%f -%f -%f  = %f +- %f \n",dataYieldSdb.first,topInflYieldSdb.first,vvInflYieldSdb.first,zSDB.first,osWHigh.first,osWHigh.second);

        std::pair<float,float> wYield         = createHistogramAndShifts(wFile_,"WTmp",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")*"+weight_+"*"+Vweight_),luminosity_*leg1Corr,prefix,false);
        std::pair<float,float> wShape         = createHistogramAndShifts(wFile_,"W",("("+relaxedSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")*"+weight_+"*"+Vweight_),luminosity_*leg1Corr,prefix,false);
        //std::pair<float,float> WShapeUp  = createHistogramAndShifts(wFile_,"W_CMS_xtt_wShape_13TeVUp",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")*"+weight_+"*"+Vweight_+"*"+Vweight_),luminosity_*leg1Corr*tauID_,prefix);
        std::pair<float,float> WShapeUp  = createHistogramAndShifts(wFile_,"W_CMS_xtt_wShape_13TeVUp",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")*"+weight_+"*"+Vweight_+"*"+Vweight_),luminosity_*leg1Corr*tauID_,prefix);
        std::pair<float,float> WShapeDown  = createHistogramAndShifts(wFile_,"W_CMS_xtt_wShape_13TeVDown",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")*"+weight_),luminosity_*leg1Corr*tauID_,prefix);



        printf("W extrapolation factor as measured in corrected MC = %f +- %f \n",wFactor.first,wFactor.second);

        //std::pair<float,float> wFactorUp = extractWFactor(wFile_,preSelection+"&&"+trigSelection_+"&&charge==0&&"+categorySelection+"",prefix,"WWeightUp");
        //std::pair<float,float> wFactorDown = extractWFactor(wFile_,preSelection+"&&"+trigSelection_+"&&charge==0&&"+categorySelection+"",prefix,"WWeightDown");

        //printf("2. Extrapolate W in the low MT region\n");
        // std::pair<float,float> osWLow = std::make_pair(osWHigh.first*wFactor.first,
        //        sqrt(osWHigh.first*osWHigh.first*wFactor.second*wFactor.second+osWHigh.second*osWHigh.second*wFactor.first*wFactor.first));

        //printf("OS W  in core  =%f *%f  = %f +- %f \n",osWHigh.first,wFactor.first,osWLow.first,osWLow.second);
        //renormalizeHistogram(filelabel_+prefix,"W",osWLow.first);

        //printf("OS W MC in signal reg  = %f +- %f \n", wYield.first, wYield.second);
        //std::pair<float,float> wMCYield       = createHistogramAndShifts(wFile_,"WTMP",("("+preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_+"&&"+categorySelection+")*"+weight_),luminosity_*leg1Corr,prefix);
        //
        output.W  = wYield.first;
        output.dW = wYield.second;
        return true;
}
*/
//-------------Helper Functions------------------

void addHistogramZTT(std::string folder, std::string histo1, std::string histo2)
{
    std::cout<<"In AddHistogramZTT"<<std::endl;
    addHistogram(folder,histo1,histo2);
    for(unsigned int i=0;i<shifts_.size();++i) {
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
    if(t==0) printf("No Tree Found in file %s\n",file.c_str());
    std::pair<float,float> yield;

    if(!keys)
        yield =makeHistogram(t,filelabel_+postfix,name,cut,scaleFactor);
    else
        yield =makeTHKeys(t,filelabel_+postfix,name,cut,scaleFactor);


    //now the shifts
    std::pair<float,float> tmpYield;
    for(unsigned int i=0;i<shifts_.size();++i) {

        std::string weightUp=weight_;
        std::string weightDown=weight_;
        //if (weightShift=="EffCSVWeight0"){ 
        //    weightUp=weight_+"*EffCSVWeight0Up";
        //    weightDown=weight_+"*EffCSVWeight0Down";
        //}
        TTree *ts=0;
        TTree *td=0;
        if (shifts_[i]=="MET"){
            ts= (TTree*)f->Get((channel_+"EventTree/eventTree").c_str());
            td= (TTree*)f->Get((channel_+"EventTree/eventTree").c_str());
        }
        else{
            ts= (TTree*)f->Get((channel_+"EventTree"+shifts_[i]+"Up/eventTree").c_str());
            td= (TTree*)f->Get((channel_+"EventTree"+shifts_[i]+"Down/eventTree").c_str());
        }


        if(ts!=0) {
            if(!keys)
                tmpYield = makeHistogram(ts,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",cut,scaleFactor,shifts_[i]+"Up");
            else
                tmpYield = makeTHKeys(ts,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",cut,scaleFactor);

            if(!normUC)
                scaleHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",yield.first/tmpYield.first);

        }
        if(td!=0) {
            if(!keys)
                tmpYield = makeHistogram(td,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",cut,scaleFactor,shifts_[i]+"Down");
            else
                tmpYield = makeTHKeys(td,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",cut,scaleFactor);

            if(!normUC)
                scaleHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",yield.first/tmpYield.first);

        }
    }
    f->Close();
    return yield;
}


std::pair<float,float> createHistogramAndShiftsFinal(std::string file,std::string name, std::string cut,float scaleFactor = 1, std::string postfix = "",bool normUC  = true, bool keys=false,bool ShapeUncertainty=true) {
    TFile *f  = new TFile(file.c_str());
    if(f==0) printf("Not file Found\n");
    //get the nominal tree first
    TTree *t= (TTree*)f->Get((channel_+"EventTreeFinal/eventTree").c_str());
    if(t==0) printf("No Tree Found in file %s\n",file.c_str());

    std::pair<float,float> yield;

    if(!keys)
        yield =makeHistogram(t,filelabel_+postfix,name,cut,scaleFactor);
    else
        yield =makeTHKeys(t,filelabel_+postfix,name,cut,scaleFactor);

    //now the shifts
    std::pair<float,float> tmpYield;
    for(unsigned int i=0;i<shifts_.size();++i) {
        TTree *ts=0;
        TTree *td=0;
        if (shifts_[i]=="MET"){
            ts= (TTree*)f->Get((channel_+"EventTreeFinal/eventTree").c_str());
            td= (TTree*)f->Get((channel_+"EventTreeFinal/eventTree").c_str());

        }
        else{
            ts= (TTree*)f->Get((channel_+"EventTreeFinal"+shifts_[i]+"Up/eventTree").c_str());
            td= (TTree*)f->Get((channel_+"EventTreeFinal"+shifts_[i]+"Down/eventTree").c_str());
        }

        if(ts!=0) {
            if(!keys)
                tmpYield = makeHistogram(ts,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",cut,scaleFactor,shifts_[i]+"Up");
            else
                tmpYield = makeTHKeys(ts,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",cut,scaleFactor);

            if(!normUC)
                scaleHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",yield.first/tmpYield.first);

        }

        if(td!=0) {
            if(!keys)
                tmpYield = makeHistogram(td,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",cut,scaleFactor,shifts_[i]+"Down");
            else
                tmpYield = makeTHKeys(td,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",cut,scaleFactor);

            if(!normUC)
                scaleHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",yield.first/tmpYield.first);

        }

    }
    if (!(name=="data_obs"||name=="data_obs_ss"||name=="data_obs_sdb"||name=="data_obs_ss_sdb"))
        FixNegativeBinsMC(filelabel_+postfix,name,false);

    f->Close();
    return yield;
}


std::pair<float,float> createHistogramAndShiftsQCD(std::string file,std::string name, std::string cut,float scaleFactor = 1, std::string postfix = "",bool normUC  = true, bool keys=false,bool ShapeUncertainty=true) {
    TFile *f  = new TFile(file.c_str());
    if(f==0) printf("Not file Found\n");
    //get the nominal tree first
    TTree *t= (TTree*)f->Get((channel_+"EventTree/eventTree").c_str());
    if(t==0) printf("No Tree Found in file %s\n",file.c_str());
    std::pair<float,float> yield;

    if(!keys)
        yield =makeHistogram(t,filelabel_+postfix,name,cut,scaleFactor);

    //now the shifts
    std::pair<float,float> tmpYield;
    for(unsigned int i=0;i<shifts_.size();++i) {

        tmpYield = makeHistogram(t,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",cut,scaleFactor);

        tmpYield = makeHistogram(t,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",cut,scaleFactor);

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

    for(unsigned int i=0;i<shifts_.size();++i) {
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

std::pair<float,float> makeHistogram(TTree* tree,std::string folder,std::string name,std::string cut,float scaleFactor = 1.,std::string shift = "none") {

    if(fout_->Get(folder.c_str())==0)
        fout_->mkdir(folder.c_str());
    TH1F *h=0;

    TString variableOld_=variable_;

    if (shift=="METUp"){
        if (variable_=="mt12") variable_="pfmtEnUp";
        else if (variable_=="met") variable_="pfmetEnUp";
    }
    else if (shift=="METDown"){
        if (variable_=="mt12") variable_="pfmtEnDown";
        else if (variable_=="met") variable_="pfmetEnDown";
    }

    if((name=="data_obs"||name=="data_obs_ss"||name=="data_obs_sdb"||name=="data_obs_ss_sdb"||name=="QCD")&&(variable_=="ptmtEnUp"||variable_=="ptmtEnDown")){
        variable_="mt12";
    }

    if((name=="data_obs"||name=="data_obs_ss"||name=="data_obs_sdb"||name=="data_obs_ss_sdb"||name=="QCD")&&(variable_=="ptmetEnUp"||variable_=="ptmetEnDown")){
        variable_="met";
    }



    if(binning_.size()==0)
        h= new TH1F(name.c_str(),name.c_str(),bins_,min_,max_);
    else 
        h = new TH1F(name.c_str(),name.c_str(),binning_.size()-1,&binning_[0]);
    h->Sumw2();

    tree->Draw((variable_+">>"+name).c_str(),cut.c_str());

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
        h->SetBinContent(1,0.00001);
    }
    h->ClearUnderflowAndOverflow();
    h->Write(h->GetName(),TObject::kOverwrite);

    return std::make_pair(yield,error);
}


void close() {
    fout_->Close();
}


std::pair<float,float> makeTHKeys(TTree* tree,std::string folder,std::string name,std::string cut,float scaleFactor = 1.) {

    if(fout_->Get(folder.c_str())==0)
        fout_->mkdir(folder.c_str());

    TH1Keys *h=0;

    if(binning_.size()==0)
        h= new TH1Keys("tmp","tmp",bins_,min_,max_,"a",0.5);
    else 
        h = new TH1Keys("tmp","tmp",binning_.size()-1,&binning_[0],"a",0.5);
    tree->Draw((variable_+">>tmp").c_str(),cut.c_str());

    if(h->Integral()==0.000) {
        printf("Keys for %s failed, retrying\n",name.c_str());
        delete h;
        if(binning_.size()==0)
            h= new TH1Keys("tmp","tmp",bins_,min_,max_,"a",0.5);
        else 
            h = new TH1Keys("tmp","tmp",binning_.size()-1,&binning_[0],"a",0.5);
        tree->Draw((variable_+">>tmp").c_str(),cut.c_str());

        if(h->Integral()==0.0) {
            printf("KEYS FAILED AGAIN \n");

        }
    }

    //now get the yield
    TH1F *hh=0;
    if(binning_.size()==0)
        hh= new TH1F("tmp2",name.c_str(),bins_,min_,max_);
    else 
        hh = new TH1F("tmp2",name.c_str(),binning_.size()-1,&binning_[0]);
    tree->Draw((variable_+">>tmp2").c_str(),cut.c_str());
    hh->Scale(scaleFactor);
    Double_t error=0.0;
    float yield = hh->IntegralAndError(1,hh->GetNbinsX(),error,"");

    fout_->cd(folder.c_str());
    TH1F * histo =(TH1F*) h->GetHisto();
    histo->Scale(yield/histo->Integral());
    histo->SetName(name.c_str());
    histo->Write();


    delete h;
    delete hh;

    return std::make_pair(yield,error);
}

std::pair<float,float> createHistogramShifted(std::string file,std::string name, std::string cut,std::string shift,float scaleFactor = 1, std::string postfix = "") {
    TFile *f  = new TFile(file.c_str());
    if(f==0) printf("Not file Found\n");
    //get the nominal tree first
    TTree *t= (TTree*)f->Get((channel_+"EventTree/eventTree").c_str());
    //TTree *t= (TTree*)f->Get((channel_+"EventTreeFinal/eventTree").c_str());
    if(t==0) printf("No Tree Found in file %s\n",file.c_str());

    std::pair<float,float> yield;

    yield =makeHistogramShifted(t,filelabel_+postfix,name,cut,shift,scaleFactor);

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
    //     printf("Created Histogram %s with entries=%f\n",name.c_str(),h->Integral());
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

    for(unsigned int i=0;i<shifts_.size();++i) {
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


std::pair<float,float> extractRFactor(std::string file,std::string preselection,std::string postfix) {
    TFile *f  = new TFile (file.c_str());
    TTree *t = (TTree*)f->Get((channel_+"EventTree/eventTree").c_str());

    std::pair<float,float> ss;
    std::pair<float,float> os;
    ss = makeHistogram(t,filelabel_+postfix,"W_SS",("("+preselection+"&&charge!=0)*"+weight_).c_str());
    os = makeHistogram(t,filelabel_+postfix,"W_OS",("("+preselection+"&&charge==0)*"+weight_).c_str());

    std::cout<<"W Rfactor Selection: ("<<preselection+"&&(charge requirement) )*"<<weight_<<std::endl;
    std::cout<<"W + Jets SS: "<< ss.first<<" OS: "<< os.first<<std::endl;

    float factor = os.first/ss.first;
    float factorerrStat = sqrt(os.second*os.second + ss.second*ss.second);
    //float factorerrSyst = factor*wFactorErr_;
    //float factorErr = sqrt(factorerrStat*factorerrStat+factorerrSyst*factorerrSyst);
    float factorErr = factorerrStat; 

    if(ss.first==0.){ 
        factor=0.;
        factorErr=0.;
    }
    return std::make_pair(factor,factorErr);   

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

std::pair<float,float> extractWFactorFinal(std::string file,std::string preselection,std::string postfix, std::string Wweight = "1") {
    TFile *f  = new TFile (file.c_str());
    TTree *t = (TTree*)f->Get((channel_+"EventTreeFinal/eventTree").c_str());

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

    std::cout<<"      Cuts in extract WFactor "<<"      OS W Mt>70: "<<high.first<<"      OS W Mt Low: "<<low.first<<std::endl;
    std::cout<<"      Extract WFactor Second"<<"      OS W Mt>70: "<<high.second<<"      OS W Mt Low: "<<low.second<<std::endl;
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

void subtractHistogramShifts(std::string folder, std::string histo1, std::string histo2)
{
    TH1F * h1 =(TH1F*) fout_->Get((folder+"/"+histo1).c_str());
    TH1F * h2 =(TH1F*) fout_->Get((folder+"/"+histo2).c_str());
    h1->Add(h2,-1.);

    for(int i=1;i<h1->GetNbinsX();++i){
        if(h1->GetBinContent(i)<0){
            h1->SetBinContent(i,0.00001);
            h1->SetBinError(i,1.0);
        }
    }

    for(unsigned int i=0;i<shifts_.size();++i) {
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



void subtractHistogram(std::string folder, std::string histo1, std::string histo2)
{
    TH1F * h1 =(TH1F*) fout_->Get((folder+"/"+histo1).c_str());
    TH1F * h2 =(TH1F*) fout_->Get((folder+"/"+histo2).c_str());
    h1->Add(h2,-1.);

    for(int i=1;i<h1->GetNbinsX();++i){
        if(h1->GetBinContent(i)<0){
            h1->SetBinContent(i,0.00001);
            h1->SetBinError(i,1.0);
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
        if (hist->GetBinContent(i) < 0){
            hist->SetBinContent(i,0.000001);
            if(verbose)
                std::cout<<"Bin "<< i << " is negative, setting to 1E6"<<std::endl;
        }
    }
    hist->ClearUnderflowAndOverflow();
    hist->Write(hist->GetName(),TObject::kOverwrite);
}

void FixNegativeBinsMC(std::string folder, std::string histo, bool verbose=false){

    FixNegativeBins(folder, histo, verbose);

    for(unsigned int i=0;i<shifts_.size();++i) {
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


bool qcdSyst(std::string channel, std::string prefix, std::string histo1, float corr, float unc)
{

    std::string chan = "mt";
    if(channel == "eleTau")
        chan = "et";
    if(channel == "diTau")
        chan = "tt";

    TH1F * h1 =(TH1F*) fout_->Get((channel+prefix+"/"+histo1).c_str());
    TH1F * h2 = (TH1F*)h1->Clone(TString(histo1+std::string("_CMS_xtt_QCDShape_"+chan+prefix+"_"+energy_+"Up")));
    TH1F * h3 = (TH1F*)h1->Clone(TString(histo1+std::string("_CMS_xtt_QCDShape_"+chan+prefix+"_"+energy_+"Down")));
    std::cout << "Channel label is " << chan << std::endl;
    float x,y;
    float cUp = corr+unc;
    float cDown = corr-unc;
    //printf("Creating QCD Shape Systematic \n");
    float scale = h1->Integral();
    for(int i=1;i<h1->GetNbinsX();++i){
        x = h1->GetXaxis()->GetBinCenter(i);
        if(x<50){
            y = h1->GetBinContent(i);
            h1->SetBinContent(i,y*corr);
            h2->SetBinContent(i,y*cUp);
            h3->SetBinContent(i,y*cDown);
        }

    }
    h1->Scale(scale/h1->Integral());
    h2->Scale(scale/h2->Integral());
    h3->Scale(scale/h3->Integral());
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
std::string zhFile_;
std::string vvFile_;
std::string zvvFile_;
std::string ewkFile_;
std::string topFile_;
std::string qcdFile_;
std::string dataFile_;
///////////////////////

//selections of regions
std::string preSelection_;
std::string osSignalSelection_;
std::string ssSignalSelection_;
std::string signalSelection_;
std::string osWSelection_;
std::string ssWSelection_;
std::string wSelection_;
std::string zSelection_;
std::string qcdSelection_;
std::string bSelection_;
std::string antibSelection_;
std::string relaxedSelection_;
std::string btagRelaxedSelection_;
std::string trigSelection_;
std::string trigSelectionDATA_;
std::string blinding_;
std::string charge_;
std::string ZLL_genLLSel_;
std::string ZTT_genTauSel_;
std::string ZJFT_genLReject_;
std::string ZLFT_genLSel_;
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
std::string Vweight_;
std::string TTweight_;
std::string embWeight_;
std::string zlftFactor_;

//external parameters
float topErr_;
float qcdErr_;//tmp
float smhErr_;
float vvErr_;
float zlftErr_;
float zjftErr_;
float wFactorErr_;
float qcdFactor_;
float qcdFactorErr_;


float bFactorZ_;
float bFactorZErr_;
float bFactorW_;
float bFactorWErr_;

float vbfFactorZ_;
float vbfFactorZErr_;
float vbfFactorW_;
float vbfFactorWErr_;


float boostFactorZ_;
float boostFactorZErr_;
float boostFactorW_;
float boostFactorWErr_;

float vhFactorZ_;
float vhFactorZErr_;
float vhFactorW_;
float vhFactorWErr_;


float bID_;
float bIDErr_;
float bMisID_;
float bMisIDErr_;

std::vector<std::string> mssmMasses_;
std::vector<std::string> smMasses_;
std::vector<std::string> smMassesDC_;

std::vector<float> mssmBBFraction_;
std::vector<float> smSigma_;
std::vector<float> vbfSigma_;
std::vector<float> vhSigma_;

std::string dir_;


};
