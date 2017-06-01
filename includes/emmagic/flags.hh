#pragma once

// If enabled, the std converters will use typed array when possible instead of regular arrays

#define EMMAGIC_USE_TYPED_ARRAYS (1 << 1)
#define EMMAGIC_NO_TYPED_ARRAYS (1 << 2)

// If enabled, the std converters will use "new Map()" instead of returning regular objects

#define EMMAGIC_USE_REAL_MAPS (1 << 3)
#define EMMAGIC_NO_REAL_MAPS (1 << 4)

// If enabled, the std converters will use "new Set()" instead of returning regular arrays

#define EMMAGIC_USE_REAL_SETS (1 << 5)
#define EMMAGIC_NO_REAL_SETS (1 << 6)

// If enabled, the std converters will try to coerce any value that would require it - has an heavy performance impact

#define EMMAGIC_COERCE_TYPES (1 << 7)
#define EMMAGIC_DONT_COERCE_TYPES (1 << 8)

// If enabled, the std converters will only try to coerce keys, not values - has a lower performance impact

#define EMMAGIC_COERCE_KEYS (1 << 9)
#define EMMAGIC_DONT_COERCE_KEYS (1 << 10)

#define EMMAGIC_HAS_FLAG(FLAGS, FLAG) (((EMMAGIC_DEFAULT_FLAGS) & (FLAG)) ? ((FLAGS) & ((FLAG) << 1)) == 0 : ((FLAGS) & (FLAG)) != 0)
