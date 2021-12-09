#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int line_count = 0;
long array_of_lines[1891714];

typedef struct log
{
    char address[50];
    char time[30];
    char request[150];
    int status;
    char bytes[10];
    char timecode[5];
    time_t date;
} log;

log lines(char *s)
{
    char month[4];
    char months[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    struct tm time;
    log line;
    sscanf(s, "%50s - - [%d/%3s/%d:%d:%d:%d %5s] \"%[^\"]\" %d %s", line.address, &time.tm_mday, month, &time.tm_year, &time.tm_hour, &time.tm_min, &time.tm_sec, line.timecode, line.request, &line.status, line.bytes);
    
    time.tm_year -= 1900;
    for (int i = 0; i <= 11; i++)
    {
        if (strcmp(months[i], month) == 0)
        {
            time.tm_mon = i;
        }
    }

    line.date = mktime(&time);

    return line;
}

void get_window(int window)
{
    int index_low = 0;
    int index_high = 0;
    int max_window = 0;

    while (!(index_high == line_count))
    {
        if (array_of_lines[index_high] - array_of_lines[index_low] <= window)
        {
            index_high++;
        }
        else
        {
            if (index_high - index_low > max_window)
            {
                max_window = index_high - index_low;
            }
            index_low++;
        }
    }
    printf("Count of requests in time window: %d\n", max_window);
}

int main(int argc, char *argv[])
{
    FILE *file = fopen("access_log_Jul95.txt", "r");
    log str_log;
    char str[250];
    long long count = 0;
    int window;
    while (fgets(str, 250, file))
    {

        str_log = lines(str);
        line_count++;
        array_of_lines[line_count] = str_log.date;

        if (str_log.status / 100 == 5)
        {
            printf("%s", str);
            count++;
        }
    }

    printf("Count of unsuccessful requests : %d\n", count);
    printf("Enter time window in seconds:");
    scanf("%d", &window);
    get_window(window);

    return 0;
}