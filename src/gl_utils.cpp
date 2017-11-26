#include "gl_utils.h"

enum _infoLogType {
  LOG_SHADER,
  LOG_PROGRAM
};

std::string _readShaderFileToEnd(std::string filePath) {
	std::string fileContents = "";
	std::ifstream vertFileStream(filePath, std::ios::in);
	if (vertFileStream.is_open()) {
		std::string currentLine = "";
		while (std::getline(vertFileStream, currentLine)) {
			fileContents += '\n' + currentLine;
		}
		vertFileStream.close();
	} else {
		std::ostringstream errorStringStream;
		errorStringStream << "Failed to open shader file " << filePath;
		throw std::runtime_error(errorStringStream.str());
	}
	return fileContents;
}

std::string _getInfoLog(GLuint targetId, _infoLogType logType) {
	GLint logMaxLength = 0;
	(logType == LOG_SHADER) ?
		glGetShaderiv(targetId, GL_INFO_LOG_LENGTH, &logMaxLength) :
		glGetProgramiv(targetId, GL_INFO_LOG_LENGTH, &logMaxLength);
	std::string infoLog;
	infoLog.reserve(logMaxLength);
	(logType == LOG_SHADER) ?
		glGetShaderInfoLog(targetId, logMaxLength, &logMaxLength, &infoLog[0]) :
		glGetProgramInfoLog(targetId, logMaxLength, &logMaxLength, &infoLog[0]);
	return infoLog;
}

void _compileShader(std::string shaderSrc, GLuint targetShaderId) {
	char const* srcPtr = shaderSrc.c_str();
	glShaderSource(targetShaderId, 1, &srcPtr, NULL);
	glCompileShader(targetShaderId);

	GLint compilationResult = 0;
	glGetShaderiv(targetShaderId, GL_COMPILE_STATUS, &compilationResult);
	if (compilationResult == GL_FALSE) {
		auto infoLog = _getInfoLog(targetShaderId, LOG_SHADER);
		std::ostringstream errorStringStream;
		errorStringStream << "[OpenGL] Shader compilation failed. Error: " << infoLog.data();
		throw std::runtime_error(errorStringStream.str());
	}
}

GLuint GLUtils::CreateProgram(std::string vertex_shader_file_path, std::string fragment_shader_file_path)
{
	std::string vertShaderSrc, fragShaderSrc;
	GLuint vertShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	vertShaderSrc = _readShaderFileToEnd(vertex_shader_file_path);
	fragShaderSrc = _readShaderFileToEnd(fragment_shader_file_path);

	_compileShader(vertShaderSrc, vertShaderId);
	_compileShader(fragShaderSrc, fragShaderId);

	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertShaderId);
	glAttachShader(programId, fragShaderId);
	glLinkProgram(programId);
	
	GLint linkResult = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, &linkResult);
	if (linkResult == GL_FALSE) {
		auto infoLog = _getInfoLog(programId, LOG_PROGRAM);
		std::ostringstream errorStringStream;
		errorStringStream << "[OpenGL] Program linking failed. Error: " << infoLog.data();
		throw std::runtime_error(errorStringStream.str());
	}

	glDetachShader(programId, vertShaderId);
	glDetachShader(programId, fragShaderId);
	glDeleteShader(vertShaderId);
	glDeleteShader(fragShaderId);

	return programId;
}
