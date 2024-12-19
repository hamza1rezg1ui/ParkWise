#include "services.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>



///////////////****************fonction AJOUTER **************************
int ajouter(const char *filename, Service nouveauService) {
    // Ouvrir le fichier services.txt en mode append
    FILE *fichier = fopen(filename, "a");
    if (fichier == NULL) {
        return 0;  // Erreur d'ouverture du fichier
    }

    // Enregistrer les informations du service dans services.txt
    fprintf(fichier, "%d %s %s %f\n", nouveauService.id, nouveauService.nom, nouveauService.description, nouveauService.tarif);

    // Ouvrir le fichier ID.txt en mode append pour ajouter l'ID
    FILE *idFile = fopen("ID.txt", "a");
    if (idFile == NULL) {
        fclose(fichier);
        return 0;  // Erreur d'ouverture du fichier ID.txt
    }

    // Ajouter l'ID dans ID.txt
    fprintf(idFile, "%d\n", nouveauService.id);
    
    fclose(fichier);
    fclose(idFile);
    return 1;  // Succès
}

//********************Fonction Modifier*******************************



int modifier(char *filename, int id, Service nouv) {
    int tr = 0;
    Service s;
    
    // Ouvrir le fichier pour lecture et un fichier temporaire pour écriture
    FILE *f = fopen(filename, "r");
    FILE *f2 = fopen("temp.txt", "w");
    
    if (f == NULL) {
        perror("Erreur d'ouverture du fichier source");
        return 0;
    }
    
    if (f2 == NULL) {
        perror("Erreur d'ouverture du fichier temporaire");
        fclose(f);
        return 0;
    }
    
    // Parcours du fichier et modification du service si trouvé
    while (fscanf(f, "%d %99s %199s %f\n", &s.id, s.nom, s.description, &s.tarif) != EOF) {
        if (s.id == id) {
            // Modification du service
            fprintf(f2, "%d %s %s %.2f\n", nouv.id, nouv.nom, nouv.description, nouv.tarif);
            tr = 1;
        } else {
            // Copie du service tel quel si non modifié
            fprintf(f2, "%d %s %s %.2f\n", s.id, s.nom, s.description, s.tarif);
        }
    }
    
    // Fermeture des fichiers
    fclose(f);
    fclose(f2);
    
    // Remplacer l'ancien fichier par le fichier temporaire
    if (tr) {
        if (remove(filename) != 0) {
            perror("Erreur lors de la suppression du fichier original");
            return 0;
        }

        if (rename("temp.txt", filename) != 0) {
            perror("Erreur lors du renommage du fichier temporaire");
            return 0;
        }

        printf("Service modifié avec succès.\n");
    } else {
        // Supprimer le fichier temporaire si aucune modification n'a eu lieu
        remove("temp.txt");
        printf("Service avec l'ID %d non trouvé.\n", id);
    }

    return tr;
}

////////////////////////*****************fonction SUPPRIMER***************////////////////////////////////////////////////////////////////////
int supprimer(char *filename, int id) {
    int tr = 0;
    Service s;
    
    // Ouverture du fichier services.txt pour lecture
    FILE *f = fopen(filename, "r");
    FILE *f2 = fopen("temp.txt", "w");

    if (f == NULL) {
        perror("Erreur d'ouverture du fichier services.txt");
        return 0;
    }

    if (f2 == NULL) {
        perror("Erreur d'ouverture du fichier temporaire");
        fclose(f);
        return 0;
    }

    // Parcours du fichier services.txt pour supprimer le service
    while (fscanf(f, "%d %s %s %f\n", &s.id, s.nom, s.description, &s.tarif) != EOF) {
        if (s.id == id) {
            tr = 1;  // Service trouvé et marqué pour suppression
        } else {
            // Écriture des services restants dans temp.txt
            fprintf(f2, "%d %s %s %.2f\n", s.id, s.nom, s.description, s.tarif);
        }
    }

    fclose(f);
    fclose(f2);

    // Suppression du fichier services.txt et remplacement par temp.txt
    if (tr) {
        if (remove(filename) != 0) {
            perror("Erreur lors de la suppression du fichier services.txt");
            return 0;
        }

        if (rename("temp.txt", filename) != 0) {
            perror("Erreur lors du renommage du fichier temporaire");
            return 0;
        }

        // Suppression de l'ID dans ID.txt
        FILE *idFile = fopen("ID.txt", "r");
        FILE *tempIDFile = fopen("temp_id.txt", "w");

        if (idFile == NULL || tempIDFile == NULL) {
            perror("Erreur d'ouverture du fichier ID.txt");
            return 0;
        }

        int currentID;
        int foundID = 0;
        while (fscanf(idFile, "%d\n", &currentID) != EOF) {
            if (currentID == id) {
                foundID = 1;  // ID trouvé, ne pas l'écrire dans temp_id.txt
            } else {
                fprintf(tempIDFile, "%d\n", currentID);
            }
        }

        fclose(idFile);
        fclose(tempIDFile);

        // Remplacer ID.txt par le fichier sans l'ID supprimé
        if (foundID) {
            if (remove("ID.txt") != 0) {
                perror("Erreur lors de la suppression de ID.txt");
                return 0;
            }

            if (rename("temp_id.txt", "ID.txt") != 0) {
                perror("Erreur lors du renommage du fichier temporaire ID.txt");
                return 0;
            }

            g_print("Service supprimé avec succès et ID supprimé de ID.txt.\n");
        } else {
            // Supprimer le fichier temporaire si l'ID n'a pas été trouvé
            remove("temp_id.txt");
            g_print("Service avec l'ID %d non trouvé dans ID.txt.\n", id);
        }
    } else {
        // Supprimer le fichier temporaire si aucune suppression n'a eu lieu
        remove("temp.txt");
        g_print("Service avec l'ID %d non trouvé dans services.txt.\n", id);
    }

    return tr;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
Service chercher(char *filename, int id) {
    Service s;
    s.id = -1;
    FILE *f = fopen(filename, "r");
    if (f != NULL) {
        while (fscanf(f, "%d %s %s %f\n", &s.id, s.nom,s.description,&s.tarif) != EOF) {
            if (s.id == id) {
                fclose(f);
                return s;
            }
        }
    }
    fclose(f);
    s.id = -1; // non trouvé
    return s;
}
void afficherTous(char *filename) {
    Service s;
    FILE *f = fopen(filename, "r");
    if (f != NULL) {
        printf("Liste des services :\n");
        while (fscanf(f, "%d %s %s %f\n", &s.id, s.nom,s.description,&s.tarif) != EOF) {
            printf("ID: %d, Nom: %s, Description: %s, Tarif: %.2f\n",s.id, s.nom, s.description, s.tarif);
        }
        fclose(f);
    } else {
        printf("Erreur lors de l'ouverture du fichier.\n");
    }
}
int affecterServiceReservation(char *filenameService, char*filenameReservation, int idService, int idReservation){
   Service s =chercher(filenameService, idService);
   if (s.id==-1){
      printf("Service introuvable.\n");
      return 0;
   }
   FILE *f= fopen(filenameReservation, "a");
   if (f != NULL){
      fprintf(f,"ID Réservation: %d, ID Service: %d, Nom Service : %s,Tarif: %2f\n", idReservation, s.id, s.nom, s.tarif);
      fclose(f);
      printf(" Le service a été affecté à la réservation avec succé.\n");
      return 1;
   }else {
      printf("Erreur lors de l'ouverture du fichier réservation.\n");
      return 0;
   }
}
/////////////////////////////////************************TREEVIEW******************************************/////////////




#include "services.h"  // Inclure le fichier d'en-tête de services
#include <gtk/gtk.h>

enum {
    ID,
    NOM,
    DESCRIPTION,
    TARIF,
    COLUMNS
};

void afficher_services(GtkWidget *treeview) {
    GtkListStore *store;
    GtkTreeIter iter;
    FILE *f;
    Service service;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    // Créer un store pour les services
    store = gtk_list_store_new(COLUMNS, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_FLOAT);

    // Ajouter les colonnes au TreeView
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", ID, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", NOM, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Description", renderer, "text", DESCRIPTION, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Tarif", renderer, "text", TARIF, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Ouvrir le fichier contenant les services
    f = fopen("services.txt", "r");
    if (f == NULL) {
        g_warning("Erreur lors de l'ouverture du fichier services.txt");
        return;
    }

    // Lire chaque ligne et ajouter un service au store
    while (fscanf(f, "%d %s %s %f\n", &service.id, service.nom, service.description, &service.tarif) != EOF) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, ID, service.id, NOM, service.nom, DESCRIPTION, service.description, TARIF, service.tarif, -1);
    }

    fclose(f);

    // Assigner le modèle de données au TreeView
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);
}
//////////////////////////////////////////////////////////////////////////////////////
void vider(GtkTreeView *treeview) {
    GtkListStore *store;

    // On récupère le modèle du TreeView
    store = GTK_LIST_STORE(gtk_tree_view_get_model(treeview));

    // Si le modèle existe, on le vide
    if (store != NULL) {
        gtk_list_store_clear(store); // Vider le store
    } else {
        // Si aucun modèle n'est attaché, on crée un modèle par défaut
        GtkCellRenderer *renderer;
        GtkTreeViewColumn *column;

        // Créer les colonnes pour afficher les informations des services
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 0, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Description", renderer, "text", 1, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Tarif", renderer, "text", 2, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        // Créer un store pour les services : id, nom, description, tarif
        store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_FLOAT);
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    }
}

