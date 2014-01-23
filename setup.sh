#!/bin/bash

WORKDIR=${PWD}

# MAKE SURE KINFIT UTILITIES ARE HERE
if [[ ! -d ../KinematicFit ]]
then
	echo "SETUP KINFIT"
	cd ..
	cvs co -d KinematicFit UserCode/pandolf/KinematicFit
	cd ${WORKDIR}
fi

# MAKE SURE BTAGSF UTILITIES ARE HERE
if [[ ! -d ../h2gglobe/BTagUtils.cc ]]
then
	echo "WARNING: NO BTagUtils.cc CAN BE SEEN, YOU SHOULD NOT BE ABLE TO RUN LIKE THIS...."
fi

# DEFINE ROOT IF NOT DEFINED
if [[ -z ${ROOTSYS} ]]
then
	echo "SETUP ROOT"
	source /afs/cern.ch/sw/lcg/contrib/gcc/4.3/x86_64-slc5/setup.sh
	source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.32.00/x86_64-slc5-gcc43-opt/root/bin/thisroot.sh
fi

# LINK TO BOOST LIBRAIRIES FOR COMPILATION TIME
if [[ -z ${BOOST_ROOT} ]]
then
	echo "SETUP BOOST"
	export BOOST_ROOT=/afs/cern.ch/sw/lcg/external/Boost/1.48.0_python2.6/x86_64-slc5-gcc43-opt/
	export BOOST_INCLUDEDIR=/afs/cern.ch/sw/lcg/external/Boost/1.48.0_python2.6/x86_64-slc5-gcc43-opt/include/boost-1_48/
	export BOOST_LIBRARYDIR=${BOOST_ROOT}/lib
	export LD_LIBRARY_PATH=${BOOST_LIBRARYDIR}:${LD_LIBRARY_PATH}
fi

