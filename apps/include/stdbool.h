#pragma once

#define __bool_true_false_are_defined 1

#ifndef __cplusplus
#define bool _Bool
#define true 1
#define false 0
#elif defined(__GNUC__) && !defined(__STRICT_ANSI__)
#define _Bool bool
#endif