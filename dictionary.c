// System include
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>


// Project include
#include "spellcheck.h"
#include "dictionary.h"
#include "parse_text.h"

int initializeDictionary(DictionaryElement *dictionary)
{
    int rc = OK;
    unsigned char letter = 0;
    
    // Should never happen....
    assert(dictionary != NULL);
    
    for( ; letter < ALPHABET_SIZE; letter++ )
    {
        dictionary[letter].initial = letter+97;
        dictionary[letter].first = NULL;
    }
    
    return rc;
}

int populateDictionary( FILE *dict_fd, DictionaryElement *dictionary)
{
    int rc = OK;
    
    // Should never happen....
    assert(dictionary != NULL);
    
    if (dict_fd != NULL)
    {
        char *token = NULL;
        ssize_t len = 0;
        size_t size = 0;

        while((len = getline(&token, &size, dict_fd)) != -1)
        {
            // Removing endline, we don't like or need it
            // If string is only endline, we just continue
            if (token[0] != '\n')
            {
                strtok(token, "\n");

                // Accept only alphabetic characters, remember
                if (isalpha(token[0]))
                {
                    // Convert first letter to array index
                    unsigned char index = tolower(token[0])-97;

                    // We check that we have a valid letter for starting (Aa-Zz)
                    // before accessing dictionary array
                    if (index >= 0 && index < ALPHABET_SIZE)
                    {

                        if (dictionary[index].first == NULL)
                        {
                            dictionary[index].first = (WordElement *)malloc(sizeof(WordElement));
                            if (dictionary[index].first != NULL)
                            {
                                dictionary[index].last_add = dictionary[index].first;

                                // Getline will allocate the memory for us and strtok
                                // will remove the newline, so the string should be ok
#ifdef HASH_DICTIONARY
                                dictionary[index].first->word = hashWord(token);
#else                                
                                dictionary[index].first->word = token;
#endif
                                dictionary[index].first->length = strlen(token);
                                dictionary[index].first->next = NULL;
                                //printf("Word read %s\n", token);
                            }
                            else
                            {
                                rc = NOK;
                                printf("ERROR: Dictionary out of memory. Aborting.\n");
                            }
                        }
                        else
                        {
                            // Using the last_add pointer we can jump on last element
                            // without need of scanning the whole list that, in a 
                            // real world scenario, can become quite big.
                            // This is important since the dictionary could well
                            // be not in alphabetic order
                            WordElement *element = dictionary[index].last_add;

                            element->next = (WordElement *)malloc(sizeof(WordElement));
                            if (element->next != NULL)
                            {
                                // Token should be fine now
#ifdef HASH_DICTIONARY
                                element->next->word = hashWord(token);
#else                                
                                element->next->word = token;
#endif
                                element->next->length = strlen(token);

                                element->next->next = NULL;
                                dictionary[index].last_add = element->next;
                                //printf("Word read %s\n", token);
                            }
                            else
                            {
                                rc = NOK;
                                printf("ERROR: Dictionary out of memory. Aborting.\n");
                            }
                        }
                    }
                }
                else
                {
                    printf("WARNING: dictionary discarded [%s], but continuing.\n", token);
                }
            }
            // This will make sure getline will allocate buffer for us
            token = NULL;
        }
    }
    else
    {
        rc = NOK;
    }
        
    return rc;
}

void deallocateDictionary(DictionaryElement *dictionary)
{
    assert(dictionary != NULL);

    for (int index = 0; index < ALPHABET_SIZE; index++)
    {
        WordElement *local = dictionary[index].first;

        while (local != NULL)
        {
            WordElement *delete = local;
            local = local->next;
            //printf("Deleted %s\n", delete->word);
#ifndef  HASH_DICTIONARY
            free(delete->word);
#endif
            free(delete);
        }
    }
}

void parseDictionary(DictionaryElement *dictionary)
{
    for (int index = 0; index < ALPHABET_SIZE; index++)
    {
        WordElement *local = dictionary[index].first;

        printf("DUMPING LETTER %c ### \n", dictionary[index].initial);
        sleep(1);

        while (local != NULL)
        {
#ifdef HASH_DICTIONARY
            printf("Word has 0x%X\n", (unsigned int)local->word);
#else
            printf("%s\n", local->word);
#endif

            local = local->next;
        }
    }
}

#ifdef HASH_DICTIONARY
unsigned long hashWord(char * str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + tolower(c); /* hash * 33 + c */
    }

    //printf("%s: Word has 0x%lX\n", __func__, hash);
    return hash;
}
#endif