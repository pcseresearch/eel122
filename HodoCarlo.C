//#include <math.h>
#include <iostream>


void HodoCarlo(){
	double l,w,d,d2;
	double cost, sint, phi,  cosp, sinp;
	double w,l,d, tant, tanty, xtop, ytop, xbot, ybot;
	int i,n;
	double hit1,hit2;
	double frac, dfrac;
	double x = 0.0;
/*	
	cout<<"number of events: ";
	cin>>n;
	cout<<endl;
*/
w=69;l=6;d=0;n=100000;	
double d2 = d+14;//how far apart
double w2 = 43;
double l2 = 2;
const int m = 4*n;
double rand[m];
TH1D *h5 = new TH1D("h5", "random gen",200,-0.2,2.0);
int neg=0;		 


//make random filled histo
	int a = pow(7,5);
	int M = pow(2,31)-1;
	int C = 0;
	int I = 1; 
for(int j=0;j<m;j++){
		int Inext = (a*I+C)%M;
		double InextD = Inext;
		double MD = M;
		rand[j]=InextD/MD;if(rand[j]<0){rand[j]=rand[j]*-1;}
		I=Inext;

		h5->Fill(rand[j]);
	//	cout<<j<<":  "<<rand[j]<<endl;
		if(rand[j]<0){neg++;}
	}	
//cout<<neg<<" events are negative"<<endl;

	const int nn = n;
	double costplot[nn],phiplot[nn],xtopplot[nn],ytopplot[nn];
	TH2D *top = new TH2D("top", "top", 200,-50,50,200,-50,50);
	TH2D *bot = new TH2D("bottom", "bottom", 400,-50,50,400,-50,50);
	TH2D *bot2 = new TH2D("bottom2", "bottom2", 400,-50,50,400,-50,50);


//main event loop
	hit1=0;
	for(int i=0;i<n;i++){
		if((i%5000)==0){
			cout<<"Event "<<i<<" out of "<<n<<endl;
		}	
	//	cout<<rand[i]<<"  "<<rand[i+n]<<"  "<<rand[i+2*n]<<"  "<<rand[i+3*n]<<endl;
		cost = pow(rand[i],0.333333333333);costplot[i]=cost;
		phi = rand[i+n]*2.0*3.14159265359;phiplot[i]=phi;
		xtop = rand[i+2*n]*w;xtopplot[i]=xtop;
		ytop = rand[i+3*n]*l;ytopplot[i]=ytop;
		top->Fill(xtop,ytop);
		sinp = sin(phi);
		cosp = cos(phi);
		sint = sqrt(1.0 - cost*cost);
		tant = sint/cost;
		tantx = tant*sinp;
		tanty = tant*cosp;
		xbot = xtop-tantx*d;
		ybot = ytop-tanty*d;
		bot->Fill(xbot,ybot);
		//cout<<"("<<xbot<<","<<ybot<<")"<<endl;		
		if((xbot>0.0&&xbot<w)&&(ybot>0.0&&ybot<l)){
			hit1++;
			xbot = xtop-tantx*d2;
			ybot = ytop-tanty*d2;
			bot2->Fill(xbot,ybot);
			if((xbot>0.0&&xbot<w2)&&(ybot>0.0&&ybot<l2)){hit2++;}
		}
		
	}
	if(true){
	TCanvas *c1 = new TCanvas("c1","Plots",40,40,800,600);
	c1->Divide(2,3);
	TH1D *h1 = new TH1D("h1", "cost distribution",200,-0.2,2.0);
	TH1D *h2 = new TH1D("h2", "phi distribution",200,-0.2,10.2);
	TH1D *h3 = new TH1D("h3", "x distribution",200,-0.2,10.2);
	TH1D *h4 = new TH1D("h4", "y distribution",200,-0.2,10.2);
	for(int i=0;i<n;i++){
		h1->Fill(costplot[i]);
		h2->Fill(phiplot[i]);
		h3->Fill(xtopplot[i]);
		h4->Fill(ytopplot[i]);
	}
		c1->cd(1);
		h1->Draw();
		c1->cd(2);
		h2->Draw();
		c1->cd(3);
		h3->Draw();
		c1->cd(4);
		h4->Draw();
		c1->cd(5);
		h5->Draw();
}
	
	
	TCanvas *c2 = new TCanvas("c2","Plots",10,10,600,600);
	c2->Divide(1,3);
	c2->cd(1);
	top->Draw("colz");
	c2->cd(2);
//	bot->Draw("colz");	
	c2->cd(3);
	bot2->Draw("colz");
	
	
	
	
	frac = ((double)hit2)/((double)n);
	dfrac = sqrt((double)hit2)/(double)n;
	cout<<endl<<endl<<"Had "<<hit2<<" hits out of "<<n<<" events"<<endl;
	cout<<"Accepted cosmic ray fraction:  "<<frac<<" +/- "<<dfrac<<endl;
	cout<<"Fraction of events lost due to scintillator position: "<<(hit1-hit2)/hit1<<endl;
	cout<<"Cosmic ray rate through top panel (hits/min): "<<1.04*l*w<<endl;
	cout<<"Cosmic ray rate through both panel [coincidence] (hits/min): "<<1.04*l*w*frac<<" +/- "<<1.04*l*w*dfrac<<endl;
	if((d>2*w)&&(d>2*l)){
		cout<<"Simple estimate (wl/d^2 formula) (hits/min): "<<.66*((w*l)*(w*l)/(d*d))<<endl;
	}
	else{
		cout<<"Simple estimate not reliable"<<endl;
	}
}


