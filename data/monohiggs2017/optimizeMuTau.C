#include "optimize.C"

void optimizeMuTau(){

    optimizeSB("~/www/ztt/2016/MuTau/optimize/muTau_met_optimize","muTauPlots/muTau_met.root","mt_inclusive","ZprimeA400Z600","ZprimeA400Z600",3,"ME_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false);
    optimizeSB("~/www/ztt/2016/MuTau/optimize/muTau_pth_optimize","muTauPlots/muTau_pt.root","mt_inclusive","ZprimeA400Z600","ZprimeA400Z600",3,"P_{T,tot} ","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false);
}

