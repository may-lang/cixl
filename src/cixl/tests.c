#include "cixl/cx.h"
#include "cixl/error.h"
#include "cixl/eval.h"
#include "cixl/scope.h"
#include "cixl/tests.h"

static void run(struct cx *cx, const char *in) {
  cx_vec_clear(&cx_scope(cx, 0)->stack);
  
  if (!cx_eval_str(cx, in)) {
    printf("%s\n", in);
    
    cx_do_vec(&cx->errors, struct cx_error, e) {
      printf("Error in row %d, col %d:\n%s\n", e->row, e->col, e->msg);
      cx_error_deinit(e);
    }
    
    cx_vec_clear(&cx->errors);
  }
}

static void int_tests() {
  struct cx cx;
  cx_init(&cx);

  run(&cx, "42 ? test");
  run(&cx, "0 ? not test");
  run(&cx, "21 + 21 = 42 test");
  run(&cx, "1 = 2 not test");

  cx_deinit(&cx);
}

static void stack_tests() {
  struct cx cx;
  cx_init(&cx);

  run(&cx, "7 14 dup + 28 = test");
  run(&cx, "7 14 dup zap + 21 = test");

  cx_deinit(&cx);
}

static void group_tests() {
  struct cx cx;
  cx_init(&cx);

  run(&cx, "(7 14 21) 21 = test");

  cx_deinit(&cx);
}

static void func_tests() {
  struct cx cx;
  cx_init(&cx);

  run(&cx, "func: foo() 42; foo = 42 test");
  run(&cx, "func: foo() 42; &foo call = 42 test");
  run(&cx, "func: bar(x A) $x + 35; bar 7 42 = test");
  run(&cx, "func: baz(x y Int z 0) $x + $y + $z; baz 1 3 5 9 = test");

  cx_deinit(&cx);
}

static void coro_tests() {
  struct cx cx;
  cx_init(&cx);

  run(&cx, "(1 2 yield 3) call + 5 = test");
  run(&cx, "({1 2 yield 3} call call) + 5 = test");
  run(&cx, "func: foo() 1 2 yield 3; foo call + 5 = test");
  run(&cx, "(let: x 42; yield $x) call 42 = test");
  run(&cx, "(let: x 42; (yield $x)) call 42 = test");

  run(&cx,
      "{1 2 yield cls 3 4} "
      "func: bar(x Lambda) $x call call; "
      "bar + 6 = test");

  //run(&cx, "(if true {1 yield 2} {} 3) call");
  
  cx_deinit(&cx);
}

void cx_tests() {
  int_tests();
  stack_tests();
  group_tests();
  func_tests();
  coro_tests();
}
