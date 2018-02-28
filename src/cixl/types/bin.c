#include <stdbool.h>

#include "cixl/bin.h"
#include "cixl/buf.h"
#include "cixl/box.h"
#include "cixl/cx.h"
#include "cixl/error.h"
#include "cixl/op.h"
#include "cixl/tok.h"
#include "cixl/scope.h"
#include "cixl/types/bin.h"
#include "cixl/types/fimp.h"
#include "cixl/types/func.h"

static void new_imp(struct cx_box *out) {
  out->as_ptr = cx_bin_new();
}

static bool equid_imp(struct cx_box *x, struct cx_box *y) {
  return x->as_ptr == y->as_ptr;
}

static bool call_imp(struct cx_box *value, struct cx_scope *scope) {
  return cx_eval(value->as_ptr, 0, scope->cx);
}

static void copy_imp(struct cx_box *dst, const struct cx_box *src) {
  struct cx_bin *b = src->as_ptr;
  dst->as_ptr = cx_bin_ref(b);
}

static void dump_imp(struct cx_box *value, FILE *out) {
  struct cx_bin *b = value->as_ptr;
  fprintf(out, "Bin(%p)r%d", b, b->nrefs);
}

static void deinit_imp(struct cx_box *value) {
  struct cx_bin *b = value->as_ptr;
  cx_bin_deref(b);
}

struct cx_type *cx_init_bin_type(struct cx *cx) {
  struct cx_type *t = cx_add_type(cx, "Bin", cx->any_type);
  t->new = new_imp;
  t->equid = equid_imp;
  t->call = call_imp;
  t->copy = copy_imp;
  t->dump = dump_imp;
  t->deinit = deinit_imp;

  return t;
}
