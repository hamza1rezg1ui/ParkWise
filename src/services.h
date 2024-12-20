#ifndef SERVICES_H_INCLUDED
#define SERVICES_H_INCLUDED
#include <gtk/gtk.h>
#include <stdio.h>


#include <string.h>

#define MAX_NAME_LENGTH 100
#define MAX_DESC_LENGTH 200

// Structure Service
typedef struct {
    int id;
    char nom[MAX_NAME_LENGTH];
    char description[MAX_DESC_LENGTH];
    float tarif;
} Service;

typedef struct {
   int idReservation;
   int idService;
}Reservation;

int ajouter(const char *filename, Service nouveauService);
int modifier(char *filename, int id, Service nouv);
int supprimer(char *filename, int id);
Service chercher(char *filename, int id);
void afficherTous(char *filename);
int affecterServiceReservation(char *filenameService,char*filenameReservation, int idService, int idReservation);
// Déclarations des fonctions
void afficher_services(GtkWidget *treeview);
void vider(GtkTreeView *treeview); 
#endif // SERVICES_H_INCLUDED
