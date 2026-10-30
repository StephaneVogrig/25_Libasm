/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svogrig < svogrig@studebt.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 19:03:14 by svogrig           #+#    #+#             */
/*   Updated: 2026/10/30 13:19:26 by svogrig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "libasm.h"

# define RED		    "\033[31m"
# define GREEN		    "\033[32m"
# define YELLOW		    "\033[33m"
# define NC				"\033[0m"

# define LEN_VERYLONG   1000000

typedef struct s_strings{
    char empty[1];
    char onechar[2];
    char allchar[256];
    char verylong[LEN_VERYLONG + 1];
} t_strings;

t_strings *init_strings(void) {
    t_strings *s;

    s = calloc(1, sizeof(t_strings));
    if (s == NULL) {
        printf("Calloc failed for strings test !\n");
        return (NULL);
    }
    s->onechar[0] = '1';
    for(int i = 0; i < 255; i++) {
        s->allchar[i] = i + 1;
    }
    memset(s->verylong, 'A', LEN_VERYLONG);

    return (s);
}

void print_title_tests(const char *title) {
    printf("\n%s%s%s\n", YELLOW, title, NC);
}

void print_title_test(const char *title) {
    printf("%-32s ... ", title);
}

/* return 1 or 0 or -1*/
static int sign(int n) {
    return (n > 0) - (n < 0);
}

char *create_string(size_t len, char c) {
    char *string;

    string = malloc(len + 1);
    if (string == NULL) {
        printf("An error occured on creating string !\n");
        return NULL;
    }
    memset(string, c, len);
    string[len] = '\0';
    return string;
}

 void print_result(const int fail) {
    char *result;
    char *couleur;

    result = fail ? "KO" : "OK" ;
    couleur = fail ? RED : GREEN;
    printf("%s[%s]%s\n", couleur, result, NC);
}

int test_strlen(const char *title, const char *string) {
    size_t result_ft;
    size_t result;
    int fail;

    print_title_test(title);
    result_ft = ft_strlen(string);
    result = strlen(string);
    printf("ft_strlen = %-10li | strlen = %-10li ", result_ft, result);
    fail = result_ft != result;
    print_result(fail);
    return fail;
}

int test_strcpy(const char *title, const char *src) {
    size_t len_src;
    char *dest_ft;
    char *dest;
    char *result_ft;
    char *result;
    int fail;

    print_title_test(title);
    fail = 1;
    len_src = strlen(src);
    dest_ft = create_string(len_src, 'X');
    if (dest_ft == NULL) {
        return (fail);
    }
    dest = create_string(len_src, 'X');
    if (dest != NULL) {
        dest_ft[len_src] = 'X';
        dest[len_src] = 'X';
        result_ft = ft_strcpy(dest_ft, src);
        result = strcpy(dest, src);
        fail = (result_ft != dest_ft) || (memcmp(result, result_ft, len_src + 1) != 0);
        print_result(fail);
    }
    free(dest_ft);
    free(dest);
    return (fail);
}

int test_strcmp(const char *title, const char *s1, const char *s2) {
    int fail;
    int result_ft;
    int result;

    print_title_test(title);
    result_ft = ft_strcmp(s1, s2);
    result = strcmp(s1, s2);
    printf("ft_strcmp = %-10i | strcmp = %-10i ", result_ft, result);
    fail = (sign(result_ft) != sign(result));
    print_result(fail);

    return fail;
}

int test_write(const char *title, const char *src) {
    int fail;

    print_title_test(title);
    fail = (NULL != src);
    print_result(fail);

    return fail;
}

int tests_strlen(t_strings *s) {
    int fail;

    print_title_tests("Tests ft_strlen");
    fail = test_strlen("Empty string", s->empty);
    fail += test_strlen("Short string", s->onechar);
    fail += test_strlen("All characters", s->allchar);
    fail += test_strlen("Long string", s->verylong);
    return (fail);
}

int tests_strcpy(t_strings *s) {
    int fail;

    print_title_tests("Tests ft_strcpy");
    fail = test_strcpy("Empty string", s->empty);
    fail += test_strcpy("Short string", s->onechar);
    fail += test_strcpy("All characters", s->allchar);
    fail += test_strcpy("Long string", s->verylong);
    return (fail);
}

int tests_strcmp(t_strings *s) {
    int fail;
    char title[64];

    print_title_tests("Tests ft_strcmp");
    fail = 0;
    const char *names[] = {"Empty string", "Short string", "All characters", "Long string", "abc", "abcd", "12345", "12045"};
    const char *strings[]  = {s->empty, s->onechar, s->allchar, s->verylong, "abc", "abcd", "12345", "12045"};
    for (long unsigned int i = 0; i < sizeof(*names); i++) {
        for (long unsigned int j = 0; j < sizeof(*strings); j++) {
            snprintf(title, sizeof title, "%s vs %s", names[i], names[j]);
            fail += test_strcmp(title, strings[i], strings[j]);
        }
    }

    fail += test_strcmp("unsigned > 127",   "\xff", "\x01");
    fail += test_strcmp("unsigned reverse", "\x01", "\xff");

    return fail;
}

int tests_write(t_strings *s) {
    int fail;

    print_title_tests("Tests ft_write");
    fail = test_write("Empty string", s->empty);
    fail += test_write("Short string", s->onechar);
    fail += test_write("All characters", s->allchar);
    fail += test_write("Long string", s->verylong);
    return (fail);
}

int main(void) {
    t_strings *strs_test;
    int fail;

    strs_test = init_strings();
    if (strs_test == NULL) {
        return (1);
    }
    // for no buffering stdout
    setvbuf(stdout, NULL, _IONBF, 0);

    fail = tests_strlen(strs_test);
    fail += tests_strcpy(strs_test);
    fail += tests_strcmp(strs_test);
    fail += tests_write(strs_test);

    printf("\n%sTests failed :%s %i\n", YELLOW, NC, fail);
    free(strs_test);
    return (fail != 0);
}
