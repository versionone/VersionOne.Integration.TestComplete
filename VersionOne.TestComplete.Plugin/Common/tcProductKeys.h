// =============================================================================================
// AutomatedQA SDK - Copyright (c) AutomatedQA Corp.
//
// This file is part of AutomatedQA SDK
//
// =============================================================================================
// 
// NOTE: Keep this file synchronized with tcProductKeys.pas
//

#pragma once

// =====================================================
// NOTE: Do not change these product keys !!!
// =====================================================

extern "C" const GUID __declspec(selectany) cProductKey_TC_Enterprise     =
    {0xD0D30094, 0xD31C, 0x48f8, {0xB4, 0x28, 0x72, 0xD9, 0x5A, 0x73, 0x8C, 0xF3}};
extern "C" const GUID __declspec(selectany) cProductKey_TC_Demo           =
    {0xD0D30094, 0x321C, 0x41d3, {0xBE, 0xEC, 0x73, 0xC0, 0xFE, 0xC5, 0xCD, 0x12}};
extern "C" const GUID __declspec(selectany) cProductKey_TC_Standard       =
    {0xE0D30094, 0x47D6, 0x477f, {0xA6, 0xD7, 0x0C, 0xA9, 0x7F, 0xB9, 0x1A, 0x43}};

extern "C" const GUID __declspec(selectany) cProductKey_TE                =
    {0xC0D30094, 0x2F7F, 0x4669, {0x9A, 0x33, 0xFB, 0x08, 0x1D, 0x60, 0x64, 0x5B}};
extern "C" const GUID __declspec(selectany) cProductKey_TE_Demo           =
    {0xC0D30094, 0xF86B, 0x427d, {0x86, 0x42, 0xE4, 0xDD, 0xDE, 0x3D, 0x55, 0x1B}};

extern "C" const GUID __declspec(selectany) cProductKey_TR_x86            =
    {0xA0D30094, 0xEE9A, 0x45a5, {0xA5, 0x81, 0xBA, 0x8E, 0x54, 0x4D, 0xE1, 0xDC}};
extern "C" const GUID __declspec(selectany) cProductKey_TR_x86_Demo       =
    {0xA0D30094, 0x4E82, 0x49e4, {0xA7, 0x96, 0xF3, 0x0D, 0x7F, 0x8E, 0xD8, 0x12}};
extern "C" const GUID __declspec(selectany) cProductKey_TR_x86_Lite       =
    {0xA0D30094, 0x9b6d, 0x46df, {0xb9, 0xa2, 0x3a, 0x49, 0x38, 0x55, 0x9e, 0x3a}};
extern "C" const GUID __declspec(selectany) cProductKey_TR_x64            =
    {0xA0D30094, 0xfa0e, 0x4009, {0x97, 0xea, 0x03, 0x6d, 0xc9, 0x26, 0xe1, 0x92}};
extern "C" const GUID __declspec(selectany) cProductKey_TR_x64_Demo       =
    {0xA0D30094, 0x536c, 0x4ed0, {0x85, 0x6b, 0xa5, 0x0a, 0x64, 0x9a, 0x3d, 0xdb}};
extern "C" const GUID __declspec(selectany) cProductKey_TR_x64_Lite       =
    {0xA0D30094, 0x0a97, 0x4992, {0x87, 0x2f, 0xb4, 0x61, 0xc5, 0xfa, 0x08, 0x35}};



extern "C" const GUID __declspec(selectany) * cAllProduct[] =
{
	&cProductKey_TC_Enterprise     ,
	&cProductKey_TC_Demo           ,
	&cProductKey_TC_Standard       ,
	&cProductKey_TE                ,
	&cProductKey_TE_Demo           ,
	&cProductKey_TR_x86            ,
	&cProductKey_TR_x86_Demo       ,
	&cProductKey_TR_x86_Lite       ,
	&cProductKey_TR_x64            ,
	&cProductKey_TR_x64_Demo       ,
	&cProductKey_TR_x64_Lite       
};

extern "C" const GUID __declspec(selectany) * cTCProduct[] =
{
	&cProductKey_TC_Enterprise     ,
	&cProductKey_TC_Demo           ,
	&cProductKey_TC_Standard       
};

extern "C" const GUID __declspec(selectany) * cTEProduct[] =
{
	&cProductKey_TE                ,
	&cProductKey_TE_Demo           
};

extern "C" const GUID __declspec(selectany) * cTRProduct[] =
{
	&cProductKey_TR_x86            ,
	&cProductKey_TR_x86_Demo       ,
	&cProductKey_TR_x86_Lite       ,
	&cProductKey_TR_x64            ,
	&cProductKey_TR_x64_Demo       ,
	&cProductKey_TR_x64_Lite       
};

extern "C" const GUID __declspec(selectany) * cEnterpriseProductOnly[] =
{
	&cProductKey_TC_Enterprise     ,
	&cProductKey_TC_Demo           ,
	&cProductKey_TE                ,
	&cProductKey_TE_Demo           ,
	&cProductKey_TR_x86            ,
	&cProductKey_TR_x86_Demo       ,
	&cProductKey_TR_x86_Lite       ,
	&cProductKey_TR_x64            ,
	&cProductKey_TR_x64_Demo       ,
	&cProductKey_TR_x64_Lite       
};

extern "C" const GUID __declspec(selectany) * cStandardProductOnly[] =
{
	&cProductKey_TC_Standard       
};
