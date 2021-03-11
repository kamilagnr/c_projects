#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
int len(char* s)
{
    int cnt=0;
    while(*s)
    {cnt++;s++;}   
    return cnt;
}
char* convert (int input, int base)
{
    int tmp, i=0;
    char* res = malloc(12);
    if(input == 0) 
    {
        res[0] = '0';
        res[1] ='\0';
        return res;
    }
    for(; input != 0; i++)
    {
        tmp = input%base;
        input = input/base;
        if(tmp<10)
            res[i] = '0'+tmp; 
        else
            res[i] = 'a'+tmp-10;
    }i--;
    for(int j=0; j-1<i/2; j++)
    {
        tmp = res[i-j];
        res[i-j] = res[j];
        res[j] = tmp; 
    }
    res[i+1] = '\0'; 
    return res;
}
char* convertu(unsigned int input, int base)
{
    int tmp, i=0;
    char* res = malloc(12);
    for(; input != 0; i++)
    {
        tmp = input%base;
        input = input/base;
        if(tmp<10)
            res[i] = '0'+tmp; 
        else
            res[i] = 'a'+tmp-10; 
        
    }i--;
    for(int j=0; j-1<i/2; j++)
    { 
        tmp = res[i-j];
        res[i-j] = res[j];
        res[j] = tmp; 
    }
    res[i+1] = '\0'; 
    return res;
}
char* convertul(unsigned long input, int base)
{
    int tmp, i=0;
    char* res = malloc(17);
    for(; input != 0; i++)
    {
        tmp = input%base;
        input = input/base;
        if(tmp<10)
            res[i] = '0'+tmp; 
        else
            res[i] = 'a'+tmp-10; 
        
    }i--;
    for(int j=0; j-1<i/2; j++)
    { 
        tmp = res[i-j];
        res[i-j] = res[j];
        res[j] = tmp; 
    }
    res[i+1] = '\0'; 
    return res;
}
int my_printf(char* n, ...)
{
    int cnt=0;
    int d; char c; char* s; unsigned int ud; void* p;
    va_list list;
    va_start(list, n);
    while(*n!= '\0')
    {
        if(*n == '%')
        {
            n++;
            switch(*n)
            {
                case 'd':
                d = va_arg(list, int);
                if(d<0)
                {
                    c = '-';
                    cnt += write(1,&c,1);
                    d = -d;
                }
                s = convert(d,10);
                cnt += write(1,s,len(s));  
                free(s);              
                    break;
                case 'o':
                ud = va_arg(list,unsigned int);
                s = convertu(ud, 8);
                cnt += write(1,s,len(s));
                free(s);                                
                    break;
                case 'u':
                ud = va_arg(list,unsigned int);
                s = convertu(ud,10);
                cnt += write(1,s,len(s));
                free(s);                
                    break;
                case 'x':
                ud = va_arg(list,unsigned int);
                s = convertu(ud, 16);
                cnt += write(1,s,len(s));
                free(s);               
                    break;
                case 'c':
                c = (char)va_arg(list, int);
                write(1,&c,1);
                cnt++;
                    break;
                case 's':
                s = va_arg(list, char *);
                if (s == NULL)
                {
                    s = "(null)";
                }
                cnt += write(1, s, len(s));
                    break;
                case 'p':
                p = va_arg(list, void*);
                unsigned long l = (unsigned long)p;
                s = convertul(l, 16);
                cnt += write(1,"0x",2);
                cnt += write(1,s,len(s));
                free(s); 
                    break;
            } 
        }
        else
        {
            write(1, n, 1);
            cnt++;
        } n++;
    }
    va_end(list);
    return cnt;
}
int main()
{
    //test cases
    my_printf("%d ",my_printf("Hello world!\n"));
    my_printf("%d ",my_printf("%c!\n",'H'));
    my_printf("%d ",my_printf("%c%c%c%c%c!\n", 'H', 'e', 'l', 'l', 'o'));
    my_printf("%d ",my_printf("Hello %s!\n", "QWASAR.IO"));
    my_printf("%d ",my_printf("Hello %s %s!\n", "Mr", "Gaetan"));
    my_printf("%d ",my_printf("NULL STRING %s!\n", (char*)NULL));
    my_printf("%d ",my_printf("%d!\n", 1337));
    my_printf("%d ",my_printf("%d - %d - %d!\n", 2048, 0, -1337));
    my_printf("%d ", my_printf("%o!\n", 100));
    my_printf("%d ", my_printf("%u!\n", 1337));
    my_printf("%d ", my_printf("%x!\n", 14));
    void* any_variable; int some_var = 7;
    any_variable = &some_var;
    my_printf("%d ", my_printf("%p!\n", any_variable));
    return 0;
}