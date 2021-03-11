#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

typedef struct arguments
{
    int opt_a;
    int opt_t;
} the_args;
typedef struct node
{
    char* name;
    struct node* next;

} the_node;
typedef struct list
{
    int size;
    char** array;
} the_lists;

the_node* create(char* data)
{
    the_node* new_node = malloc(sizeof(the_node));
    new_node->name = malloc(50*sizeof(char));
    strcpy(new_node->name, data);
    new_node->next = NULL;
    return new_node;
}

void pushback(the_node* head, char* data)
{
    if(strcmp(head->name, "\0") == 0) strcpy(head->name, data);   
    else
    {
        the_node* current = head;
        while(current->next != NULL)    current = current->next;
        current->next = create(data);
    }
}

the_args* the_arguments(int argc,char *argv[], the_lists* input)
{
    the_args* ptr = malloc(sizeof(the_args));
    ptr->opt_a = 0;
    ptr->opt_t = 0;       
    for(int i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-' && argv[i][1] != 0)
        {
            for(int j = 1; argv[i][j]; j++)
            {
                if(argv[i][j] == 'a')    ptr->opt_a = 1;
                else if(argv[i][j] == 't')    ptr->opt_t = 1;
                else printf("unknown option %s\n", argv[i]);
            }
        }
        else 
        {
            if(argv[i][0] == '.' && ptr->opt_a == 0) continue;
            strcpy(input->array[input->size], argv[i]);
            input->size++;
        }
    }
    return ptr;
}
void separate_by_type(the_node* files_head, the_node* dirs_head, the_lists* input)
{
    struct stat sb;
    for(int i=0; i<input->size; i++)
    {
        lstat(input->array[i], &sb);
        switch(sb.st_mode & S_IFMT)
        {
            case S_IFDIR:    pushback(dirs_head, input->array[i]);    break;
            case S_IFREG:    pushback(files_head, input->array[i]);   break;
            default:         perror("my_ls cannot access operand ");  break;
        }}}

void swap(char** str1, char** str2)
{    char *temp = *str1;    *str1 = *str2;    *str2 = temp;}

void sort(the_node* head) //lex sorting
{
    the_node *iterator = head, *end = NULL;
    int swap_detect;
    do
    {
        iterator = head;
        swap_detect = 0;
        while(iterator->next != end)
        {
            if(strcmp(iterator->name, iterator->next->name) > 0){    swap(&iterator->name, &iterator->next->name);    swap_detect = 1;}
            iterator = iterator->next;
        }   end = iterator;
    }   while(swap_detect); //no swap means list've been sorted
}

void printlist(the_node* head)
{
    the_node* current = head;
    while(current != NULL){     printf("%s  ", current->name);      current = current->next;}   printf("\n");
}

void sorttime(long int* time, the_node* head)
{
    the_node *iterator = head, *end = NULL;
    int swap_detect=0;
    long int* temp = malloc(sizeof(long int)); 
    do
    {
        int i=0,j=i+1;
        iterator = head;
        swap_detect = 0;
        while(iterator->next != end)
        {   
            if(time[i] < time[j])
            {
                *temp = time[i];
                time[i] = time[j];
                time[j] = *temp;
                swap(&iterator->name, &iterator->next->name);    swap_detect = 1; 
            }
            else if(time[i] == time[j])
            {
                if(strcmp(iterator->name, iterator->next->name) > 0){  swap(&iterator->name, &iterator->next->name);    swap_detect = 1;}                
            }   iterator = iterator->next; i++; j++;
        }   end = iterator;
    }   while(swap_detect); //no swap means list've been sorted
    free(temp);
}

void sort_by_time(the_node* head)
{
    the_node *iterator = head;
    int  size=0;
    while(iterator != NULL)    {size++;  iterator = iterator->next;} iterator = head;
    long int* time = malloc(size * sizeof(long int));
    struct stat sb;  
    for(int i=0; i<size; i++)
    {
        lstat(iterator->name, &sb); time[i] = sb.st_mtim.tv_sec*1000000000 + sb.st_mtim.tv_nsec;
        iterator = iterator->next;
    } 
    sorttime(time, head);
    free(time);
}

void sort_by_time_in_dir(char* dirname, the_node* head, the_args* options)
{
    the_node *iterator = head;
    int  size=0;
    while(iterator != NULL)    { size++;  iterator = iterator->next;} iterator = head;
    long int* time = malloc(size * sizeof(long int));
    struct stat sb;  
    char* filename = malloc(50); 
    for(int i=0; i<size; i++)
    {
        strcpy(filename, dirname);
        if(iterator->name[0] == '.' && options->opt_a == 0) continue; //ignore directories . ..
        filename = strcat(filename, "/");
        filename = strcat(filename, iterator->name);     
        lstat(filename, &sb); time[i] = sb.st_mtim.tv_sec*1000000000 + sb.st_mtim.tv_nsec;
        iterator = iterator->next;
    } 
    sorttime(time, head); free(time); free(filename);
}

the_node* readdirs(the_node* head, the_args* options)
{
    the_node* files_in_dir_head = create("\0");
    DIR *folder = opendir(head->name);
    struct dirent *entry;
    if(folder == NULL)        perror("Unable to read directory");
    while( (entry=readdir(folder)) )
    {
        if(entry->d_name[0] == '.' && options->opt_a == 0) continue; //ignore directories . ..
        pushback(files_in_dir_head, entry->d_name);
    }    closedir(folder);
    return files_in_dir_head;
}

void free_list(the_node* head)
{
    the_node* tmp;
    while(head != NULL)
    {
        tmp = head;
        free(head->name);
        head = head->next;
        free(tmp);
    }
}

void my_ls(int argc, char *argv[])
{
    the_lists* input = malloc(sizeof(the_lists));
    input->array = malloc(25*sizeof(char*)); input->size = 0;
    the_node* files_head = create("\0");
    the_node* dirs_head = create("\0");
    for(int i = 0; i < 25; i++ )     input->array[i] = malloc(50*sizeof(char));
    the_args* options = the_arguments(argc, argv, input);
    if(input->size == 0) 
    {
        char* current_dir = ".";
        strcpy(input->array[input->size], current_dir);
        input->size++;
    }
    separate_by_type(files_head, dirs_head, input);
//sorting and printing
    if(options->opt_t == 1)
    {
        if(strcmp(files_head->name, "\0") != 0){    sort_by_time(files_head); printlist(files_head);}
        if(strcmp(dirs_head->name, "\0") != 0) 
        {
            sort_by_time(dirs_head); the_node* current_dir = dirs_head;
            while(current_dir != NULL)
            {
                if(current_dir != dirs_head) printf("\n");
                if(dirs_head->next != NULL) printf("%s:\n", current_dir->name);
                the_node* files_in_dir = readdirs(current_dir, options);
                sort_by_time_in_dir(current_dir->name,files_in_dir, options);
                printlist(files_in_dir); free_list(files_in_dir);
                current_dir = current_dir->next;     
            }
        }
    }
    else
    {
        if(strcmp(files_head->name, "\0") != 0) {   sort(files_head); printlist(files_head);}
        if(strcmp(dirs_head->name, "\0") != 0) 
        {
            if(strcmp(files_head->name, "\0") != 0) printf("\n");
            sort(dirs_head);    the_node* current_dir = dirs_head;
            while(current_dir != NULL)
            {
                if(current_dir != dirs_head) printf("\n");
                if(dirs_head->next != NULL)     printf("%s:\n", current_dir->name);
                the_node* files_in_dir = readdirs(current_dir, options);
                sort(files_in_dir);
                printlist(files_in_dir);
                current_dir = current_dir->next;    
            }
        }
    }
    for(int i=0; i < 25; i++ )    free(input->array[i]);    
    free(input->array);    free(input);
    free(options);  free_list(files_head);  free_list(dirs_head);
}
