#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void arh_create(char *arc_name, char **text_files, int files_count)
{
    FILE *filearc = fopen(arc_name, "wb");
    fwrite(&files_count, 4, 1, filearc);
    FILE *file;
    int name_size;
    int file_size;
    for (int i = 0; i < files_count; i++)
    {
        file = fopen(text_files[i], "rb");

        name_size = strlen(text_files[i]);

        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        fwrite(&name_size, 4, 1, filearc);
        fwrite(&file_size, 4, 1, filearc);

        for (int j = 0; j < name_size; j++)
        {
            printf("%c", text_files[i][j]);
            fputc(text_files[i][j], filearc);
        }
        for (int j = 0; j < file_size; j++)
        {
            fputc(fgetc(file), filearc);
        }

        printf(" - successfully achieved! \n");
    }
}

void arh_extract(char *arc_name)
{
    FILE *filearc;
    FILE *file;
    int name_size;
    int file_size;
    char file_name[2056];

    filearc = fopen(arc_name, "rb");

    int files_count;
    fread(&files_count, 4, 1, filearc);

    for (int i = 0; i < files_count; i++)
    {
        fread(&name_size, 4, 1, filearc);
        fread(&file_size, 4, 1, filearc);
        for (int j = 0; j < name_size; j++)
        {
            file_name[j] = fgetc(filearc);
            printf("%c", file_name[j]);
        }

        file_name[name_size] = '\0';
        file = fopen(file_name, "wb");

        for (int j = 0; j < file_size; j++)
        {
            fputc(fgetc(filearc), file);
        }
        printf(" - extracted successfully!\n");
    }
    
}

void arh_list(char *arc_name)
{
    FILE *filearc;
    filearc = fopen(arc_name, "rb");
    int files_count;
    fread(&files_count, 4, 1, filearc);

    int name_size, file_size;
    for (int i = 0; i < files_count; i++)
    {
        fread(&name_size, 4, 1, filearc);
        fread(&file_size, 4, 1, filearc);

        for (int j = 0; j < name_size; j++)
        {
            printf("%c", fgetc(filearc));
        }
        printf("\n");

        fseek(filearc, file_size, SEEK_CUR);
    }
}

int main(int argc, char *argv[])
{
    char *arh_name;
    arh_name = argv[2];
    int files_count;
    char **text_files;
    
    if (!strcmp(argv[3], "--create"))
    {
        files_count = argc - 4;
        text_files = argv + 4;
        arh_create(arh_name, text_files, files_count);
    }
    else if (!strcmp(argv[3], "--extract"))
    {
        arh_extract(arh_name);
    }
    else if (!strcmp(argv[3], "--list"))
    {
        arh_list(arh_name);
    }

    return 0;
}
