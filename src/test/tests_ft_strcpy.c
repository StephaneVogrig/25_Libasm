/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_ft_strcpy.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svogrig <svogrig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 13:27:47 by svogrig           #+#    #+#             */
/*   Updated: 2026/11/05 13:31:57 by svogrig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

char *create_string(size_t len, char c) {
    char *string;

    string = malloc(len + 1);
    if (string == NULL) {
        printf("An error occurred on creating string !\n");
        return NULL;
    }
    memset(string, c, len);
    string[len] = '\0';
    return string;
}

int test_strcpy(const char *title, const char *src) {
    size_t len_src;
    char *dest_ft;
    char *dest;
    char *result_ft;
    char *result;
    int fail;

    print_title_test(title);
    fail = 0;
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

    print_title_tests("Tests ft_strcpy");
    fail = test_strcpy("Empty string", s->empty);
    fail += test_strcpy("Short string", s->onechar);
    fail += test_strcpy("All characters", s->allchar);
    fail += test_strcpy("Long string", s->verylong);
    return (fail);
}
