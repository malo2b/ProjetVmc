//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ihmFille.h"
#include "ihm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


extern TFP* FP;

//---------------------------------------------------------------------------
__fastcall TFF::TFF(TComponent* Owner)
        : TForm(Owner)
{
FP = (TFP *)Owner;
}
//---------------------------------------------------------------------------

__fastcall TFF::TFF(TComponent* Owner, int pzoneId, float pvolumeZone, int pzoneType, unsigned char heureFonctionnement[4], bool jourDeFonctionnement[7]) : TForm(Owner)
{
FP = (TFP *)Owner;
zoneId = pzoneId;
LabelZoneId->Caption = "Zone n°" + AnsiString(zoneId);
volumeZone = pvolumeZone;
LabelVolumeZone->Caption = "Volume de la zone : " + AnsiString(volumeZone) + " m3";

GridPlageDeFonctionnement->Cells[0][0] = "Lundi";
GridPlageDeFonctionnement->Cells[1][0] = "Mardi";
GridPlageDeFonctionnement->Cells[2][0] = "Mercredi";
GridPlageDeFonctionnement->Cells[3][0] = "Jeudi";
GridPlageDeFonctionnement->Cells[4][0] = "Vendredi";
GridPlageDeFonctionnement->Cells[5][0] = "Samedi";
GridPlageDeFonctionnement->Cells[6][0] = "Dimanche";

for (int i = 0; i < 7; i++)
    {
    if (jourDeFonctionnement[i])
        GridPlageDeFonctionnement->Cells[i][1] = "Active";
    else
        GridPlageDeFonctionnement->Cells[i][1] = "Inactive";
    }

LabelPlageDeFonctionnement->Caption = LabelPlageDeFonctionnement->Caption + "\n de : ";  

if (heureFonctionnement[0] < 10)
    LabelPlageDeFonctionnement->Caption = LabelPlageDeFonctionnement->Caption + "0";
LabelPlageDeFonctionnement->Caption = LabelPlageDeFonctionnement->Caption + AnsiString(heureFonctionnement[0]) + "h";
if (heureFonctionnement[1] < 10)
    LabelPlageDeFonctionnement->Caption = LabelPlageDeFonctionnement->Caption + "0";
LabelPlageDeFonctionnement->Caption = LabelPlageDeFonctionnement->Caption + AnsiString(heureFonctionnement[1]) + " à ";

if (heureFonctionnement[2] < 10)
    LabelPlageDeFonctionnement->Caption = LabelPlageDeFonctionnement->Caption + "0";
LabelPlageDeFonctionnement->Caption = LabelPlageDeFonctionnement->Caption + AnsiString(heureFonctionnement[2]) + "h";
if (heureFonctionnement[3] < 10)
    LabelPlageDeFonctionnement->Caption = LabelPlageDeFonctionnement->Caption + "0";
LabelPlageDeFonctionnement->Caption = LabelPlageDeFonctionnement->Caption + AnsiString(heureFonctionnement[3]);




if (pzoneType == 0)
    LabelZoneType->Caption = "Zone Chambre Non Fumeur";
else if (pzoneType == 1)
    LabelZoneType->Caption = "Zone Chambre Fumeur";
else if (pzoneType == 2)
    LabelZoneType->Caption = "Zone Salle Restauration";
else if (pzoneType == 3)
    LabelZoneType->Caption = "Zone Bureaux";
else if (pzoneType == 4)
    LabelZoneType->Caption = "Zone Sanitaires Collectifs";
else if (pzoneType == 5)
    LabelZoneType->Caption = "Zone Salle Hebergement";
else if (pzoneType == 6)
    LabelZoneType->Caption = "Zone Salle Enseignement MPC";
else if (pzoneType == 7)
    LabelZoneType->Caption = "Zone Salle Enseignement Lycee";
}

void TFF::setData(int pHygrometrie, int pDebitVolumiqueExtraction)
    {
    hygrometrie = pHygrometrie;
    ProgressBarHygrometrie->Position = hygrometrie;
    LabelHygrometrie->Caption = AnsiString(hygrometrie) + "%";
    debitVolumiqueExtraction = pDebitVolumiqueExtraction;
    LabelDm2->Caption = "Debit Volumique d'extraction : " + AnsiString(debitVolumiqueExtraction) + " m3/h";
    }

void TFF::setMotorisationEtat(bool active)
    {
    if (active)
        {
        LabelMotorisationEtat->Color = clLime;
        LabelMotorisationEtat->Caption = "VMC : Active";
        }
    else
        {
        LabelMotorisationEtat->Color = clRed;
        LabelMotorisationEtat->Caption = "VMC : Inactive";
        }
    }



