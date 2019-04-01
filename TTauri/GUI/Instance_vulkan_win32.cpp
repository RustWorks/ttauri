#include "Instance_vulkan_win32.hpp"

#include "Window_vulkan_win32.hpp"

namespace TTauri::GUI {

using namespace std;
using namespace gsl;

Instance_vulkan_win32::Instance_vulkan_win32() :
    Instance_vulkan({ VK_KHR_WIN32_SURFACE_EXTENSION_NAME })
{
    // Start update loop.
    updateAndRenderThread = thread(Instance_vulkan_win32::updateAndRenderLoop, not_null<Instance_vulkan_win32 *>(this));
    maintanceThread = thread(Instance_vulkan_win32::maintenanceLoop, not_null<Instance_vulkan_win32 *>(this));
}

Instance_vulkan_win32::~Instance_vulkan_win32()
{
    try {
        [[gsl::suppress(f.6)]] {
            stopUpdateAndRender = true;
            stopMaintenance = true;

            maintanceThread.join();
            updateAndRenderThread.join();
        }
    } catch (...) {
        abort();
    }
}

void Instance_vulkan_win32::createWindow(std::shared_ptr<GUI::Window::Delegate> windowDelegate, const std::string &title)
{
    std::scoped_lock lock(mutex);

    auto window = TTauri::make_shared<GUI::Window_vulkan_win32>(windowDelegate, title);
    get_singleton<Instance>()->add(window);
}

void Instance_vulkan_win32::maintenanceLoop(gsl::not_null<Instance_vulkan_win32 *> self)
{
    auto threadID = GetCurrentThread();
    SetThreadDescription(threadID, L"TTauri::GUI Maintenance");

    while (!self->stopMaintenance) {
        self->maintenance();

        std::this_thread::sleep_for(50ms);
    }
}

void Instance_vulkan_win32::updateAndRenderLoop(gsl::not_null<Instance_vulkan_win32 *> self)
{
    auto threadID = GetCurrentThread();
    SetThreadDescription(threadID, L"TTauri::GUI Update And Render");

    while (!self->stopUpdateAndRender) {
        auto const hasBlockedOnVSync = self->updateAndRender(0, 0, true);

        if (!hasBlockedOnVSync) {
            // Wait for 1/60th of a second.
            std::this_thread::sleep_for(16ms);
        }
    }
}

}
