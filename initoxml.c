/*
 * @Descripttion: 
 * @version: 
 * @Author: MYX
 * @Date: 2020-12-29 09:40:16
 * @LastEditors: MYX
 * @LastEditTime: 2020-12-30 09:23:56
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COL    1024

char *shrink_space(char *dest, const char *src, size_t n)
{
    size_t i = 0;
    size_t j = 0;

    for(i=0; i<n; i++)
    {
        switch (*(src + i))
        {
            case ' ':
            case '\t':       
            case '\n':
            case '\r':
                break;
            
            default:
                *(dest + j) = *(src + i);
                j++;            
                break;
        }
    }

    return dest;
}

char *label_head(const char *instr, char *outstr)
{
    strcat(outstr, "<");
    strcat(outstr, instr);
    strcat(outstr, ">");
    return outstr;
}

char *label_tail(const char *instr, char *outstr)
{
    strcat(outstr, "</");
    strcat(outstr, instr);
    strcat(outstr, ">");
    return outstr;    
}

char *set_commet(char *inidata, char *outdata)
{
    char *token = NULL;    

    token = strtok(inidata, ";");
    token = strtok(token, "\r\n");
    strcpy(outdata, "<!-- ");
    strcat(outdata, token);
    strcat(outdata, " -->");  
    strcat(outdata, "\n");
    return outdata;
}

char *start_section(char *inidata, char *outdata)
{
    char *token = NULL; 

    token = strtok(inidata, "[");
    token = strtok(token, "]");
    strcpy(outdata, "<");
    strcat(outdata, token);
    strcat(outdata, ">");
    strcat(outdata, "\n");
    return outdata;
}

char *set_keyvalue(char *inidata, char *outdata)
{
    char inibuff[MAX_COL] = {0};
    char tail[MAX_COL] = {0};
    char *key = NULL;
    char *value = NULL;

    shrink_space(inibuff, inidata, MAX_COL);

    value = strchr(inibuff, '=') + 1;
    key = strtok(inibuff, "=");
    

    strcpy(outdata, "\t");
    label_head(key, outdata);
    strcat(outdata, value);
    label_tail(key, tail);
    strcat(outdata, tail);
    strcat(outdata, "\n");   
    return outdata;
}

char *end_section(char *inidata, char *outdata)
{
    char *token = NULL; 

    token = strtok(inidata, "[");
    token = strtok(token, "]");
    strcpy(outdata, "</");
    strcat(outdata, token);
    strcat(outdata, ">");
    strcat(outdata, "\r\n");
    return outdata;    
}

int main()
{
    FILE *fp1;
    FILE *fp2;


    char inibuff[MAX_COL];
    char section[MAX_COL];
    char xmlbuff[MAX_COL];
    

    if((fp1 = fopen("config.ini", "r")) == NULL)
    {
        perror("open config.ini failed\n");
        exit(1);
    }

    if((fp2 = fopen("config.xml", "w+")) == NULL)
    {
        perror("open config.xml failed\n");
        exit(1);        
    }

    while(fgets(inibuff, MAX_COL, fp1) != NULL)
    {
        if(strchr(inibuff, ';') != NULL)
        {
            memset(xmlbuff, 0, MAX_COL);
            set_commet(inibuff, xmlbuff);
            fputs(xmlbuff, fp2);
        }
        else if(strchr(inibuff, '[') != NULL)
        {
            memset(xmlbuff, 0, MAX_COL);
            strcpy(section, inibuff);            
            start_section(inibuff, xmlbuff);
            fputs(xmlbuff, fp2);
        }
        else if(strchr(inibuff, '=') != NULL)
        {
            memset(xmlbuff, 0, MAX_COL);            
            set_keyvalue(inibuff, xmlbuff);
            fputs(xmlbuff, fp2);            
        }
        else if(strchr(inibuff, '\n') != NULL)
        {
            if(strlen(section) > 0)
            {
                memset(xmlbuff, 0, MAX_COL);
                end_section(section, xmlbuff);
                fputs(xmlbuff, fp2); 
                memset(section, 0, MAX_COL);
            }
        }
        
    }
    if(strlen(section) > 0)
    {
        memset(xmlbuff, 0, MAX_COL);
        end_section(section, xmlbuff);
        fputs(xmlbuff, fp2); 
        memset(section, 0, MAX_COL);        
    }

    fclose(fp1);
    fclose(fp2);

    return 0;
}