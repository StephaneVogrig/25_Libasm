/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svogrig < svogrig@studebt.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 19:03:14 by svogrig           #+#    #+#             */
/*   Updated: 2026/10/29 01:37:33 by svogrig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
    size_t len_ft;
    size_t len;
    int fail;

    printf("%s ... ", title);
    len_ft = ft_strlen(string);
    len = strlen(string);
    fail = len != len_ft;
    print_result(fail);
    return fail;
}

int tests_strlen(t_strings *s) {
    int fail;

    fail = 0;
    printf("\n%sTests ft_strlen%s\n", YELLOW, NC);
    fail += test_strlen("Empty string", s->empty);
    fail += test_strlen("Short string", s->onechar);
    fail += test_strlen("All characters", s->allchar);
    fail += test_strlen("Long string", s->verylong);
    return (fail);
}

int test_strcpy(const char *title, const char *src) {
    size_t len_src;
    char *dest_ft;
    char *dest;
    char *result_ft;
    char *result;
    int fail;

    fail = 1;
    printf("%s ... ", title);
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

int tests_strcpy(t_strings *s) {
    int fail;

    fail = 0;
    printf("\n%sTests ft_strcpy%s\n", YELLOW, NC);
    fail += test_strcpy("Empty string", s->empty);
    fail += test_strcpy("Short string", s->onechar);
    fail += test_strcpy("All characters", s->allchar);
    fail += test_strcpy("Long string", s->verylong);
    return (fail);
}

t_strings *init_strings(void) {
    t_strings *s;

    s = calloc(1, sizeof(t_strings));
    if (s == NULL) {
        printf("Calloc failed for strings test !\n");
        return (NULL);
    }

    // fill onechar
    s->onechar[0] = '1';

    // fill verylong
    memset(s->verylong, 'A', LEN_VERYLONG);

    // fill allchar
    for(int i = 0; i < 255; i++) {
        s->allchar[i] = i + 1;
    }
    return (s);
}

int main(void) {
    t_strings *strs_test;
    int fail;

    strs_test = init_strings();
    if (strs_test == NULL) {
        return (1);
    }

    fail = 0;
    fail += tests_strlen(strs_test);
    fail += tests_strcpy(strs_test);

    free(strs_test);
    return (fail != 0);
}
