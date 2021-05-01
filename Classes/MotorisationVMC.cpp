//---------------------------------------------------------------------------

#pragma hdrstop

#include "MotorisationVMC.h"
#include "K8055D.h"

MotorisationVMC hMotorisationVMC;

MotorisationVMC::MotorisationVMC() {
  adresseCarte = 0;
  //connecterCarte();
}


MotorisationVMC::~MotorisationVMC() {
  arreterMoteur();
}

int MotorisationVMC::connecterCarte () {
  int flag = OpenDevice(adresseCarte);
  return flag;
}

void MotorisationVMC::deconnecterCarte () {
  CloseDevice;
}

void MotorisationVMC::reglerVitesseMoteur(int dm2Zone0, int dm2Zone1) {
  float tensionSortie[2];
  float valueAOutput[2];

  if (dm2Zone0 != -1)
    {
    tensionSortie[0] = (float)dm2Zone0 * 0.025;
    valueAOutput[0] = 255 * tensionSortie[0] / 5;
    }
  else
    valueAOutput[0] = 0;

    
  if (dm2Zone1 != -1)
    {
    tensionSortie[1] = (float)dm2Zone1 * 0.025;
    valueAOutput[1] = 255 * tensionSortie[1] / 5;
    }
  else
    valueAOutput[1] = 0;

  OutputAllAnalog((int)valueAOutput[0],(int)valueAOutput[1]);
}

void MotorisationVMC::arreterMoteur() {
  OutputAllAnalog(0,0);
}

//---------------------------------------------------------------------------
#pragma package(smart_init)
