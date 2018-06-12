
/*****************OPTIONS*************************/
/**/const Int_t histo_max = 4596; //for adc histos only
/**/const bool y_axis_log = false;
/**/const bool show_rawdata = false;
/**/const bool show_tdcadj = false;
/**/const bool show_nongeo = false;
/**/const bool show_geo = false;
/**/const bool run_pedsig_hitcount = false;
/**/const bool run_double_hit = false;    
/**/const bool plot_center_alone = false;
/**/const bool plot_energy_sum = false;
/**/const bool plot_xymanypos = true;
/**/const bool do_fits = false;
/**/const int Ncalo=21;
/**/const int Ntdc=8;
/*************************************************/





void calo_plots_slac_multi(Int_t mynruns) {
	const int nruns = mynruns;
	Int_t nrun[nruns];
	for (int kk = 0; kk < nruns; kk++){
		cout << "Run Number " << kk+1 << "= ";
		cin >> nrun[kk];
		cout << endl;
	}
	gStyle->SetOptStat(1);
	Int_t adc[nruns][32]; // raw data array for the adc
	Int_t tdc[nruns][32]; // raw data array for the tdc
	TFile *froot[nruns];
	TTree *troot[nruns];
	Int_t hcolor[nruns];

	for (int kk = 0; kk < nruns; kk++){
	froot[kk] =  new TFile(Form("./data/calotest%d.root",nrun[kk]));
	troot[kk] = (TTree*)froot[kk]->Get("tdata");
	troot[kk]->SetBranchAddress("adc",&adc[kk]);
	troot[kk]->SetBranchAddress("tdc",&tdc[kk]);
	hcolor[kk]=kk+1;
	 }

	double adcoffset=4.0;
        double adccorr=300.0/327.0;
        double xposoffset=-0.000;
        double xposcorr=53.0000;
        double yposoffset=-0.000;
        double yposcorr=44.000;
        double energy = 1250.0;
        double es=0.05;
        signed double ep=0.05;
        double eca=5.582;
        double ecb=1.765;
	

	if(nrun[0]>=1){
		Int_t adc_index[Ncalo]={0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};// Index for 0-1,0-2,0-3,0-4,0-5,M1,M2,M3,M4
	//	int loc_index[21]={0,1,2,3,4,5,6,8,7,9,10,12,11,13,14,16,15,17,18,20,19};//use module #-1 first 5  are outer 
   //"O-01","O-05","O-02","O-04","O-03", next 16 middle "M-2","M-3","M-1","M-4"
	}
	

	//*************
	//**Locations**
	//*************
	if(nrun[0]>=1){
			int div[5]={5,2,4,6,8}; // index of 3x3 division for outer "O-01","O-02","O-03","O-04","O-05"
			int div2[4]={1,3,7,9}; // index of 3x3 division for middle :"M-1","M-2","M-3","M-4"
	 }


	
	//determines which module is in the center
	Int_t center_block = 0;
	bool run2hit_outer = false;
	for(int i=0;i<5;i++){
		 if(div[i]==5){
			  center_block = i;
			cout<<"center block is: "<<center_block<<endl;
			run2hit_outer=true;
		 }
	}
	int middle_block; //center module when it's a middle type (M-0X)
	bool run2hit_middle=false;
	for(int i=0;i<4;i++){
		if(div2[i]==5){
				middle_block = i*4+5; //sets middle_block to lowest adc of middle module 
				run2hit_middle=true;
		}
	}
	
	//bring in ped and pedsig and tdc corrections
	Double_t adcgainchannel[Ncalo],ped[Ncalo],pedsig[Ncalo],tdccorrection[Ntdc],adcgainfactor[Ncalo],xpos[Ncalo],ypos[Ncalo];
	FILE *f = fopen("slacped_01.dat","r");
	FILE *h = fopen("adcgainfactor.dat","r");
        FILE *ff = fopen("xposition.dat","r");
        FILE *fg = fopen("yposition.dat","r");
	for(int i=0;i<Ncalo;i++){
		fscanf(f,"%lf %lf\n", &ped[i], &pedsig[i]);
		fscanf(h,"%lf\n", &adcgainchannel[i]);
                fscanf(ff,"%lf\n", &xpos[i]);
                fscanf(fg,"%lf\n", &ypos[i]);
		adcgainfactor[i]=1000.0/adcgainchannel[i];
	}
	FILE *g = fopen("tdccorrection.dat","r");
	for(int i=0;i<Ntdc;i++){
		fscanf(g,"%lf\n", &tdccorrection[i]);
	}


//make our histos
	char *Modname[Ncalo]={"O-01","O-02","O-03","O-04","O-05","M-01A","M-01B","M-01C","M-01D","M-02A","M-02B","M-02C","M-02D","M-03A","M-03B","M-03C","M-03D","M-04A","M-04B","M-04C","M-04D"};
	char *tModname[Ntdc]={"Y Lower","Y Middle","Y Upper","X Left","Xmiddle","Y OR","X OR","XY AND"};
	TH1F *hadcraw[nruns][Ncalo],*hadcpedsub[nruns][Ncalo],*hadcpedsubtdc[nruns][Ncalo],*hadccent[nruns][Ncalo],*hadccut2[nruns][Ncalo];
	TH1F *htdcraw[nruns][Ntdc],*htdcadj[nruns][Ntdc];
	TH1F *hadcesum[nruns];
        TH1F *xposition[nruns];
        TH1F *yposition[nruns];
        TH1F *eupdown[nruns];
        TH1F *eleftright[nruns];
        TH2F *xyposition[nruns];
	TH2F *h2hit[nruns][4][Ncalo];
	
	TCanvas *xymanpos = new TCanvas("xymanypos","XY Position",600,600,600,600);

	for (int kk=0;kk<nruns;kk++){
	hadcesum[kk] = new TH1F(Form("hadcesum%02d",kk),"Energy Sum",100,-100,histo_max);
        xposition[kk] = new TH1F(Form("xposition%02d",kk),"X Position",300,-6.0,6.0);
        yposition[kk] = new TH1F(Form("yposition%02d",kk),"Y Position",300,-6.0,6.0);
        eupdown[kk] = new TH1F(Form("eupdown%02d",kk),"E Up Down",300,-0.5,0.5);
        eleftright[kk] = new TH1F(Form("eleftright%02d",kk),"E Left Right",300,-0.5,0.5);
        xyposition[kk] = new TH2F(Form("xyposition%02d",kk),"XY Position",300,-6.0,6.0,300,-6.0,6.0);
	for ( int i = 0; i < Ntdc ; i++) {
		htdcraw[kk][i] = new TH1F(Form("htdcraw%02d_%02d", kk, i),Form("%s    raw tdc",tModname[i]),300,3000,4500);
		htdcadj[kk][i] = new TH1F(Form("htdcadj%02d_%02d", kk, i),Form("%s    raw tdc",tModname[i]),200,3000,4000);
	}
	for ( int i = 0; i < Ncalo ; i++) {
		hadcraw[kk][i] = new TH1F(Form("hadcraw%02d_%02d", kk, i),Form("%s    raw adc",Modname[i]),300,0,300);
		hadcpedsub[kk][i] = new TH1F(Form("hadcpedsub%02d_%02d", kk, i),Form("%s    pedsub adc;ADC; Counts ",Modname[i]),500,-100,2000);
		hadcpedsubtdc[kk][i] = new TH1F(Form("hadcpedsubtdc%02d_%02d", kk, i),Form("%s    pedsub tdccut adc;ADC; Counts ",Modname[i]),500,-100,2000);
		hadccent[kk][i] = new TH1F(Form("hadccent%02d_%02d", kk, i),Form("%s    fitted adc;ADC; Counts ",Modname[i]),100,-100,histo_max);
		hadccut2[kk][i] = new TH1F(Form("hadccut2%02d_%02d", kk, i),Form("%s    fitted adc;ADC; Counts ",Modname[i]),100,-100,histo_max);
		for(int j=0; j<4; j++){	
			h2hit[kk][j][i] = new TH2F(Form("h2hit%02d__%02d_%02d",kk, j,i),Form("%s    fitted adc;ADC; Counts ",Modname[i]),100,-100,1500,100,-100,histo_max);
		}
	}
	
	int total_clean=0;
	int nentries = (int)troot[kk]->GetEntries();
	int temp;
	cout << " Nentries = "<< nentries<< endl;
	for (int ie = 0; ie < nentries; ie++) {
		troot[kk]->GetEntry(ie);
		for ( int i = 0; i < Ncalo ; i++) {
			//cout << ie <<" " << i << " " << adc[kk][i] << " " << adc_index[i] << endl;
		   hadcraw[kk][i]->Fill(adc[kk][(adc_index[i])]);
		}
		for ( int i = 0; i < Ntdc ; i++) {
			//cout << ie <<" " << i << " " << adc[kk][i] << " " << adc_index[i] << endl;
			htdcraw[kk][i]->Fill(tdc[kk][i]);//if(ie%100==0){cout<<tdc[kk][i]<<endl;}
			htdcadj[kk][i]->Fill(tdc[kk][i]+tdccorrection[i]);
		}
		
	}


// Plot histograms of raw data 

	// tdcs
	TCanvas *traw = new TCanvas(Form("traw%02d",kk),"Raw TDC spectra",800,800,800,800);
   	traw->Divide(3,3);
   	for ( int i = 0; i < Ntdc ; i++) {
      		traw->cd(i+1);
				if(y_axis_log){gPad->SetLogy();}
      		htdcraw[kk][i]->Draw();
	}
	 if(!show_rawdata){traw->Close();}
	//outer modules   
	TCanvas *craw = new TCanvas(Form("craw%02d",kk),"Raw ADC spectra OUTER type",800,800);
   craw->Divide(2,3);
   if(y_axis_log){craw->SetLogy();}
   TF1 *f1 = new TF1("f1", "gaus");
   for ( int i = 0; i < 5 ; i++) {
      craw->cd(i+1);
      if(y_axis_log){gPad->SetLogy();}
      hadcraw[kk][i]->Draw();
	}
   if(!show_rawdata){craw->Close();}
	//middle modules
   TCanvas *craw2 = new TCanvas(Form("craw2%02d",kk),"Raw ADC spectra MIDDLE type",800,800);
   craw2->Divide(4,4);
   for ( int i = 5; i < Ncalo ; i++) {
      craw2->cd(i-4);
   	if(y_axis_log){gPad->SetLogy();}
		hadcraw[kk][i]->Draw();
   }
	if(!show_rawdata){craw2->Close();}


   // rescan with ped subtraction

	//tdc with adjustment to gain match
	TCanvas *tadj = new TCanvas(Form("tadj%02d",kk),"Raw TDC spectra",800,800,800,800);
   	tadj->Divide(3,3);
   	for ( int i = 0; i < Ntdc ; i++) {
      		tadj->cd(i+1);
				if(y_axis_log){gPad->SetLogy();}
      		htdcadj[kk][i]->Draw();
	}
	if(!show_tdcadj){tadj->Close();}

   Bool_t clean_hit;
   Int_t number_hit;
   Int_t ghit;
   Double_t hit_adc[nruns][Ncalo];
   Int_t hit_mod[Ncalo];
   double hits[Ncalo+1];
   double holdesum;
   for( int i = 0; i <= Ncalo ; i++) {
      hits[i]=0;
	}
   cout << " Nentries = "<< nentries<< endl;
   Int_t chits[Ncalo]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
   for (int ie = 0; ie < nentries; ie++) {
      troot[kk]->GetEntry(ie);
      number_hit=0;
      holdesum=0;
      for ( int i = 0; i < Ncalo ; i++) {
	  //cout << ie <<" " << i << " " << adc[kk][i] << " " << adc_index[i] << endl;
			hadcpedsub[kk][i]->Fill(adccorr*(adcoffset+adcgainfactor[i]*(adc[kk][(adc_index[i])]-ped[i])));
	if (i==0 || i==1 || i==2 || i==3 || i==4 || i==7 || i==12 || i==14 || i==17){
			holdesum=holdesum+
			adccorr*(adcoffset+adcgainfactor[i]*(adc[kk][(adc_index[i])]-ped[i]));
			}
			//if(tdc[kk][1]>3700 && tdc[kk][1]<4300 && tdc[kk][4]>3700 && tdc[kk][4]<4300 ){hadcpedsubtdc[kk][i]->Fill(adccorr*(adcoffest+adcgainfactor[i]*(adc[kk][(adc_index[i])]-ped[i])));}
			//if(tdc[kk][7]>3200 && tdc[kk][7]<3600 ){hadcpedsubtdc[kk][i]->Fill(adccorr*(adcoffset+adcgainfactor[i]*(adc[kk][(adc_index[i])]-ped[i])));}
			if((adc[kk][adc_index[i]]-ped[i])>100.0) {
	  			hit_adc[kk][number_hit]=(adccorr*(adcoffset+adcgainfactor[i]*(adc[kk][adc_index[i]]-ped[i])));
	  			hit_mod[number_hit]=i;
	  			number_hit++;
			}
      }
      for ( int i = 0; i < Ncalo ; i++) {
        if(holdesum > (energy-200.0) && holdesum < (energy+200.0) ){
	        hadcpedsubtdc[kk][i]->Fill(adccorr*(adcoffset+adcgainfactor[i]*(adc[kk][(adc_index[i])]-ped[i])));
//      if (i==0 || i==2 || i==3 || i==4 || i==14 || i==17){
//              if (i==0 || i==2 || i==3 ){
//                      holdxpos=holdxpos+xpos[i]*adccorr*(adcoffset+adcgainfactor[i]*(adc[(adc_index[i])]-ped[i]));
//              }
//              if (i==0 || i==1 || i==4 ){
//                      holdypos=holdypos+ypos[i]*adccorr*(adcoffset+adcgainfactor[i]*(adc[(adc_index[i])]-ped[i]));
//              }
        }
      }
      if (holdesum > energy-200.0 && holdesum < energy+200.0){
      double eleft =  adccorr*(adcoffset+adcgainfactor[2]*(adc[kk][(adc_index[2])]-ped[2]));
      double emid =   adccorr*(adcoffset+adcgainfactor[0]*(adc[kk][(adc_index[0])]-ped[0]));
      double eright = adccorr*(adcoffset+adcgainfactor[3]*(adc[kk][(adc_index[3])]-ped[3]));
      double eupper = adccorr*(adcoffset+adcgainfactor[1]*(adc[kk][(adc_index[1])]-ped[1]));
      double elower = adccorr*(adcoffset+adcgainfactor[4]*(adc[kk][(adc_index[4])]-ped[4]));
//      cout << eleft << " " << eright << " " << emid << " " << eupper << " " << elower << endl;
//      cout << eleft+emid+eright << " " << eleft-eright << endl;
//      cout << eupper+elower+emid << " " << eupper-elower << endl << endl;
//      holdxpos=(eright-eleft)/abs(eright-eleft)*pow(abs(eleft-eright)/(eleft+emid+eright),0.5)*xposcorr+xposoffset;
//      holdypos=(eupper-elower)/abs(eupper-elower)*pow(abs(elower-eupper)/(eupper+emid+elower),0.5)*yposcorr+yposoffset;
      if(eleft >= eright){
                      eleftright[kk]->Fill( -1.0*(pow((+1.0*(eleft-eright)/(eleft+emid+eright)+es),ep)-pow(es,ep))  );
                      holdxpos=(        -1.0*(pow((+1.0*(eleft-eright)/(eleft+emid+eright)+es),ep)-pow(es,ep))*xposcorr+xposoffset  );
      }else
      {
                      eleftright[kk]->Fill( +1.0*(pow((-1.0*(eleft-eright)/(eleft+emid+eright)+es),ep)-pow(es,ep))  );
                      holdxpos=(        +1.0*(pow((-1.0*(eleft-eright)/(eleft+emid+eright)+es),ep)-pow(es,ep))*xposcorr+xposoffset  );
      }
      if(elower >= eupper){
                      eupdown[kk]->Fill(    -1.0*(pow((+1.0*(elower-eupper)/(elower+emid+eupper)+es),ep)-pow(es,ep))  );
                      holdypos=(        -1.0*(pow((+1.0*(elower-eupper)/(elower+emid+eupper)+es),ep)-pow(es,ep))*yposcorr+yposoffset  );
      }else
      {
                      eupdown[kk]->Fill(    +1.0*(pow((-1.0*(elower-eupper)/(elower+emid+eupper)+es),ep)-pow(es,ep))  );
                      holdypos=(        +1.0*(pow((-1.0*(elower-eupper)/(elower+emid+eupper)+es),ep)-pow(es,ep))*yposcorr+yposoffset  );
      }

      if(holdxpos >= 0.0){
              holdxpos=eca*(1.0-exp(-holdxpos/ecb));
      }else
      {
              holdxpos=-1.0*eca*(1.0-exp(holdxpos/ecb));
      }
      if(holdypos >= 0.0){
              holdypos=eca*(1.0-exp(-holdypos/ecb));
      }else
      {
              holdypos=-1.0*eca*(1.0-exp(holdypos/ecb));
      }

        xposition[kk]->Fill(holdxpos);
        yposition[kk]->Fill(holdypos);
        xyposition[kk]->Fill(holdxpos,holdypos);
      }

      hadcesum[kk]->Fill(holdesum);
		hits[number_hit]++;
		if(number_hit==1) {
			hadccent[kk][hit_mod[kk]]->Fill(hit_adc[kk][kk]);
			chits[ghit]++;
      }
	
		//2 hit method for outer modules
		if(number_hit==2 && run2hit_outer){
			Int_t ghit0=hit_mod[kk];//cout<<ghit0<<endl;
			Int_t ghit1=hit_mod[1];//cout<<ghit1<<endl;
			if(hit_mod[kk]==center_block){
				h2hit[kk][0][ghit1]->Fill(hit_adc[kk][1],hit_adc[kk][0]);
			}
			if(hit_mod[1]==center_block){
				h2hit[kk][0][ghit0]->Fill(hit_adc[kk][0],hit_adc[kk][1]);			
			}
		}
    
		//2 hit method for middle(2x2) modules
		if(number_hit==2 && run2hit_middle){
			Int_t ghit0=hit_mod[0];
			Int_t ghit1=hit_mod[1];
			if(ghit0==middle_block||ghit0==middle_block+1||ghit0==middle_block+2||ghit0==middle_block+3){
				if(ghit0==middle_block){
					h2hit[kk][0][ghit1]->Fill(hit_adc[kk][1],hit_adc[kk][0]);
				}
				if(ghit0==middle_block+1){
					h2hit[kk][1][ghit1]->Fill(hit_adc[kk][1],hit_adc[kk][0]);
				}
				if(ghit0==middle_block+2){
					h2hit[kk][2][ghit1]->Fill(hit_adc[kk][1],hit_adc[kk][0]);
				}
				if(ghit0==middle_block+3){
					h2hit[kk][3][ghit1]->Fill(hit_adc[kk][1],hit_adc[kk][0]);
				}

			}
			if(ghit1==middle_block||ghit1==middle_block+1||ghit1==middle_block+2||ghit1==middle_block+3){
				if(ghit1==middle_block){
					h2hit[kk][0][ghit0]->Fill(hit_adc[kk][0],hit_adc[kk][1]);
				}
				if(ghit1==middle_block+1){
					h2hit[kk][1][ghit0]->Fill(hit_adc[kk][0],hit_adc[kk][1]);
				}
				if(ghit1==middle_block+2){
					h2hit[kk][2][ghit0]->Fill(hit_adc[kk][0],hit_adc[kk][1]);
				}
				if(ghit1==middle_block+3){
					h2hit[kk][3][ghit0]->Fill(hit_adc[kk][0],hit_adc[kk][1]);
				}
			}		
		}//close 2hit middle method
   }//end of for(int ie = 0; ie < nentries; ie++)
  


	for(int i=0;i<Ncalo;i++){
 //     cout << Modname[i] << "# of clean hits = " << chits[i] << " Frac of total events =  " << float(chits[i])/float(nentries)  << endl;
		total_clean+=chits[i];
   }
	cout<<"Clean hits "<<total_clean<<endl;


  
      // Plot histograms with pedestal subtraction and gaussian fit    -  outer modules
   TCanvas *cadc = new TCanvas(Form("cadc%02d",kk),"ADC spectra ped sub OUTER type",800,800);
   cadc->Divide(2,3);
   Double_t min_fit,max_fit;
   for(int i=0;i<5;i++){
	   cadc->cd(i+1);					
		if(y_axis_log){gPad->SetLogy();}			
      hadcpedsub[kk][i]->Draw();		
      hadccent[kk][i]->Draw("same");
      hadccent[kk][i]->SetLineColor(3);
      if(hadccent[kk][i]->Integral(10.,1500.) > 100.) {
			f1->SetParameters(500, 500.,70.);
			int count = 0;
			min_fit=50.;max_fit=1500.;
			bool go = do_fits;
			while(go){	//this while loops performs iteration to adjust the fit
				hadccent[kk][i]->Fit(f1,"Q","same", min_fit,max_fit);
				Double_t mean_old=hadccent[kk][i]->GetFunction("f1")->GetParameter(1);
				Double_t mean_error_old=hadccent[kk][i]->GetFunction("f1")->GetParError(1);
				Double_t sigma_old=hadccent[kk][i]->GetFunction("f1")->GetParameter(2);
				Double_t sigma_error_old=hadccent[kk][i]->GetFunction("f1")->GetParError(2);
				min_fit = mean_old*(.2+.1*count);
				max_fit = mean_old*(2.0+.1*count);
			  	hadccent[kk][i]->Fit(f1,"Q","same", min_fit,max_fit);
				Double_t mean=hadccent[kk][i]->GetFunction("f1")->GetParameter(1);
				Double_t mean_error=hadccent[kk][i]->GetFunction("f1")->GetParError(1);
				Double_t sigma=hadccent[kk][i]->GetFunction("f1")->GetParameter(2);
				Double_t sigma_error=hadccent[kk][i]->GetFunction("f1")->GetParError(2);
				double change = mean/(mean+mean_old);
				count++;
				if(change >=.49 && change<=.51){go=false;}	
				if(count>10){go=false;}

			}	
			double nphoto = mean/sigma*mean/sigma;
			double photoerror = 2*mean/sigma*mean/sigma*sqrt((mean_error/mean)*(mean_error/mean)+(sigma_error/sigma)*(sigma_error/sigma));
			// cout<<Modname[i]<<" Mean = "<<mean<<" +/- "<<mean_error<<" Sigma = "<<sigma<<" +/- "<<sigma_error<<" Number of photo electrons = "<<nphoto<<" +/- "<<phototerror<<endl;
			if(photoerror/nphoto<=.5){
//				cout<<"||"<< Modname[i]<<"||"<<mean<<"||"<<mean_error<<"||"<<sigma<<"||"<<sigma_error<<"||"<<nphoto<<"||"<<photoerror<< endl;  
			}       
		}
   }//close for loop from line 200
   if(!show_nongeo){cadc->Close();}
	

	// Plot histograms with pedestal subtraction and gaussian fit    -  middle modules
   TCanvas *cadc2 = new TCanvas(Form("cadc2%02d",kk),"ADC spectra ped sub MIDDLE type",800,800);
   cadc2->Divide(4,4);
   Double_t mean_st,sigma_st,cnt_st;
	for(int i=5;i<Ncalo;i++){
		cadc2->cd(i-4);												
      if(y_axis_log){gPad->SetLogy();}						
      hadcpedsub[kk][i]->Draw();										
      hadccent[kk][i]->Draw("same");											
      hadccent[kk][i]->SetLineColor(3);									
      if(hadccent[kk][i]->Integral(10.,histo_max-500.) > 100.){			
			f1->SetParameters(500, 500.,70.);											
			int count = 0;
			min_fit=50.;max_fit=histo_max-500.;			
			Double_t mean = 0;
			bool go = do_fits;			
			while(go){		//this while loops performs iteration to adjust the fit
				hadccent[kk][i]->Fit(f1,"Q","same", min_fit,max_fit);
				Double_t mean_old=hadccent[kk][i]->GetFunction("f1")->GetParameter(1); 
				Double_t mean_error_old=hadccent[kk][i]->GetFunction("f1")->GetParError(1);	
				Double_t sigma_old=hadccent[kk][i]->GetFunction("f1")->GetParameter(2);
				Double_t sigma_error_old=hadccent[kk][i]->GetFunction("f1")->GetParError(2);
				min_fit = mean_old*.8;
				max_fit = mean_old*1.2;if(max_fit>1500){max_fit=1500;}
		  		hadccent[kk][i]->Fit(f1,"Q","same", min_fit,max_fit);
				mean=hadccent[kk][i]->GetFunction("f1")->GetParameter(1);
				Double_t mean_error=hadccent[kk][i]->GetFunction("f1")->GetParError(1);
				Double_t sigma=hadccent[kk][i]->GetFunction("f1")->GetParameter(2);
				Double_t sigma_error=hadccent[kk][i]->GetFunction("f1")->GetParError(2);
				double change = mean/(mean+mean_old);
				count++;
				if(change >=.49 && change<=.51){go=false;}
				if(count>10){go=false;}
			}
			double nphoto = mean/sigma*mean/sigma;
			double photoerror = 2*mean/sigma*mean/sigma*sqrt((mean_error/mean)*(mean_error/mean)+(sigma_error/sigma)*(sigma_error/sigma));
			//cout<<Modname[i]<<" Mean = "<<mean<<" +/- "<<mean_error<<" Sigma = "<<sigma<<" +/- "<<sigma_error<<" Number of photo electrons = "<<nphoto<<" +/- "<<phototerror<<endl;
			if(photoerror/nphoto<=.15){
				cout<<"||"<<Modname[i]<<"||"<<mean<<"||"<<mean_error<<"||"<<sigma<<"||"<<sigma_error<<"||"<< nphoto<<"||"<<photoerror<< endl;  
			}
		}//close if loop ~line 253
   }//closes for loop  ~line 245
  	if(!show_nongeo){cadc2->Close();}

		
	//shows pedestal sigma for each module and distribution of modules hit per event
	if(run_pedsig_hitcount){
		TCanvas *t5 = new TCanvas(Form("t5%02d",kk),"Pedestal Sigma & Hit counter",800,800);
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
			h2->SetBinContent(i+1,hits[i]);
			for(int j=0;j<hits[i];j++){
				h2->Fill(i);
			}
		}
		h2->Draw();
	}



 //********************************************************
 // ADC spectra distributed as the set-up  - geo display
 //*********************************************************
 Double_t xdim = 0.48;       
 Double_t ydim = 0.48;       
 Double_t sep = 0.005;        // the space between pads
 
 Int_t ww = 600;              // dimensions for the canvas
 Int_t wh = 600;
 Int_t wtopx = 800;
 Int_t wtopy = 20;
 
	if(show_geo){
		TCanvas * cHist = new TCanvas(Form("cHist%02d",kk), "ADC spectra ped sub", wtopx, wtopy, ww, wh);
		cHist->Divide(3,3);
		cHist->SetLogy();
		    
		//place Outer module histograms (in order of original adc)
		for(int i=0;i<5;i++){
			cHist->cd(div[i]);
			if(y_axis_log){gPad->SetLogy();}
			hadcpedsub[kk][i]->Draw();
			hadccent[kk][i]->Draw("same");
			hadccent[kk][i]->SetLineColor(3);
		}
		 
	  	//place middle module histograms
		int loc_ind = 5;
		for(int i=0;i<4;i++){
			cHist->cd(div2[i]);
			TPad * pad1 = new TPad("pad1","pad1",0,ydim+sep, xdim-sep, 1);//top-left
			TPad * pad2 = new TPad("pad2","pad2",xdim+sep,ydim+sep,1,1);//top-right
			TPad * pad3 = new TPad("pad3","pad3",xdim+sep,0, 1, ydim-sep); //bottom-right
			TPad * pad4 = new TPad("pad4","pad4",0,0, xdim-sep, ydim-sep);//bottom-left
			pad1->Draw();pad1->cd();if(y_axis_log){gPad->SetLogy();}		
			hadcpedsub[kk][loc_ind]->Draw();
			hadccent[kk][loc_ind]->Draw("same");
			hadccent[kk][loc_ind]->SetLineColor(3);
			loc_ind++;
			cHist->cd(div2[i]);
			pad2->Draw();pad2->cd();if(y_axis_log){gPad->SetLogy();}
			hadcpedsub[kk][loc_ind]->Draw();
			hadccent[kk][loc_ind]->Draw("same");
			hadccent[kk][loc_ind]->SetLineColor(3);
			loc_ind++;
			cHist->cd(div2[i]);
			pad3->Draw();pad3->cd();if(y_axis_log){gPad->SetLogy();}
			hadcpedsub[kk][loc_ind]->Draw();
			hadccent[kk][loc_ind]->Draw("same");
			hadccent[kk][loc_ind]->SetLineColor(3);
			loc_ind++;
			cHist->cd(div2[i]);
			pad4->Draw();pad4->cd();if(y_axis_log){gPad->SetLogy();}
			hadcpedsub[kk][loc_ind]->Draw();
			hadccent[kk][loc_ind]->Draw("same");
			hadccent[kk][loc_ind]->SetLineColor(3);
			loc_ind++;
			cHist->cd(div2[i]);		
		}
	}//close show_geo option

 //********************************************************
 // Double hit display methods
 //*********************************************************

	if(run_double_hit){
		if(run2hit_outer){//loop for outer module double hits
			TCanvas *doublehit = new TCanvas(Form("2hit%02d",kk),"Double hits",wtopx, wtopy, ww, wh);
			doublehit->Divide(3,3);
			//place Outer module histograms (in order of original adc)
			for(int j=0;j<1;j++){	 
				for(int i=0;i<5;i++){
					doublehit->cd(div[i]);
					h2hit[kk][j][i]->Draw("colz");
				}
				 
		  	   //place middle module histograms
		  	   int loc_ind = 5;
		  	   for(int i=0;i<4;i++){
					doublehit->cd(div2[i]);
					TPad * pad1 = new TPad("pad1","pad1",0,ydim+sep, xdim-sep, 1);//top-left
					TPad * pad2 = new TPad("pad2","pad2",xdim+sep,ydim+sep,1,1);//top-right
					TPad * pad3 = new TPad("pad3","pad3",xdim+sep,0, 1, ydim-sep);//bottom-right
					TPad * pad4 = new TPad("pad4","pad4",0,0, xdim-sep, ydim-sep); //bottom-left
					pad1->Draw();pad1->cd();
					h2hit[kk][j][loc_ind]->Draw("colz");
				
					loc_ind++;
					doublehit->cd(div2[i]);
					pad2->Draw();pad2->cd();
					h2hit[kk][j][loc_ind]->Draw("colz");
				
					loc_ind++;
					doublehit->cd(div2[i]);
					pad3->Draw();pad3->cd();
					h2hit[kk][j][loc_ind]->Draw("colz");
					
					loc_ind++;
					doublehit->cd(div2[i]);
					pad4->Draw();pad4->cd();
					h2hit[kk][j][loc_ind]->Draw("colz");
				
					loc_ind++;
					doublehit->cd(div2[i]);		
				}
			}
		}//close loop for outer module double hits

		if(run2hit_middle){//double hits for middle modules
			TCanvas *doublehit[4];
			for(int j=0;j<4;j++){	 
				doublehit[j]= new TCanvas(Form("2hit%02d_%d",kk,j),Form("Double hits_%d",j),wtopx, wtopy, ww, wh);
				doublehit[j]->Divide(3,3);
				for(int i=0;i<5;i++){
					doublehit[j]->cd(div[i]);
					h2hit[kk][j][i]->Draw("colz");
				}
				//place middle module histograms
		  	   int loc_ind = 5;
		  	   for(int i=0;i<4;i++){
					doublehit[j]->cd(div2[i]);
					TPad * pad1 = new TPad("pad1","pad1",0,ydim+sep, xdim-sep, 1);//top-left
					TPad * pad2 = new TPad("pad2","pad2",xdim+sep,ydim+sep,1,1);//top-right
					TPad * pad3 = new TPad("pad3","pad3",xdim+sep,0, 1, ydim-sep); //bottom-right
					TPad * pad4 = new TPad("pad4","pad4",0,0, xdim-sep, ydim-sep);//bottom-left
					pad1->Draw();pad1->cd();
					h2hit[kk][j][loc_ind]->Draw("colz");
				
					loc_ind++;
					doublehit[j]->cd(div2[i]);
					pad2->Draw();pad2->cd();
					h2hit[kk][j][loc_ind]->Draw("colz");
				
					loc_ind++;
					doublehit[j]->cd(div2[i]);
					pad3->Draw();pad3->cd();
					h2hit[kk][j][loc_ind]->Draw("colz");
					
					loc_ind++;
					doublehit[j]->cd(div2[i]);
					pad4->Draw();pad4->cd();
					h2hit[kk][j][loc_ind]->Draw("colz");
				
					loc_ind++;
					doublehit[j]->cd(div2[i]);		
				}
			}
		}//close double hit method for middle modules
		
	}//closes run_double_hit option loop
	if(plot_center_alone){
		TCanvas *cal = new TCanvas(Form("cal%02d",kk),"Calo block",wtopx+10, wtopy-10, ww, wh);
		hadcpedsubtdc[kk][center_block]->Draw();
		if(y_axis_log){cal->SetLogy();}
	}
	if(plot_energy_sum){
                TCanvas *esum = new TCanvas(Form("esum%02d",kk),"Energy Sum",wtopx+10, wtopy-10, ww, wh);
                hadcesum[kk]->Draw();
                TCanvas *xposit = new TCanvas(Form("xposit%02d",kk),"X Position",wtopx+10, wtopy-10, ww, wh);
                xposit->Divide(2,2);
                xposit->cd(1);
                xposition[kk]->Draw();
                xposit->cd(2);
                yposition[kk]->Draw();
                xposit->cd(3);
                xyposition[kk]->Draw();
//              eleftright->Draw();
//              xposit->cd(4);
//              eupdown->Draw();
                if(y_axis_log){esum->SetLogy();}
                if(y_axis_log){xposit->SetLogy();}
	}
	if(plot_xymanypos){
		if(kk == 0){
			xyposition[kk]->SetMarkerColor(hcolor[kk]);
			xyposition[kk]->SetMarkerStyle(21);
			xyposition[kk]->Draw();
		}else{
			xyposition[kk]->SetMarkerColor(hcolor[kk]);
			xyposition[kk]->SetMarkerStyle(21);
			xyposition[kk]->Draw("same");
		}
	}
}
}
