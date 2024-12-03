#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> // Für strtok

// Funktion, um zu prüfen, ob ein Bericht sicher ist
bool isSafe(int *levels, int size) {
    bool increasing = true, decreasing = true;

    for (int i = 1; i < size; i++) {
        int diff = levels[i] - levels[i - 1];

        // Unterschied muss zwischen 1 und 3 liegen
        if (diff < -3 || diff > 3 || diff == 0) {
            return false;
        }

        // Prüfe auf konsistente Richtung
        if (diff > 0) decreasing = false;
        if (diff < 0) increasing = false;
    }

    // Nur eine Richtung (entweder steigend oder fallend) erlaubt
    return increasing || decreasing;
}

// Funktion, um zu prüfen, ob das Entfernen eines Elements den Bericht sicher macht
bool isSafeWithDampener(int *levels, int size) {
    for (int i = 0; i < size; i++) {
        // Temporäres Array erstellen, das die aktuelle Zahl auslässt
        int tempLevels[100];
        int tempSize = 0;

        for (int j = 0; j < size; j++) {
            if (j != i) {
                tempLevels[tempSize++] = levels[j];
            }
        }

        // Prüfe, ob der Bericht ohne das aktuelle Element sicher ist
        if (isSafe(tempLevels, tempSize)) {
            return true;
        }
    }

    // Bericht bleibt unsicher
    return false;
}

int main() {
    FILE *file;
    char filename[] = "input.txt";
    char line[256];
    int safeReports = 0;

    // Datei öffnen
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Fehler beim Öffnen der Datei");
        return 1;
    }

    // Jede Zeile einlesen und überprüfen
    while (fgets(line, sizeof(line), file)) {
        int levels[100], size = 0;
        char *token = strtok(line, " \n");

        // Werte in ein Array einlesen
        while (token != NULL) {
            levels[size++] = atoi(token);
            token = strtok(NULL, " \n");
        }

        // Überprüfe, ob der Bericht sicher ist oder durch den Dampener sicher wird
        if (isSafe(levels, size) || isSafeWithDampener(levels, size)) {
            safeReports++;
        }
    }

    fclose(file); // Datei schließen

    // Ergebnis ausgeben
    printf("Anzahl der sicheren Berichte (mit Dampener): %d\n", safeReports);

    return 0;
}
