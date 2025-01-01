#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <windows.h>

char mot[25],mot2[25],error[51];
int i,j,pendu=0,difficulty,taille_max,hints,coef_score,taille,error_count=0,gamemode;
char c;
int fin=0; char v;

typedef struct{
	char pseudo_nom[50];
	int score;	
	int match_jouer;
	int gagne;
	int perdu;
	float taux_de_reussite;
}joueur;

struct detail_mot{
	char indice[100];
	int aidable;
};
struct detail_mot detail;

void title(char titre[], char sous_titre[]){
	printf("\n\n\n\n\t\t\t\t\t ---------------------------------\n");	
	printf("\n\n\t\t\t\t\t               %s          \n",titre);
	printf("\n\n\t\t\t\t\t ---------------------------------\n");
	printf("\n\n\t\t\t\t\t     %s",sous_titre);	
}

int pseudo_nom_existe(char nom_joueur1[], FILE *mon_fichier1){
	joueur personne1;
	mon_fichier1=fopen("filependu.txt","r");
	while(!feof(mon_fichier1))
			{
				fscanf(mon_fichier1,"%s",&personne1.pseudo_nom);
				if(strcmp(nom_joueur1,personne1.pseudo_nom)==0)
				{	
					system("cls");
					printf("\n\n\t\t\t\t    pardon, ce joueur d%cja il existe, r%cssayer\n", 130, 130);
					return(1);
				}
			}
}

void echange_score(FILE *mon_fichier_e, char nom_joueur11[], int score_total, int match_jouer1, int match_gagne1, int match_perdu1, int pourcentage){
	FILE *file;
	joueur personne11;
	personne11.match_jouer=match_jouer1;
	personne11.gagne=match_gagne1;
	personne11.perdu=match_perdu1;
	personne11.taux_de_reussite=pourcentage;
	file=fopen("fichier_temp.txt","a");
	fprintf(file,"%s %d %d %d %d %f\n", nom_joueur11, score_total, personne11.match_jouer, personne11.gagne, personne11.perdu, personne11.taux_de_reussite);
	fclose(file);
	mon_fichier_e=fopen("filependu.txt","r");	
	file=fopen("fichier_temp.txt","a");
	while(!feof(mon_fichier_e)){
		fscanf(mon_fichier_e,"%s %d %d %d %d %f",&personne11.pseudo_nom, &personne11.score, &personne11.match_jouer, &personne11.gagne, &personne11.perdu, &personne11.taux_de_reussite);
		if(strcmp(personne11.pseudo_nom,nom_joueur11)!=0){
			fprintf(file,"%s %d %d %d %d %f\n",personne11.pseudo_nom, personne11.score, personne11.match_jouer, personne11.gagne, personne11.perdu, personne11.taux_de_reussite);
		}
	}
	fclose(mon_fichier_e);
	fclose(file);
	remove("filependu.txt");
	rename("fichier_temp.txt","filependu.txt");
}

int trouver(FILE *fichier, char nom[])
{
	joueur person;
	fichier=fopen("filependu.txt","r");
		while(!feof(fichier))
		{
			fscanf(fichier,"%s %d %d %d %d %f",&person.pseudo_nom, &person.score, &person.match_jouer, &person.gagne, &person.perdu, &person.taux_de_reussite);
			if(strcmp(person.pseudo_nom,nom)==0)
			{
				fclose(fichier);
				return(1);
			}
		}
		return(0);
}

void Color(int couleurDuTexte,int couleurDeFond){
        HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(H,couleurDeFond*16+couleurDuTexte);
}

void clear_file(char nom_fich[],int value_def){
	FILE* f;
	f=fopen(nom_fich,"w");
	fprintf(f,"%d",value_def);
	fclose(f);
}

int random(int sup,int inf){
	int h, min, s, day, mois, an;
	time_t now;
	time(&now);
	struct tm *local = localtime(&now);
	min = local->tm_min;       
	s = local->tm_sec;
	an = local->tm_year;
	int randed=sup-(rand()*s)%sup;
	while(randed<inf) randed+=9;
	return randed;
}

int test_file(void){
	int var_test;
	FILE *f=fopen("apend.txt","r");
		fscanf(f,"%d",&var_test);
		fclose(f);
	if(var_test<16 && var_test >-1) return 1;
	else return 0;
}

void enregistrement(void){
	for(i=0;detail.indice[i]!='\0';i++)if(detail.indice[i]==' ') detail.indice[i]='_';
	if(error[0]==0 || error[0]=='\0') error[0]='_';
	FILE* fichier=fopen("apend.txt","w");
	//fprintf(fichier,"%d\n%d\n%s\n%s\n%s",gamemode,difficulty,mot,error,mot2);
	if(detail.indice[0]=='0' || detail.indice[0]=='\0') detail.indice[0]='_';
	fprintf(fichier,"%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%s\n%s\n%s\n%s",pendu,difficulty,taille_max,hints,coef_score,taille,error_count,gamemode,detail.aidable,detail.indice,mot,mot2,error);
	fclose(fichier);
	for(i=0;detail.indice[i]!='\0';i++)if(detail.indice[i]=='_') detail.indice[i]=' ';
	if(error[0]=='_') error[0]=0;
}

void chargement(void){
	FILE* fichier=fopen("apend.txt","r");
	fscanf(fichier,"%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%s\n%s\n%s\n%s",&pendu,&difficulty,&taille_max,&hints,&coef_score,&taille,&error_count,&gamemode,&detail.aidable,detail.indice,mot,mot2,error);
	fclose(fichier);
	for(i=0;detail.indice[i]!='\0';i++)if(detail.indice[i]=='_') detail.indice[i]=' ';
	if(error[0]=='_') error[0]=0;
}

void creer_mot2(void){
	for(i=0;i<taille;i++){
			mot2[i]=4; // 4: code ascii
		}
}

void proposer_mot(void){
	char indice_1[50]={"Ce mot est un fruit!"};
	char indice_2[50]={"Ce mot est un pays!"};
	char indice_3[50]={"Ce mot est une equipe de football!"};
	char indice_4[50]={"Ce mot est une propriete html/css!"};	
	char indice_5[50]={"Ce mot est un verbe du 1er groupe!"};
	char indice_6[50]={"Ce mot est un mobilier domestique!"};
	char file_name[30]={"Propositions//"};
	FILE *f;
	FILE *f2;
    char reader,m,mot_test[50];
    int line,line_count,trouve=0;
    res:;
    m=random(6,1);
	//on ajoute le nom et l'extension du fichier au file_name pour l'appeler:
    file_name[14]=m+48;
    file_name[15]='.';
    file_name[16]='t';
    file_name[17]='x';
    file_name[18]='t';
    f=fopen(file_name,"r");
   		 //on compte le nombre de mots(lignes) contenues dans le fichier:
   		while((reader=fgetc(f))!=EOF){
    		if(reader=='\n') line_count++;
  		}
    fclose(f);
    //on choisis une ligne aleatoire comme mot[]:
    f=fopen(file_name,"r");
    	line=random(line_count,0);
		for(i=0;i<=line;i++) fscanf(f,"%s\n",mot);
    fclose(f);
     //on verifie si le mot est deja posé:
    f=fopen("Propositions//Rep.txt","r");
    	for(i=0;!feof(f);i++){
    		fscanf(f,"%s",mot_test);
    		if(strcmp(mot,mot_test)==0)	goto res;
		}
    fclose(f);
     //on ajoute le mot dans la liste des mots deja posés:	
 	f=fopen("Propositions//Rep.txt","a");
 	fprintf(f,"%s\n",mot);
	fclose(f);
  	taille = strlen(mot);
   	creer_mot2();
   	    //l'indice varie selon le fichier selectionnée:
    switch(m){
    	case 1:for(i=0;i<strlen(indice_1);i++) detail.indice[i]=indice_1[i];detail.aidable=1;break;
    	case 2:for(i=0;i<strlen(indice_2);i++) detail.indice[i]=indice_2[i];detail.aidable=1;break;
    	case 3:for(i=0;i<strlen(indice_3);i++) detail.indice[i]=indice_3[i];detail.aidable=1;break;
    	case 4:for(i=0;i<strlen(indice_4);i++) detail.indice[i]=indice_4[i];detail.aidable=1;break;
    	case 5:for(i=0;i<strlen(indice_5);i++) detail.indice[i]=indice_5[i];detail.aidable=1;break;
    	case 6:for(i=0;i<strlen(indice_6);i++) detail.indice[i]=indice_6[i];detail.aidable=1;break;
	}
   	line_count=0;
   	f=fopen("Propositions//Rep.txt","r");
   		 //on compte le nombre de mots(lignes) contenues dans le fichier:
   		while((reader=fgetc(f))!=EOF){
    		if(reader=='\n') line_count++;
  		}
    fclose(f);
    if(line_count>290) clear_file("Propositions//Rep.txt",0);
}

void win(void){
	FILE *f;
    char reader;
    //on appele le fichier et on remplace les numeros (indices) par des code ascii convenables:
    f=fopen("interfaces//win.txt","r");
    while((reader=fgetc(f))!=EOF){
        switch(reader){
        	case '0':reader = ' ';break;
        	case '1':reader = 219;break;
        	case '2':reader = 220;break;
        	case '3':reader = 223;break;
        	case '4':reader = 205;break;
        	case '5':reader = 201;break;
        	case '6':reader = 186;break;
        	case '7':reader = 200;break;
        	case '8':reader = 187;break;
        	case '9':reader = 188;break;
		}
		printf("%c",reader);
    }
    fclose(f);
}

void dessin(int *pen){
	FILE *f;
    char reader;
    char file_name[15]={"Pendu//"};
    //on ajoute le nom et l'extension au file_name pour l'appeler:
    if(*pen>9){
		file_name[7]=49;
		file_name[8]=(*pen-10)+48;
    	file_name[9]='.';
    	file_name[10]='t';
    	file_name[11]='x';
    	file_name[12]='t';
	}
	else{
		file_name[7]=*pen+48;
		file_name[8]='.';
    	file_name[9]='t';
    	file_name[10]='x';
    	file_name[11]='t';
    	file_name[12]='\0';
	}
    f=fopen(file_name,"r");
     //on appele le fichier et on remplace les numeros (indices) par des code ascii convenables:
    while((reader=fgetc(f))!=EOF){
        switch(reader){
        	case '_':reader = 202;break;
        	case '|':reader = 186;break;
        	case '/':reader = 201;break;
        	case '-':reader = 205;break;
        	case 92 :reader = 203;break;
        	case 'O':reader = 2  ;break;
        	case '1':reader = 179;break;
        	case '2':reader = '\\';break;
        	case '3':reader = 47;break;
		}
		printf("%c",reader);
    }
    fclose(f);
}

void add_error(char er){
	int count;
	//on cherche si l'erreur 'er' existe deja au tableau error[] pour eviter les repetitions:
	for(i=0;error[i]!='\0';i++){
		if(error[i]==er){
			count++;
		}
	//si il n'existe pas on l'ajout à la derniere case vide:
	}
	if(count==0){
		error[error_count]=er;
		error_count++;
	}
}

int charger_mot(void){
	tag:;
	char char_test;
	int n;
	i=0;
 	while(i<100){
    	char_test=getch();
 		if(i>=taille_max && (char_test!='\r' && char_test!=8)){
 			system("cls");
 			title("\b\b\bJEU DE PENDU     ","\n");
 	    	printf("Ce mot depasse %d caracteres, reessayer:\n",taille_max);
 	    	for(j=0;j<taille_max;j++) mot[j]=0;
 	    	goto tag;
		}
		else{
		 	if(char_test=='\r' && i==0){
		 		system("cls");
		 		title("\b\b\bJEU DE PENDU    ","\n");
 				puts("Entrer un mot:");
 				goto tag;
 				break;
			}
    		else if(char_test=='\r' && i>0){
			 	break;
			}
			else if(char_test==8 && i>0){
		 		printf("\b%c\b",32);
		 		mot[i-1]='\0';
		 		i--;
			}
			else if(difficulty==1 && (char_test>='a' && char_test<='z')){
		 		mot[i]=char_test;
				n=rand();
		 		while(!(n%31<32 && n%31>14)) n=n+rand();
				printf("%c",n%31);
		 		i++;
			}
		 	else if((difficulty==2 || difficulty==3) && ((char_test>=65 && char_test<='Z') || (char_test>='a' && char_test<='z'))){
				mot[i]=char_test;
				n=rand();
		 		while(!(n%31<32 && n%31>14)) n=n+rand();
				printf("%c",n%31);
		 		i++;
			}
  		}
	}
	taille = strlen(mot); //strlen donne le nombre de characteres dans une chaine de characteres
	creer_mot2();
	return 1;		
}

int verify_letter(char letter){
	int boo=0;
	//on cherche l'emplacement de la lettre pour la remplacer (si il existe) et le nombre de repetitions 'boo' de la lettre:
	for(i=0;mot[i]!='\0';i++){
		if(mot[i]==letter){
			mot2[i]=mot[i];
			boo++;
		}
	}
	//on incremente le pendu et on l'affiche si la lettre n'est pas existante dans le mot(boo==0):
	if(boo==0){
		add_error(letter);
		pendu++;
		return 0;
	}
	else{
		return 1;
	}
}

void give_hint(void){
	int n=1;
	//on choisis aleatoirement une lettre cachée et on appele la fonction verify_letter pour s'occuper de la remplacer pour s'assurer
	//de la comparer avec tout le mot et on quitte la boucle:
	while(n>0){
		n = rand()%taille;
		if (mot2[n]==4){
			verify_letter(mot[n]);
			hints--;
			break;
		}
	}
}

int verify_mot(char tab1[],char tab2[]){
	// une fonction simple retourne 1 si les deux tableaux tab1 et tab2 sont identiques:
	if (strcmp(tab1,tab2) == 0){
		return 1;
	}
   else{
  	 	return 0;
   }
}

void set_gamemode(void){
	tegs:
	system("cls");
	title("\b\b\bJEU DE PENDU    ","\n");
	printf("choisir un mode de jeu en choisissant son num%cro: \n1.Contre l\'ordinateur \n2.Contre un(e) ami(e)\n",130);
	//le code ascii de 1 comme exemple est 49 alors on doit soustraire 48 pour arriver à l'entrée réele du joueur:
	gamemode=getch()-48;
	// ('&' - 48) équivalent à la touche 1 miniscule! (meme chose pour 2 et 3) 
	if(gamemode==1 || gamemode=='&'- 48){
		gamemode=1;
		taille_max=25;
		hints=2;
		coef_score=4;
		detail.aidable=1;
		proposer_mot();
	}
	else if(gamemode==2 || gamemode==130- 48){
		gamemode=0;
		detail.aidable=0;
		system("cls");
		title("\b\b\bJEU DE PENDU    ","\n");
		printf("choisir un niveau de difficult%c en choisissant son num%cro: \n1.D%cbutant \n2.Interm%cdiaire \n3.Expert \n",130,130,130,130);
		difficulty=getch()-48;
		system("cls");
		title("\b\b\bJEU DE PENDU    ","\n");
		if(difficulty==1 || difficulty=='&'-48){
			difficulty=1;
			taille_max=10;
			hints=2;
		}
		else if(difficulty==2 || difficulty==130-48){
			difficulty=2;
			taille_max=10;
			hints=1;
		}
		else if(difficulty==3 || difficulty==34-48){
			difficulty=3;
			taille_max=25;
			hints=0;
		}
		else{
			goto tegs;
		}
		coef_score=difficulty; 
		//ce coef sera multiplie par le nombre de tentatif restants a la fin pour donner le score du joueur en cas ou il gagne!
		printf("Votre adversaire a %d aide(s) possible(s), entrer un mot qui ne depasse pas %d lettres",hints,taille_max);
		if(difficulty == 1) puts(" tousses miniscules:");
		else{
			puts(":");
		} 
			charger_mot();
			system("cls");
			title("\b\b\bJEU DE PENDU    ","\n");
	}
	else{
		goto tegs;
	}
}

int pas_espace(char nom1_joueur[])
{
	int espace;
	for(espace=0; nom1_joueur[espace]!='\0'; espace++)
		{
			if(nom1_joueur[espace]==' ')
			{
				system("cls");
				printf("\n\n\t\t\t\t       le pseudo ne doit pas contenir l'espace\n");
				return(0);
			}
		}
	return(1);	
}


int main(int argc, char *argv[]) {
	system("title Jeu de pendu opéré par El mehdi Fathallah, Majida Khamlich et Aymane Laaroui");
	system("color f0");
	int x;
	FILE *mon_fichier;
	joueur personne;
	int choix_de_compte;
	char nom_joueur[50];
	int y;
	int match_jouer;
	int match_gagne;
	int match_perdu;
	float pourcentage_reussite;
	int nouveau_score;
	int cherche;
	
	personne.match_jouer=0;
	personne.gagne=0;
	personne.perdu=0;
	
	int pass;
	
	mon_fichier=fopen("filependu.txt","a");
	fclose(mon_fichier);
		debut:
	title("\b\b\bJEU DE PENDU    ","\n");
	printf("\n\t\t\t\t\tvous avez un compte? (oui(1) non(0))\n\n\t\t\t\t\t\t\t  ");
	choix_de_compte=getch()-48;
	system("cls");
	if(choix_de_compte!=0 && choix_de_compte!=1 && choix_de_compte!='&'-48 && choix_de_compte!='à'-48 ){
		system("cls");
		goto debut;
	}
	if(choix_de_compte==1 || choix_de_compte=='&'-48){
		espace1:
		title("COMPTE","donner votre pseudo-nom : ");
		gets(nom_joueur);
		pass=pas_espace(nom_joueur);
			if(pass==0)
			{
				goto espace1;
			}
		cherche=trouver(mon_fichier, nom_joueur);
		if(cherche==0)
		{	
			system("cls");
			printf("\n\n\t\t\t\t  Joueur introuvable ou le nom est incorr%ccte!!!!!\n",130);
			goto debut;	
		}
		else{
		mon_fichier=fopen("filependu.txt","r");
		while(!feof(mon_fichier))
		{
			fscanf(mon_fichier,"%s %d %d %d %d %f",&personne.pseudo_nom, &personne.score, &personne.match_jouer, &personne.gagne, &personne.perdu, &personne.taux_de_reussite);
			if(strcmp(personne.pseudo_nom,nom_joueur)==0)
			{
				system("cls");
				title("PROFILE","");
				printf("\n\n\t\t\t\t\t\tjoueur\t\t : %s\n\n\t\t\t\t\t\tscore\t\t : %d\n\n\t\t\t\t\t\tparties gagn%ces  : %d of %d\n\n\t\t\t\t\t\tparties perdu    : %d of %d\n\n\t\t\t\t\t\ttaux de r%cussite : %.0f %c\n\n",personne.pseudo_nom, personne.score, 130, personne.gagne, personne.match_jouer, personne.perdu, personne.match_jouer, 130, personne.taux_de_reussite, 37);
				break;
			}
		}
		fclose(mon_fichier);
		}
		getch();
		system("cls");
	}
	else{
			espace2:
			title("\b\b\b\b\bCREATION DE COMPTE","donner votre pseudo-nom : ");
			gets(nom_joueur);
			pass=pas_espace(nom_joueur);
			if(pass==0)
			{
				goto espace2;
			}
			y=pseudo_nom_existe(nom_joueur, mon_fichier);
			while(y==1)
			{
				title("\b\b\b\b\bCREATION DE COMPTE","donner votre pseudo-nom:");
				gets(nom_joueur);
				y=pseudo_nom_existe(nom_joueur, mon_fichier);
				if(y!=1)
				{
					break;
				}
			}		
	}
	system("cls");
	system("title Jeu de pendu opéré par El mehdi Fathallah, Majida Khamlich et Aymane Laaroui!");
	title("\b\b\bJEU DE PENDU    ","\n");
	printf("Bienvenu au jeu de pendu ");
	Color(4, 15);printf(" %c\n",3);Color(0, 15);
	system("PAUSE"); // equivalent à getch()
	//on definit bien le mode de jeu et la difficulte etc..
	if(test_file()==0){
		set_gamemode(); 	
	}
	else{
		rep:;
		printf("Cliquer sur 1 pour reprendre votre partie(ou 0 pour une nouvelle partie):");
		x=getch();
		if(x=='0' || x=='à'){
			clear_file("apend.txt",20);
			set_gamemode();
		}
		else if(x=='1' || x=='&'){
			chargement();
		}
		else{
			goto rep;
		}
	}
	while(fin!=1){
		system("cls");
		Color(0, 15);
		puts(mot2);
		//l'indice apparait si le pendu est superieur a 9 et le coef_score diminue par 1:
		if(detail.aidable==1 && hints<=1){
			Color(4, 15);
			puts(detail.indice);
			Color(0, 15);
		}
		else if(detail.aidable==1 && hints>1){
			puts("");
		}
		if(strlen(error)>0){
			Color(4, 15);
			printf("Vos tentatives faux:");
			for(i=0;error[i]!='\0';i++){
				printf("%c ",error[i]);
			}
			puts("");
			Color(0, 15);
		}
		else puts("");
		printf("Il vous reste %d fautes et %d aide(s)!\n",15-pendu,hints);
		if(hints!=0 && gamemode==0){
			printf("appuyer sur 1 pour vous donner une lettre(aide)\n");
		}
		else if(hints==2 && gamemode!=0){
			printf("appuyer sur 1 pour vous donner un indice\n");
		}
		else if(hints==1 && gamemode!=0){
			printf("appuyer sur 1 pour vous donner une lettre(aide)\n");
		}
		else puts("");
		puts("Devinez:");
		dessin(&pendu);
		// equivalent à scanf(c):
		c=getch();
		if(c=='\r' || c==8){
			continue;
		}
		else{
		if((c=='1' || c=='&') && hints>0 && gamemode==0){
//			coef_score--;
			give_hint();
		}
		else if((c=='1' || c=='&') && hints==2 && gamemode!=0){
			coef_score=3;
			hints=1;
		}
		else if((c=='1' || c=='&') && hints==1 && gamemode!=0){
			coef_score=2;
			give_hint();
		}	
		else if ((c=='1' || c=='&') && hints==0 && coef_score !=3){
			puts("Vous avez consommer tous vos aides!");
			c=getch();
		}
		
		else if ((c=='1' || c=='&') && hints==0 && coef_score ==3){
			puts("Pas d\'aide en mode expert!");
			c=getch();
		}
		else if((c>=97 && c<=122) || (c>=65 && c<='Z')) verify_letter(c);	
		}
		enregistrement();
		if(pendu==15){break;}
		fin=verify_mot(mot,mot2);
		//en cas pendu=15 on quite le while avec fin =0 sinon si pendu<15 on verifie si mot = mot2 si oui on sort du while avec fin=1
		// alors si on sort avec fin=0 le joueur a perdu sinon il a gagné
	}
	clear_file("apend.txt",20);
	if(choix_de_compte==0){	
		if(fin==1){
		system("cls");
		system("color f2");
		printf("%c %s %c\n",17,mot,16);
		puts("Felecitations, tu as gagne!\n");
		match_jouer=personne.match_jouer+1;
		match_gagne=personne.gagne+1;
		match_perdu=0;
		pourcentage_reussite=match_gagne*100/match_jouer;
		win();
		}
		else{
		system("cls");
		system("color f4");
		puts("Tu as perdu!");
		printf("Le mot: %s\n",mot);
		match_jouer=personne.match_jouer+1;
		match_perdu=personne.perdu+1;
		match_gagne=0;
		pourcentage_reussite=float(match_gagne*100)/match_jouer;
		dessin(&pendu);
		}
		printf("votre score de cette partie : %d\n",coef_score*(15-pendu));
		mon_fichier=fopen("filependu.txt","a");
		fprintf(mon_fichier,"%s %d %d %d %d %f\n", nom_joueur, coef_score*(15-pendu), match_jouer, match_gagne, match_perdu, pourcentage_reussite);
		fclose(mon_fichier);
		printf("score totale                : %d\n", coef_score*(15-pendu));
	}
	else{
		if(fin==1){
		system("cls");
		system("color f2");
		printf("%c %s %c\n",17,mot,16);
		puts("Felecitations, tu as gagne!\n");
		mon_fichier=fopen("filependu.txt","r");
			while(!feof(mon_fichier)){
				fscanf(mon_fichier,"%s %d %d %d %d %f", &personne.pseudo_nom, &personne.score, &personne.match_jouer, &personne.gagne, &personne.perdu, &personne.taux_de_reussite);
				if(strcmp(nom_joueur,personne.pseudo_nom)==0)
				{
					match_jouer=personne.match_jouer+1;
					match_gagne=personne.gagne+1;
					match_perdu=personne.perdu;
				}
			}
			fclose(mon_fichier);
			pourcentage_reussite=float(match_gagne*100)/match_jouer;
			win();
		}	
		else
		{
		system("cls");
		system("color f4");
		puts("Tu as perdu!");
		printf("Le mot: %s\n",mot);
			mon_fichier=fopen("filependu.txt","r");
			while(!feof(mon_fichier))
			{
				fscanf(mon_fichier,"%s %d %d %d %d %f", &personne.pseudo_nom, &personne.score, &personne.match_jouer, &personne.gagne, &personne.perdu, &personne.taux_de_reussite);
				if(strcmp(nom_joueur,personne.pseudo_nom)==0)
				{
					match_jouer=personne.match_jouer+1;
					match_perdu=personne.perdu+1;
					match_gagne=personne.gagne;
				}
			}
			fclose(mon_fichier);	
			pourcentage_reussite=float(match_gagne*100)/match_jouer;
			dessin(&pendu);
		}
		printf("votre score de cette partie : %d\n",coef_score*(15-pendu));
			mon_fichier=fopen("filependu.txt","r");
			while(!feof(mon_fichier))
			{
				fscanf(mon_fichier,"%s %d %d %d %d %f", &personne.pseudo_nom, &personne.score, &personne.match_jouer, &personne.gagne, &personne.perdu, &personne.taux_de_reussite);
				if(strcmp(nom_joueur,personne.pseudo_nom)==0)
				{
					printf("score totale                : %d\n",(coef_score*(15-pendu))+personne.score);
					break;
				}
			}
			fclose(mon_fichier);
			nouveau_score=(coef_score*(15-pendu))+personne.score;
			echange_score(mon_fichier, nom_joueur, nouveau_score, match_jouer, match_gagne, match_perdu, pourcentage_reussite);
	}
	getch();
	int rejouer;
	printf("cliquer sur 1 pour rejouer!");
	rejouer=getch();
	if(rejouer =='1' || rejouer =='&') system("START projet_test7.exe");
	return 0;
}
