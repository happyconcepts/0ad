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

#ifndef INCLUDED_TEXTUREMANAGER
#define INCLUDED_TEXTUREMANAGER

#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

#include "lib/res/handle.h"
#include "ps/CStr.h"
#include "ps/Singleton.h"

// access to sole CTextureManager  object
#define g_TexMan CTextureManager ::GetSingleton()

class XMBElement;
class CXeromyces;
class CTextureEntry;
class CTerrainProperties;

typedef shared_ptr<CTerrainProperties> CTerrainPropertiesPtr;

class CTerrainGroup
{
	// name of this terrain group (as specified by the terrain XML)
	CStr m_Name;
	// "index".. basically a bogus integer that can be used by ScEd to set texture
	// priorities
	size_t m_Index;
	// list of textures of this type (found from the texture directory)
	std::vector<CTextureEntry*> m_Terrains;

public:
	CTerrainGroup(CStr name, size_t index):
		m_Name(name),
		m_Index(index)
	{}
	
	// Add a texture entry to this terrain type
	void AddTerrain(CTextureEntry *);
	// Remove a texture entry
	void RemoveTerrain(CTextureEntry *);

	size_t GetIndex() const
	{ return m_Index; }
	CStr GetName() const
	{ return m_Name; }

	const std::vector<CTextureEntry*> &GetTerrains() const
	{ return m_Terrains; }
};

///////////////////////////////////////////////////////////////////////////////////////////
// CTextureManager : manager class for all terrain texture objects
class CTextureManager : public Singleton<CTextureManager>
{
public:
	typedef std::map<CStr, CTerrainGroup *> TerrainGroupMap;

private:
	// All texture entries created by this class, for easy freeing now that
	// textures may be in several STextureType's
	std::vector<CTextureEntry *> m_TextureEntries;

	TerrainGroupMap m_TerrainGroups;

	size_t m_LastGroupIndex;

	// Find+load all textures in directory; check if
	// there's an override XML with the same basename (if there is, load it)
	void LoadTextures(const CTerrainPropertiesPtr& props, const VfsPath& path);
	
	// Load all terrains below path, using props as the parent property sheet.
	void RecurseDirectory(const CTerrainPropertiesPtr& props, const VfsPath& path);
	
	CTerrainPropertiesPtr GetPropertiesFromFile(const CTerrainPropertiesPtr& props, const VfsPath& pathname);

public:
	// constructor, destructor
	CTextureManager();
	~CTextureManager();

	// Find all XML's in the directory (with subdirs) and try to load them as
	// terrain XML's
	int LoadTerrainTextures();

	void UnloadTerrainTextures();
	
	CTextureEntry* FindTexture(const CStr& tag);
	CTextureEntry* FindTexture(Handle handle);
	
	// Create a texture object for a new terrain texture at path, using the
	// property sheet props.
	CTextureEntry *AddTexture(const CTerrainPropertiesPtr& props, const VfsPath& path);
	
	// Remove the texture from all our maps and lists and delete it afterwards.
	void DeleteTexture(CTextureEntry* entry);
	
	// Find or create a new texture group. All terrain groups are owned by the
	// texturemanager (TerrainTypeManager)
	CTerrainGroup *FindGroup(const CStr& name);

	const TerrainGroupMap &GetGroups() const
	{ return m_TerrainGroups; }
};


#endif
