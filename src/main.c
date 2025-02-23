#include"lexer/lexer.h"
#include<stdio.h>

static const char *SOURCE = "2 * (1 + 3)";

int main()
{
    token_t* list = lexer_parse(SOURCE);

    while (list != NULL)
    {
        printf("token: ");
        printf(list->content);
        printf("\n");

        list = list->next;
    }

    return 0;
}