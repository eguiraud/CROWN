import code_generation.quantities.output as q
import code_generation.quantities.nanoAOD as nanoAOD
from code_generation.producer import Producer, VectorProducer, ProducerGroup
from code_generation.producers.electrons import DiElectronVeto
from code_generation.producers.muons import DiMuonVeto

####################
# Set of general producers for event quantities
####################

RunLumiEventFilter = VectorProducer(
    name="RunLumiEventFilter",
    call='basefunctions::FilterIntSelection<{RunLumiEventFilter_Quantity_Types}>({df}, "{RunLumiEventFilter_Quantities}", {vec_open}{RunLumiEventFilter_Selections}{vec_close}, "RunLumiEventFilter")',
    input=[],
    output=None,
    scopes=["general"],
    vec_configs=[
        "RunLumiEventFilter_Quantities",
        "RunLumiEventFilter_Quantity_Types",
        "RunLumiEventFilter_Selections",
    ],
)
MetFilter = VectorProducer(
    name="MetFilter",
    call='metfilter::ApplyMetFilter({df}, "{met_filters}", "{met_filters}")',
    input=[],
    output=None,
    scopes=["global"],
    vec_configs=["met_filters"],
)

Lumi = Producer(
    name="Lumi",
    call="basefunctions::rename<UInt_t>({df}, {input}, {output})",
    input=[nanoAOD.luminosityBlock],
    output=[q.lumi],
    scopes=["global"],
)

PUweights = Producer(
    name="PUweights",
    call='reweighting::puweights({df}, {output}, {input}, "{PU_reweighting_file}", "{PU_reweighting_hist}")',
    input=[nanoAOD.Pileup_nTrueInt],
    output=[q.puweight],
    scopes=["global"],
)

ZPtMassReweighting = Producer(
    name="ZPtMassReweighting",
    call='reweighting::zPtMassReweighting({df}, {output}, {input}, "{zptmass_file}", "{zptmass_functor}", "{zptmass_arguments}")',
    input=[
        q.recoil_genboson_p4,
    ],
    output=[q.ZPtMassReweightWeight],
    scopes=["global", "em", "et", "mt", "tt"],
)

TopPtReweighting = Producer(
    name="TopPtReweighting",
    call="reweighting::topptreweighting({df}, {output}, {input})",
    input=[
        nanoAOD.GenParticle_pdgId,
        nanoAOD.GenParticle_statusFlags,
        nanoAOD.GenParticle_pt,
    ],
    output=[q.topPtReweightWeight],
    scopes=["global", "em", "et", "mt", "tt"],
)

DiLeptonVeto = ProducerGroup(
    name="DiLeptonVeto",
    call="basefunctions::CombineFlagsAny({df}, {output}, {input})",
    input=[],
    output=[q.dilepton_veto],
    scopes=["global"],
    subproducers=[DiElectronVeto, DiMuonVeto],
)

GGH_NNLO_Reweighting = Producer(
    name="GGH_NNLO_Reweighting",
    call='htxs::ggHNLLOWeights({df}, {output}, "{ggHNNLOweightsRootfile}", "{ggH_generator}", {input})',
    input=[nanoAOD.HTXS_Higgs_pt, nanoAOD.HTXS_njets30],
    output=[q.ggh_NNLO_weight],
    scopes=["global", "em", "et", "mt", "tt"],
)

GGH_WG1_Uncertainties = Producer(
    name="GGH_WG1_Uncertainties",
    call="htxs::ggH_WG1_uncertainties({df}, {output_vec}, {input})",
    input=[
        nanoAOD.HTXS_stage_1_pTjet30,
        nanoAOD.HTXS_Higgs_pt,
        nanoAOD.HTXS_njets30,
    ],  # using non-updated stage1 flag required by the used macro
    output=[
        q.THU_ggH_Mu,
        q.THU_ggH_Res,
        q.THU_ggH_Mig01,
        q.THU_ggH_Mig12,
        q.THU_ggH_VBF2j,
        q.THU_ggH_VBF3j,
        q.THU_ggH_PT60,
        q.THU_ggH_PT120,
        q.THU_ggH_qmtop,
    ],
    scopes=["global", "em", "et", "mt", "tt"],
)

QQH_WG1_Uncertainties = Producer(
    name="QQH_WG1_Uncertainties",
    call="htxs::qqH_WG1_uncertainties({df}, {output_vec}, {input})",
    input=[
        nanoAOD.HTXS_stage1_1_fine_cat_pTjet30GeV
    ],  # using fine stage1.1 flag required by the used macro
    output=[
        q.THU_qqH_TOT,
        q.THU_qqH_PTH200,
        q.THU_qqH_Mjj60,
        q.THU_qqH_Mjj120,
        q.THU_qqH_Mjj350,
        q.THU_qqH_Mjj700,
        q.THU_qqH_Mjj1000,
        q.THU_qqH_Mjj1500,
        q.THU_qqH_25,
        q.THU_qqH_JET01,
    ],
    scopes=["global", "em", "et", "mt", "tt"],
)
