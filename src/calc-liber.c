#include "calc-liber.h"

#pragma region System Dependent Libraries

#ifdef _WIN32

static SYSTEM_INFO _system_info = { 0 };

const int _getpagesiz()
{
	if (!_system_info.dwPageSize)
		GetSystemInfo(&_system_info);

	return (const int)_system_info.dwPageSize;
}

#endif // _WIN32

// +---- Output Functions

#pragma region Output Functions

// +---- Internal

#pragma region Internal

static inline int _fputln(FILE *const stream)
#ifdef _CALC_USE_CRLF
{
	return fputs("\r\n", stream);
}
#else
{
	return fputc('\n', stream), 1;
}
#endif // _WIN32

static inline int _putln()
#ifdef _CALC_USE_CRLF
{
	return puts("\r\n");
}
#else
{
	return putchar('\n'), 1;
}
#endif // _WIN32

static inline int _vfprintfn(FILE *const stream, const char *const format, va_list arglist)
{
	return vfprintf(stream, format, arglist) + _fputln(stream);
}

static inline int _vprintfn(const char *const format, va_list arglist)
{
	return vprintf(format, arglist) + _putln();
}

#pragma endregion

// +---- Internal -- End

int fputln(FILE *const stream)
{
	return _fputln(stream);
}

int putln()
{
	return _putln();
}

int fprint(FILE *const stream, const char *const message)
{
	return fputs(message, stream);
}

int print(const char *const message)
{
	return puts(message);
}

int fprintln(FILE *const stream, const char *const message)
{
	return fputs(message, stream) + _fputln(stream);
}

int println(const char *const message)
{
	return puts(message) + _putln();
}

int vfprintfn(FILE *const stream, const char *const format, va_list arglist)
{
	return _vfprintfn(stream, format, arglist);
}

int fprintfn(FILE *const stream, const char *const format, ...)
{
	int result;
	va_list arglist;

	va_start(arglist, format);
	result = _vfprintfn(stream, format, arglist);
	va_end(arglist);

	return result;
}

int vprintfn(const char *const format, va_list arglist)
{
	return _vprintfn(format, arglist);
}

int printfn(const char *const format, ...)
{
	int result;
	va_list arglist;

	va_start(arglist, format);
	result = _vprintfn(format, arglist);
	va_end(arglist);

	return result;
}

#pragma endregion

// +---- Output Functions -- End

#pragma endregion

/* =---- Memory Management -------------------------------------= */

#pragma region Memory Management

// +---- Internal

#pragma region Internal

static inline void *_safety_abort(
#ifdef CALC_DEBUG
	const char *const funcname
#endif // CALC_DEBUG
)
{
#ifdef CALC_DEBUG
	printf("error: function %s cannot allocate memory\n", funcname);
	exit(3);
#else
	raise(SIGSEGV);
	abort();
#endif // CALC_DEBUG

	return NULL;
}

static inline void *_safety_check(
#ifdef CALC_DEBUG
	const char *const funcname,
#endif // CALC_DEBUG
	void *block
)
{
	if (!block)
		return _safety_abort(
#ifdef CALC_DEBUG
			funcname
#endif // CALC_DEBUG
		);
	else
		return block;
}

static inline void *_zerofy_block(void *block, size_t size)
{
	assert(block != NULL);

	char *p = (char *)block;

	for (; size > 0; size--)
		p[size - 1] = 0x00;

	return (void *)p;
}

#pragma endregion

// +---- Internal -- End

void *malloc_s(size_t size)
{
	return _safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		malloc(size)
	);
}

void *calloc_s(size_t count, size_t size)
{
	return _safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		calloc(count, size)
	);
}

void *mallocz_s(size_t size)
{
	return _zerofy_block(_safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		malloc(size)
	), size);
}

void *callocz_s(size_t count, size_t size)
{
	return _zerofy_block(_safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		calloc(count, size)
	), count * size);
}

void *malloca_s(size_t size, size_t alignment)
{
	return _safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		malloc(alignto(size, alignment))
	);
}

void *calloca_s(size_t count, size_t size, size_t alignment)
{
	return _safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		calloc(count, alignto(size, alignment))
	);
}

void *mallocaz_s(size_t size, size_t alignment)
{
	return _zerofy_block(_safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		malloc(size = alignto(size, alignment))
	), size);
}

void *callocaz_s(size_t count, size_t size, size_t alignment)
{
	return _zerofy_block(_safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		calloc(count, size = alignto(size, alignment))
	), count * size);
}

// Strings

char *stralloc(size_t length)
{
	return _zerofy_block(_safety_check(
#ifdef CALC_DEBUG
		__func__,
#endif // CALC_DEBUG
		malloc(length + 1)
	), length + 1);
}

#pragma endregion

/* =---- Datatypes Management ----------------------------------= */

#pragma region Datatypes Management

// Strings

bool_t streq(const char *const str1, const char *const str2)
{
	if (str1 == str2)
		return TRUE;

	if (!str1 || !str2)
		return FALSE;

	size_t len = strlen(str1);

	if (len != strlen(str2))
		return FALSE;

	for (; len > 0; len--)
	{
		if (str1[len] != str2[len])
			return FALSE;
		else
			continue;
	}

	return TRUE;
}

bool_t strieq(const char *const str1, const char *const str2)
{
	if (str1 == str2)
		return TRUE;

	if (!str1 || !str2)
		return FALSE;

	size_t len = strlen(str1);

	if (len != strlen(str2))
		return FALSE;

	for (; len > 0; len--)
	{
		if ((str1[len] != str2[len]) && (str1[len] != (tolower(str2[len]))))
			return FALSE;
		else
			continue;
	}

	return TRUE;
}

char *strntolower(char *const dest, const char *const source, size_t length)
{
	char *buf;

	if (!dest)
		buf = stralloc(length);
	else
		buf = dest, buf[length + 1] = '\0';

	size_t i;

	for (i = length; i > 0; i--)
		buf[i] = tolower(source[i]);

	return buf;
}

char *strtolower(char *const dest, const char *const source)
{
	return strntolower(dest, source, strlen(source));
}

char *strntoupper(char *const dest, const char *const source, size_t length)
{
	char *buf;

	if (!dest)
		buf = stralloc(length);
	else
		buf = dest, buf[length + 1] = '\0';

	size_t i;

	for (i = length; i > 0; i--)
		buf[i] = tolower(source[i]);

	return buf;
}

char *strtoupper(char *const dest, const char *const source)
{
	return strntoupper(dest, source, strlen(source));
}

char *strndcpy(char *const dest, const char *const source, size_t length)
{
	char *buf;

	if (!dest)
		buf = stralloc(length);
	else
		buf = dest, buf[length] = '\0';

	return strncpy(buf, source, length);
}

char *strdcpy(char *const dest, const char *const source)
{
	return strndcpy(dest, source, strlen(source));
}

#pragma endregion

/* =---- Exceptions Management ---------------------------------= */

#pragma region Exceptions Management

int error(const char *const message)
{
	return fprint(stderr, message);
}

int errorln(const char *const message)
{
	return fprintln(stderr, message);
}

int verrorf(const char *const format, va_list arglist)
{
	return vfprintf(stderr, format, arglist);
}

int verrorfn(const char *const format, va_list arglist)
{
	return _vfprintfn(stderr, format, arglist);
}

int errorf(const char *const format, ...)
{
	int result;
	va_list arglist;

	va_start(arglist, format);
	result = verrorf(format, arglist);
	va_end(arglist);

	return result;
}

int errorfn(const char *const format, ...)
{
	int result;
	va_list arglist;

	va_start(arglist, format);
	result = verrorfn(format, arglist);
	va_end(arglist);

	return result;
}

#pragma endregion

/* =---- Symbols Management ------------------------------------= */

#pragma region Symbols Management

// +---- Internal

#pragma region Internal

static inline hash_t _get_hashcode(const char *str)
{
	if (!str || (*str <= 0x20))
		return HASH_INV; // intentionally returns an unmanageable value

	hash_t hash;

	for (hash = HASH_MIN; *str > 0x20; str++)
		hash += *str - 0x21;

	return hash;
}

static inline hash_t _get_hashcode_mod(const char *str, unsigned int siz)
{
	return _get_hashcode(str) % siz;
}

#pragma endregion

// +---- Internal End

hash_t gethash(const char *const str)
{
	return _get_hashcode((const char *)str);
}

// Symbol Table

hashkey_t *create_hashkey(const char *const name, unsigned int data, hashkey_t *const prev)
{
	hashkey_t *key = alloc(hashkey_t);

	key->name = (char *)name;
	key->data = data;
	key->refs = 0;

	if (prev)
	{
		if (prev->next)
			key->next = prev->next;
		else
			key->next = NULL;

		prev->next = key;
	}
	else
	{
		key->next = NULL;
	}

	return key;
}

hashtab_t *create_hashtab(unsigned int size, hashtab_t *const prev)
{
	hashtab_t *tab = alloc(hashtab_t);

	tab->keys = dimz(hashkey_t*, size);
	tab->size = size;
	tab->used = 0;
	tab->prev = prev;

	return tab;
}

bool_t hashtab_exists(hashtab_t *const tab, const char *const name)
{
	hash_t hash = _get_hashcode_mod(name, tab->size);

	if (tab->keys[hash])
	{
		hashkey_t *s = tab->keys[hash];

		do
		{
			if (streq(name, s->name))
				return TRUE;
			else
				s = s->next;
		} while (s);
	}

	return FALSE;
}

hashkey_t *hashtab_add(hashtab_t *const tab, const char *const name, unsigned int attr)
{
	hash_t hash = _get_hashcode_mod(name, tab->size);

	if (tab->keys[hash])
	{
		hashkey_t *s = tab->keys[hash], *p;

		do
		{
			if (streq(name, s->name))
				return s->refs++, s;
			else
				p = s, s = p->next;
		} while (s);

		return create_hashkey(name, attr, p);
	}
	else
	{
		return tab->used++, tab->keys[hash] = create_hashkey(name, attr, NULL);
	}
}

hashkey_t *hashtab_get(hashtab_t *const tab, const char *const name)
{
	hash_t hash = _get_hashcode_mod(name, tab->size);

	if (tab->keys[hash])
	{
		hashkey_t *s = tab->keys[hash];

		do
		{
			if (streq(name, s->name))
				return s;
			else
				s = s->next;
		} while (s);
	}

	return NULL;
}

hashkey_t *hashtab_set(hashtab_t *const tab, const char *const name, unsigned int attr)
{
	hash_t hash = _get_hashcode_mod(name, tab->size);

	if (tab->keys[hash])
	{
		hashkey_t *s = tab->keys[hash];

		do
		{
			if (streq(name, s->name))
				return s->data = attr, s;
			else
				s = s->next;
		} while (s);
	}

	return NULL;
}

hashtab_t *delete_hashtab(hashtab_t *const tab)
{
	unsigned int i;

	for (i = 0; tab->used && (i < tab->size); i++)
	{
		if (tab->keys[i])
		{
			hashkey_t *key = tab->keys[i], *tmp;

			do
			{
				tmp = key->next;

				free(key->name);
				free(key);

				key = tmp;
			} while (key);

			tab->used--;
		}
	}

	free(tab->keys);

	return (hashtab_t *)tab->prev;
}

#ifdef CALC_DEBUG

void hashtab_print(hashtab_t *const tab)
{
	unsigned int i, c, d;

	for (i = 0, c = tab->used; c && (i < tab->size); i++)
	{
		if (tab->keys[i])
		{
			hashkey_t *key = tab->keys[i];

			d = 0;

			do
			{
				printf("0x%04X-%02X -> 0x%04X ('%s')\n", i, d, key->data, key->name);

				key = key->next;
				d++;
			} while (key);

			c--;
		}
	}

	return;
}

#endif // CALC_DEBUG

#pragma endregion

/* =------------------------------------------------------------= */

#ifndef CALC_LIBER_C_
#	define CALC_LIBER_C_
#endif
