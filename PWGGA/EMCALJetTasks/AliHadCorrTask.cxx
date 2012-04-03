// $Id$

#include "AliHadCorrTask.h"
#include <TClonesArray.h>
#include <TParticle.h>
#include "AliEmcalJet.h"
#include "AliAnalysisManager.h"
#include "AliESDtrack.h"
#include "AliFJWrapper.h"
#include "AliESDCaloCluster.h"
#include "TList.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TChain.h"
#include <TLorentzVector.h>
#include <AliCentrality.h>
#include "AliPicoTrack.h"

ClassImp(AliHadCorrTask)

//________________________________________________________________________
AliHadCorrTask::AliHadCorrTask(const char *name) : 
  AliAnalysisTaskSE("AliHadCorrTask"),
  fTracksName("Tracks"),
  fCaloName("CaloClusters"),
  fMinPt(0.15),
  fOutputList(0),
  fHistEbefore(0),
  fHistEafter(0),
  fHistNclusvsCent(0),
  fHistNclusMatchvsCent(0),
  fOutCaloName("CaloClustersOut"),
  fOutClusters(0)
{

  for(Int_t i=0; i<4; i++){
    fHistMatchEtaPhi[i]=0x0;
    fHistMatchEvsP[i]=0x0;
    fHistMatchdRvsEP[i]=0x0;
  }

  // Standard constructor.
  cout << "Constructor for HadCorrTask " << name <<endl;
  if (!name)
    return;

  SetName(name);
  fBranchNames="ESD:AliESDRun.,AliESDHeader.,PrimaryVertex.";

  DefineInput(0,TChain::Class());
  DefineOutput(1,TList::Class());
}

AliHadCorrTask::AliHadCorrTask() : 
  AliAnalysisTaskSE("AliHadCorrTask"),
  fTracksName("Tracks"),
  fCaloName("CaloClusters"),
  fOutClusters(0)
{
  // Standard constructor.

  fBranchNames="ESD:AliESDRun.,AliESDHeader.,PrimaryVertex.";
}


//________________________________________________________________________
AliHadCorrTask::~AliHadCorrTask()
{
  // Destructor
}

//________________________________________________________________________
void AliHadCorrTask::UserCreateOutputObjects()
{

  fOutClusters = new TClonesArray("AliESDCaloCluster");
  fOutClusters->SetName(fOutCaloName);
 
  OpenFile(1);
  fOutputList = new TList();
  fOutputList->SetOwner();

  char name[200];

  for(int icent=0; icent<4; icent++){
    sprintf(name,"fHistMatchEtaPhi_%i",icent);
    fHistMatchEtaPhi[icent] = new TH2F(name,name,400,-0.2,0.2,1600,-0.8,0.8);
    sprintf(name,"fHistMatchEvsP_%i",icent);
    fHistMatchEvsP[icent]=new TH2F(name,name,400,0.,200.,1000,0.,10.);
    sprintf(name,"fHistMatchdRvsEP_%i",icent);
    fHistMatchdRvsEP[icent]=new TH2F(name,name,1000,0.,1.,1000,0.,10.);

    
  fOutputList->Add(fHistMatchEtaPhi[icent]);
  fOutputList->Add(fHistMatchEvsP[icent]);
  fOutputList->Add(fHistMatchdRvsEP[icent]);

  }
  fHistNclusvsCent=new TH1F("Nclusvscent","NclusVsCent",100,0,100);
  fHistNclusMatchvsCent=new TH1F("NclusMatchvscent","NclusMatchVsCent",100,0,100);
  fHistEbefore=new TH1F("Ebefore","Ebefore",100,0,100);
  fHistEafter=new TH1F("Eafter","Eafter",100,0,100);

  fOutputList->Add(fHistNclusMatchvsCent);
  fOutputList->Add(fHistNclusvsCent);
  fOutputList->Add(fHistEbefore);
  fOutputList->Add(fHistEafter);

  PostData(1, fOutputList);
}

//________________________________________________________________________
void AliHadCorrTask::UserExec(Option_t *) 
{

  if (!(InputEvent()->FindListObject(fOutCaloName)))
    InputEvent()->AddObject(fOutClusters);
  else fOutClusters->Delete();


  AliAnalysisManager *am = AliAnalysisManager::GetAnalysisManager();
  TClonesArray *tracks = 0;
  TClonesArray *clus   = 0;
  TList *l = InputEvent()->GetList();

  AliCentrality *centrality = 0;
  centrality = dynamic_cast<AliCentrality*>(l->FindObject("Centrality"));
  Float_t fCent=-1; 
  if(centrality)
    fCent = centrality->GetCentralityPercentile("V0M");
  else
    fCent=99;//probably pp data

  if(fCent<0) return;

  if (fTracksName == "Tracks")
    am->LoadBranch("Tracks");
  tracks = dynamic_cast<TClonesArray*>(l->FindObject(fTracksName));
  if (!tracks) {
    AliError(Form("Pointer to tracks %s == 0", fTracksName.Data() ));
    return;
  }
  const Int_t Ntrks = tracks->GetEntries();
  
  if (fCaloName == "CaloClusters")
    am->LoadBranch("CaloClusters");
  clus = dynamic_cast<TClonesArray*>(l->FindObject(fCaloName));
  if (!clus) {
    AliError(Form("Pointer to clus %s == 0", fCaloName.Data() ));
    return;
  }

  Int_t centbin=-1;
  if(fCent>=0 && fCent<10) centbin=0;
  else if(fCent>=10 && fCent<30) centbin=1;
  else if(fCent>=30 && fCent<50) centbin=2;
  else if(fCent>=50 && fCent<100) centbin=3;
 
  if (clus) {
    Double_t vertex[3] = {0, 0, 0};
    InputEvent()->GetPrimaryVertex()->GetXYZ(vertex);
    const Int_t Nclus = clus->GetEntries();
    for (Int_t iClus = 0, iN = 0, clusCount=0; iClus < Nclus; ++iClus) {
      AliVCluster *c = dynamic_cast<AliVCluster*>(clus->At(iClus));
      if (!c)
        continue;
      if (!c->IsEMCAL())
        continue;
      TLorentzVector nPart;

      c->SetEmcCpvDistance(-1);
      c->SetTrackDistance(999,999);
      Double_t dEtaMin  = 1e9;
      Double_t dPhiMin  = 1e9;
      Int_t    imin     = -1;
      for(Int_t t = 0; t<Ntrks; ++t) {
        AliVTrack *track = dynamic_cast<AliVTrack*>(tracks->At(t));
        Double_t etadiff=999;
        Double_t phidiff=999;
        AliPicoTrack::GetEtaPhiDiff(track,c,phidiff,etadiff);
        Double_t dR = TMath::Sqrt(etadiff*etadiff+phidiff*phidiff);
        Double_t dRmin = TMath::Sqrt(dEtaMin*dEtaMin+dPhiMin*dPhiMin);
        if(dR > 25) 
          continue;
	if(dR<dRmin){
          dEtaMin = etadiff;
          dPhiMin = phidiff;
          imin = t;
        }
      }
      c->SetEmcCpvDistance(imin);
      c->SetTrackDistance(dPhiMin, dEtaMin);
      c->GetMomentum(nPart, vertex);
      Double_t energy = nPart.P();
      if(energy<fMinPt) continue;
      if (imin>=0) {
	Double_t dPhiMin = c->GetTrackDx();
	Double_t dEtaMin = c->GetTrackDz();
	fHistMatchEtaPhi[centbin]->Fill(dEtaMin,dPhiMin);
      }

      fHistNclusvsCent->Fill(fCent);
    
      if (fHadCorr>0) {
	if (imin>=0) {
	  Double_t dPhiMin = c->GetTrackDx();
	  Double_t dEtaMin = c->GetTrackDz();
	  Double_t dR=TMath::Sqrt(dEtaMin*dEtaMin+dPhiMin*dPhiMin);
	      
	  AliVTrack *t = dynamic_cast<AliVTrack*>(tracks->At(imin));
	  if (t) {
	    if (t->Pt()<fMinPt)
	      continue;
	    if (t->P()>0) fHistMatchEvsP[centbin]->Fill(energy,energy/t->P());
	    if (t->P()>0) fHistMatchdRvsEP[centbin]->Fill(dR,energy/t->P());
	    fHistEbefore->Fill(fCent,energy);
	    if (dPhiMin<0.05 && dEtaMin<0.025) { // pp cuts!!!
              energy -= fHadCorr*t->P();
	      fHistNclusMatchvsCent->Fill(fCent);
	    }
	    if (energy<0)
	      continue;
	  }
	}
	fHistEafter->Fill(fCent,energy);

      }//end had correction if
      if (energy>0){//Output new corrected clusters
	AliESDCaloCluster *oc = new ((*fOutClusters)[clusCount]) AliESDCaloCluster(*(dynamic_cast<AliESDCaloCluster*>(c)));
	oc->SetE(energy);
	clusCount++;
      }
    }


  }
}
 

//________________________________________________________________________
void AliHadCorrTask::Terminate(Option_t *) 
{

}

//________________________________________________________________________
