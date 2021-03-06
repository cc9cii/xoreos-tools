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

// Partially based on the TGA implementation found in ScummVM.

/** @file
 *  Decoding TGA (TarGa) images.
 */

#ifndef IMAGES_TGA_H
#define IMAGES_TGA_H

#include "src/images/decoder.h"

namespace Common {
	class SeekableReadStream;
}

namespace Images {

/** TarGa image. */
class TGA : public Decoder {
public:
	TGA(Common::SeekableReadStream &tga);
	~TGA();

private:
	// Format-spec from http://www.ludorg.net/amnesia/TGA_File_Format_Spec.html
	enum ImageType {
		kImageTypeCMap         =  1,
		kImageTypeTrueColor    =  2,
		kImageTypeBW           =  3,
		kImageTypeRLECMap      =  9,
		kImageTypeRLETrueColor = 10,
		kImageTypeRLEBW        = 11
	};

	// Loading helpers
	void load(Common::SeekableReadStream &tga);
	void readHeader(Common::SeekableReadStream &tga, ImageType &imageType, byte &pixelDepth, byte &imageDesc);
	void readData(Common::SeekableReadStream &tga, ImageType imageType, byte pixelDepth, byte imageDesc);
	void readRLE(Common::SeekableReadStream &tga, byte pixelDepth);

	bool isSupportedImageType(ImageType type) const;
};

} // End of namespace Images

#endif // IMAGES_TGA_H
