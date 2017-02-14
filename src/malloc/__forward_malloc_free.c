/* __forward_malloc(...) never frees. */
void __forward_malloc_free(void *ptr) { (void)ptr; }
/* This can result in link error: multiple definition of free,
 * if the program also defines it.
 */
void __xtiny_free(void *ptr) __attribute__((alias("__forward_malloc_free")));
