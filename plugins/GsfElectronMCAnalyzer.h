#ifndef GsfElectronMCAnalyzer_h
#define GsfElectronMCAnalyzer_h
  
//
// Package:         RecoEgamma/Examples
// Class:           GsfElectronMCAnalyzer
// 

//
// Original Author:  Ursula Berthon, Claude Charlot
//         Created:  Mon Mar 27 13:22:06 CEST 2006
// $Id: GsfElectronMCAnalyzer.h,v 1.1 2008/04/21 13:44:57 uberthon Exp $
//
//
  
  
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include "DataFormats/Common/interface/EDProduct.h"
 
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"

class MagneticField;  
class TFile;
class TH1F;
class TH2F;
class TH1I;
class TProfile;
class TTree;

class GsfElectronMCAnalyzer : public edm::EDAnalyzer
{
 public:
  
  explicit GsfElectronMCAnalyzer(const edm::ParameterSet& conf);
  
  virtual ~GsfElectronMCAnalyzer();
  
  virtual void beginJob(edm::EventSetup const& iSetup);
  virtual void endJob();
  virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
  
 private:

  TrajectoryStateTransform transformer_;
  edm::ESHandle<TrackerGeometry> pDD;
  edm::ESHandle<MagneticField> theMagField;
  TFile *histfile_;
  TTree *tree_;
  float mcEnergy[10], mcEta[10], mcPhi[10], mcPt[10], mcQ[10];
  float superclusterEnergy[10], superclusterEta[10], superclusterPhi[10], superclusterEt[10];
  float seedMomentum[10], seedEta[10], seedPhi[10], seedPt[10], seedQ[10];

  TH1F *h_mcNum;
  TH1F *h_eleNum;
  TH1F *h_gamNum;
  
  TH1F *h_simEta;
  TH1F *h_simAbsEta;
  TH1F *h_simP;
  TH1F *h_simPt;
  TH1F *h_simPhi;
  TH1F *h_simZ;
  
  TH1F *h_ele_simEta_matched;
  TH1F *h_ele_simAbsEta_matched;
  TH1F *h_ele_simPt_matched;
  TH1F *h_ele_simPhi_matched;
  TH1F *h_ele_simZ_matched;

  TH1F *h_ele_TIP_all;
  TH1F *h_ele_EoverP_all;
  TH1F *h_ele_vertexEta_all;
  TH1F *h_ele_vertexPt_all;
  TH1F *h_ele_mee_all;

  TH1F *h_ele_charge;
  TH2F *h_ele_chargeVsEta;
  TH2F *h_ele_chargeVsPhi;
  TH2F *h_ele_chargeVsPt;
  TH1F *h_ele_vertexP;
  TH1F *h_ele_vertexPt;
  TH2F *h_ele_vertexPtVsEta;
  TH2F *h_ele_vertexPtVsPhi;
  TH1F *h_ele_vertexPt_5100;
  TH1F *h_ele_vertexEta;
  TH2F *h_ele_vertexEtaVsPhi;
  TH1F *h_ele_vertexAbsEta;
  TH1F *h_ele_vertexPhi;
  TH1F *h_ele_vertexX;
  TH1F *h_ele_vertexY;
  TH1F *h_ele_vertexZ;
  TH1F *h_ele_vertexTIP;  
  TH2F *h_ele_vertexTIPVsEta;  
  TH2F *h_ele_vertexTIPVsPhi;  
  TH2F *h_ele_vertexTIPVsPt;  
  
  TH1F *histNum_;

  TH1F *histSclEn_ ;
  TH1F *histSclEoEtrue_barrel;
  TH1F *histSclEoEtrue_endcaps;
  TH1F *histSclEt_ ;
  TH2F *histSclEtVsEta_ ;
  TH2F *histSclEtVsPhi_ ;
  TH2F *histSclEtaVsPhi_ ;
  TH1F *histSclEta_ ;
  TH1F *histSclPhi_ ;

  TH1F *h_ctf_foundHits;
  TH2F *h_ctf_foundHitsVsEta;
  TH2F *h_ctf_lostHitsVsEta;

  TH1F *h_ele_foundHits;
  TH2F *h_ele_foundHitsVsEta;
  TH2F *h_ele_foundHitsVsPhi;
  TH2F *h_ele_foundHitsVsPt;
  TH1F *h_ele_lostHits;
  TH2F *h_ele_lostHitsVsEta;
  TH2F *h_ele_lostHitsVsPhi;
  TH2F *h_ele_lostHitsVsPt;
  TH1F *h_ele_chi2;
  TH2F *h_ele_chi2VsEta;
  TH2F *h_ele_chi2VsPhi;
  TH2F *h_ele_chi2VsPt;
  
  TH1F *h_ele_PoPtrue;
  TH2F *h_ele_PoPtrueVsEta;
  TH2F *h_ele_PoPtrueVsPhi;
  TH2F *h_ele_PoPtrueVsPt;
  TH1F *h_ele_PoPtrue_barrel;
  TH1F *h_ele_PoPtrue_endcaps;
  TH1F *h_ele_EtaMnEtaTrue;
  TH2F *h_ele_EtaMnEtaTrueVsEta;
  TH2F *h_ele_EtaMnEtaTrueVsPhi;
  TH2F *h_ele_EtaMnEtaTrueVsPt;
  TH1F *h_ele_PhiMnPhiTrue;
  TH1F *h_ele_PhiMnPhiTrue2;
  TH2F *h_ele_PhiMnPhiTrueVsEta;
  TH2F *h_ele_PhiMnPhiTrueVsPhi;
  TH2F *h_ele_PhiMnPhiTrueVsPt;
  TH1F *h_ele_PinMnPout;
  TH1F *h_ele_PinMnPout_mode;
  TH2F *h_ele_PinMnPoutVsEta_mode;
  TH2F *h_ele_PinMnPoutVsPhi_mode;
  TH2F *h_ele_PinMnPoutVsPt_mode;
  TH2F *h_ele_PinMnPoutVsE_mode;
  TH2F *h_ele_PinMnPoutVsChi2_mode;
  
  TH1F *h_ele_outerP;
  TH1F *h_ele_outerP_mode;
  TH2F *h_ele_outerPVsEta_mode;
  TH1F *h_ele_outerPt;
  TH1F *h_ele_outerPt_mode;
  TH2F *h_ele_outerPtVsEta_mode;
  TH2F *h_ele_outerPtVsPhi_mode;
  TH2F *h_ele_outerPtVsPt_mode;
  TH1F *h_ele_EoP;
  TH2F *h_ele_EoPVsEta;
  TH2F *h_ele_EoPVsPhi;
  TH2F *h_ele_EoPVsE;
  TH1F *h_ele_EoPout;
  TH2F *h_ele_EoPoutVsEta;
  TH2F *h_ele_EoPoutVsPhi;
  TH2F *h_ele_EoPoutVsE;
  
  TH1F *h_ele_dEtaSc_propVtx;
  TH2F *h_ele_dEtaScVsEta_propVtx;
  TH2F *h_ele_dEtaScVsPhi_propVtx;
  TH2F *h_ele_dEtaScVsPt_propVtx;
  TH1F *h_ele_dPhiSc_propVtx;
  TH2F *h_ele_dPhiScVsEta_propVtx;
  TH2F *h_ele_dPhiScVsPhi_propVtx;
  TH2F *h_ele_dPhiScVsPt_propVtx;
  TH1F *h_ele_dEtaCl_propOut;
  TH2F *h_ele_dEtaClVsEta_propOut;
  TH2F *h_ele_dEtaClVsPhi_propOut;
  TH2F *h_ele_dEtaClVsPt_propOut;
  TH1F *h_ele_dPhiCl_propOut;
  TH2F *h_ele_dPhiClVsEta_propOut;
  TH2F *h_ele_dPhiClVsPhi_propOut;
  TH2F *h_ele_dPhiClVsPt_propOut;
  
  TH1F *h_ele_classes;
  TH1F *h_ele_eta;
  TH1F *h_ele_eta_golden;
  TH1F *h_ele_eta_bbrem;
  TH1F *h_ele_eta_narrow;
  TH1F *h_ele_eta_shower;
  
  TH1F *h_ele_HoE;
  TH2F *h_ele_HoEVsEta;
  TH2F *h_ele_HoEVsPhi;
  TH2F *h_ele_HoEVsE;
  
  TProfile *h_ele_fbremVsEta_mode;
  TProfile *h_ele_fbremVsEta_mean;
  
  TH2F *h_ele_PinVsPoutGolden_mode;
  TH2F *h_ele_PinVsPoutShowering0_mode;
  TH2F *h_ele_PinVsPoutShowering1234_mode;
  TH2F *h_ele_PinVsPoutGolden_mean;
  TH2F *h_ele_PinVsPoutShowering0_mean;
  TH2F *h_ele_PinVsPoutShowering1234_mean;
  TH2F *h_ele_PtinVsPtoutGolden_mode;
  TH2F *h_ele_PtinVsPtoutShowering0_mode;
  TH2F *h_ele_PtinVsPtoutShowering1234_mode;
  TH2F *h_ele_PtinVsPtoutGolden_mean;
  TH2F *h_ele_PtinVsPtoutShowering0_mean;
  TH2F *h_ele_PtinVsPtoutShowering1234_mean;
  TH1F *histSclEoEtrueGolden_barrel;
  TH1F *histSclEoEtrueGolden_endcaps;
  TH1F *histSclEoEtrueShowering0_barrel;
  TH1F *histSclEoEtrueShowering0_endcaps;
  TH1F *histSclEoEtrueShowering1234_barrel;
  TH1F *histSclEoEtrueShowering1234_endcaps;

  std::string outputFile_; 
  edm::InputTag electronCollection_;
  edm::InputTag  mcTruthCollection_;
  
  double maxPt_;
  double maxAbsEta_;
  double deltaR_; 
  
  // histos limits and binning
  double etamin;
  double etamax;
  double phimin;
  double phimax;
  double ptmax;
  double pmax;
  double eopmax;
  double eopmaxsht;
  double detamin;
  double detamax;
  double dphimin;
  double dphimax;
  double detamatchmin;
  double detamatchmax;
  double dphimatchmin;
  double dphimatchmax;
  double fhitsmax;
  double lhitsmax;
  int nbineta;
  int nbinp;
  int nbinpt;
  int nbinpteff;
  int nbinphi;
  int nbinp2D;
  int nbinpt2D;
  int nbineta2D;
  int nbinphi2D;
  int nbineop;
  int nbineop2D;
  int nbinfhits;
  int nbinlhits;
  int nbinxyz;
  int nbindeta;
  int nbindphi;
  int nbindetamatch;
  int nbindphimatch;
  int nbindetamatch2D;
  int nbindphimatch2D;
  
 };
  
#endif
 


