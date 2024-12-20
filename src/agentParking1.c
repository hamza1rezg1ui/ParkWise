#include "agentParking1.h"

int ajouterAgent1(char *filename,agent1 agent){

  FILE * f=fopen(filename, "a");
    if(f!=NULL)
    {
        fprintf(f,"%s %s %s %s %s %s %s %s %s\n",agent.CIN,agent.nom,agent.prenom,agent.email,agent.sexe,agent.etatCivil,agent.ID_Parking,
                agent.num_tel,agent.horaires_travail
                );
        fclose(f);
        return 1;
    }
    else return 0;

}


int affecterAagent1(char *filename,char *idAgent,char *idParking){

int tr=0;
    agent1 agent;
    FILE * f=fopen(filename, "r");
    FILE * f2=fopen("nouv.txt", "w");
    if(f!=NULL && f2!=NULL)
    {
        while(fscanf(f,"%s %s %s %s %s %s %s %s %s\n",agent.CIN,agent.nom,agent.prenom,agent.email,agent.sexe,agent.etatCivil,agent.ID_Parking,
                agent.num_tel,agent.horaires_travail)!=EOF)
        {
            if(strcmp(agent.CIN,idAgent)==0)
            {
                fprintf(f2,"%s %s %s %s %s %s %s %s %s\n",agent.CIN,agent.nom,agent.prenom,agent.email,agent.sexe,agent.etatCivil,idParking,
                agent.num_tel,agent.horaires_travail);
                tr=1;
            }
            else
                fprintf(f2,"%s %s %s %s %s %s %s %s %s\n",agent.CIN,agent.nom,agent.prenom,agent.email,agent.sexe,agent.etatCivil,agent.ID_Parking,
                agent.num_tel,agent.horaires_travail);

        }
    }
    fclose(f);
    fclose(f2);
    remove(filename);
    rename("nouv.txt", filename);
    return tr;










}
