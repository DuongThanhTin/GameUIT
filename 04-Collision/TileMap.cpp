#include "Tilemap.h"
std::wstring s2ws(const std::string& s);
#define ID_TEX_TILESET_01 -10

CTileSet::CTileSet()
{
}

void CTileSet::LoadFromFile(LPCWSTR filePath)
{
	// Load info
	ifstream file(filePath);
	json j = json::parse(file);
	
	tileWidth = j["tilesets"][0]["tilewidth"].get<int>();
	tileHeight = j["tilesets"][0]["tileheight"].get<int>();
	columnNumber = j["tilesets"][0]["columns"].get<int>();
	int tileCount = j["tilesets"][0]["tilecount"].get<int>();
	
	string tmpPath = "textures\\" + j["/tilesets/0/image"_json_pointer].get<string>();
	rowNumber = (tileCount - 1) / columnNumber + 1;
	DebugOut(L"[INFO] rowNumber OK: %d\n", rowNumber);

	wstring sTmp;
	sTmp = s2ws(tmpPath);
	LPCWSTR imagePath = sTmp.c_str();

	//DebugOut(L"[INFO] imagePath OK: %d\n", imagePath);
	/*int tileWidth = 16;
	int tileHeight = 16;
	int columnNumber = 11;
	int tileCount = 165;
	int rowNumber = 15;
	LPCWSTR imagePath = L"textures\\mapFile_bank.png";*/

	// Add list tiles
	for (int i = 0; i < rowNumber; i++)
		for (int j = 0; j < columnNumber; j++)
		{
			RECT tmpRect = { j * tileWidth, i * tileHeight, (j + 1) * tileWidth, (i + 1) * tileHeight };
			int tmpID = i * columnNumber + j + 1;
			listTile.insert(pair<int, RECT>(tmpID, tmpRect));
			
			//Debug
			DebugOut(L"[INFO] tmpID OK: %d\n", tmpID);
		}

	// Add texture
	CTextures::GetInstance()->Add(ID_TEX_TILESET_01, imagePath, 0);
	this->texture = CTextures::GetInstance()->Get(ID_TEX_TILESET_01);
	if (texture)
		DebugOut(L"Load tile texure OK: %s\n", imagePath);

	file.close();
}

void CTileSet::DrawTile(int id, D3DXVECTOR2 position)
{
	RECT sourceRect = listTile[id];
	CGame::GetInstance()->Draw(position.x, position.y, texture, sourceRect.left, sourceRect.top, sourceRect.right, sourceRect.bottom);
}

int CTileSet::GetTileWidth()
{
	return tileWidth;
}

int CTileSet::GetTileHeight()
{
	return tileHeight;
}

// https://stackoverflow.com/questions/27220/how-to-convert-stdstring-to-lpcwstr-in-c-unicode
// covert std::string to std:: wstring
std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}


CTileMap::CTileMap()
{
	tileSet = new CTileSet();
}

void CTileMap::LoadFromFile(LPCWSTR filePath)
{
	// Load info
	ifstream file(filePath);
	json j = json::parse(file);

	//rowNumber = j["/layers/0/height"_json_pointer].get<int>();
	//columnNumber = j["/layers/0/width"_json_pointer].get<int>();
	//vector<int> data = j["/layers/0/data"_json_pointer].get<vector<int>>();
	rowNumber = j["layers"][0]["height"].get<int>();
	columnNumber = j["layers"][0]["width"].get<int>();
	vector<int> data = j["layers"][0]["data"].get<vector<int>>();

	//DebugOut
	DebugOut(L"rowNumber: %d\n ", rowNumber);
	DebugOut(L"columnNumber: %d\n ", columnNumber);

	// Map data from vector to matrix	
	mapData = new int*[rowNumber];
	for (int i = 0; i < rowNumber; i++)
	{
		mapData[i] = new int[columnNumber];
		
		for (int j = 0; j < columnNumber; j++)
		{
			int tmp = i * columnNumber + j;
			mapData[i][j] = data[tmp];
			//Debug
			//DebugOut(L"Mapdata :%d\n", mapData[i][j]);
		//	DebugOut(L"tmp :%d\n", tmp);
		}
	}

	// Load tileset
	tileSet->LoadFromFile(filePath);

	for (int i = 0; i < rowNumber; i++)
		for (int j = 0; j < columnNumber; j++)
			DebugOut(L"%d, ", mapData[i][j]);
}

void CTileMap::Draw(D3DXVECTOR2 position)
{
	CViewPort * viewport = CViewPort::GetInstance();

	for (int i = 0; i < 11; i++) // rowNumber
	{
		for (int j = 0; j < 60; j++) // ColumnNumber
		{
			D3DXVECTOR2 pos;
			pos.x = int (position.x + j * tileSet->GetTileWidth());
			pos.y = int (position.y + i * tileSet->GetTileHeight());
			pos = CViewPort::GetInstance()->ConvertWorldToViewPort(pos);
			tileSet->DrawTile(mapData[i][j], pos);
		}
	}
}