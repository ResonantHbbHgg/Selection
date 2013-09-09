// C++ headers
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
// ROOT headers
#include "TROOT.h"
#include "TSystem.h"
#include <TFile.h>
#include <TTree.h>
// RooFit headers
// local files
// Verbosity
#define DEBUG 0
// namespaces
using namespace std;

int main(int argc, char *argv[])
{
	cout << "argc= " << argc << endl;
	for(int iarg = 0 ; iarg < argc; iarg++)
		cout << "argv[" << iarg << "]= " << argv[iarg] << endl;

	if( argc == 1 )
	{
		cerr << "WARNING: Arguments should be passed ! Default arguments will be used" << endl;
		cerr << "WARNING: Syntax is " << argv[0] << " -i (inputfile) -it (inputtree) -o (outputfile) -ot (outputtree) -wj (whichJet) -cutLevel (cutLevel) -fs (fitStrategy) -m (mass)" << endl;
	}
	
	string inputfile = "Data_m300_StandardFullSelection_v2.root";
	string inputtree = "Data";
	string outputfile = "Data_m300_test_minimal.root";
	string outputtree = "TCVARS";
	string whichJet = "";
	string fitStrategy = "mgg";
	int cutLevel = 0;
	int mass = 300;

	for(int iarg=0 ; iarg < argc ; iarg++)
	{
		if(strcmp("-i", argv[iarg]) == 0 && argc >= iarg + 1)
			inputfile = argv[iarg+1];
		if(strcmp("-it", argv[iarg]) == 0 && argc >= iarg + 1)
			inputtree = argv[iarg+1];
		if(strcmp("-o", argv[iarg]) == 0 && argc >= iarg + 1)
			outputfile = argv[iarg+1];
		if(strcmp("-ot", argv[iarg]) == 0 && argc >= iarg + 1)
			outputtree = argv[iarg+1];
		if(strcmp("-wj", argv[iarg]) == 0 && argc >= iarg + 1)
			whichJet = argv[iarg+1];
		if(strcmp("-fs", argv[iarg]) == 0 && argc >= iarg + 1)
			fitStrategy = argv[iarg+1];
		if(strcmp("-cutLevel", argv[iarg]) == 0 && argc >= iarg + 1)
			{ std::stringstream ss ( argv[iarg+1] ); ss >> cutLevel; }
		if(strcmp("-m", argv[iarg]) == 0 && argc >= iarg + 1)
			{ std::stringstream ss ( argv[iarg+1] ); ss >> mass; }
		if((strcmp("-h", argv[iarg]) == 0) || (strcmp("--help", argv[iarg]) == 0))
		{
			cerr << "WARNING: Arguments should be passed ! Default arguments will be used" << endl;
			cerr << "WARNING: Syntax is " << argv[0] << " -i (inputfile) -it (inputtree) -o (outputfile) -ot (outputtree) -wj (whichJet) -cutLevel (cutLevel) -fs (fitStrategy) -m (mass)" << endl;
			cerr << "inputfile= " << inputfile << endl;
			cerr << "inputtree= " << inputtree << endl;
			cerr << "outputfile= " << outputfile << endl;
			cerr << "outputtree= " << outputtree << endl;
			cerr << "whichJet= " << whichJet << endl;
			cerr << "cutLevel= " << cutLevel << endl;
			cerr << "fitStrategy= " << fitStrategy << endl;
			cerr << "mass= " << mass << endl;
			return 2;
		}
	}

	if(strcmp(whichJet.c_str(), "base") == 0) whichJet="";

	cout << "inputfile= " << inputfile << endl;
	cout << "inputtree= " << inputtree << endl;
	cout << "outputfile= " << outputfile << endl;
	cout << "outputtree= " << outputtree << endl;
	cout << "whichJet= " << whichJet << endl;
	cout << "cutLevel= " << cutLevel << endl;
	cout << "fitStrategy= " << fitStrategy << endl;
	cout << "mass= " << mass << endl;

	TFile *infile = TFile::Open(inputfile.c_str());
	TTree *intree = (TTree*)infile->Get(inputtree.c_str());
	TFile *outfile = new TFile(outputfile.c_str(), "RECREATE");
	TTree *outtree = new TTree(outputtree.c_str(), Form("%s minimal", outputtree.c_str()));


	float mgg, mjj, mtot;
	float pho1_pt, pho1_e, pho1_phi, pho1_eta, pho1_mass;
	float pho2_pt, pho2_e, pho2_phi, pho2_eta, pho2_mass;
	float pho1_r9, pho2_r9;
	float jet1_pt, jet1_e, jet1_phi, jet1_eta, jet1_mass;
	float jet2_pt, jet2_e, jet2_phi, jet2_eta, jet2_mass;
	float mjj_wokinfit, mtot_wokinfit;
	int cut_based_ct, njets_kRadionID_and_CSVM, selection_cut_level;
	float evWeight;
	float regcosthetastar, minDRgregkinj;
	int njets_kLooseID;
	intree->SetBranchAddress("gg_mass", &mgg);
	intree->SetBranchAddress("pho1_pt", &pho1_pt);
	intree->SetBranchAddress("pho1_e", &pho1_e);
	intree->SetBranchAddress("pho1_phi", &pho1_phi);
	intree->SetBranchAddress("pho1_eta", &pho1_eta);
	intree->SetBranchAddress("pho1_mass", &pho1_mass);
	intree->SetBranchAddress("pho1_r9", &pho1_r9);
	intree->SetBranchAddress("pho2_pt", &pho2_pt);
	intree->SetBranchAddress("pho2_e", &pho2_e);
	intree->SetBranchAddress("pho2_phi", &pho2_phi);
	intree->SetBranchAddress("pho2_eta", &pho2_eta);
	intree->SetBranchAddress("pho2_mass", &pho2_mass);
	intree->SetBranchAddress("pho2_r9", &pho2_r9);
	intree->SetBranchAddress(Form("%sjet1_pt", whichJet.c_str()), &jet1_pt);
	intree->SetBranchAddress(Form("%sjet1_e", whichJet.c_str()), &jet1_e);
	intree->SetBranchAddress(Form("%sjet1_phi", whichJet.c_str()), &jet1_phi);
	intree->SetBranchAddress(Form("%sjet1_eta", whichJet.c_str()), &jet1_eta);
	intree->SetBranchAddress(Form("%sjet1_mass", whichJet.c_str()), &jet1_mass);
	intree->SetBranchAddress(Form("%sjet2_pt", whichJet.c_str()), &jet2_pt);
	intree->SetBranchAddress(Form("%sjet2_e", whichJet.c_str()), &jet2_e);
	intree->SetBranchAddress(Form("%sjet2_phi", whichJet.c_str()), &jet2_phi);
	intree->SetBranchAddress(Form("%sjet2_eta", whichJet.c_str()), &jet2_eta);
	intree->SetBranchAddress(Form("%sjet2_mass", whichJet.c_str()), &jet2_mass);
	intree->SetBranchAddress(Form("%sjj_mass", whichJet.c_str()), &mjj);
	intree->SetBranchAddress(Form("%sggjj_mass", whichJet.c_str()), &mtot);
// Prepare mjj and mggjj variables "without kin fit" on which to cut
// (in case there is no kin fit asked for, they are just a dumb copy/paste)
	if( (strcmp("kin", whichJet.c_str()) == 0) || (strcmp("regkin", whichJet.c_str()) == 0) )
	{
		string whichJet_tmp = "";
		if(strcmp("regkin", whichJet.c_str()) == 0) whichJet_tmp = "reg";
		intree->SetBranchAddress(Form("%sjj_mass", whichJet_tmp.c_str()), &mjj_wokinfit);
		intree->SetBranchAddress(Form("%sggjj_mass", whichJet_tmp.c_str()), &mtot_wokinfit);
	}
	intree->SetBranchAddress("njets_kRadionID_and_CSVM", &njets_kRadionID_and_CSVM);
	intree->SetBranchAddress("selection_cut_level", &selection_cut_level);
	intree->SetBranchAddress("weight", &evWeight);
	intree->SetBranchAddress("regcosthetastar", &regcosthetastar);
	intree->SetBranchAddress("minDRgregkinj", &minDRgregkinj);
	intree->SetBranchAddress("njets_kLooseID", &njets_kLooseID);
	

	outtree->Branch("pho1_pt", &pho1_pt, "pho1_pt/F");
	outtree->Branch("pho1_e", &pho1_e, "pho1_e/F");
	outtree->Branch("pho1_phi", &pho1_phi, "pho1_phi/F");
	outtree->Branch("pho1_eta", &pho1_eta, "pho1_eta/F");
	outtree->Branch("pho1_mass", &pho1_mass, "pho1_mass/F");
	outtree->Branch("pho1_r9", &pho1_r9, "pho1_r9/F");
	outtree->Branch("pho2_pt", &pho2_pt, "pho2_pt/F");
	outtree->Branch("pho2_e", &pho2_e, "pho2_e/F");
	outtree->Branch("pho2_phi", &pho2_phi, "pho2_phi/F");
	outtree->Branch("pho2_eta", &pho2_eta, "pho2_eta/F");
	outtree->Branch("pho2_mass", &pho2_mass, "pho2_mass/F");
	outtree->Branch("pho2_r9", &pho2_r9, "pho2_r9/F");
	outtree->Branch("mgg", &mgg, "mgg/F");
	outtree->Branch("mjj", &mjj, "mjj/F");
	outtree->Branch("mtot", &mtot, "mtot/F");
	outtree->Branch("mjj_wokinfit", &mjj_wokinfit, "mjj_wokinfit/F");
	outtree->Branch("mtot_wokinfit", &mtot_wokinfit, "mtot_wokinfit/F");
	outtree->Branch("cut_based_ct", &cut_based_ct, "cut_based_ct/I");
	outtree->Branch("evWeight", &evWeight, "evWeight/F");

//	cout << "strcmp mgg= " << (strcmp("mgg", fitStrategy.c_str()) ) << endl;
//	cout << "strcmp mggjj= " << (strcmp("mggjj", fitStrategy.c_str()) ) << endl;

	for(int ievt= 0 ; ievt < (int)intree->GetEntries() ; ievt++)
	{
		intree->GetEntry(ievt);
		if( (strcmp("", whichJet.c_str()) == 0) || (strcmp("reg", whichJet.c_str()) == 0) )
			{ mjj_wokinfit = mjj; mtot_wokinfit = mtot; }
		
// EXTRA CUTS
//		if( selection_cut_level < cutLevel ) continue; // hard-coded in the trees, out of date wrt to the rest of the cuts
	if( cutLevel > 0)
	{
		if(fabs(regcosthetastar) >= .9) continue;
	}
	if( cutLevel > 1)
	{
		if(fabs(regcosthetastar) >= .9) continue;
		if( njets_kLooseID >= 4 ) continue;
	}
	if( cutLevel > 5)
	{
		if(fabs(regcosthetastar) >= .9) continue;
		if(minDRgregkinj <= 1.) continue;
		if( njets_kLooseID >= 4 ) continue;
	}

// FITTING THE MGG SPECTRUM
		if( strcmp("mgg", fitStrategy.c_str()) == 0 )
		{
			if( njets_kRadionID_and_CSVM >= 2 )
			{
				if( (strcmp("", whichJet.c_str()) == 0) || (strcmp("kin", whichJet.c_str()) == 0) )
					if( mjj_wokinfit < 95. || mjj_wokinfit > 175. ) continue;
				if( (strcmp("reg", whichJet.c_str()) == 0) || (strcmp("regkin", whichJet.c_str()) == 0) )
					if( mjj_wokinfit < 90. || mjj_wokinfit > 150. ) continue;
			}
			if( njets_kRadionID_and_CSVM == 1 )
			{
				if( (strcmp("", whichJet.c_str()) == 0) || (strcmp("kin", whichJet.c_str()) == 0) )
					if( mjj_wokinfit < 100. || mjj_wokinfit > 160. ) continue;
				if( (strcmp("reg", whichJet.c_str()) == 0) || (strcmp("regkin", whichJet.c_str()) == 0) )
					if( mjj_wokinfit < 95. || mjj_wokinfit > 140. ) continue;
			}
			if( mass == 300 )
			{
				if( (strcmp("", whichJet.c_str()) == 0) || (strcmp("kin", whichJet.c_str()) == 0) )
				{
					if( njets_kRadionID_and_CSVM >= 2 && (mtot_wokinfit < 255. || mtot_wokinfit > 320.) ) continue;
					if( njets_kRadionID_and_CSVM == 1 && (mtot_wokinfit < 260. || mtot_wokinfit > 335.) ) continue;
				}
				if( (strcmp("reg", whichJet.c_str()) == 0) || (strcmp("regkin", whichJet.c_str()) == 0) )
				{
					if( njets_kRadionID_and_CSVM >= 2 && (mtot_wokinfit < 260. || mtot_wokinfit > 335.) ) continue;
					if( njets_kRadionID_and_CSVM == 1 && (mtot_wokinfit < 265. || mtot_wokinfit > 345.) ) continue;
				}
			}
			if( mass == 500 && (mtot_wokinfit < 465. || mtot_wokinfit > 535.) ) continue;
			if( mass == 700 && (mtot_wokinfit < 660. || mtot_wokinfit > 740.) ) continue;
			if( mass == 1000 && (mtot_wokinfit < 955. || mtot_wokinfit > 1055.) ) continue;
		}

// FITTING THE MGGJJ SPECTRUM
		if( strcmp("mggjj", fitStrategy.c_str()) == 0 )
		{
			if( mgg < 120. || mgg > 130. ) continue;
			if( njets_kRadionID_and_CSVM >= 2 )
			{
				if( (strcmp("", whichJet.c_str()) == 0) || (strcmp("kin", whichJet.c_str()) == 0) )
					if( mjj_wokinfit < 95. || mjj_wokinfit > 150. ) continue;
				if( (strcmp("reg", whichJet.c_str()) == 0) || (strcmp("regkin", whichJet.c_str()) == 0) )
					if( mjj_wokinfit < 100. || mjj_wokinfit > 160. ) continue;
			}
			if( njets_kRadionID_and_CSVM == 1 ) 
				if( mjj_wokinfit < 90. || mjj_wokinfit > 170. ) continue;
		}
		if( njets_kRadionID_and_CSVM >= 2 ) cut_based_ct = 0;
		if( njets_kRadionID_and_CSVM == 1 ) cut_based_ct = 1;
		outtree->Fill();
	}
  outfile->cd();
  outtree->Write();
  outfile->Close();
  infile->Close();

	return 0;
}
