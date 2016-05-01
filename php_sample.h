#ifndef PHP_SAMPLE_H
#define PHP_SAMPLE_H

/* Define extension properties */
#define PHP_SAMPLE_EXTNAME    "sample"
#define PHP_SAMPLE_EXTVER     "1.0"

/* Import configure options 
   when building outside the PHP
   source tree */
#ifndef HAVE_CONFIG_H
#include "config.h"
#endif

/* Include the php std header */
#include "php.h"

/* Define the entry point symbol
 * Zend will use when loading this module
 */
extern zend_module_entry sample_module_entry;
#define phpext_sample_ptr &sample_module_entry

#endif /* PHP_SAMPLE_H */
