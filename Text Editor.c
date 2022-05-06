#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

void touch (Dir* parent, char* name) {

	File* connectionFile = parent->head_children_files;
	while (connectionFile != NULL){
		if (strcmp(connectionFile->name,name) == 0 ) {
			printf("File already exists\n");
			return;
		}
		connectionFile = connectionFile->next;
	}
	File* new_file = (File *) malloc(sizeof(File));
	new_file->name = malloc((strlen(name) + 1) * sizeof(char));
	new_file->parent = parent;
	new_file->next = NULL;
	strcpy(new_file->name,name);
	File *current = parent->head_children_files;
    if(current == NULL){
        parent->head_children_files = new_file;
    	}
    else{
        while(current -> next != NULL)
        	current = current -> next;		
    	current -> next = new_file;
    	}		
}
	
void mkdir (Dir* parent, char* name) {	
	Dir* connectionDir = parent->head_children_dirs;
	while (connectionDir != NULL){
		if (strcmp(connectionDir->name,name) == 0 ) {
			printf("Directory already exists\n");
			return;
		}
		connectionDir = connectionDir->next;
	}

	Dir* new_dir = (Dir *) malloc(sizeof(Dir));
	new_dir -> name = malloc((strlen(name) + 1) * sizeof(char));
	new_dir -> next = NULL;
	strcpy(new_dir->name,name);
	new_dir -> parent = parent;
	new_dir -> head_children_files = NULL;
	new_dir -> head_children_dirs = NULL;
   	Dir *current = parent->head_children_dirs;
    if(current == NULL){
        parent->head_children_dirs = new_dir;
    	}
    else{
        while(current -> next != NULL){
        	current = current -> next;
        	}
    	current -> next = new_dir;
    	}		
}
	
void ls (Dir* parent) {

	Dir* connectionDir = parent->head_children_dirs;
	File* connectionFile = parent->head_children_files;
	while  (connectionDir != NULL) {
		printf("%s\n", connectionDir->name);
		connectionDir = connectionDir->next;
	}
	while (connectionFile != NULL) {
		printf("%s\n", connectionFile->name);
		connectionFile = connectionFile->next;
	}
}

void rm (Dir* parent, char* name) {
	File* connectionFile = parent->head_children_files;
	int logic = 0;
	while (connectionFile != NULL) {
		if (strcmp(connectionFile->name, name) == 0) {
			logic = 1;
			break;
		}
		connectionFile = connectionFile->next;
	}
	connectionFile = parent->head_children_files;
	while (connectionFile != NULL) {
		if (strcmp(connectionFile->name, name) == 0) {
			logic = 1;
			if  (parent->head_children_files == connectionFile){ //primul nod
				parent->head_children_files = connectionFile->next;
				free(connectionFile->name);
				free(connectionFile);
				}

			else if (parent->head_children_files != connectionFile && connectionFile->next != NULL){
				File* temp = parent->head_children_files ;
				while (temp->next != connectionFile){
					temp = temp->next;
				}
				temp->next = connectionFile->next;
				free(connectionFile->name);
				free(connectionFile);
			}
			
			else if (connectionFile->next == NULL && parent->head_children_files != connectionFile) {
				File* temp = parent->head_children_files ;
				while (temp->next != connectionFile){
					temp = temp->next;
				}
				temp->next = NULL;
				free(connectionFile->name);
				free(connectionFile);
			}
		return;
		}
	connectionFile = connectionFile->next;
	}
	if (!logic)
		printf("Could not find the file\n");


}

void rmdir (Dir* parent, char* name) {

Dir* connectionDir = parent->head_children_dirs;

	while (connectionDir != NULL ){
		if (strcmp(connectionDir->name, name) == 0 ) {
			while (connectionDir->head_children_dirs != NULL){
				//printf("abcdef");
				rmdir(connectionDir,connectionDir->head_children_dirs->name);
				//printf("abcdef");
			}
			while (connectionDir->head_children_files != NULL){
				rm(connectionDir,connectionDir->head_children_files->name);
			}

			if  (parent->head_children_dirs == connectionDir){ //primul nod
				parent->head_children_dirs = connectionDir->next;
				free(connectionDir->name);
				free(connectionDir);
		
				}

			else if (parent->head_children_dirs != connectionDir && connectionDir->next != NULL){
				Dir* temp = parent->head_children_dirs ;
				while (temp->next != connectionDir){
					temp = temp->next;
				}
				temp->next = connectionDir->next;
				free(connectionDir->name);
				free(connectionDir);
			}
			
			else if (connectionDir->next == NULL && parent->head_children_dirs != connectionDir) {
				Dir* temp = parent->head_children_dirs ;
				while (temp->next != connectionDir){
					temp = temp->next;
				}
				temp->next = NULL;
				free(connectionDir->name);
				free(connectionDir);
			}
      		return;
		}
		connectionDir = connectionDir->next;
	}
}

void cd(Dir** target, char *name) {

	Dir* connectionDir = (*target)->head_children_dirs;
	int token = 0;
	while (connectionDir != NULL){
		if (strcmp(connectionDir->name,name) == 0) {
		
			(*target) = connectionDir;
			token = 1 ;
		}
		connectionDir = connectionDir->next;
	}
	if (strcmp(name,"..") == 0) {
			
			if ( strcmp((*target)->name,"home") == 0 ) 
				return ;
			(*target) = (*target)->parent;
			token = 1 ;	
		}
	if (token == 0)
		printf("No directories found!\n"); 
}

void tree (Dir* target, int level) {

	Dir* connectionDir = target;
	File* connectionFile = target->head_children_files;
	int spatii = level;
	int copiespatii = level;

	if (connectionDir == NULL)
		return;
	else
	{
		if (connectionDir->head_children_dirs != NULL) {

			connectionDir = connectionDir->head_children_dirs;

			if (level != 0 ){
				printf("\n");
				} 

			while (copiespatii != 0){
				printf("    ");
				copiespatii--;
				}
			
			printf("%s" , connectionDir->name);
			level++;

			tree(connectionDir,level);
			
			printf("\n");
			spatii = (level-1)*4 ;

			if ((connectionFile) != NULL){
				while (spatii !=0 ){
					printf(" ");
					spatii--;
					}
				printf("%s" , connectionFile->name);
				}

			if ((connectionDir->next) != NULL )
				printf("%s" , connectionDir->next->name);

			}
		else
			return;
	}
}   

char *pwd (Dir* target) {

	char* result = (char*) malloc(MAX_INPUT_LINE_SIZE);	
	

	if  (target->head_children_dirs == NULL )
		strcpy(result,"/home");

	if (target->parent != NULL ) {
		strcat(result,pwd(target->parent));
		strcat(result,"/");
		strcat(result,target->name);
		}

	return result;
}

void stop (Dir* target) {
	while(target->parent)
		target = target->parent;
	while(target->head_children_files)
		rm(target, target->head_children_files->name);
	while(target->head_children_dirs)
		rmdir(target, target->head_children_dirs->name);
	free(target->name);
	free(target);
}

int main () {
	
		Dir* home = (Dir *) malloc(sizeof(Dir));
		home->name = malloc(5*sizeof(char));
		strcpy(home->name,"home");
		home->head_children_dirs = NULL;
		home->head_children_files = NULL;
		home->parent = NULL;
		home->next= NULL;
		Dir* director = home;
		File* fisier;
		char* linie = (char*) malloc(MAX_INPUT_LINE_SIZE);	
		char* comanda;
		char* obiect;
		char* obiect2;
		char* rezultat;
		fgets(linie,MAX_INPUT_LINE_SIZE,stdin);
		comanda = strtok(linie," \n");
		obiect = strtok(NULL," \n");
		int nivel = 0;
	do
	{
	
		if(strcmp(comanda,"touch") == 0)
			touch(director,obiect);
		if (strcmp(comanda,"mkdir") == 0)
			mkdir(director,obiect);
		if (strcmp(comanda,"ls") == 0 )
			ls(director);
		if (strcmp(comanda,"rm") == 0 )
			rm(director,obiect);
		if (strcmp(comanda,"rmdir") == 0 )
			rmdir(director,obiect);
		if (strcmp(comanda,"cd") == 0 )
			cd(&director,obiect);
		if (strcmp(comanda,"tree") == 0 )
			tree(director,nivel);
		if (strcmp(comanda,"stop") == 0 ) {
			stop(director);
			break;
		}
		if (strcmp(comanda,"pwd") == 0 ){
			rezultat = pwd(director);
			printf("%s" , rezultat);
		}
		// if (strcmp(comanda,"mv") == 0 ){
		// 	obiect2 = strtok(NULL," \n");
		// 	mv(director,obiect,obiect2);
		// }
		fgets(linie,MAX_INPUT_LINE_SIZE,stdin);
		comanda = strtok(linie," \n");
		obiect = strtok(NULL," \n");
	
	}

	while (1);
	free(linie);
	

	return 0;
}
