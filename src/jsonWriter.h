#pragma once
#include "Types.h"


class JsonWriter
{
    public:

    void end();         // universal end, finishes the nested object, array list or root
    void begin_root();  // begin the json root object (has no name part)
    
    void field_obj (const char* name);              // starts a json object
    void field_arr (const char* name);              // starts a json array
    void field_i   (const char* name, i32  val);    // append a signed int
    void field_ui  (const char* name, ui32 val);    // append an unsigned int
    void field_b   (const char* name, bool val);    // append a boolean (maybe make _byte and _char as it takes same byte)

    const char* get_cstring() const;    // asserts all good, returns string's pointer
    ui16 get_size() const;              // current json char count, excluding \0

    private:
    void writeC(const char ch);                     // helper func, append character
    void writeS(const char* str);                   // helper func, append string

    private:
    static constexpr ui16 BUFFSZ = 2048;        // fixed internal buffer size
    static constexpr ui8  MAXDEP = 16;          // max objects nesting depth
    static constexpr ui16 CAP    = BUFFSZ - 1;  // capacity, leave 1 slot for \0

    private:
    char buffer[BUFFSZ];
    ui16 cursor;                    // pos in buffer
    i8   depth = -1;                // curr obj depth
    ui8  typeField[MAXDEP];         // store 0 for objects and 1 for arrays. Resolves the closing token to be } or ]
    bool firstField[MAXDEP];        // used for ',' comma logic

    private:  // CTOR
    JsonWriter();                   // force private ctor so we have only one instance
};


extern JsonWriter json;             // the singletone instance