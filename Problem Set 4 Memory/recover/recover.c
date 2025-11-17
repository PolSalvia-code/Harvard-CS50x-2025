#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }
    // Open the memory card
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Can't open the file\n");
        return 1;
    }
    // Create a buffer for a block of data
    uint8_t buffer[512];
    // While there's still data left to read from the memory card
    FILE *img = NULL;
    int count = 0;
    int first_found = 0;
    while (fread(buffer, 1, 512, card) == 512)
    {

        char filename[8];

        // Create JPEGs from the data
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] >= 0xe0 && buffer[3] <= 0xef))
        {
            if (count == 0)
            {
                sprintf(filename, "%03i.jpg", count);
                count = 1;
                img = fopen(filename, "w");
                fwrite(buffer, 1, 512, img);
                first_found = 1;
            }
            else
            {
                fclose(img);
                img = NULL;

                sprintf(filename, "%03i.jpg", count);
                count += 1;
                img = fopen(filename, "w");
                fwrite(buffer, 1, 512, img);
            }
        }
        else
        {
            if (first_found == 1)
            {
                fwrite(buffer, 1, 512, img);
            }
        }
    }
    fclose(img);
    fclose(card);
}
