#ifndef _QUICKJS_BRIDGE_H
#define _QUICKJS_BRIDGE_H

#include <stdlib.h>
#include <time.h>
#include "quickjs.h"

typedef struct {
    time_t start_time;
    double timeout_seconds;
} InterruptValue;

extern JSValue proxy(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);

static JSValue JS_NewNull() {
    return JS_NULL;
}

static JSValue JS_NewUndefined() {
    return JS_UNDEFINED;
}

static JSValue JS_NewUninitialized() {
    return JS_UNINITIALIZED;
}

static JSValue ThrowSyntaxError(JSContext *ctx, const char *fmt) {
    return JS_ThrowSyntaxError(ctx, "%s", fmt);
}

static JSValue ThrowTypeError(JSContext *ctx, const char *fmt) {
    return JS_ThrowTypeError(ctx, "%s", fmt);
}

static JSValue ThrowReferenceError(JSContext *ctx, const char *fmt) {
    return JS_ThrowReferenceError(ctx, "%s", fmt);
}

static JSValue ThrowRangeError(JSContext *ctx, const char *fmt) {
    return JS_ThrowRangeError(ctx, "%s", fmt);
}

static JSValue ThrowInternalError(JSContext *ctx, const char *fmt) {
    return JS_ThrowInternalError(ctx, "%s", fmt);
}

static int InterruptHandler(JSRuntime *rt, void *opaque) {
    InterruptValue *val = (InterruptValue *)opaque;
    time_t now = time(NULL);
    if (difftime(now, val->start_time) >= val->timeout_seconds) {
        return 1;
    }
    return 0;
}

static void SetInterruptHandler(JSContext *ctx, double timeout, InterruptValue *val) {
    JSRuntime *rt = JS_GetRuntime(ctx);
    val->start_time = time(NULL);
    val->timeout_seconds = timeout;
    JS_SetInterruptHandler(rt, InterruptHandler, val);
}

#endif
