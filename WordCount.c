#include <stdio.h>
#include <string.h>
#include <io.h>

int count_lines(FILE *file);
int count_words(FILE*file);
int count_bytes(FILE*file);

int main(int argc, char *argv[]){

    FILE *file = fopen(argv[2], "r");
   
    if (strcmp(argv[1], "-c") == 0 || (strcmp(argv[1], "--bytes") == 0))
    {
        printf("Size of file in bytes: %d\n", count_bytes(file));
        return 0;
    }
    
    if (strcmp(argv[1], "-l") == 0 || (strcmp(argv[1], "--lines") == 0))
    {
        printf("Amount of lines: %d\n", count_lines(file));
        return 0;
    }

    if (strcmp(argv[1], "-w") == 0 || (strcmp(argv[1], "--words") == 0))
    {
        printf("Amount of words: %d\n", count_words(file));
        return 0;
    }

    return 0;

}


int count_bytes(FILE*file){

    long size;
    fseek(file, 0, SEEK_END); 
    size = ftell(file); 
    return size;
}


int count_words(FILE*file)
{
    char line[255];
    int count_line=0, word=0,words=0;
    while(fgets(line,255,file)!= NULL){
        count_line++;
        for(int i=0; i<strlen(line);i++){

            if (line[i]!=' ' && line[i-1]==' '){
                word++;
            }
        }
        }

    words=word+count_line;
    return words;
}


int count_lines(FILE *file){
    int count_line=0;
    char line[255];
    while(fgets(line,255,file)!= NULL){

        count_line++;

    }

    return count_line;
}