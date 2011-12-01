/**
 * Author: paladin_t, hellotony521@gmail.com
 * Created: Oct. 14, 2011
 * Last edited: Dec. 1, 2011
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details.
 */

#ifndef __XPL_H__
#define __XPL_H__

#ifdef _MSC_VER
#  pragma warning(disable : 4201)
#  pragma warning(disable : 4706)
#endif /* _MSC_VER */

#include <memory.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
** {========================================================
** Macros and typedefines
*/

#ifndef XPLVER
#  define XPLVER_MAJOR	1
#  define XPLVER_MINOR	0
#  define XPLVER_PATCH	2
#  define XPLVER ((XPLVER_MAJOR << 24) | (XPLVER_MINOR << 16) | (XPLVER_PATCH))
#endif /* XPLVER */

#ifndef XPLINTERNAL
#  define XPLINTERNAL static
#endif /* XPLINTERNAL */
#ifndef XPLAPI
#  define XPLAPI static
#endif /* XPLAPI */

#ifndef NULL
#  define NULL 0
#endif /* NULL */

#ifndef _countof
#  define _countof(a) (sizeof(a) / sizeof(*(a)))
#endif /* _countof */

#ifndef xpl_assert
#  define xpl_assert(e) assert(e)
#endif /* xpl_assert */

/**
 * @brief XPL scripting programming interface registering macros
 * @note The interfaces are storaged in a common array, you could put
 *  these macros at global or local scopes to make your customized
 *  interfaces.
 */
#ifndef XPL_FUNC_REGISTER
#  define XPL_FUNC_REGISTER
/**< Begin an interface declaration */
#  define XPL_FUNC_BEGIN(a) \
    static xpl_func_info_t a[] = { \
      { "if", _xpl_core_if }, \
      { "then", _xpl_core_then }, \
      { "elseif", _xpl_core_elseif }, \
      { "else", _xpl_core_else }, \
      { "endif", _xpl_core_endif }, \
      { "or", _xpl_core_or }, \
      { "and", _xpl_core_and }, \
      { "yield", _xpl_core_yield },
/**< Declare an interface */
#  define XPL_FUNC_ADD(n, f) \
      { n, f },
/**< End an interface declaration */
#  define XPL_FUNC_END \
      { NULL, NULL }, \
    };
#endif /* XPL_FUNC_REGISTER */

/**
 * @brief Skip meaningless parts, like comment and blank.
 */
#ifndef SKIP_MEANINGLESS
#  define SKIP_MEANINGLESS(s) \
  do { \
    while(_xpl_is_squote(*(unsigned char*)_s->cursor) || _xpl_is_blank(*(unsigned char*)_s->cursor)) { \
      _xpl_trim(&(s)->cursor); if(xpl_skip_comment(s) == XS_OK) _xpl_trim(&(s)->cursor); } \
  } while(0)
#endif /* SKIP_MEANINGLESS */

/**
 * @brief XPL function execution status
 */
typedef enum xpl_status_t {
  XS_OK,                    /**< Totally OK */
  XS_SUSPENT,               /**< Suspent */
  XS_ERR,                   /**< Some error occured */
  XS_NO_ENOUGH_BUFFER_SIZE, /**< String buffer too small */
  XS_NO_COMMENT,            /**< No comment found */
  XS_NO_PARAM,              /**< No param found */
  XS_PARAM_TYPE_ERROR,      /**< Parameter convertion failed */
  XS_COUNT,
} xpl_status_t;

/**
 * @brief Boolean value composing type
 */
typedef enum xpl_bool_composing_t {
  XBC_NIL, /**< Assign new value directly */
  XBC_OR,  /**< Old value OR new value */
  XBC_AND, /**< Old value AND new value */
} xpl_bool_composing_t;

struct xpl_context_t;

/**
 * @brief XPL scripting programming interface signature
 *
 * @param[in] _s - XPL context
 */
typedef xpl_status_t (* xpl_func_t)(struct xpl_context_t* _s);

/**
 * @brief XPL scripting programming interface information
 */
typedef struct xpl_func_info_t {
  const char* name; /**< Interface name */
  xpl_func_t func;  /**< Pointer to interface function */
} xpl_func_info_t;

/**
 * @brief XPL context structure
 */
typedef struct xpl_context_t {
  /**
   * @brief Registered interfaces
   */
  struct {
    xpl_func_info_t* funcs; /**< Pointer to array of registered interfaces */
    int funcs_count;        /**< Count of registered interfaces */
  };
  /**
   * @brief Script source code indicator
   */
  struct {
    const char* text;   /**< Script source text */
    const char* cursor; /**< Script execution cursor */
  };
  /**
   * @brief Boolean value
   */
  struct {
    xpl_bool_composing_t bool_composing; /**< Boolean value composing type */
    int bool_value;                      /**< Current boolean value */
  };
  void* userdata; /**< Pointer to user defined data */
} xpl_context_t;

/* ========================================================} */

/*
** {========================================================
** Function declarations
*/

/**
 * @brief Opens an XPL context
 *
 * @param[in] _s - XPL context
 * @return - Returns execution status
 */
XPLAPI xpl_status_t xpl_open(xpl_context_t* _s, xpl_func_info_t* _f);
/**
 * @brief Closes an XPL context
 *
 * @param[in] _s - XPL context
 * @return - Returns execution status
 */
XPLAPI xpl_status_t xpl_close(xpl_context_t* _s);

/**
 * @brief Loads a script
 *
 * @param[in] _s - XPL context
 * @return - Returns execution status
 */
XPLAPI xpl_status_t xpl_load(xpl_context_t* _s, const char* _t);
/**
 * @brief Reloads current script, set execution cursor to begin point.
 *
 * @param[in] _s - XPL context
 * @return - Returns execution status
 */
XPLAPI xpl_status_t xpl_reload(xpl_context_t* _s);
/**
 * @brief Unloads a script
 *
 * @param[in] _s - XPL context
 * @return - Returns execution status
 */
XPLAPI xpl_status_t xpl_unload(xpl_context_t* _s);

/**
 * @brief Runs a script
 *
 * @param[in] _s - XPL context
 * @return - Returns execution status
 */
XPLAPI xpl_status_t xpl_run(xpl_context_t* _s);
/**
 * @brief Tries to peek one function
 *
 * @param[in] _s - XPL context
 * @return - Returns execution status
 */
XPLAPI xpl_status_t xpl_peek_func(xpl_context_t* _s, xpl_func_info_t** _f);
/**
 * @brief Runs a single step
 *
 * @param[in] _s - XPL context
 * @return - Returns execution status
 */
XPLAPI xpl_status_t xpl_step(xpl_context_t* _s);

/**
 * @brief Skips a piece of comment
 *
 * @param[in] _s - XPL context
 * @return - Returns execution status
 */
XPLAPI xpl_status_t xpl_skip_comment(xpl_context_t* _s);
/**
 * @brief Determines whether current XPL context contains a parameter
 *
 * @param[in] _s - XPL context
 * @return - Returns execution status, XS_OK if has param, or XS_NO_PARAM if does not.
 */
XPLAPI xpl_status_t xpl_has_param(xpl_context_t* _s);
/**
 * @brief Pops a integer parameter from XPL context
 *
 * @param[in] _s  - XPL context
 * @param[out] _o - Destination buffer
 * @return - Returns execution status
 */
XPLAPI xpl_status_t xpl_pop_int(xpl_context_t* _s, int* _o);
/**
 * @brief Pops a float parameter from XPL context
 *
 * @param[in] _s  - XPL context
 * @param[out] _o - Destination buffer
 * @return - Returns execution status
 */
XPLAPI xpl_status_t xpl_pop_float(xpl_context_t* _s, float* _o);
/**
 * @brief Pops a string parameter from XPL context
 *
 * @param[in] _s  - XPL context
 * @param[out] _o - Destination buffer
 * @param[in] _l  - Destination buffer size
 * @return - Returns execution status
 */
XPLAPI xpl_status_t xpl_pop_string(xpl_context_t* _s, char* _o, int _l);
/**
 * @brief Pushes a boolean value to XPL context
 *
 * @param[in] _s - XPL context
 * @param[in] _b - Boolean value
 * @return - Returns execution status
 */
XPLAPI xpl_status_t xpl_push_bool(xpl_context_t* _s, int _b);

/**
 * @brief Scripting programming interface:
 *   'if' statement, dummy function.
 *
 * @param[in] _s - XPL context
 * @return - Returns execution status
 */
XPLINTERNAL xpl_status_t _xpl_core_if(xpl_context_t* _s);
/**
 * @brief Scripting programming interface:
 *   'then' statement, main logic about 'if-then-elseif-else-endif'.
 *
 * @param[in] _s - XPL context
 * @return - Returns execution status
 */
XPLINTERNAL xpl_status_t _xpl_core_then(xpl_context_t* _s);
/**
 * @brief Scripting programming interface:
 *   'elseif' statement, dummy function.
 *
 * @param[in] _s - XPL context
 * @return - Returns execution status
 */
XPLINTERNAL xpl_status_t _xpl_core_elseif(xpl_context_t* _s);
/**
 * @brief Scripting programming interface:
 *   'else' statement, dummy function.
 *
 * @param[in] _s - XPL context
 * @return - Returns execution status
 */
XPLINTERNAL xpl_status_t _xpl_core_else(xpl_context_t* _s);
/**
 * @brief Scripting programming interface:
 *   'endif' statement, dummy function.
 *
 * @param[in] _s - XPL context
 * @return - Returns execution status
 */
XPLINTERNAL xpl_status_t _xpl_core_endif(xpl_context_t* _s);
/**
 * @brief Scripting programming interface:
 *   'or' statement, set current boolean composing type as OR.
 *
 * @param[in] _s - XPL context
 * @return - Returns execution status
 */
XPLINTERNAL xpl_status_t _xpl_core_or(xpl_context_t* _s);
/**
 * @brief Scripting programming interface:
 *   'and' statement, set current boolean composing type as AND.
 *
 * @param[in] _s - XPL context
 * @return - Returns execution status
 */
XPLINTERNAL xpl_status_t _xpl_core_and(xpl_context_t* _s);
/**
 * @brief Scripting programming interface:
 *   'yield' statement, suspend current execution.
 *
 * @param[in] _s - XPL context
 * @return - Returns execution status
 */
XPLINTERNAL xpl_status_t _xpl_core_yield(xpl_context_t* _s);

/**
 * @brief Determines whether a char is a single quote
 *
 * @param[in] _c - Char to be determined
 * @return Returns non-zero if matching
 */
XPLINTERNAL int _xpl_is_squote(unsigned char _c);
/**
 * @brief Determines whether a char is a double quote
 *
 * @param[in] _c - Char to be determined
 * @return Returns non-zero if matching
 */
XPLINTERNAL int _xpl_is_dquote(unsigned char _c);
/**
 * @brief Determines whether a char is a comma
 *
 * @param[in] _c - Char to be determined
 * @return Returns non-zero if matching
 */
XPLINTERNAL int _xpl_is_comma(unsigned char _c);
/**
 * @brief Determines whether a char is an exclamation
 *
 * @param[in] _c - Char to be determined
 * @return Returns non-zero if matching
 */
XPLINTERNAL int _xpl_is_exclamation(unsigned char _c);
/**
 * @brief Determines whether a char is a colon
 *
 * @param[in] _c - Char to be determined
 * @return Returns non-zero if matching
 */
XPLINTERNAL int _xpl_is_colon(unsigned char _c);
/**
 * @brief Determines whether a char is a blank
 *
 * @param[in] _c - Char to be determined
 * @return Returns non-zero if matching
 */
XPLINTERNAL int _xpl_is_blank(unsigned char _c);
/**
 * @brief Determines whether a char is a seperator
 *
 * @param[in] _c - Char to be determined
 * @return Returns non-zero if matching
 */
XPLINTERNAL int _xpl_is_seperator(unsigned char _c);
/**
 * @brief Trims extra chars
 *
 * @param[in][out] - String to be trimmed
 * @return - Returns count of trimmed chars
 */
XPLINTERNAL int _xpl_trim(const char** _c);
/**
 * @brief Compires two strings
 *
 * @param[in] _s - Source string
 * @param[in] _d - Destination buffer
 * @return - Returns 1 if _s > _d, -1 if _s < _d, 0 if _s = _d
 */
XPLINTERNAL int _xpl_strcmp(const char* _s, const char* _d);
/**
 * @brief Compires scripting programming interface information in quick sorting
 *
 * @param[in] _l - First interface information
 * @param[in] _r - Second interface information
 * @return - Returns 1 if _l > _r, -1 if _l < _r, 0 if _l = _r
 */
XPLINTERNAL int _xpl_func_info_srt_cmp(const void* _l, const void* _r);
/**
 * @brief Compires scripting programming interface information in binary searching
 *
 * @param[in] _k - Searching key
 * @param[in] _i - Interface information
 * @return - Returns 1 if _k > _i, -1 if _k < _i, 0 if _k = _i
 */
XPLINTERNAL int _xpl_func_info_sch_cmp(const void* _k, const void* _i);

/* ========================================================} */

/*
** {========================================================
** Function definitions
*/

XPLAPI xpl_status_t xpl_open(xpl_context_t* _s, xpl_func_info_t* _f) {
  xpl_assert(_s && _f);
  memset(_s, 0, sizeof(xpl_context_t));
  _s->funcs = _f;
  while(_f[_s->funcs_count].name && _f[_s->funcs_count].func)
    _s->funcs_count++;
  qsort(_f, _s->funcs_count, sizeof(xpl_func_info_t), _xpl_func_info_srt_cmp);

  return XS_OK;
}

XPLAPI xpl_status_t xpl_close(xpl_context_t* _s) {
  xpl_assert(_s);
  memset(_s, 0, sizeof(xpl_context_t));

  return XS_OK;
}

XPLAPI xpl_status_t xpl_load(xpl_context_t* _s, const char* _t) {
  xpl_assert(_s && _t);
  if(_s->text) xpl_unload(_s);
  _s->cursor = _s->text = _t;

  return XS_OK;
}

XPLAPI xpl_status_t xpl_reload(xpl_context_t* _s) {
  xpl_assert(_s && _s->text);
  _s->cursor = _s->text;

  return XS_OK;
}

XPLAPI xpl_status_t xpl_unload(xpl_context_t* _s) {
  xpl_assert(_s);
  _s->cursor = _s->text = NULL;

  return XS_OK;
}

XPLAPI xpl_status_t xpl_run(xpl_context_t* _s) {
  xpl_status_t ret = XS_OK;
  xpl_assert(_s && _s->text);
  while(*_s->cursor && ret == XS_OK)
    ret = xpl_step(_s);

  return ret;
}

XPLAPI xpl_status_t xpl_peek_func(xpl_context_t* _s, xpl_func_info_t** _f) {
  xpl_status_t ret = XS_OK;
  xpl_func_info_t* func = NULL;
  xpl_assert(_s && _s->text && _f);
  SKIP_MEANINGLESS(_s);
  if(_xpl_is_comma(*(unsigned char*)_s->cursor)) {
    _s->cursor++;
  } else {
    func = bsearch(_s->cursor, _s->funcs, _s->funcs_count, sizeof(xpl_func_info_t), _xpl_func_info_sch_cmp);
    if(!func) return XS_ERR;
    *_f = func;
  }

  return ret;
}

XPLAPI xpl_status_t xpl_step(xpl_context_t* _s) {
  xpl_status_t ret = XS_OK;
  xpl_func_info_t* func = NULL;
  xpl_assert(_s && _s->text);
  if(ret = xpl_peek_func(_s, &func) != XS_OK) return ret;
  if(!func) return ret;
  _s->cursor += strlen(func->name);
  SKIP_MEANINGLESS(_s);
  if(ret = func->func(_s) != XS_OK) return ret;

  return ret;
}

XPLAPI xpl_status_t xpl_skip_comment(xpl_context_t* _s) {
  xpl_assert(_s && _s->text);
  if(_xpl_is_squote(*(unsigned char*)_s->cursor)) {
    do {
      _s->cursor++;
    } while(*(unsigned char*)_s->cursor != '\0' && !_xpl_is_squote(*(unsigned char*)_s->cursor));
    _s->cursor++;

    return XS_OK;
  }

  return XS_NO_COMMENT;
}

XPLAPI xpl_status_t xpl_has_param(xpl_context_t* _s) {
  xpl_func_info_t* func = NULL;
  xpl_assert(_s && _s->text);
  SKIP_MEANINGLESS(_s);
  xpl_peek_func(_s, &func);

  return ((_xpl_is_comma(*(unsigned char*)_s->cursor) || func) ? XS_NO_PARAM : XS_OK);
}

XPLAPI xpl_status_t xpl_pop_int(xpl_context_t* _s, int* _o) {
  xpl_status_t ret = XS_OK;
  char* conv_suc = NULL;
  char buf[32] = { '\0' };
  xpl_assert(_s && _s->text && _o);
  if(ret = xpl_pop_string(_s, buf, sizeof(buf)) != XS_OK) return ret;
  *_o = (int)strtol(buf, &conv_suc, 0);
  if(*conv_suc != '\0') ret = XS_PARAM_TYPE_ERROR;

  return ret;
}

XPLAPI xpl_status_t xpl_pop_float(xpl_context_t* _s, float* _o) {
  xpl_status_t ret = XS_OK;
  char* conv_suc = NULL;
  char buf[32] = { '\0' };
  xpl_assert(_s && _s->text && _o);
  if(ret = xpl_pop_string(_s, buf, sizeof(buf)) != XS_OK) return ret;
  *_o = (float)strtod(buf, &conv_suc);
  if(*conv_suc != '\0') ret = XS_PARAM_TYPE_ERROR;

  return ret;
}

XPLAPI xpl_status_t xpl_pop_string(xpl_context_t* _s, char* _o, int _l) {
  const char* src = NULL;
  char* dst = NULL;
  xpl_assert(_s && _s->text && _o);
  src = _s->cursor;
  dst = _o;
  if(_xpl_is_dquote(*(unsigned char*)src)) {
    src++;
    while(!_xpl_is_dquote(*(unsigned char*)src)) {
      *dst++ = *src++;
      if(dst + 1 - _o > _l) return XS_NO_ENOUGH_BUFFER_SIZE;
    }
    src++;
  } else {
    while(!_xpl_is_seperator(*(unsigned char*)src)) {
      *dst++ = *src++;
      if(dst + 1 - _o > _l) return XS_NO_ENOUGH_BUFFER_SIZE;
    }
  }
  _s->cursor = src;

  return XS_OK;
}

XPLAPI xpl_status_t xpl_push_bool(xpl_context_t* _s, int _b) {
  xpl_assert(_s && _s->text);
  switch(_s->bool_composing) {
    case XBC_NIL: _s->bool_value = !!_b; break;
    case XBC_OR: _s->bool_value |= !!_b; break;
    case XBC_AND: _s->bool_value &= !!_b; break;
    default: xpl_assert(!"Unknow boolean composing type"); break;
  }

  return XS_OK;
}

XPLINTERNAL xpl_status_t _xpl_core_if(xpl_context_t* _s) {
  xpl_assert(_s && _s->text);

  return XS_OK;
}

XPLINTERNAL xpl_status_t _xpl_core_then(xpl_context_t* _s) {
  xpl_status_t ret = XS_OK;
  xpl_func_info_t* func = NULL;
  xpl_assert(_s && _s->text);
  if(_s->bool_value) {
    _s->bool_value = 0;
    _s->bool_composing = XBC_NIL;
    do {
      if(ret = xpl_peek_func(_s, &func) != XS_OK) return ret;
      if(!func) continue;
      if(func->func == _xpl_core_elseif || func->func == _xpl_core_else || func->func == _xpl_core_endif) break;
      _s->cursor += strlen(func->name);
      SKIP_MEANINGLESS(_s);
      if(ret = func->func(_s) != XS_OK) return ret;
    } while(*_s->cursor);
    do {
      if(ret = xpl_peek_func(_s, &func) != XS_OK) return ret;
      if(!func) continue;
      _s->cursor += strlen(func->name);
      if(func->func == _xpl_core_endif) break;
    } while(*_s->cursor);
  } else {
    _s->bool_value = 0;
    _s->bool_composing = XBC_NIL;
    do {
      xpl_peek_func(_s, &func);
      if(!func) continue;
      if(func->func == _xpl_core_elseif || func->func == _xpl_core_else || func->func == _xpl_core_endif) break;
      _s->cursor += strlen(func->name);
    } while(*_s->cursor);
  }

  return ret;
}

XPLINTERNAL xpl_status_t _xpl_core_elseif(xpl_context_t* _s) {
  xpl_assert(_s && _s->text);

  return XS_OK;
}

XPLINTERNAL xpl_status_t _xpl_core_else(xpl_context_t* _s) {
  xpl_assert(_s && _s->text);

  return XS_OK;
}

XPLINTERNAL xpl_status_t _xpl_core_endif(xpl_context_t* _s) {
  xpl_assert(_s && _s->text);

  return XS_OK;
}

XPLINTERNAL xpl_status_t _xpl_core_or(xpl_context_t* _s) {
  xpl_assert(_s && _s->text);
  _s->bool_composing = XBC_OR;

  return XS_OK;
}

XPLINTERNAL xpl_status_t _xpl_core_and(xpl_context_t* _s) {
  xpl_assert(_s && _s->text);
  _s->bool_composing = XBC_AND;

  return XS_OK;
}

XPLINTERNAL xpl_status_t _xpl_core_yield(xpl_context_t* _s) {
  xpl_assert(_s && _s->text);

  return XS_SUSPENT;
}

XPLINTERNAL int _xpl_is_squote(unsigned char _c) {
  return _c == '\'';
}

XPLINTERNAL int _xpl_is_dquote(unsigned char _c) {
  return _c == '"';
}

XPLINTERNAL int _xpl_is_comma(unsigned char _c) {
  return _c == ',';
}

XPLINTERNAL int _xpl_is_exclamation(unsigned char _c) {
  return _c == '!';
}

XPLINTERNAL int _xpl_is_colon(unsigned char _c) {
  return _c == ':';
}

XPLINTERNAL int _xpl_is_blank(unsigned char _c) {
  return _c == ' ' || _c == '\t' || _c == '\r' || _c == '\n';
}

XPLINTERNAL int _xpl_is_seperator(unsigned char _c) {
  return _xpl_is_blank(_c) || _xpl_is_comma(_c) ||
    _xpl_is_exclamation(_c) || _xpl_is_colon(_c) ||
    _xpl_is_squote(_c) || _xpl_is_dquote(_c);
}

XPLINTERNAL int _xpl_trim(const char** _c) {
  int ret = 0;
  xpl_assert(_c && *_c);
  while(_xpl_is_blank(**_c)) {
    ret++;
    (*_c)++;
  }

  return ret;
}

XPLINTERNAL int _xpl_strcmp(const char* _s, const char* _d) {
  int ret = 0;
  while(!(ret = (_xpl_is_seperator(*(unsigned char*)_s) ? '\0' : *(unsigned char*)_s) - *(unsigned char*)_d) && *_d) {
    _s++; _d++;
  }
  if(ret < 0) ret = -1;
  else if(ret > 0) ret = 1;

  return ret;
}

XPLINTERNAL int _xpl_func_info_srt_cmp(const void* _l, const void* _r) {
  xpl_func_info_t* l = (xpl_func_info_t*)_l;
  xpl_func_info_t* r = (xpl_func_info_t*)_r;
  xpl_assert(l && r);

  return _xpl_strcmp(l->name, r->name);
}

XPLINTERNAL int _xpl_func_info_sch_cmp(const void* _k, const void* _i) {
  const char* k = (const char*)_k;
  xpl_func_info_t* i = (xpl_func_info_t*)_i;
  xpl_assert(k && i);

  return _xpl_strcmp(k, i->name);
}

/* ========================================================} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XPL_H__ */
