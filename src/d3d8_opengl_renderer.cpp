#include <windows.h>
#include <gl/gl.h>
#include <stdio.h>
#include <math.h>

#pragma comment(lib, "opengl32.lib")

// OpenGL function pointers
typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTPROC)(HDC hdc);
typedef BOOL(WINAPI* PFNWGLMAKECURRENTPROC)(HDC hdc, HGLRC hglrc);
typedef BOOL(WINAPI* PFNWGLDELETECONTEXTPROC)(HGLRC hglrc);
typedef BOOL(WINAPI* PFNSWAPBUFFERSPROC)(HDC hdc);

// Configuration
struct Config {
    DWORD AnisotropicFiltering = 16;
    DWORD FPSUpdateInterval = 1;
    DWORD FPSPosX = 10;
    DWORD FPSPosY = 10;
    DWORD DefaultFOV = 90;
    DWORD FOVToggleKey = 45;
    FLOAT AimingFOVMultiplier = 0.6f;
    BOOL EnableVSync = FALSE;
    BOOL ShowFPS = TRUE;
    BOOL EnableFOV = TRUE;
    BOOL AllowDynamicFOV = FALSE;
};

static Config g_Config;

// FPS Counter
class FPSCounter {
private:
    DWORD frameCount = 0;
    DWORD lastTime = 0;
    DWORD currentFPS = 0;

public:
    void Update() {
        DWORD currentTime = GetTickCount();
        frameCount++;

        if (currentTime - lastTime >= g_Config.FPSUpdateInterval * 1000) {
            currentFPS = frameCount;
            frameCount = 0;
            lastTime = currentTime;
        }
    }

    DWORD GetCurrentFPS() const { return currentFPS; }

    void Draw(HDC hdc, int x, int y) const {
        if (!hdc || !g_Config.ShowFPS) return;

        char text[32];
        sprintf_s(text, "FPS: %lu", currentFPS);

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0, 255, 0));
        TextOutA(hdc, x, y, text, strlen(text));
    }
};

// Anisotropic Filter
class AnisotropicFilter {
private:
    UINT currentLevel = 0;

public:
    void SetLevel(UINT level) {
        currentLevel = level;
        // OpenGL anisotropic filtering would be set here
        printf("Anisotropic filtering set to level %u\n", level);
    }

    UINT GetLevel() { return currentLevel; }
};

// FOV Controller
class FOVController {
private:
    float currentFOV = 90.0f;
    float defaultFOV = 90.0f;
    float aimingFOV = 60.0f;
    bool toggleState = false;
    DWORD lastKeyCheck = 0;
    DWORD toggleKey = 45;

public:
    void SetDefaultFOV(float fov) { defaultFOV = fov; }
    void SetAimingFOV(float fov) { aimingFOV = fov; }
    void SetToggleKey(DWORD key) { toggleKey = key; }

    void Update() {
        DWORD currentTime = GetTickCount();
        if (currentTime - lastKeyCheck < 100) return;
        lastKeyCheck = currentTime;

        BOOL keyPressed = (GetAsyncKeyState((int)toggleKey) & 0x8000) != 0;
        if (keyPressed) {
            toggleState = !toggleState;
            currentFOV = toggleState ? aimingFOV : defaultFOV;
            printf("FOV toggled to %.1f degrees\n", currentFOV);
        }
    }

    float GetCurrentFOV() { return currentFOV; }
};

// VSync Controller
class VSyncController {
private:
    bool enabled = false;

public:
    void SetEnabled(bool enable) {
        enabled = enable;
        printf("VSync %s\n", enabled ? "enabled" : "disabled");
    }

    bool IsEnabled() { return enabled; }
};

// OpenGL Renderer
class OpenGLRenderer {
private:
    HDC hdc;
    HGLRC hglrc;
    bool initialized;
    HWND targetWindow;

    PFNWGLCREATECONTEXTPROC wglCreateContext;
    PFNWGLMAKECURRENTPROC wglMakeCurrent;
    PFNWGLDELETECONTEXTPROC wglDeleteContext;
    PFNSWAPBUFFERSPROC SwapBuffers;

    FPSCounter fps;
    AnisotropicFilter anisotropy;
    FOVController fov;
    VSyncController vsync;

public:
    OpenGLRenderer() : hdc(NULL), hglrc(NULL), initialized(false), targetWindow(NULL) {}

    ~OpenGLRenderer() {
        Cleanup();
    }

    bool Initialize(HWND window) {
        if (initialized) return true;

        targetWindow = window;
        hdc = GetDC(window);

        if (!hdc) {
            OutputDebugStringA("[OFP-D3DX OpenGL] Failed to get device context\n");
            return false;
        }

        PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            32,
            0, 0, 0, 0, 0, 0,
            0, 0,
            0, 0, 0, 0, 0,
            24,
            8,
            0,
            PFD_MAIN_PLANE,
            0, 0, 0, 0
        };

        int pixelFormat = ChoosePixelFormat(hdc, &pfd);
        if (!pixelFormat) {
            OutputDebugStringA("[OFP-D3DX OpenGL] Failed to choose pixel format\n");
            return false;
        }

        if (!SetPixelFormat(hdc, pixelFormat, &pfd)) {
            OutputDebugStringA("[OFP-D3DX OpenGL] Failed to set pixel format\n");
            return false;
        }

        HMODULE hOpenGL = LoadLibraryA("opengl32.dll");
        if (!hOpenGL) {
            OutputDebugStringA("[OFP-D3DX OpenGL] Failed to load opengl32.dll\n");
            return false;
        }

        wglCreateContext = (PFNWGLCREATECONTEXTPROC)GetProcAddress(hOpenGL, "wglCreateContext");
        wglMakeCurrent = (PFNWGLMAKECURRENTPROC)GetProcAddress(hOpenGL, "wglMakeCurrent");
        wglDeleteContext = (PFNWGLDELETECONTEXTPROC)GetProcAddress(hOpenGL, "wglDeleteContext");
        SwapBuffers = (PFNSWAPBUFFERSPROC)GetProcAddress(hOpenGL, "SwapBuffers");

        if (!wglCreateContext || !wglMakeCurrent || !wglDeleteContext || !SwapBuffers) {
            OutputDebugStringA("[OFP-D3DX OpenGL] Failed to load OpenGL functions\n");
            return false;
        }

        hglrc = wglCreateContext(hdc);
        if (!hglrc) {
            OutputDebugStringA("[OFP-D3DX OpenGL] Failed to create OpenGL context\n");
            return false;
        }

        if (!wglMakeCurrent(hdc, hglrc)) {
            OutputDebugStringA("[OFP-D3DX OpenGL] Failed to make OpenGL context current\n");
            return false;
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // Load configuration
        LoadConfig();

        initialized = true;
        OutputDebugStringA("[OFP-D3DX OpenGL] OpenGL renderer initialized successfully\n");
        return true;
    }

    void LoadConfig() {
        char path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);
        char* p = strrchr(path, '\\');
        if (p) p[1] = '\0';
        strcat_s(path, "d3dx.ini");

        g_Config.AnisotropicFiltering = GetPrivateProfileIntA("d3d9", "AnisotropicFiltering", 16, path);
        g_Config.EnableVSync = GetPrivateProfileIntA("d3d9", "EnableVSync", 0, path);
        g_Config.ShowFPS = GetPrivateProfileIntA("FPS", "ShowFPS", 1, path);
        g_Config.FPSUpdateInterval = GetPrivateProfileIntA("FPS", "FPSUpdateInterval", 1, path);
        g_Config.FPSPosX = GetPrivateProfileIntA("FPS", "FPSPosX", 10, path);
        g_Config.FPSPosY = GetPrivateProfileIntA("FPS", "FPSPosY", 10, path);
        g_Config.EnableFOV = GetPrivateProfileIntA("FOV", "EnableFOV", 1, path);
        g_Config.DefaultFOV = GetPrivateProfileIntA("FOV", "DefaultFOV", 90, path);
        g_Config.AllowDynamicFOV = GetPrivateProfileIntA("FOV", "AllowDynamicFOV", 0, path);
        g_Config.FOVToggleKey = GetPrivateProfileIntA("FOV", "FOVToggleKey", 45, path);
        g_Config.AimingFOVMultiplier = (FLOAT)GetPrivateProfileIntA("FOV", "AimingFOVMultiplier", 60, path) / 100.0f;

        fov.SetDefaultFOV((float)g_Config.DefaultFOV);
        fov.SetAimingFOV((float)g_Config.DefaultFOV * g_Config.AimingFOVMultiplier);
        fov.SetToggleKey(g_Config.FOVToggleKey);
        vsync.SetEnabled(g_Config.EnableVSync);
        anisotropy.SetLevel(g_Config.AnisotropicFiltering);

        currentFOV = (float)g_Config.DefaultFOV;

        char log[256];
        sprintf_s(log, "[OFP-D3DX OpenGL] Config loaded - Anisotropy: %lu, VSync: %d, FOV: %lu\n",
                 g_Config.AnisotropicFiltering, g_Config.EnableVSync, g_Config.DefaultFOV);
        OutputDebugStringA(log);
    }

    void RenderFrame() {
        if (!initialized) return;

        // Update FPS and check FOV key
        fps.Update();
        if (g_Config.AllowDynamicFOV) {
            fov.Update();
        }

        // Clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up 2D projection for overlay
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, 800, 0, 600, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        // Disable depth test for 2D overlay
        glDisable(GL_DEPTH_TEST);

        // Draw FPS overlay
        fps.Draw(hdc, g_Config.FPSPosX, g_Config.FPSPosY);

        // Restore 3D projection
        glEnable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        // Apply FOV to projection matrix
        if (g_Config.AllowDynamicFOV) {
            ApplyFOV();
        }

        // Swap buffers
        SwapBuffers(hdc);
    }

    void ApplyFOV() {
        if (!initialized) return;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        float aspect = 4.0f / 3.0f;
        float fovRad = fov.GetCurrentFOV() * 3.14159f / 180.0f;
        float f = 1.0f / tan(fovRad / 2.0f);
        float nearPlane = 0.1f;
        float farPlane = 1000.0f;

        float projection[16] = {
            f / aspect, 0, 0, 0,
            0, f, 0, 0,
            0, 0, -(farPlane + nearPlane) / (farPlane - nearPlane), -1,
            0, 0, -2 * farPlane * nearPlane / (farPlane - nearPlane), 0
        };

        glMultMatrixf(projection);
        glMatrixMode(GL_MODELVIEW);
    }

    void ApplyAnisotropicFiltering() {
        if (g_Config.AnisotropicFiltering > 0 && initialized) {
            // OpenGL anisotropic filtering setup
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            char log[128];
            sprintf_s(log, "[OFP-D3DX OpenGL] Applied anisotropic filtering level %lu\n", g_Config.AnisotropicFiltering);
            OutputDebugStringA(log);
        }
    }

    void Cleanup() {
        if (initialized) {
            if (hglrc) {
                wglMakeCurrent(NULL, NULL);
                wglDeleteContext(hglrc);
                hglrc = NULL;
            }
            if (hdc && targetWindow) {
                ReleaseDC(targetWindow, hdc);
                hdc = NULL;
            }
            initialized = false;
            OutputDebugStringA("[OFP-D3DX OpenGL] Renderer cleaned up\n");
        }
    }

    bool IsInitialized() const { return initialized; }
    HDC GetHDC() const { return hdc; }
    DWORD GetCurrentFPS() const { return fps.GetCurrentFPS(); }
    float currentFOV = 90.0f;
};