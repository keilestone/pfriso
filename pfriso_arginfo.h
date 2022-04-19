/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 3d125bef6fa55a784a999196cf20582a8371abc5 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Pfriso_cut, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, siteId, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, word, IS_STRING, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(Pfriso, cut);

static const zend_function_entry class_Pfriso_methods[] = {
	ZEND_ME(Pfriso, cut, arginfo_class_Pfriso_cut, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_Pfriso(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "Pfriso", class_Pfriso_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	return class_entry;
}
