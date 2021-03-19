#pragma once

class RevModelLoader
{
public:
	static class RevModel* LoadModel(const std::wstring& path);
	static class RevModelData CreateModelDataFromFile(const std::wstring& path);
};