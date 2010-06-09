/**
  USAGE: 
    DATA-only:    root.exe -b -l -q TnP_ICHEP_MuonID_data_all.root "plotMuonID.cxx(\"data_all\")"
    MC data-like: root.exe -b -l -q TnP_ICHEP_MuonID_datalike_mc.root  "plotMuonID.cxx(\"datalike_mc\")"
    DATA+MC:      root.exe -b -l -q TnP_ICHEP_MuonID_data_all.root TnP_ICHEP_MuonID_datalike_mc.root  "plotMuonID.cxx(\"data_vs_mc\")"

  REQUIRES:
   1) mkdir -p plots_ichep/muonid/ plots_ichep/muonid_tk/
   2) provide a suitable "tdrStyle.cc" macro or similar
      (by default, it's taken from ~/cpp/tdrstyle.cc;
       if you need one you might want to grab ~gpetrucc/cpp/tdrstyle.cc)
*/
#include <TCanvas.h>
#include <TPad.h>
#include "plotUtil.cxx"
TString prefix = "plots_ichep_dev/muonid/";
TString basedir = "histoMuFromCal";

TFile *ref = 0;

TCanvas *c1 = 0;
void plotMuonID_ICHEP(TString scenario, int fromTk=0) {
    if (fromTk) {
        prefix = "plots_ichep_dev/muonid_tk/";
        basedir = "histoMuFromTk";
    }

    prefix = prefix+scenario+"/";
    gSystem->mkdir(prefix,true);

    gROOT->ProcessLine(".x /afs/cern.ch/user/g/gpetrucc/cpp/tdrstyle.cc");
    gStyle->SetOptStat(0);
    c1 = new TCanvas("c1","c1");

    if (gROOT->GetListOfFiles()->GetEntries() == 2) {
        ref = (TFile *) gROOT->GetListOfFiles()->At(1);
        ((TFile*) gROOT->GetListOfFiles()->At(0))->cd();
    }
    plotMuonIDData();
}

void plotMuonIDData() {
    char *ids[2] = { "Glb", "TMLSAT" };
    for (size_t i = 0; i < 2; ++i) {
        TString idname(ids[i]);

        TDirectory *fit_pt_eta = gFile->GetDirectory(basedir+"/POG_"+idname+"_pt_abseta/");
        if (ref != 0) {
            TDirectory *ref_pt_eta = ref->GetDirectory(basedir+"/POG_"+idname+"_pt_abseta/");
            refstack(fit_pt_eta, ref_pt_eta, idname+"_pt_barrel",  "pt_PLOT_abseta_bin0_");
            refstack(fit_pt_eta, ref_pt_eta, idname+"_pt_endcaps", "pt_PLOT_abseta_bin1_");
        } else {
            TDirectory *mc_pt_eta  = gFile->GetDirectory(basedir+"/POG_"+idname+"_pt_abseta_mcTrue/");
            if (mc_pt_eta) {
                mcstack(fit_pt_eta, mc_pt_eta, idname+"_pt_barrel",  "pt_PLOT_abseta_bin0_");
                mcstack(fit_pt_eta, mc_pt_eta, idname+"_pt_endcaps", "pt_PLOT_abseta_bin1_");
            } else {
                single(fit_pt_eta, idname+"_pt_barrel",  "pt_PLOT_abseta_bin0_");
                single(fit_pt_eta, idname+"_pt_endcaps", "pt_PLOT_abseta_bin1_");
            }
        }

        doCanvas(fit_pt_eta, 1, 4, idname+"_barrel_pt_%d",   "abseta_bin0__pt_bin%d_");
        doCanvas(fit_pt_eta, 1, 4, idname+"_endcaps_pt_%d",  "abseta_bin1__pt_bin%d_");
    }

}

