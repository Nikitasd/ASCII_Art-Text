/** @file Ascii_Convert.c
*   @brief Variable application
*   @date 23-01-2023
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

#include <Ascii_Convert.h>
#include "sci.h"
#include <string.h>
#include "stdint.h"


const char *const Art_Text[7] = {
"   ###    ########   ######   ########  ########  ########   ######   ##     ##    ####          ## ##    ##  ##        ##     ## ##    ##   #######  ########   #######  ########   ######   ########  ##     ## ##     ## ##      ## ##     ## ##    ## ######## ",
"  ## ##   ##     ## ##    ##  ##     ## ##        ##        ##    ##  ##     ##     ##           ## ##   ##   ##        ###   ### ###   ##  ##     ## ##     ## ##     ## ##     ## ##    ##     ##     ##     ## ##     ## ##  ##  ##  ##   ##   ##  ##       ##  ",
" ##   ##  ##     ## ##        ##     ## ##        ##        ##        ##     ##     ##           ## ##  ##    ##        #### #### ####  ##  ##     ## ##     ## ##     ## ##     ## ##           ##     ##     ## ##     ## ##  ##  ##   ## ##     ####       ##   ",
"##     ## ########  ##        ##     ## ######    ######    ##   #### #########     ##           ## #####     ##        ## ### ## ## ## ##  ##     ## ########  ##     ## ########   ######      ##     ##     ## ##     ## ##  ##  ##    ###       ##       ##    ",
"######### ##     ## ##        ##     ## ##        ##        ##    ##  ##     ##     ##     ##    ## ##  ##    ##        ##     ## ##  ####  ##     ## ##        ##  ## ## ##   ##         ##     ##     ##     ##  ##   ##  ##  ##  ##   ## ##      ##      ##     ",
"##     ## ##     ## ##    ##  ##     ## ##        ##        ##    ##  ##     ##     ##     ##    ## ##   ##   ##        ##     ## ##   ###  ##     ## ##        ##    ##  ##    ##  ##    ##     ##     ##     ##   ## ##   ##  ##  ##  ##   ##     ##     ##      ",
"##     ## ########   ######   ########  ########  ##         ######   ##     ##    ####     ######  ##    ##  ########  ##     ## ##    ##   #######  ##         ##### ## ##     ##  ######      ##      #######     ###     ###  ###  ##     ##    ##    ######## "
};

/** Storage structure input symbols */
static struct Box_of_input box_of_input = {
      .Convert_Enable = 0,
      .array_of_text = {0},
      .index = 0
};
/** Saving index use to Ascii_Converter_to_Text
 * @snippet <Ascii_Convert> Ascii_Converter_to_Text */
static uint16 index_for_output = 0;

/** @param[void] ASCII_Conv Output function of ASCII art-text*/
void Ascii_Converter_to_Text(){
    for(uint16 j = 0; j < 7; j++){
        for(uint16 i = 0; i < index_for_output - 1; i++){
            uint32 position = box_of_input.array_of_text[i] - 'A';
            for(uint16 k = 0; k < 10; k++){
                uint8 num_of_art = Art_Text[j][position*10 + k];
                sciSendByte(scilinREG, num_of_art);
            }
        }
        Ascii_Converter_Indent();
    }
    memset(box_of_input.array_of_text, 0, 16);
    box_of_input.Convert_Enable = 0;
}

/** @param[void] Indent function of indents */
void Ascii_Converter_Indent(){
    sciSendByte(scilinREG, '\r');
    sciSendByte(scilinREG, '\n');
}

/** @param[void] sciNotification function of SCI interrupt. Flag - SCI_RX_INT*/
void sciNotification(sciBASE_t *sci, uint32 flags)
{
    sciSendByte(sci, box_of_input.array_of_text[box_of_input.index]);
    box_of_input.index++;
    /**Inputting symbols more 16
     *@code
     *if(box_of_input.array_of_text[box_of_input.index - 1] == 13 && box_of_input.index >= 16){
        sciSend(sci, sizeof("Entered number of characters exceeds 16\r\n"), (uint8 *)"Entered number of characters exceeds 16\r\n");
        box_of_input.index = 0;
        memset(box_of_input.array_of_text, 0, 16);
    }
     *@endcode
     */
    if(box_of_input.array_of_text[box_of_input.index - 1] == 13 && box_of_input.index >= 16){
        sciSend(sci, sizeof("Entered number of characters exceeds 16\r\n"), (uint8 *)"Entered number of characters exceeds 16\r\n");
        box_of_input.index = 0;
        memset(box_of_input.array_of_text, 0, 16);
    }
    /**Input "Enter"
     * @code
     *if(box_of_input.array_of_text[box_of_input.index - 1] == 13 && box_of_input.index < 16){
        box_of_input.Convert_Enable = 1;
        index_for_output = box_of_input.index;
        box_of_input.index = 0;
    }
     *@endcode*/
    if(box_of_input.array_of_text[box_of_input.index - 1] == 13 && box_of_input.index < 16){
        box_of_input.Convert_Enable = 1;
        index_for_output = box_of_input.index;
        box_of_input.index = 0;
    }
    sciReceive(sci, 1, (uint8 *)&box_of_input.array_of_text[box_of_input.index]);
}

/** @param[void] Ascii_Converter_Enable_Convert() Allows to create ASCII text-art*/
void Ascii_Converter_Enable_Convert(){
    if(box_of_input.Convert_Enable){
        Ascii_Converter_Indent();
        sciDisableNotification(scilinREG, SCI_RX_INT);
        Ascii_Converter_to_Text();
        sciEnableNotification(scilinREG, SCI_RX_INT);
    }
}

void Ascii_Converter_First_Recieve(){
    box_of_input.index = 0;
    sciReceive(scilinREG, 1, (uint8 *)&box_of_input.array_of_text[box_of_input.index]);
}
