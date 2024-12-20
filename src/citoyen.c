#include "citoyen.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Function to add a new citizen
int ajouterCitoyen(const char *filename, Citoyen nouveauCitoyen) {
    FILE *fichier = fopen(filename, "a");
    if (fichier == NULL) {
        return 0;  // Error opening the file
    }

    // Correct the format specifiers for the data types
    fprintf(fichier, "%d %s %s %s %.2f %s \n", nouveauCitoyen.id, nouveauCitoyen.nom, nouveauCitoyen.prenom, nouveauCitoyen.adress, nouveauCitoyen.age, nouveauCitoyen.sexe);

    // Open the ID file to append the ID
    FILE *idFile = fopen("IDC.txt", "a");
    if (idFile == NULL) {
        fclose(fichier);
        return 0;  // Error opening the ID file
    }

    fprintf(idFile, "%d\n", nouveauCitoyen.id);

    fclose(fichier);
    fclose(idFile);
    return 1;  // Success
}

// Function to modify an existing citizen
int modifierCitoyen(char *filename, int id, Citoyen nouv) {
    int tr = 0;
    Citoyen c;

    FILE *f = fopen(filename, "r");
    FILE *f2 = fopen("temp.txt", "w");

    if (f == NULL || f2 == NULL) {
        perror("Error opening file");
        return 0;
    }

    while (fscanf(f, "%d %s %s %s %f %s \n", &c.id, c.nom, c.prenom, c.adress, &c.age, c.sexe) != EOF) {
        if (c.id == id) {
            fprintf(f2, "%d %s %s %s %.2f %s \n", nouv.id, nouv.nom, nouv.prenom, nouv.adress, nouv.age, nouv.sexe);
            tr = 1;
        } else {
            fprintf(f2, "%d %s %s %s %.2f %s \n", c.id, c.nom, c.prenom, c.adress, c.age, c.sexe);
        }
    }

    fclose(f);
    fclose(f2);

    if (tr) {
        if (remove(filename) != 0 || rename("temp.txt", filename) != 0) {
            perror("Error during file replacement");
            return 0;
        }
        printf("Citizen modified successfully.\n");
    } else {
        remove("temp.txt");
        printf("Citizen with ID %d not found.\n", id);
    }

    return tr;
}

// Function to delete an existing citizen
int supprimerCitoyen(char *filename, int id) {
    int tr = 0;
    Citoyen c;

    FILE *f = fopen(filename, "r");
    FILE *f2 = fopen("temp.txt", "w");

    if (f == NULL || f2 == NULL) {
        perror("Error opening file");
        return 0;
    }

    while (fscanf(f, "%d %s %s %s %f %s \n", &c.id, c.nom, c.prenom, c.adress, &c.age, c.sexe) != EOF) {
        if (c.id == id) {
            tr = 1;
        } else {
            fprintf(f2, "%d %s %s %s %.2f %s\n", c.id, c.nom, c.prenom, c.adress, c.age, c.sexe);
        }
    }

    fclose(f);
    fclose(f2);

    if (tr) {
        if (remove(filename) != 0 || rename("temp.txt", filename) != 0) {
            perror("Error during file replacement");
            return 0;
        }

        // Removing the ID from IDC.txt
        FILE *idFile = fopen("IDC.txt", "r");
        FILE *tempIDFile = fopen("temp_idc.txt", "w");

        if (idFile == NULL || tempIDFile == NULL) {
            perror("Error opening ID file");
            return 0;
        }

        int currentID;
        int foundID = 0;
        while (fscanf(idFile, "%d\n", &currentID) != EOF) {
            if (currentID == id) {
                foundID = 1;
            } else {
                fprintf(tempIDFile, "%d\n", currentID);
            }
        }

        fclose(idFile);
        fclose(tempIDFile);

        if (foundID) {
            if (remove("IDC.txt") != 0 || rename("temp_idc.txt", "IDC.txt") != 0) {
                perror("Error replacing ID file");
                return 0;
            }
            printf("Citizen deleted successfully and ID removed from IDC.txt.\n");
        } else {
            remove("temp_idc.txt");
            printf("Citizen with ID %d not found in IDC.txt.\n", id);
        }
    } else {
        remove("temp.txt");
        printf("Citizen with ID %d not found in %s.\n", id, filename);
    }

    return tr;
}

// Function to search for a citizen by ID
Citoyen chercherCitoyen(char *filename, int id) {
    Citoyen c;
    c.id = -1;
    FILE *f = fopen(filename, "r");

    if (f != NULL) {
        while (fscanf(f, "%d %s %s %s %f %s \n", &c.id, c.nom, c.prenom, c.adress, &c.age, c.sexe) != EOF) {
            if (c.id == id) {
                fclose(f);
                return c;
            }
        }
    }

    fclose(f);
    c.id = -1;  // Not found
    return c;
}

// Function to display all citizens
void afficherTousCitoyen(char *filename) {
    Citoyen c;
    FILE *f = fopen(filename, "r");
    if (f != NULL) {
        printf("List of Citizens:\n");
        while (fscanf(f, "%d %s %s %s %f %s \n", &c.id, c.nom, c.prenom, c.adress, &c.age, c.sexe) != EOF) {
            printf("ID: %d, Name: %s, Surname: %s, Address: %s, Age: %.2f, Gender: %s \n", c.id, c.nom, c.prenom, c.adress, c.age, c.sexe);
        }
        fclose(f);
    } else {
        printf("Error opening the file.\n");
    }
}


/////////////////////////////////************************TREEVIEW******************************************/////////////




 #include <gtk/gtk.h>
 // Pour la structure Facture

void afficher_Facture(GtkWidget *treeview) {
    GtkListStore *store;
    GtkTreeIter iter;
    FILE *f;

    // Colonnes pour le TreeView
    enum {
        COL_ID,
        COL_NBJOUR,
        COL_MONTANT,
        NUM_COLS
    };

    // Ouvrir le fichier facture.txt
    f = fopen("facture.txt", "r");
    if (!f) {
        perror("Erreur lors de l'ouverture de facture.txt");
        return;
    }

    // Créer un modèle de liste
    store = gtk_list_store_new(NUM_COLS, G_TYPE_INT, G_TYPE_INT, G_TYPE_FLOAT);

    // Lire les données du fichier et les ajouter au modèle
    Facture facture;
    while (fscanf(f, "%d %d %f\n", &facture.idf, &facture.njour, &facture.montant) != EOF) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           COL_ID, facture.idf,
                           COL_NBJOUR, facture.njour,
                           COL_MONTANT, facture.montant,
                           -1);
    }
    fclose(f);

    // Associer le modèle au TreeView
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));

    // Libérer la mémoire du modèle après l'association
    g_object_unref(store);

    // Ajouter les colonnes si elles n'existent pas encore
    GList *columns = gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview));
    if (columns == NULL || g_list_length(columns) == 0) {
        GtkCellRenderer *renderer;

        // Colonne ID
        renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview),
                                                    -1, "ID",
                                                    renderer, "text", COL_ID,
                                                    NULL);

        // Colonne Nombre de jours (NBJOUR)
        renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview),
                                                    -1, "Nombre de jours",
                                                    renderer, "text", COL_NBJOUR,
                                                    NULL);

        // Colonne Montant
        renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview),
                                                    -1, "Montant",
                                                    renderer, "text", COL_MONTANT,
                                                    NULL);
    }

    g_list_free(columns); // Libérer la liste des colonnes
}


//////////////////////////////////////////////////////////////////////////////////////
void viderF(GtkTreeView *treeview) {
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

        // Créer les colonnes pour afficher les informations des factures
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("idf", renderer, "text", 0, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("njour", renderer, "text", 1, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("montant", renderer, "text", 2, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        // Créer un store pour les Citoyens : id, nom, description, tarif
        store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_FLOAT);
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    }
}

