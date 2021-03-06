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
 *  Handling BioWare's GDAs (2DAs, two-dimensional array, within V4.0 GFFs).
 */

#ifndef AURORA_GDAFILE_H
#define AURORA_GDAFILE_H

#include <vector>
#include <map>

#include "src/common/ustring.h"

#include "src/aurora/types.h"

namespace Common {
	class UString;
	class SeekableReadStream;
}

namespace Aurora {

/** Class to hold the GFF'd two-dimensional array of a GDA file. */
class GDAFile {
public:
	static const size_t kInvalidColumn = SIZE_MAX;
	static const size_t kInvalidRow    = SIZE_MAX;

	enum Type {
		kTypeEmpty    = -1,
		kTypeString   =  0,
		kTypeInt      =  1,
		kTypeFloat    =  2,
		kTypeBool     =  3,
		kTypeResource =  4
	};

	struct Header {
		uint32 hash;
		Type type;

		uint32 field;

		Header() : hash(0), type(kTypeEmpty), field(0xFFFFFFFF) { }
	};
	typedef std::vector<Header> Headers;


	GDAFile(Common::SeekableReadStream *gda);
	~GDAFile();

	/** Add another GDA with the same column structure to the bottom of this GDA.
	 *
	 *  This effectively pastes the GDAs together, creating one combined table.
	 *  Note that the row numbers will be continous and therefore will be
	 *  different depending on the order of the pasting, making them useless
	 *  for row identification. An ID column should be used for this case.
	 */
	void add(Common::SeekableReadStream *gda);

	/** Return the number of columns in the array. */
	size_t getColumnCount() const;
	/** Return the number of rows in the array. */
	size_t getRowCount() const;

	/** Does this row exist in the GDA? */
	bool hasRow(size_t row) const;

	/** Get the column headers. */
	const Headers &getHeaders() const;

	/** Get a row as a GFF4 struct. */
	const GFF4Struct *getRow(size_t row) const;

	/** Find a row by its ID value. */
	size_t findRow(uint32 id) const;

	/** Find a column by its name. */
	size_t findColumn(const Common::UString &name) const;
	/** Find a column by its hash. */
	size_t findColumn(uint32 hash) const;

	Common::UString getString(size_t row, uint32 columnHash, const Common::UString &def = "") const;
	Common::UString getString(size_t row, const Common::UString &columnName,
	                          const Common::UString &def = "") const;

	int32 getInt(size_t row, uint32 columnHash, int32 def = 0) const;
	int32 getInt(size_t row, const Common::UString &columnName, int32 def = 0) const;

	float getFloat(size_t row, uint32 columnHash, float def = 0.0f) const;
	float getFloat(size_t row, const Common::UString &columnName, float def = 0.0f) const;


private:
	typedef std::vector<GFF4File *> GFF4s;
	typedef const GFF4List * Columns;
	typedef const GFF4List * Row;
	typedef std::vector<Row> Rows;
	typedef std::vector<size_t> RowStarts;

	typedef std::map<uint32, size_t> ColumnHashMap;
	typedef std::map<Common::UString, size_t> ColumnNameMap;


	GFF4s _gff4s;

	Headers _headers;

	Columns _columns;
	Rows    _rows;

	size_t _rowCount;

	RowStarts _rowStarts;

	mutable ColumnHashMap _columnHashMap;
	mutable ColumnNameMap _columnNameMap;


	void load(Common::SeekableReadStream *gda);
	void clear();

	uint32 identifyType(const Columns &columns, const Row &rows, size_t column) const;

	const GFF4Struct *getRowColumn(size_t row, uint32 hash, size_t &column) const;
	const GFF4Struct *getRowColumn(size_t row, const Common::UString &name, size_t &column) const;
};

} // End of namespace Aurora

#endif // AURORA_GDAFILE_H
