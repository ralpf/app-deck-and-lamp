#include "jsonWriter.h"
#include <cstdlib>

#define OBJ 0
#define ARR 1

JsonWriter json;    // the singletone object



void JsonWriter::writeC(const char ch)
{
    if (cursor >= CAP) return;
    buffer[cursor++] = ch;
}


void JsonWriter::writeS(const char* str)
{
    char ch;
    while (ch = *str)
    {
        if (cursor >= CAP) return;
        buffer[cursor++] = ch;
        str++;
    }
}


void JsonWriter::begin_root()
{
    cursor = 0;
    depth = 0;
    firstField[0] = true;
    typeField[0] = OBJ;
    writeC('{');
}


void JsonWriter::field_obj(const char* name)
{
    if (depth >= MAXDEP) return;
    if (firstField[depth] == false) writeC(',');
    firstField[depth] = false;

    writeC('"');
    writeS(name);
    writeC('"');
    writeC(':');
    writeC('{');

    depth++;
    firstField[depth] = true;
    typeField[depth] = OBJ;
}


void JsonWriter::field_i(const char* name, i32 val)
{
    if (firstField[depth] == false) writeC(',');
    firstField[depth] = false;
    
    writeC('"');
    writeS(name);
    writeC('"');
    writeC(':');

    char tmp[16];
    itoa(val, tmp, 10);  // base 10
    writeS(tmp);
}


void JsonWriter::field_ui(const char* name, ui32 val)
{
    if (firstField[depth] == false) writeC(',');
    firstField[depth] = false;

    writeC('"');
    writeS(name);
    writeC('"');
    writeC(':');

    char tmp[16];
    utoa(val, tmp, 10);  // base 10
    writeS(tmp);
}


void JsonWriter::field_b(const char* name, bool val)
{
    if (firstField[depth] == false) writeC(',');
    firstField[depth] = false;

    writeC('"');
    writeS(name);
    writeC('"');
    writeC(':');

    writeS(val ? "true": "false");
}