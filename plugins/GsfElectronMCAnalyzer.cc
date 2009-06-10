// -*- C++ -*-
//
// Package:    RecoEgamma/Examples
// Class:      GsfElectronMCAnalyzer
//
/**\class GsfElectronMCAnalyzer RecoEgamma/Examples/src/GsfElectronMCAnalyzer.cc

 Description: GsfElectrons analyzer using MC truth

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Ursula Berthon
//         Created:  Mon Mar 27 13:22:06 CEST 2006
// $Id: GsfElectronMCAnalyzer.cc,v 1.20 2009/05/27 09:37:51 fabiocos Exp $
//
//

// user include files
#include "RecoEgamma/Examples/plugins/GsfElectronMCAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/ElectronSeed.h"
#include "DataFormats/EgammaReco/interface/ElectronSeedFwd.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "CLHEP/Units/GlobalPhysicalConstants.h"
#include <iostream>
#include "TMath.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1I.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TTree.h"
#include <iostream>

using namespace reco;

GsfElectronMCAnalyzer::GsfElectronMCAnalyzer(const edm::ParameterSet& conf)
{
  outputFile_ = conf.getParameter<std::string>("outputFile");
  histfile_ = new TFile(outputFile_.c_str(),"RECREATE");
  electronCollection_=conf.getParameter<edm::InputTag>("electronCollection");
  mcTruthCollection_ = conf.getParameter<edm::InputTag>("mcTruthCollection");
  maxPt_ = conf.getParameter<double>("MaxPt");
  maxAbsEta_ = conf.getParameter<double>("MaxAbsEta");
  deltaR_ = conf.getParameter<double>("DeltaR");
  etamin=conf.getParameter<double>("Etamin");
  etamax=conf.getParameter<double>("Etamax");
  phimin=conf.getParameter<double>("Phimin");
  phimax=conf.getParameter<double>("Phimax");
  ptmax=conf.getParameter<double>("Ptmax");
  pmax=conf.getParameter<double>("Pmax");
  eopmax=conf.getParameter<double>("Eopmax");
  eopmaxsht=conf.getParameter<double>("Eopmaxsht");
  detamin=conf.getParameter<double>("Detamin");
  detamax=conf.getParameter<double>("Detamax");
  dphimin=conf.getParameter<double>("Dphimin");
  dphimax=conf.getParameter<double>("Dphimax");
  detamatchmin=conf.getParameter<double>("Detamatchmin");
  detamatchmax=conf.getParameter<double>("Detamatchmax");
  dphimatchmin=conf.getParameter<double>("Dphimatchmin");
  dphimatchmax=conf.getParameter<double>("Dphimatchmax");
  fhitsmax=conf.getParameter<double>("Fhitsmax");
  lhitsmax=conf.getParameter<double>("Lhitsmax");
  nbineta=conf.getParameter<int>("Nbineta");
  nbineta2D=conf.getParameter<int>("Nbineta2D");
  nbinp=conf.getParameter<int>("Nbinp");
  nbinpt=conf.getParameter<int>("Nbinpt");
  nbinp2D=conf.getParameter<int>("Nbinp2D");
  nbinpt2D=conf.getParameter<int>("Nbinpt2D");
  nbinpteff=conf.getParameter<int>("Nbinpteff");
  nbinphi=conf.getParameter<int>("Nbinphi");
  nbinphi2D=conf.getParameter<int>("Nbinphi2D");
  nbineop=conf.getParameter<int>("Nbineop");
  nbineop2D=conf.getParameter<int>("Nbineop2D");
  nbinfhits=conf.getParameter<int>("Nbinfhits");
  nbinlhits=conf.getParameter<int>("Nbinlhits");
  nbinxyz=conf.getParameter<int>("Nbinxyz");
  nbindeta=conf.getParameter<int>("Nbindeta");
  nbindphi=conf.getParameter<int>("Nbindphi");
  nbindetamatch=conf.getParameter<int>("Nbindetamatch");
  nbindphimatch=conf.getParameter<int>("Nbindphimatch");
  nbindetamatch2D=conf.getParameter<int>("Nbindetamatch2D");
  nbindphimatch2D=conf.getParameter<int>("Nbindphimatch2D");

}

void GsfElectronMCAnalyzer::beginJob(){

  histfile_->cd();

  TH1::SetDefaultSumw2(true);
  
  // mc truth
  h_mcNum              = new TH1F( "h_mcNum",              "# mc particles",    nbinfhits,0.,fhitsmax );
  h_eleNum             = new TH1F( "h_mcNum_ele",             "# mc electrons",             nbinfhits,0.,fhitsmax);
  h_gamNum             = new TH1F( "h_mcNum_gam",             "# mc gammas",             nbinfhits,0.,fhitsmax);

  // rec event
  histNum_= new TH1F("h_recEleNum","# rec electrons",20, 0.,20.);

  // mc
  h_simEta             = new TH1F( "h_mc_eta",             "gen #eta",           nbineta,etamin,etamax);
  h_simAbsEta             = new TH1F( "h_mc_abseta",             "gen |#eta|",           nbineta/2,0.,etamax);
  h_simP               = new TH1F( "h_mc_P",               "gen p",              nbinp,0.,pmax);
  h_simPt               = new TH1F( "h_mc_Pt",               "gen pt",            nbinpteff,5.,ptmax);
  h_simPhi               = new TH1F( "h_mc_phi",               "gen phi",        nbinphi,phimin,phimax);
  h_simZ      = new TH1F( "h_mc_z",      "gen z ",    nbinxyz, -25, 25 );

  // all electrons
  h_ele_EoverP_all       = new TH1F( "h_ele_EoverP_all",       "ele E/P_{vertex}, all reco electrons",  nbineop,0.,eopmax);
  h_ele_EseedOP_all            = new TH1F( "h_ele_EseedOP_all",            "ele E_{seed}/P_{vertex}, all reco electrons",        nbineop,0.,eopmax);
  h_ele_EoPout_all         = new TH1F( "h_ele_EoPout_all",         "ele E_{seed}/P_{out}, all reco electrons",           nbineop,0.,eopmax);
  h_ele_EeleOPout_all         = new TH1F( "h_ele_EeleOPout_all",         "ele E_{ele}/P_{out}, all reco electrons",           nbineop,0.,eopmax);
  h_ele_dEtaSc_propVtx_all = new TH1F( "h_ele_dEtaSc_propVtx_all", "ele #eta_{sc} - #eta_{tr} - prop from vertex, all reco electrons",      nbindetamatch,detamatchmin,detamatchmax);
  h_ele_dPhiSc_propVtx_all = new TH1F( "h_ele_dPhiSc_propVtx_all", "ele #phi_{sc} - #phi_{tr} - prop from vertex, all reco electrons",      nbindphimatch,dphimatchmin,dphimatchmax);
  h_ele_dEtaCl_propOut_all = new TH1F( "h_ele_dEtaCl_propOut_all", "ele #eta_{cl} - #eta_{tr} - prop from outermost, all reco electrons",   nbindetamatch,detamatchmin,detamatchmax);
  h_ele_dPhiCl_propOut_all = new TH1F( "h_ele_dPhiCl_propOut_all", "ele #phi_{cl} - #phi_{tr} - prop from outermost, all reco electrons",   nbindphimatch,dphimatchmin,dphimatchmax);
  h_ele_HoE_all = new TH1F("h_ele_HoE_all", "ele hadronic energy / em energy, all reco electrons", 55,-0.05,0.5) ;
  h_ele_vertexPt_all       = new TH1F( "h_ele_vertexPt_all",       "ele p_{T}, all reco electrons",  nbinpteff,5.,ptmax);
  h_ele_vertexEta_all      = new TH1F( "h_ele_vertexEta_all",      "ele eta, all reco electrons",    nbineta,etamin,etamax);
  h_ele_TIP_all       = new TH1F( "h_ele_TIP_all",       "ele vertex transverse radius, all reco electrons",  100,0.,0.2);
  h_ele_mee_all      = new TH1F( "h_ele_mee_all", "ele pairs invariant mass, all reco electrons", 100, 0., 150. );

  // charge ID
  h_ele_ChargeMnChargeTrue   = new TH1F( "h_ele_ChargeMnChargeTrue",   "ele charge - gen charge ",5,-1.,4.);
  h_ele_simEta_matched_qmisid             = new TH1F( "h_mc_eta_matched qmisid","charge misid vs gen eta", nbineta,etamin,etamax);
  h_ele_simAbsEta_matched_qmisid             = new TH1F( "h_mc_abseta_matched qmisid", "charge misid vs gen |eta|", nbineta/2,0.,etamax);
  h_ele_simPt_matched_qmisid               = new TH1F( "h_mc_Pt_matched qmisid", "charge misid vs gen transverse momentum", nbinpteff,5.,ptmax);
  h_ele_simPhi_matched_qmisid               = new TH1F( "h_mc_phi_matched_qmisid","charge misid vs gen phi", nbinphi,phimin,phimax);
  h_ele_simZ_matched_qmisid      = new TH1F( "h_mc_z_matched_qmisid","charge misid vs gen z",nbinxyz, -25, 25 );
  
  // matched electrons
  h_ele_charge         = new TH1F( "h_ele_charge",         "ele charge",             5,-2.,2.);
  h_ele_chargeVsEta    = new TH2F( "h_ele_chargeVsEta",         "ele charge vs eta", nbineta2D,etamin,etamax,5,-2.,2.);
  h_ele_chargeVsPhi    = new TH2F( "h_ele_chargeVsPhi",         "ele charge vs phi", nbinphi2D,phimin,phimax,5,-2.,2.);
  h_ele_chargeVsPt    = new TH2F( "h_ele_chargeVsPt",         "ele charge vs pt", nbinpt,0.,100.,5,-2.,2.);
  h_ele_vertexP        = new TH1F( "h_ele_vertexP",        "ele momentum",       nbinp,0.,pmax);
  h_ele_vertexPt       = new TH1F( "h_ele_vertexPt",       "ele transverse momentum",  nbinpt,0.,ptmax);
  h_ele_vertexPtVsEta   = new TH2F( "h_ele_vertexPtVsEta",       "ele transverse momentum vs eta",nbinpt2D,etamin,etamax,nbinpt2D,0.,ptmax);
  h_ele_vertexPtVsPhi   = new TH2F( "h_ele_vertexPtVsPhi",       "ele transverse momentum vs phi",nbinphi2D,phimin,phimax,nbinpt2D,0.,ptmax);
  h_ele_simPt_matched       = new TH1F( "h_ele_simPt_matched",       "Efficiency vs gen transverse momentum",  nbinpteff,5.,ptmax);
  h_ele_vertexEta      = new TH1F( "h_ele_vertexEta",      "ele momentum eta",    nbineta,etamin,etamax);
  h_ele_vertexEtaVsPhi  = new TH2F( "h_ele_vertexEtaVsPhi",      "ele momentum eta vs phi",nbineta2D,etamin,etamax,nbinphi2D,phimin,phimax );
  h_ele_simAbsEta_matched      = new TH1F( "h_ele_simAbsEta_matched",      "Efficiency vs gen |eta|",    nbineta/2,0.,2.5);
  h_ele_simEta_matched      = new TH1F( "h_ele_simEta_matched",      "Efficiency vs gen eta",    nbineta,etamin,etamax);
  h_ele_simPhi_matched               = new TH1F( "h_ele_simPhi_matched",               "Efficiency vs gen phi",        nbinphi,phimin,phimax);
  h_ele_vertexPhi      = new TH1F( "h_ele_vertexPhi",      "ele  momentum #phi",    nbinphi,phimin,phimax);
  h_ele_vertexX      = new TH1F( "h_ele_vertexX",      "ele vertex x",    nbinxyz,-0.1,0.1 );
  h_ele_vertexY      = new TH1F( "h_ele_vertexY",      "ele vertex y",    nbinxyz,-0.1,0.1 );
  h_ele_vertexZ      = new TH1F( "h_ele_vertexZ",      "ele vertex z",    nbinxyz,-25, 25 );
  h_ele_simZ_matched      = new TH1F( "h_ele_simZ_matched",      "Efficiency vs gen vertex z",    nbinxyz,-25,25);
  h_ele_vertexTIP      = new TH1F( "h_ele_vertexTIP",      "ele transverse impact parameter (wrt gen vtx)",    90,0.,0.15);
  h_ele_vertexTIPVsEta      = new TH2F( "h_ele_vertexTIPVsEta",      "ele transverse impact parameter (wrt gen vtx) vs eta", nbineta2D,etamin,etamax,45,0.,0.15);
  h_ele_vertexTIPVsPhi      = new TH2F( "h_ele_vertexTIPVsPhi",      "ele transverse impact parameter (wrt gen vtx) vs phi", nbinphi2D,phimin,phimax,45,0.,0.15);
  h_ele_vertexTIPVsPt      = new TH2F( "h_ele_vertexTIPVsPt",      "ele transverse impact parameter (wrt gen vtx) vs transverse momentum", nbinpt2D,0.,ptmax,45,0.,0.15);
  h_ele_PoPtrue        = new TH1F( "h_ele_PoPtrue",        "ele momentum / gen momentum", 75,0.,1.5);
  h_ele_PoPtrueVsEta   = new TH2F( "h_ele_PoPtrueVsEta",        "ele momentum / gen momentum vs eta", nbineta2D,etamin,etamax,50,0.,1.5);
  h_ele_PoPtrueVsPhi   = new TH2F( "h_ele_PoPtrueVsPhi",        "ele momentum / gen momentum vs phi", nbinphi2D,phimin,phimax,50,0.,1.5);
  h_ele_PoPtrueVsPt   = new TH2F( "h_ele_PoPtrueVsPt",        "ele momentum / gen momentum vs eta", nbinpt2D,0.,ptmax,50,0.,1.5);
  h_ele_PoPtrue_barrel         = new TH1F( "h_ele_PoPtrue_barrel",        "ele momentum / gen momentum, barrel",75,0.,1.5);
  h_ele_PoPtrue_endcaps        = new TH1F( "h_ele_PoPtrue_endcaps",        "ele momentum / gen momentum, endcaps",75,0.,1.5);
  h_ele_EtaMnEtaTrue   = new TH1F( "h_ele_EtaMnEtaTrue",   "ele momentum  eta - gen  eta",nbindeta,detamin,detamax);
  h_ele_EtaMnEtaTrueVsEta   = new TH2F( "h_ele_EtaMnEtaTrueVsEta",   "ele momentum  eta - gen  eta vs eta",nbineta2D,etamin,etamax,nbindeta/2,detamin,detamax);
  h_ele_EtaMnEtaTrueVsPhi   = new TH2F( "h_ele_EtaMnEtaTrueVsPhi",   "ele momentum  eta - gen  eta vs phi",nbinphi2D,phimin,phimax,nbindeta/2,detamin,detamax);
  h_ele_EtaMnEtaTrueVsPt   = new TH2F( "h_ele_EtaMnEtaTrueVsPt",   "ele momentum  eta - gen  eta vs pt",nbinpt,0.,ptmax,nbindeta/2,detamin,detamax);
  h_ele_PhiMnPhiTrue   = new TH1F( "h_ele_PhiMnPhiTrue",   "ele momentum  phi - gen  phi",nbindphi,dphimin,dphimax);
  h_ele_PhiMnPhiTrue2   = new TH1F( "h_ele_PhiMnPhiTrue2",   "ele momentum  phi - gen  phi",nbindphimatch2D,dphimatchmin,dphimatchmax);
  h_ele_PhiMnPhiTrueVsEta   = new TH2F( "h_ele_PhiMnPhiTrueVsEta",   "ele momentum  phi - gen  phi vs eta",nbineta2D,etamin,etamax,nbindphi/2,dphimin,dphimax);
  h_ele_PhiMnPhiTrueVsPhi   = new TH2F( "h_ele_PhiMnPhiTrueVsPhi",   "ele momentum  phi - gen  phi vs phi",nbinphi2D,phimin,phimax,nbindphi/2,dphimin,dphimax);
  h_ele_PhiMnPhiTrueVsPt   = new TH2F( "h_ele_PhiMnPhiTrueVsPt",   "ele momentum  phi - gen  phi vs pt",nbinpt2D,0.,ptmax,nbindphi/2,dphimin,dphimax);

  // matched electron, superclusters
  histSclEn_ = new TH1F("h_scl_energy","ele supercluster energy",nbinp,0.,pmax);
  histSclEoEtrue_barrel = new TH1F("h_scl_EoEtrue_barrel","ele supercluster energy / gen energy, barrel",50,0.2,1.2);
  histSclEoEtrue_barrel_eg = new TH1F("h_scl_EoEtrue_barrel_eg","ele supercluster energy / gen energy, barrel, ecal driven",50,0.2,1.2);
  histSclEoEtrue_barrel_etagap = new TH1F("h_scl_EoEtrue_barrel_etagap","ele supercluster energy / gen energy, barrel, etagap",50,0.2,1.2);
  histSclEoEtrue_barrel_phigap = new TH1F("h_scl_EoEtrue_barrel_phigap","ele supercluster energy / gen energy, barrel, phigap",50,0.2,1.2);
  histSclEoEtrue_ebeegap = new TH1F("h_scl_EoEtrue_ebeegap","ele supercluster energy / gen energy, ebeegap",50,0.2,1.2);
  histSclEoEtrue_endcaps = new TH1F("h_scl_EoEtrue_endcaps","ele supercluster energy / gen energy, endcaps",50,0.2,1.2);
  histSclEoEtrue_endcaps_eg = new TH1F("h_scl_EoEtrue_endcaps_eg","ele supercluster energy / gen energy, endcaps, ecal driven",50,0.2,1.2);
  histSclEoEtrue_endcaps_deegap = new TH1F("h_scl_EoEtrue_endcaps_deegap","ele supercluster energy / gen energy, endcaps, deegap",50,0.2,1.2);
  histSclEoEtrue_endcaps_ringgap = new TH1F("h_scl_EoEtrue_endcaps_ringgap","ele supercluster energy / gen energy, endcaps, ringgap",50,0.2,1.2);
  histSclEt_ = new TH1F("h_scl_et","ele supercluster transverse energy",nbinpt,0.,ptmax);
  histSclEtVsEta_ = new TH2F("h_scl_etVsEta","ele supercluster transverse energy vs eta",nbineta2D,etamin,etamax,nbinpt,0.,ptmax);
  histSclEtVsPhi_ = new TH2F("h_scl_etVsPhi","ele supercluster transverse energy vs phi",nbinphi2D,phimin,phimax,nbinpt,0.,ptmax);
  histSclEtaVsPhi_ = new TH2F("h_scl_etaVsPhi","ele supercluster eta vs phi",nbinphi2D,phimin,phimax,nbineta2D,etamin,etamax);
  histSclEta_ = new TH1F("h_scl_eta","ele supercluster eta",nbineta,etamin,etamax);
  histSclPhi_ = new TH1F("h_scl_phi","ele supercluster phi",nbinphi,phimin,phimax);

  histSclSigEtaEta_ =  new TH1F("h_scl_sigetaeta","ele supercluster sigma eta eta",100,0.,0.05);
  histSclSigIEtaIEtabarrel_ =  new TH1F("h_scl_sigietaieta_barrel","ele supercluster sigma ieta ieta, barrel",100,0.,0.05);
  histSclSigIEtaIEtaendcaps_ =  new TH1F("h_scl_sigietaieta_endcaps","ele supercluster sigma ieta ieta, endcaps",100,0.,0.05);
  histSclE1x5_ =  new TH1F("h_scl_E1x5","ele supercluster energy in 1x5",nbinp,0., pmax);
  histSclE2x5max_ =  new TH1F("h_scl_E2x5max","ele supercluster energy in 2x5 max",nbinp,0.,pmax);
  histSclE5x5_ =  new TH1F("h_scl_E5x5","ele supercluster energy in 5x5",nbinp,0.,pmax);
  histSclSigEtaEta_eg_ =  new TH1F("h_scl_sigetaeta_eg","ele supercluster sigma eta eta, ecal driven",100,0.,0.05);
  histSclSigIEtaIEtabarrel_eg_ =  new TH1F("h_scl_sigietaieta_barrel_eg","ele supercluster sigma ieta ieta, barrel, ecal driven",100,0.,0.05);
  histSclSigIEtaIEtaendcaps_eg_ =  new TH1F("h_scl_sigietaieta_endcaps_eg","ele supercluster sigma ieta ieta, endcaps, ecal driven",100,0.,0.05);
  histSclE1x5_eg_ =  new TH1F("h_scl_E1x5_eg","ele supercluster energy in 1x5, ecal driven",nbinp,0., pmax);
  histSclE2x5max_eg_ =  new TH1F("h_scl_E2x5max_eg","ele supercluster energy in 2x5 max, ecal driven",nbinp,0.,pmax);
  histSclE5x5_eg_ =  new TH1F("h_scl_E5x5_eg","ele supercluster energy in 5x5, ecal driven",nbinp,0.,pmax);

  histSclEoEtruePfVsEg = new TH2F("h_scl_EoEtruePfVseg","ele supercluster energy / gen energy pflow vs eg",75,-0.1,1.4, 75, -0.1, 1.4);

  // matched electron, gsf tracks
  h_ele_ambiguousTracks      = new TH1F( "h_ele_ambiguousTracks", "ele # ambiguous tracks",  5,0.,5.);
  h_ele_ambiguousTracksVsEta      = new TH2F( "h_ele_ambiguousTracksVsEta","ele # ambiguous tracks  vs eta",  nbineta2D,etamin,etamax,5,0.,5.);
  h_ele_ambiguousTracksVsPhi      = new TH2F( "h_ele_ambiguousTracksVsPhi", "ele # ambiguous tracks  vs phi",  nbinphi2D,phimin,phimax,5,0.,5.);
  h_ele_ambiguousTracksVsPt      = new TH2F( "h_ele_ambiguousTracksVsPt", "ele # ambiguous tracks vs pt",  nbinpt2D,0.,ptmax,5,0.,5.);
  h_ele_foundHits      = new TH1F( "h_ele_foundHits",      "ele track # found hits",      nbinfhits,0.,fhitsmax);
  h_ele_foundHitsVsEta      = new TH2F( "h_ele_foundHitsVsEta",      "ele track # found hits vs eta",  nbineta2D,etamin,etamax,nbinfhits,0.,fhitsmax);
  h_ele_foundHitsVsPhi      = new TH2F( "h_ele_foundHitsVsPhi",      "ele track # found hits vs phi",  nbinphi2D,phimin,phimax,nbinfhits,0.,fhitsmax);
  h_ele_foundHitsVsPt      = new TH2F( "h_ele_foundHitsVsPt",      "ele track # found hits vs pt",  nbinpt2D,0.,ptmax,nbinfhits,0.,fhitsmax);
  h_ele_lostHits       = new TH1F( "h_ele_lostHits",       "ele track # lost hits",       5,0.,5.);
  h_ele_lostHitsVsEta       = new TH2F( "h_ele_lostHitsVsEta",       "ele track # lost hits vs eta",   nbineta2D,etamin,etamax,nbinlhits,0.,lhitsmax);
  h_ele_lostHitsVsPhi       = new TH2F( "h_ele_lostHitsVsPhi",       "ele track # lost hits vs eta",   nbinphi2D,phimin,phimax,nbinlhits,0.,lhitsmax);
  h_ele_lostHitsVsPt       = new TH2F( "h_ele_lostHitsVsPt",       "ele track # lost hits vs eta",   nbinpt2D,0.,ptmax,nbinlhits,0.,lhitsmax);
  h_ele_chi2           = new TH1F( "h_ele_chi2",           "ele track #chi^{2}",         100,0.,15.);
  h_ele_chi2VsEta           = new TH2F( "h_ele_chi2VsEta",           "ele track #chi^{2} vs eta",  nbineta2D,etamin,etamax,50,0.,15.);
  h_ele_chi2VsPhi           = new TH2F( "h_ele_chi2VsPhi",           "ele track #chi^{2} vs phi",  nbinphi2D,phimin,phimax,50,0.,15.);
  h_ele_chi2VsPt           = new TH2F( "h_ele_chi2VsPt",           "ele track #chi^{2} vs pt",  nbinpt2D,0.,ptmax,50,0.,15.);
  h_ele_PinMnPout      = new TH1F( "h_ele_PinMnPout",      "ele track inner p - outer p, mean"   ,nbinp,0.,200.);
  h_ele_PinMnPout_mode      = new TH1F( "h_ele_PinMnPout_mode",      "ele track inner p - outer p, mode"   ,nbinp,0.,100.);
  h_ele_PinMnPoutVsEta_mode = new TH2F( "h_ele_PinMnPoutVsEta_mode",      "ele track inner p - outer p vs eta, mode" ,nbineta2D, etamin,etamax,nbinp2D,0.,100.);
  h_ele_PinMnPoutVsPhi_mode = new TH2F( "h_ele_PinMnPoutVsPhi_mode",      "ele track inner p - outer p vs phi, mode" ,nbinphi2D, phimin,phimax,nbinp2D,0.,100.);
  h_ele_PinMnPoutVsPt_mode = new TH2F( "h_ele_PinMnPoutVsPt_mode",      "ele track inner p - outer p vs pt, mode" ,nbinpt2D, 0.,ptmax,nbinp2D,0.,100.);
  h_ele_PinMnPoutVsE_mode = new TH2F( "h_ele_PinMnPoutVsE_mode",      "ele track inner p - outer p vs E, mode" ,nbinp2D, 0.,200.,nbinp2D,0.,100.);
  h_ele_PinMnPoutVsChi2_mode = new TH2F( "h_ele_PinMnPoutVsChi2_mode",      "ele track inner p - outer p vs track chi2, mode" ,50, 0.,20.,nbinp2D,0.,100.);
  h_ele_outerP         = new TH1F( "h_ele_outerP",         "ele track outer p, mean",          nbinp,0.,pmax);
  h_ele_outerP_mode         = new TH1F( "h_ele_outerP_mode",         "ele track outer p, mode",          nbinp,0.,pmax);
  h_ele_outerPVsEta_mode         = new TH2F( "h_ele_outerPVsEta_mode",         "ele track outer p vs eta mode", nbineta2D,etamin,etamax,50,0.,pmax);
  h_ele_outerPt        = new TH1F( "h_ele_outerPt",        "ele track outer p_{T}, mean",      nbinpt,0.,ptmax);
  h_ele_outerPt_mode        = new TH1F( "h_ele_outerPt_mode",        "ele track outer p_{T}, mode",      nbinpt,0.,ptmax);
  h_ele_outerPtVsEta_mode        = new TH2F( "h_ele_outerPtVsEta_mode", "ele track outer p_{T} vs eta, mode", nbineta2D,etamin,etamax,nbinpt2D,0.,ptmax);
  h_ele_outerPtVsPhi_mode        = new TH2F( "h_ele_outerPtVsPhi_mode", "ele track outer p_{T} vs phi, mode", nbinphi2D,phimin,phimax,nbinpt2D,0.,ptmax);
  h_ele_outerPtVsPt_mode        = new TH2F( "h_ele_outerPtVsPt_mode", "ele track outer p_{T} vs pt, mode", nbinpt2D,0.,100.,nbinpt2D,0.,ptmax);

  // matched electrons, matching
  h_ele_EoP            = new TH1F( "h_ele_EoP",            "ele E/P_{vertex}",        nbineop,0.,eopmax);
  h_ele_EoP_eg            = new TH1F( "h_ele_EoP_eg",            "ele E/P_{vertex}, ecal driven",        nbineop,0.,eopmax);
  h_ele_EoPVsEta            = new TH2F( "h_ele_EoPVsEta",            "ele E/P_{vertex} vs eta",  nbineta2D,etamin,etamax,nbineop2D,0.,eopmaxsht);
  h_ele_EoPVsPhi            = new TH2F( "h_ele_EoPVsPhi",            "ele E/P_{vertex} vs phi",  nbinphi2D,phimin,phimax,nbineop2D,0.,eopmaxsht);
  h_ele_EoPVsE            = new TH2F( "h_ele_EoPVsE",            "ele E/P_{vertex} vs E",  50,0.,pmax ,50,0.,5.);
  h_ele_EseedOP            = new TH1F( "h_ele_EseedOP",            "ele E_{seed}/P_{vertex}",        nbineop,0.,eopmax);
  h_ele_EseedOP_eg            = new TH1F( "h_ele_EseedOP_eg",            "ele E_{seed}/P_{vertex}, ecal driven",        nbineop,0.,eopmax);
  h_ele_EseedOPVsEta            = new TH2F( "h_ele_EseedOPVsEta",            "ele E_{seed}/P_{vertex} vs eta",  nbineta2D,etamin,etamax,nbineop2D,0.,eopmaxsht);
  h_ele_EseedOPVsPhi            = new TH2F( "h_ele_EseedOPVsPhi",            "ele E_{seed}/P_{vertex} vs phi",  nbinphi2D,phimin,phimax,nbineop2D,0.,eopmaxsht);
  h_ele_EseedOPVsE            = new TH2F( "h_ele_EseedOPVsE",            "ele E_{seed}/P_{vertex} vs E",  50,0.,pmax ,50,0.,5.);
  h_ele_EoPout         = new TH1F( "h_ele_EoPout",         "ele E_{seed}/P_{out}",           nbineop,0.,eopmax);
  h_ele_EoPout_eg         = new TH1F( "h_ele_EoPout_eg",         "ele E_{seed}/P_{out}, ecal driven",           nbineop,0.,eopmax);
  h_ele_EoPoutVsEta         = new TH2F( "h_ele_EoPoutVsEta",         "ele E_{seed}/P_{out} vs eta",    nbineta2D,etamin,etamax,nbineop2D,0.,eopmaxsht);
  h_ele_EoPoutVsPhi         = new TH2F( "h_ele_EoPoutVsPhi",         "ele E_{seed}/P_{out} vs phi",    nbinphi2D,phimin,phimax,nbineop2D,0.,eopmaxsht);
  h_ele_EoPoutVsE         = new TH2F( "h_ele_EoPoutVsE",         "ele E_{seed}/P_{out} vs E",    nbinp2D,0.,pmax,nbineop2D,0.,eopmaxsht);
  h_ele_EeleOPout         = new TH1F( "h_ele_EeleOPout",         "ele E_{ele}/P_{out}",           nbineop,0.,eopmax);
  h_ele_EeleOPout_eg         = new TH1F( "h_ele_EeleOPout_eg",         "ele E_{ele}/P_{out}, ecal driven",           nbineop,0.,eopmax);
  h_ele_EeleOPoutVsEta         = new TH2F( "h_ele_EeleOPoutVsEta",         "ele E_{ele}/P_{out} vs eta",    nbineta2D,etamin,etamax,nbineop2D,0.,eopmaxsht);
  h_ele_EeleOPoutVsPhi         = new TH2F( "h_ele_EeleOPoutVsPhi",         "ele E_{ele}/P_{out} vs phi",    nbinphi2D,phimin,phimax,nbineop2D,0.,eopmaxsht);
  h_ele_EeleOPoutVsE         = new TH2F( "h_ele_EeleOPoutVsE",         "ele E_{ele}/P_{out} vs E",    nbinp2D,0.,pmax,nbineop2D,0.,eopmaxsht);
  h_ele_dEtaSc_propVtx = new TH1F( "h_ele_dEtaSc_propVtx", "ele #eta_{sc} - #eta_{tr} - prop from vertex",      nbindetamatch,detamatchmin,detamatchmax);
  h_ele_dEtaSc_propVtx_eg = new TH1F( "h_ele_dEtaSc_propVtx_eg", "ele #eta_{sc} - #eta_{tr} - prop from vertex, ecal driven",      nbindetamatch,detamatchmin,detamatchmax);
  h_ele_dEtaScVsEta_propVtx = new TH2F( "h_ele_dEtaScVsEta_propVtx", "ele #eta_{sc} - #eta_{tr} vs eta, prop from vertex", nbineta2D,etamin,etamax,nbindetamatch2D,detamatchmin,detamatchmax);
  h_ele_dEtaScVsPhi_propVtx = new TH2F( "h_ele_dEtaScVsPhi_propVtx", "ele #eta_{sc} - #eta_{tr} vs phi, prop from vertex", nbinphi2D,phimin,phimax,nbindetamatch2D,detamatchmin,detamatchmax);
  h_ele_dEtaScVsPt_propVtx = new TH2F( "h_ele_dEtaScVsPt_propVtx", "ele #eta_{sc} - #eta_{tr} vs pt, prop from vertex", nbinpt2D,0.,ptmax,nbindetamatch2D,detamatchmin,detamatchmax);
  h_ele_dPhiSc_propVtx = new TH1F( "h_ele_dPhiSc_propVtx", "ele #phi_{sc} - #phi_{tr} - prop from vertex",      nbindphimatch,dphimatchmin,dphimatchmax);
  h_ele_dPhiSc_propVtx_eg = new TH1F( "h_ele_dPhiSc_propVtx_eg", "ele #phi_{sc} - #phi_{tr} - prop from vertex, ecal driven",      nbindphimatch,dphimatchmin,dphimatchmax);
  h_ele_dPhiScVsEta_propVtx = new TH2F( "h_ele_dPhiScVsEta_propVtx", "ele #phi_{sc} - #phi_{tr} vs eta, prop from vertex", nbineta2D,etamin,etamax,nbindphimatch2D,dphimatchmin,dphimatchmax);
  h_ele_dPhiScVsPhi_propVtx = new TH2F( "h_ele_dPhiScVsPhi_propVtx", "ele #phi_{sc} - #phi_{tr} vs phi, prop from vertex", nbinphi2D,phimin,phimax,nbindphimatch2D,dphimatchmin,dphimatchmax);
  h_ele_dPhiScVsPt_propVtx = new TH2F( "h_ele_dPhiScVsPt_propVtx", "ele #phi_{sc} - #phi_{tr} vs pt, prop from vertex", nbinpt2D,0.,ptmax,nbindphimatch2D,dphimatchmin,dphimatchmax);
  h_ele_dEtaCl_propOut = new TH1F( "h_ele_dEtaCl_propOut", "ele #eta_{cl} - #eta_{tr} - prop from outermost",   nbindetamatch,detamatchmin,detamatchmax);
  h_ele_dEtaCl_propOut_eg = new TH1F( "h_ele_dEtaCl_propOut_eg", "ele #eta_{cl} - #eta_{tr} - prop from outermost, ecal driven",   nbindetamatch,detamatchmin,detamatchmax);
  h_ele_dEtaClVsEta_propOut = new TH2F( "h_ele_dEtaClVsEta_propOut", "ele #eta_{cl} - #eta_{tr} vs eta, prop from out", nbineta2D,etamin,etamax,nbindetamatch2D,detamatchmin,detamatchmax);
  h_ele_dEtaClVsPhi_propOut = new TH2F( "h_ele_dEtaClVsPhi_propOut", "ele #eta_{cl} - #eta_{tr} vs phi, prop from out", nbinphi2D,phimin,phimax,nbindetamatch2D,detamatchmin,detamatchmax);
  h_ele_dEtaClVsPt_propOut = new TH2F( "h_ele_dEtaScVsPt_propOut", "ele #eta_{cl} - #eta_{tr} vs pt, prop from out", nbinpt2D,0.,ptmax,nbindetamatch2D,detamatchmin,detamatchmax);
  h_ele_dPhiCl_propOut = new TH1F( "h_ele_dPhiCl_propOut", "ele #phi_{cl} - #phi_{tr} - prop from outermost",   nbindphimatch,dphimatchmin,dphimatchmax);
  h_ele_dPhiCl_propOut_eg = new TH1F( "h_ele_dPhiCl_propOut_eg", "ele #phi_{cl} - #phi_{tr} - prop from outermost, ecal driven",   nbindphimatch,dphimatchmin,dphimatchmax);
  h_ele_dPhiClVsEta_propOut = new TH2F( "h_ele_dPhiClVsEta_propOut", "ele #phi_{cl} - #phi_{tr} vs eta, prop from out", nbineta2D,etamin,etamax,nbindphimatch2D,dphimatchmin,dphimatchmax);
  h_ele_dPhiClVsPhi_propOut = new TH2F( "h_ele_dPhiClVsPhi_propOut", "ele #phi_{cl} - #phi_{tr} vs phi, prop from out", nbinphi2D,phimin,phimax,nbindphimatch2D,dphimatchmin,dphimatchmax);
  h_ele_dPhiClVsPt_propOut = new TH2F( "h_ele_dPhiSClsPt_propOut", "ele #phi_{cl} - #phi_{tr} vs pt, prop from out", nbinpt2D,0.,ptmax,nbindphimatch2D,dphimatchmin,dphimatchmax);
  h_ele_dEtaEleCl_propOut = new TH1F( "h_ele_dEtaEleCl_propOut", "ele #eta_{EleCl} - #eta_{tr} - prop from outermost",   nbindetamatch,detamatchmin,detamatchmax);
  h_ele_dEtaEleCl_propOut_eg = new TH1F( "h_ele_dEtaEleCl_propOut_eg", "ele #eta_{EleCl} - #eta_{tr} - prop from outermost, ecal driven",   nbindetamatch,detamatchmin,detamatchmax);
  h_ele_dEtaEleClVsEta_propOut = new TH2F( "h_ele_dEtaEleClVsEta_propOut", "ele #eta_{EleCl} - #eta_{tr} vs eta, prop from out", nbineta2D,etamin,etamax,nbindetamatch2D,detamatchmin,detamatchmax);
  h_ele_dEtaEleClVsPhi_propOut = new TH2F( "h_ele_dEtaEleClVsPhi_propOut", "ele #eta_{EleCl} - #eta_{tr} vs phi, prop from out", nbinphi2D,phimin,phimax,nbindetamatch2D,detamatchmin,detamatchmax);
  h_ele_dEtaEleClVsPt_propOut = new TH2F( "h_ele_dEtaScVsPt_propOut", "ele #eta_{EleCl} - #eta_{tr} vs pt, prop from out", nbinpt2D,0.,ptmax,nbindetamatch2D,detamatchmin,detamatchmax);
  h_ele_dPhiEleCl_propOut = new TH1F( "h_ele_dPhiEleCl_propOut", "ele #phi_{EleCl} - #phi_{tr} - prop from outermost",   nbindphimatch,dphimatchmin,dphimatchmax);
  h_ele_dPhiEleCl_propOut_eg = new TH1F( "h_ele_dPhiEleCl_propOut_eg", "ele #phi_{EleCl} - #phi_{tr} - prop from outermost, ecal driven",   nbindphimatch,dphimatchmin,dphimatchmax);
  h_ele_dPhiEleClVsEta_propOut = new TH2F( "h_ele_dPhiEleClVsEta_propOut", "ele #phi_{EleCl} - #phi_{tr} vs eta, prop from out", nbineta2D,etamin,etamax,nbindphimatch2D,dphimatchmin,dphimatchmax);
  h_ele_dPhiEleClVsPhi_propOut = new TH2F( "h_ele_dPhiEleClVsPhi_propOut", "ele #phi_{EleCl} - #phi_{tr} vs phi, prop from out", nbinphi2D,phimin,phimax,nbindphimatch2D,dphimatchmin,dphimatchmax);
  h_ele_dPhiEleClVsPt_propOut = new TH2F( "h_ele_dPhiSEleClsPt_propOut", "ele #phi_{EleCl} - #phi_{tr} vs pt, prop from out", nbinpt2D,0.,ptmax,nbindphimatch2D,dphimatchmin,dphimatchmax);

  h_ele_HoE = new TH1F("h_ele_HoE", "ele hadronic energy / em energy", 55,-0.05,0.5) ;
  h_ele_HoE_eg = new TH1F("h_ele_HoE_eg", "ele hadronic energy / em energy, ecal driven", 55,-0.05,0.5) ;
  h_ele_HoE_fiducial = new TH1F("h_ele_HoE_fiducial", "ele hadronic energy / em energy, fiducial region", 55,-0.05,0.5) ;
  h_ele_HoEVsEta = new TH2F("h_ele_HoEVsEta", "ele hadronic energy / em energy vs eta", nbineta,etamin,etamax,55,-0.05,0.5) ;
  h_ele_HoEVsPhi = new TH2F("h_ele_HoEVsPhi", "ele hadronic energy / em energy vs phi", nbinphi2D,phimin,phimax,55,-0.05,0.5) ;
  h_ele_HoEVsE = new TH2F("h_ele_HoEVsE", "ele hadronic energy / em energy vs E", nbinp, 0.,300.,55,-0.05,0.5) ;

  h_ele_seed_dphi2_ = new TH1F("h_ele_seedDphi2", "ele seed dphi 2nd layer", 50,-0.003,+0.003) ;
  h_ele_seed_dphi2VsEta_ = new TH2F("h_ele_seedDphi2VsEta", "ele seed dphi 2nd layer vs eta", nbineta2D,etamin,etamax,50,-0.003,+0.003) ;
  h_ele_seed_dphi2VsPt_ = new TH2F("h_ele_seedDphi2VsPt", "ele seed dphi 2nd layer vs pt", nbinpt2D,0.,ptmax,50,-0.003,+0.003) ;
  h_ele_seed_drz2_ = new TH1F("h_ele_seedDrz2", "ele seed dr (dz) 2nd layer", 50,-0.03,+0.03) ;
  h_ele_seed_drz2VsEta_ = new TH2F("h_ele_seedDrz2VsEta", "ele seed dr/dz 2nd layer vs eta", nbineta2D,etamin,etamax,50,-0.03,+0.03) ;
  h_ele_seed_drz2VsPt_ = new TH2F("h_ele_seedDrz2VsPt", "ele seed dr/dz 2nd layer vs pt", nbinpt2D,0.,ptmax,50,-0.03,+0.03) ;
  h_ele_seed_subdet2_ = new TH1F("h_ele_seedSubdet2", "ele seed subdet 2nd layer", 10,0.,10.) ;

  // classes
  h_ele_classes = new TH1F( "h_ele_classes", "ele classes",      20,0.0,20.);
  h_ele_eta = new TH1F( "h_ele_eta", "electron eta",  nbineta/2,0.0,etamax);
  h_ele_eta_golden = new TH1F( "h_ele_eta_golden", "electron eta golden",  nbineta/2,0.0,etamax);
  h_ele_eta_bbrem = new TH1F( "h_ele_eta_bbrem", "electron eta bbrem",  nbineta/2,0.0,etamax);
  h_ele_eta_narrow = new TH1F( "h_ele_eta_narrow", "electron eta narrow",  nbineta/2,0.0,etamax);
  h_ele_eta_shower = new TH1F( "h_ele_eta_show", "electron eta showering",  nbineta/2,0.0,etamax);
  h_ele_PinVsPoutGolden_mode = new TH2F( "h_ele_PinVsPoutGolden_mode",      "ele track inner p vs outer p vs eta, golden, mode" ,nbinp2D,0.,pmax,50,0.,pmax);
  h_ele_PinVsPoutShowering_mode = new TH2F( "h_ele_PinVsPoutShowering_mode",      "ele track inner p vs outer p vs eta, showering, mode" ,nbinp2D,0.,pmax,50,0.,pmax);
  h_ele_PinVsPoutGolden_mean = new TH2F( "h_ele_PinVsPoutGolden_mean",      "ele track inner p vs outer p vs eta, golden, mean" ,nbinp2D,0.,pmax,50,0.,pmax);
  h_ele_PinVsPoutShowering_mean = new TH2F( "h_ele_PinVsPoutShowering_mean",      "ele track inner p vs outer p vs eta, showering, mean" ,nbinp2D,0.,pmax,50,0.,pmax);
  h_ele_PtinVsPtoutGolden_mode = new TH2F( "h_ele_PtinVsPtoutGolden_mode",      "ele track inner pt vs outer pt vs eta, golden, mode" ,nbinpt2D,0.,ptmax,50,0.,ptmax);
  h_ele_PtinVsPtoutShowering_mode = new TH2F( "h_ele_PtinVsPtoutShowering_mode",      "ele track inner pt vs outer pt vs eta, showering, mode" ,nbinpt2D,0.,ptmax,50,0.,ptmax);
  h_ele_PtinVsPtoutGolden_mean = new TH2F( "h_ele_PtinVsPtoutGolden_mean",      "ele track inner pt vs outer pt vs eta, golden, mean" ,nbinpt2D,0.,ptmax,50,0.,ptmax);
  h_ele_PtinVsPtoutShowering_mean = new TH2F( "h_ele_PtinVsPtoutShowering_mean",      "ele track inner pt vs outer pt vs eta, showering, mean" ,nbinpt2D,0.,ptmax,50,0.,ptmax);
  histSclEoEtrueGolden_barrel = new TH1F("h_scl_EoEtrue golden, barrel","ele supercluster energy / gen energy, golden, barrel",100,0.2,1.2);
  histSclEoEtrueGolden_endcaps = new TH1F("h_scl_EoEtrue golden, endcaps","ele supercluster energy / gen energy, golden, endcaps",100,0.2,1.2);
  histSclEoEtrueShowering_barrel = new TH1F("h_scl_EoEtrue showering, barrel","ele supercluster energy / gen energy, showering, barrel",100,0.2,1.2);
  histSclEoEtrueShowering_endcaps = new TH1F("h_scl_EoEtrue showering, endcaps","ele supercluster energy / gen energy, showering, endcaps",100,0.2,1.2);
    
  // isolation  
  h_ele_tkSumPt_dr03 = new TH1F("h_ele_tkSumPt_dr03","tk isolation deposit, dR=0.3",100,0.0,20.);
  h_ele_ecalRecHitSumEt_dr03= new TH1F("h_ele_ecalRecHitSumEt_dr03","ecal isolation deposit, dR=0.3",100,0.0,20.);
  h_ele_hcalDepth1TowerSumEt_dr03= new TH1F("h_ele_hcalDepth1TowerSumEt_dr03","hcal depth1 isolation deposit, dR=0.3",100,0.0,20.);
  h_ele_hcalDepth2TowerSumEt_dr03= new TH1F("h_ele_hcalDepth2TowerSumEt_dr03","hcal depth2 isolation deposit, dR=0.3",100,0.0,20.);
  h_ele_tkSumPt_dr04= new TH1F("h_ele_tkSumPt_dr04","tk isolation deposit, dR=0.4",100,0.0,20.);
  h_ele_ecalRecHitSumEt_dr04= new TH1F("h_ele_ecalRecHitSumEt_dr04","ecal isolation deposit, dR=0.4",100,0.0,20.);
  h_ele_hcalDepth1TowerSumEt_dr04= new TH1F("h_ele_hcalDepth1TowerSumEt_dr04","hcal depth1 isolation deposit, dR=0.4",100,0.0,20.);
  h_ele_hcalDepth2TowerSumEt_dr04= new TH1F("h_ele_hcalDepth2TowerSumEt_dr04","hcal depth2 isolation deposit, dR=0.4",100,0.0,20.);
  
  // fbrem
  h_ele_fbrem = new TH1F( "h_ele_fbrem","ele brem fraction, mode",50,0.,1.);
  h_ele_fbrem_eg = new TH1F( "h_ele_fbrem_eg","ele brem fraction, mode, ecal driven",50,0.,1.);
  h_ele_fbremVsEta_mode = new TProfile( "h_ele_fbremvsEtamode","mean ele brem fraction vs eta, mode",nbineta2D,etamin,etamax,0.,1.);
  h_ele_fbremVsEta_mean = new TProfile( "h_ele_fbremvsEtamean","mean ele brem fraction vs eta, mean",nbineta2D,etamin,etamax,0.,1.);

  // e/g et pflow electrons 
  h_ele_mva = new TH1F( "h_ele_mva","ele identification mva",50,-1.,1.);
  h_ele_mva_eg = new TH1F( "h_ele_mva_eg","ele identification mva, ecal driven",50,-1.,1.);
  h_ele_provenance = new TH1F( "h_ele_provenance","ele provenance",5,-2.,3.);
  
  // histos titles
  h_mcNum              -> GetXaxis()-> SetTitle("N_{gen}");
  h_mcNum              -> GetYaxis()-> SetTitle("Events");
  h_eleNum             -> GetXaxis()-> SetTitle("# gen ele");
  h_eleNum             -> GetYaxis()-> SetTitle("Events");
  h_gamNum             -> GetXaxis()-> SetTitle("N_{gen #gamma}");
  h_gamNum             -> GetYaxis()-> SetTitle("Events");
  h_simEta             -> GetXaxis()-> SetTitle("#eta");
  h_simEta             -> GetYaxis()-> SetTitle("Events");
  h_simP               -> GetXaxis()-> SetTitle("p (GeV/c)");
  h_simP               -> GetYaxis()-> SetTitle("Events");
  h_ele_foundHits      -> GetXaxis()-> SetTitle("N_{hits}");
  h_ele_foundHits      -> GetYaxis()-> SetTitle("Events");
  h_ele_ambiguousTracks      -> GetXaxis()-> SetTitle("N_{ambiguous tracks}");
  h_ele_ambiguousTracks      -> GetYaxis()-> SetTitle("Events");
  h_ele_lostHits       -> GetXaxis()-> SetTitle("N_{lost hits}");
  h_ele_lostHits       -> GetYaxis()-> SetTitle("Events");
  h_ele_chi2           -> GetXaxis()-> SetTitle("#Chi^{2}");
  h_ele_chi2           -> GetYaxis()-> SetTitle("Events");
  h_ele_charge         -> GetXaxis()-> SetTitle("charge");
  h_ele_charge         -> GetYaxis()-> SetTitle("Events");
  h_ele_vertexP        -> GetXaxis()-> SetTitle("p_{vertex} (GeV/c)");
  h_ele_vertexP        -> GetYaxis()-> SetTitle("Events");
  h_ele_vertexPt       -> GetXaxis()-> SetTitle("p_{T vertex} (GeV/c)");
  h_ele_vertexPt       -> GetYaxis()-> SetTitle("Events");
  h_ele_vertexEta      -> GetXaxis()-> SetTitle("#eta");
  h_ele_vertexEta      -> GetYaxis()-> SetTitle("Events");
  h_ele_vertexPhi      -> GetXaxis()-> SetTitle("#phi (rad)");
  h_ele_vertexPhi      -> GetYaxis()-> SetTitle("Events");
  h_ele_PoPtrue        -> GetXaxis()-> SetTitle("P/P_{gen}");
  h_ele_PoPtrue        -> GetYaxis()-> SetTitle("Events");
  h_ele_PoPtrue_barrel        -> GetXaxis()-> SetTitle("P/P_{gen}");
  h_ele_PoPtrue_barrel        -> GetYaxis()-> SetTitle("Events");
  h_ele_PoPtrue_endcaps        -> GetXaxis()-> SetTitle("P/P_{gen}");
  h_ele_PoPtrue_endcaps        -> GetYaxis()-> SetTitle("Events");
  histSclEoEtrue_barrel -> GetXaxis()-> SetTitle("E/E_{gen}") ;
  histSclEoEtrue_barrel -> GetYaxis()-> SetTitle("Events") ;
  histSclEoEtrue_endcaps -> GetXaxis()-> SetTitle("E/E_{gen}") ;
  histSclEoEtrue_endcaps -> GetYaxis()-> SetTitle("Events") ;
  histSclEoEtrue_barrel_etagap -> GetXaxis()-> SetTitle("E/E_{gen}");
  histSclEoEtrue_barrel_etagap -> GetYaxis()-> SetTitle("Events");
  histSclEoEtrue_barrel_phigap -> GetXaxis()-> SetTitle("E/E_{gen}");
  histSclEoEtrue_barrel_phigap -> GetYaxis()-> SetTitle("Events");
  histSclEoEtrue_ebeegap -> GetXaxis()-> SetTitle("E/E_{gen}");
  histSclEoEtrue_ebeegap -> GetYaxis()-> SetTitle("Events");
  histSclEoEtrue_endcaps_deegap -> GetXaxis()-> SetTitle("E/E_{gen}");
  histSclEoEtrue_endcaps_deegap -> GetYaxis()-> SetTitle("Events");
  histSclEoEtrue_endcaps_ringgap -> GetXaxis()-> SetTitle("E/E_{gen}");
  histSclEoEtrue_endcaps_ringgap -> GetYaxis()-> SetTitle("Events");
  histSclSigEtaEta_-> GetXaxis()-> SetTitle("#sigma_{#eta #eta}") ;
  histSclSigEtaEta_-> GetYaxis()-> SetTitle("Events") ;
  histSclSigIEtaIEtabarrel_-> GetXaxis()-> SetTitle("#sigma_{i#eta i#eta}") ;
  histSclSigIEtaIEtabarrel_-> GetYaxis()-> SetTitle("Events") ;
  histSclSigIEtaIEtaendcaps_-> GetXaxis()-> SetTitle("#sigma_{i#eta i#eta}") ;
  histSclSigIEtaIEtaendcaps_-> GetYaxis()-> SetTitle("Events") ;
  histSclE1x5_-> GetXaxis()-> SetTitle("E1x5 (GeV)") ;
  histSclE1x5_-> GetYaxis()-> SetTitle("Events") ;
  histSclE2x5max_-> GetXaxis()-> SetTitle("E2x5 (GeV)") ;
  histSclE2x5max_-> GetYaxis()-> SetTitle("Events") ;
  histSclE5x5_-> GetXaxis()-> SetTitle("E5x5 (GeV)") ;
  histSclE5x5_-> GetYaxis()-> SetTitle("Events") ;
  histSclEoEtruePfVsEg->GetXaxis()->SetTitle("E/E_{gen} (e/g)") ;
  histSclEoEtruePfVsEg->GetYaxis()->SetTitle("E/E_{gen} (pflow)") ;
  h_ele_ChargeMnChargeTrue   -> GetXaxis()-> SetTitle("q_{rec} - q_{gen}");
  h_ele_ChargeMnChargeTrue   -> GetYaxis()-> SetTitle("Events");
  h_ele_EtaMnEtaTrue   -> GetXaxis()-> SetTitle("#eta_{rec} - #eta_{gen}");
  h_ele_EtaMnEtaTrue   -> GetYaxis()-> SetTitle("Events");
  h_ele_PhiMnPhiTrue   -> GetXaxis()-> SetTitle("#phi_{rec} - #phi_{gen} (rad)");
  h_ele_PhiMnPhiTrue   -> GetYaxis()-> SetTitle("Events");
  h_ele_PinMnPout      -> GetXaxis()-> SetTitle("P_{vertex} - P_{out} (GeV/c)");
  h_ele_PinMnPout      -> GetYaxis()-> SetTitle("Events");
  h_ele_PinMnPout_mode      -> GetXaxis()-> SetTitle("P_{vertex} - P_{out}, mode (GeV/c)");
  h_ele_PinMnPout_mode      -> GetYaxis()-> SetTitle("Events");
  h_ele_outerP         -> GetXaxis()-> SetTitle("P_{out} (GeV/c)");
  h_ele_outerP         -> GetYaxis()-> SetTitle("Events");
  h_ele_outerP_mode         -> GetXaxis()-> SetTitle("P_{out} (GeV/c)");
  h_ele_outerP_mode         -> GetYaxis()-> SetTitle("Events");
  h_ele_outerPt        -> GetXaxis()-> SetTitle("P_{T out} (GeV/c)");
  h_ele_outerPt        -> GetYaxis()-> SetTitle("Events");
  h_ele_outerPt_mode        -> GetXaxis()-> SetTitle("P_{T out} (GeV/c)");
  h_ele_outerPt_mode        -> GetYaxis()-> SetTitle("Events");
  h_ele_EoP            -> GetXaxis()-> SetTitle("E/P_{vertex}");
  h_ele_EoP            -> GetYaxis()-> SetTitle("Events");
  h_ele_EseedOP            -> GetXaxis()-> SetTitle("E_{seed}/P_{vertex}");
  h_ele_EseedOP            -> GetYaxis()-> SetTitle("Events");
  h_ele_EoPout         -> GetXaxis()-> SetTitle("E_{seed}/P_{out}");
  h_ele_EoPout         -> GetYaxis()-> SetTitle("Events");
  h_ele_EeleOPout         -> GetXaxis()-> SetTitle("E_{ele}/P_{out}");
  h_ele_EeleOPout         -> GetYaxis()-> SetTitle("Events");
  h_ele_vertexX-> GetXaxis()-> SetTitle("x (cm)");
  h_ele_vertexX-> GetYaxis()-> SetTitle("Events");
  h_ele_vertexY-> GetXaxis()-> SetTitle("y (cm)");
  h_ele_vertexY-> GetYaxis()-> SetTitle("Events");
  h_ele_vertexZ-> GetXaxis()-> SetTitle("z (cm)");
  h_ele_vertexZ-> GetYaxis()-> SetTitle("Events");
  h_ele_vertexTIP-> GetXaxis()-> SetTitle("TIP (cm)");
  h_ele_vertexTIP-> GetYaxis()-> SetTitle("Events");
  h_ele_TIP_all-> GetXaxis()-> SetTitle("r_{T} (cm)");
  h_ele_TIP_all-> GetYaxis()-> SetTitle("Events");
  h_ele_vertexTIPVsEta-> GetYaxis()-> SetTitle("TIP (cm)");
  h_ele_vertexTIPVsEta-> GetXaxis()-> SetTitle("#eta");
  h_ele_vertexTIPVsPhi-> GetYaxis()-> SetTitle("TIP (cm)");
  h_ele_vertexTIPVsPhi-> GetXaxis()-> SetTitle("#phi (rad)");
  h_ele_vertexTIPVsPt-> GetYaxis()-> SetTitle("TIP (cm)");
  h_ele_vertexTIPVsEta-> GetXaxis()-> SetTitle("p_{T} (GeV/c)");
  h_ele_dEtaSc_propVtx-> GetXaxis()-> SetTitle("#eta_{sc} - #eta_{tr}");
  h_ele_dEtaSc_propVtx-> GetYaxis()-> SetTitle("Events");
  h_ele_dEtaCl_propOut-> GetXaxis()-> SetTitle("#eta_{seedcl} - #eta_{tr}");
  h_ele_dEtaCl_propOut-> GetYaxis()-> SetTitle("Events");
  h_ele_dEtaEleCl_propOut-> GetXaxis()-> SetTitle("#eta_{elecl} - #eta_{tr}");
  h_ele_dEtaEleCl_propOut-> GetYaxis()-> SetTitle("Events");
  h_ele_dPhiSc_propVtx-> GetXaxis()-> SetTitle("#phi_{sc} - #phi_{tr} (rad)");
  h_ele_dPhiSc_propVtx-> GetYaxis()-> SetTitle("Events");
  h_ele_dPhiCl_propOut-> GetXaxis()-> SetTitle("#phi_{seedcl} - #phi_{tr} (rad)");
  h_ele_dPhiCl_propOut-> GetYaxis()-> SetTitle("Events");
  h_ele_dPhiEleCl_propOut-> GetXaxis()-> SetTitle("#phi_{elecl} - #phi_{tr} (rad)");
  h_ele_dPhiEleCl_propOut-> GetYaxis()-> SetTitle("Events");
  h_ele_HoE-> GetXaxis()-> SetTitle("H/E") ;
  h_ele_HoE-> GetYaxis()-> SetTitle("Events") ;
  h_ele_HoE_fiducial-> GetXaxis()-> SetTitle("H/E") ;
  h_ele_HoE_fiducial-> GetYaxis()-> SetTitle("Events") ;
  h_ele_fbrem-> GetXaxis()-> SetTitle("P_{in} - P_{out} / P_{in}");
  h_ele_fbrem-> GetYaxis()-> SetTitle("Events");
  h_ele_seed_dphi2_-> GetXaxis()-> SetTitle("#phi_{hit}-#phi_{pred} (rad)") ;
  h_ele_seed_dphi2_-> GetYaxis()-> SetTitle("Events") ;
  h_ele_seed_drz2_-> GetXaxis()-> SetTitle("r(z)_{hit}-r(z)_{pred} (cm)") ;
  h_ele_seed_drz2_-> GetYaxis()-> SetTitle("Events") ;
  h_ele_seed_subdet2_-> GetXaxis()-> SetTitle("2nd hit subdet Id") ;
  h_ele_seed_subdet2_-> GetYaxis()-> SetTitle("Events") ;
  h_ele_classes-> GetXaxis()-> SetTitle("class Id") ;
  h_ele_classes-> GetYaxis()-> SetTitle("Events") ;
  h_ele_EoverP_all-> GetXaxis()-> SetTitle("E/P_{vertex}");
  h_ele_EoverP_all-> GetYaxis()-> SetTitle("Events");
  h_ele_EseedOP_all-> GetXaxis()-> SetTitle("E_{seed}/P_{vertex}");
  h_ele_EseedOP_all-> GetYaxis()-> SetTitle("Events");
  h_ele_EoPout_all -> GetXaxis()-> SetTitle("E_{seed}/P_{out}"); 
  h_ele_EoPout_all-> GetYaxis()-> SetTitle("Events");  
  h_ele_EeleOPout_all-> GetXaxis()-> SetTitle("E_{ele}/P_{out}");
  h_ele_EeleOPout_all-> GetYaxis()-> SetTitle("Events");
  h_ele_dEtaSc_propVtx_all-> GetXaxis()-> SetTitle("#eta_{sc} - #eta_{tr}");
  h_ele_dEtaSc_propVtx_all-> GetYaxis()-> SetTitle("Events");
  h_ele_dPhiSc_propVtx_all-> GetXaxis()-> SetTitle("#phi_{sc} - #phi_{tr} (rad)");
  h_ele_dPhiSc_propVtx_all-> GetYaxis()-> SetTitle("Events");
  h_ele_dEtaCl_propOut_all-> GetXaxis()-> SetTitle("#eta_{sc} - #eta_{tr}");
  h_ele_dEtaCl_propOut_all-> GetYaxis()-> SetTitle("Events");
  h_ele_dPhiCl_propOut_all-> GetXaxis()-> SetTitle("#phi_{sc} - #phi_{tr} (rad)");
  h_ele_dPhiCl_propOut_all-> GetYaxis()-> SetTitle("Events");
  h_ele_HoE_all-> GetXaxis()-> SetTitle("H/E") ;
  h_ele_HoE_all-> GetYaxis()-> SetTitle("Events");
  h_ele_mee_all-> GetXaxis()-> SetTitle("m_{ee} (GeV/c^{2})");
  h_ele_mee_all-> GetYaxis()-> SetTitle("Events");
  histNum_-> GetXaxis()-> SetTitle("N_{ele}");
  histNum_-> GetYaxis()-> SetTitle("Events");
  h_ele_fbremVsEta_mode-> GetXaxis()-> SetTitle("#eta");
  h_ele_fbremVsEta_mean-> GetXaxis()-> SetTitle("#eta");

}

void
GsfElectronMCAnalyzer::endJob(){

  histfile_->cd();

  std::cout << "[GsfElectronMCAnalyzer] efficiency calculation " << std::endl;
  // efficiency vs eta
  TH1F *h_ele_etaEff = (TH1F*)h_ele_simEta_matched->Clone("h_ele_etaEff");
  h_ele_etaEff->Reset();
  h_ele_etaEff->Divide(h_ele_simEta_matched,h_simEta,1,1,"b");
  h_ele_etaEff->Print();
  h_ele_etaEff->GetXaxis()->SetTitle("#eta");
  h_ele_etaEff->GetYaxis()->SetTitle("Efficiency");

  // efficiency vs z
  TH1F *h_ele_zEff = (TH1F*)h_ele_simZ_matched->Clone("h_ele_zEff");
  h_ele_zEff->Reset();
  h_ele_zEff->Divide(h_ele_simZ_matched,h_simZ,1,1,"b");
  h_ele_zEff->Print();
  h_ele_zEff->GetXaxis()->SetTitle("z (cm)");
  h_ele_zEff->GetYaxis()->SetTitle("Efficiency");

  // efficiency vs |eta|
  TH1F *h_ele_absetaEff = (TH1F*)h_ele_simAbsEta_matched->Clone("h_ele_absetaEff");
  h_ele_absetaEff->Reset();
  h_ele_absetaEff->Divide(h_ele_simAbsEta_matched,h_simAbsEta,1,1,"b");
  h_ele_absetaEff->GetXaxis()->SetTitle("|#eta|");
  h_ele_absetaEff->GetYaxis()->SetTitle("Efficiency");

  // efficiency vs pt
  TH1F *h_ele_ptEff = (TH1F*)h_ele_simPt_matched->Clone("h_ele_ptEff");
  h_ele_ptEff->Reset();
  h_ele_ptEff->Divide(h_ele_simPt_matched,h_simPt,1,1,"b");
  h_ele_ptEff->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  h_ele_ptEff->GetYaxis()->SetTitle("Efficiency");

  // efficiency vs phi
  TH1F *h_ele_phiEff = (TH1F*)h_ele_simPhi_matched->Clone("h_ele_phiEff");
  h_ele_phiEff->Reset();
  h_ele_phiEff->Divide(h_ele_simPhi_matched,h_simPhi,1,1,"b");
  h_ele_phiEff->GetXaxis()->SetTitle("#phi (rad)");
  h_ele_phiEff->GetYaxis()->SetTitle("Efficiency");

  std::cout << "[GsfElectronMCAnalyzer] q-misid calculation " << std::endl;
  // misid vs eta
  TH1F *h_ele_etaQmisid = (TH1F*)h_ele_simEta_matched_qmisid->Clone("h_ele_etaQmisid");
  h_ele_etaQmisid->Reset();
  h_ele_etaQmisid->Divide(h_ele_simEta_matched_qmisid,h_simEta,1,1,"b");
  h_ele_etaQmisid->Print();
  h_ele_etaQmisid->GetXaxis()->SetTitle("#eta");
  h_ele_etaQmisid->GetYaxis()->SetTitle("q misId");

  // misid vs z
  TH1F *h_ele_zQmisid = (TH1F*)h_ele_simZ_matched_qmisid->Clone("h_ele_zQmisid");
  h_ele_zQmisid->Reset();
  h_ele_zQmisid->Divide(h_ele_simZ_matched_qmisid,h_simZ,1,1,"b");
  h_ele_zQmisid->Print();
  h_ele_zQmisid->GetXaxis()->SetTitle("z (cm)");
  h_ele_zQmisid->GetYaxis()->SetTitle("q misId");

  // misid vs |eta|
  TH1F *h_ele_absetaQmisid = (TH1F*)h_ele_simAbsEta_matched_qmisid->Clone("h_ele_absetaQmisid");
  h_ele_absetaQmisid->Reset();
  h_ele_absetaQmisid->Divide(h_ele_simAbsEta_matched_qmisid,h_simAbsEta,1,1,"b");
  h_ele_absetaQmisid->GetXaxis()->SetTitle("|#eta|");
  h_ele_absetaQmisid->GetYaxis()->SetTitle("q misId");

  // misid vs pt
  TH1F *h_ele_ptQmisid = (TH1F*)h_ele_simPt_matched_qmisid->Clone("h_ele_ptQmisid");
  h_ele_ptQmisid->Reset();
  h_ele_ptQmisid->Divide(h_ele_simPt_matched_qmisid,h_simPt,1,1,"b");
  h_ele_ptQmisid->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  h_ele_ptQmisid->GetYaxis()->SetTitle("q misId");

  std::cout << "[GsfElectronMCAnalyzer] all reco electrons " << std::endl;
  // rec/gen all electrons
  TH1F *h_ele_etaEff_all = (TH1F*)h_ele_vertexEta_all->Clone("h_ele_etaEff_all");
  h_ele_etaEff_all->Reset();
  h_ele_etaEff_all->Divide(h_ele_vertexEta_all,h_simEta,1,1,"b");
  h_ele_etaEff_all->Print();
  h_ele_etaEff_all->GetXaxis()->SetTitle("#eta");
  h_ele_etaEff_all->GetYaxis()->SetTitle("N_{rec}/N_{gen}");
  TH1F *h_ele_ptEff_all = (TH1F*)h_ele_vertexPt_all->Clone("h_ele_ptEff_all");
  h_ele_ptEff_all->Reset();
  h_ele_ptEff_all->Divide(h_ele_vertexPt_all,h_simPt,1,1,"b");
  h_ele_ptEff_all->Print();
  h_ele_ptEff_all->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  h_ele_ptEff_all->GetYaxis()->SetTitle("N_{rec}/N_{gen}");

  // classes
  TH1F *h_ele_eta_goldenFrac = (TH1F*)h_ele_eta_golden->Clone("h_ele_eta_goldenFrac");
  h_ele_eta_goldenFrac->Reset();
  h_ele_eta_goldenFrac->Divide(h_ele_eta_golden,h_ele_eta,1,1,"b");
  h_ele_eta_goldenFrac->GetXaxis()->SetTitle("|#eta|");
  h_ele_eta_goldenFrac->GetYaxis()->SetTitle("Fraction of electrons");
  h_ele_eta_goldenFrac->SetTitle("fraction of golden electrons vs eta");
  TH1F *h_ele_eta_bbremFrac = (TH1F*)h_ele_eta_bbrem->Clone("h_ele_eta_bbremFrac");
  h_ele_eta_bbremFrac->Reset();
  h_ele_eta_bbremFrac->GetXaxis()->SetTitle("|#eta|");
  h_ele_eta_bbremFrac->GetYaxis()->SetTitle("Fraction of electrons");
  h_ele_eta_bbremFrac->Divide(h_ele_eta_bbrem,h_ele_eta,1,1,"b");
  h_ele_eta_bbremFrac->SetTitle("fraction of big brem electrons vs eta");
  TH1F *h_ele_eta_narrowFrac = (TH1F*)h_ele_eta_narrow->Clone("h_ele_eta_narrowFrac");
  h_ele_eta_narrowFrac->Reset();
  h_ele_eta_narrowFrac->Divide(h_ele_eta_narrow,h_ele_eta,1,1,"b");
  h_ele_eta_narrowFrac->GetXaxis()->SetTitle("|#eta|");
  h_ele_eta_narrowFrac->GetYaxis()->SetTitle("Fraction of electrons");
  h_ele_eta_narrowFrac->SetTitle("fraction of narrow electrons vs eta");
  TH1F *h_ele_eta_showerFrac = (TH1F*)h_ele_eta_shower->Clone("h_ele_eta_showerFrac");
  h_ele_eta_showerFrac->Reset();
  h_ele_eta_showerFrac->Divide(h_ele_eta_shower,h_ele_eta,1,1,"b");
  h_ele_eta_showerFrac->GetXaxis()->SetTitle("|#eta|");
  h_ele_eta_showerFrac->GetYaxis()->SetTitle("Fraction of electrons");
  h_ele_eta_showerFrac->SetTitle("fraction of showering electrons vs eta");

  // fbrem
  TH1F *h_ele_xOverX0VsEta = new TH1F( "h_ele_xOverx0VsEta","mean X/X_0 vs eta",nbineta/2,0.0,2.5);
  for (int ibin=1;ibin<h_ele_fbremVsEta_mean->GetNbinsX()+1;ibin++) {
    double xOverX0 = 0.;
    if (h_ele_fbremVsEta_mean->GetBinContent(ibin)>0.) xOverX0 = -log(h_ele_fbremVsEta_mean->GetBinContent(ibin));
    h_ele_xOverX0VsEta->SetBinContent(ibin,xOverX0);
  }

  //profiles from 2D histos
  TProfile *p_ele_PoPtrueVsEta = h_ele_PoPtrueVsEta->ProfileX();
  p_ele_PoPtrueVsEta->SetTitle("mean ele momentum / gen momentum vs eta");
  p_ele_PoPtrueVsEta->GetXaxis()->SetTitle("#eta");
  p_ele_PoPtrueVsEta->GetYaxis()->SetTitle("<P/P_{gen}>");
  p_ele_PoPtrueVsEta->Write();
  TProfile *p_ele_PoPtrueVsPhi = h_ele_PoPtrueVsPhi->ProfileX();
  p_ele_PoPtrueVsPhi->SetTitle("mean ele momentum / gen momentum vs phi");
  p_ele_PoPtrueVsPhi->GetXaxis()->SetTitle("#phi (rad)");
  p_ele_PoPtrueVsPhi->GetYaxis()->SetTitle("<P/P_{gen}>");
  p_ele_PoPtrueVsPhi->Write();
  TProfile *p_ele_EoEtruePfVsEg_x = histSclEoEtruePfVsEg->ProfileX();
  p_ele_EoEtruePfVsEg_x->SetTitle("mean pflow sc energy / true energy vs e/g sc energy");
  p_ele_EoEtruePfVsEg_x->GetXaxis()->SetTitle("E/E_{gen} (e/g)") ;
  p_ele_EoEtruePfVsEg_x->GetYaxis()->SetTitle("<E/E_{gen}> (pflow)") ;
  p_ele_EoEtruePfVsEg_x->Write();
  TProfile *p_ele_EoEtruePfVsEg_y = histSclEoEtruePfVsEg->ProfileY();
  p_ele_EoEtruePfVsEg_y->SetTitle("mean e/g sc energy / true energy vs pflow sc energy");
  p_ele_EoEtruePfVsEg_y->GetXaxis()->SetTitle("E/E_{gen} (pflow)") ;
  p_ele_EoEtruePfVsEg_y->GetYaxis()->SetTitle("<E/E_{gen}> (eg)") ;
  p_ele_EoEtruePfVsEg_y->Write();
  TProfile *p_ele_EtaMnEtaTrueVsEta = h_ele_EtaMnEtaTrueVsEta->ProfileX();
  p_ele_EtaMnEtaTrueVsEta->SetTitle("mean ele eta - gen eta vs eta");
  p_ele_EtaMnEtaTrueVsEta->GetXaxis()->SetTitle("#eta");
  p_ele_EtaMnEtaTrueVsEta->GetYaxis()->SetTitle("<#eta_{rec} - #eta_{gen}>");
  p_ele_EtaMnEtaTrueVsEta->Write();
  TProfile *p_ele_EtaMnEtaTrueVsPhi = h_ele_EtaMnEtaTrueVsPhi->ProfileX();
  p_ele_EtaMnEtaTrueVsPhi->SetTitle("mean ele eta - gen eta vs phi");
  p_ele_EtaMnEtaTrueVsPhi->GetXaxis()->SetTitle("#phi (rad)");
  p_ele_EtaMnEtaTrueVsPhi->GetYaxis()->SetTitle("<#eta_{rec} - #eta_{gen}>");
  p_ele_EtaMnEtaTrueVsPhi->Write();
  TProfile *p_ele_PhiMnPhiTrueVsEta = h_ele_PhiMnPhiTrueVsEta->ProfileX();
  p_ele_PhiMnPhiTrueVsEta->SetTitle("mean ele phi - gen phi vs eta");
  p_ele_PhiMnPhiTrueVsEta->GetXaxis()->SetTitle("#eta");
  p_ele_PhiMnPhiTrueVsEta->GetYaxis()->SetTitle("<#phi_{rec} - #phi_{gen}> (rad)");
  p_ele_PhiMnPhiTrueVsEta->Write();
  TProfile *p_ele_PhiMnPhiTrueVsPhi = h_ele_PhiMnPhiTrueVsPhi->ProfileX();
  p_ele_PhiMnPhiTrueVsPhi->SetTitle("mean ele phi - gen phi vs phi");
  p_ele_PhiMnPhiTrueVsPhi->GetXaxis()->SetTitle("#phi (rad)");
  p_ele_PhiMnPhiTrueVsPhi->Write();
  TProfile *p_ele_vertexPtVsEta = h_ele_vertexPtVsEta->ProfileX();
  p_ele_vertexPtVsEta->SetTitle("mean ele transverse momentum vs eta");
  p_ele_vertexPtVsEta->GetXaxis()->SetTitle("#eta");
  p_ele_vertexPtVsEta->GetYaxis()->SetTitle("<p_{T}> (GeV/c)");
  p_ele_vertexPtVsEta->Write();
  TProfile *p_ele_vertexPtVsPhi = h_ele_vertexPtVsPhi->ProfileX();
  p_ele_vertexPtVsPhi->SetTitle("mean ele transverse momentum vs phi");
  p_ele_vertexPtVsPhi->GetXaxis()->SetTitle("#phi (rad)");
  p_ele_vertexPtVsPhi->GetYaxis()->SetTitle("<p_{T}> (GeV/c)");
  p_ele_vertexPtVsPhi->Write();
  TProfile *p_ele_EoPVsEta = h_ele_EoPVsEta->ProfileX();
  p_ele_EoPVsEta->SetTitle("mean ele E/p vs eta");
  p_ele_EoPVsEta->GetXaxis()->SetTitle("#eta");
  p_ele_EoPVsEta->GetYaxis()->SetTitle("<E/P_{vertex}>");
  p_ele_EoPVsEta->Write();
  TProfile *p_ele_EoPVsPhi = h_ele_EoPVsPhi->ProfileX();
  p_ele_EoPVsPhi->SetTitle("mean ele E/p vs phi");
  p_ele_EoPVsPhi->GetXaxis()->SetTitle("#phi (rad)");
  p_ele_EoPVsPhi->GetYaxis()->SetTitle("<E/P_{vertex}>");
  p_ele_EoPVsPhi->Write();
  TProfile *p_ele_EoPoutVsEta = h_ele_EoPoutVsEta->ProfileX();
  p_ele_EoPoutVsEta->SetTitle("mean ele E/pout vs eta");
  p_ele_EoPoutVsEta->GetXaxis()->SetTitle("#eta");
  p_ele_EoPoutVsEta->GetYaxis()->SetTitle("<E_{seed}/P_{out}>");
  p_ele_EoPoutVsEta->Write();
  TProfile *p_ele_EoPoutVsPhi = h_ele_EoPoutVsPhi->ProfileX();
  p_ele_EoPoutVsPhi->SetTitle("mean ele E/pout vs phi");
  p_ele_EoPoutVsPhi->GetXaxis()->SetTitle("#phi (rad)");
  p_ele_EoPoutVsPhi->GetYaxis()->SetTitle("<E_{seed}/P_{out}>");
  p_ele_EoPoutVsPhi->Write();
  TProfile *p_ele_EeleOPoutVsEta = h_ele_EeleOPoutVsEta->ProfileX();
  p_ele_EeleOPoutVsEta->SetTitle("mean ele Eele/pout vs eta");
  p_ele_EeleOPoutVsEta->GetXaxis()->SetTitle("#eta");
  p_ele_EeleOPoutVsEta->GetYaxis()->SetTitle("<E_{ele}/P_{out}>");
  p_ele_EeleOPoutVsEta->Write();
  TProfile *p_ele_EeleOPoutVsPhi = h_ele_EeleOPoutVsPhi->ProfileX();
  p_ele_EeleOPoutVsPhi->SetTitle("mean ele Eele/pout vs phi");
  p_ele_EeleOPoutVsPhi->GetXaxis()->SetTitle("#phi (rad)");
  p_ele_EeleOPoutVsPhi->GetYaxis()->SetTitle("<E_{ele}/P_{out}>");
  p_ele_EeleOPoutVsPhi->Write();
  TProfile *p_ele_HoEVsEta = h_ele_HoEVsEta->ProfileX();
  p_ele_HoEVsEta->SetTitle("mean ele H/E vs eta");
  p_ele_HoEVsEta->GetXaxis()->SetTitle("#eta");
  p_ele_HoEVsEta->GetYaxis()->SetTitle("<H/E>");
  p_ele_HoEVsEta->Write();
  TProfile *p_ele_HoEVsPhi = h_ele_HoEVsPhi->ProfileX();
  p_ele_HoEVsPhi->SetTitle("mean ele H/E vs phi");
  p_ele_HoEVsPhi->GetXaxis()->SetTitle("#phi (rad)");
  p_ele_HoEVsPhi->GetYaxis()->SetTitle("<H/E>");
  p_ele_HoEVsPhi->Write();
  TProfile *p_ele_chi2VsEta = h_ele_chi2VsEta->ProfileX();
  p_ele_chi2VsEta->SetTitle("mean ele track chi2 vs eta");
  p_ele_chi2VsEta->GetXaxis()->SetTitle("#eta");
  p_ele_chi2VsEta->GetYaxis()->SetTitle("<#Chi^{2}>");
  p_ele_chi2VsEta->Write();
  TProfile *p_ele_chi2VsPhi = h_ele_chi2VsPhi->ProfileX();
  p_ele_chi2VsPhi->SetTitle("mean ele track chi2 vs phi");
  p_ele_chi2VsPhi->GetXaxis()->SetTitle("#phi (rad)");
  p_ele_chi2VsPhi->GetYaxis()->SetTitle("<#Chi^{2}>");
  p_ele_chi2VsPhi->Write();
  TProfile *p_ele_foundHitsVsEta = h_ele_foundHitsVsEta->ProfileX();
  p_ele_foundHitsVsEta->SetTitle("mean ele track # found hits vs eta");
  p_ele_foundHitsVsEta->GetXaxis()->SetTitle("#eta");
  p_ele_foundHitsVsEta->GetYaxis()->SetTitle("<N_{hits}>");
  p_ele_foundHitsVsEta->Write();
  TProfile *p_ele_foundHitsVsPhi = h_ele_foundHitsVsPhi->ProfileX();
  p_ele_foundHitsVsPhi->SetTitle("mean ele track # found hits vs phi");
  p_ele_foundHitsVsPhi->GetXaxis()->SetTitle("#phi (rad)");
  p_ele_foundHitsVsPhi->GetYaxis()->SetTitle("<N_{hits}>");
  p_ele_foundHitsVsPhi->Write();
  TProfile *p_ele_lostHitsVsEta = h_ele_lostHitsVsEta->ProfileX();
  p_ele_lostHitsVsEta->SetTitle("mean ele track # lost hits vs eta");
  p_ele_lostHitsVsEta->GetXaxis()->SetTitle("#eta");
  p_ele_lostHitsVsEta->GetYaxis()->SetTitle("<N_{hits}>");
  p_ele_lostHitsVsEta->Write();
  TProfile *p_ele_lostHitsVsPhi = h_ele_lostHitsVsPhi->ProfileX();
  p_ele_lostHitsVsPhi->SetTitle("mean ele track # lost hits vs phi");
  p_ele_lostHitsVsPhi->GetXaxis()->SetTitle("#phi (rad)");
  p_ele_lostHitsVsPhi->GetYaxis()->SetTitle("<N_{hits}>");
  p_ele_lostHitsVsPhi->Write();
  TProfile *p_ele_vertexTIPVsEta = h_ele_vertexTIPVsEta->ProfileX();
  p_ele_vertexTIPVsEta->SetTitle("mean tip (wrt gen vtx) vs eta");
  p_ele_vertexTIPVsEta->GetXaxis()->SetTitle("#eta");
  p_ele_vertexTIPVsEta->GetYaxis()->SetTitle("<TIP> (cm)");
  p_ele_vertexTIPVsEta->Write();
  TProfile *p_ele_vertexTIPVsPhi = h_ele_vertexTIPVsPhi->ProfileX();
  p_ele_vertexTIPVsPhi->SetTitle("mean tip (wrt gen vtx) vs phi");
  p_ele_vertexTIPVsPhi->GetXaxis()->SetTitle("#phi");
  p_ele_vertexTIPVsPhi->GetYaxis()->SetTitle("<TIP> (cm)");
  p_ele_vertexTIPVsPhi->Write();
  TProfile *p_ele_vertexTIPVsPt = h_ele_vertexTIPVsPt->ProfileX();
  p_ele_vertexTIPVsPt->SetTitle("mean tip (wrt gen vtx) vs phi");
  p_ele_vertexTIPVsPt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  p_ele_vertexTIPVsPt->GetYaxis()->SetTitle("<TIP> (cm)");
  p_ele_vertexTIPVsPt->Write();

  // mc truth
  h_mcNum->Write();
  h_eleNum->Write();
  h_gamNum->Write();

  // rec event
  histNum_->Write();

  // mc
  h_simEta->Write();
  h_simAbsEta->Write();
  h_simP->Write();
  h_simPt->Write();

  // all electrons
  h_ele_EoverP_all->Write();
  h_ele_EseedOP_all->Write(); 
  h_ele_EoPout_all->Write();    
  h_ele_EeleOPout_all ->Write();
  h_ele_dEtaSc_propVtx_all->Write(); 
  h_ele_dPhiSc_propVtx_all->Write(); 
  h_ele_dEtaCl_propOut_all ->Write();
  h_ele_dPhiCl_propOut_all->Write(); 
  h_ele_HoE_all->Write();
  h_ele_TIP_all->Write();
  h_ele_vertexPt_all->Write();
  h_ele_vertexEta_all->Write();
  h_ele_mee_all->Write();

  // charge ID
  h_ele_charge->Write();

  // matched electrons
  h_ele_vertexP->Write();
  h_ele_vertexPt->Write();
  h_ele_vertexPtVsEta->Write();
  h_ele_vertexPtVsPhi->Write();
  h_ele_simPt_matched->Write();
  h_ele_vertexEta->Write();
  h_ele_vertexEtaVsPhi->Write();
  h_ele_simAbsEta_matched->Write();
  h_ele_simEta_matched->Write();
  h_ele_vertexPhi->Write();
  h_ele_vertexX->Write();
  h_ele_vertexY ->Write();
  h_ele_vertexZ->Write();
  h_ele_vertexTIP->Write();
  h_ele_simZ_matched->Write();
  h_ele_vertexTIPVsEta->Write();
  h_ele_vertexTIPVsPhi->Write();
  h_ele_vertexTIPVsPt->Write();
  h_ele_PoPtrue->Write();
  h_ele_PoPtrueVsEta ->Write();
  h_ele_PoPtrueVsPhi->Write();
  h_ele_PoPtrueVsPt->Write();
  h_ele_PoPtrue_barrel ->Write();
  h_ele_PoPtrue_endcaps->Write();
  h_ele_ChargeMnChargeTrue->Write();
  h_ele_EtaMnEtaTrue->Write();
  h_ele_EtaMnEtaTrueVsEta ->Write();
  h_ele_EtaMnEtaTrueVsPhi->Write();
  h_ele_EtaMnEtaTrueVsPt->Write();
  h_ele_PhiMnPhiTrue ->Write();
  h_ele_PhiMnPhiTrue2 ->Write();
  h_ele_PhiMnPhiTrueVsEta->Write();
  h_ele_PhiMnPhiTrueVsPhi->Write();
  h_ele_PhiMnPhiTrueVsPt->Write();

  // matched electron, superclusters
  histSclEn_->Write();
  histSclEoEtrue_barrel->Write();
  histSclEoEtrue_endcaps->Write();
  histSclEoEtrue_barrel_eg->Write();
  histSclEoEtrue_endcaps_eg->Write();
  histSclEoEtrue_barrel_etagap->Write();
  histSclEoEtrue_barrel_phigap->Write();
  histSclEoEtrue_ebeegap->Write();
  histSclEoEtrue_endcaps->Write();
  histSclEoEtrue_endcaps_deegap->Write();
  histSclEoEtrue_endcaps_ringgap->Write();
  histSclEoEtruePfVsEg->Write();
  histSclEt_->Write();
  histSclEtVsEta_->Write();
  histSclEtVsPhi_->Write();
  histSclEtaVsPhi_ ->Write();
  histSclEta_->Write();
  histSclPhi_->Write();
  histSclSigEtaEta_->Write();
  histSclSigIEtaIEtabarrel_->Write();
  histSclSigIEtaIEtaendcaps_->Write();
  histSclE1x5_->Write();
  histSclE2x5max_->Write();
  histSclE5x5_->Write();
  histSclSigEtaEta_eg_->Write();
  histSclSigIEtaIEtabarrel_eg_->Write();
  histSclSigIEtaIEtaendcaps_eg_->Write();
  histSclE1x5_eg_->Write();
  histSclE2x5max_eg_->Write();
  histSclE5x5_eg_->Write();

  // matched electron, gsf tracks
  h_ele_ambiguousTracks->Write();
  h_ele_ambiguousTracksVsEta->Write();
  h_ele_ambiguousTracksVsPhi->Write();
  h_ele_ambiguousTracksVsPt->Write();
  h_ele_foundHits->Write();
  h_ele_foundHitsVsEta->Write();
  h_ele_foundHitsVsPhi->Write();
  h_ele_foundHitsVsPt->Write();
  h_ele_lostHits->Write();
  h_ele_lostHitsVsEta->Write();
  h_ele_lostHitsVsPhi->Write();
  h_ele_lostHitsVsPt->Write();
  h_ele_chi2 ->Write();
  h_ele_chi2VsEta ->Write();
  h_ele_chi2VsPhi ->Write();
  h_ele_chi2VsPt->Write();
  h_ele_PinMnPout->Write();
  h_ele_PinMnPout_mode->Write();
  h_ele_PinMnPoutVsEta_mode->Write();
  h_ele_PinMnPoutVsPhi_mode->Write();
  h_ele_PinMnPoutVsPt_mode->Write();
  h_ele_PinMnPoutVsE_mode->Write();
  h_ele_PinMnPoutVsChi2_mode->Write();
  h_ele_outerP ->Write();
  h_ele_outerP_mode->Write();
  h_ele_outerPVsEta_mode->Write();
  h_ele_outerPt->Write();
  h_ele_outerPt_mode ->Write();
  h_ele_outerPtVsEta_mode->Write();
  h_ele_outerPtVsPhi_mode->Write();
  h_ele_outerPtVsPt_mode->Write();

  // matched electrons, matching
  h_ele_EoP ->Write();
  h_ele_EoP_eg ->Write();
  h_ele_EoPVsEta ->Write();
  h_ele_EoPVsPhi->Write();
  h_ele_EoPVsE->Write();
  h_ele_EseedOP ->Write();
  h_ele_EseedOP_eg ->Write();
  h_ele_EseedOPVsEta ->Write();
  h_ele_EseedOPVsPhi->Write();
  h_ele_EseedOPVsE->Write();
  h_ele_EoPout->Write();
  h_ele_EoPout_eg->Write();
  h_ele_EoPoutVsEta->Write();
  h_ele_EoPoutVsPhi->Write();
  h_ele_EoPoutVsE ->Write();
  h_ele_EeleOPout->Write();
  h_ele_EeleOPout_eg->Write();
  h_ele_EeleOPoutVsEta->Write();
  h_ele_EeleOPoutVsPhi->Write();
  h_ele_EeleOPoutVsE ->Write(); 
  h_ele_dEtaSc_propVtx->Write();
  h_ele_dEtaSc_propVtx_eg->Write();
  h_ele_dEtaScVsEta_propVtx->Write();
  h_ele_dEtaScVsPhi_propVtx->Write();
  h_ele_dEtaScVsPt_propVtx ->Write();
  h_ele_dPhiSc_propVtx->Write();
  h_ele_dPhiSc_propVtx_eg->Write();
  h_ele_dPhiScVsEta_propVtx ->Write();
  h_ele_dPhiScVsPhi_propVtx->Write();
  h_ele_dPhiScVsPt_propVtx->Write();
  h_ele_dEtaCl_propOut->Write();
  h_ele_dEtaCl_propOut_eg->Write();
  h_ele_dEtaClVsEta_propOut->Write();
  h_ele_dEtaClVsPhi_propOut->Write();
  h_ele_dEtaClVsPt_propOut->Write();
  h_ele_dPhiCl_propOut->Write();
  h_ele_dPhiCl_propOut_eg->Write();
  h_ele_dPhiClVsEta_propOut->Write();
  h_ele_dPhiClVsPhi_propOut->Write();
  h_ele_dPhiClVsPt_propOut->Write();
  h_ele_dEtaEleCl_propOut->Write();
  h_ele_dEtaEleCl_propOut_eg->Write();
  h_ele_dEtaEleClVsEta_propOut->Write();
  h_ele_dEtaEleClVsPhi_propOut->Write();
  h_ele_dEtaEleClVsPt_propOut->Write();
  h_ele_dPhiEleCl_propOut->Write();
  h_ele_dPhiEleCl_propOut_eg->Write();
  h_ele_dPhiEleClVsEta_propOut->Write();
  h_ele_dPhiEleClVsPhi_propOut->Write();
  h_ele_dPhiEleClVsPt_propOut->Write();
  h_ele_HoE->Write();
  h_ele_HoE_eg->Write();
  h_ele_HoE_fiducial->Write();
  h_ele_HoEVsEta->Write();
  h_ele_HoEVsPhi->Write();
  h_ele_HoEVsE->Write();

  h_ele_seed_dphi2_->Write();
  h_ele_seed_subdet2_->Write();
  TProfile *p_ele_seed_dphi2VsEta_ = h_ele_seed_dphi2VsEta_->ProfileX();
  p_ele_seed_dphi2VsEta_->SetTitle("mean ele seed dphi 2nd layer vs eta");
  p_ele_seed_dphi2VsEta_->GetXaxis()->SetTitle("#eta");
  p_ele_seed_dphi2VsEta_->GetYaxis()->SetTitle("<#phi_{pred} - #phi_{hit}, 2nd layer> (rad)");
  p_ele_seed_dphi2VsEta_->Write();
  TProfile *p_ele_seed_dphi2VsPt_ = h_ele_seed_dphi2VsPt_->ProfileX();
  p_ele_seed_dphi2VsPt_->SetTitle("mean ele seed dphi 2nd layer vs pt");
  p_ele_seed_dphi2VsPt_->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  p_ele_seed_dphi2VsPt_->GetYaxis()->SetTitle("<#phi_{pred} - #phi_{hit}, 2nd layer> (rad)");
  p_ele_seed_dphi2VsPt_->Write();
  h_ele_seed_drz2_->Write();
  TProfile *p_ele_seed_drz2VsEta_ = h_ele_seed_drz2VsEta_->ProfileX();
  p_ele_seed_drz2VsEta_->SetTitle("mean ele seed dr(dz) 2nd layer vs eta");
  p_ele_seed_drz2VsEta_->GetXaxis()->SetTitle("#eta");
  p_ele_seed_drz2VsEta_->GetYaxis()->SetTitle("<r(z)_{pred} - r(z)_{hit}, 2nd layer> (cm)");
  p_ele_seed_drz2VsEta_->Write();
  TProfile *p_ele_seed_drz2VsPt_ = h_ele_seed_drz2VsPt_->ProfileX();
  p_ele_seed_drz2VsPt_->SetTitle("mean ele seed dr(dz) 2nd layer vs pt");
  p_ele_seed_drz2VsPt_->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  p_ele_seed_drz2VsPt_->GetYaxis()->SetTitle("<r(z)_{pred} - r(z)_{hit}, 2nd layer> (cm)");
  p_ele_seed_drz2VsPt_->Write();

  // classes
  h_ele_classes->Write();
  h_ele_eta->Write();
  h_ele_eta_golden->Write();
  h_ele_eta_bbrem->Write();
  h_ele_eta_narrow->Write();
  h_ele_eta_shower->Write();
  h_ele_PinVsPoutGolden_mode->Write();
  h_ele_PinVsPoutShowering_mode->Write();
  h_ele_PinVsPoutGolden_mean->Write();
  h_ele_PinVsPoutShowering_mean->Write();
  h_ele_PtinVsPtoutGolden_mode->Write();
  h_ele_PtinVsPtoutShowering_mode->Write();
  h_ele_PtinVsPtoutGolden_mean->Write();
  h_ele_PtinVsPtoutShowering_mean->Write();
  histSclEoEtrueGolden_barrel->Write();
  histSclEoEtrueGolden_endcaps->Write();
  histSclEoEtrueShowering_barrel->Write();
  histSclEoEtrueShowering_endcaps->Write();

  // fbrem
  h_ele_fbrem->Write();
  h_ele_fbrem_eg->Write();
  h_ele_fbremVsEta_mode->GetXaxis()->SetTitle("#eta");
  h_ele_fbremVsEta_mode->GetYaxis()->SetTitle("<P_{in} - P_{out} / P_{in}>");
  h_ele_fbremVsEta_mode->Write();
  h_ele_fbremVsEta_mean->GetXaxis()->SetTitle("#eta");
  h_ele_fbremVsEta_mean->GetYaxis()->SetTitle("<P_{in} - P_{out} / P_{in}>");
  h_ele_fbremVsEta_mean->Write();
  h_ele_eta_goldenFrac->Write();
  h_ele_eta_bbremFrac->Write();
  h_ele_eta_narrowFrac->Write();
  h_ele_eta_showerFrac->Write();
  h_ele_xOverX0VsEta->Write();

  // efficiencies
  h_ele_etaEff->Write();
  h_ele_zEff->Write();
  h_ele_phiEff->Write();
  h_ele_absetaEff->Write();
  h_ele_ptEff->Write();
  h_ele_etaEff_all->Write();
  h_ele_ptEff_all->Write();
  
  // q misid
  h_ele_etaQmisid->Write();
  h_ele_zQmisid->Write();
  h_ele_absetaQmisid->Write();
  h_ele_ptQmisid->Write();
  
  // e/g et pflow electrons 
  h_ele_mva->Write();
  h_ele_mva_eg->Write();
  h_ele_provenance->Write();

  // isolation
  h_ele_tkSumPt_dr03->GetXaxis()->SetTitle("TkIsoSum, cone 0.3 (GeV/c)");
  h_ele_tkSumPt_dr03->GetYaxis()->SetTitle("Events");
  h_ele_tkSumPt_dr03->Write();
  h_ele_ecalRecHitSumEt_dr03->GetXaxis()->SetTitle("EcalIsoSum, cone 0.3 (GeV)");
  h_ele_ecalRecHitSumEt_dr03->GetYaxis()->SetTitle("Events");
  h_ele_ecalRecHitSumEt_dr03->Write();
  h_ele_hcalDepth1TowerSumEt_dr03->GetXaxis()->SetTitle("Hcal1IsoSum, cone 0.3 (GeV)");
  h_ele_hcalDepth1TowerSumEt_dr03->GetYaxis()->SetTitle("Events");
  h_ele_hcalDepth1TowerSumEt_dr03->Write();
  h_ele_hcalDepth2TowerSumEt_dr03->GetXaxis()->SetTitle("Hcal2IsoSum, cone 0.3 (GeV)");
  h_ele_hcalDepth2TowerSumEt_dr03->GetYaxis()->SetTitle("Events");
  h_ele_hcalDepth2TowerSumEt_dr03->Write();
  h_ele_tkSumPt_dr04->GetXaxis()->SetTitle("TkIsoSum, cone 0.4 (GeV/c)");
  h_ele_tkSumPt_dr04->GetYaxis()->SetTitle("Events");
  h_ele_tkSumPt_dr04->Write();
  h_ele_ecalRecHitSumEt_dr04->GetXaxis()->SetTitle("EcalIsoSum, cone 0.4 (GeV)");
  h_ele_ecalRecHitSumEt_dr04->GetYaxis()->SetTitle("Events");
  h_ele_ecalRecHitSumEt_dr04->Write();
  h_ele_hcalDepth1TowerSumEt_dr04->GetXaxis()->SetTitle("Hcal1IsoSum, cone 0.4 (GeV)");
  h_ele_hcalDepth1TowerSumEt_dr04->GetYaxis()->SetTitle("Events");
  h_ele_hcalDepth1TowerSumEt_dr04->Write();
  h_ele_hcalDepth2TowerSumEt_dr04->GetXaxis()->SetTitle("Hcal2IsoSum, cone 0.4 (GeV)");
  h_ele_hcalDepth2TowerSumEt_dr04->GetYaxis()->SetTitle("Events");
  h_ele_hcalDepth2TowerSumEt_dr04->Write();

}

GsfElectronMCAnalyzer::~GsfElectronMCAnalyzer()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  histfile_->Write();
  histfile_->Close();

}


//=========================================================================
// Main method
//=========================================================================

void
GsfElectronMCAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  std::cout << "analyzing new event " << std::endl;
  // get electrons

  edm::Handle<GsfElectronCollection> gsfElectrons;
  iEvent.getByLabel(electronCollection_,gsfElectrons);
  edm::LogInfo("")<<"\n\n =================> Treating event "<<iEvent.id()<<" Number of electrons "<<gsfElectrons.product()->size();

  edm::Handle<edm::HepMCProduct> hepMC;
  iEvent.getByLabel(mcTruthCollection_,hepMC);

  histNum_->Fill((*gsfElectrons).size());

  // all rec electrons
  for (reco::GsfElectronCollection::const_iterator gsfIter=gsfElectrons->begin();
   gsfIter!=gsfElectrons->end(); gsfIter++){
    // preselect electrons
    if (gsfIter->pt()>maxPt_ || fabs(gsfIter->eta())>maxAbsEta_) continue;
    h_ele_EoverP_all     -> Fill( gsfIter->eSuperClusterOverP() );
    h_ele_EseedOP_all     -> Fill( gsfIter->eSeedClusterOverP() );
    h_ele_EoPout_all     -> Fill( gsfIter->eSeedClusterOverPout() );
    h_ele_EeleOPout_all     -> Fill( gsfIter->eEleClusterOverPout() );
    h_ele_dEtaSc_propVtx_all -> Fill(gsfIter->deltaEtaSuperClusterTrackAtVtx());    
    h_ele_dPhiSc_propVtx_all -> Fill(gsfIter->deltaPhiSuperClusterTrackAtVtx());    
    h_ele_dEtaCl_propOut_all -> Fill(gsfIter->deltaEtaSeedClusterTrackAtCalo());    
    h_ele_dPhiCl_propOut_all -> Fill(gsfIter->deltaPhiSeedClusterTrackAtCalo());    
    h_ele_HoE_all     -> Fill( gsfIter->hadronicOverEm() );
    double d = gsfIter->vertex().x()*gsfIter->vertex().x()+gsfIter->vertex().y()*gsfIter->vertex().y();
    h_ele_TIP_all     -> Fill( sqrt(d) );
    h_ele_vertexEta_all     -> Fill( gsfIter->eta() );
    h_ele_vertexPt_all      -> Fill( gsfIter->pt() );
    // mee
    for (reco::GsfElectronCollection::const_iterator gsfIter2=gsfIter+1;
     gsfIter2!=gsfElectrons->end(); gsfIter2++){
        math::XYZTLorentzVector p12 = (*gsfIter).p4()+(*gsfIter2).p4();
        float mee2 = p12.Dot(p12);
	h_ele_mee_all -> Fill(sqrt(mee2));
    }
  }
  
  HepMC::GenParticle* genPc=0;
  const HepMC::GenEvent *myGenEvent = hepMC->GetEvent();
  int mcNum=0, gamNum=0, eleNum=0;
  HepMC::FourVector pAssSim;

  // charge mis-ID
  for ( HepMC::GenEvent::particle_const_iterator mcIter=myGenEvent->particles_begin(); mcIter != myGenEvent->particles_end(); mcIter++ ) {

    // select electrons
    if ( (*mcIter)->pdg_id() == 11 || (*mcIter)->pdg_id() == -11 ){

      // single primary electrons or electrons from Zs or Ws
      HepMC::GenParticle* mother = 0;
      if ( (*mcIter)->production_vertex() )  {
       if ( (*mcIter)->production_vertex()->particles_begin(HepMC::parents) !=
           (*mcIter)->production_vertex()->particles_end(HepMC::parents))
            mother = *((*mcIter)->production_vertex()->particles_begin(HepMC::parents));
      }
      if ( ((mother == 0) || ((mother != 0) && (mother->pdg_id() == 23))
	                  || ((mother != 0) && (mother->pdg_id() == 32))
	                  || ((mother != 0) && (fabs(mother->pdg_id()) == 24)))) {

      genPc=(*mcIter);
      pAssSim = genPc->momentum();

      if (pAssSim.perp()> maxPt_ || fabs(pAssSim.eta())> maxAbsEta_) continue;

      // suppress the endcaps
      //if (fabs(pAssSim.eta()) > 1.5) continue;
      // select central z
      //if ( fabs((*mcIter)->production_vertex()->position().z())>50.) continue;

      // looking for the best matching gsf electron
      bool okGsfFound = false;
      double gsfOkRatio = 999999.;

      // find best matched electron
      reco::GsfElectron bestGsfElectron;
      for (reco::GsfElectronCollection::const_iterator gsfIter=gsfElectrons->begin();
       gsfIter!=gsfElectrons->end(); gsfIter++){

        double dphi = gsfIter->phi()-pAssSim.phi();
        if (fabs(dphi)>CLHEP::pi)
         dphi = dphi < 0? (CLHEP::twopi) + dphi : dphi - CLHEP::twopi;
    	double deltaR = sqrt(pow((gsfIter->eta()-pAssSim.eta()),2) + pow(dphi,2));
	if ( deltaR < deltaR_ ){
	double mc_charge = genPc->pdg_id() == 11 ? -1. : 1. ;
	h_ele_ChargeMnChargeTrue  -> Fill( fabs(gsfIter->charge()-mc_charge));
	// require here a charge mismatch
	if ( (genPc->pdg_id() == 11) && (gsfIter->charge() > 0.) || (genPc->pdg_id() == -11) &&
	(gsfIter->charge() < 0.) ){
	  double tmpGsfRatio = gsfIter->p()/pAssSim.t();
	  if ( fabs(tmpGsfRatio-1) < fabs(gsfOkRatio-1) ) {
	    gsfOkRatio = tmpGsfRatio;
	    bestGsfElectron=*gsfIter;
	    okGsfFound = true;
	  }
	}
	}
      } // loop over rec ele to look for the best one

      // analysis when the mc track is found
      if (okGsfFound){

	// generated distributions for matched electrons
	h_ele_simPt_matched_qmisid      -> Fill( pAssSim.perp() );
        h_ele_simPhi_matched_qmisid   -> Fill( pAssSim.phi() );
	h_ele_simAbsEta_matched_qmisid     -> Fill( fabs(pAssSim.eta()) );
	h_ele_simEta_matched_qmisid     -> Fill( pAssSim.eta() );
	h_ele_simZ_matched_qmisid     -> Fill( (*mcIter)->production_vertex()->position().z()/10. );

      }
      }
    }
   
  }     
  
  // association mc-reco
  for ( HepMC::GenEvent::particle_const_iterator mcIter=myGenEvent->particles_begin(); mcIter != myGenEvent->particles_end(); mcIter++ ) {

    // number of mc particles
    mcNum++;

    // counts photons
    if ((*mcIter)->pdg_id() == 22 ){ gamNum++; }

    // select electrons
    if ( (*mcIter)->pdg_id() == 11 || (*mcIter)->pdg_id() == -11 ){

      // single primary electrons or electrons from Zs or Ws
      HepMC::GenParticle* mother = 0;
      if ( (*mcIter)->production_vertex() )  {
       if ( (*mcIter)->production_vertex()->particles_begin(HepMC::parents) !=
           (*mcIter)->production_vertex()->particles_end(HepMC::parents))
            mother = *((*mcIter)->production_vertex()->particles_begin(HepMC::parents));
      }
      if ( ((mother == 0) || ((mother != 0) && (mother->pdg_id() == 23))
	                  || ((mother != 0) && (mother->pdg_id() == 32))
	                  || ((mother != 0) && (fabs(mother->pdg_id()) == 24)))) {

      genPc=(*mcIter);
      pAssSim = genPc->momentum();

      if (pAssSim.perp()> maxPt_ || fabs(pAssSim.eta())> maxAbsEta_) continue;

      // suppress the endcaps
      //if (fabs(pAssSim.eta()) > 1.5) continue;
      // select central z
      //if ( fabs((*mcIter)->production_vertex()->position().z())>50.) continue;

      eleNum++;
      h_simEta -> Fill( pAssSim.eta() );
      h_simAbsEta -> Fill( fabs(pAssSim.eta()) );
      h_simP   -> Fill( pAssSim.t() );
      h_simPt   -> Fill( pAssSim.perp() );
      h_simPhi   -> Fill( pAssSim.phi() );
      h_simZ   -> Fill( (*mcIter)->production_vertex()->position().z()/10. );

      // looking for the best matching gsf electron
      bool okGsfFound = false;
      double gsfOkRatio = 999999.;
      
      // find best matched electron
      reco::GsfElectron bestGsfElectron;
      for (reco::GsfElectronCollection::const_iterator gsfIter=gsfElectrons->begin();
       gsfIter!=gsfElectrons->end(); gsfIter++){

        double dphi = gsfIter->phi()-pAssSim.phi();
        if (fabs(dphi)>CLHEP::pi)
         dphi = dphi < 0? (CLHEP::twopi) + dphi : dphi - CLHEP::twopi;
    	double deltaR = sqrt(pow((gsfIter->eta()-pAssSim.eta()),2) + pow(dphi,2));
	if ( deltaR < deltaR_ ){
	if ( (genPc->pdg_id() == 11) && (gsfIter->charge() < 0.) || (genPc->pdg_id() == -11) &&
	(gsfIter->charge() > 0.) ){
	  double tmpGsfRatio = gsfIter->p()/pAssSim.t();
	  if ( fabs(tmpGsfRatio-1) < fabs(gsfOkRatio-1) ) {
	    gsfOkRatio = tmpGsfRatio;
	    bestGsfElectron=*gsfIter;
	    okGsfFound = true;
	  }
	}
	}
     } // loop over rec ele to look for the best one

      // analysis when the mc track is found
     if (okGsfFound){

	// electron related distributions
	h_ele_charge        -> Fill( bestGsfElectron.charge() );
	h_ele_chargeVsEta        -> Fill( bestGsfElectron.eta(),bestGsfElectron.charge() );
	h_ele_chargeVsPhi        -> Fill( bestGsfElectron.phi(),bestGsfElectron.charge() );
	h_ele_chargeVsPt        -> Fill( bestGsfElectron.pt(),bestGsfElectron.charge() );
	h_ele_vertexP       -> Fill( bestGsfElectron.p() );
	h_ele_vertexPt      -> Fill( bestGsfElectron.pt() );
	h_ele_vertexPtVsEta      -> Fill(  bestGsfElectron.eta(),bestGsfElectron.pt() );
	h_ele_vertexPtVsPhi      -> Fill(  bestGsfElectron.phi(),bestGsfElectron.pt() );
	h_ele_vertexEta     -> Fill( bestGsfElectron.eta() );
	// generated distributions for matched electrons
	h_ele_simPt_matched      -> Fill( pAssSim.perp() );
        h_ele_simPhi_matched   -> Fill( pAssSim.phi() );
	h_ele_simAbsEta_matched     -> Fill( fabs(pAssSim.eta()) );
	h_ele_simEta_matched     -> Fill( pAssSim.eta() );
	h_ele_vertexEtaVsPhi     -> Fill(  bestGsfElectron.phi(),bestGsfElectron.eta() );
	h_ele_vertexPhi     -> Fill( bestGsfElectron.phi() );
	h_ele_vertexX     -> Fill( bestGsfElectron.vertex().x() );
	h_ele_vertexY     -> Fill( bestGsfElectron.vertex().y() );
	h_ele_vertexZ     -> Fill( bestGsfElectron.vertex().z() );
        h_ele_simZ_matched   -> Fill( (*mcIter)->production_vertex()->position().z()/10. );
	double d = (bestGsfElectron.vertex().x()-(*mcIter)->production_vertex()->position().x()/10.)
	          *(bestGsfElectron.vertex().x()-(*mcIter)->production_vertex()->position().x()/10.)+
	           (bestGsfElectron.vertex().y()-(*mcIter)->production_vertex()->position().y()/10.)
		  *(bestGsfElectron.vertex().y()-(*mcIter)->production_vertex()->position().y()/10.);
	d = sqrt(d);
	h_ele_vertexTIP     -> Fill( d );
	h_ele_vertexTIPVsEta     -> Fill(  bestGsfElectron.eta(), d );
	h_ele_vertexTIPVsPhi     -> Fill(  bestGsfElectron.phi(), d );
	h_ele_vertexTIPVsPt     -> Fill(  bestGsfElectron.pt(), d );
	h_ele_EtaMnEtaTrue  -> Fill( bestGsfElectron.eta()-pAssSim.eta());
	h_ele_EtaMnEtaTrueVsEta  -> Fill( bestGsfElectron.eta(), bestGsfElectron.eta()-pAssSim.eta());
	h_ele_EtaMnEtaTrueVsPhi  -> Fill( bestGsfElectron.phi(), bestGsfElectron.eta()-pAssSim.eta());
	h_ele_EtaMnEtaTrueVsPt  -> Fill( bestGsfElectron.pt(), bestGsfElectron.eta()-pAssSim.eta());
	h_ele_PhiMnPhiTrue  -> Fill( bestGsfElectron.phi()-pAssSim.phi());
	h_ele_PhiMnPhiTrue2  -> Fill( bestGsfElectron.phi()-pAssSim.phi());
	h_ele_PhiMnPhiTrueVsEta  -> Fill( bestGsfElectron.eta(), bestGsfElectron.phi()-pAssSim.phi());
	h_ele_PhiMnPhiTrueVsPhi  -> Fill( bestGsfElectron.phi(), bestGsfElectron.phi()-pAssSim.phi());
	h_ele_PhiMnPhiTrueVsPt  -> Fill( bestGsfElectron.pt(), bestGsfElectron.phi()-pAssSim.phi());
	h_ele_PoPtrue       -> Fill( bestGsfElectron.p()/pAssSim.t());
	h_ele_PoPtrueVsEta       -> Fill( bestGsfElectron.eta(), bestGsfElectron.p()/pAssSim.t());
	h_ele_PoPtrueVsPhi       -> Fill( bestGsfElectron.phi(), bestGsfElectron.p()/pAssSim.t());
	h_ele_PoPtrueVsPt       -> Fill( bestGsfElectron.py(), bestGsfElectron.p()/pAssSim.t());
	if (bestGsfElectron.isEB()) h_ele_PoPtrue_barrel       -> Fill( bestGsfElectron.p()/pAssSim.t());
	if (bestGsfElectron.isEE()) h_ele_PoPtrue_endcaps       -> Fill( bestGsfElectron.p()/pAssSim.t());

	// supercluster related distributions
	reco::SuperClusterRef sclRef = bestGsfElectron.superCluster();
	if (!bestGsfElectron.isEcalDriven()&&bestGsfElectron.isTrackerDriven()) sclRef = bestGsfElectron.pflowSuperCluster();
        histSclEn_->Fill(sclRef->energy());
        double R=TMath::Sqrt(sclRef->x()*sclRef->x() + sclRef->y()*sclRef->y() +sclRef->z()*sclRef->z());
        double Rt=TMath::Sqrt(sclRef->x()*sclRef->x() + sclRef->y()*sclRef->y());
        histSclEt_->Fill(sclRef->energy()*(Rt/R));
        histSclEtVsEta_->Fill(sclRef->eta(),sclRef->energy()*(Rt/R));
        histSclEtVsPhi_->Fill(sclRef->phi(),sclRef->energy()*(Rt/R));
        if (bestGsfElectron.isEB())  histSclEoEtrue_barrel->Fill(sclRef->energy()/pAssSim.t());
        if (bestGsfElectron.isEE())  histSclEoEtrue_endcaps->Fill(sclRef->energy()/pAssSim.t());
        if (bestGsfElectron.isEB() && bestGsfElectron.isEcalDriven())  histSclEoEtrue_barrel_eg->Fill(sclRef->energy()/pAssSim.t());
        if (bestGsfElectron.isEE() && bestGsfElectron.isEcalDriven())  histSclEoEtrue_endcaps_eg->Fill(sclRef->energy()/pAssSim.t());
        if (bestGsfElectron.isEB() && bestGsfElectron.isEBEtaGap())  histSclEoEtrue_barrel_etagap->Fill(sclRef->energy()/pAssSim.t());
        if (bestGsfElectron.isEB() && bestGsfElectron.isEBPhiGap())  histSclEoEtrue_barrel_phigap->Fill(sclRef->energy()/pAssSim.t());
        if (bestGsfElectron.isEBEEGap())  histSclEoEtrue_ebeegap->Fill(sclRef->energy()/pAssSim.t());
        if (bestGsfElectron.isEE())  histSclEoEtrue_endcaps->Fill(sclRef->energy()/pAssSim.t());
        if (bestGsfElectron.isEE() && bestGsfElectron.isEEDeeGap())  histSclEoEtrue_endcaps_deegap->Fill(sclRef->energy()/pAssSim.t());
        if (bestGsfElectron.isEE() && bestGsfElectron.isEERingGap())  histSclEoEtrue_endcaps_ringgap->Fill(sclRef->energy()/pAssSim.t());
        histSclEta_->Fill(sclRef->eta());
        histSclEtaVsPhi_->Fill(sclRef->phi(),sclRef->eta());
        histSclPhi_->Fill(sclRef->phi());
        histSclSigEtaEta_->Fill(bestGsfElectron.scSigmaEtaEta());
        if (bestGsfElectron.isEB()) histSclSigIEtaIEtabarrel_->Fill(bestGsfElectron.scSigmaIEtaIEta());
        if (bestGsfElectron.isEE()) histSclSigIEtaIEtaendcaps_->Fill(bestGsfElectron.scSigmaIEtaIEta());
        histSclE1x5_->Fill(bestGsfElectron.scE1x5());
        histSclE2x5max_->Fill(bestGsfElectron.scE2x5Max());
        histSclE5x5_->Fill(bestGsfElectron.scE5x5());
        if (bestGsfElectron.isEcalDriven())histSclSigEtaEta_eg_->Fill(bestGsfElectron.scSigmaEtaEta());
        if (bestGsfElectron.isEB()&&bestGsfElectron.isEcalDriven()) histSclSigIEtaIEtabarrel_eg_->Fill(bestGsfElectron.scSigmaIEtaIEta());
        if (bestGsfElectron.isEE()&&bestGsfElectron.isEcalDriven()) histSclSigIEtaIEtaendcaps_eg_->Fill(bestGsfElectron.scSigmaIEtaIEta());
        if (bestGsfElectron.isEcalDriven())histSclE1x5_eg_->Fill(bestGsfElectron.scE1x5());
        if (bestGsfElectron.isEcalDriven())histSclE2x5max_eg_->Fill(bestGsfElectron.scE2x5Max());
        if (bestGsfElectron.isEcalDriven())histSclE5x5_eg_->Fill(bestGsfElectron.scE5x5());
        float pfEnergy=0., egEnergy=0.;
	if (!bestGsfElectron.superCluster().isNull()) egEnergy = bestGsfElectron.superCluster()->energy();
	if (!bestGsfElectron.pflowSuperCluster().isNull()) pfEnergy = bestGsfElectron.pflowSuperCluster()->energy();
	histSclEoEtruePfVsEg->Fill(egEnergy/pAssSim.t(),pfEnergy/pAssSim.t());
	 
	// track related distributions
	h_ele_ambiguousTracks     -> Fill( bestGsfElectron.ambiguousGsfTracksSize() );
	h_ele_ambiguousTracksVsEta     -> Fill( bestGsfElectron.eta(), bestGsfElectron.ambiguousGsfTracksSize() );
	h_ele_ambiguousTracksVsPhi     -> Fill( bestGsfElectron.phi(), bestGsfElectron.ambiguousGsfTracksSize() );
	h_ele_ambiguousTracksVsPt     -> Fill( bestGsfElectron.pt(), bestGsfElectron.ambiguousGsfTracksSize() );
	h_ele_foundHits     -> Fill( bestGsfElectron.gsfTrack()->numberOfValidHits() );
	h_ele_foundHitsVsEta     -> Fill( bestGsfElectron.eta(), bestGsfElectron.gsfTrack()->numberOfValidHits() );
	h_ele_foundHitsVsPhi     -> Fill( bestGsfElectron.phi(), bestGsfElectron.gsfTrack()->numberOfValidHits() );
	h_ele_foundHitsVsPt     -> Fill( bestGsfElectron.pt(), bestGsfElectron.gsfTrack()->numberOfValidHits() );
	h_ele_lostHits      -> Fill( bestGsfElectron.gsfTrack()->numberOfLostHits() );
	h_ele_lostHitsVsEta      -> Fill( bestGsfElectron.eta(), bestGsfElectron.gsfTrack()->numberOfLostHits() );
	h_ele_lostHitsVsPhi      -> Fill( bestGsfElectron.phi(), bestGsfElectron.gsfTrack()->numberOfLostHits() );
	h_ele_lostHitsVsPt      -> Fill( bestGsfElectron.pt(), bestGsfElectron.gsfTrack()->numberOfLostHits() );
	h_ele_chi2          -> Fill( bestGsfElectron.gsfTrack()->normalizedChi2() );
	h_ele_chi2VsEta          -> Fill( bestGsfElectron.eta(), bestGsfElectron.gsfTrack()->normalizedChi2() );
	h_ele_chi2VsPhi          -> Fill( bestGsfElectron.phi(), bestGsfElectron.gsfTrack()->normalizedChi2() );
	h_ele_chi2VsPt          -> Fill( bestGsfElectron.pt(), bestGsfElectron.gsfTrack()->normalizedChi2() );
	// from gsf track interface, hence using mean
	h_ele_PinMnPout     -> Fill( bestGsfElectron.gsfTrack()->innerMomentum().R() - bestGsfElectron.gsfTrack()->outerMomentum().R() );
	h_ele_outerP        -> Fill( bestGsfElectron.gsfTrack()->outerMomentum().R() );
	h_ele_outerPt       -> Fill( bestGsfElectron.gsfTrack()->outerMomentum().Rho() );
        // from electron interface, hence using mode
	h_ele_PinMnPout_mode     -> Fill( bestGsfElectron.trackMomentumAtVtx().R() - bestGsfElectron.trackMomentumOut().R() );
	h_ele_PinMnPoutVsEta_mode     -> Fill(  bestGsfElectron.eta(), bestGsfElectron.trackMomentumAtVtx().R() - bestGsfElectron.trackMomentumOut().R() );
	h_ele_PinMnPoutVsPhi_mode     -> Fill(  bestGsfElectron.phi(), bestGsfElectron.trackMomentumAtVtx().R() - bestGsfElectron.trackMomentumOut().R() );
	h_ele_PinMnPoutVsPt_mode     -> Fill(  bestGsfElectron.pt(), bestGsfElectron.trackMomentumAtVtx().R() - bestGsfElectron.trackMomentumOut().R() );
	h_ele_PinMnPoutVsE_mode     -> Fill(  bestGsfElectron.caloEnergy(), bestGsfElectron.trackMomentumAtVtx().R() - bestGsfElectron.trackMomentumOut().R() );
	h_ele_PinMnPoutVsChi2_mode     -> Fill(  bestGsfElectron.gsfTrack()->normalizedChi2(), bestGsfElectron.trackMomentumAtVtx().R() - bestGsfElectron.trackMomentumOut().R() );
	h_ele_outerP_mode        -> Fill( bestGsfElectron.trackMomentumOut().R() );
	h_ele_outerPVsEta_mode        -> Fill(bestGsfElectron.eta(),  bestGsfElectron.trackMomentumOut().R() );
	h_ele_outerPt_mode       -> Fill( bestGsfElectron.trackMomentumOut().Rho() );
	h_ele_outerPtVsEta_mode       -> Fill(bestGsfElectron.eta(),  bestGsfElectron.trackMomentumOut().Rho() );
	h_ele_outerPtVsPhi_mode       -> Fill(bestGsfElectron.phi(),  bestGsfElectron.trackMomentumOut().Rho() );
	h_ele_outerPtVsPt_mode       -> Fill(bestGsfElectron.pt(),  bestGsfElectron.trackMomentumOut().Rho() );

        edm::RefToBase<TrajectorySeed> seed = bestGsfElectron.gsfTrack()->extra()->seedRef();
	ElectronSeedRef elseed=seed.castTo<ElectronSeedRef>();
	h_ele_seed_dphi2_-> Fill(elseed->dPhi2());
        h_ele_seed_dphi2VsEta_-> Fill(bestGsfElectron.eta(), elseed->dPhi2());
        h_ele_seed_dphi2VsPt_-> Fill(bestGsfElectron.pt(), elseed->dPhi2()) ;
        h_ele_seed_drz2_-> Fill(elseed->dRz2());
        h_ele_seed_drz2VsEta_-> Fill(bestGsfElectron.eta(), elseed->dRz2());
        h_ele_seed_drz2VsPt_-> Fill(bestGsfElectron.pt(), elseed->dRz2());
        h_ele_seed_subdet2_-> Fill(elseed->subDet2());

	// match distributions
	h_ele_EoP    -> Fill( bestGsfElectron.eSuperClusterOverP() );
	if (bestGsfElectron.isEcalDriven()) h_ele_EoP_eg    -> Fill( bestGsfElectron.eSuperClusterOverP() );
	h_ele_EoPVsEta    -> Fill(bestGsfElectron.eta(),  bestGsfElectron.eSuperClusterOverP() );
	h_ele_EoPVsPhi    -> Fill(bestGsfElectron.phi(),  bestGsfElectron.eSuperClusterOverP() );
	h_ele_EoPVsE    -> Fill(bestGsfElectron.caloEnergy(),  bestGsfElectron.eSuperClusterOverP() );
	h_ele_EseedOP    -> Fill( bestGsfElectron.eSeedClusterOverP() );
	if (bestGsfElectron.isEcalDriven()) h_ele_EseedOP_eg    -> Fill( bestGsfElectron.eSeedClusterOverP() );
	h_ele_EseedOPVsEta    -> Fill(bestGsfElectron.eta(),  bestGsfElectron.eSeedClusterOverP() );
	h_ele_EseedOPVsPhi    -> Fill(bestGsfElectron.phi(),  bestGsfElectron.eSeedClusterOverP() );
	h_ele_EseedOPVsE    -> Fill(bestGsfElectron.caloEnergy(),  bestGsfElectron.eSeedClusterOverP() );
	h_ele_EoPout -> Fill( bestGsfElectron.eSeedClusterOverPout() );
	if (bestGsfElectron.isEcalDriven()) h_ele_EoPout_eg -> Fill( bestGsfElectron.eSeedClusterOverPout() );
	h_ele_EoPoutVsEta -> Fill( bestGsfElectron.eta(), bestGsfElectron.eSeedClusterOverPout() );
	h_ele_EoPoutVsPhi -> Fill( bestGsfElectron.phi(), bestGsfElectron.eSeedClusterOverPout() );
	h_ele_EoPoutVsE -> Fill( bestGsfElectron.caloEnergy(), bestGsfElectron.eSeedClusterOverPout() );
	h_ele_EeleOPout -> Fill( bestGsfElectron.eEleClusterOverPout() );
	if (bestGsfElectron.isEcalDriven()) h_ele_EeleOPout_eg -> Fill( bestGsfElectron.eEleClusterOverPout() );
	h_ele_EeleOPoutVsEta -> Fill( bestGsfElectron.eta(), bestGsfElectron.eEleClusterOverPout() );
	h_ele_EeleOPoutVsPhi -> Fill( bestGsfElectron.phi(), bestGsfElectron.eEleClusterOverPout() );
	h_ele_EeleOPoutVsE -> Fill( bestGsfElectron.caloEnergy(), bestGsfElectron.eEleClusterOverPout() );
	h_ele_dEtaSc_propVtx -> Fill(bestGsfElectron.deltaEtaSuperClusterTrackAtVtx());
	if (bestGsfElectron.isEcalDriven()) h_ele_dEtaSc_propVtx_eg -> Fill(bestGsfElectron.deltaEtaSuperClusterTrackAtVtx());
	h_ele_dEtaScVsEta_propVtx -> Fill( bestGsfElectron.eta(),bestGsfElectron.deltaEtaSuperClusterTrackAtVtx());
	h_ele_dEtaScVsPhi_propVtx -> Fill(bestGsfElectron.phi(),bestGsfElectron.deltaEtaSuperClusterTrackAtVtx());
	h_ele_dEtaScVsPt_propVtx -> Fill(bestGsfElectron.pt(),bestGsfElectron.deltaEtaSuperClusterTrackAtVtx());
	h_ele_dPhiSc_propVtx -> Fill(bestGsfElectron.deltaPhiSuperClusterTrackAtVtx());
	if (bestGsfElectron.isEcalDriven()) h_ele_dPhiSc_propVtx_eg -> Fill(bestGsfElectron.deltaPhiSuperClusterTrackAtVtx());
	h_ele_dPhiScVsEta_propVtx -> Fill( bestGsfElectron.eta(),bestGsfElectron.deltaPhiSuperClusterTrackAtVtx());
	h_ele_dPhiScVsPhi_propVtx -> Fill(bestGsfElectron.phi(),bestGsfElectron.deltaPhiSuperClusterTrackAtVtx());
	h_ele_dPhiScVsPt_propVtx -> Fill(bestGsfElectron.pt(),bestGsfElectron.deltaPhiSuperClusterTrackAtVtx());
	h_ele_dEtaCl_propOut -> Fill(bestGsfElectron.deltaEtaSeedClusterTrackAtCalo());
	if (bestGsfElectron.isEcalDriven()) h_ele_dEtaCl_propOut_eg -> Fill(bestGsfElectron.deltaEtaSeedClusterTrackAtCalo());
	h_ele_dEtaClVsEta_propOut -> Fill( bestGsfElectron.eta(),bestGsfElectron.deltaEtaSeedClusterTrackAtCalo());
	h_ele_dEtaClVsPhi_propOut -> Fill(bestGsfElectron.phi(),bestGsfElectron.deltaEtaSeedClusterTrackAtCalo());
	h_ele_dEtaClVsPt_propOut -> Fill(bestGsfElectron.pt(),bestGsfElectron.deltaEtaSeedClusterTrackAtCalo());
	h_ele_dPhiCl_propOut -> Fill(bestGsfElectron.deltaPhiSeedClusterTrackAtCalo());
	if (bestGsfElectron.isEcalDriven()) h_ele_dPhiCl_propOut_eg -> Fill(bestGsfElectron.deltaPhiSeedClusterTrackAtCalo());
	h_ele_dPhiClVsEta_propOut -> Fill( bestGsfElectron.eta(),bestGsfElectron.deltaPhiSeedClusterTrackAtCalo());
	h_ele_dPhiClVsPhi_propOut -> Fill(bestGsfElectron.phi(),bestGsfElectron.deltaPhiSeedClusterTrackAtCalo());
	h_ele_dPhiClVsPt_propOut -> Fill(bestGsfElectron.pt(),bestGsfElectron.deltaPhiSeedClusterTrackAtCalo());
	h_ele_dEtaEleCl_propOut -> Fill(bestGsfElectron.deltaEtaEleClusterTrackAtCalo());
	if (bestGsfElectron.isEcalDriven()) h_ele_dEtaEleCl_propOut_eg -> Fill(bestGsfElectron.deltaEtaEleClusterTrackAtCalo());
	h_ele_dEtaEleClVsEta_propOut -> Fill( bestGsfElectron.eta(),bestGsfElectron.deltaEtaEleClusterTrackAtCalo());
	h_ele_dEtaEleClVsPhi_propOut -> Fill(bestGsfElectron.phi(),bestGsfElectron.deltaEtaEleClusterTrackAtCalo());
	h_ele_dEtaEleClVsPt_propOut -> Fill(bestGsfElectron.pt(),bestGsfElectron.deltaEtaEleClusterTrackAtCalo());
	h_ele_dPhiEleCl_propOut -> Fill(bestGsfElectron.deltaPhiEleClusterTrackAtCalo());
	if (bestGsfElectron.isEcalDriven()) h_ele_dPhiEleCl_propOut_eg -> Fill(bestGsfElectron.deltaPhiEleClusterTrackAtCalo());
	h_ele_dPhiEleClVsEta_propOut -> Fill( bestGsfElectron.eta(),bestGsfElectron.deltaPhiEleClusterTrackAtCalo());
	h_ele_dPhiEleClVsPhi_propOut -> Fill(bestGsfElectron.phi(),bestGsfElectron.deltaPhiEleClusterTrackAtCalo());
	h_ele_dPhiEleClVsPt_propOut -> Fill(bestGsfElectron.pt(),bestGsfElectron.deltaPhiEleClusterTrackAtCalo());
	h_ele_HoE -> Fill(bestGsfElectron.hadronicOverEm());
	if (bestGsfElectron.isEcalDriven()) h_ele_HoE_eg -> Fill(bestGsfElectron.hadronicOverEm());
	if (!bestGsfElectron.isEBEtaGap() && !bestGsfElectron.isEBPhiGap()&& !bestGsfElectron.isEBEEGap() &&
	    !bestGsfElectron.isEERingGap() && !bestGsfElectron.isEEDeeGap()) h_ele_HoE_fiducial -> Fill(bestGsfElectron.hadronicOverEm());
	h_ele_HoEVsEta -> Fill( bestGsfElectron.eta(),bestGsfElectron.hadronicOverEm());
	h_ele_HoEVsPhi -> Fill(bestGsfElectron.phi(),bestGsfElectron.hadronicOverEm());
	h_ele_HoEVsE -> Fill(bestGsfElectron.caloEnergy(),bestGsfElectron.hadronicOverEm());

	//classes
	int eleClass = bestGsfElectron.classification();
	if (bestGsfElectron.isEE()) eleClass+=10;
	h_ele_classes ->Fill(eleClass);

        if (bestGsfElectron.classification() == GsfElectron::GOLDEN && bestGsfElectron.isEB())  histSclEoEtrueGolden_barrel->Fill(sclRef->energy()/pAssSim.t());
        if (bestGsfElectron.classification() == GsfElectron::GOLDEN && bestGsfElectron.isEE())  histSclEoEtrueGolden_endcaps->Fill(sclRef->energy()/pAssSim.t());
        if (bestGsfElectron.classification() == GsfElectron::SHOWERING && bestGsfElectron.isEB())  histSclEoEtrueShowering_barrel->Fill(sclRef->energy()/pAssSim.t());
        if (bestGsfElectron.classification() == GsfElectron::SHOWERING && bestGsfElectron.isEE())  histSclEoEtrueShowering_endcaps->Fill(sclRef->energy()/pAssSim.t());

	//eleClass = eleClass%100; // get rid of barrel/endcap distinction
        h_ele_eta->Fill(fabs(bestGsfElectron.eta()));
        if (bestGsfElectron.classification() == GsfElectron::GOLDEN) h_ele_eta_golden ->Fill(fabs(bestGsfElectron.eta()));
        if (bestGsfElectron.classification() == GsfElectron::BIGBREM) h_ele_eta_bbrem ->Fill(fabs(bestGsfElectron.eta()));
        if (bestGsfElectron.classification() == GsfElectron::NARROW) h_ele_eta_narrow ->Fill(fabs(bestGsfElectron.eta()));
        if (bestGsfElectron.classification() == GsfElectron::SHOWERING) h_ele_eta_shower ->Fill(fabs(bestGsfElectron.eta()));

	//fbrem
	double fbrem_mean =  1. - bestGsfElectron.gsfTrack()->outerMomentum().R()/bestGsfElectron.gsfTrack()->innerMomentum().R();
	double fbrem_mode =  bestGsfElectron.fbrem();
	h_ele_fbrem->Fill(fbrem_mode);
	if (bestGsfElectron.isEcalDriven()) h_ele_fbrem_eg->Fill(fbrem_mode);
	h_ele_fbremVsEta_mode->Fill(bestGsfElectron.eta(),fbrem_mode);
	h_ele_fbremVsEta_mean->Fill(bestGsfElectron.eta(),fbrem_mean);

        if (bestGsfElectron.classification() == GsfElectron::GOLDEN) h_ele_PinVsPoutGolden_mode -> Fill(bestGsfElectron.trackMomentumOut().R(), bestGsfElectron.trackMomentumAtVtx().R());
        if (bestGsfElectron.classification() == GsfElectron::SHOWERING)
	 h_ele_PinVsPoutShowering_mode -> Fill(bestGsfElectron.trackMomentumOut().R(), bestGsfElectron.trackMomentumAtVtx().R());
	if (bestGsfElectron.classification() == GsfElectron::GOLDEN) h_ele_PinVsPoutGolden_mean -> Fill(bestGsfElectron.gsfTrack()->outerMomentum().R(), bestGsfElectron.gsfTrack()->innerMomentum().R());
        if (bestGsfElectron.classification() == GsfElectron::SHOWERING)
	 h_ele_PinVsPoutShowering_mean ->  Fill(bestGsfElectron.gsfTrack()->outerMomentum().R(), bestGsfElectron.gsfTrack()->innerMomentum().R());
        if (bestGsfElectron.classification() == GsfElectron::GOLDEN) h_ele_PtinVsPtoutGolden_mode -> Fill(bestGsfElectron.trackMomentumOut().Rho(), bestGsfElectron.trackMomentumAtVtx().Rho());
        if (bestGsfElectron.classification() == GsfElectron::SHOWERING)
	 h_ele_PtinVsPtoutShowering_mode -> Fill(bestGsfElectron.trackMomentumOut().Rho(), bestGsfElectron.trackMomentumAtVtx().Rho());
	if (bestGsfElectron.classification() == GsfElectron::GOLDEN) h_ele_PtinVsPtoutGolden_mean -> Fill(bestGsfElectron.gsfTrack()->outerMomentum().Rho(), bestGsfElectron.gsfTrack()->innerMomentum().Rho());
        if (bestGsfElectron.classification() == GsfElectron::SHOWERING)
	 h_ele_PtinVsPtoutShowering_mean ->  Fill(bestGsfElectron.gsfTrack()->outerMomentum().Rho(), bestGsfElectron.gsfTrack()->innerMomentum().Rho());

        h_ele_mva->Fill(bestGsfElectron.mva());
        if (bestGsfElectron.isEcalDriven()) h_ele_mva_eg->Fill(bestGsfElectron.mva());
	if (bestGsfElectron.isEcalDriven()) h_ele_provenance->Fill(1.);
	if (bestGsfElectron.isTrackerDriven()) h_ele_provenance->Fill(-1.);
	if (bestGsfElectron.isTrackerDriven()||bestGsfElectron.isEcalDriven()) h_ele_provenance->Fill(0.);
	if (bestGsfElectron.isTrackerDriven()&&!bestGsfElectron.isEcalDriven()) h_ele_provenance->Fill(-2.);
	if (!bestGsfElectron.isTrackerDriven()&&bestGsfElectron.isEcalDriven()) h_ele_provenance->Fill(2.);	

        h_ele_tkSumPt_dr03->Fill(bestGsfElectron.dr03TkSumPt());
        h_ele_ecalRecHitSumEt_dr03->Fill(bestGsfElectron.dr03EcalRecHitSumEt());
        h_ele_hcalDepth1TowerSumEt_dr03->Fill(bestGsfElectron.dr03HcalDepth1TowerSumEt());
        h_ele_hcalDepth2TowerSumEt_dr03->Fill(bestGsfElectron.dr03HcalDepth2TowerSumEt());
        h_ele_tkSumPt_dr04->Fill(bestGsfElectron.dr04TkSumPt());
        h_ele_ecalRecHitSumEt_dr04->Fill(bestGsfElectron.dr04EcalRecHitSumEt());
        h_ele_hcalDepth1TowerSumEt_dr04->Fill(bestGsfElectron.dr04HcalDepth1TowerSumEt());
        h_ele_hcalDepth2TowerSumEt_dr04->Fill(bestGsfElectron.dr04HcalDepth2TowerSumEt());

      } // gsf electron found

    } // mc particle found

    }

  } // loop over mc particle

  h_mcNum->Fill(mcNum);
  h_eleNum->Fill(eleNum);

}


