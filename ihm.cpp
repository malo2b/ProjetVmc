//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include "ihm.h"

TFP *FP;
extern MotorisationVMC hMotorisationVMC;
extern BDD_Exploitation hBDD_Exploitation;
//---------------------------------------------------------------------------
__fastcall TFP::TFP(TComponent* Owner)
    : TForm(Owner)
{
// Definir valeurs par defaut //

lireFichierConfigInitialisation();

ClientSocketHygrometrie->Address = ipAutomate;
ClientSocketHygrometrie->Port = portAutomate;

localType = -1;

nbrCapteursHygrometrie = 8;
adressePremierMot = 21; // De 21 a 28

pFile = NULL;

m_Hotel = NULL;
m_Enseignement = NULL;
m_Bureaux = NULL;

Timer->Enabled = false;

m_MotorisationVMC = &hMotorisationVMC;
m_BDD_Exploitation = &hBDD_Exploitation;
m_TimeStamp = new TimeStamp;



for (int i = 0; i < 22; i++)
    {
    donneesHygrometrie[i] = NULL;
    dm1[i] = NULL;
    dm2[i] = NULL;
    }

m_TimeStamp->setDatas();
ServerSocket->Open();
}
//---------------------------------------------------------------------------
void __fastcall TFP::Quitter1Click(TObject *Sender)
{
FP->Close();
}

//---------------------------------------------------------------------------
void TFP::getDonneesHygrometrie(int pnombreZones)
{
char buffer[12];

*(buffer)=0x00;
*(buffer+1)=0x00;
*(buffer+2)=0x00;
*(buffer+3)=0x00;
*(buffer+4)=0x00;
*(buffer+5)=0x06; // longueur
*(buffer+6)=0x01; // unit identifier
*(buffer+7)=0x03; // Numero Fonction
*(buffer+8)=0x00;
*(buffer+9)=21; 
*(buffer+10)=0x00;

if (nbrZones < 23)
    *(buffer + 11) = pnombreZones;
else
    *(buffer + 11) = 1; 

try
    {
    ClientSocketHygrometrie->Socket->SendBuf(buffer, 12);
    }
catch(...)
    {
    ShowMessage("Erreur Transmission Socket avec l'automate !");
    }
}
//---------------------------------------------------------------------------
void __fastcall TFP::ClientSocketHygrometrieRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
char buffer[25];
int taille = Socket->ReceiveLength();
int cpt = taille + 1 - (nbrZones * 2); 
int cpt2 = 0;
int donnee = 0;

memset(buffer, '\0', 25);

Socket->ReceiveBuf(buffer,taille);

if (*(buffer + 7) == 0x03) // Si code fonction est bon
    {
    while(cpt2 < 8)
        {
        if (cpt < taille)
            {
            donnee = *(buffer + cpt);
            donneesHygrometrie[cpt2] = donnee;
            }
        else
            {
            donneesHygrometrie[cpt2] = NULL;
            }
        cpt2++;
        cpt = cpt + 2;
        }
    }
else
    ShowMessage("Erreur Reception Message !!!");

calculerDebitExtraction();
updateDonnees();
if (ConnexionMoteur->Tag == 1)
    m_MotorisationVMC->reglerVitesseMoteur(dm2[0], dm2[1]);
if (Timer->Tag %60 == 0 && ConnecterBDD->Tag == 1)
    m_BDD_Exploitation->ecrireDonnees(m_DonneesExploitationParZone, nbrZones);
}
//---------------------------------------------------------------------------

void __fastcall TFP::TimerTimer(TObject *Sender)
{
if (ClientSocketHygrometrie->Tag == 1)
    {
    getDonneesHygrometrie(nbrZones);
    Timer->Tag++;
    }
}
//---------------------------------------------------------------------------

void TFP::calculerDebitExtraction() {
int i =0, j=0;
int nbrMaxExtracteurs = 2;
float flag = 0;
    while (i < nbrZones)
    {
    if (donneesHygrometrie[i] >= 50 && estDansPlageFonctionnement(i) && i < nbrMaxExtracteurs)
        {
        flag = donneesHygrometrie[i];
        while (flag > 60)
            {
            flag = flag - 10;
            j++;
            }
        dm2[i] = dm1[i] * ((j * 0.05) + 1);
        }
    else
        dm2[i] = 0;
        
    i++;
    }
}

//---------------------------------------------------------------------------

void TFP::recupererDonneesDm1() {
for (int i = 0; i < nbrZones; i++)
    {
    if (localType == 0)
        dm1[i] = m_Hotel->m_ZoneHotel[i]->getDm1();
    else if (localType == 1)
        dm1[i] = m_Bureaux->m_ZoneBureaux[i]->getDm1();
    else if (localType == 2)
        dm1[i] = m_Enseignement->m_ZoneEnseignement[i]->getDm1();
    }
}

//---------------------------------------------------------------------------

void TFP::setDonneesExploitationParZones()
{
int cpt = 0;
while (cpt < nbrZones)
    {
    if (localType == 0)
        {
        m_DonneesExploitationParZone[cpt]->setVolumeZone(m_Hotel->m_ZoneHotel[cpt]->getVolumeZone());
        m_DonneesExploitationParZone[cpt]->setLocalType(m_Hotel->getLocalType());
        m_DonneesExploitationParZone[cpt]->setZoneId(cpt);
        m_DonneesExploitationParZone[cpt]->setZoneType(m_Hotel->m_ZoneHotel[cpt]->getZoneType());
        m_DonneesExploitationParZone[cpt]->setHygrometrie(donneesHygrometrie[cpt]);
        m_DonneesExploitationParZone[cpt]->setTimeStampId(m_TimeStamp->getTimeStampId());
        m_DonneesExploitationParZone[cpt]->setDebitAirExtrait(dm2[cpt]);
        }
    else if (localType == 1)
        {
        m_DonneesExploitationParZone[cpt]->setVolumeZone(m_Bureaux->m_ZoneBureaux[cpt]->getVolumeZone());
        m_DonneesExploitationParZone[cpt]->setLocalType(m_Bureaux->getLocalType());
        m_DonneesExploitationParZone[cpt]->setZoneId(cpt);
        m_DonneesExploitationParZone[cpt]->setZoneType(m_Bureaux->m_ZoneBureaux[cpt]->getZoneType());
        m_DonneesExploitationParZone[cpt]->setHygrometrie(donneesHygrometrie[cpt]);
        m_DonneesExploitationParZone[cpt]->setTimeStampId(m_TimeStamp->getTimeStampId());
        m_DonneesExploitationParZone[cpt]->setDebitAirExtrait(dm2[cpt]);
        }
    else if (localType == 2)
        {
        m_DonneesExploitationParZone[cpt]->setVolumeZone(m_Enseignement->m_ZoneEnseignement[cpt]->getVolumeZone());
        m_DonneesExploitationParZone[cpt]->setLocalType(m_Enseignement->getLocalType());
        m_DonneesExploitationParZone[cpt]->setZoneId(cpt);
        m_DonneesExploitationParZone[cpt]->setZoneType(m_Enseignement->m_ZoneEnseignement[cpt]->getZoneType());
        m_DonneesExploitationParZone[cpt]->setHygrometrie(donneesHygrometrie[cpt]);
        m_DonneesExploitationParZone[cpt]->setTimeStampId(m_TimeStamp->getTimeStampId());
        m_DonneesExploitationParZone[cpt]->setDebitAirExtrait(dm2[cpt]);
        }
    cpt++;
    }
}

//---------------------------------------------------------------------------

void __fastcall TFP::ServerSocketClientRead(TObject *Sender,TCustomWinSocket *Socket)
{
char bufferRequete[3];
char bufferReponse[735]; // Taille Max de transmission
int lenghtBuffer;
unsigned char clientIdentifier;

memset(bufferRequete, NULL, 3);
memset(bufferReponse, NULL, 735);

lenghtBuffer = Socket->ReceiveLength();
Socket->ReceiveBuf(bufferRequete, lenghtBuffer);

clientIdentifier = lireTrameRequete(bufferRequete);

if(clientIdentifier > 0)
    {
    ecrireTrameReponse(bufferReponse ,m_DonneesExploitationParZone,m_TimeStamp, nbrZones, clientIdentifier);
    try
        {
        Socket->SendBuf(bufferReponse, 3 + sizeof(DonneesExploitationParZone) * nbrZones + sizeof(TimeStamp));
        }
    catch(...)
        {
        ShowMessage("Erreur transmission de données");
        }

    }

}
//---------------------------------------------------------------------------

void __fastcall TFP::FormClose(TObject *Sender, TCloseAction &Action)
{
ServerSocket->Close();

if (ConnecterBDD->Tag == 1)
    m_BDD_Exploitation->deconnexionBDD();

if (ClientSocketHygrometrie->Tag == 1)
    ClientSocketHygrometrie->Close();

if (ConnexionMoteur->Tag == 1)
    {
    m_MotorisationVMC->arreterMoteur();
    m_MotorisationVMC->deconnecterCarte();
    }

if (Tag == 1)
    {
    /*if (localType == 0)
        delete m_Hotel;
    else if (localType == 1)
        delete m_Bureaux;
    else if (localType == 2)
        delete m_Enseignement;  */

    delete [] m_DonneesExploitationParZone;
    delete m_TimeStamp;
    delete [] FF;
    }
}

//---------------------------------------------------------------------------

unsigned char TFP::lireTrameRequete(char* trame){
    unsigned char clientIdentifier;
    int transactionIdentifier = 0;
    int unitIdentifier = 0;
    int numeroFonction = 0;


    transactionIdentifier = *(trame+0);

    if ((transactionIdentifier & 0x0F) == 0) // est pc creation csv
        {
        clientIdentifier = transactionIdentifier & 0xF0;
        }
    else if ((transactionIdentifier & 0xF0) == 0) // est pc supervision
        {
        clientIdentifier = transactionIdentifier & 0x0F;
        }

    unitIdentifier = *(trame + 1);

    numeroFonction = *(trame + 2);

    // Traitement de la requete //
    if (unitIdentifier == 0x01)    // Serveur
        {
        if (numeroFonction == 0x01) // Fonction demande de données
            {
            // Trame conforme //
            return clientIdentifier;
            }
        else
            return 0;
        }
    else
        return 0;
}

//---------------------------------------------------------------------------

void TFP::ecrireTrameReponse(char * buffer, DonneesExploitationParZone* donnees[], TimeStamp* ts,int nbrZones, unsigned char clientIdentifier){

    int i = 0;

    buffer[0] =  clientIdentifier;  // Transaction Identifier contenu dans le client identifier
    buffer[1] =  0x01;   // unit identifier
    buffer[2] =  0x01;   // numero de fonction

    // ecriture de n objets DonneesExploitationParZone de facon sérialisée
    while (i < nbrZones)
        {
        sprintf(buffer + 3 + (sizeof(DonneesExploitationParZone) * i), "%d %d %d %d %f %f %f",
            donnees[i]->getZoneType(),
            donnees[i]->getZoneId(),
            donnees[i]->getLocalType(),
            ts->getTimeStampId(),
            donnees[i]->getVolumeZone(),
            donnees[i]->getHygrometrie(),
            donnees[i]->getDebitAirExtrait()
            );
        i++;
        }

    // Ecriture de l'objet timestamp
    sprintf(buffer + 3 + (sizeof(DonneesExploitationParZone) * i), "%d %d %d %d %d %d",
        ts->getTimeStampId(),
        ts->getMin(),
        ts->getHour(),
        ts->getMday(),
        ts->getMon(),
        ts->getYear()
    );
}

//---------------------------------------------------------------------------

void TFP::lireFichierConfigInitialisation()
        {
        int i = -1;
        int flag = 0;
        char flag2 = 0;
        char buffer[64];
        memset(buffer, '\0', 64);
        FILE* pFile = fopen("config.ini", "r");
        while (i == -1 || buffer[i] != '\n' && i < 64)
            {
            i++;
            buffer[i] = fgetc(pFile);
            }
        if (i >= 64)
            {
            flag = -1;
            }
        buffer[i] = '\0';
        if (AnsiString(buffer) == AnsiString("[Settings]"))
                {
                i = -1;
                memset(buffer, '\0', 64);
                // ip automate
                while (i == -1 || buffer[i] != '=')
                        {
                        i++;
                        buffer[i] = fgetc(pFile);
                        }

                if (AnsiString(buffer) == AnsiString("ipAutomate ="))
                    {
                    i = -1;
                    memset(buffer, '\0', 64);
                    while (i == -1 || (buffer[i] != '\n' && i < 64))
                        {
                        i++;
                        buffer[i] = fgetc(pFile);
                        }
                    buffer[i] = '\0';
                    memcpy(ipAutomate, buffer, strlen(buffer));
                    }
                else
                    {
                    flag = -1;
                    }
                memset(buffer, '\0', 64);

                // port automate
                i = -1;
                while (i == -1 || buffer[i] != '=')
                        {
                        i++;
                        buffer[i] = fgetc(pFile);
                        }
                if (AnsiString(buffer) == AnsiString("portAutomate ="))
                    {
                    i = -1;
                    memset(buffer, '\0', 64);
                    while (i == -1 || buffer[i] != '\n' && i < 64)
                        {
                        i++;
                        buffer[i] = fgetc(pFile);
                        }
                    portAutomate = atoi(buffer);
                    }
                else
                    {
                    flag = -1;
                    }
                    
                // ip server bdd
                i = -1;
                while (i == -1 || buffer[i] != '=')
                        {
                        i++;
                        buffer[i] = fgetc(pFile);
                        }

                if (AnsiString(buffer) == AnsiString("ipServerBDD ="))
                    {
                    i = -1;
                    memset(buffer, '\0', 64);
                    while (i == -1 || (buffer[i] != '\n' && i < 64))
                        {
                        i++;
                        buffer[i] = fgetc(pFile);
                        }
                    buffer[i] = '\0';
                    memcpy(ipServerBDD, buffer,strlen(buffer));
                    }
                else
                    {
                    flag = -1;
                    }
                memset(buffer, '\0', 64);

                // port server bdd
                i = -1;
                while (i == -1 || buffer[i] != '=')
                        {
                        i++;
                        buffer[i] = fgetc(pFile);
                        }
                if (AnsiString(buffer) == AnsiString("portServerBDD ="))
                    {
                    i = -1;
                    memset(buffer, '\0', 64);
                    while (i == -1 || buffer[i] != '\n' && i < 64)
                        {
                        i++;
                        buffer[i] = fgetc(pFile);
                        }
                    portServerBDD = atoi(buffer);
                    }
                else
                    {
                    flag = -1;
                    }

                // id server bdd
                i = -1;
                while (i == -1 || buffer[i] != '=')
                        {
                        i++;
                        buffer[i] = fgetc(pFile);
                        }
                if (AnsiString(buffer) == AnsiString("idServerBDD ="))
                    {
                    i = -1;
                    memset(buffer, '\0', 64);
                    while (i == -1 || buffer[i] != '\n' && i < 64)
                        {
                        i++;
                        buffer[i] = fgetc(pFile);
                        }
                    buffer[i] = '\0';
                    memcpy(idServerBDD, buffer, strlen(buffer));

                    memset(buffer, '\0', 64);
                    }
                else
                    {
                    flag = -1;
                    }

                // password server bdd
                i = -1;
                while (i == -1 || buffer[i] != '=')
                        {
                        i++;
                        buffer[i] = fgetc(pFile);
                        }
                if (AnsiString(buffer) == AnsiString("passwordServerBdd ="))
                    {
                    i = -1;
                    memset(buffer, '\0', 64);
                    while (i == -1 || buffer[i] != '\n' && i < 64)
                        {
                        i++;
                        buffer[i] = fgetc(pFile);
                        }
                    buffer[i] = '\0';
                    strrev(buffer);

                    buffer[strlen(buffer)-2] = '\0';

                    memcpy(passwordServerBdd, buffer, strlen(buffer));
                    }
                else
                    {
                    flag = -1;
                    }

                }
        if (flag == -1)
                {
                ShowMessage("Erreur fichier de configuration config.ini");
                FP->Close();
                }
        }


void __fastcall TFP::ChargerFichierConfiguration1Click(TObject *Sender)
{
bool fichierSelectionne = OpenDialog->Execute();
if (fichierSelectionne)
    {
    fichierConfiguration = OpenDialog->FileName;
    chargerFichierConfiguration();
    }
}


//---------------------------------------------------------------------------


void __fastcall TFP::Cascade1Click(TObject *Sender)
{
Cascade();
for (int i = 0; i < nbrZones; i++)
    FF[i]->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFP::Mosaique1Click(TObject *Sender)
{
Tile();
for (int i = 0; i < nbrZones; i++)
    FF[i]->Enabled = false;    
}
//---------------------------------------------------------------------------


void __fastcall TFP::FormResize(TObject *Sender)
{
if (Tag == 1) // Fichier config chargé
    {
    Tile();
    }
}
//---------------------------------------------------------------------------

void TFP::chargerFichierConfiguration()
{
if (Tag == 0)
    {
    int i;
    localType = -1;
    unsigned char heureFonctionnement[4];
    int flag;
    bool jourDeFonctionnement[7];

    pFile = fopen(fichierConfiguration.c_str(),"rb");
    if (pFile != NULL)
        {
        fseek(pFile, 100, SEEK_SET); // Deplacer le pointeur apres les 100 octets rfid
        fread(&localType,sizeof(int),1,pFile);

        if (localType == 0)
            lireFichierConfigHotel();
        else if (localType == 1)
            lireFichierConfigBureau();
        else if (localType == 2)
            lireFichierConfigEnseignement();

        }
    else
        {
        ShowMessage("Erreur fichier de configuration");
        FP->Close();
        }

    i = nbrZones - 1;

    while (i >= 0)
        {
        heureFonctionnement[0] = m_PlageDeFonctionnement[i]->getTmHeureDebut();
        heureFonctionnement[1] = m_PlageDeFonctionnement[i]->getTmMinuteDebut();
        heureFonctionnement[2] = m_PlageDeFonctionnement[i]->getTmHeureFin();
        heureFonctionnement[3] = m_PlageDeFonctionnement[i]->getTmMinuteFin();

        for (int j = 0; j < 7; j++)
            jourDeFonctionnement[j] = m_PlageDeFonctionnement[i]->estJourDeFonctionnement(j);
        if (localType == 0)
            FF[i] = new TFF(this, i, m_Hotel->m_ZoneHotel[i]->getVolumeZone(),m_Hotel->m_ZoneHotel[i]->getZoneType(), heureFonctionnement, jourDeFonctionnement);
        else if (localType == 1)
            FF[i] = new TFF(this, i, m_Bureaux->m_ZoneBureaux[i]->getVolumeZone(),m_Bureaux->m_ZoneBureaux[i]->getZoneType(), heureFonctionnement, jourDeFonctionnement);
        else if (localType == 2)
            FF[i] = new TFF(this, i, m_Enseignement->m_ZoneEnseignement[i]->getVolumeZone(),m_Enseignement->m_ZoneEnseignement[i]->getZoneType(), heureFonctionnement, jourDeFonctionnement);

        i--;
        }

    recupererDonneesDm1();

    Affichage1->Enabled = true;
    Tile();

    for (int i = 0; i < nbrZones; i++)
        {
        FF[i]->Caption = "Zone n°" + AnsiString(i);
        m_DonneesExploitationParZone[i] = new DonneesExploitationParZone;
        }

    if (ClientSocketHygrometrie->Tag == 1)
        {
        Timer->Enabled = true;
        getDonneesHygrometrie(nbrZones);
        }
    }
    Tag = 1;
}

void TFP::lireFichierConfigHotel()
{
ZoneHotel* hZoneHotel[21];
ChambreFumeur* hChFumeur[10];
ChambreNonFumeur* hChNonFumeur[10];
SalleRestauration* hSalleRestauration;

m_Hotel = new Hotel;

int compteurCF=0,compteurCNF=0;
int zoneType;

memset(hChFumeur, NULL, 10);
memset(hChNonFumeur, NULL, 10);
memset(hZoneHotel, NULL, 21);
memset(m_PlageDeFonctionnement, NULL, 21);
hSalleRestauration = NULL;

if (pFile != NULL)
    {
    fread(m_Hotel,sizeof(Hotel),1,pFile);

    nbrZones = m_Hotel->getNombreZones();

    for(int compteur=0;compteur<nbrZones;compteur++)
        {
        hZoneHotel[compteur] = new ZoneHotel;
        fread(hZoneHotel[compteur],sizeof(ZoneHotel),1,pFile);
        }

    for(int compteur=0;compteur<nbrZones;compteur++)
        {
        m_PlageDeFonctionnement[compteur] = new plageDeFonctionnement;
        fread(m_PlageDeFonctionnement[compteur],sizeof(plageDeFonctionnement),1,pFile);
        }

    for (int compteur = 0; compteur < m_Hotel->getNbrChambreFumeur(); compteur++)
        hChFumeur[compteur] = new ChambreFumeur;

    for (int compteur = 0; compteur < m_Hotel->getNbrChambreNonFumeur(); compteur++)
        hChNonFumeur[compteur] = new ChambreNonFumeur;


    for(int compteur=0;compteur<m_Hotel->getNbrChambreNonFumeur();compteur++)
        fread(hChNonFumeur[compteur],sizeof(ChambreNonFumeur),1,pFile);

    for(int compteur=0;compteur<m_Hotel->getNbrChambreFumeur();compteur++)
        fread(hChFumeur[compteur],sizeof(ChambreFumeur),1,pFile);

    if (m_Hotel->getNbrSalleRestauration() == 1)
        {
        hSalleRestauration = new SalleRestauration;
        fread(hSalleRestauration,sizeof(SalleRestauration),1,pFile);
        }

    fclose(pFile);

    for(int compteur=0;compteur<nbrZones;compteur++)
        {
        m_Hotel->m_ZoneHotel[compteur] = hZoneHotel[compteur];

        zoneType = m_Hotel->m_ZoneHotel[compteur]->getZoneType();


        if(zoneType ==0 )
            {
            for(int compteur2 = 0;compteur2<m_Hotel->m_ZoneHotel[compteur]->getNbrPiece();compteur2++)
                {
                m_Hotel->m_ZoneHotel[compteur]->m_ChambreNonFumeur[compteur2] = hChNonFumeur[compteurCNF];
                compteurCNF++;
                }
            }
        else if(zoneType ==1)
            {
            for(int compteur2 = 0;compteur2<m_Hotel->m_ZoneHotel[compteur]->getNbrPiece();compteur2++)
                {
                m_Hotel->m_ZoneHotel[compteur]->m_ChambreFumeur[compteur2] = hChFumeur[compteurCF];
                compteurCF++;
                }
            }
        else if(zoneType == 2)
            {
            m_Hotel->m_ZoneHotel[compteur]->m_SalleRestauration = hSalleRestauration;
            }
        }
    }
else
    {
    ShowMessage("Erreur fichier de configuration");
    FP->Close();
    }
}

//---------------------------------------------------------------------------

void TFP::lireFichierConfigBureau()
{
ZoneBureaux* hZoneBureaux[11];
Bureau* hBureau[10];
SanitaireCollectifs* hSanitaireCollectifs;

m_Bureaux = new Bureaux;

int zoneType;
int compteurBureau = 0;

memset(hBureau, NULL, 10);
memset(hZoneBureaux, NULL, 11);
memset(m_PlageDeFonctionnement, NULL, 21);

if (pFile != NULL)
    {
    fread(m_Bureaux,sizeof(Bureaux),1,pFile);

    nbrZones = m_Bureaux->getNombreZones();

    for(int compteur=0;compteur<nbrZones;compteur++)
        {
        hZoneBureaux[compteur] = new ZoneBureaux;
        fread(hZoneBureaux[compteur],sizeof(ZoneBureaux),1,pFile);
        }

    for(int compteur=0;compteur<nbrZones;compteur++)
        {
        m_PlageDeFonctionnement[compteur] = new plageDeFonctionnement;
        fread(m_PlageDeFonctionnement[compteur],sizeof(plageDeFonctionnement),1,pFile);
        }

    for (int compteur = 0; compteur < m_Bureaux->getNbrBureau(); compteur++)
        hBureau[compteur] = new Bureau;


    fclose(pFile);

    for(int compteur=0;compteur<nbrZones;compteur++)
        {
        m_Bureaux->m_ZoneBureaux[compteur] = hZoneBureaux[compteur];

        zoneType = m_Bureaux->m_ZoneBureaux[compteur]->getZoneType();

        if(zoneType ==3)
            {
            for(int compteur2 = 0;compteur2<m_Bureaux->m_ZoneBureaux[compteur]->getNbrPiece();compteur2++)
                {
                m_Bureaux->m_ZoneBureaux[compteur]->m_Bureau[compteur2] = hBureau[compteurBureau];
                compteurBureau++;
                }
            }
        else if(zoneType ==4)
            {
            m_Bureaux->m_ZoneBureaux[compteur]->m_SanitaireCollectifs = hSanitaireCollectifs;
            }
        }
    }
else
    {
    ShowMessage("Erreur fichier de configuration");
    FP->Close();
    }
}

//---------------------------------------------------------------------------

void TFP::lireFichierConfigEnseignement()
{
ZoneEnseignement* hZoneEnseignement[22];
SalleHebergement* hSalleHebergement;
SalleEnseignementMPC* hSalleEnseignementMPC[10];
SalleEnseignementLycee* hSalleEnseignementLycee[10];
SalleRestauration* hSalleRestauration;

m_Enseignement = new Enseignement;

int zoneType;

memset(hZoneEnseignement, NULL, 22);
hSalleHebergement = NULL;
memset(hSalleEnseignementMPC, NULL, 10);
memset(hSalleEnseignementLycee, NULL, 10);

if (pFile != NULL)
    {
    fread(m_Enseignement,sizeof(Enseignement),1,pFile);

    nbrZones = m_Enseignement->getNombreZones();

    for(int compteur=0;compteur<nbrZones;compteur++)
        {
        hZoneEnseignement[compteur] = new ZoneEnseignement;
        fread(hZoneEnseignement[compteur],sizeof(ZoneEnseignement),1,pFile);
        }

    for(int compteur=0;compteur<nbrZones;compteur++)
        {
        m_PlageDeFonctionnement[compteur] = new plageDeFonctionnement;
        fread(m_PlageDeFonctionnement[compteur],sizeof(plageDeFonctionnement),1,pFile);
        }

    for (int compteur = 0; compteur < m_Enseignement->getNbrSalleEnseignementMPC(); compteur++)
        {
        hSalleEnseignementMPC[compteur] = new SalleEnseignementMPC;
        fread(hSalleEnseignementMPC[compteur],sizeof(SalleEnseignementMPC),1,pFile);
        }
    for (int compteur = 0; compteur < m_Enseignement->getNbrSalleEnseignementLycee(); compteur++)
        {
        hSalleEnseignementLycee[compteur] = new SalleEnseignementLycee;
        fread(hSalleEnseignementLycee[compteur],sizeof(SalleEnseignementLycee),1,pFile);
        }

    if (m_Enseignement->getNbrSalleRestauration() == 1)
        {
        hSalleRestauration = new SalleRestauration;
        fread(hSalleRestauration,sizeof(SalleRestauration),1,pFile);
        }

    if (m_Enseignement->getNbrSalleHebergement() == 1)
        {
        hSalleHebergement = new SalleHebergement;
        fread(hSalleHebergement,sizeof(SalleHebergement),1,pFile);
        }

    fclose(pFile);

    for(int compteur=0;compteur<nbrZones;compteur++)
        {
        m_Enseignement->m_ZoneEnseignement[compteur] = hZoneEnseignement[compteur];

        zoneType = m_Enseignement->m_ZoneEnseignement[compteur]->getZoneType();

        if(zoneType == 5)      // MPC
            {
            for(int compteurEnseignement = 0;compteurEnseignement<m_Enseignement->m_ZoneEnseignement[compteur]->getNbrPiece();compteurEnseignement++)
                {
                m_Enseignement->m_ZoneEnseignement[compteur]->m_SalleEnseignementMPC[compteurEnseignement] = hSalleEnseignementMPC[compteurEnseignement];
                compteurEnseignement++;
                }
            }

        else if(zoneType == 6) // Lycee
            {
            for(int compteurEnseignement = 0;compteurEnseignement<m_Enseignement->m_ZoneEnseignement[compteur]->getNbrPiece();compteurEnseignement++)
                {
                m_Enseignement->m_ZoneEnseignement[compteur]->m_SalleEnseignementLycee[compteurEnseignement] = hSalleEnseignementLycee[compteurEnseignement];
                compteurEnseignement++;
                }
            }
            
        else if (zoneType == 7) // Hebergement
            m_Enseignement->m_ZoneEnseignement[compteur]->m_SalleHebergement = hSalleHebergement;

        else if (zoneType == 2) // Restauration
            m_Enseignement->m_ZoneEnseignement[compteur]->m_SalleRestauration = hSalleRestauration;

        }
    }
else
    {
    ShowMessage("Erreur fichier de configuration");
    FP->Close();
    }
}

//---------------------------------------------------------------------------

void __fastcall TFP::ClientSocketHygrometrieConnect(TObject *Sender, TCustomWinSocket *Socket)
    {
    ClientSocketHygrometrie->Tag = 1;
    if (Tag == 1)
        Timer->Enabled = true;

    ConnectAutomate->Caption = "Deconnexion Automate";
    }
//---------------------------------------------------------------------------

void __fastcall TFP::ClientSocketHygrometrieDisconnect(TObject *Sender, TCustomWinSocket *Socket)
    {
    ClientSocketHygrometrie->Tag = 0;
    Timer->Enabled = false;
    ConnectAutomate->Caption = "Activer Connexion Automate";
    }
//---------------------------------------------------------------------------

void __fastcall TFP::ConnectAutomateClick(TObject *Sender)
    {
    if (ClientSocketHygrometrie->Tag == 0)
        {
        ClientSocketHygrometrie->Open();
        }
    else if (ClientSocketHygrometrie->Tag == 1)
        {
        ClientSocketHygrometrie->Close();
        }
    }
//---------------------------------------------------------------------------

void TFP::updateDonnees()
    {
    m_TimeStamp->setDatas();
    setDonneesExploitationParZones();
    for (int i = 0; i < nbrZones; i++)
        {
        FF[i]->setData(donneesHygrometrie[i] , dm2[i]);
        }

    if (dm2[0] > 0)
        {
        FF[0]->setMotorisationEtat(true);
        }
    else
        {
        FF[0]->setMotorisationEtat(false);
        }

    if (dm2[1] > 0)
        {
        FF[1]->setMotorisationEtat(true);
        }
    else
        {
        FF[1]->setMotorisationEtat(false);
        }

    }

//---------------------------------------------------------------------------

void __fastcall TFP::ClientSocketHygrometrieError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
    {
    Timer->Enabled = false;
    ClientSocketHygrometrie->Close();
    ShowMessage("Erreur Client Socket Hygrometrie, Code d'erreur : " + ErrorCode);
    }
//---------------------------------------------------------------------------

void __fastcall TFP::ConnecterBDDClick(TObject *Sender)
{
if (ConnecterBDD->Tag == 0)
    {
    int flag = m_BDD_Exploitation->connexionBDD(ipServerBDD, idServerBDD, passwordServerBdd, portServerBDD);
    if (flag != 1)
        {
        ShowMessage("Erreur Connexion BDD");
        ConnecterBDD->Tag = 0;
        }
    else
        {
        ConnecterBDD->Tag = 1;
        ConnecterBDD->Caption = "Deconnecter BDD";
        }
    }
else if (ConnecterBDD->Tag == 1)
    {
    m_BDD_Exploitation->deconnexionBDD();
    ConnecterBDD->Tag = 0;
    ConnecterBDD->Caption = "Connecter BDD";
    }
}
//---------------------------------------------------------------------------


void __fastcall TFP::ConnexionMoteurClick(TObject *Sender)
{
int flag;
if (ConnexionMoteur->Tag == 0)
        {
        flag = m_MotorisationVMC->connecterCarte();
        if (flag == m_MotorisationVMC->getAdresseCarte())
                {
                ConnexionMoteur->Tag = 1;
                ConnexionMoteur->Caption = "Deconnecter Moteurs";
                }
        else
                ShowMessage("Erreur connexion avec la motorisation");
        }
else if (ConnexionMoteur->Tag == 1)
        {
        m_MotorisationVMC->arreterMoteur();
        m_MotorisationVMC->deconnecterCarte();
        ConnexionMoteur->Tag = 0;
        ConnexionMoteur->Caption = "Connecter Moteurs";
        }


}
//---------------------------------------------------------------------------

bool TFP::estDansPlageFonctionnement(int zoneId)
    {
    TimeStamp hTimeStamp;
    int sommeMinutesDebut;
    int sommeMinutesFin;

    hTimeStamp.setDatas();

    sommeMinutesDebut = m_PlageDeFonctionnement[zoneId]->getTmMinuteDebut() + (m_PlageDeFonctionnement[zoneId]->getTmHeureDebut() * 60);
    sommeMinutesFin = m_PlageDeFonctionnement[zoneId]->getTmMinuteFin() + (m_PlageDeFonctionnement[zoneId]->getTmHeureFin() * 60);

    if (m_PlageDeFonctionnement[zoneId]->testJourDeFonctionnement(hTimeStamp.getWday()))
        {
        if (sommeMinutesDebut <= (hTimeStamp.getMin() + hTimeStamp.getHour() * 60))
            {
            if (sommeMinutesFin >= (hTimeStamp.getMin() + hTimeStamp.getHour() * 60))
                {
                return true;
                }
            else
                return false;
            }
        else
            return false;
        }
    else
        return false;
    }


//---------------------------------------------------------------------------


