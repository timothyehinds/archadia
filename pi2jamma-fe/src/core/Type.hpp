#pragma once

#include <inttypes.h>
#include <string>

#define OmRef ref
#define OmFs(x, ...) formatString((x),__VA_ARGS__)
#define OmSl(x) x
#define OmVector std::vector
#define Sl(x) x
#define Fs(x,...) formatString((x),__VA_ARGS__)
#define Ts(x) x
#define OmTs(x) x

using OmString = std::string;

using Utf8Char = char;
using OmUtf8Char = char;
using OmInt = int;
using OmUInt = unsigned int;
using OmBool = bool;
using OmFloat64 = double;
using OmFloat32 = float;
using OmFloat = float;
using OmChar = char;
using OmUInt8 = unsigned char;
using OmUInt32 = uint32_t;
using OmInt32 = int32_t;
using OmInt64 = int64_t;
using OmUInt64 = uint64_t;
using OmUnicode16Char = uint16_t;

const bool OmFalse = false;
const bool OmTrue = true;

#define OM_ASSERT ASSERT
