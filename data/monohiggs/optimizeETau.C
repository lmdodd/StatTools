#include "optimize.C"

void optimizeETau(){

    optimizeSB("~/www/ztt/2016/ETau/optimize/eleTau_pth_optimize","eleTauPlots/eleTau_pth.root","et_inclusive","ZprimeA300Z1200","ZprimeA300Z600",3,"P_{T,tot} ","GeV",false,"#tau_{e}#tau_{h}","Golden",true);
    optimizeSB("~/www/ztt/2016/ETau/optimize/eleTau_met_optimize","eleTauPlots/eleTau_met.root","et_inclusive","ZprimeA300Z1200","ZprimeA300Z600",3,"ME_{T} ","GeV",false,"#tau_{e}#tau_{h}","Golden",true);
}

