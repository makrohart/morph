# Morph - C++ UI Framework with JavaScript Integration

Morph is a modern C++ UI framework that combines the power of C++ with JavaScript integration through V8 engine. It features:

- **Cross-platform rendering** using SDL3
- **Flexbox layout** powered by Facebook's Yoga layout engine
- **JavaScript integration** via V8 engine for dynamic UI creation
- **MVVM pattern** support with property change notifications
- **Event-driven architecture** with custom event handling
- **Modern C++20** features and RAII resource management

## Features

### Core Components
- **View**: Base class for all UI elements
- **WindowView**: Main window container
- **DivView**: Container view with background and border
- **ButtonView**: Interactive button component
- **TextView**: Text rendering with TTF font support

### Layout System
- Flexbox-based layout using Yoga engine
- Support for margins, padding, borders
- Responsive design with percentage-based sizing
- Background and border color customization

### JavaScript Integration
- V8 engine for JavaScript execution
- C++ classes exposed to JavaScript
- Property binding and event handling
- React-like component system support

## Building

### Prerequisites
- CMake 3.10+
- C++20 compatible compiler
- SDL3 development libraries
- V8 engine libraries
- Yoga layout engine

### Build Instructions
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Usage

### Basic C++ Usage
```cpp
#include "morph/WindowView.h"
#include "morph/DivView.h"
#include "morph/ButtonView.h"

// Create a window
auto window = new morph::WindowView();
window->setProperty("styleWidth", "800");
window->setProperty("styleHeight", "600");

// Create a container
auto container = new morph::DivView();
container->setProperty("widthPercent", "100");
container->setProperty("heightPercent", "100");

// Create a button
auto button = new morph::ButtonView();
button->setProperty("width", "200");
button->setProperty("height", "50");

// Build the hierarchy
button->addTo(container);
container->addTo(window);

// Show the window
window->show();
```

### JavaScript Integration
```javascript
// Create UI elements from JavaScript
const window = new WindowView();
window.setProperty("styleWidth", "800");
window.setProperty("styleHeight", "600");

const container = new DivView();
container.setProperty("widthPercent", "100");
container.setProperty("heightPercent", "100");

const button = new ButtonView();
button.setProperty("width", "200");
button.setProperty("height", "50");

// Set up event handling
button.onEvent("onClick", () => {
    console.log("Button clicked!");
});

// Build the hierarchy
button.addTo(container);
container.addTo(window);

// Show the window
window.show();
```

## Architecture

### Core Components
- **View System**: Hierarchical UI component tree
- **Layout Engine**: Yoga-based flexbox layout
- **Rendering**: SDL3-based hardware-accelerated rendering
- **JavaScript Bridge**: V8-based C++/JS interop
- **Event System**: Custom event handling and propagation

### Design Patterns
- **RAII**: Automatic resource management
- **MVVM**: Model-View-ViewModel pattern support
- **Observer**: Event-driven architecture
- **Factory**: Component creation patterns

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

## License

This project is licensed under the MIT License - see the LICENSE file for details.