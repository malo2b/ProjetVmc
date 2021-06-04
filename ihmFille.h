//---------------------------------------------------------------------------

#ifndef ihmFilleH
#define ihmFilleH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ihm.h"
#include <ComCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------

class TFP;

class TFF : public TForm
{
__published:	// Composants gérés par l'EDI
    TLabel *LabelZoneId;
    TProgressBar *ProgressBarHygrometrie;
    TLabel *LabelHygrometrie;
    TLabel *LabelVolumeZone;
    TLabel *LabelTauxHygrometrie;
    TLabel *LabelZoneType;
    TLabel *LabelMotorisationEtat;
    TLabel *LabelDm2;
    TStringGrid *GridPlageDeFonctionnement;
    TLabel *LabelPlageDeFonctionnement;
private:	// Déclarations utilisateur
    TFP* FP;
    int zoneId;
    float volumeZone;
    int hygrometrie;
    int debitVolumiqueExtraction;

public:		// Déclarations utilisateur
    void setData(int pHygrometrie, int pDebitVolumiqueExtraction);
    void setMotorisationEtat(bool active);
        __fastcall TFF(TComponent* Owner);
        __fastcall TFF(TComponent* Owner, int pzoneId, float pvolumeZone, int pzoneType, unsigned char heureFonctionnement[4], bool jourDeFonctionnement[7]);
};
//---------------------------------------------------------------------------
extern PACKAGE TFF *FF;
//---------------------------------------------------------------------------
#endif
