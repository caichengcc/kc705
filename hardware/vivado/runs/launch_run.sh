#!/bin/bash

if [ "$1" == "bist" ]; then
  rm -rf k7_emb_bist
  mkdir k7_emb_bist
  cd k7_emb_bist
  echo " ***********************************************************************"
  echo "     K7 Embedded BIST Design Implementation takes around 1.5 hrs        "
  echo " ***********************************************************************"
  vivado -mode batch -source ../../scripts/k7_emb_bist_project.tcl 
elif [ "$1" == "vdemo" ]; then
  rm -rf k7_emb_vdemo
  mkdir k7_emb_vdemo
  cd k7_emb_vdemo
  echo " ***********************************************************************"
  echo "    K7 Embedded Video Design Implementation takes around 3.5 hrs        "
  echo " ***********************************************************************"
  vivado -mode batch -source ../../scripts/k7_emb_vdemo_project.tcl
else
  echo " ***********************************************************************"
  echo "                     < Bad Argument passed >                            "
  echo "    For valid Arguments please refer readme.txt file                    "
  echo " ***********************************************************************"
fi
