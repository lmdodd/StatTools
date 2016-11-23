#include "tdrstyle.C" 
#include "CMS_lumi.C" 

void applyStyle(TH1F* h, Color_t fill, Color_t line,int fillStyle)
{
    h->SetFillColor(fill);
    h->SetLineColor(line);
    h->SetFillStyle(fillStyle);
    h->SetLineWidth(1);
}


void applySignalStyle(TH1F* h)
{
    h->SetFillStyle(0);
    h->SetLineWidth(2);
    h->SetLineColor(kBlue+3);
    h->SetLineStyle(7);
}


void applyDATAStyle(TH1F* h)
{
    h->SetMarkerStyle(20.);
    h->SetMarkerSize(1.0);
    h->SetLineWidth(1.0);
    h->SetLineColor(kBlack);
}

void convertToDNDM(TH1F* histo) {
    for(int i=1;i<=histo->GetNbinsX();i++) {
        histo->SetBinContent(i,histo->GetBinContent(i)/histo->GetXaxis()->GetBinWidth(i));
        histo->SetBinError(i,histo->GetBinError(i)/histo->GetXaxis()->GetBinWidth(i));
    }
}



void makeLTauStack(TString name,TString file,TString dir,int s,TString labelX,TString units = "GeV",bool left=false,TString channel = "",TString json = "Golden",bool log = false,bool dndm=false,bool doRatio = false, int blind=900)
{
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
    else lumi_13TeV = channel+"    2016, "+json+" fb^{-1}";

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

    if (!doRatio) {
        c->SetLeftMargin(L/W);
        c->SetRightMargin(R/W);
        c->SetTopMargin(T/H);
        c->SetBottomMargin(B/H);
    }
    c->cd();


    TPad * plotPad = 0;
    TPad * ratioPad = 0;
    if(doRatio){
        plotPad = new TPad("pad1","",0.0,0.3,1.0,1.0);
        plotPad->SetTicks(0,0);
        plotPad->SetLeftMargin  (L/W);
        plotPad->SetRightMargin (R/W);
        plotPad->SetTopMargin   (T/H);
        plotPad->SetBottomMargin(B_ratio/H); 
        plotPad->SetFillColor(0);
        plotPad->SetBottomMargin(0);

        ratioPad = new TPad("pad2","",0.0,0.0,1.0,0.31);
        ratioPad->SetLeftMargin  (L/W);
        ratioPad->SetRightMargin (R/W);
        ratioPad->SetTopMargin   (T_ratio/H);
        ratioPad->SetBottomMargin(B_ratio_label/H);
        ratioPad->SetGridy(1);
        ratioPad->SetFillColor(4000);
    }
    else{
        plotPad = new TPad("pad1","",0.0,0.0,1.0,1.0);
        plotPad->SetLeftMargin     (L*1.4/W);
        plotPad->SetRightMargin    (R/W);
        plotPad->SetTopMargin      (T/H);
        plotPad->SetBottomMargin   (B/H);
    }

    plotPad->Draw();
    plotPad->cd();


    TFile *f = new TFile(file);

    TH1F * topErr = (TH1F*)(f->Get(dir+"/TOPBKGErr"));
    float topError = topErr->GetBinContent(1);
    TH1F * vvErr = (TH1F*)(f->Get(dir+"/VVBKGErr"));
    float vvError = vvErr->GetBinContent(1);
    TH1F * smhErr = (TH1F*)(f->Get(dir+"/SMHBKGErr"));
    float smhError = smhErr->GetBinContent(1);
    TH1F * wErr = (TH1F*)(f->Get(dir+"/WBKGErr"));
    float wError = wErr->GetBinContent(1);
    TH1F * qcdErr = (TH1F*)(f->Get(dir+"/QCDBKGErr"));
    float qcdError = qcdErr->GetBinContent(1);
    TH1F * zttErr = (TH1F*)(f->Get(dir+"/ZTTBKGErr"));
    float zttError = zttErr->GetBinContent(1);
    TH1F * zlftErr = (TH1F*)(f->Get(dir+"/ZLFTBKGErr"));
    float zlftError = zlftErr->GetBinContent(1);
    TH1F * zjftErr = (TH1F*)(f->Get(dir+"/ZJFTBKGErr"));
    float zjftError = zjftErr->GetBinContent(1);
    TH1F * zvvErr = (TH1F*)(f->Get(dir+"/ZVVBKGErr"));
    float zvvError = 0.0;

    //for(int i=1;i<errorBand->GetNbinsX();i++){
    //   y = errorBand->GetBinContent(i);
    //    errorBand->SetBinError(i,y*err);
    //}  



    TH1F * data = (TH1F*)(f->Get(dir+"/data_obs"));
    if (dndm) convertToDNDM(data);
    applyDATAStyle(data);

    TH1F * QCD = (TH1F*)(f->Get(dir+"/QCD"));
    std::cout<<"QCD error: "<<qcdError<<std::endl;
    for(int i=1;i<QCD->GetNbinsX()+1;i++){
        float y = QCD->GetBinContent(i);
        float r = y*qcdError;
        QCD->SetBinError(i,0.1);
        //std::cout<<"Setting QCD Bin "<<i<<" with Bin Content "<<y<<" with Bin Error "<<r<<std::endl;
    }  
    if (dndm) convertToDNDM(QCD);
    applyStyle(QCD,kMagenta-10,1,1001);

    TH1F * ttbar = (TH1F*)(f->Get(dir+"/TT"));
    std::cout<<"TOP error: "<<topError<<std::endl;
    for(int i=1;i<ttbar->GetNbinsX()+1;i++){
        float y = ttbar->GetBinContent(i);
        float r = y*topError;
        ttbar->SetBinError(i,r);
        //std::cout<<"Setting TOP Bin "<<i<<" with Bin Content "<<y<<" with Bin Error "<<r<<std::endl;
    }
    if (dndm) convertToDNDM(ttbar);
    applyStyle(ttbar,kBlue-8,1,1001);

    TH1F * zvv = (TH1F*)(f->Get(dir+"/ZVV"));
    std::cout<<"ZVV error: "<<zvvError<<std::endl;
    for(int i=1;i<zvv->GetNbinsX()+1;i++){
        float y = zvv->GetBinContent(i);
        float r = y*zvvError;
        zvv->SetBinError(i,r);
        //std::cout<<"Setting ZVV Bin "<<i<<" with Bin Content "<<y<<" with Bin Error "<<r<<std::endl;
    }
    if (dndm) convertToDNDM(zvv);
    applyStyle(zvv,kRed-9,1,1001);



    TH1F * EWK = (TH1F*)(f->Get(dir+"/W"));
    std::cout<<"W error: "<<wError<<std::endl;
    for(int i=1;i<EWK->GetNbinsX()+1;i++){
        float y = EWK->GetBinContent(i);
        float r = y*wError;
        EWK->SetBinError(i,r);
        //std::cout<<"Setting EWK Bin "<<i<<" with Bin Content "<<y<<" with Bin Error "<<r<<std::endl;
    }
    if (dndm) convertToDNDM(EWK);
    applyStyle(EWK,kRed-6,1,1001);

    TH1F * VV = (TH1F*)(f->Get(dir+"/VV"));
    std::cout<<"VV error: "<<vvError<<std::endl;
    for(int i=1;i<VV->GetNbinsX()+1;i++){
        float y = VV->GetBinContent(i);
        float r = y*vvError;
        VV->SetBinError(i,r);
        //std::cout<<"Setting VV Bin "<<i<<" with Bin Content "<<y<<" with Bin Error "<<r<<std::endl;
    }
    if (dndm) convertToDNDM(VV);
    applyStyle(VV,kMagenta+2,1,1001);

    TH1F * SMH = (TH1F*)(f->Get(dir+"/SMH"));
    std::cout<<"SMH error: "<<smhError<<std::endl;
    for(int i=1;i<SMH->GetNbinsX()+1;i++){
        float y = SMH->GetBinContent(i);
        float r = y*smhError;
        SMH->SetBinError(i,r);
        //std::cout<<"Setting SMH Bin "<<i<<" with Bin Content "<<y<<" with Bin Error "<<r<<std::endl;
    }
    if (dndm) convertToDNDM(SMH);
    applyStyle(SMH,kAzure-3,1,1001);

    TH1F * ZEE=0;
    if (channel == "#tau_{e}#tau_{h}"||channel == "#tau_{#mu}#tau_{h}"){
        if(f->Get(dir+"/ZLL")!=0)
            ZEE = (TH1F*)(f->Get(dir+"/ZLL"));	  	
        if(f->Get(dir+"/ZL")!=0&&f->Get(dir+"/ZLL")==0)
            ZEE = (TH1F*)(f->Get(dir+"/ZL"));
        if(f->Get(dir+"/ZJ")!=0&&f->Get(dir+"/ZLL")==0)
            ZEE->Add((TH1F*)(f->Get(dir+"/ZJ")));
        std::cout<<"ZL error: "<<zlftError<<std::endl;
        std::cout<<"ZJ error: "<<zjftError<<std::endl;
        for(int i=1;i<ZEE->GetNbinsX()+1;i++){
            float y = ZEE->GetBinContent(i);
            float r = y*zlftError;
            ZEE->SetBinError(i,r);
            std::cout<<"Setting ZEE Bin "<<i<<" with Bin Content "<<y<<" with Bin Error "<<r<<std::endl;
        }
        if (dndm) convertToDNDM(ZEE);
        applyStyle(ZEE,kAzure-9,1,1001);	
    }


    TH1F * ZTT = (TH1F*)(f->Get(dir+"/ZTT"));
    for(int i=1;i<ZTT->GetNbinsX()+1;i++){
        float y = ZTT->GetBinContent(i);
        float r = y*zttError;
        ZTT->SetBinError(i,r);
        //std::cout<<"Setting ZTT Bin "<<i<<" with Bin Content "<<y<<" with Bin Error "<<r<<std::endl;
    }
    if (dndm) convertToDNDM(ZTT);
    applyStyle(ZTT,kOrange-4,1,1001);

    TH1F *signalZH=0;
    TH1F *signalAH=0;

    //more signal styles to be added later
    THStack *hs = new THStack("hs","");
    //hs->SetLineWidth(2);
    hs->Add(ttbar);
    hs->Add(zvv);
    hs->Add(QCD);
    hs->Add(SMH);
    hs->Add(VV);
    hs->Add(EWK);
    if(channel == "#tau_{e}#tau_{h}") hs->Add(ZEE);
    if(channel == "#tau_{#mu}#tau_{h}") hs->Add(ZEE);
    hs->Add(ZTT);

    if(s==3) {
        TH1F * ZH = (TH1F*)(f->Get(dir+"/Zprime1200A400"));
        TH1F * AH = (TH1F*)(f->Get(dir+"/Zprime600A400"));
        TH1F *zh = ZH;
        TH1F *ah = AH;
        zh->Scale(1);//FIXME
        ah->Scale(1);//FIXME
        //TH1F * SM2 = (TH1F*)(f->Get(dir+"/qqH125"));
        //TH1F *sm2 = SM2;
        //sm2->Scale(100);//FIXME
        //sm->Add(sm2);
        signalZH=zh;
        signalAH=ah;
        signalZH->SetLineColor(kRed);
        signalAH->SetLineColor(kBlue);
        signalZH->SetLineWidth(3);
        signalAH->SetLineWidth(3);
        signalAH->SetLineStyle(7);
        //applyStyle(signal,kGray+2,1,3005);
        if (dndm) convertToDNDM(signalAH);
        if (dndm) convertToDNDM(signalZH);
        //hs->Add(SM);
    }

    TH1F* errorBand = (TH1F*)ttbar->Clone();
    errorBand->Add(EWK);
    errorBand->Add(VV);
    errorBand->Add(SMH);
    errorBand->Add(ZTT);
    errorBand->Add(zvv);
    errorBand->Add(QCD);
    //errorBand->Add(ZEE);
    errorBand->SetMarkerSize(0);
    errorBand->SetFillColor(kGray+2);
    errorBand->SetFillStyle(3005);
    errorBand->SetLineWidth(1);


    hs->Draw("HIST");
    if(data->GetMaximum()*1.2+sqrt(data->GetMaximum())>hs->GetMaximum()) {
        float max=0.0;
        if(data->GetMaximum()>hs->GetMaximum())
            max=data->GetMaximum();
        else
            max=hs->GetMaximum();

        if(log) {
            max*=10;
            hs->SetMinimum(0.5);
        }
        hs->SetMaximum(max*1.5+sqrt(data->GetMaximum()));

    }
    else{
        hs->SetMaximum(hs->GetMaximum()*1.8);
    }
    if(dndm)       hs->SetMinimum(0.001);


    hs->Draw("HIST");
    errorBand->Draw("e2same");

    if(doRatio){
        hs->GetXaxis()->SetLabelSize(0);
    }
    else
    {
        if(units!="")
            hs->GetXaxis()->SetTitle(labelX+" ["+units+"]");
        else
            hs->GetXaxis()->SetTitle(labelX);
    }

    hs->GetYaxis()->SetTitle("Events");
    hs->GetYaxis()->SetTitleOffset(1);
    if (!doRatio) {
        hs->GetYaxis()->SetTitleOffset(1.4);
        hs->GetXaxis()->SetLabelSize(0.03);
    }

    if(dndm)
        hs->GetYaxis()->SetTitle("dN/d"+labelX);

    if(s>0){
        signalAH->Draw("HIST,SAME");
        signalZH->Draw("HIST,SAME");
        if (s==3){
            std::cout<<"Total signal: "<<signalAH->Integral()<<std::endl;
            std::cout<<"Total signal: "<<signalZH->Integral()<<std::endl;
            int numb=signalAH->GetNbinsX();
            float y=0;
            for (int i=0; i<numb;i++){
                float sig = signalAH->GetBinContent(i+1);
                TH1 *last = (TH1*)hs->GetStack()->Last();
                float bkg = last->GetBinContent(i+1);
                y = sig/(TMath::Sqrt(bkg + (0.09*bkg)*(0.09*bkg)));
                if (y>=500) { 
                    std::cout<<" blinding bin "<<i+1<<std::endl;
                    data->SetBinContent(i+1,0);
                }
            }
        }
    }

    if(s==2) for (int i=0; i<data->GetNbinsX();i++){
        int xval = data->GetXaxis()->GetBinCenter(i+1);
        if (xval>blind) data->SetBinContent(i+1,0); 
    }
    data->Draw("e,SAME");


    c->cd();

    if(doRatio){
        TH1F * data2 = (TH1F*)data->Clone("data");
        TH1F * mc = (TH1F*)(ttbar);
        TH1F * mcErr = (TH1F*) errorBand->Clone("errorBand");

        mcErr->SetMarkerSize(0);
        mcErr->SetFillColor(kGray+2);
        mcErr->SetFillStyle(3005);
        mcErr->SetLineWidth(1);

        mc->Add(QCD);
        mc->Add(zvv);
        mc->Add(EWK);
        mc->Add(VV);
        mc->Add(SMH);
        mc->Add(ZTT);
        if (channel =="#tau_{e}#tau_{h}") mc->Add(ZEE);
        if (channel =="#tau_{#mu}#tau_{h}") mc->Add(ZEE);

        double xmin = mc->GetXaxis()->GetXmin();
        double xmax = mc->GetXaxis()->GetXmax();
        TLine *line = new TLine(xmin,1.0,xmax,1.0);
        line->SetLineWidth(1);
        line->SetLineColor(kBlack);

        ratioPad->Draw();
        ratioPad->cd();
        for(int i=1;i<mc->GetNbinsX();i++){
            float y = TMath::Sqrt(mc->GetBinContent(i));
            mc->SetBinError(i,y);
        }  


        data2->Divide(data2,mc);
        mcErr->Divide(mcErr,mcErr);

        data2->SetMarkerStyle(20);
        data2->SetTitleSize  (0.12,"Y");
        data2->SetTitleOffset(0.40,"Y");
        data2->SetTitleSize  (0.12,"X");
        data2->SetLabelSize  (0.10,"X");
        data2->SetLabelSize  (0.08,"Y");
        data2->GetYaxis()->SetRangeUser(0.5,1.5);
        //data2->GetYaxis()->SetRangeUser(0.62,1.38);
        data2->GetYaxis()->SetNdivisions(305);
        data2->GetYaxis()->SetTitle("Obs/Exp   ");

        //What does this do
        //->this affects the stat box style
        //gStyle->SetOptTitle(0);


        if (units!="")
            data2->GetXaxis()->SetTitle(labelX+" ["+units+"]");
        else
            data2->GetXaxis()->SetTitle(labelX);

        data2->Draw("P");
        mcErr->Draw("E2Same");
        line->Draw();

    }

    c->cd();
    plotPad->cd();  

    TLegend *l = new TLegend(xR,0.6,xR+0.5,0.9);
    l->AddEntry(data,"Observed","P");

    l->AddEntry(ZTT,"Z#rightarrow#tau#tau","F");
    if(channel == "#tau_{e}#tau_{h}") l->AddEntry(ZEE,"Z#rightarrowee","F");
    if(channel == "#tau_{#mu}#tau_{h}") l->AddEntry(ZEE,"Z#rightarrow#mu#mu","F");
    l->AddEntry(EWK,"WJets","F");
    l->AddEntry(VV,"VV Single Top","F");
    l->AddEntry(SMH,"ggH, vbfH, ZH","F");
    l->AddEntry(QCD,"QCD","F");
    l->AddEntry(ttbar,"t#bar{t}","F");
    l->AddEntry(zvv,"Z#rightarrow#nu#nu +Jets","F");
    l->AddEntry(errorBand, "bkg. uncertainty" , "F" );


    if(log){
        if(s==3){
            l->AddEntry(signalAH,"Zprime(600)#rightarrowA(400)#rightarrow#Chi#Chih#rightarrow#tau#tau 1 pb","L");
            l->AddEntry(signalZH,"Zprime(1200)#rightarrowA(400)#rightarrow#Chi#Chih#rightarrow#tau#tau 1 pb","L");
        }
    }
    else{
        if(s==3){
            l->AddEntry(signalAH,"Zprime(600)#rightarrow(A(400)#rightarrow#Chi#Chi)(h#rightarrow#tau#tau) 1 pb","F");
            l->AddEntry(signalZH,"Zprime(1200)#rightarrow(A(400)#rightarrow#Chi#Chi)(h#rightarrow#tau#tau) 1 pb","F");
        }
    }

    l->SetBorderSize(0);
    l->SetFillColor(0);
    l->SetFillStyle (0);

    l->Draw();


    float factor=0.0;
    if(left) factor = 1./2.2;
    else factor = 1.05;

    float xL = hs->GetXaxis()->GetXmin()+(hs->GetXaxis()->GetXmax()-hs->GetXaxis()->GetXmin())*xR*factor;
    float yL = hs->GetMaximum()*0.35;

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



