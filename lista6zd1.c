#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>

int main(){

	pid_t pid;
	char nazwa_pliku[64];
	char obr_map[] = "./obr_map";
	int deskryptor;
	int deskryptor2;
	struct stat stat_pliku;
	void *addr = NULL;
	
	pid = fork(); // dzielenie procesu
	
	if(pid < 0){//obsługa bledu
		printf("Nie utworzono procesu potomnego\n");
		return 1;
	}
	if(pid == 0){
        //dziecko
		execlp("display","display","-update","1",obr_map,NULL); //int execlp(const char *file, const char *arg, ...);
	}
	if(pid > 0){
        //rodzic
		while(1){
			printf("Podaj nazwę pliku z obrazem:\t");
			scanf("%s",nazwa_pliku);
           // printf("flaaga1");
			deskryptor = open(nazwa_pliku, O_RDONLY);
			while(deskryptor < 0){
				printf("Nie otwarto pliku\n");
				printf("Podaj nową nazwę pliku:\t");
				scanf("%s",nazwa_pliku);
				deskryptor = open(nazwa_pliku, O_RDONLY);
			}
			printf("\n");
            // printf("flaaga2");
			stat(nazwa_pliku, &stat_pliku); //int stat(const char *path, struct stat *buf); funkcja wczuca informacje o pliku z path do buff
			deskryptor2 = open(obr_map, O_RDWR); // otweiranie d2
			truncate(obr_map,stat_pliku.st_size); // obcinamy plik do zadanaej wartosci
			addr = mmap(NULL, stat_pliku.st_size,PROT_WRITE | PROT_READ, MAP_SHARED, deskryptor2, 0); // zapis do wskaznika, mapowanie
			read(deskryptor, addr, stat_pliku.st_size); // wczytywanie d1
			msync(addr, stat_pliku.st_size,MS_SYNC);
			close(deskryptor); 
			close(deskryptor2);
		}
	}
	munmap(addr, stat_pliku.st_size);
	return 0;
}

