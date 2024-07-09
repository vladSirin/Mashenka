#pragma once

#include <string> // this is the string library
#include <chrono> // this is the time library
#include <algorithm> // this is the algorithm library, which includes functions: sort, max, min, etc
#include <fstream> // file stream
#include <iomanip>

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
    using FloatingMicroseconds = std::chrono::duration<double, std::micro>; // this is the floating microseconds

    struct ProfileResult // this is a struct that contains the profile result
    {
        std::string Name;
        FloatingMicroseconds Start;
        std::chrono::microseconds ElapsedTime;
        std::thread::id ThreadID;
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
        std::mutex m_Mutex;
        InstrumentationSession* m_CurrentSession;

        std::ofstream m_OutputStream;
        // this is the output stream, used for writing to file
        // std::ofstream is a class to write on files, from the fstream library in C++

    public:
        Instrumentor()
            : m_CurrentSession(nullptr) // profile count is removed because it is not used
        {
        }

        // BeginSession and EndSession are used to start and end the profiling session
        void BeginSession(const std::string& name, const std::string& filepath = "results.json")
        {
            std::lock_guard lock(m_Mutex); // lock the mutex, for thread safety
            if (m_CurrentSession)
            {
                // If there is already a current session, then close it before beginning new one
                // Subsequent profiling output meant for the original session will end up in the
                // newly opened session instead. That's better than having badly formatted profiling output.
                if (Log::GetCoreLogger()) // if the core logger is available
                {
                    // Edge case: BeginSession() might be before Log::Init()
                    MK_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name,
                                  m_CurrentSession->Name);
                }
                InternalEndSession();
            }
            m_OutputStream.open(filepath);
            if (m_OutputStream.is_open())
            {
                m_CurrentSession = new InstrumentationSession({name});
                WriteHeader();
            }
            else
            {
                if (Log::GetCoreLogger())
                {
                    MK_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
                }
            }
        }

        void EndSession()
        {
            std::lock_guard lock(m_Mutex); // lock the mutex, for thread safety
            InternalEndSession();
        }

        void WriteProfile(const ProfileResult& result)
        {
            std::stringstream json; // this is the json stream
            std::string name = result.Name;
            std::replace(name.begin(), name.end(), '"', '\'');

            // setup the json format
            json << std::setprecision(3) << std::fixed; // set the precision to 3 and fixed
            json << ",{";
            json << "\"cat\":\"function\",";
            json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
            json << "\"name\":\"" << result.Name << "\",";
            json << "\"ph\":\"X\",";
            json << "\"pid\":0,";
            json << "\"tid\":" << result.ThreadID << ",";
            json << "\"ts\":" << result.Start.count();
            json << "}";


            std::lock_guard lock(m_Mutex);
            if (m_CurrentSession)
            {
                m_OutputStream << json.str(); // write the json to the output stream
                m_OutputStream.flush();
            }
        }

        // this is the Get function, used to get the instance of the instrumentor
        // Singleton pattern, this needs to be called when using the instrumentor
        static Instrumentor& Get()
        {
            static Instrumentor instance;
            return instance;
        }

    private:
        void WriteHeader()
        {
            m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
            m_OutputStream.flush();
        }

        void WriteFooter()
        {
            m_OutputStream << "]}";
            m_OutputStream.flush();
        }

        // Note: you must already own lock on m_Mutex before
        // calling InternalEndSession()
        void InternalEndSession()
        {
            if (m_CurrentSession)
            {
                WriteFooter();
                m_OutputStream.close();
                delete m_CurrentSession;
                m_CurrentSession = nullptr;
            }
        }
    };


    class InstrumentorTimer // this is the instrumentor timer class
    {
    public:
        // this is the constructor for the instrumentor timer
        InstrumentorTimer(const char* name)
            : m_Name(name), m_Stopped(false)
        {
            // steady_clock is used for measuring time intervals, it is not tied to the system clock
            m_StartTimepoint = std::chrono::steady_clock::now();
        }

        ~InstrumentorTimer()
        {
            if (!m_Stopped)
                Stop();
        }

        void Stop()
        {
            auto endTimepoint = std::chrono::steady_clock::now();
            auto highResStart = FloatingMicroseconds(m_StartTimepoint.time_since_epoch());
            auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch()
                - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

            Instrumentor::Get().WriteProfile({m_Name, highResStart, elapsedTime, std::this_thread::get_id()});

            m_Stopped = true;
        }

    private:
        const char* m_Name;
        std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
        bool m_Stopped;
    };
}

namespace InstrumentorUtils
{
    // This is a template struct with a parameter of N.
    // This contains a character array @Size of N;
    template <size_t N>
    struct ChangeResult
    {
        char Data[N];
    };

    /* This is a template function that takes into 2 parameters, N & K, which are the sizes of two strings
     * The size of the array will be evaluated at compile time, this allows compiler optimization
     * This allows the function to work with array of any size and remove dependencies to things like std::string
     */
    template <size_t N, size_t K>
    constexpr auto CleanupOutputString(const char (&expr)[N], const char (&remove)[K])
    {
        // initialize the result arary
        ChangeResult<N> result = {};

        // index initialization
        size_t srcIndex = 0;
        size_t dstIndex = 0;

        // Loop go through the 'expr' array until 'srcIndex' reaches N
        while (srcIndex < N)
        {
            // Initialize matchIndex, this tracks how many characters of the 'remove' string have been matched
            size_t matchIndex = 0;

            // Substring main loop
            // 'matchIndex < K-1' ensures we don't go out of the bounds of the 'remove' string
            // 'srcIndex + matchIndex < N - 1' ensures we don't go out of bounds of the 'expr' string
            // 'expr[srcIndex+matchIndex] == remove[matchIndex] checks if the characters match
            // if it matches, increment matchIndex
            while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[
                matchIndex])
                matchIndex++;

            // if a full match of the 'remove' substring is found, 'srcIndex' is incremented by 'matchIndex' to skip over the matched substring
            if (matchIndex == K - 1)
            {
                srcIndex += matchIndex;
            }
            else
            {
                if (srcIndex < N - 1) //Ensure we don't access out of bounds

                    // Ternary operator, condition ? value_if_true : value_if_false;
                    // this line copies the indexed data from expr to result.Data, and replace " with '
                    // in C++ single quote ' is used to denote character literals, that you cannot denote ' itself using '''
                    // which will leads to error, a backlash is used to escape this
                    result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
            }
            srcIndex++;
        }
        return result;
    }
}

/*
 * Macros for profiling
 * Get the instance of the instrumentor and call the BeginSession, EndSession, WriteProfile functions
 * Using the SCOPE to create a timer and call the WriteProfile function, using LINE for unique name
 * Using the FUNCTION to create scope for function with the name of the function
 * If MK_PROFILE is not defined, all marcos will be empty and not effective
 */
#define MK_PROFILE 0
#if MK_PROFILE
// Resolve which function signature macro will be used based on the compiler that is using here
// Note tha this only is resolved when the (pre)compiler starts
// so the syntax highlighting will not work properly in your editor

#if defined(__GNUC__) || (defined(_MWERKS) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) ||defined(__ghs__)
    #define MK_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
    #define MK_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__)) || (_MSC_VER) // checking MSVC version or FUNCSIG defined
    #define MK_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMC__) && (__IBMC__ >= 500))
    #define MK_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
    #define MK_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
    #define MK_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
    #define MK_FUNC_SIG __func__
#else
#define MK_FUNC_SIG "MK_FUNC_SIG unknown!"
#endif

#define MK_PROFILE_BEGIN_SESSION(name, filepath) ::Mashenka::Instrumentor::Get().BeginSession(name, filepath)
#define MK_PROFILE_END_SESSION() ::Mashenka::Instrumentor::Get().EndSession()

// The purpose of this macro is to create a scope-based profiling timer with a cleaned-up function name,
// ensuring unique variable names by including the line number in the identifiers.

#define HZ_PROFILE_SCOPE(name) \
constexpr auto fixedName##__LINE__ = ::Hazel::InstrumentorUtils::CleanupOutputString(name, "__cdecl "); \
::Hazel::InstrumentationTimer timer##__LINE__(fixedName##__LINE__.Data)



#define MK_PROFILE_FUNCTION() MK_PROFILE_SCOPE(MK_FUNC_SIG)
#else
#define MK_PROFILE_BEGIN_SESSION(name, filepath)
#define MK_PROFILE_END_SESSION()
#define MK_PROFILE_SCOPE(name)
#define MK_PROFILE_FUNCTION()
#endif
