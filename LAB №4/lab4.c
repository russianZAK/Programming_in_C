#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned __int8 uint8_t;

typedef struct Header
{
    char marker[3];
    uint8_t version[2];
    uint8_t flags;
    uint8_t size[4];
} Header;

typedef struct Frame
{
    uint8_t frameID[4];
    uint8_t size[4];
    uint8_t flags[2];
    uint8_t unicode;

} Frame;
Header header;
Frame frame;


int bytetoint(uint8_t mass[4]) 
{

    return (int)mass[0] << 21 | (int)mass[1] << 14 | (int)mass[2] << 7 | (int)mass[3] << 0;
}

int show(char *filename, int command, int *oldsize, uint8_t *name, int *flag)
{

    FILE *mp3File = fopen(filename, "rb");

    fread(&header, 1, 10, mp3File);

    int temp1 = bytetoint(header.size);
    int poswrite = 0;
    printf("ID3v2.%d.%d\n", header.version[0], header.version[1]);

    while (fread(&frame, 1, 11, mp3File)) 
    {

        //frame.unicode = getc(mp3File);
        int temp1 = bytetoint(frame.size); 
        uint8_t *mass = (uint8_t *)calloc(temp1, 1);
        fread(mass, 1, temp1 - 1, mp3File); 
        mass[temp1 - 1] = 0;                

        if (command == 1)
        {

            printf("%-5s -- ", frame.frameID, temp1);

            if (frame.unicode == 1)
            {
                printf("%ls\n", mass + 2);
            }
            else
            {
                printf("%s\n", mass);
            }
        }

        else if (!strcmp(frame.frameID, name))
        {
            if (flag == 2)
            {
                printf("%-5s -- ", frame.frameID); 
                if (frame.unicode == 1)
                {
                    printf("%ls\n", mass + 2);
                }
                else
                {
                    printf("%s\n", mass);
                }
            }
           
        }
        free(mass);
    }
    fclose(mp3File);
    return poswrite;
}



int main(int argc, char *argv[])
{

    int command = -1;

    uint8_t *filepath = strtok(argv[1], "=");
    filepath = strtok(NULL, "");

    uint8_t *name = NULL;
    uint8_t *value = NULL;
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


    return 0;
}