#pragma once

/* calc-liber.h - Copyright (c) 2024 Frithurik Grint */

#ifndef CALC_LIBER_H_
#define CALC_LIBER_H_

#ifndef _CRT_SECURE_NO_WARNINGS
/// @brief Avoid deprecation warnings.
#   define _CRT_SECURE_NO_WARNINGS 1
#endif // _CRT_SECURE_NO_WARNINGS

#ifdef __cplusplus
#   ifndef CALC_C_HEADER_BEGIN
/// @brief This macro marks the C header beginning for C++ compilers.
#       define CALC_C_HEADER_BEGIN extern "C" {
#   endif // CALC_C_HEADER_BEGIN

#   ifndef CALC_C_HEADER_END
/// @brief This macro marks the C header ending for C++ compilers.
#       define CALC_C_HEADER_END }
#   endif // CALC_C_HEADER_END
#else
#   ifndef CALC_C_HEADER_BEGIN
/// @brief This macro marks the C header beginning for C++ compilers.
#       define CALC_C_HEADER_BEGIN
#   endif // CALC_C_HEADER_BEGIN

#   ifndef CALC_C_HEADER_END
/// @brief This macro marks the C header ending for C++ compilers.
#       define CALC_C_HEADER_END
#   endif // CALC_C_HEADER_END
#endif // __cplusplus

#ifdef _DEBUG
#	ifndef CALC_DEBUG
/// @brief Internal debug flag.
#		define CALC_DEBUG 1
#	endif // CALC_DEBUG
#endif // _DEBUG

#ifdef _UNICODE
#	ifndef CALC_UNICODE
/// @biref Internal unicode charset flag.
#		define CALC_UNICODE 1
#	endif // CALC_UNICODE
#endif // _UNICODE

#include <assert.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include <ctype.h>
#include <stdio.h>

// +---- System Dependent Libraries

#pragma region System Dependent Libraries

#ifdef _WIN32
#   include <io.h>
#   include <process.h>
#   include <direct.h>

/// @brief Existance test access.
#   define F_OK 0
/// @brief Write mode access.
#   define W_OK 2
/// @brief Read mode access.
#   define R_OK 4
/// @brief Execution mode access.
#   define X_OK 6

/// @brief Ddetermine accessibility of a file.
/// @param path File or directory path.
/// @param mode Read/write attribute.
/// @return 0 if the file has the given mode.
///         The function returns -1 if the named
///         filedoesn't exist or doesn't have
///         the given mode.
#   define access       _access
/// @brief Creates a second file descriptor for an open file.
/// @param fd File descriptor referring to open file.
/// @return A new file descriptor.
#   define dup          _dup
/// @brief Reassigns a file descriptor.
/// @param fd1 File descriptor referring to open file.
/// @param fd2 Any file descriptor.
/// @return 0 to indicate success. If an error occurs,
///         each function returns -1 and sets errno to
///         EBADF if the file descriptor is invalid,
///         or to EMFILE if no more file descriptors
///         are available.
#   define dup2         _dup2
/// @brief Loads and executes new child processes.
/// @param cmdname Path of the file to execute.
/// @param argv Array of pointers to parameters.
/// @return If successful, these functions don't return
///         to the calling process. A return value of -1
///         indicates an error, in which case the errno
///         global variable is set.
#   define execve       _execve
/// @brief Changes the size of a file.
/// @param fd File descriptor referring to an open file.
/// @param size New length of the file in bytes.
/// @return 0 if the file size is successfully changed.
///         A return value of -1 indicates an error.
#   define ftruncate    _chsize
/// @brief Delete a file.
/// @param filename Name of file to remove.
/// @return 0 if successful. Otherwise, the function
///         returns -1 and sets errno to EACCES
#   define unlink       _unlink
/// @brief Gets the file descriptor associated with a stream.
/// @param stream Pointer to the FILE structure.
/// @return _fileno returns the file descriptor. There's no
///         error return. The result is undefined if stream
///         doesn't specify an open file. 
#   define fileno       _fileno
/// @brief Gets the current working directory.
/// @param buffer Storage location for the path.
/// @param maxlen Maximum length of the path in characters.
/// @return Returns a pointer to buffer. A NULL return value
///         indicates an error.
#   define getcwd       _getcwd
/// @brief Determines whether a file descriptor is
///        associated with a character device.
/// @param fd File descriptor that refers to the device to
///           be tested.
/// @return _isatty returns a nonzero value if the descriptor
///         is associated with a character device. Otherwise,
///         _isatty returns 0.
#   define chdir        _chdir
/// @brief Determines whether a file descriptor is
///        associated with a character device.
/// @param fd File descriptor that refers to the device to
///           be tested.
/// @return _isatty returns a nonzero value if the descriptor
///         is associated with a character device. Otherwise,
///         _isatty returns 0.
#   define isatty       _isatty
/// @brief Moves a file pointer to the specified location.
/// @param fd File descriptor referring to an open file.
/// @param offset Number of bytes from origin.
/// @param origin Initial position.
/// @return _lseek returns the offset, in bytes, of the
///         new position from the beginning of the file.
#   define lseek        _lseek

#   ifdef _WIN64
/// @brief Used for a count of bytes or an error indication.
#       define ssize_t  __int64
#   else
/// @brief Used for a count of bytes or an error indication.
#       define ssize_t  long
#   endif // _WIN64

/// @brief stdin fileno.
#   define STDIN_FILENO  0
/// @brief stdout fileno.
#   define STDOUT_FILENO 1
/// @brief stderr fileno.
#   define STDERR_FILENO 2

CALC_C_HEADER_BEGIN

/// @brief Gets memory page size.
/// @return Memory page size.
const int _getpagesiz();

CALC_C_HEADER_END

/// @brief Gets memory page size.
#   define pagesiz       _getpagesiz()
/// @brief Gets data word size.
#   define wordsiz       sizeof(void *)
#else
#   include <unistd.h>

/// @brief Gets memory page size.
#   define pagesiz       sysconf(_SC_PAGESIZE)
/// @brief Gets data word size.
#   define wordsiz       sizeof(int)
#endif // _WIN32

#pragma endregion

// +---- System Dependent Libraries -- End

// +---- System Output Functions

#pragma region System Output Functions

CALC_C_HEADER_BEGIN

/// @brief Puts the system endline char
///        sequence on stream.
/// @param stream Output stream.
/// @return Number of characters printed.
int fputln(FILE *const stream);
/// @brief Puts the system endline char
///        sequence on stdout.
/// @return Number of characters printed.
int putln();

/// @brief Prints a message in a stream.
/// @param stream Output stream.
/// @param message Message to print.
/// @return Number of characters written.
int fprint(FILE *const stream, const char *const message);
/// @brief Prints a message in stdout.
/// @param message Message to print.
/// @return Number of characters written.
int print(const char *const message);
/// @brief Prints a message in a stream.
/// @param stream Output stream.
/// @param message Message to print.
/// @return Number of characters written.
int fprintln(FILE *const stream, const char *const message);
/// @brief Prints a message in stdout.
/// @param message Message to print.
/// @return Number of characters written.
int println(const char *const message);

/// @brief Prints a message in stdout.
/// @param stream Output stream.
/// @param format Fromat of the string to print.
/// @param arglist Arguments to use to format
///               the message.
/// @return Number of characters written.
int vfprintfn(FILE *const stream, const char *const format, va_list arglist);
/// @brief Prints a formatted message in a stream.
/// @param stream Output stream.
/// @param format Fromat of the string to print.
/// @param others Arguments to use to format
///               the message.
/// @return Number of characters written.
int fprintfn(FILE *const stream, const char *const format, ...);
/// @brief Print a formatted message in stdout.
/// @param format Fromat of the string to print.
/// @param arglist Arguments to use to format
///               the message.
/// @return Number of characters written.
int vprintfn(const char *const format, va_list arglist);
/// @brief Print a formatted message in stdout.
/// @param format Fromat of the string to print.
/// @param others Arguments to use to format
///               the message.
/// @return Number of characters written.
int printfn(const char *const format, ...);

CALC_C_HEADER_END

#pragma endregion System Output Functions

// +---- System Output Functions -- End

CALC_C_HEADER_BEGIN

/* =---- Exceptions Management ---------------------------------= */

#pragma region Exceptions Management

// TODO: better exception management

/// @brief Prints an error message.
/// @param message Message to print.
/// @return Number of characters written.
int error(const char *const message);
/// @brief Prints an error message on a line.
/// @param message Message to print. If it's
///                NULL it prints only an empty
///                line.
/// @return Number of characters written.
int errorln(const char *const message);
/// @brief Prints an error message.
/// @param format Format of the message to print.
/// @param arglist Arguments to use for format
///                the message.
/// @return Number of characters written.
int verrorf(const char *const format, va_list arglist);
/// @brief Prints an error message on a line.
/// @param format Format of the message to print.
/// @param arglist Arguments to use for format
///                the message.
/// @return Number of characters written.
int verrorfn(const char *const format, va_list arglist);
/// @brief 
/// @param format Format of the message to print.
/// @param others Arguments to use for format
///                the message.
/// @return Number of characters written.
int errorf(const char *const format, ...);
/// @brief 
/// @param format Format of the message to print.
/// @param others Arguments to use for format
///                the message.
/// @return Number of characters written.
int errorfn(const char *const format, ...);

#pragma endregion

/* =---- Memory Management -------------------------------------= */

#pragma region Memory Management

/// @brief Allocate a block of memory in the heap.
/// @param size Number of bytes to allocate.
/// @return Pointer to the beginning of the allocated
///			region of memory.
void *malloc_s(size_t size);
/// @brief Allocate a series of contiguous blocks
///		   of memory in the heap.
/// @param count Number of blocks to allocate.
/// @param size Number of bytes in each block.
/// @return Pointer to the beginning of the first
///			allocated block
void *calloc_s(size_t count, size_t size);

#ifndef alloc
/// @brief Allocate a new instance of the specified
///        type.
#	define alloc(type) (type *)malloc_s(sizeof(type))
#endif // alloc

#ifndef dim
/// @brief Allocate a new array of the specified type
///        the specified number of members
#	define dim(type, count) (type *)calloc_s((count), sizeof(type))
#endif // dim

/// @brief Allocate a block of bytes, all set to
///		   zero.
/// @param size Number of bytes to allocate.
/// @return Pointer to the beginning of the
///			allocated region of memory.
void *mallocz_s(size_t size);
/// @brief Allocate a series of coniguous block of
///		   bytes, all set to zero.
/// @param count Number of blocks to allocate.
/// @param size Number of bytes in each block
///		   of memory.
/// @return Pointer to the first allocated block.
void *callocz_s(size_t count, size_t size);

#ifndef allocz
/// @brief Allocate a new zero or default instance
///        of a specified type.
#	define allocz(type) (type *)mallocz_s(sizeof(type))
#endif // allocz

#ifndef dimz
/// @brief Allocate a new array of type with all
///        members set to zero.
#	define dimz(type, count) (type *)callocz_s((count), sizeof(type))
#endif // dimz

#ifndef alignto
/// @brief Align two sizes. Used in *alloca_s and *allocaz_s
///        functions.
#   define alignto(size, alignment) ((size + alignment - 1) & ~(alignment - 1))
#endif // alignto

/// @brief Allocate a block of memory in the heap,
///		   aligned to a specific width.
/// @param size Number of byte to allocate.
/// @param alignment Width of alignment.
/// @return Pointer to the beginning of the allocated
///			block of memory.
void *malloca_s(size_t size, size_t alignment);
/// @brief Allocate a series of cotiguous blocks
///		   of memory in the heap aligned to a 
///		   specific width.
/// @param count Number of blocks to allocate.
/// @param size Number of bytes in each block.
/// @param alignment Width of the alignment.
/// @return Pointer to the beginning of the
///			first allocated region.
void *calloca_s(size_t count, size_t size, size_t alignment);

#ifndef alloca
/// @brief Allocate a new instance of the specified
///        type, aligned to another type.
#	define alloca(type1, type2) (type1 *)malloca_s(sizeof(type1), sizeof(type2))
#endif // alloca

#ifndef dima
/// @brief Allocate a new array of the specified type
///        with members aligned to another type.
#	define dima(type1, type2, count) (type1 *)calloca_s((count), sizeof(type1), sizeof(type2))
#endif // dima

/// @brief Allocate a block of memory in the heap,
///		   aligned to a specific width, all set to
///		   zero.
/// @param size Number of byte to allocate.
/// @param alignment Width of alignment.
/// @return Pointer to the beginning of the allocated
///			block of memory.
void *mallocaz_s(size_t size, size_t alignment);
/// @brief Allocate a series of cotiguous blocks
///		   of memory in the heap aligned to a 
///		   specific width, all set to zero.
/// @param count Number of blocks to allocate.
/// @param size Number of bytes in each block.
/// @param alignment Width of the alignment.
/// @return Pointer to the beginning of the
///			first allocated region.
void *callocaz_s(size_t count, size_t size, size_t alignment);

#ifndef allocaz
/// @brief Allocate a new zero instance of the specified
///        type, aligned to another type.
#	define allocaz(type1, type2) (type1 *)mallocaz_s(sizeof(type1), sizeof(type2))
#endif // allocaz

#ifndef dimaz
/// @brief Allocate a new array of the specified type
///        with members aligned to another type all
///        set to zero.
#	define dimaz(type1, type2, count) (type1 *)callocaz_s((count), sizeof(type1), sizeof(type2))
#endif // dimaz

// Strings

/// @brief Allocate a new array of bytes (string).
/// @param length Number of characters in the string.
/// @return A pointer to the first character of the 
///         string.
char *stralloc(size_t length);

#pragma endregion

/* =---- Data Management ---------------------------------------= */

#pragma region Data Management

#ifndef numcmp
/// @brief Compares two generic numbers.
#   define numcmp(num1, num2)   ((num1) > (num2)) ? +1 : \
                                ((num1) < (num2)) ? -1 : 0
#endif // numcmp

// +---- Datatypes

#pragma region Datatypes

// Boolean

#ifndef CALC_BOOL_T
/// @brief This macro allow boolean data type modification.
#   define CALC_BOOL_T unsigned char
#endif // CALC_BOOL_T

/// @brief Boolean data type, can be only set to TRUE
///        or to FALSE.
typedef CALC_BOOL_T bool_t;

#ifndef TRUE
/// @brief This macro provides true value (1) for bool_t
///        data type.
#   define TRUE ((bool_t)0x01)
#endif // TRUE

#ifndef FALSE
/// @brief This macro provides false value (0) for bool_t
///        data type.
#   define FALSE ((bool_t)0x00)
#endif // FALSE

// Byte

#ifndef CALC_BYTE_T
/// @brief This macro allow byte data type modification.
#   define CALC_BYTE_T unsigned char
#endif // CALC_BYTE_T

/// @brief Byte data type, its value must be into BYTE_MIN
///        and BYTE_MAX range.
typedef CALC_BYTE_T byte_t;

#ifndef BYTE_MIN
/// @brief This macro provides the minimun value for
///        byte_t data type.
#   define BYTE_MIN ((byte_t)0x00)
#endif // BYTE_MIN

#ifndef BYTE_MAX
/// @brief This macro provides the maximum value for
///        byte_t data type.
#   define BYTE_MAX ((byte_t)0xFF)
#endif // BYTE_MAX

// String

/// @brief Equals two strings.
/// @param str1 First string. (is better to use a costant)
/// @param str2 Second string.
/// @return TRUE if the strings have the same content and the same
///         length, or if are the same. (also if are both NULL)
bool_t streq(const char *const str1, const char *const str2);
/// @brief Equals two strings ignoring letters case.
/// @param str1 First string. (is better to use a costant)
/// @param str2 Second string.
/// @return TRUE if the strings have the same content and the same
///         length, or if are the same. (also if are both NULL)
bool_t strieq(const char *const str1, const char *const str2);

/// @brief Converts a string to lower case.
/// @param dest Destination buffer (if NULL duplicate).
/// @param source Source string.
/// @param length Number of characters.
/// @return A pointer to dest or the new string.
char *strntolower(char *const dest, const char *const source, size_t length);
/// @brief Converts a string to lower case.
/// @param dest Destination buffer (if NULL duplicate).
/// @param source Source string.
/// @return A pointer to dest or the new string.
char *strtolower(char *const dest, const char *const source);
/// @brief Converts a string to lower case.
/// @param dest Destination buffer (if NULL duplicate).
/// @param source Source string.
/// @param length Number of characters.
/// @return A pointer to dest or the new string.
char *strntoupper(char *const dest, const char *const source, size_t length);
/// @brief Converts a string to lower case.
/// @param dest Destination buffer (if NULL duplicate).
/// @param source Source string.
/// @return A pointer to dest or the new string.
char *strtoupper(char *const dest, const char *const source);

/// @brief Create a duplicate of a string or copy it in
///        dest.
/// @param dest Destination buffer (if NULL duplicate).
/// @param source Source string.
/// @param length Number of character to copy.
/// @return A pointer to a new string or dest.
char *strndcpy(char *const dest, const char *const source, size_t length);
/// @brief Create a duplicate of a string or copy it in
///        dest.
/// @param dest Destination buffer (if NULL duplicate).
/// @param source Source string.
/// @return A pointer to a new string or dest.
char *strdcpy(char *const dest, const char *const source);

/// @brief Unescape a character.
/// @param str Source char.
/// @return The pointer to unescaped str.
char *unesc(char *const dest, int c);

#pragma endregion

// +---- Datatypes -- End

// +---- Datastructs (DSA)

#pragma region Datastructs

/// @brief Hash code data type.
typedef unsigned int hash_t;

#ifndef HASH_MIN
/// @brief Minimum hash code value.
#   define HASH_MIN ((hash_t)0x00)
#endif // HASH_MIN

#ifndef HASH_MAX
/// @brief Maximum hash code value.
#   define HASH_MAX ((hash_t)-2)
#endif // HASH_MAX

#ifndef HASH_INV
/// @brief Erroneus hash code value.
#   define HASH_INV ((hash_t)-1)
#endif // HASH_INV

/// @brief Compute hash code of a string.
/// @param str Key string.
/// @return Computed hash code.
hash_t gethash(const char *const str);

// Hash Table

/// @brief Hash table bucket structure.
typedef struct _calc_hash_table_bucket
{
    /// @brief Key string.
    char *name;
    /// @brief Hash code of the bucket in
    ///        the table.
    hash_t hash;
    /// @brief Index of the datas in the
    ///        data record.
    unsigned int data;
    /// @brief Pointer to the colliding
    ///        bucket with the same hash.
    struct _calc_hash_table_bucket *next;
} hashbuc_t;

/// @brief Creates a new hash table bucket
///        with data address and colliding
///        bucket.
/// @param name Key string.
/// @param hash Computed hash code.
/// @param data Data address.
/// @param prev In case of collision this points
///             to the colliding bucket.
/// @return A pointer to the new hash table
///         bucket.
hashbuc_t *create_hashbuc(char *const name, hash_t hash, unsigned int data, hashbuc_t *const prev);
/// @brief Deletes the specified hash table
///        bucket, releasing its memory. (this
///        frees name pointer)
/// @param bucket Bucket to delete.
/// @return A pointer to the colliding bucket.
hashbuc_t *delete_hashbuc(hashbuc_t *const bucket);

#ifndef CALC_HASHTAB_BUCKSNUM
/// @brief Default number of buckets in a chunk
///        of the hash table.
#   define CALC_HASHTAB_BUCKSNUM 0xFF
#endif // CALC_HASHTAB_BUCKSNUM

/// @brief Hash table data structure.
typedef struct _calc_hash_table
{
    /// @brief Array of pointers to the buckets.
    hashbuc_t **buck;
    /// @brief Maximum number of buckets.
    unsigned int size;
    /// @brief Number of used buckets.
    unsigned int used;
    /// @brief Pointer to previous hash table
    ///        chunk.
    struct _calc_hash_table *prev;
} hashtab_t;

/// @brief Creates a new hash table with the
///        specified number of buckets.
/// @param size Numeber of buckets to allocate
///             in the chunk. (if size is 0 will
///             be allocated a chunk of
///             CALC_HASHTAB_BUCKSNUM buckets)
/// @param prev Pointer to the previous chunk.
/// @return A pointer to the new hash table chunk.
hashtab_t *create_hashtab(unsigned int size, hashtab_t *const prev);
/// @brief Deletes the specified hash table with
///        its buckets, releasing all used memory.
/// @param tab Pointer to table to delete.
/// @return A pointer to the previous hash table chunk.
hashtab_t *delete_hashtab(hashtab_t *const tab);

/// @brief If specified key is not inserted in the hash table
///        chunk, inserts it, else gets it.
/// @param tab Pointer to the hash table chunk on which operate.
/// @param key Key to add or get.
/// @return A pointer to the new (or old) hash bucket.
hashbuc_t *hashtab_add(hashtab_t *const tab, char *const key);
/// @brief Gets bucket with the specified key.
/// @param tab Pointer to the hash table chunk on which operate.
/// @param key Key to find.
/// @return A pointer to the found bucket or null if not found.
hashbuc_t *hashtab_get(hashtab_t *const tab, char *const key);
/// @brief Sets the data address of the bucket at the specified
///        key.
/// @param tab Pointer to the hash table chunk on which operate.
/// @param key Key to find.
/// @param data Data address to set to the found bucket.
/// @return A pointer to the found bucket or null if not found.
hashbuc_t *hashtab_set(hashtab_t *const tab, char *const key, unsigned int data);

/// @brief Checks if hash table contains a specified key.
/// @param tab Pointer to the hash table chunk on which operate.
/// @param key Key to check.
/// @return TRUE if the hash table contains the value, else FALSE.
bool_t hashtab_contains(hashtab_t *const tab, char *const key);

/// @brief Removes an hash bucket from the table.
/// @param tab Pointer to the hash table chunk on which operate.
/// @param key Key of the bucket to remove.
/// @return A pointer to the removed hash bucket.
hashbuc_t *hashtab_remove(hashtab_t *const tab, char *const key);
/// @brief Deletes an hash bucket from the table.
/// @param tab Pointer to the hash table chunk on which operate.
/// @param key Key of the bucket to delete.
/// @return A pointer to the colliding bucket.
hashbuc_t *hashtab_delete(hashtab_t *const tab, char *const key);

/// @brief Dumps hash table content on a stream.
/// @param stream Stream in which dump content of the hash
///               table.
/// @param tab Pointer to the last hash table chunk of the
///            hash table to dump on the selected stream.
void hashtab_dump(FILE *const stream, hashtab_t *const tab);

#pragma endregion

// +---- Datastructs -- End

#pragma endregion

/* =------------------------------------------------------------= */

CALC_C_HEADER_END

#ifdef _CALC_BUILD_AS_ONE
#   ifndef CALC_LIBER_C_
#       include "calc-liber.c"
#   endif // CALC_LIBER_C_
#endif // _CALC_BUILD_AS_ONE

#endif // CALC_LIBER_H_