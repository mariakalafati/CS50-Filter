#include "helpers.h"
#include <math.h>
#include <stdbool.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int av;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int blue = image[i][j].rgbtBlue;
            int green = image[i][j].rgbtGreen;
            int red = image[i][j].rgbtRed;
            av = round((round(blue) + round(green) + round(red)) / 3);
            image[i][j].rgbtBlue = av;
            image[i][j].rgbtGreen = av;
            image[i][j].rgbtRed = av;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int mid = width / 2;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < mid; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            copy[h][w] = image[h][w];
        }
    }
    bool t[9];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < 9; k++)
            {
                t[k] = true;
            }
            if (i == 0)
            {
                t[0] = false;
                t[1] = false;
                t[2] = false;
            }
            else if (i == height - 1)
            {
                t[6] = false;
                t[7] = false;
                t[8] = false;
            }

            if (j == 0)
            {
                t[0] = false;
                t[3] = false;
                t[6] = false;
            }
            else if (j == width - 1)
            {
                t[2] = false;
                t[5] = false;
                t[8] = false;
            }

            float count = 0.00;
            int bsum = 0;
            int gsum = 0;
            int rsum = 0;
            int r;
            int c;
            for (int k = 0; k < 9; k++)
            {
                if (t[k] == true)
                {
                    if (k < 3)
                    {
                        r = i - 1;
                    }
                    else if (k >= 3 && k <= 5)
                    {
                        r = i;
                    }
                    else
                    {
                        r = i + 1;
                    }

                    if (k == 0 || k == 3 || k == 6)
                    {
                        c = j - 1;
                    }
                    else if (k == 1 || k == 4 || k == 7)
                    {
                        c = j;
                    }
                    else
                    {
                        c = j + 1;
                    }
                    count++;
                    bsum += copy[r][c].rgbtBlue;
                    gsum += copy[r][c].rgbtGreen;
                    rsum += copy[r][c].rgbtRed;
                }
            }
            int avb = round(bsum / count);
            int avg = round(gsum / count);
            int avr = round(rsum / count);
            image[i][j].rgbtBlue = avb;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avr;
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            copy[h][w] = image[h][w];
        }
    }
    RGBTRIPLE temp[9];
    bool t[9];
    int Gx[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int Gy[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < 9; k++)
            {
                t[k] = true;
            }
            if (i == 0)
            {
                t[0] = false;
                t[1] = false;
                t[2] = false;
            }
            else if (i == height - 1)
            {
                t[6] = false;
                t[7] = false;
                t[8] = false;
            }

            if (j == 0)
            {
                t[0] = false;
                t[3] = false;
                t[6] = false;
            }
            else if (j == width - 1)
            {
                t[2] = false;
                t[5] = false;
                t[8] = false;
            }
            int r, c;
            for (int k = 0; k < 9; k++)
            {
                if (t[k] == true)
                {
                    if (k < 3)
                    {
                        r = i - 1;
                    }
                    else if (k >= 3 && k <= 5)
                    {
                        r = i;
                    }
                    else
                    {
                        r = i + 1;
                    }

                    if (k == 0 || k == 3 || k == 6)
                    {
                        c = j - 1;
                    }
                    else if (k == 1 || k == 4 || k == 7)
                    {
                        c = j;
                    }
                    else
                    {
                        c = j + 1;
                    }
                    temp[k] = copy[r][c];
                }
                else
                {
                    temp[k].rgbtBlue = 0;
                    temp[k].rgbtGreen = 0;
                    temp[k].rgbtRed = 0;
                }
            }
            int gxblue = 0;
            int gyblue = 0;
            int gxgreen = 0;
            int gygreen = 0;
            int gxred = 0;
            int gyred = 0;
            for (int k = 0; k < 9; k++)
            {
                gxblue += temp[k].rgbtBlue * Gx[k];
                gyblue += temp[k].rgbtBlue * Gy[k];
                gxgreen += temp[k].rgbtGreen * Gx[k];
                gygreen += temp[k].rgbtGreen * Gy[k];
                gxred += temp[k].rgbtRed * Gx[k];
                gyred += temp[k].rgbtRed * Gy[k];
            }
            double blue = round(sqrt((gxblue * gxblue) + (gyblue * gyblue)));
            double green = round(sqrt((gxgreen * gxgreen) + (gygreen * gygreen)));
            double red = round(sqrt((gxred * gxred) + (gyred * gyred)));
            if (blue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = blue;
            }
            if (green > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = green;
            }
            if (red > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = red;
            }
        }
    }
}
