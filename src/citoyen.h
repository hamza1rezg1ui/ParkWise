#ifndef CITOYEN_H_INCLUDED
#define CITOYEN_H_INCLUDED
#include <gtk/gtk.h>
#include <stdio.h>


#include <string.h>

#define MAX_NAME_LENGTH 100
#define MAX_PREN_LENGTH 200
#define MAX_ADRESS_LENGTH 50

// Structure Service
typedef struct {
    int id;
    char nom[100];
    char prenom[100];
    char adress[200];
    float age;
    char sexe[10];
} Citoyen;


typedef struct {
    int idf;
    int njour;
    float montant;
} Facture;


int ajouterCitoyen(const char *filename, Citoyen nouveauService);
int modifierCitoyen(char *filename, int id, Citoyen nouv);
int supprimerCitoyen(char *filename, int id);
Citoyen chercherCitoyen(char *filename, int id);
void afficherTousCitoyen(char *filename);

// Déclarations des fonctions
void afficher_Facture(GtkWidget *treeview);   
void viderH(GtkTreeView *treeview); 
#endif // CITOYEN_H_INCLUDED
