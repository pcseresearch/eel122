
/*****************OPTIONS*************************/
/**/const Int_t histo_max = 4596; //for adc histos only
/**/const bool y_axis_log = true;
/**/const bool show_rawdata = false;
/**/const bool show_tdcadj = false;
/**/const bool show_nongeo = false;
/**/const bool show_geo = true;//must be true for position plot to work
/**/const bool run_pedsig_hitcount = false;
/**/const bool run_double_hit = false;    
/**/const bool plot_center_alone = false;
/**/const bool plot_energy_sum = true;//normally true
/**/const bool do_fits = false;
/**/const int Ncalo=21;
/**/const int Ntdc=8;
/*************************************************/





void multi_calo_plots_slac_xpos_sinh(){
//Int_t nrunlist[5]={69,72,73,74,75}; //3 GeV runs
Int_t nrunlist[5]={81,85,86,87,88}; //12GeV runs
Int_t electronpeakfocus = 2;

for(int runcounter=0;runcounter<5;runcounter++){
	Int_t nrun = nrunlist[runcounter];
	cout << "Nrun= "<< nrun << endl;
	gStyle->SetOptStat(1);
	TFile *froot =  new TFile(Form("../data/calotest%d.root",nrun));
	TTree *troot = (TTree*)froot->Get("tdata");
	Int_t adc[32]; // raw data array for the adc
	Int_t tdc[32]; // raw data array for the tdc
	troot->SetBranchAddress("adc",&adc);
	troot->SetBranchAddress("tdc",&tdc);

	//double adcoffset=4.0;
	//double adccorr=300.0/327.0;
	double adcoffset=-1.8;
	double adccorr=300.0/319.386;
        double xposoffset=0.000;
 	double xposcorr=1.0000;	
        double yposoffset=0.000;
 	double yposcorr=1.000;
	double energy = 300.0;
	double deltae = 400.0;
	double sigmaxa=1.08; //for left XOR right 
	double sigmaya=1.18;	//up XOR down 
	double sigmaxb=1.0;	//left AND right
	double sigmayb=1.0;	//up AND down
	double block_size=11.2;


	if(nrun>=1){
		Int_t adc_index[Ncalo]={0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};// Index for 0-1,0-2,0-3,0-4,0-5,M1,M2,M3,M4
	//	int loc_index[21]={0,1,2,3,4,5,6,8,7,9,10,12,11,13,14,16,15,17,18,20,19};//use module #-1 first 5  are outer 
   //"O-01","O-05","O-02","O-04","O-03", next 16 middle "M-2","M-3","M-1","M-4"
	}
	

	//*************
	//**Locations**
	//*************
	if(nrun>=1){
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
	
	//changes for marker placement
	Double_t posindex, rl_rnum, rl_xpos, rl_ypos, rl_energy;
	if(center_block == 0){
		FILE *runlist = fopen("../data/runlist_block_O-01.dat","r");
		for(int i=0;i<16;i++){
			fscanf(runlist,"%lf %lf %lf %lf\n", &rl_rnum, &rl_xpos, &rl_ypos, &rl_energy);
			if(rl_rnum==nrun){
				energy=rl_energy*100.0*electronpeakfocus;
				cout<<"position is   ("<<rl_xpos<<","<<rl_ypos<<")"<<endl;
				break;
			}		
		}		
	}

//make our histos
	char *Modname[Ncalo]={"O-01","O-02","O-03","O-04","O-05","M-01A","M-01B","M-01C","M-01D","M-02A","M-02B","M-02C","M-02D","M-03A","M-03B","M-03C","M-03D","M-04A","M-04B","M-04C","M-04D"};
	char *tModname[Ntdc]={"Y Lower","Y Middle","Y Upper","X Left","Xmiddle","Y OR","X OR","XY AND"};
	TH1F *hadcraw[Ncalo],*hadcpedsub[Ncalo],*hadcpedsubtdc[Ncalo],*hadccent[Ncalo],*hadccut2[Ncalo];
	TH1F *htdcraw[Ntdc],*htdcadj[Ntdc];
	TH1F *hadcesum;
	TH1F *xposition;
	TH1F *yposition;
	TH1F *eupdown;
	TH1F *eleftright;
	TH2F *xyposition;
	TH2F *adcevent;
	hadcesum = new TH1F("hadcesum","Energy Sum",300,-100,4000);
	xposition = new TH1F("xposition","X Position",300,-6.0,6.0);
	yposition = new TH1F("yposition","Y Position",300,-6.0,6.0);
	eupdown = new TH1F("eupdown","E Up Down",300,-0.5,0.5);
	eleftright = new TH1F("eleftright","E Left Right",300,-0.5,0.5);
	xyposition = new TH2F("xyposition","XY Position",300,-6.0,6.0,300,-6.0,6.0);
	adcevent = new TH2F("adcevent","ADC vs Event Number",1000,0,1000,400,7900,8300);
	TH2F *h2hit[4][Ncalo];
	for ( int i = 0; i < Ntdc ; i++) {
		htdcraw[i] = new TH1F(Form("htdcraw%02d", i),Form("%s    raw tdc",tModname[i]),300,3000,4500);
		htdcadj[i] = new TH1F(Form("htdcadj%02d", i),Form("%s    raw tdc",tModname[i]),200,3000,4000);
	}
	for ( int i = 0; i < Ncalo ; i++) {
		hadcraw[i] = new TH1F(Form("hadcraw%02d", i),Form("%s    raw adc",Modname[i]),300,0,300);
		hadcpedsub[i] = new TH1F(Form("hadcpedsub%02d", i),Form("%s    pedsub adc;ADC; Counts ",Modname[i]),500,-100,2000);
		hadcpedsubtdc[i] = new TH1F(Form("hadcpedsubtdc%02d", i),Form("%s    pedsub tdccut adc;ADC; Counts ",Modname[i]),500,-100,2000);
		hadccent[i] = new TH1F(Form("hadccent%02d", i),Form("%s    fitted adc;ADC; Counts ",Modname[i]),100,-100,histo_max);
		hadccut2[i] = new TH1F(Form("hadccut2%02d", i),Form("%s    fitted adc;ADC; Counts ",Modname[i]),100,-100,histo_max);
		for(int j=0; j<4; j++){	
			h2hit[j][i] = new TH2F(Form("h2hit_%02d_%02d",j,i),Form("%s    fitted adc;ADC; Counts ",Modname[i]),100,-100,1500,100,-100,histo_max);
		}
	}
	int total_clean=0;
	int nentries = (int)troot->GetEntries();
	//int nentries = 1;
	int temp;
	cout << " Nentries = "<< nentries<< endl;
	for (int ie = 0; ie < nentries; ie++) {
		troot->GetEntry(ie);
		for ( int i = 0; i < Ncalo ; i++) {
			//cout << ie <<" " << i << " " << adc[i] << " " << adc_index[i] << endl;
		   hadcraw[i]->Fill(adc[(adc_index[i])]);
		   if(i==0){adcevent->Fill(ie,adc[(adc_index[i])]);}
		}
		for ( int i = 0; i < Ntdc ; i++) {
			//cout << ie <<" " << i << " " << adc[i] << " " << adc_index[i] << endl;
			htdcraw[i]->Fill(tdc[i]);//if(ie%100==0){cout<<tdc[i]<<endl;}
			htdcadj[i]->Fill(tdc[i]+tdccorrection[i]);
		}
		
	}


// Plot histograms of raw data 

	// tdcs
	TCanvas *traw = new TCanvas("traw","Raw TDC spectra",600,600,600,600);
   	traw->Divide(3,3);
   	for ( int i = 0; i < Ntdc ; i++) {
      		traw->cd(i+1);
		if(y_axis_log){gPad->SetLogy();}
      		htdcraw[i]->Draw();
	}
	 if(!show_rawdata){traw->Close();}
	
	 //outer modules   
	TCanvas *craw = new TCanvas("craw","Raw ADC spectra OUTER type",600,600);
   craw->Divide(2,3);
   if(y_axis_log){craw->SetLogy();}
   TF1 *f1 = new TF1("f1", "gaus");
   for ( int i = 0; i < 5 ; i++) {
      craw->cd(i+1);
      if(y_axis_log){gPad->SetLogy();}
      hadcraw[i]->Draw();
	}
   if(!show_rawdata){craw->Close();}
	
   //middle modules
   TCanvas *craw2 = new TCanvas("craw2","Raw ADC spectra MIDDLE type",600,600);
   craw2->Divide(4,4);
   for ( int i = 5; i < Ncalo ; i++) {
      craw2->cd(i-4);
   	if(y_axis_log){gPad->SetLogy();}
		hadcraw[i]->Draw();
   }
	if(!show_rawdata){craw2->Close();}


   // rescan with ped subtraction
//	cout << "Got to here 0" << endl;

	//tdc with adjustment to gain match
	TCanvas *tadj = new TCanvas("tadj","Raw TDC spectra",600,600,600,600);
   	tadj->Divide(3,3);
   	for ( int i = 0; i < Ntdc ; i++) {
      		tadj->cd(i+1);
				if(y_axis_log){gPad->SetLogy();}
      		htdcadj[i]->Draw();
	}
	if(!show_tdcadj){tadj->Close();}
//	cout << "Got to here 1" << endl;

   Bool_t clean_hit;
   Int_t number_hit;
   Int_t ghit;
   Double_t hit_adc[Ncalo];
   Int_t hit_mod[Ncalo];
   double hits[Ncalo+1];
   double holdesum,holdxpos,holdypos;
   for( int i = 0; i <= Ncalo ; i++) {
      hits[i]=0;
	}
   cout << " Nentries = "<< nentries<< endl;
   Int_t chits[Ncalo]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
   for (int ie = 0; ie < nentries; ie++) {
      troot->GetEntry(ie);
      number_hit=0;
      holdesum=0.0;
      holdxpos=0.0;
      holdypos=0.0;
      for ( int i = 0; i < Ncalo ; i++) {
	  //cout << ie <<" " << i << " " << adc[i] << " " << adc_index[i] << endl;
			hadcpedsub[i]->Fill(adccorr*(adcoffset+adcgainfactor[i]*(adc[(adc_index[i])]-ped[i])));
	if (i==0 || i==1 || i==2 || i==3 || i==4 || i==7 || i==12 || i==14 || i==17){
			holdesum=holdesum+
			adccorr*(adcoffset+adcgainfactor[i]*(adc[(adc_index[i])]-ped[i]));
			}
			//if(tdc[1]>3700 && tdc[1]<4300 && tdc[4]>3700 && tdc[4]<4300 ){hadcpedsubtdc[i]->Fill(adccorr*(adcoffset+adcgainfactor[i]*(adc[(adc_index[i])]-ped[i])));}
	//		if(tdc[7]>3200 && tdc[7]<3600 ){hadcpedsubtdc[i]->Fill(adccorr*(adcoffset+adcgainfactor[i]*(adc[(adc_index[i])]-ped[i])));}
			if((adc[adc_index[i]]-ped[i])>100.0) {
	  			hit_adc[number_hit]=(adccorr*(adcoffset+adcgainfactor[i]*(adc[adc_index[i]]-ped[i])));
	  			hit_mod[number_hit]=i;
	  			number_hit++;
			}
      }
      
      for ( int i = 0; i < Ncalo ; i++) {
	if(holdesum > (energy-deltae) && holdesum < (energy+deltae) ){
		hadcpedsubtdc[i]->Fill(adccorr*(adcoffset+adcgainfactor[i]*(adc[(adc_index[i])]-ped[i])));

//	if (i==0 || i==2 || i==3 || i==4 || i==14 || i==17){
//		if (i==0 || i==2 || i==3 ){
//			holdxpos=holdxpos+xpos[i]*adccorr*(adcoffset+adcgainfactor[i]*(adc[(adc_index[i])]-ped[i]));
//		}
//		if (i==0 || i==1 || i==4 ){
//			holdypos=holdypos+ypos[i]*adccorr*(adcoffset+adcgainfactor[i]*(adc[(adc_index[i])]-ped[i]));
//		}
	}
      }
      //if (holdesum > energy-deltae && holdesum < energy+deltae && tdc[7]>3200 && tdc[7]<3600){
      if (holdesum > energy-deltae && holdesum < energy+deltae){
      double eleft =  adccorr*(adcoffset+adcgainfactor[2]*(adc[(adc_index[2])]-ped[2]));
      double emid =   adccorr*(adcoffset+adcgainfactor[0]*(adc[(adc_index[0])]-ped[0]));
      double eright = adccorr*(adcoffset+adcgainfactor[3]*(adc[(adc_index[3])]-ped[3]));
      double eupper = adccorr*(adcoffset+adcgainfactor[1]*(adc[(adc_index[1])]-ped[1]));
      double elower = adccorr*(adcoffset+adcgainfactor[4]*(adc[(adc_index[4])]-ped[4]));
//      cout << eleft << " " << eright << " " << emid << " " << eupper << " " << elower << endl;
//      cout << eleft+emid+eright << " " << eleft-eright << endl;
//      cout << eupper+elower+emid << " " << eupper-elower << endl << endl;
//      holdxpos=(eright-eleft)/abs(eright-eleft)*pow(abs(eleft-eright)/(eleft+emid+eright),0.5)*xposcorr+xposoffset;
//      holdypos=(eupper-elower)/abs(eupper-elower)*pow(abs(elower-eupper)/(eupper+emid+elower),0.5)*yposcorr+yposoffset;
//
// new position calculation!!	
    
        double pedthresh = 40.0;

	if(eleft < pedthresh && eright > pedthresh && emid > pedthresh) {
        	eleftright->Fill((block_size/2.0-sigmaxa*log(0.5*(emid/eright)+1.0)));
		holdxpos=((block_size/2.0-sigmaxa*log(0.5*(emid/eright)+1.0))*xposcorr+xposoffset  );
	}else if (eleft > pedthresh && eright < pedthresh && emid > pedthresh) {
        	eleftright->Fill((-1.0*block_size/2.0+sigmaxa*log(0.5*(emid/eleft)+1.0)));
		holdxpos=((-1.0*block_size/2.0+sigmaxa*log(0.5*(emid/eleft)+1.0))*xposcorr+xposoffset  );
	}else if (eleft > pedthresh && eright > pedthresh) {
        	eleftright->Fill( (sigmaxb/2.0*log(eleft/eright))  );
		holdxpos=((sigmaxb/2.0*log(eleft/eright))*xposcorr+xposoffset  );
	}else{
		eleftright->Fill(0.0);
		holdxpos=0.0;
	}
	
	if(elower < pedthresh && eupper > pedthresh && emid > pedthresh) {
        	eupdown->Fill((block_size/2.0-sigmaya*log(0.5*(emid/eupper)+1.0)));
		holdypos=((block_size/2.0-sigmaya*log(0.5*(emid/eupper)+1.0))*yposcorr+yposoffset  );
	}else if (elower > pedthresh && eupper < pedthresh && emid > pedthresh) {
        	eupdown->Fill((-1.0*block_size/2.0+sigmaya*log(0.5*(emid/elower)+1.0)));
		holdypos=((-1.0*block_size/2.0+sigmaya*log(0.5*(emid/elower)+1.0))*yposcorr+yposoffset  );
	}else if (elower > pedthresh && eupper > pedthresh) {
        	eupdown->Fill( (sigmayb/2.0*log(elower/eupper))  );
		holdypos=((sigmayb/2.0*log(elower/eupper))*yposcorr+yposoffset  );
	}else{
		eupdown->Fill(0.0);
		holdypos=0.0;
	}

	xposition->Fill(holdxpos);
      	yposition->Fill(holdypos);
	xyposition->Fill(holdxpos,holdypos);
      }

      hadcesum->Fill(holdesum);
		hits[number_hit]++;
		if(number_hit==1) {
			hadccent[hit_mod[0]]->Fill(hit_adc[0]);
			chits[ghit]++;
      }
	
		//2 hit method for outer modules
		if(number_hit==2 && run2hit_outer){
			Int_t ghit0=hit_mod[0];//cout<<ghit0<<endl;
			Int_t ghit1=hit_mod[1];//cout<<ghit1<<endl;
			if(hit_mod[0]==center_block){
				h2hit[0][ghit1]->Fill(hit_adc[1],hit_adc[0]);
			}
			if(hit_mod[1]==center_block){
				h2hit[0][ghit0]->Fill(hit_adc[0],hit_adc[1]);			
			}
		}
    
		//2 hit method for middle(2x2) modules
		if(number_hit==2 && run2hit_middle){
			Int_t ghit0=hit_mod[0];
			Int_t ghit1=hit_mod[1];
			if(ghit0==middle_block||ghit0==middle_block+1||ghit0==middle_block+2||ghit0==middle_block+3){
				if(ghit0==middle_block){
					h2hit[0][ghit1]->Fill(hit_adc[1],hit_adc[0]);
				}
				if(ghit0==middle_block+1){
					h2hit[1][ghit1]->Fill(hit_adc[1],hit_adc[0]);
				}
				if(ghit0==middle_block+2){
					h2hit[2][ghit1]->Fill(hit_adc[1],hit_adc[0]);
				}
				if(ghit0==middle_block+3){
					h2hit[3][ghit1]->Fill(hit_adc[1],hit_adc[0]);
				}

			}
			if(ghit1==middle_block||ghit1==middle_block+1||ghit1==middle_block+2||ghit1==middle_block+3){
				if(ghit1==middle_block){
					h2hit[0][ghit0]->Fill(hit_adc[0],hit_adc[1]);
				}
				if(ghit1==middle_block+1){
					h2hit[1][ghit0]->Fill(hit_adc[0],hit_adc[1]);
				}
				if(ghit1==middle_block+2){
					h2hit[2][ghit0]->Fill(hit_adc[0],hit_adc[1]);
				}
				if(ghit1==middle_block+3){
					h2hit[3][ghit0]->Fill(hit_adc[0],hit_adc[1]);
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
   TCanvas *cadc = new TCanvas("cadc","ADC spectra ped sub OUTER type",600,600);
   cadc->Divide(2,3);
   Double_t min_fit,max_fit;
   for(int i=0;i<5;i++){
	   cadc->cd(i+1);					
		if(y_axis_log){gPad->SetLogy();}			
      hadcpedsub[i]->Draw();		
      hadccent[i]->Draw("same");
      hadccent[i]->SetLineColor(3);
      if(hadccent[i]->Integral(10.,1500.) > 100.) {
			f1->SetParameters(500, 500.,70.);
			int count = 0;
			min_fit=50.;max_fit=1500.;
			bool go = do_fits;
			while(go){	//this while loops performs iteration to adjust the fit
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
   TCanvas *cadc2 = new TCanvas("cadc2","ADC spectra ped sub MIDDLE type",600,600);
   cadc2->Divide(4,4);
   Double_t mean_st,sigma_st,cnt_st;
	for(int i=5;i<Ncalo;i++){
		cadc2->cd(i-4);												
      if(y_axis_log){gPad->SetLogy();}						
      hadcpedsub[i]->Draw();										
      hadccent[i]->Draw("same");											
      hadccent[i]->SetLineColor(3);									
      if(hadccent[i]->Integral(10.,histo_max-500.) > 100.){			
			f1->SetParameters(500, 500.,70.);											
			int count = 0;
			min_fit=50.;max_fit=histo_max-500.;			
			hadccent[i]->Fit(f1,"Q","same", min_fit,max_fit);
			Double_t mean = 0;			
			bool go = do_fits;
			while(go){		//this while loops performs iteration to adjust the fit
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
		TCanvas *t5 = new TCanvas("t5","Pedestal Sigma & Hit counter",600,600);
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
		TCanvas * cHist = new TCanvas("cHist", "ADC spectra ped sub", wtopx, wtopy, ww, wh);
		cHist->Divide(3,3);
		cHist->SetLogy();
		    
		//place Outer module histograms (in order of original adc)
		for(int i=0;i<5;i++){
			cHist->cd(div[i]);
			if(y_axis_log){gPad->SetLogy();}
			hadcpedsubtdc[i]->Draw();
			//hadccent[i]->Draw("same");
			//hadccent[i]->SetLineColor(3);
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
			hadcpedsubtdc[loc_ind]->Draw();
			//hadccent[loc_ind]->Draw("same");
			//hadccent[loc_ind]->SetLineColor(3);
			loc_ind++;
			cHist->cd(div2[i]);
			pad2->Draw();pad2->cd();if(y_axis_log){gPad->SetLogy();}
			hadcpedsubtdc[loc_ind]->Draw();
			//hadccent[loc_ind]->Draw("same");
			//hadccent[loc_ind]->SetLineColor(3);
			loc_ind++;
			cHist->cd(div2[i]);
			pad3->Draw();pad3->cd();if(y_axis_log){gPad->SetLogy();}
			hadcpedsubtdc[loc_ind]->Draw();
			//hadccent[loc_ind]->Draw("same");
			//hadccent[loc_ind]->SetLineColor(3);
			loc_ind++;
			cHist->cd(div2[i]);
			pad4->Draw();pad4->cd();if(y_axis_log){gPad->SetLogy();}
			hadcpedsubtdc[loc_ind]->Draw();
			//hadccent[loc_ind]->Draw("same");
			//hadccent[loc_ind]->SetLineColor(3);
			loc_ind++;
			cHist->cd(div2[i]);		
		}
	}//close show_geo option

 //********************************************************
 // Double hit display methods
 //*********************************************************

	if(run_double_hit){
		if(run2hit_outer){//loop for outer module double hits
			TCanvas *doublehit = new TCanvas("2hit","Double hits",wtopx, wtopy, ww, wh);
			doublehit->Divide(3,3);
			//place Outer module histograms (in order of original adc)
			for(int j=0;j<1;j++){	 
				for(int i=0;i<5;i++){
					doublehit->cd(div[i]);
					h2hit[j][i]->Draw("colz");
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
					h2hit[j][loc_ind]->Draw("colz");
				
					loc_ind++;
					doublehit->cd(div2[i]);
					pad2->Draw();pad2->cd();
					h2hit[j][loc_ind]->Draw("colz");
				
					loc_ind++;
					doublehit->cd(div2[i]);
					pad3->Draw();pad3->cd();
					h2hit[j][loc_ind]->Draw("colz");
					
					loc_ind++;
					doublehit->cd(div2[i]);
					pad4->Draw();pad4->cd();
					h2hit[j][loc_ind]->Draw("colz");
				
					loc_ind++;
					doublehit->cd(div2[i]);		
				}
			}
		}//close loop for outer module double hits

		if(run2hit_middle){//double hits for middle modules
			TCanvas *doublehit[4];
			for(int j=0;j<4;j++){	 
				doublehit[j]= new TCanvas(Form("2hit_%d",j),Form("Double hits_%d",j),wtopx, wtopy, ww, wh);
				doublehit[j]->Divide(3,3);
				for(int i=0;i<5;i++){
					doublehit[j]->cd(div[i]);
					h2hit[j][i]->Draw("colz");
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
					h2hit[j][loc_ind]->Draw("colz");
				
					loc_ind++;
					doublehit[j]->cd(div2[i]);
					pad2->Draw();pad2->cd();
					h2hit[j][loc_ind]->Draw("colz");
				
					loc_ind++;
					doublehit[j]->cd(div2[i]);
					pad3->Draw();pad3->cd();
					h2hit[j][loc_ind]->Draw("colz");
					
					loc_ind++;
					doublehit[j]->cd(div2[i]);
					pad4->Draw();pad4->cd();
					h2hit[j][loc_ind]->Draw("colz");
				
					loc_ind++;
					doublehit[j]->cd(div2[i]);		
				}
			}
		}//close double hit method for middle modules
		
	}//closes run_double_hit option loop
	if(plot_center_alone){
		TCanvas *cal = new TCanvas("cal","Calo block",wtopx+600, wtopy-0, ww, wh);
		//int icenter_block=0; // O1
		//int icenter_block=2; // O3
		//int icenter_block=3; // O4
		int icenter_block=4; // O5
		hadcpedsubtdc[icenter_block]->Draw();
		if(y_axis_log){cal->SetLogy();}
	}
	if(plot_energy_sum){
		TCanvas *esum = new TCanvas("esum","Energy Sum",wtopx+0, wtopy-800, ww, wh);
		hadcesum->Draw();
		TCanvas *xposit = new TCanvas("xposit","X Position",wtopx+800, wtopy-0, ww, wh);
   		xposit->Divide(2,2);
		xposit->cd(1);
      		if(y_axis_log){gPad->SetLogy();}
		xposition->Draw();
		xposit->cd(2);
      		if(y_axis_log){gPad->SetLogy();}
		yposition->Draw();
		xposit->cd(3);
		xyposition->Draw();
		//draw marker
		xposit->Close();		
		

//**************THIS DRAWS MULTIPLE POSITION PLOT AND PLACES MARKER**********//

		TMarker *marker = new TMarker(rl_xpos, rl_ypos, 34); //marker for actual beam position, last int is style 
		marker->SetMarkerColor(1);
		marker->SetMarkerSize(2);
		marker->Draw();
		if(runcounter==0){
			TCanvas *finpos = new TCanvas("finpos","Position", wtopx+800, wtopy-0, ww, wh);
			xyposition->Draw();
			marker->Draw();
			gStyle->SetOptStat(0);
//			finpos->Print(Form("./pics/xyposition_doublee_%d.jpg",nrun),"jpg");
		}
		else{
			xyposition->SetMarkerColor(1+runcounter); //setting color of reconstruction dots
			xyposition->Draw("same");
			marker->Draw("same");
			gStyle->SetOptStat(0);
		}

//		eleftright->Draw();
//		xposit->cd(4);
//		eupdown->Draw();
		if(y_axis_log){esum->SetLogy();}
		if(y_axis_log){xposit->SetLogy();}
	}
}
}
