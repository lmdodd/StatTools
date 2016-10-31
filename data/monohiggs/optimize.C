#include "tdrstyle.C" 
#include "CMS_lumi.C" 


void applyStyle(TH1F* h, Color_t line)
{
    h->SetFillStyle(0);
    h->SetLineWidth(2);
    h->SetLineColor(line);
}




void optimizeSB(TString name,TString file,TString dir,TString sampleB, TString sampleW, int s, TString labelX,TString units = "GeV",bool left=false,TString channel = "",TString json = "Golden",bool log = false, bool lessthan = false){

    setTDRStyle();
    gROOT->SetBatch(kTRUE);

    writeExtraText = true;       // if extra text
    extraText  = "Preliminary";  // default extra text is "Preliminary"
    //extraText  = "Analysis in Progress";  // default extra text is "Preliminary"
    lumi_8TeV  = "19.1 fb^{-1}"; // default is "19.7 fb^{-1}"
    lumi_7TeV  = "4.9 fb^{-1}";  // default is "5.1 fb^{-1}"
    lumi_sqrtS = "13 TeV";
    if (json=="FuckMuons") lumi_13TeV = channel+"    2016, 3.4 fb^{-1}";
    if (json=="Golden") lumi_13TeV = channel+"    2016, 12.9 fb^{-1}";

    int iPeriod = 4;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV 


    float xR=0.5; //legend parameters
    if(left) xR=0.2;//legend parameters

    int H = 600;
    int W = 600;

    int H_ref = 600;
    int W_ref = 600;

    // references for T, B, L, R
    float T = 0.08*H_ref;
    float B = 0.12*H_ref; 
    float L = 0.12*W_ref;
    float R = 0.04*W_ref;

    //margins for inbetween the pads in a ratio plot
    float B_ratio = 0.1*H_ref; 
    float T_ratio = 0.03*H_ref; 

    //margin required for label on bottom of raito plot
    float B_ratio_label = 0.3*H_ref; 


    TCanvas * c = new TCanvas(name,name,50,50,W,H);
    c->SetFillColor(0);
    c->SetBorderMode(0);
    c->SetFrameFillStyle(0);
    c->SetFrameBorderMode(0);

    c->SetLeftMargin(L/W);
    c->SetRightMargin(R/W);
    c->SetTopMargin(T/H);
    c->SetBottomMargin(B/H);
    c->cd();

    std::cout<<"plotPad"<<std::endl;
    TPad * plotPad =0 ;
    plotPad = new TPad("pad1","",0.0,0.0,1.0,1.0);
    plotPad->SetLeftMargin     (L*1.4/W);
    plotPad->SetRightMargin    (R/W);
    plotPad->SetTopMargin      (T/H);
    plotPad->SetBottomMargin   (B/H);

    plotPad->Draw();
    plotPad->cd();

    std::cout<<"newfile"<<std::endl;

    TFile *f = new TFile(file);


    std::cout<<"QCD"<<std::endl;
    TH1F * QCD = (TH1F*)(f->Get(dir+"/QCD"));
    std::cout<<"TT"<<std::endl;

    TH1F * ttbar = (TH1F*)(f->Get(dir+"/TT"));

    std::cout<<"ZVV"<<std::endl;
    TH1F * zvv = (TH1F*)(f->Get(dir+"/ZVV"));

    std::cout<<"W"<<std::endl;
    TH1F * EWK = (TH1F*)(f->Get(dir+"/W"));

    std::cout<<"VV"<<std::endl;
    TH1F * VV = (TH1F*)(f->Get(dir+"/VV"));

    std::cout<<"ZEE"<<std::endl;
    TH1F * ZEE =0;
    if(f->Get(dir+"/ZLL")!=0)
        ZEE = (TH1F*)(f->Get(dir+"/ZLL"));	  	
    if(f->Get(dir+"/ZL")!=0&&f->Get(dir+"/ZLL")==0)
        ZEE = (TH1F*)(f->Get(dir+"/ZL"));
    if(f->Get(dir+"/ZJ")!=0&&f->Get(dir+"/ZLL")==0)
        ZEE->Add((TH1F*)(f->Get(dir+"/ZJ")));

    std::cout<<"ZTT"<<std::endl;
    TH1F * ZTT = (TH1F*)(f->Get(dir+"/ZTT"));

    TH1F *signalZH=0;
    TH1F *signalAH=0;


    std::cout<<"THstack"<<std::endl;
    //more signal styles to be added later
    THStack *hs = new THStack("hs","");
    //hs->SetLineWidth(2);
    hs->Add(ttbar);
    hs->Add(zvv);
    hs->Add(QCD);
    hs->Add(EWK);
    hs->Add(VV);
    if(channel == "#tau_{e}#tau_{h}") hs->Add(ZEE);
    if(channel == "#tau_{#mu}#tau_{h}") hs->Add(ZEE);
    hs->Add(ZTT);
    //hs->Draw();
    std::cout<<"All histos found" <<" getting smalpes histos"<<std::endl; 

    TH1F * ZH = (TH1F*)(f->Get(dir+"/"+sampleB));
    TH1F * AH = (TH1F*)(f->Get(dir+"/"+sampleW));
    std::cout<<"Sample histos found"<<std::endl;
    TH1F *zh = ZH;
    TH1F *ah = AH;
    ah->Scale(1);
    zh->Scale(1);
    signalZH=zh;
    signalAH=ah;

    TH1F *optimizeBest = 0;
    TH1F *optimizeWorst = 0;
    TH1F * temp1B  = (TH1F*)(f->Get(dir+"/"+sampleB));
    TH1F * temp1W  = (TH1F*)(f->Get(dir+"/"+sampleW));
    TH1F *tempB  = temp1B;
    TH1F *tempW  = temp1W;
    optimizeBest = tempB;
    optimizeWorst = tempW;
    applyStyle(optimizeBest,kRed-9);
    applyStyle(optimizeWorst,kBlue-9);

    int yieldB=signalZH->Integral();
    std::cout<<"yield B: "<<yieldB<<std::endl;
    int yieldW=signalAH->Integral();
    std::cout<<"yield W: "<<yieldW<<std::endl;
    int numb=signalAH->GetNbinsX();
    std::cout<<"number of bins: "<<numb<<std::endl;
    float yB=0;
    float yW=0;

    for (int i=0; i<numb;i++){
        float sigB = signalZH->Integral(i+1,numb+1);
        float sigW = signalAH->Integral(i+1,numb+1);
        //std::cout<<"sig: "<<sig<<" in bin number "<<i+1<<std::endl;
        TH1 *last = (TH1*)hs->GetStack()->Last();
        float bkg = last->Integral(i+1,numb+1);
        if (lessthan){
            float sigB = signalZH->Integral(1,i+1);
            float sigW = signalAH->Integral(1,i+1);
            float bkg = last->Integral(1,i+1);
            if (bkg==0) bkg=1;
        }
        //std::cout<<"bkg: "<<bkg<<" in bin number "<<i+1<<std::endl;
        yB = sigB/(TMath::Sqrt(bkg));
        yW = sigW/(TMath::Sqrt(bkg));
        //std::cout<<"s/Sqrt(B): "<<yB<<" in bin number "<<i+1<<std::endl;
        optimizeBest->SetBinContent(i+1,yB);
        optimizeWorst->SetBinContent(i+1,yW);
    }
    optimizeBest->GetYaxis()->SetTitle("S/Sqrt(B)");
    if (log){
        optimizeBest->SetMaximum(optimizeBest->GetMaximum()*10);
        optimizeBest->SetMinimum(0.5);
    }
    else 
        optimizeBest->SetMaximum(optimizeBest->GetMaximum()*1.8);

    optimizeBest->Draw();
    optimizeWorst->Draw("same");

    if(units!="")
        optimizeBest->GetXaxis()->SetTitle(labelX+" ["+units+"]");
    else
        optimizeBest->GetXaxis()->SetTitle(labelX);

    int binmaxB = optimizeBest->GetMaximumBin();
    double xB = optimizeBest->GetXaxis()->GetBinCenter(binmaxB);
    double maxW = optimizeWorst->GetMaximum();
    int binmaxW = optimizeWorst->GetMaximumBin();
    double xW = optimizeWorst->GetXaxis()->GetBinCenter(binmaxW);

    TLatex latex;
    char array[20];
    sprintf(array, "max=%0.2f at %0.2f",maxW,xW );
    latex.DrawLatex(100,9,array);

    c->cd();

    plotPad->cd();  

    TLegend *l = new TLegend(xR,0.6,xR+0.3,0.9);

    l->AddEntry(optimizeBest ,sampleB,"L");
    l->AddEntry(optimizeWorst ,sampleW,"L");

    l->SetBorderSize(0);
    l->SetFillColor(0);
    l->SetFillStyle (0);

    l->Draw();


    float factor=0.0;
    if(left) factor = 1./2.2;
    else factor = 1.05;

    float yL = optimizeBest->GetMaximum()*0.35;

    if(log)
        yL = hs->GetMaximum()*0.35;

    float offsetF=yL-0.1*hs->GetMaximum();
    float offsetFF=yL-0.2*hs->GetMaximum();

    if(log) {

        offsetF=yL/2.;
        offsetFF=yL/5.;
    }

    CMS_lumi(c,4,11);
    plotPad->Draw();
    CMS_lumi(c,4,11);
    if(log)
        plotPad->SetLogy();

    //c->RedrawAxis(); 
    c->SaveAs(name+".png");
    c->SaveAs(name+".pdf");
    c->SaveAs(name+".root");

}



