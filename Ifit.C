//
//   Example of a program to fit non-equidistant data points
//   =======================================================
//
//   The fitting function fcn is a simple chisquare function
//   The data consists of 5 data points (arrays x,y,z) + the errors in errorsz
//   More details on the various functions or parameters for these functions
//   can be obtained in an interactive ROOT session with:
//    Root > TMinuit *minuit = new TMinuit(10);
//    Root > minuit->mnhelp("*")  to see the list of possible keywords
//    Root > minuit->mnhelp("SET") explains most parameters
//Author: Rene Brun

#include "gausfit.C"
#include "TMinuit.h"
#define max_events 100000;
//Double_t mean[max_events];
Double_t tdc1[max_events],tdc2[max_events],tdc3[max_events],tdc4[max_events],tdc5[max_events],tdc6[max_events];
Double_t adc1[max_events],adc2[max_events],adc3[max_events],adc4[max_events],adc5[max_events],adc6[max_events];
Int_t opt_events_counter;
Double_t mean, sigma, min, max;

//______________________________________________________________________________
Double_t func(Double_t tdc1, Double_t tdc2, Double_t tdc3, Double_t tdc4, Double_t tdc5, Double_t tdc6,
	      Double_t adc1, Double_t adc2, Double_t adc3, Double_t adc4, Double_t adc5, Double_t adc6, Double_t *par)
{
  Double_t t1c, t2c, t3c, t4c, t5c, t6c, t1, t2, t3, t13, deltat;
  t1c = tdc1 - par[0]/adc1;
  t2c = tdc2 - par[0]/adc2;
  t3c = tdc3 - par[0]/adc3;
  t4c = tdc4 - par[0]/adc4;
  t5c = tdc5 - par[0]/adc5;
  t6c = tdc6 - par[0]/adc6;
  t1 = (t4c+t1c)/2;
  t2 = (t5c+t2c)/2;
  t3 = (t6c+t3c)/2;
  t13 = t1-t3;
  deltat = (t2 - (t1+t3)/2);
  return deltat;
}

//______________________________________________________________________________
void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{
  Int_t i;

  //calculate chisquare
  Double_t chisq = 0;
  Double_t diff;
  for (i=0;i<opt_events_counter; i++) {
    diff = mean - func(tdc1[i],tdc2[i],tdc3[i],tdc4[i],tdc5[i],tdc6[i],
		adc1[i],adc2[i],adc3[i],adc4[i],adc5[i],adc6[i],par);
    chisq  = chisq + diff*diff;
  }
  f = chisq;
}

//______________________________________________________________________________
void Ifit()
{
  Int_t adc[32], tdc[32];
  TTree *T = (TTree*)gDirectory->Get("tdata");
  T->SetBranchAddress("adc",adc);  
  T->SetBranchAddress("tdc",tdc);  

  
  Int_t i, j, nevents;
  Bool_t tdc_cut=false, adc_cut=false, t13_cut=false, delta_cut=false;
  Double_t tdc_c[6]; Double_t tt1, tt2, tt3, tt13;
  gausfit(T,mean,sigma);
  min = mean - 2*sigma;
  max = mean + 2*sigma;

  opt_events_counter=0;
  nevents = T->GetEntries();
  if (nevents>max_events){
    nevents = max_events;
  }
  for(i=0;i<nevents;i++){
    if(i%1000==0){
      cout << "event " << i << endl;
    }
    tdc_cut=false;
    adc_cut=false;
    t13_cut=false;
    delta_cut=false;
    T->GetEntry(i);
    if(tdc[1]>900 && tdc[2]>900 && tdc[3]>900 && tdc[4]>900 && tdc[5]>900 && tdc[6]>900){
      tdc_cut = true;
    }
    if(adc[1]>600 && adc[2]>600 && adc[3]>500 && adc[4]>500 && adc[5]>400 && adc[6]>400 &&
       adc[1]<1200 && adc[2]<1200 && adc[3]<1200 && adc[4]<1200 && adc[5]<1200 && adc[6]<1100){
      adc_cut = true;
    }
  
    if(adc_cut && tdc_cut){
      for(j=1;j<7;j++){
	tdc_c[j-1] = tdc[j] - 13000/adc[j];
      }
      tt1 = (tdc_c[3]+tdc_c[0])/2;
      tt2 = (tdc_c[4]+tdc_c[1])/2;
      tt3 = (tdc_c[5]+tdc_c[2])/2;
      tt13 = tt1-tt3;
      delta = tt2-(tt1+tt3)/2;
      if(tt13+39<10 && tt13+39>-10){
	t13_cut=true;
      }
      if(delta<max && delta>min){
	delta_cut=true;
      }
      if(t13_cut && delta_cut){
	tdc1[opt_events_counter] = tdc[1];
	tdc2[opt_events_counter] = tdc[2];
	tdc3[opt_events_counter] = tdc[3];
	tdc4[opt_events_counter] = tdc[4];
	tdc5[opt_events_counter] = tdc[5];
	tdc6[opt_events_counter] = tdc[6];
	adc1[opt_events_counter] = adc[1];
	adc2[opt_events_counter] = adc[2];
	adc3[opt_events_counter] = adc[3];
	adc4[opt_events_counter] = adc[4];
	adc5[opt_events_counter] = adc[5];
	adc6[opt_events_counter] = adc[6];
	opt_events_counter++;
      }
    }
  }
  cout << "Events to optimize: " << opt_events_counter << endl;


  
  TMinuit *gMinuit = new TMinuit(2);  //initialize TMinuit with a maximum of 5 params
  gMinuit->SetFCN(fcn);
  
  Double_t arglist[10];
  Int_t ierflg = 0;
  
  arglist[0] = 1;
  gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);
  
  // Set starting values and step sizes for parameters
  static Double_t vstart[1] = {13000.0};
  static Double_t step[1] = {1.0};
  gMinuit->mnparm(0, "a1", vstart[0], step[0], 0,0,ierflg);
  //  gMinuit->FixParameter(1); 

  // Now ready for minimization step
  arglist[0] = 500;
  arglist[1] = 1.;
  gMinuit->mnexcm("MIGRAD", arglist ,2,ierflg);
  
  // Print results
  Double_t amin,edm,errdef;
  Int_t nvpar,nparx,icstat;
  gMinuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
  gMinuit->mnprin(3,amin);

}

