/* pfriso extension for PHP */

#ifndef PHP_PFRISO_H
# define PHP_PFRISO_H

#include "friso/src/friso_API.h"
#include "friso/src/friso.h"


extern zend_module_entry pfriso_module_entry;
# define phpext_pfriso_ptr &pfriso_module_entry

# define PHP_PFRISO_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_PFRISO)
ZEND_TSRMLS_CACHE_EXTERN()
# endif


#endif	/* PHP_PFRISO_H */


ZEND_BEGIN_MODULE_GLOBALS(pfriso)
	char *i_file;
    char *d_directory;
	friso_config_t config;
    friso_hash_t friso_dic_hashtable;
ZEND_END_MODULE_GLOBALS(pfriso)