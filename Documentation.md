# Vane Menu Framework Documentation

## Table of Contents

- [Overview](#overview)
- [Requirements](#requirements)
- [Installation and Setup](#installation-and-setup)
- [Integrating Vane into Your Application](#integrating-vane-into-your-application)
- [API Overview](#api-overview)
  - [Vane Class and Core Components](#vane-class-and-core-components)
  - [Nested Classes](#nested-classes)
- [Widget and Menu Construction](#widget-and-menu-construction)
- [Event Handling and Rendering](#event-handling-and-rendering)
- [Advanced Usage](#advanced-usage)
- [Troubleshooting and Tips](#troubleshooting-and-tips)

---

## Overview

Vane is a lightweight framework that simplifies the creation of interactive menus for applications using Direct3D 11. It provides an extensive API to add labels, tabs, checkboxes, sliders, color pickers, buttons, hotkeys, dropdown lists, and more. Vane also includes built-in support for theming and configuration management.

---

## Requirements

- **Operating System:** Windows  
- **Compiler:** C++17 compliant, Visual Studio 2022  
- **Graphics API:** Direct3D 11

---

## Installation and Setup

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/yourusername/vane-menu-framework.git
   ```

2. **Open the Project:**

   Open the solution file in Visual Studio 2022. Make sure your project settings target C++17 or higher.

3. **Configure Dependencies:**

   Ensure that d3d11.lib and d3dcompiler.lib are properly set up in your project. Vane includes headers from its own Renderer and Contents directories.

---

## Integrating Vane into Your Application

Before calling `Vane::Init()`, **you must add your widgets and register your config modules**. This is crucial because Vane uses these settings during initialization.

### Steps Overview

1. **Initialize Direct3D 11:**

   Set up your Direct3D device, swap chain, and render target view.

2. **Add Widgets and Config Modules:**

   Call functions such as `Vane::AddLabel()`, `Vane::AddTab()`, `Vane::AddDefaultThemeTab()`, and register configuration modules (using `Vane::Config::AddModule<T>()`). This must be done **before** calling `Vane::Init()`.

3. **Initialize Vane:**

   After adding widgets and configuration modules, initialize Vane by calling:

   ```cpp
   if (!Vane::Init(g_pSwapChain, XyVec2(800.f, 550.f))) {
       // Handle initialization error
       std::cerr << "Failed to initialize Vane \"" << Vane::Errors::Get() << "\"\n";
       return -1;
   }
   ```

4. **Process Windows Messages:**

   Forward window messages to Vane, **except for WM_MOUSEMOVE** (see note below).

### WM_MOUSEMOVE Handling

When processing window messages in your `WndProc`, **do not forward WM_MOUSEMOVE messages to Vane's `WndProc`**. Vane handles mouse input every frame separately.

**Example:**

```cpp
LRESULT CALLBACK WndProc(HWND hWnd, uint32_t msg, uint64_t wParam, int64_t lParam) {
    if (msg == WM_MOUSEMOVE) {
        // Process WM_MOUSEMOVE here without forwarding to Vane.
        return Vane::open ? S_OK : DefWindowProc(hWnd, msg, wParam, lParam);
    }

    // Setting Cursor (example code)
    if (msg == WM_SETCURSOR)
    {
        if (Vane::open && Vane::Cursor::actual.load() != Vane::Cursor::arrow)
        {
            Vane::Cursor::Set();
            return S_OK;
        }
        else
        {
            SetCursor(Vane::Cursor::arrow);
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }

    }

    // Forward other messages to Vane's WndProc.
    auto ret = Vane::WndProc(msg, wParam, lParam);
    if (ret.has_value())
        return ret.value();

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
```

---

## API Overview

### Vane Class and Core Components

The main class `Vane` encapsulates the framework functionality and includes several nested classes and static functions:

- **Initialization & Shutdown:**  
  - `Vane::Init(IDXGISwapChain*, const XyVec2 menuSize)` – Initializes Vane.
  - `Vane::Destroy()` – Cleans up resources.
  
- **Rendering:**  
  - `Vane::Render()` – Called every frame to render the menu.
  
- **Event Handling:**  
  - `Vane::WndProc(uint32_t msg, uint64_t wParam, int64_t lParam)` – Processes window messages (excluding WM_MOUSEMOVE).
  
- **Widget Management:**  
  - `Vane::AddTab(const std::string& text)` – Adds a new tab.
  - `Vane::AddLabel(const std::string& text)` – Adds a label.
  - `Vane::AddConfigTab(const std::string& text)` – Creates a configuration tab.
  - `Vane::AddDefaultThemeTab()` – Adds theme options.
  - `Vane::LastTab()` – Returns the most recently added tab.

### Nested Classes

Vane is organized into several nested classes that manage styles, data, utilities, backgrounds, cursors, icons, error handling, and configuration.

- **Style:**  
  Holds static styling settings (sizes, colors, spacing, etc.).

- **Data:**  
  Stores runtime data such as the last cursor position and font pointer.

- **Util:**  
  Provides helper functions for interpolation, color conversion, and icon loading.

- **Background:**  
  Manages background rendering and effects through shader constants and textures.

- **Cursor:**  
  Handles cursor initialization and setting.

- **Icons:**  
  Manages icon textures used throughout the framework.

- **Errors:**  
  Provides methods for logging and retrieving error messages.

- **Config:**  
  Manages configuration modules, allowing you to save and load settings. Use `Vane::Config::AddModule<T>()` to register configuration items before initialization.

---

## Widget and Menu Construction

Widgets are added using simple static functions. For example, you might create a tab, add a child tab, and then insert various widget types:

```cpp
// Create a new tab called "Theme"
TabNormal* tab = Vane::AddTab("Theme");

// Add a child tab with two areas (ensure this is done before calling Vane::Init)
ChildTab* child = tab->AddChildTab("Menu", 2);
child->SetIcon(_Icons::_acTheme, sizeof(_Icons::_acTheme));

// Add a group titled "Colors" in area 0
child->AddGroupTitle("Colors", 0);
child->AddColorpicker("Background", &Style::Background, true, 0);
child->AddColorpicker("Accent", &Style::Accent, true, 0);
child->EndGroup(0);
```

---

## Event Handling and Rendering

During each frame in your render loop:
- Clear the render target.
- Call `Vane::Render()` to draw the menu.
- Present the frame using the swap chain.

---

## Advanced Usage

### Configuration System

Vane’s configuration system allows you to store and reset various settings. Register configuration modules using the template function `Vane::Config::AddModule<T>()` **before** initializing Vane.

**Example:**

```cpp
#define Add Vane::Config::AddModule

void AddConfig() {
    Add<XyColor>(&Vane::Style::Background, "Style::Background");
    Add<float>(&Vane::Style::AnimationSpeed, "Style::AnimationSpeed");
    // Add more modules as needed...
}
```

### Customizing Themes and Styles

Modify the static variables in `Vane::Style` to adjust colors, spacing, and other layout properties.

### Overlays and Widget Linking

Some Widgets can be linked with overlays or child windows. For example, a checkbox might be linked color picker:

```cpp
auto checkbox = child->AddCheckbox("Enable Feature", &someBoolean, area);
checkbox->LinkTo(child->AddColorpickerOverlay(&color_value, true));
```

or a ChildWindow:

```cpp
auto checkbox = child->AddCheckbox("Enable Feature", &someBoolean, area);
checkbox->LinkTo(child->AddChildWindow("ChildWindow"));
```

---

## Troubleshooting and Tips

- **Order of Initialization:**  
  Ensure that widgets and configuration modules are added before calling `Vane::Init()`. This is critical for proper initialization.

- **Resource Management:**  
  Call Vane::Init() when resizing, dont call Vane::Destroy() if you dont actually want to destroy the whole thing

- **Input Handling:**  
  If you encounter issues with input, double-check your message loop and ensure that only the appropriate messages are forwarded to Vane.
