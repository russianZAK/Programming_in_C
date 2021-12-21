#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

int w, h;
unsigned char *read_bmp(char *file_name, int *width, int *height)
{
    unsigned char head[54];
    FILE *file = fopen(file_name, "rb");
    fread(head, 1, 54, file);
    int w = head[18] + (((int)head[19]) << 8) + (((int)head[20]) << 16) + (((int)head[21]) << 24);
    int h = head[22] + (((int)head[23]) << 8) + (((int)head[24]) << 16) + (((int)head[25]) << 24);

    int line_size = (w / 8 + (w / 8) % 4);
    int file_size = line_size * h;

    unsigned char *img = malloc(w * h);
    unsigned char *data = malloc(file_size);
    fseek(file, 54, SEEK_SET);
    fseek(file, 8, SEEK_CUR);
    fread(data, 1, file_size, file);

    int i, byte, j, reversed_j;
    for (j = 0, reversed_j = h - 1; j < h; j++, reversed_j--)
    {
        for (i = 0; i < w; i++)
        {
            byte = i / 8;
            unsigned char data_byte = data[j * line_size + byte];
            int pos = reversed_j * w + i;
            unsigned char mask = 0x80 >> i % 8;
            img[pos] = (data_byte & mask) ? 1 : 0;
        }
    }

    free(data);
    *width = w;
    *height = h;
    return img;
}

int neighbours_count(int game_grid[h][w], int i, int j, int size)
{
    int neighbour_count = 0;
    if (i - 1 >= 0 && j - 1 >= 0)
    {
        if (game_grid[i - 1][j - 1] >= 1)
        {
            neighbour_count++;
        }
    }

    if (i - 1 >= 0)
    {
        if (game_grid[i - 1][j] >= 1)
        {
            neighbour_count++;
        }
    }

    if (i - 1 >= 0 && j + 1 < size)
    {
        if (game_grid[i - 1][j + 1] >= 1)
        {
            neighbour_count++;
        }
    }

    if (j - 1 >= 0)
    {
        if (game_grid[i][j - 1] >= 1)
        {
            neighbour_count++;
        }
    }

    if (j + 1 < size)
    {
        if (game_grid[i][j + 1] >= 1)
        {
            neighbour_count++;
        }
    }

    if (i + 1 < size && j - 1 >= 0)
    {
        if (game_grid[i + 1][j - 1] >= 1)
        {
            neighbour_count++;
        }
    }

    if (i + 1 < size)
    {
        if (game_grid[i + 1][j] >= 1)
        {
            neighbour_count++;
        }
    }

    if (i + 1 < size && j + 1 < size)
    {
        if (game_grid[i + 1][j + 1] >= 1)

        {
            neighbour_count++;
        }
    }

    return neighbour_count;
}

void mass_to_bmp(int mas[h][w], int height, int width, int steps, char *dir)
{
    char directory[512];
    strcpy(directory, dir);
    char temp_str[512];
    int i;
    int line;
    int column;
    int size = width * height * 4;

    char head[54] = {0};
    strcpy(head, "BM");
    memset(&head[2], 54 + size, 1);
    memset(&head[10], 54, 1);
    memset(&head[14], 40, 1);
    memset(&head[18], width, 1);
    memset(&head[22], height, 1);
    memset(&head[26], 1, 1);
    memset(&head[28], 32, 1);
    memset(&head[34], size, 1);
    unsigned char *capture = malloc(size);

    for (line = height - 1, i = 0; line >= 0; line--, i++)
    {
        for (column = 0; column < width; column++)
        {
            int p = (line * width + column) * 4;
            if (mas[i][column] != 0)
            {
                capture[p] = 0;
                capture[++p] = 0;
                capture[++p] = 0;
            }
            else
            {
                capture[p] = 255;
                capture[++p] = 255;
                capture[++p] = 255;
            }
        }
    }

    if (!strcmp(directory, ""))
    {
        char filename[512];
        sprintf(filename, "life_game_%d.bmp", steps);
        FILE *fout = fopen(filename, "wb");
        fwrite(head, 1, 54, fout);
        fwrite(capture, 1, size, fout);
        free(capture);
        fclose(fout);
    }
    else
    {
        sprintf(temp_str, "life_game_%d", steps);
        strcat(directory, temp_str);
        strcat(directory, ".bmp");
        FILE *fout = fopen(directory, "wb");
        fwrite(head, 1, 54, fout);
        fwrite(capture, 1, size, fout);
        free(capture);
        fclose(fout);
    }
}
int main(int argc, char *argv[])
{

    char file[512] = "";
    char dir[512] = "";
    int steps = 20;
    int frequency = 1;

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "--input"))
        {
            strcpy(file, argv[++i]);
        }
        else if (!strcmp(argv[i], "--output"))
        {
            strcpy(dir, argv[++i]);
        }
        else if (!strcmp(argv[i], "--max_iter"))
        {

            steps = atoi(argv[++i]);
        }
        else if (!strcmp(argv[i], "--dump_freq"))
        {

            frequency = atoi(argv[++i]);
        }
    }

    int i, j;
    unsigned char *bmp = read_bmp(file, &w, &h);
    int game_grid[h][w];

    for (j = 0; j < h; j++)
    {
        for (i = 0; i < w; i++)
        {
            game_grid[j][i] = (int)(bmp[j * w + i] ? '0' : '1') - 48;
        }
    }

    int neighbour_count[h][w];

    for (int k = 0; k <= steps; k++)
    {
        for (i = 0; i < h; ++i)
        {
            for (j = 0; j < h; ++j)
            {
                if (k % frequency == 0)
                {
                    mass_to_bmp(game_grid, h, w, k, dir);
                }

                neighbour_count[i][j] = neighbours_count(game_grid, i, j, h);
            }
        }

        for (i = 0; i < h; ++i)
        {
            for (j = 0; j < h; ++j)
            {
                if (game_grid[i][j] >= 1)
                {
                    if (neighbour_count[i][j] <= 1 || neighbour_count[i][j] >= 4)
                        game_grid[i][j] = 0;
                }
                else if (neighbour_count[i][j] == 3)
                    game_grid[i][j] = 1;
            }
        }
    }

    return 0;
}
