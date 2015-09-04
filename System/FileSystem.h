// FileSystem.h:

class FileSystem
{
public:
	
	// Default constructor
	FileSystem(const char* filename)
		:m_cFilename(filename)
	{};

	void SyncRead(char* &buffer, size_t length)
	{
		FILE* f = fopen(m_cFilename, "r");
		//fread(buffer, );
		fclose(f);
	}

private:

	const char*							m_cFilename;
};