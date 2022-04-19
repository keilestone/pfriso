/* pfriso extension for PHP */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_pfriso.h"
#include "main/php_scandir.h"
#include "pfriso_arginfo.h"

#include "ext/standard/file.h"
#include "ext/standard/flock_compat.h"

// static friso_hash_t friso_dic_hashtable;

#define PFRISO_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(pfriso, v)

ZEND_DECLARE_MODULE_GLOBALS(pfriso); // zend_pfriso_globals pfriso_globals;

zend_class_entry *pfriso_class_ce;

// HashTable *global_site_id;

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE()  \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

/* }}}*/

#define PALLOC_HASHTABLE(ht)                                \
	do                                                      \
	{                                                       \
		(ht) = (HashTable *)pemalloc(sizeof(HashTable), 1); \
	} while (0)

/* {{{ php_robbe_globals_construct */
static void php_pfriso_globals_construct(zend_pfriso_globals *pfriso_globals_ptr)
{
	pfriso_globals_ptr->config = friso_new_config();
	friso_init_from_ifile(pfriso_globals_ptr->config, PFRISO_G(i_file));
}
/* }}} */

static void php_pfriso_load_dict(zend_pfriso_globals *pfriso_globals_ptr, friso_dic_t dict, fstring dirname) /* {{{ */
{
	friso_dic_load_from_lex_file(dict, pfriso_globals_ptr->config, dirname,
								 pfriso_globals_ptr->config->max_len * 3);
}
/* }}} */

static void php_pfiso_init_dict(friso_hash_t friso_dic_hash, HashTable *ht, char *key, friso_dic_t dict)
{
	zend_string *filename = zend_string_init(key, strlen(key), 1);

	zend_hash_next_index_insert(ht, filename);
}

static void php_pfriso_free_dic_hash(hash_entry_t entry)
{
	friso_dic_free(entry->_val);
}

/* {{{ php_robbe_globals_destruct*/
static void php_pfriso_globals_destruct(zend_pfriso_globals *pfriso_globals_ptr)
{
	/*
	 * cause friso_free will free the dictionary
	 *        so here we don't have to call the friso_dic_free to free the
	 *    the robbe_dic global variable.
	 */
	// friso_dic_free( friso_globals->friso_dic );
	// friso_globals->friso_dic = NULL;
	friso_free_config(pfriso_globals_ptr->config);
	// friso_free( pfriso_globals_ptr->friso );

	if (PFRISO_G(friso_dic_hashtable))
	{
		// zval *val;
		// ZEND_HASH_FOREACH_VAL(global_site_id, val) {
		// 	char* key;

		// 	zend_string* str;
		// 	ZVAL_STR(&val, str);

		// } ZEND_HASH_FOREACH_END();

		free_hash_table(PFRISO_G(friso_dic_hashtable), php_pfriso_free_dic_hash);
	}
}
/* }}} */

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
// STD_PHP_INI_ENTRY这个宏的参数 配置名称，默认值，修改权限, 判断函数指针，对应的全局变量名, 全局struct type, global struct
/**
 * PHP_INI_SYSTEM 这个表示只能在ini文件里面修改
 * PHP_INI_PERDIR 表示 httpd.conf or .htaccess (if AllowOverride OPTIONS is set) using the php_flag/php_value directives
 * PHP_INI_USER 通过ini_set()来设定
 * PHP_INI_ALL 为所欲为
 *
 * 默认的判断函数
 * OnUpdateBool
 * OnUpdateInt
 * OnUpdateReal
 * OnUpdateString
 * OnUpdateStringUnempty
 */
STD_PHP_INI_ENTRY("pfriso.ini_file", "", PHP_INI_SYSTEM, OnUpdateString, i_file, zend_pfriso_globals, pfriso_globals)
STD_PHP_INI_ENTRY("pfriso.dic_directory", "", PHP_INI_SYSTEM, OnUpdateString, d_directory, zend_pfriso_globals, pfriso_globals)
PHP_INI_END()
/* }}} */

// #define Z_PFRISOOBJ_P(zv)   (php_pfriso_fetch_object(Z_OBJ_P(zv)))
// static inline pfriso_object *php_pfriso_fetch_object(zend_object *obj) /* {{{ */ {
// 	return (pfriso_object *)((char*)(obj) - XtOffsetOf(pfriso_object, std));
// }

/* {{{ PHP_GINIT_FUNCTION
 */
PHP_GINIT_FUNCTION(pfriso)
{
	pfriso_globals->i_file = NULL;
}
/* }}} */

#define FRISO_RET_WORD (1 << 0)
#define FRISO_RET_TYPE (1 << 1)
#define FRISO_RET_OFF (1 << 2)
#define FRISO_RET_LEN (1 << 3)
#define FRISO_RET_RLEN (1 << 4)
#define FRISO_RET_POS (1 << 5)

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(pfriso)
{
	REGISTER_LONG_CONSTANT("FRISO_SIMPLE", __FRISO_SIMPLE_MODE__, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRISO_COMPLEX", __FRISO_COMPLEX_MODE__, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRISO_DETECT", __FRISO_DETECT_MODE__, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRISO_LEX_CJK", __LEX_CJK_WORDS__, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRISO_LEX_STOP", __LEX_STOPWORDS__, CONST_CS | CONST_PERSISTENT);

	// return parts for rb_split.
	REGISTER_LONG_CONSTANT("FRISO_RET_WORD", FRISO_RET_WORD, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRISO_RET_TYPE", FRISO_RET_TYPE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRISO_RET_OFF", FRISO_RET_OFF, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRISO_RET_LEN", FRISO_RET_LEN, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRISO_RET_RLEN", FRISO_RET_RLEN, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRISO_RET_POS", FRISO_RET_POS, CONST_CS | CONST_PERSISTENT);

	// lex type constants.
	REGISTER_LONG_CONSTANT("FRISO_TYP_CJK", __LEX_CJK_WORDS__, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRISO_TYP_ECM", __LEX_ECM_WORDS__, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRISO_TYP_CEM", __LEX_CEM_WORDS__, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRISO_TYP_EPUN", __LEX_ENPUN_WORDS__, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRISO_TYP_PUN", __LEX_OTHER_WORDS__, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRISO_TYP_UNK", __LEX_UNKNOW_WORDS__, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRISO_TYP_OTR", __LEX_OTHER_WORDS__, CONST_CS | CONST_PERSISTENT);

	REGISTER_INI_ENTRIES();
	/*
	zend_class_entry 是内核中定义的一个结构体，是内核实现 PHP 语言中类与对象的一个非常基础、关键的结构类型，相当于我们定义类的原型。
	zend_register_internal_class_ex 这个带有 _ex 后缀的宏，这个宏的第二个参数zend_exception_get_default（注意头文件）指定了父类
	*/
	pfriso_class_ce = register_class_Pfriso();

	php_pfriso_globals_construct(&pfriso_globals);

	const char *dirname;
	size_t dirlen;

	// PALLOC_HASHTABLE(global_site_id);
	// zend_hash_init(global_site_id, 256, NULL, NULL, 1);

	PFRISO_G(friso_dic_hashtable) = new_hash_table();

	if ((dirname = PFRISO_G(d_directory)) && (dirlen = strlen(dirname)))
	{
		zval result;
		int ndir;
		struct dirent **namelist;

		if ((ndir = php_scandir(dirname, &namelist, 0, php_alphasort)) > 0)
		{
			uint32_t i;

			for (i = 0; i < ndir; i++)
			{
				struct dirent **dic_file;
				int dic_num;

				char sub_dirname[MAXPATHLEN];

				zend_stat_t sb;

				sprintf(sub_dirname, "%s/%s", dirname, namelist[i]->d_name);

				if (VCWD_STAT(sub_dirname, &sb) == 0)
				{
					if (S_ISDIR(sb.st_mode))
					{
						//站点文件夹下的配置文件
						if ((dic_num = php_scandir(sub_dirname, &dic_file, 0, php_alphasort)) > 0)
						{
							uint32_t j;
							for (j = 0; j < dic_num; j++)
							{
								if (strcmp(dic_file[j]->d_name, "lex-keyword.lex"))
								{
									free(dic_file[j]);
									continue;
								}

								char lex_dirname[MAXPATHLEN];

								snprintf(lex_dirname, MAXPATHLEN, "%s/%s/%s", dirname, namelist[i]->d_name, dic_file[j]->d_name);

								friso_dic_t dict = friso_dic_new();

								php_pfriso_load_dict(&pfriso_globals, dict, lex_dirname);

								zend_string *filename = zend_string_init(namelist[i]->d_name, strlen(namelist[i]->d_name), 1);

								// php_pfiso_init_dict(friso_dic_hashtable, key, dict);
								hash_put_mapping(PFRISO_G(friso_dic_hashtable), ZSTR_VAL(filename), dict);
							}
						}

						free(dic_file);
					}
				}
				free(namelist[i]);
			}

			free(namelist);
		}
	}

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(pfriso)
{
#if defined(ZTS) && defined(COMPILE_DL_PFRISO)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(pfriso)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "pfriso support", "enabled");
	php_info_print_table_end();
}
/* }}} */

ZEND_METHOD(Pfriso, cut)
{
	zend_string *site_id;
	zend_string *word;

	zval ret_val;

	// php_pfriso_init_array(ret_val, 8);

	// PALLOC_HASHTABLE(ret_val);
	// zend_hash_init(ret_val, 8, NULL, NULL, 1);

	ZEND_PARSE_PARAMETERS_START(2, 2)
	Z_PARAM_STR(site_id)
	Z_PARAM_STR(word)
	ZEND_PARSE_PARAMETERS_END();

	char *sid = ZSTR_VAL(site_id);

	array_init(&ret_val);

	fprintf(stderr, "%s", sid);

	if (!hash_exist_mapping(PFRISO_G(friso_dic_hashtable), sid))
	{
		RETURN_ZVAL(&ret_val, 0, 0);
		return;
	}

	friso_t copy_friso = friso_new();

	friso_dic_t dic_from_hashmap = hash_get_value(PFRISO_G(friso_dic_hashtable), sid);

	copy_friso->dic = dic_from_hashmap;

	char *_str = ZSTR_VAL(word);

	friso_task_t task;
	task = friso_new_task();
	friso_set_text(task, _str);

	while (pfriso_globals.config->next_token(copy_friso, pfriso_globals.config, task) != NULL)
	{
		zend_string *text = zend_string_init(task->token->word, task->token->length, 0);

		add_next_index_str(&ret_val, text);
	}

	friso_free_task(task);

	copy_friso->dic = NULL;

	friso_free(copy_friso);

	RETURN_ZVAL(&ret_val, 0, 0);
}

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(pfriso)
{
	UNREGISTER_INI_ENTRIES();

	php_pfriso_globals_destruct(&pfriso_globals);

	return SUCCESS;
}
/* }}} */

/* {{{ pfriso_module_entry */
zend_module_entry pfriso_module_entry = {
	STANDARD_MODULE_HEADER,
	"pfriso",			   /* Extension name */
	NULL,				   /* zend_function_entry */
	PHP_MINIT(pfriso),	   /* PHP_MINIT - Module initialization */
	PHP_MSHUTDOWN(pfriso), /* PHP_MSHUTDOWN - Module shutdown */
	NULL,				   /* PHP_RINIT - Request initialization */
	NULL,				   /* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(pfriso),	   /* PHP_MINFO - Module info */
	PHP_PFRISO_VERSION,	   /* Version */
	STANDARD_MODULE_PROPERTIES};
/* }}} */

#ifdef COMPILE_DL_PFRISO
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(pfriso)
#endif
