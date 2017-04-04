#include "optimize.C"

void optimizeETau(){

    optimizeSB("~/www/ztt/2016/ETau/optimize/eleTau_pth_optimize","eleTauPlots/eleTau_pth.root","et_inclusive","ZprimeA400Z600","ZprimeA400Z600",3,"P_{T,tot} ","GeV",false,"#tau_{e}#tau_{h}","Golden",false);
    optimizeSB("~/www/ztt/2016/ETau/optimize/eleTau_met_optimize","eleTauPlots/eleTau_met.root","et_inclusive","ZprimeA400Z600","ZprimeA400Z600",3,"ME_{T} ","GeV",false,"#tau_{e}#tau_{h}","Golden",false);
}

