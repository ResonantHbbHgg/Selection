CC        = g++
#CCFLAGS   = -Wall -g
CCFLAGS   = -Wall -O2
SOURCES   =
ROOTFLAGS = `root-config --cflags`
ROOTLIBS  = `root-config --libs --ldflags`
ROOFITLIBS = -lRooFit -lRooFitCore -lMinuit -lFoam -I${ROOFITSYS}include -L${ROOFITSYS}lib
ROOSTATSLIBS = -lRooStats -I${ROOFITSYS}include/RooStats/
# boost
BOOSTFLAGS = -I${BOOST_ROOT}include/boost-1_48
BOOSTLIBS = -L${BOOST_ROOT}lib -lboost_program_options-gcc43-mt-1_48

TMVA = -L${ROOTSYS}lib -lTMVA

all: selection.exe quickTrees.exe obtainWeights.exe

prepareOpTreeInJetTree_forTraining.exe: prepareOpTreeInJetTree_forTraining.cc
	$(CC) $(CCFLAGS) $(ROOTFLAGS) $(ROOTLIBS) prepareOpTreeInJetTree_forTraining.cc -o prepareOpTreeInJetTree_forTraining.exe

prepareCheckOverTraining.exe: prepareCheckOverTraining.cc
	$(CC) $(TMVA) $(CCFLAGS) $(ROOTFLAGS) $(ROOTLIBS) prepareCheckOverTraining.cc -o prepareCheckOverTraining.exe

trainRegression.exe: trainRegression.cc
	$(CC) $(TMVA) $(CCFLAGS) $(ROOTFLAGS) $(ROOTLIBS) trainRegression.cc -o trainRegression.exe

TFitParticleEtEtaPhi.o: KinematicFit/TFitParticleEtEtaPhi.cc
	$(CC) $(CCFLAGS) $(ROOTFLAGS) -c KinematicFit/TFitParticleEtEtaPhi.cc -o TFitParticleEtEtaPhi.o

TKinFitter.o: KinematicFit/TKinFitter.cc
	$(CC) $(CCFLAGS) $(ROOTFLAGS) -c KinematicFit/TKinFitter.cc -o TKinFitter.o

TFitConstraintM.o: KinematicFit/TFitConstraintM.cc
	$(CC) $(CCFLAGS) $(ROOTFLAGS) -c KinematicFit/TFitConstraintM.cc -o TFitConstraintM.o

TAbsFitParticle.o: KinematicFit/TAbsFitParticle.cc
	$(CC) $(CCFLAGS) $(ROOTFLAGS) -c KinematicFit/TAbsFitParticle.cc -o TAbsFitParticle.o

DiJetKinFitter.o: KinematicFit/DiJetKinFitter.cc
	$(CC) $(CCFLAGS) $(ROOTFLAGS) -c KinematicFit/DiJetKinFitter.cc -o DiJetKinFitter.o

TAbsFitConstraint.o: KinematicFit/TAbsFitConstraint.cc
	$(CC) $(CCFLAGS) $(ROOTFLAGS) -c KinematicFit/TAbsFitConstraint.cc -o TAbsFitConstraint.o

selection.o: selection.cc selection.h
	$(CC) $(TMVA) $(CCFLAGS) $(ROOTFLAGS) $(BOOSTFLAGS) -c selection.cc -o selection.o

selection.exe: selection.o DiJetKinFitter.o TKinFitter.o TFitParticleEtEtaPhi.o TAbsFitParticle.o TFitConstraintM.o TAbsFitConstraint.o BTagUtils.o
	$(CC) $(TMVA)  $(ROOTLIBS) $(BOOSTLIBS) selection.o DiJetKinFitter.o TKinFitter.o TFitParticleEtEtaPhi.o TAbsFitParticle.o TFitConstraintM.o TAbsFitConstraint.o BTagUtils.o -o selection.exe

BTagUtils.o: BTagUtils.cc BTagUtils.h
	$(CC) $(CCFLAGS) $(ROOTFLAGS) -c BTagUtils.cc -o BTagUtils.o

quickTrees.o: quickTrees.cc quickTrees.h
	$(CC) $(CCFLAGS) $(ROOTFLAGS) $(BOOSTFLAGS) -c quickTrees.cc -o quickTrees.o

quickTrees.exe: quickTrees.o BTagUtils.o
	$(CC) $(ROOTLIBS) $(BOOSTLIBS) quickTrees.o BTagUtils.o -o quickTrees.exe

obtainWeights.o: obtainWeights.cc
	$(CC) $(CCFLAGS) $(ROOTFLAGS) $(BOOSTFLAGS) -c obtainWeights.cc -o obtainWeights.o

obtainWeights.exe: obtainWeights.o
	$(CC) $(ROOTLIBS) $(BOOSTLIBS) obtainWeights.o -o obtainWeights.exe

fitMass.o: fitMass.cc
	$(CC) $(CCFLAGS) $(ROOTFLAGS) $(ROOFITLIBS) $(BOOSTFLAGS) -c fitMass.cc -o fitMass.o

fitMass.exe: fitMass.o
	$(CC) $(ROOTLIBS) $(ROOFITLIBS) $(BOOSTLIBS) fitMass.o -o fitMass.exe

compare_limitTrees.exe: compare_limitTrees.cc
	$(CC) $(CCFLAGS) $(ROOTFLAGS) $(ROOTLIBS) compare_limitTrees.cc -o compare_limitTrees.exe

undoVarTRansformNorm.exe: undoVarTRansformNorm.cc
	$(CC) $(CCFLAGS) $(ROOTFLAGS) $(ROOTLIBS) undoVarTRansformNorm.cc -o undoVarTRansformNorm.exe

clean:
	rm *.exe; rm *.o
