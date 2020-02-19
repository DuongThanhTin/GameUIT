#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include <fstream>
#include <string>
#include <vector>
#include "json.hpp"
#include "debug.h"
#include "Sprites.h"
#include "Textures.h"
#include "Game.h"

using json = nlohmann::json;


class CTileSet {
private:
	int	tileWidth;
	int tileHeight;
	int rowNumber;
	int columnNumber;
	map<int, RECT> listTile;
	LPDIRECT3DTEXTURE9 texture;
public:
	CTileSet();
	void LoadFromFile(LPCWSTR filePath);
	void DrawTile(int id, D3DXVECTOR2 position);
	int GetTileWidth();
	int GetTileHeight();
	~CTileSet();
};

typedef CTileSet* LPTILESHEET;

class CTileMap
{
private:
	int rowNumber;
	int columnNumber;
	LPTILESHEET tileSet;
	int** mapData;
public:
	CTileMap();
	void LoadFromFile(LPCWSTR filePath);
	void Draw(D3DXVECTOR2 position);
	~CTileMap();
};


