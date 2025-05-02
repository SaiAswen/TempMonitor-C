#include <stdio.h>
#include <stdlib.h>
#define MAX 1000
#define THRESHOLD 95.0

typedef struct {
    char t[20];
    int id;
    float temp;
} D;
int read_data(const char *f, D *d)
{
    FILE *fp = fopen(f, "r");
    if(!fp)
    {
        printf("Err opening %s\n", f);
        return -1;
    }

    char ln[256];
    int count = 0;
    fgets(ln, sizeof(ln), fp);
    while (fgets(ln, sizeof(ln), fp))
    {
        sscanf(ln, "%19[^,],%d,%f", d[count].t, &d[count].id, &d[count].temp);
        count++;
    }

    fclose(fp);
    return count;
}
void calc(D *d, int count, float *avg, float *min, float *max)
{
    *min = *max = d[0].temp;
    float sum = 0;
    for(int i = 0; i < count; i++)
    {
        sum += d[i].temp;
        if(d[i].temp < *min)
        {
            *min = d[i].temp;
        }
        if(d[i].temp > *max)
        {
            *max = d[i].temp;
        }
    }

    *avg = sum / count;
}
void save_high(D *d, int count, const char *out)
{
    FILE *fp = fopen(out, "w");
    if(!fp)
    {
        printf("Err opening %s\n", out);
        return;
    }
    fprintf(fp, "Time,ID,Temp\n");
    for(int i = 0; i < count; i++)
    {
        if (d[i].temp > THRESHOLD)
        {
            fprintf(fp, "%s,%d,%.2f\n", d[i].t, d[i].id, d[i].temp);
        }
    }

    fclose(fp);
}
int main()
{
    D d[MAX];
    const char *in = "C:\\Users\\admin\\OneDrive\\Documents\\Embedded c\\programs\\Project 1\\data.csv";
    const char *out = "C:\\Users\\admin\\OneDrive\\Documents\\Embedded c\\programs\\Project 1\\high_temp.csv";
    int count = read_data(in, d);
    if (count == -1)
    {
        return 1;
    }
    float avg, min, max;
    calc(d, count, &avg, &min, &max);
    printf("Average Temp: %.2f°C\n", avg);
    printf("Max Temp: %.2f°C\n", max);
    printf("Min Temp: %.2f°C\n", min);
    save_high(d, count, out);
    printf("High temps saved to %s\n", out);
    return 0;
}
