#!/bin/sh
cat entitylistheader.txt > entitylist.txt
cd ../../code
grep -E 'Ifc([A-Z][a-z]*)+' -o IFCLoader.cpp IFCGeometry.cpp IFCCurve.cpp IFCProfile.cpp IFCMaterial.cpp | sort | uniq | sed s/.*:// >> ../scripts/IFCImporter/entitylist.txt
