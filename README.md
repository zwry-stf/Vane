# Vane Menu Framework

Vane is a modern C++17 (and higher) framework for creating dynamic, interactive menus rendered directly inside a Direct3D 11 window. It provides a clean, easy-to-use API to add and customize various widgets such as labels, tabs, checkboxes, sliders, buttons, and more.

## Features

- **Dynamic Menu Creation:** Quickly add and configure widgets to build complex menus.
- **Rich Widget Set:** Includes support for labels, tabs, checkboxes, sliders, color pickers, buttons, hotkeys, and dropdown lists.
- **Theming:** Comes with a default theme and allows customization for both the menu and background.
- **Integrated Configuration System:** Easily manage and persist settings through a built-in configuration module.
- **Direct3D 11 Integration:** Specifically designed to work within a d3d11 context, making it ideal for game or real-time application interfaces.

## Requirements

- **C++17 or higher**
- **Direct3D 11 (d3d11)**
- **Visual Studio 2022** (recommended for Windows development)

## Installation

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/zwry-stf/Vane.git
   ```
2. **Open in Visual Studio 2022:**
   Open the solution file and ensure your project settings are configured for C++17 (or higher).
3. **Build and Run:**
   Build your project and integrate Vane into your d3d11 application.

## Usage

After initializing Direct3D 11, you can begin adding widgets to your menu. Below is an example demonstrating how to set up a basic menu structure:

```cpp
// After initializing d3d11, add widgets

// Adding labels
Vane::AddLabel("Name");

// Adding a tab
Vane::AddTab("Tab");

// Inside a Tab (2 is the number of areas in the child tab)
auto child = Vane::LastTab()->AddChildTab("Name", 2);

// Optionally add an icon (Use Bin2C on PNG files)
child->SetIcon(IconData, sizeof(IconData));

// Add Widgets 
auto checkbox = child->AddCheckbox("Checkbox 1", &value, area);

// Linking to other widgets like a color picker overlay or child window
checkbox->LinkTo(
    child->AddColorpickerOverlay(&color_value, true)
);
// Alternatively:
checkbox->LinkTo(
    child->AddChildWindow("ChildWindow 1")
);

// Additional widget options include sliders, standard color pickers, buttons, hotkeys, and dropdown lists.

// Adding default theme options for the menu and background
Vane::AddDefaultThemeTab();

// Adding a configuration tab
Vane::AddConfigTab("Config");

// Config System usage:
Vane::Config::AddModule<bool>(&value, "value");
```

**Important**:
Add Widgets and Config Modules before calling Vane::Init(). 
A more detailed Documentation can be found [here](Documentation.md)

## Images

Visual examples can help users understand the capabilities of Vane. Add screenshots of your framework in action to an `images` folder in your repository, and reference them here. For example:

![image](https://github.com/user-attachments/assets/ffbf5845-e0c3-43db-8686-b3f9c0c5f85f)
![image](https://github.com/user-attachments/assets/1ed38b91-e47f-438d-a3a7-070ba8cb5982)

## License

This project is licensed under a custom license. See the [LICENSE](LICENSE) file for full terms and conditions.
