#pragma once
#include "Types.h"


class JsonWriter
{
    public:

    void end();         // universal ends the nested object, array list or root
    void begin_root();  // begin the json root object (has no name part)

    void field_obj (const char* name);
    void field_arr (const char* name);
    void field_i   (const char* name, i32  val);
    void field_ui  (const char* name, ui32 val);
    void field_b   (const char* name, bool val);

    private:
    void writeC(const char ch);
    void writeS(const char* str);
    bool check_no_overflow() { return depth < MAXDEP && cursor < CAP; }

    private:
    static constexpr ui16 BUFFSZ = 2048;        // fixed internal buffer size
    static constexpr ui8  MAXDEP = 16;          // max objects nesting depth
    static constexpr ui16 CAP    = BUFFSZ - 1;  // capacity, leave 1 slot for \0

    private:
    char buffer[BUFFSZ];
    ui16 cursor;                    // pos in buffer
    ui8  depth;                     // curr obj depth
    ui8  typeField[MAXDEP];         // store 0 for objects and 1 for arrays. Resolves the closing token to be } or ]
    bool firstField[MAXDEP];        // used for ',' comma logic

    private:  // CTOR
    JsonWriter();                   // force private ctor so we have only one instance
};


extern JsonWriter json;             // the singletone instance