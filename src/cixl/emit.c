#include <stdlib.h>
#include <string.h>

#include "cixl/bin.h"
#include "cixl/cx.h"
#include "cixl/emit.h"

char *cx_emit_id(const char *prefix, const char *in) {
  char *out = malloc(strlen(prefix)+strlen(in)*2+2), *p = out;
  for (const char *c=prefix; *c; c++) { *p++ = *c; }
  *p++ = '_';
  
  for (const char *c=in; *c; c++) {
    switch (*c) {
    case ' ':
      break;
    case '+':
      *p++ = 'A';
      break;
    case '`':
      *p++ = 'B';
    case '&':
      *p++ = 'C';
      break;
    case '|':
      *p++ = 'D';
      break;
    case '/':
      *p++ = 'E';
      break;
    case '.':
      *p++ = 'F';
      break;      
    case '=':
      *p++ = 'G';
      break;
    case '!':
      *p++ = 'H';
      break;
    case '<':
      *p++ = 'I';
      break;
    case '>':
      *p++ = 'J';
      break;
    case '*':
      *p++ = 'K';
      break;
    case '%':
      *p++ = 'L';
      break;
    case '?':
      *p++ = 'M';
      break;
    case '-':
      *p++ = 'N';
      break;
    case '~':
      *p++ = 'S';
      break;
    default:
      *(p++) = *c;
    }
  }

  *p = 0;
  return out;
}

bool cx_emit_file(struct cx *cx, const char *fname, FILE *out) {
  struct cx_bin *bin = cx_bin_new();
  bool ok = false;
  
  if (!cx_load(cx, fname, bin)) { goto exit; }

  fputs("#include <stdlib.h>\n"
	"#include <time.h>\n"
	"#include \"cixl/arg.h\"\n"
	"#include \"cixl/bin.h\"\n"
	"#include \"cixl/call.h\"\n"
	"#include \"cixl/catch.h\"\n"
	"#include \"cixl/cx.h\"\n"
	"#include \"cixl/error.h\"\n"
	"#include \"cixl/func.h\"\n"
	"#include \"cixl/lambda.h\"\n"
	"#include \"cixl/rec.h\"\n"
	"#include \"cixl/op.h\"\n"
	"#include \"cixl/scope.h\"\n"
	"#include \"cixl/stack.h\"\n"
	"#include \"cixl/str.h\"\n\n",
	out);
  
  if (!cx_emit(bin, out, cx)) { goto exit; }
      
  fputs("int main(int argc, char *argv[]) {\n"
	"  srand((ptrdiff_t)argv + clock());\n"
	"  struct cx cx;\n"
	"  cx_init(&cx);\n"
	"  cx_init_libs(&cx);\n"
	"  cx_use(&cx, \"cx/abc\", \"Str\");\n\n"
	"  for (int i=1; i < argc; i++) {\n"
	"    cx_box_init(cx_push(cx.root_scope), cx.str_type)->as_str = "
	      "cx_str_new(argv[i]);\n"
	"  }\n\n"
	"  if (!eval(&cx)) {\n"
	"    cx_dump_errors(&cx, stderr);\n"
	"    return -1;\n"
	"  }\n\n"
	"  cx_deinit(&cx);\n"
	"  return 0;\n"
	"}",
	out);

  ok = true;
 exit:
  cx_bin_deref(bin);
  return ok;
}
