#pragma once

constexpr uint64_t do_hash(const char *x) {
    // ap hash
    uint64_t rval = 0;
    for (size_t i = 0; x[i]; ++i) {
        if (i & 1) {
            rval ^= (~((rval << 11) ^ x[i] ^ (rval >> 5)));
        } else {
            rval ^= (~((rval << 7) ^ x[i] ^ (rval >> 3)));
        }
    }
    return rval;
}

constexpr uint64_t do_hash(const char *x, int len) {
    // ap hash
    uint64_t rval = 0;
    for (size_t i = 0; i < len; ++i) {
        if (i & 1) {
            rval ^= (~((rval << 11) ^ x[i] ^ (rval >> 5)));
        } else {
            rval ^= (~((rval << 7) ^ x[i] ^ (rval >> 3)));
        }
    }
    return rval;
}
template<size_t N>
struct FixedString {
    char buf[N + 1]{};

    constexpr FixedString(char const *s) {
        for (unsigned i = 0; i != N; ++i)
            buf[i] = s[i];
    }

    constexpr operator const char *() const {
        return buf;
    }

    constexpr const char *c_str() const {
        return buf;
    }
};

template<size_t N>
FixedString(char const (&)[N]) -> FixedString<N - 1>;

