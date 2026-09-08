#pragma once

namespace RS
{
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