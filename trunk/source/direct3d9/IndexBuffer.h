/*
* Copyright (c) 2007-2008 SlimDX Group
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/
#pragma once

#include <d3dx9.h>

#include "Enums.h"
#include "Resource.h"
#include "../DataStream.h"

//using namespace SlimDX::Direct3D;

namespace SlimDX
{
	namespace Direct3D9
	{
		public ref class IndexBuffer : public Resource
		{
			Format m_Format;
			SlimDX::Direct3D9::ResourceType m_Type;
			Usage m_Usage;
			Pool m_Pool;
			int m_SizeInBytes;
			
			void InitDescription();
			
		internal:
			property IDirect3DIndexBuffer9* IbPointer
			{
				IDirect3DIndexBuffer9* get() { return static_cast<IDirect3DIndexBuffer9*>( InternalPointer ); }
			}

			IndexBuffer( IDirect3DIndexBuffer9* buffer );

		public:
			IndexBuffer( System::IntPtr buffer );
			IndexBuffer( Device^ device, int sizeBytes, Usage usage, Pool pool, bool sixteenBit );
			virtual ~IndexBuffer() { }

			DataStream^ Lock( int offset, int size, LockFlags flags );
			Result Unlock();
			
			property SlimDX::Direct3D9::Format Format
			{
				SlimDX::Direct3D9::Format get() { return m_Format; }
			private:
				void set( SlimDX::Direct3D9::Format value ) { m_Format = value; }
			}

			property SlimDX::Direct3D9::Usage Usage
			{
				SlimDX::Direct3D9::Usage get() { return m_Usage; }
			private:
				void set( SlimDX::Direct3D9::Usage value ) { m_Usage = value; }
			}

			property SlimDX::Direct3D9::Pool Pool
			{
				SlimDX::Direct3D9::Pool get() { return m_Pool; }
			private:
				void set( SlimDX::Direct3D9::Pool value ) { m_Pool = value; }
			}

			property int SizeInBytes
			{
				int get() { return m_SizeInBytes; }
			private:
				void set( int value ) { m_SizeInBytes = value; }
			}
		};
	}
}
