#pragma once

#include <stdbool.h>
#include <stddef.h>

/*
 * On-calculator standalone academic engine.
 * Returns true when it produced an answer.
 */
bool qb_offline_answer(
    char const *subject,
    char const *mode,
    char const *level,
    char const *prompt,
    char *out,
    size_t out_size
);
