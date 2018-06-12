#include <ostream>

Double_t fitfunction(Double_t *x, Double_t *par)
{
	Float_t xx = x[0];
	Double_t f = par[1]+par[0]/pow(xx,0.5);
	return f;	
}

void energy_resolution_fit2() {
  
   //gStyle->SetOptFit(kFALSE);	
   gStyle->SetOptFit(1);	
   TCanvas *c1 = new TCanvas("c1","Energy Resolution 3 GeV",200,10,700,500);
   FILE *f = fopen("energy_resolution_sigma2.dat","r");

   c1->SetFillColor(42);
   c1->SetGrid();

   const Int_t n = 10000;
   Double_t x[n],energy[n], adc[n], sigma[n], eratio[n];
   Int_t i=0;

   while (!feof(f)){
	  fscanf(f,"%lf %lf %lf\n",&energy[i],&adc[i],&sigma[i]);
          printf("%i %f %f %f\n",i,energy[i],adc[i],sigma[i]);
	  adc[i]=adc[i]+30.0;
	  eratio[i]=sigma[i]/adc[i];
	  x[i]=i+1;
	  i++;
   }

   const Int_t n = i;

   gr = new TGraph(n,energy,eratio);
   TF1 *pfit1 = new TF1("fitfunction",fitfunction,0.5,5.5,1);
   pfit1->SetParameters(0.00,0.10);
   pfit1->SetParNames("B","A");
   
   gr->SetMarkerStyle(21);
   gr->SetTitle("Nominal Fit");
   gr->GetXaxis()->SetTitle("Energy");
   gr->GetYaxis()->SetTitle("Resolution");
//   TAxis *axis = gr->GetXaxis();
//   axis->SetLimits(-5.,5.);
   gr->GetHistogram()->SetMaximum(0.15);
   gr->GetHistogram()->SetMinimum(0.0);


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
