#include "Instrumentor.h"

namespace NGN
{
	Instrumentor::Instrumentor() : m_CurrentSession(nullptr)
	{
	}

	Instrumentor::~Instrumentor()
	{
	}

	void Instrumentor::BeginSession(const std::string& name, const std::string& filePath)
	{
		std::lock_guard lock(m_Mutex);

		m_ProfileCount = 0;

		if (m_CurrentSession)
			EndSession();

		std::filesystem::path path(filePath);
		if (path.has_parent_path())
			std::filesystem::create_directories(path.parent_path());

		m_OutputStream.open(filePath);
		if (!m_OutputStream.is_open())
		{
			NGN_CORE_ERROR("Instrumentor: Failed to open profiling file: {}", filePath);
			return;
		}

		m_CurrentSession = new InstrumentationSession{ name };
		WriteHeader();
	}

	void Instrumentor::EndSession()
	{
		std::lock_guard lock(m_Mutex);

		if (!m_CurrentSession)
			return;

		WriteFooter();
		m_OutputStream.close();

		delete m_CurrentSession;
		m_CurrentSession = nullptr;
	}

	void Instrumentor::WriteHeader()
	{
		m_OutputStream << "{ \"otherData\": {}, \"traceEvents\": [";
		m_OutputStream.flush();
	}

	void Instrumentor::WriteFooter()
	{
		m_OutputStream << "]}";
		m_OutputStream.flush();
	}

	void Instrumentor::WriteProfile(const ProfileResult& result)
	{
		std::lock_guard lock(m_Mutex);

		if (!m_CurrentSession)
			return;

		m_FrameResults.push_back(result);

		if (m_ProfileCount++ > 0)
			m_OutputStream << ",";

		m_OutputStream
			<< "{"
			<< "\"cat\":\"function\","
			<< "\"dur\":" << (result.End - result.Start) << ','
			<< "\"name\":\"" << result.Name << "\","
			<< "\"ph\":\"X\","
			<< "\"pid\":0,"
			<< "\"tid\":" << result.ThreadID << ", "
			<< "\"ts\":" << result.Start
			<< "}";

		m_OutputStream.flush();
	}

	InstrumentationTimer::~InstrumentationTimer()
	{
		if (!m_Stopped)
			Stop();
	}

	void InstrumentationTimer::Stop()
	{
		auto endTimePoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint)
			.time_since_epoch().count();

		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint)
			.time_since_epoch().count();

		uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());

		Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });
		m_Stopped = true;
	}
}