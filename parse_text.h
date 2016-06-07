#ifndef _PARSE_TEXT_H
#define _PARSE_TEXT_H

/* 
 * parseText
 * 
 * Execute the parsing of the document searching for words not in the 
 * dictionary.
 * 
 * @param const FILE *doc_fd File pointer to document to spellcheck
 * @param DictionaryElement * dictionary[] pointer to dictionary
 * 
 */
int parseText(FILE *doc_fd, DictionaryElement *dictionary);

#endif // _PARSE_TEXT_H