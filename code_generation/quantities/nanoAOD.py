from code_generation.quantity import NanoAODQuantity

run = NanoAODQuantity("run")
luminosityBlock = NanoAODQuantity("luminosityBlock")
event = NanoAODQuantity("event")

Tau_pt = NanoAODQuantity("Tau_pt")
Tau_eta = NanoAODQuantity("Tau_eta")
Tau_phi = NanoAODQuantity("Tau_phi")
Tau_mass = NanoAODQuantity("Tau_mass")
Tau_dz = NanoAODQuantity("Tau_dz")
Tau_dxy = NanoAODQuantity("Tau_dxy")
Tau_charge = NanoAODQuantity("Tau_charge")
Tau_decayMode = NanoAODQuantity("Tau_decayMode")
Tau_genMatch = NanoAODQuantity("Tau_genPartFlav")
Tau_IDraw = NanoAODQuantity("Tau_rawDeepTau2017v2p1VSjet")
Tau_indexToGen = NanoAODQuantity("Tau_genPartIdx")
Tau_associatedJet = NanoAODQuantity("Tau_jetIdx")

Muon_pt = NanoAODQuantity("Muon_pt")
Muon_eta = NanoAODQuantity("Muon_eta")
Muon_phi = NanoAODQuantity("Muon_phi")
Muon_mass = NanoAODQuantity("Muon_mass")
Muon_iso = NanoAODQuantity("Muon_pfRelIso04_all")
Muon_dz = NanoAODQuantity("Muon_dz")
Muon_dxy = NanoAODQuantity("Muon_dxy")
Muon_charge = NanoAODQuantity("Muon_charge")
Muon_genMatch = NanoAODQuantity("Muon_genPartFlav")
Muon_indexToGen = NanoAODQuantity("Muon_genPartIdx")

Electron_pt = NanoAODQuantity("Electron_pt")
Electron_eta = NanoAODQuantity("Electron_eta")
Electron_dxy = NanoAODQuantity("Electron_dxy")
Electron_dz = NanoAODQuantity("Electron_dz")
Electron_phi = NanoAODQuantity("Electron_phi")
Electron_mass = NanoAODQuantity("Electron_mass")
Electron_iso = NanoAODQuantity("Electron_pfRelIso03_all")
Electron_charge = NanoAODQuantity("Electron_charge")

GenJet_pt = NanoAODQuantity("GenJet_pt")
GenJet_eta = NanoAODQuantity("GenJet_eta")
GenJet_phi = NanoAODQuantity("GenJet_phi")

Jet_eta = NanoAODQuantity("Jet_eta")
Jet_phi = NanoAODQuantity("Jet_phi")
Jet_pt = NanoAODQuantity("Jet_pt")
Jet_mass = NanoAODQuantity("Jet_mass")
Jet_ID = NanoAODQuantity("Jet_jetId")
Jet_associatedGenJet = NanoAODQuantity("Jet_genJetIdx")
BJet_discriminator = NanoAODQuantity("Jet_btagDeepFlavB")

Pileup_nTrueInt = NanoAODQuantity("Pileup_nTrueInt")
rho = NanoAODQuantity("Pileup_pudensity")

GenParticle_eta = NanoAODQuantity("GenPart_eta")
GenParticle_phi = NanoAODQuantity("GenPart_phi")
GenParticle_pt = NanoAODQuantity("GenPart_pt")
GenParticle_mass = NanoAODQuantity("GenPart_mass")
GenParticle_pdgId = NanoAODQuantity("GenPart_pdgId")
GenParticle_status = NanoAODQuantity("GenPart_status")
GenParticle_statusFlags = NanoAODQuantity("GenPart_statusFlags")

## Trigger Objects
TriggerObject_bit = NanoAODQuantity("TrigObj_filterBits")
TriggerObject_pt = NanoAODQuantity("TrigObj_pt")
TriggerObject_eta = NanoAODQuantity("TrigObj_eta")
TriggerObject_phi = NanoAODQuantity("TrigObj_phi")
TriggerObject_id = NanoAODQuantity("TrigObj_id")

## HTXS quantities
HTXS_Higgs_pt = NanoAODQuantity("HTXS_Higgs_pt")
HTXS_njets30 = NanoAODQuantity("HTXS_njets30")
HTXS_stage_0 = NanoAODQuantity("HTXS_stage_0")
HTXS_stage_1_pTjet30 = NanoAODQuantity("HTXS_stage_1_pTjet30")
HTXS_stage1_1_fine_cat_pTjet30GeV = NanoAODQuantity("HTXS_stage1_1_fine_cat_pTjet30GeV")
HTXS_stage1_2_cat_pTjet30GeV = NanoAODQuantity("HTXS_stage1_2_cat_pTjet30GeV")
HTXS_stage1_2_fine_cat_pTjet30GeV = NanoAODQuantity("HTXS_stage1_2_fine_cat_pTjet30GeV")

## MET quantities
## TODO Swich to Puppi versions for METCOV and Signifiance as soon as they are in the nanoAOD
MET_covXX = NanoAODQuantity("MET_covXX")
MET_covXY = NanoAODQuantity("MET_covXY")
MET_covYY = NanoAODQuantity("MET_covYY")
MET_significance = NanoAODQuantity("MET_significance")

MET_phi = NanoAODQuantity("PuppiMET_phi")
MET_pt = NanoAODQuantity("PuppiMET_pt")
MET_sumEt = NanoAODQuantity("PuppiMET_sumEt")
