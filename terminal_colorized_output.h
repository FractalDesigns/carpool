//
//  terminal_colorized_output.h
//  
//
//  Created by Achraf EL AFRIT on 8/3/15.
//
//

#ifndef ____terminal_colorized_output__
#define ____terminal_colorized_output__

#include <ostream>
enum colorCodes {
    RED      = 31,
    GREEN    = 32,
    BROWN    = 33,
    BLUE     = 34,
    PURPLE   = 35,
    CYAN     = 36,
    
    DEFAULT  = 39,
    BG_RED      = 41,
    BG_GREEN    = 42,
    BG_BLUE     = 44,
    BG_PURPLE   = 45,
    BG_DEFAULT  = 49
};
class terminal_colorized_output {
    colorCodes colorCode;
public:
    terminal_colorized_output(colorCodes c) : colorCode(c) {}
    friend std::ostream& operator<<(std::ostream& os, const terminal_colorized_output& modifier) {
        return os << "\033[" << modifier.colorCode << "m";
    }
};

#endif /* defined(____terminal_colorized_output__) */
