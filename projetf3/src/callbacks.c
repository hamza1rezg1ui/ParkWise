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

        // Créer et afficher la fenêtre principale (mainWindow)
        GtkWidget *mainWindow = create_mainWindow();
        gtk_widget_show(mainWindow);
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


