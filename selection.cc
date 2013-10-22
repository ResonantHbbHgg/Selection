// Radion Selection implementation
// O. Bondu (May 2013)
// TMVA headers
#include "TMVA/Reader.h"
#include "TMVA/Tools.h"
// C++ headers
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <boost/program_options.hpp>
// ROOT headers
#include "TROOT.h"
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>
// Analysis headers
#include "../KinematicFit/DiJetKinFitter.h"
// Verbosity
#define DEBUG 0
#define BLIND 0
#define SYNCHRO 0
#define SYNCHRO_GGANA 0
#define SYNCHRO_LIGHT 0
#define SYNC_W_CHIARA 0
#define SYNC_W_CHIARA_GGJJ 0
#define SYNC 0
#define USEHT 0
// namespaces
using namespace std;
namespace po = boost::program_options;

int main(int argc, char *argv[])
{
	// declare arguments
 	string inputfile = "root://eoscms//eos/cms/store/cmst3/user/obondu/H2GGLOBE/Radion/trees/radion_redu_08_tree_05/Graviton_Radion-nm.root";
	string inputtree = "Radion_m300_8TeV_nm";
	string outputfile = "Radion_m300_8TeV_nm_genjet_globeinputs.root";
	string outputtree = "Radion_m300_8TeV_nm";
	string regressionfile = "/afs/cern.ch/work/o/obondu/public/forRadion/factoryJetRegGen2_globeinputs_BDT.weights.xml";
	int numberOfSplit = 1;
	int treeSplit = 0;
	int isMC = 0; // Same conventions as in h2gglobe: <0 = signal ; =0 = data ; >0 = background

	// print out passed arguments
	copy(argv, argv + argc, ostream_iterator<char*>(cout, " ")); cout << endl;
	// argument parsing
	try
	{
		po::options_description desc("Allowed options");
		desc.add_options()
			("help,h", "produce help message")
			("inputfile,i", po::value<string>(&inputfile)->default_value("root://eoscms//eos/cms/store/cmst3/user/obondu/H2GGLOBE/Radion/trees/radion_tree_v06/Radion_Graviton_nm.root"), "input file")
			("inputtree,t", po::value<string>(&inputtree)->default_value("Radion_m300_8TeV_nm"), "input tree")
			("outputfile,O", po::value<string>(&outputfile)->default_value("selected.root"), "output file")
			("outputtree,T", po::value<string>(&outputtree)->default_value("Radion_m300_8TeV_nm"), "output tree")
			("regressionfile,r", po::value<string>(&regressionfile)->default_value("/afs/cern.ch/user/h/hebda/public/"), "regression file")
			("numberOfSplit,n", po::value<int>(&numberOfSplit)->default_value(1), "number of split (regression files)")
			("isMC", po::value<int>(&isMC)->default_value(1), "same conventions as in h2gglobe: <0 = signal ; =0 = data ; >0 = background")
		;
		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
		if (vm.count("help")) {
			cout << desc << "\n";
			return 1;
		}
	} catch(exception& e) {
		cerr << "error: " << e.what() << "\n";
		return 1;
	} catch(...) {
		cerr << "Exception of unknown type!\n";
	}
	// end of argument parsing
  //################################################


	if(DEBUG) cout << "End of argument parsing" << endl;





/*
	cout << "argc= " << argc << endl;
	for(int iarg = 0 ; iarg < argc; iarg++)
		cout << "argv[" << iarg << "]= " << argv[iarg] << endl;

	string syntaxMessage =  Form("WARNING: Syntax is %s -i (inputfile) -it (inputtree) -o (outputfile) -ot (outputtree) -rf (regressionfile) -mc (isMC) -n (numberOfSplit) -j (treeSplit)", argv[0]);
	if( argc == 1 )
	{
		cerr << "WARNING: Arguments should be passed ! Default arguments will be used" << endl;
		cerr << syntaxMessage << endl;
	}

	
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
		if(strcmp("-rf", argv[iarg]) == 0 && argc >= iarg + 1)
			regressionfile = argv[iarg+1];
		if(strcmp("-mc", argv[iarg]) == 0 && argc >= iarg + 1)
			{ std::stringstream ss ( argv[iarg+1] ); ss >> isMC; }
		if(strcmp("-n", argv[iarg]) == 0 && argc >= iarg + 1)
			{ std::stringstream ss ( argv[iarg+1] ); ss >> numberOfSplit; }
		if(strcmp("-j", argv[iarg]) == 0 && argc >= iarg + 1)
			{ std::stringstream ss ( argv[iarg+1] ); ss >> treeSplit; }
		if((strcmp("-h", argv[iarg]) == 0) || (strcmp("--help", argv[iarg]) == 0))
		{
			cerr << "WARNING: Arguments should be passed ! Default arguments will be used" << endl;
			cerr << syntaxMessage << endl;
			cerr << "inputfile= " << inputfile << endl;
			cerr << "inputtree= " << inputtree << endl;
			cerr << "outputfile= " << outputfile << endl;
			cerr << "outputtree= " << outputtree << endl;
			cerr << "regressionfile= " << regressionfile << endl;
			return 2;
		}
	}
*/
	cout << "inputfile= " << inputfile << endl;
	cout << "inputtree= " << inputtree << endl;
	cout << "outputfile= " << outputfile << endl;
	cout << "outputtree= " << outputtree << endl;
	cout << "regressionfile= " << regressionfile << endl;

	TFile *infile = TFile::Open(inputfile.c_str());
	TTree *intree = (TTree*)infile->Get(inputtree.c_str());
	TFile *outfile = new TFile(outputfile.c_str(), "RECREATE");
	TTree *outtree = new TTree(outputtree.c_str(), Form("%s reduced", outputtree.c_str()));
	ofstream synchrofile;
	if(SYNCHRO) synchrofile.open("synchronisation.txt");
	if(SYNCHRO_GGANA) synchrofile.open("synchronisation_ggAna.txt");

	if(DEBUG) cout << "Setup tree inputs" << endl;
	// setup tree inputs
// gen level info
	float gr_radion_p4_pt, gr_radion_p4_eta, gr_radion_p4_phi, gr_radion_p4_mass;
	float gr_hgg_p4_pt, gr_hgg_p4_eta, gr_hgg_p4_phi, gr_hgg_p4_mass;
	float gr_hbb_p4_pt, gr_hbb_p4_eta, gr_hbb_p4_phi, gr_hbb_p4_mass;
	float gr_hjj_p4_pt, gr_hjj_p4_eta, gr_hjj_p4_phi, gr_hjj_p4_mass;
	float gr_g1_p4_pt, gr_g1_p4_eta, gr_g1_p4_phi, gr_g1_p4_mass;
	float gr_g2_p4_pt, gr_g2_p4_eta, gr_g2_p4_phi, gr_g2_p4_mass;
	float gr_b1_p4_pt, gr_b1_p4_eta, gr_b1_p4_phi, gr_b1_p4_mass;
	float gr_b2_p4_pt, gr_b2_p4_eta, gr_b2_p4_phi, gr_b2_p4_mass;
	float gr_j1_p4_pt, gr_j1_p4_eta, gr_j1_p4_phi, gr_j1_p4_mass;
	float gr_j2_p4_pt, gr_j2_p4_eta, gr_j2_p4_phi, gr_j2_p4_mass;
	gr_radion_p4_pt = gr_radion_p4_eta = gr_radion_p4_phi = gr_radion_p4_mass = 0.;
	gr_hgg_p4_pt = gr_hgg_p4_eta = gr_hgg_p4_phi = gr_hgg_p4_mass = 0.;
	gr_hbb_p4_pt = gr_hbb_p4_eta = gr_hbb_p4_phi = gr_hbb_p4_mass = 0.;
	gr_hjj_p4_pt = gr_hjj_p4_eta = gr_hjj_p4_phi = gr_hjj_p4_mass = 0.;
	gr_g1_p4_pt = gr_g1_p4_eta = gr_g1_p4_phi = gr_g1_p4_mass = 0.;
	gr_g2_p4_pt = gr_g2_p4_eta = gr_g2_p4_phi = gr_g2_p4_mass = 0.;
	gr_b1_p4_pt = gr_b1_p4_eta = gr_b1_p4_phi = gr_b1_p4_mass = 0.;
	gr_b2_p4_pt = gr_b2_p4_eta = gr_b2_p4_phi = gr_b2_p4_mass = 0.;
	gr_j1_p4_pt = gr_j1_p4_eta = gr_j1_p4_phi = gr_j1_p4_mass = 0.;
	gr_j2_p4_pt = gr_j2_p4_eta = gr_j2_p4_phi = gr_j2_p4_mass = 0.;
// event variables
	float met_corr_pfmet, met_corr_phi_pfmet, met_corr_eta_pfmet, met_corr_e_pfmet;
	float pu_n, nvtx, rho;
	float weight, evweight, pu_weight;
	float event;
	float ev_weight, ev_evweight, ev_pu_weight;
// object variables
	float ph1_eta, ph2_eta, ph1_pt, ph2_pt, PhotonsMass, ph1_phi, ph2_phi, ph1_e, ph2_e, ph1_r9, ph2_r9;
	int ph1_ciclevel, ph2_ciclevel;
	float j1_e, j1_pt, j1_phi, j1_eta, j1_beta, j1_betaStar, j1_betaStarClassic, j1_dR2Mean, j1_csvBtag, j1_csvMvaBtag, j1_jetProbBtag, j1_tcheBtag, j1_ptD, j1_nSecondaryVertices, j1_secVtxPt, j1_secVtx3dL, j1_secVtx3deL, j1_emfrac, j1_hadfrac, j1_axis1, j1_axis2, j1_pull, /*j1_Rchg, j1_Rneutral, j1_R, j1_chargedMultiplicity, j1_neutralMultiplicity, j1_Chadfrac, j1_Nhadfrac, j1_Phofrac, j1_Mufrac, j1_Elefrac, j1_dPhiMet,*/ j1_radionMatched, j1_btagSF;
	int j1_ntk, j1_nNeutrals, j1_nCharged/*, j1_pfloose*/;
	float j2_e, j2_pt, j2_phi, j2_eta, j2_beta, j2_betaStar, j2_betaStarClassic, j2_dR2Mean, j2_csvBtag, j2_csvMvaBtag, j2_jetProbBtag, j2_tcheBtag, j2_ptD, j2_nSecondaryVertices, j2_secVtxPt, j2_secVtx3dL, j2_secVtx3deL, j2_emfrac, j2_hadfrac, j2_axis1, j2_axis2, j2_pull, /*j2_Rchg, j2_Rneutral, j2_R, j2_chargedMultiplicity, j2_neutralMultiplicity, j2_Chadfrac, j2_Nhadfrac, j2_Phofrac, j2_Mufrac, j2_Elefrac, j2_dPhiMet,*/ j2_radionMatched, j2_btagSF;
	int j2_ntk, j2_nNeutrals, j2_nCharged/*, j2_pfloose*/;
	float j3_e, j3_pt, j3_phi, j3_eta, j3_beta, j3_betaStar, j3_betaStarClassic, j3_dR2Mean, j3_csvBtag, j3_csvMvaBtag, j3_jetProbBtag, j3_tcheBtag, j3_ptD, j3_nSecondaryVertices, j3_secVtxPt, j3_secVtx3dL, j3_secVtx3deL, j3_emfrac, j3_hadfrac, j3_axis1, j3_axis2, j3_pull, /*j3_Rchg, j3_Rneutral, j3_R, j3_chargedMultiplicity, j3_neutralMultiplicity, j3_Chadfrac, j3_Nhadfrac, j3_Phofrac, j3_Mufrac, j3_Elefrac, j3_dPhiMet,*/ j3_radionMatched, j3_btagSF;
	int j3_ntk, j3_nNeutrals, j3_nCharged/*, j3_pfloose*/;
	float j4_e, j4_pt, j4_phi, j4_eta, j4_beta, j4_betaStar, j4_betaStarClassic, j4_dR2Mean, j4_csvBtag, j4_csvMvaBtag, j4_jetProbBtag, j4_tcheBtag, j4_ptD, j4_nSecondaryVertices, j4_secVtxPt, j4_secVtx3dL, j4_secVtx3deL, j4_emfrac, j4_hadfrac, j4_axis1, j4_axis2, j4_pull, /*j4_Rchg, j4_Rneutral, j4_R, j4_chargedMultiplicity, j4_neutralMultiplicity, j4_Chadfrac, j4_Nhadfrac, j4_Phofrac, j4_Mufrac, j4_Elefrac, j4_dPhiMet,*/ j4_radionMatched, j4_btagSF;
	int j4_ntk, j4_nNeutrals, j4_nCharged/*, j4_pfloose*/;
	float jet_e, jet_pt, jet_phi, jet_eta;
//jet_beta, jet_betaStar, 
	float jet_betaStarClassic, jet_dR2Mean, jet_csvBtag;
//, jet_csvMvaBtag, jet_jetProbBtag, jet_tcheBtag, 
	float jet_ptD, jet_nSecondaryVertices, jet_secVtxPt, jet_secVtx3dL, jet_secVtx3deL, jet_emfrac, jet_hadfrac, jet_btagSF;
	int jet_nNeutrals, jet_nCharged, jet_nConstituents;
	float jet_nConstituents_;
	float jet_dPhiMet_fabs;
//, jet_ntk, jet_axis1, jet_axis2, jet_pull, jet_Rchg, jet_Rneutral, jet_R, jet_chargedMultiplicity, jet_neutralMultiplicity, 
	float /*jet_Chadfrac, jet_Nhadfrac, jet_Phofrac, jet_Mufrac, jet_Elefrac,*/ jet_dPhiMet, jet_radionMatched, jet_regPt, jet_regkinPt;
	int jet_index/*, jet_pfloose*/;
//	float ev_met_corrMet, ev_met_corrMetPhi, ev_pu_n, ev_nvtx, ev_rho;

	if(DEBUG) cout << "Setup tree outputs" << endl;
// setup tree outputs
	float pho1_pt, pho1_e, pho1_phi, pho1_eta, pho1_mass, pho1_r9;
	float pho2_pt, pho2_e, pho2_phi, pho2_eta, pho2_mass, pho2_r9;
	float jet1_pt, jet1_e, jet1_phi, jet1_eta, jet1_mass, jet1_csvBtag, jet1_btagSF;
	float jet2_pt, jet2_e, jet2_phi, jet2_eta, jet2_mass, jet2_csvBtag, jet2_btagSF;
	float regjet1_emfrac, regjet1_hadfrac, regjet1_secVtxPt, regjet1_secVtx3dL, regjet1_dPhiMet;
	int regjet1_nConstituents;
	float regjet2_emfrac, regjet2_hadfrac, regjet2_secVtxPt, regjet2_secVtx3dL, regjet2_dPhiMet;
	int regjet2_nConstituents;
	float regjet1_pt, regjet1_e, regjet1_phi, regjet1_eta, regjet1_mass, regjet1_csvBtag, regjet1_btagSF;
	float regjet2_pt, regjet2_e, regjet2_phi, regjet2_eta, regjet2_mass, regjet2_csvBtag, regjet2_btagSF;
	float regkinjet1_pt, regkinjet1_e, regkinjet1_phi, regkinjet1_eta, regkinjet1_mass, regkinjet1_csvBtag, regkinjet1_btagSF;
	float regkinjet2_pt, regkinjet2_e, regkinjet2_phi, regkinjet2_eta, regkinjet2_mass, regkinjet2_csvBtag, regkinjet2_btagSF;
	float kinjet1_pt, kinjet1_e, kinjet1_phi, kinjet1_eta, kinjet1_mass, kinjet1_csvBtag, kinjet1_btagSF;
	float kinjet2_pt, kinjet2_e, kinjet2_phi, kinjet2_eta, kinjet2_mass, kinjet2_csvBtag, kinjet2_btagSF;
	float jj_pt, jj_e, jj_phi, jj_eta, jj_mass, jj_DR, jj_btagSF;
	float regjj_pt, regjj_e, regjj_phi, regjj_eta, regjj_mass, regjj_btagSF;
	float regkinjj_pt, regkinjj_e, regkinjj_phi, regkinjj_eta, regkinjj_mass, regkinjj_btagSF;
	float kinjj_pt, kinjj_e, kinjj_phi, kinjj_eta, kinjj_mass, kinjj_btagSF;
	float gg_pt, gg_e, gg_phi, gg_eta, gg_mass;
	float ggjj_pt, ggjj_e, ggjj_phi, ggjj_eta, ggjj_mass, regjj_DR, regkinjj_DR, kinjj_DR;
	float regggjj_pt, regggjj_e, regggjj_phi, regggjj_eta, regggjj_mass;
	float regkinggjj_pt, regkinggjj_e, regkinggjj_phi, regkinggjj_eta, regkinggjj_mass;
	float kinggjj_pt, kinggjj_e, kinggjj_phi, kinggjj_eta, kinggjj_mass;
	int selection_cut_level = 0;
	int category = 0;
	float costhetastar, regcosthetastar, regkincosthetastar, kincosthetastar;
	float minDRgj, minDRgregj, minDRgregkinj, minDRgkinj;
	float HT_gg;

	int njets_passing_kLooseID;
	intree->SetBranchAddress("njets_passing_kLooseID", &njets_passing_kLooseID);
	int njets_passing_kLooseID_and_CSVM;
	intree->SetBranchAddress("njets_passing_kLooseID_and_CSVM", &njets_passing_kLooseID_and_CSVM);
	int njets_kLooseID, njets_kRadionID;
	int njets_kLooseID_and_CSVM, njets_kRadionID_and_CSVM;

	if(DEBUG) cout << "SetBranchAddresses" << endl;
	intree->SetBranchAddress("ph1_eta", &ph1_eta);
	intree->SetBranchAddress("ph2_eta", &ph2_eta);
	intree->SetBranchAddress("ph1_pt", &ph1_pt);
	intree->SetBranchAddress("ph2_pt", &ph2_pt);
	intree->SetBranchAddress("ph1_phi", &ph1_phi);
	intree->SetBranchAddress("ph2_phi", &ph2_phi);
	intree->SetBranchAddress("ph1_e", &ph1_e);
	intree->SetBranchAddress("ph2_e", &ph2_e);
	intree->SetBranchAddress("ph1_r9", &ph1_r9);
	intree->SetBranchAddress("ph2_r9", &ph2_r9);
	intree->SetBranchAddress("PhotonsMass", &PhotonsMass);
	intree->SetBranchAddress("ph1_ciclevel", &ph1_ciclevel);
	intree->SetBranchAddress("ph2_ciclevel", &ph2_ciclevel);
	intree->SetBranchAddress("met_corr_pfmet", &met_corr_pfmet);
	intree->SetBranchAddress("met_corr_phi_pfmet", &met_corr_phi_pfmet);
	intree->SetBranchAddress("met_corr_eta_pfmet", &met_corr_eta_pfmet);
	intree->SetBranchAddress("met_corr_e_pfmet", &met_corr_e_pfmet);
	intree->SetBranchAddress("pu_n", &pu_n);
	intree->SetBranchAddress("nvtx", &nvtx);
	intree->SetBranchAddress("rho", &rho);
	intree->SetBranchAddress("event", &event);
	intree->SetBranchAddress("weight", &ev_weight);
	intree->SetBranchAddress("evweight", &ev_evweight);
	intree->SetBranchAddress("pu_weight", &ev_pu_weight);

	if( isMC < 0 )
	{
		intree->SetBranchAddress("gr_radion_p4_pt", &gr_radion_p4_pt);
		intree->SetBranchAddress("gr_radion_p4_eta", &gr_radion_p4_eta);
		intree->SetBranchAddress("gr_radion_p4_phi", &gr_radion_p4_phi);
		intree->SetBranchAddress("gr_radion_p4_mass", &gr_radion_p4_mass);
		intree->SetBranchAddress("gr_hgg_p4_pt", &gr_hgg_p4_pt);
		intree->SetBranchAddress("gr_hgg_p4_eta", &gr_hgg_p4_eta);
		intree->SetBranchAddress("gr_hgg_p4_phi", &gr_hgg_p4_phi);
		intree->SetBranchAddress("gr_hgg_p4_mass", &gr_hgg_p4_mass);
		intree->SetBranchAddress("gr_hbb_p4_pt", &gr_hbb_p4_pt);
		intree->SetBranchAddress("gr_hbb_p4_eta", &gr_hbb_p4_eta);
		intree->SetBranchAddress("gr_hbb_p4_phi", &gr_hbb_p4_phi);
		intree->SetBranchAddress("gr_hbb_p4_mass", &gr_hbb_p4_mass);
		intree->SetBranchAddress("gr_g1_p4_pt", &gr_g1_p4_pt);
		intree->SetBranchAddress("gr_g1_p4_eta", &gr_g1_p4_eta);
		intree->SetBranchAddress("gr_g1_p4_phi", &gr_g1_p4_phi);
		intree->SetBranchAddress("gr_g1_p4_mass", &gr_g1_p4_mass);
		intree->SetBranchAddress("gr_g2_p4_pt", &gr_g2_p4_pt);
		intree->SetBranchAddress("gr_g2_p4_eta", &gr_g2_p4_eta);
		intree->SetBranchAddress("gr_g2_p4_phi", &gr_g2_p4_phi);
		intree->SetBranchAddress("gr_g2_p4_mass", &gr_g2_p4_mass);
		intree->SetBranchAddress("gr_b1_p4_pt", &gr_b1_p4_pt);
		intree->SetBranchAddress("gr_b1_p4_eta", &gr_b1_p4_eta);
		intree->SetBranchAddress("gr_b1_p4_phi", &gr_b1_p4_phi);
		intree->SetBranchAddress("gr_b1_p4_mass", &gr_b1_p4_mass);
		intree->SetBranchAddress("gr_b2_p4_pt", &gr_b2_p4_pt);
		intree->SetBranchAddress("gr_b2_p4_eta", &gr_b2_p4_eta);
		intree->SetBranchAddress("gr_b2_p4_phi", &gr_b2_p4_phi);
		intree->SetBranchAddress("gr_b2_p4_mass", &gr_b2_p4_mass);
		intree->SetBranchAddress("gr_j1_p4_pt", &gr_j1_p4_pt);
		intree->SetBranchAddress("gr_j1_p4_eta", &gr_j1_p4_eta);
		intree->SetBranchAddress("gr_j1_p4_phi", &gr_j1_p4_phi);
		intree->SetBranchAddress("gr_j1_p4_mass", &gr_j1_p4_mass);
		intree->SetBranchAddress("gr_j2_p4_pt", &gr_j2_p4_pt);
		intree->SetBranchAddress("gr_j2_p4_eta", &gr_j2_p4_eta);
		intree->SetBranchAddress("gr_j2_p4_phi", &gr_j2_p4_phi);
		intree->SetBranchAddress("gr_j2_p4_mass", &gr_j2_p4_mass);
	}
	intree->SetBranchAddress("j1_e", &j1_e);
	intree->SetBranchAddress("j1_pt", &j1_pt);
	intree->SetBranchAddress("j1_phi", &j1_phi);
	intree->SetBranchAddress("j1_eta", &j1_eta);
	intree->SetBranchAddress("j1_beta", &j1_beta);
	intree->SetBranchAddress("j1_betaStar", &j1_betaStar);
	intree->SetBranchAddress("j1_betaStarClassic", &j1_betaStarClassic);
	intree->SetBranchAddress("j1_dR2Mean", &j1_dR2Mean);
	intree->SetBranchAddress("j1_csvBtag", &j1_csvBtag);
	intree->SetBranchAddress("j1_csvMvaBtag", &j1_csvMvaBtag);
	intree->SetBranchAddress("j1_jetProbBtag", &j1_jetProbBtag);
	intree->SetBranchAddress("j1_tcheBtag", &j1_tcheBtag);
	intree->SetBranchAddress("j1_radionMatched", &j1_radionMatched);
	intree->SetBranchAddress("j1_ptD", &j1_ptD);
	intree->SetBranchAddress("j1_nSecondaryVertices", &j1_nSecondaryVertices);
	intree->SetBranchAddress("j1_secVtxPt", &j1_secVtxPt);
	intree->SetBranchAddress("j1_secVtx3dL", &j1_secVtx3dL);
	intree->SetBranchAddress("j1_secVtx3deL", &j1_secVtx3deL);
	intree->SetBranchAddress("j1_emfrac", &j1_emfrac);
	intree->SetBranchAddress("j1_hadfrac", &j1_hadfrac);
	intree->SetBranchAddress("j1_btagSF", &j1_btagSF);
	intree->SetBranchAddress("j1_ntk", &j1_ntk);
	intree->SetBranchAddress("j1_nNeutrals", &j1_nNeutrals);
	intree->SetBranchAddress("j1_nCharged", &j1_nCharged);
	intree->SetBranchAddress("j1_axis1", &j1_axis1);
	intree->SetBranchAddress("j1_axis2", &j1_axis2);
	intree->SetBranchAddress("j1_pull", &j1_pull);
/*	intree->SetBranchAddress("j1_Rchg", &j1_Rchg);
	intree->SetBranchAddress("j1_Rneutral", &j1_Rneutral);
	intree->SetBranchAddress("j1_R", &j1_R);
	intree->SetBranchAddress("j1_chargedMultiplicity", &j1_chargedMultiplicity);
	intree->SetBranchAddress("j1_neutralMultiplicity", &j1_neutralMultiplicity);
	intree->SetBranchAddress("j1_Chadfrac", &j1_Chadfrac);
	intree->SetBranchAddress("j1_Nhadfrac", &j1_Nhadfrac);
	intree->SetBranchAddress("j1_Phofrac", &j1_Phofrac);
	intree->SetBranchAddress("j1_Mufrac", &j1_Mufrac);
	intree->SetBranchAddress("j1_Elefrac", &j1_Elefrac);
	intree->SetBranchAddress("j1_dPhiMet", &j1_dPhiMet);
	intree->SetBranchAddress("j1_pfloose", &j1_pfloose);
*/

	intree->SetBranchAddress("j2_e", &j2_e);
	intree->SetBranchAddress("j2_pt", &j2_pt);
	intree->SetBranchAddress("j2_phi", &j2_phi);
	intree->SetBranchAddress("j2_eta", &j2_eta);
	intree->SetBranchAddress("j2_beta", &j2_beta);
	intree->SetBranchAddress("j2_betaStar", &j2_betaStar);
	intree->SetBranchAddress("j2_betaStarClassic", &j2_betaStarClassic);
	intree->SetBranchAddress("j2_dR2Mean", &j2_dR2Mean);
	intree->SetBranchAddress("j2_csvBtag", &j2_csvBtag);
	intree->SetBranchAddress("j2_csvMvaBtag", &j2_csvMvaBtag);
	intree->SetBranchAddress("j2_jetProbBtag", &j2_jetProbBtag);
	intree->SetBranchAddress("j2_tcheBtag", &j2_tcheBtag);
	intree->SetBranchAddress("j2_radionMatched", &j2_radionMatched);
	intree->SetBranchAddress("j2_ptD", &j2_ptD);
	intree->SetBranchAddress("j2_nSecondaryVertices", &j2_nSecondaryVertices);
	intree->SetBranchAddress("j2_secVtxPt", &j2_secVtxPt);
	intree->SetBranchAddress("j2_secVtx3dL", &j2_secVtx3dL);
	intree->SetBranchAddress("j2_secVtx3deL", &j2_secVtx3deL);
	intree->SetBranchAddress("j2_emfrac", &j2_emfrac);
	intree->SetBranchAddress("j2_hadfrac", &j2_hadfrac);
	intree->SetBranchAddress("j2_btagSF", &j2_btagSF);
	intree->SetBranchAddress("j2_ntk", &j2_ntk);
	intree->SetBranchAddress("j2_nNeutrals", &j2_nNeutrals);
	intree->SetBranchAddress("j2_nCharged", &j2_nCharged);
	intree->SetBranchAddress("j2_axis1", &j2_axis1);
	intree->SetBranchAddress("j2_axis2", &j2_axis2);
	intree->SetBranchAddress("j2_pull", &j2_pull);
/*	intree->SetBranchAddress("j2_Rchg", &j2_Rchg);
	intree->SetBranchAddress("j2_Rneutral", &j2_Rneutral);
	intree->SetBranchAddress("j2_R", &j2_R);
	intree->SetBranchAddress("j2_chargedMultiplicity", &j2_chargedMultiplicity);
	intree->SetBranchAddress("j2_neutralMultiplicity", &j2_neutralMultiplicity);
	intree->SetBranchAddress("j2_Chadfrac", &j2_Chadfrac);
	intree->SetBranchAddress("j2_Nhadfrac", &j2_Nhadfrac);
	intree->SetBranchAddress("j2_Phofrac", &j2_Phofrac);
	intree->SetBranchAddress("j2_Mufrac", &j2_Mufrac);
	intree->SetBranchAddress("j2_Elefrac", &j2_Elefrac);
	intree->SetBranchAddress("j2_dPhiMet", &j2_dPhiMet);
	intree->SetBranchAddress("j2_pfloose", &j2_pfloose);
*/

	intree->SetBranchAddress("j3_e", &j3_e);
	intree->SetBranchAddress("j3_pt", &j3_pt);
	intree->SetBranchAddress("j3_phi", &j3_phi);
	intree->SetBranchAddress("j3_eta", &j3_eta);
	intree->SetBranchAddress("j3_beta", &j3_beta);
	intree->SetBranchAddress("j3_betaStar", &j3_betaStar);
	intree->SetBranchAddress("j3_betaStarClassic", &j3_betaStarClassic);
	intree->SetBranchAddress("j3_dR2Mean", &j3_dR2Mean);
	intree->SetBranchAddress("j3_csvBtag", &j3_csvBtag);
	intree->SetBranchAddress("j3_csvMvaBtag", &j3_csvMvaBtag);
	intree->SetBranchAddress("j3_jetProbBtag", &j3_jetProbBtag);
	intree->SetBranchAddress("j3_tcheBtag", &j3_tcheBtag);
	intree->SetBranchAddress("j3_radionMatched", &j3_radionMatched);
	intree->SetBranchAddress("j3_ptD", &j3_ptD);
	intree->SetBranchAddress("j3_nSecondaryVertices", &j3_nSecondaryVertices);
	intree->SetBranchAddress("j3_secVtxPt", &j3_secVtxPt);
	intree->SetBranchAddress("j3_secVtx3dL", &j3_secVtx3dL);
	intree->SetBranchAddress("j3_secVtx3deL", &j3_secVtx3deL);
	intree->SetBranchAddress("j3_emfrac", &j3_emfrac);
	intree->SetBranchAddress("j3_hadfrac", &j3_hadfrac);
	intree->SetBranchAddress("j3_btagSF", &j3_btagSF);
	intree->SetBranchAddress("j3_ntk", &j3_ntk);
	intree->SetBranchAddress("j3_nNeutrals", &j3_nNeutrals);
	intree->SetBranchAddress("j3_nCharged", &j3_nCharged);
	intree->SetBranchAddress("j3_axis1", &j3_axis1);
	intree->SetBranchAddress("j3_axis2", &j3_axis2);
	intree->SetBranchAddress("j3_pull", &j3_pull);
/*	intree->SetBranchAddress("j3_Rchg", &j3_Rchg);
	intree->SetBranchAddress("j3_Rneutral", &j3_Rneutral);
	intree->SetBranchAddress("j3_R", &j3_R);
	intree->SetBranchAddress("j3_chargedMultiplicity", &j3_chargedMultiplicity);
	intree->SetBranchAddress("j3_neutralMultiplicity", &j3_neutralMultiplicity);
	intree->SetBranchAddress("j3_Chadfrac", &j3_Chadfrac);
	intree->SetBranchAddress("j3_Nhadfrac", &j3_Nhadfrac);
	intree->SetBranchAddress("j3_Phofrac", &j3_Phofrac);
	intree->SetBranchAddress("j3_Mufrac", &j3_Mufrac);
	intree->SetBranchAddress("j3_Elefrac", &j3_Elefrac);
	intree->SetBranchAddress("j3_dPhiMet", &j3_dPhiMet);
	intree->SetBranchAddress("j3_pfloose", &j3_pfloose);
*/

	intree->SetBranchAddress("j4_e", &j4_e);
	intree->SetBranchAddress("j4_pt", &j4_pt);
	intree->SetBranchAddress("j4_phi", &j4_phi);
	intree->SetBranchAddress("j4_eta", &j4_eta);
	intree->SetBranchAddress("j4_beta", &j4_beta);
	intree->SetBranchAddress("j4_betaStar", &j4_betaStar);
	intree->SetBranchAddress("j4_betaStarClassic", &j4_betaStarClassic);
	intree->SetBranchAddress("j4_dR2Mean", &j4_dR2Mean);
	intree->SetBranchAddress("j4_csvBtag", &j4_csvBtag);
	intree->SetBranchAddress("j4_csvMvaBtag", &j4_csvMvaBtag);
	intree->SetBranchAddress("j4_jetProbBtag", &j4_jetProbBtag);
	intree->SetBranchAddress("j4_tcheBtag", &j4_tcheBtag);
	intree->SetBranchAddress("j4_radionMatched", &j4_radionMatched);
	intree->SetBranchAddress("j4_ptD", &j4_ptD);
	intree->SetBranchAddress("j4_nSecondaryVertices", &j4_nSecondaryVertices);
	intree->SetBranchAddress("j4_secVtxPt", &j4_secVtxPt);
	intree->SetBranchAddress("j4_secVtx3dL", &j4_secVtx3dL);
	intree->SetBranchAddress("j4_secVtx3deL", &j4_secVtx3deL);
	intree->SetBranchAddress("j4_emfrac", &j4_emfrac);
	intree->SetBranchAddress("j4_hadfrac", &j4_hadfrac);
	intree->SetBranchAddress("j4_btagSF", &j4_btagSF);
	intree->SetBranchAddress("j4_ntk", &j4_ntk);
	intree->SetBranchAddress("j4_nNeutrals", &j4_nNeutrals);
	intree->SetBranchAddress("j4_nCharged", &j4_nCharged);
	intree->SetBranchAddress("j4_axis1", &j4_axis1);
	intree->SetBranchAddress("j4_axis2", &j4_axis2);
	intree->SetBranchAddress("j4_pull", &j4_pull);
/*	intree->SetBranchAddress("j4_Rchg", &j4_Rchg);
	intree->SetBranchAddress("j4_Rneutral", &j4_Rneutral);
	intree->SetBranchAddress("j4_R", &j4_R);
	intree->SetBranchAddress("j4_chargedMultiplicity", &j4_chargedMultiplicity);
	intree->SetBranchAddress("j4_neutralMultiplicity", &j4_neutralMultiplicity);
	intree->SetBranchAddress("j4_Chadfrac", &j4_Chadfrac);
	intree->SetBranchAddress("j4_Nhadfrac", &j4_Nhadfrac);
	intree->SetBranchAddress("j4_Phofrac", &j4_Phofrac);
	intree->SetBranchAddress("j4_Mufrac", &j4_Mufrac);
	intree->SetBranchAddress("j4_Elefrac", &j4_Elefrac);
	intree->SetBranchAddress("j4_dPhiMet", &j4_dPhiMet);
	intree->SetBranchAddress("j4_pfloose", &j4_pfloose);
*/

	outtree->Branch("category", &category, "category/I");
	outtree->Branch("selection_cut_level", &selection_cut_level, "selection_cut_level/I");
	outtree->Branch("event", &event, "event/F");
	outtree->Branch("weight", &weight, "weight/F");
	outtree->Branch("evweight", &evweight, "evweight/F");
	outtree->Branch("pu_weight", &pu_weight, "pu_weight/F");
	outtree->Branch("met_corr_pfmet", &met_corr_pfmet, "met_corr_pfmet/F");
	outtree->Branch("met_corr_phi_pfmet", &met_corr_phi_pfmet, "met_corr_phi_pfmet/F");
	outtree->Branch("met_corr_eta_pfmet", &met_corr_eta_pfmet, "met_corr_eta_pfmet/F");
	outtree->Branch("met_corr_e_pfmet", &met_corr_e_pfmet, "met_corr_e_pfmet/F");
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
	outtree->Branch("jet1_pt", &jet1_pt, "jet1_pt/F");
	outtree->Branch("jet1_e", &jet1_e, "jet1_e/F");
	outtree->Branch("jet1_phi", &jet1_phi, "jet1_phi/F");
	outtree->Branch("jet1_eta", &jet1_eta, "jet1_eta/F");
	outtree->Branch("jet1_mass", &jet1_mass, "jet1_mass/F");
	outtree->Branch("jet1_csvBtag", &jet1_csvBtag, "jet1_csvBtag/F");
	outtree->Branch("jet1_btagSF", &jet1_btagSF, "jet1_btagSF/F");
	outtree->Branch("jet2_pt", &jet2_pt, "jet2_pt/F");
	outtree->Branch("jet2_e", &jet2_e, "jet2_e/F");
	outtree->Branch("jet2_phi", &jet2_phi, "jet2_phi/F");
	outtree->Branch("jet2_eta", &jet2_eta, "jet2_eta/F");
	outtree->Branch("jet2_mass", &jet2_mass, "jet2_mass/F");
	outtree->Branch("jet2_csvBtag", &jet2_csvBtag, "jet2_csvBtag/F");
	outtree->Branch("jet2_btagSF", &jet2_btagSF, "jet2_btagSF/F");
// storing inputs of the regression for comparison
	outtree->Branch("regjet1_emfrac", &regjet1_emfrac, "regjet1_emfrac/F");
	outtree->Branch("regjet1_hadfrac", &regjet1_hadfrac, "regjet1_hadfrac/F");
	outtree->Branch("regjet1_secVtxPt", &regjet1_secVtxPt, "regjet1_secVtxPt/F");
	outtree->Branch("regjet1_secVtx3dL", &regjet1_secVtx3dL, "regjet1_secVtx3dL/F");
	outtree->Branch("regjet1_dPhiMet", &regjet1_dPhiMet, "regjet1_dPhiMet/F");
	outtree->Branch("regjet1_nConstituents", &regjet1_nConstituents, "regjet1_nConstituents/I");
	outtree->Branch("regjet2_emfrac", &regjet2_emfrac, "regjet2_emfrac/F");
	outtree->Branch("regjet2_hadfrac", &regjet2_hadfrac, "regjet2_hadfrac/F");
	outtree->Branch("regjet2_secVtxPt", &regjet2_secVtxPt, "regjet2_secVtxPt/F");
	outtree->Branch("regjet2_secVtx3dL", &regjet2_secVtx3dL, "regjet2_secVtx3dL/F");
	outtree->Branch("regjet2_dPhiMet", &regjet2_dPhiMet, "regjet2_dPhiMet/F");
	outtree->Branch("regjet2_nConstituents", &regjet2_nConstituents, "regjet2_nConstituents/I");
// regressed / kin fitted jets
	outtree->Branch("regjet1_pt", &regjet1_pt, "regjet1_pt/F");
	outtree->Branch("regjet1_e", &regjet1_e, "regjet1_e/F");
	outtree->Branch("regjet1_phi", &regjet1_phi, "regjet1_phi/F");
	outtree->Branch("regjet1_eta", &regjet1_eta, "regjet1_eta/F");
	outtree->Branch("regjet1_mass", &regjet1_mass, "regjet1_mass/F");
	outtree->Branch("regjet1_csvBtag", &regjet1_csvBtag, "regjet1_csvBtag/F");
	outtree->Branch("regjet1_btagSF", &regjet1_btagSF, "regjet1_btagSF/F");
	outtree->Branch("regjet2_pt", &regjet2_pt, "regjet2_pt/F");
	outtree->Branch("regjet2_e", &regjet2_e, "regjet2_e/F");
	outtree->Branch("regjet2_phi", &regjet2_phi, "regjet2_phi/F");
	outtree->Branch("regjet2_eta", &regjet2_eta, "regjet2_eta/F");
	outtree->Branch("regjet2_mass", &regjet2_mass, "regjet2_mass/F");
	outtree->Branch("regjet2_csvBtag", &regjet2_csvBtag, "regjet2_csvBtag/F");
	outtree->Branch("regjet2_btagSF", &regjet2_btagSF, "regjet2_btagSF/F");
	outtree->Branch("regkinjet1_pt", &regkinjet1_pt, "regkinjet1_pt/F");
	outtree->Branch("regkinjet1_e", &regkinjet1_e, "regkinjet1_e/F");
	outtree->Branch("regkinjet1_phi", &regkinjet1_phi, "regkinjet1_phi/F");
	outtree->Branch("regkinjet1_eta", &regkinjet1_eta, "regkinjet1_eta/F");
	outtree->Branch("regkinjet1_mass", &regkinjet1_mass, "regkinjet1_mass/F");
	outtree->Branch("regkinjet1_csvBtag", &regkinjet1_csvBtag, "regkinjet1_csvBtag/F");
	outtree->Branch("regkinjet1_btagSF", &regkinjet1_btagSF, "regkinjet1_btagSF/F");
	outtree->Branch("regkinjet2_pt", &regkinjet2_pt, "regkinjet2_pt/F");
	outtree->Branch("regkinjet2_e", &regkinjet2_e, "regkinjet2_e/F");
	outtree->Branch("regkinjet2_phi", &regkinjet2_phi, "regkinjet2_phi/F");
	outtree->Branch("regkinjet2_eta", &regkinjet2_eta, "regkinjet2_eta/F");
	outtree->Branch("regkinjet2_mass", &regkinjet2_mass, "regkinjet2_mass/F");
	outtree->Branch("regkinjet2_csvBtag", &regkinjet2_csvBtag, "regkinjet2_csvBtag/F");
	outtree->Branch("regkinjet2_btagSF", &regkinjet2_btagSF, "regkinjet2_btagSF/F");
	outtree->Branch("kinjet1_pt", &kinjet1_pt, "kinjet1_pt/F");
	outtree->Branch("kinjet1_e", &kinjet1_e, "kinjet1_e/F");
	outtree->Branch("kinjet1_phi", &kinjet1_phi, "kinjet1_phi/F");
	outtree->Branch("kinjet1_eta", &kinjet1_eta, "kinjet1_eta/F");
	outtree->Branch("kinjet1_mass", &kinjet1_mass, "kinjet1_mass/F");
	outtree->Branch("kinjet1_csvBtag", &kinjet1_csvBtag, "kinjet1_csvBtag/F");
	outtree->Branch("kinjet1_btagSF", &kinjet1_btagSF, "kinjet1_btagSF/F");
	outtree->Branch("kinjet2_pt", &kinjet2_pt, "kinjet2_pt/F");
	outtree->Branch("kinjet2_e", &kinjet2_e, "kinjet2_e/F");
	outtree->Branch("kinjet2_phi", &kinjet2_phi, "kinjet2_phi/F");
	outtree->Branch("kinjet2_eta", &kinjet2_eta, "kinjet2_eta/F");
	outtree->Branch("kinjet2_mass", &kinjet2_mass, "kinjet2_mass/F");
	outtree->Branch("kinjet2_csvBtag", &kinjet2_csvBtag, "kinjet2_csvBtag/F");
	outtree->Branch("kinjet2_btagSF", &kinjet2_btagSF, "kinjet2_btagSF/F");
	outtree->Branch("jj_pt", &jj_pt, "jj_pt/F");
	outtree->Branch("jj_e", &jj_e, "jj_e/F");
	outtree->Branch("jj_phi", &jj_phi, "jj_phi/F");
	outtree->Branch("jj_eta", &jj_eta, "jj_eta/F");
	outtree->Branch("jj_mass", &jj_mass, "jj_mass/F");
	outtree->Branch("jj_btagSF", &jj_btagSF, "jj_btagSF/F");
	outtree->Branch("jj_DR", &jj_DR, "jj_DR/F");
	outtree->Branch("regjj_pt", &regjj_pt, "regjj_pt/F");
	outtree->Branch("regjj_e", &regjj_e, "regjj_e/F");
	outtree->Branch("regjj_phi", &regjj_phi, "regjj_phi/F");
	outtree->Branch("regjj_eta", &regjj_eta, "regjj_eta/F");
	outtree->Branch("regjj_mass", &regjj_mass, "regjj_mass/F");
	outtree->Branch("regjj_btagSF", &regjj_btagSF, "regjj_btagSF/F");
	outtree->Branch("regjj_DR", &regjj_DR, "regjj_DR/F");
	outtree->Branch("regkinjj_pt", &regkinjj_pt, "regkinjj_pt/F");
	outtree->Branch("regkinjj_e", &regkinjj_e, "regkinjj_e/F");
	outtree->Branch("regkinjj_phi", &regkinjj_phi, "regkinjj_phi/F");
	outtree->Branch("regkinjj_eta", &regkinjj_eta, "regkinjj_eta/F");
	outtree->Branch("regkinjj_mass", &regkinjj_mass, "regkinjj_mass/F");
	outtree->Branch("regkinjj_btagSF", &regkinjj_btagSF, "regkinjj_btagSF/F");
	outtree->Branch("regkinjj_DR", &regkinjj_DR, "regkinjj_DR/F");
	outtree->Branch("kinjj_pt", &kinjj_pt, "kinjj_pt/F");
	outtree->Branch("kinjj_e", &kinjj_e, "kinjj_e/F");
	outtree->Branch("kinjj_phi", &kinjj_phi, "kinjj_phi/F");
	outtree->Branch("kinjj_eta", &kinjj_eta, "kinjj_eta/F");
	outtree->Branch("kinjj_mass", &kinjj_mass, "kinjj_mass/F");
	outtree->Branch("kinjj_btagSF", &kinjj_btagSF, "kinjj_btagSF/F");
	outtree->Branch("kinjj_DR", &kinjj_DR, "kinjj_DR/F");
	outtree->Branch("gg_pt", &gg_pt, "gg_pt/F");
	outtree->Branch("gg_e", &gg_e, "gg_e/F");
	outtree->Branch("gg_phi", &gg_phi, "gg_phi/F");
	outtree->Branch("gg_eta", &gg_eta, "gg_eta/F");
	outtree->Branch("gg_mass", &gg_mass, "gg_mass/F");
	outtree->Branch("ggjj_pt", &ggjj_pt, "ggjj_pt/F");
	outtree->Branch("ggjj_e", &ggjj_e, "ggjj_e/F");
	outtree->Branch("ggjj_phi", &ggjj_phi, "ggjj_phi/F");
	outtree->Branch("ggjj_eta", &ggjj_eta, "ggjj_eta/F");
	outtree->Branch("ggjj_mass", &ggjj_mass, "ggjj_mass/F");
	outtree->Branch("regggjj_pt", &regggjj_pt, "regggjj_pt/F");
	outtree->Branch("regggjj_e", &regggjj_e, "regggjj_e/F");
	outtree->Branch("regggjj_phi", &regggjj_phi, "regggjj_phi/F");
	outtree->Branch("regggjj_eta", &regggjj_eta, "regggjj_eta/F");
	outtree->Branch("regggjj_mass", &regggjj_mass, "regggjj_mass/F");
	outtree->Branch("regkinggjj_pt", &regkinggjj_pt, "regkinggjj_pt/F");
	outtree->Branch("regkinggjj_e", &regkinggjj_e, "regkinggjj_e/F");
	outtree->Branch("regkinggjj_phi", &regkinggjj_phi, "regkinggjj_phi/F");
	outtree->Branch("regkinggjj_eta", &regkinggjj_eta, "regkinggjj_eta/F");
	outtree->Branch("regkinggjj_mass", &regkinggjj_mass, "regkinggjj_mass/F");
	outtree->Branch("kinggjj_pt", &kinggjj_pt, "kinggjj_pt/F");
	outtree->Branch("kinggjj_e", &kinggjj_e, "kinggjj_e/F");
	outtree->Branch("kinggjj_phi", &kinggjj_phi, "kinggjj_phi/F");
	outtree->Branch("kinggjj_eta", &kinggjj_eta, "kinggjj_eta/F");
	outtree->Branch("kinggjj_mass", &kinggjj_mass, "kinggjj_mass/F");
	outtree->Branch("njets_kLooseID", &njets_kLooseID, "njets_kLooseID/I");
	outtree->Branch("njets_kLooseID_and_CSVM", &njets_kLooseID_and_CSVM, "njets_kLooseID_and_CSVM/I");
	outtree->Branch("njets_kRadionID", &njets_kRadionID, "njets_kRadionID/I");
	outtree->Branch("njets_kRadionID_and_CSVM", &njets_kRadionID_and_CSVM, "njets_kRadionID_and_CSVM/I");
	outtree->Branch("costhetastar", &costhetastar, "costhetastar/F");
	outtree->Branch("regcosthetastar", &regcosthetastar, "regcosthetastar/F");
	outtree->Branch("regkincosthetastar", &regkincosthetastar, "regkincosthetastar/F");
	outtree->Branch("kincosthetastar", &kincosthetastar, "kincosthetastar/F");
	outtree->Branch("minDRgj", &minDRgj, "minDRgj/F");
	outtree->Branch("minDRgregj", &minDRgregj, "minDRgregj/F");
	outtree->Branch("minDRgregkinj", &minDRgregkinj, "minDRgregkinj/F");
	outtree->Branch("minDRgkinj", &minDRgkinj, "minDRgkinj/F");
// gen level info
	outtree->Branch("gr_radion_p4_pt", &gr_radion_p4_pt, "gr_radion_p4_pt/F");
	outtree->Branch("gr_radion_p4_eta", &gr_radion_p4_eta, "gr_radion_p4_eta/F");
	outtree->Branch("gr_radion_p4_phi", &gr_radion_p4_phi, "gr_radion_p4_phi/F");
	outtree->Branch("gr_radion_p4_mass", &gr_radion_p4_mass, "gr_radion_p4_mass/F");
	outtree->Branch("gr_hgg_p4_pt", &gr_hgg_p4_pt, "gr_hgg_p4_pt/F");
	outtree->Branch("gr_hgg_p4_eta", &gr_hgg_p4_eta, "gr_hgg_p4_eta/F");
	outtree->Branch("gr_hgg_p4_phi", &gr_hgg_p4_phi, "gr_hgg_p4_phi/F");
	outtree->Branch("gr_hgg_p4_mass", &gr_hgg_p4_mass, "gr_hgg_p4_mass/F");
	outtree->Branch("gr_hbb_p4_pt", &gr_hbb_p4_pt, "gr_hbb_p4_pt/F");
	outtree->Branch("gr_hbb_p4_eta", &gr_hbb_p4_eta, "gr_hbb_p4_eta/F");
	outtree->Branch("gr_hbb_p4_phi", &gr_hbb_p4_phi, "gr_hbb_p4_phi/F");
	outtree->Branch("gr_hbb_p4_mass", &gr_hbb_p4_mass, "gr_hbb_p4_mass/F");
	outtree->Branch("gr_hjj_p4_pt", &gr_hjj_p4_pt, "gr_hjj_p4_pt/F");
	outtree->Branch("gr_hjj_p4_eta", &gr_hjj_p4_eta, "gr_hjj_p4_eta/F");
	outtree->Branch("gr_hjj_p4_phi", &gr_hjj_p4_phi, "gr_hjj_p4_phi/F");
	outtree->Branch("gr_hjj_p4_mass", &gr_hjj_p4_mass, "gr_hjj_p4_mass/F");
	outtree->Branch("gr_g1_p4_pt", &gr_g1_p4_pt, "gr_g1_p4_pt/F");
	outtree->Branch("gr_g1_p4_eta", &gr_g1_p4_eta, "gr_g1_p4_eta/F");
	outtree->Branch("gr_g1_p4_phi", &gr_g1_p4_phi, "gr_g1_p4_phi/F");
	outtree->Branch("gr_g1_p4_mass", &gr_g1_p4_mass, "gr_g1_p4_mass/F");
	outtree->Branch("gr_g2_p4_pt", &gr_g2_p4_pt, "gr_g2_p4_pt/F");
	outtree->Branch("gr_g2_p4_eta", &gr_g2_p4_eta, "gr_g2_p4_eta/F");
	outtree->Branch("gr_g2_p4_phi", &gr_g2_p4_phi, "gr_g2_p4_phi/F");
	outtree->Branch("gr_g2_p4_mass", &gr_g2_p4_mass, "gr_g2_p4_mass/F");
	outtree->Branch("gr_b1_p4_pt", &gr_b1_p4_pt, "gr_b1_p4_pt/F");
	outtree->Branch("gr_b1_p4_eta", &gr_b1_p4_eta, "gr_b1_p4_eta/F");
	outtree->Branch("gr_b1_p4_phi", &gr_b1_p4_phi, "gr_b1_p4_phi/F");
	outtree->Branch("gr_b1_p4_mass", &gr_b1_p4_mass, "gr_b1_p4_mass/F");
	outtree->Branch("gr_b2_p4_pt", &gr_b2_p4_pt, "gr_b2_p4_pt/F");
	outtree->Branch("gr_b2_p4_eta", &gr_b2_p4_eta, "gr_b2_p4_eta/F");
	outtree->Branch("gr_b2_p4_phi", &gr_b2_p4_phi, "gr_b2_p4_phi/F");
	outtree->Branch("gr_b2_p4_mass", &gr_b2_p4_mass, "gr_b2_p4_mass/F");
	outtree->Branch("gr_j1_p4_pt", &gr_j1_p4_pt, "gr_j1_p4_pt/F");
	outtree->Branch("gr_j1_p4_eta", &gr_j1_p4_eta, "gr_j1_p4_eta/F");
	outtree->Branch("gr_j1_p4_phi", &gr_j1_p4_phi, "gr_j1_p4_phi/F");
	outtree->Branch("gr_j1_p4_mass", &gr_j1_p4_mass, "gr_j1_p4_mass/F");
	outtree->Branch("gr_j2_p4_pt", &gr_j2_p4_pt, "gr_j2_p4_pt/F");
	outtree->Branch("gr_j2_p4_eta", &gr_j2_p4_eta, "gr_j2_p4_eta/F");
	outtree->Branch("gr_j2_p4_phi", &gr_j2_p4_phi, "gr_j2_p4_phi/F");
	outtree->Branch("gr_j2_p4_mass", &gr_j2_p4_mass, "gr_j2_p4_mass/F");

	if(DEBUG) cout << "Prepare for regression" << endl;
// prepare for regression
	TMVA::Reader* readerRegres = new TMVA::Reader( "!Color:!Silent" );
	readerRegres->AddVariable( "jet_eta", &jet_eta); 
	readerRegres->AddVariable( "jet_emfrac", &jet_emfrac);
	readerRegres->AddVariable( "jet_hadfrac", &jet_hadfrac);
	readerRegres->AddVariable( "jet_nconstituents", &jet_nConstituents_);
	readerRegres->AddVariable( "jet_vtx3dL", &jet_secVtx3dL);
	readerRegres->AddVariable( "MET", &met_corr_pfmet);
	readerRegres->AddVariable( "jet_dPhiMETJet", &jet_dPhiMet_fabs);
/*
	readerRegres->AddVariable( "jet_pt", &jet_pt);
	readerRegres->AddVariable( "jet_eta", &jet_eta);
	readerRegres->AddVariable( "jet_emfrac", &jet_emfrac);
	readerRegres->AddVariable( "jet_nConstituents", &jet_nConstituents_);
	readerRegres->AddVariable( "jet_hadfrac", &jet_hadfrac);
	readerRegres->AddVariable( "jet_secVtxPt", &jet_secVtxPt);
	readerRegres->AddVariable( "jet_secVtx3dL", &jet_secVtx3dL);
	readerRegres->AddVariable( "ev_met_corr_pfmet", &met_corr_pfmet);
	readerRegres->AddVariable( "jet_dPhiMet", &jet_dPhiMet);
*/
// Adding variables
//	readerRegres->AddVariable( "ev_rho", &rho);
	if( USEHT ) readerRegres->AddVariable( "ph1_pt+ph2_pt", &HT_gg);
	if(numberOfSplit <= 1)
		readerRegres->BookMVA("BDT", regressionfile.c_str());
	else {
		for(int i = 0; i < numberOfSplit ; i++)
		{
//			readerRegres->BookMVA(Form("BDT_%i", i), Form("weights/2013-08-08_test_n%i_j%i_BDT.weights.xml", numberOfSplit, i));
//			readerRegres->BookMVA(Form("BDT_%i", i), Form("weights/2013-08-08_test02_n%i_j%i_BDT.weights.xml", numberOfSplit, i));
//			readerRegres->BookMVA(Form("BDT_%i", i), Form("weights/2013-08-08_test03_n%i_j%i_BDT.weights.xml", numberOfSplit, i));
//			readerRegres->BookMVA(Form("BDT_%i", i), Form("weights/%s_n%i_j%i_BDT.weights.xml", regressionfile.c_str(), numberOfSplit, i));
			readerRegres->BookMVA(Form("BDT_%i", i), Form("%s/TMVARegression_10_Cat%i_BDTG.weights.xml", regressionfile.c_str(), i));
		}
	}


	int nevents[30] = {0};
	float nevents_w[30] = {0.};
	float nevents_w_btagSF[30] = {0.};
	int nevents_sync[30] = {0};
	int nevents1btag[30] = {0};
	int nevents2btag[30] = {0};
	string eventcut[30];
	int njets[30] = {0};
	string jetcut[30];
  int decade = 0;
	int n_sync_events_before_mjj = 0;
	int n_sync_events_before_mggjj = 0;
  int totevents = intree->GetEntries();
  if(DEBUG) totevents = 100;
  cout << "#entries= " << totevents << endl;
  // loop over events
  for(int ievt=0 ; ievt < totevents ; ievt++)
//  for(int ievt=10127 ; ievt < 10128 ; ievt++)
  {
		if(DEBUG) cout << "#####\tievt= " << ievt << endl;
    double progress = 10.0*ievt/(1.0*totevents);
    int k = TMath::FloorNint(progress);
    if (k > decade) cout<<10*k<<" %"<<endl;
    decade = k;

		int njets_kRadionID_ = 0;
		int njets_kRadionID_and_CSVM_ = 0;
    intree->GetEntry(ievt);
		if(DEBUG) cout << "event= " << event << endl;
		if( isMC < 0 && ((int)event % 2 == 0)) continue; // use regression only on odd events
	
// Compute hjj system
		TLorentzVector gj1, gj2;
		if( gr_j1_p4_pt > .01 && gr_j2_p4_pt > .01)
		{
			gj1.SetPtEtaPhiM(gr_j1_p4_pt, gr_j1_p4_eta, gr_j1_p4_phi, gr_j1_p4_mass);
			gj2.SetPtEtaPhiM(gr_j2_p4_pt, gr_j2_p4_eta, gr_j2_p4_phi, gr_j2_p4_mass);
			TLorentzVector hjj = gj1 + gj2;
			gr_hjj_p4_pt = hjj.Pt();
			gr_hjj_p4_eta = hjj.Eta();
			gr_hjj_p4_phi = hjj.Phi();
			gr_hjj_p4_mass = hjj.M();
		} else {
			gr_hjj_p4_pt = 0.;
			gr_hjj_p4_eta = 0.;
			gr_hjj_p4_phi = 0.;
			gr_hjj_p4_mass = 0.;
		}

		// Apply photon ID cuts
		nevents[0]++; eventcut[0] = "Before photon ID";
		nevents_w[0] += weight;
		nevents_sync[0]++;
//		if( (fabs(ph1_eta) > 2.5) || (fabs(ph2_eta) > 2.5) ) continue;
		nevents[1]++; eventcut[1] = "(OBSOLETE) After eta < 2.5";
		nevents_w[1] += weight;
//		if( (fabs(ph1_eta) < 1.566) && (fabs(ph1_eta) >1.4442) ) continue;
		nevents[2]++; eventcut[2] = "(OBSOLETE) After eta gap for photon 1";
		nevents_w[2] += weight;
//		if( (fabs(ph2_eta) < 1.566) && (fabs(ph2_eta) >1.4442) ) continue;
		nevents[3]++; eventcut[3] = "(OBSOLETE) After eta gap for photon 2";
		nevents_w[3] += weight;
		if( ph1_pt < (float)(40.*PhotonsMass)/(float)120. ) continue;
		nevents[4]++; eventcut[4] = "After floating pt cut for photon 1 (40*mgg/120 GeV)";
		nevents_w[4] += weight;
		if( ph2_pt < 25. ) continue;
		nevents[5]++; eventcut[5] = "After fixed pt cut for photon 2 (25 GeV)";
		nevents_w[5] += weight;
		nevents_sync[1]++;
		if(DEBUG) cout << "ph1_ciclevel= " << ph1_ciclevel << "\tph2_ciclevel= " << ph2_ciclevel << endl;
		if( (ph1_ciclevel < 4) || (ph2_ciclevel < 4) ) continue;
		nevents[6]++; eventcut[6] = "After cic cut on both photons";
		nevents_w[6] += weight;
		nevents_sync[2]++;
		if( (PhotonsMass < 100.) || (PhotonsMass > 180.) ) continue;
		nevents[7]++; eventcut[7] = "After 100 < mgg < 180";
		nevents_w[7] += weight;
		if(BLIND)
			{ if( (PhotonsMass > 120.) && (PhotonsMass < 130.) ) continue; }
		nevents[8]++; eventcut[8] = "After blinding data in 120 < mgg < 130";
		nevents_w[8] += weight;
		nevents_sync[3]++;
		HT_gg = ph1_pt + ph2_pt;

		// take only the subset of events where at least two jets remains
		if( njets_passing_kLooseID < 2 ) continue;
		nevents[9]++; eventcut[9] = "After njet >= 2";
		nevents_w[9] += weight;
		nevents_sync[4]++;
//		if( njets_passing_kLooseID_and_CSVM < 1 ) continue;
// alternative counting: taking into account only the 4 jets stored !
		int nbjet_tmp = 0;
//		float csv_cut = 0.244; // CSVL
		float csv_cut = 0.679; // CSVM
		for( int ijet = 0 ; ijet < min(njets_passing_kLooseID, 4); ijet ++ )
		{
			if( ijet == 0 )
				if(j1_csvBtag > csv_cut)
					nbjet_tmp++; 
			if( ijet == 1 )
				if(j2_csvBtag > csv_cut)
					nbjet_tmp++; 
			if( ijet == 2 )
				if(j3_csvBtag > csv_cut)
					nbjet_tmp++; 
			if( ijet == 3 )
				if(j4_csvBtag > csv_cut)
					nbjet_tmp++; 
		}
		if( nbjet_tmp < 1 ) continue;
		nevents[10]++; eventcut[10] = "After nbjet >= 1";
		nevents_w[10] += weight;
		nevents_sync[5]++;
		if( nbjet_tmp == 1) nevents1btag[10]++; 
		else nevents2btag[10]++;

		TLorentzVector jet;
		vector<float> jetPt;
		vector<float> jetbtagSF;
		vector<float> jetE;
		vector<float> jetEta;
		vector<float> jetPhi;
		vector<float> jetCSV;
		vector<float> jetRegPt;
		vector<float> jetRegKinPt;
// regression inputs
		vector<float> jetEmfrac;
		vector<float> jetHadfrac;
		vector<float> jetSecVtxPt;
		vector<float> jetSecVtx3dL;
		vector<float> jetDPhiMet;
		vector<int> jetNConstituents;

		TLorentzVector met;
		met.SetPtEtaPhiE(met_corr_pfmet, met_corr_eta_pfmet, met_corr_phi_pfmet, met_corr_e_pfmet);

		// loop over jets, store jet info + info on closest genjet / parton (no selection applied)
		if(DEBUG) cout << "njets_passing_kLooseID= " << njets_passing_kLooseID << endl;
		for( int ijet = 0 ; ijet < min(njets_passing_kLooseID, 4); ijet ++ )
		{
			njets[0]++; jetcut[0] = "Before JetID";
			if( ijet == 0 )
			{
				jet_e = j1_e;
				jet_pt = j1_pt;
				jet_phi = j1_phi;
				jet_eta = j1_eta;
/*				jet_beta = j1_beta;
				jet_betaStar = j1_betaStar;
*/				jet_betaStarClassic = j1_betaStarClassic;
				jet_dR2Mean = j1_dR2Mean;
				jet_csvBtag = j1_csvBtag;
/*				jet_csvMvaBtag = j1_csvMvaBtag;
				jet_jetProbBtag = j1_jetProbBtag;
				jet_tcheBtag = j1_tcheBtag;
*/				jet_radionMatched = j1_radionMatched;
				jet_ptD = j1_ptD;
				jet_nSecondaryVertices = j1_nSecondaryVertices;
				jet_secVtxPt = j1_secVtxPt;
				jet_secVtx3dL = j1_secVtx3dL;
				jet_secVtx3deL = j1_secVtx3deL;
				jet_emfrac = j1_emfrac;
				jet_hadfrac = j1_hadfrac;
				jet_btagSF = j1_btagSF;
				jet_nNeutrals = j1_nNeutrals;
				jet_nCharged = j1_nCharged;
				jet_nConstituents = jet_nNeutrals + jet_nCharged;
/*
				jet_ntk = j1_ntk;
				jet_axis1 = j1_axis1;
				jet_axis2 = j1_axis2;
				jet_pull = j1_pull;
				jet_Rchg = j1_Rchg;
				jet_Rneutral = j1_Rneutral;
				jet_R = j1_R;
				jet_chargedMultiplicity = j1_chargedMultiplicity;
				jet_neutralMultiplicity = j1_neutralMultiplicity;
				jet_Chadfrac = j1_Chadfrac;
				jet_Nhadfrac = j1_Nhadfrac;
				jet_Phofrac = j1_Phofrac;
				jet_Mufrac = j1_Mufrac;
				jet_Elefrac = j1_Elefrac;
				jet_pfloose = j1_pfloose;
*/				jet_index = 1;
				jet.SetPtEtaPhiE(j1_pt, j1_eta, j1_phi, j1_e);
				jet_dPhiMet = jet.DeltaPhi(met);
			} // end if jet == 0

			if( ijet == 1 )
			{
				jet_e = j2_e;
				jet_pt = j2_pt;
				jet_phi = j2_phi;
				jet_eta = j2_eta;
/*				jet_beta = j2_beta;
				jet_betaStar = j2_betaStar;
*/				jet_betaStarClassic = j2_betaStarClassic;
				jet_dR2Mean = j2_dR2Mean;
				jet_csvBtag = j2_csvBtag;
/*				jet_csvMvaBtag = j2_csvMvaBtag;
				jet_jetProbBtag = j2_jetProbBtag;
				jet_tcheBtag = j2_tcheBtag;
*/				jet_radionMatched = j2_radionMatched;
				jet_ptD = j2_ptD;
				jet_nSecondaryVertices = j2_nSecondaryVertices;
				jet_secVtxPt = j2_secVtxPt;
				jet_secVtx3dL = j2_secVtx3dL;
				jet_secVtx3deL = j2_secVtx3deL;
				jet_emfrac = j2_emfrac;
				jet_hadfrac = j2_hadfrac;
				jet_btagSF = j2_btagSF;
				jet_nNeutrals = j2_nNeutrals;
				jet_nCharged = j2_nCharged;
				jet_nConstituents = jet_nNeutrals + jet_nCharged;
	/*			jet_ntk = j2_ntk;
				jet_axis1 = j2_axis1;
				jet_axis2 = j2_axis2;
				jet_pull = j2_pull;
				jet_Rchg = j2_Rchg;
				jet_Rneutral = j2_Rneutral;
				jet_R = j2_R;
				jet_chargedMultiplicity = j2_chargedMultiplicity;
				jet_neutralMultiplicity = j2_neutralMultiplicity;
				jet_Chadfrac = j2_Chadfrac;
				jet_Nhadfrac = j2_Nhadfrac;
				jet_Phofrac = j2_Phofrac;
				jet_Mufrac = j2_Mufrac;
				jet_Elefrac = j2_Elefrac;
				jet_pfloose = j2_pfloose;
*/				jet_index = 2;
				jet.SetPtEtaPhiE(j2_pt, j2_eta, j2_phi, j2_e);
				jet_dPhiMet = jet.DeltaPhi(met);
			} // end if jet == 1

			if( ijet == 2 )
			{
				jet_e = j3_e;
				jet_pt = j3_pt;
				jet_phi = j3_phi;
				jet_eta = j3_eta;
/*				jet_beta = j3_beta;
				jet_betaStar = j3_betaStar;
*/				jet_betaStarClassic = j3_betaStarClassic;
				jet_dR2Mean = j3_dR2Mean;
				jet_csvBtag = j3_csvBtag;
/*				jet_csvMvaBtag = j3_csvMvaBtag;
				jet_jetProbBtag = j3_jetProbBtag;
				jet_tcheBtag = j3_tcheBtag;
*/				jet_radionMatched = j3_radionMatched;
				jet_ptD = j3_ptD;
				jet_nSecondaryVertices = j3_nSecondaryVertices;
				jet_secVtxPt = j3_secVtxPt;
				jet_secVtx3dL = j3_secVtx3dL;
				jet_secVtx3deL = j3_secVtx3deL;
				jet_emfrac = j3_emfrac;
				jet_hadfrac = j3_hadfrac;
				jet_btagSF = j3_btagSF;
				jet_nNeutrals = j3_nNeutrals;
				jet_nCharged = j3_nCharged;
				jet_nConstituents = jet_nNeutrals + jet_nCharged;
/*				jet_ntk = j3_ntk;
				jet_axis1 = j3_axis1;
				jet_axis2 = j3_axis2;
				jet_pull = j3_pull;
				jet_Rchg = j3_Rchg;
				jet_Rneutral = j3_Rneutral;
				jet_R = j3_R;
				jet_chargedMultiplicity = j3_chargedMultiplicity;
				jet_neutralMultiplicity = j3_neutralMultiplicity;
				jet_Chadfrac = j3_Chadfrac;
				jet_Nhadfrac = j3_Nhadfrac;
				jet_Phofrac = j3_Phofrac;
				jet_Mufrac = j3_Mufrac;
				jet_Elefrac = j3_Elefrac;
				jet_pfloose = j3_pfloose;
*/				jet_index = 3;
				jet.SetPtEtaPhiE(j3_pt, j3_eta, j3_phi, j3_e);
				jet_dPhiMet = jet.DeltaPhi(met);
			} // end if jet == 2

			if( ijet == 3 )
			{
				jet_e = j4_e;
				jet_pt = j4_pt;
				jet_phi = j4_phi;
				jet_eta = j4_eta;
/*				jet_beta = j4_beta;
				jet_betaStar = j4_betaStar;
*/				jet_betaStarClassic = j4_betaStarClassic;
				jet_dR2Mean = j4_dR2Mean;
				jet_csvBtag = j4_csvBtag;
/*				jet_csvMvaBtag = j4_csvMvaBtag;
				jet_jetProbBtag = j4_jetProbBtag;
				jet_tcheBtag = j4_tcheBtag;
*/				jet_radionMatched = j4_radionMatched;
				jet_ptD = j4_ptD;
				jet_nSecondaryVertices = j4_nSecondaryVertices;
				jet_secVtxPt = j4_secVtxPt;
				jet_secVtx3dL = j4_secVtx3dL;
				jet_secVtx3deL = j4_secVtx3deL;
				jet_emfrac = j4_emfrac;
				jet_hadfrac = j4_hadfrac;
				jet_btagSF = j4_btagSF;
				jet_nNeutrals = j4_nNeutrals;
				jet_nCharged = j4_nCharged;
				jet_nConstituents = jet_nNeutrals + jet_nCharged;
/*				jet_ntk = j4_ntk;
				jet_axis1 = j4_axis1;
				jet_axis2 = j4_axis2;
				jet_pull = j4_pull;
				jet_Rchg = j4_Rchg;
				jet_Rneutral = j4_Rneutral;
				jet_R = j4_R;
				jet_chargedMultiplicity = j4_chargedMultiplicity;
				jet_neutralMultiplicity = j4_neutralMultiplicity;
				jet_Chadfrac = j4_Chadfrac;
				jet_Nhadfrac = j4_Nhadfrac;
				jet_Phofrac = j4_Phofrac;
				jet_Mufrac = j4_Mufrac;
				jet_Elefrac = j4_Elefrac;
				jet_pfloose = j4_pfloose;
*/				jet_index = 4;
				jet.SetPtEtaPhiE(j4_pt, j4_eta, j4_phi, j4_e);
				jet_dPhiMet = jet.DeltaPhi(met);
			} // end if jet == 3
			jet_nConstituents_ = (float) jet_nConstituents;
			jet_dPhiMet_fabs = fabs(jet_dPhiMet);

//			if(DEBUG) cout << "input= " << jet_pt << "\toutput= " << readerRegres->EvaluateRegression("BDTG method")[0] << endl; // Phil regression
//			if(DEBUG) cout << "input= " << jet_pt << "\toutput (BDT)= " << readerRegres->EvaluateMVA("BDT") << endl; // Olivier regression
			if(DEBUG) cout << "input= " << jet_pt << "\toutput (BDT_0)= " << readerRegres->EvaluateMVA(Form("BDT_%i", 0)) << "\toutput (BDT_1)= " << readerRegres->EvaluateMVA(Form("BDT_%i", 1)) << endl; // Olivier regression
//			TLorentzVector jnew;
//			jnew.SetPtEtaPhiE(jet_pt, jet_eta, jet_phi, jet_e);
			if( jet_csvBtag < 0. ) continue;
			njets[5]++; jetcut[5] = "After jet_csvBtag < 0.";
			if(DEBUG) cout << "now with the regression" << endl;
//			jnew = ((float)readerRegres->EvaluateRegression("BDTG method")[0]/(float)jet_pt) * jnew; // Phil regression
//			jet_regPt = (float)(readerRegres->EvaluateMVA("BDT"));
			if(numberOfSplit == -1)
				jet_regPt = jet_pt; // no regression applied
			else if(numberOfSplit <= 1)
				jet_regPt = (float)(readerRegres->EvaluateMVA("BDT"));
			else
				jet_regPt = (float)(readerRegres->EvaluateRegression(Form("BDT_%i", jet_pt < 80. ? 0 : 1))[0]) * jet_pt;
//			if(DEBUG) cout << "jet_pt= " << jet_pt << "\tjet_regPt= " << jet_regPt << endl;
			jet_regkinPt = jet_regPt;
//			jnew = (float)jet_regPt / (float)jet_pt * jnew; // applying regression
//			jet_regPt = jet_pt; // FIXME DEBUG SYNCHRONISATION
			// jet selection
			// ** acceptance + pu id **
			if( jet_regPt < 25. ) continue;
			njets[1]++; jetcut[1] = "After jet pt > 25";
			if( fabs(jet_eta) > 2.5 ) continue;
			njets[2]++; jetcut[2] = "After jet |eta| < 2.5";
			if( jet_betaStarClassic > 0.2 * log( nvtx - 0.64) ) continue;
			njets[3]++; jetcut[3] = "After jet_betaStarClassic > 0.2 * log( nvtx - 0.64)";
			if( jet_dR2Mean > 0.06 ) continue;
			njets[4]++; jetcut[4] = "After jet_dR2Mean > 0.06";
			if(DEBUG) cout << "Jet is passing selection cuts" << endl;
			// ** call regression to correct the pt **
			// ** store 4-momentum + csv output for combinatorics **
			jetPt.push_back(jet_pt);
			jetbtagSF.push_back(jet_btagSF);
			jetE.push_back(jet_e);
			jetEta.push_back(jet_eta);
			jetPhi.push_back(jet_phi);
			jetCSV.push_back(jet_csvBtag);
			jetRegPt.push_back(jet_regPt);
			jetRegKinPt.push_back(jet_regkinPt);
			jetEmfrac.push_back(jet_emfrac);
			jetHadfrac.push_back(jet_hadfrac);
			jetSecVtxPt.push_back(jet_secVtxPt);
			jetSecVtx3dL.push_back(jet_secVtx3dL);
			jetDPhiMet.push_back(jet_dPhiMet);
			jetNConstituents.push_back(jet_nConstituents);


			njets_kRadionID_++;
			if(jet_csvBtag > csv_cut) njets_kRadionID_and_CSVM_++;
		} // end of loop over jets
		
		if(DEBUG) cout << "jetPt.size()= " << jetPt.size() << endl;
		// jet combinatorics
		if( jetPt.size() < 2 ) continue;
		nevents[11]++; eventcut[11] = "After njet >=2 passing the jet selection";
		nevents_w[11] += weight;
		nevents_sync[6]++;

		vector<int> btaggedJet;
		for( unsigned int ijet = 0 ; ijet < jetPt.size() ; ijet++ )
		{
			if( jetCSV[ijet] > csv_cut )
				btaggedJet.push_back(ijet);
		}

		if( btaggedJet.size() < 1 ) continue;
		nevents[12]++; eventcut[12] = "After nbjet >=1 passing the jet selection";
		nevents_w[12] += weight;
		nevents_sync[7]++;
		if( btaggedJet.size() == 1) nevents1btag[12]++; 
		else nevents2btag[12]++;

		if(nevents[0] == 1 && SYNCHRO_GGANA) synchrofile << "event" << "\t" << "ph1_pt" << "\t" << "ph2_pt" << "\t" << "PhotonsMass" << "\t" << "j1_pt" << "\t" << "j2_pt" << "\t" << "j3_pt" << "\t" << "j4_pt" << endl;
		if(SYNCHRO_GGANA) synchrofile << event << "\t" << ph1_pt << "\t" << ph2_pt << "\t" << PhotonsMass << "\t" << j1_pt << "\t" << j2_pt << "\t" << j3_pt << "\t" << j4_pt << endl;


		int ij1, ij2;
		int ij1Reg, ij2Reg;
		int ij1RegKin, ij2RegKin;
	
		if(DEBUG) cout << "btaggedJet.size()= " << btaggedJet.size() << endl;
		if(DEBUG)
			for(int ijet_=0; ijet_ < (int)btaggedJet.size() ; ijet_++)
				cout << "jetPt[btaggedJet[" << ijet_ << "]]= " << jetPt[btaggedJet[ijet_]] << endl;
		// if exactly one btag, pick it up, then find the other jet that gives max ptjj
		if( btaggedJet.size() == 1 )
		{
			if(DEBUG) cout << "Entering jet combinatorics: 1btag category" << endl;
			category = 1;
			unsigned int ij = btaggedJet[0];
			if(DEBUG) cout << "btaggedJet[0]= " << btaggedJet[0] << endl;
			TLorentzVector j, jreg, jregkin;
			j.SetPtEtaPhiE(jetPt[ij], jetEta[ij], jetPhi[ij], jetE[ij]);
			jreg = ((float)jetRegPt[ij]/(float)jetPt[ij]) * j;
			jregkin = ((float)jetRegKinPt[ij]/(float)jetPt[ij]) * j;
			int imaxptjj;
			int imaxptjjReg;
			int imaxptjjRegKin;
			float maxptjj = -99.;
			float maxptjjReg = -99.;
			float maxptjjRegKin = -99.;
			for(unsigned int ijet = 0 ; ijet < jetPt.size() ; ijet++)
			{
				if( ijet == ij ) continue;
				TLorentzVector tmp_j;
				TLorentzVector tmp_jReg;
				TLorentzVector tmp_jRegKin;
				tmp_j.SetPtEtaPhiE(jetPt[ijet], jetEta[ijet], jetPhi[ijet], jetE[ijet]);
				tmp_jReg = ((float)jetRegPt[ijet]/(float)jetPt[ijet]) * tmp_j;
				tmp_jRegKin = ((float)jetRegKinPt[ijet]/(float)jetPt[ijet]) * tmp_j;
				TLorentzVector jj = j + tmp_j;
				TLorentzVector jjReg = jreg + tmp_jReg;
				TLorentzVector jjRegKin = jregkin + tmp_jRegKin;
				if( jj.Pt() > maxptjj )
				{
					maxptjj = jj.Pt();
					imaxptjj = ijet;
				}
				if( jjReg.Pt() > maxptjjReg )
				{
					maxptjjReg = jjReg.Pt();
					imaxptjjReg = ijet; 
				}
				if( jjRegKin.Pt() > maxptjjRegKin )
				{
					maxptjjRegKin = jjRegKin.Pt();
					imaxptjjRegKin = ijet; 
				}
			}
			ij1 = ij;
			ij2 = imaxptjj;
			ij1Reg = ij;
			ij2Reg = imaxptjjReg;
			ij1RegKin = ij;
			ij2RegKin = imaxptjjRegKin;
		}
		// if two or more bjets, then loop only over btagged jets
		if( btaggedJet.size() > 1 )
		{
			category = 2;
			if(DEBUG) cout << "Entering jet combinatorics: 2btag category" << endl;
			int ij;
			int imaxptjj;
			int imaxptjjReg;
			int imaxptjjRegKin;
			int jmaxptjj;
			int jmaxptjjReg;
			int jmaxptjjRegKin;
			float maxptjj = -99.;
			float maxptjjReg = -99.;
			float maxptjjRegKin = -99.;
			for( unsigned int i = 0 ; i < btaggedJet.size() - 1 ; i++ )
			{
				ij = btaggedJet[i];
				if(DEBUG) cout << "btaggedJet[" << i << "]= " << ij << "\tjetPt[" << ij << "]= " << jetPt[ij] << endl;
				TLorentzVector j, jreg, jregkin;
				j.SetPtEtaPhiE(jetPt[ij], jetEta[ij], jetPhi[ij], jetE[ij]);
				jreg = ((float)jetRegPt[ij]/(float)jetPt[ij]) * j;
				jregkin = ((float)jetRegKinPt[ij]/(float)jetPt[ij]) * j;
				for(unsigned int k = i+1 ; k < btaggedJet.size() ; k++)
				{
					int ijet = btaggedJet[k];
					TLorentzVector tmp_j;
					TLorentzVector tmp_jReg;
					TLorentzVector tmp_jRegKin;
					tmp_j.SetPtEtaPhiE(jetPt[ijet], jetEta[ijet], jetPhi[ijet], jetE[ijet]);
					tmp_jReg = ((float)jetRegPt[ijet]/(float)jetPt[ijet]) * tmp_j;
					tmp_jRegKin = ((float)jetRegKinPt[ijet]/(float)jetPt[ijet]) * tmp_j;
					TLorentzVector jj = j + tmp_j;
					TLorentzVector jjReg = jreg + tmp_jReg;
					TLorentzVector jjRegKin = jregkin + tmp_jRegKin;
					if(DEBUG) cout << "btaggedJet[" << k << "]= " << btaggedJet[k] << "\tjetPt[" << ijet << "]= " << jetPt[ijet] << "\tjj.Pt()= " << jj.Pt() << "\t(maxptjj= " << maxptjj << ")" << endl;
					if( jj.Pt() > maxptjj )
					{
						maxptjj = jj.Pt();
						imaxptjj = ij;
						jmaxptjj = ijet;
					}
					if( jjReg.Pt() > maxptjjReg )
					{
						maxptjjReg = jjReg.Pt();
						imaxptjjReg = ij;
						jmaxptjjReg = ijet; 
					}
					if( jjRegKin.Pt() > maxptjjRegKin )
					{
						maxptjjRegKin = jjRegKin.Pt();
						imaxptjjRegKin = ij;
						jmaxptjjRegKin = ijet; 
					}
				}
			} // end of loop over bjets
			ij1 = imaxptjj;
			ij2 = jmaxptjj;
			ij1Reg = imaxptjjReg;
			ij2Reg = jmaxptjjReg;
			ij1RegKin = imaxptjjReg;
			ij2RegKin = jmaxptjjRegKin;
		} // end of if two bjets

		TLorentzVector pho1;
		TLorentzVector pho2;
		TLorentzVector jet1;
		TLorentzVector jet2;
		TLorentzVector regjet1;
		TLorentzVector regjet2;
		TLorentzVector regkinjet1;
		TLorentzVector regkinjet2;
		TLorentzVector kinjet1;
		TLorentzVector kinjet2;
		pho1.SetPtEtaPhiE(ph1_pt, ph1_eta, ph1_phi, ph1_e);
		pho2.SetPtEtaPhiE(ph2_pt, ph2_eta, ph2_phi, ph2_e);
		jet1.SetPtEtaPhiE(jetPt[ij1], jetEta[ij1], jetPhi[ij1], jetE[ij1]);
		jet2.SetPtEtaPhiE(jetPt[ij2], jetEta[ij2], jetPhi[ij2], jetE[ij2]);
		regjet1.SetPtEtaPhiE(jetPt[ij1Reg], jetEta[ij1Reg], jetPhi[ij1Reg], jetE[ij1Reg]);
		regjet2.SetPtEtaPhiE(jetPt[ij2Reg], jetEta[ij2Reg], jetPhi[ij2Reg], jetE[ij2Reg]);
		regjet1 = ((float)jetRegPt[ij1Reg]/(float)jetPt[ij1Reg]) * regjet1;
		regjet2 = ((float)jetRegPt[ij2Reg]/(float)jetPt[ij2Reg]) * regjet2;
		regkinjet1 = regjet1;
		regkinjet2 = regjet2;
		float Hmass = 125.;
		DiJetKinFitter* fitter_jetsH = new DiJetKinFitter( "fitter_jetsH", "fitter_jets", Hmass );
		pair<TLorentzVector,TLorentzVector> jets_kinfitH = fitter_jetsH->fit(regkinjet1, regkinjet2);
		regkinjet1 = jets_kinfitH.first;
		regkinjet2 = jets_kinfitH.second;
		kinjet1 = jet1;
		kinjet2 = jet2;
		jets_kinfitH = fitter_jetsH->fit(kinjet1, kinjet2);
		kinjet1 = jets_kinfitH.first;
		kinjet2 = jets_kinfitH.second;

		TLorentzVector jj = jet1 + jet2;
		TLorentzVector regjj = regjet1 + regjet2;
		TLorentzVector regkinjj = regkinjet1 + regkinjet2;
		TLorentzVector kinjj = kinjet1 + kinjet2;
		TLorentzVector gg = pho1 + pho2;
		TLorentzVector ggjj = jj + gg;
		TLorentzVector regggjj = regjj + gg;
		TLorentzVector regkinggjj = regkinjj + gg;
		TLorentzVector kinggjj = kinjj + gg;

		selection_cut_level = 0;
		weight = ev_weight;
		evweight = ev_evweight;
		pu_weight = ev_pu_weight;
		pho1_pt = pho1.Pt();
		pho1_e = pho1.E();
		pho1_phi = pho1.Phi();
		pho1_eta = pho1.Eta();
		pho1_mass = pho1.M();
		pho1_r9 = ph1_r9;
		pho2_pt = pho2.Pt();
		pho2_e = pho2.E();
		pho2_phi = pho2.Phi();
		pho2_eta = pho2.Eta();
		pho2_mass = pho2.M();
		pho2_r9 = ph2_r9;
		jet1_pt = jet1.Pt();
		jet1_e = jet1.E();
		jet1_phi = jet1.Phi();
		jet1_eta = jet1.Eta();
		jet1_mass = jet1.M();
		jet1_csvBtag = jetCSV[ij1];
		jet1_btagSF = jetbtagSF[ij1];
		jet2_pt = jet2.Pt();
		jet2_e = jet2.E();
		jet2_phi = jet2.Phi();
		jet2_eta = jet2.Eta();
		jet2_mass = jet2.M();
		jet2_csvBtag = jetCSV[ij2];
		jet2_btagSF = jetbtagSF[ij2];
		regjet1_pt = regjet1.Pt();
		regjet1_e = regjet1.E();
		regjet1_phi = regjet1.Phi();
		regjet1_eta = regjet1.Eta();
		regjet1_mass = regjet1.M();
		regjet1_csvBtag = jetCSV[ij1Reg];
		regjet1_btagSF = jetbtagSF[ij1Reg];
		regjet2_pt = regjet2.Pt();
		regjet2_e = regjet2.E();
		regjet2_phi = regjet2.Phi();
		regjet2_eta = regjet2.Eta();
		regjet2_mass = regjet2.M();
		regjet2_csvBtag = jetCSV[ij2Reg];
		regjet2_btagSF = jetbtagSF[ij2Reg];
		regjet1_emfrac = jetEmfrac[ij1Reg];
		regjet1_hadfrac = jetHadfrac[ij1Reg];
		regjet1_secVtxPt = jetSecVtxPt[ij1Reg];
		regjet1_secVtx3dL = jetSecVtx3dL[ij1Reg];
		regjet1_dPhiMet = jetDPhiMet[ij1Reg];
		regjet1_nConstituents = jetNConstituents[ij1Reg];
		regjet2_emfrac = jetEmfrac[ij2Reg];
		regjet2_hadfrac = jetHadfrac[ij2Reg];
		regjet2_secVtxPt = jetSecVtxPt[ij2Reg];
		regjet2_secVtx3dL = jetSecVtx3dL[ij2Reg];
		regjet2_dPhiMet = jetDPhiMet[ij2Reg];
		regjet2_nConstituents = jetNConstituents[ij2Reg];
		regkinjet1_pt = regkinjet1.Pt();
		regkinjet1_e = regkinjet1.E();
		regkinjet1_phi = regkinjet1.Phi();
		regkinjet1_eta = regkinjet1.Eta();
		regkinjet1_mass = regkinjet1.M();
		regkinjet1_csvBtag = jetCSV[ij1RegKin];
		regkinjet1_btagSF = jetbtagSF[ij1RegKin];
		regkinjet2_pt = regkinjet2.Pt();
		regkinjet2_e = regkinjet2.E();
		regkinjet2_phi = regkinjet2.Phi();
		regkinjet2_eta = regkinjet2.Eta();
		regkinjet2_mass = regkinjet2.M();
		regkinjet2_csvBtag = jetCSV[ij2RegKin];
		regkinjet2_btagSF = jetbtagSF[ij2RegKin];
		kinjet1_pt = kinjet1.Pt();
		kinjet1_e = kinjet1.E();
		kinjet1_phi = kinjet1.Phi();
		kinjet1_eta = kinjet1.Eta();
		kinjet1_mass = kinjet1.M();
		kinjet1_csvBtag = jetCSV[ij1];
		kinjet1_btagSF = jetbtagSF[ij1];
		kinjet2_pt = kinjet2.Pt();
		kinjet2_e = kinjet2.E();
		kinjet2_phi = kinjet2.Phi();
		kinjet2_eta = kinjet2.Eta();
		kinjet2_mass = kinjet2.M();
		kinjet2_csvBtag = jetCSV[ij2];
		kinjet2_btagSF = jetbtagSF[ij2];
		jj_pt = jj.Pt();
		jj_e = jj.E();
		jj_phi = jj.Phi();
		jj_eta = jj.Eta();
		jj_mass = jj.M();
		jj_btagSF = jet1_btagSF * jet2_btagSF;
		jj_DR = jet1.DeltaR(jet2);
		regjj_pt = regjj.Pt();
		regjj_e = regjj.E();
		regjj_phi = regjj.Phi();
		regjj_eta = regjj.Eta();
		regjj_mass = regjj.M();
		regjj_btagSF = regjet1_btagSF * regjet2_btagSF;
		regjj_DR = regjet1.DeltaR(regjet2);
		regkinjj_pt = regkinjj.Pt();
		regkinjj_e = regkinjj.E();
		regkinjj_phi = regkinjj.Phi();
		regkinjj_eta = regkinjj.Eta();
		regkinjj_mass = regkinjj.M();
		regkinjj_btagSF = regkinjet1_btagSF * regkinjet2_btagSF;
		regkinjj_DR = regkinjet1.DeltaR(regkinjet2);
		kinjj_pt = kinjj.Pt();
		kinjj_e = kinjj.E();
		kinjj_phi = kinjj.Phi();
		kinjj_eta = kinjj.Eta();
		kinjj_mass = kinjj.M();
		kinjj_btagSF = kinjet1_btagSF * kinjet2_btagSF;
		kinjj_DR = kinjet1.DeltaR(kinjet2);
		gg_pt = gg.Pt();
		gg_e = gg.E();
		gg_phi = gg.Phi();
		gg_eta = gg.Eta();
		gg_mass = gg.M();
		ggjj_pt = ggjj.Pt();
		ggjj_e = ggjj.E();
		ggjj_phi = ggjj.Phi();
		ggjj_eta = ggjj.Eta();
		ggjj_mass = ggjj.M();
		regggjj_pt = regggjj.Pt();
		regggjj_e = regggjj.E();
		regggjj_phi = regggjj.Phi();
		regggjj_eta = regggjj.Eta();
		regggjj_mass = regggjj.M();
		regkinggjj_pt = regkinggjj.Pt();
		regkinggjj_e = regkinggjj.E();
		regkinggjj_phi = regkinggjj.Phi();
		regkinggjj_eta = regkinggjj.Eta();
		regkinggjj_mass = regkinggjj.M();
		kinggjj_pt = kinggjj.Pt();
		kinggjj_e = kinggjj.E();
		kinggjj_phi = kinggjj.Phi();
		kinggjj_eta = kinggjj.Eta();
		kinggjj_mass = kinggjj.M();
		njets_kLooseID = njets_passing_kLooseID;
		njets_kLooseID_and_CSVM = njets_passing_kLooseID_and_CSVM;
		njets_kRadionID = njets_kRadionID_;
		njets_kRadionID_and_CSVM = njets_kRadionID_and_CSVM_;
// costhetastar
		TLorentzVector Hgg_Rstar(gg);
		TLorentzVector regHgg_Rstar(gg);
		TLorentzVector regkinHgg_Rstar(gg);
		TLorentzVector kinHgg_Rstar(gg);
		Hgg_Rstar.Boost(-ggjj.BoostVector());
		regHgg_Rstar.Boost(-regggjj.BoostVector());
		regkinHgg_Rstar.Boost(-regkinggjj.BoostVector());
		kinHgg_Rstar.Boost(-kinggjj.BoostVector());
		costhetastar = Hgg_Rstar.CosTheta();
		regcosthetastar = regHgg_Rstar.CosTheta();
		regkincosthetastar = regkinHgg_Rstar.CosTheta();
		kincosthetastar = kinHgg_Rstar.CosTheta();
// min DR(g, j)
		minDRgj = 999999.0;
		minDRgregj = 999999.0;
		minDRgregkinj = 999999.0;
		minDRgkinj = 999999.0;
		minDRgj = min(minDRgj, (float)pho1.DeltaR(jet1));
		minDRgj = min(minDRgj, (float)pho1.DeltaR(jet2));
		minDRgj = min(minDRgj, (float)pho2.DeltaR(jet1));
		minDRgj = min(minDRgj, (float)pho2.DeltaR(jet2));
		minDRgregj = min(minDRgregj, (float)pho1.DeltaR(regjet1));
		minDRgregj = min(minDRgregj, (float)pho1.DeltaR(regjet2));
		minDRgregj = min(minDRgregj, (float)pho2.DeltaR(regjet1));
		minDRgregj = min(minDRgregj, (float)pho2.DeltaR(regjet2));
		minDRgregkinj = min(minDRgregkinj, (float)pho1.DeltaR(regkinjet1));
		minDRgregkinj = min(minDRgregkinj, (float)pho1.DeltaR(regkinjet2));
		minDRgregkinj = min(minDRgregkinj, (float)pho2.DeltaR(regkinjet1));
		minDRgregkinj = min(minDRgregkinj, (float)pho2.DeltaR(regkinjet2));
		minDRgkinj = min(minDRgkinj, (float)pho1.DeltaR(kinjet1));
		minDRgkinj = min(minDRgkinj, (float)pho1.DeltaR(kinjet2));
		minDRgkinj = min(minDRgkinj, (float)pho2.DeltaR(kinjet1));
		minDRgkinj = min(minDRgkinj, (float)pho2.DeltaR(kinjet2));

		if(regjet1_btagSF == -1001 || regjet2_btagSF == -1001)
		{
			cout << "WARNING: undefined btagSF, skipping the event:\tevent= " << event << "\tregjet1_btagSF= " << regjet1_btagSF << "\tregjet2_btagSF= " << regjet2_btagSF << "\tregjet1_pt= " << regjet1_pt << "\tregjet2_pt= " << regjet2_pt << endl;
			continue;
		}

// categorisation
		selection_cut_level = 0;
		if(SYNCHRO) synchrofile << jet1_pt << "\t" << jet2_pt << "\t" << jj_mass << "\t" << ggjj_mass << endl;
		if(njets_kRadionID_and_CSVM == 1)
		{
			category = 1;
			nevents[13]++;
			nevents_w[13] += weight;
			nevents_sync[8]++;
			eventcut[13] = "1btag category";
		} else if( njets_kRadionID_and_CSVM >=2) {
			category = 2;
			nevents[14]++;
			nevents_w[14] += weight;
			nevents_sync[9]++;
			eventcut[14] = "2btag category";
		}

// | cos theta* | <.9
		selection_cut_level = 1;
/*
		if( fabs(regcosthetastar) >= .9 )
		{
			outtree->Fill();
			continue;
		}
		if(njets_kRadionID_and_CSVM == 1)
		{
			category = 1;
			nevents[15]++;
			eventcut[15] = "1btag category, after | cos theta* | <.9";
		} else if( njets_kRadionID_and_CSVM >=2) {
			category = 2;
			nevents[16]++;
			eventcut[16] = "2btag category, after | cos theta* | <.9";
		}
*/


// mjj cut (90/150 for 1btag and 95/140 for 2btags)
		if(SYNC_W_CHIARA && n_sync_events_before_mjj == 0) cout << "event\tjet1_index\tjet2_index\tjet1_regPt\tjet2_regPt\tregjj_mass" << endl;
		if(SYNC_W_CHIARA && n_sync_events_before_mjj < 10) cout << event << "\t" << ij1Reg << "\t" << ij2Reg << "\t" << regjet1_pt << "\t" << regjet2_pt << "\t" << regjj_mass << endl;
		if(SYNC_W_CHIARA) n_sync_events_before_mjj++;
		selection_cut_level = 2;
		bool pass_mjj = false;
		if(!SYNC)
			pass_mjj = (njets_kRadionID_and_CSVM == 1 && (regjj_mass < 90. || regjj_mass > 150.)) || (njets_kRadionID_and_CSVM >= 2 && (regjj_mass < 95. || regjj_mass > 140.));
		else
			pass_mjj = (njets_kRadionID_and_CSVM == 1 && (regjj_mass < 90. || regjj_mass > 165.)) || (njets_kRadionID_and_CSVM >= 2 && (regjj_mass < 95. || regjj_mass > 140.));
		if(pass_mjj)
		{
			outtree->Fill();
			continue;
		}
		if(njets_kRadionID_and_CSVM == 1)
		{
			category = 1;
			nevents[17]++;
			nevents_w[17] += weight;
			nevents_sync[10]++;
			if(SYNC)
				eventcut[17] = "1btag category, after mjj cut (90/165 for 1btag and 95/140 for 2btags)";
			else
				eventcut[17] = "1btag category, after mjj cut (90/150 for 1btag and 95/140 for 2btags)";
		} else if( njets_kRadionID_and_CSVM >=2) {
			category = 2;
			nevents[18]++;
			nevents_w[18] += weight;
			nevents_sync[11]++;
			if(SYNC)
				eventcut[18] = "2btag category, after mjj cut (90/165 for 1btag and 95/140 for 2btags)";
			else
				eventcut[18] = "2btag category, after mjj cut (90/150 for 1btag and 95/140 for 2btags)";
		}

// kin fit
// MOVED UPSTREAM, SHOULD BE TRANSPARENT
	
		if(njets_kRadionID_and_CSVM == 1)
		{
			category = 1;
			nevents[19]++;
			eventcut[19] = "1btag category, after kin fit";
		} else if( njets_kRadionID_and_CSVM >=2) {
			category = 2;
			nevents[20]++;
			eventcut[20] = "2btag category, after kin fit";
		}

		if(SYNC_W_CHIARA_GGJJ && n_sync_events_before_mggjj == 0) cout << "event\tregjet1_pt\tregjet2_pt\tregkinjet1_pt\tregkinjet2_pt\tregjj_mass\tregggjj_mass\tregkinjj_mass\tregkinggjj_mass" << endl; 
		if(SYNC_W_CHIARA_GGJJ && n_sync_events_before_mggjj < 10) cout << event << "\t" << regjet1_pt << "\t" << regjet2_pt << "\t" << regkinjet1_pt << "\t" << regkinjet2_pt << "\t" << regjj_mass << "\t" << regggjj_mass << "\t" << regkinjj_mass << "\t" << regkinggjj_mass << endl;
		if(SYNC_W_CHIARA_GGJJ) n_sync_events_before_mggjj++;

// mggjj cut (260/335 and 255/320)
		selection_cut_level = 3;
		bool pass_mggjj_cut = false;
		if(!SYNC) 
			pass_mggjj_cut = (njets_kRadionID_and_CSVM == 1 && (regkinggjj_mass < 260. || regkinggjj_mass > 335.) ) || (njets_kRadionID_and_CSVM >= 2 && (regkinggjj_mass < 255. || regkinggjj_mass > 320.) );
		else
			pass_mggjj_cut = (njets_kRadionID_and_CSVM == 1 && (regkinggjj_mass < 255. || regkinggjj_mass > 340.) ) || (njets_kRadionID_and_CSVM >= 2 && (regkinggjj_mass < 265. || regkinggjj_mass > 320.) );
		if( pass_mggjj_cut )
		{
			outtree->Fill();
			continue;
		}
		if(njets_kRadionID_and_CSVM == 1)
		{
			category = 1;
			nevents[21]++;
			nevents_w[21] += weight;
			nevents_sync[12]++;
			if(!SYNC)
				eventcut[21] = "1btag category, after mggjj cut (260/335 and 255/320)";
			else
				eventcut[21] = "1btag category, after mggjj cut (255/340 and 265/320)";
		} else if( njets_kRadionID_and_CSVM >=2) {
			category = 2;
			nevents[22]++;
			nevents_w[22] += weight;
			nevents_sync[13]++;
			if(!SYNC)
				eventcut[22] = "2btag category, after mggjj cut (260/335 and 255/320)";
			else
				eventcut[22] = "2btag category, after mggjj cut (255/340 and 265/320)";
		}

// deltaR(g,j) >= 1
		selection_cut_level = 4;
		if( minDRgregkinj < 1.)
		{
			outtree->Fill();
			continue;
		}
		if(njets_kRadionID_and_CSVM == 1)
		{
			category = 1;
			nevents[23]++;
			eventcut[23] = "1btag category, after deltaR(g,j) >= 1";
		} else if( njets_kRadionID_and_CSVM >=2) {
			category = 2;
			nevents[24]++;
			eventcut[24] = "2btag category, after deltaR(g,j) >= 1";
		}

// njets < 4
		selection_cut_level = 5;
		if( njets_passing_kLooseID >= 3 )
		{
			outtree->Fill();
			continue;
		}
		if(njets_kRadionID_and_CSVM == 1)
		{
			category = 1;
			nevents[25]++;
			eventcut[25] = "1btag category, after njets < 4";
		} else if( njets_kRadionID_and_CSVM >=2) {
			category = 2;
			nevents[26]++;
			eventcut[26] = "2btag category, after njets < 4";
		}



		selection_cut_level = 6;
		outtree->Fill();

	} // end of loop over events


	for(int i=0 ; i < 27 ; i++)
    if(!SYNCHRO_LIGHT) cout << "#nevents[" << i << "]= " << nevents[i] << "\t#nevents_w[" << i << "]= " << nevents_w[i] << "\teventcut[" << i << "]= " << eventcut[i] << "\t\t( 1btag= " << nevents1btag[i] << " , 2btag= " << nevents2btag[i] << " ) " << endl;
    else
		{
			if(i>0 && i< 5) continue;
			else if(i==8) continue;
			else cout << nevents[i] << endl;
		}
	if(!SYNCHRO_LIGHT) cout << endl;
	if(!SYNCHRO_LIGHT)
		for(int i=0 ; i < 6 ; i++)
    	cout << "#njets[" << i << "]= " << njets[i] << "\tjetcut[" << i << "]= " << jetcut[i] << endl;

	if(SYNC)
		for(int i=0 ; i < 14 ; i++)
			cout << nevents_sync[i] << endl;

	if(SYNCHRO) synchrofile.close();
	if(SYNCHRO_GGANA) synchrofile.close();
  outfile->cd();
  outtree->Write();
  outfile->Close();
  infile->Close();


	return 0;
}

