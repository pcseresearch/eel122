#include "gausfit.C"
void test(){
  TTree *T = (TTree*)gDirectory->Get("tdata"); 
  Double_t mean, sigma;
  gausfit(T,mean,sigma);
  cout << "mean = " << mean << endl;
  cout << "sigma = " << sigma << endl;
}
