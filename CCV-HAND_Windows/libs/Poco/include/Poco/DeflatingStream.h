//
// DeflatingStream.h
//
// $Id: //poco/1.3/Foundation/include/Poco/DeflatingStream.h#1 $
//
// Library: Foundation
// Package: Streams
// Module:  ZLibStream
//
// Definition of the DeflatingStream class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Foundation_DeflatingStream_INCLUDED
#define Foundation_DeflatingStream_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/BufferedStreamBuf.h"
#include <istream>
#include <ostream>
#include "Poco/zlib.h"


namespace Poco {


class Foundation_API DeflatingStreamBuf: public BufferedStreamBuf
	/// This is the streambuf class used by DeflatingInputStream and DeflatingOutputStream.
	/// The actual work is delegated to zlib 1.2.1 (see http://www.gzip.org).
	/// Both zlib (deflate) streams and gzip streams are supported.
	/// Output streams should always call close() to ensure
	/// proper completion of compression.
	/// A compression level (0 to 9) can be specified in the constructor.
{
public:
	enum StreamType
	{
		STREAM_ZLIB,
		STREAM_GZIP
	};

	DeflatingStreamBuf(std::istream& istr, StreamType type, int level);
	DeflatingStreamBuf(std::ostream& ostr, StreamType type, int level);
	~DeflatingStreamBuf();
	int close();

protected:
	int readFromDevice(char* buffer, std::streamsize length);
	int writeToDevice(const char* buffer, std::streamsize length);

private:
	enum 
	{
		STREAM_BUFFER_SIZE  = 1024,
		DEFLATE_BUFFER_SIZE = 32768
	};

	std::istream* _pIstr;
	std::ostream* _pOstr;
	char*    _buffer;
	z_stream _zstr;
	bool     _eof;
};


class Foundation_API DeflatingIOS: public virtual std::ios
	/// The base class for DeflatingOutputStream and DeflatingInputStream.
	///
	/// This class is needed to ensure the correct initialization
	/// order of the stream buffer and base classes.
{
public:
	DeflatingIOS(std::ostream& ostr, DeflatingStreamBuf::StreamType type = DeflatingStreamBuf::STREAM_ZLIB, int level = Z_DEFAULT_COMPRESSION);
	DeflatingIOS(std::istream& istr, DeflatingStreamBuf::StreamType type = DeflatingStreamBuf::STREAM_ZLIB, int level = Z_DEFAULT_COMPRESSION);
	~DeflatingIOS();
	DeflatingStreamBuf* rdbuf();

protected:
	DeflatingStreamBuf _buf;
};


class Foundation_API DeflatingOutputStream: public DeflatingIOS, public std::ostream
	/// This stream compresses all data passing through it
	/// using zlib's deflate algorithm.
	/// After all data has been written to the stream, close()
	/// must be called to ensure completion of compression.
	/// Example:
	///     std::ofstream ostr("data.gz", std::ios::binary);
	///     DeflatingOutputStream deflater(ostr, DeflatingStreamBuf::STREAM_GZIP);
	///     deflater << "Hello, world!" << std::endl;
	///     deflater.close();
	///     ostr.close();
{
public:
	DeflatingOutputStream(std::ostream& ostr, DeflatingStreamBuf::StreamType type = DeflatingStreamBuf::STREAM_ZLIB, int level = Z_DEFAULT_COMPRESSION);
	~DeflatingOutputStream();
	int close();
};


class Foundation_API DeflatingInputStream: public DeflatingIOS, public std::istream
	/// This stream compresses all data passing through it
	/// using zlib's deflate algorithm.
{
public:
	DeflatingInputStream(std::istream& istr, DeflatingStreamBuf::StreamType type = DeflatingStreamBuf::STREAM_ZLIB, int level = Z_DEFAULT_COMPRESSION);
	~DeflatingInputStream();
};


} // namespace Poco


#endif // Foundation_DeflatingStream_INCLUDED
