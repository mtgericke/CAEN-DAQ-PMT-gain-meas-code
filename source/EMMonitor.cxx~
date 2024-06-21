///////////////////////////////////////////////////////////////////////////////
//
// name: EMMonitor.cxx
// date: 5-29-2021
// auth: Michael Gericke 
//
///////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <EMMonitor.h>

EMMonitor::EMMonitor(int *argc, char *argv[], const TGWindow *p, UInt_t w, UInt_t h)
  : TGMainFrame(p, w, h)
{  
  dMWWidth = w;
  dMWHeight = h;
  
  std::set_new_handler(0);

  MakeMenuLayout();
  MakeCountingModeTab(); 
  MapSubwindows();
  Resize(GetDefaultSize());
  MapWindow();
  
  QTPBaseAddr = 0;
  DiscrBaseAddr = 0;
  handle = -1; 
  VMEerror = 0;

  
  DataType = DATATYPE_HEADER;
  EnableHistoFiles = 0;		        
  EnableListFile = 0;			
  EnableRawDataFile = 0;		
  EnableSuppression = 1;		
  DiscrChMask = 0;		        
  DiscrOutputWidth = 10;	        
  Iped = 255;
  QDCChannel = 0;
  BoardNChannels = 32;
  VMEerror = 0;
   
  for(int i=0; i < NUM_CHANNELS; i++) {
    NSamples[i]=0;
    if(i < 16)
      DiscrThreshold[i] = 5;
    QTP_LLD[i] = 0;
  }

  string label;
  TString arg;
  TString argp;
  int nargs = *argc, n = 0;
  if(nargs >= 2){
    while(n < nargs){      
      arg = argv[n];
      if(arg == Form("-c") && n < nargs-1){
	argp = argv[n+1];	
	OpenConfigFile(argp.Data());
	ConfigureVMEModule();
	n += 2;
      }
      else
	n++;

    }
  }
  
}

uint16_t EMMonitor::ReadRegister(uint16_t reg_addr)
{
  uint16_t data=0;
  CVErrorCodes ret;
  ret = CAENVME_ReadCycle(handle, BaseAddress + reg_addr, &data, cvA32_U_DATA, cvD16);
  if(ret != cvSuccess) {
    sprintf(ErrorString, "Cannot read at address %08X\n", (uint32_t)(BaseAddress + reg_addr));
    VMEerror = 1;
  }
  // if (ENABLE_LOG)
  //   fprintf(logfile, " Reading register at address %08X; data=%04X; ret=%d\n", (uint32_t)(BaseAddress + reg_addr), data, (int)ret);
  return(data);
}

void EMMonitor::WriteRegister(uint16_t reg_addr, uint16_t data)
{
  CVErrorCodes ret;
  ret = CAENVME_WriteCycle(handle, BaseAddress + reg_addr, &data, cvA32_U_DATA, cvD16);
  if(ret != cvSuccess) {
    sprintf(ErrorString, "Cannot write at address %08X\n", (uint32_t)(BaseAddress + reg_addr));
    VMEerror = 1;
  }
  // if (ENABLE_LOG)
  //   fprintf(logfile, " Writing register at address %08X; data=%04X; ret=%d\n", (uint32_t)(BaseAddress + reg_addr), data, (int)ret);
}

int EMMonitor::ConfigureDiscr(uint16_t OutputWidth, uint16_t Threshold[16], uint16_t EnableMask)
{
  int i;
  
  BaseAddress = DiscrBaseAddr;
  // Set channel mask
  WriteRegister(0x004A, EnableMask);
  
  // set output width (same for all channels)
  WriteRegister(0x0040, OutputWidth);
  WriteRegister(0x0042, OutputWidth);
  
  // set CFD threshold
  for(i=0; i<16; i++)
    WriteRegister(i*2, Threshold[i]);
  
  if (VMEerror) {
    printf("Error during CFD programming: ");
    //   printf(ErrorString);
    return -1;
  } else {
    printf("Discriminator programmed successfully\n");
    return 0;
  }
  BaseAddress = QTPBaseAddr;
  
}


void EMMonitor::FindModelVersion(uint16_t model, uint16_t vers, char *modelVersion, int *ch)
{
  switch (model) {
  case 792:
    switch (vers) {
    case 0x11:
      strcpy(modelVersion, "AA");
      *ch = 32;
      return;
    case 0x13:
      strcpy(modelVersion, "AC");
      *ch = 32;
      return;
    case 0xE1:
      strcpy(modelVersion, "NA");
      *ch = 16;
      return;
    case 0xE3:
      strcpy(modelVersion, "NC");
      *ch = 16;
      return;
    default:
      strcpy(modelVersion, "-");
      *ch = 32;
      return;
    }
    break;
  case 965:
    switch (vers) {
    case 0x1E:
      strcpy(modelVersion, "A");
      *ch = 16;
      return;
    case 0xE3:
    case 0xE1:
      strcpy(modelVersion, " ");
      *ch = 32;
      return;
    default:
      strcpy(modelVersion, "-");
      *ch = 32;
      return;
    }
    break;
  case 775:
    switch (vers) {
    case 0x11:
      strcpy(modelVersion, "AA");
      *ch = 32;
      return;
    case 0x13:
      strcpy(modelVersion, "AC");
      *ch = 32;
      return;
    case 0xE1:
      strcpy(modelVersion, "NA");
      *ch = 16;
      return;
    case 0xE3:
      strcpy(modelVersion, "NC");
      *ch = 16;
      return;
    default:
      strcpy(modelVersion, "-");
      *ch = 32;
      return;
    }
    break;
  case 785:
    switch (vers) {
    case 0x11:
      strcpy(modelVersion, "AA");
      *ch = 32;
      return;
    case 0x12:
      strcpy(modelVersion, "Ab");
      *ch = 32;
      return;
    case 0x13:
      strcpy(modelVersion, "AC");
      *ch = 32;
      return;
    case 0x14:
      strcpy(modelVersion, "AD");
      *ch = 32;
      return;
    case 0x15:
      strcpy(modelVersion, "AE");
      *ch = 32;
      return;
    case 0x16:
      strcpy(modelVersion, "AF");
      *ch = 32;
      return;
    case 0x17:
      strcpy(modelVersion, "AG");
      *ch = 32;
      return;
    case 0x18:
      strcpy(modelVersion, "AH");
      *ch = 32;
      return;
    case 0x1B:
      strcpy(modelVersion, "AK");
      *ch = 32;
      return;
    case 0xE1:
      strcpy(modelVersion, "NA");
      *ch = 16;
      return;
    case 0xE2:
      strcpy(modelVersion, "NB");
      *ch = 16;
      return;
    case 0xE3:
      strcpy(modelVersion, "NC");
      *ch = 16;
      return;
    case 0xE4:
      strcpy(modelVersion, "ND");
      *ch = 16;
      return;
    default:
      strcpy(modelVersion, "-");
      *ch = 32;
      return;
    }
    break;
  case 862:
    switch (vers) {
    case 0x11:
      strcpy(modelVersion, "AA");
      *ch = 32;
      return;
    case 0x13:
      strcpy(modelVersion, "AC");
      *ch = 32;
      return;
    default:
      strcpy(modelVersion, "-");
      *ch = 32;
      return;
    }
    break;
  }
  
}



void EMMonitor::StartDataCollection()
{
  RUN_START = true;
  RUN_STOP = false;
  // RUN_PAUSE = false;
  // RUN_ON = true;
  // std::thread th1(&EMMonitor::GetServerData,this,&dataQue,aData,&RUN_ON);
  // th1.detach();

  GetData();
  //FillDataPlots();
}

void EMMonitor::ClearDataBuffers()
{
  for(int n = 0; n < NUM_CHANNELS; n++){
    ChSigHst[n]->Reset();
    //dCurrMSmplHstCv[n]->GetCanvas()->cd();
      // ChSigHst[k]->Draw();
      // dCurrMSmplHstCv[k]->GetCanvas()->Update();
      // pt = (TPaveText*)(dCurrMSmplHstCv[k]->GetCanvas()->GetPrimitive("title"));
      // pt->SetTextSize(0.05);
    dCurrMSmplHstCv[n]->GetCanvas()->Modified(); 
    dCurrMSmplHstCv[n]->GetCanvas()->Update();

  }
}

void EMMonitor::GetData()
{

  printf("Acquisition Started. Plot is currently set on channel %d\n", QDCChannel);

  int  DataError=0;//, LogMeas=0, lognum=0;
  // int link=0, bdnum=0;
  // uint16_t fwrev, vers, sernum, model;
  // uint16_t Iped = 255;			// pedestal of the QDC (or resolution of the TDC)
  // uint32_t histo[32][4096];		// histograms (charge, peak or TAC)
  // uint32_t buffer[MAX_BLT_SIZE/4];// readout buffer (raw data from the board)
  // uint16_t ADCdata[32];			// ADC data (charge, peak or TAC)
  // long CurrentTime, PrevPlotTime, PrevKbTime, ElapsedTime;	// time of the PC
  // FILE *gnuplot=NULL;				// gnuplot (will be opened in a pipe)
  
  char c;
  int quit = 0;
  int pnt = 0;  // word pointer
  int wcnt = 0; // num of lword read in the MBLT cycle
  int i, j, nch, chindex, nev = 0, bcnt, totnb=0;
  uint32_t buffer[MAX_BLT_SIZE/4];// readout buffer (raw data from the board)
  buffer[0] = DATATYPE_FILLER;
  uint16_t ADCdata[BoardNChannels];			// ADC data (charge, peak or TAC)
  DataType = DATATYPE_HEADER;
  long CurrentTime, PrevPlotTime, PrevKbTime, ElapsedTime;	// time of the PC

  float rate = 0.0;				// trigger rate
  FILE *of_list=NULL;				// list data file
  FILE *of_raw=NULL;				// raw data file
  FILE *f_ini;					// config file
  FILE *fh;						// plotting data file 
  
  
  // clear Event Counter
  WriteRegister(0x1040, 0x0);
  // clear QTP
  WriteRegister(0x1032, 0x4);
  WriteRegister(0x1034, 0x4);
  
  PrevPlotTime = get_time();
  PrevKbTime = PrevPlotTime;
  
  // ------------------------------------------------------------------------------------
  // Acquisition loop
  // ------------------------------------------------------------------------------------
  while(1)  {    
    
    //The actual acqusition:
    if ((pnt == wcnt) || ((buffer[pnt] & DATATYPE_MASK) == DATATYPE_FILLER)) {
      CAENVME_FIFOMBLTReadCycle(handle, BaseAddress, (char *)buffer, MAX_BLT_SIZE, cvA32_U_MBLT, &bcnt);
      // if (ENABLE_LOG && (bcnt>0)) {
      // 	int b;
      // 	fprintf(logfile, "Read Data Block: size = %d bytes\n", bcnt);
      // 	for(b=0; b<(bcnt/4); b++)
      // 	  fprintf(logfile, "%2d: %08X\n", b, buffer[b]);
      // }
      wcnt = bcnt/4;
      totnb += bcnt;
      pnt = 0;
    }
    if (wcnt == 0)  // no data available
      continue;
    
    // save raw data (board memory dump)
    if (of_raw != NULL)
      fwrite(buffer, sizeof(char), bcnt, of_raw);
    
    /* header */
    switch (DataType) {
    case DATATYPE_HEADER :
      if((buffer[pnt] & DATATYPE_MASK) != DATATYPE_HEADER) {
	//printf("Header not found: %08X (pnt=%d)\n", buffer[pnt], pnt);
	DataError = 1;
      } else {
	nch = (buffer[pnt] >> 8) & 0x3F;
	chindex = 0;
	nev++;
	memset(ADCdata, 0xFFFF, 32*sizeof(uint16_t));
	if (nch>0)
	  DataType = DATATYPE_CHDATA;
	else
	  DataType = DATATYPE_EOB;
      }
      break;
      
      /* Channel data */
    case DATATYPE_CHDATA :
      if((buffer[pnt] & DATATYPE_MASK) != DATATYPE_CHDATA) {
	//printf("Wrong Channel Data: %08X (pnt=%d)\n", buffer[pnt], pnt);
	DataError = 1;
      } else {
	if (BoardNChannels == 32)
	  j = (int)((buffer[pnt] >> 16) & 0x3F);  // for V792 (32 channels)
	else
	  j = (int)((buffer[pnt] >> 17) & 0x3F);  // for V792N (16 channels)

	//****************************************************
	//Fill Root Histogram here 
	// histo[j][buffer[pnt] & 0xFFF]++;
	ChSigHst[j]->Fill(buffer[pnt] & 0xFFF);
	//****************************************************
	
	ADCdata[j] = buffer[pnt] & 0xFFF;
	NSamples[j]++;
	if (chindex == (nch-1))
	  DataType = DATATYPE_EOB;
	chindex++;
      }
      break;

      /* EOB */
    case DATATYPE_EOB :
      if((buffer[pnt] & DATATYPE_MASK) != DATATYPE_EOB) {
	//printf("EOB not found: %08X (pnt=%d)\n", buffer[pnt], pnt);
	DataError = 1;
      } else {
	DataType = DATATYPE_HEADER;
	if (of_list != NULL) {
	  fprintf(of_list, "Event Num. %d\n", buffer[pnt] & 0xFFFFFF);
	  for(i=0; i< BoardNChannels; i++) {
	    if (ADCdata[i] != 0xFFFF)
	      fprintf(of_list, "Ch %2d: %d\n", i, ADCdata[i]);
	  }
	}
      }
      break;
    }
    pnt++;
    
    if (DataError) {
      pnt = wcnt;
      WriteRegister(0x1032, 0x4);
      WriteRegister(0x1034, 0x4);
      DataType = DATATYPE_HEADER;
      DataError=0;
    }


    CurrentTime = get_time(); // Time in milliseconds
    if ((CurrentTime - PrevKbTime) > 2000) {
      for(int n = 0; n < NUM_CHANNELS; n++){
	dCurrMSmplHstCv[n]->GetCanvas()->cd();
	gPad->Modified();
	gPad->Update();
      }      
      PrevKbTime = CurrentTime;
    }
    gSystem->ProcessEvents();
    
    if(RUN_STOP) break;
  }
  
}

// void EMMonitor::FillDataPlots()
// {
//   double t1 = 0, t2 = 0;
//   int p = 0;

//   while(1){

//     for(int n = 0; n < BoardNChannels; n++){

      
	
//       if(n == 0){
// 	  ChSigHst[n]->Fill(data->ch0smp*2.048/131072);
// 	  if(!(p%10000)){
// 	    dCurrMSmplHstCv[n]->GetCanvas()->cd();
// 	    // ChSigHst[n]->Draw();
// 	    gPad->Modified();
// 	    gPad->Update();
// 	  }
// 	}
// 	if(n == 1){
// 	  ChSigHst[n]->Fill(data->ch1smp*2.048/131072);
// 	  if(!(p%10000)){
// 	    dCurrMSmplHstCv[n]->GetCanvas()->cd();
// 	    // ChSigHst[n]->Draw();
// 	    gPad->Modified();
// 	    gPad->Update();	
// 	  }
// 	}
//       }
//       t1 = data->ts/125.0;
//       p++;
//       dataQue.pop();
//       delete data;
//       if(!(p%10000)){
// 	//cout << "queue size = " << dataQue.size() << endl;
// 	dRateCounter[0]->SetText(Form("%ld",dataQue.size()));
// 	dRateFrame[0]->Layout();
// 	// gSystem->ProcessEvents();
//       }
//   }
//   else if(RUN_STOP) break;
//   gSystem->ProcessEvents();

// }


void EMMonitor::MakeMenuLayout()
{
  
  dMenuFile = new TGPopupMenu(fClient->GetRoot());  
  dMenuFile->AddEntry("&Write (Text output)...", M_TEXT_FILE_WRITE);
  dMenuFile->AddEntry("&RWrite (ROOT output)...", M_ROOT_FILE_WRITE);  
  dMenuFile->AddEntry("&Open (Text file)...", M_FILE_OPEN);
  dMenuFile->AddEntry("RO&pen (ROOT file)...", M_ROOT_FILE_OPEN);
  //dMenuFile->AddEntry("Open Data Window",M_OPEN_DATAWINDOW);
  dMenuFile->AddEntry("&Close (Text file ...)", M_FILE_CLOSE);
  dMenuFile->AddEntry("C&lose (ROOT file ...)", M_ROOT_FILE_CLOSE);  
  dMenuFile->AddSeparator();
  dMenuFile->AddEntry("Root File Browser", M_VIEW_BROWSER);
  dMenuFile->AddSeparator();
  dMenuFile->AddSeparator();
  dMenuFile->AddEntry("E&xit", M_FILE_EXIT);

  dMenuRunCntrl = new TGPopupMenu(fClient->GetRoot());
  //dMenuRunCntrl->AddEntry("Set Connection Options...", M_SET_CONNECT);
  //dMenuRunCntrl->AddEntry("Cofigure...", M_CONNECT);
  dMenuRunCntrl->AddEntry("Start...", M_RUN_START);
  dMenuRunCntrl->AddEntry("Stop...", M_RUN_STOP);
  // dMenuRunCntrl->AddEntry("Pause...", M_RUN_PAUSE);
  dMenuRunCntrl->AddEntry("CLear (Data)...", M_RUN_CLEAR);

  dMenuConfig = new TGPopupMenu(fClient->GetRoot());
  //dMenuRunCntrl->AddEntry("Set Connection Options...", M_SET_CONNECT);
  dMenuConfig->AddEntry("Cofigure...", M_CONNECT);

  dMenuFile->Associate(this);
  dMenuRunCntrl->Associate(this);
  dMenuConfig->Associate(this);
  
  dMenuBar = new TGMenuBar(this, 1, 1, kHorizontalFrame);
  dMenuBar->AddPopup("&File", dMenuFile, new TGLayoutHints(kLHintsTop | kLHintsLeft ,0, 4, 0, 0));
  dMenuBar->AddPopup("&Config",dMenuConfig, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0));
  dMenuBar->AddPopup("&Run",dMenuRunCntrl, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0));
  
  AddFrame(dMenuBar, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX,0, 0, 1, 1));
}

void EMMonitor::MakeCountingModeTab()
{
  TGTab *Tab = new TGTab(this,dMWWidth-15,dMWHeight-80);  
  TGCompositeFrame *tf = Tab->AddTab("Channel 0-15");
  TGVerticalFrame *vf1 = new TGVerticalFrame(tf,10,10);
  
  TGHorizontalFrame *hf[NUM_CHANNELS/8];
  //TGVerticalFrame *vf[NUM_CHANNELS/8];
  // TGLabel *dChanLabel[NUM_CHANNELS];
  // TGLabel *dRateLabel[NUM_CHANNELS];

  TPaveText *pt;
 

  int k = 0;
  for(int n = 0; n < NUM_CHANNELS/8; n++){

    hf[n] = new TGHorizontalFrame(vf1,10,10);

    for(int m = 0; m < NUM_CHANNELS/8; m++){
      k = m + n*NUM_CHANNELS/8;
      dCurrMSmplHstCv[k] = new TRootEmbeddedCanvas(Form("SmplHst_Ch%d",k), hf[n], 10, 10);
      hf[n]->AddFrame(dCurrMSmplHstCv[k],new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsExpandY,2, 2, 5, 5));
      ChSigHst[k] = new TH1D(Form("HstCh_%02d",k),Form("Channel %02d",k),4096,0,4096);
    }
    vf1->AddFrame(hf[n], new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
    if(n < NUM_CHANNELS/8 -1){
      TGHorizontal3DLine *Line = new TGHorizontal3DLine(vf1);
      vf1->AddFrame(Line, new TGLayoutHints(kLHintsTop | kLHintsExpandX));
    }
    
    for(int m = 0; m < NUM_CHANNELS/8; m++){
      k = m + n*NUM_CHANNELS/8;
      dCurrMSmplHstCv[k]->GetCanvas()->cd();
      ChSigHst[k]->Draw();
      dCurrMSmplHstCv[k]->GetCanvas()->Update();
      pt = (TPaveText*)(dCurrMSmplHstCv[k]->GetCanvas()->GetPrimitive("title"));
      pt->SetTextSize(0.05);
      dCurrMSmplHstCv[k]->GetCanvas()->Modified(); 
      dCurrMSmplHstCv[k]->GetCanvas()->Update();
    }
    
  }
  
  vf1->Resize(dMWWidth-15,dMWHeight-15);
  tf->AddFrame(vf1,new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
  AddFrame(Tab, new TGLayoutHints(kLHintsBottom | kLHintsExpandX | kLHintsExpandY,2, 2, 5, 1));
  Tab->MapSubwindows();
  Tab->Layout();

  //Tab = new TGTab(this,dMWWidth-15,dMWHeight-80);  
  TGCompositeFrame *tf2 = Tab->AddTab("Channel 16-31");
  TGVerticalFrame *vf2 = new TGVerticalFrame(tf2,10,10);
  
  TGHorizontalFrame *hf2[NUM_CHANNELS/8];

  k = 0;
  for(int n = 0; n < NUM_CHANNELS/8; n++){

    hf2[n] = new TGHorizontalFrame(vf2,10,10);

    for(int m = 0; m < NUM_CHANNELS/8; m++){
      k = 16 + m + n*NUM_CHANNELS/8;
      dCurrMSmplHstCv[k] = new TRootEmbeddedCanvas(Form("SmplHst_Ch%d",k), hf2[n], 10, 10);
      hf2[n]->AddFrame(dCurrMSmplHstCv[k],new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsExpandY,2, 2, 5, 5));
      ChSigHst[k] = new TH1D(Form("HstCh_%02d",k),Form("Channel %02d",k),4096,0,4096);
    }
    vf2->AddFrame(hf2[n], new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
    if(n < NUM_CHANNELS/8 -1){
      TGHorizontal3DLine *Line = new TGHorizontal3DLine(vf2);
      vf2->AddFrame(Line, new TGLayoutHints(kLHintsTop | kLHintsExpandX));
    }
    for(int m = 0; m < NUM_CHANNELS/8; m++){
      k = 16 + m + n*NUM_CHANNELS/8;
      dCurrMSmplHstCv[k]->GetCanvas()->cd();
      gStyle->SetTitleFontSize(0.05);
      ChSigHst[k]->Draw();
      dCurrMSmplHstCv[k]->GetCanvas()->Update();
      pt = (TPaveText*)(dCurrMSmplHstCv[k]->GetCanvas()->GetPrimitive("title"));
      pt->SetTextSize(0.1);
      dCurrMSmplHstCv[k]->GetCanvas()->Modified(); 
      dCurrMSmplHstCv[k]->GetCanvas()->Update();
    }
    
  }
  
  vf2->Resize(dMWWidth-15,dMWHeight-15);
  tf2->AddFrame(vf2,new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
  // AddFrame(Tab, new TGLayoutHints(kLHintsBottom | kLHintsExpandX | kLHintsExpandY,2, 2, 5, 1));
  Tab->MapSubwindows();
  Tab->Layout();

   // for(int n = 0; n < NUM_CHANNELS; n++){

  //   hf[n] = new TGHorizontalFrame(vf1,10,10);
  //   vLabF[n] = new TGVerticalFrame(hf[n],10,10);
  //   vLabF[n]->Resize(400,10); //this doesn't seem to be doing anything in the case :(
  //   dChanLabel[n] = new TGLabel(vLabF[n],Form("Channel %02d                    ",n)); //leave the empty space to stretch the label
  //   vLabF[n]->AddFrame(dChanLabel[n],new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));
  //   dRateFrame[n] = new TGHorizontalFrame(vLabF[n],10,10);
  //   dRateLabel[n] = new TGLabel(dRateFrame[n],"Rate:  "); //leave the empty space to stretch the label
  //   dRateCounter[n] = new TGLabel(dRateFrame[n],"1.0");
  //   dRateFrame[n]->AddFrame(dRateLabel[n],new TGLayoutHints(kLHintsTop | kLHintsLeft, 10, 2, 2, 2));
  //   dRateFrame[n]->AddFrame(dRateCounter[n],new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));
  //   vLabF[n]->AddFrame(dRateFrame[n],new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));   
  //   dCurrMSmplGrCv[n] = new TRootEmbeddedCanvas(Form("SmplGr_Ch%d",n), hf[n], 10, 10);
  //   dCurrMSmplHstCv[n] = new TRootEmbeddedCanvas(Form("SmplHst_Ch%d",n), hf[n], 10, 10);
  //   dCurrMSmplFFTCv[n] = new TRootEmbeddedCanvas(Form("SmplFFT_Ch%d",n), hf[n], 10, 10);
  //   hf[n]->AddFrame(vLabF[n], new TGLayoutHints( kLHintsLeft | kLHintsTop | kLHintsExpandY, 2, 2, 5, 5));
  //   hf[n]->AddFrame(dCurrMSmplGrCv[n],new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsExpandY, 2, 2, 5, 5));
  //   hf[n]->AddFrame(dCurrMSmplHstCv[n],new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsExpandY,2, 2, 5, 5));
  //   hf[n]->AddFrame(dCurrMSmplFFTCv[n],new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsExpandY,2, 2, 5, 5));
  //   vf1->AddFrame(hf[n], new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
  //   if(n < NUM_CHANNELS-1){
  //     TGHorizontal3DLine *Line = new TGHorizontal3DLine(vf1);
  //     vf1->AddFrame(Line, new TGLayoutHints(kLHintsTop | kLHintsExpandX));
  //   }


}



void EMMonitor::OnObjClose(char *obj)
{
  TString str = obj;

  if(str.Contains("dataWindow")) CloseWindow();
}

EMMonitor::~EMMonitor()
{

}

void EMMonitor::CloseWindow()
{
  gApplication->Terminate(0);
}

void EMMonitor::PadIsPicked(TPad* selpad, TObject* selected, Int_t event)
{
  printf("Pad number = %d\n",selpad->GetNumber());
}

void EMMonitor::MainTabEvent(Int_t event, Int_t x, Int_t y, TObject* selobject)
{
  if(event == kButton1Double){
    // Int_t pad = dMainCanvas->GetCanvas()->GetSelectedPad()->GetNumber();

    // if(pad == 3){
    //   // if(!dM2M1RatioGraph) return;
    //   // PlotMiscData(dM2M1RatioGraph);
    // }
    // if(pad == 4){
    //   // if(!dPolGraph) return;
    //   // PlotMiscData(dPolGraph);
    // }
    // if(pad == 5){
    //   // if(!dM3M2RatioGraph) return;
    //   // PlotMiscData(dM3M2RatioGraph);
    // }
  }
}


Bool_t EMMonitor::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  // Handle messages send to the MainFrame object. E.g. all menu button
  // messages.
  switch (GET_MSG(msg)){

  case kC_TEXTENTRY:
    switch (GET_SUBMSG(msg)) {
    case kTE_ENTER:
      switch (parm1) {

      case M_RUN_SELECT:

      default:
	break;
      }

    default:
      break;
    }

  case kC_COMMAND:
    switch (GET_SUBMSG(msg)) {
      
    case kCM_COMBOBOX:
      {
	switch (parm1) {
	case M_TBIN_SELECT:
	  break;
	}
      }
      break;

    case kCM_MENUSELECT:
      break;
      
    case kCM_MENU:
      switch (parm1) {
      
      case M_ROOT_FILE_OPEN:
	//OpenRootFile();
	break;
	
      case M_FILE_OPEN:

	break;

      case M_OPEN_DATAWINDOW:
	break;
	
      case M_FILE_EXIT:
	CloseWindow();   // this also terminates theApp
	break;
	
      case M_ROOT_FILE_CLOSE:
	// CloseRootFile();
	break;
	
      case M_FILE_CLOSE:
	break;

      case M_TEXT_FILE_WRITE:
	WriteDataTxt();
	break;

      case M_CONNECT:
	if(OpenConfigFile(0) !=  PROCESS_FAILED)
	  ConfigureVMEModule();
	break;

      case M_RUN_START:
	StartDataCollection();
	break;

      case M_RUN_STOP:
	RUN_STOP = true;
	RUN_START = false;
	// RUN_ON = false;
	break;

      case M_RUN_CLEAR:
	ClearDataBuffers();

      case M_VIEW_BROWSER:
	// OpenRootBrowser();
	break;

      case M_HELP_CONTENTS:
	// Open the online help manual in a web browser.
        { 
		// pid_t child = fork(); 
		// if (-1 == child) perror("couldn't fork to open web browser"); 
		// if (0 == child) {
		// 	execl("/bin/sh", "/bin/sh", "-c", 
		// 	      "mozilla "
		// 	      "http://sns.phys.utk.edu/svn/npdg/trunk/analysis/online/Manual/OnLineAnalysisTasks.html",
		// 	      (char*)0); 
		// 	perror("couldn't exec shell for web browser"); 
		// 	exit(1); 
		// }
	}
	break;

      case M_HELP_SEARCH:
	 {
// #		define URL "http://sns.phys.utk.edu/svn/npdg/trunk/analysis/online"
// 		pid_t child = fork(); 
// 		if (-1 == child) perror("couldn't fork to show change history"); 
// 		if (0 == child) {
// 			execl("/bin/sh", "/bin/sh", "-c", 
// 			      "xterm -title 'Online Analysis Change History' "
// 			      " -e \" "
// 			      "(echo changes to " URL " && svn log " URL " ) | less"
// 			      " \" "
// 			      , (char*)0); 
// 			perror("couldn't exec shell for web browser"); 
// 			exit(1); 
// 		}
	 }
	 break;

      case M_HELP_ABOUT:
	// were I able to make a cute box, I would put it here
	break;

      default:
	break;
      }
      
    default:
      break;
    }
    
  default:
    break;
  }
  
  return kTRUE;
}



Int_t EMMonitor::WriteDataTxt(const char* file)
{  
  char filename[NAME_STR_MAX];
  if(!file){
    if(GetFilenameFromDialog(filename,"cnf",FS_OLD) == PROCESS_FAILED)
      return PROCESS_FAILED;
  }
  else
    strcpy(filename,file);  
      
  ofstream *oFile = new ofstream(filename);
  if(oFile){
    if(oFile->is_open()){

      *oFile << ChSigHst[0]->GetEntries();
      *oFile << '\n';

      for(int b = 1; b <= 4096; b++){

	for(int c = 0; c < 32; c++){

	  *oFile << ChSigHst[c]->GetBinContent(b);
	  *oFile << '\t';
	  
	}
	*oFile << '\n';
      }

      oFile->close();
    }    
  }
  cout << "Done Writing Data To File." << endl; 

  return PROCESS_OK;;
  
}

Int_t EMMonitor::OpenConfigFile(const char* file)
{  
  char filename[NAME_STR_MAX];
  if(!file){
    if(GetFilenameFromDialog(filename,"cnf",FS_OLD) == PROCESS_FAILED)
      return PROCESS_FAILED;
  }
  else
    strcpy(filename,file);

  if(strcmp(".cnf",strrchr(filename,'.'))) return PROCESS_FAILED;

  FILE *f_ini = NULL; 

  if(!(f_ini = fopen(filename, "r"))){  
    printf("Can't open Configuration File %s\n", filename);
    return PROCESS_FAILED;
  }

  printf("Reading Configuration File %s\n", filename);
  while(!feof(f_ini)) {
    char str[500];
    int data;
    
    str[0] = '#';
    fscanf(f_ini, "%s", str);
    if (str[0] == '#')
      fgets(str, 1000, f_ini);
    else {
      
      // Output Files
      if (strstr(str, "ENABLE_LIST_FILE")!=NULL) fscanf(f_ini, "%d", &EnableListFile);
      if (strstr(str, "ENABLE_HISTO_FILES")!=NULL) fscanf(f_ini, "%d", &EnableHistoFiles);
      if (strstr(str, "ENABLE_RAW_DATA_FILE")!=NULL) fscanf(f_ini, "%d", &EnableRawDataFile);
      
      // Base Addresses
      if (strstr(str, "QTP_BASE_ADDRESS")!=NULL)
	fscanf(f_ini, "%x", &QTPBaseAddr);
      if (strstr(str, "DISCR_BASE_ADDRESS")!=NULL)
	fscanf(f_ini, "%x", &DiscrBaseAddr);
      
      // I-pedestal
      if (strstr(str, "IPED")!=NULL) {
	fscanf(f_ini, "%d", &data);
	Iped = (uint16_t)data;
      }
      
      // Discr_ChannelMask
      if (strstr(str, "DISCR_CHANNEL_MASK")!=NULL) {
	fscanf(f_ini, "%x", &data);
	DiscrChMask = (uint16_t)data;
      }
      
      // Discr_OutputWidth
      if (strstr(str, "DISCR_OUTPUT_WIDTH")!=NULL) {
	fscanf(f_ini, "%d", &data);
	DiscrOutputWidth = (uint16_t)data;
      }
      
      // Discr_Threshold
      if (strstr(str, "DISCR_THRESHOLD")!=NULL) {
	int ch, thr;
	fscanf(f_ini, "%d", &ch);
	fscanf(f_ini, "%d", &thr);
	if (ch < 0) {
	  for(int i=0; i<16; i++)
	    DiscrThreshold[i] = thr;
	} else if (ch < 16) {
	  DiscrThreshold[ch] = thr; 
	}
      }
      
      // LLD for the QTP 
      if (strstr(str, "QTP_LLD")!=NULL) {
	int ch, lld;
	fscanf(f_ini, "%d", &ch);
	fscanf(f_ini, "%d", &lld);
	if (ch < 0) {
	  for(int i=0; i< BoardNChannels; i++)
	    QTP_LLD[i] = lld;
	} else if (ch < BoardNChannels) {
	  QTP_LLD[ch] = lld; 
	}
      }
      
      
      // I-pedestal
      if (strstr(str, "ENABLE_SUPPRESSION")!=NULL) {
	fscanf(f_ini, "%d", &EnableSuppression);
      }
      
      
    }
  }
  fclose (f_ini);

  return PROCESS_OK;

}


Int_t EMMonitor::ConfigureVMEModule()
{
  // int i, j, ch=0, chindex, wcnt, nch, pnt, ns[32], bcnt;
  char modelVersion[3];
  uint16_t fwrev, vers, sernum, model;
  int link=0, bdnum=0;

  // open VME bridge (V1718 or V2718)
  if (CAENVME_Init(cvV1718, link, bdnum, &handle) != cvSuccess) {
    if (CAENVME_Init(cvV2718, link, bdnum, &handle) != cvSuccess) {
      printf("Can't open VME controller\n");
      Sleep(1000);
      if (handle >= 0) CAENVME_End(handle);
      return PROCESS_FAILED;
    }
  }

  // Program the discriminator (if the base address is set in the config file)
  if (DiscrBaseAddr > 0) {
    int ret;
    printf("Discr Base Address = 0x%08X\n", DiscrBaseAddr);
    ret = ConfigureDiscr(DiscrOutputWidth, DiscrThreshold, DiscrChMask);
    if (ret < 0) {
      printf("Can't access to the discriminator at Base Address 0x%08X\n", DiscrBaseAddr);
      printf("Skipping Discriminator configuration\n");
    }
  }

  // Check if the base address of the QTP board has been set (otherwise exit)
  if (QTPBaseAddr == 0) {
    printf("No Base Address setting found for the QTP board.\n");
    printf("Skipping QTP readout\n");
    if (handle >= 0) CAENVME_End(handle);
    return PROCESS_FAILED;
  }
  printf("QTP Base Address = 0x%08X\n", QTPBaseAddr);
  BaseAddress = QTPBaseAddr;

  // clear histograms
  for(int i=0; i< BoardNChannels; i++) {
    NSamples[i]=0;
    ChSigHst[i]->Reset();
  }


  WriteRegister(0x1016, 0);
  if (VMEerror) {
    printf("Error during QTP programming: ");
    if (handle >= 0) CAENVME_End(handle);    
    return PROCESS_FAILED;
  }

  // Read FW revision
  fwrev = ReadRegister(0x1000);
  if (VMEerror) {
    if (handle >= 0) CAENVME_End(handle);    
    return PROCESS_FAILED;
  }

  model = (ReadRegister(0x803E) & 0xFF) + ((ReadRegister(0x803A) & 0xFF) << 8);
  // read version (> 0xE0 = 16 channels)
  vers = ReadRegister(0x8032) & 0xFF;
  
  FindModelVersion(model, vers, modelVersion, &BoardNChannels);
  printf("Model = V%d%s\n", model, modelVersion);
  
  // Read serial number
  sernum = (ReadRegister(0x8F06) & 0xFF) + ((ReadRegister(0x8F02) & 0xFF) << 8);
  printf("Serial Number = %d\n", sernum);
  
  printf("FW Revision = %d.%d\n", (fwrev >> 8) & 0xFF, fwrev & 0xFF);
  
  WriteRegister(0x1060, Iped);  // Set pedestal
  WriteRegister(0x1010, 0x60);  // enable BERR to close BLT at and of block
  
  // Set LLD (low level threshold for ADC data)
  WriteRegister(0x1034, 0x100);  // set threshold step = 16
  for(int i=0; i<BoardNChannels; i++) {
    if (BoardNChannels == 16)
      WriteRegister(0x1080 + i*4, QTP_LLD[i]/16);
    else
      WriteRegister(0x1080 + i*2, QTP_LLD[i]/16);
  }
  
  if (!EnableSuppression) {
    WriteRegister(0x1032, 0x0010);  // disable zero suppression
    WriteRegister(0x1032, 0x0008);  // disable overrange suppression
    WriteRegister(0x1032, 0x1000);  // enable empty events
  }
  
  //printf("Ctrl Reg = %04X\n", read_reg(0x1032));  
  printf("QTP board programmed\n");
  
  return PROCESS_OK;
}

Int_t EMMonitor::OpenOutputFiles()
{
  return 0;
}

Int_t EMMonitor::GetFilenameFromDialog(char *file, const char *ext,
				       ERFileStatus status, Bool_t kNotify,
				       const char *notifytext)
{
  int retval = 0;

  if(kNotify && notifytext){

    new TGMsgBox(fClient->GetRoot(), this,"File Open Operation",
		 notifytext,kMBIconQuestion, kMBOk | kMBCancel, &retval);
    if(retval == kMBCancel) return PROCESS_FAILED;
  }

  if(!ext || !*ext) return PROCESS_FAILED;
  if(!file) return PROCESS_FAILED;
  TString fext = ext;
  const char *filetypes[12];
  Int_t index = 0;


  if(fext.Contains("dat")){
     filetypes[index] = "Data files"; index++;
     filetypes[index] = "*.dat"; index++;
  }
  if(fext.Contains("txt")){
     filetypes[index] = "Config files"; index++;
     filetypes[index] = "*.cnf"; index++;
  }
  if(fext.Contains("csv")){
     filetypes[index] = "Data files"; index++;
     filetypes[index] = "*.csv"; index++;
  }
  if(fext.Contains("root")){
     filetypes[index] = "Root files"; index++;
     filetypes[index] = "*.root"; index++;
  }
  if(fext.Contains("log")){
     filetypes[index] = "Log files"; index++;
     filetypes[index] = "*.log"; index++;
  }
  filetypes[index] = "All files"; index++;
  filetypes[index] = "*"; index++;
  filetypes[index] = 0; index++;
  filetypes[index] = 0; index++;

  // sprintf(dMiscbuffer2,"/home/",gSystem->Getenv("USER"));
  static TString dir("./");
  TGFileInfo fi;

  if(status == FS_OLD){
    fi.fFileTypes = (const char **)filetypes;
    fi.fIniDir    = StrDup(dir);
    new TGFileDialog(fClient->GetRoot(), this, kFDOpen, &fi);
    dir = fi.fIniDir;
    if(!fi.fFilename) {return PROCESS_FAILED;};
  }

  if(status == FS_NEW){
    fi.fFileTypes = (const char **)filetypes;
    fi.fIniDir    = StrDup(dir);
    new TGFileDialog(fClient->GetRoot(), this, kFDSave, &fi);
    dir = fi.fIniDir;

    if(!fi.fFilename) {return PROCESS_FAILED;};
  }
  strcpy(file,fi.fFilename);
  return PROCESS_OK;
}


int main(int argc, char *argv[])
{
  int argl = 1;

  
   TApplication theApp("EMMonitor", &argl, argv);

   if (gROOT->IsBatch()) {
      fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
      return 1;
   }

   EMMonitor mainWindow( &argc, argv, gClient->GetRoot(), 1200,800);

   theApp.Run();


   return 0;
}

long EMMonitor::get_time()
{
  long time_ms;
  struct timeval t1;
  struct timezone tz;
  gettimeofday(&t1, &tz);
  time_ms = (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
  return time_ms;
};
