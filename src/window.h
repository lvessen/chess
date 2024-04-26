#include <memory>


struct WindowImpl;

class Window 
{
private:
	std::unique_ptr<WindowImpl> pImpl;
public:
	Window(int width, int height, const char * name);
	void MakeCurrentContext();
	void SwapBuffers();
	static void * GetAnyProcAddress(const char * name);
	bool ShouldClose();
	~Window();
private:

};
