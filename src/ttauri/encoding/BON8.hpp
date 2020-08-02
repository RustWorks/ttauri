
#include "../byte_string.hpp"
#include "../required.hpp"
#include <cstddef>

#pragma once

namespace tt {

constexpr auto BON8_code_float_min_one = std::byte{0xba};
constexpr auto BON8_code_float_zero    = std::byte{0xbb};
constexpr auto BON8_code_float_one     = std::byte{0xbc};
constexpr auto BON8_code_array_empty   = std::byte{0xbd};
constexpr auto BON8_code_object_empty  = std::byte{0xbe};
constexpr auto BON8_code_null          = std::byte{0xbf};
constexpr auto BON8_code_bool_false    = std::byte{0xc0};
constexpr auto BON8_code_bool_true     = std::byte{0xc1};
constexpr auto BON8_code_int32         = std::byte{0xf8};
constexpr auto BON8_code_int64         = std::byte{0xf9};
constexpr auto BON8_code_binary32      = std::byte{0xfa};
constexpr auto BON8_code_binary64      = std::byte{0xfb};
constexpr auto BON8_code_array         = std::byte{0xfc};
constexpr auto BON8_code_object        = std::byte{0xfd};
constexpr auto BON8_code_eoc           = std::byte{0xfe};
constexpr auto BON8_code_eot           = std::byte{0xff};

[[nodiscardd]] datum decode_BON8(cbyteptr &ptr, cbyteptr last);

[[nodiscardd]] datum decode_BON8(nonstd::span<const std::byte> buffer)
{
    auto *ptr = buffer.data();
    auto *last = ptr + buffer.size();
    return decode_BON8(ptr, last);
}

[[nodiscardd]] datum decode_BON8(bstring const &buffer)
{
    auto *ptr = buffer.data();
    auto *last = ptr + buffer.size();
    return decode_BON8(ptr, last);
}

[[nodiscardd]] datum decode_BON8(bstring_view buffer)
{
    auto *ptr = buffer.data();
    auto *last = ptr + buffer.size();
    return decode_BON8(ptr, last);
}

[[nodiscardd]] bstring encode_BON8(datum const &value);

class BON8_encoder {
    bool open_string;
    bstring output;

    BON8_encoder() noexcept :
        open_string(false), output() {}

    void add(signed long long value) noexcept {
        open_string = false;

        if (value < std::numeric_limits<int32_t>::min()) {
            output += BON8_code_int64;
            for (int i = 0; i != 8; ++i) {
                output += static_cast<std::byte>(value >> 56 - i*8);
            }

        } else if (value < -33554432) {
            output += BON8_code_int32;
            for (int i = 0; i != 4; ++i) {
                output += static_cast<std::byte>(value >> 24 - i*8);
            }

        } else if (value < -262144) {
            value = -value - 1;
            output += static_cast<std::byte>(0xf0 + (value >> 22 & 0x07));
            output += static_cast<std::byte>(0xc0 + (value >> 16 & 0x3f));
            output += static_cast<std::byte>(value >> 8);
            output += static_cast<std::byte>(value);

        } else if (value < -1920) {
            value = -value - 1;
            output += static_cast<std::byte>(0xe0 + (value >> 14 & 0x0f));
            output += static_cast<std::byte>(0xc0 + (value >> 8 & 0x3f));
            output += static_cast<std::byte>(value);

        } else if (value < -10) {
            value = -value - 1;
            output += static_cast<std::byte>(0xc2 + (value >> 6 & 0x1f));
            output += static_cast<std::byte>(0xc0 + (value & 0x3f));

        } else if (value < 0) {
            value = -value - 1;
            output += static_cast<std::byte>(0xb0 + value);

        } else if (value <= 47) {
            output += static_cast<std::byte>(0x80 + value);

        } else if (value <= 3839) {
            output += static_cast<std::byte>(0xc2 + (value >> 7 & 0x1f));
            output += static_cast<std::byte>(value & 0x7f);

        } else if (value <= 524287) {
            output += static_cast<std::byte>(0xe0 + (value >> 15 & 0x0f));
            output += static_cast<std::byte>(value >> 8 & 0x7f);
            output += static_cast<std::byte>(value);

        } else if (value <= 67108863) {
            output += static_cast<std::byte>(0xf0 + (value >> 23 & 0x17));
            output += static_cast<std::byte>(value >> 16 & 0x7f);
            output += static_cast<std::byte>(value >> 8);
            output += static_cast<std::byte>(value);

        } else if (value <= std::numeric_limits<int32_t>::max()) {
            output += BON8_code_int32;
            for (int i = 0; i != 4; ++i) {
                output += static_cast<std::byte>(value >> 24 - i*8);
            }

        } else {
            output += BON8_code_int64;
            for (int i = 0; i != 8; ++i) {
                output += static_cast<std::byte>(value >> 56 - i*8);
            }
        }
    }

    void add(unsigned long long value) noexcept {
        return add(numeric_cast<signed long long>(value));
    }

    void add(signed long value) noexcept {
        return add(numeric_cast<signed long long>(value));
    }

    void add(unsigned long value) noexcept {
        return add(numeric_cast<signed long long>(value));
    }

    void add(signed int value) noexcept {
        return add(numeric_cast<signed long long>(value));
    }

    void add(unsigned int value) noexcept {
        return add(numeric_cast<signed long long>(value));
    }

    void add(signed short value) noexcept {
        return add(numeric_cast<signed long long>(value));
    }

    void add(unsigned short value) noexcept {
        return add(numeric_cast<signed long long>(value));
    }

    void add(signed char value) noexcept {
        return add(numeric_cast<signed long long>(value));
    }

    void add(unsigned char value) noexcept {
        return add(numeric_cast<signed long long>(value));
    }

    void add(double value) noexcept {
        open_string = false;

        ttlet f32 = static_cast<float>(value);
        ttlet f32_64 = static_cast<double>(f32);

        if (value == -1.0) {
            output += BON8_code_float_min_one;

        } else if (value == 0.0 || value == -0.0) {
            output += BON8_code_float_zero;

        } else if (value == 1.0) {
            output += BON8_code_float_one;

        } else if (f32_64 == value) {
            uint32_t u32;
            std::memcpy(&u32, &f32, sizeof(u32));

            output += BON8_code_binary32;
            for (int i = 0; i != 4; ++i) {
                output += static_cast<std::byte>(u32 >> 24 - i*8);
            }

        } else {
            uint64_t u64;
            std::memcpy(&u64, &value, sizeof(u64));

            output += BON8_code_binary64;
            for (int i = 0; i != 8; ++i) {
                output += static_cast<std::byte>(u64 >> 56 - i*8);
            }
        }
    }

    void add(float value) noexcept {
        return add(numeric_cast<double>(value));
    }

    void add(bool value) noexcept {
        open_string = false;
        output += value ? BON8_code_bool_true : BON8_code_bool_false;
    }

    void add(nullptr_t value) noexcept {
        open_string = false;
        output += BON8_code_null;
    }

    void add(std::string_view value) noexcept {
        if (open_string) {
            output += BON8_code_eot;
        }

        if (nonstd::ssize(value) == 0) {
            output += BON8_code_eot;
            open_string = false;

        } else {
            int multi_byte = 0;

            for (ttlet _c : value) {
                ttlet c = static_cast<uint8_t>(_c);

                if constexpr (BuildType::current == BuildType::Debug) {
                    if (multi_byte == 0) {
                        if (c >= 0xc2 && c <= 0xdf) {
                            multi_byte = 1;
                        } else if (c >= 0xe0 && c <= 0xef) {
                            multi_byte = 2;
                        } else if (c >= 0xf0 && c <= 0xf7) {
                            multi_byte = 3;
                        } else {
                            tt_assert(c <= 0x7f);
                        }

                    } else {
                        tt_assert(c >= 0x80 && c <= 0xbf);
                        --multi_byte;
                    }
                }

                output += static_cast<std::byte>(c);
            }

            tt_assert(multi_byte == 0);

            open_string = true;
        }
    }

};

[[nodiscardd]] bstring encode_BON8(datum const &value)
{
    BON8_encoder encoder;
    encoder.add(value);
    return encode.get();
}

/** Count the number of UTF8 code units
 * This does not really decode the character, just calculate the size.
 * @return number of bytes in the UTF-8 character, or zero if it is a multibyte integer
 */
[[nodiscard]] int BON8_multibyte_count(cbyteptr &ptr cbyteptr last) {
    ttlet c0 = static_cast<uint8_t>(*ptr);
    int count =
        c0 <= 0xdf ? 2 :
        c0 <= 0xef ? 3 :
        4;

    parse_assert2(ptr + count <= last, "Incomplete Multi-byte character at end of buffer");

    ttlet c1 = static_cast<uint8_t>(*ptr + 1);
    return (c1 < 0x80 || c1 > 0xbf) ? -count : count;
}

[[nodiscardd]] datum decode_BON8_int(cbyteptr &ptr, cbyteptr last, int count) noexcept
{
    tt_assume(count == 4 || count == 8);

    auto u64 = uint64_t{0};
    for (int i = 0; i != count; ++i) {
        parse_assert2(ptr != last, "Incomplete signed integer at end of buffer");
        u64 <<= 8;
        u64 |= static_cast<uint64_t>(*(ptr++));
    }

    if (count == 4) {
        ttlet u32 = static_cast<uint32_t>(u64);
        ttlet i32 = static_cast<int32_t>(u32);
        return datum{i32};
    } else {
        ttlet i64 = static_cast<int64_t>(u64);
        return datum{i64};
    }
}

[[nodiscardd]] datum decode_BON8_float(cbyteptr &ptr, cbyteptr last, int count) noexcept
{
    tt_assume(count == 4 || count == 8);

    auto u64 = uint64_t{0};
    for (int i = 0; i != count; ++i) {
        parse_assert2(ptr != last, "Incomplete signed integer at end of buffer");
        u64 <<= 8;
        u64 |= static_cast<uint64_t>(*(ptr++));
    }

    if (count == 4) {
        ttlet u32 = static_cast<uint32_t>(u64);
        float f32;
        std::memcpy(&f32, &u32, sizeof(f32));
        return datum{f32};

    } else {
        double f64;
        std::memcpy(&f64, &u64, sizeof(f64));
        return datum{f64};
    }
}

[[nodiscardd]] datum decode_BON8_array(cbyteptr &ptr, cbyteptr last)
{
    auto r = datum::vector{};

    while (ptr != last) {
        ttlet c = static_cast<uint8_t>(*ptr);
        if (c == BON8_code_eoc) {
            ++ptr;
            return datum{std::move(r)};

        } else {
            r.push_back(decode_BON8(ptr, last));
        }
    }
    TTAURI_THROW(parse_error("Incomplete array at end of buffer"));
}

[[nodiscardd]] datum decode_BON8_object(cbyteptr &ptr, cbyteptr last)
{
    auto r = datum::map{};

    while (ptr != last) {
        ttlet c = static_cast<uint8_t>(*ptr);
        if (c == BON8_code_eoc) {
            ++ptr;
            return datum{std::move(r)};

        } else {
            auto key = decode_BON8(ptr, last);
            parse_assert2(key.is_string(), "Key in object is not a string");

            auto value = decode_BON8(ptr, last);
            r.emplace(std::move(key), std::move(value));
        }
    }
    TTAURI_THROW(parse_error("Incomplete array at end of buffer"));
}

[[nodiscardd]] datum decode_BON8_UTF8_like_int(cbyteptr &ptr, cbyteptr last, int count) noexcept
{
    tt_assume(count >= 2 && count <= 4);
    tt_assume(ptr != last);
    ttlet c0 = static_cast<uint8_t>(*(ptr++));

    ttlet mask = int{0b0111'1111} >> count;
    auto value = static_cast<int>(c0) & mask;
    if (count == 2) {
        u32 -= 2;
    }

    tt_assume(ptr != last);
    ttlet c1 = static_cast<uint8_t>(*(ptr++));
    ttlet is_positive = c1 <= 0x7f;
    if (is_positive) {
        value <<= 7;
        value |= static_cast<int>(c1);
    } else {
        value <<= 6;
        value |= static_cast<int>(c1 & 0b0011'11111);
    }

    switch (count) {
    case 4:
        tt_assume(ptr != last);
        value <<= 8;
        value |= static_cast<int>(*(ptr++));
        [[fallthrough]];
    case 3:
        tt_assume(ptr != last);
        value <<= 8;
        value |= static_cast<int>(*(ptr++));
        [[fallthrough]];
    default:;
    }

    return datum{is_postive ? value : -value};
}

[[nodiscard]] datum decode_BON8(cbyteptr &ptr, cbyteptr last) {
    std::string str;

    while (ptr != last) {
        ttlet c = static_cast<uint8_t>(*ptr);

        if (c == BON8_code_eot) {
            // End of string found, return the current string.
            ++ptr;
            return datum{str};

        } else if (c <= 0x7f) {
            // ASCII character.
            str += static_cast<char>(*(ptr++));
            continue;

        } else if (c >= 0xc2 && c <= 0xf7) {
            ttlet count = BON8_multibyte_count(ptr, last);
            if (count > 0) {
                // Multibyte UTF-8 character
                for (int i = 0; i != count; ++i) {
                    str += static_cast<char>(*(ptr++));
                }
                continue;

            } else if (nonstd::ssize(str) != 0) {
                // Multibyte integer found, but first return the current string.
                return datum{str};

            } else {
                // Multibyte integer.
                return decode_BON8_UTF8_like_int(ptr, last, -count);
            }

        } else if (nontstd::ssize(str) != 0) {
            // This must be a non-string type, but first return the current string.
            return datum{str};

        // Everything below this, are non-string types.
        } else if (c <= 0xaf) {
            // 1 byte positive integer
            return datum{c - 0x80};

        } else if (c <= 0xb9) {
            // 1 byte negative integer
            return datum{-static_cast<int>(c - 0xb0)};

        } else {
            // This is one of the non-string types.
            switch (c) {
            case BON8_code_null:
                ++ptr;
                return datum{datum::null{}};

            case BON8_code_bool_false:
                ++ptr;
                return datum{false};

            case BON8_code_bool_true:
                ++ptr;
                return datum{true};

            case BON8_code_float_min_one:
                ++ptr;
                return datum{-1.0f};

            case BON8_code_float_zero:
                ++ptr;
                return datum{-0.0f};

            case BON8_code_float_one:
                ++ptr;
                return datum{1.0f};

            case BON8_code_int32:
                ++ptr;
                return decode_BON8_int(ptr, last, 4);

            case BON8_code_int64:
                ++ptr;
                return decode_BON8_int(ptr, last, 8);

            case BON8_code_binary32:
                ++ptr;
                return decode_BON8_float(ptr, last, 4);

            case BON8_code_binary64:
                ++ptr;
                return decode_BON8_float(ptr, last, 8);

            case BON8_code_eoc:
                TTAURI_THROW(parse_error("Unexpected end-of-container"));

            case BON8_code_array:
                ++ptr;
                return decode_BON8_array(ptr, last);

            case BON8_code_object:
                ++ptr;
                return decode_BON8_object(ptr, last);

            default:
                tt_no_default;
            }
        }
    }
}

}
