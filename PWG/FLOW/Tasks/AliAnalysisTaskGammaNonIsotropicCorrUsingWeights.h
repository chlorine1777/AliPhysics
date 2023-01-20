/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice */
/* $Id: $ */
///////////////////////////////////////////////////////////
// Task To Analyse Gamma Delta Correlator with ESE. 
// Works with 15o and 18q/r. Support to be added for LHC10h 
// PA: Rihan Haque (mhaque@cern.ch, rihanphys@gmail.com)
///////////////////////////////////////////////////////////




#ifndef ALIANALYSISTASKGAMMANONISOTROPICCORRUSINGWEIGHTS_H
#define ALIANALYSISTASKGAMMANONISOTROPICCORRUSINGWEIGHTS_H


#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TList.h"
#include "TFile.h"
#include "TString.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "AliAnalysisTaskSE.h"
#include <iostream>
#include <vector>

class    AliAnalysisTaskGammaNonIsotropicCorrUsingWeights;
class    AliVEvent;      
class    AliVVertex;
class    AliAODv0;
class    AliESDEvent;       
class    AliAODEvent;
class    AliAODTrack;   
class    AliPIDResponse;    
class    AliMultSelection;    
class    AliAnalysisUtils;



class AliAnalysisTaskGammaNonIsotropicCorrUsingWeights : public AliAnalysisTaskSE {

 public:

  //-----> Mandatory Functions:
  AliAnalysisTaskGammaNonIsotropicCorrUsingWeights();
  AliAnalysisTaskGammaNonIsotropicCorrUsingWeights(const char *name);
  virtual ~AliAnalysisTaskGammaNonIsotropicCorrUsingWeights();
  virtual void UserCreateOutputObjects();
  virtual void UserExec(Option_t * /*option*/);
  virtual void Terminate(Option_t *);
  
  //-----> Simple User Defined Functions:
  void  SetListForV0MCorr(TList *flist)      {this->fListV0MCorr = (TList *) flist->Clone(); }
  void  SetListForZDCCorr(TList *flist)      {this->fListZDCCorr = (TList *) flist->Clone(); }
  void  SetListForMCEfficiencyWeightForTrack(TList *flist)      {this->fListMCEfficiencyWeightForTrack = (TList *) flist->Clone(); }

  void  Calculate2pCorrelator();
  void  CalculateNonIsotropicTerms();
  void  CalculateDifferential3pCorrelator();
  //// Event Cuts or Ranges ////
  //void SetPileUpCutParam(Float_t m,Float_t c)  {this->fPileUpSlopeParm = m; this->fPileUpConstParm = c;}
  //void SetCentralityPercentileMin(Double_t cMin) {this->fCentralityMin  = cMin;}
  //void SetCentralityPercentileMax(Double_t cMax) {this->fCentralityMax  = cMax;}
  void SetDetectorforEventPlane(TString sdetEP)  {this->sDetectorForEP  = sdetEP;}
  void SetCentralityEstimator(TString sEstim)    {this->sCentrEstimator = sEstim;}
  void SetFlagSkipPileUpCuts(Bool_t bP)          {this->bSkipPileUpCut     =  bP;}
  void SetVzRangeMin(Double_t vzMin)             {this->fMinVzCut       =  vzMin;}
  void SetVzRangeMax(Double_t vzMax)             {this->fMaxVzCut       =  vzMax;}
  void SetwhichData(Double_t wD)                 {this->whichData       =  wD;}
  void Setperiod(TString p)                 {this->period       =  p;}

  void ResetEventByEventQuantities();
  
  /// Track Cut Ranges ///
  void SetTrackCutNclusterMin(Int_t nclTPC)   {this->fTPCclustMin   = nclTPC;}
  void SetTrackCutTPCsharedMin(Int_t nTPCs)   {this->fTPCsharedCut   = nTPCs;}
  void SetTrackCutUseTPCCrossedRows(Bool_t bTPCcr)   {this->bUseTPCCrossedRows   = bTPCcr;}
  void SetFlagUseKinkTracks(Bool_t bKink)     {this->bUseKinkTracks = bKink;}
  void SetCumulantHarmonic(Int_t harm)        {this->gHarmonic      = harm;}
  void SetTrackCutdEdxMin(Float_t mindedx)    {this->fTPCdEdxMin    = mindedx;}
  void SetTrackCutChi2Min(Double_t chi2Min)   {this->fTrkChi2Min    = chi2Min;}
  void SetTrackCutChi2Max(Double_t chi2Max)   {this->fTrkChi2Max    = chi2Max;}
  void SetNSigmaCutTPC(Double_t nSigTPC)      {this->fNSigmaTPCCut  = nSigTPC;}
  void SetNSigmaCutTOF(Double_t nSigTOF)      {this->fNSigmaTOFCut  = nSigTOF;}
  void SetDCAXYRangeMax(Double_t dcaxy)       {this->fDCAxyMax      = dcaxy;}
  void SetDCAZRangeMax(Double_t dcaz)         {this->fDCAzzMax      = dcaz;}
  void SetEtaRangeMin(Double_t etamin)        {this->fMinEtaCut     = etamin;}
  void SetEtaRangeMax(Double_t etamax)        {this->fMaxEtaCut     = etamax;}
  void SetPtRangeMin(Double_t ptLow)          {this->fMinPtCut      = ptLow;}
  void SetPtRangeMax(Double_t ptHigh)         {this->fMaxPtCut      = ptHigh;}
  void SetParticlePID(Int_t partID)           {this->gParticleID    = partID;}
  void SetTrackChi2Max(Double_t chi2)         {this->fTrkChi2Max    = chi2;}
  void SetFilterBit(Int_t fb)                 {this->fFilterBit     = fb;}
  void SetEtaGapNeg(Double_t etaL)            {this->fEtaGapNeg     = etaL;}
  void SetEtaGapPos(Double_t etaH)            {this->fEtaGapPos     = etaH;}


  
  //------ End of user defined function -------
 protected:

 private:
  Int_t whichData;
  TString period;

  AliVEvent             *fVevent;             //! event
  AliESDEvent           *fESD;                //! esd Event
  AliAODEvent           *fAOD;                //! aod Event
  AliPIDResponse        *fPIDResponse;        //! PID Handler
  AliMultSelection      *fMultSelection;      //! For Centrality 
  AliAnalysisUtils      *fAnalysisUtil;       //! Event Selection Options
  TList                 *fListHist;           //! OutputList
  TList                 *fListV0MCorr;        //  Supplied from AddTask  
  TList                 *fListV0MCorrRunByRun;//  Supplied from AddTask  
  TList                 *fListZDCCorr;        //  Supplied from AddTask
  TList                 *fListZDCCorrRunByRun;//  Supplied from AddTask 
  TList                 *fListMCEfficiencyWeightForTrack;      //  Supplied from AddTask
  TList                 *fTempList;           //! Holding Temperary Histograms
  TTree                 *treeEvent;           //!
  
  /// Functions for Pile Up Event Removal:
  TF1                   *fV0CutPU;      //!
  TF1                   *fSPDCutPU;     //!
  TF1                   *fMultCutPU;    //!
  TF1                   *fCenCutLowPU;  //!
  TF1                   *fCenCutHighPU; //!

  // variables for Qvec
  const static Int_t fCRCnHar = 2;
  const static Int_t fCMEnEtaBin = 4;
  Double_t fCRCEtaMin = -0.8;
  Double_t fCRCEtaMax = 0.8;
  
  // 3.) Event-by-event quantities:
  Float_t centrality;
  Double_t fPsi2V0C = 0.;
  Double_t fPsi2V0A = 0.;
  Double_t fPsiZDCC = 0.;
  Double_t fPsiZDCA = 0.;
  Double_t fPsiZDCCA = 0; 
  TProfile *fCMEQReRP;        //! real part cos(n psi_RP), n from 1 to 2, RP is same as POI OS
  TProfile *fCMEQImRP;        //! imaginary part sin(n psi_RP)
  TProfile *f2pCorrelatorCos2PsiDiff2PsiV0RPPerEvent;        //! cos(2PsiRP-2PsiV0) for v2
  TProfile *f2pCorrelatorCos2PsiDiff2PsiZDCRPPerEvent;        //! cos(2PsiRP-2PsiZDC) for v2
  TProfile *f2pCorrelatorCosPsiDiffPerEvent;        //! cos(dPsi1-dPsi2)
  TProfile *f2pCorrelatorCos2PsiDiffPerEvent;        //! cos(2(dPsi1-dPsi2)) for v2 TPC
  TProfile *fCMEQRePOIPos;        //! real part cos(n psi_POIPos)
  TProfile *fCMEQImPOIPos;        //! imaginary part sin(n psi_POIPos)
  TProfile *fCMEQRePOINeg;        //! real part cos(n psi_POINeg)
  TProfile *fCMEQImPOINeg;        //! imaginary part sin(n psi_POINeg)
  TProfile *fNITV0OS;        //! non isotropic terms for V0 and RP
  TProfile *fNITZDCOS;        //! non isotropic terms for ZDC and RP
  TProfile *fNITV0POIPos;        //! non isotropic terms for V0 and POIPos
  TProfile *fNITZDCPOIPos;        //! non isotropic terms for ZDC and POIPos
  TProfile *fNITV0POINeg;        //! non isotropic terms for V0 and POINeg
  TProfile *fNITZDCPOINeg;        //! non isotropic terms for ZDC and POINeg

  
  TProfile *fRePEBEOS;        //! Cos(dPsi1+dPsi2) POIOS
  TProfile *fImPEBEOS;        //! Sin(dPsi1+dPsi2) POIOS
  TProfile *f2pCorrelatorCosPsiDiffOS;        //! cos(dPsi1-dPsi2) POIOS for overlap (only correct for wgt = 1)
  TProfile *f2pCorrelatorCos2PsiDiffOS;        //! cos(2(dPsi1-dPsi2)) POIOS for overlap (only correct for wgt = 1)
  TProfile *fOverlapEBEOSPOI1;                 //! w1^3w2cos(dPsi1-dPsi2) for POI1 OS overlap term for wgt != 1
  TProfile *fOverlapEBEOSPOI2;                 //! w1w2^3cos(dPsi1-dPsi2) for POI2 OS overlap term for wgt != 1
  TProfile *fRePEBEPP;        //! Cos(dPsi1+dPsi2) POIPP
  TProfile *fImPEBEPP;        //! Sin(dPsi1+dPsi2) POIPP
  TProfile *f2pCorrelatorCosPsiDiffPP;        //! cos(dPsi1-dPsi2) POIPP for overlap (only correct for wgt = 1)
  TProfile *f2pCorrelatorCos2PsiDiffPP;        //! cos(2(dPsi1-dPsi2)) POIPP for overlap (only correct for wgt = 1)
  TProfile *fOverlapEBEPPPOI1;                 //! w1^3w2cos(dPsi1-dPsi2) for POI1 PP overlap term for wgt != 1
  TProfile *fOverlapEBEPPPOI2;                 //! w1w2^3cos(dPsi1-dPsi2) for POI2 PP overlap term for wgt != 1
  TProfile *fRePEBENN;        //! Cos(dPsi1+dPsi2) POINN
  TProfile *fImPEBENN;        //! Sin(dPsi1+dPsi2) POINN
  TProfile *f2pCorrelatorCosPsiDiffNN;        //! cos(dPsi1-dPsi2) POINN for overlap (only correct for wgt = 1)
  TProfile *f2pCorrelatorCos2PsiDiffNN;        //! cos(2(dPsi1-dPsi2)) POINN for overlap (only correct for wgt = 1)
  TProfile *fOverlapEBENNPOI1;                 //! w1^3w2cos(dPsi1-dPsi2) for POI1 NN overlap term for wgt != 1
  TProfile *fOverlapEBENNPOI2;                 //! w1w2^3cos(dPsi1-dPsi2) for POI2 NN overlap term for wgt != 1
  
  // QA hist for ZDC-C and ZDC-A tower energy
  //const static Int_t nBinQAZDCAvgTowEnergyFraction = 18;
  //TProfile *fZDCCAvgTow1EnergyFraction[nBinQAZDCAvgTowEnergyFraction]; //0-90 centrality
  //TProfile *fZDCCAvgTow2EnergyFraction[nBinQAZDCAvgTowEnergyFraction]; //0-90 centrality
  //TProfile *fZDCCAvgTow3EnergyFraction[nBinQAZDCAvgTowEnergyFraction]; //0-90 centrality
  //TProfile *fZDCCAvgTow4EnergyFraction[nBinQAZDCAvgTowEnergyFraction]; //0-90 centrality
  //TProfile *fZDCAAvgTow1EnergyFraction[nBinQAZDCAvgTowEnergyFraction]; //0-90 centrality
  //TProfile *fZDCAAvgTow2EnergyFraction[nBinQAZDCAvgTowEnergyFraction]; //0-90 centrality
  //TProfile *fZDCAAvgTow3EnergyFraction[nBinQAZDCAvgTowEnergyFraction]; //0-90 centrality
  //TProfile *fZDCAAvgTow4EnergyFraction[nBinQAZDCAvgTowEnergyFraction]; //0-90 centrality
  

  
  
  //Track Variables to be used:
  Int_t                 gHarmonic;  //
  Int_t               gParticleID;  //
  Int_t                fFilterBit;  //
  Int_t              fTPCclustMin;  //
  Int_t             gOldRunNumber;  //!
  Int_t             fTPCsharedCut;
  Bool_t            bUseTPCCrossedRows;
  
  Float_t               fMinVzCut;  //
  Float_t               fMaxVzCut;  //
  Float_t               fMinPtCut;  //
  Float_t               fMaxPtCut;  //
  Float_t               fDCAxyMax;  //
  Float_t               fDCAzzMax;  //   
  Float_t              fMinEtaCut;  //
  Float_t              fMaxEtaCut;  //
  Float_t              fEtaGapNeg;  // 
  Float_t              fEtaGapPos;  //
  Float_t             fTrkChi2Min;  //
  Float_t             fTrkChi2Max;  //
  Float_t             fTPCdEdxMin;  //
  Float_t           fNSigmaTPCCut;  // 
  Float_t           fNSigmaTOFCut;  //
  Float_t           fVertexZEvent;  //!

  TString          sDetectorForEP;  //
  TString         sCentrEstimator;  //
  
  Bool_t           bUseKinkTracks;  //
  Bool_t           bSkipPileUpCut;  //   
  Bool_t           bUseMCEfficiencyWeight; //

  


  //// Histograms:
  TH1F          *fHistVertexZcm;     //! See Vz Distribution
  TH1F          *fHistAnalysisInfo;  //! information about cuts
  TH1F          *fCentDistBeforCut;  //! Cent before Any Cut
  TH1F          *fCentDistAfterCut;  //! Cent After All Cuts
  TH1F          *fDebugwEventCount;  //! Event after Various cuts

  /// Correction Histograms:
  TH1D         *fHCorrectQNxV0C; //!
  TH1D         *fHCorrectQNyV0C; //!
  TH1D         *fHCorrectQNxV0A; //!
  TH1D         *fHCorrectQNyV0A; //!
  TH1D         *fHCorrectQ3xV0C; //!
  TH1D         *fHCorrectQ3yV0C; //!
  TH1D         *fHCorrectQ3xV0A; //!
  TH1D         *fHCorrectQ3yV0A; //!      
  TH1D         *fHCorrectMCPosChrg; //!
  TH1D         *fHCorrectMCPosPion; //!
  TH1D         *fHCorrectMCPosKaon; //!
  TH1D         *fHCorrectMCPosProt; //!   
  TH1D         *fHCorrectMCNegChrg; //!
  TH1D         *fHCorrectMCNegPion; //!
  TH1D         *fHCorrectMCNegKaon; //!
  TH1D         *fHCorrectMCNegProt; //! 

  
  //TH2F          *fHistTPCPsiNPosPlane;   //!
  //TH2F          *fHistTPCPsiNNegPlane;   //!
  //TH2F          *fHistTPCPsi3PosPlane;   //!
  //TH2F          *fHistTPCPsi3NegPlane;   //!
  //TH2F          *fHistTPCPsi4PosPlane;   //!
  //TH2F          *fHistTPCPsi4NegPlane;   //!  
  TH2F          *fHistV0CPsiNEventPlane; //!  
  TH2F          *fHistV0APsiNEventPlane; //!
  //TH2F          *fHistTPCPosqVectorvsCent; //!!
  //TH2F          *fHistTPCNegqVectorvsCent; //!!  
  TH2F          *fHistV0CDetqVectorvsCent; //!!
  TH2F          *fHistV0ADetqVectorvsCent; //!!

  TH2F          *fHCorrectV0ChWeghts;     //!

  TH2F          *fHistTPConlyVsCL1Before; //!
  TH2F          *fHistTPConlyVsCL1After;  //!
  TH2F          *fHistTPConlyVsV0MBefore; //!
  TH2F          *fHistTPConlyVsV0MAfter;  //!
  TH2F          *fHistCentCL0VsV0MBefore; //!
  TH2F          *fHistCentCL0VsV0MAfter;  //!  
  TH2F          *fHistTPCVsESDTrkBefore;  //!
  TH2F          *fHistTPCVsESDTrkAfter;   //!

  // 2-p correlator
  Int_t fCMESPPPCenBin;
  TProfile *f2pCorrelatorCos2PsiDiff2PsiV0CRP;        //! cos(2PsiRP-2PsiV0C) for v2
  TProfile *f2pCorrelatorCos2PsiDiff2PsiV0ARP;        //! cos(2PsiRP-2PsiV0A) for v2
  TProfile *f2pCorrelatorCos2PsiDiff2PsiZDCCRP;        //! cos(2PsiRP-2PsiZDC) for v2
  TProfile *f2pCorrelatorCos2PsiDiff2PsiZDCARP;        //! cos(2PsiRP-2PsiZDC) for v2
  TProfile *f2pCorrelatorCos2PsiDiff2PsiZDCCARP;        //! cos(2PsiRP-2PsiZDC) for v2
  
  TProfile *f2pCorrelatorCosPsiDiff;        //! cos(dPsi1-dPsi2)
  TProfile *f2pCorrelatorCos2PsiDiff;        //! cos(2(dPsi1-dPsi2)) for v2 TPC
  
  TProfile *f2pCorrelatorCosPsiDiffEBE; //!
  TProfile *f2pCorrelatorCos2PsiDiffEBE; //!
  TProfile *f2pCorrelatorCos2PsiDiff2PsiV0CRPEBE; //!
  TProfile *f2pCorrelatorCos2PsiDiff2PsiV0ARPEBE; //!
  TProfile *f2pCorrelatorCos2PsiDiff2PsiZDCCRPEBE; //!
  TProfile *f2pCorrelatorCos2PsiDiff2PsiZDCARPEBE; //!
  TProfile *f2pCorrelatorCos2PsiDiff2PsiZDCCARPEBE; //!
	
  // 3-p correlator
  TProfile *f3pCorrelatorTPCOSPro; //! cos[n(psi1+psi2-2phi3)] OS
  TProfile *f3pCorrelatorV0COSPro; //!
  TProfile *f3pCorrelatorV0AOSPro; //!
  TProfile *f3pCorrelatorZDCCOSPro; //!
  TProfile *f3pCorrelatorZDCAOSPro; //!
  TProfile *f3pCorrelatorZDCCAOSPro; //!
    
  TProfile *f3pCorrelatorTPCPPPro; //! cos[n(psi1+psi2-2phi3)] PP
  TProfile *f3pCorrelatorV0CPPPro; //!
  TProfile *f3pCorrelatorV0APPPro; //!
  TProfile *f3pCorrelatorZDCCPPPro; //!
  TProfile *f3pCorrelatorZDCAPPPro; //!
  TProfile *f3pCorrelatorZDCCAPPPro; //!
    
  TProfile *f3pCorrelatorTPCNNPro; //! cos[n(psi1+psi2-2phi3)] NN
  TProfile *f3pCorrelatorV0CNNPro; //!
  TProfile *f3pCorrelatorV0ANNPro; //!
  TProfile *f3pCorrelatorZDCCNNPro; //!
  TProfile *f3pCorrelatorZDCANNPro; //!
  TProfile *f3pCorrelatorZDCCANNPro; //!
    
  TProfile *f3pCorrelatorTPCSSPro; //! cos[n(psi1+psi2-2phi3)] SS
  TProfile *f3pCorrelatorV0CSSPro; //!
  TProfile *f3pCorrelatorV0ASSPro; //!
  TProfile *f3pCorrelatorZDCCSSPro; //!
  TProfile *f3pCorrelatorZDCASSPro; //!
  TProfile *f3pCorrelatorZDCCASSPro; //!
    
  // non-isotropic terms 3-p 
  TProfile *fNonIsotropicTermsPro[10]; //! <<cos(n*(phi1))>>, <<sin(n*(phi1))>>, <<cos(2n*(phi1))>>, <<sin(2n*(phi1))>> 
                                       // <<cos(n*(phi1+phi2))>>, <<sin(n*(phi1+phi2))>>, <<cos(n*(2phi1-phi2))>>, <<sin(n*(2phi1-phi2))>>
                                       // <<cos(n*(phi1-phi2-phi3))>>, <<sin(n*(phi1-phi2-phi3))>> 
  TProfile *fNonIsotropicTermsV0Pro[8]; //! Cos(fPsi2V0C), Sin(fPsi2V0C), Cos(fPsi2V0A), Sin(fPsi2V0A)
                                        // Cos(2*fPsi2V0C), Sin(2*fPsi2V0C), Cos(2*fPsi2V0A), Sin(2*fPsi2V0A)
  TProfile *fNonIsotropicTermsZDCPro[12]; //!
  
  TProfile *fNonIsotropicTermsOSPro[10]; //! <<cos(#psi_{POI_1})>>, <<sin(#psi_{POI_1})>>, <<cos(#psi_{POI_2})>>, <<sin(#psi_{POI_2})>>
                                         // <<cos(#psi_{POI_1}-2*phi)>>, <<sin(#psi_{POI_1}-2*phi)>>
                                         // <<cos(#psi_{POI_2}-2*phi)>>, <<sin(#psi_{POI_2}-2*phi)>>
                                         // <<cos(#psi_{POI_1}+#psi_{POI_2})>>, <<sin(#psi_{POI_1}+#psi_{POI_2})>>
  TProfile *fNonIsotropicTermsV0OSPro[8]; //! <<cos(#psi_{POI_1}-2*phi_{V0C})>>, <<sin(#psi_{POI_1}-2*phi_{V0C})>>
                                          // <<cos(#psi_{POI_1}-2*phi_{V0A})>>, <<sin(#psi_{POI_1}-2*phi_{V0A})>>
                                          // <<cos(#psi_{POI_2}-2*phi_{V0C})>>, <<sin(#psi_{POI_2}-2*phi_{V0C})>>
                                          // <<cos(#psi_{POI_2}-2*phi_{V0A})>>, <<sin(#psi_{POI_2}-2*phi_{V0A})>>
  TProfile *fNonIsotropicTermsZDCOSPro[12]; //! <<cos(#psi_{POI_1}-2*phi_{ZDCC})>>, <<sin(#psi_{POI_1}-2*phi_{ZDCC})>>, ...
  
  TProfile *fNonIsotropicTermsPPPro[10]; //! <<cos(#psi_{POI_1})>>, <<sin(#psi_{POI_1})>>, <<cos(#psi_{POI_2})>>, <<sin(#psi_{POI_2})>>
                                         // <<cos(#psi_{POI_1}-2*phi)>>, <<sin(#psi_{POI_1}-2*phi)>>
                                         // <<cos(#psi_{POI_2}-2*phi)>>, <<sin(#psi_{POI_2}-2*phi)>>
                                         // <<cos(#psi_{POI_1}+#psi_{POI_2})>>, <<sin(#psi_{POI_1}+#psi_{POI_2})>>
  TProfile *fNonIsotropicTermsV0PPPro[8]; //! <<cos(#psi_{POI_1}-2*phi_{V0C})>>, <<sin(#psi_{POI_1}-2*phi_{V0C})>>
                                          // <<cos(#psi_{POI_1}-2*phi_{V0A})>>, <<sin(#psi_{POI_1}-2*phi_{V0A})>>
                                          // <<cos(#psi_{POI_2}-2*phi_{V0C})>>, <<sin(#psi_{POI_2}-2*phi_{V0C})>>
                                          // <<cos(#psi_{POI_2}-2*phi_{V0A})>>, <<sin(#psi_{POI_2}-2*phi_{V0A})>>
  TProfile *fNonIsotropicTermsZDCPPPro[12]; //! <<cos(#psi_{POI_1}-2*phi_{ZDCC})>>, <<sin(#psi_{POI_1}-2*phi_{ZDCC})>>, ...
  
  TProfile *fNonIsotropicTermsNNPro[10]; //! <<cos(#psi_{POI_1})>>, <<sin(#psi_{POI_1})>>, <<cos(#psi_{POI_2})>>, <<sin(#psi_{POI_2})>>
                                         // <<cos(#psi_{POI_1}-2*phi)>>, <<sin(#psi_{POI_1}-2*phi)>>
                                         // <<cos(#psi_{POI_2}-2*phi)>>, <<sin(#psi_{POI_2}-2*phi)>>
                                         // <<cos(#psi_{POI_1}+#psi_{POI_2})>>, <<sin(#psi_{POI_1}+#psi_{POI_2})>>
  TProfile *fNonIsotropicTermsV0NNPro[8]; //! <<cos(#psi_{POI_1}-2*phi_{V0C})>>, <<sin(#psi_{POI_1}-2*phi_{V0C})>>
                                          // <<cos(#psi_{POI_1}-2*phi_{V0A})>>, <<sin(#psi_{POI_1}-2*phi_{V0A})>>
                                          // <<cos(#psi_{POI_2}-2*phi_{V0C})>>, <<sin(#psi_{POI_2}-2*phi_{V0C})>>
                                          // <<cos(#psi_{POI_2}-2*phi_{V0A})>>, <<sin(#psi_{POI_2}-2*phi_{V0A})>>
  TProfile *fNonIsotropicTermsZDCNNPro[12]; //! <<cos(#psi_{POI_1}-2*phi_{ZDCC})>>, <<sin(#psi_{POI_1}-2*phi_{ZDCC})>>, ...
  
  TProfile *fNonIsotropicTermsSSPro[10]; //! <<cos(#psi_{POI_1})>>, <<sin(#psi_{POI_1})>>, <<cos(#psi_{POI_2})>>, <<sin(#psi_{POI_2})>>
                                         // <<cos(#psi_{POI_1}-2*phi)>>, <<sin(#psi_{POI_1}-2*phi)>>
                                         // <<cos(#psi_{POI_2}-2*phi)>>, <<sin(#psi_{POI_2}-2*phi)>>
                                         // <<cos(#psi_{POI_1}+#psi_{POI_2})>>, <<sin(#psi_{POI_1}+#psi_{POI_2})>>
  TProfile *fNonIsotropicTermsV0SSPro[8]; //! <<cos(#psi_{POI_1}-2*phi_{V0C})>>, <<sin(#psi_{POI_1}-2*phi_{V0C})>>
                                          // <<cos(#psi_{POI_1}-2*phi_{V0A})>>, <<sin(#psi_{POI_1}-2*phi_{V0A})>>
                                          // <<cos(#psi_{POI_2}-2*phi_{V0C})>>, <<sin(#psi_{POI_2}-2*phi_{V0C})>>
                                          // <<cos(#psi_{POI_2}-2*phi_{V0A})>>, <<sin(#psi_{POI_2}-2*phi_{V0A})>>
  TProfile *fNonIsotropicTermsZDCSSPro[12]; //! <<cos(#psi_{POI_1}-2*phi_{ZDCC})>>, <<sin(#psi_{POI_1}-2*phi_{ZDCC})>>, ...
  
  /// V0 recentering inputs
  TH1D          *fHV0Cparameters;          //!
  TH1D          *fHV0Aparameters;          //!
  
  /// ZDC recentering inputs
  TH1D          *fHZDCCparameters;          //!
  TH1D          *fHZDCAparameters;          //!
  
  /// TPC track efficiency input
  TH1D          *fHMCEfficiencyWeightForPosTrack[10];   //!
  TH1D          *fHMCEfficiencyWeightForNegTrack[10];   //!
  
  //// Store the EP <Q> vectors:
  TProfile      *hAvgQNXvsCentV0C;  //!
  TProfile      *hAvgQNYvsCentV0C;  //!
  TProfile      *hAvgQNXvsCentV0A;  //!
  TProfile      *hAvgQNYvsCentV0A;  //!

  TProfile     *hTPCPsiNCorrelation;  //!
  TProfile     *hTPCPsi3Correlation;  //!
  TProfile     *hTPCPsi4Correlation;  //!
  TProfile     *hV0CV0APsiNCorrelation;  //!
  //TProfile     *hV0CTPCPsiNCorrelation;  //!
  //TProfile     *hV0ATPCPsiNCorrelation;  //!
  TProfile     *hV0CV0APsi3Correlation;  //!
  //TProfile     *hV0CTPCPsi3Correlation;  //!
  //TProfile     *hV0ATPCPsi3Correlation;  //!  
  
  //TProfile      *fAvgCos2PsivsCentEtaPos; //!  Nth Harmonic usually N=2 
  //TProfile      *fAvgSin2PsivsCentEtaPos; //!
  //TProfile      *fAvgCos2PsivsCentEtaNeg; //!
  //TProfile      *fAvgSin2PsivsCentEtaNeg; //!
  //TProfile      *fAvgCos3PsivsCentEtaPos; //!  3rd Harmonic is hardcoded.
  //TProfile      *fAvgSin3PsivsCentEtaPos; //!
  //TProfile      *fAvgCos3PsivsCentEtaNeg; //!
  //TProfile      *fAvgSin3PsivsCentEtaNeg; //!
  //TProfile      *fAvgCos4PsivsCentEtaPos; //!  4th Harmonic is hardcoded.
  //TProfile      *fAvgSin4PsivsCentEtaPos; //!
  //TProfile      *fAvgCos4PsivsCentEtaNeg; //!
  //TProfile      *fAvgSin4PsivsCentEtaNeg; //!   

  
  //TProfile     *fHistVxvsVzMinBias;  //!
  //TProfile     *fHistVyvsVzMinBias;  //!
  TProfile2D     *hAvgV0ChannelsvsVz;   //!

  
  
  //// Some more functions:
  void  SetupQAHistograms();
  void  SetupAnalysisHistograms();
  void  SetupEventAndTaskConfigInfo();
  void  SetupPileUpRemovalFunctions18qPass3();
  void  SetupPileUpRemovalFunctions18rPass3();
  
  void  GetV0MCorrectionHist(Int_t run=0);
  void  GetZDCCorrectionHist(Int_t run);
  void  GetMCEfficiencyWeightForTrackHist();
  void  ApplyV0XqVectRecenter(Float_t fCent,Int_t gPsiN,Double_t &qnxV0C, Double_t &qnyV0C, Double_t &qnxV0A, Double_t &qnyV0A);
  
  
  Bool_t   CheckEventIsPileUp2018(AliAODEvent* faod);
  Bool_t   CheckPIDofParticle(AliAODTrack* ftrack,Int_t pidToCheck);
  Bool_t   GetGainCorrectedV0Qvector(AliAODEvent *faod,Double_t fVtxZ,Int_t gPsiN,Double_t &qnxV0C,Double_t &qnyV0C,Double_t &qnxV0A,Double_t &qnyV0A); 
  Bool_t   GetGainCorrectedV0Qvector(AliAODEvent *faod,Double_t fVtxZ,Int_t gPsiN,Double_t &qnxV0C,Double_t &qnyV0C,Double_t &qnxV0A,Double_t &qnyV0A,Double_t &sumMultV0C,Double_t &sumMultV0A); 

 

  
  
  AliAnalysisTaskGammaNonIsotropicCorrUsingWeights(const AliAnalysisTaskGammaNonIsotropicCorrUsingWeights &other);
  AliAnalysisTaskGammaNonIsotropicCorrUsingWeights &operator=(const AliAnalysisTaskGammaNonIsotropicCorrUsingWeights &other);    
  ClassDef(AliAnalysisTaskGammaNonIsotropicCorrUsingWeights, 1) 

};

#endif
