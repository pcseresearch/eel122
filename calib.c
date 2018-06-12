#include <TROOT.h>
#include <TMath.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TH1.h>
#include <TH2.h>
#include <TBox.h>
#include <TFrame.h>
#include <TPolyLine.h>
#include <TLegend.h>
#include <TMultiGraph.h>
#include <iostream>
#include <fstream>
#include <iomanip>
void calib() {
   gROOT->Reset();
   //#include "Riostream.h"

// an ifstream per file
   ifstream in_1;
   ifstream in_2;
   ifstream in_3;
   ifstream in_4;
   ifstream in_5;

//here we open the ascii files
   in_1.open("O-01.dat");
   in_2.open("O-02.dat");
   in_3.open("O-03.dat");
   in_4.open("O-04.dat");
   in_5.open("O-05.dat");

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
  
   
   TCanvas *c1 = new TCanvas("c1","HV vs Mean",200,10,700,500);

   c1->SetGridx();
   c1->SetGridy();

   TMultiGraph *mg = new TMultiGraph;
   for(int i=0;i<4;i++){
     errvolt[i]=errvolt[i]/(volt[i]*TMath::Log(10));
     volt[i]=TMath::Log10(volt[i]);
   erradc_1[i]=erradc_1[i]/(adc_1[i]*TMath::Log(10));
   erradc_2[i]=erradc_2[i]/(adc_2[i]*TMath::Log(10));
   erradc_3[i]=erradc_3[i]/(adc_3[i]*TMath::Log(10));
   erradc_4[i]=erradc_4[i]/(adc_4[i]*TMath::Log(10));
   erradc_5[i]=erradc_5[i]/(adc_5[i]*TMath::Log(10));
   adc_1[i]=TMath::Log10(adc_1[i]);
   adc_2[i]=TMath::Log10(adc_2[i]);
   adc_3[i]=TMath::Log10(adc_3[i]);
   adc_4[i]=TMath::Log10(adc_4[i]);
   adc_5[i]=TMath::Log10(adc_5[i]);
   }
   TGraphErrors *gr_1 = new TGraphErrors(4,volt,adc_1,errvolt,erradc_1);
   TGraphErrors *gr_2 = new TGraphErrors(4,volt,adc_2,errvolt,erradc_2); 
   TGraphErrors *gr_3 = new TGraphErrors(4,volt,adc_3,errvolt,erradc_3);
   TGraphErrors *gr_4 = new TGraphErrors(4,volt,adc_4,errvolt,erradc_4);
   TGraphErrors *gr_5 = new TGraphErrors(4,volt,adc_5,errvolt,erradc_5);

   gr_1->SetName("gr_1");
   gr_2->SetName("gr_2");
   gr_3->SetName("gr_3");
   gr_4->SetName("gr_4");
   gr_5->SetName("gr_5");

   gr_1->SetFillColor(10);
   gr_1->SetLineColor(kRed);
   gr_1->SetMarkerColor(kRed);
   gr_1->SetMarkerStyle(20);
   gr_1->Fit("pol1");
   mg->Add(gr_1);

   gr_2->SetFillColor(10);
   gr_2->SetLineColor(kMagenta);
   gr_2->SetMarkerColor(kMagenta);
   gr_2->SetMarkerStyle(21);
   gr_2->Fit("pol1");
   mg->Add(gr_2);

   gr_3->SetFillColor(10);
   gr_3->SetLineColor(kBlue);
   gr_3->SetMarkerColor(kBlue);
   gr_3->SetMarkerStyle(22);
   gr_3->Fit("pol1");
   mg->Add(gr_3);

   gr_4->SetFillColor(10);
   gr_4->SetLineColor(kGreen);
   gr_4->SetMarkerColor(kGreen);
   gr_4->SetMarkerStyle(23);
   gr_4->Fit("pol1");
   mg->Add(gr_4);

   gr_5->SetFillColor(10);
   gr_5->SetLineColor(kBlack);
   gr_5->SetMarkerColor(kBlack);
   gr_5->SetMarkerStyle(8);
   gr_5->Fit("pol1");
   mg->Add(gr_5);

   mg->Draw("ap");
   TLegend *legend = new TLegend(.15,.6,.25,.8);
	legend->SetHeader("Legend");
	legend->AddEntry("gr_1","O-01","lep");
	legend->AddEntry("gr_2","O-02","lep");
	legend->AddEntry("gr_3","O-03","lep");
	legend->AddEntry("gr_4","O-04","lep");
	legend->AddEntry("gr_5","O-05","lep");
	legend->Draw();

   //Add axis titles.
   //A graph is drawn using the services of the TH1F histogram class.
   //The histogram is created by TGraph::Paint.
   //TGraph::Paint is called by TCanvas::Update. This function is called by default
   //when typing <CR> at the keyboard. In a macro, one must force TCanvas::Update.

   c1->Update(); 
   c1->GetFrame()->SetBorderSize(12);
   mg->GetHistogram()->SetYTitle("Log(ADC) (u.a.)");
   mg->GetHistogram()->SetXTitle("Log(HV) (V)");
   
   c1->Modified();
}

