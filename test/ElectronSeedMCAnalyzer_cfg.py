import FWCore.ParameterSet.Config as cms

process = cms.Process("readseeds")

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("RecoEcal.EgammaClusterProducers.ecalClusteringSequence_cff")
process.load("RecoEgamma.EgammaElectronProducers.pixelMatchGsfElectronSequence_cff")
process.load("RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi")
process.load("RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitConverter_cfi")
process.load("RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitMatcher_cfi")
process.load("RecoLocalTracker.SiStripRecHitConverter.StripCPEfromTrackAngle_cfi")
process.load("RecoLocalTracker.SiStripZeroSuppression.SiStripZeroSuppression_cfi")
process.load("RecoLocalTracker.SiStripClusterizer.SiStripClusterizer_cfi")
process.load("RecoLocalTracker.SiPixelClusterizer.SiPixelClusterizer_cfi")
process.load("RecoLocalTracker.SiPixelRecHits.PixelCPEESProducers_cff")
process.load("RecoTracker.TransientTrackingRecHit.TTRHBuilders_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.source = cms.Source ("PoolSource",
    fileNames = cms.untracked.vstring (
#   '/store/relval/CMSSW_2_1_8/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0002/00E17DCF-8D82-DD11-BF03-000423D987E0.root',
    )
)
process.electronSeedAnalysis = cms.EDAnalyzer("ElectronPixelSeedAnalyzer",
    inputCollection = cms.InputTag("electronPixelSeeds"),
)

process.p = cms.Path(process.siPixelRecHits*process.siStripMatchedRecHits*process.newSeedFromPairs*process.newSeedFromTriplets*process.newCombinedSeeds*process.electronPixelSeeds*process.electronSeedAnalysis)
process.GlobalTag.globaltag = 'STARTUP_V7::All'
#process.GlobalTag.globaltag = 'IDEAL_V7::All'

