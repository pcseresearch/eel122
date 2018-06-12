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
void calo_plots_test(Int_t nrun) {
  cout << "Nrun= "<< nrun << endl;
  gROOT->Reset();
  gStyle->SetOptStat(0);
  //
TFile *froot =  new TFile(Form("eeltest%d.root",nrun));
TTree *troot = (TTree*)froot->Get("tdata");
 Int_t adc[32]; // data array for the adc
 troot->SetBranchAddress("adc",&adc);
 //
 Int_t center_block=10; // module number of center block
 const int Ncalo=21;
 // Int_t adc_index[Ncalo]={0,1,2,3,4,5,6,7,8,9,10,11,12,16,17,18,19,20,21,22,23};
 Int_t adc_index[Ncalo]={0,24,2,3,4,5,6,7,8,9,10,11,12,16,17,18,19,20,21,22,23};//adc 1 changed by 24 (Carlos)
 Double_t ped[Ncalo],pedsig[Ncalo];
 char *Modname[Ncalo]={"O-01","O-02","O-03","O-04","O-05","M-02A","M-02B","M-02C","M-02D","M-03A","M-03B","M-03C","M-03D","M-01A","M-01B","M-01C","M-01D","M-04A","M-04B","M-04C","M-04D"};
 TH1F *hadccent = new TH1F("hadccent"," ADC spectr cleanhits: ADC:",150,-200,1000);
  TH1F *hadcraw[Ncalo],*hadcpedsub[Ncalo];
 for ( int i = 0; i < Ncalo ; i++) {
   hadcraw[i] = new TH1F(Form("hadcraw%02d", i),Form("%s    raw adc[%2d]",Modname[i],i),300,0,300);
   hadcpedsub[i] = new TH1F(Form("hadcpedsub%02d", i),Form("%s    pedsub adc[%2d]",Modname[i],i),150,-200,1000);
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
       hadcraw[i]->Fit(f1,"","", hadcraw[i]->GetMean()-20, hadcraw[i]->GetMean()+20);
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
       hadcraw[i]->Fit(f1,"","", hadcraw[i]->GetMean()-20, hadcraw[i]->GetMean()+20);
       ped[i]=hadcraw[i]->GetFunction("f1")->GetParameter(1);
       pedsig[i]=hadcraw[i]->GetFunction("f1")->GetParameter(2);
    }
    // rescan with ped subtraction
    Bool_t clean_hit;
     cout << " Nentries = "<< nentries<< endl;
     Int_t chits=0;
     for (int ie = 0; ie < nentries; ie++) {
        troot->GetEntry(ie);
        clean_hit = 1;
        for ( int i = 0; i < Ncalo ; i++) {
	  //cout << ie <<" " << i << " " << adc[i] << " " << adc_index[i] << endl;
	  hadcpedsub[i]->Fill(adc[(adc_index[i])]-ped[i]);
          if ( i !=center_block) {
	    clean_hit = clean_hit&&((adc[adc_index[i]]-ped[i])<(pedsig[i]+10));
	    //cout << ie << " " << i << " " << clean_hit << " " << adc[adc_index[i]]-ped[i] << " " << pedsig[i]+10 << endl;
	  }
	}
        if (clean_hit) {
	    chits++;
            //cout << adc_index[center_block] << " " << adc[adc_index[center_block]] << endl;
            hadccent->Fill(adc[adc_index[center_block]]-ped[center_block]);
        }
      }
     cout << " Number of clean hits = " << chits << " " << float(chits)/float(nentries) << endl;
     // Plot histogram
    TCanvas *cadc = new TCanvas("cadc","ADC spectra ped sub OUTER type",800,800);
    cadc->Divide(2,3);
    cadc->SetLogy();
    for ( int i = 0; i < 5 ; i++) {
      cadc->cd(i+1);
      gPad->SetLogy();
       hadcpedsub[i]->Draw();
    }
   //
    TCanvas *cadc2 = new TCanvas("cadc2","ADC spectra ped sub MIDDLE type",800,800);
    cadc2->Divide(4,4);
    cadc2->SetLogy();
    for ( int i = 5; i < Ncalo ; i++) {
      cadc2->cd(i-4);
      gPad->SetLogy();
       hadcpedsub[i]->Draw();
    }
   //
     // Plot histogram
    TCanvas *cadccenter = new TCanvas("cadccenter","ADC spectra ped sub center block",800,800);
    cadccenter->Divide(1,1);
      cadccenter->cd(1);
       gPad->SetLogy();
       hadccent->Draw("same");
       hadccent->SetLineColor(3);
       f1->SetParameters(500, 500.,70.);
       hadccent->Fit(f1,"","", 180.,380.);
       Double_t mean=hadccent->GetFunction("f1")->GetParameter(1);
       Double_t sigma=hadccent->GetFunction("f1")->GetParameter(2);
       hadcpedsub[center_block]->Draw("same");

       cout << " Number of photo electrons = " << mean/sigma*mean/sigma << endl;
   //

//***********************************************************************************************************
// ADC spectra distributed as the set-up
//***********************************************************************************************************
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

       cHist->cd(1);       
       gPad->SetLogy();
       hadcpedsub[2]->Draw();

       cHist->cd(2);       
       TPad * pad1 = new TPad("pad1","pad1",0,0, xdim-sep, ydim-sep); //bottom-left
       TPad * pad2 = new TPad("pad2","pad2",xdim+sep,0, 1, ydim-sep);//bottom-right
       TPad * pad3 = new TPad("pad3","pad3",0,ydim+sep, xdim-sep, 1);//top-left
       TPad * pad4 = new TPad("pad4","pad4",xdim+sep,ydim+sep,1,1);//top-right

       pad1->Draw();
       pad1->cd();
       gPad->SetLogy();
       hadcpedsub[6]->Draw();
       cHist->cd(2);       

       pad2->Draw();    
       pad2->cd();     
       gPad->SetLogy();  
       hadcpedsub[7]->Draw();
       cHist->cd(2);       

       pad3->Draw();
       pad3->cd();     
       gPad->SetLogy();  
       hadcpedsub[5]->Draw();
       cHist->cd(2);       

       pad4->Draw();
       pad4->cd();     
       gPad->SetLogy();  
       hadcpedsub[8]->Draw();
       cHist->cd(2);       

       cHist->cd(3);   
       gPad->SetLogy();    
       hadcpedsub[0]->Draw();

       cHist->cd(4);       
    
       TPad * pad1 = new TPad("pad1","pad1",0,0, xdim-sep, ydim-sep);
       TPad * pad2 = new TPad("pad2","pad2",xdim+sep,0, 1, ydim-sep);
       TPad * pad3 = new TPad("pad3","pad3",0,ydim+sep, xdim-sep, 1);
       TPad * pad4 = new TPad("pad4","pad4",xdim+sep,ydim+sep,1,1);

       pad1->Draw();
       pad1->cd();     
       gPad->SetLogy();  
       hadcpedsub[17]->Draw();
       cHist->cd(4);       

       pad2->Draw();    
       pad2->cd();     
       gPad->SetLogy();  
       hadcpedsub[18]->Draw();
       cHist->cd(4);       

       pad3->Draw();
       pad3->cd();     
       gPad->SetLogy();  
       hadcpedsub[20]->Draw();
       cHist->cd(4);       

       pad4->Draw();
       pad4->cd();     
       gPad->SetLogy();  
       hadcpedsub[19]->Draw();
       cHist->cd(4);       

       cHist->cd(5);   
       gPad->SetLogy();    
       hadcpedsub[1]->Draw();

       cHist->cd(6);       
    
       TPad * pad1 = new TPad("pad1","pad1",0,0, xdim-sep, ydim-sep);
       TPad * pad2 = new TPad("pad2","pad2",xdim+sep,0, 1, ydim-sep);
       TPad * pad3 = new TPad("pad3","pad3",0,ydim+sep, xdim-sep, 1);
       TPad * pad4 = new TPad("pad4","pad4",xdim+sep,ydim+sep,1,1);

       pad1->Draw();
       pad1->cd();     
       gPad->SetLogy();  
       hadcpedsub[11]->Draw();
       cHist->cd(6);       

       pad2->Draw();    
       pad2->cd();     
       gPad->SetLogy();  
       hadcpedsub[12]->Draw();
       cHist->cd(6);       

       pad3->Draw();
       pad3->cd();     
       gPad->SetLogy();  
       hadcpedsub[10]->Draw();
       cHist->cd(6);       

       pad4->Draw();
       pad4->cd();     
       gPad->SetLogy();  
       hadcpedsub[9]->Draw();
       cHist->cd(6);       

       cHist->cd(7);   
       gPad->SetLogy();    
       hadcpedsub[3]->Draw();

       cHist->cd(8);       
    
       TPad * pad1 = new TPad("pad1","pad1",0,0, xdim-sep, ydim-sep);
       TPad * pad2 = new TPad("pad2","pad2",xdim+sep,0, 1, ydim-sep);
       TPad * pad3 = new TPad("pad3","pad3",0,ydim+sep, xdim-sep, 1);
       TPad * pad4 = new TPad("pad4","pad4",xdim+sep,ydim+sep,1,1);

       pad1->Draw();
       pad1->cd();     
       gPad->SetLogy();  
       hadcpedsub[16]->Draw();
       cHist->cd(8);       

       pad2->Draw();    
       pad2->cd();     
       gPad->SetLogy();  
       hadcpedsub[14]->Draw();
       cHist->cd(8);       

       pad3->Draw();
       pad3->cd();     
       gPad->SetLogy();  
       hadcpedsub[15]->Draw();
       cHist->cd(8);       

       pad4->Draw();
       pad4->cd();     
       gPad->SetLogy();  
       hadcpedsub[14]->Draw();
       cHist->cd(8);       

       cHist->cd(9);   
       gPad->SetLogy();    
       hadcpedsub[4]->Draw();



}
