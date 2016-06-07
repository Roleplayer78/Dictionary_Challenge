// System include
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Project include
#include "spellcheck.h"
#include "dictionary.h"
#include "parse_text.h"

// Static function declaration
static int openFiles(char *argv[], FILE **dict, FILE **doc);
static int closeFile(FILE **fd);



/*******************************************************************************
 * main
 ******************************************************************************/
int main (int argc, char *argv[])
{
    int rc = NOK;

    // We need 3 (and only) arguments to proceed
    if (argc == 3)
    {
        FILE *dict_fd = NULL, *doc_fd = NULL;

        if ((rc = openFiles(argv, &dict_fd, &doc_fd)) == OK)
        {
            DictionaryElement dictionary[ALPHABET_SIZE];
            
            // Initialize dictionary structure
            if ((rc = initializeDictionary(dictionary)) == OK)
            {
                // Parse the dictionary file and store it in memory
                if ((rc = populateDictionary(dict_fd, dictionary)) == OK)
                {
                    //parseDictionary(dictionary);

                    parseText(doc_fd, dictionary);
                }
            }

            deallocateDictionary(dictionary);
            closeFile(&dict_fd);
            closeFile(&doc_fd);
        }
    }
    else
    {
        printf("%s: invalid options\n", argv[0]);
        printf("usage: %s <dictionary> <document>\n", argv[0]);
        printf("\t<dictionary> text file of known words\n");
        printf("\t<document> text document to spell check\n");
    }
    
    exit(rc);
}



/*******************************************************************************
 * Static functions
 ******************************************************************************/

/* 
 * Open Dictionary and Document files
 * 
 * @param char * argv[] Array containing path to files (dict and doc)
 * @param FILE * dict File descriptor for the dictionary
 * @param FILE * doc File descriptor for the document
 * @return OK or NOK
 * 
 * Since this function will open both files, if one fopen fails, all files are
 * closed, so this function succeed or not.
 * 
 * Note: we use fopen family functions because they use some buffer optimizations
 * in the kernel that could results in faster/smoother disk I/O
 */
static int openFiles(char *argv[], FILE **dict_fd, FILE **doc_fd)
{
    int rc = OK;

    if ((*dict_fd = fopen(argv[1], "r")) != NULL)
    {
        // We open the document file only if dictionary is valid
        if ((*doc_fd = fopen(argv[2], "r")) == NULL)
        {
            rc = NOK;
            printf("ERROR: Can't open document %s: errno %d\n", argv[2], errno);
            
            fclose(*dict_fd);
            dict_fd = NULL;
        }
    }
    else
    {
        rc = NOK;
        printf("ERROR: Can't open dictionary %s: errno %d\n", argv[1], errno);
    }

    
    return rc;
}

/* 
 * Close Dictionary and Document files
 * 
 * @param FILE * dict File descriptor for the dictionary
 * @param FILE * doc File descriptor for the document
 * @return OK or NOK
 * 
 * NOTE: we are overzelous about printing errors on close, but devs should be
 * aware if resources are not cleanly released
 */
static int closeFile(FILE **fd)
{
    int rc = OK;
    
    if (fd != NULL)
    {
        rc = fclose(*fd);
        *fd = NULL;
        if (rc != OK)
        {
            printf("ERROR: Can't close dictionary file: errno %d\n", errno);
        }
    }    
    return rc;
}