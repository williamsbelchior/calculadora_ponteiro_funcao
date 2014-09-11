#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>
#include "lib/plugin.h"

int main(int argc, char **argv){
	
	/** Pegando os arquivos e guardando o nome no array **/
	DIR *dir;
	struct dirent *ent;
	char *files[256];
	int cont = 0;
	if ((dir = opendir ("./plugins/")) != NULL) {		
		while ((ent = readdir (dir)) != NULL) {
			int len = strlen(ent->d_name);
			if(len >= 2){
				if (strcmp (".so", ent->d_name + len - 3) == 0){
					files[cont] = strndup(ent->d_name + 3, (len - 6));
					cont++;
				}
			}
		}
		closedir (dir);
	} else {
		printf("Could not open directory");
		exit(1);
	}
		
	/** Carregando arquivos .so **/
	int (*f_execute)(int, int[]);
	void *lib_handles[256];
	char *error;
	char *a = "./plugins/lib";
	char *b = ".so";
	char path[80];
	for (int i = 0; i < cont; i++) { 
		strcpy (path, a);
		strcat (path, files[i]);
		strcat (path, b);
		lib_handles[i] = dlopen(path, RTLD_LAZY);
		if (!lib_handles[i]) {
			fprintf(stderr, "%s\n", dlerror());
			exit(1);
		}
		if ((error = dlerror()) != NULL)  {
			fprintf(stderr, "%s\n", error);
			exit(1);
		}
	}
	
	int choice = 0;
	int qtd;
	while (choice <= cont+1) {
        /** Exibindo opções **/
		printf("\n>>>>>>>>>>>>>> Escolha uma opção <<<<<<<<<<<<<<<\n\n");
        printf("\n0 atualizar \n\n");
		for (int i=0; i < cont; i++)
            printf("%d %s\n", (i+1), files[i]);
		printf("\n%d sair", (cont+1));
		printf("\n\n>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<\n\n");
		scanf("%d", &choice);
        if (choice == 0) {
            for (int i=0; i < cont; i++)
                dlclose(lib_handles[i]);
            cont = 0;
            if ((dir = opendir ("./plugins/")) != NULL) {
                while ((ent = readdir (dir)) != NULL) {
                    int len = strlen(ent->d_name);
                    if(len >= 2){
                        if (strcmp (".so", ent->d_name + len - 3) == 0){
                            files[cont] = strndup(ent->d_name + 3, (len - 6));
                            cont++;
                        }
                    }
                }
                closedir (dir);
            } else {
                printf("Could not open directory");
                exit(1);
            }
            for (int i = 0; i < cont; i++) {
                strcpy (path, a);
                strcat (path, files[i]);
                strcat (path, b);
                lib_handles[i] = dlopen(path, RTLD_LAZY);
                if (!lib_handles[i]) {
                    fprintf(stderr, "%s\n", dlerror());
                    exit(1);
                }
                if ((error = dlerror()) != NULL)  {
                    fprintf(stderr, "%s\n", error);
                    exit(1);
                }
            }
        } else if (choice >= cont+1){
            for (int i=0; i < cont; i++)
                dlclose(lib_handles[i]);
			exit(1);
        } else {
            printf("Entre com a quantidade de números: ");
            scanf("%d", &qtd);
            int values[qtd];
            for (int i=0; i < qtd; i++){
                printf("\n%dº número: ", (i+1));
                scanf("%d", &values[i]);
            }
            /** Exibindo resultado **/
            f_execute = dlsym(lib_handles[(choice-1)], "execute");
            int r = (*f_execute)(qtd, values);
            printf("Resultado = %d\n", r);
        }
	}

} 
