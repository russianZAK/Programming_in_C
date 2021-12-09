#include <stdio.h>
#include <stdlib.h>

typedef struct Header
{
    char marker[3];
    int version[2];
    int flags;
    int size[4];
} Header;

typedef struct Frame
{
    char frameID[4];
    int size[4];
    int flags[2];
    int unicode;

} Frame;


Header header;
Frame frame;

void inttobyte(int x, char *mass)
{
    mass[3] = x & 0xFF;
    mass[2] = (x >> 8) & 0xFF;
    mass[1] = (x >> 16) & 0xFF;
    mass[0] = (x >> 24) & 0xFF;
}

int bytetoint(char mass[4])
{
    int temp_mass[4];
    temp_mass[0] = (int)mass[0] << 24;
    temp_mass[1] = (int)mass[1] << 16;
    temp_mass[2] = (int)mass[2] << 8;
    temp_mass[3] = (int)mass[3];

    return temp_mass[0] + temp_mass[1] + temp_mass[2] + temp_mass[3];
}

int show(char *filename, int command, int *oldsize, char *name, int *flag)
{

    FILE *mp3File = fopen(filename, "rb");
    fread(&header, 1, 10, mp3File); 
    int position = 0;
    printf("ID3v2.%d.%d\n", header.version[0], header.version[1]);

    while (fread(&frame, 1, 10, mp3File)) 
    {
        frame.unicode = getc(mp3File);
        int length_str = bytetoint(frame.size); 
        char *mass = calloc(length_str, 1);
        fread(mass, 1, length_str - 1, mp3File);

        if (command == 1)
        {

            printf("%s -- ", frame.frameID);

            if (frame.unicode == 1)
            {
                printf("%ls\n", mass + 2);
            }
            else
            {
                printf("%s\n", mass);
            }
        }

        else if (strcmp(frame.frameID, name) == 0)
        {
            if (flag == 2)
            {
                printf("%s -- ", frame.frameID);
                if (frame.unicode == 1)
                {
                    printf("%ls\n", mass + 2);
                }
                else
                {
                    printf("%s\n", mass);
                }
            }
            else
            {
                *oldsize = length_str;
                position = ftell(mp3File) -length_str - 10 ;
                break;
            }
        }
        free(mass);
    }
    fclose(mp3File);
    return position;
}

void set(char *name, char *value, char *filepath)
{

    int flag = 1;
    char sizeframe[4];
    FILE *mp3File = fopen(filepath, "rb");
    FILE *mp3File2 = fopen("temp", "wb");
    int *oldsize = calloc(1, sizeof(int));

    int position = show(filepath, 2, oldsize, name, flag);
    int sizediff = strlen(value) - *oldsize;
    fread(&header, 1, 10, mp3File);
    int size = bytetoint(header.size);
    size += sizediff;

    inttobyte(size, header.size);
    fwrite(&header, 1, 10, mp3File2);

    char *mass = malloc(position);
    fread(mass, 1, position - 10, mp3File);
    fwrite(mass, 1, position - 10, mp3File2);
    free(mass);
    memcpy(frame.frameID, name, 4);

    inttobyte(strlen(value) + 1, sizeframe);
    memcpy(frame.size, sizeframe, 4);
    fwrite(&frame, 1, 11, mp3File2);
    fwrite(value, 1, strlen(value), mp3File2);
    fseek(mp3File, 0, SEEK_END);
    int readoff;
    readoff = position + 10 + *oldsize;
    int readsize = ftell(mp3File) - readoff;
    mass = (char *)malloc(readsize);
    fseek(mp3File, readoff, SEEK_SET);
    fread(mass, 1, readsize, mp3File);
    fwrite(mass, 1, readsize, mp3File2);
    free(mass);

    fclose(mp3File);
    fclose(mp3File2);
    remove(filepath);
    rename("temp", filepath);
}

int main(int argc, char *argv[])
{

    int command = -1;
    char *filepath = strtok(argv[1], "=");
    filepath = strtok(NULL, "");
    char *name;
    char *value;
    int flag = 0;

    if (strcmp(argv[2], "--show") == 0)
    {
        command = 1;
    }

    else if (argv[2][2] == 'g')
    {
        command = 2;
        name = strpbrk(argv[2], "=") + 1;
        flag = 2;
    }
    else if (argv[2][2] == 's')
    {
        command = 3;
        name = strpbrk(argv[2], "=") + 1;
        value = strpbrk(argv[3], "=") + 1;
    }

    if (command == 1)
    {
        show(filepath, command, NULL, NULL, NULL);
    }

    else if (command == 2)
    {
        show(filepath, command, NULL, name, flag);
    }

    else if (command == 3)
    {
        set(name, value, filepath);
    }

    return 0;
}