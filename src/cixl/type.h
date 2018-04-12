#ifndef CX_TYPE_H
#define CX_TYPE_H

#include <stdint.h>
#include <stdio.h>
#include "cixl/set.h"

#define cx_type_push_args(t, ...) ({					\
      struct cx_type *args[] = {__VA_ARGS__};				\
      unsigned int nargs = sizeof(args) / sizeof(struct cx_type *);	\
      cx_type_vpush_args(t, nargs, args);				\
    })									\

#define cx_type_get(t, ...) ({						\
      struct cx_type *args[] = {__VA_ARGS__};				\
      unsigned int nargs = sizeof(args) / sizeof(struct cx_type *);	\
      cx_type_vget(t, nargs, args);					\
    })									\

struct cx;
struct cx_box;
struct cx_iter;
struct cx_scope;

struct cx_type {
  struct cx_lib *lib;
  char *id, *emit_id;
  size_t tag, level;
  struct cx_type *raw;
  struct cx_set parents, children;
  struct cx_vec is, args;
  bool trait;
  
  void (*new)(struct cx_box *);
  bool (*eqval)(struct cx_box *, struct cx_box *);
  bool (*equid)(struct cx_box *, struct cx_box *);
  enum cx_cmp (*cmp)(const struct cx_box *, const struct cx_box *);
  bool (*call)(struct cx_box *, struct cx_scope *);
  bool (*ok)(struct cx_box *);
  void (*copy)(struct cx_box *dst, const struct cx_box *src);
  void (*clone)(struct cx_box *dst, struct cx_box *src);
  void (*iter)(struct cx_box *, struct cx_box *);
  void (*write)(struct cx_box *, FILE *);
  void (*dump)(struct cx_box *, FILE *);
  void (*print)(struct cx_box *, FILE *);
  bool (*emit)(struct cx_box *, const char *, FILE *);
  void (*deinit)(struct cx_box *);

  struct cx_type *(*type_new)(struct cx_type *,
			      const char *id,
			      int nargs, struct cx_type *args[]);

  void (*type_init)(struct cx_type *, int nargs, struct cx_type *args[]);
  void *(*type_deinit)(struct cx_type *);
};

struct cx_type *cx_type_new(struct cx_lib *lib, const char *id);

struct cx_type *cx_type_init(struct cx_type *type,
			     struct cx_lib *lib,
			     const char *id);
struct cx_type *cx_type_reinit(struct cx_type *type);
void *cx_type_deinit(struct cx_type *type);

void cx_type_vpush_args(struct cx_type *t, int nargs, struct cx_type *args[]);
struct cx_type *cx_type_vget(struct cx_type *t, int nargs, struct cx_type *args[]);

void cx_derive(struct cx_type *child, struct cx_type *parent);
bool cx_is(struct cx_type *child, struct cx_type *parent);

struct cx_type *cx_init_meta_type(struct cx_lib *lib);

#endif
