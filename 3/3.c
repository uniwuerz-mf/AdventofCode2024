#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h> // Für reguläre Ausdrücke

int main() {
    // Datei öffnen
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Fehler beim Öffnen der Datei");
        return 1;
    }

    // Dateiinhalt in einen Speicherpuffer laden
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char *input = (char *)malloc(fileSize + 1);
    if (input == NULL) {
        perror("Speicherreservierung fehlgeschlagen");
        fclose(file);
        return 1;
    }

    fread(input, 1, fileSize, file);
    input[fileSize] = '\0'; // Nullterminator
    fclose(file);

    // Regex-Patterns
    char *mulPattern = "mul\\(([0-9]+),([0-9]+)\\)";
    char *doPattern = "do\\(\\)";
    char *dontPattern = "don't\\(\\)";
    regex_t mulRegex, doRegex, dontRegex;
    regmatch_t matches[3];

    // Regex kompilieren
    if (regcomp(&mulRegex, mulPattern, REG_EXTENDED) != 0 ||
        regcomp(&doRegex, doPattern, REG_EXTENDED) != 0 ||
        regcomp(&dontRegex, dontPattern, REG_EXTENDED) != 0) {
        fprintf(stderr, "Fehler beim Kompilieren der regulären Ausdrücke\n");
        free(input);
        return 1;
    }

    char *cursor = input;
    int totalSum = 0;
    int mulEnabled = 1; // Anfangszustand: mul ist aktiviert

    while (*cursor != '\0') {
        // Prüfen, ob eine `do()`-Instruktion vorliegt
        if (regexec(&doRegex, cursor, 1, matches, 0) == 0 && matches[0].rm_so == 0) {
            mulEnabled = 1;
            cursor += matches[0].rm_eo;
            continue;
        }

        // Prüfen, ob eine `don't()`-Instruktion vorliegt
        if (regexec(&dontRegex, cursor, 1, matches, 0) == 0 && matches[0].rm_so == 0) {
            mulEnabled = 0;
            cursor += matches[0].rm_eo;
            continue;
        }

        // Prüfen, ob eine `mul(X,Y)`-Instruktion vorliegt
        if (regexec(&mulRegex, cursor, 3, matches, 0) == 0 && matches[0].rm_so == 0) {
            if (mulEnabled) {
                // Extrahiere die Zahlen X und Y
                int len1 = matches[1].rm_eo - matches[1].rm_so;
                int len2 = matches[2].rm_eo - matches[2].rm_so;

                char num1[10], num2[10];
                strncpy(num1, cursor + matches[1].rm_so, len1);
                num1[len1] = '\0';

                strncpy(num2, cursor + matches[2].rm_so, len2);
                num2[len2] = '\0';

                // Konvertiere die Zahlen zu Ganzzahlen und multipliziere
                int x = atoi(num1);
                int y = atoi(num2);
                totalSum += x * y;
            }
            cursor += matches[0].rm_eo;
            continue;
        }

        // Weiter zur nächsten Position
        cursor++;
    }

    // Regex freigeben und Speicher freigeben
    regfree(&mulRegex);
    regfree(&doRegex);
    regfree(&dontRegex);
    free(input);

    // Ergebnis ausgeben
    printf("Die Summe der aktivierten Multiplikationen ist: %d\n", totalSum);

    return 0;
}
