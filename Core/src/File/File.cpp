#include "File.h"
#include "FileInfo.h"
#include "Core/String.h"
#include "Core/Log.h"

#include <algorithm>
#include <filesystem>
#include <chrono>

namespace NGN::File
{
	bool Delete(const std::string& path)
	{
		try {
			if (std::filesystem::remove(path)) {
				return true;
			}
			NGN_CORE_WARN("File::Delete: File not found or could not be deleted: {0}", path);
			return false;
		}
		catch (const std::filesystem::filesystem_error& e) {
			NGN_CORE_ERROR("File::Delete: Filesystem error: {0}", e.what());
			return false;
		}
	}

	uint64_t GetLastModifiedTime(const std::string& path)
	{
		try {
			auto ftime = std::filesystem::last_write_time(path);
			auto systemTime = std::chrono::clock_cast<std::chrono::system_clock>(ftime);
			return std::chrono::duration_cast<std::chrono::seconds>(systemTime.time_since_epoch()).count();
		}
		catch (const std::filesystem::filesystem_error& e) {
			NGN_CORE_ERROR("File::GetLastModifiedTime: Filesystem error: {0}", e.what());
			return 0;
		}
	}

	bool GetSize(const std::string& path, size_t& outSize)
	{
		std::ifstream file(path, std::ios::binary | std::ios::ate);
		if (!file) {
			NGN_CORE_ERROR("File::GetSize() failed to open file: {0}", path);
			return false;
		}
		outSize = static_cast<size_t>(file.tellg());
		return true;
	}

	bool Exists(const std::string_view path)
	{
		return std::filesystem::exists(std::filesystem::path(std::string(path)));
	}

	bool Exists(const std::string& directory, const std::string& extension, const std::string& name)
	{
		const std::string lowerName = NGN::String::ToLower(name);
		for (const FileInfo& fileInfo : NGN::File::IterateDirectory(directory, { extension }))
		{
			if (NGN::String::ToLower(fileInfo.name) == lowerName) {
				return true;
			}
		}
		return false;
	}

	bool Rename(const std::string& oldPath, const std::string& newPath)
	{
		if (!Exists(oldPath)) {
			NGN_CORE_ERROR("File::Rename() failed: old path does not exist: {0}", oldPath);
			return false;
		}

		try {
			std::filesystem::rename(oldPath, newPath);
			return true;
		}
		catch (const std::filesystem::filesystem_error& e) {
			NGN_CORE_ERROR("File::Rename() failed: {0}", e.what());
			return false;
		}
	}

	std::string GetName(const std::string& path) 
	{
		return std::filesystem::path(path).stem().string();
	}

	std::string GetExtension(const std::string& path)
	{
		std::string extension = std::filesystem::path(path).extension().string();
		return extension.starts_with('.') ? extension.substr(1) : extension;
	}

	std::string RemoveExtension(const std::string& path)
	{
		std::filesystem::path fileSystemPath(path);
		fileSystemPath.replace_extension();
		return fileSystemPath.string();
	}

	FileInfo GetFileInfo(const std::string& path)
	{
		if (!Exists(path)) {
			NGN_CORE_ERROR("File::GetFileInfo() failed: path does not exist: {0}", path);
			return {};
		}
		const std::filesystem::path fileSystemPath(path);
		return {
			fileSystemPath.string(),
			fileSystemPath.stem().string(),
			GetExtension(path),
			fileSystemPath.parent_path().string()
		};
	}

	std::vector<FileInfo> IterateDirectory(const std::string& directory, std::vector<std::string> extensions)
	{
		std::vector<FileInfo> fileInfoList;
		if (!std::filesystem::exists(directory)) return fileInfoList;

		for (const auto& entry : std::filesystem::directory_iterator(directory))
		{
			if (!std::filesystem::is_regular_file(entry)) continue;

			FileInfo fileInfo = {
				entry.path().string(),
				entry.path().stem().string(),
				GetExtension(entry.path().string()),
				directory
			};

			if (extensions.empty() || std::find(extensions.begin(), extensions.end(), fileInfo.ext) != extensions.end())
			{
				fileInfoList.push_back(fileInfo);
			}
		}
		return fileInfoList;
	}
}