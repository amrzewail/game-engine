#pragma once

#include <glad/glad.h>

#include <string>
#include <vector>

#include <Moths/API.h>
#include <Moths/Core/Vector.h>
#include <Moths/Core/Vector2.h>
#include <Moths/Core/Color.h>
#include <Moths/Core/Matrix4x4.h>

using namespace std;

namespace MothsEngine
{
	class MOTHS_API Shader
	{
	private:
		vector<string>* _defines = new vector<string>();

	public:

		static const string FLAG_TRANSPARENT;
		static const string FLAG_Blend_SrcAlpha_OneMinusSrcAlpha;
		static const string FLAG_DEPTH_TEST_OFF;
		static const string FLAG_CULL_FACE_OFF;

		// the program ID
		const unsigned int id;

		static Shader* Create(const char* vertShader, const char* fragShader);

		//// constructor reads and builds the shader
		explicit Shader(const unsigned int _id) : id(_id) {}

		~Shader();

		//// use/activate the shader
		void Use() const;

		//// utility uniform functions
		void SetBool(const std::string& name, bool value) const;
		void SetInt(const std::string& name, int value) const;
		void SetFloat(const std::string& name, float value) const;
		void SetVector(const std::string& name, const Vector& v) const;
		void SetVector2(const std::string& name, const Vector2& v) const;
		void SetColor(const std::string& name, const Color& c) const;
		void SetMatrix4x4(const std::string& name, const Matrix4x4& m) const;
		void SetMatrix4x4Array(const std::string& name, const Matrix4x4 array[], int length) const;

		void SetDefine(string define)
		{
			_defines->push_back(define);
		}

		bool IsDefined(string define) const
		{
			for (auto d : *_defines)
			{
				if (d == define) return true;
			}
			return false;
		}
	};
}