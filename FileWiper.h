#ifndef FILE_WIPER_H
#define FILE_WIPER_H

#include <string>

#ifdef FILEWIPER_EXPORTS
#define WIPER_API __declspec(dllexport)
#else
#define WIPER_API __declspec(dllimport)
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
			DOD5200_28STD,
			PETER_GUTMANN
		};


public:
		FileWiper();
		FileWiper(const std::string &file, WipeMode mode = DOD5220_22M);
		~FileWiper();

		bool isValid() const;

		void setFile(const std::string &file);
		void setWipeMode(WipeMode mode);
		void start();


private:
		class WipeImpl;
		WipeImpl* m_impl;

};

#ifdef __cplusplus
}
#endif

#endif