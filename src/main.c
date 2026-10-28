/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svogrig < svogrig@studebt.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 19:03:14 by svogrig           #+#    #+#             */
/*   Updated: 2026/10/28 21:21:14 by svogrig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libasm.h"

# define RED		    "\033[31m"
# define GREEN		    "\033[32m"
# define NC				"\033[0m"

char *create_string(int len, char c) {
    char *string;

    string = malloc(len + 1);
    if (string == NULL) {
        printf("An error occured on creating string !");
        return NULL;
    }
    memset(string, c, len);
    string[len] = '\0';
    return string;
}

char *string_all_character() {
    char *string;

    string = create_string(255, '\0');
    if (string != NULL) {
        for(int i = 0; i < 255; i++) {
            string[i] = i + 1;
        }
    }
    return string;
}

void test_strlen(const char *title, const char *string) {
    size_t len_ft;
    size_t len;

    len_ft = ft_strlen(string);
    len = strlen(string);
    if (len == len_ft) {
        printf("%s[OK]%s ", GREEN, NC);
    } else {
        printf("%s[KO]%s ", RED, NC);
    }
    printf(" %s : %zu - %zu\n", title, len_ft, len);
}

void tests_strlen() {
    char *string;

    printf("\nTests ft_strlen\n");
    test_strlen("Empty string", "");
    test_strlen("Short string", "1");
    string = create_string(1000000, 'A');
    if (string != NULL) {
        test_strlen("Long string", string);
        free(string);
    }
    string = string_all_character();
    if (string != NULL) {
        test_strlen("All characters", string);
        free(string);
    }
}

int main() {
    tests_strlen();
    return (0);
}
