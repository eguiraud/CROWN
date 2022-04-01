#include "ROOT/RDataFrame.hxx"
#include "RooFunctor.h"
#include "RooWorkspace.h"
#include "TFile.h"
#include "basefunctions.hxx"
#include "correction.h"
#include "utility/Logger.hxx"
#include "utility/RooFunctorThreadsafe.hxx"
/// namespace used for scale factor related functions
namespace scalefactor {
namespace muon {
/**
 * @brief Function used to evaluate id scale factors from muons
 *
 * @param df The input dataframe
 * @param pt muon pt
 * @param eta muon eta
 * @param id_output name of the id scale factor column
 * @param workspace_name path to the Rooworkspace
 * @param id_functor_name name of the function from the workspace
 * @param id_arguments arguments of the function
 * @return a new dataframe containing the new column
 */
auto id_rooworkspace(auto &df, const std::string &pt, const std::string &eta,
                     const std::string &id_output,
                     const std::string &workspace_name,
                     const std::string &id_functor_name,
                     const std::string &id_arguments) {

    Logger::get("muonsf")->debug("Setting up functions for muon sf");
    Logger::get("muonsf")->debug("ID - Function {} // argset {}",
                                 id_functor_name, id_arguments);

    const std::shared_ptr<RooFunctorThreadsafe> id_function =
        loadFunctor(workspace_name, id_functor_name, id_arguments);
    auto df1 = basefunctions::evaluateWorkspaceFunction(df, id_output,
                                                        id_function, pt, eta);
    return df1;
}
/**
 * @brief Function used to evaluate iso scale factors from muons
 *
 * @param df The input dataframe
 * @param pt muon pt
 * @param eta muon eta
 * @param iso muon iso
 * @param iso_output name of the iso scale factor column
 * @param workspace_name path to the Rooworkspace
 * @param iso_functor_name name of the function from the workspace
 * @param iso_arguments arguments of the function
 * @return a new dataframe containing the new column
 */
auto iso_rooworkspace(auto &df, const std::string &pt, const std::string &eta,
                      const std::string &iso, const std::string &iso_output,
                      const std::string &workspace_name,
                      const std::string &iso_functor_name,
                      const std::string &iso_arguments) {

    Logger::get("muonsf")->debug("Setting up functions for muon sf");
    Logger::get("muonsf")->debug("Iso - Function {} // argset {}",
                                 iso_functor_name, iso_arguments);

    const std::shared_ptr<RooFunctorThreadsafe> iso_function =
        loadFunctor(workspace_name, iso_functor_name, iso_arguments);
    auto df1 = basefunctions::evaluateWorkspaceFunction(
        df, iso_output, iso_function, pt, eta, iso);
    return df1;
}
/**
 * @brief Function used to evaluate id scale factors from muons with
 * correctionlib. Configuration:
 * - [UL2018 Muon
 * ID](https://cms-nanoaod-integration.web.cern.ch/commonJSONSFs/MUO_muon_Z_Run2_UL/MUO_muon_Z_2018_UL.html)
 * - [UL2017 Muon
 * ID](https://cms-nanoaod-integration.web.cern.ch/commonJSONSFs/MUO_muon_Z_Run2_UL/MUO_muon_Z_2017_UL.html)
 * - [UL2016preVFP Muon
 * ID](https://cms-nanoaod-integration.web.cern.ch/commonJSONSFs/MUO_muon_Z_Run2_UL/MUO_muon_Z_2016preVFP_UL.html)
 * - [UL2016postVFP Muon
 * ID](https://cms-nanoaod-integration.web.cern.ch/commonJSONSFs/MUO_muon_Z_Run2_UL/MUO_muon_Z_2016postVFP_UL.html)
 *
 * @param df The input dataframe
 * @param pt muon pt
 * @param eta muon eta
 * @param year_id id for the year of data taking and mc compaign
 * @param variation id for the variation of the scale factor "sf" for nominal
 * and "systup"/"systdown" for up/down variation
 * @param id_output name of the id scale factor column
 * @param sf_file path to the file with the muon scale factors
 * @param sf_name name of the muon id scale factor
 * @return a new dataframe containing the new column
 */
auto id(auto &df, const std::string &pt, const std::string &eta,
        const std::string &year_id, const std::string &variation, 
        const std::string &id_output, const std::string &sf_file,
        const std::string &sf_name) {

    Logger::get("muonIdSF")->debug("Setting up functions for muon id sf");
    Logger::get("muonIdSF")->debug("ID - Name {}", sf_name);
    auto evaluator = correction::CorrectionSet::from_file(sf_file)->at(sf_name);
    auto df1 = df.Define(
        id_output,
        [evaluator, year_id, variation](const float &pt, const float &eta) {
            Logger::get("muonIdSF")->debug("ID - pt {}, eta {}", pt, eta);
            double sf = 1.;
            // preventing muons with default values due to tau energy correction
            // shifts below good tau pt selection
            if (pt >= 0.0 && std::abs(eta) >= 0.0) {
                sf = evaluator->evaluate(
                    {year_id, std::abs(eta), pt, variation});
            }
            return sf;
        },
        {pt, eta});
    return df1;
}
/**
 * @brief Function used to evaluate iso scale factors from muons with
 * correctionlib. Configurations:
 * - [UL2018 Muon
 * Iso](https://cms-nanoaod-integration.web.cern.ch/commonJSONSFs/MUO_muon_Z_Run2_UL/MUO_muon_Z_2018_UL.html)
 * - [UL2017 Muon
 * Iso](https://cms-nanoaod-integration.web.cern.ch/commonJSONSFs/MUO_muon_Z_Run2_UL/MUO_muon_Z_2017_UL.html)
 * - [UL2016preVFP Muon
 * Iso](https://cms-nanoaod-integration.web.cern.ch/commonJSONSFs/MUO_muon_Z_Run2_UL/MUO_muon_Z_2016preVFP_UL.html)
 * - [UL2016postVFP Muon
 * Iso](https://cms-nanoaod-integration.web.cern.ch/commonJSONSFs/MUO_muon_Z_Run2_UL/MUO_muon_Z_2016postVFP_UL.html)
 *
 * @param df The input dataframe
 * @param pt muon pt
 * @param eta muon eta
 * @param year_id id for the year of data taking and mc compaign
 * @param variation id for the variation of the scale factor "sf" for nominal
 * and "systup"/"systdown" the up/down variation
 * @param iso_output name of the iso scale factor column
 * @param sf_file path to the file with the muon scale factors
 * @param sf_name name of the muon iso scale factor
 * @return a new dataframe containing the new column
 */
auto iso(auto &df, const std::string &pt, const std::string &eta,
         const std::string &year_id, const std::string &variation,
         const std::string &iso_output, const std::string &sf_file,
         const std::string &sf_name) {

    Logger::get("muonIsoSF")->debug("Setting up functions for muon iso sf");
    Logger::get("muonIsoSF")->debug("ISO - Name {}", sf_name);
    auto evaluator = correction::CorrectionSet::from_file(sf_file)->at(sf_name);
    auto df1 = df.Define(
        iso_output,
        [evaluator, year_id, variation](const float &pt, const float &eta) {
            Logger::get("muonIsoSF")->debug("ISO - pt {}, eta {}", pt, eta);
            double sf = 1.;
            // preventing muons with default values due to tau energy correction
            // shifts below good tau pt selection
            if (pt >= 0.0 && std::abs(eta) >= 0.0) {
                sf = evaluator->evaluate(
                    {year_id, std::abs(eta), pt, variation});
            }
            return sf;
        },
        {pt, eta});
    return df1;
}
} // namespace muon
namespace tau {
/**
 * @brief Function used to evaluate vsJets tau id scale factors with
 * correctionlib

Description of the bit map used to define the tau id working points of the
DeepTau2017v2p1 tagger.
vsJets                              | Value | Bit (value used in the config)
------------------------------------|-------|-------
no ID selection (takes every tau)   |  0    | -
VVVLoose                            |  1    | 1
VVLoose                             |  2    | 2
VLoose                              |  4    | 3
Loose                               |  8    | 4
Medium                              |  16   | 5
Tight                               |  32   | 6
VTight                              |  64   | 7
VVTight                             |  128  | 8
 * @param df The input dataframe
 * @param pt tau pt
 * @param decayMode decay mode of the tau
 * @param genMatch column with genmatch values (from prompt e, prompt mu,
 * tau->e, tau->mu, had. tau)
 * @param selectedDMs list of allowed decay modes for which a scale factor
 * should be calculated
 * @param wp working point of the ID cut
 * @param variation_jet id for the variation of the scale factor "sf" for nominal
 * and "systup"/"systdown" the up/down variation
 * @param sf_dependence "pt", "dm" or "eta" based scale factors
 * @param id_output name of the id scale factor column
 * @param sf_file path to the file with the tau scale factors
 * @param sf_name name of the tau id scale factor
 * @return a new dataframe containing the new column
 */
auto id_vsJet(auto &df, const std::string &pt, const std::string &decayMode,
              const std::string &genMatch, const std::vector<int> &selectedDMs,
              const std::string &wp, const std::string &variation_jet,
              const std::string &sf_dependence, const std::string &id_output,
              const std::string &sf_file, const std::string &sf_name) {

    Logger::get("TauIDvsJetSF")
        ->debug("Setting up function for tau id vsJet sf");
    Logger::get("TauIDvsJetSF")->debug("ID - Name {}", sf_name);
    auto evaluator = correction::CorrectionSet::from_file(sf_file)->at(sf_name);
    auto idSF_calculator = [evaluator, wp, variation_jet, sf_dependence,
                            selectedDMs,
                            sf_name](const float &pt, const int &decayMode,
                                     const UChar_t &genMatch) {
        double sf = 1.;
        Logger::get("TauIDvsJetSF")->debug("ID - decayMode {}", decayMode);
        // only calculate SFs for allowed tau decay modes (also excludes default
        // values due to tau energy correction shifts below good tau pt
        // selection)
        if (std::find(selectedDMs.begin(), selectedDMs.end(), decayMode) !=
            selectedDMs.end()) {
            Logger::get("TauIDvsJetSF")
                ->debug("ID {} - pt {}, decayMode {}, genMatch {}, wp {}, "
                        "variation {}, "
                        "sf_dependence {}",
                        sf_name, pt, decayMode, genMatch, wp, variation_jet,
                        sf_dependence);
            sf = evaluator->evaluate({pt, decayMode, static_cast<int>(genMatch),
                                      wp, variation_jet, sf_dependence});
            Logger::get("TauIDvsJetSF")->debug("Scale Factor {}", sf);
        }

        return sf;
    };
    auto df1 = df.Define(id_output, idSF_calculator, {pt, decayMode, genMatch});
    return df1;
}
/**
 * @brief Function used to evaluate vsEle tau id scale factors with
 * correctionlib

Description of the bit map used to define the tau id working points of the
DeepTau2017v2p1 tagger.
vsElectrons                         | Value | Bit (value used in the config)
------------------------------------|-------|-------
no ID selection (takes every tau)   |  0    | -
VVVLoose                            |  1    | 1
VVLoose                             |  2    | 2
VLoose                              |  4    | 3
Loose                               |  8    | 4
Medium                              |  16   | 5
Tight                               |  32   | 6
VTight                              |  64   | 7
VVTight                             |  128  | 8

vsMuons                             | Value | Bit (value used in the config)
------------------------------------|-------|-------
no ID selection (takes every tau)   |  0    | -
VLoose                              |  1    | 1
Loose                               |  2    | 2
Medium                              |  4    | 3
Tight                               |  8    | 4
 * @param df The input dataframe
 * @param eta tau eta
 * @param decayMode decay mode of the tau
 * @param genMatch column with genmatch values (from prompt e, prompt mu,
 * tau->e, tau->mu, had. tau)
 * @param selectedDMs list of allowed decay modes for which a scale factor
 * should be calculated
 * @param wp working point of the ID cut
 * @param variation_ele id for the variation of the scale factor "sf" for nominal
 * and "systup"/"systdown" the up/down variation
 * @param id_output name of the id scale factor column
 * @param sf_file path to the file with the tau scale factors
 * @param sf_name name of the tau id scale factor
 * @return a new dataframe containing the new column
 */
auto id_vsEle(auto &df, const std::string &eta, const std::string &decayMode,
                const std::string &genMatch,
                const std::vector<int> &selectedDMs, const std::string &wp,
                const std::string &variation_ele, const std::string &id_output,
                const std::string &sf_file, const std::string &sf_name) {

    Logger::get("TauIDvsLepSF")
        ->debug("Setting up function for tau id vsEle sf");
    Logger::get("TauIDvsLepSF")->debug("ID - Name {}", sf_name);
    auto evaluator = correction::CorrectionSet::from_file(sf_file)->at(sf_name);
    auto idSF_calculator = [evaluator, wp, variation_ele, selectedDMs,
                            sf_name](const float &eta, const int &decayMode,
                                     const UChar_t &genMatch) {
        double sf = 1.;
        Logger::get("TauIDvsLepSF")->debug("ID - decayMode {}", decayMode);
        // only calculate SFs for allowed tau decay modes (also excludes
        // default values due to tau energy correction shifts below good tau
        // pt selection)
        if (std::find(selectedDMs.begin(), selectedDMs.end(), decayMode) !=
            selectedDMs.end()) {
            Logger::get("TauIDvsLepSF")
                ->debug("ID {} - eta {}, genMatch {}, wp {}, variation {} ",
                        sf_name, eta, genMatch, wp, variation_ele);
            sf = evaluator->evaluate(
                {std::abs(eta), static_cast<int>(genMatch), wp, variation_ele});
            Logger::get("TauIDvsLepSF")->debug("Scale Factor {}", sf);
        }
        return sf;
    };
    auto df1 =
        df.Define(id_output, idSF_calculator, {eta, decayMode, genMatch});
    return df1;
}
/**
 * @brief Function used to evaluate vsMu tau id scale factors with
 * correctionlib

Description of the bit map used to define the tau id working points of the
DeepTau2017v2p1 tagger.
vsElectrons                         | Value | Bit (value used in the config)
------------------------------------|-------|-------
no ID selection (takes every tau)   |  0    | -
VVVLoose                            |  1    | 1
VVLoose                             |  2    | 2
VLoose                              |  4    | 3
Loose                               |  8    | 4
Medium                              |  16   | 5
Tight                               |  32   | 6
VTight                              |  64   | 7
VVTight                             |  128  | 8

vsMuons                             | Value | Bit (value used in the config)
------------------------------------|-------|-------
no ID selection (takes every tau)   |  0    | -
VLoose                              |  1    | 1
Loose                               |  2    | 2
Medium                              |  4    | 3
Tight                               |  8    | 4
 * @param df The input dataframe
 * @param eta tau eta
 * @param decayMode decay mode of the tau
 * @param genMatch column with genmatch values (from prompt e, prompt mu,
 * tau->e, tau->mu, had. tau)
 * @param selectedDMs list of allowed decay modes for which a scale factor
 * should be calculated
 * @param wp working point of the ID cut
 * @param variation_mu id for the variation of the scale factor "sf" for nominal
 * and "systup"/"systdown" the up/down variation
 * @param id_output name of the id scale factor column
 * @param sf_file path to the file with the tau scale factors
 * @param sf_name name of the tau id scale factor
 * @return a new dataframe containing the new column
 */
auto id_vsMu(auto &df, const std::string &eta, const std::string &decayMode,
                const std::string &genMatch,
                const std::vector<int> &selectedDMs, const std::string &wp,
                const std::string &variation_mu, const std::string &id_output,
                const std::string &sf_file, const std::string &sf_name) {

    Logger::get("TauIDvsLepSF")
        ->debug("Setting up function for tau id vsMu sf");
    Logger::get("TauIDvsLepSF")->debug("ID - Name {}", sf_name);
    auto evaluator = correction::CorrectionSet::from_file(sf_file)->at(sf_name);
    auto idSF_calculator = [evaluator, wp, variation_mu, selectedDMs,
                            sf_name](const float &eta, const int &decayMode,
                                     const UChar_t &genMatch) {
        double sf = 1.;
        Logger::get("TauIDvsLepSF")->debug("ID - decayMode {}", decayMode);
        // only calculate SFs for allowed tau decay modes (also excludes
        // default values due to tau energy correction shifts below good tau
        // pt selection)
        if (std::find(selectedDMs.begin(), selectedDMs.end(), decayMode) !=
            selectedDMs.end()) {
            Logger::get("TauIDvsLepSF")
                ->debug("ID {} - eta {}, genMatch {}, wp {}, variation {} ",
                        sf_name, eta, genMatch, wp, variation_mu);
            sf = evaluator->evaluate(
                {std::abs(eta), static_cast<int>(genMatch), wp, variation_mu});
            Logger::get("TauIDvsLepSF")->debug("Scale Factor {}", sf);
        }
        return sf;
    };
    auto df1 =
        df.Define(id_output, idSF_calculator, {eta, decayMode, genMatch});
    return df1;
}
} // namespace tau

namespace electron {
/**
 * @brief Function used to evaluate id scale factors of electrons with
 * correctionlib, configurations:
 * - [UL2018 Electron
 * ID](https://cms-nanoaod-integration.web.cern.ch/commonJSONSFs/EGM_electron_Run2_UL/EGM_electron_2018_UL.html)
 * - [UL2017 Electron
 * ID](https://cms-nanoaod-integration.web.cern.ch/commonJSONSFs/EGM_electron_Run2_UL/EGM_electron_2017_UL.html)
 * - [UL2016preVFP Electron
 * ID](https://cms-nanoaod-integration.web.cern.ch/commonJSONSFs/EGM_electron_Run2_UL/EGM_electron_2016preVFP_UL.html)
 * - [UL2016postVFP Electron
 * ID](https://cms-nanoaod-integration.web.cern.ch/commonJSONSFs/EGM_electron_Run2_UL/EGM_electron_2016postVFP_UL.html)
 * @param df The input dataframe
 * @param pt electron pt
 * @param eta electron eta
 * @param year_id id for the year of data taking and mc compaign
 * @param wp wp of the electron id
 * @param variation id for the variation of the scale factor. Available Values:
 * sf, sfdown, sfup
 * @param id_output name of the id scale factor column
 * @param sf_file path to the file with the electron scale factors
 * @param sf_name name of the electron id scale factor
 * @return a new dataframe containing the new column
 */
auto id(auto &df, const std::string &pt, const std::string &eta,
        const std::string &year_id, const std::string &wp,
        const std::string &variation, const std::string &id_output,
        const std::string &sf_file, const std::string &sf_name) {

    Logger::get("electronIDSF")
        ->debug("Setting up functions for electron id sf with correctionlib");
    Logger::get("electronIDSF")->debug("ID - Name {}", sf_name);
    auto evaluator = correction::CorrectionSet::from_file(sf_file)->at(sf_name);
    auto df1 = df.Define(
        id_output,
        [evaluator, year_id, sf_name, wp, variation](const float &pt,
                                                     const float &eta) {
            Logger::get("electronIDSF")
                ->debug("Year {}, Name {}, WP {}", year_id, sf_name, wp);
            Logger::get("electronIDSF")->debug("ID - pt {}, eta {}", pt, eta);
            double sf = 1.;
            if (pt >= 0.0) {
                sf = evaluator->evaluate({year_id, variation, wp, eta, pt});
            }
            Logger::get("electronIDSF")->debug("Scale Factor {}", sf);
            return sf;
        },
        {pt, eta});
    return df1;
}
} // namespace electron
} // namespace scalefactor