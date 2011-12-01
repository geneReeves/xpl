/**
 * Author: paladin_t, hellotony521@gmail.com
 * Created: Oct. 14, 2011
 * Last edited: Oct. 15, 2011
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details.
 */

#include "xpl.h"

xpl_status_t test1(xpl_context_t* _s) {
  float f = 0.0f;
  printf("test1\n");
  if(xpl_has_param(_s) == XS_OK) {
    xpl_pop_float(_s, &f);
    printf("has_param %f\n", f);
  }

  return XS_OK;
}

xpl_status_t test2(xpl_context_t* _s) {
  char buf[64] = { '\0' };
  printf("test2\n");
  if(xpl_has_param(_s) == XS_OK) {
    xpl_pop_string(_s, buf, 64);
    printf("has_param %s\n", buf);
  }

  return XS_OK;
}

xpl_status_t test3(xpl_context_t* _s) {
  printf("test3\n");

  return XS_OK;
}

xpl_status_t cond1(xpl_context_t* _s) {
  printf("cond1\n");
  xpl_push_bool(_s, 0);

  return XS_OK;
}

xpl_status_t cond2(xpl_context_t* _s) {
  printf("cond2\n");
  xpl_push_bool(_s, 1);

  return XS_OK;
}

static xpl_context_t xpl;

int main() {
  XPL_FUNC_BEGIN(funcs)
    XPL_FUNC_ADD("test3", test3)
    XPL_FUNC_ADD("test2", test2)
    XPL_FUNC_ADD("test1", test1)
    XPL_FUNC_ADD("cond2", cond2)
    XPL_FUNC_ADD("cond1", cond1)
  XPL_FUNC_END

  xpl_open(&xpl, funcs);
    xpl_load(&xpl, "if cond1 then test1 3.14 elseif cond2 then test2 \"hello world\" else test3 endif");
    xpl_run(&xpl);
    xpl_unload(&xpl);
  xpl_close(&xpl);

  return 0;
}
