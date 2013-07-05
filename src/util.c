/**
 * Copyright 2009 Jan Miderbäck
 *
 * This file is part of Hooce.
 *
 * Hooce is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Hooce is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Hooce.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "hooche.h"

char* str_tolower(char* s)
{
    if (s != NULL) {
        unsigned int i;
        for (i = 0; i < strlen(s); ++i) s[i] = tolower(s[i]);
    }

    return s;
}


int str_icmp(const char* s1, const char* s2)
{
    char* s1_cpy;
    char* s2_cpy;
    int   retval;

    assert(s1 != NULL);
    assert(s2 != NULL);

    s1_cpy = malloc(strlen(s1) + 1);
    s2_cpy = malloc(strlen(s2) + 1);
    strcpy(s1_cpy, s1);
    strcpy(s2_cpy, s2);
    str_tolower(s1_cpy);
    str_tolower(s2_cpy);
    retval = strcmp(s1_cpy, s2_cpy);
    free(s1_cpy);
    free(s2_cpy);
    return retval;
}
