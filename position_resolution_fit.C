#include <ostream>

Double_t fitfunction(Double_t *x, Double_t *par)
{
	Float_t xx = x[0];
	Double_t f = par[0]*(1.0-exp(xx/par[1]));
	return f;	
}

void position_resolution_fit() {
  
   //gStyle->SetOptFit(kFALSE);	
   gStyle->SetOptFit(1);	
   TCanvas *c1 = new TCanvas("c1","Position Resolution 12 GeV",200,10,700,500);
   FILE *f = fopen("position_resolution_calibration.dat","r");

   c1->SetFillColor(42);
   c1->SetGrid();

   const Int_t n = 10000;
   Double_t xactual[n],xmeasured[n];
   Int_t i=0;

   cout << "1";

   while (!feof(f)){
	  fscanf(f,"%lf %lf\n",&xactual[i],&xmeasured[i]);
          printf("%i %f %f\n",i,xactual[i],xmeasured[i]);
	  i++;
   }
   cout << "2";

   const Int_t n = i;

   gr = new TGraph(n,xmeasured,xactual);
   TF1 *pfit1 = new TF1("fitfunction",fitfunction,-6.0,1.0,2);
   pfit1->SetParameters(1.0,1.00);
   pfit1->SetParNames("A","B");
   
   gr->SetMarkerStyle(21);
   gr->SetTitle("Nominal Fit");
   gr->GetXaxis()->SetTitle("X Measured");
   gr->GetYaxis()->SetTitle("X Actual");
//   TAxis *axis = gr->GetXaxis();
//   axis->SetLimits(-5.,5.);
   gr->GetHistogram()->SetMaximum(10.0);
   gr->GetHistogram()->SetMinimum(-10.0);


   pfit1->SetLineColor(2);
   gr->Fit("fitfunction","V");
   Double_t pfit1chi2 = pfit1->GetChisquare();
   Double_t pfit1ndf = pfit1->GetNDF();
   Double_t pfit1chi2ndf = pfit1chi2/pfit1ndf;
//   printf("Fit 1: %f %f \n",pfit1chi2,pfit1ndf);
   gr->Draw("AP");

   // TCanvas::Update() draws the frame, after which one can change it
   c1->Update();
   c1->GetFrame()->SetFillColor(21);
   c1->GetFrame()->SetBorderSize(12);
   c1->Modified();
}
