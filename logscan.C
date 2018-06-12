

void logscan(){	
	
double aa[21],bb[21];
int cc = 0;




TCanvas *c1 = new TCanvas("c1","HV_Mean Outer",40,40,1200,800);
c1->Divide(2,3);

for(int xx=1;xx<6;xx++){

c1->cd(xx);
// an ifstream per file
   ifstream in_1;
 
 

//here we open the ascii files
   in_1.open(Form("pmttest/pmt_O-0%i.dat",xx));
   
	
   Double_t adc_1[4],erradc_1[4];
   Double_t adc_2[4],erradc_2[4];
  
   Double_t volt[4]={log(1650),log(1700),log(1750),log(1800)};
   Double_t errvolt[4]={log(0.1),log(0.1),log(0.1),log(0.1)};


//here are the different lines in order to read and fill the histograms
   
   for(int i=0;i<4;i++){
       in_1 >>adc_1[i]>>erradc_1[i];
		 adc_1[i]=log(adc_1[i]);
		 erradc_1[i]=log(erradc_1[i]);
   }
	for(int i=0;i<4;i++){
       in_1 >>adc_2[i]>>erradc_2[i];
	    adc_2[i]=log(adc_2[i]);
		 erradc_2[i]=log(erradc_2[i]);
   }
	
   gPad->SetGridx();
   gPad->SetGridy();

   
   

   TLegend *legend = new TLegend(.15,.55,.43,.8);
	legend->SetHeader("Legend");

   TMultiGraph *mg = new TMultiGraph;
   
   TGraph *gr_1 = new TGraph(4,volt,adc_1);
   TGraph *gr_2 = new TGraph(4,volt,adc_2); 
   
   gr_1->SetFillColor(10);
   gr_1->SetLineColor(kRed);
   gr_1->SetMarkerColor(kRed);
   gr_1->SetMarkerStyle(20);
   gr_1->Fit("pol1");
   mg->Add(gr_1);

   TString s = Form("O-0%i in block  n=",xx);
   legend->AddEntry(gr_1,s,"lep");
	aa[cc]=pol1->GetParameter(0);bb[cc++]=pol1->GetParameter(1);

   gr_2->SetFillColor(10);
   gr_2->SetLineColor(kMagenta);
   gr_2->SetMarkerColor(kMagenta);
   gr_2->SetMarkerStyle(21);

//   gr_2->Fit("pol1");
//   mg->Add(gr_2);

   TString s = Form("O-0%i in box    n=",xx);
   legend->AddEntry(gr_2,s,"lep");
//aa[cc]=fit->GetParameter(0);bb[cc++]=fit->GetParameter(1);


   
   mg->Draw("ap");
   
   legend->Draw();


   gPad->Update(); 
   gPad->GetFrame()->SetBorderSize(12);
   mg->GetHistogram()->SetYTitle("ADC (u.a.)");
   mg->GetHistogram()->SetXTitle("HV (V)");
   gPad->Update();
   
}


//*********************
//  MIDDLE MODULES   **
//*********************
TCanvas *c2[5];
for(int a=1;a<=4;a++){

c2[a] = new TCanvas(Form("c1_%i",a),Form("HV_Mean PMT scans_%i",a),40,40,1200,800);
c2[a]->Divide(2,2);

// an ifstream per file
   ifstream in_1;
//here we open the ascii files
   in_1.open(Form("pmttest/pmt_M-0%i.dat",a));
   

for(int xx=1;xx<=4;xx++){
c2[a]->cd(xx);

	
   Double_t adc_1[4],erradc_1[4];
   Double_t adc_2[4],erradc_2[4];
  
 Double_t volt[4]={log(1650),log(1700),log(1750),log(1800)};
   Double_t errvolt[4]={log(0.1),log(0.1),log(0.1),log(0.1)};


//here are the different lines in order to read and fill the histograms
   
   for(int i=0;i<4;i++){
       in_1 >>adc_1[i]>>erradc_1[i];
		 adc_1[i]=log(adc_1[i]);
		 erradc_1[i]=log(erradc_1[i]);
   }
	for(int i=0;i<4;i++){
       in_1 >>adc_2[i]>>erradc_2[i];
		 adc_2[i]=log(adc_2[i]);
		 erradc_2[i]=log(erradc_2[i]);
   }
		
   gPad->SetGridx();
   gPad->SetGridy();

   TLegend *legend = new TLegend(.15,.55,.43,.8);
	legend->SetHeader("Legend");

   TMultiGraph *mg = new TMultiGraph;
   
   TGraph *gr_1 = new TGraph(4,volt,adc_1);
   TGraph *gr_2 = new TGraph(4,volt,adc_2); 
   
   gr_1->SetFillColor(10);
   gr_1->SetLineColor(kRed);
   gr_1->SetMarkerColor(kRed);
   gr_1->SetMarkerStyle(20);
   gr_1->Fit("pol1");
   mg->Add(gr_1);
 
   TString s = Form("M-0%i in block ",a);
   legend->AddEntry(gr_1,s,"lep");
	aa[cc]=pol1->GetParameter(0);bb[cc++]=pol1->GetParameter(1);


   gr_2->SetFillColor(10);
   gr_2->SetLineColor(kMagenta);
   gr_2->SetMarkerColor(kMagenta);
   gr_2->SetMarkerStyle(21);
//   gr_2->Fit("pol1");
 //  mg->Add(gr_2);
   TString s = Form("M-0%i in box",a);
   legend->AddEntry(gr_2,s,"lep");
//aa[cc]=fit->GetParameter(0);bb[cc++]=fit->GetParameter(1);


   
   mg->Draw("ap");
   
   legend->Draw();

   
}
}

for(int i=0;i<21;i++){
//	cout<<aa[i]<<"  "<<bb[i]<<endl;
cout<<exp((log(400)-aa[i])/bb[i])<<endl;
}
}
