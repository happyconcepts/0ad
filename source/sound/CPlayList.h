/* Copyright (C) 2009 Wildfire Games.
 * This file is part of 0 A.D.
 *
 * 0 A.D. is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * 0 A.D. is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 0 A.D.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDED_CPLAYLIST
#define INCLUDED_CPLAYLIST

#include <vector>
#include "lib/file/vfs/vfs_path.h"

class CPlayList
{
public:
	CPlayList();
	CPlayList(const VfsPath& pathname);
	~CPlayList();
	void Load(const VfsPath& pathname);
	void List();
	void Add(std::wstring name);

private:
	std::vector<CStrW> tracks;
};

#endif
