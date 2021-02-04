#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void *my_realloc( void *ptr, size_t size, int oldsize ) //cause realloc is not authorized
{
    void *newptr;
    if ( size <= oldsize ) {
        return ptr;
    }
    newptr = malloc( size );
    memcpy( newptr, ptr, oldsize );
    free( ptr );
    return newptr;
}

char *my_strcpy( char* dest, const char* str ) //cause strcpy is not authorized
{
    for ( size_t i=0; i <= strlen( str ); i++ ) 
        dest[i] = str[i];
    return dest;
}

char *itoa ( long long value, char * str, int base )
{
    memset( str, '\0', 20 * sizeof(char) );  //clearing the str to avoid string errors 
    int i = 0; 
    _Bool Negative = 0;
    if ( value == 0 ) 
    { 
        str[0] = '0';  
        return str; 
    }
    if ( value < 0 && base == 10 ) 
    { 
        Negative = 1; 
        value = -value; 
    }
    int rem; 
    i=0;
    long long memcapacity = 20 * sizeof(char);  //  WE stopped here MARAT!!!!!!!!!!! Realloc for the greater number of the little
    while ( value > 0 ) {
        rem = value % base;
        str[i] = ( rem > 9 ) ? ( rem-10 ) + 'a' : rem + '0'; 
        i++;
        value = value / base;
        if ( i + 1 == memcapacity / sizeof(char) ) {
            str = (char*) my_realloc( str, 2 * memcapacity, memcapacity );
            memset( &str[i+1], '\0', memcapacity );
            memcapacity *= 2;
        }
    } 
    if( Negative )
        str[i++] = '-';
    str[i] = '\0';  //to be very safe with strings
    char reversing[i+1];  //to reverse str.
    int j=0;
    for ( j=0; 0<i; j++, i-- ) {
        reversing[j] = str[i-1];
    }
    reversing[j] = '\0';
    for ( i=0; i < strlen( reversing ); i++ ) {
        str[i] = reversing[i];
    }
    return str;
}

int my_printf( char * restrict format, ... )
{
    va_list arg;
    va_start( arg, format );
    char *forstrings;
    char *buff = (char*) malloc( sizeof(char) * ( strlen( format ) + 1 ) );
    long long memory = sizeof(char) * ( strlen( format ) + 1 );
    int crntindex=0, buffsize=0, checkpercnt;   //crntindex - current index of 'format' string
                                                //check - to trace consequent occurences of <%> sign, divided by spaces.
    unsigned long pointercase;                  //to convert pointer's value to a typecasted uns. int value
    _Bool dflt = 0;                             //needed to show if some special cases were run.
    void *ptrcaseptr;
    char *little = (char*) malloc( sizeof(char) * 20 );  //just an arbitrary size. if needed, little reallocs to a greater size
    memset( little, '\0', 20 );                 //Not to cause some errors with string size
    for ( ; format[crntindex] != '\0'; crntindex++, buffsize++ ) {
        if ( format[crntindex] == '%' ) {
            dflt = 0;
            crntindex++;                         //just to not to write format[crntindex+1] below
            switch( format[crntindex] ) {
            case '%' :
                    dflt = 1;
                    buff[buffsize]='%';
                    break;
            case 'c' :
                    dflt = 1;
                    buff[buffsize] = (char) va_arg( arg, int );
                    break;
            case 'u' :
            case 'i' :
            case 'd' :
                    itoa( va_arg( arg, int ), little, 10 );
                    break;
            case 'o' :
                    itoa( va_arg( arg, int ), little, 8 );
                    break;
            case 'x' :
                    itoa( va_arg( arg, int ), little, 16 );
                    break;
            case 'p' :
                    ptrcaseptr = va_arg( arg, void* );
                    if( ptrcaseptr != NULL ) {
                        pointercase = (unsigned long) ptrcaseptr;
                        little[0] = '0';
                        little[1] = 'x';
                        char* tmp = (char*) malloc( sizeof(char) * 20 );
                        itoa( pointercase, tmp, 16 );
                        my_strcpy( &little[2], tmp );
                        free( tmp );
                    }
                    else 
                        my_strcpy( little, "(nil)\0" );
                    break;
                
            case 's' :
                    forstrings = va_arg( arg, char* );
                    if ( forstrings != NULL ) {
                        size_t lenght = strlen( forstrings );
                        if( lenght > 20 )
                            little = (char*) malloc( sizeof(char) * ( lenght + 1 ) );
                        my_strcpy( little, forstrings );
                        little[lenght] = '\0';
                    }
                    else 
                        my_strcpy( little, "(null)\0" );
                    break;
            default :
                    dflt = 1;
                    checkpercnt = crntindex;
                    while( format[checkpercnt] == ' ' ){
                        if ( format[checkpercnt + 1] == '%' && format[checkpercnt] != '%' ) {
                            crntindex = checkpercnt + 1;
                        }
                        checkpercnt++;
                    }
                    buff[buffsize] = '%';
            }
            if( !dflt ) {
                buff = (char*) my_realloc( buff, sizeof(char) * strlen( little ) + memory, memory );
                memory += sizeof(char) * strlen( little );
                my_strcpy( &buff[buffsize], little );
                buffsize = buffsize + strlen( little ) - 1;
            }
        }
        else
            buff[buffsize] = format[crntindex];
    }
    buff[buffsize] = '\0';
    write( 1, buff, sizeof(char) * buffsize );
    va_end( arg );
    free( buff );
    free( little );
    return buffsize;
}
/*
int main()
{
    int o = -5;
    int * p = &o, *nl = NULL, *zero = 0;
    char litl[30] = "ARAFbadadaf";
    int k = my_printf("Mart\n %s %d   %p  %x    %p  %s \n",litl, o, p, nl, zero, NULL);
    int z = printf("Mart\n %s %d   %p  %x    %p  %s \n",litl, o, p, nl, zero, NULL);
    printf("\n%i %i", k, z);
}*/
//Hi! Hope this one is more readeable
