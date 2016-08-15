#include <TChain.h>
#include <TH1F.h>
#include <vector>
#include <TFormula.h>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class CutOptimizer {
 public:
  CutOptimizer();
  ~CutOptimizer();
  
  CutOptimizer(TChain*,TChain*);
  void addCut(string&,string&,double,double,double);
  void setPreselections(std::string,std::string);
  void setSignificance(string&);
  void run();

 private:
  void replace(const std::string&,const std::string&,std::string&);
  void loop(const ostringstream&,int);


  
  TChain *signal_;
  TChain *bkg_;

  vector<string> cuts_;
  vector<string> dirs_;
  vector<double> min_;
  vector<double> max_;
  vector<double> step_;


  std::string signalCuts_;
  std::string bkgCuts_;

  std::string optimalCut_;
  double optSignificance_;

  TFormula * formula;
};
