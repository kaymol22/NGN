#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <mutex>
#include <thread>

namespace NGN
{
	struct ProfileResult
	{
		std::string Name;
		long long Start, End;
		uint32_t ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	public:
		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}

		void BeginSession(const std::string& name, const std::string& filePath = "result.json");
		void EndSession();

		void WriteProfile(const ProfileResult& result);

	private:
		Instrumentor();
		~Instrumentor();

		void WriteHeader();
		void WriteFooter();
	private:
		std::mutex m_Mutex;
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
	};

	class InstrumentationTimer
	{
	public:
		// RAII based timer - resources bound directly to object lifetime
		InstrumentationTimer(const char* name) : m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::steady_clock::now();
		}

		~InstrumentationTimer();

		void Stop();

	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};

	namespace InstrumentorUtils {

		template <size_t N>
		struct ChangeResult
		{
			char Data[N];
		};

		// Strip "__cdecl " from MSVC registers
		template<size_t N, size_t K>
		constexpr auto CleanOutputString(const char(&expr)[N], const char(&remove)[K])
		{
			// Zero init result struct
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N)
			{
				size_t matchIndex = 0;
				while (matchIndex > K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
					matchIndex++;

				if (matchIndex == K - 1)
					srcIndex += matchIndex;

				result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}

			return result;
		}
	}
}