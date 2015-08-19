#!/bin/sh
cat entitylistheader.txt > entitylist.txt
cd ../../code
grep -E 'Ifc([A-Z][a-zA-Z0-9]*)+' -o IFCLoader.cpp IFCGeometry.cpp IFCCurve.cpp IFCProfile.cpp IFCMaterial.cpp | sed s/.*:// | sort | uniq >> ../scripts/IFCImporter/entitylist.txt
