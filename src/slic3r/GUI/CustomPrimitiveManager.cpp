#include "CustomPrimitiveManager.hpp"
#include "libslic3r/Model.hpp"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <wx/event.h>

namespace fs = boost::filesystem;

namespace Slic3r {
namespace GUI {

CustomPrimitiveManager::CustomPrimitiveManager()
{
    // Initialize supported file extensions
    m_supported_extensions = {".stl", ".obj", ".3mf"};
    
    // Bind the filesystem watcher event
    Bind(wxEVT_FSWATCHER, &CustomPrimitiveManager::onDirectoryChanged, this);
}

CustomPrimitiveManager::~CustomPrimitiveManager()
{
    if (m_watcher) {
        m_watcher->RemoveAll();
    }
}

void CustomPrimitiveManager::initialize(const std::string& directory_path)
{
    m_directory_path = directory_path;
    
    // Create directory if it doesn't exist
    if (!fs::exists(m_directory_path)) {
        fs::create_directories(m_directory_path);
    }
    
    // Initialize filesystem watcher
    m_watcher = std::make_unique<wxFileSystemWatcher>();
    m_watcher->SetOwner(this);
    m_watcher->Add(wxFileName(m_directory_path), wxFSW_EVENT_ALL);
    
    scanDirectory();
}

void CustomPrimitiveManager::onDirectoryChanged(wxFileSystemWatcherEvent& event)
{
    // Rescan directory when changes are detected
    if (event.GetChangeType() != wxFSW_EVENT_WARNING) {
        scanDirectory();
        // Post event to update menu
        wxCommandEvent evt(EVT_REFRESH_CUSTOM_PRIMITIVES);
        wxPostEvent(this, evt);
    }
}

void CustomPrimitiveManager::refresh()
{
    scanDirectory();
}

void CustomPrimitiveManager::scanDirectory()
{
    m_primitives.clear();
    
    try {
        for (const auto& entry : fs::directory_iterator(m_directory_path)) {
            if (fs::is_regular_file(entry) && isValidModelFile(entry.path().string())) {
                CustomPrimitive primitive;
                primitive.filepath = entry.path().string();
                primitive.name = entry.path().stem().string();
                primitive.file_type = entry.path().extension().string();
                m_primitives.push_back(primitive);
            }
        }
    } catch (const fs::filesystem_error& ex) {
        // Handle filesystem errors
        BOOST_LOG_TRIVIAL(error) << "Error scanning custom primitives directory: " << ex.what();
    }
}

bool CustomPrimitiveManager::isValidModelFile(const std::string& filename)
{
    std::string extension = fs::path(filename).extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    
    return std::find(m_supported_extensions.begin(), 
                    m_supported_extensions.end(), 
                    extension) != m_supported_extensions.end();
}

std::unique_ptr<Model> CustomPrimitiveManager::loadPrimitive(const CustomPrimitive& primitive)
{
    try {
        return Model::read_from_file(primitive.filepath);
    } catch (const std::exception& ex) {
        BOOST_LOG_TRIVIAL(error) << "Error loading primitive model: " << ex.what();
        return nullptr;
    }
}

} // namespace GUI
} // namespace Slic3r 