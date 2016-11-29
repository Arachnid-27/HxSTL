#ifndef _STRING_H_
#define _STRING_H_


#include "basic_string.h"


namespace HxSTL {

    typedef HxSTL::basic_string<char>       string;
    typedef HxSTL::basic_string<wchar_t>    wstring; 
    typedef HxSTL::basic_string<char16_t>   u16string; 
    typedef HxSTL::basic_string<char32_t>   u32string; 

}


#endif
