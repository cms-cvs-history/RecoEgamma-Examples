#!/bin/csh

# This script can be used to generate a web page to compare histograms from 
# two input root files produced using the EDAnalyzers in RecoEgamma/Examples.

#============= Configuration =================
# You can change the default values below,
# tuned for the Oval tests suite, with any
# value you would prefer.

setenv VAL_ENV $1
setenv VAL_OUTPUT_FILE $2

if ( ${?VAL_NEW_FILE} == "0" ) then
  setenv VAL_NEW_FILE ""
endif
if ( ${VAL_NEW_FILE} == "" ) then
  setenv VAL_NEW_FILE ${cwd}/cmsRun.${VAL_ENV}.olog.${VAL_OUTPUT_FILE}
endif

if ( ${?VAL_REF_FILE} == "0" ) then
  setenv VAL_REF_FILE ""
endif
if ( ${VAL_REF_FILE} == "" ) then
  if ( -e "${cwd}/cmsRun.${VAL_ENV}.oref.${VAL_OUTPUT_FILE}" ) then
    setenv VAL_REF_FILE ${cwd}/cmsRun.${VAL_ENV}.oref.${VAL_OUTPUT_FILE}
  endif
endif
 
#Release versions to be compared (affects output directory name and html description only)
#setenv VAL_NEW_RELEASE 220pre1IDEAL
#setenv VAL_REF_RELEASE 219IDEAL

#VAL_ANALYZER must be one of GsfElectronMCAnalyzer, GsfElectronFakeAnalyzer,
#SimplePhotonAnalyzer or SimpleConvertedPhotonAnalyzer
#setenv VAL_ANALYZER GsfElectronMCAnalyzer

#Name of DBS_SAMPLE (affects output directory name and html description only)
#setenv DBS_SAMPLE RelValQCD_Pt_80_120


#============== Prepare the output directories ==================

#Location of output.  The default will put your output in:
#http://cmsdoc.cern.ch/Physics/egamma/www/validation/

setenv CURRENTDIR $cwd
cd /afs/cern.ch/cms/Physics/egamma/www/validation

if (! -d $VAL_NEW_RELEASE) then
  mkdir $VAL_NEW_RELEASE
endif
cd $VAL_NEW_RELEASE

if (! -d "vs${VAL_REF_RELEASE}") then
  mkdir "vs${VAL_REF_RELEASE}"
endif
cd "vs${VAL_REF_RELEASE}"

if (! -d ${DBS_SAMPLE}) then
  mkdir ${DBS_SAMPLE}
endif
cd ${DBS_SAMPLE}

if (! -d gifs) then
  mkdir gifs
endif

cp -f $CURRENTDIR/newvalidation.C .

#============== Prepare the list of histograms ==================

if ( $VAL_ANALYZER == GsfElectronMCAnalyzer ) then

cat > histos.txt <<EOF
h_ele_PoPtrue   1
h_ele_PoPtrue_barrel   1
h_ele_PoPtrue_endcaps   1
h_scl_EoEtrue_barrel   1
h_scl_EoEtrue_endcaps   1
h_ele_EtaMnEtaTrue   1
h_ele_PhiMnPhiTrue 1
h_ele_vertexP 1
h_ele_vertexPt 1
h_ele_outerP_mode 1
h_ele_outerPt_mode 1
h_ele_vertexX 1
h_ele_vertexY 1
h_ele_vertexZ 1
h_ele_EoP 1
h_ele_EoPout 1
h_ele_dEtaCl_propOut 1
h_ele_dEtaSc_propVtx 1
h_ele_dPhiCl_propOut 1
h_ele_dPhiSc_propVtx 1
h_ele_HoE 1
h_ele_chi2 1
h_ele_foundHits 1
h_ele_lostHits 1
h_ele_PinMnPout_mode 1
h_ele_classes 1
h_ele_charge 1
h_ele_EoverP_all 1
h_recEleNum 1
EOF

cat >> histos.txt <<EOF
h_ele_absetaEff	0
h_ele_etaEff	0
h_ele_ptEff	0
h_ele_phiEff	0
h_ele_zEff	0
h_ele_etaEff_all	0
h_ele_ptEff_all	0
h_ele_PoPtrueVsEta_pfx 	0  
h_ele_PoPtrueVsPhi_pfx   	0
h_ele_EtaMnEtaTrueVsEta_pfx  	0
h_ele_PhiMnPhiTrueVsEta_pfx 	0
h_ele_vertexPtVsEta_pfx 	0
h_ele_EoPVsEta_pfx 	0
h_ele_EoPoutVsEta_pfx 	0
h_ele_HoEVsEta_pfx 	0
h_ele_chi2VsEta_pfx 	0
h_ele_foundHitsVsEta_pfx 	0
h_ele_fbremvsEtamean	0
h_ele_fbremvsEtamode	0
h_ele_eta_bbremFrac 	0
h_ele_eta_goldenFrac 	0
h_ele_eta_narrowFrac 	0
h_ele_eta_showerFrac 	0
EOF

else if ($VAL_ANALYZER == GsfElectronFakeAnalyzer ) then

cat > histos.txt <<EOF
h_ele_vertexP  	1
h_ele_vertexPt  	1
h_ele_outerP_mode  	1
h_ele_outerPt_mode  	1
h_ele_vertexX 	1
h_ele_vertexY 	1
h_ele_vertexZ 	1
h_ele_EoP 	1
h_ele_EoPout 	1
h_ele_dEtaCl_propOut 	1
h_ele_dEtaSc_propVtx 	1
h_ele_dPhiCl_propOut 	1
h_ele_dPhiSc_propVtx 	1
h_ele_HoE 	1
h_ele_chi2 	1
h_ele_foundHits 	1
h_ele_lostHits 	1
h_ele_classes 	1
h_ele_charge	1
h_ele_mee_all	1
h_recEleNum	1
EOF

cat >> histos.txt <<EOF
h_ele_absetaEff 	0
h_ele_etaEff 	0
h_ele_ptEff 	0
h_ele_phiEff 	0
h_ele_zEff 	0
h_ele_etaEff_all 	0
h_ele_ptEff_all 	0
h_ele_vertexPtVsEta_pfx  	0
h_ele_EoPVsEta_pfx  	0
h_ele_EoPoutVsEta_pfx  	0
h_ele_HoEVsEta_pfx  	0
h_ele_chi2VsEta_pfx  	0
h_ele_foundHitsVsEta_pfx  	0
h_ele_fbremvsEtamean 	0
h_ele_fbremvsEtamode 	0
h_ele_eta_bbremFrac  	0
h_ele_eta_goldenFrac  	0
h_ele_eta_narrowFrac  	0
h_ele_eta_showerFrac  	0
EOF

else if ( $VAL_ANALYZER == SimplePhotonAnalyzer ) then

cat > histos.txt <<EOF
scE 1
scEt 1
scEta 1
scPhi 1
deltaEtaSC 1
deltaPhiSC 1
phoE 1
phoEta 1
phoPhi 1
phoR9Barrel 1
phoR9Endcap 1
recEoverTrueEBarrel 1
recEoverTrueEEndcap 1
recESCoverTrueEBarrel 1
recESCoverTrueEEndcap 1
e5x5_unconvBarrelOverEtrue 1
e5x5_unconvEndcapOverEtrue 1
ePho_convBarrelOverEtrue 1
ePho_convEndcapOverEtrue 1
deltaEta 1
deltaPhi 1
EOF

else if ( $VAL_ANALYZER == SimpleConvertedPhotonAnalyzer ) then

cat > histos.txt <<EOF
deltaE 1
deltaPhi 1
deltaEta 1
MCphoE 1
MCphoPhi 1
MCphoEta 1
MCConvE 1
MCConvPt 1
MCConvEta 1
scE 1
scEta 1
scPhi 1
phoE 1
phoEta 1
phoPhi 1
EOF

endif


#================= Generate the gifs and validation.html =====================

root -b -l -q newvalidation.C
echo "You can view your validation plots here:"
echo "http://cmsdoc.cern.ch/Physics/egamma/www/validation/${VAL_NEW_RELEASE}/vs${VAL_REF_RELEASE}/${DBS_SAMPLE}/validation.html"