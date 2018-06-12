void adc(Int_t run_num){
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
  tdata->Draw("adc[9]>>h(4096,0,4095)");
  cout<<h->Integral(1900,2750)<<endl;

}
