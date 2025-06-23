#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <stdlib.h>

#define BUF_SIZE 6144
#define SIG_SIZE 49152

unsigned char buffer[BUF_SIZE];
unsigned char idle[BUF_SIZE];
unsigned char signal[SIG_SIZE]; 

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

void parse_binary(char *input, char *output)
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
                signal[index++] = buffer[BUF_SIZE - 1];
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

    bytes_written = fwrite(signal, 1, SIG_SIZE, out_data);
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
    struct dirent *dir;
    for (int i = 0; (dir = readdir(d)) != NULL; i++)
    {   
        if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, ".."))
        {
            continue;
        }
        fprintf(stderr, "%s\n", dir->d_name);
        strcpy(input, "./Documents/");
        strcpy(output, "./Data/Data_");
        strlcat(input, dir->d_name, sizeof(input));
        strlcat(output, dir->d_name, sizeof(output));
        parse_binary(input, output);
    }
    free(d);
    return 0;
}

