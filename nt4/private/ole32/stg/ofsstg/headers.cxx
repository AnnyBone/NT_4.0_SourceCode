//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1993.
//
//  File:	headers.cxx
//
//  Contents:	Precompiled header file
//
//  History:	02-Nov-93	DrewB	Created
//
//----------------------------------------------------------------------------

extern "C"
{
#include <nt.h>
#include <ntrtl.h>
#include <nturtl.h>
}

#if WIN32 != 300
#include <compobj.h>
#include <storage.h>
#endif
#ifdef _CAIRO_
#define _CAIROSTG_
#include <oleext.h>
#endif
#include <stgint.h>
#include <oledb.h>

#include <valid.h>
#include <debnot.h>

#include <otrack.hxx>

#include <dfexcept.hxx>
#include <dfmsp.hxx>
#include <funcs.hxx>
#include <docfilep.hxx>
#include <ole.hxx>
#include <safedecl.hxx>

#include <infs.hxx>
#include <stgstm.hxx>
#include <ntsupp.hxx>
#include <ptrcache.hxx>
#include "odirstg.hxx"
#include "ofilstg.hxx"
#include "odocstg.hxx"
