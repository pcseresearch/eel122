#include <iostream>
#include <fstream>
#include <iomanip>
#include <TFile.h>
#include <TNtuple.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TMath.h>
#include <TBox.h>
#include <TPolyLine.h>
void calo_plots_mkj(Int_t nrun) {
  cout << "Nrun= "<< nrun << endl;
  gROOT->Reset();
  gStyle->SetOptStat(0);
  //
TFile *froot =  new TFile(Form("data/eeltest%d.root",nrun));
TTree *troot = (TTree*)froot->Get("tdata");
 Int_t adc[32]; // data array for the adc
 troot->SetBranchAddress("adc",&adc);
 //
 
 const int Ncalo=21;
if(nrun<=50){
Int_t adc_index[Ncalo]={0,1,2,3,4,5,6,7,8,9,10,11,12,16,17,18,19,20,21,22,23};
}
 else{
   Int_t adc_index[Ncalo]={0,2,4,3,24,5,6,7,8,9,10,11,12,16,17,18,19,20,21,22,23};//adc 1 changed by 24 (Carlos)
 }
if(nrun==65){
   Int_t adc_index[Ncalo]={0,24,2,3,4,5,6,7,8,9,10,11,12,16,17,18,19,20,21,22,23};//
}

//*************
//Locations   *
//*************
if(nrun<=49){
   int loc_index[21]={0,1,2,3,4,5,8,6,7,10,9,11,12,15,14,16,13,20,19,17,18};//use module #-1 first 5  are outer
   //"O-01","O-05","O-02","O-04","O-03", next 16 middle "M-2","M-3","M-1","M-4"
 int div[5]={5,1,9,7,3}; // index of 3x3 division for outer "O-01","O-02","O-03","O-04","O-05"
 int div2[4]={2,6,8,4}; // index of 3x3 division for middle :"M-2","M-3","M-1","M-4"
 }  
 if(nrun>=50){
   int loc_index[21]={0,1,2,3,4,5,8,6,7,10,9,11,12,15,14,16,13,20,19,17,18};//use module #-1 first 5  are outer
   //"O-01","O-05","O-02","O-04","O-03", next 16 middle "M-2","M-3","M-1","M-4"
   int div[5]={3,1,9,7,5}; // index of 3x3 division for outer "O-01","O-02","O-03","O-04","O-05"
   int div2[4]={2,6,8,4}; // index of 3x3 division for middle :"M-2","M-3","M-1","M-4"
 }  
 if(nrun>=52){
   int loc_index[21]={0,1,2,3,4,5,8,6,7,10,9,11,12,15,14,16,13,20,19,17,18};//use module #-1 first 5  are outer
   //"O-01","O-05","O-02","O-04","O-03", next 16 middle "M-2","M-3","M-1","M-4"
   int div[5]={3,5,9,7,1}; // index of 3x3 division for outer "O-01","O-02","O-03","O-04","O-05"
   int div2[4]={2,6,8,4}; // index of 3x3 division for middle :"M-2","M-3","M-1","M-4"
 }  
 if(nrun>=55){
   int loc_index[21]={0,1,2,3,4,5,8,6,7,10,9,11,12,15,14,16,13,20,19,17,18};//use module #-1 first 5  are outer
   //"O-01","O-05","O-02","O-04","O-03", next 16 middle "M-2","M-3","M-1","M-4"
   int div[5]={3,7,9,5,1}; // index of 3x3 division for outer "O-01","O-02","O-03","O-04","O-05"
   int div2[4]={2,6,8,4}; // index of 3x3 division for middle :"M-2","M-3","M-1","M-4"
 }  
 if(nrun>=60){
   int loc_index[21]={0,1,2,3,4,5,8,6,7,9,11,10,12,15,14,16,13,20,19,17,18};//use module #-1 first 5  are outer 	MODULE M-03 ROTATED
   //"O-01","O-05","O-02","O-04","O-03", next 16 middle "M-2","M-3","M-1","M-4"
   int div[5]={3,7,5,9,1}; // index of 3x3 division for outer "O-01","O-02","O-03","O-04","O-05"
   int div2[4]={2,6,8,4}; // index of 3x3 division for middle :"M-2","M-3","M-1","M-4"
 }  
 if(nrun>=61){
   int loc_index[21]={0,1,2,3,4,5,8,6,7,9,11,10,12,15,14,16,13,20,19,17,18};//use module #-1 first 5  are outer
   //"O-01","O-05","O-02","O-04","O-03", next 16 middle "M-2","M-3","M-1","M-4"
   int div[5]={3,7,8,9,1}; // index of 3x3 division for outer "O-01","O-02","O-03","O-04","O-05"
   int div2[4]={2,6,5,4}; // index of 3x3 division for middle :"M-2","M-3","M-1","M-4"
 }  
 if(nrun>=63){
   int loc_index[21]={0,1,2,3,4,5,8,6,7,9,11,10,12,15,14,16,13,20,19,17,18};//use module #-1 first 5  are outer
   //"O-01","O-05","O-02","O-04","O-03", next 16 middle "M-2","M-3","M-1","M-4"
   int div[5]={3,7,8,9,1}; // index of 3x3 division for outer "O-01","O-02","O-03","O-04","O-05"
   int div2[4]={2,6,4,5}; // index of 3x3 division for middle :"M-2","M-3","M-1","M-4"
 }  
if(nrun>=65){
   int loc_index[21]={0,1,2,3,4,5,8,6,7,9,11,10,12,16,15,13,14,20,19,17,18};//use module #-1 first 5  are outer   MODULE M-01 ROTATED
   //"O-01","O-05","O-02","O-04","O-03", next 16 middle "M-2","M-3","M-1","M-4"
   int div[5]={3,7,5,9,1}; // index of 3x3 division for outer "O-01","O-02","O-03","O-04","O-05"
   int div2[4]={2,6,8,4}; // index of 3x3 division for middle :"M-2","M-3","M-1","M-4"
 }  
if(nrun>=70){
int loc_index[21]={0,1,2,3,4,5,8,6,7,9,11,10,12,16,15,13,14,20,19,17,18};//use module #-1 first 5  are outer 
   //"O-01","O-05","O-02","O-04","O-03", next 16 middle "M-2","M-3","M-1","M-4"
   int div[5]={5,7,3,9,1}; // index of 3x3 division for outer "O-01","O-02","O-03","O-04","O-05"
   int div2[4]={2,6,8,4}; // index of 3x3 division for middle :"M-2","M-3","M-1","M-4"
 }  
if(nrun>=74){
int loc_index[21]={0,1,2,3,4,5,8,6,7,9,11,10,12,16,15,13,14,20,19,17,18};//use module #-1 first 5  are outer 
   //"O-01","O-05","O-02","O-04","O-03", next 16 middle "M-2","M-3","M-1","M-4"
   int div[5]={1,7,3,9,5}; // index of 3x3 division for outer "O-01","O-02","O-03","O-04","O-05"
   int div2[4]={2,6,8,4}; // index of 3x3 division for middle :"M-2","M-3","M-1","M-4"
 }  
if(nrun>=78){
int loc_index[21]={0,1,2,3,4,5,8,6,7,9,11,10,12,16,15,13,14,20,19,17,18};//use module #-1 first 5  are outer 
   //"O-01","O-05","O-02","O-04","O-03", next 16 middle "M-2","M-3","M-1","M-4"
   int div[5]={1,5,3,9,7}; // index of 3x3 division for outer "O-01","O-02","O-03","O-04","O-05"
   int div2[4]={2,6,8,4}; // index of 3x3 division for middle :"M-2","M-3","M-1","M-4"
 }  
if(nrun>=82){
int loc_index[21]={0,1,2,3,4,5,8,6,7,9,11,10,12,16,15,13,14,20,19,17,18};//use module #-1 first 5  are outer 
   //"O-01","O-05","O-02","O-04","O-03", next 16 middle "M-2","M-3","M-1","M-4"
   int div[5]={1,9,3,5,7}; // index of 3x3 division for outer "O-01","O-02","O-03","O-04","O-05"
   int div2[4]={2,6,8,4}; // index of 3x3 division for middle :"M-2","M-3","M-1","M-4"
 }  
if(nrun>=86){
int loc_index[21]={0,1,2,3,4,5,8,6,7,9,11,10,12,16,15,13,14,20,19,17,18};//use module #-1 first 5  are outer 
   //"O-01","O-05","O-02","O-04","O-03", next 16 middle "M-2","M-3","M-1","M-4"
   int div[5]={1,9,3,8,7}; // index of 3x3 division for outer "O-01","O-02","O-03","O-04","O-05"
   int div2[4]={2,6,5,4}; // index of 3x3 division for middle :"M-2","M-3","M-1","M-4"
 }  
if(nrun>=90){
int loc_index[21]={0,1,2,3,4,5,8,6,7,9,11,10,12,16,15,13,14,20,19,17,18};//use module #-1 first 5  are outer 
   //"O-01","O-05","O-02","O-04","O-03", next 16 middle "M-2","M-3","M-1","M-4"
   int div[5]={1,9,3,8,7}; // index of 3x3 division for outer "O-01","O-02","O-03","O-04","O-05"
   int div2[4]={5,6,2,4}; // index of 3x3 division for middle :"M-2","M-3","M-1","M-4"
 }  
if(nrun>=94||nrun==107){
int loc_index[21]={0,1,2,3,4,5,8,6,7,9,11,10,12,16,15,13,14,20,19,17,18};//use module #-1 first 5  are outer 
   //"O-01","O-05","O-02","O-04","O-03", next 16 middle "M-2","M-3","M-1","M-4"
   int div[5]={1,9,3,8,7}; // index of 3x3 division for outer "O-01","O-02","O-03","O-04","O-05"
   int div2[4]={6,5,2,4}; // index of 3x3 division for middle :"M-2","M-3","M-1","M-4"
 }  

if(nrun>=104&&nrun<107){
int loc_index[21]={0,1,2,3,4,5,8,6,7,9,11,10,12,16,15,13,14,20,19,17,18};//use module #-1 first 5  are outer 
   //"O-01","O-05","O-02","O-04","O-03", next 16 middle "M-2","M-3","M-1","M-4"
   int div[5]={1,9,3,8,7}; // index of 3x3 division for outer "O-01","O-02","O-03","O-04","O-05"
   int div2[4]={6,4,2,5}; // index of 3x3 division for middle :"M-2","M-3","M-1","M-4"
 }  

Int_t center_block = 0;
bool run2hit = false;
for(int i=0;i<5;i++){
    if(div[i]==5){
        center_block = loc_index[i];
		cout<<"center block is: "<<center_block<<endl;
		run2hit=true;
    }
}


 Double_t ped[Ncalo],pedsig[Ncalo];
 char *Modname[Ncalo]={"O-01","O-02","O-03","O-04","O-05","M-02A","M-02B","M-02C","M-02D","M-03A","M-03B","M-03C","M-03D","M-01A","M-01B","M-01C","M-01D","M-04A","M-04B","M-04C","M-04D"};
 TH1F *hadcraw[Ncalo],*hadcpedsub[Ncalo],*hadccent[Ncalo],*hadccut2[Ncalo];
 TH2F *h2hit[Ncalo];
 for ( int i = 0; i < Ncalo ; i++) {
   hadcraw[i] = new TH1F(Form("hadcraw%02d", i),Form("%s    raw adc[%2d]",Modname[i],i),300,0,300);
   hadcpedsub[i] = new TH1F(Form("hadcpedsub%02d", i),Form("%s    pedsub adc[%2d];ADC; Counts ",Modname[i],i),100,-100,2000);//range was/is till 1500. 3000 is because M-01-C has a huge gain
    hadccent[i] = new TH1F(Form("hadccent%02d", i),Form("%s    fitted adc;ADC; Counts ",Modname[i]),80,-100,2000);
    hadccut2[i] = new TH1F(Form("hadccut2%02d", i),Form("%s    fitted adc;ADC; Counts ",Modname[i]),100,-100,2000);
	h2hit[i] = new TH2F(Form("h2hit%02d", i),Form("%s    fitted adc;ADC; Counts ",Modname[i]),100,-100,1500,100,-100,1500);
 }
 //
 int nentries = (int)troot->GetEntries();
 int temp;
 cout << " Nentries = "<< nentries<< endl;
 for (int ie = 0; ie < nentries; ie++) {
   troot->GetEntry(ie);
   for ( int i = 0; i < Ncalo ; i++) {
     //cout << ie <<" " << i << " " << adc[i] << " " << adc_index[i] << endl;
     hadcraw[i]->Fill(adc[(adc_index[i])]);
   }
 }
     // Plot histogram
     TCanvas *craw = new TCanvas("craw","Raw ADC spectra OUTER type",800,800);
     craw->Divide(2,3);
    craw->SetLogy();
    TF1 *f1 = new TF1("f1", "gaus");
    for ( int i = 0; i < 5 ; i++) {
      craw->cd(i+1);
      gPad->SetLogy();
      hadcraw[i]->Draw();
      f1->SetParameters(500, hadcraw[i]->GetMean(),hadcraw[i]->GetRMS());
      hadcraw[i]->Fit(f1,"Q","", hadcraw[i]->GetMean()-20, hadcraw[i]->GetMean()+20);
      ped[i]=hadcraw[i]->GetFunction("f1")->GetParameter(1);
      pedsig[i]=hadcraw[i]->GetFunction("f1")->GetParameter(2);
    }
    //
    TCanvas *craw2 = new TCanvas("craw2","Raw ADC spectra MIDDLE type",800,800);
    craw2->Divide(4,4);
    for ( int i = 5; i < Ncalo ; i++) {
      craw2->cd(i-4);
       gPad->SetLogy();
       f1->SetParameters(500, hadcraw[i]->GetMean(),hadcraw[i]->GetRMS());
       hadcraw[i]->Fit(f1,"Q","", hadcraw[i]->GetMean()-20, hadcraw[i]->GetMean()+20);
       ped[i]=hadcraw[i]->GetFunction("f1")->GetParameter(1);
       pedsig[i]=hadcraw[i]->GetFunction("f1")->GetParameter(2);
    }
    // rescan with ped subtraction
    Bool_t clean_hit;
    Int_t number_hit;
    Int_t ghit;
    Double_t hit_adc[Ncalo];
    Double_t hit_mod[Ncalo];
    double hits[Ncalo+1];
    for( int i = 0; i <= Ncalo ; i++) {
      hits[i]=0;
	}
    cout << " Nentries = "<< nentries<< endl;
    Int_t chits[Ncalo]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
    for (int ie = 0; ie < nentries; ie++) {
      troot->GetEntry(ie);
      number_hit=0;
      for ( int i = 0; i < Ncalo ; i++) {
	  //cout << ie <<" " << i << " " << adc[i] << " " << adc_index[i] << endl;
	hadcpedsub[i]->Fill(adc[(adc_index[i])]-ped[i]);
	if ((adc[adc_index[i]]-ped[i])>2*pedsig[i]) {
	  hit_adc[number_hit]=(adc[adc_index[i]]-ped[i]);
	  hit_mod[number_hit]=i;
	  number_hit++;
	}
      }
      hits[number_hit]++;
      if (number_hit==1) {
	ghit=hit_mod[0];
	hadccent[ghit]->Fill(adc[adc_index[ghit]]-ped[ghit]);
	chits[ghit]++;
      }
//run2hit=false;
	if(number_hit==2 && run2hit){
		Int_t ghit0=hit_mod[0];//cout<<ghit0<<endl;
		Int_t ghit1=hit_mod[1];//cout<<ghit1<<endl;
		if(hit_mod[0]==center_block){
			h2hit[ghit1]->Fill(adc[adc_index[ghit1]]-ped[ghit1],adc[adc_index[ghit0]]-ped[ghit0]);
		}
		if(hit_mod[1]==center_block){
			h2hit[ghit0]->Fill(adc[adc_index[ghit0]]-ped[ghit0],adc[adc_index[ghit1]]-ped[ghit1]);			
		}
	}

    /**    if ((adc[adc_index[center_block]]-ped[i])>(pedsig[i]+50.)&&number_hit==1) {
	  //cout << " event = " << ie << " number of blocks hit = " << number_hit << endl;
	  for ( int i = 0; i < number_hit  ; i++) {
	    //cout << " mod = " << hit_mod[i] << " adc = " << hit_adc[i] << endl ;
	  }
        }*/
    }
    for ( int i = 0; i < Ncalo ; i++) {
      cout << Modname[i] << "# of clean hits = " << chits[i] << " Frac of total events =  " << float(chits[i])/float(nentries)  << endl;
    }

    if (1==1) { 
      // Plot histogram
    TCanvas *cadc = new TCanvas("cadc","ADC spectra ped sub OUTER type",800,800);
    cadc->Divide(2,3);
    cadc->SetLogy();
    Double_t min_fit,max_fit;
    for ( int i = 0; i < 5 ; i++) {
      cadc->cd(i+1);
      gPad->SetLogy();
      hadcpedsub[i]->Draw();
       hadccent[i]->Draw("same");
       hadccent[i]->SetLineColor(3);
       if ( hadccent[i]->Integral(10.,1500.) > 100.) {
	 f1->SetParameters(500, 500.,70.);

int count = 0;
bool go =true;
min_fit=50.;max_fit=1500.;
while(go==true){//this while loops performs newton's method type fitting loop
	hadccent[i]->Fit(f1,"Q","same", min_fit,max_fit);
	Double_t mean_old=hadccent[i]->GetFunction("f1")->GetParameter(1);
	Double_t mean_error_old=hadccent[i]->GetFunction("f1")->GetParError(1);
	Double_t sigma_old=hadccent[i]->GetFunction("f1")->GetParameter(2);
	Double_t sigma_error_old=hadccent[i]->GetFunction("f1")->GetParError(2);
	min_fit = mean_old*(.2+.1*count);
	max_fit = mean_old*(2.0+.1*count);
  	hadccent[i]->Fit(f1,"Q","same", min_fit,max_fit);
	Double_t mean=hadccent[i]->GetFunction("f1")->GetParameter(1);
	Double_t mean_error=hadccent[i]->GetFunction("f1")->GetParError(1);
	Double_t sigma=hadccent[i]->GetFunction("f1")->GetParameter(2);
	Double_t sigma_error=hadccent[i]->GetFunction("f1")->GetParError(2);
	double change = mean/(mean+mean_old);
	count++;
	if(change >=.49 && change<=.51){go=false;}	
	if(count>10){go=false;}
}	


/**double range_factor=1;              this was the attempt at chi-square optimization, not functional
double best_chi=999;
double best_factor;
for(int k=0;k<20;k++){
	min_fit=(1-range_factor)*mean;
	max_fit=(1+range_factor)*mean;if(max_fit>1500){max_fit=1500;}
	hadccent[i]->Fit(f1,"Q","same", min_fit,max_fit);
	Double_t chi_val = hadccent[i]->GetFunction("f1")->GetChisquare();
	Int_t dof = hadccent[i]->GetFunction("f1")->GetNDF();
        Double_t chiperdof = chi_val/float(dof);
//		cout<<"range factor = "<<range_factor<<"   Chi Squared Value = "<<chi_val << " " << chiperdof<<endl;
	if(chiperdof<best_chi){
		best_chi = chi_val;
		best_factor = range_factor;
//		cout<<"range factor = "<<range_factor<<"   Chi Squared Value = "<<chi_val << " " << chiperdof<<endl;
	}
	range_factor = range_factor-.05;
}
min_fit=(1-best_factor)*mean;
max_fit=(1+best_factor)*mean;if(max_fit>1500){max_fit=1500;}
hadent[i]->Fit(f1,"Q","same", min_fit,max_fit);*/


// cout  <<  Modname[i] << " Mean = " << mean <<" +/- "<<mean_error<< " Sigma = " << sigma << " +/- "<<sigma_error<< " Number of photo electrons = " << mean/sigma*mean/sigma<<" +/- "<<2*mean/sigma*mean/sigma*sqrt((mean_error/mean)*(mean_error/mean)+(sigma_error/sigma)*(sigma_error/sigma)) << endl;
double nphoto = mean/sigma*mean/sigma;
double photoerror = 2*mean/sigma*mean/sigma*sqrt((mean_error/mean)*(mean_error/mean)+(sigma_error/sigma)*(sigma_error/sigma));
if(photoerror/nphoto<=.5){
cout  <<"|-"<<endl<<"|"<<nrun<<"||"<< Modname[i] << "||" << mean <<"||"<<mean_error<< "||" << sigma << "||"<<sigma_error<< "||" << nphoto<<"||"<<photoerror<< endl;  }       }
    }
    //
    TCanvas *cadc2 = new TCanvas("cadc2","ADC spectra ped sub MIDDLE type",800,800);
    cadc2->Divide(4,4);
    cadc2->SetLogy();
    Double_t mean_st,sigma_st,cnt_st;
    
    for ( int i = 5; i < Ncalo ; i++) {
      cadc2->cd(i-4);
      gPad->SetLogy();
      hadcpedsub[i]->Draw();
      hadccut2[i]->Draw("same");
      hadccut2[i]->SetLineColor(5);
      hadccent[i]->Draw("same");
      hadccent[i]->SetLineColor(3);
      if ( hadccent[i]->Integral(10.,1500.) > 50.) {
	f1->SetParameters(500, 500.,70.);
int count = 0;
bool go =true;
min_fit=50.;max_fit=1500.;
hadccent[i]->Fit(f1,"Q","same", min_fit,max_fit);
Double_t mean = 0;
while(go){		//this while loops performs newton's method type fitting loop
	
	Double_t mean_old=hadccent[i]->GetFunction("f1")->GetParameter(1);
	Double_t mean_error_old=hadccent[i]->GetFunction("f1")->GetParError(1);
	Double_t sigma_old=hadccent[i]->GetFunction("f1")->GetParameter(2);
	Double_t sigma_error_old=hadccent[i]->GetFunction("f1")->GetParError(2);
	min_fit = mean_old*.8;
	max_fit = mean_old*1.2;if(max_fit>1500){max_fit=1500;}
  	hadccent[i]->Fit(f1,"Q","same", min_fit,max_fit);
	 mean=hadccent[i]->GetFunction("f1")->GetParameter(1);
	Double_t mean_error=hadccent[i]->GetFunction("f1")->GetParError(1);
	Double_t sigma=hadccent[i]->GetFunction("f1")->GetParameter(2);
	Double_t sigma_error=hadccent[i]->GetFunction("f1")->GetParError(2);
	double change = mean/(mean+mean_old);
	count++;
//	cout<<"*#*#**#*#*#*#*#*#*#*#*#*#*#*#*    "<<count<<endl;
	if(change >=.49 && change<=.51){go=false;}
	if(count>10){go=false;}
 }
//go=true;
//	cout  <<  Modname[i] << " Mean = " << mean <<" +/- "<<mean_error<< " Sigma = " << sigma << " +/- "<<sigma_error<< " Number of photo electrons = " << mean/sigma*mean/sigma<<" +/- "<<2*mean/sigma*mean/sigma*sqrt((mean_error/mean)*(mean_error/mean)+(sigma_error/sigma)*(sigma_error/sigma)) << endl;
double nphoto = mean/sigma*mean/sigma;
double photoerror = 2*mean/sigma*mean/sigma*sqrt((mean_error/mean)*(mean_error/mean)+(sigma_error/sigma)*(sigma_error/sigma));
if(true||photoerror/nphoto<=.15){
cout  <<"|-"<<endl<<"|"<<nrun<<"||"<< Modname[i] << "||" << mean <<"||"<<mean_error<< "||" << sigma << "||"<<sigma_error<< "||" << nphoto<<"||"<<photoerror<< endl;  }
    }
    }

    }
    //
    //
    
TCanvas *t5 = new TCanvas("t5","Pedestal Sigma & Hit counter",800,800);
t5->Divide(1,2);
t5->cd(1);
 TH1D *h1 = new TH1D("h1","Pedestal Sigma",21,0,21);
 for(int i=0;i<Ncalo;i++){
   h1->SetBinContent(i,pedsig[i]);
 }
 h1->Draw();   
t5->cd(2);
 TH1D *h2 = new TH1D("h2","Number of Modules hit per event",22,-0.5,21.5);
 for(int i=0;i<=Ncalo;i++){
   h2->SetBinContent(i+1,hits[i]);//cout<<"bin #"<<i+1<<"  :"<<hits[i]<<endl;
   //	for(int j=0;j<hits[i];j++){
   //	h2->Fill(i);
   //}
}
 h2->Draw();
 //********************************************************
 // ADC spectra distributed as the set-up
 //*********************************************************
 Double_t xdim = 0.48;       
 Double_t ydim = 0.48;       
 Double_t sep = 0.005;        // the space between pads
 
 Int_t ww = 900;              // dimensions for the canvas
 Int_t wh = 900;
 Int_t wtopx = 800;
 Int_t wtopy = 20;
 
 
 TCanvas * cHist = new TCanvas("cHist", "ADC spectra ped sub", wtopx, wtopy, ww, wh);
       cHist->Divide(3,3);
       cHist->SetLogy();
       
	//place Outer module histograms (in order of original adc)
       for(int i=0;i<5;i++){
	 cHist->cd(div[i]);
	 gPad->SetLogy();
	 hadcpedsub[loc_index[i]]->Draw();
	 hadccent[loc_index[i]]->Draw("same");
	 hadccent[loc_index[i]]->SetLineColor(3);
       }
	 
       //place middle module histograms
       int loc_ind = 5;
       for(int i=0;i<4;i++){
		cHist->cd(div2[i]);
		TPad * pad1 = new TPad("pad1","pad1",0,ydim+sep, xdim-sep, 1);//top-left
		TPad * pad2 = new TPad("pad2","pad2",xdim+sep,ydim+sep,1,1);//top-right
		TPad * pad3 = new TPad("pad3","pad3",0,0, xdim-sep, ydim-sep); //bottom-left
		TPad * pad4 = new TPad("pad4","pad4",xdim+sep,0, 1, ydim-sep);//bottom-right
		pad1->Draw();pad1->cd();gPad->SetLogy();		
		hadcpedsub[loc_index[loc_ind]]->Draw();
		hadccent[loc_index[loc_ind]]->Draw("same");
		hadccent[loc_index[loc_ind]]->SetLineColor(3);
		loc_ind++;
		cHist->cd(div2[i]);
		pad2->Draw();pad2->cd();gPad->SetLogy();
		hadcpedsub[loc_index[loc_ind]]->Draw();
		hadccent[loc_index[loc_ind]]->Draw("same");
		hadccent[loc_index[loc_ind]]->SetLineColor(3);
		loc_ind++;
		cHist->cd(div2[i]);
		pad3->Draw();pad3->cd();gPad->SetLogy();
		hadcpedsub[loc_index[loc_ind]]->Draw();
		hadccent[loc_index[loc_ind]]->Draw("same");
		hadccent[loc_index[loc_ind]]->SetLineColor(3);
		loc_ind++;
		cHist->cd(div2[i]);
		pad4->Draw();pad4->cd();gPad->SetLogy();
		hadcpedsub[loc_index[loc_ind]]->Draw();
		hadccent[loc_index[loc_ind]]->Draw("same");
		hadccent[loc_index[loc_ind]]->SetLineColor(3);
		loc_ind++;
		cHist->cd(div2[i]);		
	}
if(run2hit){
	TCanvas *doublehit = new TCanvas("2hit","Double hits",wtopx, wtopy, ww, wh);
	doublehit->Divide(3,3);
	//place Outer module histograms (in order of original adc)
    for(int i=0;i<5;i++){
		doublehit->cd(div[i]);
		h2hit[loc_index[i]]->Draw("colz");
	 
    }
	 
       //place middle module histograms
       int loc_ind = 5;
       for(int i=0;i<4;i++){
		doublehit->cd(div2[i]);
		TPad * pad1 = new TPad("pad1","pad1",0,ydim+sep, xdim-sep, 1);//top-left
		TPad * pad2 = new TPad("pad2","pad2",xdim+sep,ydim+sep,1,1);//top-right
		TPad * pad3 = new TPad("pad3","pad3",0,0, xdim-sep, ydim-sep); //bottom-left
		TPad * pad4 = new TPad("pad4","pad4",xdim+sep,0, 1, ydim-sep);//bottom-right
		pad1->Draw();pad1->cd();
		h2hit[loc_index[loc_ind]]->Draw("colz");
		
		loc_ind++;
		doublehit->cd(div2[i]);
		pad2->Draw();pad2->cd();
		h2hit[loc_index[loc_ind]]->Draw("colz");
		
		loc_ind++;
		doublehit->cd(div2[i]);
		pad3->Draw();pad3->cd();
		h2hit[loc_index[loc_ind]]->Draw("colz");
		
		loc_ind++;
		doublehit->cd(div2[i]);
		pad4->Draw();pad4->cd();
		h2hit[loc_index[loc_ind]]->Draw("colz");
		
		loc_ind++;
		doublehit->cd(div2[i]);		
	}
}
}
