// ChildNodeIterator.cpp : Implementation of CChildNodeIterator

#include "stdafx.h"
#include "ChildNodeIterator.h"


// CChildNodeIterator

// ItcProjectNodeIterator Methods

HRESULT STDMETHODCALLTYPE CChildNodeIterator::Next(ItcProjectNode * * Value)
{
	_ASSERT( Value != NULL );
	*Value = NULL;
	return S_OK;
}


// IaqIterator Methods

void STDMETHODCALLTYPE CChildNodeIterator::Skip(long /*Count*/)
{
}


VARIANT_BOOL STDMETHODCALLTYPE CChildNodeIterator::HasNext()
{
	return VARIANT_FALSE;
}


void STDMETHODCALLTYPE CChildNodeIterator::Reset()
{
}
