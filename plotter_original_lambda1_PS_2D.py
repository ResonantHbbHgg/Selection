#!/usr/bin/env python
#// Small dumb code to play with trees
#// O.Bondu, F. Bojarski (May 2014)
# Various python imports
from os import path
from math import log10, pow
# ROOT setup
import ROOT
from ROOT import TFile, TTree, TLine, TChain, TCanvas, TH1D, TLatex, TLegend, TLorentzVector
ROOT.gROOT.Reset()
ROOT.gROOT.SetBatch()
ROOT.gROOT.ProcessLine(".x setTDRStyle.C")
ROOT.TGaxis.SetMaxDigits(3);

c1 = TCanvas()
afs_plottree = "/afs/cern.ch/user/o/obondu/public/forAkanksha/2014-06-25_selection_noRegression_noMassCut_v15"

intL = 19706.
samples = []
# samples.append([ name, dirpath, subdir, file, tree, color, style, label , sigma , N])
#samples.append(["ggHH_8TeV", "", "2014-05-20_selection_noRegression_noMassCut_v10_hhh", "ggHH_8TeV_noRegression_noMassCut_v10_hhh.root", "ggHH_8TeV", ROOT.kRed, 0, "ggHH", 0.0000212, 96880])
#samples.append(["diphojet_sherpa_8TeV", "", "2014-05-20_selection_noRegression_noMassCut_v10_hhh", "diphojet_sherpa_8TeV_noRegression_noMassCut_v10_hhh.root", "diphojet_sherpa_8TeV", ROOT.kBlue, 0, "QCD #gamma#gamma bb", 0.1, 96880])

samples.append(["ggHH1_8TeV", "", "../LHEAnalysis", "original_gen_1.root", "test", "1", ROOT.kBlue, 0, "ggHH, #lambda = 1", 0.0000212, 100000])
samples.append(["ggHH_8TeV", afs_plottree, "", "ggHH_8TeV_noRegression_noMassCut_v15.root", "ggHH_8TeV", "1",  ROOT.kMagenta, 0, "ggHH, SM", 0.0000212, 96880])

#####plots.append([ name2, variable, cut, norm, binning, title, additional_info, cutline, cutline2 ])
plots = []

hDivided = []

#gamma1 and gamma2 (lhe2root.py)
plots.append(["2D_PS_#lambda1_gr_g1_p4_pt", "gr_g1_p4_pt", "", 1., "(50, 0, 500)", "p_{T}^{#gamma1} (GeV)", "", "", ""])
plots.append(["2D_PS_#lambda1_gr_g1_p4_eta", "gr_g1_p4_eta", "", 1., "(50, -4, 4)", "eta^{#gamma1} ", "", "", ""])
plots.append(["2D_PS_#lambda1_gr_g1_p4_phi", "gr_g1_p4_phi", "", 1., "(50, -4, 4)", "phi^{#gamma1} (radian)", "", "", ""])
plots.append(["2D_PS_#lambda1_gr_g1_p4_mass", "gr_g1_p4_mass", "", 1., "(100, 0, 200)", "mass^{#gamma1} (GeV", "", "", ""])
plots.append(["2D_PS_#lambda1_gr_g2_p4_pt", "gr_g2_p4_pt", "", 1., "(50, 0, 500)", "p_{T}^{#gamma1} (GeV)", "", "", ""])
plots.append(["2D_PS_#lambda1_gr_g2_p4_eta", "gr_g2_p4_eta", "", 1., "(50, -4, 4)", "eta^{#gamma1} ", "", "", ""])
plots.append(["2D_PS_#lambda1_gr_g2_p4_phi", "gr_g2_p4_phi", "", 1., "(50, -4, 4)", "phi^{#gamma1} (radian)", "", "", ""])
plots.append(["2D_PS_#lambda1_gr_g2_p4_mass", "gr_g2_p4_mass", "", 1., "(100, 0, 200)", "mass^{#gamma1} (GeV)", "", "", ""])

# b quark and anti b quark (lhe2root.py)
plots.append(["2D_PS_#lambda1_gr_b1_p4_pt", "gr_b1_p4_pt", "", 1., "(50, 0, 500)", "p_{T}^{#b1} (GeV)", "", "", ""])
plots.append(["2D_PS_#lambda1_gr_b1_p4_eta", "gr_b1_p4_eta", "", 1., "(50, -4, 4)", "eta^{#b1} ", "", "", ""])
plots.append(["2D_PS_#lambda1_gr_b1_p4_phi", "gr_b1_p4_phi", "", 1., "(50, -4, 4)", "phi^{#b1} (radian)", "", "", ""])
plots.append(["2D_PS_#lambda1_gr_b1_p4_mass", "gr_b1_p4_mass", "", 1., "(100, 0, 200)", "mass^{#b1} (GeV", "", "", ""])
plots.append(["2D_PS_#lambda1_gr_b2_p4_pt", "gr_b2_p4_pt", "", 1., "(50, 0, 500)", "p_{T}^{#b2} (GeV)", "", "", ""])
plots.append(["2D_PS_#lambda1_gr_b2_p4_eta", "gr_b2_p4_eta", "", 1., "(50, -4, 4)", "eta^{#b2} ", "", "", ""])
plots.append(["2D_PS_#lambda1_gr_b2_p4_phi", "gr_b2_p4_phi", "", 1., "(50, -4, 4)", "phi^{#b2} (radian)", "", "", ""])
plots.append(["2D_PS_#lambda1_gr_b2_p4_mass", "gr_b2_p4_mass", "", 1., "(100, 0, 200)", "mass^{#b2} (GeV)", "", "", ""])

# hgg and hbb (lhe2root.py)
plots.append(["2D_PS_#lambda1_gr_hgg_p4_pt", "gr_hgg_p4_pt", "", 1., "(50, 0, 500)", "p_{T}^{#hgg} (GeV)", "", "", ""])
plots.append(["2D_PS_#lambda1_gr_hgg_p4_eta", "gr_hgg_p4_eta", "", 1., "(50, -4, 4)", "eta^{#hgg} ", "", "", ""])
plots.append(["2D_PS_#lambda1_gr_hgg_p4_phi", "gr_hgg_p4_phi", "", 1., "(50, -4, 4)", "phi^{#hgg} (radian)", "", "", ""])
plots.append(["2D_PS_#lambda1_gr_hgg_p4_mass", "gr_hgg_p4_mass", "", 1., "(100, 0, 200)", "mass^{#hgg} (GeV", "", "", ""])
plots.append(["2D_PS_#lambda1_gr_hbb_p4_pt", "gr_hbb_p4_pt", "", 1., "(50, 0, 500)", "p_{T}^{#hbb} (GeV)", "", "", ""])
plots.append(["2D_PS_#lambda1_gr_hbb_p4_eta", "gr_hbb_p4_eta", "", 1., "(50, -4, 4)", "eta^{#hbb} ", "", "", ""])
plots.append(["2D_PS_#lambda1_gr_hbb_p4_phi", "gr_hbb_p4_phi", "", 1., "(50, -4, 4)", "phi^{#hbb} (radian)", "", "", ""])
plots.append(["2D_PS_#lambda1_gr_hbb_p4_mass", "gr_hbb_p4_mass", "", 1., "(100, 0, 200)", "mass^{#hbb} (GeV)", "", "", ""])

plots.append(["2D_PS_#lambda1_gr_costhetastar", "fabs(gr_hbbhgg_costhetastar_CS)", "", 1., "(20, 0, 1)", "|cos#theta*|^{#hh}", "", "", ""])
#plots.append(["2D_PS_#lambda1_gr_delta_eta", "dEta_gg_jj", "", 1., "(50, -6, 6)", "#Delta#eta^{#hh}", "", "", ""])
#plots.append(["2D_PS_#lambda1_gr_delta_phi", "dPhi_gg_jj", "", 1., "(50, -6, 6)", "#Delta#phi^{#hh}", "", "", ""])

# dividing the plots

# increased range of pt
#plots.append(["gr_g1_p4_pt", "gr_g1_p4_pt", "", 1., "(500, 0, 1000)", "p_{T}^{#gamma1} (GeV)", "", "", ""])
#plots.append(["gr_g2_p4_pt", "gr_g2_p4_pt", "", 1., "(500, 0, 1000)", "p_{T}^{#gamma2} (GeV)", "", "", ""])
#plots.append(["gr_b1_p4_pt", "gr_b1_p4_pt", "", 1., "(500, 0, 1000)", "p_{T}^{#b1} (GeV)", "", "", ""])
#plots.append(["gr_b2_p4_pt", "gr_b2_p4_pt", "", 1., "(500, 0, 1000)", "p_{T}^{#b2} (GeV)", "", "", ""])
#plots.append(["gr_hgg_p4_pt", "gr_hgg_p4_pt", "", 1., "(500, 0, 1000)", "p_{T}^{#hgg} (GeV)", "", "", ""])
#plots.append(["gr_hbb_p4_pt", "gr_hbb_p4_pt", "", 1., "(500, 0, 1000)", "p_{T}^{#hbb} (GeV)", "", "", ""])

for iplot, [name2, variable, cut, norm, binning, title, additional_info, cutline, cutline2] in enumerate(plots):
    c1 = TCanvas()
    legend = TLegend(0.45, 0.82, 0.90, 0.93, "")
    legend.SetTextSize(0.025)
    legend.SetFillColor(ROOT.kWhite)
    legend.SetLineColor(ROOT.kWhite)
    legend.SetShadowColor(ROOT.kWhite)
    if len(samples) > 1:
        legend.SetNColumns(2)
    xnbin, xlow, xhigh = map(float, binning.strip().strip("()").split(","))
    ymax = -1
    ymin = 10000000
    firsthistname = ""
    if cut == "": cut = "1"

    hDivided = []
    

    for ifile, [ name, dirpath, subdir, file, tree, sample_weight, color, style, label , sigma , N] in enumerate(samples):
#        print ifile, file, color, style, label
        chain = TChain(tree)
        chain.Add( path.join(dirpath, subdir, file) )
        sample_cut = cut
        if norm == 1.:
            sample_cut = "((" + sample_cut + ") * (" + sample_weight + "))/" + str( chain.GetEntries() )
        else:
            sample_cut = "((" + sample_cut + ") * (" + sample_weight + ")) * (" + str(sigma) + " * " + str(intL) + ")/" + str(N)
        option = ""
        print sample_cut
        if ifile != 0:
            option = "same"
        chain.Draw(variable + ">>h_tmp" + binning, sample_cut, option)
  
        # Clsosmetics
        h = ROOT.gDirectory.Get("h_tmp")
        h.SetName(name + "_" + name2 + "_" + str(ifile))
        if ifile == 0:
            firsthistname = name + "_" + name2 + "_" + str(ifile)
        h.SetLineWidth(3)
        h.SetLineColor(color)
        h.SetFillColor(color)
        h.SetFillStyle(style)
        h.GetXaxis().SetTitle( title )       

        unit = ""
        if title.find("(") != -1:
            unit = title[title.find("(")+1:title.find(")")]
        if norm == 1.:
            h.GetYaxis().SetTitle( "Norm. to unity / ( " + str(((xhigh - xlow) / xnbin)) + " " + unit + " )")
        else:
            h.GetYaxis().SetTitle( "# events / ( " + str(((xhigh - xlow) / xnbin)) + " " + unit + " )")
        legend.AddEntry(h.GetName(), label)
        ymax = max(ymax, h.GetMaximum())
        ymin = min(ymin, h.GetMinimum(0.0))
        del chain, h



    ymin_lin = ymin / 10.
    yrange_lin = ymax - ymin_lin
    ymax_lin = .25 * yrange_lin + ymax
    yrange_log = (log10(ymax) - log10(ymin)) / .77
    ymax_log = pow(10., .25*yrange_log + log10(ymax))
    ymin_log = pow(10., log10(ymin) - .03*yrange_log)

    latexLabel = TLatex()
    latexLabel.SetTextSize(.03)
    latexLabel.SetNDC()
    latexLabel.DrawLatex(.25, .96, "CMS Internal     L = 19.7 fb^{-1}     #sqrt{s} = 8 TeV")
    latexLabel.DrawLatex(.28, .79, "lambda = 1 (gen level) and ggHH after parton showering" )
    ROOT.gPad.RedrawAxis()
    legend.Draw()
    c1.Update()

    line = TLine()
    line.SetLineStyle(2)
    line.SetLineWidth(2)
    line2 = TLine()
    line2.SetLineStyle(2)
    line2.SetLineWidth(2)

    h = ROOT.gDirectory.Get(firsthistname)
    h.SetMaximum(ymax_lin)
    h.SetMinimum(ymin_lin)
    if cutline != "":
        line.SetX1(cutline); line.SetY1(ymin_lin); line.SetX2(cutline); line.SetY2(ymax)
        line.Draw("same")
    if cutline2 != "":
        line2.SetX1(cutline2); line2.SetY1(ymin_lin); line2.SetX2(cutline2); line2.SetY2(ymax)
        line2.Draw("same")
    c1.Update()
    c1.Print("unweighted_original_plots_pdf/" + name2 + ".pdf")
    c1.Print("unweighted_original_plots_gif/" + name2 + ".gif")
    c1.Print("unweighted_original_plots_root/" + name2 + ".root")


    c1.SetLogy(1)
    h.SetMaximum(ymax_log)
    h.SetMinimum(ymin_log)
    h.GetYaxis().SetRangeUser(ymin_log, ymax_log)
    if cutline != "":
        line.SetX1(cutline); line.SetY1(ymin_log); line.SetX2(cutline); line.SetY2(ymax)
        line.Draw("same")
    if cutline2 != "":
        line2.SetX1(cutline2); line2.SetY1(ymin_log); line2.SetX2(cutline2); line2.SetY2(ymax)
        line2.Draw("same")
    c1.Update()
    c1.Print("unweighted_original_plots_pdf/" + name2 + "_log.pdf")
    c1.Print("unweighted_original_plots_gif/" + name2 + "_log.gif")
    c1.Print("unweighted_original_plots_root/" + name2 + "_log.root")
    c1.SetLogy(0)

    c1.Clear()

    del c1