//---------------------------------------------------------------------------

#ifndef ihmH
#define ihmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ScktComp.hpp>
//---------------------------------------------------------------------------
#include <ExtCtrls.hpp>
#include "Hotel.h"
#include "Enseignement.h"
#include "ChambreFumeur.h"
#include "ChambreNonFumeur.h"
#include <ExtCtrls.hpp>
#include "plageDeFonctionnement.h"
#include "DonneesExploitationParZone.h"
#include <ExtCtrls.hpp>
#include "MotorisationVMC.h"
#include "TimeStamp.h"
#include "ihmFille.h"
#include "BDD_Exploitation.h"
#include "Bureaux.h"
#include <time.h>
//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>     
#include <Buttons.hpp>
#include <Chart.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------

class TFF;
class Bureaux;
class Enseignement;

class TFP : public TForm
{
__published:	// Composants gérés par l'EDI
    TMainMenu *Menu;
    TMenuItem *Application1;
    TMenuItem *ChargerFichierConfiguration1;
    TMenuItem *Quitter1;
    TMenuItem *Affichage1;
    TClientSocket *ClientSocketHygrometrie;
    TTimer *Timer;
    TServerSocket *ServerSocket;
    TMenuItem *Cascade1;
    TMenuItem *Mosaique1;
    TMenuItem *ConnectAutomate;
    TMenuItem *ConnecterBDD;
        TMenuItem *ConnexionMoteur;
    TOpenDialog *OpenDialog;
    void __fastcall Quitter1Click(TObject *Sender);
    void __fastcall ClientSocketHygrometrieRead(TObject *Sender,
    TCustomWinSocket *Socket);
    void __fastcall TimerTimer(TObject *Sender);
    void __fastcall ServerSocketClientRead(TObject *Sender,
    TCustomWinSocket *Socket);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall Cascade1Click(TObject *Sender);
    void __fastcall Mosaique1Click(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall ClientSocketHygrometrieConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocketHygrometrieDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ConnectAutomateClick(TObject *Sender);
    void __fastcall ClientSocketHygrometrieError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall ConnecterBDDClick(TObject *Sender);
        void __fastcall ConnexionMoteurClick(TObject *Sender);
    void __fastcall ChargerFichierConfiguration1Click(TObject *Sender);
private:	// Déclarations utilisateur
    FILE* pFile;
    Hotel* m_Hotel;
    Bureaux* m_Bureaux;
    TimeStamp* m_TimeStamp;
    Enseignement* m_Enseignement;
    BDD_Exploitation* m_BDD_Exploitation;
    MotorisationVMC* m_MotorisationVMC;
    DonneesExploitationParZone* m_DonneesExploitationParZone[22];
    TFF* FF[22];
    AnsiString fichierConfiguration;
    int localType; // 0 Hotel / 1 Bureaux / 2 Enseignement
    int donneesHygrometrie[22];
    plageDeFonctionnement* m_PlageDeFonctionnement[22];
    float dm1[22];
    float dm2[22];
    int nbrZones;
    char ipAutomate[16];
    char ipServerBDD[16];
    char idServerBDD[16];
    char passwordServerBdd[64];
    unsigned int portAutomate;
    unsigned int portServerBDD;
    char adressePremierMot;
    int nbrCapteursHygrometrie;
    int adressePremierMotAutomate;
    void chargerFichierConfiguration();
    void getDonneesHygrometrie(int pnombreZones);
    void setDonneesExploitationParZones();
    void updateDonnees();
    void calculerDebitExtraction();
    void recupererDonneesDm1();
    void recupererVolumeZones();
    void lireFichierConfigInitialisation();
    void lireFichierConfigHotel();
    void lireFichierConfigBureau();
    void lireFichierConfigEnseignement();
    static unsigned char lireTrameRequete(char* trame);
    static void ecrireTrameReponse(char* buffer, DonneesExploitationParZone* donnees[], TimeStamp* ts, int nbrZones, unsigned char clientIdentifier);
    bool estDansPlageFonctionnement(int zoneId);

public:		// Déclarations utilisateur
    __fastcall TFP(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFP *FP;
//---------------------------------------------------------------------------
#endif
