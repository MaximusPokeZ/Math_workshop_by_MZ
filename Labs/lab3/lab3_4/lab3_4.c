#include "mail.c"



int main() 
{

    Post *post = NULL;
    post = (Post * )malloc(sizeof(Post) * MAIL_INCREMENT);
    if (post == NULL)
    {
        return 1;
    }

    post->mails = (Mail *)malloc(MAIL_INCREMENT * sizeof(Mail));
    if (post->mails == NULL)
    {
        free(post);
        return 1;
    }
    post->mail_capacity = MAIL_INCREMENT;
    post->mail_count = 0;
    //указать адрес post
    interactive(&post);


    for (size_t i = 0; i < post->mail_count; i++)
    {
        free_mail(&(post->mails[i]));
    }
    free(post->mails);
    free(post);

















    // enum status_codes status;
    // String str1;
    // String str2;
    // str1 = create_string("abc", &status);
    // switch (status)
    // {
    //     case memory_allocation_problem:
    //         printf("Memory allocation problem\n");
    //         return 1;
    //     default: break;
    // }

    // printf("%s %d\n", str1.data, str1.length);

    // str2 = create_string("abc", &status);
    // switch (status)
    // {
    //     case memory_allocation_problem:
    //         printf("Memory allocation problem\n");
    //         return 1;
    //     default: break;
    // }
    // printf("%s %d\n", str2.data, str2.length);

    // int status_comp;
    // switch (compare_strings(str1, str2, &status_comp))
    // {
    //     case give_null_pointer:
    //         printf("Error: Free null ptr\n");
    //         return 1;
    //     default: break;
    // }
    // if(status_comp < 0) { printf ("%s %s\n", str1.data, str2.data);}
    // else if (status_comp > 0) {printf ("%s %s\n", str2.data, str1.data);}
    // else if (status_comp == 0) {printf("Equal\n");}

    // switch (equal(str1, str2, &status_comp))
    // {
    //     case give_null_pointer:
    //         printf("Error: Free null ptr\n");
    //         return 1;
    //     default: break;
    // }
    // if (status_comp == 0) {printf("Equal\n");}

    // delete_string(&str1);
    // delete_string(&str2);

    // str1 = create_string("Hello, ", &status);
    // switch (status)
    // {
    //     case memory_allocation_problem:
    //         printf("Memory allocation problem\n");
    //         return 1;
    //     default: break;
    // }

    // printf("%s %d\n", str1.data, str1.length);

    // str2 = create_string("World!", &status);
    // switch (status)
    // {
    //     case memory_allocation_problem:
    //         printf("Memory allocation problem\n");
    //         return 1;
    //     default: break;
    // }
    // printf("%s %d\n", str2.data, str2.length);


    // concatenate_strings(&str1, &str2);
    // printf("%s %d\n", str1.data, str1.length);


    // cpystr(&str1, &str2);
    // printf("%s %d\n", str1.data, str1.length);

    // String * new;
    // new = copy_to_new_string(str1, &status);
    // switch (status)
    // {
    //     case memory_allocation_problem:
    //         printf("Memory allocation problem\n");
    //         return 1;
    //     default: break;
    // }
    // printf("%s %d\n", new -> data, new -> length);

    // delete_string(new);
    // free(new);
    // delete_string(&str1);
    // delete_string(&str2);

    return 0;
}
