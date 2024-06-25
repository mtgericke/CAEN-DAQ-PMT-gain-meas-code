void PlotGainCurve(const char* file)
{

   ifstream *iFile = new ifstream(file);

   string tmp;
   Double_t  PMT;
   Double_t  HV;
   Double_t  Gain;
   Double_t  Error;
   TGraphErrors *gr = new TGraphErrors();
   Int_t p = 0;

   if(iFile){
     if(iFile->is_open()){
       for(int n = 0; n < 7; n++){
	 *iFile >> tmp;
	 cout << tmp << endl;
       }
       while(1){
	 if(!(*iFile >> PMT)) break;
	 *iFile >> HV;
	 *iFile >> Gain;
	 *iFile >> Error;
	 cout << HV << ", " << Gain << ", " << Error << endl; 
	 gr->SetPoint(p,HV,Gain);
	 gr->SetPointError(p,Error,0);
	 p++;
       }
       iFile->close();
     }
   }
   gr->SetMarkerStyle(8);
   gr->SetMarkerSize(1);
   gr->GetXaxis()->SetTitle("HV");
   gr->GetYaxis()->SetTitle("Gain/10^{6}");
   gr->Draw("ap");
   
}
