#include <memory>


struct WindowImpl;

class Window 
{
private:
	std::unique_ptr<WindowImpl> pImpl;
public:
	Window(int width, int height, const char * name);
	~Window();
	friend bool ShouldClose(Window& window);
	friend void SwapBuffers(Window& window);
	friend void MakeCurrentContext(Window& window);

	uint32_t width;
	uint32_t height;
private:
};

void * GetAnyProcAddress(const char * name);
