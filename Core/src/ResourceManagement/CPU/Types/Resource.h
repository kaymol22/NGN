#pragma once

namespace RS
{
	/* Resource Types 
		CUBEMAP_FRAME_BUFFER,
		CUBEMAP_VIEW,
		FRAME_BUFFER,
		GENERIC_MESH,
		MESH_BUFFER,
		SHADER,
		SHADOW_CUBE_MAP_ARRAY,
		SHADOW_MAP,
		SHADOW_MAP_ARRAY,
		SSBO,
		TEXTURE,
		TEXTURE_3D,
		TEXTURE_ARRAY,
	VK-Specific:
		ACCELERATION_STRUCTURE
	*/
	class Resource
	{
	public:
		explicit Resource(const std::string& id) : resourceId(id) {}
		virtual ~Resource() = default;

		const std::string& GetId() const { return resourceId; }
		bool IsLoaded() const { return loaded; }

		bool Load() {
			loaded = doLoad();
			return loaded;
		}

		void Unload() {
			doUnload();
			loaded = false;
		}

	protected:
		virtual bool doLoad() = 0;
		virtual bool doUnload() = 0;

	private:
		std::string resourceId;
		bool loaded = false;
	};
}