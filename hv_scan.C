void hv_scan(){	
	
TCanvas *c1 = new TCanvas("c1","HV_Mean Outer Modules",40,40,1200,800);

// an ifstream per file
   ifstream in_1;
   ifstream in_2;
   ifstream in_3;
   ifstream in_4;
   ifstream in_5;

//here we open the ascii files
   in_1.open("data/O-01.dat");
   in_2.open("data/O-02.dat");
   in_3.open("data/O-03.dat");
   in_4.open("data/O-04.dat");
   in_5.open("data/O-05.dat");

   Double_t adc_1[4],erradc_1[4];
   Double_t adc_2[4],erradc_2[4];
   Double_t adc_3[4],erradc_3[4];
   Double_t adc_4[4],erradc_4[4];
   Double_t adc_5[4],erradc_5[4];
   Double_t volt[4]={1650,1700,1750,1800};
   Double_t errvolt[4]={0.1,0.1,0.1,0.1};


//here are the different lines in order to read and fill the histograms
   
   for(int i=0;i<4;i++)
   {
       in_1 >>adc_1[i]>>erradc_1[i];
       in_2 >>adc_2[i]>>erradc_2[i];
       in_3 >>adc_3[i]>>erradc_3[i];
       in_4 >>adc_4[i]>>erradc_4[i];
       in_5 >>adc_5[i]>>erradc_5[i];
   }

   c1->SetGridx();
   c1->SetGridy();

   TF1 *fit = new TF1("fit", "[0]*x^[1]",1500.,2500.);
   fit->SetParameter(0,1e-24);
   fit->SetParameter(1,7.5);
  

   TLegend *legend = new TLegend(.15,.6,.25,.8);
	legend->SetHeader("Legend");

   TMultiGraph *mg = new TMultiGraph;
   
   TGraphErrors *gr_1 = new TGraphErrors(4,volt,adc_1,errvolt,erradc_1);
   TGraphErrors *gr_2 = new TGraphErrors(4,volt,adc_2,errvolt,erradc_2); 
   TGraphErrors *gr_3 = new TGraphErrors(4,volt,adc_3,errvolt,erradc_3);
   TGraphErrors *gr_4 = new TGraphErrors(4,volt,adc_4,errvolt,erradc_4);
   TGraphErrors *gr_5 = new TGraphErrors(4,volt,adc_5,errvolt,erradc_5);

   gr_1->SetFillColor(10);
   gr_1->SetLineColor(kRed);
   gr_1->SetMarkerColor(kRed);
   gr_1->SetMarkerStyle(20);
   gr_1->Fit("fit");
   mg->Add(gr_1);
   double x = fit->GetParameter(1);
   double y = fit->GetParameter(0);
   TString s = Form("O-01  %f x ^ %f",y,x);
   legend->AddEntry(gr_1,s,"lep");


   gr_2->SetFillColor(10);
   gr_2->SetLineColor(kMagenta);
   gr_2->SetMarkerColor(kMagenta);
   gr_2->SetMarkerStyle(21);
   fit->SetLineColor(kMagenta);
   gr_2->Fit("fit");
   mg->Add(gr_2);
   x = fit->GetParameter(1);
   TString s = Form("O-02  n=%f",x);
   legend->AddEntry(gr_2,s,"lep");


   gr_3->SetFillColor(10);
   gr_3->SetLineColor(kBlue);
   gr_3->SetMarkerColor(kBlue);
   gr_3->SetMarkerStyle(22);
   fit->SetLineColor(kBlue);
   gr_3->Fit("fit");
   mg->Add(gr_3);
   x = fit->GetParameter(1);
   TString s = Form("O-03  n=%f",x);
   legend->AddEntry(gr_3,s,"lep");
	

   gr_4->SetFillColor(10);
   gr_4->SetLineColor(kGreen);
   gr_4->SetMarkerColor(kGreen);
   gr_4->SetMarkerStyle(23);
   fit->SetLineColor(kGreen);
   gr_4->Fit("fit");
   mg->Add(gr_4);
   x = fit->GetParameter(1);
   TString s = Form("O-04  n=%f",x);
   legend->AddEntry(gr_4,s,"lep");

   gr_5->SetFillColor(10);
   gr_5->SetLineColor(kBlack);
   gr_5->SetMarkerColor(kBlack);
   gr_5->SetMarkerStyle(8);
   fit->SetLineColor(kBlack);
   gr_5->Fit("fit");
   mg->Add(gr_5);
   x = fit->GetParameter(1);
   TString s = Form("O-05  n=%f",x);
   legend->AddEntry(gr_5,s,"lep");

   mg->Draw("ap");
   
   legend->Draw();

   //Add axis titles.
   //A graph is drawn using the services of the TH1F histogram class.
   //The histogram is created by TGraph::Paint.
   //TGraph::Paint is called by TCanvas::Update. This function is called by default
   //when typing <CR> at the keyboard. In a macro, one must force TCanvas::Update.

   c1->Update(); 
   c1->GetFrame()->SetBorderSize(12);
   mg->GetHistogram()->SetYTitle("ADC (u.a.)");
   mg->GetHistogram()->SetXTitle("HV (V)");
   
   c1->Modified();

//*********************
//  MIDDLE MODULES   **
//*********************



	TCanvas *c2 = new TCanvas("c2","HV_Mean Middle Modules",40,40,1200,800);
	c2->Divide(2,2);
  
for(int j=1;j<=4;j++){
	c2->cd(j);
	ifstream in_1;
	in_1.open(Form("data/M-0%d.dat",j));
	
	Double_t adc[16],erradc[16];
   Double_t adc_1[4],erradc_1[4];
   Double_t adc_2[4],erradc_2[4];
   Double_t adc_3[4],erradc_3[4];
   Double_t adc_4[4],erradc_4[4];
   Double_t volt[4]={1650,1700,1750,1800};
   Double_t errvolt[4]={0.1,0.1,0.1,0.1};	

	for(int i=0;i<16;i++){
       in_1 >>adc[i]>>erradc[i];
   }
   in_1.close();

	for(int i=0;i<4;i++){
      adc_1[i]=adc[i];
      adc_2[i]=adc[i+4];
      adc_3[i]=adc[i+8];
      adc_4[i]=adc[i+12];
      erradc_1[i]=erradc[i];
      erradc_2[i]=erradc[i+4];
      erradc_3[i]=erradc[i+8];
      erradc_4[i]=erradc[i+12];
   }   
   
   gPad->SetGridx();
   gPad->SetGridy();
	gPad->GetFrame()->SetBorderSize(12);
   
	TF1 *fit = new TF1("fit", "[0]*x^[1]",1500.,2500.);
   fit->SetParameter(0,1e-24);
   fit->SetParameter(1,8.);
   //	fit->SetParameter(1,8);

   TLegend *legend = new TLegend(.13,.75,.36,1.);
	legend->SetHeader("Legend");

   TMultiGraph *mg = new TMultiGraph;
   
   TGraphErrors *gr_1 = new TGraphErrors(4,volt,adc_1,errvolt,erradc_1);
   TGraphErrors *gr_2 = new TGraphErrors(4,volt,adc_2,errvolt,erradc_2); 
   TGraphErrors *gr_3 = new TGraphErrors(4,volt,adc_3,errvolt,erradc_3);
   TGraphErrors *gr_4 = new TGraphErrors(4,volt,adc_4,errvolt,erradc_4);
  
   gr_1->SetFillColor(10);
   gr_1->SetLineColor(kRed);
   gr_1->SetMarkerColor(kRed);
   gr_1->SetMarkerStyle(20);
	fit->SetLineColor(kRed);
   gr_1->Fit("fit");
   mg->Add(gr_1);
   double x = fit->GetParameter(1);
   TString s = Form("M-0%dA  n=%f",j,x);
   legend->AddEntry(gr_1,s,"lep");


   gr_2->SetFillColor(10);
   gr_2->SetLineColor(kMagenta);
   gr_2->SetMarkerColor(kMagenta);
   gr_2->SetMarkerStyle(21);
   fit->SetLineColor(kMagenta);
   gr_2->Fit("fit");
   mg->Add(gr_2);
   x = fit->GetParameter(1);
   TString s = Form("M-0%dB  n=%f",j,x);
   legend->AddEntry(gr_2,s,"lep");


   gr_3->SetFillColor(10);
   gr_3->SetLineColor(kBlue);
   gr_3->SetMarkerColor(kBlue);
   gr_3->SetMarkerStyle(22);
   fit->SetLineColor(kBlue);
   gr_3->Fit("fit");
   mg->Add(gr_3);
   x = fit->GetParameter(1);
   TString s = Form("M-0%dC  n=%f",j,x);
   legend->AddEntry(gr_3,s,"lep");
	

   gr_4->SetFillColor(10);
   gr_4->SetLineColor(kGreen);
   gr_4->SetMarkerColor(kGreen);
   gr_4->SetMarkerStyle(23);
   fit->SetLineColor(kGreen);
   gr_4->Fit("fit");
   mg->Add(gr_4);
   x = fit->GetParameter(1);
   TString s = Form("M-0%dD  n=%f",j,x);
   legend->AddEntry(gr_4,s,"lep");

   mg->Draw("ap");//gPad->SetLogy();
   legend->Draw();

	mg->GetHistogram()->SetYTitle("ADC (u.a.)");
   mg->GetHistogram()->SetXTitle("HV (V)");
	gPad->Modified();

}   


//*********************
//  PHOTO ELECTRONS  **
//*********************


	
TCanvas *c3 = new TCanvas("c3","Photoelectrons - Outer Modules",40,40,1200,800);

// an ifstream per file
   ifstream in_1;
   ifstream in_2;
   ifstream in_3;
   ifstream in_4;
   ifstream in_5;

//here we open the ascii files
   in_1.open("data/photo_O-01.dat");
   in_2.open("data/photo_O-02.dat");
   in_3.open("data/photo_O-03.dat");
   in_4.open("data/photo_O-04.dat");
   in_5.open("data/photo_O-05.dat");

   Double_t adc_1[4],erradc_1[4];
   Double_t adc_2[4],erradc_2[4];
   Double_t adc_3[4],erradc_3[4];
   Double_t adc_4[4],erradc_4[4];
   Double_t adc_5[4],erradc_5[4];
   Double_t volt[4]={1650,1700,1750,1800};
   Double_t errvolt[4]={0.1,0.1,0.1,0.1};


//here are the different lines in order to read and fill the histograms
   
   for(int i=0;i<4;i++)
   {
       in_1 >>adc_1[i]>>erradc_1[i];
       in_2 >>adc_2[i]>>erradc_2[i];
       in_3 >>adc_3[i]>>erradc_3[i];
       in_4 >>adc_4[i]>>erradc_4[i];
       in_5 >>adc_5[i]>>erradc_5[i];
   }

   c3->SetGridx();
   c3->SetGridy();

   

   TLegend *legend = new TLegend(.15,.6,.25,.8);
	legend->SetHeader("Legend");

   TMultiGraph *mg = new TMultiGraph;
   
   TGraphErrors *gr_1 = new TGraphErrors(4,volt,adc_1,errvolt,erradc_1);
   TGraphErrors *gr_2 = new TGraphErrors(4,volt,adc_2,errvolt,erradc_2); 
   TGraphErrors *gr_3 = new TGraphErrors(4,volt,adc_3,errvolt,erradc_3);
   TGraphErrors *gr_4 = new TGraphErrors(4,volt,adc_4,errvolt,erradc_4);
   TGraphErrors *gr_5 = new TGraphErrors(4,volt,adc_5,errvolt,erradc_5);

   gr_1->SetFillColor(10);
   gr_1->SetLineColor(kRed);
   gr_1->SetMarkerColor(kRed);
   gr_1->SetMarkerStyle(20);
   gr_1->Fit("pol2");            //currently set to pol2 instead of pol1 to show that it is not a line
   gr_1->GetFunction("pol2")->SetLineColor(kRed);
   mg->Add(gr_1);
   legend->AddEntry(gr_1,"O-01","lep");


   gr_2->SetFillColor(10);
   gr_2->SetLineColor(kMagenta);
   gr_2->SetMarkerColor(kMagenta);
   gr_2->SetMarkerStyle(21);
   gr_2->Fit("pol2");
   gr_2->GetFunction("pol2")->SetLineColor(kMagenta);
   mg->Add(gr_2);
   legend->AddEntry(gr_2,"O-02","lep");


   gr_3->SetFillColor(10);
   gr_3->SetLineColor(kBlue);
   gr_3->SetMarkerColor(kBlue);
   gr_3->SetMarkerStyle(22);
   gr_3->Fit("pol2");
   gr_3->GetFunction("pol2")->SetLineColor(kBlue);
   mg->Add(gr_3);
   legend->AddEntry(gr_3,"O-03","lep");
	

   gr_4->SetFillColor(10);
   gr_4->SetLineColor(kGreen);
   gr_4->SetMarkerColor(kGreen);
   gr_4->SetMarkerStyle(23);
   gr_4->Fit("pol2");
   gr_4->GetFunction("pol2")->SetLineColor(kGreen);
   mg->Add(gr_4);
   legend->AddEntry(gr_4,"O-04","lep");

   gr_5->SetFillColor(10);
   gr_5->SetLineColor(kBlack);
   gr_5->SetMarkerColor(kBlack);
   gr_5->SetMarkerStyle(8);
   gr_5->Fit("pol2");
   gr_5->GetFunction("pol2")->SetLineColor(kBlack);
   mg->Add(gr_5);
   legend->AddEntry(gr_5,"O-05","lep");

   mg->Draw("ap");
   
   legend->Draw();

   //Add axis titles.
   //A graph is drawn using the services of the TH1F histogram class.
   //The histogram is created by TGraph::Paint.
   //TGraph::Paint is called by TCanvas::Update. This function is called by default
   //when typing <CR> at the keyboard. In a macro, one must force TCanvas::Update.

   c3->Update(); 
   c3->GetFrame()->SetBorderSize(12);
   mg->GetHistogram()->SetYTitle("ADC (u.a.)");
   mg->GetHistogram()->SetXTitle("HV (V)");
   
   c3->Modified();



}
