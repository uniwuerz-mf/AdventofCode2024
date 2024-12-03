#include <stdio.h>
#include <stdlib.h>

// Vergleichsfunktion für qsort
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Funktion, um die Anzahl der Vorkommen eines Wertes in einem Array zu zählen
int countOccurrences(int *array, int size, int value) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (array[i] == value) {
            count++;
        }
    }
    return count;
}

int main() {
    FILE *file;
    char filename[] = "input.txt"; // Name der Eingabedatei
    int *left = NULL, *right = NULL; // Dynamische Arrays
    int size = 0, capacity = 100;    // Startkapazität
    int tempLeft, tempRight;

    // Speicher reservieren
    left = (int *)malloc(capacity * sizeof(int));
    right = (int *)malloc(capacity * sizeof(int));
    if (left == NULL || right == NULL) {
        perror("Speicherreservierung fehlgeschlagen");
        return 1;
    }

    // Datei öffnen
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Fehler beim Öffnen der Datei");
        free(left);
        free(right);
        return 1;
    }

    // Daten aus der Datei lesen
    while (fscanf(file, "%d %d", &tempLeft, &tempRight) == 2) {
        // Speicher erweitern, wenn nötig
        if (size >= capacity) {
            capacity *= 2; // Verdoppele die Kapazität
            left = (int *)realloc(left, capacity * sizeof(int));
            right = (int *)realloc(right, capacity * sizeof(int));
            if (left == NULL || right == NULL) {
                perror("Speichererweiterung fehlgeschlagen");
                fclose(file);
                free(left);
                free(right);
                return 1;
            }
        }

        // Werte hinzufügen
        left[size] = tempLeft;
        right[size] = tempRight;
        size++;
    }

    fclose(file); // Datei schließen

    // Berechne die Ähnlichkeitsbewertung (Similarity Score)
    long long similarityScore = 0; // Verwende long long für große Ergebnisse
    for (int i = 0; i < size; i++) {
        int occurrences = countOccurrences(right, size, left[i]);
        similarityScore += left[i] * occurrences;
    }

    // Ergebnis ausgeben
    printf("Similarity Score: %lld\n", similarityScore);

    // Speicher freigeben
    free(left);
    free(right);

    return 0;
}
