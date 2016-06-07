Short notes - could be obsolete ... 

* Find attached the code for the test you sent me.
  I've tried to document the code, but is something I normally do, no more, no less.

* The linked list of how the dictionary is organized is briefly documented in 
  dictionary.h with some simple ascii diagram.

* Follow some assumptions & limitations:

** getline -> this function is POSIX.2008, so I tought was ok to use.
   for very old compiler or platform that doesn't support it, can be
   easily implemented in C

** As dictionary I assumed a spellcheck of only alphabetic character

** I assumed ASCII character set

** I assumed enough RAM is present in the system

** I made some basic optimiztion regarding the dictionary by braking it
   using letters, but I guess could be improved

** Potentially we could extend the dictionary to support mutiple languages

** Bugs & corner case: well, yes. I expect a few.

** "parseDictionary" could uncommented and use it for dumping the dictionary

** I removed all debug print, as requested

** In spellcheck.c I've made the functions "static since they are not used
   anywhere else and the compiler can make some minor optimization.
   Also make sure things remain cleaner and at the right scope

** Probably a couple of include file are not used in some of the .c and could
   be removed, to help the compiler and keep compilation smooth and clean.

** The creation of dictionary could be smarter:

*** Will create a new entry for entry, even if already exist
*** Will not alphabetically order the dictionary

*** The assumption is that in this case is more time consuming smartening the
*** dictionary creation then checking the document.
*** Ideal, is much more efficient that who create dictionary.txt will avoid
*** doubles and order the list

*** Dictionary support words like "don't", "We'll" etc, as long as they start
*** with a letter

* Test results

** On my local machine (Intel i3 2Core 1.6Ghz) the long test took:
ireal	0m5.886s
user	0m5.768s
sys	0m0.068s

NOTE: I've purged all kernel caches before running
