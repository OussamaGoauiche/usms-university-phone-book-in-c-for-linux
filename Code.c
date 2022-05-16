#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include<time.h>
#include <ctype.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/param.h>
#include <fcntl.h>
#include<string.h>
struct personne
{
	char * nom;
	char *prenom;		
	char *numero;	
};
typedef struct personne personne;


void Affichage_Arbitraire(const char *repertoire)
{
	DIR* dir;
	struct dirent *entree;
	dir=opendir(repertoire);
	if(dir==NULL)
	return;
	fprintf(stdout,"%s : \n",repertoire);
	while((entree=readdir(dir))!=NULL)
	{
	fprintf(stdout,">Inode %ld|",entree->d_ino);
	fprintf(stdout,"nom %-25s|",entree->d_name);
	fprintf(stdout,"longueur %d|",entree->d_reclen);
	fprintf(stdout,"type: %u\n",entree->d_type);
	}
	closedir(dir);

}

void Affichage_trie(const char *repertoire)
{

struct dirent ** nameList;
int n,i;
n=scandir(repertoire,&nameList,NULL,alphasort);
if (n < 0) {
		fprintf(stderr,"Echec\n");
		exit(EXIT_FAILURE);
	}


for (i = 0; i < n; i++) {
		struct dirent *entree= nameList[i];
		 fprintf(stdout,">nom %-25s|",entree->d_name);
		fprintf(stdout,"Inode %ld|",entree->d_ino);
	        fprintf(stdout,"longueur %d|",entree->d_reclen);
	        fprintf(stdout,"type: %u\n",entree->d_type);
		}
for (i = 0; i < n; i++) {
		free(nameList[i]);
	}
free(nameList);
}
void List_contenue_repertoire(const char *repertoire,int choix)
{	
	if(choix==0)
	Affichage_Arbitraire(repertoire);
	else 
	Affichage_trie(repertoire);
}
void Ajouter_sRepertoire(const char *repertoire)
{
	if(mkdir(repertoire,0777)!=0)
	{
	perror("");
	return;
	}
	else
	{
	FILE *files[2];
       for (int i = 0; i < 2; i++)
       {
       char filename[20];
       sprintf(filename, "%s/Fichier%d.txt",repertoire,i);
       files[i] = fopen(filename, "a");
       }
       }
}
       
void supprimer(const char *nom)
{

char cmd[32] = { 0 };

    int ret = 0;

    sprintf(cmd, "rm -rf %s", nom);

    ret = system(cmd);

    if (ret == 0)
        printf("succe\n");
    else
        printf("Echec  %s\n", nom);

}

void Ajouter(const char *file,personne p)
{
	FILE *fp;
        fp= fopen(file, "a");
	fputs(p.nom,fp);
	fputs(";",fp);
	fputs(p.prenom,fp);
	fputs(";",fp);
	fputs(p.numero,fp);
	fputs("\0",fp);
	fputs("\n",fp);
	fclose(fp);

}
int Recherche(const char *file,const char *wrd)
{
	char  buffer[256],tmp[20];
	int n, m, i, j, k,d=0,line=0;
	FILE* fp;
	fp = fopen(file, "r");
	m = strlen(wrd); 
	line = 0;
	while (fgets(buffer, 256, fp) != NULL) {
		i = 0;
		n = strlen(buffer);	
		while (i < n) {
             j = 0;
			while (i < n && j < m && buffer[i] == wrd[j]) {
				++i, ++j;
			}
			if ((buffer[i] ==';') && j == m) {
				printf("%s;",wrd);
				k=0;
				i++;
				while(buffer[i]!='\n'){
				tmp[k]=buffer[i];
				i++;
				k++;
				}
				printf("%s\n",tmp);	
			}
			while (i < n && buffer[i] !=';') {
				++i;
			}
			++i;
		}
		++line;
	}
         fclose(fp);
         return line-1;
}
int RechercheLine(const char *file,const char *wrd)
{
	char  buffer[256],tmp[20];
	int n, m, i, j, k,d=0,line=0;
	FILE* fp;
	fp = fopen(file, "r");
	m = strlen(wrd); 	
	line = 0;
	while (fgets(buffer, 256, fp) != NULL) {
		i = 0;
		n = strlen(buffer);
		while (i < n) {
            j = 0;
			while (i < n && j < m && buffer[i] == wrd[j]) {
				++i, ++j;
			}
			if ((buffer[i] ==';') && j == m) {
				        return line;
			}
			while (i < n && buffer[i] !=';') {
				++i;
			}
			++i;

		}
		++line;
	}
         fclose(fp);
       
}
int Modifier(const char *file,const char *wrd)
{
        int MAX=256;
        FILE *fptr1, *fptr2;
        int lno, linectr = 0;
        char str[MAX],fname[MAX];        
        char newln[MAX], temp[] = "temp.txt";	
        fptr1 = fopen(file, "r");
        if (!fptr1) 
        {
                printf("Echec ouverture fichier!!\n");
                return 0;
        }
        fptr2 = fopen(temp, "w");
        if (!fptr2) 
        {
                printf("Echec ouverture fichier temporaire!!\n");
                fclose(fptr1);
                return 0;
        }
        printf(" Nouvelle Entrer ");
        scanf("%s",newln);
        lno=RechercheLine(file,wrd);
        lno++;
        while (!feof(fptr1)) 
        {
            strcpy(str, "\0");
            fgets(str, MAX, fptr1);
            if (!feof(fptr1)) 
            {
                linectr++;
                if (linectr != lno) 
                    {
                        fprintf(fptr2, "%s", str);
                    } 
                    else 
                    {
                        fprintf(fptr2, "%s", newln);
                    }
                }
        }
        fclose(fptr1);
        fclose(fptr2);
        remove(file);
        rename(temp,file);
        printf(" succÃ©e!! \n");
}

int main(int argc,char *argv[]){
	personne p;
	int MAX=256;
	int n,choix;
        char *repertoire,*nom,*prenom,*numero,*word;
        repertoire=(char*)malloc(sizeof(char));
        nom=(char*)malloc(sizeof(char));
        prenom=(char*)malloc(sizeof(char));
        numero=(char*)malloc(sizeof(char));
        word=(char*)malloc(sizeof(char));
        char*file=(char*)malloc(sizeof(char));

    printf("!!!!!!Bienvenue dans le repertoire d'universite sultan moulay sliman!!!!!!");
    printf("\ntaper 1 pour Affichage arbitraire");
    printf("\ntaper 2 pour Affichage trie");
    printf("\ntaper 3 pour Lister le contenu du repertoire");
    printf("\ntaper 4 pour Ajouter un sous repertoire contenant deux fichiers txt");
    printf("\ntaper 5 pour Supprimer un repertoire ou un fichier donnï¿½");
    printf("\ntaper 6 pour ajouter une personne d un fichier donnï¿½");
    printf("\ntaper 7 pour Rechercher les informations d une entree donnee dans un fichier specifique");
    printf("\ntaper 8 pour modifier les informations d une entree donnee dans un fichier specifique\n\n");
    printf("entrer votre choix");
    scanf("%d",&n);
    
    switch(n)
    {
    	        case 1:
    	        printf("entrer le chemin de repertoire\n");
		scanf("%s",repertoire);	
		Affichage_Arbitraire(repertoire);
		break;
		case 2:
		printf("entrer le chemin de repertoire\n");
		scanf("%s",repertoire);
		Affichage_trie(repertoire);
		break;
		case 3:
		printf("entrer le chemin de repertoire\n");
	        scanf("%s",repertoire);
		printf("entrer une choix || 0 ou autre choix\n");
		scanf("%d",&choix);
		List_contenue_repertoire(repertoire,choix);
		break;
		case 4:
		printf("entrer le chemin de repertoire\n");
		scanf("%s",repertoire);
		Ajouter_sRepertoire(repertoire);
		break;
		case 5:
		printf("entrer le chemin de repertoire ou fichier\n");
		scanf("%s",file);
		supprimer(file);
		break;
		case 6:
		printf("entrer le chemin du fichier ou vous voulez ajouter ce perrsonne\n");
		scanf("%s",file);
		printf("entrer le nom de ce personne\n");
		scanf("%s",nom);
		p.nom=nom;
		printf("entrer le prenom de ce personne\n");
		scanf("%s",prenom);
		p.prenom=prenom;
		printf("entrer le numero de ce personne\n");
		scanf("%s",numero);
		p.numero=numero;
		Ajouter(file,p);
		break;
		case 7:	
		printf("entrer le chemin du fichier ou vous voulez faire le recherche\n");
		scanf("%s",file);
		printf("donner le mot a chercher");
		scanf("%s",word);
		Recherche(file,word);
		break;
		case 8:
		printf("entrer le chemin du fichier ou vous voulez faire la modification\n");
		scanf("%s",file);
		printf("donner le nom a  modifier\n");
		scanf("%s",word);
		Modifier(file,word);
		break;
			
    		
	}
           return 0;

}

