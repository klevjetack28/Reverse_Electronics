#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <stdlib.h>

#define BUF_SIZE 10000
#define SIG_SIZE 49152
#define RECIPE_SIZE 2048
#define MAX_COUNT 128

unsigned char buffer[BUF_SIZE];
unsigned char idle[BUF_SIZE];
unsigned char signal[SIG_SIZE]; 
char          recipe[RECIPE_SIZE];

void init_recipe(char* filename)
{
    char temp[48];
    snprintf(temp, sizeof(temp), "const uint16_t %s[] = {", filename);
    memset(recipe, '\0', sizeof(recipe));
    strncpy(recipe, temp, sizeof(recipe) - 1);
}

void add_signal(int count, int index)
{
    if (count == 0)
    {
        fprintf(stderr, "Count == 0\n");
        return;
    }

    char temp[32];
    if (index % 10 == 0)
    {
        snprintf(temp, sizeof(temp), "%d, \n\t\t\t", count * 10);
    }
    else 
    {
        snprintf(temp, sizeof(temp), "%d, ", count * 10);
    }
    strncat(recipe, temp, sizeof(recipe) - strlen(recipe) - 1);
}

void create_recipe(char* filename)
{
    init_recipe(filename);
    int i = 0,count = 0, recipeIndex = 1;
    while (signal[i + 2] != '\0')
    {
        while (signal[i] != signal[i + 1] || signal[i + 1] != signal[i + 2]) //signal[i] ^ signal[i + 1] ^ signal[i + 2] > 0)
        {
            count++;
            i++;
        }
        add_signal(count, recipeIndex);
        recipeIndex++;
        count = 0;

        while (signal[i] == signal[i + 1] && signal[i + 1] == signal[i + 2]) //signal[i] ^ signal[i + 1] ^ signal[i + 2] == 0)
        {
            count++;
            i++;
        }
        add_signal(count, recipeIndex);
        recipeIndex++;
        count = 0;
    }
    strncat(recipe, "};\n\0", sizeof(recipe) - strlen(recipe) - 1);
}

// make faster with double pointer from each side
bool cmp_str(unsigned char s1[], unsigned char s2[])
{
    for (int i = 0; i < BUF_SIZE; i++)
    {
        if (s1[i] != s2[i])
        {
            return false;
        }
    }
    return true;
}


void init_idle()
{
    for (int i = 0; i < BUF_SIZE; i++)
    {
        idle[i] = 0xFF;
    }
}

void pin_one()
{
    for (int i = 0; i < BUF_SIZE; i++)
    {
        buffer[i] = buffer[i] & 0b00000001;
    }
}

void parse_binary(char* filename, char *input, char *output)
{
    FILE *binary = fopen(input, "rb");                   // Open the file
    if (binary == NULL)                                     // Check for errors
    {
        fprintf(stderr, "File not found: %s\n", input); 
        return;
    }
    int index = 0;
    size_t bytes_read, bytes_written;
    bool started = false;
    while (!feof(binary))
    {
        bytes_read = fread(buffer, 1, BUF_SIZE, binary);
        if (bytes_read < BUF_SIZE)
        {
            break;
        }
        if (!cmp_str(buffer, idle))
        {
            pin_one();
            if (index < SIG_SIZE)
            {
                if (buffer[BUF_SIZE - 1] == 0x01)
                {
                    signal[index++] = '1';
                }
                else 
                {
                    signal[index++] = '0';
                }
            }
        }
        fseek(binary, 1 - BUF_SIZE, SEEK_CUR);
    }
    FILE *out_data = fopen(output, "wb");
    if (out_data == NULL)
    {
        fprintf(stderr, "Error opening file: %s", output);
        return;
    }
    create_recipe(filename);
    fwrite(recipe, 1, RECIPE_SIZE, out_data);
    fclose(binary);
    fclose(out_data);
}

int main(int argc, char *argv[])
{
    init_idle();
    DIR *d = opendir("./Documents");
    if (d == NULL)
    {
        fprintf(stderr, "Error opening directory");
        return 1;
    }
    int index = 0;
    char input[64];
    char output[64];
    char filename[64];
    struct dirent *dir;
    for (int i = 0; (dir = readdir(d)) != NULL; i++)
    {   
        if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, ".."))
        {
            continue;
        }
        fprintf(stderr, "%s\n", dir->d_name);
        strcpy(input, "./Documents/");
        strcpy(output, "./Data/Arduino_");
        strlcat(input, dir->d_name, sizeof(input));
        strlcat(output, dir->d_name, sizeof(output));
        strcpy(filename, dir->d_name);
        filename[strlen(filename) - 11] = '\0';
        printf("%s\n", filename);
        parse_binary(filename, input, output);
    }
    free(d);
    return 0;
}
