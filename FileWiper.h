#ifndef FILE_WIPER_H
#define FILE_WIPER_H

#ifdef WIN32
#ifdef FILEWIPER_EXPORTS
#define WIPER_API __declspec(dllexport)
#else
#define WIPER_API __declspec(dllimport)
#endif
#endif

#ifdef __linux__
#define WIPER_API
#endif

#ifdef __cplusplus
extern "C" {
#endif 

class WIPER_API FileWiper{

public:
	enum WipeMode
	{
		ZEROES,
		PSEUDO_RANDOM,
		DOD5220_22M,
		PETER_GUTMANN
	};

public:
	FileWiper();
	FileWiper(const char* file, WipeMode mode = DOD5220_22M);
	~FileWiper();

	void setFile(const char* file);
	void setMode(WipeMode mode);
	bool start();

private:
	class WipeImpl;
	WipeImpl* m_impl;
};

#ifdef __cplusplus
}
#endif

#endif
