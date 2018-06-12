gROOT->Reset();

const int Ncalo = 21;			//Number of Calorimeter Modules (by PMT count)
int hist_max = 4000;			//Maximum bin of the histograms
int hist_min = 0;				//Minimum bin of the histograms
int hist_bins = hist_max;		//# of bins in Histograms
bool show_raw = false;			//boolean as to whether to show raw adc histograms or not
bool show_pedcut = false;		//boolean as to whether to show pedestal cut adc histograms or not
bool show_adc_corrected = false;//boolean as to whether to show adc corrected histograms or not
bool show_Esum = true;			//boolean as to whether to show energy sum histogram or not
bool fit_esum = false;			//boolean as to whether to fit the energy sum histograms first peak
double cut_width;				//cut_width multiplies sigma for the appropriate length we want our cut to go out

//Hard Code pedestal values obtained from PedSub.C for runs 70 & 71 and ADCcorrection factors from ADCcorrection.C
double ped_mean[4];			//Pedestal mean values for each outer block
double ped_mean_error[4];	//Pedestal mean errors for each outer block
double ped_sigma[4];		//Pedestal sigma values for each outer block
double ped_sigma_error[4];	//Pedestal sigma errors for each outer block
double ADC_correction[4];	//ADCcorrection factors for each outer block

ped_mean[0] = 140.72; ped_mean[1] = 77.139; ped_mean[2] = 194.71; ped_mean[3] = 209.23; ped_mean[4] = 208.26;
ped_sigma[0] = 6.333; ped_sigma[1] = 2.848; ped_sigma[2] = 2.509; ped_sigma[3] = 2.392; ped_sigma[4] = 2.991;

ped_mean_error[0] = .06831; ped_mean_error[1] = .03758; ped_mean_error[2] = .02538; ped_mean_error[3] = .02524; ped_mean_error[4] = .03027;
ped_sigma_error[0] = .04218; ped_sigma_error[1] = .02301; ped_sigma_error[2] = .01738; ped_sigma_error[3] = .01555; ped_sigma_error[4] = .01930;

ADC_correction[0] = 0.879097; ADC_correction[1] = 1; ADC_correction[2] = 0.911784; ADC_correction[3] = 1.01006; ADC_correction[4] = 0.954909;


//Program will take the ADC correction factors found in the ADCcorrection.C program, as well as, 
//the pedestal means and sigmas found in the PedSub.C program to sort through the raw ADC data
//to sum the energy from the adjacent blocks 
void EnergySum (int run)
{
	//section requesting values of boolean variables for programs tasks
	cout << "Would you like to see raw adc histograms? (true/false) " << endl;
	//	cin >> show_raw;
	show_raw=0;
	cout << "Would you like to see pedestal cut adc histograms? (true/false) " << endl;
	//	cin >> show_pedcut;
	show_pedcut=0;
	cout << "Would you like to see adc correction histograms? (true/false) " << endl;
	//	cin >> show_adc_corrected;
	show_adc_corrected=0;
	cout << "Would you like to fit energy sum histogram?" << endl;
	//	cin >> fit_esum;
	fit_esum=0;
	cout << "How many sigma into the histograms would you like to cut?" << endl;
	//	cin >> cut_width;
	cut_width=0;
	
/************************************Read in Data and Declare Histograms******************************************/
//{	
	cout << "run # = " << run << endl;
	gStyle->SetOptStat(1);
	
	//	TFile *froot =  new TFile(Form("C:/Users/Matt/Skydrive/SlacFiles/eel122/data/calotest%d.root",run));
	TFile *froot =  new TFile(Form("./data/calotest%d.root",run));
	TTree *troot = (TTree*)froot->Get("tdata");   	//Get data from tree in root file and name "troot"
	
	Int_t adc[32]; // raw data array for the adc
	Int_t tdc[32]; // raw data array for the tdc
	
	troot->SetBranchAddress("adc",&adc);			//Set Branch Address' for the tree
	troot->SetBranchAddress("tdc",&tdc);
	
	if(run>=1)				//Declare the indices defining the positions of the calorimeter modules
	{
		//Index for 0-1,0-2,0-3,0-4,0-5,M1,M2,M3,M4
		Int_t adc_index[Ncalo]={0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};

		int div[5]={5,2,4,6,8}; 	// index of 3x3 division for outer "O-01","O-02","O-03","O-04","O-05"
		int div2[4]={1,3,7,9};	 	// index of 3x3 division for middle :"M-1","M-2","M-3","M-4"
	}
	
	//Array of Names for histograms
	char *Modname[Ncalo]={"O-01","O-02","O-03","O-04","O-05","M-01A","M-01B","M-01C","M-01D","M-02A","M-02B","M-02C","M-02D","M-03A","M-03B","M-03C","M-03D","M-04A","M-04B","M-04C","M-04D"};
	TH1F *hadcraw[Ncalo]; 				//Declare array of raw adc data histograms
	TH1F *hadcpedcut[Ncalo];			//Array of pedestal cut adc histograms
	TH1F *hadccorrect[Ncalo];			//Array of adc corrected adc histograms
	TH1F *EnergySum = new TH1F("Energy Sum", "Energy Sum of Blocks", hist_max, 0, hist_max);	//Energy sum histogram
	
	for ( int i = 0; i < Ncalo ; i++) 				//Form the histograms	
	{
		//Create raw adc data histograms
		hadcraw[i] = new TH1F(Form("hadcraw%02d", i),Form("%s    raw adc",Modname[i]),hist_max,0,hist_max);
		hadcpedcut[i] = new TH1F(Form("hadcpedcut%02d", i),Form("%s    ped cut adc",Modname[i]),hist_max,0,hist_max);
		hadccorrect[i] = new TH1F(Form("hadccorrect%02d", i),Form("%s    corrected adc",Modname[i]),hist_max,0,hist_max);
	}
	
	
	
	int nentries = (int)troot->GetEntries();		//Get total # of entries in root file
	cout << " Nentries = "<< nentries << endl;
	
	
	for (int ie = 0; ie < nentries; ie++) 			//Fill the Histogram from the adc data tree
	{
		troot->GetEntry(ie);						//Get entry # ie from tree
		
		for ( int i = 0; i < Ncalo ; i++) 
		{											//Within event ie
		   hadcraw[i]->Fill(adc[(adc_index[i])]);	//Loop through adc_index array created earlier and fill 
													//with raw adc histograms with the corresponding data
		}
	}
//}	
/**********************************End Data read in and Histogram Declaring***************************************/


/************************************Cut Pedestal Mean out of Histograms*****************************************/
//{
	double temp;
	
	for (int i = 0; i < 5; i++)		//Cut pedestal from the data and into new histograms by shifting histogram
	{								//Could also cut it out when we fill the histograms from the tree in loop above
		for (int j = 0; j < hist_max; j++)	//For block i, go through histogram bins j
		{
			temp = hadcraw[i]->GetBinContent(j);					//Get number of entries in bin j of raw hist
			hadcpedcut[i]->SetBinContent((j-ped_mean[i]),temp);		//Set the bin j-ped of pedcut hist to value temp
		}
	}
//}	
/*********************************************End Pedestal Cut***************************************************/


/**********************************************ADC correction****************************************************/
//{
	for (int i = 0; i < 5; i++)
	{						//take the ped cut histogram and correct the data for the adc correction
		for (int j = 0; j < hist_max; j++)
		{
			temp = hadcpedcut[i]->GetBinContent(j);
			hadccorrect[i]->SetBinContent((j*ADC_correction[i]),temp);
		}
	}
	
//}
/*********************************************End ADC correction*************************************************/


/******************************************Find center block hit*************************************************/
//{	
	int max_bin[4];		//array to store number the bin with most counts in it for each outer block
	int block_hit;		//variable to store the index number of the block hit
	double counts = 0;		//variable to store counts after some value in current block, will be compared to maxcounts
	double maxcounts = 0;	//variable to store the current block found with the most counts after some value
	
	for (int i = 0; i < 5; i++)		//Find the block that contains the majority of the event
	{								
		counts = 0;					//set counts to zero for each new block
		
		for (int j = 200; j < hist_max; j++)	//Find the histogram/block with the most counts above a certain value
		{										//That block will be designated as the main block hit
			counts += hadcpedcut[i]->GetBinContent(j);		//counts equals previous value plus # of entries
		}													//in next bin j
		
		if (counts > maxcounts)		//Compare counts to the max counts variable
		{
			maxcounts = counts;		//If counts is greater than max counts, maxcounts takes on counts value
			block_hit = i;			//Store the block index number that had the current maxcounts
		}
	}
	cout << "the block hit is " << Modname[block_hit] << endl;
//}
/******************************************End Center block find*************************************************/

	double counts = 0;
	double max = 0;
	int peak_bin = 0;
				
	for (j = 50; j < hist_max; j++)	//Loop to find the bin with max entries this is the single electron peak
	{								//will use this to fit the s.e. peak for the energy resolution program
		counts = hadccorrect[block_hit]->GetBinContent(j);
			
		if (counts > max)
		{
			max = counts;
			peak_bin = j;
		}
	}
	hadccorrect[block_hit]->Fit("gaus","","",(peak_bin-100),(peak_bin+100));
	

/*******************************Sum Energy of Center block and its neighbors************************************/
//{
	double esum;
	
	for (int ie = 0; ie < nentries; ie++) 			//Fill the Histogram from the adc data tree
	{
		troot->GetEntry(ie);						//Get entry # ie from tree
		esum = 0;
		
		for ( int i = 0; i < 5 ; i++) 
		{											//Within event ie
			temp = ADC_correction[i]*(adc[(adc_index[i])]-(ped_mean[i]+(cut_width*ped_sigma[i])));
				//energy sum is equal to the (adc_correction factor for that channel times (adc channel value minus
				//(pedestal mean value plus (cut_width times sigma for that channel))))
			if (temp > 0) 	//If the temp variable above is greater than zero then we want to include it in esum
			{				//for the event ie			
				esum += temp;
			}
		}
		EnergySum->Fill(esum);	//Fill the energy sum histogram with total energy esum for event ie
	}
	
//}	
/********************************************End Energy Sum*****************************************************/


/********************************************Plot Histograms****************************************************/
//{	
	if (show_raw == true)
	{		//If show_raw value is true create canvas for raw adc data
		TCanvas *craw = new TCanvas("craw","Raw ADC spectra OUTER type",600,600); 	//Create Canvas for raw adc Outer modules
		craw->Divide(2,3);
	}

	if (show_pedcut == true)
	{		//If show_pedcut value is true create canvas for pedestal cut adc data
		TCanvas *cpedcut = new TCanvas("cpedcut","Pedestal Cut ADC spectra OUTER type",600,600); 	//Create Canvas for ped cut adc Outer modules
		cpedcut->Divide(2,3);
	}
	
	if (show_adc_corrected == true)
	{		//If show_adc_corrected value is true create canvas for adc corrected adc data
		TCanvas *ccorr = new TCanvas("ccorr","ADC corrected spectra OUTER type",600,600); 	//Create Canvas for raw adc Outer modules
		ccorr->Divide(2,3);
	}
	
	if (show_Esum == true)
	{		//If show_Esum value is true create canvas for energy sum histogram
		TCanvas *cEsum = new TCanvas("cEsum","Energy Sum",600,600);
	}
	
	
	for ( int i = 0; i < 5 ; i++) 		//Draw histograms
	{		//Loop through all 5 main blocks
		if (show_raw == true)
		{	//Draw raw adc data for each block
			craw->cd(i+1);
			hadcraw[i]->Draw();
		}
		if (show_pedcut == true)
		{	//Draw pedestal cut adc data for each block
			cpedcut->cd(i+1);
			hadcpedcut[i]->Draw();
			hadcpedcut[i]->SetLineColor(2);
		}
		if(show_adc_corrected == true)
		{	//Draw adc corrected data for each block
			ccorr->cd(i+1);
			hadccorrect[i]->Draw();
			hadccorrect[i]->SetLineColor(9);
		}			
	}
	
		if(show_Esum == true)
		{	//Draw energy sum histogram
			cEsum->cd();
			EnergySum->Draw();
			EnergySum->SetLineColor(3);
			
			if(fit_esum == true)
			{	//Fit energy sum histogram's first peak if requested
				double counts = 0;
				double max = 0;
				int first_peak_bin = 0;
				int second_peak_bin = 0;
				int third_peak_bin = 0;
				
				for (j = 50; j < hist_max; j++)	//Loop to find the bin with max entries this is the single electron peak
				{								//will use this to fit the s.e. peak for the energy resolution program
					counts = EnergySum->GetBinContent(j);
					
					if (counts > max)
					{
						max = counts;
						first_peak_bin = j;
					}
				}
				EnergySum->Fit("gaus","+","",(first_peak_bin-100),(first_peak_bin+100));	//Fit the energy sum histogram's first
				
				counts = 0;
				max = 0;
				
				for (j = first_peak_bin + 100; j < hist_max; j++)	//Loop to find the bin with max entries this is the single electron peak
				{								//will use this to fit the s.e. peak for the energy resolution program
					counts = EnergySum->GetBinContent(j);
					
					if (counts > max)
					{
						max = counts;
						second_peak_bin = j;
					}
				}
				EnergySum->Fit("gaus","+","",(second_peak_bin-100),(second_peak_bin+100));	//Fit the energy sum histogram's first
				
				counts = 0;
				max = 0;
				
				for (j = second_peak_bin + 100; j < hist_max; j++)	//Loop to find the bin with max entries this is the single electron peak
				{								//will use this to fit the s.e. peak for the energy resolution program
					counts = EnergySum->GetBinContent(j);
					
					if (counts > max)
					{
						max = counts;
						third_peak_bin = j;
					}
				}
				EnergySum->Fit("gaus","+","",(third_peak_bin-100),(third_peak_bin+100));	//Fit the energy sum histogram's first
			}																//peak
		}

//}
/*******************************************End Plot Histograms************************************************/

/*******************************************Energy Resolution**************************************************/

double energy[2];		//array of energies for plotting
double mean[2];			//s.e. peak means (3Gev,9Gev,12GeV in order)
double sigma[2];		//s.e. peak sigmas ("   "	    	"   ")
double resolution[2];	//energy resolution sigma/mean

energy[0] = 3;		energy[1] = 9;		energy[2] = 12;		//Hard code the values received from previous fits
mean[0] = 314.007;	mean[1] = 1007.78;	mean[2] = 1309.44;	//of center block at 3,9, and 12 GeV runs
sigma[0] = 20.471;	sigma[1] = 88.991;	sigma[2] = 42.441;	//

resolution[0]=sigma[0]/mean[0]; 		//Energy resolution is sigma/energy(in adc channels) 
resolution[1]=sigma[1]/mean[1];
resolution[2]=sigma[2]/mean[2];


TCanvas *cEres = new TCanvas("cEres","Energy Resolution",600,600);
cEres->cd();
TGraph *EnergyRes = new TGraph(3,&energy[0],&resolution[0]);
EnergyRes->SetLineColor(2);
EnergyRes->SetLineWidth(2.25);
EnergyRes->SetMarkerColor(4);
EnergyRes->SetMarkerStyle(29);
EnergyRes->Draw("AP");

TF1 *res = new TF1("res","TMath::Sqrt(TMath::Power([0]/TMath::Sqrt(x),2)+TMath::Power([1]/x,2)+([2]*[2]))",3,12);		//Should be fit of res = Sqrt((A/Sqrt(E))^2 + (B/E)^2 + C^2)
res->SetParName(0,"A");
res->SetParName(1,"B");
res->SetParName(2,"C");
res->SetParameter(0,0.11);
res->SetParameter(1,1);
res->SetParameter(2,3);

EnergyRes->Fit("res");


/*******************************************End Energy Resolution***********************************************/


}	
