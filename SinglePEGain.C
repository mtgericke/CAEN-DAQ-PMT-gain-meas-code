Double_t SinglePESpectrum(Double_t *x, Double_t *par);
Double_t SpeFunction(Double_t *x, Double_t *par);
Double_t GaussZGaussN(Double_t *x, Double_t *par, Int_t n);
Double_t GaussBackground(Double_t *x, Double_t *par, Int_t n);
void DoFit(TH1D *hst, Int_t Ped, Int_t Pedw, Int_t Spe, Int_t Spew);

Int_t cPE = 4;
Int_t events;

Double_t GainResults[3];
ofstream oFile;


void SinglePEGain(const char* file, Int_t app, double_t HV, Int_t SerialN, Int_t nPE, Int_t Ped = 0, Int_t Pedw = 0, Int_t Spe = 0, Int_t Spew = 0)
{
  cPE = nPE;
  
  ifstream *iFile = new ifstream(file);
  
  Double_t binContent;
  TH1D *hst = new TH1D("hst","",4096,0,4096);
  
  if(iFile){
    if(iFile->is_open()){
      
      *iFile >> events;
      
      for(int b = 1; b <= 4096; b++){
	
	for(int c = 0; c < 32; c++){
	  
	  *iFile >> binContent;
	  if(c == 2){
	    hst->SetBinContent(b,binContent);
	  }
	}
      }
      
      iFile->close();
    }    
  }

  hst->Scale(1.0/hst->Integral());
  hst->Draw();
  DoFit(hst,Ped,Pedw,Spe,Spew);
  
  TString fname(Form("PMTGainResults_PMT_%d.txt",SerialN));
  
  if(app){
    oFile.open(fname.Data(),std::ios_base::app);
  }
  // else{
  //   oFile.open(fname.Data());
  // }
  
  if(oFile.is_open()){
    if(!app){
      oFile << "PMT" << " " << "HV [V]" << " " << "Gain [e6]" << " " << "Error [e6]" << '\n';
    }
      
    oFile << SerialN << " " << HV <<  " " << GainResults[1] <<  " " << GainResults[2] << '\n';
    oFile.close();
    
  }
  
}

void DoFit(TH1D *hst, Int_t ped, Int_t pedw, Int_t spe, Int_t spew)
{
  //fit parameters used in fp[] below:
  //par 0 = mu  Poisson distribution mean for SPE 
  //par 1 = w   Background event fracation
  //par 2 = q0  Mean pedestal peak in bins
  //par 3 = q1  Mean single PE peak location in bins beyond the pedestal 
  //par 4 = sig0  Pedestal standard deviation
  //par 5 = sig1  Single PE peak standard deviation
  //par 6 = alpha  Background level fall-off constant 
  //par 7 = events   Number of data  events in the histogram  
  
  int npeaks = 2;
  Double_t peaks_m[npeaks];
  Double_t peaks_h[npeaks];
  TSpectrum *s = new TSpectrum(2*npeaks);
  Int_t nfound = s->Search(hst,20,"",0.05);
  if(nfound == 0){
    cout << "No peaks to fit to have been found. Returning ..." << endl;
    return;
  }
  peaks_m[0] =  s->GetPositionX()[0];
  peaks_h[0] =  s->GetPositionY()[0];
  
  cout << "Pedestal peak located at " << peaks_m[0] << " height = " << peaks_h[0] << endl;  
  //Int_t nfound2 = s->Search(hst,50,"",0.05);
  if(nfound > 1){
    peaks_m[1] =  s->GetPositionX()[1];
    peaks_h[1] =  s->GetPositionY()[1];
    cout << "Event peak located at " << peaks_m[1] << " height = " << peaks_h[1] << endl;
  }
  else{
    peaks_m[1] =  peaks_m[0] + 50;  //set the single PE peak marginally higher than the pedestal peak 
    peaks_h[1] =  peaks_h[0];
  }
  if(ped) peaks_m[0] = ped;
  if(spe) peaks_m[1] = spe;
  
  gPad->Update();

  Double_t fr[2];
  fr[0] = peaks_m[0] - 150;
  fr[1] = 4096;
  Double_t fp[8];// = {1,0.381,574,234,30,10,0.065,40000};
  fp[0] = 1;
  fp[1] = 0.1;  
  fp[2] = peaks_m[0];
  fp[3] = peaks_m[1]-peaks_m[0];
  fp[4] = 10;
  if(pedw) fp[4] = pedw;
  fp[5] = 50;
  if(spew) fp[5] = spew;
  fp[6] = 0.01;
  fp[7] = 1;
  
  TF1 *func = new TF1("func",SinglePESpectrum,fr[0],fr[1],8);
  func->SetNpx(5000);
  func->SetParameters(fp);
  func->SetParNames("#mu","w","Q0","Q1","sigma 0","sigma 1","alpha","ampl");
  func->SetParLimits(0,0,1);
  func->SetParLimits(1,0,1);
  func->SetParLimits(2,fp[2]-50,fp[2]+50);
  func->SetParLimits(3,fp[3]-50,fp[3]+50);
  func->SetParLimits(4,1,fp[4]+50);
  func->SetParLimits(5,1,fp[5]+50);
  func->SetParLimits(6,0,100);
  func->SetParLimits(7,0,1);

  hst->Fit("func","RB");

  Double_t fitres[8];
  func->GetParameters(fitres);    // obtain fit parameters
  //func->SetParameters(fitres);
  //hst->Fit("func","RB");


  GainResults[1] =  fitres[3]/1.6/4/5;                   // Nbins * (25e-15 C/bin) / eCharge / preamp gain (= ~5)       
  GainResults[2] =  func->GetParError(3)/1.6/4/5;

  
  Double_t pepar[9];
  for(int k = 0; k < 8; k++) pepar[k] = fitres[k];
  TF1 *pefunc;

  for(int k = 0; k < cPE; k++){
    pepar[8] = k;
    pefunc = new TF1(Form("pefunc%d",k),SpeFunction,fr[0],fr[1],9);
    pefunc->SetParameters(pepar);
    pefunc->SetLineStyle(2);
    pefunc->Draw("same");
  }

  //= new TF1("func",SpeFunction,fr[0],fr[1],9);

  
  cout << "Gain = " << fitres[3]/1.6/4/5 << "e6 " << endl;    // Nbins * (25e-15 C/bin) / eCharge / preamp gain (= ~5) 
  
   // for (i=0; i<4; i++) {
   //    fiterrors[i] = ffit->GetParError(i);     // obtain fit parameter errors
   // }
   // ChiSqr[0] = ffit->GetChisquare();  // obtain chi^2
   // NDF[0] = ffit->GetNDF();           // obtain ndf
 

}

void TestPlot()
{

  //  Double_t fp[8] = {0.718,0.381,313.51,24,5.29,10.42,0.065,4};
  //Double_t fp[8] = {1,0.381,320,50,6,15,0.065,4};

  //TF1 *func = new TF1("func",SinglePESpectrum,308,4906,8);
  //func->SetNpx(5000);
  //func->SetParameters(fp);

  //func->Draw();

  TF1 *func2 = new TF1("func2",SpeFunction,308,4906,8);
  Double_t fp2[9] = {1,0.1,574,234,50,100,0.065,2*20990};
  func2->SetNpx(5000);
  func2->SetParameters(fp2);

  
  func2->Draw("same");
}

Double_t SinglePESpectrum(Double_t *x, Double_t *par)
{

  //par 0 = mu  Poisson distribution mean for SPE 
  //par 1 = w   Background event fracation
  //par 2 = q0  Mean pedestal peak in bins
  //par 3 = q1  Mean single PE peak location in bins beyond the pedestal 
  //par 4 = sig0  Pedestal standard deviation
  //par 5 = sig1  Single PE peak standard deviation
  //par 6 = alpha  Background level fall-off constant 
  //par 7 = events   Number of data  events in the histogram  

  Double_t F = 0;
  Double_t amp  = 1;//par[7];
  
  for(int n = 0; n < cPE; n++){

    //if(n) amp = 1; 

    F += amp*((1-par[1])*GaussZGaussN(x,par,n) + par[1]*GaussBackground(x,par,n))*TMath::Power(par[0],n)*TMath::Exp(-par[0])/TMath::Factorial(n);

  }
  return F;
}

Double_t SpeFunction(Double_t *x, Double_t *par)
{
  int n = par[8];
  
  return ((1-par[1])*GaussZGaussN(x,par,n) + par[1]*GaussBackground(x,par,n))*TMath::Power(par[0],n)*TMath::Exp(-par[0])/TMath::Factorial(n);
  //return par[7]*((1-par[1])*GaussZGaussN(x,par,n))*TMath::Power(par[0],n)*TMath::Exp(-par[0])/TMath::Factorial(n);

}

Double_t GaussZGaussN(Double_t *x, Double_t *par, Int_t n)
{

  Double_t sig_n = TMath::Sqrt(par[4]*par[4] + n*par[5]*par[5]);
  Double_t q_n = par[2] + n*par[3];
  
  Double_t F = TMath::Exp(-(x[0]-q_n)*(x[0]-q_n)/(2*sig_n*sig_n))/(sig_n*TMath::Sqrt(2*TMath::Pi()));

  return F;
}

Double_t GaussBackground(Double_t *x, Double_t *par, Int_t n)
{

  Double_t sig_n = TMath::Sqrt(par[4]*par[4] + n*par[5]*par[5]);
  Double_t q_n = par[2] + n*par[3];
  Double_t erfn = TMath::Erf(TMath::Abs(par[2]-q_n-sig_n*sig_n*par[6])/(sig_n*TMath::Sqrt(2)));
  Double_t erfx = TMath::Erf(TMath::Abs(x[0]-q_n-sig_n*sig_n*par[6])/(sig_n*TMath::Sqrt(2)));
  Double_t sgn = TMath::Sign(1,x[0]-q_n-sig_n*sig_n*par[6]);
  Double_t F = par[6]*TMath::Exp(-par[6]*(x[0]-q_n-par[6]*sig_n*sig_n))*(erfn + sgn*erfx)/2;

  return F;

}

