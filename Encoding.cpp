
#include "Encoding.h"
#include <Windows.h>

std::string CEncoding::Wide2Default(std::wstring in)
{
    std::string outstr = "";

    char defaultchar[2] = { (char)32, 0 };
    BOOL b = NULL;

    size_t outmaxlen = in.length() * 4;
    char *out;

    out = reinterpret_cast<char *>(malloc(outmaxlen));
    size_t outlen = WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, in.c_str(), static_cast<int>(in.length()), out, static_cast<int>(outmaxlen), &defaultchar[0], &b);
    out[outlen] = 0;

    outstr.append(out);
    free(out);

    return outstr;
}
