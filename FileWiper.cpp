#include "FileWiper.h"

#include <cstdio>
#include <random>

class FileWiper::WipeImpl{
public:
	
	bool m_isValid;
	std::string m_file;
	FileWiper::WipeMode m_mode;

	FILE* m_fp;

public:
	WipeImpl() :m_isValid(false), m_fp(nullptr){}
	~WipeImpl(){}

public:

	size_t getFileSize()
	{
		size_t fileSize;
		
		errno_t err = fopen_s(&m_fp, m_file.c_str(), "rb");
		if (err != 0) {
			
			return -1;
		}
		
		fseek(m_fp, 0, SEEK_END);
		fileSize = ftell(m_fp);
		
		closeFile();
		
		return fileSize;
	}

	void openFile()
	{
		errno_t err = fopen_s(&m_fp, m_file.c_str(), "wb");
		if (err != 0) m_isValid = false;
    }

	void closeFile()
	{
		fclose(m_fp);
		m_fp = nullptr;
	}

	void RenameAnddeleteFile()
	{
		if (rename(m_file.c_str(), "C"))
		{
			return;
		}
		
		remove("C");
	}
	
	void zeroes()
	{
		
		openFile();
		
		unsigned char garbage = 0x00;
		
		if (getFileSize() <= 0)
		{
			closeFile();
			return;
		}
		
		//for one pass;
		fwrite(&garbage, sizeof(garbage), getFileSize(), m_fp);

		closeFile();

		RenameAnddeleteFile();

	}

	void wipe(unsigned char c)
	{
		for (long i = 0; i < getFileSize(); ++i)
			fwrite(&c, 1, 1, m_fp);
	}
	
	void dod5220_22M()
	{
		openFile();

		for (int i = 0; i < 3; ++i)
		{
			wipe(0x00);
			wipe(0x01);
		}
		std::random_device rd;

		wipe(rd() % 0xff);

		closeFile();
		RenameAnddeleteFile();
	}
	
	void psedoRandom()
	{ 
		openFile();

		size_t file_size = getFileSize();
		if (file_size <= 0)
		{
			closeFile();
			return;
		}

		std::random_device rd;
		
		wipe(rd() % 0xff);

		closeFile();
		RenameAnddeleteFile();
	}
	
	void dod5200_28STD()
	{
		openFile();

		closeFile();
		RenameAnddeleteFile();
	}
	
	void peterGutMann()
	{
		openFile();

		closeFile();
		RenameAnddeleteFile();
	}
};


FileWiper::FileWiper()
{
	m_impl = new WipeImpl();
}

FileWiper::FileWiper(const std::string &file, WipeMode mode)
{
	m_impl = new WipeImpl();
	m_impl->m_file = file;
	m_impl->m_mode = mode;
}

FileWiper::~FileWiper()
{
	if (m_impl) delete m_impl;
}

bool FileWiper::isValid() const 
{
	return m_impl->m_isValid;
}

void FileWiper::setFile(const std::string &file)
{
	m_impl->m_file = file;
}

void FileWiper::setWipeMode(WipeMode mode)
{
	m_impl->m_mode = mode;
}

void FileWiper::start()
{
	switch (m_impl->m_mode)
	{
	case WipeMode::ZEROES:
		m_impl->zeroes();
		break;
	case WipeMode::DOD5220_22M:
		m_impl->dod5220_22M();
		break;
	case WipeMode::PSEUDO_RANDOM:
		m_impl->psedoRandom();
		break;
	case WipeMode::DOD5200_28STD:
		m_impl->dod5200_28STD();
		break;
	case WipeMode::PETER_GUTMANN:
		m_impl->peterGutMann();
		break;
	default:
		break;
	}
}

