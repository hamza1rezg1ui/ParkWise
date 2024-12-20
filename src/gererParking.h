#ifndef GERERPARKING_H_INCLUDED
#define GERERPARKING_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <gtk/gtk.h>

typedef struct {
  char id[50];
  char nom[50];
  char adresse[50];
  int capacite;
  int placeDisponible;
  float tarif;
  char Type_tarifaire[50]; //  (jounalier ou horaire )
  char securite_disponible[50];

}Parking;

int ajouterParking(char *filename,Parking parking);
int modifierParking(char *filename,char *id,Parking nouv);
int supprimerParking(char *filename,char *id);
Parking chercherParkingById(char *filename,char *id);
int isExistParkingById(char *filename,char *id);
int listerParking(char *filename, Parking *resultat);
void afficherParking(GtkWidget *liste, char *filename);
void viderParking(GtkWidget *liste);
void afficherParkingByID(GtkWidget *liste, char *filename,char *nom);
void afficherParkingByAdresse(GtkWidget *liste, char *filename,char *nom);
#endif // GERERPARKING_H_INCLUDED
