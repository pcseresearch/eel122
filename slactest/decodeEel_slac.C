////////////////////////////////////////////////////////
//
//    EEL VME test stand simple decoding macro
//    to be used with Analyzer v1.5 or more recent
//    For 1 V792 and one V775
//
//    Dec 23rd 2008 
//    
//    A. Camsonne
//    camsonne@jlab.org
//
/////////////////////////////////////////////////////////


//adjust according to the analysis computer used
#define DATAFILENAME "/home/coda/slacdata/calotest_%d.dat"

void decodeEel_slac(int run)
{
  THaCodaFile * coda = new THaCodaFile(Form(DATAFILENAME,run));

  // variable declaration
  int * buffer; // coda buffer where the event will be stored
  int adc[32]; // data array for the adc
  int tdc[32]; // data array for the tddc
  int offset=2; // offset where the first data word appears after the header
  int chan,val;// decoded channel and corresponding value variables
  int index=0; // index used to go through the buffer
  int evcount=0;// number of events processed
  int size; // number of channels to be read
  if (coda != NULL) {                               
    TFile * out = new TFile (Form("slacrootfiles/slactest%d.root",run),"RECREATE"); // creation of the output file                                         
    TTree * tdata = new TTree("tdata", Form("Run %d",run)); // creation of the tree
    tdata->Branch("adc",&adc,"adc[32]/I"); // creation of the branch to hold the adc data
    tdata->Branch("tdc",&tdc,"tdc[32]/I"); // creation of the branch to hold the tdc data
    THaCodaDecoder * ev = new THaCodaDecoder;
    while (coda->codaRead()==0 )
      {
	// initialisation of variables used for each events
	index=0;
	evcount++;
	for ( int i = 0 ;i <32 ;i++)
	  {
	    adc[i]=0;
	    tdc[i]=0;
	  }
	buffer = coda->getEvBuffer(); // load the data of th event buffer
	ev->LoadEvent(buffer);
	int nevt=ev->GetEvLength();

	while (index<nevt && ((buffer[index]&0xffffffff)!=0x0da00adc0))
	  {
	    //cout<<hex<<buffer[index]<<endl;
 	    index++; // look for header of the adc
	  }
	size = (0xf000&buffer[index+1])>>8  ;
	//cout<<" INdex = " << index << "  Size :"<<size<<endl;
	 // decode the channels read 
	 for ( int i = 0 ; i < size ; i++)
	   {
	     chan=(((buffer[i+index+offset])&0x1f0000)>>16);
	     val=((buffer[i+index+offset])&0xfff);
	     //cout<< i << " " << offset << " " << evcount<<" "<<chan<<" "<<val<<endl;
	     adc[chan]=val;
	   }

	 while (index<nevt && ((buffer[index]&0xffffffff)!=0x0da00ddc0))
          {
            index++;//look for header of the tdc
	    //	cout<<dec<<index<<"l2 "<<hex<<buffer[index]<<" "<<buffer[index+1]<<endl;
          }
	 //  cout<<dec<<index<<" "<<hex<<buffer[index]<<" "<<buffer[index+1]<<endl;
   size = buffer[index+1] -2 ;
 // decode the tdc channels read 
   for ( int i = 0 ; i < size ; i++)
     {
	     chan=(((buffer[i+index+offset])&0x1f0000)>>16);
	     val=((buffer[i+index+offset])&0xfff);
	     //cout<<chan<<" "<<val<<endl;
	     tdc[chan]=val;
     }

	
   tdata->Fill();// fill the tree with the new eventB
   if ( (evcount%100)==0)
     {
       cout<<evcount<<endl; // display the number of events processed every 100 events
     }
      }
    tdata->Write(); // end of the coda file ... write the tree
    out->Close(); // and close the output file

  }
 
}
