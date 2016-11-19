#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_


namespace HxSTL {

    class exception {
    public:
        virtual ~exception() {}
        virtual const char* what() const { return ""; }
    };

}


#endif
