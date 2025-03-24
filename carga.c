#include <stdio.h>

typedef struct {
    float peso;
    float volume;
} compartimento;

typedef struct {
    float peso;
    float volume;
    float ganho;
} carregamento;

int main() {
    int k, n;
    scanf("%d %d", &k, &n);

    compartimento compartimentos[k];
    carregamento carregamentos[n];

    for (int i = 0; i < k; i++) {
        scanf("%g %g", &compartimentos[i].peso, &compartimentos[i].volume);
    }
    for (int j = 0; j < n; j++) {
        scanf("%g %g %g", &carregamentos[j].peso, &carregamentos[j].volume, &carregamentos[j].ganho);
    }    

    FILE* arquivo = fopen("problema.lp", "w+");

    /* escreve a funcao objetivo */
    fprintf(arquivo, "max: ");
    for (int i = 0; i < k-1; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(arquivo, "%gx%d%d + ", carregamentos[j].ganho, i+1, j+1);
        }
    }
    for (int j = 0; j < n-1; j++) {
        fprintf(arquivo, "%gx%d%d + ", carregamentos[j].ganho, k, j+1);
    }
    fprintf(arquivo, "%gx%d%d;\n\n", carregamentos[n-1].ganho, k, n);

    /* restricoes para garantir que nao seja pego
       mais de um carregamento do que eh permitido */
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < k-1; i++) {
            fprintf(arquivo, "x%d%d + ", i+1, j+1);
        }
        fprintf(arquivo, "x%d%d <= %g;\n", k, j+1, carregamentos[j].peso);
    }
    fprintf(arquivo, "\n");

    /* restricoes para garantir que os compartimentos
       nao fiquem com sobrepeso */
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < n-1; j++) {
            fprintf(arquivo, "x%d%d + ", i+1, j+1);
        }
        fprintf(arquivo, "x%d%d <= %g;\n", i+1, n, compartimentos[i].peso);
    }
    fprintf(arquivo, "\n");

    /* restricoes para garantir que os compartimentos
       nao fiquem com excesso de volume */
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < n-1; j++) {
            fprintf(arquivo, "%gx%d%d + ", carregamentos[j].volume, i+1, j+1);
        }
        fprintf(arquivo, "%gx%d%d <= %g;\n",carregamentos[n-1].volume, i+1, n, compartimentos[i].volume);
    }
    fprintf(arquivo, "\n");

    /* restricoes para garantir as proporcoes
       de peso dos compartimentos */
    for (int i = 0; i < k-1; i++) {
        for (int j = 0; j < n-1; j++) {
            fprintf(arquivo, "%gx%d%d + ", compartimentos[i+1].peso, i+1, j+1);
        }
        fprintf(arquivo, "%gx%d%d = ", compartimentos[i+1].peso, i+1, n);

        for (int j = 0; j < n-1; j++) {
            fprintf(arquivo, "%gx%d%d + ", compartimentos[i].peso, i+2, j+1);
        }
        fprintf(arquivo, "%gx%d%d;\n", compartimentos[i+1].peso, i+2, n);
    }
    fprintf(arquivo, "\n");

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(arquivo, "x%d%d >= 0;\n", i+1, j+1);
        }
    }

    fclose(arquivo);
    return 0;
}