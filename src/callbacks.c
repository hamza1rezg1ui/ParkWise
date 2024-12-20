#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "services.h" 
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agent.h"
#include "gererParking.h"
#include "agentParking1.h"
#include "citoyen.h"

#define MAX_IDS 100

//**************************Authentication******************************


            // Fonction pour vérifier l'identifiant et le mot de passe
gboolean verifier_authentification(const char *id, const char *mdp, const char *fichier) {
    FILE *file = fopen(fichier, "r");
    if (file == NULL) {
        g_print("Erreur : Impossible d'ouvrir le fichier %s\n", fichier);
        return FALSE;
    }

    char ligne[256], fichier_id[50], fichier_mdp[50];
    while (fgets(ligne, sizeof(ligne), file)) {
        sscanf(ligne, "%s %s", fichier_id, fichier_mdp);
        if (strcmp(id, fichier_id) == 0 && strcmp(mdp, fichier_mdp) == 0) {
            fclose(file);
            return TRUE; // Authentification réussie
        }
    }

    fclose(file);
    return FALSE; // Authentification échouée
}
 ////----------------------------------------------------------------------------------------------------------------//

  void on_buttonConnect_clicked(GtkButton *button, gpointer user_data) {
    // Widgets pour récupérer les informations
    GtkWidget *entryAdmin = lookup_widget(GTK_WIDGET(button), "entryAdmin");
    GtkWidget *entryPass = lookup_widget(GTK_WIDGET(button), "entryPass");
    GtkWidget *authWindow = lookup_widget(GTK_WIDGET(button), "Authentication");

    // Récupérer le texte des champs
    const char *id = gtk_entry_get_text(GTK_ENTRY(entryAdmin));
    const char *mdp = gtk_entry_get_text(GTK_ENTRY(entryPass));

    // Vérifier les informations
    if (verifier_authentification(id, mdp, "admin.txt")) {
        g_print("Connexion réussie.\n");

        // Fermer la fenêtre Authentication
        gtk_widget_destroy(authWindow);

        // Créer et afficher la fenêtre principale (Choix)
        GtkWidget *Choix = create_Choix();
        gtk_widget_show(Choix);
	// Ajouter l'image dans la fenêtre principale
   // Afficher l'image
    } else {
        g_print("Échec de la connexion.\n");

        // Afficher une boîte de dialogue d'erreur
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(authWindow),
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_ERROR,
                                                  GTK_BUTTONS_OK,
                                                  "Identifiant ou mot de passe incorrect.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}









//******************************mainWindow*******************************
void on_buttonChercherService_clicked(GtkButton *button, gpointer user_data) {
    // Récupérer le widget GtkEntry qui contient l'ID du service
    GtkWidget *entryIdService = lookup_widget(button, "entryIdService");  // Assurez-vous que le nom correspond à l'ID de votre GtkEntry dans l'interface
    const char *idServiceStr = gtk_entry_get_text(GTK_ENTRY(entryIdService));  // Récupérer l'ID du service en tant que chaîne
    int idService = atoi(idServiceStr);  // Convertir l'ID en entier

    // Chercher le service avec l'ID dans le fichier
    Service s = chercher("services.txt", idService);  // Remplacer "services.txt" par le nom du fichier que tu utilises pour stocker les services

    // Si le service est trouvé
    if (s.id != -1) {
        // Créer un message de dialogue pour afficher les détails du service
        GtkWidget *dialog = gtk_dialog_new_with_buttons("Détails du service",  // Titre de la fenêtre
                                                       GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                                       GTK_DIALOG_MODAL,
                                                       GTK_STOCK_OK, GTK_RESPONSE_OK,
                                                       NULL);

        // Récupérer le contenu du dialogue pour y ajouter les détails
        GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

        // Créer un box vertical pour organiser les widgets (labels)
        GtkWidget *vbox = gtk_vbox_new(FALSE, 10);
        gtk_container_add(GTK_CONTAINER(content_area), vbox);

        // Afficher le nom
        GtkWidget *labelNom = gtk_label_new(s.nom);
        gtk_box_pack_start(GTK_BOX(vbox), labelNom, FALSE, FALSE, 0);

        // Afficher la description
        GtkWidget *labelDescription = gtk_label_new(s.description);
        gtk_box_pack_start(GTK_BOX(vbox), labelDescription, FALSE, FALSE, 0);

        // Afficher le tarif
        char tarifStr[20];
        snprintf(tarifStr, sizeof(tarifStr), "%.2f", s.tarif);
        GtkWidget *labelTarif = gtk_label_new(tarifStr);
        gtk_box_pack_start(GTK_BOX(vbox), labelTarif, FALSE, FALSE, 0);

        // Afficher le dialogue
        gtk_widget_show_all(dialog);

        // Attendre que l'utilisateur clique sur le bouton OK pour fermer la fenêtre
        gtk_dialog_run(GTK_DIALOG(dialog));

        // Fermer le dialogue
        gtk_widget_destroy(dialog);
    } else {
        // Si le service n'est pas trouvé, afficher un message d'erreur sous forme de boîte de dialogue
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_ERROR,
                                                  GTK_BUTTONS_OK,
                                                  "Service non trouvé.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);  // Fermer la boîte de dialogue après avoir cliqué sur OK
    }
}


// Déclaration de la fonction ajouter
int ajouter(const char *filename, Service nouveauService);

int existeID(char *filename, int id) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        return 0;  // Erreur d'ouverture du fichier
    }

    Service service;
    while (fscanf(f, "%d %s %s %f", &service.id, service.nom, service.description, &service.tarif) == 4) {
        if (service.id == id) {
            fclose(f);
            return 1;  // ID déjà existant
        }
    }

    fclose(f);
    return 0;  // ID n'existe pas
}

void on_Ajouter2_clicked(GtkButton *button, gpointer user_data) {
    // Récupérer les entrées dans les champs GtkEntry
    GtkWidget *entryID = lookup_widget(button, "entry19");  // ID
    GtkWidget *entryNom = lookup_widget(button, "entry20");  // Nom
    GtkWidget *entryDescription = lookup_widget(button, "entry21");  // Description
    GtkWidget *spinbuttonTarif = lookup_widget(button, "spinbutton1");  // SpinButton pour le Tarif

    // Récupérer les valeurs saisies dans les GtkEntry
    const gchar *idText = gtk_entry_get_text(GTK_ENTRY(entryID));
    const gchar *nomText = gtk_entry_get_text(GTK_ENTRY(entryNom));
    const gchar *descriptionText = gtk_entry_get_text(GTK_ENTRY(entryDescription));
    
    // Récupérer la valeur du tarif depuis le spinbutton
    double tarifValue = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinbuttonTarif));

    // Vérification que tous les champs sont remplis
    if (strlen(idText) == 0 || strlen(nomText) == 0 || strlen(descriptionText) == 0 || tarifValue == 0) {
        // Si un champ est vide ou si le tarif est égal à 0, afficher un message d'erreur
        GtkWidget *message = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Tous les champs doivent être remplis, y compris un tarif valide.");
        gtk_dialog_run(GTK_DIALOG(message));
        gtk_widget_destroy(message);
        return;  // Empêcher l'ajout si un champ est vide
    }

    // Créer un objet Service
    Service nouveauService;

    // Convertir les valeurs appropriées
    nouveauService.id = atoi(idText);  // Convertir ID en entier
    strncpy(nouveauService.nom, nomText, sizeof(nouveauService.nom));  // Nom
    strncpy(nouveauService.description, descriptionText, sizeof(nouveauService.description));  // Description
    nouveauService.tarif = (float) tarifValue;  // Tarif depuis le spinbutton

    // Vérifier si l'ID existe déjà dans le fichier
    char *filename = "services.txt";
    if (existeID(filename, nouveauService.id)) {
        // Si l'ID existe déjà, afficher un message d'erreur et demander un nouvel ID
        GtkWidget *message = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "L'ID existe déjà. Veuillez saisir un autre ID.");
        gtk_dialog_run(GTK_DIALOG(message));
        gtk_widget_destroy(message);
        // Réinitialiser le champ ID
        gtk_entry_set_text(GTK_ENTRY(entryID), "");
        return;  // Ne pas ajouter le service
    }

    // Appeler la fonction ajouter pour ajouter le service au fichier
    if (ajouter(filename, nouveauService)) {
        // Si l'ajout a réussi, afficher un message de confirmation ou réinitialiser les champs
        gtk_entry_set_text(GTK_ENTRY(entryID), "");  // Réinitialiser le champ ID
        gtk_entry_set_text(GTK_ENTRY(entryNom), "");  // Réinitialiser le champ Nom
        gtk_entry_set_text(GTK_ENTRY(entryDescription), "");  // Réinitialiser le champ Description
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbuttonTarif), 0);  // Réinitialiser le spinbutton Tarif

        // Afficher un message de succès
        GtkWidget *message = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Service ajouté avec succès !");
        gtk_dialog_run(GTK_DIALOG(message));
        gtk_widget_destroy(message);
    } else {
        // Si l'ajout échoue, afficher un message d'erreur
        GtkWidget *message = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Erreur lors de l'ajout du service.");
        gtk_dialog_run(GTK_DIALOG(message));
        gtk_widget_destroy(message);
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// Fonction pour charger les IDs depuis le fichier
int load_ids_from_file(const char *filename, char ids[MAX_IDS][50]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        return 0;
    }

    int count = 0;
    while (fgets(ids[count], 50, file) != NULL && count < MAX_IDS) {
        // Retirer le caractère de nouvelle ligne à la fin de chaque ligne
        ids[count][strcspn(ids[count], "\n")] = 0;
        count++;
    }

    fclose(file);
    return count;
}

// Fonction appelée lors du clic sur le bouton "Initialiser"
void on_Intialiser_clicked(GtkButton *button, gpointer user_data) {
     // Le GtkComboBox où on va ajouter les IDs
    char ids[MAX_IDS][50]; // Tableau pour stocker les IDs
    int id_count, i;

    // Associer le GtkComboBox à la variable
    GtkWidget *combobox1 = lookup_widget(button, "combobox1"); // Ici, 'user_data' devrait être le GtkComboBox que tu passes lors de l'appel
     int n_items = gtk_combo_box_get_model(GTK_COMBO_BOX(combobox1)) ? gtk_tree_model_iter_n_children(gtk_combo_box_get_model(GTK_COMBO_BOX(combobox1)), NULL) : 0;
    for (i = 0; i < n_items; i++) {
        gtk_combo_box_remove_text(GTK_COMBO_BOX(combobox1), 0);  // Supprimer chaque élément
    }

    // Charger les IDs depuis le fichier
    id_count = load_ids_from_file("ID.txt", ids);

    if (id_count == 0) {
        g_print("Aucun ID chargé.\n");
        return;
    }

    

    // Ajouter les IDs dans le GtkComboBox
    for (i = 0; i < id_count; i++) {
        gtk_combo_box_append_text(GTK_COMBO_BOX(combobox1), ids[i]);
    }
}

/////////////////////////////////////////////////////////////////////////////////////

float tarif=25.0;

void on_radiobutton25DT_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) {
        tarif = 25.0;
        g_print("Radiobutton 25DT activé\n");
    }
}

void on_radiobutton75DT_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) {
        tarif = 75.0;
        g_print("Radiobutton 75DT activé\n");
    }
}

void on_radiobutton50DT_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) {
        tarif = 50.0;
        g_print("Radiobutton 50DT activé\n");
    }
}

void on_checkbuttonVerifier_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) {
        g_print("Checkbutton vérifié\n");
    } else {
        g_print("Checkbutton non vérifié\n");
    }
}

void on_Modifier2_clicked(GtkButton *button, gpointer user_data) {
    // Récupération des widgets
    GtkWidget *combobox1 = lookup_widget(GTK_WIDGET(button), "combobox1");
    GtkWidget *entryNom = lookup_widget(GTK_WIDGET(button), "entry25");
    GtkWidget *entryDescription = lookup_widget(GTK_WIDGET(button), "entry26");
    GtkWidget *checkbutton2 = lookup_widget(GTK_WIDGET(button), "checkbutton2");

    const char *id_selected = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox1));
    int id = atoi(id_selected);

    const char *nom = gtk_entry_get_text(GTK_ENTRY(entryNom));
    const char *description = gtk_entry_get_text(GTK_ENTRY(entryDescription));
    gboolean est_verifie = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton2));

    // Création de l'objet Service
    Service nouv;
    nouv.id = id;
    strcpy(nouv.nom, nom);
    strcpy(nouv.description, description);
    nouv.tarif = tarif;

    // Mise à jour dans le fichier
    int resultat = modifier("services.txt", id, nouv);

    if (resultat) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_INFO,
                                                   GTK_BUTTONS_OK,
                                                   "Service mis à jour avec succès.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        // Réinitialiser les champs
        gtk_entry_set_text(GTK_ENTRY(entryNom), "");
        gtk_entry_set_text(GTK_ENTRY(entryDescription), "");
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton2), FALSE);
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Erreur lors de la mise à jour du service.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}















/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// Déclaration de la variable globale pour savoir si le CheckButton est coché
static gboolean suppression_confirmee = FALSE;
static int service_a_supprimer_id = -1;

// Fonction appelée lors du clic sur le CheckButton (remplaçant des radiobuttons OUI et NON)
void on_checkbutton3_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    // Vérifier si le CheckButton est coché
    if (gtk_toggle_button_get_active(togglebutton)) {
        suppression_confirmee = TRUE;  // La suppression est confirmée
        g_print("Suppression confirmée.\n");
    } else {
        suppression_confirmee = FALSE;  // La suppression est annulée
        g_print("Suppression annulée.\n");
    }
}

// Fonction appelée lors du clic sur le bouton "Initialiser"
void on_Initialiser2_clicked(GtkButton *button, gpointer user_data) {
    // Le GtkComboBox où on va ajouter les IDs
    char ids[MAX_IDS][50]; // Tableau pour stocker les IDs
    int id_count, i;

    // Associer le GtkComboBox à la variable
    GtkWidget *combobox2 = lookup_widget(button, "combobox2");
     int n_items = gtk_combo_box_get_model(GTK_COMBO_BOX(combobox2)) ? gtk_tree_model_iter_n_children(gtk_combo_box_get_model(GTK_COMBO_BOX(combobox2)), NULL) : 0;
    for (i = 0; i < n_items; i++) {
        gtk_combo_box_remove_text(GTK_COMBO_BOX(combobox2), 0);  // Supprimer chaque élément
    }

    // Charger les IDs depuis le fichier
    id_count = load_ids_from_file("ID.txt", ids);

    if (id_count == 0) {
        g_print("Aucun ID chargé.\n");
        return;
    }

    // Ajouter les IDs dans le GtkComboBox
    for (i = 0; i < id_count; i++) {
        gtk_combo_box_append_text(GTK_COMBO_BOX(combobox2), ids[i]);
    }

    // Réinitialiser la variable de confirmation de suppression et le CheckButton
    GtkWidget *checkbutton3 = lookup_widget(GTK_WIDGET(button), "checkbutton3");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton3), FALSE);  // Décocher le CheckButton
    suppression_confirmee = FALSE;  // Annuler la confirmation de suppression
    service_a_supprimer_id = -1;  // Réinitialiser l'ID du service à supprimer

    g_print("Interface initialisée.\n");
}

// Fonction appelée lors du clic sur le bouton "Supprimer"
void on_buttonSupprimer_clicked(GtkButton *button, gpointer user_data) {
    // Récupérer l'ID du service à supprimer depuis combobox2
    GtkWidget *combobox2 = lookup_widget(GTK_WIDGET(button), "combobox2");
    const char *id_selected = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox2));

    if (id_selected != NULL) {
        service_a_supprimer_id = atoi(id_selected);  // Convertir l'ID en entier
    }

    // Vérifier si la suppression est confirmée
    if (suppression_confirmee && service_a_supprimer_id != -1) {
        // Appeler une fonction pour supprimer le service
        int resultat = supprimer("services.txt", service_a_supprimer_id);
        if (resultat) {
            g_print("Service supprimé avec succès.\n");

            // Afficher un message de confirmation
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                      GTK_DIALOG_MODAL,
                                                      GTK_MESSAGE_INFO,
                                                      GTK_BUTTONS_OK,
                                                      "Service supprimé avec succès.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        } else {
            // Afficher un message d'erreur si la suppression échoue
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                      GTK_DIALOG_MODAL,
                                                      GTK_MESSAGE_ERROR,
                                                      GTK_BUTTONS_OK,
                                                      "Erreur lors de la suppression du service.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        }
    } else if (!suppression_confirmee) {
        // Afficher un message si la suppression n'est pas confirmée
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_WARNING,
                                                  GTK_BUTTONS_OK,
                                                  "Suppression annulée. Veuillez confirmer la suppression.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        // Afficher un message d'erreur si aucun service n'est sélectionné
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_ERROR,
                                                  GTK_BUTTONS_OK,
                                                  "Veuillez sélectionner un service à supprimer.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

// Fonction appelée lors du clic sur le bouton "Annuler"
void on_buttonAnnuler_clicked(GtkButton *button, gpointer user_data) {
    // Réinitialiser les radiobuttons et combobox
    GtkWidget *checkbutton3 = lookup_widget(GTK_WIDGET(button), "checkbutton3");
    GtkWidget *combobox2 = lookup_widget(GTK_WIDGET(button), "combobox2");

    // Décocher le CheckButton
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton3), FALSE);

    // Réinitialiser la combobox
    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox2), -1);

    // Annuler la confirmation de la suppression
    suppression_confirmee = FALSE;
    service_a_supprimer_id = -1;

    g_print("Suppression annulée.\n");
}



















//////////////Ouverture TREEVIEW//////////////

void on_AfficherTout_clicked(GtkButton *button, gpointer user_data) {
    // Récupérer la fenêtre principale (mainWindow) et la fenêtre TREEVIEW
    GtkWidget *main_window = lookup_widget(GTK_WIDGET(button), "mainWindow");
    GtkWidget *tree_view_window = lookup_widget(GTK_WIDGET(button), "TREEVIEW");

    // Si la fenêtre TREEVIEW n'est pas trouvée, utiliser une méthode alternative pour l'obtenir
    if (tree_view_window == NULL) {
        tree_view_window = create_TREEVIEW();  // Crée la fenêtre TREEVIEW si elle n'existe pas encore
    }

    // Vérification finale que TREEVIEW est valide
    if (tree_view_window == NULL) {
        g_warning("Erreur : La fenêtre TREEVIEW est introuvable.");
        return;
    }

    // Récupérer le TreeView de la fenêtre TREEVIEW
    GtkWidget *treeview = lookup_widget(tree_view_window, "treeview");

    // Afficher les services dans le TreeView
    afficher_services(treeview);

    // Cacher la fenêtre principale
    gtk_widget_hide(main_window);

    // Afficher la fenêtre TREEVIEW
    gtk_widget_show(tree_view_window);
}


////////////////////////////////////SIGNAL ACTUALISER ******************



void on_buttonActualiser_clicked(GtkButton *button, gpointer user_data) {
    // Récupérer la fenêtre principale (mainWindow)
    GtkWidget *main_window = lookup_widget(GTK_WIDGET(button), "mainWindow");
    if (main_window == NULL) {
        g_warning("Erreur : Le widget mainWindow est introuvable.");
    } else {
        g_message("Le widget mainWindow a été trouvé.");
    }

    // Vérifier que le widget est un widget valide avant de le cacher
    if (GTK_IS_WIDGET(main_window)) {
        g_message("Cacher la fenêtre principale.");
        gtk_widget_hide(main_window);
    }

    // Créer et afficher la fenêtre TREEVIEW
    GtkWidget *tree_view_window = lookup_widget(GTK_WIDGET(button), "TREEVIEW");
    if (tree_view_window == NULL) {
        tree_view_window = create_TREEVIEW();  // Crée la fenêtre TREEVIEW si elle n'existe pas encore
    }

    if (GTK_IS_WIDGET(tree_view_window)) {
        g_message("Afficher la fenêtre TREEVIEW.");
        gtk_widget_show(tree_view_window);
    } else {
        g_warning("Erreur : La fenêtre TREEVIEW est introuvable.");
    }
}



////////////////////************BOUTON RETOUR****************************************////////////////////
void on_buttonRetour_clicked(GtkButton *button, gpointer user_data) {
    // Récupérer la fenêtre TREEVIEW et la fenêtre mainWindow
    GtkWidget *tree_view_window = lookup_widget(GTK_WIDGET(button), "TREEVIEW");
    GtkWidget *main_window = lookup_widget(GTK_WIDGET(button), "mainWindow");

    // Si la fenêtre mainWindow n'est pas trouvée, la recréer
    if (main_window == NULL) {
        main_window = create_mainWindow(); // Crée la fenêtre mainWindow si elle n'existe pas encore
    }

    // Vérifier que TREEVIEW est valide et la cacher
    if (tree_view_window != NULL) {
        gtk_widget_hide(tree_view_window); // Cacher la fenêtre TREEVIEW
    } else {
        g_warning("Erreur : La fenêtre TREEVIEW est introuvable.");
    }

    // Afficher la fenêtre mainWindow
    gtk_widget_show(main_window);
}

////////////////////////////////////////////////SUPPRESSION ////////////////////////////////////////////

void on_treeview_row_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
    GtkTreeIter iter;
    gchar *nom;
    gchar *description;
    gfloat tarif;
    int id_service;  // On récupère l'ID du service à supprimer
    Service s; // Structure pour stocker les données du service

    GtkTreeModel *model = gtk_tree_view_get_model(treeview);

    // Si l'itérateur correspond à la ligne sélectionnée, on récupère les données
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        gtk_tree_model_get(GTK_LIST_STORE(model), &iter, 0, &id_service, 1, &nom, 2, &description, 3, &tarif, -1);

        // Copier les valeurs récupérées dans la structure s
        s.id = id_service;
        strcpy(s.nom, nom);
        strcpy(s.description, description);
        s.tarif = tarif;

        // Appeler la fonction pour supprimer le service
        if (supprimer("services.txt", s.id)) {
            // Si la suppression réussit, mettre à jour l'affichage
            vider(treeview);  // Vider le TreeView avant de le remplir avec les nouvelles données
            afficher_services(treeview);  // Ré-afficher les services après suppression
        } else {
            g_print("Échec de la suppression du service avec l'ID %d.\n", s.id);
        }
    }

    // Libération de la mémoire si nécessaire
    g_free(nom);
    g_free(description);
}

/////////////////////////////***********************************************************/////////////////////////////////////////


// Fonction appelée lors du clic sur le bouton "Charger Service"
void on_buttonChargerS_clicked(GtkButton *button, gpointer user_data) {
     // Le GtkComboBox où on va ajouter les IDs
    char ids[MAX_IDS][50]; // Tableau pour stocker les IDs
    int id_count, i;

    // Associer le GtkComboBox à la variable
    GtkWidget *combobox3 = lookup_widget(button, "combobox3"); // Ici, 'user_data' devrait être le GtkComboBox que tu passes lors de l'appel
     int n_items = gtk_combo_box_get_model(GTK_COMBO_BOX(combobox3)) ? gtk_tree_model_iter_n_children(gtk_combo_box_get_model(GTK_COMBO_BOX(combobox3)), NULL) : 0;
    for (i = 0; i < n_items; i++) {
        gtk_combo_box_remove_text(GTK_COMBO_BOX(combobox3), 0);  // Supprimer chaque élément
    }

    // Charger les IDs depuis le fichier
    id_count = load_ids_from_file("ID.txt", ids);

    if (id_count == 0) {
        g_print("Aucun ID chargé.\n");
        return;
    }

    

    // Ajouter les IDs dans le GtkComboBox
    for (i = 0; i < id_count; i++) {
        gtk_combo_box_append_text(GTK_COMBO_BOX(combobox3), ids[i]);
    }
}
///////////////////////////////////////////////////////**********************************************************//////////////////////////////
void on_buttonChargerR_clicked(GtkButton *button, gpointer user_data) {
     // Le GtkComboBox où on va ajouter les IDs
    char ids[MAX_IDS][50]; // Tableau pour stocker les IDs
    int id_count, i;

    // Associer le GtkComboBox à la variable
    GtkWidget *combobox4 = lookup_widget(button, "combobox4"); // Ici, 'user_data' devrait être le GtkComboBox que tu passes lors de l'appel
     int n_items = gtk_combo_box_get_model(GTK_COMBO_BOX(combobox4)) ? gtk_tree_model_iter_n_children(gtk_combo_box_get_model(GTK_COMBO_BOX(combobox4)), NULL) : 0;
    for (i = 0; i < n_items; i++) {
        gtk_combo_box_remove_text(GTK_COMBO_BOX(combobox4), 0);  // Supprimer chaque élément
    }

    // Charger les IDs depuis le fichier
    id_count = load_ids_from_file("IDR.txt", ids);

    if (id_count == 0) {
        g_print("Aucun ID chargé.\n");
        return;
    }

    

    // Ajouter les IDs dans le GtkComboBox
    for (i = 0; i < id_count; i++) {
        gtk_combo_box_append_text(GTK_COMBO_BOX(combobox4), ids[i]);
    }
}
/////////////////////////////////////////////////////*****************************************************//////////////////////////////////////////////////
void on_buttonVoirDetail_clicked(GtkButton *button, gpointer user_data) {
    // Récupérer les combobox et clists
    GtkWidget *combobox3 = lookup_widget(button, "combobox3");
    GtkWidget *combobox4 = lookup_widget(button, "combobox4");
    GtkWidget *clist1 = lookup_widget(button, "clist1");
    GtkWidget *clist2 = lookup_widget(button, "clist2");

    // Récupérer l'ID sélectionné dans combobox3 (service ID)
    gchar *id_service_str = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox3));
    if (!id_service_str) {
        g_print("Aucun ID de service sélectionné.\n");
        return;
    }
    int id_service = atoi(id_service_str);

    // Récupérer l'ID sélectionné dans combobox4 (réservation ID)
    gchar *id_reservation_str = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox4));
    if (!id_reservation_str) {
        g_print("Aucun ID de réservation sélectionné.\n");
        return;
    }
    int id_reservation = atoi(id_reservation_str);

    // Initialiser les GtkCLists (vider les anciennes données)
    gtk_clist_clear(GTK_CLIST(clist1));
    gtk_clist_clear(GTK_CLIST(clist2));

    // Rechercher le service correspondant dans "services.txt"
    FILE *f_service = fopen("services.txt", "r");
    if (f_service) {
        int service_id;
        char service_nom[100], service_description[200];
        float service_tarif;
        while (fscanf(f_service, "%d %s %s %f", &service_id, service_nom, service_description, &service_tarif) != EOF) {
            if (service_id == id_service) {
                // Ajouter les informations dans clist1
                char tarif_str[20];
                snprintf(tarif_str, sizeof(tarif_str), "%.2f", service_tarif);
                gchar *row[] = {service_nom, service_description, tarif_str};
                gtk_clist_append(GTK_CLIST(clist1), row);
                break;
            }
        }
        fclose(f_service);
    } else {
        g_print("Impossible d'ouvrir services.txt.\n");
    }

    // Rechercher la réservation correspondante dans "reservation.txt"
    FILE *f_reservation = fopen("reservation.txt", "r");
    if (f_reservation) {
        int reservation_id;
        char reservation_nom[100];
        char reservation_date[20];
        while (fscanf(f_reservation, "%d %s %s", &reservation_id, reservation_nom, reservation_date) != EOF) {
            if (reservation_id == id_reservation) {
                // Ajouter les informations dans clist2
                gchar *row[] = {reservation_nom, reservation_date};
                gtk_clist_append(GTK_CLIST(clist2), row);
                break;
            }
        }
        fclose(f_reservation);
    } else {
        g_print("Impossible d'ouvrir reservation.txt.\n");
    }

    // Libérer la mémoire des IDs lus
    g_free(id_service_str);
    g_free(id_reservation_str);
}

///////////////////////////////////*****AFFECTATION******///////////////////////////////////////////////////////////////////////


void on_buttonAffecter_clicked(GtkButton *button, gpointer user_data) {
    // Récupérer les widgets nécessaires
    GtkWidget *combobox3 = lookup_widget(button, "combobox3");
    GtkWidget *combobox4 = lookup_widget(button, "combobox4");

    if (!combobox3 || !combobox4) {
        g_print("Erreur : Widgets combobox introuvables.\n");
        return;
    }

    // Récupérer l'ID sélectionné dans combobox3 (service ID)
    gchar *id_service_str = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox3));
    if (!id_service_str) {
        g_print("Aucun ID de service sélectionné.\n");
        return;
    }
    int id_service = atoi(id_service_str);

    // Récupérer l'ID sélectionné dans combobox4 (réservation ID)
    gchar *id_reservation_str = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox4));
    if (!id_reservation_str) {
        g_print("Aucun ID de réservation sélectionné.\n");
        g_free(id_service_str);
        return;
    }
    int id_reservation = atoi(id_reservation_str);

    // Variables pour stocker les informations
    char nom_service[100] = "";
    char description_service[200] = "";
    float tarif_service = 0.0;
    char nom_citoyen[100] = "";
    char date_reservation[20] = "";

    // Lire les informations du service dans "services.txt"
    FILE *f_service = fopen("services.txt", "r");
    if (f_service) {
        int service_id;
        char service_nom[100], service_description[200];
        float service_tarif;
        while (fscanf(f_service, "%d %s %s %f", &service_id, service_nom, service_description, &service_tarif) != EOF) {
            if (service_id == id_service) {
                strncpy(nom_service, service_nom, sizeof(nom_service) - 1);
                strncpy(description_service, service_description, sizeof(description_service) - 1);
                tarif_service = service_tarif;
                break;
            }
        }
        fclose(f_service);
    } else {
        g_print("Erreur : Impossible d'ouvrir services.txt.\n");
    }

    // Lire les informations de la réservation dans "reservation.txt"
    FILE *f_reservation = fopen("reservation.txt", "r");
    if (f_reservation) {
        int reservation_id;
        char reservation_nom[100];
        char reservation_date[20];
        while (fscanf(f_reservation, "%d %s %s", &reservation_id, reservation_nom, reservation_date) != EOF) {
            if (reservation_id == id_reservation) {
                strncpy(nom_citoyen, reservation_nom, sizeof(nom_citoyen) - 1);
                strncpy(date_reservation, reservation_date, sizeof(date_reservation) - 1);
                break;
            }
        }
        fclose(f_reservation);
    } else {
        g_print("Erreur : Impossible d'ouvrir reservation.txt.\n");
    }

    // Vérifier que toutes les informations ont été trouvées
    if (strlen(nom_service) == 0 || strlen(nom_citoyen) == 0) {
        g_print("Erreur : Informations du service ou de la réservation introuvables.\n");

        // Afficher un dialogue d'erreur
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Échec de l'affectation : informations introuvables."
        );
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        g_free(id_service_str);
        g_free(id_reservation_str);
        return;
    }

    // Ajouter l'affectation dans le fichier "affectations.txt"
    FILE *f_affectations = fopen("affectations.txt", "a");
    if (f_affectations) {
        fprintf(f_affectations, "%d %s %d %s\n", id_service, nom_service, id_reservation, date_reservation);
        fclose(f_affectations);

        // Afficher un dialogue de succès
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Affectation réussie :\nService %s affecté à la réservation %d (%s).",
            nom_service, id_reservation, date_reservation
        );
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        g_print("Affectation effectuée : Service %d (%s) -> Réservation %d (%s)\n", id_service, nom_service, id_reservation, date_reservation);
    } else {
        g_print("Erreur : Impossible d'ouvrir affectations.txt.\n");

        // Afficher un dialogue d'erreur
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Échec de l'affectation : impossible d'ouvrir affectations.txt."
        );
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }

    // Libérer les chaînes allouées par gtk_combo_box_get_active_text
    g_free(id_service_str);
    g_free(id_reservation_str);
}

////////////////////////////////////*******QUIT/////////////////////////////////
void on_buttonQuitter_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog;
    GtkWidget *parent_window = lookup_widget(GTK_WIDGET(button), "mainWindow");

    // Créer une boîte de dialogue de confirmation
    dialog = gtk_message_dialog_new(GTK_WINDOW(parent_window),
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_QUESTION,
                                    GTK_BUTTONS_YES_NO,
                                    "Êtes-vous sûr de vouloir quitter ?");

    // Afficher la boîte de dialogue et récupérer la réponse
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_YES) {
        // Quitter l'application
        gtk_main_quit();
    }

    // Détruire la boîte de dialogue
    gtk_widget_destroy(dialog);
}

///////////////////////////////Citoyen//////////////////////////////////////////////////////////


void
on_buttonCtoyen_clicked  (GtkWidget *objet_graphique, gpointer user_data)
{
GtkWidget*n;
GtkWidget*p;
n=lookup_widget(objet_graphique, "AdminCitoyen");
p=lookup_widget(objet_graphique, "Citoyen");
p=  create_Citoyen ();
gtk_widget_show(p);
gtk_widget_destroy(n);

}

/////////////////////////////////**********************/////////////////////////////////////////////////
void
on_buttonAdmin_clicked  (GtkWidget *objet_graphique, gpointer user_data)
{
GtkWidget*n;
GtkWidget*p;
n=lookup_widget(objet_graphique, "AdminCitoyen");
p=lookup_widget(objet_graphique, "Authentication");
p=  create_Authentication ();
gtk_widget_show(p);
gtk_widget_destroy(n);

}

////////////////////////////////////////////****gestionaire Parking///////********************************************////////////////////////////////



//////////////////////////////////Gestionaire Agents/////////////////////////////////////////////////////////////////////////////////////////////////////

int x=1;
int x2=1;
int c=0;
void
on_treeview1_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{
GtkTreeIter iter;
gchar *idag,*nom_ag,*prenom_ag,*sexe,*poste;
gint  *day_ag,*month_ag,*year_ag;
agent d;
GtkTreeModel *model=gtk_tree_view_get_model(treeview);
if (gtk_tree_model_get_iter(model,&iter,path))
{ gtk_tree_model_get(GTK_TREE_MODEL(model),&iter,0,idag,1,nom_ag,2,prenom_ag,3,day_ag,4,month_ag,5,year_ag,6,sexe,7,poste,-1);
strcpy(d.idag,idag);
strcpy(d.nom_ag,nom_ag);
strcpy(d.prenom_ag,prenom_ag);
strcpy(d.sexe,sexe);
strcpy(d.poste,poste);
d.age.jj=*day_ag;
d.age.mm=*month_ag;
d.age.aa=*year_ag;
afficheragent(treeview);}
}


void
on_btngotoaj_clicked                   (GtkWidget *objet_graphique, gpointer user_data)
{
GtkWidget*n;
GtkWidget*p;
n=lookup_widget(objet_graphique, "AfficherAgents");
p=lookup_widget(objet_graphique, "AjoutAgent");
p=  create_AjoutAgent();
gtk_widget_show(p);
gtk_widget_destroy(n);
}


void
on_btngotomodif_clicked                (GtkWidget *objet_graphique, gpointer user_data)
{
GtkWidget*n;
GtkWidget*p;
n=lookup_widget(objet_graphique, "AfficherAgents");
p=lookup_widget(objet_graphique, "modifieragent");
p=  create_modifieragent();
gtk_widget_show(p);
gtk_widget_destroy(n);
}


void
on_gotosupp_clicked                    (GtkWidget *objet_graphique, gpointer user_data)
{
GtkWidget*n;
GtkWidget*p;
n=lookup_widget(objet_graphique, "AfficherAgents");
p=lookup_widget(objet_graphique, "supprimeragent");
p=  create_supprimeragent();
gtk_widget_show(p);
gtk_widget_destroy(n);
}


void
on_actualiser_clicked                  (GtkWidget *objet_graphique, gpointer user_data)
{
GtkWidget *treeview;
treeview=lookup_widget(objet_graphique,"treeview1");
afficheragent(GTK_TREE_VIEW(treeview));
}


void
on_checkbuttonconfaj_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton)))
c=1;
}


void
on_buttonrtaj_clicked                  (GtkWidget *objet_graphique, gpointer user_data)
{
GtkWidget*n;
GtkWidget*p;
n=lookup_widget(objet_graphique, "AjoutAgent");
p=lookup_widget(objet_graphique, "AfficherAgents");
p=  create_AfficherAgents();
gtk_widget_show(p);
gtk_widget_destroy(n);
}


void
on_btnajagent_clicked                  (GtkWidget *objet_graphique, gpointer user_data)
{
GtkWidget*id;
GtkWidget*nom;
GtkWidget*prenom;
GtkWidget*sexe;
GtkWidget*jour;
GtkWidget*mois;
GtkWidget*anne;
GtkWidget*poste;
GtkWidget*label;
agent a;
int t;
label=lookup_widget(objet_graphique, "labelccaj");
id=lookup_widget(objet_graphique, "entryidagentaj");
nom=lookup_widget(objet_graphique, "entrynomagentaj");
prenom=lookup_widget(objet_graphique, "entryprenomagentaj");
strcpy(a.idag,gtk_entry_get_text(GTK_ENTRY(id)));
strcpy(a.nom_ag,gtk_entry_get_text(GTK_ENTRY(nom))); 
strcpy(a.prenom_ag,gtk_entry_get_text(GTK_ENTRY(prenom))); 
poste=lookup_widget(objet_graphique, "comboboxentryposteaj");
strcpy(a.poste,gtk_combo_box_get_active_text(GTK_COMBO_BOX(poste)));
if (x==1){
strcpy(a.sexe,"homme");}
else 
if (x==2){
strcpy(a.sexe,"femme");}
jour=lookup_widget(objet_graphique, "spinbuttonjouraj");
mois=lookup_widget(objet_graphique, "spinbuttonmoisaj");
anne=lookup_widget(objet_graphique, "spinbuttonanneaj");
a.age.jj=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(jour));
a.age.mm=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(mois));
a.age.aa=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(anne));
if(c==1){
t=ajoutagent(a);
if(t==1)
gtk_label_set_text(GTK_LABEL(label)," valider");
else
gtk_label_set_text(GTK_LABEL(label)," error");
}
else
gtk_label_set_text(GTK_LABEL(label)," Confirmer !");
}


void
on_radiobuttonhommeaj_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton)))
x=1;
}


void
on_radiobuttonfemmeaj_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton)))
x=2;
}


void
on_buttonrechidmodif_clicked           (GtkWidget *objet_graphique, gpointer user_data)
{
GtkWidget*id;
GtkWidget*nom;
GtkWidget*prenom;
GtkWidget*poste;
GtkWidget*jour;
GtkWidget*mois;
GtkWidget*annee;
GtkWidget*homme;
GtkWidget*femme;
GtkWidget*label;
char ch[50];
agent *a;
id=lookup_widget(objet_graphique, "entryidagentmodif");
nom=lookup_widget(objet_graphique, "entrynomagmodif");
prenom=lookup_widget(objet_graphique, "entryprenomagenmodif");
poste=lookup_widget(objet_graphique, "comboboxentrypostemodif");
jour=lookup_widget(objet_graphique, "spinbuttondayagmodif");
mois=lookup_widget(objet_graphique, "spinbuttonmoisagmodif");
annee=lookup_widget(objet_graphique, "spinbuttonanneeagentmodif");
homme=lookup_widget(objet_graphique, "radiobuttonhommemodif");
femme=lookup_widget(objet_graphique, "radiobuttonfemmodif");
label=lookup_widget(objet_graphique, "lablresultmodifrech");
strcpy(ch,gtk_entry_get_text(GTK_ENTRY(id))); 
a=chercheragent(ch);
if(a==NULL)
gtk_label_set_text(GTK_LABEL(label),"non trouvé");
else{
gtk_entry_set_text(GTK_ENTRY(nom),a->nom_ag);
gtk_entry_set_text(GTK_ENTRY(prenom),a->prenom_ag);
if(strcmp(a->sexe,"homme")==0){
gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(homme),TRUE);
}
else{
gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(femme),TRUE);
}
gtk_spin_button_set_value(GTK_SPIN_BUTTON(jour),a->age.jj);
gtk_spin_button_set_value(GTK_SPIN_BUTTON(mois),a->age.mm);
gtk_spin_button_set_value(GTK_SPIN_BUTTON(annee),a->age.aa);
if(strcmp(a->poste,"acceuil")==0)
gtk_combo_box_set_active(GTK_COMBO_BOX(poste),0);
else if(strcmp(a->poste,"securite")==0)
gtk_combo_box_set_active(GTK_COMBO_BOX(poste),1);
else
gtk_combo_box_set_active(GTK_COMBO_BOX(poste),2);
}
}


void
on_radiobuttonfemmodif_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton)))
x2=2;
}


void
on_radiobuttonhommemodif_toggled       (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton)))
x2=1;
}


void
on_btnmodifieragent_clicked            (GtkWidget *objet_graphique, gpointer user_data)
{
GtkWidget*id;
GtkWidget*nom;
GtkWidget*prenom;
GtkWidget*poste;
GtkWidget*jour;
GtkWidget*mois;
GtkWidget*annee;
GtkWidget*label;
agent a;
int t;
id=lookup_widget(objet_graphique, "entryidagentmodif");
nom=lookup_widget(objet_graphique, "entrynomagmodif");
prenom=lookup_widget(objet_graphique, "entryprenomagenmodif");
poste=lookup_widget(objet_graphique, "comboboxentrypostemodif");
jour=lookup_widget(objet_graphique, "spinbuttondayagmodif");
mois=lookup_widget(objet_graphique, "spinbuttonmoisagmodif");
annee=lookup_widget(objet_graphique, "spinbuttonanneeagentmodif");
label=lookup_widget(objet_graphique, "labccmodif");
strcpy(a.idag,gtk_entry_get_text(GTK_ENTRY(id)));
strcpy(a.nom_ag,gtk_entry_get_text(GTK_ENTRY(nom))); 
strcpy(a.prenom_ag,gtk_entry_get_text(GTK_ENTRY(prenom)));
strcpy(a.poste,gtk_combo_box_get_active_text(GTK_COMBO_BOX(poste)));
if (x2==1){
strcpy(a.sexe,"homme");}
else 
if (x2==2){
strcpy(a.sexe,"femme");}
a.age.jj=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(jour));
a.age.mm=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(mois));
a.age.aa=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(annee));
t=modiferagent(a);
if(t==1)
gtk_label_set_text(GTK_LABEL(label)," valider");
else
gtk_label_set_text(GTK_LABEL(label)," error"); 
}


void
on_buttonrtmodi_clicked                (GtkWidget *objet_graphique, gpointer user_data)
{
GtkWidget*n;
GtkWidget*p;
n=lookup_widget(objet_graphique, "modifieragent");
p=lookup_widget(objet_graphique, "AfficherAgents");
p=  create_AfficherAgents();
gtk_widget_show(p);
gtk_widget_destroy(n);
}


void
on_buttonrtsupp_clicked                (GtkWidget *objet_graphique, gpointer user_data)
{
GtkWidget*n;
GtkWidget*p;
n=lookup_widget(objet_graphique, "supprimeragent");
p=lookup_widget(objet_graphique, "AfficherAgents");
p=  create_AfficherAgents();
gtk_widget_show(p);
gtk_widget_destroy(n);
}


void
on_buttonsupprimeagent_clicked         (GtkWidget *objet_graphique, gpointer user_data)
{
GtkWidget*id;
GtkWidget*label;
char ch[50];
int t=0;
label=lookup_widget(objet_graphique, "labelccsupprimer");
id=lookup_widget(objet_graphique, "entrysupprimerag");
strcpy(ch,gtk_entry_get_text(GTK_ENTRY(id))); 
t=supprimeragent(ch);
if(t==1)
gtk_label_set_text(GTK_LABEL(label)," valider");
else
gtk_label_set_text(GTK_LABEL(label)," error");
}


/////////////////////////////////////////////////////////////////////
void
on_buttonGestionaireParking_clicked    (GtkWidget *objet_graphique, gpointer user_data)
{
GtkWidget*n;
GtkWidget*p;
n=lookup_widget(objet_graphique, "Choix");
p=lookup_widget(objet_graphique, "windowParkingHome");
p=  create_windowParkingHome ();
gtk_widget_show(p);
gtk_widget_destroy(n);

}

///////////////////////////////////////////////////////////////////////////////////////////
void
on_buttonGestionaireAgents_clicked     (GtkWidget *objet_graphique, gpointer user_data)
{
GtkWidget*n;
GtkWidget*p;
n=lookup_widget(objet_graphique, "Choix");
p=lookup_widget(objet_graphique, "AfficherAgents");
p=  create_AfficherAgents ();
gtk_widget_show(p);
gtk_widget_destroy(n);

}
//////////////////////////////////////////////////////////////////////////
void
on_buttonGestionaireServices_clicked (GtkWidget *objet_graphique, gpointer user_data)
{
GtkWidget*n;
GtkWidget*p;
n=lookup_widget(objet_graphique, "Choix");
p=lookup_widget(objet_graphique, "mainWindow");
p=  create_mainWindow ();
gtk_widget_show(p);
gtk_widget_destroy(n);

}

//////////////////////////////////////////////////////////////////////////les Bouton Retour/////////////////////////////////////////////////////*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*


char filenameParking[] = "parkings.txt";
char filenameAgentParking1[] = "agentParking1.txt";
int checkSecuriteAjoutParking=1;
int checkSecuriteModifParking=1;
int radioTarifAjoutParking = 1;
int radioTarifModifParking = 1;
void
on_treeviewParkingHome_row_activated   (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{

}


void
on_buttonAjouterParkingW_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *windowParkingAjout,*windowParkingHome;
     windowParkingHome = lookup_widget(button,"windowParkingHome");
      
     windowParkingAjout = lookup_widget(button,"windowParkingAjout");
     windowParkingAjout = create_windowParkingAjout();
     gtk_widget_show(windowParkingAjout);
     gtk_widget_destroy(windowParkingHome);
}


void
on_buttonModifierParkingW_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *windowParkingModif,*windowParkingHome;
     windowParkingHome = lookup_widget(button,"windowParkingHome");
      
     windowParkingModif = lookup_widget(button,"windowParkingModif");
     windowParkingModif = create_windowParkingModif();
     gtk_widget_show(windowParkingModif);
     gtk_widget_destroy(windowParkingHome);
}


void
on_buttonSupprimerParkingW_clicked     (GtkButton       *button,
                                        gpointer         user_data)
{
 GtkWidget *windowParkingSup,*windowParkingHome;
     windowParkingHome = lookup_widget(button,"windowParkingHome");
      
     windowParkingSup = lookup_widget(button,"windowParkingSup");
     windowParkingSup = create_windowParkingSup();
     gtk_widget_show(windowParkingSup);
     gtk_widget_destroy(windowParkingHome);
}


void
on_buttonActualiserParkingW_clicked    (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *windowParkingHome,*w1,*treeview;
      w1=lookup_widget(button,"windowParkingHome");
      windowParkingHome=create_windowParkingHome();
       gtk_widget_show(windowParkingHome);
       gtk_widget_hide(w1);
       treeview=lookup_widget(windowParkingHome,"treeviewParkingHome");
       viderParking(treeview);
       afficherParking(treeview,filenameParking);

}


void
on_buttonStatistiqueParkingW_clicked   (GtkButton       *button,
                                        gpointer         user_data)
{
 GtkWidget *windowParkingAffectation,*windowParkingHome;
     windowParkingHome = lookup_widget(button,"windowParkingHome");
      
     windowParkingAffectation = lookup_widget(button,"windowParkingAffectation");
     windowParkingAffectation = create_windowParkingAffectation();
     gtk_widget_show(windowParkingAffectation);
    // gtk_widget_destroy(windowParkingHome);
}


void
on_buttonRechercheParking_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
       GtkWidget *nom,*pInfo,*windowParkingHome,*w1,*treeview;
       int b = 1;
       char nom1[50];
       windowParkingHome = lookup_widget(button,"windowParkingHome");
       windowParkingHome = create_windowParkingHome();
       nom = lookup_widget(button,"ParkingSearchTextField");
       strcpy(nom1,gtk_entry_get_text(GTK_ENTRY(nom)));
     
       if(strcmp(nom1,"")==0 ){
      
       pInfo = gtk_message_dialog_new(GTK_WINDOW(user_data),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"case est vide  ✔️");
       switch(gtk_dialog_run(GTK_DIALOG(pInfo))){
               case GTK_RESPONSE_OK:
               gtk_widget_destroy(pInfo);
                 
               break;}
       }else{

         
       w1=lookup_widget(button,"windowParkingHome");
       windowParkingHome=create_windowParkingHome();
       gtk_widget_show(windowParkingHome);
       gtk_widget_hide(w1);
       treeview=lookup_widget(windowParkingHome,"treeviewParkingHome");
       viderParking(treeview);
       afficherParkingByID(treeview,filenameParking,nom1);
}
}


void
on_buttonFiltreParking_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{

       GtkWidget *nom,*pInfo,*windowParkingHome,*w1,*treeview;
       int b = 1;
       char nom1[50];
       windowParkingHome = lookup_widget(button,"windowParkingHome");
       windowParkingHome = create_windowParkingHome();
       nom = lookup_widget(button,"ParkingFiltreTextField");
       strcpy(nom1,gtk_combo_box_get_active_text(GTK_COMBO_BOX(nom)));
     
       if(strcmp(nom1,"")==0 ){
      
       pInfo = gtk_message_dialog_new(GTK_WINDOW(user_data),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"case est vide  ✔️");
       switch(gtk_dialog_run(GTK_DIALOG(pInfo))){
               case GTK_RESPONSE_OK:
               gtk_widget_destroy(pInfo);
                 
               break;}
       }else{

         
       w1=lookup_widget(button,"windowParkingHome");
       windowParkingHome=create_windowParkingHome();
       gtk_widget_show(windowParkingHome);
       gtk_widget_hide(w1);
       treeview=lookup_widget(windowParkingHome,"treeviewParkingHome");
       viderParking(treeview);
       afficherParkingByAdresse(treeview,filenameParking,nom1);
}
}


void on_buttonReturnMenuParking_clicked(GtkButton *button, gpointer user_data) {
    // Récupérer la fenêtre "windowParkingHome" et la fenêtre "Choix"
    GtkWidget *window_parking_home = lookup_widget(GTK_WIDGET(button), "windowParkingHome");
    GtkWidget *choix_window = lookup_widget(GTK_WIDGET(button), "Choix");

    // Si la fenêtre Choix n'existe pas encore, créez-la
    if (choix_window == NULL) {
        choix_window = create_Choix();  // Crée la fenêtre Choix si elle n'existe pas encore
    }

    // Vérification finale que la fenêtre Choix est valide
    if (choix_window == NULL) {
        g_warning("Erreur : La fenêtre Choix est introuvable.");
        return;
    }

    // Cacher la fenêtre "windowParkingHome"
    gtk_widget_hide(window_parking_home);

    // Afficher la fenêtre "Choix"
    gtk_widget_show(choix_window);
}



void
on_btnSupParking_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *id ,*windowParkingSup,*windowParkingHome,*labelControl,*pInfo;
    // initialisation de varibale de controlle saisie
       int b = 1;
	char id1[50];
   // declaration de notre struct 
      
       // declaration des variables 
 	windowParkingHome = lookup_widget(button,"windowParkingHome");
 	windowParkingSup = lookup_widget(button,"windowParkingSup");
 	id = lookup_widget(button,"textIdSupParking");
 	
 	labelControl = lookup_widget(button,"textIdSupParkingControl");
 	
 	  // recuperation des information d apres interfaces graphique 
            // recuperation de textField
     strcpy(id1,gtk_entry_get_text(GTK_ENTRY(id)));
          // recuperation de combobox
  
           

        // controlle saisie 
        if(strcmp(id1,"")==0 ){
            
           b=0;
             }
         

     //  ajouter et logique de programme 

          if (b == 0){
         

               gtk_label_set_text(GTK_LABEL(labelControl),"  ⚠️ Erreur : Veuillez vérifier vos données et réessayer. ");



         }else{
          if(!isExistParkingById(filenameParking,id1)){
              gtk_label_set_text(GTK_LABEL(labelControl),"  ⚠️ Id Introuvable , il n existe pas ... ");

	   }else{
       int t =  supprimerParking(filenameParking,id1);
       if(t == 1){
          pInfo = gtk_message_dialog_new(GTK_WINDOW(user_data),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"✅ Votre élément a été Suppression avec succès !");
      switch(gtk_dialog_run(GTK_DIALOG(pInfo))){
               case GTK_RESPONSE_OK:
               gtk_widget_destroy(pInfo);
                  gtk_entry_set_text(GTK_ENTRY(id),"");
                     
                   gtk_label_set_text(GTK_LABEL(labelControl),"");
                
          		 
              
             windowParkingHome = create_windowParkingHome();
             gtk_widget_show(windowParkingHome);
              gtk_widget_destroy(windowParkingSup);
                 
       
               break;


          }
       }else{

   


         gtk_label_set_text(GTK_LABEL(labelControl),"❌ Une erreur est survenue lors de l'Suppression de l'élément. Veuillez réessaye  ");

      }
}
}
}


void
on_btnCancelSupParking_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
   GtkWidget *windowParkingSup,*windowParkingHome;
     windowParkingHome = lookup_widget(button,"windowParkingHome");
      
     windowParkingSup = lookup_widget(button,"windowParkingSup");
     windowParkingHome = create_windowParkingHome();
     gtk_widget_show(windowParkingHome);
     gtk_widget_destroy(windowParkingSup);
}


void
on_checkSecNonAjoutParking_toggled     (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
 if (gtk_toggle_button_get_active(togglebutton))
 {checkSecuriteAjoutParking=2;}
}


void
on_checkSecOuiAjoutParking_toggled     (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
   if (gtk_toggle_button_get_active(togglebutton))
 {checkSecuriteAjoutParking=1;}
}


void
on_radioHoraireAjoutParking_toggled    (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
   if (gtk_toggle_button_get_active(togglebutton))
 {radioTarifAjoutParking=1;}
}


void
on_radioJournalierAjoutParking_toggled (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
   if (gtk_toggle_button_get_active(togglebutton))
 {radioTarifAjoutParking=2;}
}


void
on_btnAjoutParking_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
      GtkWidget *id ,*nom,*adresse,*capacite,*tarif,*windowParkingAjout,*windowParkingHome,*labelControl,*pInfo;
   // initialisation de varibale de controlle saisie
       int b = 1;
   // declaration de notre struct 
       Parking r;
     // declaration des variables 
        windowParkingHome = lookup_widget(button,"windowParkingHome");
 	windowParkingAjout = lookup_widget(button,"windowParkingAjout");
        
        id = lookup_widget(button,"textIdAjoutParking");
         nom = lookup_widget(button,"textNomAjoutParking");
         adresse = lookup_widget(button,"comboAdresseAjoutParking");
         capacite = lookup_widget(button,"spinCapaciteAjoutParking");
	tarif = lookup_widget(button,"spinTarifAjoutParking");
	labelControl = lookup_widget(button,"windowParkingAjoutControl");
           // recuperation des information d apres interfaces graphique 
            // recuperation de textField
    	 strcpy(r.id,gtk_entry_get_text(GTK_ENTRY(id)));
         strcpy(r.nom,gtk_entry_get_text(GTK_ENTRY(nom)));
          // recuperation de combobox
     	strcpy(r.adresse,gtk_combo_box_get_active_text(GTK_COMBO_BOX(adresse)));
        // recuperation de spinButton
    	 r.capacite = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(capacite));
         r.tarif =(float) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(tarif));
         r.placeDisponible = r.capacite;

	// recuperation de radio button 
        if(radioTarifAjoutParking == 1){


          strcpy(r.Type_tarifaire,"horaire");
        }else if(radioTarifAjoutParking == 2){

          strcpy(r.Type_tarifaire,"journalier");
       }

        // recuperation de check box 
      if(checkSecuriteAjoutParking == 1){
          strcpy(r.securite_disponible,"oui");
      }else if(checkSecuriteAjoutParking ==2){
          strcpy(r.securite_disponible,"non");
      }

      // controlle saisie 
        if(strcmp(r.id,"")==0 ){
            
           b=0;
             }


           if(strcmp(r.nom,"")==0 ){
           b=0;
             }
            if(strcmp(r.adresse,"")==0 ){
            
           b=0;
             }
	/*	 if(strcmp(r.Type_tarifaire,"")==0 ){
            
           b=0;
             }

             	 if(strcmp(r.securite_disponible,"")==0 ){
            
           b=0;
             }*/
     
    
          
        //  ajouter et logique de programme 

          if (b == 0){
         

               gtk_label_set_text(GTK_LABEL(labelControl),"  ⚠️ Erreur : Veuillez vérifier vos données et réessayer. ");



         }else{
           if(isExistParkingById("parkings.txt",r.id)){
              gtk_label_set_text(GTK_LABEL(labelControl),"  ⚠️ Id existe deja ... ");

	   }else{

         
       int t =  ajouterParking("parkings.txt",r);
       if(t == 1){
          pInfo = gtk_message_dialog_new(GTK_WINDOW(user_data),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"✅ Votre élément a été ajouté avec succès !");
      switch(gtk_dialog_run(GTK_DIALOG(pInfo))){
               case GTK_RESPONSE_OK:
               gtk_widget_destroy(pInfo);
                  gtk_entry_set_text(GTK_ENTRY(id),"");
                      gtk_entry_set_text(GTK_ENTRY(nom),"");
			
                   gtk_label_set_text(GTK_LABEL(labelControl),"");
                
          		 
              
             windowParkingHome = create_windowParkingHome();
             gtk_widget_show(windowParkingHome);
              gtk_widget_destroy(windowParkingAjout);
                 
       
               break;


          }
       }else{

   


         gtk_label_set_text(GTK_LABEL(labelControl),"❌ Une erreur est survenue lors de l'ajout de l'élément. Veuillez réessaye  ");

      }

   }
}



        
}


void
on_btnCancelAjoutParking_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
 GtkWidget *windowParkingAjout,*windowParkingHome;
     windowParkingHome = lookup_widget(button,"windowParkingHome");
      
     windowParkingAjout = lookup_widget(button,"windowParkingAjout");
     windowParkingHome = create_windowParkingHome();
     gtk_widget_show(windowParkingHome);
     gtk_widget_destroy(windowParkingAjout);
}


void
on_buttonParkinghercheRecModif_clicked (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *id ,*nom,*adresse,*capacite,*tarif,*horaire,*journalier,*oui,*non,*windowParkingModif,*windowParkingHome,*labelControl,*pInfo;
   // initialisation de varibale de controlle saisie
       int b = 1;
   // declaration de notre struct 
       Parking r;
  char id1[50];
     // declaration des variables 
        windowParkingHome = lookup_widget(button,"windowParkingHome");
 	windowParkingModif = lookup_widget(button,"windowParkingModif");
        
        id = lookup_widget(button,"textIdModifParking");
         nom = lookup_widget(button,"textNomModifParking");
         adresse = lookup_widget(button,"comboAdresseModifParking");
         capacite = lookup_widget(button,"spinCapaciteModifParking");
	tarif = lookup_widget(button,"spinTarifModifParking");
	labelControl = lookup_widget(button,"windowParkingModifControl");
        horaire = lookup_widget(button,"radioHoraireModifParking");
        journalier = lookup_widget(button,"radioJournalierModifParking");
        oui = lookup_widget(button,"checkSecOuiModifParking");
        non = lookup_widget(button,"checkSecNonModifParking");

                  // recuperation de textField
     strcpy(id1,gtk_entry_get_text(GTK_ENTRY(id)));


                // controlle saisie 
        if(strcmp(id1,"")==0 ){
            
           b=0;
             }


     
                   if(b==0){
            gtk_label_set_text(GTK_LABEL(labelControl),"  ⚠️ Erreur : Veuillez vérifier vos données et réessayer. ");
          }else{
          if(!isExistParkingById(filenameParking,id1)){
              gtk_label_set_text(GTK_LABEL(labelControl),"  ⚠️ Id Introuvable , il n existe pas ... ");

	   }else{
		r = chercherParkingById(filenameParking,id1);
                
               gtk_entry_set_text(GTK_ENTRY(id),r.id);
		  gtk_entry_set_text(GTK_ENTRY(nom),r.nom);
		gtk_combo_box_set_active(GTK_COMBO_BOX(adresse),1);
                gtk_spin_button_set_value(capacite, r.capacite);
                gtk_spin_button_set_value(tarif,r.tarif);
              
                  
                 

		// Set Radio Button Value
                if(strcmp(r.Type_tarifaire,"horaire")==0){
		gtk_toggle_button_set_active(GTK_RADIO_BUTTON(horaire),TRUE);
		}else if(strcmp(r.Type_tarifaire,"journalier")==0){
		gtk_toggle_button_set_active(GTK_RADIO_BUTTON(journalier),TRUE);
		}
		
            
		 // Set check Button Value
                   if(strcmp(r.securite_disponible,"oui")==0){
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(oui),TRUE);
		}else if(strcmp(r.securite_disponible,"non")==0){
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(non),TRUE);
		}

               
           }

}
         
}


void
on_checkSecNonModifParking_toggled     (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if (gtk_toggle_button_get_active(togglebutton))
 {checkSecuriteModifParking=2;}
}


void
on_checkSecOuiModifParking_toggled     (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if (gtk_toggle_button_get_active(togglebutton))
 {checkSecuriteModifParking=1;}
}


void
on_radioHoraireModifParking_toggled    (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if (gtk_toggle_button_get_active(togglebutton))
 {radioTarifModifParking=1;}
}


void
on_radioJournalierModifParking_toggled (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if (gtk_toggle_button_get_active(togglebutton))
 {radioTarifModifParking=2;}
}


void
on_btnModifParking_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *id ,*nom,*adresse,*capacite,*tarif,*windowParkingModif,*windowParkingHome,*labelControl,*pInfo;
   // initialisation de varibale de controlle saisie
       int b = 1;
   // declaration de notre struct 
       Parking r;
     // declaration des variables 
        windowParkingHome = lookup_widget(button,"windowParkingHome");
 	windowParkingModif = lookup_widget(button,"windowParkingModif");
        
        id = lookup_widget(button,"textIdModifParking");
         nom = lookup_widget(button,"textNomModifParking");
         adresse = lookup_widget(button,"comboAdresseModifParking");
         capacite = lookup_widget(button,"spinCapaciteModifParking");
	tarif = lookup_widget(button,"spinTarifModifParking");
	labelControl = lookup_widget(button,"windowParkingModifControl");
           // recuperation des information d apres interfaces graphique 
            // recuperation de textField
    	 strcpy(r.id,gtk_entry_get_text(GTK_ENTRY(id)));
         strcpy(r.nom,gtk_entry_get_text(GTK_ENTRY(nom)));
          // recuperation de combobox
     	strcpy(r.adresse,gtk_combo_box_get_active_text(GTK_COMBO_BOX(adresse)));
        // recuperation de spinButton
    	 r.capacite = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(capacite));
         r.tarif =(float) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(tarif));
         r.placeDisponible = r.capacite;

	// recuperation de radio button 
        if(radioTarifModifParking == 1){


          strcpy(r.Type_tarifaire,"horaire");
        }else if(radioTarifModifParking == 2){

          strcpy(r.Type_tarifaire,"journalier");
       }

        // recuperation de check box 
      if(checkSecuriteModifParking == 1){
          strcpy(r.securite_disponible,"oui");
      }else if(checkSecuriteModifParking ==2){
          strcpy(r.securite_disponible,"non");
      }

      // controlle saisie 
        if(strcmp(r.id,"")==0 ){
            
           b=0;
             }
           if(strcmp(r.nom,"")==0 ){
           b=0;
             }
            if(strcmp(r.adresse,"")==0 ){
            
           b=0;
             }
	/*	 if(strcmp(r.Type_tarifaire,"")==0 ){
            
           b=0;
             }

             	 if(strcmp(r.securite_disponible,"")==0 ){
            
           b=0;
             }*/
     
        //  ajouter et logique de programme 

       
          if (b == 0){
         

               gtk_label_set_text(GTK_LABEL(labelControl),"  ⚠️ Erreur : Veuillez vérifier vos données et réessayer. ");



         }else{
           if(!isExistParkingById(filenameParking,r.id)){
              gtk_label_set_text(GTK_LABEL(labelControl)," ⚠️ Id Introuvable , il n existe pas ... ");

	   }else{

         
       int t =  modifierParking(filenameParking,r.id,r);
       if(t == 1){
          pInfo = gtk_message_dialog_new(GTK_WINDOW(user_data),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"✅ Votre élément a été modifié avec succès !");
      switch(gtk_dialog_run(GTK_DIALOG(pInfo))){
               case GTK_RESPONSE_OK:
               gtk_widget_destroy(pInfo);
                  gtk_entry_set_text(GTK_ENTRY(id),"");
                       gtk_entry_set_text(GTK_ENTRY(nom),"");
			
                   gtk_label_set_text(GTK_LABEL(labelControl),"");
                
          		 
              
             windowParkingHome = create_windowParkingHome();
             gtk_widget_show(windowParkingHome);
              gtk_widget_destroy(windowParkingModif);
                 
       
               break;


          }
       }else{

   


         gtk_label_set_text(GTK_LABEL(labelControl),"❌ Une erreur est survenue lors de l'ajout de l'élément. Veuillez réessaye  ");

      }

   }
}
}


void
on_btnCancelModifParking_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
     GtkWidget *windowParkingModif,*windowParkingHome;
     windowParkingHome = lookup_widget(button,"windowParkingHome");
      
     windowParkingModif = lookup_widget(button,"windowParkingModif");
     windowParkingHome = create_windowParkingHome();
     gtk_widget_show(windowParkingHome);
     gtk_widget_destroy(windowParkingModif);
}


void
on_btnAffectationParking_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
 GtkWidget *idPar,*idAge ,*windowParkingAffectation,*windowParkingHome,*labelControl,*pInfo;
    // initialisation de varibale de controlle saisie
       int b = 1;
	char idParking[50];
	char idAgent[50];
      
       // declaration des variables 
 	windowParkingHome = lookup_widget(button,"windowParkingHome");
 	windowParkingAffectation = lookup_widget(button,"windowParkingAffectation");
 	idPar = lookup_widget(button,"comboIdParkingAffectParking");
 	idAge = lookup_widget(button,"comboIdAgentAffectParking");
 	labelControl = lookup_widget(button,"textIdAffectationParkingControl");
        // recuperation des donne
         strcpy(idParking,gtk_combo_box_get_active_text(GTK_COMBO_BOX(idPar)));
         strcpy(idAgent,gtk_combo_box_get_active_text(GTK_COMBO_BOX(idAge)));

           // controlle saisie 
        if(strcmp(idParking,"")==0 ){
            
           b=0;
             }

        if(strcmp(idAgent,"")==0 ){
            
           b=0;
             }
         
         //  ajouter et logique de programme 

          if (b == 0){
         

               gtk_label_set_text(GTK_LABEL(labelControl),"  ⚠️  Veuillez vérifier vos données ");



         }else{
     
       int t =  affecterAagent1(filenameAgentParking1,idAgent,idParking);
       if(t == 1){
          pInfo = gtk_message_dialog_new(GTK_WINDOW(user_data),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"✅ Votre élément a été Suppression avec succès !");
      switch(gtk_dialog_run(GTK_DIALOG(pInfo))){
               case GTK_RESPONSE_OK:
               gtk_widget_destroy(pInfo);
                
                     
                   gtk_label_set_text(GTK_LABEL(labelControl),"");
                
          		 
              
             windowParkingHome = create_windowParkingHome();
             gtk_widget_show(windowParkingHome);
              gtk_widget_destroy(windowParkingAffectation);
                 
       
               break;


          }
       }else{

   


         gtk_label_set_text(GTK_LABEL(labelControl),"❌ Une erreur est survenue  ");

      }
}


}


void
on_btnCancelAffectationParking_clicked (GtkButton       *button,
                                        gpointer         user_data)
{
 GtkWidget *windowParkingAffectation,*windowParkingHome;
     windowParkingHome = lookup_widget(button,"windowParkingHome");
      
     windowParkingAffectation = lookup_widget(button,"windowParkingAffectation");
     windowParkingHome = create_windowParkingHome();
     gtk_widget_show(windowParkingHome);
     gtk_widget_destroy(windowParkingAffectation);
}


void
on_buttonFilterParking_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_CheckSecNonAjoutParking_toggled     (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}





void on_buttonRetourGestionServices_clicked(GtkButton *button, gpointer user_data) {
    // Récupérer la fenêtre principale (mainWindow) et la fenêtre Choix
    GtkWidget *main_window = lookup_widget(GTK_WIDGET(button), "mainWindow");  // La fenêtre principale
    GtkWidget *choix_window = lookup_widget(GTK_WIDGET(button), "Choix");  // La fenêtre Choix

    // Si la fenêtre Choix n'existe pas encore, créez-la
    if (choix_window == NULL) {
        choix_window = create_Choix();  // Crée la fenêtre Choix si elle n'existe pas encore
    }

    // Vérification finale que la fenêtre Choix est valide
    if (choix_window == NULL) {
        g_warning("Erreur : La fenêtre Choix est introuvable.");
        return;
    }

    // Cacher la fenêtre mainWindow
    gtk_widget_hide(main_window);

    // Afficher la fenêtre Choix
    gtk_widget_show(choix_window);
}


void
on_buttonRetourAuthentication_clicked  (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_buttonRetourGestionAgents_clicked   (GtkButton       *button,
                                        gpointer         user_data)
{

}

//////////////////////////////////////////CITOYEN///////////////////////////////

void on_buttonChercherCitoyen_clicked(GtkButton *button, gpointer user_data) {
    // Récupérer le widget GtkEntry qui contient l'ID du service
    GtkWidget *entryIDCitoyen = lookup_widget(button, "entryIDCitoyen");  // Assurez-vous que le nom correspond à l'ID de votre GtkEntry dans l'interface
    const char *idCitoyenStr = gtk_entry_get_text(GTK_ENTRY(entryIDCitoyen));  // Récupérer l'ID du service en tant que chaîne
    int idCitoyen = atoi(idCitoyenStr);  // Convertir l'ID en entier

    // Chercher le service avec l'ID dans le fichier
    Citoyen c = chercherCitoyen("citoyen.txt", idCitoyen);  // Remplacer "citoyen.txt" par le nom du fichier que tu utilises pour stocker les services

    // Si le service est trouvé
    if (c.id != -1) {
        // Créer un message de dialogue pour afficher les détails du service
        GtkWidget *dialog = gtk_dialog_new_with_buttons("Détails du Citoyen",  // Titre de la fenêtre
                                                       GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                                       GTK_DIALOG_MODAL,
                                                       GTK_STOCK_OK, GTK_RESPONSE_OK,
                                                       NULL);

        // Récupérer le contenu du dialogue pour y ajouter les détails
        GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

        // Créer un box vertical pour organiser les widgets (labels)
        GtkWidget *vbox = gtk_vbox_new(FALSE, 10);
        gtk_container_add(GTK_CONTAINER(content_area), vbox);

        // Afficher le nom
        GtkWidget *labelNom = gtk_label_new(c.nom);
        gtk_box_pack_start(GTK_BOX(vbox), labelNom, FALSE, FALSE, 0);

        // Afficher la description
        GtkWidget *labelPrenom = gtk_label_new(c.prenom);
        gtk_box_pack_start(GTK_BOX(vbox), labelPrenom, FALSE, FALSE, 0);
	
	GtkWidget *labelAdress = gtk_label_new(c.adress);
        gtk_box_pack_start(GTK_BOX(vbox), labelAdress, FALSE, FALSE, 0);

        // Afficher le tarif
        char tarifStr[20];
        snprintf(tarifStr, sizeof(tarifStr), "%.2f", c.age);
        GtkWidget *labelTarif = gtk_label_new(tarifStr);
        gtk_box_pack_start(GTK_BOX(vbox), labelTarif, FALSE, FALSE, 0);

        // Afficher le dialogue
        gtk_widget_show_all(dialog);

        // Attendre que l'utilisateur clique sur le bouton OK pour fermer la fenêtre
        gtk_dialog_run(GTK_DIALOG(dialog));

        // Fermer le dialogue
        gtk_widget_destroy(dialog);
    } else {
        // Si le service n'est pas trouvé, afficher un message d'erreur sous forme de boîte de dialogue
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_ERROR,
                                                  GTK_BUTTONS_OK,
                                                  "Citoyen non trouvé.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);  // Fermer la boîte de dialogue après avoir cliqué sur OK
    }
}


////////////////////////////////////////////////////////////////////////////////////////
// Inclure pour accéder à la fonction afficher_Facture

void on_buttonAfficherFacture_clicked(GtkButton *button, gpointer user_data) {
    // Récupérer la fenêtre principale (Citoyen) et la fenêtre TREEVIEW
    GtkWidget *citoyen_window = lookup_widget(GTK_WIDGET(button), "Citoyen");  // La fenêtre "Citoyen"
    GtkWidget *facture_window = lookup_widget(GTK_WIDGET(button), "Facture");  // La fenêtre contenant le TreeView

    // Si la fenêtre Facture n'existe pas encore, créez-la
    if (facture_window == NULL) {
        facture_window = create_Facture();  // Crée la fenêtre Facture si elle n'existe pas encore
    }

    // Vérification finale que la fenêtre Facture est valide
    if (facture_window == NULL) {
        g_warning("Erreur : La fenêtre Facture est introuvable.");
        return;
    }

    // Récupérer le TreeView de la fenêtre Facture
    GtkWidget *treeviewFacture = lookup_widget(facture_window, "treeviewFacture");

    // Appel de la fonction pour afficher les factures dans le TreeView
    afficher_Facture(treeviewFacture);

    // Cacher la fenêtre Citoyen
    gtk_widget_hide(citoyen_window);

    // Afficher la fenêtre Facture
    gtk_widget_show(facture_window);
}


///////////////////////////////////////////////////////////////////////////////////////////
void on_buttonRetourCitoyen_clicked(GtkButton *button, gpointer user_data) {
    // Récupérer la fenêtre "Citoyen" et la fenêtre "Choix"
    GtkWidget *window_citoyen = lookup_widget(GTK_WIDGET(button), "Citoyen");
    GtkWidget *choix_window = lookup_widget(GTK_WIDGET(button), "Choix");

    // Si la fenêtre Choix n'existe pas encore, créez-la
    if (choix_window == NULL) {
        choix_window = create_Choix();  // Crée la fenêtre Choix si elle n'existe pas encore
    }

    // Vérification finale que la fenêtre Choix est valide
    if (choix_window == NULL) {
        g_warning("Erreur : La fenêtre Choix est introuvable.");
        return;
    }

    // Cacher la fenêtre "Citoyen"
    gtk_widget_hide(window_citoyen);

    // Afficher la fenêtre "Choix"
    gtk_widget_show(choix_window);
}


void on_buttonQuitterCitoyen_clicked(GtkButton *button, gpointer user_data) {
    // Fermer la fenêtre "Citoyen"
    GtkWidget *window_citoyen = lookup_widget(GTK_WIDGET(button), "Citoyen");

    // Vérifier si la fenêtre existe
    if (window_citoyen != NULL) {
        gtk_widget_destroy(window_citoyen);  // Fermer la fenêtre
    }

    // Quitter l'application GTK
    gtk_main_quit();  // Met fin à la boucle principale et ferme l'application
}

///////////////////////////////////////////////////////////////

int ajouterCitoyen(const char *filename, Citoyen nouveauCitoyen);

int existeIDC(const char *filename, int id) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        return 0;  // Erreur d'ouverture du fichier
    }

    Citoyen citoyen;
    while (fscanf(f, "%d %s %s %s %d %s\n", &citoyen.id, citoyen.nom, citoyen.prenom, citoyen.adress, &citoyen.age, citoyen.sexe) == 7) {
        if (citoyen.id == id) {
            fclose(f);
            return 1;  // ID déjà existant
        }
    }

    fclose(f);
    return 0;  // ID n'existe pas
}
/////////////////

void on_buttonAjouterCitoyen_clicked(GtkButton *button, gpointer user_data) {
    // Récupérer les entrées dans les champs GtkEntry
    GtkWidget *entryIDAjout = lookup_widget(GTK_WIDGET(button), "entryIDAjout");
    GtkWidget *entryNom = lookup_widget(GTK_WIDGET(button), "entryNom");
    GtkWidget *entryPrenom = lookup_widget(GTK_WIDGET(button), "entryPrenom");
    GtkWidget *spinbutton2 = lookup_widget(GTK_WIDGET(button), "spinbutton2");
    GtkWidget *combobox5 = lookup_widget(GTK_WIDGET(button), "combobox5");

    // Récupération des boutons radio pour le sexe
    GtkWidget *radiobuttonHomme = lookup_widget(GTK_WIDGET(button), "radiobuttonHomme");
    GtkWidget *radiobuttonFemme = lookup_widget(GTK_WIDGET(button), "radiobuttonFemme");

    // Lire les valeurs des champs
    const gchar *idText = gtk_entry_get_text(GTK_ENTRY(entryIDAjout));
    const gchar *nomText = gtk_entry_get_text(GTK_ENTRY(entryNom));
    const gchar *prenomText = gtk_entry_get_text(GTK_ENTRY(entryPrenom));
    int ageValue = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton2));
    const gchar *adresseText = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox5));

    // Lire le sexe
    const char *sexe;
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobuttonHomme))) {
        sexe = "Homme";
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobuttonFemme))) {
        sexe = "Femme";
    } else {
        sexe = "";
    }

    // Vérifier si tous les champs sont remplis
    if (strlen(idText) == 0 || strlen(nomText) == 0 || strlen(prenomText) == 0 || strlen(adresseText) == 0 || ageValue <= 0 || strlen(sexe) == 0) {
        GtkWidget *message = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Tous les champs doivent être remplis.");
        gtk_dialog_run(GTK_DIALOG(message));
        gtk_widget_destroy(message);
        return;
    }

    // Remplir la structure Citoyen
    Citoyen nouveauCitoyen;
    nouveauCitoyen.id = atoi(idText);
    strncpy(nouveauCitoyen.nom, nomText, sizeof(nouveauCitoyen.nom));
    strncpy(nouveauCitoyen.prenom, prenomText, sizeof(nouveauCitoyen.prenom));
    strncpy(nouveauCitoyen.adress, adresseText, sizeof(nouveauCitoyen.adress)); // Vérifiez le nom "adress" ou "adresse"
    nouveauCitoyen.age = ageValue;
    strncpy(nouveauCitoyen.sexe, sexe, sizeof(nouveauCitoyen.sexe));


    // Vérifier si l'ID existe déjà
    const char *filename = "citoyen.txt";
    if (existeIDC(filename, nouveauCitoyen.id)) {
        GtkWidget *message = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "L'ID existe déjà. Veuillez saisir un autre ID.");
        gtk_dialog_run(GTK_DIALOG(message));
        gtk_widget_destroy(message);
        return;
    }

    // Ajouter le citoyen au fichier
    if (ajouterCitoyen(filename, nouveauCitoyen)) {
        GtkWidget *message = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Citoyen ajouté avec succès !");
        gtk_dialog_run(GTK_DIALOG(message));
        gtk_widget_destroy(message);

        // Réinitialiser les champs
        gtk_entry_set_text(GTK_ENTRY(entryIDAjout), "");
        gtk_entry_set_text(GTK_ENTRY(entryNom), "");
        gtk_entry_set_text(GTK_ENTRY(entryPrenom), "");
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton2), 0);
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox5), -1);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobuttonHomme), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobuttonFemme), FALSE);
    } else {
        GtkWidget *message = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Erreur lors de l'ajout du citoyen.");
        gtk_dialog_run(GTK_DIALOG(message));
        gtk_widget_destroy(message);
    }
}


/////////////////////////////////////////////////////////////////

void on_radiobuttonFemme_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
   
}


void
on_radiobuttonHomme_toggled            (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{ 

}

//////////////////////////////////////////////////////////////////
void
on_radiobuttonFemmeModifier_toggled    (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_radiobuttonHommeModifier_toggled    (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}
///////////////////////////

// Fonction pour charger les IDs depuis le fichier


// Fonction appelée lors du clic sur le bouton "Initialiser"
void on_buttonInitialiserCitoyen_clicked(GtkButton *button, gpointer user_data) {
     // Le GtkComboBox où on va ajouter les IDs
    char ids[MAX_IDS][50]; // Tableau pour stocker les IDs
    int id_count, i;

    // Associer le GtkComboBox à la variable
    GtkWidget *comboboxIDCitoyenModifier = lookup_widget(button, "comboboxIDCitoyenModifier"); // Ici, 'user_data' devrait être le GtkComboBox que tu passes lors de l'appel
     int n_items = gtk_combo_box_get_model(GTK_COMBO_BOX(comboboxIDCitoyenModifier)) ? gtk_tree_model_iter_n_children(gtk_combo_box_get_model(GTK_COMBO_BOX(comboboxIDCitoyenModifier)), NULL) : 0;
    for (i = 0; i < n_items; i++) {
        gtk_combo_box_remove_text(GTK_COMBO_BOX(comboboxIDCitoyenModifier), 0);  // Supprimer chaque élément
    }

    // Charger les IDs depuis le fichier
    id_count = load_ids_from_file("IDC.txt", ids);

    if (id_count == 0) {
        g_print("Aucun ID chargé.\n");
        return;
    }

    

    // Ajouter les IDs dans le GtkComboBox
    for (i = 0; i < id_count; i++) {
        gtk_combo_box_append_text(GTK_COMBO_BOX(comboboxIDCitoyenModifier), ids[i]);
    }
}

/////////////////////////////////////////////////
void on_ButtonModifierCitoyen_clicked(GtkButton *button, gpointer user_data) {
    // Récupération des widgets
    GtkWidget *comboboxIDCitoyenModifier = lookup_widget(GTK_WIDGET(button), "comboboxIDCitoyenModifier");
    GtkWidget *entryNom = lookup_widget(GTK_WIDGET(button), "entry31");
    GtkWidget *entryPrenom = lookup_widget(GTK_WIDGET(button), "entry32");
    GtkWidget *comboboxAdresse = lookup_widget(GTK_WIDGET(button), "combobox7");
    GtkWidget *spinbuttonAge = lookup_widget(GTK_WIDGET(button), "spinbutton3");
    GtkWidget *radiobuttonHomme = lookup_widget(GTK_WIDGET(button), "radiobuttonHommeModifier");
    GtkWidget *radiobuttonFemme = lookup_widget(GTK_WIDGET(button), "radiobuttonFemmeModifier");
    GtkWidget *checkbuttonVerification = lookup_widget(GTK_WIDGET(button), "checkbuttonVerification");

    // Récupération des données des widgets
    const gchar *id_selected = gtk_combo_box_get_active_text(GTK_COMBO_BOX(comboboxIDCitoyenModifier));
    if (id_selected == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez sélectionner un ID valide.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    int id = atoi(id_selected);

    const gchar *nom = gtk_entry_get_text(GTK_ENTRY(entryNom));
    const gchar *prenom = gtk_entry_get_text(GTK_ENTRY(entryPrenom));
    const gchar *adresse = gtk_combo_box_get_active_text(GTK_COMBO_BOX(comboboxAdresse));
    int age = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbuttonAge));
    gboolean est_verifie = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbuttonVerification));

    const gchar *sexe;
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobuttonHomme))) {
        sexe = "Homme";
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobuttonFemme))) {
        sexe = "Femme";
    } else {
        sexe = "";
    }

    // Validation des champs
    if (strlen(nom) == 0 || strlen(prenom) == 0 || adresse == NULL || strlen(sexe) == 0 || age <= 0 || !est_verifie) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Tous les champs doivent être remplis et la vérification doit être cochée.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Création de l'objet Citoyen
    Citoyen nouv;
    nouv.id = id;
    strncpy(nouv.nom, nom, sizeof(nouv.nom));
    strncpy(nouv.prenom, prenom, sizeof(nouv.prenom));
    strncpy(nouv.adress, adresse, sizeof(nouv.adress)); // Vérifiez le champ "adress" dans votre structure
    nouv.age = age;
    strncpy(nouv.sexe, sexe, sizeof(nouv.sexe));

    // Mise à jour dans le fichier
    int resultat = modifierCitoyen("citoyen.txt", id, nouv);

    if (resultat) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_INFO,
                                                   GTK_BUTTONS_OK,
                                                   "Citoyen mis à jour avec succès.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        // Réinitialiser les champs
        gtk_entry_set_text(GTK_ENTRY(entryNom), "");
        gtk_entry_set_text(GTK_ENTRY(entryPrenom), "");
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbuttonAge), 0);
        gtk_combo_box_set_active(GTK_COMBO_BOX(comboboxAdresse), -1);
        gtk_combo_box_set_active(GTK_COMBO_BOX(comboboxIDCitoyenModifier), -1);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobuttonHomme), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobuttonFemme), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbuttonVerification), FALSE);
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Erreur lors de la mise à jour du citoyen.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}


////////////////////////////////////////////////////////
void
on_checkbuttonVerification_toggled     (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}

////////////////////////////////////////////////////////

void on_buttonInitialiserIDSuprimer_clicked (GtkButton *button, gpointer user_data) {
     // Le GtkComboBox où on va ajouter les IDs
    char ids[MAX_IDS][50]; // Tableau pour stocker les IDs
    int id_count, i;

    // Associer le GtkComboBox à la variable
    GtkWidget *comboboxIDCitoyenSupprimer = lookup_widget(button, "comboboxIDCitoyenSupprimer"); // Ici, 'user_data' devrait être le GtkComboBox que tu passes lors de l'appel
     int n_items = gtk_combo_box_get_model(GTK_COMBO_BOX(comboboxIDCitoyenSupprimer)) ? gtk_tree_model_iter_n_children(gtk_combo_box_get_model(GTK_COMBO_BOX(comboboxIDCitoyenSupprimer)), NULL) : 0;
    for (i = 0; i < n_items; i++) {
        gtk_combo_box_remove_text(GTK_COMBO_BOX(comboboxIDCitoyenSupprimer), 0);  // Supprimer chaque élément
    }

    // Charger les IDs depuis le fichier
    id_count = load_ids_from_file("IDC.txt", ids);

    if (id_count == 0) {
        g_print("Aucun ID chargé.\n");
        return;
    }

    

    // Ajouter les IDs dans le GtkComboBox
    for (i = 0; i < id_count; i++) {
        gtk_combo_box_append_text(GTK_COMBO_BOX(comboboxIDCitoyenSupprimer), ids[i]);
    }
}

////////////////////////////////////////////////////////////////


void
on_buttonAnnulerCitoyen_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{

}


static int citoyen_a_supprimer_id = -1;

// Fonction appelée lors du clic sur le CheckButton (remplaçant des radiobuttons OUI et NON)
void on_checkbuttonVerifierSupprimer_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    // Vérifier si le CheckButton est coché
    if (gtk_toggle_button_get_active(togglebutton)) {
        suppression_confirmee = TRUE;  // La suppression est confirmée
        g_print("Suppression confirmée.\n");
    } else {
        suppression_confirmee = FALSE;  // La suppression est annulée
        g_print("Suppression annulée.\n");
    }
}

// Fonction appelée lors du clic sur le bouton "Supprimer"
void on_buttonSupprimerCitoyen_clicked(GtkButton *button, gpointer user_data) {
    // Récupérer l'ID du service à supprimer depuis combobox2
    GtkWidget *comboboxIDCitoyenSupprimer = lookup_widget(GTK_WIDGET(button), "comboboxIDCitoyenSupprimer");
    const char *id_selected = gtk_combo_box_get_active_text(GTK_COMBO_BOX(comboboxIDCitoyenSupprimer));

    if (id_selected != NULL) {
        service_a_supprimer_id = atoi(id_selected);  // Convertir l'ID en entier
    }

    // Vérifier si la suppression est confirmée
    if (suppression_confirmee && service_a_supprimer_id != -1) {
        // Appeler une fonction pour supprimer le service
        int resultat = supprimerCitoyen("citoyen.txt", service_a_supprimer_id);
        if (resultat) {
            g_print("Citoyen supprimé avec succès.\n");

            // Afficher un message de confirmation
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                      GTK_DIALOG_MODAL,
                                                      GTK_MESSAGE_INFO,
                                                      GTK_BUTTONS_OK,
                                                      "Citoyen supprimé avec succès.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        } else {
            // Afficher un message d'erreur si la suppression échoue
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                      GTK_DIALOG_MODAL,
                                                      GTK_MESSAGE_ERROR,
                                                      GTK_BUTTONS_OK,
                                                      "Erreur lors de la suppression du citoyen.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        }
    } else if (!suppression_confirmee) {
        // Afficher un message si la suppression n'est pas confirmée
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_WARNING,
                                                  GTK_BUTTONS_OK,
                                                  "Suppression annulée. Veuillez confirmer la suppression.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        // Afficher un message d'erreur si aucun service n'est sélectionné
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_ERROR,
                                                  GTK_BUTTONS_OK,
                                                  "Veuillez sélectionner un citoyen à supprimer.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}


void on_buttonRetournerFacture_clicked(GtkButton *button, gpointer user_data) {
    // Fermer la fenêtre "Facture"
    GtkWidget *window_facture = lookup_widget(GTK_WIDGET(button), "Facture");
    if (window_facture != NULL) {
        gtk_widget_destroy(window_facture);  // Fermer la fenêtre "Facture"
    }

    // Ouvrir la fenêtre "Citoyen"
    GtkWidget *window_citoyen = lookup_widget(GTK_WIDGET(button), "Citoyen");
    if (window_citoyen != NULL) {
        gtk_widget_show(window_citoyen);  // Afficher la fenêtre "Citoyen"
    }
}



void on_buttonActualiserFacture_clicked(GtkButton *button, gpointer user_data) {
    // Récupérer le treeviewFacture dans la fenêtre "Facture"
    GtkWidget *treeview_facture;
    treeview_facture = lookup_widget(GTK_WIDGET(button), "treeviewFacture");

    // Actualiser le contenu du treeviewFacture en appelant la fonction afficher_facture
    afficher_Facture("facture.txt");
}





void on_treeviewFacture_row_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
    GtkTreeIter iter;
    int id_facture;
    int nbjour;
    float montant;
    Facture f; // Structure pour stocker les données de la facture

    GtkTreeModel *model = gtk_tree_view_get_model(treeview);

    // Si l'itérateur correspond à la ligne sélectionnée, on récupère les données
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        gtk_tree_model_get(GTK_LIST_STORE(model), &iter, 0, &id_facture, 1, &nbjour, 2, &montant, -1);

        // Copier les valeurs récupérées dans la structure f
        f.idf = id_facture;
        f.njour = nbjour;
        f.montant = montant;

        // Affichage des données récupérées (vous pouvez les utiliser autrement)
        g_print("Facture ID: %d\n", f.idf);
        g_print("Nombre de jours: %d\n", f.njour);
        g_print("Montant: %.2f\n", f.montant);

        // Vous pouvez appeler d'autres fonctions pour traiter ces données, 
        // par exemple pour afficher une fenêtre avec plus de détails, etc.
    }
}

