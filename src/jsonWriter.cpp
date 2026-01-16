#include "jsonWriter.h"
#include "assertCrash.h"
#include <Arduino.h>


#define OBJ 0
#define ARR 1


char singleBuffer[JsonWriter::BUFFSZ];


//..............................................................

JsonWriter::JsonWriter()
{
    buffer = singleBuffer;
}


//..............................................................

void JsonWriter::writeC(const char ch)
{
    ASSERT(cursor < CAP, "buffer is full");
    buffer[cursor++] = ch;
}


void JsonWriter::writeS(const char* str)
{
    char ch;
    while (ch = *str)
    {
        ASSERT(cursor < CAP - 1, "buffer is full");
        if (ch == '"' || ch == '\\') buffer[cursor++] = '\\';
        buffer[cursor++] = ch;
        str++;
    }
}

//..............................................................

void JsonWriter::root()
{
    cursor = 0;
    depth = 0;
    firstField[0] = true;
    typeField[0] = OBJ;
    writeC('{');
}


void JsonWriter::end()
{
    ASSERT(depth >= 0, "no root object to close");
    auto type = typeField[depth];
    ASSERT(type == OBJ || type == ARR, "unexpected type");

    writeC(type == OBJ ? '}' : ']');

    if (--depth < 0) buffer[cursor] = 0;
}


const char* JsonWriter::get_cstring() const
{
    ASSERT(cursor > 0, "buffer is empty, no data");
    ASSERT(depth < 0, "json object not closed (ended)");
    ASSERT(cursor < CAP, "json object did not fit in buffer");
    ASSERT(buffer[cursor] == 0, "json object c-string not null terminated");
    return buffer;
}


ui16 JsonWriter::get_size() const
{
    ASSERT(cursor > 0, "buffer is empty, no data");
    ASSERT(cursor < CAP, "json object did not fit in buffer");
    return cursor;
}

//..............................................................

void JsonWriter::field_obj(const char* name)
{
    ASSERT(depth >= 0, "start root object first");
    ASSERT(depth < MAXDEP, "depth overflow");

    if (firstField[depth] == false) writeC(',');
    firstField[depth] = false;

    writeC('"');
    writeS(name);
    writeC('"');
    writeC(':');
    writeC('{');

    depth++;
    firstField[depth] = true;
    typeField [depth] = OBJ;
}


void JsonWriter::field_arr(const char* name)
{
    ASSERT(depth >= 0, "start root object first");
    ASSERT(depth < MAXDEP, "depth overflow");

    if (firstField[depth] == false) writeC(',');
    firstField[depth] = false;

    writeC('"');
    writeS(name);
    writeC('"');
    writeC(':');
    writeC('[');

    depth++;
    firstField[depth] = true;
    typeField [depth] = ARR;
}


void JsonWriter::field_i(const char* name, i32 val)
{
    ASSERT(depth >= 0, "start root object first");
    ASSERT(depth < MAXDEP, "depth overflow");

    if (firstField[depth] == false) writeC(',');
    firstField[depth] = false;

    if (typeField[depth] == OBJ)
    {
        writeC('"');
        writeS(name);
        writeC('"');
        writeC(':');
    }

    char tmp[16];
    itoa(val, tmp, 10);  // base 10
    writeS(tmp);
}


void JsonWriter::field_ui(const char* name, ui32 val)
{
    ASSERT(depth >= 0, "start root object first");
    ASSERT(depth < MAXDEP, "depth overflow");
    
    if (firstField[depth] == false) writeC(',');
    firstField[depth] = false;
    
    if (typeField[depth] == OBJ)
    {
        writeC('"');
        writeS(name);
        writeC('"');
        writeC(':');
    }
    
    char tmp[16];
    utoa(val, tmp, 10);  // base 10
    writeS(tmp);
}


void JsonWriter::field_f(const char* name, float val)
{
    ASSERT(depth >= 0, "start root object first");
    ASSERT(depth < MAXDEP, "depth overflow");
    
    if (firstField[depth] == false) writeC(',');
    firstField[depth] = false;
    
    if (typeField[depth] == OBJ)
    {
        writeC('"');
        writeS(name);
        writeC('"');
        writeC(':');
    }
    
    char tmp[32];
    dtostrf(val, 0, 4, tmp);
    writeS(tmp);
}


void JsonWriter::field_b(const char* name, bool val)
{
    ASSERT(depth >= 0, "start root object first");
    ASSERT(depth < MAXDEP, "depth overflow");

    if (firstField[depth] == false) writeC(',');
    firstField[depth] = false;

    if (typeField[depth] == OBJ)
    {
        writeC('"');
        writeS(name);
        writeC('"');
        writeC(':');
    }

    writeS(val ? "true": "false");
}


void JsonWriter::field_s(const char* name, const char* val)
{
    ASSERT(depth >= 0, "start root object first");
    ASSERT(depth < MAXDEP, "depth overflow");

    if (firstField[depth] == false) writeC(',');
    firstField[depth] = false;

    if (typeField[depth] == OBJ)
    {
        writeC('"');
        writeS(name);
        writeC('"');
        writeC(':');
    }

    writeC('"');
    writeS(val);
    writeC('"');
}