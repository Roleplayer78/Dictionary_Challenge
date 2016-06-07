#ifndef _DICTIONARY_H
#define _DICTIONARY_H

/*******************************************************************************
 * DICTIONARY DESIGN - Linked list structure
 * 
 * The dictionary DB will be grouped by initial letter of every word, so to
 * optimize the search algorithm, and avoid looping all the words every time
 * 
 * The dictionary will not differentiate between capital/non-capital:
 *     I.E. Hello, hello and hellO will be the same word
 * 
 * The words must start with an alphabet letter (Aa-Zz) or will be discarded
 * by the dictionary and the document and malformed
 * 
 * Every DictionaryElement will be single linked list, since we should not need
 * to search backwards
 * 
 * Every word is allocated dynamically
 *      NOTE: for sake of simplicity I allocate every single word, but we could
 *     optimize and allocate a number of words (elements) in one pass
 *
 *   +-----------+ +-----------+ +-----------+
 *   |Initial A  | |Initial B  | |Initial C  |
 *   +-----------+ +-----------+ +-----------+
 *   |All        | |Best       | |Call       | <- This array will be statically
 *   |           | |           | |           |     initialized
 *   +-----------+ +-----------+ +-----------+
 *         |             |             |
 *         |             |             |
 *   +-----v-----+ +-----v-----+ +-----v-----+
 *   |           | |           | |           |
 *   |Another    | |Brother    | |Class      |
 *   |           | |           | |           |
 *   |           | |           | |           |
 *   +-----------+ +-----------+ +-----------+
 *         |             |             |
 *         |             |             |
 *   +-----v-----+ +-----v-----+ +-----v-----+
 *   |           | |           | |           |
 *   |Angel      | |Bye        | |Cludge     |
 *   |           | |           | |           |
 *   |           | |           | |           |
 *   +-----------+ +-----------+ +-----------+
 *
 * 
 ******************************************************************************/

#define ALPHABET_SIZE   26  // Size of the alphabet size
#define MAX_WORD_LENGTH 100 // Maximum word length

typedef struct WordT{
#ifdef HASH_DICTIONARY
    unsigned long        word;    // The first letter of a word
    struct      WordT   *left;    // Next word in the dictionary
    struct      WordT   *right;    // Next word in the dictionary
#else
    char                *word;    // The first letter of a word
    struct      WordT   *next;    // Next word in the dictionary
    unsigned char       length;   // Word length
#endif


}WordElement;

typedef struct {
    unsigned char   initial;    // The first letter of a word
    WordElement     *first;     // Pointer to first word for this letter
#ifdef HASH_DICTIONARY
    WordElement     *last_left;
    WordElement     *last_right;
#else
    WordElement     *last_add;  // Pointer to last inserted element, will save time
                                // when we will populate the dictionary
#endif
}DictionaryElement;


/* 
 * initializeDictionary
 * 
 * Initialize the static array of the dictionary.
 * Remember, we assume that only letters from Aa-Zz are supported.
 * 
 * @param DictionaryElement * dictionary[] pointer to dictionary root
 * 
 */
int initializeDictionary(DictionaryElement *dictionary);


/* 
 * populateDictionary
 * 
 * Scan trough the dictionary file and fill the linked list, starting from
 * every root element for each and every letter (a,b,c,...).
 * 
 * It is allowed to have empty letters (I.E. no words under "d", or "l", ecc).
 * If we don't have letters populated all words starting with that letter will
 * be handled as "misspelled"
 * 
 * @param DictionaryElement * dictionary[] pointer to dictionary
 * @param FILE              * dict_fd pointer to the dictionary file
 * 
 */
int populateDictionary(FILE *dict_fd, DictionaryElement *dictionary);


/* 
 * deallocateDictionary
 * 
 * Deallocate all the dictionary from memory when program exit
 * 
 * @param DictionaryElement * dictionary[] pointer to dictionary
 * 
 */
void deallocateDictionary(DictionaryElement *dictionary);


/* 
 * parseDictionary
 * 
 * debug function to print all words in the dictionary, essentially a dump
 * of the linked list
 * 
 * @param char * argv[] Array containing path to files (dict and doc
 * 
 */
void parseDictionary(DictionaryElement *dictionary);

/* 
 * parseDictionary
 * 
 * debug function to print all words in the dictionary, essentially a dump
 * of the linked list
 * 
 * @param char * argv[] Array containing path to files (dict and doc
 * 
 */
#ifdef HASH_DICTIONARY
unsigned long hashWord(char * str);
#endif

#endif // _DICTIONARY_H


