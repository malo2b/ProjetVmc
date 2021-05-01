///////////////////////////////////////////////////////////
//
//  SalleRestauration.cpp
//  Implementation of the Class SalleRestauration
//  Generated by Enterprise Architect
//  Created on:      11-Jan-2021 13:29:33
//  Original author: 
//  
///////////////////////////////////////////////////////////
//  Modification history:
//  
//
///////////////////////////////////////////////////////////

#include "SalleRestauration.h"

SalleRestauration::SalleRestauration(){

    volume = 0;
    zoneId = -1;
    idPiece = -1;
}

SalleRestauration::SalleRestauration(float volumeParam,int zoneIdParam,int idPieceParam){
    volume = volumeParam;
    zoneId = zoneIdParam;
    idPiece = idPieceParam;
}

SalleRestauration::SalleRestauration(float volumeParam,int pieceId)
{
   volume = volumeParam;
   idPiece = pieceId;
}


SalleRestauration::~SalleRestauration(){

}


