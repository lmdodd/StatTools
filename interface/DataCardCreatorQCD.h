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
    float QCD, dQCD;

    BkgOutput(){};

    BkgOutput(float i) {
        // This assumes that a-d are all of the same type and all in order
        std::fill(&DATA, &dQCD+1, i);
    }

};



class DataCardCreatorQCD {
    public:

        DataCardCreatorQCD(optutl::CommandLineParser parser) {
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
                shiftsPostFix_.push_back("CMS_scale_"+shiftL+"_"+chan+"_"+energy_);
            }

            //read input files
            zttFile_  = parser.stringValue("zttFile");
            zllFile_  = parser.stringValue("zllFile");
            wFile_    = parser.stringValue("wFile");
            vvFile_   = parser.stringValue("vvFile");
            topFile_  = parser.stringValue("topFile");
            qcdFile_  = parser.stringValue("qcdFile");
            dataFile_  = parser.stringValue("dataFile");

            //read control and signal regions
            preSelection_ = parser.stringValue("preselection");

            signalSelection_      = parser.stringValue("signalselection");
            wSelection_           = parser.stringValue("wselection");
            qcdSelection_         = parser.stringValue("qcdselection");
            relaxedSelection_     = parser.stringValue("relaxedSelection");
            trigSelection_        = parser.stringValue("trigSelection");
            trigSelection50ns_        = parser.stringValue("trigSelection50ns");
            trigSelection25ns_        = parser.stringValue("trigSelection25ns");
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
            Zweight_        = parser.stringValue("Zweight");
            TTweight_        = parser.stringValue("TTweight");
            embWeight_     = parser.stringValue("embWeight");

            //define the histogram binning
            bins_ = parser.integerValue("bins");
            max_  = parser.doubleValue("max");


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


        /*-------------Create Histograms------------------*/
        /*
         * Run the full extrapolation in the Btag category
         * Needed: BtagSF, categorySelection
         *
         */

        BkgOutput runFullExtrapBtag(std::string preSelection, std::string relaxedSelection, std::string qcdSelection, std::string categorySelection_, std::string prefix, std::string bTagSF) {

            std::cout<<"======================"<<std::endl;
            std::cout<<"Run Full Extrapolation"<<std::endl;
            std::cout<<"categorySelection: "<<categorySelection_<<std::endl;
            //TODO: Fix this category seleciton business, should be more elegant
            std::string categorySelection = categorySelection_;
            std::string dummySelection_   = osSignalSelection_;

            weight_ = weight_+"*"+bTagSF;

            BkgOutput output;    

            float leg1Corr=1.0;
            if(muID_!=0) leg1Corr*=muID_;
            if(eleID_!=0) leg1Corr*=eleID_;

            printf("Tau ID Scale Factor is %.3f \n",tauID_);

            std::cout<<"Create QCD"<<std::endl;
            //Create QCD
            if(!runQCD(preSelection, prefix, output, categorySelection, relaxedSelection,qcdSelection)){
                std::cout<<"Error Creating QCD"<<std::endl;
                return output;
            }



            std::cout<<std::endl;
            std::cout<<"=============Data Yields============="<<std::endl;
            std::cout<<std::endl;
            std::cout<<"QCD: "<< output.QCD<<std::endl;

            return output;

        }


        /*
         * For Inclusive take the Ztt normalization directly from MC (zttFile_)
         * For a category the Ztt normalization is as follows,
         * ZttCategoryYield = ZttMCInclusiveYield * (ZttEmbedded_Category) / *Ztt(
         */
        bool runQCD(std::string preSelection, std::string prefix, BkgOutput &output, std::string categorySelection, std::string relaxedSelection, std::string qcdSelection) 
        {
            float leg1Corr=1.0;
            if(muID_!=0) leg1Corr*=muID_;
            if(eleID_!=0) leg1Corr*=eleID_;
            std::cout<<"QCD Shape Selections= "<<"("<<relaxedSelection<<"&&"<<trigSelection50ns_<<"&&"<<ssSignalSelection_<<"&&"<<qcdSelection<<")"<<std::endl;

            std::pair<float,float> dataQCDShape = createHistogramAndShifts(dataFile_,"QCD","("+relaxedSelection+"&&"+trigSelection50ns_+"&&"+ssSignalSelection_+"&&"+qcdSelection+")",scaleUp_,prefix); 
            printf("      Data events in SS Signal QCDShape sideband region = %f + %f \n",dataQCDShape.first,dataQCDShape.second);
            std::pair<float,float> ZQCDShape = createHistogramAndShifts(zttFile_,"ZQCD","("+relaxedSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+qcdSelection+")*"+weight_+"*"+Zweight_,luminosity_,prefix); 
            printf("      Z events in SS Signal QCDShape sideband region = %f + %f \n",ZQCDShape.first,ZQCDShape.second);
            std::pair<float,float> TopQCDShape = createHistogramAndShifts(topFile_,"TopQCD","("+relaxedSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+qcdSelection+")*"+weight_,luminosity_,prefix); 
            //std::pair<float,float> TopQCDShape = createHistogramAndShifts(topFile_,"TopQCD","("+relaxedSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+qcdSelection+")*"+weight_+"*"+TTweight_,luminosity_,prefix); 
            printf("      TOP events in SS Signal QCDShape sideband region = %f + %f \n",TopQCDShape.first,TopQCDShape.second);
            std::pair<float,float> VVQCDShape       = createHistogramAndShifts(vvFile_,"VVQCD",("("+relaxedSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+qcdSelection+")*"+weight_),luminosity_,prefix,false);
            printf("      VV events in SS Signal QCDShape sideband region = %f + %f \n",VVQCDShape.first,VVQCDShape.second);
            std::pair<float,float> WQCDLowShape       = createHistogramAndShifts(wFile_,"WQCD",("("+relaxedSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+qcdSelection+")*"+weight_),luminosity_,prefix,false);

            std::cout<<"QCD Yield Selections= "<<"("<<preSelection<<"&&"<<trigSelection50ns_<<"&&"<<ssSignalSelection_<<"&&"<<categorySelection<<")"<<std::endl;
            std::pair<float,float> dataQCD = createHistogramAndShifts(dataFile_,"QCDYield","("+preSelection+"&&"+trigSelection50ns_+"&&"+ssSignalSelection_+"&&"+categorySelection+")",scaleUp_,prefix); 
            printf("      Data events in SS Signal QCD sideband region = %f + %f \n",dataQCD.first,dataQCD.second);
            std::pair<float,float> ZQCD = createHistogramAndShifts(zttFile_,"ZQCDYield","("+preSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")*"+weight_+"*"+Zweight_,luminosity_,prefix); 
            printf("      Z events in SS Signal QCD sideband region = %f + %f \n",ZQCD.first,ZQCD.second);
            std::pair<float,float> TopQCD = createHistogramAndShifts(topFile_,"TopQCDYield","("+preSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")*"+weight_+"*"+TTweight_,luminosity_,prefix); 
            printf("      TOP events in SS Signal QCD sideband region = %f + %f \n",TopQCD.first,TopQCD.second);
            std::pair<float,float> VVQCD       = createHistogramAndShifts(vvFile_,"VVQCDYield",("("+preSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")*"+weight_),luminosity_,prefix,false);
            printf("      VV events in SS Signal QCD sideband region = %f + %f \n",VVQCD.first,VVQCD.second);

            std::pair<float,float> ssWLow       = createHistogramAndShifts(wFile_,"WQCDYield",("("+preSelection+"&&"+trigSelection_+"&&"+ssSignalSelection_+"&&"+categorySelection+")*"+weight_),luminosity_*leg1Corr,prefix,false);
            printf("      (MonteCarlo) W events in SS region = %f + %f \n",ssWLow.first,ssWLow.second);

            /*
             * DATADRIVEN W FOUND 
             */

            //Now subtracting off bkgd shapes from data ss shape
            subtractHistogram(filelabel_+prefix,"QCDYield","ZQCDYield");
            subtractHistogram(filelabel_+prefix,"QCDYield","VVQCDYield");
            subtractHistogram(filelabel_+prefix,"QCDYield","TopQCDYield");
            subtractHistogram(filelabel_+prefix,"QCDYield","WQCDYield");

            subtractHistogram(filelabel_+prefix,"QCD","ZQCD");
            subtractHistogram(filelabel_+prefix,"QCD","VVQCD");
            subtractHistogram(filelabel_+prefix,"QCD","TopQCD");
            subtractHistogram(filelabel_+prefix,"QCD","WQCD");


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
                qcdSyst(filelabel_, prefix, "QCD", 1.1, 0.1);
            }

            return true;


        }

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
            if(t==0) printf("Not Tree Found in file %s\n",file.c_str());
            std::pair<float,float> yield;

            if(!keys)
                yield =makeHistogram(t,filelabel_+postfix,name,cut,scaleFactor);
            else
                yield =makeTHKeys(t,filelabel_+postfix,name,cut,scaleFactor);

            //now the shifts
            std::pair<float,float> tmpYield;
            for(unsigned int i=0;i<shifts_.size();++i) {

                TTree *ts= (TTree*)f->Get((channel_+"EventTree"+shifts_[i]+"Up/eventTree").c_str());
                if(ts!=0) {
                    if(!keys)
                        tmpYield = makeHistogram(ts,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",cut,scaleFactor);
                    else
                        tmpYield = makeTHKeys(ts,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",cut,scaleFactor);

                    if(!normUC)
                        scaleHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",yield.first/tmpYield.first);

                }
                TTree *td= (TTree*)f->Get((channel_+"EventTree"+shifts_[i]+"Down/eventTree").c_str());
                if(td!=0) {
                    if(!keys)
                        tmpYield = makeHistogram(td,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",cut,scaleFactor);
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
            if(t==0) printf("Not Tree Found in file %s\n",file.c_str());

            std::pair<float,float> yield;

            if(!keys)
                yield =makeHistogram(t,filelabel_+postfix,name,cut,scaleFactor);
            else
                yield =makeTHKeys(t,filelabel_+postfix,name,cut,scaleFactor);

            //now the shifts
            std::pair<float,float> tmpYield;
            for(unsigned int i=0;i<shifts_.size();++i) {

                TTree *ts= (TTree*)f->Get((channel_+"EventTreeFinal"+shifts_[i]+"Up/eventTree").c_str());
                if(ts!=0) {
                    if(!keys)
                        tmpYield = makeHistogram(ts,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",cut,scaleFactor);
                    else
                        tmpYield = makeTHKeys(ts,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",cut,scaleFactor);

                    if(!normUC)
                        scaleHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",yield.first/tmpYield.first);

                }
                TTree *td= (TTree*)f->Get((channel_+"EventTreeFinal"+shifts_[i]+"Down/eventTree").c_str());
                if(td!=0) {
                    if(!keys)
                        tmpYield = makeHistogram(td,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",cut,scaleFactor);
                    else
                        tmpYield = makeTHKeys(td,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",cut,scaleFactor);

                    if(!normUC)
                        scaleHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",yield.first/tmpYield.first);

                }
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

        std::pair<float,float> makeHistogram(TTree* tree,std::string folder,std::string name,std::string cut,float scaleFactor = 1.) {

            if(fout_->Get(folder.c_str())==0)
                fout_->mkdir(folder.c_str());
            TH1F *h=0;

            TString variableOld_=variable_;
            if((name=="data_obs"||name=="data_obs_ss"||name=="data_obs_sdb"||name=="data_obs_ss_sdb"||name=="QCD")&&variable_=="pt2ES"){
                variable_="pt_2";
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
            TTree *t= (TTree*)f->Get((channel_+"EventTreeFinal/eventTree").c_str());
            if(t==0) printf("Not Tree Found in file %s\n",file.c_str());

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


        void FixNegativeBins(std::string folder, std::string histo, bool is_qcd, bool verbose=false) {

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

        void FixNegativeBinsMC(std::string folder, std::string histo, bool is_qcd, bool verbose=false){

            FixNegativeBins(folder, histo, is_qcd, verbose);

            for(unsigned int i=0;i<shifts_.size();++i) {
                TH1F * hh =(TH1F*) fout_->Get((folder+"/"+histo+"_"+shiftsPostFix_[i]+"Up").c_str());
                if(hh!=0) {
                    FixNegativeBins(folder, (histo+"_"+shiftsPostFix_[i]+"Up").c_str(), is_qcd, verbose);
                }

                TH1F * hhh =(TH1F*) fout_->Get((folder+"/"+histo+"_"+shiftsPostFix_[i]+"Down").c_str());
                if(hhh!=0) {
                    FixNegativeBins(folder, (histo+"_"+shiftsPostFix_[i]+"Down").c_str(), is_qcd, verbose);
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

            TH1F * h1 =(TH1F*) fout_->Get((channel+prefix+"/"+histo1).c_str());
            TH1F * h2 = (TH1F*)h1->Clone(TString(histo1+std::string("_CMS_htt_QCDShape_"+chan+prefix+"_"+energy_+"Up")));
            TH1F * h3 = (TH1F*)h1->Clone(TString(histo1+std::string("_CMS_htt_QCDShape_"+chan+prefix+"_"+energy_+"Down")));
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
        std::string vvFile_;
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
        std::string qcdSelection_;
        std::string relaxedSelection_;
        std::string trigSelection_;
        std::string trigSelection50ns_;
        std::string trigSelection25ns_;
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
        std::string TTweight_;
        std::string embWeight_;

        std::string zlftFactor_;
        //external parameters
        float topErr_;
        float qcdErr_;//tmp
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
