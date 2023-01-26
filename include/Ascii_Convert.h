/** @file Ascii_Convert.H
*   @brief Variable application
*   @date 23-01-2023
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

#pragma once

#include "sci.h"
#include "stdint.h"

void Ascii_Converter_to_Text();

/// Input character storage structure
struct Box_of_input{
    uint8 array_of_text[16]; // An array which stores entered characters
    uint16 index; // Counter of array array_of_text[]
    bool Convert_Enable; //Enable converting an array array_of_text[] to ASCII text-art
};

void Ascii_Converter_First_Recieve();

void Ascii_Converter_Enable_Convert();

void Ascii_Converter_Indent();

