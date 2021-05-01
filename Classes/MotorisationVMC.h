//---------------------------------------------------------------------------

#ifndef MotorisationVMCH
#define MotorisationVMCH

class MotorisationVMC
{
private:
    int adresseCarte;
protected:
public:
  MotorisationVMC();
  ~MotorisationVMC();
  int connecterCarte();
  void deconnecterCarte();
  void reglerVitesseMoteur(int dm2Zone0, int dm2Zone1);
  void arreterMoteur();
  int getAdresseCarte()  {return adresseCarte;}

};

//---------------------------------------------------------------------------
#endif
 