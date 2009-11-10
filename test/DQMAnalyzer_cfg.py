import FWCore.ParameterSet.Config as cms

process = cms.Process("readelectrons")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring
    (
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0028/DAA8926E-CE8B-DE11-9654-0030487DF78A.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0022/5471E7BB-0089-DE11-91BC-000423CA664C.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0021/F6436740-D487-DE11-AC1F-00E0814002B3.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0021/900B3AE4-9787-DE11-A714-0030485C6962.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0021/8ED78A6B-3C87-DE11-A7DA-00144F0D6806.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0021/8E801D2F-5B87-DE11-A4B8-00144F0D68C8.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0021/8E375136-F08C-DE11-A63F-001E0B47E400.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0021/8C7378E5-F88C-DE11-BDAE-001CC47BEE5E.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0021/8C0D7DAC-5787-DE11-B65D-00144F0D67FC.root',  
        '/store/mc/Summer09/QCDDiJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0046/FC5BB47E-FF8F-DE11-8DAF-001CC47B8E48.root',
        '/store/mc/Summer09/QCDDiJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0046/FC17B2FB-0990-DE11-94E9-0018FEFAA36E.root',
        '/store/mc/Summer09/QCDDiJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0046/EAD3BA51-0B90-DE11-8B47-003048C45AEA.root',
        '/store/mc/Summer09/QCDDiJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0046/E2A8A1D1-0490-DE11-ABAB-0018FEFAA2D6.root',
        '/store/mc/Summer09/QCDDiJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0046/D818499A-0190-DE11-BB2B-0018FEFAA3AC.root',
        '/store/mc/Summer09/QCDDiJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0046/D2726869-7D90-DE11-9805-001F29C9C5C4.root',
        '/store/mc/Summer09/QCDDiJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0046/D26C66CE-E390-DE11-BCB3-0030487DA364.root',
        '/store/mc/Summer09/QCDDiJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0046/C611568D-0B90-DE11-BB1F-003048945312.root',
        '/store/mc/Summer09/QCDDiJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0046/C43D56A4-FC8F-DE11-81D2-001F29C95558.root',
        '/store/mc/Summer09/QCDDiJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0046/BE1EE0AA-FF8F-DE11-B863-001E68A993EE.root',
        '/store/mc/Summer09/QCDDiJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0046/B8309C53-0C90-DE11-8A5B-0018FEFAA3AC.root',
        '/store/mc/Summer09/QCDDiJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0046/9A0F8633-0490-DE11-95D2-001F29086E74.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0028/DAA8926E-CE8B-DE11-9654-0030487DF78A.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0022/5471E7BB-0089-DE11-91BC-000423CA664C.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0021/F6436740-D487-DE11-AC1F-00E0814002B3.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0021/900B3AE4-9787-DE11-A714-0030485C6962.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0021/8ED78A6B-3C87-DE11-A7DA-00144F0D6806.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0021/8E801D2F-5B87-DE11-A4B8-00144F0D68C8.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0021/8E375136-F08C-DE11-A63F-001E0B47E400.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0021/8C7378E5-F88C-DE11-BDAE-001CC47BEE5E.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0021/8C0D7DAC-5787-DE11-B65D-00144F0D67FC.root',      
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0006/C63567A9-DD80-DE11-87D5-001EC9AF17F9.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0006/C4BFA0DD-0F81-DE11-914E-003048775E54.root', 
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0006/660A4969-D680-DE11-838B-003048772E0E.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0006/5CFDB41C-9C80-DE11-AE56-001A649700A8.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0006/586087FE-4280-DE11-A151-003048D479E2.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0006/34B667B2-C780-DE11-9E61-003048D47908.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0006/30540CAB-C780-DE11-AF6E-0030485A0556.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0006/1A17EF5D-0281-DE11-919E-001A64970B6C.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0005/B80406C1-ED80-DE11-8652-0030487D2BE4.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0005/A4F5EA5D-0D80-DE11-A495-0030485A0592.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0005/5C11746E-4A80-DE11-B4BE-001A6497108C.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0005/0E218482-5080-DE11-ABEB-001EC9AEEEEB.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0003/5421CDB4-807F-DE11-B52E-00E081B190CA.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0002/24646E05-3C7E-DE11-A4AF-003048D43690.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0001/F80A6B12-A97D-DE11-B897-0030485A053E.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0001/D6474748-9F7D-DE11-AA36-003048775D68.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0001/C0A9E086-4680-DE11-BC67-001EC9AEFC7E.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0001/82201521-CE7D-DE11-A49A-003048779662.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0001/821CEA49-3D7E-DE11-BA93-00E081B190CA.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0001/6C34FEF2-B07D-DE11-9004-0030488C6259.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0001/5EE38A25-A17D-DE11-8B95-00304877248E.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0001/3A962A76-CC7D-DE11-AC96-001A64971014.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/FA879EDF-1F7D-DE11-A45A-001A64971014.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/F61578CF-7B7D-DE11-BE01-001EC9AF22D5.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/F0519B11-6D7D-DE11-B354-001EC9AF2249.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/EED9ADAB-6C7D-DE11-B250-001EC9AEEEEB.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/ECBD9CFE-757D-DE11-A1C6-001EC9B2183F.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/E8D5C7DD-847D-DE11-8ED9-003048779868.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/CAA929DB-067D-DE11-9C47-0030485A0592.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/C82A3F7C-A37D-DE11-B89B-001A649710C0.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/C4DCED72-F07D-DE11-99EE-00E081B190CA.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/BC8D8ED1-207D-DE11-93E5-001A64970E28.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/BAFA6A63-9C7D-DE11-B03C-003048D376EE.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/BA52CA85-A47D-DE11-A7D1-001EC9B209FD.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/AADB5D29-A47D-DE11-8531-001A649710C0.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/A856505C-9C7D-DE11-B6AE-003048D43690.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/A0210253-777D-DE11-91B1-001EC9B3AC2A.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/9C017060-897D-DE11-BF77-003048772E0E.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/96FEED35-997D-DE11-BECE-00E081305C3C.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/808269D7-A47D-DE11-B5C4-001A64970CB4.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/74997A95-8E7D-DE11-B479-003048775D72.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/6E52451C-A37D-DE11-A1E3-00144F1FD0F8.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/641ED9E0-6F7D-DE11-B71E-003048772DC0.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/605162E4-AE7D-DE11-A12D-001EC9B22B53.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/52C8A8F7-5F7D-DE11-9E8D-001EC9AF22D5.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/4C8F0DE1-B47D-DE11-A6A5-001EC9B218A8.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/4859C7B9-207D-DE11-A3A3-001A6478F28C.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/404AB96F-587D-DE11-9F7F-003048772420.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/3A1CBE5E-0C7D-DE11-8954-0030485A04F6.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/34920F8E-FC7D-DE11-9C44-00E081B18CF6.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/32E7564A-0B7D-DE11-B974-0030485A07E2.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/26CC055E-977D-DE11-82E9-0030487721FE.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/269E2C7C-627D-DE11-BEAE-003048772458.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/1E35F4B1-717D-DE11-B654-001EC9AF22D5.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/1ADBB137-787D-DE11-810A-001EC9AF1312.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/12350EDD-1F7D-DE11-B91F-001A649708BC.root',
        '/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3-v1/0000/0A09727F-A27D-DE11-9C5B-00144F1FCF80.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0028/DAA8926E-CE8B-DE11-9654-0030487DF78A.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0022/5471E7BB-0089-DE11-91BC-000423CA664C.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0021/F6436740-D487-DE11-AC1F-00E0814002B3.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0021/900B3AE4-9787-DE11-A714-0030485C6962.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0021/8ED78A6B-3C87-DE11-A7DA-00144F0D6806.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0021/8E801D2F-5B87-DE11-A4B8-00144F0D68C8.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0021/8E375136-F08C-DE11-A63F-001E0B47E400.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0021/8C7378E5-F88C-DE11-BDAE-001CC47BEE5E.root',
        '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3-v1/0021/8C0D7DAC-5787-DE11-B65D-00144F0D67FC.root'    
    ),
    secondaryFileNames = cms.untracked.vstring(),
)

process.mergedSuperClusters = cms.EDFilter("EgammaSuperClusterMerger",
    src = cms.VInputTag(cms.InputTag("correctedHybridSuperClusters"), cms.InputTag("multi5x5SuperClustersWithPreshower"))
)

from RecoEgamma.Examples.dataAnalyzerStdBiningParameters_cff import *
from RecoEgamma.Examples.dataAnalyzerFineBiningParameters_cff import *

process.DQMgsfElectronAnalysis = cms.EDAnalyzer("DQMAnalyzer",
    electronCollection = cms.InputTag("gsfElectrons"),
    readAOD = cms.bool(False),
    outputFile = cms.string('DQMtest.root'),
    triggerResults = cms.InputTag("TriggerResults::HLT"),
    hltPaths = cms.vstring('HLT_Ele10_SW_L1R','HLT_Ele15_SW_L1R','HLT_Ele15_SW_EleId_L1R','HLT_Ele15_SW_LooseTrackIso_L1R','HLT_Ele15_SC15_SW_LooseTrackIso_L1R','HLT_Ele15_SC15_SW_EleId_L1R','HLT_Ele20_SW_L1R','HLT_Ele20_SC15_SW_L1R','HLT_Ele25_SW_L1R','HLT_Ele25_SW_EleId_LooseTrackIso_L1R','HLT_DoubleEle10_SW_L1R'),
    matchingObjectCollection = cms.InputTag("mergedSuperClusters"),
    matchingCondition = cms.string("Cone"),
    DeltaR = cms.double(0.3),
    MaxPtMatchingObject = cms.double(100.0),
    MaxAbsEtaMatchingObject = cms.double(2.5),

    Selection = cms.int32(1),   # 0=All elec,  1=Etcut, 2=Iso, 3=eId, 4=T&P
    MassLow = cms.double(60),
    MassHigh = cms.double(120),
    TPchecksign = cms.bool(False),  
    TAGcheckclass = cms.bool(False),  
    PROBEetcut = cms.bool(False),
    PROBEcheckclass = cms.bool(False),                                        

                                             
    MinEt = cms.double(15.),
    MinPt = cms.double(0.),
    MaxAbsEta = cms.double(2.5),
    SelectEB = cms.bool(False),
    SelectEE = cms.bool(False),
    SelectNotEBEEGap = cms.bool(False),
    SelectEcalDriven = cms.bool(False),
    SelectTrackerDriven = cms.bool(False),
    MinEOverPBarrel = cms.double(0.),
    MaxEOverPBarrel = cms.double(10000.),
    MinEOverPEndcaps = cms.double(0.),
    MaxEOverPEndcaps = cms.double(10000.),
    MinDetaBarrel = cms.double(0.),
    MaxDetaBarrel = cms.double(10000.),
    MinDetaEndcaps = cms.double(0.),
    MaxDetaEndcaps = cms.double(10000.),
    MinDphiBarrel = cms.double(0.),
    MaxDphiBarrel = cms.double(10000.),
    MinDphiEndcaps = cms.double(0.),
    MaxDphiEndcaps = cms.double(10000.),
    MinSigIetaIetaBarrel = cms.double(0.),
    MaxSigIetaIetaBarrel = cms.double(10000.),
    MinSigIetaIetaEndcaps = cms.double(0.),
    MaxSigIetaIetaEndcaps = cms.double(10000.),
    MaxHoEBarrel = cms.double(10000.),
    MaxHoEEndcaps = cms.double(10000.),
    MinMVA = cms.double(-10000.),
    MaxTipBarrel = cms.double(10000.),
    MaxTipEndcaps = cms.double(10000.),
    MaxTkIso03 = cms.double(10000.),
    MaxHcalIso03Depth1Barrel = cms.double(10000.),
    MaxHcalIso03Depth1Endcaps = cms.double(10000.),
    MaxHcalIso03Depth2Endcaps = cms.double(10000.),
    MaxEcalIso03Barrel = cms.double(10000.),
    MaxEcalIso03Endcaps = cms.double(10000.),
    HistosConfigurationData = cms.PSet(
    Etamin = cms.double(-2.5),
    Etamax = cms.double(2.5),
    Phimax = cms.double(3.2),
    Phimin = cms.double(-3.2),
    Ptmax = cms.double(100.0),
    Pmax = cms.double(300.0),
    Eopmax = cms.double(5.0),
    Eopmaxsht = cms.double(3.0),
    Detamin = cms.double(-0.005),
    Detamax = cms.double(0.005),
    Dphimin = cms.double(-0.01),
    Dphimax = cms.double(0.01),
    Dphimatchmin = cms.double(-0.2),
    Dphimatchmax = cms.double(0.2),
    Detamatchmin = cms.double(-0.05),
    Detamatchmax = cms.double(0.05),
    Fhitsmax = cms.double(30.0),
    Lhitsmax = cms.double(10.0),
    Nbinxyz = cms.int32(50),
    Nbineop2D = cms.int32(30),
    Nbinp = cms.int32(50),
    Nbineta2D = cms.int32(50),
    Nbinfhits = cms.int32(30),
    Nbinlhits = cms.int32(5),
    Nbinpteff = cms.int32(19),
    Nbinphi2D = cms.int32(32),
    Nbindetamatch2D = cms.int32(50),
    Nbineta = cms.int32(50),
    Nbinp2D = cms.int32(50),
    Nbindeta = cms.int32(100),
    Nbinpt2D = cms.int32(50),
    Nbindetamatch = cms.int32(100),
    Nbinphi = cms.int32(64),
    Nbindphimatch = cms.int32(100),
    Nbinpt = cms.int32(50),
    Nbindphimatch2D = cms.int32(50),
    Nbindphi = cms.int32(100),
    Nbineop = cms.int32(50),
    Nbinpoptrue = cms.int32(75),
    Poptruemin = cms.double(0.0),
    Poptruemax = cms.double(1.5),
    Nbinmee = cms.int32(100),
    Meemin = cms.double(0.0),
    Meemax = cms.double(150.),
    Nbinhoe = cms.int32(100),
    Hoemin = cms.double(0.0),
    Hoemax = cms.double(0.5)
    #dataAnalyzerStdBiningParameters
    #dataAnalyzerFineBiningParameters
    )
)

process.p = cms.Path(process.mergedSuperClusters*process.DQMgsfElectronAnalysis)


