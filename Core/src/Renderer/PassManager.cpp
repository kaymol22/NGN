#include "PassManager.h"

namespace NGN
{
	void PassManager::SetTarget(std::string_view passName, Ref<Framebuffer> target)
	{
		auto pass = m_Passes.find(std::string(passName));
		NGN_CORE_ASSERT(pass != m_Passes.end(), "PassManafer::SetTarget - couldn't find specified pass");
		pass->second.target = std::move(target);
	}

	void PassManager::SetOrder(std::vector<std::string> order)
	{
		m_ExecutionOrder = std::move(order);
	}

	void PassManager::ExecuteAll()
	{
		for (const std::string& name : m_ExecutionOrder)
		{
			auto passName = m_Passes.find(name);
			if (passName == m_Passes.end()) continue;

			PassEntry& entry = passName->second;
			if (entry.pass)
				// Call Execute() for specified pass on target FB
				entry.pass->Execute(entry.target);
		}
	}

	void PassManager::ClearAll()
	{
		for (auto& [name, entry] : m_Passes)
		{
			if (entry.pass)
				entry.pass->Clear();
		}
	}
}