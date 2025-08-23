#pragma once

#include <string>
#include "Shader.h"

class ShaderCompiler
{

private:
	static std::string Trim(const std::string& str, const std::string& character = "\t")
	{
		const auto strBegin = str.find_first_not_of(character);
		if (strBegin == std::string::npos)
			return ""; // no content

		const auto strEnd = str.find_last_not_of(character);
		const auto strRange = strEnd - strBegin + 1;

		return str.substr(strBegin, strRange);
	}

	static std::string Include(std::string includeLine)
	{
		std::string includeFile = "";
		bool begin = false;
		for (int i = 0; i < includeLine.size(); i++)
		{
			if (begin && includeLine[i] == '\"') break;
			if (begin) includeFile += includeLine[i];
			if (!begin && includeLine[i] == '\"') begin = true;
		}
		TextAsset& lib = Assets::Load<TextAsset>(includeFile.data());
		std::string text = lib.GetText();
		std::string output = "";
		std::istringstream iss = std::istringstream(text);
		for (std::string line; std::getline(iss, line); )
		{
			line = Trim(line, " "); line = Trim(line, "\t"); line = Trim(line, "\0"); line = Trim(line, "\r");

			if (line.empty() || line == "\n" || line == " " || line == "\0") continue;

			if (Token(line, "#include")) line = Include(line);
			if (Token(line, "#version")) continue;

			output += line + '\n';
		}

		return output;
	}

	static std::string Define(std::string defineLine)
	{
		return Trim(Trim(defineLine, "#define"), " ");
	}

	static bool Token(std::string line, std::string keyword)
	{
		if (line.size() >= keyword.size() && line.substr(0, keyword.size()) == keyword)
		{
			return true;
		}
		return false;
	}


public:
	static Shader* Compile(const std::string shaderText)
	{
		std::string vertShader = "";
		std::string fragShader = "";

		bool beginVert = false;
		bool beginFrag = false;

		int openBraceCount = 0;

		std::istringstream iss = std::istringstream(shaderText);

		std::vector<std::string> defines = std::vector<std::string>();

		for (std::string line; std::getline(iss, line); )
		{
			line = Trim(line, " "); line = Trim(line, "\t"); line = Trim(line, "\0"); line = Trim(line, "\r");

			if (line.empty() || line == "\n" || line == " " || line == "\0") continue;

			//std::cout << line.size() << ">>" << line << std::endl;

			if (Token(line, "#define"))
			{
				defines.push_back(Define(line));
				continue;
			}

			if (Token(line, "vert"))
			{
				beginVert = true;
				continue;
			}

			if (Token(line, "frag"))
			{
				beginFrag = true;
				continue;
			}


			if (Token(line, "{")) openBraceCount++;

			if (Token(line, "}"))
			{
				openBraceCount--;

				if ((beginVert || beginFrag) && openBraceCount == 0)
				{
					beginVert = false;
					beginFrag = false;
				}
			}

			if (Token(line, "#include"))
			{
				line = Include(line);
			}

			if (Token(line, "{") && openBraceCount == 1) continue;

			if (beginVert)
			{
				vertShader += line + '\n';
			}

			if (beginFrag)
			{
				fragShader += line + '\n';
			}

		}

		int lineIndex = 0;

		iss = std::istringstream(vertShader);
		lineIndex = 0;
		//for (std::string line; std::getline(iss, line); ) std::cout << lineIndex++ << ">>" << line << std::endl;

		iss = std::istringstream(fragShader);
		lineIndex = 0;
		//for (std::string line; std::getline(iss, line); ) std::cout << lineIndex++ << ">>" << line << std::endl;

		Shader* shader = Shader::Create(vertShader.c_str(), fragShader.c_str());
		for (auto d : defines)
		{
			shader->SetDefine(d);
		}
		return shader;
	}

};