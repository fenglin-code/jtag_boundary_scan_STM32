/*
 * BSReg_Def.h
 *
 *  Created on: Aug 14, 2019
 *      Author: user
 */

#ifndef BSREG_DEF_H_
#define BSREG_DEF_H_

#include "BoundaryScan.h"

/*
 * Definition for Boundary Scan Register (GPIO pins) [STM32F103C86T]
 * BSReg = {inputCellNum, outputCellNum, controlCellNum}
 * */

// GPIO_A
BSReg pa12 = {55, 56, 57};
BSReg pa11 = {58, 59, 60};
BSReg pa10 = {61, 62, 63};
BSReg pa9  = {64, 65, 66};
BSReg pa8  = {67, 68, 69};
BSReg pa7  = {166, 167, 168};
BSReg pa6  = {169, 170, 171};
BSReg pa5  = {172, 173, 174};
BSReg pa4  = {175, 176, 177};
BSReg pa3  = {178, 179, 180};
BSReg pa2  = {181, 182, 183};
BSReg pa1  = {184, 185, 186};
BSReg pa0  = {187, 188, 189};

// GPIO_B
BSReg pb15 = {106, 107, 108};
BSReg pb14 = {109, 110, 111};
BSReg pb13 = {112, 113, 114};
BSReg pb12 = {115, 116, 117};
BSReg pb11 = {118, 119, 120};
BSReg pb10 = {121, 122, 123};
BSReg pb9 = {6, 7, 8};
BSReg pb8 = {9, 10, 11};
BSReg pb7 = {13, 14, 15};
BSReg pb6 = {16, 17, 18};
BSReg pb5 = {19, 20, 21};
BSReg pb2 = {151, 152, 153};
BSReg pb1 = {154, 155, 156};
BSReg pb0 = {157, 158, 159};


// GPIO_C
BSReg pc15 = {208, 209, 210};
BSReg pc14 = {211, 212, 213};
BSReg pc13 = {214, 215, 216};

// Oscillator
BSReg oscOutPd1 = {202, 203, 204};
BSReg oscOutPd0 = {205, 206, 207};

#endif /* BSREG_DEF_H_ */
