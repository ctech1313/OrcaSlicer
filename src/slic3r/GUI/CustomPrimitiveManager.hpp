#ifndef slic3r_GUI_CustomPrimitiveManager_hpp_
#define slic3r_GUI_CustomPrimitiveManager_hpp_

#include <string>
#include <vector>
#include <memory>
#include <wx/string.h>
#include <wx/fswatcher.h>
#include "libslic3r/Model.hpp"

namespace Slic3r {
namespace GUI {

struct CustomPrimitive {
    std::string name;
    std::string filepath;
    std::string file_type;
};

class CustomPrimitiveManager : public wxEvtHandler {
public:
    CustomPrimitiveManager();
    ~CustomPrimitiveManager();
    
    // Initialize the manager with the custom primitives directory
    void initialize(const std::string& directory_path);
    
    // Scan directory for valid primitive models
    void scanDirectory();
    
    // Get list of available custom primitives
    const std::vector<CustomPrimitive>& getPrimitives() const { return m_primitives; }
    
    // Load a specific primitive model
    std::unique_ptr<Model> loadPrimitive(const CustomPrimitive& primitive);

    // Manually refresh primitives list
    void refresh();

private:
    std::string m_directory_path;
    std::vector<CustomPrimitive> m_primitives;
    std::vector<std::string> m_supported_extensions;
    std::unique_ptr<wxFileSystemWatcher> m_watcher;
    
    bool isValidModelFile(const std::string& filename);
    void onDirectoryChanged(wxFileSystemWatcherEvent& event);
};

} // namespace GUI
} // namespace Slic3r

#endif 