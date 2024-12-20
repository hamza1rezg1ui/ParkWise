#include <gtk/gtk.h>


void
on_buttonChercherService_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonSupprimer_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonAfficherTout_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonModifier_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonAjouter_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_Ajouter2_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_radiobutton75DT_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbuttonVerifier_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton25DT_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton50DT_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_Modifier2_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_radiobuttonOUI_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton5_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobuttonNON_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_buttonSupprimer2_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonAnnuler_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_Intialiser_clicked                  (GtkButton       *button,
                                        gpointer         user_data);
void remplir_combobox_avec_ids(GtkComboBoxEntry *combobox); 

void
on_radiobutton12_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton11_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobuttonOUI_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobuttonNON_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_buttonSupprimer_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonAnnuler_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_Initialiser2_clicked                (GtkButton       *button,
                                        gpointer         user_data);
void on_window_show(GtkWidget *window, gpointer user_data); 

void
on_checkbutton3_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_AfficherTout_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonConnect_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonActualiser_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonRetour_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_treeview_row_activated              (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data);

void
on_buttonChargerS_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonChargerR_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonVoirDetail_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonAffecter_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonQuitter_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonCtoyen_clicked  (GtkWidget *objet_graphique, gpointer user_data); 

void
on_buttonAdmin_clicked  (GtkWidget *objet_graphique, gpointer user_data);

/////////////////////////////Gestion de Parking/////////////////////////////////////////////


///////////////////////********************Gestion Agents /////////////////////////////////////
void
on_treeview1_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data);

void
on_btngotoaj_clicked                   (GtkWidget *objet_graphique, gpointer user_data);

void
on_btngotomodif_clicked                (GtkWidget *objet_graphique, gpointer user_data);

void
on_gotosupp_clicked                    (GtkWidget *objet_graphique, gpointer user_data);

void
on_actualiser_clicked                  (GtkWidget *objet_graphique, gpointer user_data);

void
on_checkbuttonconfaj_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_buttonrtaj_clicked                  (GtkWidget *objet_graphique, gpointer user_data);

void
on_btnajagent_clicked                  (GtkWidget *objet_graphique, gpointer user_data);

void
on_radiobuttonhommeaj_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobuttonfemmeaj_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_buttonrechidmodif_clicked           (GtkWidget *objet_graphique, gpointer user_data);

void
on_radiobuttonfemmodif_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobuttonhommemodif_toggled       (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_btnmodifieragent_clicked            (GtkWidget *objet_graphique, gpointer user_data);

void
on_buttonrtmodi_clicked                (GtkWidget *objet_graphique, gpointer user_data);

void
on_buttonrtsupp_clicked                (GtkWidget *objet_graphique, gpointer user_data);

void
on_buttonsupprimeagent_clicked         (GtkWidget *objet_graphique, gpointer user_data);



void
on_buttonGestionaireParking_clicked    (GtkWidget *objet_graphique, gpointer user_data); 

void
on_buttonGestionaireAgents_clicked     (GtkWidget *objet_graphique, gpointer user_data); 

void
on_buttonGestionaireServices_clicked (GtkWidget *objet_graphique, gpointer user_data); 

void
on_buttonRetourGestionServices_clicked (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonRetourAuthentication_clicked  (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonRetourGestionParking_clicked  (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonRetourGestionAgents_clicked   (GtkButton       *button,
                                        gpointer         user_data);
////////////////////////////////////////////////////////////////////////




void
on_treeviewParkingHome_row_activated   (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data);

void
on_buttonAjouterParkingW_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonModifierParkingW_clicked      (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonSupprimerParkingW_clicked     (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonActualiserParkingW_clicked    (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonStatistiqueParkingW_clicked   (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonRechercheParking_clicked      (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonFiltreParking_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonReturnMenuParking_clicked     (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnSupParking_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnCancelSupParking_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_checkSecNonAjoutParking_toggled     (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkSecOuiAjoutParking_toggled     (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radioHoraireAjoutParking_toggled    (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radioJournalierAjoutParking_toggled (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_btnAjoutParking_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnCancelAjoutParking_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonParkinghercheRecModif_clicked (GtkButton       *button,
                                        gpointer         user_data);

void
on_checkSecNonModifParking_toggled     (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkSecOuiModifParking_toggled     (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radioHoraireModifParking_toggled    (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radioJournalierModifParking_toggled (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_btnModifParking_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnCancelModifParking_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnAffectationParking_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnCancelAffectationParking_clicked (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonFilterParking_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_CheckSecNonAjoutParking_toggled     (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkSecNonModifParking_toggled     (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_buttonRetourGestionServices_clicked (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonRetourAuthentication_clicked  (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonRetourGestionAgents_clicked   (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonChercherCitoyen_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonAfficherFacture_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonRetourCitoyen_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonQuitterCitoyen_clicked        (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonAjouterCitoyen_clicked        (GtkButton       *button,
                                        gpointer         user_data);

void
on_radiobuttonFemme_toggled            (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobuttonHomme_toggled            (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobuttonFemmeModifier_toggled    (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobuttonHommeModifier_toggled    (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_buttonInitialiserCitoyen_clicked    (GtkButton       *button,
                                        gpointer         user_data);

void
on_ButtonModifierCitoyen_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_checkbuttonVerification_toggled     (GtkToggleButton *togglebutton,
                                        gpointer         user_data);


void
on_checkbuttonVerifierSupprimer_toggled
                                        (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_buttonAnnulerCitoyen_clicked        (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonSupprimerCitoyen_clicked      (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonRetournerFacture_clicked      (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonActualiserFacture_clicked     (GtkButton       *button,
                                        gpointer         user_data);




void
on_buttonInitialiserIDSuprimer_clicked (GtkButton       *button,
                                        gpointer         user_data);

void
on_treeviewFacture_row_activated       (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data);
