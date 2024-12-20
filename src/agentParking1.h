#ifndef AGENTPARKING1_H_INCLUDED
#define AGENTPARKING1_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    char CIN[20];
    char nom[100];
    char prenom[100];
    char email[100];
    char sexe[50];
    char etatCivil[100];
    char ID_Parking[100];
    char num_tel[20];
    char horaires_travail[100];
} agent1;
int ajouterAgent1(char *filename,agent1 agent);
int affecterAagent1(char *filename,char *idAgent,char *idParking);
#endif // AGENTPARKING1_H_INCLUDED
