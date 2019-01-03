#include "../base/nx_prototype.h"

//------------------------------------------------------------------------------
//
//	Copyright (C) 2009 Nexell Co., All Rights Reserved
//	Nexell Co. Proprietary & Confidential
//
//	NEXELL INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
//	AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
//	BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
//	FOR A PARTICULAR PURPOSE.
//
//	Module		:
//	File		: nx_DualDisplay.h
//	Description	:
//	Author		:
//	History		:
//------------------------------------------------------------------------------

//-----------------------------
// Display Module Header File ( Must Declarated Out Of Define Statement )
//-----------------------------
#include "nx_mlc.h"
#include "nx_dpc.h"
//-----------------------------



#ifndef __NX_DUALDISPLAY_H__
#define __NX_DUALDISPLAY_H__

//---------------
// DisplayTop이 있다면 Include한다.
#ifdef NUMBER_OF_DISPLAYTOP_MODULE
#include "nx_displaytop.h"
#endif
//---------------




#include "../base/nx_prototype.h"

#ifdef	__cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
/// @defgroup	DUALDISPLAY
//------------------------------------------------------------------------------
//--------------------------------------------------------------------------
/// @DISPLAY BLOCK CONFIGURATION
// Display Block 은 하나만 있다고 가정한다. 만약에 4 ch을 사용할것이라면 quad mlc display 를 생성하도록 한다.
#define NUMBER_OF_MLC_MODULE 2
#define NUMBER_OF_DPC_MODULE 2

#define PHY_BASEADDR_MLC0_MODULE		( PHY_BASEADDR_DUALDISPLAY_MODULE +    0 )
#define PHY_BASEADDR_MLC1_MODULE		( PHY_BASEADDR_DUALDISPLAY_MODULE + 1024 )

#define PHY_BASEADDR_DPC0_MODULE		( PHY_BASEADDR_DUALDISPLAY_MODULE + 2048 )
#define PHY_BASEADDR_DPC1_MODULE		( PHY_BASEADDR_DUALDISPLAY_MODULE + 3072 )








#define PADINDEX_OF_DPC0_o_nCS   PADINDEX_OF_DUALDISPLAY_o_nCS
#define PADINDEX_OF_DPC0_o_nRD   PADINDEX_OF_DUALDISPLAY_o_nRD
#define PADINDEX_OF_DPC0_o_RS    PADINDEX_OF_DUALDISPLAY_o_RS
#define PADINDEX_OF_DPC0_o_nWR   PADINDEX_OF_DUALDISPLAY_o_nWR

#define PADINDEX_OF_DPC0_PADPrimVCLK     PADINDEX_OF_DUALDISPLAY_PADPrimVCLK
#define PADINDEX_OF_DPC0_o_PrimPADnHSync PADINDEX_OF_DUALDISPLAY_o_PrimPADnHSync
#define PADINDEX_OF_DPC0_o_PrimPADnVSync PADINDEX_OF_DUALDISPLAY_o_PrimPADnVSync
#define PADINDEX_OF_DPC0_o_PrimPADDE     PADINDEX_OF_DUALDISPLAY_o_PrimPADDE

#define PADINDEX_OF_DPC0_PrimPADRGB24_0_  PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_0_
#define PADINDEX_OF_DPC0_PrimPADRGB24_1_  PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_1_
#define PADINDEX_OF_DPC0_PrimPADRGB24_2_  PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_2_
#define PADINDEX_OF_DPC0_PrimPADRGB24_3_  PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_3_
#define PADINDEX_OF_DPC0_PrimPADRGB24_4_  PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_4_
#define PADINDEX_OF_DPC0_PrimPADRGB24_5_  PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_5_
#define PADINDEX_OF_DPC0_PrimPADRGB24_6_  PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_6_
#define PADINDEX_OF_DPC0_PrimPADRGB24_7_  PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_7_
#define PADINDEX_OF_DPC0_PrimPADRGB24_8_  PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_8_
#define PADINDEX_OF_DPC0_PrimPADRGB24_9_  PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_9_
#define PADINDEX_OF_DPC0_PrimPADRGB24_10_ PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_10_
#define PADINDEX_OF_DPC0_PrimPADRGB24_11_ PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_11_
#define PADINDEX_OF_DPC0_PrimPADRGB24_12_ PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_12_
#define PADINDEX_OF_DPC0_PrimPADRGB24_13_ PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_13_
#define PADINDEX_OF_DPC0_PrimPADRGB24_14_ PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_14_
#define PADINDEX_OF_DPC0_PrimPADRGB24_15_ PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_15_
#define PADINDEX_OF_DPC0_PrimPADRGB24_16_ PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_16_
#define PADINDEX_OF_DPC0_PrimPADRGB24_17_ PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_17_
#define PADINDEX_OF_DPC0_PrimPADRGB24_18_ PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_18_
#define PADINDEX_OF_DPC0_PrimPADRGB24_19_ PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_19_
#define PADINDEX_OF_DPC0_PrimPADRGB24_20_ PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_20_
#define PADINDEX_OF_DPC0_PrimPADRGB24_21_ PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_21_
#define PADINDEX_OF_DPC0_PrimPADRGB24_22_ PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_22_
#define PADINDEX_OF_DPC0_PrimPADRGB24_23_ PADINDEX_OF_DUALDISPLAY_PrimPADRGB24_23_


#define PADINDEX_OF_DPC1_o_nCS          0
#define PADINDEX_OF_DPC1_o_nRD          0
#define PADINDEX_OF_DPC1_o_RS           0
#define PADINDEX_OF_DPC1_o_nWR          0

#define PADINDEX_OF_DPC1_PADPrimVCLK      PADINDEX_OF_DUALDISPLAY_PADSecondVCLK
#define PADINDEX_OF_DPC1_o_PrimPADnHSync  PADINDEX_OF_DUALDISPLAY_o_SecondPADnHSync
#define PADINDEX_OF_DPC1_o_PrimPADnVSync  PADINDEX_OF_DUALDISPLAY_o_SecondPADnVSync
#define PADINDEX_OF_DPC1_o_PrimPADDE      PADINDEX_OF_DUALDISPLAY_o_SecondPADDE

#define PADINDEX_OF_DPC1_PrimPADRGB24_0_  PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_0_
#define PADINDEX_OF_DPC1_PrimPADRGB24_1_  PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_1_
#define PADINDEX_OF_DPC1_PrimPADRGB24_2_  PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_2_
#define PADINDEX_OF_DPC1_PrimPADRGB24_3_  PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_3_
#define PADINDEX_OF_DPC1_PrimPADRGB24_4_  PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_4_
#define PADINDEX_OF_DPC1_PrimPADRGB24_5_  PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_5_
#define PADINDEX_OF_DPC1_PrimPADRGB24_6_  PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_6_
#define PADINDEX_OF_DPC1_PrimPADRGB24_7_  PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_7_
#define PADINDEX_OF_DPC1_PrimPADRGB24_8_  PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_8_
#define PADINDEX_OF_DPC1_PrimPADRGB24_9_  PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_9_
#define PADINDEX_OF_DPC1_PrimPADRGB24_10_ PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_10_
#define PADINDEX_OF_DPC1_PrimPADRGB24_11_ PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_11_
#define PADINDEX_OF_DPC1_PrimPADRGB24_12_ PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_12_
#define PADINDEX_OF_DPC1_PrimPADRGB24_13_ PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_13_
#define PADINDEX_OF_DPC1_PrimPADRGB24_14_ PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_14_
#define PADINDEX_OF_DPC1_PrimPADRGB24_15_ PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_15_
#define PADINDEX_OF_DPC1_PrimPADRGB24_16_ PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_16_
#define PADINDEX_OF_DPC1_PrimPADRGB24_17_ PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_17_
#define PADINDEX_OF_DPC1_PrimPADRGB24_18_ PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_18_
#define PADINDEX_OF_DPC1_PrimPADRGB24_19_ PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_19_
#define PADINDEX_OF_DPC1_PrimPADRGB24_20_ PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_20_
#define PADINDEX_OF_DPC1_PrimPADRGB24_21_ PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_21_
#define PADINDEX_OF_DPC1_PrimPADRGB24_22_ PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_22_
#define PADINDEX_OF_DPC1_PrimPADRGB24_23_ PADINDEX_OF_DUALDISPLAY_SecondPADRGB24_23_



//------------------------------------------------------------------------------
///	@name	reset Interface
//------------------------------------------------------------------------------
//@{
U32 NX_DUALDISPLAY_GetResetNumber ( U32 ModuleIndex );
//@}

//@}

#ifdef	__cplusplus
}
#endif


#endif // __NX_DUALDISPLAY_H__
