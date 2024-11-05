# OrcaSlicer Custom Primitives Implementation Guide

## 1. Setup Development Environment

### Required Software
- Visual Studio 2019
- CMake (version 3.14 or later)
- Git and Git LFS
- Strawberry Perl

### Required Runtimes
- Microsoft Edge WebView2 Runtime
- Microsoft Visual C++ Redistributable 2019 (vcredist2019_x64)

## 2. Repository Setup
```bash
git clone https://github.com/SoftFever/OrcaSlicer.git
cd OrcaSlicer
git lfs pull
```

## 3. Understanding the Codebase

### UI Components
- Navigate to the `src` directory
- Identify files related to the Prepare tab and right-click context menu

### Menu Structure
- Examine existing menu item implementation
- Understand Qt's menu handling mechanisms

## 4. Feature Design

### Custom Primitives Support
- Supported Formats:
  - STL (.stl)
  - OBJ (.obj)
  - Additional formats as needed

### Model Preparation
- Collect/create 3D models for custom primitives
- Place models in designated folder

### Menu Integration
- Dynamic population of custom primitives
- Retention of native primitive access

## 5. Implementation

### Directory Structure
```
custom_primitives/
├── model1.stl
├── model2.obj
└── ...
```

### Menu Population
```cpp
#include <QDir>
#include <QAction>
#include <QMenu>

const QString CUSTOM_PRIMITIVES_PATH = "path/to/custom_primitives/";

void OrcaSlicer::populatePrepareContextMenu(QMenu* prepareContextMenu) {
    addNativePrimitives(prepareContextMenu);

    QDir dir(CUSTOM_PRIMITIVES_PATH);
    QStringList filters = {"*.stl", "*.obj"};
    QFileInfoList fileList = dir.entryInfoList(filters, QDir::Files);

    for (const QFileInfo &fileInfo : fileList) {
        QString modelName = fileInfo.baseName();
        QAction *action = new QAction(tr("Add %1").arg(modelName), this);
        connect(action, &QAction::triggered, this, [this, fileInfo]() {
            addCustomModel(fileInfo.absoluteFilePath());
        });
        prepareContextMenu->addAction(action);
    }
}
```

### Model Loading
```cpp
void OrcaSlicer::addCustomModel(const QString &filePath) {
    Model customModel = loadModel(filePath);
    insertModel(customModel);
}

Model OrcaSlicer::loadModel(const QString &filePath) {
    QFileInfo fileInfo(filePath);
    QString extension = fileInfo.suffix().toLower();

    if (extension == "stl") {
        return loadSTL(filePath);
    } else if (extension == "obj") {
        return loadOBJ(filePath);
    } else {
        // Handle unsupported format
    }
}
```

### File System Monitoring
```cpp
#include <QFileSystemWatcher>

void OrcaSlicer::initializeWatcher() {
    QFileSystemWatcher *watcher = new QFileSystemWatcher(this);
    watcher->addPath(CUSTOM_PRIMITIVES_PATH);
    connect(watcher, &QFileSystemWatcher::directoryChanged, 
            this, &OrcaSlicer::refreshCustomPrimitives);
}
```

## 6. Testing

### Unit Tests
- Verify menu item generation
- Test model loading
- Validate format support

### Integration Tests
- Native primitive functionality
- Custom/native primitive interaction
- UI responsiveness

### User Testing
- Conduct usability tests
- Gather user feedback

## 7. Documentation

### User Guide Updates
- Custom primitive addition process
- Supported format specifications
- Directory structure explanation

### Developer Documentation
- Code documentation
- Function descriptions
- Implementation details

## 8. Build and Deployment

### Build Process
```bash
cmake -B build -G "Visual Studio 16 2019" .
cmake --build build --config Release
```

### Contribution
- Follow code quality guidelines
- Include detailed commit messages
- Submit pull request with comprehensive description

## 9. Maintenance

### Issue Management
- Monitor bug reports
- Track user feedback
- Implement fixes as needed

### Continuous Improvement
- Performance optimization
- Feature refinement
- Bug fixes

## 10. Additional Considerations

### Security
- Input validation
- File path sanitization
- Exception handling

### Performance
- Optimize dynamic loading
- Test with large model sets
- Monitor memory usage

### Community Guidelines
- License compliance
- Contribution policy adherence
