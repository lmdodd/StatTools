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
	for(int i=1;i<=histo->GetNbinsX();++i) {
		histo->SetBinContent(i,histo->GetBinContent(i)/histo->GetXaxis()->GetBinWidth(i));
		histo->SetBinError(i,histo->GetBinError(i)/histo->GetXaxis()->GetBinWidth(i));
	}
}



makeLTauStack(TString name,TString file,TString dir,int s,TString labelX,TString units = "GeV",bool left=false,TString channel = "",TString json = "Golden",bool log = false,bool dndm=false,bool doRatio = false)
{
	setTDRStyle();

	writeExtraText = true;       // if extra text
	extraText  = "Preliminary";  // default extra text is "Preliminary"
	lumi_8TeV  = "19.1 fb^{-1}"; // default is "19.7 fb^{-1}"
	lumi_7TeV  = "4.9 fb^{-1}";  // default is "5.1 fb^{-1}"
	lumi_sqrtS = "13 TeV";
	if (json=="Golden") lumi_13TeV = channel+"    2016, 0.8 fb^{-1}";

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
		c->SetTicks(0,0);
	}
	c->cd();

	if(doRatio){
		TPad * plotPad = new TPad("pad1","",0.0,0.3,1.0,1.0);
		plotPad->SetTicks(0,0);
		plotPad->SetLeftMargin  (L/W);
		plotPad->SetRightMargin (R/W);
		plotPad->SetTopMargin   (T/H);
		plotPad->SetBottomMargin(B_ratio/H); 
		plotPad->SetFillColor(0);
		plotPad->SetBottomMargin(0);

		TPad * ratioPad = new TPad("pad2","",0.0,0.0,1.0,0.31);
		ratioPad->SetLeftMargin  (L/W);
		ratioPad->SetRightMargin (R/W);
		ratioPad->SetTopMargin   (T_ratio/H);
		ratioPad->SetBottomMargin(B_ratio_label/H);
		ratioPad->SetGridy(1);
		ratioPad->SetFillColor(4000);
	}
	else{
		TPad * plotPad = new TPad("pad1","",0.0,0.0,1.0,1.0);
		plotPad->SetLeftMargin     (L*1.4/W);
		plotPad->SetRightMargin    (R/W);
		plotPad->SetTopMargin      (T/H);
		plotPad->SetBottomMargin   (B/H);
	}

	plotPad->Draw();
	plotPad->cd();


	TFile *f = new TFile(file);


	TH1F * data1 = (TH1F*)(f->Get(dir+"/data_obs"));
	TH1F * data = new TH1F("data","",3,0,3); 
	for(int i=1;i<4;++i){
  		int j =i;
		if (i==3) j=11; 
		float bc = data1->GetBinContent(j);
                std::cout<<"Set data bin "<<i<<" with "<<bc<<std::endl; 
		data->SetBinContent(i,bc);
	}
	if (dndm) convertToDNDM(data);
	applyDATAStyle(data);

	TH1F * QCD1 = (TH1F*)(f->Get(dir+"/QCD"));
	TH1F * QCD = new TH1F("QCD","",3,0,3); 
	for(int i=1;i<4;i++){
		int j= i;
		if (i==3) j=11; 
                float bc = QCD1->GetBinContent(j);
		QCD->SetBinContent(i,bc);
        }	
	if (dndm) convertToDNDM(QCD);
	applyStyle(QCD,kMagenta-10,1,1001);

	TH1F * ttbar1 = (TH1F*)(f->Get(dir+"/TT"));
	TH1F * ttbar = new TH1F("ttbar","",3,0,3);
	for(int i=1;i<4;i++){
		int j =i;
		if (i==3) j=11; 
                float bc = ttbar1->GetBinContent(j);
		ttbar->SetBinContent(i,bc);
        }
	if (dndm) convertToDNDM(ttbar);
	applyStyle(ttbar,kBlue-8,1,1001);


	TH1F * EWK1 = (TH1F*)(f->Get(dir+"/W"));
	EWK1->Add((TH1F*)(f->Get(dir+"/VV")));
	TH1F * EWK = new TH1F("EWK","",3,0,3); 
	for(int i=1;i<4;i++){
		int j =i;
		if (i==3) j=11; 
                float bc = EWK1->GetBinContent(j);
		EWK->SetBinContent(i,bc);
        }
	if (dndm) convertToDNDM(EWK);
	applyStyle(EWK,kRed-6,1,1001);

	if(f->Get(dir+"/ZLL")!=0)
		TH1F * ZEE1 = (TH1F*)(f->Get(dir+"/ZLL"));	  	
	if(f->Get(dir+"/ZL")!=0&&f->Get(dir+"/ZLL")==0)
		TH1F * ZEE1 = (TH1F*)(f->Get(dir+"/ZL"));
	if(f->Get(dir+"/ZJ")!=0&&f->Get(dir+"/ZLL")==0)
		ZEE1->Add((TH1F*)(f->Get(dir+"/ZJ")));
	TH1F * ZEE = new TH1F("ZEE","",3,0,3); 
	for(int i=1;i<4;i++){
		int j =i;
		if (i==3) j=11; 
                float bc = ZEE1->GetBinContent(j);
		ZEE->SetBinContent(i,bc);
        }

	if (dndm) convertToDNDM(ZEE);
	applyStyle(ZEE,kAzure-9,1,1001);	


	TH1F * ZTT1 = (TH1F*)(f->Get(dir+"/ZTT"));
	TH1F * ZTT = new TH1F("ZTT","",3,0,3); 
	for(int i=1;i<4;i++){
		int j =i;
		if (i==3) j=11; 
                float bc = ZTT1->GetBinContent(j);
		ZTT->SetBinContent(i,bc);
        }

	if (dndm) convertToDNDM(ZTT);
	applyStyle(ZTT,kOrange-4,1,1001);

	TH1F *signal=0;


	//more signal styles to be added later
	if(s==3) {
		TH1F * SM = (TH1F*)(f->Get(dir+"/ggH125"));
		TH1F *sm = SM;
		sm->Scale(100);//FIXME
		//TH1F * SM2 = (TH1F*)(f->Get(dir+"/qqH125"));
		//TH1F *sm2 = SM2;
		//sm2->Scale(100);//FIXME
		//sm->Add(sm2);
		sm->SetLineStyle(11);
		signal=sm;

		if (dndm) convertToDNDM(signal);

		applySignalStyle(signal);
	}

	THStack *hs = new THStack("hs","");
	THStack *hs1 = new THStack("hs1","");
	//hs->SetLineWidth(2);
	hs->Add(ttbar);
	hs->Add(QCD);
	hs->Add(EWK);
	if(channel == "#tau_{e}#tau_{h}") hs->Add(ZEE);
	if(channel == "#tau_{#mu}#tau_{h}") hs->Add(ZEE);
	hs->Add(ZTT);

	//if(s>0&(!log))
	//  hs->Add(signal);
	//if(sm) {
	//  TH1F * SM = (TH1F*)(f->Get(dir+"/ggH125"));
	//  SM->Add((TH1F*)(f->Get(dir+"/qqH125")));
	//  applyStyle(SM,kGray+2,1,3005);
	//  hs->Add(SM);
	// }
	//  else {
	//  hs->Add(MSSM);



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

       
        hs->GetXaxis()->SetBinLabel(1,"h^{#pm}");
        hs->GetXaxis()->SetBinLabel(2,"h^{#pm }#pi^{0}s");
        hs->GetXaxis()->SetBinLabel(3,"h^{#pm}h^{#mp}h^{#pm}");
	hs->Draw("HIST");

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
	if (!doRatio) hs->GetYaxis()->SetTitleOffset(1.4);

	if(dndm)
		hs->GetYaxis()->SetTitle("dN/d"+labelX);

	if(s>0)
		signal->Draw("HIST,SAME");
 
        data->GetXaxis()->SetBinLabel(1,"h^{#pm}");
        data->GetXaxis()->SetBinLabel(2,"h^{#pm }#pi^{0}s");
        data->GetXaxis()->SetBinLabel(3,"h^{#pm}h^{#mp}h^{#pm}");
	
	data->Draw("e,SAME");


	c->cd();

	if(doRatio){
		TH1F * data2 = (TH1F*)data->Clone("data");
		TH1F * mc = (TH1F*)(ttbar);
		mc->Add(QCD);
		mc->Add(EWK);
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

		data2->Divide(data2,mc);

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
		line->Draw();

	}

	c->cd();
	plotPad->cd();  

	TLegend *l = new TLegend(xR,0.7,xR+0.5,0.9);
	l->AddEntry(data,"Observed","P");

	l->AddEntry(ZTT,"Z#rightarrow#tau#tau","F");
	if(channel == "#tau_{e}#tau_{h}") l->AddEntry(ZEE,"Z#rightarrowee","F");
	if(channel == "#tau_{#mu}#tau_{h}") l->AddEntry(ZEE,"Z#rightarrow#mu#mu","F");
	l->AddEntry(EWK,"Electroweak","F");
	l->AddEntry(QCD,"QCD","F");
	l->AddEntry(ttbar,"t#bar{t}","F");


	if(log){
		if(s==3)
			l->AddEntry(signal,"100xSM H(125)#rightarrow#tau#tau","L");
	}
	else{
		if(s==3)
			l->AddEntry(signal,"100xSM H(125)#rightarrow#tau#tau","F");
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



