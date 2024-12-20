#include "gererParking.h"

enum{PARID,
     PARNOM,
     PARADRESSE,
     PARCAPACITE,
     PARPLACE,
     PARTARIF,
     PARTYPE,
     PARSECURITE,
     COLUMNS
      };



void afficherParkingByAdresse(GtkWidget *liste, char *filename,char *nom){

GtkCellRenderer *renderer; // afficheur de cellule (Cellule contient un texte, image, case à cocher)
      GtkTreeViewColumn *column; // visualisation des colonnes
       GtkTreeIter iter;          /**/
       GtkListStore *store;       // création du modèle de type liste

char capacite[50];
  char placeDisponible[50];
char tarif[50];

    store = NULL;
   Parking parking;
    FILE *f;
    store = gtk_tree_view_get_model(liste);
    if (store == NULL)
    {
        
	renderer=gtk_cell_renderer_text_new();                                                               // cellule contenant du texte
        column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", PARID, NULL); // création d'une colonne avec du texte
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);                                             // associer la colonne à l'afficheur (GtkTreeView)

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("NOM", renderer, "text", PARNOM, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("ADRESSE", renderer, "text", PARADRESSE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("CAPACITE", renderer, "text", PARCAPACITE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);
       
          renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("PLACE DISPO", renderer, "text", PARPLACE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

          renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("TARIF", renderer, "text", PARTARIF, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("TYPE", renderer, "text", PARTYPE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("SECURITE", renderer, "text", PARSECURITE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

       

       

        // La liste contient 7 colonnes de type chaine de caractères
    }
   store = gtk_list_store_new(COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,-1);


     


              f = fopen(filename, "r");

    if (f == NULL)
    {
        return;
    }
    else
    {
       
        while (fscanf(f,"%s %s %s %d %d %f %s %s\n",parking.id,parking.nom,parking.adresse,&parking.capacite,&parking.placeDisponible,
                &parking.tarif,parking.Type_tarifaire,parking.securite_disponible)!=EOF)
        {  
            
         if(strcmp(parking.adresse,nom)==0  ){
         sprintf(capacite,"%d",parking.capacite);
          sprintf(placeDisponible,"%d",parking.placeDisponible);
          sprintf(tarif,"%.2f",parking.tarif);
     
        





            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store,&iter,PARID,parking.id,PARNOM,parking.nom,PARADRESSE,parking.adresse,PARCAPACITE,capacite,PARPLACE,placeDisponible,PARTARIF,tarif,PARTYPE,parking.Type_tarifaire,

PARSECURITE,parking.securite_disponible,-1);}
        }
        fclose(f);
        gtk_tree_view_set_model(GTK_TREE_VIEW(liste), GTK_TREE_MODEL(store));
        g_object_unref(store);
    }


}


void afficherParkingByID(GtkWidget *liste, char *filename,char *nom){

GtkCellRenderer *renderer; // afficheur de cellule (Cellule contient un texte, image, case à cocher)
      GtkTreeViewColumn *column; // visualisation des colonnes
       GtkTreeIter iter;          /**/
       GtkListStore *store;       // création du modèle de type liste

char capacite[50];
  char placeDisponible[50];
char tarif[50];

    store = NULL;
   Parking parking;
    FILE *f;
    store = gtk_tree_view_get_model(liste);
    if (store == NULL)
    {
        
	renderer=gtk_cell_renderer_text_new();                                                               // cellule contenant du texte
        column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", PARID, NULL); // création d'une colonne avec du texte
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);                                             // associer la colonne à l'afficheur (GtkTreeView)

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("NOM", renderer, "text", PARNOM, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("ADRESSE", renderer, "text", PARADRESSE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("CAPACITE", renderer, "text", PARCAPACITE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);
       
          renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("PLACE DISPO", renderer, "text", PARPLACE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

          renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("TARIF", renderer, "text", PARTARIF, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("TYPE", renderer, "text", PARTYPE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("SECURITE", renderer, "text", PARSECURITE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

       

       

        // La liste contient 7 colonnes de type chaine de caractères
    }
   store = gtk_list_store_new(COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,-1);


     


              f = fopen(filename, "r");

    if (f == NULL)
    {
        return;
    }
    else
    {
       
        while (fscanf(f,"%s %s %s %d %d %f %s %s\n",parking.id,parking.nom,parking.adresse,&parking.capacite,&parking.placeDisponible,
                &parking.tarif,parking.Type_tarifaire,parking.securite_disponible)!=EOF)
        {  
            
if(strcmp(parking.id,nom)==0 || strcmp(parking.nom,nom)==0  ){
         sprintf(capacite,"%d",parking.capacite);
          sprintf(placeDisponible,"%d",parking.placeDisponible);
          sprintf(tarif,"%.2f",parking.tarif);
     
        





            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store,&iter,PARID,parking.id,PARNOM,parking.nom,PARADRESSE,parking.adresse,PARCAPACITE,capacite,PARPLACE,placeDisponible,PARTARIF,tarif,PARTYPE,parking.Type_tarifaire,

PARSECURITE,parking.securite_disponible,-1);}
        }
        fclose(f);
        gtk_tree_view_set_model(GTK_TREE_VIEW(liste), GTK_TREE_MODEL(store));
        g_object_unref(store);
    }


}


void afficherParking(GtkWidget *liste, char *filename){

GtkCellRenderer *renderer; // afficheur de cellule (Cellule contient un texte, image, case à cocher)
      GtkTreeViewColumn *column; // visualisation des colonnes
       GtkTreeIter iter;          /**/
       GtkListStore *store;       // création du modèle de type liste

char capacite[50];
  char placeDisponible[50];
char tarif[50];

    store = NULL;
   Parking parking;
    FILE *f;
    store = gtk_tree_view_get_model(liste);
    if (store == NULL)
    {
        
	renderer=gtk_cell_renderer_text_new();                                                               // cellule contenant du texte
        column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", PARID, NULL); // création d'une colonne avec du texte
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);                                             // associer la colonne à l'afficheur (GtkTreeView)

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("NOM", renderer, "text", PARNOM, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("ADRESSE", renderer, "text", PARADRESSE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("CAPACITE", renderer, "text", PARCAPACITE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);
       
          renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("PLACE DISPO", renderer, "text", PARPLACE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

          renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("TARIF", renderer, "text", PARTARIF, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("TYPE", renderer, "text", PARTYPE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("SECURITE", renderer, "text", PARSECURITE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

       

       

        // La liste contient 7 colonnes de type chaine de caractères
    }
   store = gtk_list_store_new(COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,-1);


     


              f = fopen(filename, "r");

    if (f == NULL)
    {
        return;
    }
    else
    {
       
        while (fscanf(f,"%s %s %s %d %d %f %s %s\n",parking.id,parking.nom,parking.adresse,&parking.capacite,&parking.placeDisponible,
                &parking.tarif,parking.Type_tarifaire,parking.securite_disponible)!=EOF)
        {  
            

         sprintf(capacite,"%d",parking.capacite);
          sprintf(placeDisponible,"%d",parking.placeDisponible);
          sprintf(tarif,"%.2f",parking.tarif);
     
        





            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store,&iter,PARID,parking.id,PARNOM,parking.nom,PARADRESSE,parking.adresse,PARCAPACITE,capacite,PARPLACE,placeDisponible,PARTARIF,tarif,PARTYPE,parking.Type_tarifaire,

PARSECURITE,parking.securite_disponible,-1);
        }
        fclose(f);
        gtk_tree_view_set_model(GTK_TREE_VIEW(liste), GTK_TREE_MODEL(store));
        g_object_unref(store);
    }


}

void viderParking(GtkWidget *liste){

GtkCellRenderer *renderer; // afficheur de cellule (Cellule contient un texte, image, case à cocher)
      GtkTreeViewColumn *column; // visualisation des colonnes
       GtkTreeIter iter;          /**/
       GtkListStore *store;       // création du modèle de type liste

char capacite[50];
  char placeDisponible[50];
char tarif[50];

    store = NULL;
   Parking parking;
    FILE *f;
    store = gtk_tree_view_get_model(liste);
    if (store == NULL)
    {
        
	renderer=gtk_cell_renderer_text_new();                                                               // cellule contenant du texte
        column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", PARID, NULL); // création d'une colonne avec du texte
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);                                             // associer la colonne à l'afficheur (GtkTreeView)

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("NOM", renderer, "text", PARNOM, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("ADRESSE", renderer, "text", PARADRESSE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("CAPACITE", renderer, "text", PARCAPACITE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);
       
          renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("PLACE DISPO", renderer, "text", PARPLACE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

          renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("TARIF", renderer, "text", PARTARIF, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("TYPE", renderer, "text", PARTYPE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("SECURITE", renderer, "text", PARSECURITE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

       

       

        // La liste contient 7 colonnes de type chaine de caractères
    }
   store = gtk_list_store_new(COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,-1);
}




int ajouterParking(char *filename,Parking parking){

   FILE * f=fopen(filename, "a");
    if(f!=NULL)
    {
        fprintf(f,"%s %s %s %d %d %f %s %s\n",parking.id,parking.nom,parking.adresse,parking.capacite,parking.placeDisponible,
                parking.tarif,parking.Type_tarifaire,parking.securite_disponible
                );
        fclose(f);
        return 1;
    }
    else return 0;

}


int modifierParking(char *filename,char *id,Parking nouv){
int tr=0;
    Parking parking;
    FILE * f=fopen(filename, "r");
    FILE * f2=fopen("nouv.txt", "w");
    if(f!=NULL && f2!=NULL)
    {
        while(fscanf(f,"%s %s %s %d %d %f %s %s\n",parking.id,parking.nom,parking.adresse,&parking.capacite,&parking.placeDisponible,
                &parking.tarif,parking.Type_tarifaire,parking.securite_disponible)!=EOF)
        {
            if(strcmp(parking.id,id)==0)
            {
                fprintf(f2,"%s %s %s %d %d %f %s %s\n",nouv.id,nouv.nom,nouv.adresse,nouv.capacite,nouv.placeDisponible,
                nouv.tarif,nouv.Type_tarifaire,nouv.securite_disponible);
                tr=1;
            }
            else
                fprintf(f2,"%s %s %s %d %d %f %s %s\n",parking.id,parking.nom,parking.adresse,parking.capacite,parking.placeDisponible,
                parking.tarif,parking.Type_tarifaire,parking.securite_disponible);

        }
    }
    fclose(f);
    fclose(f2);
    remove(filename);
    rename("nouv.txt", filename);
    return tr;




}

int supprimerParking(char *filename,char *id){

int tr=0;
    Parking parking;
    FILE * f=fopen(filename, "r");
    FILE * f2=fopen("nouv.txt", "w");
    if(f!=NULL && f2!=NULL)
    {
        while(fscanf(f,"%s %s %s %d %d %f %s %s\n",parking.id,parking.nom,parking.adresse,&parking.capacite,&parking.placeDisponible,
                &parking.tarif,parking.Type_tarifaire,parking.securite_disponible)!=EOF)
        {
            if(strcmp(parking.id,id)==0)
                tr=1;
            else
                fprintf(f2,"%s %s %s %d %d %f %s %s\n",parking.id,parking.nom,parking.adresse,parking.capacite,parking.placeDisponible,
                parking.tarif,parking.Type_tarifaire,parking.securite_disponible);
        }
    }
    fclose(f);
    fclose(f2);
    remove(filename);
    rename("nouv.txt", filename);
    return tr;



}


Parking chercherParkingById(char *filename,char *id){
   Parking parking;
    int tr=0;
    FILE * f=fopen(filename, "r");
    if(f!=NULL)
    {
        while(tr==0&& fscanf(f,"%s %s %s %d %d %f %s %s\n",parking.id,parking.nom,parking.adresse,&parking.capacite,&parking.placeDisponible,
                &parking.tarif,parking.Type_tarifaire,parking.securite_disponible)!=EOF)
        {
            if(strcmp(parking.id,id)==0)
                tr=1;
        }
    }
    fclose(f);
    if(tr==0)
        strcpy(parking.id,"-1");
    return parking;



}


int isExistParkingById(char *filename,char *id){


   Parking parking;
    int tr=0;
    FILE * f=fopen(filename, "r");
    if(f!=NULL)
    {
        while(tr==0&& fscanf(f,"%s %s %s %d %d %f %s %s\n",parking.id,parking.nom,parking.adresse,&parking.capacite,&parking.placeDisponible,
                &parking.tarif,parking.Type_tarifaire,parking.securite_disponible)!=EOF)
        {
            if(strcmp(parking.id,id)==0)
                tr=1;
        }
    }
    fclose(f);

    return tr;




}


int listerParking(char *filename, Parking *resultat){

Parking parking;

     int i = 0;
      FILE * f=fopen(filename, "r");
        if(f!=NULL)
    {
        while( fscanf(f,"%s %s %s %d %d %f %s %s\n",parking.id,parking.nom,parking.adresse,&parking.capacite,&parking.placeDisponible,
                &parking.tarif,parking.Type_tarifaire,parking.securite_disponible)!=EOF)
        {

             resultat[i] = parking;
               i++;

        }
    }
    fclose(f);
    return i;




}
