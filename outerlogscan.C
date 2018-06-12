#include <iostream>

void outerlogscan(){	
	
double aa[21],bb[21];
int cc = 0;

const int numdata = 4;

cout<<"asdgasgasrg  3"<<endl;
TCanvas *c1 = new TCanvas("c1","HV_Mean Outer",40,40,1200,800);
c1->Divide(2,3);
cout<<"asdfasrf  0"<<endl;
for(int xx=1;xx<6;xx++){

c1->cd(xx);

		cout<<"ASDGASGasg    1"<<endl;
// an ifstream per file
   ifstream in_1;
//here we open the ascii files
   in_1.open(Form("pmttest/outerlogdata_O-0%i.dat",xx));
   
cout<<"asdgasasg  2"<<endl;	
   Double_t adc_1[numdata];
	Double_t erradc_1[numdata];
   Double_t volt[numdata];
   Double_t errvolt[numdata];


//here are the different lines in order to read and fill the histograms
   for(int i=0;i<numdata;i++){

		 in_1 >>volt[i]>>adc_1[i]>>erradc_1[i];
			


	    volt[i]=log(volt[i]);
		 adc_1[i]=log(adc_1[i]);
		 errvolt[i]=log(.1);
		 erradc_1[i]=log(erradc_1[i]);

   }

   gPad->SetGridx();
   gPad->SetGridy();

   
  

   TLegend *legend = new TLegend(.15,.55,.43,.8);
	legend->SetHeader("Legend");

   
   
   TGraph *gr_1 = new TGraph(numdata,volt,adc_1);

   
   gr_1->SetFillColor(10);
   gr_1->SetLineColor(kRed);
   gr_1->SetMarkerColor(kRed);
   gr_1->SetMarkerStyle(20);
   gr_1->Fit("pol1");
  

   TString s = Form("O-0%i in block  n=",xx);
   legend->AddEntry(gr_1,s,"lep");
	aa[cc]=pol1->GetParameter(0);bb[cc++]=pol1->GetParameter(1);


   
   gr_1->Draw("ap");
   
   legend->Draw();


   gPad->Update(); 
   gPad->GetFrame()->SetBorderSize(12);
   gr_1->GetHistogram()->SetYTitle("ADC (u.a.)");
   gr_1->GetHistogram()->SetXTitle("HV (V)");
   gPad->Update();
   
}

	for(int i=0;i<21;i++){
		//	cout<<aa[i]<<"  "<<bb[i]<<endl;
		cout<<exp((log(100)-aa[i])/bb[i])<<endl;
	}
}
