#include "UWAnalysis/ROOT/interface/CutOptimizer.h"

CutOptimizer::CutOptimizer()
{}

CutOptimizer::~CutOptimizer()
{}

CutOptimizer::CutOptimizer(TChain *signal , TChain* bkg)
{
  signal_ = signal;
  bkg_ = bkg;

}


void 
CutOptimizer::addCut(string& cut,string& dir,double min,double max,double step)
{
  cuts_.push_back(cut);
  dirs_.push_back(dir);
  min_.push_back(min);
  max_.push_back(max);
  step_.push_back(step);
}

void 
CutOptimizer::setPreselections(string sig,string bkg)
{
  signalCuts_ = sig;
  bkgCuts_    = bkg;
}

void 
CutOptimizer::setSignificance(string& f)
{
  std::string form = f;
  replace("SIG","[0]",form);
  replace("BKG","[1]",form);
  
  formula = new TFormula("f",form.c_str());

  if(formula->Compile()!=0)
    printf("BAD FORMULA\n");


}


void 
CutOptimizer::replace(const std::string& oldSubStr,const std::string& newSubStr,std::string& str_)
{
  if ( oldSubStr == newSubStr ) return;
  if ( oldSubStr.empty() ) return;
  const std::string::size_type lengthOldSubStr = oldSubStr.size();
  const std::string::size_type lengthNewSubStr = newSubStr.size();
  std::string::size_type positionPreviousMatch = 0;
  std::string::size_type positionNextMatch = 0;
  while ( (positionNextMatch = str_.find(oldSubStr, positionPreviousMatch)) != std::string::npos ) {
    str_.replace(positionNextMatch, lengthOldSubStr, newSubStr);
    positionPreviousMatch = positionNextMatch + lengthNewSubStr;
  } 
}


void 
CutOptimizer::run()
{
  //create a small histogram and fill it with the results
  ostringstream cut;
  optimalCut_="";
  optSignificance_=0;

  loop(cut,0);

  printf("Optimization results-----------------\n");
  printf("Maximum Significance = %f\n",optSignificance_);
  printf("OptimalCut = %s \n",optimalCut_.c_str());
}


void
CutOptimizer::loop(const ostringstream& cut,int i)
{

  //  printf("Looping with cut=%d  %s\n",i,cut.str().c_str());

  if((unsigned int)i==cuts_.size())
    {
      //run the algo
      TH1F *cutHisto = new TH1F("cutHisto","H",2,0.,2.);
      std::string ex = cut.str()+">>cutHisto";
      //      printf("Running %s \n",ex.c_str());
      signal_->Draw(ex.c_str(),signalCuts_.c_str(),"goff");

      const double dummyArray[] = {0.0};
      std::vector<double> input;
      
      input.push_back(cutHisto->GetBinContent(2));
      bkg_->Draw(ex.c_str(),bkgCuts_.c_str(),"goff");
      input.push_back(cutHisto->GetBinContent(2));

      if(input[0]>0||input[1]>0)
	{
	  double significance = formula->EvalPar(dummyArray,&input[0]);
	  if(significance>optSignificance_)
	    {
	      optimalCut_ = cut.str();
	      optSignificance_ = significance;
	    }
	}
      else
	{
	  printf("Found ZERO S,B events\n");
	}

      cutHisto->Delete();
    }
  else
    {
      for(double c =min_[i];c<max_[i];c+=step_[i])
	{
	  ostringstream newCut;
	  if(i>0)
	    newCut << cut.str()<<"*("<<cuts_[i]<<dirs_[i]<<c<<")";
	  else
	    newCut << cut.str()<<"("<<cuts_[i]<<dirs_[i]<<c<<")";

	  printf("Looping for %s \n",newCut.str().c_str());
	  loop(newCut,i+1);
	}
    }


}
