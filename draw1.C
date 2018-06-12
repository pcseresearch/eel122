void draw1(Int_t run_num){
  //-------------------Load root file -----------------------------------------
  TString file_name;
  file_name = Form("eeltest%d.root",run_num);
  TFile *f1 = new TFile(file_name);
  TTree *T = (TTree*)gDirectory->Get("tdata"); 

  //-------------------Crete alias with correct numbering----------------------
  T->SetAlias("tdc1","tdc[1]");
  T->SetAlias("tdc2","tdc[2]");
  T->SetAlias("tdc3","tdc[3]");
  T->SetAlias("tdc4","tdc[4]");
  T->SetAlias("tdc5","tdc[5]");
  T->SetAlias("tdc6","tdc[6]");
  T->SetAlias("adc1","adc[1]");
  T->SetAlias("adc2","adc[2]");
  T->SetAlias("adc3","adc[3]");
  T->SetAlias("adc4","adc[4]");
  T->SetAlias("adc5","adc[5]");
  T->SetAlias("adc6","adc[6]");
  T->SetAlias("x1","tdc4-tdc1");
  T->SetAlias("x2","tdc5-tdc2");
  T->SetAlias("x3","tdc6-tdc3");
  T->SetAlias("delta","x1+x3-2*x2");

  //-------------------Define cuts---------------------------------------------
  TCut cut_t1 = "tdc1>100";
  TCut cut_t2 = "tdc2>100";
  TCut cut_t3 = "tdc3>100";
  TCut cut_t4 = "tdc4>100";
  TCut cut_t5 = "tdc5>100";
  TCut cut_t6 = "tdc6>100";
  TCut cut_all = cut_t1 && cut_t2 && cut_t3 && cut_t4 && cut_t5 && cut_t6;

  //-------------------Define histograms---------------------------------------
  TH1F *hADC[4];
  TH1F *hTDC[4]; 
  TH1F *hADC_cutTDC[4];
  TH1F *hDiff[3], *delta;

  TString hname, htit;
  Int_t i;
  Int_t adc_bin = 100;
  Double_t adc_min = 0.0, adc_max = 1000.0;
  Int_t tdc_bin = 100;
  Double_t tdc_min = 1000.0, tdc_max = 2500.0;
  for (i=0;i<4;i++){
    hname = Form("ADC_%d",i+1);
    htit = Form("ADC_%d spectrum",i+1);
    hADC[i] = new TH1F(hname, htit, adc_bin, adc_min, adc_max);
    hname = Form("TDC_%d",i+1);
    htit = Form("TDC_%d spectrum",i+1);
    hTDC[i] = new TH1F(hname, htit, tdc_bin, tdc_min, tdc_max);
    hname = Form("ADC_cutTDC_%d",i+1);
    htit = Form("ADC_%d spectrum with cut on TDC",i+1);
    hADC_cutTDC[i] = new TH1F(hname, htit, adc_bin, adc_min, adc_max);
  }
  for (i=0;i<3;i++){
    hname = Form("diff_%d",i+1);
    htit = Form("x%i = TDC%d - TDC%d, shifted on 1000 ch",i+1,2*i+2,2*i+1);
    hDiff[i] = new TH1F(hname, htit, 100, 700, 1300);
  }
  TH1F *delta = new TH1F("delta","(x1+x3)/2 - x2, shifted on 1000 ch", 100, 700, 1300);


  //-------------------Create canvases-----------------------------------------
  TCanvas *c1 = new TCanvas("c1","c1",900,600);
  c1->Divide(2,2);
  TCanvas *c2 = new TCanvas("c2","c2",900,600);
  c2->Divide(2,2);
  TCanvas *c3 = new TCanvas("c3","c3",900,600);
  c3->Divide(2,2);
  TCanvas *c4 = new TCanvas("c4","c4",900,600);
  c4->Divide(2,2);
  
  //-------------------Fill and draw histograms--------------------------------
  c1->cd(1);
  T->Draw("tdc1>>TDC_1",cut_t1);
  c1->cd(2);
  T->Draw("tdc2>>TDC_2",cut_t2);
  c1->cd(3);
  T->Draw("tdc3>>TDC_3",cut_t3);
  c1->cd(4);
  T->Draw("tdc4>>TDC_4",cut_t4);

  c2->cd(1);
  T->Draw("adc1>>ADC_1");
  c2->cd(2);
  T->Draw("adc2>>ADC_2");
  c2->cd(3);
  T->Draw("adc3>>ADC_3");
  c2->cd(4);
  T->Draw("adc4>>ADC_4");

  c3->cd(1);
  T->Draw("adc1>>ADC_cutTDC_1",cut_t1);
  c3->cd(2);
  T->Draw("adc2>>ADC_cutTDC_2",cut_t1);
  c3->cd(3);
  T->Draw("adc3>>ADC_cutTDC_3",cut_t1);
  c3->cd(4);
  T->Draw("adc4>>ADC_cutTDC_4",cut_t1);

  c4->cd(1);
  T->Draw("x1+1000>>diff_1",cut_t1&&cut_t2);
  c4->cd(2);
  T->Draw("x2+1000>>diff_2",cut_t3&&cut_t4);
  c4->cd(3);
  T->Draw("x3+1000>>diff_3",cut_t5&&cut_t6);
  c4->cd(4);
  T->Draw("(x1+x3)/2 - x2 + 1000>>delta",cut_all);
}
