#include <iostream>
#include <fstream>
#include <TGClient.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TRandom.h>
#include <TGButton.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>
#include <RQ_OBJECT.h>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"

TH2Poly *h2p = new TH2Poly();
int numentries;
TFile *froot;
TTree *troot;
const int Ncalo=21;
Double_t hit_adc[Ncalo][11000];
Double_t hit_mod[Ncalo][11000];
double hitx[Ncalo]; 
double hity[Ncalo];
	
	
	
	
class MyMainFrame {
    RQ_OBJECT("MyMainFrame")
private:
    TGMainFrame         *fMain;
    TRootEmbeddedCanvas *fEcanvas;
    TFile *Read();
    Int_t i;
     TGTextEntry *jump;
 
public:
    MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h);
    virtual ~MyMainFrame();
    void DoDraw();
    void DoDraw2();
	 void Jump();
};



MyMainFrame::MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h) {
   // Create a main frame
   fMain = new TGMainFrame(p,w,h);

   // Create canvas widget
   fEcanvas = new TRootEmbeddedCanvas("Ecanvas",fMain,900,800);
   fMain->AddFrame(fEcanvas, new TGLayoutHints(kLHintsExpandX|kLHintsExpandY, 10,10,10,1));

   // Create a horizontal frame widget with buttons 
   TGHorizontalFrame *hframe = new TGHorizontalFrame(fMain,200,40);

   i=0;

	jump = new TGTextEntry(hframe,"000000");
	jump->Connect("ReturnPressed()","MyMainFrame",this,"Jump()");//action of the button
	jump->SetToolTipText("integers only", 100);
	hframe->AddFrame(jump,new TGLayoutHints(kLHintsCenterX,5,5,3,4));//position

   TGTextButton *draw2 = new TGTextButton(hframe,"&Previous");//button constructor
   draw2->Connect("Clicked()","MyMainFrame",this,"DoDraw2()");//action of the button 
   hframe->AddFrame(draw2, new TGLayoutHints(kLHintsCenterX,5,5,3,4));//position 

   TGTextButton *draw = new TGTextButton(hframe,"&Next");//button constructor
   draw->Connect("Clicked()","MyMainFrame",this,"DoDraw()");//action of the button 
   hframe->AddFrame(draw, new TGLayoutHints(kLHintsCenterX,5,5,3,4));//position 

   TGTextButton *exit = new TGTextButton(hframe,"&Exit","gApplication->Terminate(0)");

   hframe->AddFrame(exit, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
   fMain->AddFrame(hframe, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
	
   // Set a name to the main frame
   fMain->SetWindowName("Event Display");

   // Map all subwindows of main frame
   fMain->MapSubwindows();

   // Initialize the layout algorithm
   fMain->Resize(fMain->GetDefaultSize());

   // Map main frame
   fMain->MapWindow();
	DoDraw();
}

//this method listens to the "text entry zone"
void MyMainFrame::Jump() {
	h2p->ClearBinContents();

	std::string myString = jump->GetText();
	int value = atoi(myString.c_str());

	i=value;
	h2p->SetTitle(Form("Event %d",i));
	h2p->Draw("colz");
	
	for(int j=0;j<Ncalo;j++){
		int hold=hit_mod[j][i];
		h2p->Fill(hitx[hold],hity[hold],hit_adc[j][i]);
	}
	gStyle->SetOptStat(0);
	TCanvas *fCanvas = fEcanvas->GetCanvas();
    fCanvas->cd();
    fCanvas->Update();
}

//this method listens to the "previous" button
void MyMainFrame::DoDraw2() {
	h2p->ClearBinContents();
	i--;
	if(i<0){i=numentries;}
	h2p->SetTitle(Form("Event %d",i));
	h2p->Draw("colz");
	
	for(int j=0;j<Ncalo;j++){
		int hold=hit_mod[j][i];
		h2p->Fill(hitx[hold],hity[hold],hit_adc[j][i]);
	}
	gStyle->SetOptStat(0);
	TCanvas *fCanvas = fEcanvas->GetCanvas();
    fCanvas->cd();
    fCanvas->Update();
}

//this method listens to the "next" button
void MyMainFrame::DoDraw() {
	h2p->ClearBinContents();
	i++;
	h2p->SetTitle(Form("Event %d",i));
	if(i>=numentries){i=0;}
	h2p->Draw("colz");
	
	for(int j=0;j<Ncalo;j++){
		int hold=hit_mod[j][i];
		h2p->Fill(hitx[hold],hity[hold],hit_adc[j][i]);
	}
	gStyle->SetOptStat(0);
	TCanvas *fCanvas = fEcanvas->GetCanvas();
    fCanvas->cd();
    fCanvas->Update();
}




MyMainFrame::~MyMainFrame() {
   // Clean up used widgets: frames, buttons, layout hints
   fMain->Cleanup();
   delete fMain;
}

   





void position(int nrun){
	froot =  new TFile(Form("../data/calotest%d.root",nrun));
	troot = (TTree*)froot->Get("tdata");
	Int_t adc[32]; // data array for the adc
	troot->SetBranchAddress("adc",&adc);
	
 	if(nrun>=1){
		Int_t adc_index[Ncalo]={0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};// Index for 0-1,0-2,0-3,0-4,0-5,M1,M2,M3,M4
	}
	
	//*************
	//**Locations**
	//*************
	if(nrun>=1){
			int div[5]={5,2,4,6,8}; // index of 3x3 division for outer "O-01","O-02","O-03","O-04","O-05"
			int div2[4]={1,3,7,9}; // index of 3x3 division for middle :"M-1","M-2","M-3","M-4"
	 }

	Double_t adcgainchannel[Ncalo],ped[Ncalo],pedsig[Ncalo],adcgainfactor[Ncalo];
	FILE *f = fopen("../slacped_01.dat","r");
	FILE *h = fopen("../adcgainfactor.dat","r");
	for(int i=0;i<Ncalo;i++){
		fscanf(f,"%lf %lf\n", &ped[i], &pedsig[i]);
		fscanf(h,"%lf\n", &adcgainchannel[i]);
		adcgainfactor[i]=1000.0/adcgainchannel[i];
	}

 
	int nentries = (int)troot->GetEntries();
	numentries = nentries; //this is used as global variable for gui display
   Int_t number_hit;
   double hits[Ncalo+1];
   for(int i=0;i<=Ncalo;i++){
      hits[i]=0;
	}

   Int_t chits[Ncalo]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
   for(int ie=0;ie<nentries;ie++){
      troot->GetEntry(ie);
      number_hit=0;
		for(int i=0;i<Ncalo;i++){
			if((adc[adc_index[i]]-ped[i])>150) {
				  hit_adc[number_hit][ie]=(adcgainfactor[i]*(adc[adc_index[i]]-ped[i]));
				  hit_mod[number_hit][ie]=i;
				  number_hit++;
			}
     	}

   }
   
   
   
   
   	
//***************************************
//***************************************
//***********  Histo  *******************
//***************************************
    //******************************************************************
    //code to improve the color palette
    //******************************************************************
    Double_t r[5]    = { 0.00, 0.00, 0.87, 1.00, 0.51 }; 
    Double_t g[5]    = { 0.00, 0.81, 1.00, 0.20, 0.00 }; 
    Double_t b[5]    = { 0.51, 1.00, 0.12, 0.00, 0.00 }; 
    Double_t stop[5] = { 0.00, 0.34, 0.61, 0.84, 1.00 }; 
    Int_t FI = TColor::CreateGradientColorTable(5, stop, r, g, b, 255);
    gStyle->SetNumberContours(255); 
//  gStyle->setFontSize(12);
   //******************************************************************
	h2p->SetMaximum(3000);
	h2p->SetMinimum(10);
	double xbuildpoints[9] = {0,11.50,23.00,0,11.50,23.00,0,11.50,23.00};
	double ybuildpoints[9] = {23,23,23,11.5,11.5,11.5,0,0,0};
	double x[4];
	double y[4];

	
	for(int i=0;i<5;i++){
		x[0]=xbuildpoints[div[i]-1];
		y[0]=ybuildpoints[div[i]-1];
		
		x[1]=x[0];x[2]=x[1]+11.5;x[3]=x[2];
		y[1]=y[0]+11.5;y[2]=y[1];y[3]=y[0];
		
		h2p->AddBin(4,x,y);
		hitx[i]=x[0]+1;
		hity[i]=y[0]+1;
	//	cout<<"Module O-0"<<i+1<<" in slot "<<div[i]<<":    ("<<hitx[i]<<","<<hity[i]<<")"<<endl;
	}
	
Int_t count = 5;
	for(int j=0;j<4;j++){
		//Drawing pads in this order
		//		1  2
		//		4  3
		//
		
			x[0]=xbuildpoints[div2[j]-1]+0.00;//start drawing box in bottom left corner
			y[0]=ybuildpoints[div2[j]-1]+5.75;
			x[1]=x[0]+0.00;	x[2]=x[1]+5.75;	x[3]=x[2]+0.00;
			y[1]=y[0]+5.75;	y[2]=y[1]+0.00;	y[3]=y[2]-5.75;
			h2p->AddBin(4,x,y);
			hitx[count]=x[0]+1;
			hity[count]=y[0]+1;
			count++;
//			cout<<"Module M-0"<<i+1<<" in slot "<<div[i]<<":    ("<<hitx[i]<<","<<hity[i]<<")"<<endl;
			
			x[0]=x[4];//start drawing box in bottom left corner
			y[0]=y[4];
			x[1]=x[0]+0.00;	x[2]=x[1]+5.75;	x[3]=x[2]+0.00;
			y[1]=y[0]+5.75;	y[2]=y[1]+0.00;	y[3]=y[2]-5.75;
			h2p->AddBin(4,x,y);
			hitx[count]=x[0]+1;
			hity[count]=y[0]+1;
			count++;		
//			cout<<"Module M-0"<<i+1<<" in slot "<<div[i]<<":    ("<<hitx[i]<<","<<hity[i]<<")"<<endl;
			
			x[0]=x[0];//start drawing box in bottom left corner
			y[0]=y[0]-5.75;
			x[1]=x[0]+0.00;	x[2]=x[1]+5.75;	x[3]=x[2]+0.00;
			y[1]=y[0]+5.75;	y[2]=y[1]+0.00;	y[3]=y[2]-5.75;
			h2p->AddBin(4,x,y);
			hitx[count]=x[0]+1;
			hity[count]=y[0]+1;
			count++;	
//			cout<<"Module M-0"<<i+1<<" in slot "<<div[i]<<":    ("<<hitx[i]<<","<<hity[i]<<")"<<endl;
			
			x[0]=x[0]-5.75;//start drawing box in bottom left corner
			y[0]=y[0];
			x[1]=x[0]+0.00;	x[2]=x[1]+5.75;	x[3]=x[2]+0.00;
			y[1]=y[0]+5.75;	y[2]=y[1]+0.00;	y[3]=y[2]-5.75;
			h2p->AddBin(4,x,y);
			hitx[count]=x[0]+1;
			hity[count]=y[0]+1;
			count++;	
//			cout<<"Module M-0"<<i+1<<" in slot "<<div[i]<<":    ("<<hitx[i]<<","<<hity[i]<<")"<<endl;			
		}
	
	// Popup the GUI...
   new MyMainFrame(gClient->GetRoot(),200,200);	
}

