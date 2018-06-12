void draw(Int_t run_num){
  //-------------------Load root file -----------------------------------------
  TString file_name;
  file_name = Form("eeltest%d.root",run_num);
  TFile *f1 = new TFile(file_name);
  TTree *T = (TTree*)gDirectory->Get("tdata"); 
  if (!T) {
    cout << "Wrong filename: " << file_name << " or corrupted file" << endl;   
    return;
  }
  
  //-------------------Define histograms---------------------------------------
  TH1F *hADC[4];
  TH1F *hTDC[4]; 
  TH1F *hADC_cutTDC[4];
  TH1F *diff[3], *delta;

  TString hname, htit;
  Int_t i;
  Int_t adc_bin = 100;
  Double_t adc_min = 0.0, adc_max = 1000.0;
  Int_t tdc_bin = 100;
  Double_t tdc_min = 2000.0, tdc_max = 5000.0;
  for (i=0;i<4;i++){
    hname = Form("ADC_%d",i);
    htit = Form("ADC_%d spectrum",i);
    hADC[i] = new TH1F(hname, htit, adc_bin, adc_min, adc_max);
    hname = Form("TDC_%d",i);
    htit = Form("TDC_%d spectrum",i);
    hTDC[i] = new TH1F(hname, htit, tdc_bin, tdc_min, tdc_max);
    hname = Form("ADC_cutTDC_%d",i);
    htit = Form("ADC_%d spectrum with cut on TDC",i);
    hADC_cutTDC[i] = new TH1F(hname, htit, adc_bin, adc_min, adc_max);
  }
  for (i=0;i<3;i++){
    hname = Form("diff_%d",i+1);
    htit = Form("x%i = TDC%d - TDC%d, shifted on 1000 ch",i+1,2*i+2,2*i+1);
    hADC_cutTDC[i] = new TH1F(hname, htit, 100, 700, 1300);
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
  TString dr, dr1, cut, cut1;
  Double_t cut_min = 3600, cut_max=3700; 
  for(i=0;i<4;i++){
    c1->cd(i+1);
    hname = Form("ADC_%d",i);
    dr = Form("adc[%d]",i);
    dr = dr + ">>" + hname;
    T->Draw(dr);

    c2->cd(i+1);
    hname = Form("TDC_%d",i);
    dr = Form("tdc[%d]",i);
    dr = dr + ">>" + hname;
    T->Draw(dr);

    c3->cd(i+1);
    hname = Form("ADC_%d",i);
    dr = Form("adc[%d]",i);
    dr = dr + ">>" + hname;
    cut = Form("tdc[%d]>%lf && tdc[%d]<%lf",i,cut_min,i,cut_max);
    T->Draw(dr,cut);

    if(i<3){
      c4->cd(i+1);
      hname = Form("diff_%d",i+1);
      dr = Form("tdc[%d]-tdc[%d]", 2*i+2, 2*i+1);
      T->SetAlias(Form("x%d",i+1),dr);
      dr = dr + " + 1000";
      dr = dr + ">>" + hname;
      cut = Form("tdc[%d]>100 && tdc[%d]>100",2*i+1,2*i+2);
      T->Draw(dr,cut);
      if(i==0){
	cut1 = cut;
      }
      else{
	cut1 = cut1 + "&&" + cut; 
      }
    }
    else{
      c4->cd(4);
      T->Draw("(x1+x3)/2 - x2 + 1000>>delta",cut1);
    }
  }
}
