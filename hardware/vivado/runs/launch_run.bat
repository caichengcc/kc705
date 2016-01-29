@echo off
if "%1" == "bist" (
  del k7_emb_bist /f /s /q 
  rd k7_emb_bist /s /q 
  mkdir k7_emb_bist
  cd k7_emb_bist
  echo " ***********************************************************************"
  echo "     K7 Embedded BIST Design Implementation takes around 1.5 hrs        "
  echo " ***********************************************************************"
  vivado -mode batch -source ../../scripts/k7_emb_bist_project.tcl 
)  else (
  if "%1" == "vdemo"  ( 
    del k7_emb_vdemo /f /s /q 
    rd k7_emb_vdemo /s /q 
    mkdir k7_emb_vdemo
    cd k7_emb_vdemo
  echo " ***********************************************************************"
  echo "    K7 Embedded Video Design Implementation takes around 3.5 hrs        "
  echo " ***********************************************************************"
    vivado -mode batch -source ../../scripts/k7_emb_vdemo_project.tcl 
  )    else  (
  echo " ***********************************************************************"
  echo "                     < Bad Argument passed >                            "
  echo "    For valid Arguments please refer readme.txt file                    "
  echo " ***********************************************************************"
  )  
)