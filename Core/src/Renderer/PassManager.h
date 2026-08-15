#pragma once
#include "ngnpch.h"
#include "RenderPass.h"

namespace NGN
{
	// Own renderpass instances & their fb targets
	// Registers passes & executes in specfied order
	class PassManager
	{
	public:
		/*template <typename TPass, typename... Args>
			requires std::derived_from<TPass, RenderPass>
		TPass& AddPass(std::string_view name, Args&&... args)
		{
			auto pass = std::make_unique<TPass>(std::forward<Args>(args)...);
			TPass& ref = *pass;
			m_Passes[std::string(name)] = { std::move(pass), nullptr };
			return ref;
		}*/

		void AddPass(std::string_view name, Scope<RenderPass> pass)
		{
			m_Passes[std::string(name)] = { std::move(pass), nullptr };
		}

		template <typename TPass>
			requires std::derived_from<TPass, RenderPass>
		TPass* GetPass(std::string_view name)
		{
			auto pass = m_Passes.find(std::string(name));
			if (pass == m_Passes.end()) return nullptr;
			return static_cast<TPass*>(pass->second.pass.get());
		}

		void SetTarget(std::string_view passName, Ref<Framebuffer> target);
		void SetOrder(std::vector<std::string> order);

		// TODO: Add more robust pass sorting + 'dirty' bool

		void ExecuteAll();
		void ClearAll();
	private:
		struct PassEntry
		{
			Scope<RenderPass> pass;
			Ref<Framebuffer> target;
		};

		std::unordered_map<std::string, PassEntry> m_Passes;
		std::vector<std::string> m_ExecutionOrder;
	};
}