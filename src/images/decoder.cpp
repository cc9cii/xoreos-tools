/* xoreos-tools - Tools to help with xoreos development
 *
 * xoreos-tools is the legal property of its developers, whose names
 * can be found in the AUTHORS file distributed with this source
 * distribution.
 *
 * xoreos-tools is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * xoreos-tools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with xoreos-tools. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file
 *  Generic image decoder interface.
 */

#include <cassert>

#include "src/common/util.h"
#include "src/common/error.h"
#include "src/common/memreadstream.h"

#include "src/images/decoder.h"
#include "src/images/util.h"
#include "src/images/s3tc.h"
#include "src/images/dumptga.h"

namespace Images {

Decoder::MipMap::MipMap() : width(0), height(0), size(0), data(0) {
}

Decoder::MipMap::~MipMap() {
	delete[] data;
}

void Decoder::MipMap::swap(MipMap &right) {
	SWAP(width , right.width );
	SWAP(height, right.height);
	SWAP(size  , right.size  );
	SWAP(data  , right.data  );
}


Decoder::Decoder() : _format(kPixelFormatR8G8B8A8) {
}

Decoder::~Decoder() {
	for (std::vector<MipMap *>::iterator m = _mipMaps.begin(); m != _mipMaps.end(); ++m)
		delete *m;
}

Common::SeekableReadStream *Decoder::getTXI() const {
	return 0;
}

bool Decoder::isCompressed() const {
	return (_format == kPixelFormatDXT1) ||
	       (_format == kPixelFormatDXT3) ||
	       (_format == kPixelFormatDXT5);
}

PixelFormat Decoder::getFormat() const {
	return _format;
}

size_t Decoder::getMipMapCount() const {
	return _mipMaps.size();
}

const Decoder::MipMap &Decoder::getMipMap(size_t mipMap) const {
	assert(mipMap < _mipMaps.size());

	return *_mipMaps[mipMap];
}

void Decoder::decompress(MipMap &out, const MipMap &in, PixelFormat format) {
	if ((format != kPixelFormatDXT1) &&
	    (format != kPixelFormatDXT3) &&
	    (format != kPixelFormatDXT5))
		throw Common::Exception("Unknown compressed format %d", format);

	out.width  = in.width;
	out.height = in.height;
	out.size   = MAX(out.width * out.height * 4, 64);
	out.data   = new byte[out.size];

	Common::MemoryReadStream *stream = new Common::MemoryReadStream(in.data, in.size);

	if      (format == kPixelFormatDXT1)
		decompressDXT1(out.data, *stream, out.width, out.height, out.width * 4);
	else if (format == kPixelFormatDXT3)
		decompressDXT3(out.data, *stream, out.width, out.height, out.width * 4);
	else if (format == kPixelFormatDXT5)
		decompressDXT5(out.data, *stream, out.width, out.height, out.width * 4);

	delete stream;
}

void Decoder::decompress() {
	if (!isCompressed())
		return;

	for (std::vector<MipMap *>::iterator m = _mipMaps.begin(); m != _mipMaps.end(); ++m) {
		MipMap decompressed;

		decompress(decompressed, **m, _format);

		decompressed.swap(**m);
	}

	_format = kPixelFormatR8G8B8A8;
}

bool Decoder::dumpTGA(const Common::UString &fileName) const {
	if (_mipMaps.size() < 1)
		return false;

	if (!isCompressed()) {
		Images::dumpTGA(fileName, *this);
		return true;
	}

	MipMap mipMap;
	decompress(mipMap, *_mipMaps[0], _format);
	Images::dumpTGA(fileName, mipMap.data, mipMap.width, mipMap.height, kPixelFormatR8G8B8A8);

	return true;
}

void Decoder::flipHorizontally() {
	decompress();

	for (std::vector<MipMap *>::iterator m = _mipMaps.begin(); m != _mipMaps.end(); ++m)
		::Images::flipHorizontally((*m)->data, (*m)->width, (*m)->height, getBPP(_format));
}

void Decoder::flipVertically() {
	decompress();

	for (std::vector<MipMap *>::iterator m = _mipMaps.begin(); m != _mipMaps.end(); ++m)
		::Images::flipVertically((*m)->data, (*m)->width, (*m)->height, getBPP(_format));
}

} // End of namespace Images
