#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int temp =
                round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = temp;
            image[i][j].rgbtGreen = temp;
            image[i][j].rgbtRed = temp;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sum_red = 0, sum_green = 0, sum_blue = 0;
            int count = 0;

            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;

                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        sum_red += copy[ni][nj].rgbtRed;
                        sum_green += copy[ni][nj].rgbtGreen;
                        sum_blue += copy[ni][nj].rgbtBlue;
                        count++;
                    }
                }
            }

            image[i][j].rgbtRed = round(sum_red / (float) count);
            image[i][j].rgbtGreen = round(sum_green / (float) count);
            image[i][j].rgbtBlue = round(sum_blue / (float) count);
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE extended[height + 2][width + 2];
    for (int i = 0; i < height + 2; i++)
    {
        for (int j = 0; j < width + 2; j++)
        {
            extended[i][j].rgbtRed = 0;
            extended[i][j].rgbtGreen = 0;
            extended[i][j].rgbtBlue = 0;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            extended[i + 1][j + 1].rgbtRed = image[i][j].rgbtRed;
            extended[i + 1][j + 1].rgbtGreen = image[i][j].rgbtGreen;
            extended[i + 1][j + 1].rgbtBlue = image[i][j].rgbtBlue;
        }
    }

    for (int i = 1; i < height + 1; i++)
    {
        for (int j = 1; j < width + 1; j++)
        {
            int temp_red_Gx =
                round((extended[i + 1][j + 1].rgbtRed * 1 + extended[i + 1][j].rgbtRed * 0 +
                       extended[i][j + 1].rgbtRed * 2 + extended[i + 1][j - 1].rgbtRed * -1 +
                       extended[i - 1][j + 1].rgbtRed * 1 + extended[i - 1][j - 1].rgbtRed * -1 +
                       extended[i - 1][j].rgbtRed * 0 + extended[i][j - 1].rgbtRed * -2 +
                       extended[i][j].rgbtRed * 0));

            int temp_red_Gy =
                round((extended[i + 1][j + 1].rgbtRed * 1 + extended[i + 1][j].rgbtRed * 2 +
                       extended[i][j + 1].rgbtRed * 0 + extended[i + 1][j - 1].rgbtRed * 1 +
                       extended[i - 1][j + 1].rgbtRed * -1 + extended[i - 1][j - 1].rgbtRed * -1 +
                       extended[i - 1][j].rgbtRed * -2 + extended[i][j - 1].rgbtRed * 0 +
                       extended[i][j].rgbtRed * 0));
            int temp_green_Gx =
                round((extended[i + 1][j + 1].rgbtGreen * 1 + extended[i + 1][j].rgbtGreen * 0 +
                       extended[i][j + 1].rgbtGreen * 2 + extended[i + 1][j - 1].rgbtGreen * -1 +
                       extended[i - 1][j + 1].rgbtGreen * 1 +
                       extended[i - 1][j - 1].rgbtGreen * -1 + extended[i - 1][j].rgbtGreen * 0 +
                       extended[i][j - 1].rgbtGreen * -2 + extended[i][j].rgbtGreen * 0));

            int temp_green_Gy =
                round((extended[i + 1][j + 1].rgbtGreen * 1 + extended[i + 1][j].rgbtGreen * 2 +
                       extended[i][j + 1].rgbtGreen * 0 + extended[i + 1][j - 1].rgbtGreen * 1 +
                       extended[i - 1][j + 1].rgbtGreen * -1 +
                       extended[i - 1][j - 1].rgbtGreen * -1 + extended[i - 1][j].rgbtGreen * -2 +
                       extended[i][j - 1].rgbtGreen * 0 + extended[i][j].rgbtGreen * 0));

            int temp_blue_Gx =
                round((extended[i + 1][j + 1].rgbtBlue * 1 + extended[i + 1][j].rgbtBlue * 0 +
                       extended[i][j + 1].rgbtBlue * 2 + extended[i + 1][j - 1].rgbtBlue * -1 +
                       extended[i - 1][j + 1].rgbtBlue * 1 + extended[i - 1][j - 1].rgbtBlue * -1 +
                       extended[i - 1][j].rgbtBlue * 0 + extended[i][j - 1].rgbtBlue * -2 +
                       extended[i][j].rgbtBlue * 0));

            int temp_blue_Gy =
                round((extended[i + 1][j + 1].rgbtBlue * 1 + extended[i + 1][j].rgbtBlue * 2 +
                       extended[i][j + 1].rgbtBlue * 0 + extended[i + 1][j - 1].rgbtBlue * 1 +
                       extended[i - 1][j + 1].rgbtBlue * -1 + extended[i - 1][j - 1].rgbtBlue * -1 +
                       extended[i - 1][j].rgbtBlue * -2 + extended[i][j - 1].rgbtBlue * 0 +
                       extended[i][j].rgbtBlue * 0));

            int G_red = (int) round(sqrt(temp_red_Gx * temp_red_Gx + temp_red_Gy * temp_red_Gy));
            int G_green =
                (int) round(sqrt(temp_green_Gx * temp_green_Gx + temp_green_Gy * temp_green_Gy));
            int G_blue =
                (int) round(sqrt(temp_blue_Gx * temp_blue_Gx + temp_blue_Gy * temp_blue_Gy));

            if (G_red > 255)
            {
                G_red = 255;
            }
            if (G_green > 255)
            {
                G_green = 255;
            }
            if (G_blue > 255)
            {
                G_blue = 255;
            }

            image[i - 1][j - 1].rgbtRed = G_red;
            image[i - 1][j - 1].rgbtGreen = G_green;
            image[i - 1][j - 1].rgbtBlue = G_blue;
        }
    }
    return;
}
