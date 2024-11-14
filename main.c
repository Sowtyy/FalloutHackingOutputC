#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <Windows.h>

#include "Utils.c"
#include "AppInfo.c"

struct Settings
{
    int maxRowCharacters;
    int maxRows;
    int keywordRate;
    short toWriteFile;
    char* noiseCharacters;
    int noiseCharactersCount;
    char** keywords;
    int keywordsCount;
};

static void insert_keyword_into_row(char* row, struct Settings* settings)
{
    unsigned int timeMilliseconds = (unsigned int)get_time_milliseconds();
    char* keyword = settings->keywords[get_random_number(timeMilliseconds, 0, settings->keywordsCount - 1)];
    int keywordLength = (int)strlen(keyword);
    int tempRowCharacterIndex = settings->maxRowCharacters - keywordLength;
    int rowCharacterIndex = tempRowCharacterIndex < 0 ? 0 : (int)get_random_number(timeMilliseconds, 0, tempRowCharacterIndex);

    if (settings->maxRowCharacters >= keywordLength + rowCharacterIndex &&
        get_random_number(timeMilliseconds, 0, settings->keywordRate) == 0)
    {
        for (int i = rowCharacterIndex, j = 0; i < settings->maxRowCharacters && j < keywordLength; i++, j++)
        {
            row[i] = keyword[j];
        }
    }
}

static char* generate_one_row(struct Settings* settings)
{
    int maxRowSize = settings->maxRowCharacters + 1;
    char* row = malloc(maxRowSize);

    if (row == 0)
    {
        printf("Error: generate_one_row - memory allocation failed.\n");
        exit(1);
    }

    memset(row, 0, maxRowSize);

    for (int i = 0; i < settings->maxRowCharacters; i++)
    {
        //Sleep(1);
        long long noiseCharacterIndex = get_random_number((unsigned int)get_time_milliseconds() + i * i, 0, settings->noiseCharactersCount - 1);
        row[i] = settings->noiseCharacters[noiseCharacterIndex];
        insert_keyword_into_row(row, settings);
    }

    return row;
}

static char** generate_rows(struct Settings* settings)
{
    char** rows = malloc(sizeof(char*) * settings->maxRows);

    if (rows == 0)
    {
        printf("Error: generate_rows - memory allocation failed.\n");
        exit(1);
    }

    for (int i = 0; i < settings->maxRows; i++)
    {
        rows[i] = generate_one_row(settings);
        Sleep(1);
    }

    return rows;
}

static void print_rows(struct Settings* settings, char** rows)
{
    for (int i = 0; i < settings->maxRows; i++)
    {
        printf("%s\n", rows[i]);
    }
}

static free_rows(struct Settings* settings, char** rows)
{
    for (int i = 0; i < settings->maxRows; i++)
    {
        free(rows[i]);
    }

    free(rows);
}

static char* rows_to_string(char** rows, struct Settings* settings)
{
    int rowTextSize = (settings->maxRowCharacters + 1) * settings->maxRows + settings->maxRows;
    char* rowText = malloc(rowTextSize);

    if (rowText == 0)
    {
        printf("Error: rows_to_string - memory allocation failed.\n");
        exit(1);
    }

    memset(rowText, 0, rowTextSize);

    for (int i = 0; i < settings->maxRows; i++)
    {
        strcat_s(rowText, rowTextSize, rows[i]);
        strcat_s(rowText, rowTextSize, "\n");
    }

    return rowText;
}

static void write_file(char* path, char* contents)
{
    FILE* file = fopen(path, "w");
    fprintf(file, "%s", contents);
    fclose(file);
}

static void parse_arguments_to_settings(struct Settings* settings, int argc, char** argv)
{
    settings->maxRowCharacters = atoi(argv[1]);
    settings->maxRows = atoi(argv[2]);
    settings->keywordRate = atoi(argv[3]);
    settings->toWriteFile = argv[4][0] == '1';

    int parsedArgumentsNumber = 5;
    int remainingArgumentsNumber = argc - parsedArgumentsNumber;

    settings->keywordsCount = remainingArgumentsNumber;
    settings->keywords = malloc(sizeof(char*) * remainingArgumentsNumber);

    if (settings->keywords == 0)
    {
        printf("Error: parse_arguments_to_settings - memory allocation failed.\n");
        exit(1);
    }

    memset(settings->keywords, 0, sizeof(settings->keywords));

    for (int i = parsedArgumentsNumber, j = 0; i < argc; i++, j++)
    {
        settings->keywords[j] = argv[i];
    }
}

int main(int argc, char** argv)
{
    struct AppInfo appInfo = get_app_info();

    if (argc < 2)
    {
        printf("Arguments missing.\n\n"
               "Argument 1 - Amount of characters in a row.\n"
               "Argument 2 - Amount of rows.\n"
               "Argument 3 - Keyword appearing chance (1 in X).\n"
               "Argument 4 - Write output to file? ('1' == yes, '0' == no).\n"
               "Argument 5 to Indefinite - Keywords.\n\n"
               "%s by %s.\n%s, %s.\n", appInfo.name, appInfo.author, appInfo.version, appInfo.versionDate);
        return 0;
    }

    struct Settings settings = { 0 };
    settings.noiseCharacters = "!@#$%^&*()_+-=[]{}/\\,.<>?|~`:;'\"";
    settings.noiseCharactersCount = (int)strlen(settings.noiseCharacters);
    parse_arguments_to_settings(&settings, argc, argv);

    char** rows = generate_rows(&settings);
    print_rows(&settings, rows);

    if (settings.toWriteFile)
    {
        char* rowsText = rows_to_string(rows, &settings);
        write_file("output.txt", rowsText);
        free(rowsText);
    }

    free_rows(&settings, rows);
    free(settings.keywords);

    return 0;
}
