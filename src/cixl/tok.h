#ifndef CX_TOK_H
#define CX_TOK_H

#include "cixl/box.h"

#define cx_tok_type(id, ...)			\
  struct cx_tok_type *id() {			\
    static struct cx_tok_type type;		\
    static bool init = true;			\
						\
    if (init) {					\
      __VA_ARGS__;				\
    }						\
						\
    return &type;				\
  }						\

struct cx_tok;

struct cx_tok_type {
  bool (*eval)(struct cx_tok *, struct cx *);
  void (*copy)(struct cx_tok *dst, struct cx_tok *src);
  void (*deinit)(struct cx_tok *);
};

struct cx_tok_type *cx_tok_type_init(struct cx_tok_type *type);

struct cx_tok {
  struct cx_tok_type *type;
  int row, col;

  union {
    struct cx_box as_box;
    struct cx_vec as_vec;
    void *as_ptr;
  };
};

struct cx_tok *cx_tok_init(struct cx_tok *tok,
			   struct cx_tok_type *type,
			   int row, int col);

struct cx_tok *cx_tok_deinit(struct cx_tok *tok);
void cx_tok_copy(struct cx_tok *dst, struct cx_tok *src);

#endif
