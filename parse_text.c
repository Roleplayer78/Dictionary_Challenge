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

// Static function declarations
static unsigned long compareWord(WordElement *word_p, char *word);
static void purgeWord(char *word, unsigned char *len);


int parseText(FILE *doc_fd, DictionaryElement *dictionary)
{
    int rc = OK;
    
    // Should never happen....
    assert(dictionary != NULL);
    
    if (doc_fd != NULL)
    {
        char *token = NULL;
        unsigned int line = 1;
        ssize_t len = 0;
        size_t size = 0;

        // Reading trough the text
        while((len = getline(&token, &size, doc_fd)) != -1)
        {
            // Removing endline, we don't like or need it
            // If string is only endline, we just continue
            if (token[0] != '\n')
            {
                char *saveptr = NULL;
                char *word = NULL;
                strtok(token, "\n");

                word = strtok_r(token, " ", &saveptr);
                do
                {
                    // We can end up with word == NULL. This is not healthy
                    if (word != NULL)
                    {
                        // Accept only alphabetic characters, this is a design
                        // decision.
                        if (isalpha(word[0]))
                        {
                            // Convert first letter to array index
                            unsigned char index = tolower(word[0])-97;

                            // Be sure to have a valid letter for starting (Aa-Zz)
                            // before accessing dictionary array, even if we
                            // should be safe here after the isalpha check
                            if (index >= 0 && index < ALPHABET_SIZE)
                            {
                                unsigned char match = 0;
                                // We can end up having a letter not populated 
                                // in the dictionary
                                if (dictionary[index].first != NULL)
                                {
                                    WordElement *local = dictionary[index].first;
                                    unsigned char wordLen = strlen(word);
                        
                                    // Clear "." and ",", so we get less false negative
                                    purgeWord(word, &wordLen);

                                    while(local != NULL)
                                    {
                                        // If length doesn't match, wrong word
                                        if (local->length == wordLen)
                                        {
                                            if ((match = compareWord(local, word)) == 1)
                                            {
                                                break;
                                            }
                                        }
                                        local = local->next;
                                    }
                                }
                                if (!match)
                                {
                                    printf("INFO: Mispelled word=[%s] at line=[%u]\n", word, line);
                                }
                            }
                        }
                        else
                        {
                            printf("INFO: Malformed word=[%s] at line=[%u]\n", word, line);
                        }
                    }
                }while((word = strtok_r(NULL, " ", &saveptr)) != NULL);
                
                // Avoid memory leak with free and reset pointers to start clean
                free(token);
                token = NULL;
                saveptr = NULL;
            }
            
            // Use this to tell on what line the error is
            line++;
            
            if (rc == NOK)
            {
                break;
            }
        }
    }
    else
    {
        rc = NOK;
    }
    return rc;
}

/******************************************************************************
 * purgeWord
 * 
 * @param char *word word to purge
 * 
 * This function just try to make the spellcheck smarter, by removing some 
 * common delimeter
 */
static inline void purgeWord(char *word, unsigned char *len)
{
    switch (word[*len -1])
    {
        case ',':
        case '.':
        case '?':
        case '!':
        case ':':
        case ';':
            word[*len-1]=0;
            *len -= 1;
            break;
        default:
            break;
    }
}

/******************************************************************************
 * compareWord
 * 
 * @param WordElement *word_p dictionary word to compare against
 * @param char *word word to check
 * 
 * This function is very simple: 
 * it loops for the length of the whole string comparing lower case chars.
 * As soon as a char differ it quits
 * 
 * If the loops end comparing all chars, a match has been found
 */
#ifdef HASH_DICTIONARY
static inline unsigned long compareWord(WordElement *word_p, char *word)
{
    //printf("%s: Comparing 0x%lX == 0x%lX\n", __func__, word_p->word, hashWord(word));
    return (word_p->word == hashWord(word));
}
#else
static inline unsigned long compareWord(WordElement *word_p, char *word)
{
    int match = 1;
    unsigned char w1 = 0, w2 = 0;
    char *dw = word_p->word, *lc = word;
    
    assert(word_p != NULL);
    assert(word != NULL);

    // I've indulged myself on a recursive function, but timing was awful:
    //    * recursive time = ~6.5secs
    //    * strcmp time = ~5.5 secs
    //    * current time = ~1.4 secs
    while ((w1 = *dw++) && (w2 = *lc++))
    {
        if (tolower(w1) != tolower(w2))
        {
            match = 0;
            break;
        }
    }
    return match;
}
#endif
