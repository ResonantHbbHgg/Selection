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
afs_plottree = "/afs/cern.ch/work/o/obondu/public/forRadion/plotTrees/v10/"
eos_tree = "root://eoscms//eos/cms/store/cmst3/group/hbbhgg/H2GGLOBE/Radion/trees/radion_tree_v09"

intL = 19706.
samples = []
# samples.append([ name, dirpath, subdir, file, tree, color, style, label , sigma , N])
#samples.append(["ggHH_8TeV", "", "2014-05-20_selection_noRegression_noMassCut_v10_hhh", "ggHH_8TeV_noRegression_noMassCut_v10_hhh.root", "ggHH_8TeV", ROOT.kRed, 0, "ggHH", 0.0000212, 96880])
#samples.append(["diphojet_sherpa_8TeV", "", "2014-05-20_selection_noRegression_noMassCut_v10_hhh", "diphojet_sherpa_8TeV_noRegression_noMassCut_v10_hhh.root", "diphojet_sherpa_8TeV", ROOT.kBlue, 0, "QCD #gamma#gamma bb", 0.1, 96880])

samples.append(["ggHH1_8TeV", "", "../LHEAnalysis", "ggHH1.root", "test", ROOT.kBlue, 0, "ggHH, #lambda = 1", 0.0000212, 100000])
samples.append(["ggHH0_8TeV", "", "../LHEAnalysis", "ggHH0.root", "test", ROOT.kRed, 0, "ggHH, #lambda = 0", 0.0000212, 100000])
samples.append(["ggHH2_8TeV", "", "../LHEAnalysis", "ggHH2.root", "test", ROOT.kGreen, 0, "ggHH, #lambda = 2", 0.0000212, 100000])
samples.append(["ggHH_8TeV", eos_tree, "", "SMHiggs.root", "ggHH_8TeV", ROOT.kMagenta, 0, "ggHH, SM", 0.0000212, 96880])

#####plots.append([ name2, variable, cut, norm, binning, title, additional_info, cutline, cutline2 ])
plots = []

hDivided = []
#gamma1 and gamma2
#plots.append(["pho1_pt", "pho1_pt", "", 1., "(100, 0, 500)", "p_{T}^{#gamma1} (GeV)", "", "", ""])
#plots.append(["pho2_pt", "pho2_pt", "", 1., "(100, 0, 500)", "p_{T}^{#gamma2} (GeV)", "", "", ""])
#plots.append(["pho1_eta", "pho1_eta", "", 1., "(50, -4, 4)", "eta^{#gamma1} ", "", "", ""])
#plots.append(["pho2_eta", "pho2_eta", "", 1., "(50, -4, 4)", "eta^{#gamma2} ", "", "", ""])
#plots.append(["pho1_phi", "pho1_phi", "", 1., "(50, -4, 4)", "phi^{#gamma1} (radian)", "", "", ""])
#plots.append(["pho2_phi", "pho2_phi", "", 1., "(50, -4, 4)", "phi^{#gamma2} (radian)", "", "", ""])
#plots.append(["pho1_e", "pho1_e", "", 1., "(100, 0, 500)", "e^{#gamma1} (GeV)", "", "", ""])
#plots.append(["pho2_e", "pho2_e", "", 1., "(100, 0, 500)", "e^{#gamma2} (GeV)", "", "", ""])

#jet1 and jet2
#plots.append(["jet1_pt", "jet1_pt", "", 1., "(100, 0, 500)", "p_{T}^{#jet1} (GeV)", "", "", ""])
#plots.append(["jet2_pt", "jet2_pt", "", 1., "(100, 0, 500)", "p_{T}^{#jet2} (GeV)", "", "", ""])
#plots.append(["jet1_eta", "jet1_eta", "", 1., "(50, -4, 4)", "eta^{#jet1} ", "", "", ""])
#plots.append(["jet2_eta", "jet2_eta", "", 1., "(50, -4, 4)", "eta^{#jet2} ", "", "", ""])
#plots.append(["jet1_phi", "jet1_phi", "", 1., "(50, -4, 4)", "phi^{#jet1} (radian)", "", "", ""])
#plots.append(["jet2_phi", "jet2_phi", "", 1., "(50, -4, 4)", "phi^{#jet2} (radian)", "", "", ""])
#plots.append(["jet1_e", "jet1_e", "", 1., "(100, 0, 500)", "e^{#jet1} (GeV)", "", "", ""])
#plots.append(["jet2_e", "jet2_e", "", 1., "(100, 0, 500)", "e^{#jet2} (GeV)", "", "", ""])

#two gammas
#plots.append(["gg_pt", "gg_pt", "", 1., "(100, 0, 500)", "p_{T}^{#gg} (GeV)", "", "", ""])
#plots.append(["gg_eta", "gg_eta", "", 1., "(50, -6, 6)", "eta^{#gg} ", "", "", ""])
#plots.append(["gg_phi", "gg_phi", "", 1., "(50, -4, 4)", "phi^{#gg} (radian)", "", "", ""])
#plots.append(["gg_e", "gg_e", "", 1., "(100, 0, 1000)", "e^{#gg} (GeV)", "", "", ""])
#plots.append(["gg_mass", "gg_mass", "", 1., "(100, 0, 250)", "mass^{#gg} (GeV)", "", "", ""])

#two jets
#plots.append(["jj_pt", "jj_pt", "", 1., "(100, 0, 500)", "p_{T}^{#jj} (GeV)", "", "", ""])
#plots.append(["jj_eta", "jj_eta", "", 1., "(50, -6, 6)", "eta^{#jj} ", "", "", ""])
#plots.append(["jj_phi", "jj_phi", "", 1., "(50, -4, 4)", "phi^{#jj} (radian)", "", "", ""])
#plots.append(["jj_e", "jj_e", "", 1., "(100, 0, 1000)", "e^{#jj} (GeV)", "", "", ""])
#plots.append(["jj_mass", "jj_mass", "", 1., "(100, 0, 250)", "mass^{#jj} (GeV)", "", "", ""])

#two jets and two gammas
#plots.append(["ggjj_pt", "ggjj_pt", "", 1., "(100, 0, 500)", "p_{T}^{#ggjj} (GeV)", "", "", ""])
#plots.append(["ggjj_eta", "ggjj_eta", "", 1., "(50, -6, 6)", "eta^{#ggjj} ", "", "", ""])
#plots.append(["ggjj_phi", "ggjj_phi", "", 1., "(50, -4, 4)", "phi^{#ggjj} (radian)", "", "", ""])
#plots.append(["ggjj_e", "ggjj_e", "", 1., "(500, 0, 1500)", "e^{#ggjj} (GeV)", "", "", ""])
#plots.append(["ggjj_mass", "ggjj_mass", "", 1., "(500, 0, 1000)", "mass^{#ggjj} (GeV)", "", "", ""])

#gamma1 and gamma2 (lhe2root.py)
plots.append(["SM_gr_g1_p4_pt", "gr_g1_p4_pt", "", 1., "(50, 0, 500)", "p_{T}^{#gamma1} (GeV)", "", "", ""])
plots.append(["SM_gr_g1_p4_eta", "gr_g1_p4_eta", "", 1., "(50, -4, 4)", "eta^{#gamma1} ", "", "", ""])
plots.append(["SM_gr_g1_p4_phi", "gr_g1_p4_phi", "", 1., "(50, -4, 4)", "phi^{#gamma1} (radian)", "", "", ""])
plots.append(["SM_gr_g1_p4_mass", "gr_g1_p4_mass", "", 1., "(100, 0, 200)", "mass^{#gamma1} (GeV", "", "", ""])
plots.append(["SM_gr_g2_p4_pt", "gr_g2_p4_pt", "", 1., "(50, 0, 500)", "p_{T}^{#gamma1} (GeV)", "", "", ""])
plots.append(["SM_gr_g2_p4_eta", "gr_g2_p4_eta", "", 1., "(50, -4, 4)", "eta^{#gamma1} ", "", "", ""])
plots.append(["SM_gr_g2_p4_phi", "gr_g2_p4_phi", "", 1., "(50, -4, 4)", "phi^{#gamma1} (radian)", "", "", ""])
plots.append(["SM_gr_g2_p4_mass", "gr_g2_p4_mass", "", 1., "(100, 0, 200)", "mass^{#gamma1} (GeV)", "", "", ""])

# b quark and anti b quark (lhe2root.py)
plots.append(["SM_gr_b1_p4_pt", "gr_b1_p4_pt", "", 1., "(50, 0, 500)", "p_{T}^{#b1} (GeV)", "", "", ""])
plots.append(["SM_gr_b1_p4_eta", "gr_b1_p4_eta", "", 1., "(50, -4, 4)", "eta^{#b1} ", "", "", ""])
plots.append(["SM_gr_b1_p4_phi", "gr_b1_p4_phi", "", 1., "(50, -4, 4)", "phi^{#b1} (radian)", "", "", ""])
plots.append(["SM_gr_b1_p4_mass", "gr_b1_p4_mass", "", 1., "(100, 0, 200)", "mass^{#b1} (GeV", "", "", ""])
plots.append(["SM_gr_b2_p4_pt", "gr_b2_p4_pt", "", 1., "(50, 0, 500)", "p_{T}^{#b2} (GeV)", "", "", ""])
plots.append(["SM_gr_b2_p4_eta", "gr_b2_p4_eta", "", 1., "(50, -4, 4)", "eta^{#b2} ", "", "", ""])
plots.append(["SM_gr_b2_p4_phi", "gr_b2_p4_phi", "", 1., "(50, -4, 4)", "phi^{#b2} (radian)", "", "", ""])
plots.append(["SM_gr_b2_p4_mass", "gr_b2_p4_mass", "", 1., "(100, 0, 200)", "mass^{#b2} (GeV)", "", "", ""])

# hgg and hbb (lhe2root.py)
plots.append(["SM_gr_hgg_p4_pt", "gr_hgg_p4_pt", "", 1., "(50, 0, 500)", "p_{T}^{#hgg} (GeV)", "", "", ""])
plots.append(["SM_gr_hgg_p4_eta", "gr_hgg_p4_eta", "", 1., "(50, -4, 4)", "eta^{#hgg} ", "", "", ""])
plots.append(["SM_gr_hgg_p4_phi", "gr_hgg_p4_phi", "", 1., "(50, -4, 4)", "phi^{#hgg} (radian)", "", "", ""])
plots.append(["SM_gr_hgg_p4_mass", "gr_hgg_p4_mass", "", 1., "(100, 0, 200)", "mass^{#hgg} (GeV", "", "", ""])
plots.append(["SM_gr_hbb_p4_pt", "gr_hbb_p4_pt", "", 1., "(50, 0, 500)", "p_{T}^{#hbb} (GeV)", "", "", ""])
plots.append(["SM_gr_hbb_p4_eta", "gr_hbb_p4_eta", "", 1., "(50, -4, 4)", "eta^{#hbb} ", "", "", ""])
plots.append(["SM_gr_hbb_p4_phi", "gr_hbb_p4_phi", "", 1., "(50, -4, 4)", "phi^{#hbb} (radian)", "", "", ""])
plots.append(["SM_gr_hbb_p4_mass", "gr_hbb_p4_mass", "", 1., "(100, 0, 200)", "mass^{#hbb} (GeV)", "", "", ""])



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
    

    for ifile, [ name, dirpath, subdir, file, tree, color, style, label , sigma , N] in enumerate(samples):
#        print ifile, file, color, style, label
        chain = TChain(tree)
        chain.Add( path.join(dirpath, subdir, file) )
        sample_cut = cut
        if norm == 1.:
            sample_cut = "(" + sample_cut + ")/" + str( chain.GetEntries() )
        else:
            sample_cut = "(" + sample_cut + ") * (" + str(sigma) + " * " + str(intL) + ")/" + str(N)
        option = ""
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

        hDen = ROOT.gDirectory.Get(firsthistname)
#        hMul = hDen.Clone("hMul")
        hFrac = h.Clone("hFrac")
        
        hFrac.Divide(hDen)
        hFrac.SetName("hFrac_" + name)              
        hDivided.append(hFrac)

#        hFrac.Multiply(hMul)
#        hDivided.append(hFrac)        

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
    latexLabel.DrawLatex(.55, .79, "-- gen level info and hadronized plot (magenta)")
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
    c1.Print("pdf/" + name2 + ".pdf")
    c1.Print("gif/" + name2 + ".gif")
    c1.Print("root/" + name2 + ".root")


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
    c1.Print("pdf/" + name2 + "_log.pdf")
    c1.Print("gif/" + name2 + "_log.gif")
    c1.Print("root/" + name2 + "_log.root")
    c1.SetLogy(0)

    c1.Clear()


    for ihisto, histo in enumerate(hDivided):

        if ihisto == 0:
            histo.SetMaximum(5)
            histo.SetMinimum(0)

            histo.Draw()
        else:
            histo.Draw("SAME")

    c1.Print("gif/" + name2 + "_ratio.gif")
    c1.Print("pdf/" + name2 + "_ratio.pdf")
    c1.Print("root/" + name2 + "_ratio.root")

    fileOption = "UPDATE"
    if iplot == 0:
        fileOption = "RECREATE"
    fileOut = TFile("weights.root", fileOption)
    for ihisto, histo in enumerate(hDivided):
        print histo.GetName()
        histo.Write(histo.GetName() + "_" +  name2) 
    fileOut.Close()

    del c1
    del fileOut