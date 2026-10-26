/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svogrig < svogrig@studebt.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 19:03:14 by svogrig           #+#    #+#             */
/*   Updated: 2026/10/26 13:55:21 by svogrig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "libasm.h"

# define STRING_EMPTY "\0"
# define STRING_SHORT "string short\0"
# define STRING_LONG ""

void test_strlen() {
    size_t len_ft;
    size_t len;

    len_ft = ft_strlen(STRING_EMPTY);
    len = strlen(STRING_EMPTY);
    if (len == len_ft) {
        printf("[OK] ");
    } else {
        printf("[KO] ");
    }
    printf("Test ft_strlen : string empty");
}

int main() {
    test_strlen();
    return (0);
}
