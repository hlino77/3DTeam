#pragma once
#include "Engine_Defines.h"

class ENGINE_DLL CAsUtils
{
public:
	static bool StartsWith(string str, string comp);
	static bool StartsWith(wstring str, wstring comp);

	static void Replace(OUT string& str, string comp, string rep);
	static void Replace(OUT wstring& str, wstring comp, wstring rep);

	static wstring ToWString(string value);
	static string ToString(wstring value);

	static wstring S2W(const string& strValue);
	static string W2S(const wstring& szValue);

	static _float Lerpf(_float fStart, _float fEnd, _float fTime);
};

