// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0). 
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "SaveSystem/ToroSaveManager.h"

UToroSaveGame* UToroSaveManager::GetOrCreateSaveObject(const TSubclassOf<UToroSaveGame> Class)
{
	if (!Class)
	{
		return nullptr;
	}

	const TObjectPtr<UToroSaveGame>* Element = SaveObjects.Find(Class);
	if (Element && *Element)
	{
		return *Element;
	}

	UToroSaveGame* NewSaveObj = NewObject<UToroSaveGame>(this, Class);
	SaveObjects.Add(Class, NewSaveObj);
	return NewSaveObj;
}
