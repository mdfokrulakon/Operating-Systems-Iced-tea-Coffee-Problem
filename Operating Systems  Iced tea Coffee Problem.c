#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_LINES 6
#define MAX_LEN 100

sem_t sem_muntasir, sem_fokrul;
int error_count = 0;

const char *muntasir_lines[MAX_LINES] = {
    "Assalamualaikum fokrul.",
    "I'm doing great, you?",
    "Have you tried that new iced coffee?",
    "It was really refreshing after a long day.",
    "Nothing beats iced coffee in summer!",
    "See you later, Allah hafez."
};

char fokrul_lines[MAX_LINES][MAX_LEN] = {
    "Walaikumassalam muntasir, How are you?",
    "I'm fine, thanks!",
    "Yes! I love iced tea.",
    "Wait, I meant iced tea. Oops.",
    "Iced tea is my favorite!",
    "Allah hafez."
};

void replace_word(char *line) {
    char *pos;
   while ((pos = strstr(line, "iced tea")) != NULL || (pos = strstr(line, "Iced tea")) != NULL)
 {
        memmove(pos + 11, pos + 8, strlen(pos + 8) + 1);

        if (strncmp(pos, "Iced tea", 8) == 0)
            memcpy(pos, "Iced coffee", 11);
        else
            memcpy(pos, "iced coffee", 11);

        error_count++;
    }
}

void *muntasir_thread(void *arg) {
    for (int i = 0; i < MAX_LINES; i++) {
        sem_wait(&sem_muntasir);
        printf("Muntasir: %s\n", muntasir_lines[i]);
        sleep(1);
        sem_post(&sem_fokrul);
    }
    return NULL;
}

void *fokrul_thread(void *arg) {
    for (int i = 0; i < MAX_LINES; i++) {
        sem_wait(&sem_fokrul);
        replace_word(fokrul_lines[i]);
        printf("Fokrul: %s\n", fokrul_lines[i]);
        sleep(1);
        sem_post(&sem_muntasir);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    sem_init(&sem_muntasir, 0, 1);
    sem_init(&sem_fokrul, 0, 0);

    pthread_create(&t1, NULL, muntasir_thread, NULL);
    pthread_create(&t2, NULL, fokrul_thread, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&sem_muntasir);
    sem_destroy(&sem_fokrul);

    printf("\nNumber of errors detected and corrected: %d\n", error_count);

    return 0;
}
