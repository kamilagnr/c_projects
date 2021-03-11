typedef struct arguments the_args;
typedef struct list the_lists;
typedef struct node the_node;

the_node* create(char* data);

void pushback(the_node* head, char* data);

the_args* the_arguments(int argc,char *argv[], the_lists* input);

void separate_by_type(the_node* files_head, the_node* dirs_head, the_lists* input);

void swap(char** str1, char** str2);

void sort(the_node* head);

void printlist(the_node* head);

void sorttime(long int* time, the_node* head);

void sort_by_time(the_node* head);

void sort_by_time_in_dir(char* dirname, the_node* head, the_args* options);

the_node* readdirs(the_node* head, the_args* options);

void free_list(the_node* head);

void my_ls(int argc, char *argv[]);