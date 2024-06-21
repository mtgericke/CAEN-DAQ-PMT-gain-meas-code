////////////////////////////////////////////////////////////////////////////////
//
// name: EMMonitor.h
// date: 5-24-2021
// auth: Michael Gericke
// mail: Michael.Gericke@umanitoba.ca
//
// desc: This is a simple application that monitors and handles data from the
//       MOLLER ADC in pre-production diagnostic mode (meaning no special acqusition
//       software, such as the JLab CODA system has been implemented). The data is
//       sent by the ADC over ethernet and received by this application, utilizing the
//       the ZeroMQ library to handle the network traffic.
//
//       The application needs the ip address of the ADC board and the port (which is
//       fixed at 5555 on the ADC).
//
//       The application make suse of the ROOT analysis framework to display the data
//       and the RData library to provide a basic interface.
//
////////////////////////////////////////////////////////////////////////////////



#ifndef CMMONITOR_H
#define CMMONITOR_H

#include <TROOT.h>
#include <TApplication.h>
#include <TGMenu.h>
#include <TGClient.h>
#include <TStyle.h>
#include <RDataContainer.h>
#include <RVegas.h>
#include <RDataWindow.h>
#include <RDataFit.h>
#include <RProcessLog.h>
#include <TString.h>
#include <TText.h>
#include <TH1D.h>
#include <TF1.h>
#include <TGraph.h>
#include <TRootEmbeddedCanvas.h>
#include <TGTab.h>
#include <TG3DLine.h>
#include <TGTextEntry.h>
#include <TGTextEdit.h>
#include <TGFileDialog.h>
#include <TPaveText.h>
#include <string.h>
#include <sys/time.h> /* struct timeval, select() */
// #include <time.h>
#include <iostream>
// #include <zmq.h>
#include <fstream>
#include <thread>
#include <signal.h>
#include <queue>
#include "EMMonitorDef.h"

#define LINUX 1  //needed by CAENVMEtypes.h

#include <CAENVMElib.h>
#include <CAENVMEtypes.h>

// #include "Console.h"

#define Sleep(x) usleep((x)*1000)

using namespace std;


class EMMonitor : public TGMainFrame {

  RQ_OBJECT("EMMonitor");

private:

  int                     dMWWidth;
  int                     dMWHeight;

  TGMenuBar              *dMenuBar;
  TGPopupMenu            *dMenuFile;
  TGPopupMenu            *dMenuRunCntrl;
  TGPopupMenu            *dMenuConfig;
  TGPopupMenu            *dMenuTools;
  TGPopupMenu            *dMenuHelp;
		        
  // TRootEmbeddedCanvas    *dCurrMSmplGrCv[NUM_CHANNELS];
  TRootEmbeddedCanvas    *dCurrMSmplHstCv[NUM_CHANNELS];
  // TRootEmbeddedCanvas    *dCurrMSmplFFTCv[NUM_CHANNELS];
  
  TGVerticalFrame        *vLabF[NUM_CHANNELS];
  TGLabel                *dRateCounter[NUM_CHANNELS];
  TGHorizontalFrame      *dRateFrame[NUM_CHANNELS];


  RProcessLog            *processLog;
  RDataWindow            *dataWindow;
  RDataContainer         *dataCont, *dataCont2;

  uint32_t BaseAddress;
  uint32_t QTPBaseAddr;
  uint32_t DiscrBaseAddr;  
  // handle for the V1718/V2718 
  int32_t handle; 
  
  int VMEerror;
  char ErrorString[100];
  FILE *logfile;

  int DataType;
  int EnableHistoFiles;		// Enable periodic saving of histograms (once every second)
  int EnableListFile;	        // Enable saving of list file (sequence of events)
  int EnableRawDataFile;	// Enable saving of raw data (memory dump)
  int EnableSuppression;	// Enable Zero and Overflow suppression if QTP boards
  int BoardNChannels;           // Number of channels the board has
  int QDCChannel;               // QDC Channel
  int NSamples[32];             // Number of collected samples for each channel
  uint16_t DiscrChMask;		// Channel enable mask of the discriminator
  uint16_t DiscrOutputWidth;	// Output width of the discriminator
  uint16_t DiscrThreshold[16];
  uint16_t QTP_LLD[32];
  uint16_t Iped;
 
  
  TH1D                   *ChSigHst[NUM_CHANNELS];

  Bool_t                  RUN_START;
  Bool_t                  RUN_STOP;
  // Bool_t                  RUN_PAUSE;
  // Bool_t                  RUN_ON;


  uint16_t                ReadRegister(uint16_t reg_addr);
  void                    WriteRegister(uint16_t reg_addr, uint16_t data);
  int                     ConfigureDiscr(uint16_t OutputWidth, uint16_t Threshold[16], uint16_t EnableMask);
  void                    FindModelVersion(uint16_t model, uint16_t vers, char *modelVersion, int *ch);
  Int_t                   ConfigureVMEModule();

  Int_t                   GetFilenameFromDialog(char *file, const char *ext,
						ERFileStatus status, Bool_t kNotify = 0,
						const char *notifytext = 0);

  Int_t                   OpenConfigFile(const char* file = 0);
  Int_t                   OpenOutputFiles();
  Int_t                   WriteDataTxt(const char* file = 0);



  void                    MakeCountingModeTab();
  void                    MakeMenuLayout();
  void                    GetData();
  void                    ClearDataBuffers();
  // void                    FillDataPlots();
  void                    StartDataCollection();
  virtual Bool_t          ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  long                    get_time();
  

public:
  EMMonitor(int *argc, char *argv[], const TGWindow *p, UInt_t w, UInt_t h);
  virtual ~EMMonitor();
  
  virtual void CloseWindow();

  void OnObjClose(char *obj);
  void OnReceiveMessage(char *obj){};
  void PadIsPicked(TPad* selpad, TObject* selected, Int_t event);
  void MainTabEvent(Int_t,Int_t,Int_t,TObject*);

  //std::thread spawn() {
  //  return std::thread(&EMMonitor::GetServerData,this);
  //};
  
  ClassDef(EMMonitor,0);
};


#endif
