#pragma once

#include <string> // this is the string library
#include <chrono> // this is the time library
#include <algorithm> // this is the algorithm library, which includes functions: sort, max, min, etc
#include <fstream> // file stream

#include <thread>

/*
 * SUMMARY:
 * This is the Instrumentor class, used for profiling the application
 * - It contains the ProfileResult struct, which contains the profile result
 * - It contains the InstrumentationSession struct, which contains the instrumentation session
 * - It contains the Instrumentor class, which contains the BeginSession, EndSession, WriteProfile, WriteHeader, WriteFooter, Get functions
 * - It contains the InstrumentorTimer class, which contains the constructor, destructor, Stop functions
 * - It contains the macros for profiling
 *
 * HOW IT WORKS:
 * The Instrumentor class is a singleton class, only created when called by Get function, here, by the Marco BeginSession
 * The InstrumentorTimer is being used and calling the Instrumentor class WriteProfile function to write the profile result when destroyed
 */

namespace Mashenka
{
    struct ProfileResult // this is a struct that contains the profile result
    {
        std::string Name;
        long long Start, End;
        size_t ThreadID;
    };

    struct InstrumentationSession // this is a struct that contains the instrumentation session
    {
        std::string Name;
    };

    class Instrumentor // this is the instrumentor class
    {
    private:
        //a raw ptr is used as the session is created and destroyed in the Instrumentor class
        // here the profiling session is not tied to the lifetime of any object, thus a raw prt is used
        InstrumentationSession* m_CurrentSession;

        std::ofstream m_OutputStream;
        // this is the output stream, used for writing to file
        // std::ofstream is a class to write on files, from the fstream library in C++
        
        int m_ProfileCount; // this is the profile count, used to add commas to the json file to make it valid
        
    public:
        Instrumentor()
            : m_CurrentSession(nullptr), m_ProfileCount(0)
        {
        }

        // BeginSession and EndSession are used to start and end the profiling session
        void BeginSession(const std::string& name, const std::string& filepath = "results.json")
        {
            m_OutputStream.open(filepath);
            WriteHeader();
            m_CurrentSession = new InstrumentationSession({ name });
        }

        void EndSession()
        {
            WriteFooter();
            m_OutputStream.close();
            delete m_CurrentSession;
            m_CurrentSession = nullptr;
            m_ProfileCount = 0;
        }

        void WriteProfile(const ProfileResult& result)
        {
            if (m_ProfileCount++ > 0)
                m_OutputStream << ",";
            std::string name = result.Name;
            std::replace(name.begin(), name.end(), '"', '\'');

            // this is the json format
            m_OutputStream << "{";
            m_OutputStream << "\"cat\":\"function\",";
            m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
            m_OutputStream << "\"name\":\"" << name << "\",";
            m_OutputStream << "\"ph\":\"X\",";
            m_OutputStream << "\"pid\":0,";
            m_OutputStream << "\"tid\":" << result.ThreadID << ",";
            m_OutputStream << "\"ts\":" << result.Start;
            m_OutputStream << "}";

            m_OutputStream.flush(); // flush the output stream
        }

        void WriteHeader()
        {
            m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
            m_OutputStream.flush();
        }

        void WriteFooter()
        {
            m_OutputStream << "]}";
            m_OutputStream.flush();
        }

        // this is the Get function, used to get the instance of the instrumentor
        // Singleton pattern, this needs to be called when using the instrumentor
        static Instrumentor& Get()
        {
            static Instrumentor instance;
            return instance;
        }

    };

    class InstrumentorTimer // this is the instrumentor timer class
    {
    public:
        // this is the constructor for the instrumentor timer
        InstrumentorTimer(const char* name)
            : m_Name(name), m_Stopped(false)
        {
            m_StartTimepoint = std::chrono::high_resolution_clock::now();
        }

        ~InstrumentorTimer()
        {
            if (!m_Stopped)
                Stop();
        }

        void Stop()
        {
            auto endTimepoint = std::chrono::high_resolution_clock::now();
            long long start = std::chrono::time_point_cast
                <std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast
                <std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

            size_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());  // NOLINT(clang-diagnostic-shorten-64-to-32)
            Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID }); // Create a profile result and write it to the instrumentor

            m_Stopped = true;
        }

    private:
        const char* m_Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
        bool m_Stopped;
    };
}


/*
 * Macros for profiling
 * Get the instance of the instrumentor and call the BeginSession, EndSession, WriteProfile functions
 * Using the SCOPE to create a timer and call the WriteProfile function, using LINE for unique name
 * Using the FUNCTION to create scope for function with the name of the function
 * If MK_PROFILE is not defined, all marcos will be empty and not effective
 */
#define MK_PROFILE 1
#if MK_PROFILE
    #define MK_PROFILE_BEGIN_SESSION(name, filepath) ::Mashenka::Instrumentor::Get().BeginSession(name, filepath)
    #define MK_PROFILE_END_SESSION() ::Mashenka::Instrumentor::Get().EndSession()
    #define MK_PROFILE_SCOPE(name) ::Mashenka::InstrumentorTimer timer##__LINE__(name);
    #define MK_PROFILE_FUNCTION() MK_PROFILE_SCOPE(__FUNCSIG__)
#else
    #define MK_PROFILE_BEGIN_SESSION(name, filepath)
    #define MK_PROFILE_END_SESSION()
    #define MK_PROFILE_SCOPE(name)
    #define MK_PROFILE_FUNCTION()
#endif
