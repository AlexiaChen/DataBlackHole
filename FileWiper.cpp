/*

为什么需要多次覆盖呢？直观上理解，一次写入后，磁盘上的数据就变化了。其实在硬件层面，不是这样的，还是能够恢复的。恢复的技术有下面两种：

第一种是：当硬盘读写头在硬盘上写入一位数据时，它使用的信号强度只是用来写入一位数据，并不会影响到相邻位的数据。由于这个写入信号并不是很强，

因此你可以通过它写入的数据位的绝对信号强度来判断此前该数据位所保存的是何种数据。换句话说，如果二进制数据0被二进制数据1所覆盖，其信号强度会比

二进制数据1被覆盖要弱一些。使用专门的硬件就可以检测出准确的信号强度。把被覆盖区域读出的信号减去所覆盖数据的标准信号强度，就能获得原先数据的一个副本。

更令人吃惊的是，这一恢复过程可以被重复7次！因此如果想避免别人使用这种方法来窃取你的数据，至少要覆盖该数据区域7次，而且每次还应该使用不同的随机数据。

第二种数据恢复技术则是利用了硬盘读写头的另一个特点：读写头每次进行写操作的位置并不一定对得十分精确。这就能让专家们在磁道的边缘侦测到原有的数据

（也被称为影子数据）。只有重复地覆写数据才能消除这些磁道边缘的影子数据。由于硬件的这种特性，在销毁文件的时候，才会需要多次覆盖。

*/



#include "FileWiper.h"

#include <cstdio>
#include <random>
#include <string>
#include <memory>

class FileWiper::WipeImpl{
public:
	
	std::string m_file;
	FileWiper::WipeMode m_mode;

	FILE* m_fp;

public:
	WipeImpl() :m_fp(nullptr){}
	~WipeImpl(){}

public:

	size_t getFileSize()
	{
		size_t fileSize;
		FILE *fp = nullptr;
#ifdef WIN32
		errno_t err = fopen_s(&fp, m_file.c_str(), "rb");
		if (err != 0) return 0;
#elif __linux__
		FILE *fp = fopen(m_file.c_str(), "rb");
		if (fp == nullptr) return 0;
#endif
		fseek(fp, 0, SEEK_END);
		fileSize = ftell(fp);
		
		fclose(fp);
		
		return fileSize;
	}

	bool openFile()
	{
#ifdef WIN32
		errno_t err = fopen_s(&m_fp, m_file.c_str(), "wb");
		if (err != 0) return false;
#elif __linux__
		m_fp = fopen(m_file.c_str(), "rb");
		if (m_fp == nullptr) return false;
#endif
		return true;
    }

	void closeFile()
	{
		fclose(m_fp);
		m_fp = nullptr;
	}

	bool RenameAnddeleteFile()
	{
		if (rename(m_file.c_str(), "C"))
		{
			return false;
		}
		
		if (remove("C"))
		{
			return false;
		}

		return true;
	}

	void wipe(unsigned char c, size_t filesize)
	{
		fwrite(&c, sizeof(unsigned char), filesize, m_fp);
	}

	void wipe_buffer(char* buffer,size_t len)
	{
		fwrite(buffer, sizeof(char)*len, 1, m_fp);
	}

	void gen_random_sequence_characters(char *buffer, size_t len) {
		static const char alphaBet[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";

		std::random_device rd;
		
		for (size_t i = 0; i < len; ++i) {
			buffer[i] = alphaBet[rd() % (sizeof(alphaBet) - 1)];
		}

	}
	
	bool zeroes()
	{
		
		if (!openFile())
		{
			return false;
		}
		
		size_t file_size = getFileSize();
		
		if (file_size <= 0)
		{
			closeFile();
			return false;
		}
		
		//for 5 pass;
		for (int i = 0; i < 5; ++i)
		{
			wipe(0x00, file_size);
		}
		
		
		closeFile();

		if (!RenameAnddeleteFile())
		{
			return false;
		}
		
		return true;

	}
	
	bool dod5220_22M()
	{
		if (!openFile())
		{
			return false;
		}

		size_t file_size = getFileSize();

		if (file_size <= 0)
		{
			closeFile();
			return false;
		}

		for (int i = 0; i < 3; ++i)
		{
			wipe(0x00, file_size);
			wipe(0x01, file_size);
		}
		
		std::random_device rd;

		wipe(rd() % 0xff, file_size);

		closeFile();
		
		if (!RenameAnddeleteFile())
		{
			return false;
		}

		return true;
	}
	
	bool psedoRandom()
	{ 
		if (!openFile())
		{
			return false;
		}

		size_t file_size = getFileSize();
		if (file_size <= 0)
		{
			closeFile();
			return false;
		}

		std::random_device rd;

		for (int i = 0; i < 5; ++i)
		{
			wipe(rd() & 0xff, file_size);
		}
		
		closeFile();
		
		if (!RenameAnddeleteFile())
		{
			return false;
		}

		return true;
	}
	
	
	bool peterGutMann()
	{
		if (!openFile())
		{
			return false;
		}

		size_t file_size = getFileSize();
		if (file_size <= 0)
		{
			closeFile();
			return false;
		}

		char* pRandomBuff = new char[file_size];
		memset(pRandomBuff, 0x00, file_size);

		gen_random_sequence_characters(pRandomBuff, file_size);
		
		for (int i = 0; i < 35; ++i)
		{
			wipe_buffer(pRandomBuff, file_size);
		}
		
		closeFile();
		
		if (!RenameAnddeleteFile())
		{
			delete pRandomBuff;
			return false;
		}

		delete pRandomBuff;

		return true;
	}
};


FileWiper::FileWiper()
{
	m_impl = new WipeImpl();
}

FileWiper::FileWiper(const char* file, WipeMode mode)
{
	m_impl = new WipeImpl();
	m_impl->m_file.assign(file);
	m_impl->m_mode = mode;
}

FileWiper::~FileWiper()
{
	if (m_impl) delete m_impl;
}

void FileWiper::setFile(const char* file)
{
	m_impl->m_file.assign(file);
}

void FileWiper::setWipeMode(WipeMode mode)
{
	m_impl->m_mode = mode;
}

bool FileWiper::start()
{
	
	bool ret;
	
	switch (m_impl->m_mode)
	{
	case WipeMode::ZEROES:
		ret = m_impl->zeroes();
		break;
	case WipeMode::DOD5220_22M:
		ret = m_impl->dod5220_22M();
		break;
	case WipeMode::PSEUDO_RANDOM:
		ret = m_impl->psedoRandom();
		break;
	case WipeMode::PETER_GUTMANN:
		ret = m_impl->peterGutMann();
		break;
	default:
		ret = false;
		break;
	}
	
	return ret;
}

