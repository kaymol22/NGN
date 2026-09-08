#include "GL_Shader.h"
#include <glad/gl.h>

struct ShaderParseContext {
	std::unordered_set<std::string> includedPaths;
	bool rootVersionSeen = false;
};

static void ParseFile(const std::string& filePath, std::string& outputString, std::vector<std::string>& lineToFile,
	ShaderParseContext& context, const std::string& rootFilePath);
static std::string LTrimCopy(const std::string& s);
static bool StartsWith(const std::string& s, const char* prefix);
static bool TryParseInclude(const std::string& line, std::string outIncludeFile);
void StripUTF8FromLine(std::string& line);
void InsertDefines(std::string& source, const std::vector<std::string>& defines);
int GetErrorLineNumber(const std::string& error);
std::string GetErrorMessage(const std::string& line);
std::string GetLinkingErrors(uint32_t programId);
std::string GetShaderCompileErrors(uint32_t shader, const std::string& filename, const std::vector<std::string>& lineToFile);

namespace OpenGL
{
	OpenGLShader::OpenGLShader(std::vector<std::string> shaderPaths, const std::string subDirectory, const std::vector<std::string>& defines)
	{
		m_Defines = defines;
		m_ShaderPaths = shaderPaths;
		m_SubDirectory = subDirectory;

		Load(m_ShaderPaths);
	}

	bool OpenGLShader::Load(std::vector<std::string> shaderPaths)
	{
		std::vector<OpenGLShaderModule> modules;
		for (std::string& shaderPath : shaderPaths)
		{
			std::string fullPath = m_SubDirectory.empty() ? shaderPath : m_SubDirectory + "/" + shaderPath;
			modules.push_back(OpenGLShaderModule(fullPath, m_Defines));
		}

		bool errorsFound = false;
		for (OpenGLShaderModule& module : modules)
		{
			if (module.CompilationFailed()) {
				errorsFound = true;
				break;
			}
		}
		if (errorsFound)
		{
			std::cout << "\n-------------------------------------------------------------------------\n\n";
			// Print comp errors
			for (OpenGLShaderModule& module : modules)
			{
				if (module.CompilationFailed()) {
					std::cout << " COMPILATION ERROR: " << module.GetFileName() << "\n\n";
					std::cout << module.GetErrors() << "\n";
				}
				glDeleteShader(module.GetHandle());
			}
			std::cout << "-------------------------------------------------------------------------\n";
			return false;
		}

		uint32_t tempHandle = glCreateProgram();
		for (OpenGLShaderModule& module : modules) {
			glAttachShader(tempHandle, module.GetHandle());
		}
		glLinkProgram(tempHandle);
		std::string linkingErrors = GetLinkingErrors(tempHandle);

		// Print link errors
		if (linkingErrors.length())
		{
			std::cout << "\n-------------------------------------------------------------------------\n\n";
			std::cout << " LINKING ERROR: ";
			for (int i = 0; i < modules.size(); i++) {
				std::cout << modules[i].GetFileName();
				if (i != modules.size() - 1) {
					std::cout << "/";
				}
			}
			std::cout << linkingErrors << "\n";
			std::cout << "-------------------------------------------------------------------------\n";
			for (OpenGLShaderModule& module : modules) {
				glDeleteShader(module.GetHandle());
			}
			return false;
		}
		// Store handle to compiled shader
		else {
			if (m_Handle) {
				glDeleteProgram(m_Handle);
			}
			m_Handle = tempHandle;
			m_UniformLocations.clear();
		}
		for (OpenGLShaderModule& module : modules)
		{
			glDeleteShader(module.GetHandle());
		}
		return true;
	}

	bool OpenGLShader::HotLoad()
	{
		return Load(m_ShaderPaths);
	}

	int32_t OpenGLShader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocations.find(name) == m_UniformLocations.end()) {
			m_UniformLocations[name] = glGetUniformLocation(m_Handle, name.c_str());
		}
		return m_UniformLocations[name];
	}

	size_t OpenGLShader::GetCPUAllocatedByteCount() const
	{
		size_t byteCount = m_SubDirectory.capacity();

		byteCount += m_Defines.capacity() * sizeof(std::string);
		for (const std::string& define : m_Defines) {
			byteCount += define.capacity();
		}

		byteCount += m_ShaderPaths.capacity() * sizeof(std::string);
		for (const std::string& shaderPath : m_ShaderPaths) {
			byteCount += shaderPath.capacity();
		}

		byteCount += m_UniformLocations.size() * (sizeof(std::string) + sizeof(int32_t));
		for (const auto& uniformLocation : m_UniformLocations) {
			byteCount += uniformLocation.first.capacity();
		}

		return byteCount;
	}

	OpenGLShaderModule::OpenGLShaderModule(const std::string& fileName, const std::vector<std::string>& defines)
	{
		ShaderParseContext context;
		std::vector<std::string> lineMap;
		std::string parsedShaderSource = "";

		ParseFile("res/shaders/OpenGL/" + fileName, parsedShaderSource, lineMap, context, "res/shaders/OpenGL/" + fileName);
		InsertDefines(parsedShaderSource, defines);

		// Get type based on ext
		std::string extension = std::filesystem::path(fileName).extension().string();
		static const std::unordered_map<std::string, uint32_t> shaderTypeMap = {
			{".vert", GL_VERTEX_SHADER},
			{".frag", GL_FRAGMENT_SHADER},
			{".geom", GL_GEOMETRY_SHADER},
			{".tesc", GL_TESS_CONTROL_SHADER},
			{".tese", GL_TESS_EVALUATION_SHADER},
			{".comp", GL_COMPUTE_SHADER},
			{".task", GL_TASK_SHADER_EXT},
			{".mesh", GL_MESH_SHADER_EXT}
		};
		uint32_t shaderType = shaderTypeMap.contains(extension) ? shaderTypeMap.at(extension) : GL_NONE;

		// Check for errors
		const char* shaderCode = parsedShaderSource.c_str();
		m_Handle = glCreateShader(shaderType);
		glShaderSource(m_Handle, 1, &shaderCode, NULL);
		glCompileShader(m_Handle);
		m_Errors = GetShaderCompileErrors(m_Handle, fileName, lineMap);
		m_FileName = fileName;

		m_LineMap = lineMap;
		// Store for debug output
		m_FinalSource = parsedShaderSource;
	}

	uint32_t OpenGLShaderModule::GetHandle()
	{
		return m_Handle;
	}

	bool OpenGLShaderModule::CompilationFailed()
	{
		return m_Errors.length();
	}

	std::string& OpenGLShaderModule::GetFileName()
	{
		return m_FileName;
	}

	std::string& OpenGLShaderModule::GetErrors()
	{
		return m_Errors;
	}

	std::vector<std::string>& OpenGLShaderModule::GetLineMap()
	{
		return m_LineMap;
	}
}

static void ParseFile(const std::string& filePath, std::string& outputString, std::vector<std::string>& lineToFile,
	ShaderParseContext& context, const std::string& rootFilePath)
{
	std::string baseDir = std::filesystem::path(filePath).parent_path().string();
	std::string fileName = std::filesystem::path(filePath).filename().string();
	std::ifstream file(filePath);
	std::string line;
	bool firstLineOfFile = true;
	int fileLineNumber = 1;

	if (!file.is_open()) {
		std::cout << "\n------------------------------------------------------------------------ - \n\n";
		std::cout << " SHADER PARSE ERROR: failed to open file: " << filePath << "\n";
		std::cout << "-------------------------------------------------------------------------\n";
		return;
	}

	while (std::getline(file, line)) {
		// strip BOM
		if (firstLineOfFile) {
			StripUTF8FromLine(line);
			firstLineOfFile = false;
		}

		std::string includeFile;
		if (TryParseInclude(line, includeFile)) {
			std::string includePath = std::filesystem::weakly_canonical(baseDir + "/" + includeFile).string();

			// Check if already in included paths
			if (context.includedPaths.insert(includePath).second) {
				ParseFile(includePath, outputString, lineToFile, context, rootFilePath);
			}

			fileLineNumber++;
			continue;
		}

		// Protect output from accidental '#version' includes
		std::string trimmed = LTrimCopy(line);
		if (StartsWith(trimmed, "#version")) {
			if (filePath != rootFilePath) {
				std::cout << "\n-------------------------------------------------------------------------\n\n";
				std::cout << " SHADER PARSE WARNING: #version found in an included file, skipping it: " << filePath << " (line " << fileLineNumber << ")\n";
				std::cout << "-------------------------------------------------------------------------\n";
				fileLineNumber++;
				continue;
			}
			context.rootVersionSeen = true;
		}

		outputString += line + "\n";
		lineToFile.emplace_back(fileName + " (line " + std::to_string(fileLineNumber) + ")");

		fileLineNumber++;
	}
}

static std::string LTrimCopy(const std::string& s) {
	size_t i = 0;
	while (i < s.size() && (s[i] == ' ' || s[i] == '\t' || s[i] == '\r')) {
		i++;
	}
	return s.substr(i);
}

static bool StartsWith(const std::string& s, const char* prefix) {
	const size_t n = std::char_traits<char>::length(prefix);
	if (s.size() < n) return false;
	return s.compare(0, n, prefix) == 0;
}

static bool TryParseInclude(const std::string& line, std::string outIncludeFile)
{
	std::string trimmed = LTrimCopy(line);
	if (!StartsWith(trimmed, "#include")) {
		return false;
	}
	size_t firstQuote = trimmed.find('"');
	if (firstQuote == std::string::npos) return false;
	size_t secondQuote = trimmed.find('"', firstQuote + 1);
	if (secondQuote == std::string::npos) return false;
	outIncludeFile = trimmed.substr(firstQuote + 1, secondQuote - firstQuote - 1);
	return !outIncludeFile.empty();
}

void StripUTF8FromLine(std::string& line)
{
	if (line.size() >= 3) {
		const unsigned char b0 = (unsigned char)line[0];
		const unsigned char b1 = (unsigned char)line[1];
		const unsigned char b2 = (unsigned char)line[2];
		if (b0 == 0xEF && b1 == 0xBB && b2 == 0xBF) {
			line.erase(0, 3);
		}
	}
}

void InsertDefines(std::string& source, const std::vector<std::string>& defines)
{
	if (defines.empty()) return;

	std::string definesBlock = "";
	for (const std::string& define : defines) {
		definesBlock += "#define " + define + "\n";
	}

	size_t versionPos = source.find("#version");
	if (versionPos != std::string::npos) {
		// Find end of version line
		size_t newLinePos = source.find('\n', versionPos);
		// Insert defines
		if (newLinePos != std::string::npos) {
			source.insert(newLinePos + 1, definesBlock);
		}
	}
}

int GetErrorLineNumber(const std::string& error)
{
	size_t firstColon = error.find(':');
	if (firstColon != std::string::npos) {
		size_t secondColon = error.find(':', firstColon + 1);
		if (secondColon != std::string::npos) {
			size_t thirdColon = error.find(':', secondColon + 1);
			if (thirdColon != std::string::npos) {
				std::string lineNumberStr = error.substr(secondColon + 1, thirdColon - secondColon - 1);
				return std::stoi(lineNumberStr);
			}
		}
	}
	return -1;
}

std::string GetErrorMessage(const std::string& line)
{
	size_t firstColon = line.find(':');
	if (firstColon != std::string::npos) {
		size_t secondColon = line.find(':', firstColon + 1);
		if (secondColon != std::string::npos) {
			size_t thirdColon = line.find(':', secondColon + 1);
			if (thirdColon != std::string::npos) {
				size_t messageStart = thirdColon + 2; // Skip the colon and space
				if (messageStart < line.length()) {
					return line.substr(messageStart);
				}
			}
		}
	}
	return ""; // Return empty string if parsing fails
}

std::string GetLinkingErrors(uint32_t programId)
{
	GLint linkStatus;
	glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);

	if (linkStatus == GL_FALSE) {
		GLint logLength;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0) {
			std::vector<char> infoLogBuffer(logLength + 1); // +1 for null terminator
			glGetProgramInfoLog(programId, logLength, NULL, &infoLogBuffer[0]);

			std::string fullLog(infoLogBuffer.data());
			std::stringstream logStream(fullLog);
			std::string line;
			std::string resultToShow = "\n";

			const std::string assemblyStartDelimiter = "-- internal assembly text --";
			bool assemblySectionEncountered = false;

			while (std::getline(logStream, line)) {
				if (assemblySectionEncountered) {
					break;
				}

				resultToShow += "    " + line + "\n";

				// Now, check if THIS line was the delimiter
				if (line.find(assemblyStartDelimiter) != std::string::npos) {
					resultToShow += "    (Following internal assembly text omitted for brevity)\n";
					assemblySectionEncountered = true;
					break;
				}
			}
			return resultToShow;
		}
		else {
			return "\n    An unknown linking error occurred (no info log available).\n";
		}
	}
	return "";
}

std::string GetShaderCompileErrors(uint32_t shader, const std::string& /*filename*/, const std::vector<std::string>& lineToFile)
{
	int success;
	char infoLog[1024];
	std::string result = "";
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
		// Parse error log to extract line numbers
		std::stringstream logStream(infoLog);
		std::string line;
		while (std::getline(logStream, line)) {
			if ((line.substr(0, 7) == "ERROR: ")) {
				int lineNumber = GetErrorLineNumber(line);
				if (lineNumber >= 0 && lineNumber < (int)lineToFile.size()) {
					result += "  " + lineToFile[lineNumber] + ": " + GetErrorMessage(line) + "\n";
				}
			}
		}
	}
	return result;
}